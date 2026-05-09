#ifndef _USART1_H
#define _USART1_H

#include "stm32f0xx.h"
#include <stdio.h>


//串口接收缓存区数组长度
#define rcvBufLen 250

extern uint8_t head;
//串口数据尾部
extern uint8_t trail;
//串口接收缓存区
extern uint8_t rcvBuf[rcvBufLen];


void USART1_Init(uint32_t baud);//串口1初始化

void uart1_send(uint8_t *buf);//串口1发送字符串
void Init_Uart_Variables_4G(void);//初始化4G模块串口变量
void Init_4G_Hardware(void);//初始化串口和串口变量


#endif
