#include "SysTick.h"
#ifndef _WIN32 
#include "hardware.h"
#else
#include <windows.h>
#include <time.h>
#endif // !_WIN32 

/*Max number of SysTick Functions*/
#define MAX 254

/*Static variables for systick*/
//Indicates if it's already initialized
static int init=0;
//Number of functions stored for the SysTick callback
static unsigned nFuncs=0;
//Functions to be called during the SysTick
static void (*callBacks[MAX])();

#ifdef _WIN32   
/*If current system is windows, the SysTick runs on a thread*/
static clock_t cl = 0;
DWORD WINAPI SysTick_Handler(void* data){
	for (;;)
	{
		if (cl == 0)
			cl = clock();
		/*Simulate the systick calling the thread every 1ms*/
		if (((double)(clock() - cl)) / CLOCKS_PER_SEC > 0.001)
		{
			cl = clock();
#else
__ISR__  SysTick_Handler(void) {
#endif

	/*Call the SysTick callbacks*/
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
		/*If program is runing on kinetis set up the SysTick*/
		SysTick->CTRL = 0x00;
		SysTick->LOAD = 100000L - 1; //199999L; // <= 1 ms @ 100Mhz
		SysTick->VAL = 0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
#else
		/*If running on windows create the SysTick thread*/
		HANDLE thread = CreateThread(NULL, 0, SysTick_Handler, NULL, 0, NULL);
		if (thread == NULL)
			return 0;
#endif // !_WIN32 

		init=1;
	}
	return init;
}
int SysTickAddFunc(void (*funcallback)(void))
{
	if(nFuncs<MAX)
	{
		/*Add the callback to the callback array*/
		callBacks[nFuncs++]=funcallback;
		return 1;
	}
	return 0;
}





