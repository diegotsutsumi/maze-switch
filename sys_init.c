#include "sys_init.h"

void SYS_Initialize()
{
	SYS_ClockInit();
	BSP_Init();
	SYS_FlashInit();
	SYS_NVICInit();
	SYS_TimerInit();
	
	MEM_Init();
	UI_Init();
	APP_Init();
	
	TIM_Cmd(TIM2, ENABLE);
}

void SYS_ClockInit()
{
	RCC_DeInit();
	SYS_FlashInit();
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	while(RCC_GetSYSCLKSource() != 0x08)
	{
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
	RCC_APB2Periph_GPIOE, ENABLE);
}

void SYS_FlashInit()
{
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
}

void SYS_TimerInit()
{
	TIM_TimeBaseInitTypeDef tmrInit;
	
	//1ms Timer
	tmrInit.TIM_Prescaler=16;
	tmrInit.TIM_CounterMode = TIM_CounterMode_Down;
	tmrInit.TIM_Period=500;
	tmrInit.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2,&tmrInit);
	TIM_PrescalerConfig(TIM2, 16, TIM_PSCReloadMode_Immediate);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	
	TIM_Cmd(TIM2, DISABLE);
}

void SYS_NVICInit()
{
	NVIC_InitTypeDef NVIC_InitStructure;  

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // O que é isso?

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}