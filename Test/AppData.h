#pragma once
#define  MAX_ID_LEN    8
#define  MAX_PASS_LEN  4
typedef struct {
	unsigned currIDlen, currPassLen;
	
	char currID[MAX_ID_LEN+1];
	char currPsswd[MAX_PASS_LEN+1];
}AppData;