#include "UserInt.h"
#include "Clock.h"
#include "Display.h"
#include "Leds.h"
static clock_t timer = 0;
static State* lastState=0;
int uiInit()
{
	if(clockInit()==1)
		if(dispInit()==1)
			if(ledsInit()==1)
				return 1;
	return 0;

}
void uiLoop(State* curr, event ev, AppData* data) {
#ifndef _WIN32 
	if (curr == &Open)
	{
		ledsOff();
		print('O', 3);
		print('P', 2);
		print('E', 1);
		print('N', 0);
		ledsGreen(1);
	}
	else if (timer>0||(lastState == &InPassword && curr==&Idle) || (lastState == &InID && curr == &Idle))
	{
		ledsOff();
		print('E', 3);
		print('r', 2);
		print('r', 1);
		print(' ', 0);
		if(timer==0)
			timer = getTime();
		ledsRed(1);
	}
	else if (curr == &InID)
	{
		ledsOff();
		int pos=0;
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		if(timer!=0)
			timer=0;
		for (unsigned i = (data->currIDlen ) < 4 ? 0 : (data->currIDlen - 4); i < data->currIDlen; i++)
		{
			pos=data->currIDlen-i-1;
			print(data->currID[i], pos);
		}
		ledsBlue(1);

	}
	else if (curr == &InPassword)
	{
		ledsOff();
		int pos=0;
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		if(timer!=0)
			timer=0;
		for (unsigned  i = (data->currPassLen ) < 4 ? 0 : (data->currPassLen - 4); i < data->currPassLen; i++)
		{
			pos=data->currPassLen-i-1;
			print('*', pos);
		}
		ledsBlue(1);
	}
	else if(curr == &Idle && timer==0)
	{
		ledsOff();
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		ledsBlue(1);
	}
	if (timer!=0 && ((double)getTime() - timer) / CLOCKS_PER_SEC > 2)
	{
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		timer = 0;
	}
#else
	system("cls");
	printf("CurrentState= %s\nCurrent ID= %s\nCurrent Password= %s\n", curr->name, data->currID, data->currPsswd);
#endif
	lastState = curr;
}
