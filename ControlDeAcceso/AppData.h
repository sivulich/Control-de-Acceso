#pragma once
#ifndef APPDATA_H
#define APPDATA_H
#define MAX_ID_LEN    20
#define MAX_PASS_LEN  10
#define MAX_USERS     254 
struct addUserData {
	int state;
	char newID[MAX_ID_LEN + 1];
	unsigned newIDlen;
	char newPass[MAX_PASS_LEN + 1];
	unsigned newPassLen;

};
typedef struct {
	unsigned currIDlen, currPassLen;

	char currID[MAX_ID_LEN + 1];
	char currPsswd[MAX_PASS_LEN + 1];
	struct addUserData auData;
}AppData;
#endif // !APPDATA_H
