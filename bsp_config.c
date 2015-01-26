#include "bsp_config.h"
void BSP_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Relay1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay1_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Relay2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay2_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Relay3_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay3_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Relay4_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay4_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Relay5_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay5_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Relay6_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Relay6_Port, &GPIO_InitStructure);
	
	
	
	/*GPIO_InitStructure.GPIO_Pin = Footswitch1_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Footswitch1_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Footswitch2_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Footswitch2_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Footswitch3_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Footswitch3_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FootswitchLeft_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FootswitchLeft_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FootswitchRight_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FootswitchRight_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SaveButton_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SaveButton_port, &GPIO_InitStructure);*/
	
	
	
	GPIO_InitStructure.GPIO_Pin = Display_LatchEnable_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Display_LatchEnable_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Display_SerialIn_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Display_SerialIn_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Display_Clock_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Display_Clock_Port, &GPIO_InitStructure);
}