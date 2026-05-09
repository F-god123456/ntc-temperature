#ifndef __POWEY_H__
#define __POWER_H__

/*电源外设配置信息*/
#define Power_RCC    RCC_AHBPeriph_GPIOB
#define Power_Pin    GPIO_Pin_5
#define Power_Port   GPIOB

/*电源读取IO操作*/
#define powerStatusPin    GPIO_ReadInputDataBit(POWER_GPIO,POWER_PIN)

typedef struct
{
 unsigned char	 State;            //传感器当前状态
 unsigned int Con_Time;         //传感器连接时间
 unsigned int Discon_Time;      //传感器断开时间
}Power;


//定义电源的防抖时间
#define POWERANTISHAKETIME      250

//电池延时打开时间
#define batteryDelayedOpenTime  250    //250*20ms=5000ms 约

//电池延时关闭时间
#define batteryDelayedCloseTime 500		//约15s关闭



//电池关闭延时时间
extern unsigned int batteryDelayedClose;
//更新电源状态标志位
extern unsigned char updataPowerStatusFlag;
//电源状态
extern Power power;

extern void Power_Init(void);//工作灯初始化

extern void timerMonitorPowerStatus(void);//定时器监测电源状态

#endif


