/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "States.h"
#include "EventGenerator.h"
#include "Clock.h"
#include "UserInt.h"
#include "Keyboard.h"
#include "Mcr.h"
#include <stdint.h>


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	clockInit();
	uiInit();
	kBInit();
	mcrInit();
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	State* curr = &Idle;
	AppData data;
	data.currIDlen = 0;
	data.currPassLen = 0;
	for (int i = 0; i < MAX_ID_LEN+1; i++)
		data.currID[i] = 0;
	for (int i = 0; i < MAX_PASS_LEN+1; i++)
		data.currPsswd[i] = 0;
	int ev,del=0;
	clock_t c=getTime();
	for (int i = 0; ; i++)
	{
		ev = getEvent();
		if (ev == KEY_PRESS || ev==CARD_SWIPE)
			del = 1;
		curr = mainLoop((event)ev, curr, (void* )&data);
		if (del == 1)
		{
			del = 0;
			kbFlush();
			mcrFlush();
		}
		if (((double)getTime() - c) / CLOCKS_PER_SEC > 0.2)
		{
			uiLoop(curr, ev, &data);
			c = getTime();
		}
	}
}


