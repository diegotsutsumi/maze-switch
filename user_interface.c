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
	UI_ButtonsStateMachine();
	UI_DisplayStateMachine();
}

void UI_ButtonsInit()
{
	btnData.actionHandler=0;
	btnData.timerDebounce=0;
	btnData.editTimer=0;
	btnData.editTimerEnabled=0;
	btnData.switchesState.switchesByte=0;
	btnData.switchesNewState.switchesByte=0;
	btnData.currentState = UI_BTN_STATE_Init;
}

void UI_ButtonsStateMachine()
{
	switch(btnData.currentState)
	{
		case UI_BTN_STATE_Init:
		{
			btnData.switchesState.foot1 = GPIO_ReadInputDataBit(Footswitch1_port, Footswitch1_pin);
			btnData.switchesState.foot2 = GPIO_ReadInputDataBit(Footswitch2_port, Footswitch2_pin);
			btnData.switchesState.foot3 = GPIO_ReadInputDataBit(Footswitch3_port, Footswitch3_pin);
			btnData.switchesState.footLeft = GPIO_ReadInputDataBit(FootswitchLeft_port, FootswitchLeft_pin);
			btnData.switchesState.footRight = GPIO_ReadInputDataBit(FootswitchRight_port, FootswitchRight_pin);
			btnData.switchesState.buffSwitch = GPIO_ReadInputDataBit(BufferSwitch_port, BufferSwitch_pin);
			
			//btnData.editButton = GPIO_ReadInputDataBit(EditButton_port, EditButton_pin);
			
			btnData.currentState = UI_BTN_STATE_ListeningButtons;
		}
		case UI_BTN_STATE_ListeningButtons:
		{
			btnData.switchesNewState.foot1 = GPIO_ReadInputDataBit(Footswitch1_port, Footswitch1_pin);
			btnData.switchesNewState.foot2 = GPIO_ReadInputDataBit(Footswitch2_port, Footswitch2_pin);
			btnData.switchesNewState.foot3 = GPIO_ReadInputDataBit(Footswitch3_port, Footswitch3_pin);
			btnData.switchesNewState.footLeft = GPIO_ReadInputDataBit(FootswitchLeft_port, FootswitchLeft_pin);
			btnData.switchesNewState.footRight = GPIO_ReadInputDataBit(FootswitchRight_port, FootswitchRight_pin);
			btnData.switchesNewState.buffSwitch = GPIO_ReadInputDataBit(BufferSwitch_port, BufferSwitch_pin);
			
			btnData.editButton = GPIO_ReadInputDataBit(EditButton_port, EditButton_pin);
			
			if((btnData.switchesState.switches != btnData.switchesNewState.switches) || 
				(btnData.editButton==0))
			{
				btnData.timerDebounce = UI_BTN_DEBOUNCING_TIME;
				btnData.currentState = UI_BTN_STATE_Debouncing;
			}
		}
		break;
		
		case UI_BTN_STATE_Debouncing:
		{
			if(btnData.timerDebounce==0)
			{
				btnData.switchesNewState.foot1 = GPIO_ReadInputDataBit(Footswitch1_port, Footswitch1_pin);
				btnData.switchesNewState.foot2 = GPIO_ReadInputDataBit(Footswitch2_port, Footswitch2_pin);
				btnData.switchesNewState.foot3 = GPIO_ReadInputDataBit(Footswitch3_port, Footswitch3_pin);
				btnData.switchesNewState.footLeft = GPIO_ReadInputDataBit(FootswitchLeft_port, FootswitchLeft_pin);
				btnData.switchesNewState.footRight = GPIO_ReadInputDataBit(FootswitchRight_port, FootswitchRight_pin);
				btnData.switchesNewState.buffSwitch = GPIO_ReadInputDataBit(BufferSwitch_port, BufferSwitch_pin);
				
				btnData.editButton = GPIO_ReadInputDataBit(EditButton_port, EditButton_pin);

				if(btnData.editButton==0)
				{
					btnData.editTimer=0;
					btnData.editTimerEnabled=1;
					btnData.currentState = UI_BTN_STATE_CountingEditButton;
				}
				else
				{
					{
						Switches diff;
						diff.switchesByte = btnData.switchesState.switchesByte ^ btnData.switchesNewState.switchesByte;
						if(diff.foot1)
						{
							btnData.actionHandler(FOOT_1_PRESSED);
						}
						else if(diff.foot2)
						{
							btnData.actionHandler(FOOT_2_PRESSED);
						}
						else if(diff.foot3)
						{
							btnData.actionHandler(FOOT_3_PRESSED);
						}
						else if(diff.footLeft)
						{
							btnData.actionHandler(FOOT_LEFT_PRESSED);
						}
						else if(diff.footRight)
						{
							btnData.actionHandler(FOOT_RIGHT_PRESSED);
						}
						else if(diff.buffSwitch)
						{
							btnData.actionHandler((btnData.switchesNewState.buffSwitch)?BUFFER_SWITCHED_ON:BUFFER_SWITCHED_OFF);
						}
						else
						{
							btnData.actionHandler(ERROR);
						}
						btnData.currentState = UI_BTN_STATE_WaitingRelease;
					}
				}
			}
		}
		break;
		
		case UI_BTN_STATE_WaitingRelease:
		{
		}
		break;
		
		case UI_BTN_STATE_CountingEditButton:
		{
			btnData.editButton = GPIO_ReadInputDataBit(EditButton_port, EditButton_pin);
			if(btnData.editButton)
			{
				btnData.editTimerEnabled=0;
				if(btnData.editTimer>=UI_BTN_CANCEL_TIME)
				{
					btnData.actionHandler(SAVE_CANCEL);
				}
				else
				{
					btnData.actionHandler(SAVE_OK);
				}
				btnData.currentState = UI_BTN_STATE_WaitingRelease;
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
	dispData.currentDisp.displayWord=0;
	dispData.whichBlinks.displayWord=0;
	dispData.bufferBlink.displayWord=0;
	dispData.bufferOut.displayWord=0;

	dispData.buffCount=0;	
	dispData.entry_flag=0;
	dispData.control=UI_DISPLAY_INIT_BIT;

	dispData.updatePending=0;
	
	dispData.blinkTimer=0;
	dispData.blinkTimerEnabled=0;
	dispData.blinkTurn=0;
	
	dispData.currentState=UI_DISP_STATE_Idle;
}

void UI_DisplayStateMachine()
{
	switch(dispData.currentState)
	{
		case UI_DISP_STATE_Idle:
		{
			if(dispData.updatePending)
			{
				dispData.updatePending=0;
				dispData.bufferOut.displayWord = dispData.currentDisp.displayWord;
				
				if(!dispData.blinkTimerEnabled)
				{
					dispData.currentState=UI_DISP_STATE_SendingInfo;
				}
			}
			else
			{
				if(dispData.blinkTimerEnabled && dispData.blinkTimer>=UI_DISP_BLINKING_TIME)
				{
					dispData.blinkTimer=0;
					if(dispData.blinkTurn==0)
					{
						dispData.blinkTurn=1;
						dispData.bufferOut.displayWord = dispData.bufferBlink.displayWord;
					}
					else
					{
						dispData.blinkTurn=0;
						dispData.bufferOut.displayWord = dispData.currentDisp.displayWord;
					}
					dispData.currentState=UI_DISP_STATE_SendingInfo;
				}
			}
		}
		break;
		
		case UI_DISP_STATE_SendingInfo:
		{
			if(dispData.entry_flag)
			{
				dispData.entry_flag=0;
				dispData.buffCount=0;
				dispData.control=UI_DISPLAY_INIT_BIT;
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

void UI_DisplayAddWhichBlinks(UI_DISPLAY_PARTS part, Display customAdd)
{
	switch(part)
	{
		case UI_DISPLAY_ALL:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_ALL_MASK;
		}
		break;
		case UI_DISPLAY_7SEG:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_7SEG_MASK;
		}
		break;
		case UI_DISPLAY_VOLUME:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_VOL_MASK;
		}
		break;
		case UI_DISPLAY_LEDPEDAL:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_PEDAL_MASK;
		}
		break;
		case UI_DISPLAY_LEDFOOT:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_FOOT_MASK;
		}
		break;
		case UI_DISPLAY_LEDBUFFER:
		{
			dispData.whichBlinks.displayWord|=UI_DISP_BUFFER_MASK;
		}
		break;
		case UI_DISPLAY_CUSTOM:
		{
			dispData.whichBlinks.displayWord|=customAdd.displayWord;
		}
		default:
		{
		}
		break;
	}
}

void UI_DisplayRemoveWhichBlinks(UI_DISPLAY_PARTS part, Display customRemove)
{
	switch(part)
	{
		case UI_DISPLAY_ALL:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_ALL_MASK);
		}
		break;
		case UI_DISPLAY_7SEG:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_7SEG_MASK);
		}
		break;
		case UI_DISPLAY_VOLUME:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_VOL_MASK);
		}
		break;
		case UI_DISPLAY_LEDPEDAL:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_PEDAL_MASK);
		}
		break;
		case UI_DISPLAY_LEDFOOT:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_FOOT_MASK);
		}
		break;
		case UI_DISPLAY_LEDBUFFER:
		{
			dispData.whichBlinks.displayWord&=~(UI_DISP_BUFFER_MASK);
		}
		break;
		case UI_DISPLAY_CUSTOM:
		{
			dispData.whichBlinks.displayWord&=~(customRemove.displayWord);
		}
		break;
		default:
		{
		}
		break;
	}
}

void UI_DisplayUpdate(UI_DISPLAY_PARTS part, Display newDisp)
{
	unsigned int mask;
	switch(part)
	{
		case UI_DISPLAY_ALL:
		{
			mask = UI_DISP_ALL_MASK;
		}
		break;
		case UI_DISPLAY_7SEG:
		{
			mask = UI_DISP_7SEG_MASK;
		}
		break;
		case UI_DISPLAY_VOLUME:
		{
			mask = UI_DISP_VOL_MASK;
		}
		break;
		case UI_DISPLAY_LEDPEDAL:
		{
			mask = UI_DISP_PEDAL_MASK;
		}
		break;
		case UI_DISPLAY_LEDFOOT:
		{
			mask = UI_DISP_FOOT_MASK;
		}
		break;
		case UI_DISPLAY_LEDBUFFER:
		{
			mask = UI_DISP_BUFFER_MASK;
		}
		break;

		default:
		{
			mask = 0;
		}
		break;
	}

	dispData.currentDisp.displayWord = ((newDisp.displayWord) & (mask)) | ((dispData.currentDisp.displayWord) & ~(mask));
}

void UI_DisplayExtEvent(UI_DISPLAY_EVENTS extEvent, void * eventData1, void * eventData2)
{
	switch(extEvent)
	{
		case UI_DISPLAY_UPDATE:
		{
			if(eventData1!=0 && eventData2!=0)
			{
				UI_DisplayUpdate(*((UI_DISPLAY_PARTS*)(eventData1)), *((Display*)(eventData2)));

				dispData.bufferBlink.displayWord = dispData.currentDisp.displayWord;
				dispData.bufferBlink.displayWord &= ~(dispData.whichBlinks.displayWord);
				dispData.updatePending=1;
			}
		}
		break;
		
		case UI_DISPLAY_START_BLINKING:
		{
			if(eventData1!=0)
			{
				if(!dispData.blinkTimerEnabled)
				{
					dispData.blinkTimer=0;
					dispData.blinkTimerEnabled=1;
				}

				Display aux;
				aux.displayWord = (eventData2)?*((Display*)(eventData2)).displayWord:0;

				UI_DisplayAddWhichBlinks(*((UI_DISPLAY_PARTS*)(eventData1)), aux);
				dispData.bufferBlink.displayWord = dispData.currentDisp.displayWord;
				dispData.bufferBlink.displayWord &= ~(dispData.whichBlinks.displayWord);
			}
		}
		break;
		
		case UI_DISPLAY_STOP_BLINKING:
		{
			if(eventData1!=0)
			{
				if(dispData.blinkTimerEnabled)
				{
					if((*((UI_DISPLAY_PARTS*)(eventData1))==UI_DISPLAY_ALL) || (eventData2)?((*((Display*)(eventData2)).displayWord==0xFFFFFFF)?1:0):0)
					{
						dispData.blinkTimerEnabled=0;
						dispData.blinkTimer=0;
					}

					Display aux;
					aux.displayWord = (eventData2)?*((Display*)(eventData2)).displayWord:0;

					UI_DisplayRemoveWhichBlinks(*((UI_DISPLAY_PARTS*)(eventData1)), aux.displayWord);
					dispData.bufferBlink.displayWord = dispData.currentDisp.displayWord;
					dispData.bufferBlink.displayWord &= ~(dispData.whichBlinks.displayWord);
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

void UI_TIM2_ISR()
{
	if(btnData.timerDebounce>0)
	{
		btnData.timerDebounce--;
	}
	if(btnData.editTimerEnabled)
	{
		btnData.editTimer++;
	}
	
	
	if(dispData.blinkTimerEnabled)
	{
		dispData.blinkTimer++;
	}
}