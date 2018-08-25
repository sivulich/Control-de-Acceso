#include <stdlib.h>
#include "States.h"
#include "Keyboard.h"

static event Idle_InID(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	char c = readKb();
	data->currID[0] = c;
	data->currIDlen++;
	return BREAK;
}

static const Transition TableIdle[] = {
	{ CARD_SLIDE,NULL,&InPassword },//Idle_InPassword
	{ KEY_PRESS,Idle_InID,&InID },      //Idle_InID
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