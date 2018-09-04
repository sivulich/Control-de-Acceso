#include "FSM.h"
State* mainLoop(event ev, State* curr, void* uData)
{
	int i = 0;
	//Stop processing
	if (ev == BREAK)
		return curr;
	for (; curr->transitions[i].ev != END_OF_TABLE; i++)
		//If theres a transition defined for the event
		if (curr->transitions[i].ev != END_OF_TABLE && ev == curr->transitions[i].ev)
		{
			if (curr->transitions[i].tranFunc != NULL)
			{
				//Get the event of the transition
				ev = curr->transitions[i].tranFunc(curr, ev, uData);
				if (ev != BREAK)
					//Returns the event of the next state with this new event
					return mainLoop(ev, curr->transitions[i].next, uData);
			}
			//If no transition or event break, return next
			return curr->transitions[i].next;
		}
	//If ev is not break continue in the FSM
	return mainLoop(curr->loop(curr, uData), curr, uData);
}