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

/* GPIO与ADC引脚定义 */
#define NTC_GPIO_PORT       GPIOB
#define NTC_GPIO_PIN        GPIO_Pin_0
#define NTC_GPIO_CLK        RCC_AHBPeriph_GPIOB
#define NTC_ADC             ADC1
#define NTC_ADC_CHANNEL     ADC_Channel_8
#define NTC_ADC_CLK         RCC_APB2Periph_ADC1
#define NTC_ADC_IRQn        ADC1_COMP_IRQn

/*参数说明*/
extern float T;//当前温度
extern float T_temp[5];//5秒内记录的温度，用于计算平均值
extern volatile uint8_t time_over_flag;//1s到达标志位
extern uint8_t time_count;//1s到达次

/*函数说明*/
extern void NTC_Init(void);                      // NTC初始化（调用ADC1_Init）
extern int HandleADCTemp(float *temp);              // 获取温度值(C)，返回0成功

#endif /* __NTC_H__ */
