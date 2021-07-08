#include "tetris.h"
#include "random.h"

const static int size[7] = { 5, 3, 3, 3, 3, 3, 3 };

const static int tetromino[7][4][5][5] = {
	{
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0},
			{1, 1, 1, 1, 0},
			{0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0},
			{0, 0, 1, 0, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{2, 2, 2},
			{2, 0, 0}
		},
		{
			{0, 2, 0},
			{0, 2, 0},
			{0, 2, 2}
		},
		{
			{0, 0, 2},
			{2, 2, 2},
			{0, 0, 0}
		},
		{
			{2, 2, 0},
			{0, 2, 0},
			{0, 2, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{3, 3, 3},
			{0, 0, 3}
		},
		{
			{0, 3, 3},
			{0, 3, 0},
			{0, 3, 0}
		},
		{
			{3, 0, 0},
			{3, 3, 3},
			{0, 0, 0}
		},
		{
			{0, 3, 0},
			{0, 3, 0},
			{3, 3, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{0, 4, 4},
			{0, 4, 4}
		},
		{
			{0, 4, 4},
			{0, 4, 4},
			{0, 0, 0}
		},
		{
			{4, 4, 0},
			{4, 4, 0},
			{0, 0, 0}
		},
		{
			{0, 0, 0},
			{4, 4, 0},
			{4, 4, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{5, 5, 0},
			{0, 5, 5}
		},
		{
			{0, 0, 5},
			{0, 5, 5},
			{0, 5, 0}
		},
		{
			{5, 5, 0},
			{0, 5, 5},
			{0, 0, 0}
		},
		{
			{0, 5, 0},
			{5, 5, 0},
			{5, 0, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{6, 6, 6},
			{0, 6, 0}
		},
		{
			{0, 6, 0},
			{0, 6, 6},
			{0, 6, 0}
		},
		{
			{0, 6, 0},
			{6, 6, 6},
			{0, 0, 0}
		},
		{
			{0, 6, 0},
			{6, 6, 0},
			{0, 6, 0}
		}
	},
	{
		{
			{0, 0, 0},
			{0, 7, 7},
			{7, 7, 0}
		},
		{
			{0, 7, 0},
			{0, 7, 7},
			{0, 0, 7}
		},
		{
			{0, 7, 7},
			{7, 7, 0},
			{0, 0, 0}
		},
		{
			{7, 0, 0},
			{7, 7, 0},
			{0, 7, 0}
		}
	}
};

const static int kickInfos[3][4][5][2] = {
	{
		{
			{0, 0},
			{0, 0},
			{0, 0},
			{0, 0},
			{0, 0}
		},
		{
			{0, 0},
			{1, 0},
			{1, -1},
			{0, 2},
			{1, 2}
		},
		{
			{0, 0},
			{0, 0},
			{0, 0},
			{0, 0},
			{0, 0}
		},
		{
			{0, 0},
			{-1, 0},
			{-1, -1},
			{0, 2},
			{-1, 2}
		}
	},
	{
		{
			{0, 0},
			{-1, 0},
			{2, 0},
			{-1, 0},
			{2, 0}
		},
		{
			{-1, 0},
			{0, 0},
			{0, 0},
			{0, 1},
			{0, -2}
		},
		{
			{-1, 1},
			{1, 1},
			{-2, 1},
			{1, 0},
			{-2, 0}
		},
		{
			{0, 1},
			{0, 1},
			{0, 1},
			{0, -1},
			{0, 2}
		}
	},
	{
		{
			{0, 0}
		},
		{
			{0, -1}
		},
		{
			{-1, -1}
		},
		{
			{-1, 0}
		}
	}
};

const int initialPos[7][2] = {
	{2, 18},
	{3, 19},
	{3, 19},
	{3, 19},
	{3, 19},
	{3, 19},
	{3, 19}
};

static int matrix[40][10];

static int currentType;
static int currentState;
static int currentX;
static int currentY;
static int ghostY;
static int hold;

static int holdFlag;
static int clearing;

static int linesExecuted[40];

static int previewQueue[5];
static int previewI;

static void setGhostY();

static int lastMove;
static int isTSpin;
static int isTSpinMini;
static int backToBack;

static enum Technique technique;

void Tetris_Init()
{
	Random_Init();
	int i, j;
	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 10; j++)
		{
			matrix[i][j] = 0;
		}
	}
	currentType = Random_Bag();
	currentState = 0;
	currentX = initialPos[currentType][0];
	currentY = initialPos[currentType][1];
	for (i = 0; i < 5; i++)
	{
		previewQueue[i] = Random_Bag();
	}
	previewI = 0;
	hold = -1;
	holdFlag = 0;
	clearing = 0;
	technique = Tech_None;
	setGhostY();
}

int Tetris_Matrix(int x, int y)
{
	if (x < 0 || x >= 10 || y < 0 || y >= 40)
	{
		return -1;
	}
	int r = matrix[y][x];
	if (!r && !clearing)
	{
		if (x >= currentX && x < currentX + size[currentType] && y >= ghostY && y < ghostY + size[currentType] && tetromino[currentType][currentState][y - ghostY][x - currentX])
		{
			r = tetromino[currentType][currentState][y - ghostY][x - currentX] + 10;
		}
		if (x >= currentX && x < currentX + size[currentType] && y >= currentY && y < currentY + size[currentType] && tetromino[currentType][currentState][y - currentY][x - currentX])
		{
			r = tetromino[currentType][currentState][y - currentY][x - currentX];
		}
	}
	return r;
}

int Tetris_Verify(int x, int y, int state)
{
	int i, j;
	for (i = 0; i < size[currentType]; i++)
	{
		for (j = 0; j < size[currentType]; j++)
		{
			if (tetromino[currentType][state][j][i] && (x + i < 0 || x + i >= 10 || y + j < 0 || y + j >= 40 || matrix[y + j][x + i]))
			{
				return 0;
			}
		}
	}
	return 1;
}

int Tetris_Rotate(int dir)
{
	int desiredState = currentState + dir;
	while (desiredState < 0)
		desiredState += 4;
	while (desiredState >= 4)
		desiredState -= 4;
	int type;
	if (currentType == 0)
		type = 1;
	else if (currentType == 3)
		type = 2;
	else
		type = 0;
	int i;
	for (i = 0; i < 5; i++)
	{
		int dx = kickInfos[type][currentState][i][0] - kickInfos[type][desiredState][i][0];
		int dy = kickInfos[type][currentState][i][1] - kickInfos[type][desiredState][i][1];
		if (Tetris_Verify(currentX + dx, currentY + dy, desiredState))
		{
			currentX += dx;
			currentY += dy;
			currentState = desiredState;
			setGhostY();
			lastMove = i<4?1:2;
			return 1;
		}
	}
	return 0;
}

int Tetris_Shift(int dir)
{
	if (Tetris_Verify(currentX + dir, currentY, currentState))
	{
		currentX += dir;
		setGhostY();
		lastMove = 3;
		return 1;
	}
	return 0;
}

int Tetris_Fall()
{
	if (Tetris_Verify(currentX, currentY - 1, currentState))
	{
		currentY--;
		lastMove = 4;
		return 1;
	}
	return 0;
}

int Tetris_Fallable()
{
	return Tetris_Verify(currentX, currentY - 1, currentState);
}

void Tetris_Lock()
{
	if (clearing)
	{
		return;
	}
	isTSpin = 0;
	isTSpinMini = 0;
	if (currentType == 5 && (lastMove == 1 || lastMove == 2))
	{
		int cnt = 0;
		if (currentX < 0 || currentY < 0 || matrix[currentY][currentX])
			cnt++;
		if (currentX + 2 >= 10 || currentY < 0 || matrix[currentY][currentX + 2])
			cnt++;
		if (currentX + 2 >= 10 || currentY + 2 >= 40 || matrix[currentY + 2][currentX + 2])
			cnt++;
		if (currentX < 0 || currentY + 2 >= 40 || matrix[currentY + 2][currentX])
			cnt++;
		if (cnt >= 3)
		{
			isTSpin = 1;
		}
		if (isTSpin && lastMove == 1)
		{
			switch (currentState)
			{
			case 0:
				if (!matrix[currentY + 2][currentX] || !matrix[currentY + 2][currentX + 2])
				{
					isTSpin = 0;
					isTSpinMini = 1;
				}
				break;
			case 1:
				if (!matrix[currentY][currentX+2] || !matrix[currentY + 2][currentX + 2])
				{
					isTSpin = 0;
					isTSpinMini = 1;
				}
				break;
			case 2:
				if (!matrix[currentY][currentX] || !matrix[currentY][currentX + 2])
				{
					isTSpin = 0;
					isTSpinMini = 1;
				}
				break;
			case 3:
				if (!matrix[currentY][currentX] || !matrix[currentY + 2][currentX])
				{
					isTSpin = 0;
					isTSpinMini = 1;
				}
				break;
			}
		}
	}
	int i, j;
	for (i = 0; i < size[currentType]; i++)
	{
		for (j = 0; j < size[currentType]; j++)
		{
			if (tetromino[currentType][currentState][i][j])
			{
				matrix[currentY + i][currentX + j] = tetromino[currentType][currentState][i][j];
			}
		}
	}
	holdFlag = 0;
}

enum Technique Tetris_GetTechnique()
{
	return technique;
}

int Tetris_ClearFullLine()
{
	if (clearing)
	{
		return 0;
	}
	int i, j;
	int cnt = 0;
	clearing = 0;
	for (i = 0; i < 40; i++)
	{
		linesExecuted[i] = 1;
		for (j = 0; j < 10; j++)
		{
			if (matrix[i][j] == 0)
			{
				linesExecuted[i] = 0;
				break;
			}
		}
		if (linesExecuted[i])
		{
			for (j = 0; j < 10; j++)
			{
				matrix[i][j] = 0;
			}
			clearing = 1;
			cnt++;
		}
	}

	int allclear = 1;
	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (matrix[i][j])
			{
				allclear = 0;
				break;
			}
		}
		if (!allclear)
			break;
	}

	technique = cnt;
	if (isTSpin)
		technique |= Tech_TSpin;
	else if (isTSpinMini)
		technique |= Tech_TSpinMini;
	if (backToBack)
		technique |= Tech_BackToBack;
	if (allclear)
		technique |= Tech_AllClear;

	return clearing;
}

void Tetris_PullEmptyLine()
{
	if (!clearing)
	{
		return;
	}
	int i, j, k = 0;
	for (i = 0; i < 40; i++)
	{
		while (i + k < 40 && linesExecuted[i+k])
			k++;
		for (j = 0; j < 10; j++)
		{
			matrix[i][j] = i + k < 40 ? matrix[i + k][j] : 0;
		}
	}
	clearing = 0;
}

int Tetris_Harddrop()
{
	int r = currentY - ghostY;
	currentY = ghostY;
	return r;
}

void Tetris_Hold()
{
	if (holdFlag)
	{
		return;
	}
	holdFlag = 1;
	if (hold < 0)
	{
		hold = currentType;
		Tetris_Next();
	}
	else
	{
		int t = hold;
		hold = currentType;
		currentType = t;
		currentState = 0;
		currentX = initialPos[currentType][0];
		currentY = initialPos[currentType][1];
		setGhostY();
	}
}

int Tetris_WhatHold()
{
	return hold;
}

int Tetris_CanHold()
{
	return holdFlag;
}

int Tetris_Preview(int index)
{
	return previewQueue[(previewI + index) % 5];
}

int Tetris_Next()
{
	if (!Tetris_Verify(initialPos[previewQueue[previewI]][0], initialPos[previewQueue[previewI]][1], 0))
	{
		return 0;
	}
	currentType = previewQueue[previewI];
	previewQueue[previewI] = Random_Bag();
	previewI = (previewI + 1) % 5;
	currentState = 0;
	currentX = initialPos[currentType][0];
	currentY = initialPos[currentType][1];
	setGhostY();
	return 1;
}

int Tetris_GhostY()
{
	return ghostY;
}

int Tetris_Garbage()
{
	int c = Random_Garbage();
	int i, j;
	for (i = 0; i < 10; i++)
	{
		if (matrix[39][i])
		{
			return 0;
		}
	}
	for (i = 39; i > 0; i--)
	{
		for (j = 0; j < 10; j++)
		{
			matrix[i][j] = matrix[i - 1][j];
		}
	}
	for (i = 0; i < 10; i++)
	{
		if (i == c)
		{
			matrix[0][i] = 0;
		}
		else
		{
			matrix[0][i] = 8;
		}
	}
	return 1;
}

static void setGhostY()
{
	ghostY = currentY;
	while (Tetris_Verify(currentX, ghostY - 1, currentState))
	{
		ghostY--;
	}
}