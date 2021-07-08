#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include "multiPlayScene.h"
#include "tetris.h"
#include "game.h"
#include "font.h"

static void setRectPos(SDL_Rect* rect, int x, int y, int w, int h);
static void setPeerRectPos(SDL_Rect* rect, int x, int y, int w, int h);

static const float gravity[5][2] = {
	{ 1.f / 30, 10.f / 30 },
	{ 1.f / 20, 10.f / 20 },
	{ 1.f / 10, 10.f / 10 },
	{ 1.f / 1, 10.f / 1 },
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

static int levelCnt;

static void init();

static UDPsocket socket;
static IPaddress ip;
static UDPpacket* packet;

static int peerMatrix[40][10];

static int garbageC;

static int win;
static int quit;

static void sendCurrent();
static void sendGarbages(int c);
static void sendGameover();

void MultiPlayScene_SetPeer(IPaddress address)
{
	ip = address;
}

void MultiPlayScene_Init(SDL_Renderer* renderer)
{
	SDLNet_Init();
	socket = SDLNet_UDP_Open(37646);
	packet = SDLNet_AllocPacket(512);
	tileTexture = IMG_LoadTexture(renderer, "res/tile.png");
	SDL_SetTextureBlendMode(tileTexture, SDL_BLENDMODE_BLEND);
	tetrominosTexture = IMG_LoadTexture(renderer, "res/tetrominos.png");
	SDL_SetTextureBlendMode(tetrominosTexture, SDL_BLENDMODE_BLEND);
	holdUnavTexture = IMG_LoadTexture(renderer, "res/holdunav.png");
	SDL_SetTextureBlendMode(holdUnavTexture, SDL_BLENDMODE_BLEND);
	holdDescTexture = Font_GetTexture(renderer, "Hold", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	previewDescTexture = Font_GetTexture(renderer, "Next", 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	init();
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
	combo = 0;
	gameover = 0;
	gameoverCounter = 0;
	garbageC = 0;
	win = 0;
	quit = 0;
	Tetris_Init();
}

void MultiPlayScene_Event(SDL_Event* event)
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
			Tetris_Harddrop();
			lockCounter = 1.f;
			break;
		case SDLK_ESCAPE:
			Game_ChangeScene(MULTI_CONNECT);
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
			Tetris_Harddrop();
			lockCounter = 1.f;
			break;
		case SDL_CONTROLLER_BUTTON_START:
			Game_ChangeScene(MULTI_CONNECT);
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

void MultiPlayScene_Update(double delta)
{
	if (SDLNet_UDP_Recv(socket, packet) > 0)
	{
		if (packet->address.host == ip.host && packet->address.port == ip.port && packet->data[0] == 'T' && packet->data[1] == 'o' && packet->data[2] == 'p')
		{
			int i, j;
			switch (packet->data[3])
			{
			case 'u':
				for (i = 0; i < 40; i++)
				{
					for (j = 0; j < 10; j++)
					{
						peerMatrix[i][j] = packet->data[i * 10 + j + 4];
					}
				}
				break;
			case 'b':
				garbageC += packet->data[4];
				break;
			case 'o':
				gameover = 1;
				win = 1;
				break;
			case 'q':
				gameover = 1;
				win = 1;
				quit = 1;
				break;
			}
		}
	}
	if (gameover)
	{
		gameoverCounter += (float)delta * 60 * gameoverDelay;
		if (gameoverCounter > 1.f)
		{
			if (quit)
			{
				Game_ChangeScene(MULTI_CONNECT);
			}
			else
			{
				init();
			}
		}
	}
	else
	{
		if (nextWait)
		{
			nextCounter += (float)delta * 60 * nextDelay;
			if (garbageC > 0 && nextCounter > .5f)
			{
				if (Tetris_Garbage())
				{
					gameover = 1;
				}
				nextCounter = 0;
			}
			if (nextCounter > 1.f)
			{
				nextCounter = 0;
				nextWait = 0;
				Tetris_PullEmptyLine();
				sendCurrent();
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
			fallCounter += (float)delta * 60 * gravity[SDL_min(levelCnt/10, 4)][softDrop];
			if (!Tetris_Fallable())
				lockCounter += (float)delta * 60 * lockDelay;
			while (fallCounter > 1.f)
			{
				fallCounter -= 1.f;
				if (Tetris_Fall())
				{
					lockCounter = 0;
				}
				sendCurrent();
			}
			if (lockCounter > 1.f)
			{
				lockCounter = 0;
				Tetris_Lock();
				if (Tetris_ClearFullLine())
				{
					enum Technique technique = Tetris_GetTechnique();
					levelCnt += lineCleared[technique];
					switch (technique)
					{
					case Tech_Double:
						sendGarbages(1);
						break;
					case Tech_Triple:
						sendGarbages(2);
						break;
					case Tech_Tetris:
						sendGarbages(4);
						break;
					case Tech_B_Tetris:
						sendGarbages(5);
						break;
					case Tech_TSpin_Single:
						sendGarbages(2);
						break;
					case Tech_TSpin_Double:
						sendGarbages(4);
						break;
					case Tech_TSpin_Triple:
						sendGarbages(6);
						break;
					case Tech_B_TSpin_Single:
						sendGarbages(3);
						break;
					case Tech_B_TSpin_Double:
						sendGarbages(5);
						break;
					case Tech_B_TSpin_Triple:
						sendGarbages(7);
						break;
					case Tech_TSpinM_Single:
						sendGarbages(1);
						break;
					case Tech_TSpinM_Double:
						sendGarbages(3);
						break;
					case Tech_B_TSpinM_Single:
						sendGarbages(2);
						break;
					case Tech_B_TSpinM_Double:
						sendGarbages(4);
						break;
					}
					nextWait = 1;
				}
				else
				{
					combo = 0;
					if (!Tetris_Next())
					{
						gameover = 1;
					}
					fallCounter = 0;
				}
				sendCurrent();
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
					sendCurrent();
				}
			}
			if (leftFlag)
			{
				leftCounter += (float)delta * 60 * leftInterval;
				while (leftCounter > 1.f)
				{
					leftCounter -= 1.f;
					Tetris_Shift(-1);
					sendCurrent();
				}
			}
		}
	}
}

void MultiPlayScene_Render(SDL_Renderer* renderer)
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
			if (gameover && !win)
			{
				int matrix = Tetris_Matrix(i, j);
				if (matrix == -1)
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0xFF, 0xFF);
				else if (matrix == 0)
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
			setPeerRectPos(&rect, i, j, w, h);
			if (gameover && win)
			{
				int matrix;
				if (i < 0 || i >= 10 || j < 0 || j >= 40)
					matrix = -1;
				else
					matrix = peerMatrix[j][i];
				if (matrix == -1)
					SDL_SetTextureColorMod(tileTexture, 0xFF, 0xFF, 0xFF);
				else if (matrix == 0)
					SDL_SetTextureColorMod(tileTexture, 0x00, 0x00, 0x00);
				else
					SDL_SetTextureColorMod(tileTexture, 0x80, 0x80, 0x80);
			}
			else
			{
				int matrix;
				if (i < 0 || i >= 10 || j < 0 || j >= 40)
					matrix = -1;
				else
					matrix = peerMatrix[j][i];
				switch (matrix)
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
	SDL_RenderDrawLine(renderer, w / 4 - h / 22 * 5, h / 2 - h / 22 * 10, w / 4 + h / 22 * 5, h / 2 - h / 22 * 10);
	SDL_RenderDrawLine(renderer, w * 3 / 4 - h / 22 * 5, h / 2 - h / 22 * 10, w * 3 / 4 + h / 22 * 5, h / 2 - h / 22 * 10);

	i = Tetris_WhatHold();
	if (i >= 0)
	{
		rect.w = tetrominosSplit[i].w / 64 * h / 22;
		rect.h = tetrominosSplit[i].h / 64 * h / 22;
		rect.x = w / 2 - rect.w;
		rect.y = h * 7 / 22 - rect.h / 2;
		SDL_RenderCopy(renderer, tetrominosTexture, &tetrominosSplit[i], &rect);
		if (Tetris_CanHold())
		{
			SDL_RenderCopy(renderer, holdUnavTexture, NULL, &rect);
		}
	}
	rect.w = h * 3 / 22;
	rect.h = h * 3 / 22;
	rect.x = w / 2 - rect.w;
	rect.y = h*4 / 22 - rect.h / 2;
	SDL_RenderCopy(renderer, holdDescTexture, NULL, &rect);

	for (i = 0; i < 5; i++)
	{
		j = Tetris_Preview(i);
		rect.w = tetrominosSplit[j].w / 64 * h / 22;
		rect.h = tetrominosSplit[j].h / 64 * h / 22;
		rect.x = w / 2;
		rect.y = h * (i * 3 + 7) / 22 - rect.h / 2;
		SDL_RenderCopy(renderer, tetrominosTexture, &tetrominosSplit[j], &rect);
	}
	rect.w = h * 3 / 22;
	rect.h = h * 3 / 22;
	rect.x = w / 2;
	rect.y = h * 4 / 22 - rect.h / 2;;
	SDL_RenderCopy(renderer, previewDescTexture, NULL, &rect);
}

void MultiPlayScene_Quit()
{
	packet->address = ip;
	SDL_strlcpy(packet->data, "Topq", packet->maxlen);
	packet->len = 5;
	SDLNet_UDP_Send(socket, -1, packet);
	SDLNet_UDP_Close(socket);
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(tetrominosTexture);
	SDL_DestroyTexture(holdUnavTexture);
	SDL_DestroyTexture(holdDescTexture);
	SDL_DestroyTexture(previewDescTexture);
}

static void setRectPos(SDL_Rect* rect, int x, int y, int w, int h)
{
	rect->w = rect->h = h / 22;
	rect->x = w / 4 + rect->w * (x - 5);
	rect->y = h / 2 - rect->h * (y - 9);
}

static void setPeerRectPos(SDL_Rect* rect, int x, int y, int w, int h)
{
	rect->w = rect->h = h / 22;
	rect->x = w * 3 / 4 + rect->w * (x - 5);
	rect->y = h / 2 - rect->h * (y - 9);
}

static void sendCurrent()
{
	packet->address = ip;
	SDL_strlcpy(packet->data, "Topu", packet->maxlen);
	int i, j;
	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 10; j++)
		{
			packet->data[i + 10 + j + 4] = Tetris_Matrix(i, j);
		}
	}
	packet->len = 404;
	SDLNet_UDP_Send(socket, -1, packet);
}

static void sendGarbages(int c)
{
	packet->address = ip;
	SDL_strlcpy(packet->data, "Topb", packet->maxlen);
	packet->data[4] = c;
	packet->len = 6;
	SDLNet_UDP_Send(socket, -1, packet);
}

static void sendGameover()
{
	packet->address = ip;
	SDL_strlcpy(packet->data, "Topo", packet->maxlen);
	packet->len = 5;
	SDLNet_UDP_Send(socket, -1, packet);
}