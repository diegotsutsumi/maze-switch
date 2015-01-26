#include "user_interface.h"

BTN_Data btnData;
DISP_Data dispData;

void UI_Init()
{
	UI_ButtonsInit();
	UI_DisplayInit();
}

void UI_Tasks()
{
	//UI_ButtonsStateMachine();
	UI_DisplayStateMachine();
}

void UI_ButtonsInit()
{
	//TODO: Read all footswitch states. And load it to RAM memory
	//They'll be random states and we just manage transitions
}

void UI_ButtonsStateMachine()
{
	switch(btnData.currentState)
	{
		case UI_BTN_STATE_ListeningButtons:
		{
			//TODO: Poll all the switches
			if(0/*Reading different from btnData.allSwitches.switchesByte*/)
			{
				//Start Bouncing timer
				//btnData.beforeBounce = currentReading
				btnData.currentState = UI_BTN_STATE_Debouncing;
			}
		}
		break;
		
		case UI_BTN_STATE_Debouncing:
		{
			if(0/*Bouncing Timer Finished*/)
			{
				//Poll all the switches

				if(0/*GPIO READ Save button == 0*/)
				{
					//start save button timer
					btnData.currentState = UI_BTN_STATE_CountingSaveButton;
				}
			
				//If any other button was pressed:
					//generate an Action
					//btnData.currentState = UI_BTN_STATE_WaitingRelease;
			}
		}
		break;
		
		case UI_BTN_STATE_WaitingRelease:
		{
			//TODO: Poll all the switches
				//btnData.allSwitches = current reading; //To keep tracking the state change but not doing any actions
		}
		break;
		
		case UI_BTN_STATE_CountingSaveButton:
		{
			if(0/*GPIO Read Save Button == 1*/)
			{
				//TODO: Count the timer and generate Save Action
				//btnData.currentState = UI_BTN_STATE_WaitingRelease;
			}
		}
		break;
		
		default:
		{
		}
		break;
	}
}

void UI_ButtonsExtEvent(UI_BUTTON_EVENTS extEvent)
{
	switch(extEvent)
	{
		case LOCK:
		{
			if(btnData.currentState==UI_BTN_STATE_ListeningButtons)
			{
				btnData.currentState = UI_BTN_STATE_WaitingRelease;
			}
		}
		break;
		
		case RELEASE:
		{
			if(btnData.currentState==UI_BTN_STATE_WaitingRelease)
			{
				btnData.currentState = UI_BTN_STATE_ListeningButtons;
			}
		}
		break;
		
		default:
		{
		}
		break;
	}
}

unsigned char UI_DisplayEncoder(unsigned char _7seg, unsigned char volume, unsigned char pedals,
							unsigned char foots, unsigned char bufferSwitches, unsigned char edit, Display * disp_out)
{
	(*disp_out).displayWord=0;
	if(_7seg>9 || (volume>10) || pedals>31 || foots>7 || bufferSwitches>3 || edit>1)
	{
		return 0;
	}
	
	if(_7seg!=1 && _7seg!=4)
	{
		(*disp_out)._7SegUp=1;
	}
	if(_7seg!=0 && _7seg!=1 && _7seg!=7)
	{
		(*disp_out)._7SegMiddle=1;
	}
	if(_7seg!=1 && _7seg!=4 && _7seg!=7)
	{
		(*disp_out)._7SegDown=1;
	}
	if(_7seg!=1 && _7seg!=2 && _7seg!=3 && _7seg!=7)
	{
		(*disp_out)._7SegLeftUp=1;
	}
	if(_7seg==0 || _7seg==2 || _7seg==6 || _7seg==8)
	{
		(*disp_out)._7SegLeftDown=1;
	}
	if(_7seg!=5 && _7seg!=6)
	{
		(*disp_out)._7SegRightUp=1;
	}
	if(_7seg!=2)
	{
		(*disp_out)._7SegRightDown=1;
	}
	
	if(volume>0)
	{
		(*disp_out).VolumeBar1=1;
	}
	if(volume>1)
	{
		(*disp_out).VolumeBar2=1;
	}
	if(volume>2)
	{
		(*disp_out).VolumeBar3=1;
	}
	if(volume>3)
	{
		(*disp_out).VolumeBar4=1;
	}
	if(volume>4)
	{
		(*disp_out).VolumeBar5=1;
	}
	if(volume>5)
	{
		(*disp_out).VolumeBar6=1;
	}
	if(volume>6)
	{
		(*disp_out).VolumeBar7=1;
	}
	if(volume>7)
	{
		(*disp_out).VolumeBar8=1;
	}
	if(volume>8)
	{
		(*disp_out).VolumeBar9=1;
	}
	if(volume>9)
	{
		(*disp_out).VolumeBar10=1;
	}
	
	(*disp_out).LEDPedal1=(pedals & (0x01 << 0))?1:0;
	(*disp_out).LEDPedal2=(pedals & (0x01 << 1))?1:0;
	(*disp_out).LEDPedal3=(pedals & (0x01 << 2))?1:0;
	(*disp_out).LEDPedal4=(pedals & (0x01 << 3))?1:0;
	(*disp_out).LEDPedal5=(pedals & (0x01 << 4))?1:0;
	
	(*disp_out).LEDFoot1=(foots & (0x01 << 0))?1:0;
	(*disp_out).LEDFoot2=(foots & (0x01 << 1))?1:0;
	(*disp_out).LEDFoot3=(foots & (0x01 << 2))?1:0;
	
	(*disp_out).LEDBuffer=(bufferSwitches & (0x01 << 0))?1:0;
	(*disp_out).LEDSwBuffer=(bufferSwitches & (0x01 << 1))?1:0;
	
	(*disp_out).LEDEdit=edit;
	return 1;
}

void UI_DisplayInit()
{
	dispData.entry_flag=0;
	dispData.buffCount=0;
	dispData.control=0;
	dispData.currentState=UI_DISP_STATE_Idle;
}

void UI_DisplayStateMachine()
{
	switch(dispData.currentState)
	{
		case UI_DISP_STATE_Idle:
		{
		}
		break;
		
		case UI_DISP_STATE_SendingInfo:
		{
			if(dispData.entry_flag)
			{
				dispData.entry_flag=0;
				dispData.buffCount=0;
				dispData.control=0;
			}
			else
			{
				switch(dispData.control)
				{
					case UI_DISPLAY_INIT_BIT:
					{
						if(dispData.buffCount>=UI_DISP_WORD_SIZE)
						{
							dispData.control = UI_DISPLAY_LATCH_EN;
							GPIO_SetBits(Display_LatchEnable_Port,Display_LatchEnable_Pin);
						}
                        else
                        {
                            GPIO_ResetBits(Display_Clock_Port, Display_Clock_Pin);
                            dispData.control = UI_DISPLAY_WRITE_BIT;
                        }
					}
					break;
					case UI_DISPLAY_WRITE_BIT:
					{
						if(dispData.bufferOut.displayWord & (0x01 << dispData.buffCount))
						{
							GPIO_SetBits(Display_SerialIn_Port, Display_SerialIn_Pin);
						}
						else
						{
							GPIO_ResetBits(Display_SerialIn_Port, Display_SerialIn_Pin);
						}
						dispData.control = UI_DISPLAY_FLIP_CLOCK;
					}
					break;
					case UI_DISPLAY_FLIP_CLOCK:
					{
						GPIO_SetBits(Display_Clock_Port, Display_Clock_Pin);
						dispData.buffCount++;
						dispData.control=UI_DISPLAY_INIT_BIT;
					}
					break;
					case UI_DISPLAY_LATCH_EN:
					{
						GPIO_ResetBits(Display_LatchEnable_Port,Display_LatchEnable_Pin);
						dispData.currentState=UI_DISP_STATE_Idle;
						dispData.entry_flag=0;
					}
					break;
				}
			}
		}
		break;
		
		default:
		{
		}
		break;
	}
}

void UI_DisplayExtEvent(UI_DISPLAY_EVENTS extEvent, Display * eventData)
{
	switch(extEvent)
	{
		case UI_DISPLAY_SEND_INFO:
		{
			if(dispData.currentState==UI_DISP_STATE_Idle)
			{
				dispData.bufferOut.displayWord = (*eventData).displayWord;
				dispData.entry_flag=1;
				dispData.currentState = UI_DISP_STATE_SendingInfo;
			}
		}
		break;
		
		default:
		{
		}
		break;
	}
}

void UI_TIM2_ISR()
{

}