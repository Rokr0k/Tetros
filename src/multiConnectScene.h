#ifndef MULTI_CONNECT_SCENE_H_
#define MULTI_CONNECT_SCENE_H_

void MultiConnectScene_Init(SDL_Renderer* renderer);
void MultiConnectScene_Event(SDL_Event* event);
void MultiConnectScene_Update(double delta);
void MultiConnectScene_Render(SDL_Renderer* renderer);
void MultiConnectScene_Quit();

#endif