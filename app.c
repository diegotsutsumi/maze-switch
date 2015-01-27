#include "app.h"
APP_Data appData;

void APP_Init()
{
	appData.entry_flag=1;
	//TODO: LOAD last preset from FLASH into RAM (relayState + displayState + currentBank)
	
	UI_DisplayEncoder(8,10,31,7,3,1,&(appData.displayState));//temporary
	
	APP_ChangeState(APP_STATE0_Playing,APP_STATE1_Setting_Up,APP_STATE2_None);
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
					UI_DISPLAY_PARTS updatePart = UI_DISPLAY_ALL;
					GPIO_WriteBit(Relay_Port, Relay1_Pin, (appData.relayState.relay1)?Bit_SET:Bit_RESET);
					GPIO_WriteBit(Relay_Port, Relay2_Pin, (appData.relayState.relay2)?Bit_SET:Bit_RESET);
					GPIO_WriteBit(Relay_Port, Relay3_Pin, (appData.relayState.relay3)?Bit_SET:Bit_RESET);
					GPIO_WriteBit(Relay_Port, Relay4_Pin, (appData.relayState.relay4)?Bit_SET:Bit_RESET);
					GPIO_WriteBit(Relay_Port, Relay5_Pin, (appData.relayState.relay5)?Bit_SET:Bit_RESET);
					GPIO_WriteBit(Relay_Port, Relay6_Pin, (appData.relayState.relay6)?Bit_SET:Bit_RESET);
					
					UI_DisplayExtEvent(UI_DISPLAY_UPDATE,(void*)(&updatePart),(void*)(&(appData.displayState)));
                       
					appData.entry_flag=1;
					APP_ChangeState(APP_STATE0_Playing,APP_STATE1_Idle,APP_STATE2_None);
				}
				break;
				case APP_STATE1_Idle:
				{
				}
				break;
				case APP_STATE1_Reading_Preset:
				{
					//TODO: Load preset from FLASH into RAM (relayState + displayState)
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

void APP_UIEventHandler(UI_BUTTON_ACTIONS event)
{
	switch(event)
	{
		case FOOT_1_PRESSED:
		{
		}
		break;
		case FOOT_2_PRESSED:
		{
		}
		break;
		case FOOT_3_PRESSED:
		{
		}
		break;
		case FOOT_LEFT_PRESSED:
		{
			if(current_state.lvl1==APP_STATE1_Idle && current_bank<9)
			{
				current_bank++;
				APP_ChangeState(APP_STATE0_Playing,APP_STATE1_Bank_Updating,APP_STATE2_None);
			}
		}
		break;
		case FOOT_RIGHT_PRESSED:
		{
			if(current_state.lvl1==APP_STATE1_Idle && current_bank>1)
			{
				current_bank--;
				APP_ChangeState(APP_STATE0_Playing,APP_STATE1_Bank_Updating,APP_STATE2_None);
			}
		}
		break;
		case BUFFER_SWITCHED_ON:
		{
		}
		break;
		case BUFFER_SWITCHED_OFF:
		{
		}
		break;
		case SAVE_OK:
		{
		}
		break;
		case SAVE_CANCEL:
		{
		}
		break;
		case ERROR:
		{
		}
		break;
		default
		{
		}
		break;
	}
}