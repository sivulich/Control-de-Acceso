#pragma once

#define DISPLAYS 4
#define ROWS 4
#define COLUMNS 3
//Sacar esto
void muxKbDsPISR();

int muxKbDsInit();

void muxKbDsWriteDisplay(unsigned char ds, unsigned int pos);

void muxKbDsGetKbState(unsigned char** kbState);
