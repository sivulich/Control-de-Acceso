#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "States.h"
#include "Keyboard.h"
#include "Display.h"
#include "DataBase.h"
#include "Mcr.h"
#include "Game.h"

enum{BRIGHTNESS=PROP_EVENT_BASE,ADD_USER,DELETE_USER,CHANGE_PASS,END};
typedef struct string String;
static event(*func)(void* uData)=NULL;
static event modifyDataBase(void* uData)
{
	AppData* data=(AppData*)uData;
	switch (data->auData.state)
	{
		case 0:
			data->auData.state = 1;
			data->auData.newIDlen=0;
			data->auData.newPassLen = 0;
			for (int i = 0; i < MAX_ID_LEN; i++)
				data->auData.newID[i] = 0;
			for (int i = 0; i < MAX_PASS_LEN; i++)
				data->auData.newPass[i] = 0;
			break;
		case 1:
			if (data->auData.newPassLen >= strlen(MASTER_PASS))
			{
				encrypt(data->auData.newPass);
				if (!strcmp(MASTER_PASS, data->auData.newPass))
				{
					for (int i = 0; i < strlen(MASTER_PASS); i++)
						data->auData.newPass[i] = 0;
					data->auData.newPassLen = 0;
					data->auData.state = 2;
				}
				else
				{
					data->auData.state = 0;
					return END;
				}
			}
			else if (kbHit() )
			{
				data->auData.newPass[data->auData.newPassLen] = readKb();
				data->auData.newPassLen++;
			}
			break;
		case 2:
			if (hasCardValue())
			{
				data->auData.state = 4;
				break;
			}
			else if (kbHit())
				data->auData.state = 3;

		case 3:
			if (kbHit())
			{
				char c = readKb();
				if(c=='#' || data->auData.newIDlen>=MAX_ID_LEN)
					if (data->auData.newIDlen > 0)
					{
						if (data->auData.mode == 0 || data->auData.mode == 2) //AddUser or ChangePass
							data->auData.state = 5;
						else if (data->auData.mode == 1)  //DeleteUser
							data->auData.state = 7;

					}
					else
					{
						data->auData.state = 0;
						return END;
					}
				else if (c == '*')
				{
					if (data->auData.newIDlen > 0)
					{
						data->auData.newIDlen--;
						data->auData.newID[data->auData.newIDlen] = 0;
					}
					else
					{
						data->auData.state = 0;
						return END;
					}
				}
				else
					data->auData.newID[data->auData.newIDlen++] = c;
			}
			break;

		case 4:
			getLastCard(data->auData.newID);
			data->auData.newIDlen = strlen(data->auData.newID);
			if (data->auData.mode == 0 || data->auData.mode == 2) //AddUser or ChangePass
				data->auData.state = 5;
			else if (data->auData.mode == 1)  //DeleteUser
				data->auData.state = 7;
			break;
		case 5:
			if (kbHit())
			{
				char c = readKb();
				if (c == '#' || data->auData.newPassLen >= MAX_ID_LEN)
					if (data->auData.newPassLen > 0)
					{
						if (data->auData.mode == 0)		  //AddUser
							data->auData.state = 6;
						else if (data->auData.mode == 1)  //DeleteUser
							data->auData.state = 7;
						else if (data->auData.mode == 2)  //ChangePass
							data->auData.state = 8;
					}
					else
					{
						data->auData.state = 0;
						return END;
					}
				else if (c == '*')
				{
					if (data->auData.newPassLen > 0)
					{
						data->auData.newPassLen--;
						data->auData.newPass[data->auData.newPassLen] = 0;
					}
					else
					{
						data->auData.state = 0;
						return END;
					}
				}
				else
					data->auData.newPass[data->auData.newPassLen++] = c;
			}
			break;
		case 6:		//AddUser
			if (data->auData.newIDlen > 0)
			{
				unsigned i = 0;
				while (i < MAX_USERS && keys[i][0][0] != 0 && strcmp(keys[i][0], data->auData.newID)) i++;
				if (keys[i][0][0] == 0 && i<MAX_USERS)
				{
					encrypt(data->auData.newID);
					encrypt(data->auData.newPass);
					strcpy(keys[i][0], data->auData.newID);
					strcpy(keys[i][1], data->auData.newPass);
				}
			}
			data->auData.state = 0;
			return END;
			break;
		case 7:		//DeleteUser
			if (data->auData.newIDlen > 0)
			{
				unsigned i = 0, j = 0;
				encrypt(data->auData.newID);
				while (i < MAX_USERS && keys[i][0][0] != 0 && strcmp(keys[i][0], data->auData.newID)) i++;	//Look the pos of the given ID
				while (j < MAX_USERS && keys[j][0][0] != 0) j++;	//Look for the final pos
				if (!strcmp(keys[i][0], data->auData.newID) && i<MAX_USERS && keys[j][0][0] == 0)
				{
					//Copy the last user on the pos of the user to delete
					strcpy(keys[i][0], keys[j - 1][0]);
					strcpy(keys[i][1], keys[j - 1][1]);
					//Delete the last user info as its now in the pos i
					char *del = "";
					strcpy(keys[j - 1][0], del);
					strcpy(keys[j - 1][1], del);
				}
			}
			data->auData.state = 0;
			return END;
			break;
		case 8:		//ChangePass
			if (data->auData.newIDlen > 0)
			{
				unsigned i = 0;
				encrypt(data->auData.newID);
				while (i < MAX_USERS && keys[i][0][0] != 0 && strcmp(keys[i][0], data->auData.newID)) i++;	//Look the pos of the given ID
				if (!strcmp(keys[i][0], data->auData.newID) && i<MAX_USERS)
				{
					//Change the pass of the given ID
					encrypt(data->auData.newPass);
					strcpy(keys[i][1], data->auData.newPass);
				}
			}
			data->auData.state = 0;
			return END;
	}
	return BREAK;
}


static event brightness(void* uData)
{
	char c;
	if (kbHit())
	{
		c = readKb();
		setBrightness(c - '0');
		return END;
	}
	return BREAK;

}

static event adminModifyDataBase(State* thi, event ev, void* uData)
{
	if (func == NULL)
		func = modifyDataBase;
	return BREAK;
}
static event adminReset(State* thi, event ev, void* uData)
{
	func = NULL;
	return BREAK;
}
static event adminBrightness(State* thi, event ev, void* uData)
{
	if (func == NULL)
		func = brightness;
	return BREAK;
}

static const Transition TableAdmin[] = {
	{ CONTINUE, NULL,&Idle },
	{ BRIGHTNESS, adminBrightness,&Admin},
	{ ADD_USER,adminModifyDataBase,&Admin },
	{ DELETE_USER,adminModifyDataBase,&Admin },
	{ CHANGE_PASS,adminModifyDataBase,&Admin },
	{ END,adminReset,&Idle},
	{ END_OF_TABLE,NULL, &Idle }
};
static event adminLoop(State* thi, void* uData)
{
	if (func != NULL)
		return func(uData);
	char c;
	AppData* data=(AppData*)uData;
	if(data->auData.state!=0)
		data->auData.state=0;
	if (kbHit())
	{
		c = readKb();
		if (c == '1')
			return BRIGHTNESS;
		else if (c == '2')
		{
			data->auData.mode = 0;
			return ADD_USER;
		}
		else if (c == '3')
		{
			data->auData.mode = 1;
			return DELETE_USER;
		}
		else if (c == '4')
		{
			data->auData.mode = 2;
			return CHANGE_PASS;
		}
		else if (c== '0')
		{
			gameRun();
			return CONTINUE;
		}
		else if (c == '#')
			return CONTINUE;
	}
	return BREAK;
}
State Admin = { "ADMIN", adminLoop, TableAdmin };
