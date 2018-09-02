#ifndef DATABASE_H
#define DATABASE_H
#include "AppData.h"
#define MASTER_PASS "26:3"
extern char keys[MAX_USERS][2][MAX_ID_LEN + 1];
void encrypt(char*);
#endif // !DATABASE_H

