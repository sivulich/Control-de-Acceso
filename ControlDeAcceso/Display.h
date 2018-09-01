#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H
#define MAX_BRIGHTNESS 9
int dispInit();
void print(const char c, int pos);
void setBrightness(int c);
#endif // !DISPLAY_H
