#include "delay.h"

static int8_t  fac_us=0;//us

/*
功能：初始化系统滴答定时器（SysTick）
参数：
	SYSCLK：这是一个无符号8位整数，表示系统的时钟频率（单位为MHz）。
返回值：无
*/
void delay_init(uint8_t SYSCLK)
{
	SysTick->CTRL = SysTick_CTRL_COUNTFLAG_Msk |				//
									SysTick_CTRL_ENABLE_Msk;						//开启SYSTICK
	fac_us = SYSCLK / 8;								//1微秒的计数值

	//fac_ms = (uint16_t)(fac_us * 1000);	//??1????? 
}  

/*
功能：延时us
参数：
	nus：多少us延时
返回值：无
*/
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
/*
功能：延时ms
参数：
	nus：多少ms延时
返回值：无
*/
void delay_ms(uint16_t nms)
{	 		  	  
	delay_us((uint32_t)(nms * 1000));					//利用毫秒转微秒进行延时   	    
} 


uint32_t Get_SysTick(void)
{
	return SysTick->VAL;
}


uint32_t end_val,diff,time_us;

// 计算从开始到现在经过的时间（单位：微秒）
uint32_t systick_timer_elapsed_us(uint32_t start_val) 
{

	//获取结束时间戳
	end_val = Get_SysTick();

	if (end_val <= start_val) 
	{
		// 没有发生计数器回绕
		diff = start_val - end_val;
	} 
	else 
	{
		// 发生了计数器回绕
		diff = ((SysTick->LOAD) - end_val) + start_val;
	}
	
	//计算得出经过的us时间
	time_us=diff/fac_us;
	
	return time_us;
}
