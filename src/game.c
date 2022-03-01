#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include "font.h"
#include "game.h"
#include "gamecontroller.h"
#include "titleScene.h"
#include "playScene.h"

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

	Mix_OpenAudio(22050, AUDIO_S16LSB, 1, 2048);
	music = Mix_LoadMUS("res/backgroundmusic.ogg");
	Mix_PlayMusic(music, -1);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	GC_Init();
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
	case TITLE:
		TitleScene_Update(delta);
		break;
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