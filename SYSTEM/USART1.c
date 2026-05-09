#include "main.h"


uint8_t head = 0;//解析命令数组索引
uint8_t trail = 0;//串口数据缓冲区下标
uint8_t rcvBuf[rcvBufLen];//串口数据缓冲区


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7)))
  {
		
	}
  USART1->TDR = ch;
  return (ch);
}
#endif 

/********************************* 
功能：USART初始化 
参数：baud 波特率
返回：无
*********************************/
void USART1_Init(uint32_t baud)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = baud;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;   //奇偶位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //控制位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //接受模式
	USART_Init(USART1, &USART_InitStructure);	


	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	//USART_ITConfig(USART1, USART_IT_ORE, ENABLE);          //使能串口溢出中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}

/*****************************************************
函数名称:串口1中断服务函数
输入参数:无
输出参数:无
*****************************************************/
void USART1_IRQHandler(void)
{
// 	if (USART_GetITStatus(USART1, USART_IT_ORE) == SET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_ORE);
//        USART_ReceiveData( USART1 ); //这句一定要加
//	}
//	if (USART_GetITStatus(USART1, USART_IT_PE) == SET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_PE);
//        USART_ReceiveData( USART1 ); //这句一定要加
//	}
//	if (USART_GetITStatus(USART1, USART_IT_FE) == SET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_FE);
//        USART_ReceiveData( USART1 ); //这句一定要加
//	}
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//检查 USART1 接收中断是否被触发
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		if(head <= trail)                                  // 0-------头-------尾--------rcvBufLen
		{
			//从头开始接收
			if(trail + 1 >= rcvBufLen)                       // 超出最大值，循环开始
			{
				if((trail + 1 - rcvBufLen) == head)            // 尾部数据+1的地址是头部数据，说明缓存满了
				{
					return;
				}
			}
			//接收数据
			rcvBuf[trail] = USART_ReceiveData(USART1);
			trail++;
			
			if( trail >= rcvBufLen )
			{
				trail = trail - rcvBufLen;
			}
		}
		else//head > trail
		{                                       						// 0-------尾------头---------rcvBufLen
			if((trail + 1) == head)                          	// 尾部数据+1的地址是头部数据，说明缓存满了
			{
				return;
			}
			//接收数据
			rcvBuf[trail] = USART_ReceiveData(USART1);
			trail++;
			if( trail >= rcvBufLen )
			{
				trail = trail - rcvBufLen;
			}
		}
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}

/*****************************************************
函数名称:串口发送一个数据
输入参数:byte:需要发送的数据
输出参数:无
*****************************************************/
void uart_send_byte(uint8_t byte)
{
	USART_SendData(USART1 , byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/*****************************************************
函数名称:
函数功能:串口1发送字符串
输入参数:buf:需要发送的字符串的指针
输出参数:无
*****************************************************/
void uart1_send(uint8_t *buf)
{
	while(*buf!=0)
	{
		uart_send_byte(*buf++);
	}
}


