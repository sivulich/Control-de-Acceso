#include "States.h"
#include "Keyboard.h"
#include <stdio.h>
#include <string.h>
#include "DataBase.h"
#include "Clock.h"
typedef struct {
	clock_t c;
	/*Bits: N N N N | N N s2 s1*/
	unsigned char uFlags[MAX_USERS];
} InPasswordDat;
#define STRIKE_1 0b01
#define STRIKE_2 0b10
#define BLOCKED  0b11

static event  InPsswd_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	InPasswordDat* myDat = (InPasswordDat*)thi->stateData;
	char c = readKb();
	if (c == '*' && data->currPassLen > 0)
	{
		myDat->c = getTime();
		data->currPassLen--;
		data->currPsswd[data->currPassLen] = 0;
	}
	else if (c != '*' && c != '#' && data->currPassLen < MAX_PASS_LEN)
	{
		myDat->c = getTime();
		data->currPsswd[data->currPassLen] = c;
		data->currPassLen++;
	}
	else if (c == '#')
		return CONTINUE;
	else
		return ERROR_WRONG_PASS;
	return BREAK;
}
static event InPassword_Reset(State* thi, event ev, void* uData)
{
	InPasswordDat* myDat = (InPasswordDat*)thi->stateData;
	myDat->c = 0;
	kbFlush();
	return BREAK;
}

static event InPsswd_Open(State* thi, event ev, void* uData)
{
	InPasswordDat* myDat = (InPasswordDat*)thi->stateData;
	AppData* data = (AppData*)uData;
	myDat->c = 0;
	unsigned char i = 0;
	encrypt(data->currID);
	encrypt(data->currPsswd);
	while (keys[i][0][0] != 0)
	{
		if (!strcmp(keys[i][0], data->currID))
		{
			if ((myDat->uFlags[i] & BLOCKED) == BLOCKED)
			{
				if (strcmp(MASTER_PASS, data->currPsswd))
					return ERROR_WRONG_ID;
				else
				{
					myDat->uFlags[i] = myDat->uFlags[i] & ~BLOCKED;
					return OPEN;
				}
			}

			if (!strcmp(keys[i][1], data->currPsswd))
			{
				myDat->uFlags[i] = myDat->uFlags[i] & ~BLOCKED;
				return OPEN;
			}
			else
			{
				if ((myDat->uFlags[i] & BLOCKED) != STRIKE_2)
					if ((myDat->uFlags[i] & BLOCKED) != STRIKE_1)
						myDat->uFlags[i] |= STRIKE_1;
					else
						myDat->uFlags[i] = (myDat->uFlags[i] & ~BLOCKED) | STRIKE_2;
				else
					myDat->uFlags[i] |= BLOCKED;

				return ERROR_WRONG_PASS;
			}
		}
		i++;
	}
	return ERROR_WRONG_ID;
}
static const Transition TableInPassword[] = {
	{ KEY_PRESS,InPsswd_addKey,&InPassword },
{ CONTINUE,InPsswd_Open,&Open },
{ ERROR_WRONG_PASS,InPassword_Reset,&Idle },
{ ERROR_WRONG_ID,InPassword_Reset,&Idle },
{ ERROR_TIME_OUT,InPassword_Reset,&Idle },
{ END_OF_TABLE,NULL,&InPassword }
};


static event inPsswdLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	InPasswordDat* myDat = (InPasswordDat*)thi->stateData;
	if (myDat->c == 0)
		myDat->c = getTime();
	if (((double)getTime() - myDat->c) / CLOCKS_PER_SEC > 5)
		return ERROR_TIME_OUT;
	if (data->currPassLen == MAX_PASS_LEN)
		return CONTINUE;
	return BREAK;
};


static InPasswordDat data = { 0 };
State InPassword = { "IN PSSWD",inPsswdLoop,TableInPassword, &data };

