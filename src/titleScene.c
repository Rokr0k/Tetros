#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "titleScene.h"
#include "font.h"
#include "game.h"

static SDL_Texture* titleTexture;
static SDL_Rect titleRect;

static SDL_Texture* singleTexture;
static SDL_Rect singleRect;
static int singleHover;

static SDL_Texture* multiTexture;
static SDL_Rect multiRect;
static int multiHover;

static SDL_Texture* cursorTexture;
static SDL_Rect cursorRect;
static int cursorX, cursorY;

static void quitGame();

void TitleScene_Init(SDL_Renderer* renderer)
{
	titleTexture = IMG_LoadTexture(renderer, "res/title.png");
	singleTexture = Font_GetTexture(renderer, "Single", 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	multiTexture = Font_GetTexture(renderer, "Multi", 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	cursorTexture = IMG_LoadTexture(renderer, "res/cursor.png");
	cursorX = 640;
	cursorY = 360;
}

void TitleScene_Event(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
		cursorX += event->motion.xrel;
		cursorY += event->motion.yrel;
		if (cursorX < 0)
			cursorX = 0;
		else if (cursorX >= 1280)
			cursorX = 1279;
		if (cursorY < 0)
			cursorY = 0;
		else if (cursorY >= 720)
			cursorY = 719;
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			if (singleHover)
			{
				Game_ChangeScene(SINGLE_PLAY);
			}
			else if (multiHover)
			{
				Game_ChangeScene(MULTI_CONNECT);
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			quitGame();
			break;
		}
		break;
	case SDL_CONTROLLERAXISMOTION:
		if (event->caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			cursorX += event->caxis.value / 8192;
			if (cursorX < 0)
				cursorX = 0;
			else if (cursorX >= 1280)
				cursorX = 1279;
		}
		else if (event->caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			cursorY += event->caxis.value / 8192;
			if (cursorY < 0)
				cursorY = 0;
			else if (cursorY >= 720)
				cursorY = 719;
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		switch (event->cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
			if (singleHover)
			{
				Game_ChangeScene(SINGLE_PLAY);
			}
			else if (multiHover)
			{
				Game_ChangeScene(MULTI_CONNECT);
			}
			break;
		case SDL_CONTROLLER_BUTTON_B:
			quitGame();
			break;
		}
	}
}

void TitleScene_Update(double delta)
{
	singleHover = cursorRect.x >= singleRect.x && cursorRect.x < singleRect.x + singleRect.w && cursorRect.y >= singleRect.y && cursorRect.y < singleRect.y + singleRect.h;
	multiHover = cursorRect.x >= multiRect.x && cursorRect.x < multiRect.x + multiRect.w && cursorRect.y >= multiRect.y && cursorRect.y < multiRect.y + multiRect.h;
}

void TitleScene_Render(SDL_Renderer* renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	titleRect.w = w * 700 / 1280;
	titleRect.h = h * 300 / 720;
	titleRect.x = w / 2 - titleRect.w / 2;
	titleRect.y = h / 3 - titleRect.h / 2;
	SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

	singleRect.w = w * 300 / 1280;
	singleRect.h = h * 100 / 720;
	singleRect.x = w / 3 - singleRect.w / 2;
	singleRect.y = h * 2 / 3 - singleRect.h / 2;
	if (singleHover)
	{
		SDL_SetRenderDrawColor(renderer, 0x9F, 0x9F, 0x9F, SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	}
	SDL_RenderFillRect(renderer, &singleRect);
	SDL_RenderCopy(renderer, singleTexture, NULL, &singleRect);

	multiRect.w = w * 300 / 1280;
	multiRect.h = h * 100 / 720;
	multiRect.x = w * 2 / 3 - multiRect.w / 2;
	multiRect.y = h * 2 / 3 - multiRect.h / 2;
	if (multiHover)
	{
		SDL_SetRenderDrawColor(renderer, 0x9F, 0x9F, 0x9F, SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	}
	SDL_RenderFillRect(renderer, &multiRect);
	SDL_RenderCopy(renderer, multiTexture, NULL, &multiRect);

	cursorRect.w = 40 * w / 1280;
	cursorRect.h = 60 * h / 720;
	cursorRect.x = cursorX * w / 1280;
	cursorRect.y = cursorY * h / 720;
	SDL_RenderCopy(renderer, cursorTexture, NULL, &cursorRect);
}

void TitleScene_Quit()
{
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(singleTexture);
	SDL_DestroyTexture(multiTexture);
	SDL_DestroyTexture(cursorTexture);
}

static void quitGame()
{
	SDL_Event quit;
	quit.type = SDL_QUIT;
	SDL_PushEvent(&quit);
}