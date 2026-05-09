#ifndef __NTC_H__
#define __NTC_H__

#include "stm32f0xx.h"
#include <math.h>
#include <stdio.h>
#define  NUMSAMP   640
#define  NUMHEAD   40
#define  TEMPMAX   4090
#define  TEMPMIN   0

#define  T_MAX     120
#define  T_MIN     -40

/*参数说明*/
extern float T;//当前温度
extern float T_temp[5];//5秒内记录的温度，用于计算平均值
extern volatile uint8_t time_over_flag;//1s到达标志位
extern uint8_t time_count;//1s到达次

/*函数说明*/
extern void NTC_Init(void);                      // NTC初始化（调用ADC1_Init）
extern int HandleADCTemp(float *temp);              // 获取温度值(C)，返回0成功

#endif /* __NTC_H__ */
