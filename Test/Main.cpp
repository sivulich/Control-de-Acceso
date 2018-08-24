#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "States.h"

int main(void)
{
	
	State* curr = &Idle;
	AppData data;
	data.currIDlen = 0;
	data.currPassLen = 0;
	for (int i = 0; i < MAX_ID_LEN; i++)
		data.currID[i] = 0;
	for (int i = 0; i < MAX_PASS_LEN; i++)
		data.currPsswd[i] = 0;
	int ev;
	for (int i = 0; ; i++)
	{
		scanf("%d", &ev);
		curr = mainLoop((event)ev, curr, (void* )&data);
		printf("%s\n", curr->name);
	}
		
	return 0;
}
