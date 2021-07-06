#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "titleScene.h"
#include "font.h"
#include "game.h"

static SDL_Texture* titleTexture;
static SDL_Rect titleRect;

static SDL_Texture* singleButtonText;
static SDL_Rect singleButtonRect;
static int singleButtonHover;

static void quitGame();

void TitleScene_Init(SDL_Renderer* renderer)
{
	titleTexture = IMG_LoadTexture(renderer, "res/title.png");
	SDL_Color black = { 0x00, 0x00, 0x00, 0xFF };
	SDL_Surface* temp = TTF_RenderText_Solid(Font_GetFont(), "Start", black);
	singleButtonText = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
}

void TitleScene_Event(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_SPACE:
			Game_ChangeScene(PLAY);
			break;
		case SDLK_ESCAPE:
			quitGame();
			break;
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		switch (event->cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
		case SDL_CONTROLLER_BUTTON_B:
			Game_ChangeScene(PLAY);
			break;
		case SDL_CONTROLLER_BUTTON_START:
			quitGame();
			break;
		}
	}
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

	singleButtonRect.w = w * 300 / 1280;
	singleButtonRect.h = h * 100 / 720;
	singleButtonRect.x = w / 2 - singleButtonRect.w / 2;
	singleButtonRect.y = h * 2 / 3 - singleButtonRect.h / 2;
	if (singleButtonHover)
	{
		SDL_SetRenderDrawColor(renderer, 0x9F, 0x9F, 0x9F, SDL_ALPHA_OPAQUE);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	}
	SDL_RenderFillRect(renderer, &singleButtonRect);
	SDL_RenderCopy(renderer, singleButtonText, NULL, &singleButtonRect);
}

void TitleScene_Quit()
{
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(singleButtonText);
}

static void quitGame()
{
	SDL_Event quit;
	quit.type = SDL_QUIT;
	SDL_PushEvent(&quit);
}