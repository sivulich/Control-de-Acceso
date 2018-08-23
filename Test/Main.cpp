#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FSM.h"

State Idle, InID, InPassword, Open;

Transition TableIdle[] = {
	{CARD_SLIDE,NULL,&InPassword}, //Idle_InPassword
	{KEY_PRESS,NULL,&InID}, //Idle_InID
	//Falta agregar modo admin
	{END_OF_TABLE,NULL,&Idle}
};

Transition TableInID[] = {
	{KEY_PRESS,NULL,&InID}, //InID_addKey
	{CONTINUE,NULL,&InPassword}, //InID_InPassword
	{CARD_SLIDE,NULL,&Idle}, //error
	{ERROR_WRONG_ID,NULL,&Idle},
	{ ERROR_TIME_OUT,NULL,&Idle },
	{END_OF_TABLE,NULL,&InID}
};

Transition TableInPassword[] = {
	{KEY_PRESS,NULL,&InPassword},
	{CONTINUE,NULL,&Open},
	{ERROR_WRONG_PASS,NULL,&Idle},
	{ERROR_WRONG_ID,NULL,&Idle},
	{ERROR_TIME_OUT,NULL,&Idle},
	{ END_OF_TABLE,NULL,&InPassword }
};

Transition TableOpen[] = {
	{ KEY_PRESS,NULL,&Idle },
	{ END_OF_TABLE,NULL,&Open }
};
event loop(State* thi, void* uData) {
	//printf("%s\n", thi->name);
	return NO_EVENT;
}
int main(void)
{
	Idle = { "IDLE", loop, TableIdle};
	InID = { "IN ID",loop,TableInID };
	InPassword = { "IN PSSWD",loop,TableInPassword };
	Open = { "OPEN",loop,TableOpen };
	State* curr = &Idle;
	int ev;
	for (int i = 0; ; i++)
	{
		scanf("%d", &ev);
		curr = mainLoop((event)ev, curr, NULL);
		printf("%s\n", curr->name);
	}
		
	return 0;
}
