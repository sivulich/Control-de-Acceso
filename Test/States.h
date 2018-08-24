#pragma once
#include "FSM.h"
#include "AppData.h"
enum{  KEY_PRESS=EVENT_BASE, PASSWORD, CARD_SLIDE, ERROR_TIME_OUT, ERROR_WRONG_PASS, ERROR_WRONG_ID, CONTINUE };
extern State InID;
extern State InPassword;
extern State Idle;
extern State Open;
