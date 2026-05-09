#include "main.h"

bool beep_state=0;//蜂鸣器状态
/***************************
功能：蜂鸣器初始化
参数：无
返回：无
***************************/
void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Beep_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Beep_Pin;					//
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_OUT;			//配置PB0为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度为50MHZ
	GPIO_Init(Beep_Port, &GPIO_InitStructure);
}
/***************************
功能：开启蜂鸣器
参数：无
返回：无
***************************/
void BEEP_ON(void)
{

	GPIO_WriteBit(Beep_Port,Beep_Pin,Bit_SET);
}
/***************************
功能：关闭蜂鸣器
参数：无
返回：无
***************************/
void BEEP_OFF(void)
{

	GPIO_WriteBit(Beep_Port,Beep_Pin,Bit_RESET);
}
/*
*********************************************************************************************************
*	函 数 名: handleBeepState
*	功能说明: 处理蜂鸣器状态。此函数用于在按键扫描过程中更新蜂鸣器状态。
*	           当蜂鸣器需要响铃时，该函数将计数，并在达到一定次数后关闭蜂鸣器。
*	形    参: bcout : 指向蜂鸣器状态计数器的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void handleBeepState(uint8_t *bcout) 
{
    (*bcout)++;
    if (*bcout == 2) 
	{
        beep_state = 0;
        BEEP_OFF();
        *bcout = 0;
    }
}
