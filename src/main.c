#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game.h"


int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);

#ifdef _DEBUG
	SDL_Window* window = SDL_CreateWindow("Tetros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
#else
	SDL_Window* window = SDL_CreateWindow("Tetros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
#endif

	Game_Init(window);

	int quit = 0;
	Uint64 counter = 0;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = 1;
			}
			else
			{
				Game_Event(&event);
			}
		}
		Game_Update((double)(SDL_GetPerformanceCounter() - counter) / SDL_GetPerformanceFrequency());
		counter = SDL_GetPerformanceCounter();
		Game_Render();
	}

	Game_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}