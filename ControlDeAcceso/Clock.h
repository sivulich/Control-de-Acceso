#pragma once
#ifndef CLOCK_H
#define CLOCK_H
/*Defines the frequency of the clock*/
#define CLOCKS_PER_SEC 1000
/*Clock variable*/
#define clock_t unsigned long long

/*Init  the clock*/
int clockInit();

/*Returns the current time, to get seconds getTime()/CLOCKS_PER_SEC*/
clock_t getTime();

#endif // !CLOCK_H


