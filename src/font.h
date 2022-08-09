#ifndef FONT_H_
#define FONT_H_

void Font_Init();
SDL_Texture *Font_GetTexture(SDL_Renderer *renderer, const char *text, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void Font_Quit();

#endif