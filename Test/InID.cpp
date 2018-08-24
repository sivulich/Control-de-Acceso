#include "States.h"
//Sacar esto despues
#include <stdio.h>
#include <string.h>

static event  InID_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	//++++++++++++++ Temporal despues tiene que llamar al driver
	getchar();
	char c = getchar();
	//--------------
	if (data->currIDlen < MAX_ID_LEN)
	{
		data->currID[data->currIDlen] = c;
		data->currIDlen++;
		if (data->currIDlen == MAX_ID_LEN)
			return CONTINUE;
		return NO_EVENT;
	}
	else
		return ERROR_WRONG_ID;
}

static const Transition TableInID[] = {
	{ KEY_PRESS,InID_addKey,&InID }, //InID_addKey
	{ CONTINUE,NULL,&InPassword }, //InID_InPassword
	{ CARD_SLIDE,NULL,&Idle }, //error
	{ ERROR_WRONG_ID,NULL,&Idle },
	{ ERROR_TIME_OUT,NULL,&Idle },
	{ END_OF_TABLE,NULL,&InID }
};

static event inIDLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	if (data->currIDlen == MAX_ID_LEN)
		return CONTINUE;
	return NO_EVENT;
};

State InID = { "IN ID",inIDLoop,TableInID };