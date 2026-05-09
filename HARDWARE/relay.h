#ifndef __RELAY_H
#define __RELAY_H

//继电器的GPIO配置信息
#define Relay_RCC    RCC_AHBPeriph_GPIOF
#define Relay_Pin    GPIO_Pin_6
#define Relay_Port   GPIOF

/*继电器开和闭动作*/
#define Relay_On      GPIO_WriteBit(Relay_Port,Relay_Pin,Bit_SET)
#define Relay_OFF     GPIO_WriteBit(Relay_Port,Relay_Pin,Bit_RESET)


extern uint8_t switch_state;//继电器状态，1：开启，0关闭

extern void Relay_Init(void);//工作灯初始化
extern void ControlRelay(uint8_t RelayStatus);//控制继电器开闭
extern void SwitchExecuteTask(void);//上电执行继电器的动作
extern void RevRelay(void);//继电器工作状态翻转

#endif


