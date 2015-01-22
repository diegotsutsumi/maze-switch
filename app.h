#ifndef _APP_H_
#define _APP_H_

#include "user_interface.h"

typedef union
{
	struct
	{
		unsigned char relay1:1;
		unsigned char relay2:1;
		unsigned char relay3:1;
		unsigned char relay4:1;
		unsigned char relay5:1;
		unsigned char relay6:1;
	};
	struct
	{
	    unsigned char relayByte:6;
	};
}Relays;

typedef enum
{
	APP_STATE0_Playing,
	APP_STATE0_Editing,
	APP_STATE0_None
}APP_STATES_LVL0;

typedef enum
{
	APP_STATE1_Idle,
	APP_STATE1_Setting_Up,
	APP_STATE1_Reading_Preset,
	APP_STATE1_Bank_Updating,
	APP_STATE1_Selecting_Pedals,
	APP_STATE1_Selecting_Buffer,
	APP_STATE1_None
}APP_STATES_LVL1;

typedef enum
{
	APP_STATE2_Idle,
	APP_STATE2_TogglingPedals,
	APP_STATE2_TogglingBuffer,
	APP_STATE2_None
}APP_STATES_LVL2;


typedef struct
{
	APP_STATES_LVL0 lvl0;
	APP_STATES_LVL1 lvl1;
	APP_STATES_LVL2 lvl2;
}APP_State;

typedef struct
{
	unsigned char entry_flag;
	Relays relayState;
	Display displayState;
	APP_State current_state;
}APP_Data;

void APP_Init();
void APP_ChangeState(APP_STATES_LVL0 a, APP_STATES_LVL1 b, APP_STATES_LVL2 c);
void APP_Tasks();

#endif