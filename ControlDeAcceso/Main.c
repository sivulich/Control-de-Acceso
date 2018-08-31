#ifndef _WIN32 
#include "hardware.h"
#endif
void App_Init (void);
void App_Run (void);


int main (void)
{
#ifndef _WIN32 
	hw_Init();
	hw_DisableInterrupts();
#endif
	App_Init(); /* Program-specific setup */
#ifndef _WIN32 
	hw_EnableInterrupts();
#endif
	
	App_Run(); /* Program-specific loop  */
	return 0;
}
