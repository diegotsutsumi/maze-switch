#include "stm32f10x_gpio.h"

#define Relay1_Pin	GPIO_Pin_10
#define Relay1_Port	GPIOB
#define Relay2_Pin	GPIO_Pin_11
#define Relay2_Port	GPIOB
#define Relay3_Pin	GPIO_Pin_12
#define Relay3_Port	GPIOB
#define Relay4_Pin	GPIO_Pin_13
#define Relay4_Port	GPIOB
#define Relay5_Pin	GPIO_Pin_14
#define Relay5_Port	GPIOB
#define Relay6_Pin	GPIO_Pin_15
#define Relay6_Port	GPIOB
#define Relay_Port GPIOB

#define Display_LatchEnable_Pin		GPIO_Pin_2
#define Display_LatchEnable_Port	GPIOB
#define Display_SerialIn_Pin		GPIO_Pin_0
#define Display_SerialIn_Port		GPIOB
#define Display_Clock_Pin			GPIO_Pin_1
#define Display_Clock_Port			GPIOB

#define Footswitch1_pin			GPIO_Pin_0
#define Footswitch1_port		GPIOA
#define Footswitch2_pin			GPIO_Pin_1
#define Footswitch2_port		GPIOA
#define Footswitch3_pin			GPIO_Pin_2
#define Footswitch3_port		GPIOA
#define FootswitchLeft_pin		GPIO_Pin_3
#define FootswitchLeft_port		GPIOA
#define FootswitchRight_pin		GPIO_Pin_4
#define FootswitchRight_port	GPIOA

#define BufferSwitch_pin		GPIO_Pin_0 //TODO: Verify with the Hardware
#define BufferSwitch_port		GPIOA //TODO: Verify with the hardawre

#define EditButton_pin		GPIO_Pin_8
#define EditButton_port		GPIOA

void BSP_Init();