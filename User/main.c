#include "stm32f0xx.h"
#include "delay.h"
#include "led.h"
#include "USART1.h"
#include <stdio.h>
#include <string.h>
#include "user_adc.h"

int main(void)
{
	uint32_t temp;
	USART1_Init(9600); // 初始化串口，并配置波特率为9600bps
	delay_init(48);	   // 初始化SYSTICK

	ADC1_Init();
	ADC_GPIO_Config(GPIO_Pin_0);
	printf("\r\nUSART串口发送_中断接收_测试程序演示->->->\r\n\r\n");
	while (1)
	{

		temp = HandleADC();

		if (NTC_tempFlag)
			printf("\r\n NTC temp: -%dC\r\n", temp);
		else
			printf("\r\n NTC temp: %dC\r\n", temp);

		delay_ms(1000);
	}
}
