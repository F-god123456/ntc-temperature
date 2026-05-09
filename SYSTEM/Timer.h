#ifndef __TIMER_H__
#define __TIMER_H__

extern int ten_ms_count;//5s翻转在数码管显示温湿度的计时器
extern int TH_ten_ms_count;//读取温湿度的计时器
extern int sensor_time;//传感器计时器

extern void TIM3_Init(uint16_t psc , uint16_t arr);//定时器3初始化
extern void TIM14_Init(uint16_t psc , uint16_t arr);//定时器14初始化
extern void TIM15_Init(uint16_t psc , uint16_t arr);//定时器15初始化
extern void TIM16_Init(uint16_t psc , uint16_t arr);//定时器16初始化
extern void timer_init(void);//全部定时器初始化

#endif


