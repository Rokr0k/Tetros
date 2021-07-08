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
int Tetris_ClearFullLine();
void Tetris_PullEmptyLine();
int Tetris_Harddrop();
void Tetris_Hold();
int Tetris_WhatHold();
int Tetris_CanHold();
int Tetris_Preview(int index);

int Tetris_Next();

int Tetris_GhostY();

int Tetris_Garbage();

enum Technique
{
	Tech_None, Tech_Single, Tech_Double, Tech_Triple, Tech_Tetris, Tech_B_Tetris,
	Tech_TSpin, Tech_TSpin_Single, Tech_TSpin_Double, Tech_TSpin_Triple, Tech_B_TSpin, Tech_B_TSpin_Single, Tech_B_TSpin_Double, Tech_B_TSpin_Triple,
	Tech_TSpinM, Tech_TSpinM_Single, Tech_TSpinM_Double, Tech_B_TSpinM, Tech_B_TSpinM_Single, Tech_B_TSpinM_Double, Tech_Gameover
};
enum Technique Tetris_GetTechnique();

#endif