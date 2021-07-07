#include <SDL.h>
#include "gamecontroller.h"

static SDL_GameController* controller;

void GC_Init()
{
	SDL_GameControllerAddMappingsFromFile("res/gamecontrollerdb.txt");
	int i;
	for (i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			controller = SDL_GameControllerOpen(i);
			break;
		}
	}
}

SDL_GameController* GC_GetController()
{
	return controller;
}

void GC_Quit()
{
	SDL_GameControllerClose(controller);
}