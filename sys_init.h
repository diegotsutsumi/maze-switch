#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_

#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_conf.h"
#include "misc.h"
#include "bsp_config.h"
#include "user_interface.h"
#include "app.h"
#include "memory.h"

void SYS_Initialize();
void SYS_ClockInit();
void SYS_FlashInit();
void SYS_TimerInit();
void SYS_NVICInit();

#endif
