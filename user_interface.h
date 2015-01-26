#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "bsp_config.h"

#define UI_BTN_DEBOUNCING_TIME 2
#define UI_BTN_CANCEL_TIME 1000

#define UI_DISP_WORD_SIZE 28
typedef enum
{
	FOOT_1_PRESSED,
	FOOT_2_PRESSED,
	FOOT_3_PRESSED,
	FOOT_4_PRESSED,
	FOOT_5_PRESSED,
	BUFFER_SWITCHED,
	SAVE_OK,
	SAVE_CANCEL
}UI_BUTTON_ACTIONS;

typedef enum
{
	LOCK, //External
	RELEASE, //External
	SAVE_RISING_EDGE, //Detected by Hardware change
	SAVE_FALLING_EDGE
}UI_BUTTON_EVENTS;

typedef enum
{
	UI_BTN_STATE_ListeningButtons,
	UI_BTN_STATE_Debouncing,
	UI_BTN_STATE_WaitingRelease,
	UI_BTN_STATE_CountingSaveButton
}UI_BUTTON_STATES;

typedef void (*UI_Action_Handler)(UI_BUTTON_ACTIONS action);

typedef union
{
	struct
	{
		unsigned char foot1:1;
		unsigned char foot2:1;
		unsigned char foot3:1;
		unsigned char foot4:1;
		unsigned char foot5:1;
		unsigned char buffSwitch:1;
		unsigned char editButton:1;
	};
	struct
	{
		unsigned char foots:5;
		unsigned char :1;
		unsigned char :1;
	};
	struct
	{
	    unsigned char switchesByte:7;
	};
}Switches;

typedef struct
{
	UI_BUTTON_STATES currentState;
	Switches allSwitches;
	Switches beforeBounce; //Just in case
	UI_Action_Handler actionHandler;
}BTN_Data;




typedef enum
{
	UI_DISPLAY_SEND_INFO
}UI_DISPLAY_EVENTS;

typedef enum
{
	UI_DISPLAY_INIT_BIT=0,
	UI_DISPLAY_WRITE_BIT,
	UI_DISPLAY_FLIP_CLOCK,
	UI_DISPLAY_LATCH_EN
}UI_DISPLAY_CONTROL;

typedef enum
{
	UI_DISP_STATE_Idle,
	UI_DISP_STATE_SendingInfo
}UI_DISPLAY_STATES;

typedef union //TODO: Reorder according to the hardware wiring
{
	struct
	{
		unsigned int LEDBuffer:1;
		unsigned int LEDSwBuffer:1;
		unsigned int LEDPedal1:1;
		unsigned int LEDFoot1:1;
		
		unsigned int LEDPedal2:1;
		unsigned int LEDPedal3:1;
		unsigned int LEDPedal4:1;
		unsigned int LEDPedal5:1;
		unsigned int VolumeBar10:1;
		unsigned int VolumeBar9:1;
		unsigned int VolumeBar8:1;
		unsigned int LEDFoot2:1;
		
		unsigned int VolumeBar7:1;
		unsigned int VolumeBar6:1;
		unsigned int VolumeBar5:1;
		unsigned int VolumeBar4:1;
		unsigned int VolumeBar3:1;
		unsigned int VolumeBar2:1;
		unsigned int VolumeBar1:1;
		unsigned int LEDFoot3:1;
		
		unsigned int _7SegUp:1;
		unsigned int _7SegRightUp:1;
		unsigned int _7SegRightDown:1;
		unsigned int _7SegDown:1;
		unsigned int _7SegLeftDown:1;
		unsigned int _7SegLeftUp:1;
		unsigned int _7SegMiddle:1;
		unsigned int LEDEdit:1;
	};
	struct
	{
	    unsigned int displayWord:UI_DISP_WORD_SIZE;
	};
}Display;

typedef struct
{
	UI_DISPLAY_STATES currentState;
	Display bufferOut;
	unsigned char buffCount;
	unsigned char entry_flag;
	unsigned char control;
}DISP_Data;


void UI_Init();
void UI_Tasks();

void UI_ButtonsInit();
void UI_ButtonsStateMachine();
void UI_ButtonsExtEvent(UI_BUTTON_EVENTS extEvent);

void UI_DisplayInit();
unsigned char UI_DisplayEncoder(unsigned char _7seg, unsigned char volume, unsigned char pedals, unsigned char foots, unsigned char bufferSwitches, unsigned char edit, Display * disp_out);
void UI_DisplayStateMachine();
void UI_DisplayExtEvent(UI_DISPLAY_EVENTS extEvent, Display * eventData);

void UI_TIM2_ISR();
#endif