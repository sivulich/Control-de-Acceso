#include "UI.h"
#include "Clock.h"
static clock_t timer = 0;
static State* lastState=0;
void uiLoop(State* curr, event ev, AppData* data) {
	if (curr == &Open)
	{
		print('O', 3);
		print('P', 2);
		print('E', 1);
		
		print('N', 0);
		//temp
		print('\n', 0);
	}
	else if (timer>0||(lastState == &InPassword && curr==&Idle) || (lastState == &InID && curr == &Idle))
	{
		print('E', 3);
		print('R', 2);
		print('R', 1);
		if(timer==0)
			timer = getTime();
		//temp
		print('\n', 3);
	}
	else if (curr == &InID)
	{
		for (unsigned i = (data->currIDlen ) < 4 ? 0 : (data->currIDlen - 4); i < data->currIDlen; i++)
			print(data->currID[i], 0);
		//temp
		print('\n', 3);
	}
	else if (curr == &InPassword)
	{
		for (unsigned  i = (data->currPassLen ) < 4 ? 0 : (data->currPassLen - 4); i < data->currPassLen; i++)
			print('*', 1);
		//temp
		print('\n', 3);
	}
	if (timer!=0 && ((double)getTime() - timer) / CLOCKS_PER_SEC > 2)
	{
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		//temp
		print('\n', 0);
		timer = 0;
	}
	lastState = curr;
	//temp
	print('\n', 0);
}
