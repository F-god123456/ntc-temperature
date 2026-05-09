#ifndef __LED_AUTO_H
#define __LED_AUTO_H

/*配置自动/手动模式灯外设的信息*/
#define Led_auto_RCC    RCC_AHBPeriph_GPIOB
#define Led_auto_Pin    GPIO_Pin_8
#define Led_auto_Port   GPIOB

/*自动模式/手动模式IO口操作*/
#define Led_auto_OFF     GPIO_WriteBit(Led_auto_Port,Led_auto_Pin,Bit_SET)
#define Led_auto_On      GPIO_WriteBit(Led_auto_Port,Led_auto_Pin,Bit_RESET)

extern uint8_t Led_Auto_State;//工作模式灯状态，1为手动模式，0为自动模式

extern void Led_auto_Init(void);//工作灯初始化
extern void Led_Auto_Control(uint8_t state);//控制灯的开关
extern void Led_Auto_Toggle(void) ;//翻转LED自动模式状态
extern void Led_Auto_Restore(void);//恢复自动模式灯状态
#endif


