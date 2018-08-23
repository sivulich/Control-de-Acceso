#pragma once
typedef struct State State;
typedef struct Transition Transition;
typedef enum { END_OF_TABLE, NO_EVENT, KEY_PRESS, PASSWORD, CARD_SLIDE, ERROR_TIME_OUT, ERROR_WRONG_PASS,ERROR_WRONG_ID, CONTINUE } event;

struct State {
	const char* name;
	event(*loop)(State* thi, void* uData);
	Transition* transitions;
};
struct Transition {
	event ev;
	void(*tranFunc)(State* thi, event ev, void* uData);
	State* next;
};

State* mainLoop(event ev, State* curr, void* uData)
{
	if (ev == NO_EVENT)
		return curr;
	for (int i = 0; curr->transitions[i].ev != END_OF_TABLE; i++)
		if (curr->transitions[i].ev != END_OF_TABLE && ev == curr->transitions[i].ev)
		{
			if (curr->transitions[i].tranFunc != NULL)
				curr->transitions[i].tranFunc(curr, ev, uData);
			return curr->transitions[i].next;
		}
	return mainLoop(curr->loop(curr, uData), curr, uData);
}
