#pragma once
#include "FSM.h"
#include <stdlib.h>
extern State InID;
extern State InPassword;
extern State Idle;


Transition TableIdle[] = {
	{ CARD_SLIDE,NULL,&InPassword }, //Idle_InPassword
	{ KEY_PRESS,NULL,&InID }, //Idle_InID
						  //Falta agregar modo admin
	{ END_OF_TABLE,NULL, &Idle}
};
event idleLoop(State* thi, void* uData) {
	return NO_EVENT;
};

State Idle = { "IDLE", idleLoop, TableIdle };