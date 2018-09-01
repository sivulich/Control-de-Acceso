#include <stdlib.h>
#include "States.h"
#include "Keyboard.h"
#include "Display.h"
enum{BRIGHTNESS=PROP_EVENT_BASE,ADD_USER,DELETE_USER,CHANGE_PASS,END};
typedef struct string String;
static event(*func)(void* uData)=NULL;
static event brightness(void* uData)
{
	char c;
	if (kbHit())
	{
		c = readKb();
		setBrightness(c - '0');
		return END;
	}
	return BREAK;

}
event adminReset(State* thi, event ev, void* uData)
{
	func = NULL;
	return BREAK;
}
static event adminBrightness(State* thi, event ev, void* uData)
{
	if (func == NULL)
		func = brightness;
	return BREAK;
}
static const Transition TableAdmin[] = {
	{ CONTINUE, NULL,&Idle },
	{ BRIGHTNESS, adminBrightness,&Admin},
	{ END,adminReset,&Idle},
	{ END_OF_TABLE,NULL, &Idle }
};
static event adminLoop(State* thi, void* uData)
{
	if (func != NULL)
		return func(uData);
	char c;
	if (kbHit())
	{
		c = readKb();
		if (c == '1')
			return BRIGHTNESS;
		else if (c == '#')
			return CONTINUE;
	}
	return BREAK;
}
State Admin = { "ADMIN", adminLoop, TableAdmin };