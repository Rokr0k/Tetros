#ifndef MULTI_SERVER_SCENE_H_
#define MULTI_SERVER_SCENE_H_

void MultiServerScene_Init(SDL_Renderer* renderer);
void MultiServerScene_Event(SDL_Event* event);
void MultiServerScene_Update(double delta);
void MultiServerScene_Render(SDL_Renderer* renderer);
void MultiServerScene_Quit();

#endif