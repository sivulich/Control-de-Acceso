#pragma once
#ifndef STATES_H
#define STATES_H
#include "FSM.h"
#include "AppData.h"
enum { KEY_PRESS = EVENT_BASE, OPEN, CARD_SWIPE,ADMIN, ERROR_TIME_OUT, ERROR_WRONG_PASS, ERROR_WRONG_ID, CONTINUE,PROP_EVENT_BASE };
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;
extern State Admin;

#endif // !STATES_H

