#pragma once

#ifndef MUXKBDS_H
#define MUXKBDS_H
/*Hardware defines*/
//Number of displays
#define DISPLAYS 4
//Number of rows in KB
#define ROWS 4
//Number of columns in KB
#define COLUMNS 3

/*Init the driver to control the hardware*/
int muxKbDsInit();

/*Write bits of ds to display in pos*/
//The bit convention is ds=|p|a|b|c|d|e|f|g|
void muxKbDsWriteDisplay(unsigned char ds, unsigned int pos);

/*Writes to kbs the current keyboard state 1 corresponds to a pressed key, 0 corresponds to a 0*/
void muxKbDsGetKbState(unsigned char (*kbS)[COLUMNS]);

/*Writes to the led on the board*/
void muxKbDsLed(int);

/*Sets the seven segment displays brightness*/
//The number c represents that 1 over c cycles the displays will be on
void muxKbDsSetBrightness(unsigned c);


#endif // !MUXKBDS_H

