/*
 * SysTick.c
 *
 *  Created on: 17 ago. 2018
 *      Author: Crakus
 */
#include "SysTick.h"
#ifndef _WIN32 
#include "hardware.h"
#else
#include <windows.h>
#include <time.h>
#endif // !_WIN32 
#define MAX 255
int init=0;
static unsigned nFuncs=0;
static void (*callBacks[MAX])();

#ifdef _WIN32   
static clock_t cl = 0;
DWORD WINAPI SysTick_Handler(void* data){
	for (;;)
	{
		if (cl == 0)
			cl = clock();
		if (((double)(clock() - cl)) / CLOCKS_PER_SEC > 0.001)
		{
			cl = clock();
#else
__ISR__  SysTick_Handler(void) {
#endif


	for (unsigned i = 0; i < nFuncs; i++)
		callBacks[i]();
#ifdef _WIN32 
		}
	}
#endif
}



int SysTickInit ()
{
	if(init==0)
	{
#ifndef _WIN32 
		SysTick->CTRL = 0x00;
		SysTick->LOAD = 100000L - 1; //199999L; // <= 1 ms @ 100Mhz
		SysTick->VAL = 0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
#else
		HANDLE thread = CreateThread(NULL, 0, SysTick_Handler, NULL, 0, NULL);
#endif // !_WIN32 
		init=1;
	}
	return init;
}
int SysTickAddFunc(void (*funcallback)(void))
{
	if(nFuncs<MAX)
	{
		callBacks[nFuncs]=funcallback;
		nFuncs++;
		return 1;
	}
	return 0;
}





