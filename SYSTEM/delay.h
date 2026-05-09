#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f0xx.h"

extern void delay_init(uint8_t SYSCLK);//滴答定时器初始化
extern void delay_us(uint32_t nus);//延时us
extern void delay_ms(uint16_t nms);//延时ms
extern uint32_t Get_SysTick(void);
extern uint32_t systick_timer_elapsed_us(uint32_t start_val) ;// 计算从开始到现在经过的时间（单位：微秒）
#endif
