#pragma once
#ifndef STATES_H
#define STATES_H
#include "FSM.h"
#include "AppData.h"
/*Define general events for every state*/
enum { KEY_PRESS = EVENT_BASE, OPEN, CARD_SWIPE,ADMIN, ERROR_TIME_OUT, ERROR_WRONG_PASS, ERROR_WRONG_ID, CONTINUE,PROP_EVENT_BASE };

/*Pre declaration of every state*/
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;
extern State Admin;

#endif // !STATES_H

