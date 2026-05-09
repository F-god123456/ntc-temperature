#include "delay.h"

static int8_t  fac_us=0;//us
//static int16_t fac_ms=0;//ms

void delay_init(uint8_t SYSCLK)
{
	SysTick->CTRL = SysTick_CTRL_COUNTFLAG_Msk |				//
									SysTick_CTRL_ENABLE_Msk;						//开启SYSTICK
	fac_us = SYSCLK / 8;								//1微秒的计数值

	//fac_ms = (uint16_t)(fac_us * 1000);	//??1????? 
}  
//延时Nus
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told , tnow , tcnt;
	uint32_t reload;			
	
	reload = SysTick->LOAD;			//LOAD的值
	ticks = nus * fac_us; 			//要计时的总半数值
	tcnt = 0;
	told = SysTick->VAL;        //当前计数值
	
	while(1)
	{
		tnow = SysTick->VAL;	    //获取最新时间计数值
		if(tnow != told)
		{	    
			if(tnow < told)
			{
				tcnt += told - tnow;		//这里注意一下SYSTICK是个递减计数器
			}
			else 
			{
				tcnt += reload - tnow + told;
			}
	    
			told = tnow;
			
			if(tcnt >= ticks)
			{
				break;				//计数时间超过/等于要延时时间到跳出循环
			}
		}  
	}
}

void delay_ms(uint16_t nms)
{	 		  	  

	delay_us((uint32_t)(nms * 1000));					//利用毫秒转微秒进行延时   	    
} 
