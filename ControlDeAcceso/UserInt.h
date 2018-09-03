#pragma once

#ifndef USERINT_H
#define USERINT_H
#include "AppData.h"
#include "States.h"
/*Init the user interface*/
int uiInit();
/*This loop must be called on a peridic basis to update the display*/
void uiLoop(State* curr, event ev, AppData* data);

#endif // !USERINT_H
