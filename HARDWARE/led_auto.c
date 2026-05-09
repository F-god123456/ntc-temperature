#include "main.h"

static	GPIO_InitTypeDef  GPIO_InitStructure;

uint8_t Led_Auto_State=0;//工作模式灯状态，1为手动模式，0为自动模式


/*********************************
功能：工作灯初始化
参数：无
返回：无
**********************************/
void Led_auto_Init(void)
{
	/* 使能GPIOF时钟 */
	RCC_AHBPeriphClockCmd(Led_auto_RCC, ENABLE);

	/* 配置Relay相应引脚PF6*/
	GPIO_InitStructure.GPIO_Pin = Led_auto_Pin;					//
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_OUT;			//配置PB0为输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度为50MHZ
	GPIO_Init(Led_auto_Port, &GPIO_InitStructure);
	
	Led_auto_On;//自动模式开启，默认自动模式灯亮
	Led_Auto_State=1;
}
/*********************************
功能：LED自动灯打开
参数：无
返回：无
**********************************/
void Led_Auto_On(void)
{
	Led_auto_On;//自动模式开启
	Led_Auto_State=1;
}
/*********************************
功能：LED自动灯关闭
参数：无
返回：无
**********************************/
void Led_Auto_Off(void)
{
	Led_auto_OFF;//自动模式关闭
	Led_Auto_State=0;
}

/*********************************
功能：控制灯的开关
参数：无
返回：无
**********************************/
void Led_Auto_Control(uint8_t state)
{
	if(state)
		Led_Auto_On();
	else
		Led_Auto_Off();
}

/*********************************
功能：翻转LED自动模式状态
参数：无
返回：无
**********************************/
void Led_Auto_Toggle(void) 
{
    // 翻转状态
    Led_Auto_State = !Led_Auto_State;
    
    // 根据状态执行相应的操作
    if (Led_Auto_State) {
        // 自动模式开启
        Led_Auto_On(); // Led_Auto_On()是开启LED自动模式的函数
    } else {
        // 自动模式关闭
        Led_Auto_Off(); // Led_Auto_Off()是关闭LED自动模式的函数
    }
}


/*********************************
功能：翻转LED自动模式状态
参数：无
返回：无
**********************************/
void Led_Auto_Restore(void)
{
	if(FLASH_Save_User_Data.switchMode==0)//自动模式
	{
		 // 自动模式开启
        Led_Auto_On(); // Led_Auto_On()是开启LED自动模式的函数
	}
	else//手动模式
	{
		// 自动模式关闭
        Led_Auto_Off(); // Led_Auto_Off()是关闭LED自动模式的函数
	}

}

