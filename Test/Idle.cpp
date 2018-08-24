#include <stdlib.h>
#include "States.h"

static event Idle_InPassword(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	return NO_EVENT;
}

static const Transition TableIdle[] = {
	{ CARD_SLIDE,NULL,&InPassword },//Idle_InPassword
	{ KEY_PRESS,NULL,&InID },      //Idle_InID
							   //Falta agregar modo admin
	{ END_OF_TABLE,NULL, &Idle }
};
static event idleLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	if (data->currIDlen > 0 || data->currPassLen > 0)
	{
		data->currIDlen = 0;
		data->currPassLen = 0;
		for (int i = 0; i < MAX_ID_LEN; i++)
			data->currID[i] = 0;
		for (int i = 0; i < MAX_PASS_LEN; i++)
			data->currPsswd[i] = 0;
	}

	return NO_EVENT;
};

State Idle = { "IDLE", idleLoop, TableIdle };