#include <SDL.h>
#include "game.h"
#include <SDL_image.h>
#include <SDL_ttf.h>


int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	SDL_GameControllerAddMappingsFromFile("res/gamecontrollerdb.txt");

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
		Game_Update((SDL_GetPerformanceCounter() - counter) * 1. / SDL_GetPerformanceFrequency());
		counter = SDL_GetPerformanceCounter();
		Game_Render();
	}

	Game_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}