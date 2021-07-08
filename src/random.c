#include "random.h"
#include <stdlib.h>
#include <time.h>

static int bag[7];
static int gb;

static void fillBag();

void Random_Init()
{
	srand((unsigned int)time(NULL));
	fillBag();
	gb = -1;
}

int Random_Bag()
{
	int i;
	for (i = 0; i < 7; i++)
	{
		if (bag[i] != -1)
		{
			int r = bag[i];
			bag[i] = -1;
			return r;
		}
	}
	fillBag();
	return Random_Bag();
}

int Random_Garbage()
{
	if (gb < 0)
	{
		gb = rand() % 10;
		return gb;
	}
	else
	{
		if (rand() % 5 < 1)
		{
			return gb;
		}
		else
		{
			gb = rand() % 10;
			return gb;
		}
	}
}

static void fillBag()
{
	int i;
	for (i = 0; i < 7; i++)
	{
		bag[i] = i;
	}
	for (i = 0; i < 7; i++)
	{
		int j = rand() % 7;
		int t = bag[i];
		bag[i] = bag[j];
		bag[j] = t;
	}
}