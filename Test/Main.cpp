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
#include "Idle.h"
#include "InID.h"
#include "InPassword.h"
#include "Open.h"


int main(void)
{
	
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
