#include "main.h"

/*状态值*/
enum{CLOSE=0,OPEN};

uint8_t switch_state;//继电器状态，1：开启，0关闭

/*********************************
功能：工作灯初始化
参数：无
返回：无
**********************************/
void Relay_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Relay_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Relay_Pin;					//
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_OUT;			//配置PB0为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度为50MHZ
	GPIO_Init(Relay_Port, &GPIO_InitStructure);

}

/*********************************
功能：继电器开关关闭
参数：无
返回：无
**********************************/
void RelayClose(void)
{
	Relay_OFF;
	switch_state=0;

}

/*********************************
功能：继电器开关打开
参数：无
返回：无
**********************************/
void RelayOpen(void)
{
	Relay_On;
	switch_state=1;

}

/*********************************
功能：继电器开关控制
参数：无
返回：无
**********************************/
void ControlRelay(uint8_t RelayStatus)
{
	//直接操作继电器开关
	if(RelayStatus == 0)//关闭
	{
		RelayClose();
	}
	else if(RelayStatus == 1)//打开
	{
		RelayOpen();
		
	}
	upSwitchStatusFlag = 1;
}
/*********************************
功能：翻转继电器开关控制
参数：无
返回：无
**********************************/
void RevRelay(void)
{	
	ControlRelay(switch_state^1);
}

