#pragma once
#ifndef CLOCK_H
#define CLOCK_H
//Sacar esto
#include <time.h>
//Reemplazar por 100kHz
//#define CLOCKS_PER_SEC CLOCKS_PER_SEC
//Reemplazar por unsigned long long
#define clock_t clock_t

clock_t getTime();
#endif // !CLOCK_H


