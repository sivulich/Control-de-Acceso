#pragma once
#include "MuxKbDs.h"
#include "Gpio.h"
#define COL_0 0
#define COL_1 1
#define COL_2 2
#define ROW_0 0
#define ROW_1 1
#define ROW_2 2
#define ROW_3 3
#define DISP_A 0
#define DISP_B 1
#define DISP_C 2
#define DISP_D 3
#define DISP_E 4
#define DISP_F 5
#define DISP_G 6
#define DISP_P 7
#define LED    8
static uint8_t sevenSegments[8] = { DISP_A,DISP_B,DISP_C,DISP_D,DISP_E, DISP_F, DISP_G, DISP_P };

static uint8_t rows[ROWS] = {   ROW_0,ROW_1, ROW_2, ROW_3 };
static uint8_t cols[COLUMNS] = {  COL_0,COL_1, COL_2 };

static unsigned char init = 0;

static unsigned int active = 0;

static unsigned char display[DISPLAYS];

static unsigned char kbState[ROWS][COLUMNS] = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };

int muxKbDsInit()
{
	if (init == 0)
	{
		for (unsigned i = 0; i < ROWS; i++)
			pinMode(rows[i], OUTPUT);
		for (unsigned i = 0; i < COLUMNS; i++)
			pinMode(cols[i], INPUT_PULLDOWN);
		for (unsigned i = 0; i < 8; i++)
			pinMode(sevenSegments[i], OUTPUT);
		pinMode(LED, OUTPUT);
		for (unsigned i = 0; i < ROWS; i++)
			digitalWrite(rows[i], 1);
		//Registrar PISR en SysTick

		init = 1;
	}
	return init;
}

void muxKbDsWriteDisplay(unsigned char ds,unsigned int pos)
{
	if (pos < DISPLAYS)
		display[pos] = ds;
}

void muxKbDsGetKbState(unsigned char** kbS) {
	for (unsigned i = 0; i < ROWS; i++)
		for (unsigned j = 0; j < COLUMNS; j++)
			kbS[i][j] = kbState[i][j];
}

void muxKbDsPISR()
{
	//Pensar si checkear los tics aca o en la funcion de systick
	digitalWrite(rows[active], 1);

	if (active < ROWS - 1)
		active++;
	else
		active = 0;
	unsigned i = active;
	digitalWrite(rows[active], 0);
	for (unsigned j = 0; j < COLUMNS; j++)
		kbState[i][j] = digitalRead(cols[j]);
	for (unsigned j = 0; j < 8; j++)
		digitalWrite(sevenSegments[j], (display[i] >> j) & 1);
}