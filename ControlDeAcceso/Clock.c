#include "Clock.h"
#include "SysTick.h"
static int init=0;
static clock_t cl=0;

void clockPISR()
{
	//Increase the clock
	cl+=1;
}
int clockInit()
{
	if(init==0)
		if(SysTickInit()==1) //Init SysTick
			if(SysTickAddFunc(clockPISR)==1) //Add function to SysTick
				init=1;
	return init;
}

clock_t getTime()
{
	return cl;
}

