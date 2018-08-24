#include "States.h"
#include <stdio.h>

static event  InPsswd_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	//++++++++++++++ Temporal despues tiene que llamar al driver
	getchar();
	char c = getchar();
	//--------------
	if (data->currPassLen < MAX_PASS_LEN)
	{
		data->currPsswd[data->currPassLen] = c;
		data->currPassLen++;
	}
	else
		return ERROR_WRONG_PASS;
	return NO_EVENT;
}

static const Transition TableInPassword[] = {
	{ KEY_PRESS,InPsswd_addKey,&InPassword },
	{ CONTINUE,NULL,&Open },
	{ ERROR_WRONG_PASS,NULL,&Idle },
	{ ERROR_WRONG_ID,NULL,&Idle },
	{ ERROR_TIME_OUT,NULL,&Idle },
	{ END_OF_TABLE,NULL,&InPassword }
};

static event inPsswdLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	printf("%s\n", data->currID);
	return NO_EVENT;
};
State InPassword = { "IN PSSWD",inPsswdLoop,TableInPassword };

