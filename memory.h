#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "stm32f10x_flash.h"

typedef union
{
	struct
	{
		unsigned short Bank:7;

		unsigned short Volume:4;
		
		unsigned short Pedal1:1;
		unsigned short Pedal2:1;
		unsigned short Pedal3:1;
		unsigned short Pedal4:1;
		unsigned short Pedal5:1;
	};
	struct
	{
		unsigned short :7;

		unsigned short :4;

		unsigned short PedalChunk:5;
	};
	struct
	{
	    unsigned short memoryHWord;
	};
}Memory;
#endif