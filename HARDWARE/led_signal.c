#include "main.h"

static	GPIO_InitTypeDef  GPIO_InitStructure;
/*********************************
功能：工作灯初始化
参数：无
返回：无
**********************************/
void Led_signal_Init(void)
{
	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Led_signal_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Led_signal_Pin;					//
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_OUT;			//配置PB0为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度为50MHZ
	GPIO_Init(Led_signal_Port, &GPIO_InitStructure);

}