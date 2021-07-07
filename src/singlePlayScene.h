#ifndef SINGLE_PLAY_SCENE_H_
#define SINGLE_PLAY_SCENE_H_

void SinglePlayScene_Init(SDL_Renderer* renderer);
void SinglePlayScene_Event(SDL_Event* event);
void SinglePlayScene_Update(double delta);
void SinglePlayScene_Render(SDL_Renderer* renderer);
void SinglePlayScene_Quit();

#endif