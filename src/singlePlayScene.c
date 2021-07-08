#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "singlePlayScene.h"
#include "font.h"
#include "tetris.h"
#include "game.h"

static void setRectPos(SDL_Rect* rect, int x, int y, int w, int h);

static const float gravity[30][2] = {
	{ 1.f / 48, 10.f / 48 },
	{ 1.f / 43, 10.f / 43 },
	{ 1.f / 38, 10.f / 38 },
	{ 1.f / 33, 10.f / 33 },
	{ 1.f / 28, 10.f / 28 },
	{ 1.f / 23, 10.f / 23 },
	{ 1.f / 18, 10.f / 18 },
	{ 1.f / 13, 10.f / 13 },
	{ 1.f / 8, 10.f / 8 },
	{ 1.f / 6, 10.f / 6 },
	{ 1.f / 5, 10.f / 5 },
	{ 1.f / 5, 10.f / 5 },
	{ 1.f / 5, 10.f / 5 },
	{ 1.f / 5, 10.f / 5 },
	{ 1.f / 4, 10.f / 4 },
	{ 1.f / 4, 10.f / 4 },
	{ 1.f / 4, 10.f / 4 },
	{ 1.f / 4, 10.f / 4 },
	{ 1.f / 3, 10.f / 3 },
	{ 1.f / 3, 10.f / 3 },
	{ 1.f / 3, 10.f / 3 },
	{ 1.f / 3, 10.f / 3 },
	{ 1.f / 2, 10.f / 2 },
	{ 1.f / 2, 10.f / 2 },
	{ 1.f / 2, 10.f / 2 },
	{ 1.f / 2, 10.f / 2 },
	{ 1.f, 10.f },
	{ 1.f, 10.f },
	{ 1.f, 10.f },
	{ 20.f, 20.f },
};
static const int lineCleared[20] = { 0, 1, 2, 3, 4, 4, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 0, 1, 2 };

static int softDrop;
static float fallCounter;

static const float lockDelay = 1.f / 30;
static float lockCounter;

static int rightFlag;
static const float rightInterval = 1.f / 3;
static float rightCounter;
static int leftFlag;
static const float leftInterval = 1.f / 3;
static float leftCounter;

static int nextWait;
static const float nextDelay = 1.f / 30;
static float nextCounter;

static int gameover;
static const float gameoverDelay = 1.f / 90;
static float gameoverCounter;

static SDL_Texture* tileTexture;
static SDL_Texture* tetrominosTexture;
static const SDL_Rect tetrominosSplit[7] = {
	{0, 64, 256, 64},
	{256, 0, 192, 128},
	{448, 0, 192, 128},
	{640, 0, 128, 128},
	{768, 0, 192, 128},
	{960, 0, 192, 128},
	{1152, 0, 192, 128},
};

static SDL_Texture* holdUnavTexture;

static int combo;

static SDL_Texture* holdDescTexture;
static SDL_Texture* previewDescTexture;

static int score;
static SDL_Texture* scoreTexture;
static int scoreTextureW;
static int scoreTextureH;
static int scoreChanged;

static int levelCnt;
static SDL_Texture* levelTexture;
static int levelTextureW;
static int levelTextureH;
static int levelChanged;

static void init();

void SinglePlayScene_Init(SDL_Renderer* renderer)
{
	init();
	tileTexture = IMG_LoadTexture(renderer, "res/tile.png");
	SDL_SetTextureBlendMode(tileTexture, SDL_BLENDMODE_BLEND);
	tetrominosTexture = IMG_LoadTexture(renderer, "res/tetrominos.png");
	SDL_SetTextureBlendMode(tetrominosTexture, SDL_BLENDMODE_BLEND);
	holdUnavTexture = IMG_LoadTexture(renderer, "res/holdunav.png");
	SDL_SetTextureBlendMode(holdUnavTexture, SDL_BLENDMODE_BLEND);
	scoreTexture = Font_GetTexture(renderer, "Score: 0", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreTextureW, &scoreTextureH);
	levelTexture = Font_GetTexture(renderer, "Level: 1", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_QueryTexture(levelTexture, NULL, NULL, &levelTextureW, &levelTextureH);
	holdDescTexture = Font_GetTexture(renderer, "Hold", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	previewDescTexture = Font_GetTexture(renderer, "Next", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
}

static void init()
{
	softDrop = 0;
	fallCounter = 0;
	lockCounter = 0;
	rightFlag = 0;
	rightCounter = 0;
	leftFlag = 0;
	leftCounter = 0;
	nextWait = 0;
	nextCounter = 0;
	combo = -1;
	score = 0;
	scoreChanged = 1;
	levelCnt = 0;
	levelChanged = 1;
	gameover = 0;
	gameoverCounter = 0;
	Tetris_Init();
}

void SinglePlayScene_Event(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.repeat)
		{
			break;
		}
		switch (event->key.keysym.sym)
		{
		case SDLK_DOWN:
			softDrop = 1;
			break;
		case SDLK_RIGHT:
			Tetris_Shift(1);
			rightFlag = 1;
			rightCounter = -2.f;
			lockCounter = 0;
			break;
		case SDLK_LEFT:
			Tetris_Shift(-1);
			leftFlag = 1;
			leftCounter = -2.f;
			lockCounter = 0;
			break;
		case SDLK_UP:
		case SDLK_x:
			Tetris_Rotate(1);
			lockCounter = 0;
			break;
		case SDLK_LCTRL:
		case SDLK_z:
			Tetris_Rotate(-1);
			lockCounter = 0;
			break;
		case SDLK_LSHIFT:
			Tetris_Hold();
			break;
		case SDLK_SPACE:
			score += Tetris_Harddrop() * 2;
			lockCounter = 1.f;
			scoreChanged = 1;
			break;
		case SDLK_ESCAPE:
			Game_ChangeScene(TITLE);
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym)
		{
		case SDLK_DOWN:
			softDrop = 0;
			break;
		case SDLK_RIGHT:
			rightFlag = 0;
			break;
		case SDLK_LEFT:
			leftFlag = 0;
			break;
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		switch (event->cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			softDrop = 1;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			Tetris_Shift(1);
			rightFlag = 1;
			rightCounter = -2.f;
			lockCounter = 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			Tetris_Shift(-1);
			leftFlag = 1;
			leftCounter = -2.f;
			lockCounter = 0;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			Tetris_Rotate(1);
			lockCounter = 0;
			break;
		case SDL_CONTROLLER_BUTTON_A:
			Tetris_Rotate(-1);
			lockCounter = 0;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			Tetris_Hold();
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			score += Tetris_Harddrop() * 2;
			lockCounter = 1.f;
			scoreChanged = 1;
			break;
		case SDL_CONTROLLER_BUTTON_START:
			Game_ChangeScene(TITLE);
			break;
		}
		break;
	case SDL_CONTROLLERAXISMOTION:
		switch (event->caxis.axis)
		{
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			if (event->caxis.value > 6553)
			{
				Tetris_Hold();
			}
			break;
		}
		break;
	case SDL_CONTROLLERBUTTONUP:
		switch (event->cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			softDrop = 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			rightFlag = 0;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			leftFlag = 0;
			break;
		}
		break;
	}
}

void SinglePlayScene_Update(double delta)
{
	if (gameover)
	{
		gameoverCounter += (float)delta * 60 * gameoverDelay;
		if (gameoverCounter > 1.f)
		{
			init();
		}
	}
	else
	{
		if (nextWait)
		{
			nextCounter += (float)delta * 60 * nextDelay;
			if (nextCounter > 1.f)
			{
				nextCounter = 0;
				nextWait = 0;
				Tetris_PullEmptyLine();
				if (!Tetris_Next())
				{
					gameover = 1;
				}
				fallCounter = 0;
				lockCounter = 0;
			}
		}
		else
		{
			fallCounter += (float)delta * 60 * gravity[SDL_min(levelCnt / 10, 29)][softDrop];
			if (!Tetris_Fallable())
				lockCounter += (float)delta * 60 * lockDelay;
			while (fallCounter > 1.f)
			{
				fallCounter -= 1.f;
				if (Tetris_Fall())
				{
					if (softDrop)
					{
						score++;
						scoreChanged = 1;
					}
					lockCounter = 0;
				}
			}
			if (lockCounter > 1.f)
			{
				lockCounter = 0;
				Tetris_Lock();
				if (Tetris_ClearFullLine())
				{
					combo++;
					enum Technique technique = Tetris_GetTechnique();
					int scoreD = 0;
					switch (technique & 0x07)
					{
					case Tech_Single:
						if (technique & Tech_TSpin)
						{
							scoreD = 800;
						}
						else if (technique & Tech_TSpinMini)
						{
							scoreD = 200;
						}
						else
						{
							scoreD = 100;
						}
						break;
					case Tech_Double:
						if (technique & Tech_TSpin)
						{
							scoreD = 1200;
						}
						else if (technique & Tech_TSpinMini)
						{
							scoreD = 400;
						}
						else
						{
							scoreD = 300;
						}
						break;
					case Tech_Triple:
						if (technique & Tech_TSpin)
						{
							scoreD = 1600;
						}
						else
						{
							scoreD = 500;
						}
						break;
					case Tech_Tetris:
						scoreD = 800;
						break;
					}
					if (technique & Tech_BackToBack)
					{
						scoreD = scoreD * 3 / 2;
					}
					scoreD += 50 * combo;
					score += scoreD * (levelCnt / 10 + 1);
					levelCnt += technique & 0x07;
					nextWait = 1;
					scoreChanged = 1;
					levelChanged = 1;
				}
				else
				{
					combo = -1;
					enum Technique technique = Tetris_GetTechnique();
					int scoreD = 0;
					if (technique & Tech_TSpinMini)
						scoreD = 100;
					else if (technique & Tech_TSpin)
						scoreD = 400;
					score += scoreD * (levelCnt / 10 + 1);
					scoreChanged = 1;
					if (!Tetris_Next())
					{
						gameover = 1;
					}
					fallCounter = 0;
				}
			}
		}
		if (rightFlag ^ leftFlag)
		{
			if (rightFlag)
			{
				rightCounter += (float)delta * 60 * rightInterval;
				while (rightCounter > 1.f)
				{
					rightCounter -= 1.f;
					Tetris_Shift(1);
				}
			}
			if (leftFlag)
			{
				leftCounter += (float)delta * 60 * leftInterval;
				while (leftCounter > 1.f)
				{
					leftCounter -= 1.f;
					Tetris_Shift(-1);
				}
			}
		}
	}
}

void SinglePlayScene_Render(SDL_Renderer* renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	int i, j;
	SDL_Rect rect;
	for (i = -1; i <= 10; i++)
	{
		for (j = -1; j <= 40; j++)
		{
			setRectPos(&rect, i, j, w, h);
			if (gameover)
			{
				int matrix = Tetris_Matrix(i, j);
				if(matrix == -1)
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0xFF, 0xFF);
				else if(matrix == 0)
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x00, 0x00);
				else
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x80, 0x80);
			}
			else
			{
				switch (Tetris_Matrix(i, j))
				{
				case -1:
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0xFF, 0xFF);
					break;
				case 0:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x00, 0x00);
					break;
				case 1:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0xFF, 0xFF);
					break;
				case 2:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x00, 0xFF);
					break;
				case 3:
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0x80, 0x00);
					break;
				case 4:
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0xFF, 0x00);
					break;
				case 5:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0xFF, 0x00);
					break;
				case 6:
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0x00, 0xFF);
					break;
				case 7:
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0x00, 0x00);
					break;
				case 11:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x80, 0x80);
					break;
				case 12:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x00, 0x80);
					break;
				case 13:
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x40, 0x00);
					break;
				case 14:
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x80, 0x00);
					break;
				case 15:
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x80, 0x00);
					break;
				case 16:
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x00, 0x80);
					break;
				case 17:
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x00, 0x00);
					break;
				}
			}
			SDL_RenderCopy(renderer, tileTexture, NULL, &rect);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, w / 2 - h / 22 * 5, h / 2 - h / 22 * 10, w / 2 + h / 22 * 5, h / 2 - h / 22 * 10);

	i = Tetris_WhatHold();
	if (i >= 0)
	{
		rect.w = tetrominosSplit[i].w / 64 * h / 22;
		rect.h = tetrominosSplit[i].h / 64 * h / 22;
		rect.x = w / 4 - rect.w / 2;
		rect.y = h * 2 / 11 - rect.h / 2;
		SDL_RenderCopy(renderer, tetrominosTexture, &tetrominosSplit[i], &rect);
		if (Tetris_CanHold())
		{
			SDL_RenderCopy(renderer, holdUnavTexture, NULL, &rect);
		}
	}
	rect.w = h * 3 / 22;
	rect.h = h * 3 / 22;
	rect.x = w / 4 - rect.w / 2;
	rect.y = h / 22 - rect.h / 2;
	SDL_RenderCopy(renderer, holdDescTexture, NULL, &rect);

	for (i = 0; i < 5; i++)
	{
		j = Tetris_Preview(i);
		rect.w = tetrominosSplit[j].w / 64 * h / 22;
		rect.h = tetrominosSplit[j].h / 64 * h / 22;
		rect.x = w * 3 / 4 - rect.w / 2;
		rect.y = h * (i * 3 + 4) / 22 - rect.h / 2;
		SDL_RenderCopy(renderer, tetrominosTexture, &tetrominosSplit[j], &rect);
	}
	rect.w = h * 3 / 22;
	rect.h = h * 3 / 22;
	rect.x = w * 3 / 4 - rect.w / 2;
	rect.y = h / 22 - rect.h / 2;
	SDL_RenderCopy(renderer, previewDescTexture, NULL, &rect);

	if (levelChanged)
	{
		char levelText[19] = "Level: ";
		SDL_itoa(levelCnt / 10 + 1, levelText + 7, 10);
		SDL_DestroyTexture(levelTexture);
		levelTexture = Font_GetTexture(renderer, levelText, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		SDL_QueryTexture(levelTexture, NULL, NULL, &levelTextureW, &levelTextureH);
		levelChanged = 0;
	}
	rect.h = h / 10;
	rect.w = levelTextureW * rect.h / levelTextureH / 3;
	rect.x = h/22;
	rect.y = h * 4 / 5 - rect.h / 2;
	SDL_RenderCopy(renderer, levelTexture, NULL, &rect);

	if (scoreChanged)
	{
		char scoreText[19] = "Score: ";
		SDL_itoa(score, scoreText + 7, 10);
		SDL_DestroyTexture(scoreTexture);
		scoreTexture = Font_GetTexture(renderer, scoreText, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreTextureW, &scoreTextureH);
		scoreChanged = 0;
	}
	rect.h = h / 10;
	rect.w = scoreTextureW * rect.h / scoreTextureH / 3;
	rect.x = h/22;
	rect.y = h * 9 / 10 - rect.h / 2;
	SDL_RenderCopy(renderer, scoreTexture, NULL, &rect);
}

void SinglePlayScene_Quit()
{
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(tetrominosTexture);
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(levelTexture);
	SDL_DestroyTexture(holdUnavTexture);
	SDL_DestroyTexture(holdDescTexture);
	SDL_DestroyTexture(previewDescTexture);
}

static void setRectPos(SDL_Rect* rect, int x, int y, int w, int h)
{
	rect->w = rect->h = h / 22;
	rect->x = w / 2 + rect->w * (x - 5);
	rect->y = h / 2 - rect->h * (y - 9);
}