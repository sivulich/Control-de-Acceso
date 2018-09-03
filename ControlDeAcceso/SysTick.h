#ifndef _SYSTICK_H_
#define _SYSTICK_H_


#define SYSTICK_ISR_FREQUENCY_HZ 100U

/*Initialize systick*/
int SysTickInit ();

/*Add a function to be called during the systick call*/
int SysTickAddFunc(void (*funcallback)(void));

#endif // _SYSTICK_H_
