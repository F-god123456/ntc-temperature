#include "main.h"

static	GPIO_InitTypeDef  GPIO_InitStructure;

/*********************************
功能：工作灯初始化
参数：无
返回：无
**********************************/
void battery_Init(void)
{
	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Battery_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Battery_Pin;					//
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_OUT;			//配置PB0为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度为50MHZ
	GPIO_Init(Battery_Port, &GPIO_InitStructure);

}

/*****************************************************
函数名称：备用电池打开
输入参数：无
输出参数：无
*****************************************************/
void batteryOpen(void)
{
	batteryControlPin(1);
}

/*****************************************************
函数名称：备用电池关闭
输入参数：无
输出参数：无
*****************************************************/
void batteryClose(void)
{
	batteryControlPin(0);
}

/*****************************************************
函数名称：定时监测电池，延时开启或者关闭电池
输入参数：无
输出参数：无
*****************************************************/
void timerMonitorBattery(void)
{
	//延时打开后备电池
	if(batteryDelayedClose != 0)
	{
		batteryDelayedClose--;
		if(batteryDelayedClose == 0)
		{
			batteryClose();
		}
	}
}
