#include "app.h"
APP_Data appData;

void APP_Init()
{
	appData.entry_flag=1;
	appData.relayState.relayByte=0;
	APP_ChangeState(APP_STATE0_Playing,APP_STATE1_Reading_Preset,APP_STATE2_None);
}

void APP_ChangeState(APP_STATES_LVL0 a, APP_STATES_LVL1 b, APP_STATES_LVL2 c)
{
	appData.current_state.lvl0 = a;
	appData.current_state.lvl1 = b;
	appData.current_state.lvl2 = c;
}

void APP_Tasks()
{
	switch(appData.current_state.lvl0)
	{
		case APP_STATE0_Playing:
		{
			switch(appData.current_state.lvl1)
			{
				case APP_STATE1_Setting_Up:
				{
					uint16_t gpioState;
					if(appData.entry_flag)
					{
						//TODO: Pick relays from memory
						appData.relayState.relay1 = 1;//Temporary Test
						appData.relayState.relay2 = 0;
						appData.relayState.relay3 = 0;
						appData.relayState.relay4 = 1;
						appData.relayState.relay5 = 0;
						appData.relayState.relay6 = 0;
						
						gpioState = GPIO_ReadOutputData(Relay_Port);
						gpioState = gpioState ^ ((uint16_t)(appData.relayState.relayByte) << 9);
						GPIO_Write(Relay_Port, gpioState);
						
						UI_DisplayEncoder(3,4,15,3,0,0,&(appData.displayState));
						UI_DisplayExtEvent(UI_DISPLAY_SEND_INFO,&(appData.displayState));
					}
				}
				break;
				case APP_STATE1_Idle:
				{
				}
				break;
				case APP_STATE1_Reading_Preset:
				{
				}
				break;
				case APP_STATE1_Bank_Updating:
				{
				}
				break;
			}
		}
		break;
		
		case APP_STATE0_Editing:
		{
			switch(appData.current_state.lvl1)
			{
				case APP_STATE1_Selecting_Pedals:
				{
					switch(appData.current_state.lvl2)
					{
						case APP_STATE2_Idle:
						{
						}
						break;
						case APP_STATE2_TogglingPedals:
						{
						}
						break;
					}
				}
				case APP_STATE1_Selecting_Buffer:
				{
					switch(appData.current_state.lvl2)
					{
						case APP_STATE2_Idle:
						{
						}
						break;
						case APP_STATE2_TogglingBuffer:
						{
						}
						break;
					}
				}
				break;
			}
		}
		break;
	}
}