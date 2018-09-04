#pragma once
#ifndef APPDATA_H
#define APPDATA_H
/*Aplication defines*/
#define MAX_ID_LEN    20
#define MAX_PASS_LEN  10
#define MAX_USERS     254 

/*Data used to add a new user*/
struct adminUserData {
	//Current state of the adding process
	int state;
	//Mode of operation: 0,addUser ; 1,deleteUser ; 2,changePass
	int mode;
	//New ID
	char newID[MAX_ID_LEN + 1];
	unsigned newIDlen;
	//New Password
	char newPass[MAX_PASS_LEN + 1];
	unsigned newPassLen;

};

/*Application information*/
typedef struct {
	//Lengths of the information
	unsigned currIDlen, currPassLen;
	//Current ID
	char currID[MAX_ID_LEN + 1];
	//Current password
	char currPsswd[MAX_PASS_LEN + 1];
	//Data structure used to create a new user
	struct adminUserData auData;
}AppData;
#endif // !APPDATA_H
