#ifndef __BATTERY_H__
#define __BATTERY_H__

/*电池外设配置信息*/
#define Battery_RCC    RCC_AHBPeriph_GPIOB
#define Battery_Pin    GPIO_Pin_6
#define Battery_Port   GPIOB

//后备电池控制管脚
#define batteryControlPin(x) GPIO_WriteBit(Battery_Port,Battery_Pin,(BitAction)x)

extern void battery_Init(void) ;//工作灯初始化
extern void batteryOpen(void);//电池开关打开
extern void batteryClose(void);//电池开关关闭
extern void timerMonitorBattery(void);//定时监测电池，延时开启或者关闭电池
#endif

