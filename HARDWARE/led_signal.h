#ifndef __LED_SIGNAL_H
#define __LED_SIGNAL_H



#define Led_signal_RCC    RCC_AHBPeriph_GPIOB
#define Led_signal_Pin    GPIO_Pin_8
#define Led_signal_Port   GPIOB

#define Led_auto_OFF     GPIO_WriteBit(Led_signal_Port,Led_signal_Pin,Bit_SET)
#define Led_auto_On      GPIO_WriteBit(Led_signal_Port,Led_signal_Pin,Bit_RESET)

extern void Led_signal_Init(void);



#endif


