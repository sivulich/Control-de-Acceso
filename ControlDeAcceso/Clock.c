#include "Clock.h"
#include "SysTick.h"
static int init=0;
static clock_t cl=0;

void clockPISR()
{
	cl+=1;
}
int clockInit()
{
	if(init==0)
		if(SysTickAddFunc(clockPISR)==1)
			init=1;
	return init;
}

clock_t getTime()
{
	return cl;
}

