#include <stdlib.h>
#include <string.h>
#include "States.h"
#include "Keyboard.h"
#include "Mcr.h"

static event Idle_Check(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	char c = readKb();
	if (c <= '9' && '0' <= c)
	{
		data->currID[0] = c;
		data->currIDlen++;
		return CONTINUE;
	}
	
	return BREAK;
}
static event Idle_InPassword(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	getLastCard(data->currID);
	data->currIDlen = strlen(data->currID);
	return BREAK;
}

static const Transition TableIdle[] = {
	{ CARD_SWIPE,Idle_InPassword,&InPassword },     //Idle_InPassword
	{ KEY_PRESS,Idle_Check,&Idle },      //Idle_InID
	{ CONTINUE, NULL,&InID },
	//Falta agregar modo admin
	{ END_OF_TABLE,NULL, &Idle }
};
static event idleLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	if (data->currIDlen > 0 || data->currPassLen > 0)
	{
		data->currIDlen = 0;
		data->currPassLen = 0;
		for (int i = 0; i < MAX_ID_LEN+1; i++)
			data->currID[i] = 0;
		for (int i = 0; i < MAX_PASS_LEN+1; i++)
			data->currPsswd[i] = 0;
	}

	return BREAK;
};

State Idle = { "IDLE", idleLoop, TableIdle };
