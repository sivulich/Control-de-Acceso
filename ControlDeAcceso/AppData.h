#pragma once
#ifndef APPDATA_H
#define APPDATA_H
#define MAX_ID_LEN    20
#define MAX_PASS_LEN  10
#define MAX_USERS     254 
typedef struct {
	unsigned currIDlen, currPassLen;

	char currID[MAX_ID_LEN + 1];
	char currPsswd[MAX_PASS_LEN + 1];
}AppData;
#endif // !APPDATA_H
