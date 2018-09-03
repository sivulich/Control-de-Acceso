#pragma once
#ifndef FSM_H
#define FSM_H
#include <stdlib.h>


/*Data type definitions*/
typedef struct State State;
typedef struct Transition Transition;
typedef int event;

/*FSM layer events*/
enum { NO_EVENT, END_OF_TABLE, BREAK, EVENT_BASE };

/*State structure*/
struct State {
	//State name
	const char* name;
	//Function to be called if NO_EVENT ocurs o no specific event handler is defined
	event(*loop)(State* thi, void* uData);
	//Transitions of this state
	const Transition* transitions;
	//State specific data
	void* stateData;
};

/*Transition structure*/
struct Transition {
	//Event on which this transition ocurrs
	event ev;
	//Function to be executed when this event ocurrs
	event(*tranFunc)(State* thi, event ev, void* uData);
	//Next state
	State* next;
};
//Main loop of a state, to be called if NO_EVENT ocurs o no specific event handler is defined
State* mainLoop(event ev, State* curr, void* uData);
#endif // !FSM_H

