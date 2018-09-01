#pragma once

#ifndef MUXKBDS_H
#define MUXKBDS_H
#define DISPLAYS 4
#define ROWS 4
#define COLUMNS 3
void muxKbDsPISR();

int muxKbDsInit();

void muxKbDsWriteDisplay(unsigned char ds, unsigned int pos);

void muxKbDsGetKbState(unsigned char (*kbS)[COLUMNS]);

void muxKbDsLed(int);

void muxKbDsSetBrightness(unsigned c);
#endif // !MUXKBDS_H

