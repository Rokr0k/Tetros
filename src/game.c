#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "font.h"
#include "game.h"
#include "gamecontroller.h"
#include "titleScene.h"
#include "singlePlayScene.h"
#include "multiConnectScene.h"
#include "multiServerScene.h"
#include "multiClientScene.h"
#include "multiPlayScene.h"

static SDL_Renderer* renderer;

static enum SceneType sceneId;

static Mix_Music* music;

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
	SceneInit();

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 2048);
	music = Mix_LoadMUS("res/backgroundmusic.ogg");
	Mix_PlayMusic(music, -1);

	//SDL_SetRelativeMouseMode(SDL_TRUE);
	GC_Init();
}

void Game_Event(SDL_Event* event)
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Event(event);
		break;
	case SINGLE_PLAY:
		SinglePlayScene_Event(event);
		break;
	case MULTI_CONNECT:
		MultiConnectScene_Event(event);
		break;
	case MULTI_SERVER:
		MultiServerScene_Event(event);
		break;
	case MULTI_CLIENT:
		MultiClientScene_Event(event);
		break;
	case MULTI_PLAY:
		MultiPlayScene_Event(event);
		break;
	}
}

void Game_Update(double delta)
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Update(delta);
		break;
	case SINGLE_PLAY:
		SinglePlayScene_Update(delta);
		break;
	case MULTI_CONNECT:
		MultiConnectScene_Update(delta);
		break;
	case MULTI_SERVER:
		MultiServerScene_Update(delta);
		break;
	case MULTI_CLIENT:
		MultiClientScene_Update(delta);
		break;
	case MULTI_PLAY:
		MultiPlayScene_Update(delta);
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
	case SINGLE_PLAY:
		SinglePlayScene_Render(renderer);
		break;
	case MULTI_CONNECT:
		MultiConnectScene_Render(renderer);
		break;
	case MULTI_SERVER:
		MultiServerScene_Render(renderer);
		break;
	case MULTI_CLIENT:
		MultiClientScene_Render(renderer);
		break;
	case MULTI_PLAY:
		MultiPlayScene_Render(renderer);
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
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

static void SceneInit()
{
	switch (sceneId)
	{
	case TITLE:
		TitleScene_Init(renderer);
		break;
	case SINGLE_PLAY:
		SinglePlayScene_Init(renderer);
		break;
	case MULTI_CONNECT:
		MultiConnectScene_Init(renderer);
		break;
	case MULTI_SERVER:
		MultiServerScene_Init(renderer);
		break;
	case MULTI_CLIENT:
		MultiClientScene_Init(renderer);
		break;
	case MULTI_PLAY:
		MultiPlayScene_Init(renderer);
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
	case SINGLE_PLAY:
		SinglePlayScene_Quit();
		break;
	case MULTI_CONNECT:
		MultiConnectScene_Quit();
		break;
	case MULTI_SERVER:
		MultiServerScene_Quit();
		break;
	case MULTI_CLIENT:
		MultiClientScene_Quit();
		break;
	case MULTI_PLAY:
		MultiPlayScene_Quit();
		break;
	}
}