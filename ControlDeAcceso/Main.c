#ifndef _WIN32 
#include "hardware.h"
#endif
void App_Init (void);
void App_Run (void);


int main (void)
{
#ifndef _WIN32 
	/*Hardware init*/
	hw_Init();
	/*Disable interrupts to ensure a correct app initialization*/
	hw_DisableInterrupts();
#endif
	/*Init all the modules necessary for the app to run*/
	App_Init();
#ifndef _WIN32 
	/*Enable interrupts*/
	hw_EnableInterrupts();
#endif
	/*Run the aplication*/
	App_Run();
	return 0;
}
