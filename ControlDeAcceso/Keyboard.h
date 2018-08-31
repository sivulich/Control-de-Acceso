#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H
/*Keyboard Init*/
int kBInit();
/*Peek next char*/
char kBPeek();

/*Function returns cero if no char is on the buffer*/
char readKb(void);

/*Tells if keyboard has available keys*/
int  kbHit();

/*Keyboard flush, returns read ptr to wrt ptr*/
void kbFlush();
#endif // !KEYBOARD_H

