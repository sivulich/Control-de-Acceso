#pragma once
#ifndef CLOCK_H
#define CLOCK_H
//Reemplazar por 100kHz
#define CLOCKS_PER_SEC 1000
//Reemplazar por unsigned long long
#define clock_t unsigned long long
int clockInit();
clock_t getTime();
#endif // !CLOCK_H


