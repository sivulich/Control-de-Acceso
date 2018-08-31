
#include "Leds.h"
#include "gpio.h"
#include "MuxKbDs.h"
int ledsInit()
{
	pinMode(PIN_LED_BLUE,OUTPUT);
	pinMode(PIN_LED_RED,OUTPUT);
	pinMode(PIN_LED_GREEN,OUTPUT);
	ledsOff();
	return 1;
}
void ledsOff()
{
#ifndef _WIN32 
	ledsBlue(0);
	ledsRed(0);
	ledsGreen(0);
	ledsBoard(0);
#endif
}
void ledsBlue(int v)
{
#ifndef _WIN32 
	digitalWrite(PIN_LED_BLUE,!(v&&1));
#endif
}
void ledsRed(int v)
{
#ifndef _WIN32 
	digitalWrite(PIN_LED_RED,!(v&&1));
#endif
}
void ledsGreen(int v)
{
#ifndef _WIN32 
	digitalWrite(PIN_LED_GREEN,!(v&&1));
#endif
}
void ledsBoard(int v)
{
#ifndef _WIN32 
	muxKbDsLed(v);
#endif
}

