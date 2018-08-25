#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "States.h"
#include "Keyboard.h"
#include <conio.h>
int main(void)
{
	
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
		if (_kbhit())
		{
			ev = KEY_PRESS;
			del = 1;
		}
		else
			ev = NO_EVENT;
		curr = mainLoop((event)ev, curr, (void* )&data);
		if (del == 1)
		{
			del = 0;
			kbFlush();
		}
		if (((double)clock() - c) / CLOCKS_PER_SEC > 0.2)
		{
			system("cls");
			printf("CurrentState= %s\nCurrent ID= %s\nCurrent Password= %s\n\n", curr->name, data.currID, data.currPsswd);
			c = clock();
		}
	}
		
	return 0;
}
