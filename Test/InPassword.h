#pragma once
#include "FSM.h"
#include <stdlib.h>
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;


Transition TableInPassword[] = {
	{ KEY_PRESS,NULL,&InPassword },
	{ CONTINUE,NULL,&Open },
	{ ERROR_WRONG_PASS,NULL,&Idle },
	{ ERROR_WRONG_ID,NULL,&Idle },
	{ ERROR_TIME_OUT,NULL,&Idle },
	{ END_OF_TABLE,NULL,&InPassword }
};

event inPsswdLoop(State* thi, void* uData) {
	return NO_EVENT;
};
State InPassword = { "IN PSSWD",inPsswdLoop,TableInPassword };