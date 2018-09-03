#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H
//Display DEFINES
#define MAX_BRIGHTNESS 9

/*Init display*/
int dispInit();

/*Prints character c to pos in the displays*/
void print(const char c, int pos);

/*Sets the display brightness*/
void setBrightness(int c);

#endif // !DISPLAY_H
