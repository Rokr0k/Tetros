#ifndef PLAY_SCENE_H_
#define PLAY_SCENE_H_

void PlayScene_Init(SDL_Renderer* renderer);
void PlayScene_Event(SDL_Event* event);
void PlayScene_Update(double delta);
void PlayScene_Render(SDL_Renderer* renderer);
void PlayScene_Quit();

#endif