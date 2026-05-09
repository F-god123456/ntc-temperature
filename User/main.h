#ifndef __MAIN_H__
#define __MAIN_H__

//系统自带库文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_flash.h"
#include "stm32f0xx_iwdg.h"
#include <math.h>
#include "stdbool.h"

//用户自带文件
#include "key.h"
#include "delay.h"
#include "led.h"
#include "USART1.h"
#include "Timer.h"
#include "menu.h"
#include "Debug.h"
#include "ntc.h"
#include "relay.h"
#include "digitaltube.h"
#include "led_auto.h"
#include "Flash.h"
#include "cmd_handle.h"
#include "iwdg.h"
#include "beep.h"
#include "cmd_str_handle.h"
#include "cmd_data_up.h"
#include "data_save.h"
#include "battery.h"
#include "power.h"
#include "process.h"

extern void PrintUserData(_FLASH_Save_User_Data *userData);

#endif


