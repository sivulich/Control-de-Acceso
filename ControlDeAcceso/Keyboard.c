#include "Keyboard.h"
#include "MuxKbDs.h"
#include <stdio.h>  
#define KB_BUF_LEN 256
static unsigned char kbState[ROWS][COLUMNS] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
static unsigned char kbTime[ROWS][COLUMNS] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
static unsigned char newKbState[ROWS][COLUMNS];
static unsigned char buffer[KB_BUF_LEN];
static unsigned long long readPtr = 0, writePtr = 0;

static const unsigned char pos2key[ROWS][COLUMNS]= { { '1','2','3' },{ '4','5','6' },{ '7','8','9' },{ '*','0','#' } };
int kBInit()
{
	return muxKbDsInit();
}

static void update()
{
	muxKbDsGetKbState(newKbState);
	for (unsigned i = 0; i < ROWS; i++)
	{
		for (unsigned j = 0; j < COLUMNS; j++)
		{
			if (kbState[i][j] == 0 && newKbState[i][j] == 1)
			{
				if(kbTime[i][j]==50) //Cambiar el magic number por algo
				{
					buffer[writePtr%KB_BUF_LEN] = pos2key[i][j];
					kbState[i][j] = newKbState[i][j];
					writePtr++;
				}
				else
					kbTime[i][j]++;

			}
			else if(kbState[i][j] == 1 && newKbState[i][j] == 0)
				kbState[i][j] = newKbState[i][j];
			if(newKbState[i][j] == 0)
				kbTime[i][j]=0;

		}
	}
}
char readKb(void)
{
	update();
	if (readPtr < writePtr)
		return buffer[readPtr++%KB_BUF_LEN];
	return 0;
}
int kbHit()
{
	update();
	if (writePtr > readPtr)
		return 1;
	return 0;
}
void kbFlush()
{
	readPtr = writePtr;
}
