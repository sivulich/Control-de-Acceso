#pragma once
#ifndef STATES_H
#define STATES_H
#include "FSM.h"
#include "AppData.h"
//Sacar
#include <stdio.h>
enum { KEY_PRESS = EVENT_BASE, OPEN, CARD_SWIPE, ERROR_TIME_OUT, ERROR_WRONG_PASS, ERROR_WRONG_ID, CONTINUE };
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;
#endif // !STATES_H

