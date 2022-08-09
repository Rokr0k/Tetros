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
	Tech_None,
	Tech_Single,
	Tech_Double,
	Tech_Triple,
	Tech_Tetris,
	Tech_TSpin = 0x08,
	Tech_TSpinMini = 0x10,
	Tech_BackToBack = 0x20,
	Tech_AllClear = 0x40
};
enum Technique Tetris_GetTechnique();

#endif