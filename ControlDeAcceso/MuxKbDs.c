#include "MuxKbDs.h"
#include "gpio.h"
#include "SysTick.h"

/*Set the pins used by the driver*/
#define COL_0 PORTNUM2PIN(PC,17)
#define COL_1 PORTNUM2PIN(PB,19)
#define COL_2 PORTNUM2PIN(PC,16)
#define ROW_0 PORTNUM2PIN(PA,1)
#define ROW_1 PORTNUM2PIN(PC,8)
#define ROW_2 PORTNUM2PIN(PB,9)
#define ROW_3 PORTNUM2PIN(PC,1)
#define DISP_A PORTNUM2PIN(PC,3)
#define DISP_B PORTNUM2PIN(PC,5)
#define DISP_C PORTNUM2PIN(PC,2)
#define DISP_D PORTNUM2PIN(PC,7)
#define DISP_E PORTNUM2PIN(PA,2)
#define DISP_F PORTNUM2PIN(PC,0)
#define DISP_G PORTNUM2PIN(PB,23)
#define DISP_P PORTNUM2PIN(PC,9)
#define LED    PORTNUM2PIN(PB,18)

/*Create a handy array for easy access to the pins*/
static uint8_t sevenSegments[8] = { DISP_G,DISP_F,DISP_E,DISP_D,DISP_C, DISP_B, DISP_A, DISP_P };
//Same for rows and columns
static uint8_t rows[ROWS] = {   ROW_0,ROW_1, ROW_2, ROW_3 };
static uint8_t cols[COLUMNS] = {  COL_0,COL_1, COL_2 };

/*Variables used by the driver*/
//If init == 1 the driver has initialized correctly
static unsigned char init = 0;
//Number of calls to the PISR function
static unsigned long long nCall=0;
//Active row
static unsigned int active = 0;

/*Variables used by the display*/
//Current brightness
static unsigned char brightness=1;
//Current display state
static unsigned char display[DISPLAYS];

/*Keyboard state as read from the pins*/
static unsigned char kbState[ROWS][COLUMNS] = { {0,0,0},{0,0,0},{0,0,0},{0,0,0} };

//Pre declaration of the PISR function
void muxKbDsPISR();

void muxKbDsLed(int v)
{
	//Write to board led
	digitalWrite(LED,v&&1);
}
int muxKbDsInit()
{
	if (init == 0) //If not already initialized
	{
		if(SysTickInit()==1) //Init SysTick
		{
			if(SysTickAddFunc(muxKbDsPISR)==1) //Add function to SysTick
			{
				/*Set the mode for every pin*/
				for (unsigned i = 0; i < ROWS; i++)
					pinMode(rows[i], OUTPUT);
				for (unsigned i = 0; i < COLUMNS; i++)
					pinMode(cols[i], INPUT_PULLDOWN);
				for (unsigned i = 0; i < 8; i++)
					pinMode(sevenSegments[i], OUTPUT);
				pinMode(LED, OUTPUT);
				/*Set every row pin to 0*/
				for (unsigned i = 0; i < ROWS; i++)
					digitalWrite(rows[i], 1);
				init = 1;
			}

		}

	}
	return init;
}

void muxKbDsWriteDisplay(unsigned char ds,unsigned int pos)
{
	if (pos < DISPLAYS)
		display[pos] = ds;
}

void muxKbDsGetKbState(unsigned char (*kbS)[COLUMNS]) {
	for (unsigned i = 0; i < ROWS; i++)
		for (unsigned j = 0; j < COLUMNS; j++)
				kbS[i][j] = kbState[i][j];

}

void muxKbDsPISR()
{
	//Increase the number of calls
	nCall++;
	//Disable the row pin so the changes are not observable
	digitalWrite(rows[active], 1);
	if (active < ROWS - 1)
		active++;
	else
		active = 0;
	unsigned i = active;
	//Divided by 4 so every screen has the same brightness
	if((nCall/4)%brightness==0)
		for (unsigned j = 0; j < 8; j++)
			digitalWrite(sevenSegments[j], (display[i] >> j) & 1);
	else
		for (unsigned j = 0; j < 8; j++)
			digitalWrite(sevenSegments[j], 0 );
	//Enable the row
	digitalWrite(rows[i], 0);
	//Read the keyboard
	for (unsigned j = 0; j < COLUMNS; j++)
			kbState[i][j] = digitalRead(cols[j]);
}
void muxKbDsSetBrightness(unsigned c)
{
	brightness = c;
}