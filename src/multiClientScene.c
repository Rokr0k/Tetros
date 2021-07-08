#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include "multiClientScene.h"
#include "font.h"
#include "game.h"
#include "multiPlayScene.h"

static UDPsocket socket;
static UDPpacket* packet;

static SDL_Texture* textTexture;

static SDL_Texture* hostsTexture[10];
static IPaddress hosts[10];
static SDL_Rect hostsRect[10];
static int hostsHover;
static int hostsRendered;
static int hostsC;

static SDL_Texture* cursorTexture;
static SDL_Rect cursorRect;
static int cursorX, cursorY;

static void load();
static void part();

void MultiClientScene_Init(SDL_Renderer* renderer)
{
	cursorTexture = IMG_LoadTexture(renderer, "res/cursor.png");
	cursorX = 640;
	cursorY = 360;
	SDLNet_Init();
	socket = SDLNet_UDP_Open(0);
	packet = SDLNet_AllocPacket(5);
	load();
	textTexture = Font_GetTexture(renderer, "Please Wait", 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
}

void MultiClientScene_Event(SDL_Event* event)
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
			if (hostsHover >= 0)
			{
				part();
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_r:
			load();
			break;
		case SDLK_ESCAPE:
			Game_ChangeScene(MULTI_CONNECT);
			break;
		}
		break;
	case SDL_CONTROLLERAXISMOTION:
		if (event->caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			cursorX += event->caxis.value / 8191;
			if (cursorX < 0)
				cursorX = 0;
			else if (cursorX >= 1280)
				cursorX = 1279;
		}
		else if (event->caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			cursorY += event->caxis.value / 8191;
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
			if (hostsHover >= 0)
			{
				part();
			}
			break;
		case SDL_CONTROLLER_BUTTON_X:
			load();
			break;
		case SDL_CONTROLLER_BUTTON_B:
			Game_ChangeScene(MULTI_CONNECT);
			break;
		}
	}
}

void MultiClientScene_Update(double delta)
{
	hostsHover = -1;
	int i;
	for (i = 0; i < hostsRendered; i++)
	{
		if (cursorRect.x >= hostsRect[i].x && cursorRect.x < hostsRect[i].x + hostsRect[i].w && cursorRect.y >= hostsRect[i].y && cursorRect.y < hostsRect[i].y + hostsRect[i].h)
		{
			hostsHover = i;
			break;
		}
	}
	if (SDLNet_UDP_Recv(socket, packet) > 0)
	{
		if (packet->data[0] == 'T' && packet->data[1] == 'o' && packet->data[2] == 's')
		{
			switch (packet->data[3])
			{
			case 'g':
				hosts[hostsC++] = packet->address;
				break;
			case 'e':
				MultiPlayScene_SetPeer(packet->address);
				Game_ChangeScene(MULTI_PLAY);
				break;
			}
		}
	}
}

void MultiClientScene_Render(SDL_Renderer* renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	SDL_Rect rect;
	rect.w = w / 2;
	rect.h = h / 2;
	rect.x = w / 2 - rect.w / 2;
	rect.y = h / 2 - rect.h / 2;
	SDL_RenderCopy(renderer, textTexture, NULL, &rect);

	for (; hostsRendered < hostsC; hostsRendered++)
	{
		hostsTexture[hostsRendered] = Font_GetTexture(renderer, SDLNet_ResolveIP(&hosts[hostsRendered]), 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	}

	int i;
	for (i = 0; i < hostsC; i++)
	{
		hostsRect[i].w = w / 3;
		hostsRect[i].h = h / 6;
		hostsRect[i].x = w * (i / 5 * 2 + 1) / 4 - hostsRect[i].w / 2;
		hostsRect[i].y = h * (i % 5 + 1) / 6 - hostsRect[i].h / 2;
		if (i == hostsHover)
		{
			SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		}
		SDL_RenderFillRect(renderer, &hostsRect[i]);
		SDL_RenderCopy(renderer, hostsTexture[i], NULL, &hostsRect[i]);
	}

	cursorRect.w = 40 * w / 1280;
	cursorRect.h = 60 * h / 720;
	cursorRect.x = cursorX * w / 1280;
	cursorRect.y = cursorY * h / 720;
	SDL_RenderCopy(renderer, cursorTexture, NULL, &cursorRect);
}

void MultiClientScene_Quit()
{
	SDLNet_FreePacket(packet);
	SDLNet_UDP_Close(socket);
	SDLNet_Quit();
	int i;
	for (i = 0; i < hostsRendered; i++)
	{
		SDL_DestroyTexture(hostsTexture[i]);
	}
	hostsRendered = 0;
	SDL_DestroyTexture(cursorTexture);
	SDL_DestroyTexture(textTexture);
}

static void load()
{
	int i;
	for (i = 0; i < hostsRendered; i++)
	{
		SDL_DestroyTexture(hostsTexture[i]);
	}
	hostsRendered = 0;
	hostsC = 0;
	SDLNet_ResolveHost(&packet->address, "255.255.255.255", 37646);
	SDL_strlcpy(packet->data, "Tocg", packet->maxlen);
	packet->len = (int)SDL_strlen(packet->data) + 1;
	SDLNet_UDP_Send(socket, -1, packet);
}

static void part()
{
	packet->address = hosts[hostsHover];
	SDL_strlcpy(packet->data, "Toce", packet->maxlen);
	packet->len = (int)SDL_strlen(packet->data) + 1;
	SDLNet_UDP_Send(socket, -1, packet);
}