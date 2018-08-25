#include <stdlib.h>
#include "States.h"
#include <stdio.h>
#include "Clock.h"
/*static void open_wait();*/
typedef struct {
	clock_t c;
} dat;
static event Open_ResetTimer(State* thi, event ev, void* uData)
{
	dat* mydat = (dat*)thi->stateData;
	mydat->c = getTime();
	return BREAK;
}
static const Transition TableOpen[] = {
	{ CONTINUE,NULL,&Idle },
	{ OPEN,Open_ResetTimer,&Open },
	{ END_OF_TABLE,NULL,&Open }
};
static event openLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	dat* mydat = (dat*) thi->stateData;
	if (((double)getTime() - mydat->c) / CLOCKS_PER_SEC > 5)
		return CONTINUE;
	return BREAK;
};
dat data;
State Open = { "OPEN",openLoop,TableOpen,(void*)&data };