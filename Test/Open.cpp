#include <stdlib.h>
#include "States.h"
#include <stdio.h>

/*static void open_wait();*/

static const Transition TableOpen[] = {
	{ CONTINUE,NULL,&Idle },
	{ END_OF_TABLE,NULL,&Open }
};
static event openLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	printf("%s\n", data->currPsswd);
	return NO_EVENT;
};

State Open = { "OPEN",openLoop,TableOpen };