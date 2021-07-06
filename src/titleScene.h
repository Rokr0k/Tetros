#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

void TitleScene_Init(SDL_Renderer* renderer);
void TitleScene_Event(SDL_Event* event);
void TitleScene_Render(SDL_Renderer* renderer);
void TitleScene_Quit();

#endif