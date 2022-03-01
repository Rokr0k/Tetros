#ifndef GAME_H_
#define GAME_H_

enum SceneType
{
	TITLE, PLAY
};

void Game_Init(SDL_Window* window);

void Game_Event(SDL_Event* event);
void Game_Update(double delta);
void Game_Render();

void Game_ChangeScene(enum SceneType sceneType);

void Game_Quit();

#endif