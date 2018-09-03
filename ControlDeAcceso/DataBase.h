#ifndef DATABASE_H
#define DATABASE_H
#include "AppData.h"

/*Master password used to unlock users and admin functions*/
#define MASTER_PASS "26:3"

/*Main data base stores ID in keys[i][0] and password in keys[i][1]*/
extern char keys[MAX_USERS][2][MAX_ID_LEN + 1];

/*Encrypts the given string to be safely stored*/
void encrypt(char*);


#endif // !DATABASE_H

