#pragma once
#include "FSM.h"
#include <stdlib.h>
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;

Transition TableOpen[] = {
	{ CONTINUE,NULL,&Idle },
	{ END_OF_TABLE,NULL,&Open }
};
event openLoop(State* thi, void* uData) {
	return NO_EVENT;
};

State Open = { "OPEN",openLoop,TableOpen };