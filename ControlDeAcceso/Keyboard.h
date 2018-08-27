#pragma once

/*Function returns cero if no char is on the buffer*/
char readKb(void);

/*Tells if keyboard has available keys*/
int  kbHit();

/*Keyboard flush, returns read ptr to wrt ptr*/
void kbFlush();
