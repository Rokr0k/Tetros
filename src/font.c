#include <SDL.h>
#include <SDL_ttf.h>
#include "font.h"

static TTF_Font* font;

void Font_Init()
{
	TTF_Init();
	font = TTF_OpenFont("res/ComicSans.ttf", 60);
}

SDL_Texture* Font_GetTexture(SDL_Renderer* renderer, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color color = { r, g, b, a };
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void Font_Quit()
{
	TTF_CloseFont(font);
	TTF_Quit();
}