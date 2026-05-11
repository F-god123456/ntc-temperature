#include "main.h"

/*
	STM32F030K6T6 按键引脚分配：

		KEY_MENU         : PB9    (低电平表示按下)
		KEY_UP           : PB1    (低电平表示按下)
		KEY_DOWN         : PB2    (低电平表示按下)
		KEY_RETURN       : PB3    (低电平表示按下)
		KEY_ONOFF        : PB4    (低电平表示按下)
*/

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */

static uint8_t Key4_turn=0;//追踪按键K4的状态变化

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);



/*
*********************************************************************************************************
*	函 数 名: IsKeyDown1
*	功能说明: 检测指定的按键是否被按下
*	形    参:  无
*	返 回 值: 
*			1 - 按键被按下
*			0 - 按键未被按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown1(void)
{
	if ( GPIO_ReadInputDataBit(KEY_MENU_GPIO_Port,KEY_MENU_Pin) == 0)
		return 1;
	else 
		return 0;
}

/*
*********************************************************************************************************
*	函 数 名: IsKeyDown2
*	功能说明: 检测指定的按键是否被按下
*	形    参:  无
*	返 回 值: 
*			1 - 按键被按下
*			0 - 按键未被按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown2(void)
{
	if ( GPIO_ReadInputDataBit(KEY_UP_GPIO_Port,KEY_UP_Pin) == 0)
		return 1;
	else 
		return 0;
}

/*
*********************************************************************************************************
*	函 数 名: IsKeyDown3
*	功能说明: 检测指定的按键是否被按下
*	形    参:  无
*	返 回 值: 
*			1 - 按键被按下
*			0 - 按键未被按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown3(void)
{	
	if ( GPIO_ReadInputDataBit(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin) == 0 )
		return 1;
	else 
		return 0;
}
/*
*********************************************************************************************************
*	函 数 名: IsKeyDown4
*	功能说明: 检测指定的按键是否被按下
*	形    参:  无
*	返 回 值: 
*			1 - 按键被按下
*			0 - 按键未被按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown4(void)
{
	if ( GPIO_ReadInputDataBit(KEY_RETURN_GPIO_Port,KEY_RETURN_Pin)== 0)
		return 1;
	else 
		return 0;
}

/*
*********************************************************************************************************
*	函 数 名: IsKeyDown5
*	功能说明: 检测指定的按键是否被按下
*	形    参:  无
*	返 回 值: 
*			1 - 按键被按下
*			0 - 按键未被按下
*********************************************************************************************************
*/
static uint8_t IsKeyDown5(void)	
{
	if (GPIO_ReadInputDataBit(KEY_ONOFF_GPIO_Port,KEY_ONOFF_Pin) == 0)
		return 1;
	else 
		return 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();			/* 初始化按键变量 */
	bsp_InitKeyHard();		    /* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey2
*	功能说明: 从按键FIFO缓冲区读取一个键值。独立的读指针。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参:  _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参：_ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyHard
*	功能说明: 配置按键对应的GPIO
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOB时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	
		/* 设置KEY_MENU_Pin对应的按键 */
	GPIO_InitStructure.GPIO_Pin = KEY_MENU_Pin;	
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	GPIO_Init(KEY_MENU_GPIO_Port, &GPIO_InitStructure);

		/* 设置KEY_UP_Pin对应的按键 */
	GPIO_InitStructure.GPIO_Pin = KEY_UP_Pin;	
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	GPIO_Init(KEY_UP_GPIO_Port, &GPIO_InitStructure);
	
		/* 设置KEY_DOWN_Pin对应的按键 */
	GPIO_InitStructure.GPIO_Pin = KEY_DOWN_Pin;		
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	GPIO_Init(KEY_DOWN_GPIO_Port, &GPIO_InitStructure);

		/* 设置KEY_RETURN_Pin对应的按键 */
	GPIO_InitStructure.GPIO_Pin = KEY_RETURN_Pin;	
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	GPIO_Init(KEY_RETURN_GPIO_Port, &GPIO_InitStructure);
	
		/* 设置KEY_ONOFF_Pin对应的按键 */
	GPIO_InitStructure.GPIO_Pin = KEY_ONOFF_Pin;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN;			//配置对应引脚为输入模式
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;			//配置对应引脚带上拉输入		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度为50MHZ
	GPIO_Init(KEY_ONOFF_GPIO_Port, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* 按键按下的键值代码 */
		//s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* 按键弹起的键值代码 */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* 按键被持续按下的键值代码 */
		s_tBtn[i].RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
	/* 比如，我们希望按键1按下超过1秒后，自动重发相同键值 */
	s_tBtn[KID_K1].LongTime = 100;
	//s_tBtn[KID_K1].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	s_tBtn[KID_K2].LongTime = 100;
	//s_tBtn[KID_K2].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */
	
//	s_tBtn[KID_JOY_D].LongTime = 100;
//	s_tBtn[KID_JOY_D].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

//	s_tBtn[KID_JOY_L].LongTime = 100;
//	s_tBtn[KID_JOY_L].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

//	s_tBtn[KID_JOY_R].LongTime = 100;
//	s_tBtn[KID_JOY_R].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */

	/* 判断按键按下的函数 */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	s_tBtn[4].IsKeyDownFunc = IsKeyDown5;

}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参:  按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	/*
		如果没有初始化按键函数，则报错
		if (s_tBtn[i].IsKeyDownFunc == 0)
		{
//			printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
		}
	*/
/*按键按下*/
	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc()==1)
	{
		/*短按键，软件处理消抖*/
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}	
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* 发送按钮按下的消息 */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			/*处理长按键*/
			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮长按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* 长按键后，每隔pBtn->RepeatSpeed*10ms发送1个按键 */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* 松开按键KEY_FILTER_TIME后 发送按钮弹起的消息 */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

/*
*********************************************************************************************************
*	函 数 名: checkKeyPresses
*	功能说明: 检测按键事件并执行相应的操作
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void checkKeyPresses(void)
{
	uint8_t ucKeyCode;
	uint8_t Key_Down_Flag=0;
	static uint8_t bcout=0;
	
	ucKeyCode = bsp_GetKey(); 
	if (ucKeyCode != KEY_NONE)
	{
		if(beep_state==1)//处理蜂鸣器报警时操作
		{
			handleBeepState(&bcout);
			return;
		}
		
		switch (ucKeyCode)	
		{	
			case KEY_DOWN_K0://按下
				Key_Down_Flag=1;
				if(MenuFlag)
					Menu_KEY0_OK=1;
				break;

			case KEY_LONG_K0://长按
				Key_Down_Flag=1;
				if(MenuFlag!=1)
					MenuFlag =1;
				break;	
			
			case KEY_DOWN_K1://按下
				Key_Down_Flag=1;
				if(MenuFlag)
					Menu_KEY1_UP = 1;
				break;
				
			case KEY_UP_K1://弹起
				Key_Long_Flag=0;
				break;
			
			case KEY_LONG_K1:
				Key_Down_Flag=1;
				if(TestMenu.current->downfun !=0)
					Key_Long_Flag = 1;
				break;	
			
			case KEY_DOWN_K2:
				Key_Down_Flag=1;
				if(MenuFlag)
					Menu_KEY2_DOWN = 1;
				break;
				
			case KEY_UP_K2:
				Key_Long_Flag=0;
				break;
			
			case KEY_LONG_K2:
				Key_Down_Flag=1;
				if(TestMenu.current->downfun !=0)
					Key_Long_Flag = 2;
				break;
			
			case KEY_DOWN_K3:
				Key_Down_Flag=1;
				if(MenuFlag)
					Menu_KEY3_RETURN = 1;
				break;
				
			case KEY_UP_K3:
				break;
			
			case KEY_LONG_K3:
				Key_Down_Flag=1;
				break;
			
			case KEY_DOWN_K4:
				Key_Down_Flag=1;
				Key4_turn=0;
				break;
				
			case KEY_UP_K4:
				if(MenuFlag!=1 && Key4_turn==0)//没有进去菜单时候
					RevRelay();//翻转继电器
				
				break;
			
			case KEY_LONG_K4:
				Key_Down_Flag=1;
				Key4_turn=1;
				if(MenuFlag!=1)//没有进去菜单时候
				{
					Led_Auto_Toggle();
					FLASH_Save_User_Data.switchMode ^=1;
					FLASH_SAVE_Sign=1;
					upSwitchModeFlag=1;
				}
				break;
			
			default:
				break;
		}
		if(Key_Down_Flag==1&&MenuFlag)//在菜单里面时候无按键按下超时操作
		{
			setMenuTimeout(&Key_Down_Flag,&Menu_timeout);
		}
	}

}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

