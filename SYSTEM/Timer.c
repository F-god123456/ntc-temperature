#include "main.h"

static  NVIC_InitTypeDef			NVIC_InitStructure; 
static	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;         
	
int ten_ms_count=0;//5s翻转在数码管显示温湿度的计时器
int TH_ten_ms_count=0;//读取温湿度的计时器
int sensor_time=0;//传感器计时器

/*
***功能：更新一个传感器时间倒计时重发。
***参数：
		sensor_time： 用于传递和更新传感器时间计数器的值。
***返回值：无
*/
void updateSensorTime(int *sensor_time) 
{
    if (Sensor_Hour_Falg == 1 && sensor_time != NULL) 
	{
        (*sensor_time)++;
        if(*sensor_time >= 50) //一秒到达 50
		{
            *sensor_time = 0;
            currentTime.seconds += 1;//秒数++ 
            if (currentTime.seconds >= 60) //60s到达  60
			{
                currentTime.seconds = 0;//清空记录秒数变量
                currentTime.milliseconds += 1;//分钟++

                if (currentTime.milliseconds >= 60) //1小时到达   60
				{
                    currentTime.milliseconds = 0;//清空分钟变量
					Sensor_Hour_Falg = 0;
                }
            }
        }
    }
}

/*
***功能：1S读取当前温湿度
***参数：
		time：用于传递和更新传感器时间计数器的值。
***返回值：无
*/
void Read_TH_Data(void)
{
	TH_ten_ms_count++;
	if(TH_ten_ms_count>=50&&time_over_flag==0)//1s  20ms*50 =1000ms  1s=1000ms
	{
		TH_ten_ms_count =0;
		//1s读取温湿度数据完成标志位
		time_over_flag=1;

	}
}

/*****************************************
***功能：定时器3初始化
***参数： psc   预分频值
	      arr   自动重载值
***返回值：无	   
******************************************/
void TIM3_Init(uint16_t psc , uint16_t arr)
{     
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); //使能TIM3时钟   
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 		//TIM3通道中断注册
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 		//配置TIM3时钟频率除数的预分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = arr; 								//配置下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //配置时钟分割为1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //概据TIM_TimeBaseInitStructure指定参数初始化TIM3
	 
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 		//使能指定的TIM3中断，允许更新中断
	TIM_Cmd(TIM3, ENABLE); 	//使能TIM3开始计数
}


/*****************************************
***功能：定时器14初始化
***参数： psc   预分频值
	      arr   自动重载值
***返回值：无	   
******************************************/
void TIM14_Init(uint16_t psc , uint16_t arr)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 , ENABLE); //使能TIM14时钟    
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn; 		//TIM14通道中断注册
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 		//配置TIM14时钟频率除数的预分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = arr; 								//配置下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //配置时钟分割为1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStructure); //概据TIM_TimeBaseInitStructure指定参数初始化TIM14
	 
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE); 		//使能指定的TIM14中断，允许更新中断
	TIM_Cmd(TIM14, ENABLE); 	//使能TIM14开始计数
}

/*****************************************
***功能：定时器16初始化
***参数： psc   预分频值
	      arr   自动重载值
***返回值：无	   
******************************************/
void TIM16_Init(uint16_t psc , uint16_t arr)
{
	NVIC_InitTypeDef	NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;         
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 , ENABLE); //使能TIM16时钟   
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn; 		//TIM16通道中断注册
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 		//配置TIM16时钟频率除数的预分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = arr; 								//配置下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //配置时钟分割为1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseInitStructure); //概据TIM_TimeBaseInitStructure指定参数初始化TIM16
	 
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE); 		//使能指定的TIM16中断，允许更新中断
	TIM_Cmd(TIM16, ENABLE); 	//使能TIM16开始计数
}

/*****************************************
***功能：定时器16初始化
***参数： psc   预分频值
	      arr   自动重载值
***返回值：无	   
******************************************/
void TIM15_Init(uint16_t psc , uint16_t arr)
{
	NVIC_InitTypeDef	NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;         
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 , ENABLE); //使能TIM16时钟   
	 
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn; 		//TIM16通道中断注册
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 		//配置TIM16时钟频率除数的预分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = arr; 								//配置下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //配置时钟分割为1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseInitStructure); //概据TIM_TimeBaseInitStructure指定参数初始化TIM16
	 
	TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE); 		//使能指定的TIM16中断，允许更新中断
	TIM_Cmd(TIM15, DISABLE); 	//使能TIM15开始计数
}

/*****************************************
***功能：定时器3中断函数
***参数： 无
***返回值：无	   
******************************************/
void TIM3_IRQHandler(void)   //TIM3
{	
	static int long_time=0;
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查指定的TIM3中断源发生与否
	{
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update);  //清除TIM1中断标志
		
		bsp_KeyScan();//按键扫描
		
		HandleLongPressAdd(&long_time);//长按加
		
		HandleLongPressSub(&long_time);//长按减

	}
}

/*****************************************
***功能：定时器14中断函数
***参数： 无
***返回值：无	   
******************************************/
void TIM14_IRQHandler(void)   //TIM14中断服务函数
{
	static uint8_t count=0;
	
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  //检查指定的TIM14中断源发生与否
	{
		TIM_ClearITPendingBit(TIM14 , TIM_IT_Update);  //清除TIM1中断标志
		count++;						//对毫秒进行累加
		if(count >3)		  //9毫秒已达到
		{
			count = 0;				//清除毫秒累加器，重新从0开始累加
		}
		NIXI_Show(count,Show_Data_Buf[count]);//加载段码显示
	}
}

/*****************************************
***功能：定时器16中断函数(20ms进入一次)
***参数： 无
***返回值：无	   
******************************************/
void TIM16_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)  //检查指定的TIM16中断源发生与否
	{
		//清除TIM1中断标志
		TIM_ClearITPendingBit(TIM16 , TIM_IT_Update);  
		
		/*检测错误发送命令*/
		updateSensorTime(&sensor_time);
		
		//定时监测电池
		timerMonitorBattery();
		
		//定时器监测电源状态
		timerMonitorPowerStatus();
		
		/*1s读取模块环境温湿度*/
		Read_TH_Data();
		
		/*检测按下后60内无按键按下，触发时间*/
		timerMonitorMenu();
	}
}




/*****************************************************
函数名称：定时器初始化
输入参数：无
输出参数：无
人话：设置了10ms定时中断
*****************************************************/
void timer_init(void)
{
	TIM3_Init((48 - 1) , 10*1000);	  //10ms进去定时中断一次  按键检测
	
	TIM14_Init((48 - 1) , 3*1000);    //3ms进入一次 ，数码管显示
	
	TIM16_Init((48 - 1) , 20*1000);	  //20ms进入一次
}





