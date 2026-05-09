#include "main.h"

/*
***2025-8-4
1.增加了禁止总中断和开启总中断在读取高低电平哪里
2.使用滴答定时器来计算低电平时间，来判断逻辑1和逻辑0
3.优化了定时器循环扫描数码显示函数
*/


/*
***功能：入口函数
***参数：无
***返回值：
		  0  ：正常退出
*/
int main(void)
{
	IWDG_Init();			 //看门狗初始化
	
	USART1_Init(115200);	 //初始化串口，并配置波特率为115200bps
	
	delay_init(48);			 //初始化SYSTICK 
	
	FLASH_Init();			 //FLASH初始化
	
	bsp_InitKey();			 //按键初始化

    Menu_Init();			 //菜单初始化

	Beep_Init();			 //蜂鸣器
	
	Led_auto_Init();         //自动/手动指示灯初始化
	
	Relay_Init();			 //控制开关灯初始化
	
	NIXI_GPIO_Init();		 //数码管初始化
	
	NTC_Init();		 			//NTC初始化
	
	Power_Init();			 //电源初始化
	
	Read_configuration();    //从FLASH中读出用户参数，然后MCU把数据读出来赋值给结构体
	
	Led_Auto_Restore();		 //恢复自动模式灯
	
	timer_init();			 //定时器初始化
	
	while(1)
	{
		monitor();				//串口命令解析及上传命令
		
		EnvConditions_Show();   //温度切换显示
		
//		checkKeyPresses();      //扫描按键状态
		
		ExecuteMenuTask();		//执行菜单任务
		
		FLASH_SAVE_Monitor();   // 数据有变化后，保存数据到FLASH闪存里面
		
		EnvSense_ProcessData(); //温度控制逻辑以及报警功能
		
		IWDG_ReloadCounter();   // 喂狗操作
	}
	
}



