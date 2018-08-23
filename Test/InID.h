#pragma once
#include "FSM.h"
#include <stdlib.h>
extern State InID;
extern State InPassword;
extern State Idle;

Transition TableInID[] = {
	{ KEY_PRESS,NULL,&InID }, //InID_addKey
	{ CONTINUE,NULL,&InPassword }, //InID_InPassword
	{ CARD_SLIDE,NULL,&Idle }, //error
	{ ERROR_WRONG_ID,NULL,&Idle },
	{ ERROR_TIME_OUT,NULL,&Idle },
	{ END_OF_TABLE,NULL,&InID }
};
event inIDLoop(State* thi, void* uData) {
	return NO_EVENT;
};

State InID = { "IN ID",inIDLoop,TableInID };