#ifndef _USER_ADC_H
#define _USER_ADC_H

#include "stm32f0xx.h"


#define  NUMSAMP   640
#define  NUMHEAD   40
#define  TEMPMAX   4090
#define  TEMPMIN   0

extern volatile unsigned int ADC_Buf[NUMSAMP + 1]; // 放单片机ADC采集原始值
extern volatile unsigned int ADC_BufCnt ;		// 采集量变量
extern volatile unsigned char NTC_tempFlag ;	// 负温标志



/* ADC 初始化 */
extern void ADC1_Init(void);

/* 配置 ADC 通道对应的 GPIO 为模拟输入 */
extern void ADC_GPIO_Config(uint32_t GPIO_Pin);

/* 配置 ADC 转换通道并读取转换结果 */
extern uint16_t ADC_ReadChannel(uint32_t ADC_Channel);

/* 获取电压值（mV），参考电压 3.3V */
extern uint32_t ADC_GetVoltage_mV(uint16_t adc_value);
/*******************************************************************************
 * 将采集的原始值计算为温度值
 * 入口参数：无
 * 返回值  ：温度值（有符号范围 -54 ~ 125）
 ******************************************************************************/
extern unsigned char HandleADC(void);


#endif
