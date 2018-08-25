#include "States.h"
#include "Keyboard.h"

static event  InID_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	char c = readKb();
	if (data->currIDlen < MAX_ID_LEN)
	{
		data->currID[data->currIDlen] = c;
		data->currIDlen++;
		if (data->currIDlen == MAX_ID_LEN)
			return CONTINUE;
		return BREAK;
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
	return BREAK;
};

State InID = { "IN ID",inIDLoop,TableInID };