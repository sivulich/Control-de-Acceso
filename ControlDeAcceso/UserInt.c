#include "UserInt.h"
#include "Clock.h"
#include "Display.h"
#include "Leds.h"
#ifdef _WIN32
#include <stdio.h>
#endif
/*User interface satitc vars*/
static clock_t timer = 0,ledTimer=0;
static State* lastState=0;

int uiInit()
{
	/*Init User interface*/
	if(clockInit()==1)
		if(dispInit()==1)
			if(ledsInit()==1)
				return 1;
	return 0;

}
void uiLoop(State* curr, event ev, AppData* data) {
#ifndef _WIN32 
	/*If running on kintetis use seven segment displays and leds*/
	if(curr !=&Open && ledTimer!=0)
		ledTimer = 0;
	if (curr == &Open)
	{
		if(ledTimer==0)
			ledsOff();
		print('O', 3);
		print('P', 2);
		print('E', 1);
		print('N', 0);
		ledsGreen(1);
		if(ledTimer==0)
		{
			ledTimer=getTime();
			ledsBoard(1);
		}

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
		print('0'+data->currPassLen, 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		if(timer!=0)
			timer=0;
		for (unsigned  i = (data->currPassLen ) < 3 ? 0 : (data->currPassLen - 3); i < data->currPassLen; i++)
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
	else if (curr == &Admin)
	{
		ledsOff();
		if(timer!=0)
			timer=0;
		if(data->auData.state==0)
		{
			print('S', 3);
			print('u', 2);
			print('d', 1);
			print('o', 0);
		}
		else if(data->auData.state==1)
		{
			print('P', 3);
			print('A', 2);
			print('S', 1);
			print('S', 0);
		}
		else if(data->auData.state==2)
		{
			print('U', 3);
			print('S', 2);
			print('E', 1);
			print('r', 0);
		}
		else if(data->auData.state==3)
		{
			int pos=0;
			print(' ', 3);
			print(' ', 2);
			print(' ', 1);
			print(' ', 0);
			for (unsigned i = (data->auData.newIDlen ) < 4 ? 0 : (data->auData.newIDlen - 4); i < data->auData.newIDlen; i++)
			{
				pos=data->auData.newIDlen-i-1;
				print(data->auData.newID[i], pos);
			}
		}
		else if(data->auData.state==5)
		{
			int pos=0;
			print('0'+data->auData.newPassLen, 3);
			print(' ', 2);
			print(' ', 1);
			print(' ', 0);
			for (unsigned  i = (data->auData.newPassLen ) < 3 ? 0 : (data->auData.newPassLen - 3); i < data->auData.newPassLen; i++)
			{
				pos=data->auData.newPassLen-i-1;
				print('*', pos);
			}
		}
		ledsBlue(1);
		ledsRed(1);
	}

	if(ledTimer!=0 &&((double)getTime() - ledTimer) / CLOCKS_PER_SEC > 0.5)
	{
		if(ledTimer%1000<500)
			ledsBoard(0);
		else
			ledsBoard(1);
		ledTimer=getTime();
	}
	if (timer!=0 && ((double)getTime() - timer) / CLOCKS_PER_SEC > 2)
	{
		print(' ', 3);
		print(' ', 2);
		print(' ', 1);
		print(' ', 0);
		ledsOff();
		timer = 0;
	}
#else
	/*If running on windows use console*/
	if (timer == 0)
		timer = getTime();
	if ((double)(getTime() - timer) / CLOCKS_PER_SEC > 0.1)
	{
		system("cls");
		printf("CurrentState= %s\nCurrent ID= %s\nCurrent Password= %s\n", curr->name, data->currID, data->currPsswd);
		timer = getTime();
	}
	
#endif
	lastState = curr;
}
