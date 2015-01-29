#include "memory.h"

MEM_Data memData;

void MEM_Init()
{
	memData.addrCount=0;
	memData.setReset=0;
	memData.slotToWrite.memoryHWord=0;
	
	for(int i=0;i<512;i++)
	{
		memData.flashMirror[i].memoryHWord=0;
	}

	memData.entryFlag=0;
	MEM_ChangeState(MEM_STATE0_FetchingFlash,MEM_STATE1_None);
}

void MEM_ExternalEvent(MEM_EVENTS extEvent, unsigned char bank, unsigned char preset, MemorySlot * data)
{
	switch(extEvent)
	{
		case MEM_EVENT_SavePreset:
		{
			int idx;
			if(memData.currentState.lvl0==MEM_STATE0_Idle)
			{
				idx = MEM_GetIndex(bank, preset);
				if(idx==-1 || data==0)
				{
					return;
				}

				if((*eventData).Volume==VOLUME_ZERO_GAIN && (*eventData).RelayChunk==0)
				{
					memData.flashMirror[idx].Volume=VOLUME_ZERO_GAIN;
					memData.flashMirror[idx].RelayChunk=0;
					memData.flashMirror[idx].SlotWritten=0;
					MEM_ChangeState(MEM_STATE0_Saving,MEM_STATE1_ErasingPage);
				}
				else
				{
					memData.flashMirror[idx].memoryHWord = (*eventData).memoryHWord;
					MEM_ChangeState(MEM_STATE0_Saving,MEM_STATE1_ErasingPage);
				}
			}
		}
		break;
		
		case MEM_EVENT_SaveActive:
		{
			if(memData.currentState.lvl0==MEM_STATE0_Idle)
			{
				int idx;
				idx = MEM_GetIndex(bank, preset);
				if(idx==-1)
				{
					return;
				}

				if(memData.flashMirror[idx].SlotWritten)
				{
					memData.flashMirror[0].memoryHWord = memData.flashMirror[idx].memoryHWord;
					MEM_ChangeState(MEM_STATE0_Saving,MEM_STATE1_ErasingPage);	
				}
				else
				{
					memData.flashMirror[0].Volume=VOLUME_ZERO_GAIN;
					memData.flashMirror[0].RelayChunk=0;
					memData.flashMirror[0].SlotWritten=1;
					MEM_ChangeState(MEM_STATE0_Saving,MEM_STATE1_ErasingPage);
				}
			}
		}
		break;
	}
}

int MEM_GetIndex(unsigned char bank, unsigned char preset)
{
	if((bank>9 || bank==0) || (preset>3 || preset==0))
	{
		return -1;
	}
	
	return (3*bank + preset - 4);
}

unsigned int MEM_GetAddress(int index)
{
	if(index>511)
	{
		return 999;
	}
	
	return FLASH_LAST_PAGE + (index*2);
}

void MEM_ChangeState(MEM_STATES_LVL0 a, MEM_STATES_LVL1 b)
{
	memData.currentState.lvl0 = a;
	memData.currentState.lvl1 = b;
}

unsigned char MEM_GetPreset(unsigned char bank, unsigned char preset, MemorySlot * out)
{
	int idx = MEM_GetIndex(bank, preset);
	if(idx=-1)
	{
		return 0;
	}

	if(memData.flashMirror[idx].SlotWritten)
	{
		(*out).memoryHWord = memData.flashMirror[idx].memoryHWord;
	}
	else
	{
		(*out).memoryHWord = memData.flashMirror[511].memoryHWord; //The last position is the default preset (all relays off)
	}

	return 1;
}

void MEM_SetActionHandler(MEM_SetActionHandler _handler)
{
	if(!_handler)
	{
		memData.handler = _handler;
	}
}

void MEM_Tasks()
{
	switch(memData.currentState.lvl0)
	{
		case MEM_STATE0_FetchingFlash:
		{
			if(memData.addrCount<512)
			{
				memData.flashMirror[memData.addrCount].memoryHWord = *((unsigned short *)(MEM_GetAddress(memData.addrCount)));
				memData.addrCount++;
			}
			else
			{
				MEM_ChangeState(MEM_STATE0_Idle,MEM_STATE1_None);
				memData.handler(MEM_ACTION_FlashReady);
			}
		}
		break;

		case MEM_STATE0_Idle:
		{
		}
		break;

		case MEM_STATE0_Saving:
		{
			switch(memData.currentState.lvl1)
			{
				case MEM_STATE1_ErasingPage:
				{
					memData.hwStatus = FLASH_GetStatus();
					if(memData.hwStatus==FLASH_COMPLETE)
					{
						if(memData.entryFlag)
						{
							FLASH->CR |= CR_PER_Set;
							FLASH->AR = FLASH_LAST_PAGE; 
							FLASH->CR |= CR_STRT_Set;
							
							memData.entryFlag=0;
						}
						else
						{
							FLASH->CR &= CR_PER_Reset;
							memData.entryFlag=1;
							memData.addrCount=0;
							memData.setReset=1;
							MEM_ChangeState(MEM_STATE0_SavingActive,MEM_STATE1_ProgrammingPage);
							memData.handler(MEM_ACTION_SavingComplete);
						}
					}
					else
					{
						if(memData.hwStatus==FLASH_ERROR_PG || memData.hwStatus==FLASH_ERROR_WRP)
						{
							MEM_ChangeState(MEM_STATE0_Error,MEM_STATE1_None);
						}
					}
				}
				break;
				case MEM_STATE1_ProgrammingPage:
				{
					memData.hwStatus = FLASH_GetStatus();
					if(memData.hwStatus==FLASH_COMPLETE)
					{
						if(memData.setReset)
						{
							FLASH->CR |= CR_PG_Set;
							*((__IO uint16_t*)(MEM_GetAddress(memData.addrCount))) = memData.flashMirror[memData.addrCount];
							memData.setReset=0;
						}
						else
						{
							FLASH->CR &= CR_PG_Reset;
							memData.addrCount++;
							memData.setReset=1;
							if(memData.addrCount>=512) //TODO: Implement memData.usedSlotsNumber
							{
								memData.entryFlag=1;
								MEM_ChangeState(MEM_STATE0_Idle,MEM_STATE1_None);
								memData.handler(MEM_ACTION_SavingComplete);
							}
						}
					}
					else
					{
						if(memData.hwStatus==FLASH_ERROR_PG || memData.hwStatus==FLASH_ERROR_WRP)
						{
							MEM_ChangeState(MEM_STATE0_Error,MEM_STATE1_None);
						}
					}
				}
				break;
				default:
				{
					MEM_ChangeState(MEM_STATE0_Error,MEM_STATE1_None);
				}
				break;
			}
		}
		break;

		case MEM_STATE0_Error:
		{
			if(memData.entryFlag)
			{
				memData.handler(MEM_ACTION_Error);
				memData.entryFlag=0;
			}
		}
		break;

		default:
		{
		}
		break;
	}
}