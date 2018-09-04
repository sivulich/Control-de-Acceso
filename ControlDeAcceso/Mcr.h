
#ifndef _MCR_H_
#define _MCR_H_

#include "board.h"

#define MCR_ENABLE   PORTNUM2PIN(PC,4)
#define MCR_CLK   PORTNUM2PIN(PD,0)
#define MCR_DATA   PORTNUM2PIN(PD,2)





#define MCR_CARD_MEMORY 25 //Cantidad de cards que se pueden guardar en el buffercircular
#define MCR_CARD_VALUE_LENGTH 19 //19 dígitos tiene el campo de ID en una tarjeta.


int mcrInit(void);

int hasCardValue();

void getLastCard(char* dest);

void mcrFlush(void);

#endif
