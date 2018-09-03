#ifndef LEDS_H
#define LEDS_H

/*Init leds*/
int ledsInit();

/*Turn off every led*/
void ledsOff();

/*Sets blue led to the given value, 1 ON 0 OFF*/
void ledsBlue(int);

/*Sets red led to the given value, 1 ON 0 OFF*/
void ledsRed(int);

/*Sets green led to the given value, 1 ON 0 OFF*/
void ledsGreen(int);

/*Sets board led to the given value, 1 ON 0 OFF*/
void ledsBoard(int);


#endif
