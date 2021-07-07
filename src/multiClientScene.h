#ifndef MULTI_CLIENT_SCENE_H_
#define MULTI_CLIENT_SCENE_H_

void MultiClientScene_Init(SDL_Renderer* renderer);
void MultiClientScene_Event(SDL_Event* event);
void MultiClientScene_Update(double delta);
void MultiClientScene_Render(SDL_Renderer* renderer);
void MultiClientScene_Quit();

#endif