#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "titleScene.h"
#include "font.h"
#include "game.h"

static SDL_Texture *titleTexture;
static SDL_Rect titleRect;

static SDL_Texture *playTexture;
static SDL_Rect playRect;

static void quitGame();

void TitleScene_Init(SDL_Renderer *renderer)
{
	titleTexture = IMG_LoadTexture(renderer, "res/title.png");
	playTexture = Font_GetTexture(renderer, "Play", 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
}

void TitleScene_Event(SDL_Event *event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			quitGame();
			break;
		case SDLK_RETURN:
			Game_ChangeScene(PLAY);
			break;
		}
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		switch (event->cbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
				Game_ChangeScene(PLAY);
			break;
		case SDL_CONTROLLER_BUTTON_B:
			quitGame();
			break;
		}
	}
}

void TitleScene_Update(double delta)
{
}

void TitleScene_Render(SDL_Renderer *renderer)
{
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	titleRect.w = w * 700 / 1280;
	titleRect.h = h * 300 / 720;
	titleRect.x = w / 2 - titleRect.w / 2;
	titleRect.y = h / 3 - titleRect.h / 2;
	SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

	playRect.w = w * 300 / 1280;
	playRect.h = h * 100 / 720;
	playRect.x = w / 2 - playRect.w / 2;
	playRect.y = h * 2 / 3 - playRect.h / 2;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &playRect);
	SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
}

void TitleScene_Quit()
{
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(playTexture);
}

static void quitGame()
{
	SDL_Event quit;
	quit.type = SDL_QUIT;
	SDL_PushEvent(&quit);
}
