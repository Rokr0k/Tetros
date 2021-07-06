#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "font.h"
#include "game.h"
#include "titleScene.h"
#include "playScene.h"

static SDL_Renderer* renderer;

static enum SceneType sceneId;

static void SceneInit();
static void SceneQuit();

void Game_Init(SDL_Window* window)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	Font_Init();
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_OGG);
	sceneId = TITLE;
	SceneInit(NULL);
}

void Game_Event(SDL_Event* event)
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Event(event);
		break;
	case PLAY:
		PlayScene_Event(event);
		break;
	}
}

void Game_Update(double delta)
{
	switch (sceneId)
	{
	case PLAY:
		PlayScene_Update(delta);
		break;
	}
}

void Game_Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Render(renderer);
		break;
	case PLAY:
		PlayScene_Render(renderer);
		break;
	}
	SDL_RenderPresent(renderer);
}

void Game_ChangeScene(enum SceneType sceneType)
{
	SceneQuit();
	sceneId = sceneType;
	SceneInit();
}

void Game_Quit()
{
	SDL_DestroyRenderer(renderer);
	SceneQuit();
	Font_Quit();
	IMG_Quit();
	Mix_Quit();
}

static void SceneInit()
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Init(renderer);
		break;
	case PLAY:
		PlayScene_Init(renderer);
		break;
	}
}

static void SceneQuit()
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Quit();
		break;
	case PLAY:
		PlayScene_Quit();
		break;
	}
}