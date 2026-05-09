#ifndef __BEEP_H
#define __BEEP_H

/*配置喇叭GPIO的信息*/
#define Beep_RCC    RCC_AHBPeriph_GPIOF
#define Beep_Pin    GPIO_Pin_7
#define Beep_Port   GPIOF

extern bool beep_state;//蜂鸣器状态

extern void Beep_Init(void);//工作灯初始化
extern void BEEP_ON(void);//打开喇叭
extern void BEEP_OFF(void);//关闭喇叭
void handleBeepState(uint8_t *bcout); //处理蜂鸣器状态。此函数用于在按键扫描过程中更新蜂鸣器状态。当蜂鸣器需要响铃时，该函数将计数，并在达到一定次数后关闭蜂鸣器。
#endif


