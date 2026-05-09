#include "main.h"

static	GPIO_InitTypeDef  GPIO_InitStructure;


/*电源状态管脚
 *高电平1:断电
 *低电平0:有电
*/
#define powerStatusPin    GPIO_ReadInputDataBit(POWER_GPIO,POWER_PIN)

//电池延时打开
unsigned int batteryDelayedOpen = batteryDelayedOpenTime;
//电池延时关闭
unsigned int batteryDelayedClose = 0;

//电源状态
Power power;

//更新电源状态标志位
unsigned char updataPowerStatusFlag = 0;

/*********************************
功能：工作灯初始化
参数：无
返回：无
**********************************/
void Power_Init(void)
{
	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Power_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Power_Pin;					//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Power_Port, &GPIO_InitStructure);
	
	battery_Init();			//电池初始化
	 //关闭电池
	batteryControlPin(0);
}

/*****************************************************
函数名称：定时器监测电源状态
输入参数：无										                 
输出参数：无									   	               
*****************************************************/
void timerMonitorPowerStatus(void)
{
	if(GPIO_ReadInputDataBit(Power_Port,Power_Pin) == 0) //有电
	{
		power.Discon_Time = 0;
		//断开时间清零
		if(power.Con_Time<POWERANTISHAKETIME)           //计时接通的时间
		{
			power.Con_Time++;
		}
		else                                            //接通时间超过1秒
		{
			if(power.State!=1)                          //如果状态不是接通
			{
				power.State=1;                          //设置状态为接通
				batteryDelayedClose = 0;
				updataPowerStatusFlag=1;                         //统一更新上传状态
			}
		}
	}
	else//无电
	{ 
		power.Con_Time=0;
		//接通时间清零
		if(power.Discon_Time<POWERANTISHAKETIME)         //计时断开的时间   
		{
			power.Discon_Time++;
		}
		else                                            //断开时间超过1秒                
		{
			if(power.State!=0)                             //如果状态不是断开
			{
				power.State=0;                          //设置状态为断开
				batteryDelayedClose = batteryDelayedCloseTime;             //断电，电池延时20秒后关闭
				updataPowerStatusFlag=1;                //统一更新上传检测端状态
			}
		}
	}
}
