#include "Keyboard.h"
#include <conio.h>  
#include <stdio.h>  
char readKb(void)
{
	if (_kbhit())
		return _getch();
	return 0;
}
int kbHit()
{
	return _kbhit();
}
void kbFlush()
{
	while (_kbhit())
		_getch();
}