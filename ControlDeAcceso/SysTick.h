#ifndef _SYSTICK_H_
#define _SYSTICK_H_


#define SYSTICK_ISR_FREQUENCY_HZ 100U


int SysTickInit ();

int SysTickAddFunc(void (*funcallback)(void));

#endif // _SYSTICK_H_
