#include "EventGenerator.h"

event getEvent()
{
	if (hasCardValue())
		return CARD_SWIPE;
	else if (kbHit())
		return KEY_PRESS;
	return NO_EVENT;
}