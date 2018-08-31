#pragma once

#ifndef USERINT_H
#define USERINT_H
#include "AppData.h"
#include "States.h"
int uiInit();
void uiLoop(State* curr, event ev, AppData* data);
#endif // !USERINT_H
