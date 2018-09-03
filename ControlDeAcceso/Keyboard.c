#include "Keyboard.h"
#include "MuxKbDs.h"
#include <stdio.h> 
#ifdef _WIN32 
#include <conio.h>
#endif

/*Circular buffer length*/
#define KB_BUF_LEN 254

/*Keyboard information*/
//Current state
static unsigned char kbState[ROWS][COLUMNS] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
//Current time of key press
static unsigned char kbTime[ROWS][COLUMNS] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
//New keyboard state
static unsigned char newKbState[ROWS][COLUMNS];
//Circular buffer for storing keys
static unsigned char buffer[KB_BUF_LEN];
//Pointers
static unsigned long long readPtr = 0, writePtr = 0;
//Look up table for keyboard positions
static const unsigned char pos2key[ROWS][COLUMNS]= { { '1','2','3' },{ '4','5','6' },{ '7','8','9' },{ '*','0','#' } };

char kBPeek()
{
	//Peek the next character
	if(kbHit()==1)
		return buffer[readPtr%KB_BUF_LEN];
	return 0;
}
int kBInit()
{
	return muxKbDsInit();
}

static void update()
{
#ifndef _WIN32 
	/*If running on the kinetis*/
	//Get the new keyboard state
	muxKbDsGetKbState(newKbState);
	//How many keys are pressed
	unsigned keyP=0;
	for (unsigned i = 0; i < ROWS; i++)
		for (unsigned j = 0; j < COLUMNS; j++)
			keyP+=newKbState[i][j];
	//If more than one, the keyboard can have an error
	if(keyP>1)
		return;
	for (unsigned i = 0; i < ROWS; i++)
	{
		for (unsigned j = 0; j < COLUMNS; j++)
		{
			if (kbState[i][j] == 0 && newKbState[i][j] == 1)
			{
				//Software anti bounce
				if (kbTime[i][j] == 100) 
				{
					buffer[writePtr%KB_BUF_LEN] = pos2key[i][j];
					kbState[i][j] = newKbState[i][j];
					writePtr++;
				}
				else
					kbTime[i][j]++;

			}
			else if (kbState[i][j] == 1 && newKbState[i][j] == 0)
				kbState[i][j] = newKbState[i][j];
			if (newKbState[i][j] == 0)
				kbTime[i][j] = 0;

		}
	}
#else
	/*If the program is running on windows use windows functions*/
	if (_kbhit())
	{
		buffer[writePtr%KB_BUF_LEN] = _getch();
		writePtr++;
	}
#endif // !_WIN32 

	
}
char readKb(void)
{
	//Update the keyboard
	update();
	//If i have a character return it
	if (readPtr < writePtr)
		return buffer[readPtr++%KB_BUF_LEN];
	return 0;
}
int kbHit()
{
	//Update the keyboard
	update();
	//If i have a character return 1
	if (writePtr > readPtr)
		return 1;
	return 0;
}
void kbFlush()
{
	//Flush all stored keys
	readPtr = writePtr;
}
