#include <SDL.h>
#include <SDL_ttf.h>
#include "font.h"

TTF_Font* font;
void Font_Init()
{
	TTF_Init();
	font = TTF_OpenFont("res/ComicSans.ttf", 60);
}

TTF_Font* Font_GetFont()
{
	return font;
}

void Font_Quit()
{
	TTF_CloseFont(font);
	TTF_Quit();
}