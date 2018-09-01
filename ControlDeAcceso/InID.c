#include "States.h"
#include "Keyboard.h"
#include "Clock.h"
typedef struct {
	clock_t c;
} InIDdat;
static event  InID_addKey(State* thi, event ev, void* uData)
{
	AppData* data = (AppData*)uData;
	InIDdat* myDat = (InIDdat*)thi->stateData;
	char c = readKb();
	if (c == '*' && data->currIDlen > 0)
	{
		myDat->c = getTime();
		data->currIDlen--;
		data->currID[data->currIDlen] = 0;
		return BREAK;
	}
	else if (c != '*'&&c!='#'  && data->currIDlen < MAX_ID_LEN)
	{
		myDat->c = getTime();
		data->currID[data->currIDlen] = c;
		data->currIDlen++;
		if (data->currIDlen == MAX_ID_LEN)
			return CONTINUE;
		return BREAK;
	}
	else if (c == '#' && data->currIDlen > 0)
		return CONTINUE;
	else
		return ERROR_WRONG_ID;
}

static event InID_Reset(State* thi, event ev, void* uData)
{
	InIDdat* myDat = (InIDdat*)thi->stateData;
	myDat->c = 0;
	return BREAK;
}

static const Transition TableInID[] = {
	{ KEY_PRESS,InID_addKey,&InID }, //InID_addKey
	{ CONTINUE,InID_Reset,&InPassword }, //InID_InPassword
	{ CARD_SWIPE,InID_Reset,&Idle }, //error
	{ ERROR_WRONG_ID,InID_Reset,&Idle },
	{ ERROR_TIME_OUT,InID_Reset,&Idle },
	{ END_OF_TABLE,NULL,&InID }
};

static event inIDLoop(State* thi, void* uData) {
	AppData* data = (AppData*)uData;
	InIDdat* myDat = (InIDdat*)thi->stateData;
	if (myDat->c == 0)
		myDat->c = getTime();
	if (((double)getTime() - myDat->c) / CLOCKS_PER_SEC > 5)
		return ERROR_TIME_OUT;
	if (data->currIDlen == MAX_ID_LEN)
		return CONTINUE;
	return BREAK;
};
static InIDdat data = {0};
State InID = { "IN ID",inIDLoop,TableInID,&data };