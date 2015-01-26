#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "bsp_config.h"

#define UI_BTN_DEBOUNCING_TIME 3
#define UI_BTN_CANCEL_TIME 2000

#define UI_DISP_BLINKING_TIME 500
#define UI_DISP_WORD_SIZE 28
#define UI_DISP_ALL_MASK 0xFFFFFFF
#define UI_DISP_7SEG_MASK 0x7F00000
#define UI_DISP_VOL_MASK 0x7F700
#define UI_DISP_PEDAL_MASK 0xF4
#define UI_DISP_FOOT_MASK 0x80808
#define UI_DISP_BUFFER_MASK 0x3

typedef enum
{
	FOOT_1_PRESSED,
	FOOT_2_PRESSED,
	FOOT_3_PRESSED,
	FOOT_LEFT_PRESSED,
	FOOT_RIGHT_PRESSED,
	BUFFER_SWITCHED_ON,
	BUFFER_SWITCHED_OFF,
	SAVE_OK,
	SAVE_CANCEL,
	ERROR
}UI_BUTTON_ACTIONS;

typedef enum
{
	LOCK, //External
	RELEASE, //External
}UI_BUTTON_EVENTS;

typedef enum
{
	UI_BTN_STATE_ListeningButtons,
	UI_BTN_STATE_Debouncing,
	UI_BTN_STATE_WaitingRelease,
	UI_BTN_STATE_CountingEditButton
}UI_BUTTON_STATES;

typedef void (*UI_ActionHandler)(UI_BUTTON_ACTIONS action);

typedef union
{
	struct
	{
		unsigned char foot1:1;
		unsigned char foot2:1;
		unsigned char foot3:1;
		unsigned char footLeft:1;
		unsigned char footRight:1;
		unsigned char buffSwitch:1;
	};
	struct
	{
	    unsigned char switchesByte:6;
	};
}Switches;

typedef struct
{
	UI_BUTTON_STATES currentState;
	Switches switchesState;
	Switches switchesNewState;
	unsigned char editButton
	UI_ActionHandler actionHandler;
	unsigned short timerDebounce;
	unsigned int editTimer;
	unsigned char editTimerEnabled;
}BTN_Data;




typedef enum
{
	UI_DISPLAY_ALL=0,
	UI_DISPLAY_7SEG,
	UI_DISPLAY_VOLUME,
	UI_DISPLAY_LEDPEDAL,
	UI_DISPLAY_LEDFOOT,
	UI_DISPLAY_LEDBUFFER,
	UI_DISPLAY_CUSTOM
}UI_DISPLAY_PARTS;

typedef enum
{
	UI_DISPLAY_UPDATE,
	UI_DISPLAY_START_BLINKING,
	UI_DISPLAY_STOP_BLINKING
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

typedef union
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
	
	Display currentDisp;
	Display whichBlinks;
	Display bufferBlink;
	Display bufferOut;
	
	unsigned char buffCount;
	unsigned char entry_flag;
	UI_DISPLAY_CONTROL control;
	
	unsigned char updatePending;
	
	unsigned int blinkTimer;
	unsigned char blinkTimerEnabled;
	unsigned char blinkTurn;
}DISP_Data;


void UI_Init();
void UI_Tasks();

void UI_ButtonsInit();
void UI_ButtonsStateMachine();
void UI_ButtonsExtEvent(UI_BUTTON_EVENTS extEvent);

void UI_DisplayInit();
unsigned char UI_DisplayEncoder(unsigned char _7seg, unsigned char volume, unsigned char pedals, unsigned char foots, unsigned char bufferSwitches, unsigned char edit, Display * disp_out);
void UI_DisplayStateMachine();
void UI_DisplayExtEvent(UI_DISPLAY_EVENTS extEvent, void * eventData, void * eventData2);

void UI_TIM2_ISR();
#endif