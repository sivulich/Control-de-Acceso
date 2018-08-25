#include "FSM.h"
State* mainLoop(event ev, State* curr, void* uData)
{
	int i = 0;
	if (ev == BREAK)
		return curr;
	for (; curr->transitions[i].ev != END_OF_TABLE; i++)
	if (curr->transitions[i].ev != END_OF_TABLE && ev == curr->transitions[i].ev)
	{
		if (curr->transitions[i].tranFunc != NULL)
		{
			ev = curr->transitions[i].tranFunc(curr, ev, uData);
			if (ev != BREAK)
				return mainLoop(ev, curr->transitions[i].next, uData);
		}
		return curr->transitions[i].next;
	}
	return mainLoop(curr->loop(curr, uData), curr, uData);
}