#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "stm32f10x_flash.h"

#define FLASH_LAST_PAGE 0x0801FC00 //For STM32F103C8T6
#define VOLUME_ZERO_GAIN 5

typedef enum
{
	MEM_STATE0_FetchingFlash,
	MEM_STATE0_Idle,
	MEM_STATE0_Saving,
	MEM_STATE0_Error
}MEM_STATES_LVL0;

typedef enum
{
	MEM_STATE1_ErasingPage,
	MEM_STATE1_ProgrammingPage,
	MEM_STATE1_None
}MEM_STATES_LVL1;

typedef struct
{
	MEM_STATES_LVL0 lvl0;
	MEM_STATES_LVL1 lvl1;
}MEM_STATE;

typedef enum
{
	MEM_EVENT_SavePreset,
	MEM_EVENT_SaveActive,
}MEM_EVENTS;

typedef enum
{
	MEM_ACTION_FlashReady,
	MEM_ACTION_SavingComplete,
	MEM_ACTION_Error
}MEM_ACTIONS;

typedef void (*MEM_ActionHandler)(MEM_ACTIONS action);

typedef union
{
	struct
	{
		unsigned short Volume:4;
		
		unsigned short Relay6:1;
		unsigned short Relay5:1;
		unsigned short Relay4:1;
		unsigned short Relay3:1;
		unsigned short Relay2:1;
		unsigned short Relay1:1;

		unsigned short SlotWritten:1;
	};
	struct
	{
		unsigned short :4;

		unsigned short RelayChunk:6;

		unsigned short SlotWritten:1;
	};
	struct
	{
	    unsigned short memoryHWord;
	};
}MemorySlot;

typedef struct
{
	MEM_STATES currentState;
	MemorySlot slotToWrite;
	MemorySlot flashMirror[512];
	
	FLASH_Status hwStatus;
	
	unsigned char setReset;
	unsigned char entryFlag;
	
	unsigned int addrCount;

	MEM_ActionHandler handler;
	
}MEM_Data;


void MEM_Init();
void MEM_ExternalEvent(MEM_EVENTS extEvent, void * eventData);
int MEM_GetIndex(unsigned char bank, unsigned char preset);
unsigned int MEM_GetAddress(int index);
void MEM_ChangeState(MEM_STATES_LVL0 a, MEM_STATES_LVL1 b);
void MEM_SetActionHandler(MEM_ActionHandler handler);
void MEM_Tasks(MEM_STATES_LVL0 a, MEM_STATES_LVL1 b);


#endif