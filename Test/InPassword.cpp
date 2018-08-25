#include "States.h"
#include "Keyboard.h"
#include <stdio.h>
#include <string.h>
static event  InPsswd_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	char c = readKb();
	if (data->currPassLen < MAX_PASS_LEN)
	{
		data->currPsswd[data->currPassLen] = c;
		data->currPassLen++;
	}
	else
		return ERROR_WRONG_PASS;
	return BREAK;
}
static event InPsswd_Open(State* thi, event ev, void* uData)
{
	return OPEN;
}
static const Transition TableInPassword[] = {
	{ KEY_PRESS,InPsswd_addKey,&InPassword },
	{ CONTINUE,InPsswd_Open,&Open },
	{ ERROR_WRONG_PASS,NULL,&Idle },
	{ ERROR_WRONG_ID,NULL,&Idle },
	{ ERROR_TIME_OUT,NULL,&Idle },
	{ END_OF_TABLE,NULL,&InPassword }
};

static const char keys[254][2][MAX_ID_LEN+1] = {	
	{ "39868596","1996" },
	{ "12345678","1234" }
};
static event inPsswdLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	if (data->currPassLen == MAX_PASS_LEN)
	{
		unsigned char i = 0;
		while (keys[i][0][0] != 0)
		{
			if (!strcmp(keys[i][0], data->currID))
			{
				if (!strcmp(keys[i][1], data->currPsswd))
					return CONTINUE;
				else
					return ERROR_WRONG_PASS;
			}
			i++;
		}
		return ERROR_WRONG_ID;
	}
	return BREAK;
};




State InPassword = { "IN PSSWD",inPsswdLoop,TableInPassword };

