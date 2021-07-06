#ifndef TETRIS_H_
#define TETRIS_H_

void Tetris_Init();
int Tetris_Verify(int x, int y, int state);
int Tetris_Matrix(int x, int y);
int Tetris_Rotate(int dir);
int Tetris_Shift(int dir);
int Tetris_Fall();
int Tetris_Fallable();
void Tetris_Lock();
int Tetris_GetTechnique();
int Tetris_ClearFullLine();
void Tetris_PullEmptyLine();
int Tetris_Harddrop();
void Tetris_Hold();
int Tetris_WhatHold();
int Tetris_CanHold();
int Tetris_Preview(int index);

int Tetris_Next();

int Tetris_GhostY();

#endif