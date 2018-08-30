#pragma once
#ifndef CARDREADER_H
#define CARDREADER_H
//Te dice si hay un nuevo valor de una tarjeta
int hasCardValue();
//Te devuelve el valor
void getCard(char* dest);
//Tira los valores del buffer
void flush();
#endif // !CARDREADER_H
