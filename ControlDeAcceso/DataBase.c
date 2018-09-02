#include "DataBase.h"
#include <string.h>
char keys[MAX_USERS][2][MAX_ID_LEN + 1] = {
	{ "463::;6@8=AABDGE" , "97" },
	{ "622876<88<BC>E??CJ" , "1:;9" },
	{ "463::;689=B==D>C" , "58" },
	{ "13579;=?" , "1357" }
};
void encrypt(char* s) {
	for (unsigned k = 0; k < strlen(s); k++)
		s[k] += k;
}