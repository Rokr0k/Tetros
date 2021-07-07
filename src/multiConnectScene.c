#include <SDL.h>
#include <SDL_image.h>
#include "multiConnectScene.h"
#include "font.h"
#include "game.h"

static SDL_Texture* serverTexture;
static SDL_Rect serverRect;
static int serverHover;

static SDL_Texture* clientTexture;
static SDL_Rect clientRect;
static int clientHover;

static SDL_Texture* cursorTexture;
static SDL_Rect cursorRect;
static int cursorX, cursorY;

void MultiConnectScene_Init(SDL_Renderer* renderer)
{
	serverTexture = Font_GetTexture(renderer, "Open Server", 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	clientTexture = Font_GetTexture(renderer, "Join Server", 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	cursorTexture = IMG_LoadTexture(renderer, "res/cursor.png");
	cursorX = 640;
	cursorY = 360;
}

void MultiConnectScene_Event(SDL_Event* event)
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
			if (serverHover)
			{
				Game_ChangeScene(MULTI_SERVER);
			}
			else if (clientHover)
			{
				Game_ChangeScene(MULTI_CLIENT);
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game_ChangeScene(TITLE);
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
			if (serverHover)
			{
				Game_ChangeScene(MULTI_SERVER);
			}
			else if (clientHover)
			{
				Game_ChangeScene(MULTI_CLIENT);
			}
			break;
		case SDL_CONTROLLER_BUTTON_B:
			Game_ChangeScene(TITLE);
			break;
		}
	}
}

void MultiConnectScene_Update(double delta)
{
	serverHover = cursorRect.x >= serverRect.x && cursorRect.x < serverRect.x + serverRect.w && cursorRect.y >= serverRect.y && cursorRect.y < serverRect.y + serverRect.h;
	clientHover = cursorRect.x >= clientRect.x && cursorRect.x < clientRect.x + clientRect.w && cursorRect.y >= clientRect.y && cursorRect.y < clientRect.y + clientRect.h;
}

void MultiConnectScene_Render(SDL_Renderer* renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	serverRect.w = w * 500 / 1280;
	serverRect.h = h * 100 / 720;
	serverRect.x = w / 2 - serverRect.w / 2;
	serverRect.y = h / 3 - serverRect.h / 2;
	if (serverHover)
	{
		SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	}
	SDL_RenderFillRect(renderer, &serverRect);
	SDL_RenderCopy(renderer, serverTexture, NULL, &serverRect);

	clientRect.w = w * 500 / 1280;
	clientRect.h = h * 100 / 720;
	clientRect.x = w / 2 - clientRect.w / 2;
	clientRect.y = h * 2 / 3 - clientRect.h / 2;
	if (clientHover)
	{
		SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	}
	SDL_RenderFillRect(renderer, &clientRect);
	SDL_RenderCopy(renderer, clientTexture, NULL, &clientRect);

	cursorRect.w = 40 * w / 1280;
	cursorRect.h = 60 * h / 720;
	cursorRect.x = cursorX * w / 1280;
	cursorRect.y = cursorY * h / 720;
	SDL_RenderCopy(renderer, cursorTexture, NULL, &cursorRect);
}

void MultiConnectScene_Quit()
{
	SDL_DestroyTexture(serverTexture);
	SDL_DestroyTexture(clientTexture);
	SDL_DestroyTexture(cursorTexture);
}