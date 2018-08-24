#pragma once
#include <stdlib.h>
typedef struct State State;
typedef struct Transition Transition;
typedef int event;
enum { END_OF_TABLE, NO_EVENT,EVENT_BASE};

struct State {
	const char* name;
	event(*loop)(State* thi, void* uData);
	const Transition* transitions;
	void* stateData;
};
struct Transition {
	event ev;
	event(*tranFunc)(State* thi, event ev, void* uData);
	State* next;
};

State* mainLoop(event ev, State* curr, void* uData);
