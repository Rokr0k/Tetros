#ifndef MULTI_PLAY_SCENE_H_
#define MULTI_PLAY_SCENE_H_

void MultiPlayScene_Init(SDL_Renderer* renderer);
void MultiPlayScene_Event(SDL_Event* event);
void MultiPlayScene_Update(double delta);
void MultiPlayScene_Render(SDL_Renderer* renderer);
void MultiPlayScene_Quit();
void MultiPlayScene_SetPeer(IPaddress);

#endif