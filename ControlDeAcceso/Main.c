#include "States.h"
#include "EventGenerator.h"
#include "Clock.h"
#include "UserInt.h"
#include "MuxKbDs.h"
int main(void)
{
	muxKbDsInit();
	muxKbDsInit();
	muxKbDsPISR();
	getchar();
	State* curr = &Idle;
	AppData data;
	data.currIDlen = 0;
	data.currPassLen = 0;
	for (int i = 0; i < MAX_ID_LEN+1; i++)
		data.currID[i] = 0;
	for (int i = 0; i < MAX_PASS_LEN+1; i++)
		data.currPsswd[i] = 0;
	int ev,del=0;
	clock_t c=clock();
	for (int i = 0; ; i++)
	{
		ev = getEvent();
		if (ev == KEY_PRESS)
			del = 1;
		curr = mainLoop((event)ev, curr, (void* )&data);
		if (del == 1)
		{
			del = 0;
			kbFlush();
		}
		if (((double)getTime() - c) / CLOCKS_PER_SEC > 0.2)
		{
			system("cls");
			printf("CurrentState= %s\nCurrent ID= %s\nCurrent Password= %s\nDisplay 7 seg:", curr->name, data.currID, data.currPsswd);
			uiLoop(curr, ev, &data);
			c = clock();
			
		}
	}
		
	return 0;
}