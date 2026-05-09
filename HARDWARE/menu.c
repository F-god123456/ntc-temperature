#include "main.h"


struct _Menu TestMenu;//管理菜单的结构体
 
uint8_t g_trun_flag=1;//用于控制菜单显示的状态
uint8_t Menu_KEY0_OK;//确定键标志位
uint8_t Menu_KEY1_UP;//向上加标志位
uint8_t Menu_KEY2_DOWN;//向下减标志位
uint8_t Menu_KEY3_RETURN;//返回标志位
uint8_t MenuFlag=0;//菜单标志位
uint8_t DataChangeFlag=0;//FALSH数据有修改变化
uint8_t Key_Long_Flag=0;//按键长按标志位，如1长加 2长减

uint32_t Menu_timeout=0;//按键最长无按下时间
/*
***功能：刷新页面函数，用于显示菜单项
***参数：
		buf ：菜单管理结构体
		index：菜单级别的序号
***返回：无
*/
void MenuDisplay(struct _sMenuList *buf, uint16_t index)
{
//	printf("标题:%s\n",buf->title); // 显示标题
//	printf("项目:%s\n", (&buf[index])->label); // 显示项目
//	printf("下标：%d\n",index);
	
	switch(index)
	{
		case 0:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 1;
		break;
		case 1:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 2;
		break;
		case 2:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 3;
		break;
		case 3:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 4;
		break;
		case 4:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 5;
		break;
		case 5:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 6;
		break;
		case 6:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 7;
		break;
		case 7:
			g_DP_Pos=14;
			Show_Data_Buf[0] = 'S';
			Show_Data_Buf[1] = '-';
			Show_Data_Buf[2] = '-';
			Show_Data_Buf[3] = 8;
		break;
		
	}
}

/*
***功能：首次显示二级界面的函数
***参数：
		index：菜单级别的序号
***返回：无
*/
void Show_Enterdata(uint8_t index)
{
	switch(index)
	{
		case 0:
			NIXI_StartTemperature_Show(FLASH_Save_User_Data.switchOnTemp);
			break;
		case 1:
			NIXI_StopTemperature_Show(FLASH_Save_User_Data.switchOffTemp);
			break;
		case 2:
			NIXI_tempOffset_Show(FLASH_Save_User_Data.tempOffset);
			break;
		case 3:
			NIXI_highTempAlarm_Show(FLASH_Save_User_Data.highTempAlarm);
			break;
		case 4:
			NIXI_highMaxTemp_Show(FLASH_Save_User_Data.highMaxTemp);
			break;
		case 5:
			NIXI_lowTempAlarm_Show(FLASH_Save_User_Data.lowTempAlarm);
			break;
		case 6:
			NIXI_lowMinTemp_Show(FLASH_Save_User_Data.lowMinTemp);
			break;
		case 7:
			NIXI_tempAlarmBuf_Show(FLASH_Save_User_Data.tempAlarmBuf);
			break;
	}
}

/*
***功能：向上加并显示起始温度当前值
***参数：无
***返回：无
*/
void StartTemperature_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.switchOnTemp>-100)
	{
		FLASH_Save_User_Data.switchOnTemp*=10.0;
		FLASH_Save_User_Data.switchOnTemp+=1;
		FLASH_Save_User_Data.switchOnTemp/=10.0;
		if(FLASH_Save_User_Data.switchOnTemp>=T_MAX)
			FLASH_Save_User_Data.switchOnTemp=T_MAX;
		//显示数码管
		NIXI_StartTemperature_Show(FLASH_Save_User_Data.switchOnTemp);

	}
	else
	{
		FLASH_Save_User_Data.switchOnTemp+=1;
		if(FLASH_Save_User_Data.switchOnTemp>=T_MAX)
			FLASH_Save_User_Data.switchOnTemp=T_MAX;
		//显示数码管
		NIXI_StartTemperature_Show(FLASH_Save_User_Data.switchOnTemp);
	}
}
/*
***功能：向下减并显示起始温度当前值
***参数：无
***返回：无
*/
void StartTemperature_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.switchOnTemp>-100)
	{
		FLASH_Save_User_Data.switchOnTemp*=10.0;
		FLASH_Save_User_Data.switchOnTemp-=1;
		FLASH_Save_User_Data.switchOnTemp/=10.0;
		if(FLASH_Save_User_Data.switchOnTemp<=T_MIN)
			FLASH_Save_User_Data.switchOnTemp=T_MIN;
		//显示数码管
		NIXI_StartTemperature_Show(FLASH_Save_User_Data.switchOnTemp);

	}
	else
	{
		FLASH_Save_User_Data.switchOnTemp-=1;
		if(FLASH_Save_User_Data.switchOnTemp<=T_MIN)
			FLASH_Save_User_Data.switchOnTemp=T_MIN;
		//显示数码管
		NIXI_StartTemperature_Show(FLASH_Save_User_Data.switchOnTemp);

	}
}
/*
***功能：向上加并显示停止温度当前值
***参数：无
***返回：无
*/
void StopTemperature_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.switchOffTemp>-100)
	{
		FLASH_Save_User_Data.switchOffTemp*=10.0;
		FLASH_Save_User_Data.switchOffTemp+=1;
		FLASH_Save_User_Data.switchOffTemp/=10.0;
		if(FLASH_Save_User_Data.switchOffTemp>=T_MAX)
			FLASH_Save_User_Data.switchOffTemp=T_MAX;
		//显示数码管
		NIXI_StopTemperature_Show(FLASH_Save_User_Data.switchOffTemp);

	}
	else
	{

		FLASH_Save_User_Data.switchOffTemp+=1;
		if(FLASH_Save_User_Data.switchOffTemp>=T_MAX)
			FLASH_Save_User_Data.switchOffTemp=T_MAX;
		//显示数码管
		NIXI_StopTemperature_Show(FLASH_Save_User_Data.switchOffTemp);

	}
}
/*
***功能：向下减并显示停止温度当前值
***参数：无
***返回：无
*/
void StopTemperature_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.switchOffTemp>-100)
	{
		FLASH_Save_User_Data.switchOffTemp*=10.0;
		FLASH_Save_User_Data.switchOffTemp-=1;
		FLASH_Save_User_Data.switchOffTemp/=10.0;
		if(FLASH_Save_User_Data.switchOffTemp<=T_MIN)
			FLASH_Save_User_Data.switchOffTemp=T_MIN;
		//显示数码管
		NIXI_StopTemperature_Show(FLASH_Save_User_Data.switchOffTemp);

	}
	else
	{
		FLASH_Save_User_Data.switchOffTemp-=1;
		if(FLASH_Save_User_Data.switchOffTemp<=T_MIN)
			FLASH_Save_User_Data.switchOffTemp=T_MIN;
		//显示数码管
		NIXI_StopTemperature_Show(FLASH_Save_User_Data.switchOffTemp);

	}
}
/*
***功能：向上加并显示温度偏移量
***参数：无
***返回：无
*/
void tempOffset_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0)
	{
		FLASH_Save_User_Data.tempOffset*=10.0;
		FLASH_Save_User_Data.tempOffset+=1;
		FLASH_Save_User_Data.tempOffset/=10.0;
		if(FLASH_Save_User_Data.tempOffset>=99.9)
			FLASH_Save_User_Data.tempOffset=99.9;
		//显示数码管
		NIXI_revealDecimal_Show(FLASH_Save_User_Data.tempOffset);

	}
	else
	{
		FLASH_Save_User_Data.tempOffset+=1;
		if(FLASH_Save_User_Data.tempOffset>=99.9)
			FLASH_Save_User_Data.tempOffset=99.9;
		//显示数码管
		NIXI_revealDecimal_Show(FLASH_Save_User_Data.tempOffset);

	}
}
/*
***功能：向下减并显示温度偏移量
***参数：无
***返回：无
*/
void tempOffset_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0)
	{
		FLASH_Save_User_Data.tempOffset*=10;
		FLASH_Save_User_Data.tempOffset-=1;
		FLASH_Save_User_Data.tempOffset/=10.0;
		if(FLASH_Save_User_Data.tempOffset<=-99.9)
			FLASH_Save_User_Data.tempOffset=-99.9;
		//显示数码管
		NIXI_revealDecimal_Show(FLASH_Save_User_Data.tempOffset);

	}
	else
	{
		FLASH_Save_User_Data.tempOffset-=1;
		if(FLASH_Save_User_Data.tempOffset<=-99.9)
			FLASH_Save_User_Data.tempOffset=-99.9;
		//显示数码管
		NIXI_revealDecimal_Show(FLASH_Save_User_Data.tempOffset);

	}
}
/*
***功能：向上加并显示超高温度报警
***参数：无
***返回：无
*/
void highTempAlarm_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	FLASH_Save_User_Data.highTempAlarm=true;
	//显示数码管
	NIXI_highTempAlarm_Show(FLASH_Save_User_Data.highTempAlarm);

}
/*
***功能：向下减并显示超高温度报警
***参数：无
***返回：无
*/
void highTempAlarm_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	FLASH_Save_User_Data.highTempAlarm=false;
	//显示数码管
	NIXI_highTempAlarm_Show(FLASH_Save_User_Data.highTempAlarm);

}
/*
***功能：向上加并显示超高温度报警值
***参数：无
***返回：无
*/
void highMaxTemp_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.highMaxTemp>-100)
	{
		FLASH_Save_User_Data.highMaxTemp*=10;
		FLASH_Save_User_Data.highMaxTemp+=1;
		FLASH_Save_User_Data.highMaxTemp/=10.0;
		if(FLASH_Save_User_Data.highMaxTemp>=T_MAX)
			FLASH_Save_User_Data.highMaxTemp=T_MAX;
		//显示数码管
		NIXI_highMaxTemp_Show(FLASH_Save_User_Data.highMaxTemp);

	}
	else
	{
		FLASH_Save_User_Data.highMaxTemp+=1;
		if(FLASH_Save_User_Data.highMaxTemp>=T_MAX)
			FLASH_Save_User_Data.highMaxTemp=T_MAX;
		//显示数码管
		NIXI_highMaxTemp_Show(FLASH_Save_User_Data.highMaxTemp);

	}
}
/*
***功能：向下减并显示超高温度报警值
***参数：无
***返回：无
*/
void highMaxTemp_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.highMaxTemp>-100)
	{
		FLASH_Save_User_Data.highMaxTemp*=10;
		FLASH_Save_User_Data.highMaxTemp-=1;
		FLASH_Save_User_Data.highMaxTemp/=10.0;
		if(FLASH_Save_User_Data.highMaxTemp<=T_MIN)
			FLASH_Save_User_Data.highMaxTemp=T_MIN;
		//显示数码管
		NIXI_highMaxTemp_Show(FLASH_Save_User_Data.highMaxTemp);

	}
	else
	{
		FLASH_Save_User_Data.highMaxTemp-=1;
		if(FLASH_Save_User_Data.highMaxTemp<=T_MIN)
			FLASH_Save_User_Data.highMaxTemp=T_MIN;
		//显示数码管
		NIXI_highMaxTemp_Show(FLASH_Save_User_Data.highMaxTemp);

	}
}

/*
***功能：向上加并显示超低温度报警
***参数：无
***返回：无
*/
void lowTempAlarm_up_key(bool Long_Fun)
{

	DataChangeFlag =1;
	
	FLASH_Save_User_Data.lowTempAlarm=true;
	//显示数码管
	NIXI_lowTempAlarm_Show(FLASH_Save_User_Data.lowTempAlarm);

}
/*
***功能：向下减并显示超低温度报警
***参数：无
***返回：无
*/
void lowTempAlarm_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	FLASH_Save_User_Data.lowTempAlarm=false;
	//显示数码管
	NIXI_lowTempAlarm_Show(FLASH_Save_User_Data.lowTempAlarm);

}

/*
***功能：向上加并显示超低温度报警值
***参数：无
***返回：无
*/
void lowMinTemp_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.lowMinTemp>-100)
	{
		FLASH_Save_User_Data.lowMinTemp*=10;
		FLASH_Save_User_Data.lowMinTemp+=1;
		FLASH_Save_User_Data.lowMinTemp/=10.0;
		if(FLASH_Save_User_Data.lowMinTemp>=T_MAX)
			FLASH_Save_User_Data.lowMinTemp=T_MAX;
		//显示数码管
		NIXI_lowMinTemp_Show(FLASH_Save_User_Data.lowMinTemp);

	}
	else
	{
		FLASH_Save_User_Data.lowMinTemp+=1;
		if(FLASH_Save_User_Data.lowMinTemp>=T_MAX)
			FLASH_Save_User_Data.lowMinTemp=T_MAX;
		//显示数码管
		NIXI_lowMinTemp_Show(FLASH_Save_User_Data.lowMinTemp);

	}
}
/*
***功能：向下减并显示超低温度报警值
***参数：无
***返回：无
*/
void lowMinTemp_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0&&FLASH_Save_User_Data.lowMinTemp>-100)
	{
		FLASH_Save_User_Data.lowMinTemp*=10;
		FLASH_Save_User_Data.lowMinTemp-=1;
		FLASH_Save_User_Data.lowMinTemp/=10.0;
		if(FLASH_Save_User_Data.lowMinTemp<=T_MIN)
			FLASH_Save_User_Data.lowMinTemp=T_MIN;
		//显示数码管
		NIXI_lowMinTemp_Show(FLASH_Save_User_Data.lowMinTemp);

	}
	else
	{
		FLASH_Save_User_Data.lowMinTemp-=1;
		if(FLASH_Save_User_Data.lowMinTemp<=T_MIN)
			FLASH_Save_User_Data.lowMinTemp=T_MIN;
		//显示数码管
		NIXI_lowMinTemp_Show(FLASH_Save_User_Data.lowMinTemp);

	}
}

/*
***功能：向上加并显示温度报警缓冲区值
***参数：无
***返回：无
*/
void tempAlarmBuf_up_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0)
	{
		FLASH_Save_User_Data.tempAlarmBuf*=10;
		FLASH_Save_User_Data.tempAlarmBuf+=1;
		FLASH_Save_User_Data.tempAlarmBuf/=10.0;
		if(FLASH_Save_User_Data.tempAlarmBuf>=100.0)
			FLASH_Save_User_Data.tempAlarmBuf=100.0;
		//显示数码管
		NIXI_tempAlarmBuf_Show(FLASH_Save_User_Data.tempAlarmBuf);

	}
	else
	{
		FLASH_Save_User_Data.tempAlarmBuf+=1;
		if(FLASH_Save_User_Data.tempAlarmBuf>=100.0)
			FLASH_Save_User_Data.tempAlarmBuf=100.0;
		//显示数码管
		NIXI_tempAlarmBuf_Show(FLASH_Save_User_Data.tempAlarmBuf);

	}
}

/*
***功能：向下减并显示温度报警缓冲区值
***参数：无
***返回：无
*/
void tempAlarmBuf_down_key(bool Long_Fun)
{
	DataChangeFlag =1;
	if(Long_Fun == 0)
	{
		FLASH_Save_User_Data.tempAlarmBuf*=10;
		FLASH_Save_User_Data.tempAlarmBuf-=1;
		FLASH_Save_User_Data.tempAlarmBuf/=10.0;
		if(FLASH_Save_User_Data.tempAlarmBuf<=0)
			FLASH_Save_User_Data.tempAlarmBuf=0;
		//显示数码管
		NIXI_tempAlarmBuf_Show(FLASH_Save_User_Data.tempAlarmBuf);

	}
	else
	{
		FLASH_Save_User_Data.tempAlarmBuf-=1;
		if(FLASH_Save_User_Data.tempAlarmBuf<=0)
			FLASH_Save_User_Data.tempAlarmBuf=0;
		//显示数码管
		NIXI_tempAlarmBuf_Show(FLASH_Save_User_Data.tempAlarmBuf);

	}
}


//当前菜单功能项总数、标题名称、 项目名称、向上功能、向下功能、长按加功能、长按减功能、下一级菜单、上一级菜单
struct _sMenuList Menu2_StartTemperature[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置启动温度", StartTemperature_down_key,StartTemperature_up_key, StartTemperature_up_key,StartTemperature_down_key,0 ,&Menu1_Main[0]},
};
struct _sMenuList Menu2_StopTemperature[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置停止温度", StopTemperature_down_key,StopTemperature_up_key, StopTemperature_up_key, StopTemperature_down_key,0, &Menu1_Main[1]},
};
struct _sMenuList Menu2_tempOffset[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置温度补偿", tempOffset_down_key,tempOffset_up_key, tempOffset_up_key,tempOffset_down_key, 0, &Menu1_Main[2]},
};
struct _sMenuList Menu2_highTempAlarm[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置超高温度报警", highTempAlarm_down_key,highTempAlarm_up_key,  highTempAlarm_up_key,highTempAlarm_down_key,0, &Menu1_Main[3]},
};
struct _sMenuList Menu2_highMaxTemp[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置超高温度最高值", highMaxTemp_down_key,highMaxTemp_up_key, highMaxTemp_up_key,highMaxTemp_down_key,0, &Menu1_Main[4]},
};
struct _sMenuList Menu2_lowTempAlarm[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置超低温度报警", lowTempAlarm_down_key,lowTempAlarm_up_key, lowTempAlarm_up_key ,lowTempAlarm_down_key,0, &Menu1_Main[5]},
};
struct _sMenuList Menu2_lowMinTemp[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置超低温度最低值", lowMinTemp_down_key,lowMinTemp_up_key,  lowMinTemp_up_key,lowMinTemp_down_key,0,&Menu1_Main[6]},
};

struct _sMenuList Menu2_tempAlarmBuf[] = {
    {TEST_MENU2_FILE_NUM, "-= 二级文件 =-", "设置温度报警缓存区",tempAlarmBuf_down_key, tempAlarmBuf_up_key,  tempAlarmBuf_up_key,tempAlarmBuf_down_key,0, &Menu1_Main[7]},
};


//当前菜单功能项总数、标题名称、 项目名称、向上功能、向下功能、长按功能、下一级菜单、上一级菜单
// 菜单数组定义
struct _sMenuList Menu1_Main[] = {
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "1.开关启动温度"  , 0, 0, 0,0,Menu2_StartTemperature, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "2.开关关闭温度"  , 0, 0, 0,0,Menu2_StopTemperature, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "3.温度补偿"      , 0, 0, 0,0,Menu2_tempOffset, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "4.超高温度报警"  , 0, 0, 0, 0, Menu2_highTempAlarm, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "5.超高温度最高值", 0, 0,  0 ,0,  Menu2_highMaxTemp, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "6.超低温度报警"  , 0, 0, 0, 0, Menu2_lowTempAlarm, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "7.超低温度最低值", 0, 0,  0 ,0,  Menu2_lowMinTemp, Menu1_Main},
    {MENU1_MAIN_NUM, "-= 一级菜单 =-", "8.温度报警缓存区", 0, 0,  0, 0,  Menu2_tempAlarmBuf, Menu1_Main},
};

/*
***函数名：Menu_Init
***功能：初始化菜单系统，设置初始状态和默认值。
***参数;无
***返回值：无
*/
void Menu_Init(void)
{
	int i;
	TestMenu.current=Menu1_Main; //指向一个管理
	for(i=0;i<MENU_LEVEL_NUM;i++)
		TestMenu.index[i]=0;
	TestMenu.level=0;
}



/*
***函数名：ExecuteMenuTask
***功能：菜单执行任务
***参数;无
***返回值：无
*/
void ExecuteMenuTask(void) 
{	
	if(MenuFlag)
	{
		if(g_trun_flag)
		{	
			g_trun_flag =0;
			MenuDisplay(TestMenu.current, TestMenu.index[TestMenu.level]);
		}
		if(Menu_KEY0_OK == 1)//确定键
		{
			Menu_KEY0_OK = 0;
			if (TestMenu.current->next!=NULL) 
			{
				Show_Enterdata(TestMenu.index[TestMenu.level]);
				TestMenu.current = TestMenu.current[TestMenu.index[TestMenu.level]].next;
				TestMenu.level++; // 菜单级别++
				g_trun_flag=0;
				
			}	
		}	
		else if(Menu_KEY1_UP == 1)//向上移动/加
		{
			Menu_KEY1_UP = 0;
			if(TestMenu.current->downfun==0 )
			{
				if (++TestMenu.index[TestMenu.level] >= TestMenu.current->num) 
				{
					TestMenu.index[TestMenu.level] = 0;
				}
				MenuDisplay(TestMenu.current, TestMenu.index[TestMenu.level]);
			}
			else if(TestMenu.current->downfun !=0)
			{
				TestMenu.current[TestMenu.index[TestMenu.level]].downfun(0);
				g_trun_flag=0;
			}				
		}
		else if(Menu_KEY2_DOWN == 1)//向下移动/减
		{
			Menu_KEY2_DOWN =0;
			if(TestMenu.current->upfun==0)
			{
				if (--TestMenu.index[TestMenu.level] == 0xffff) // 超出上限
				{       
					TestMenu.index[TestMenu.level] = TestMenu.current->num - 1;
				}
				MenuDisplay(TestMenu.current, TestMenu.index[TestMenu.level]);
			}
			else if(TestMenu.current->upfun!=0)
			{
				TestMenu.current->upfun(0);
				g_trun_flag=0;
			}
				
		}	
		else if(Menu_KEY3_RETURN == 1)//返回上一级
		{
			Menu_KEY3_RETURN = 0; 
			if (TestMenu.level == 0) 
			{
				MenuFlag=0;
				g_trun_flag=1;
				TestMenu.index[TestMenu.level]=0;
				return ;
			}
			else if(TestMenu.level == 1&&DataChangeFlag ==1)//返回到第一级菜单后且数据有修改迹象
			{
				DataChangeFlag  = 0;
				
				FLASH_SAVE_Sign =1;//保存数据
				
				upswitchUpDeviProFlag = 1;//上传所有保存到数据命令
			}
			TestMenu.current = TestMenu.current[TestMenu.index[TestMenu.level]].prev->prev;
			TestMenu.level--; // 菜单级别--
			MenuDisplay(TestMenu.current, TestMenu.index[TestMenu.level]);
		}
	}
}


/*
***函数名：HandleLongPressAdd
***功能：长按增加功能
***参数：
	time：一个指向整数的指针，用于跟踪按键被按下的时间长度。
***返回值：无
*/
void HandleLongPressAdd(int* time)	
{
    if (Key_Long_Flag == 1) 
	{
        (*time)++;
        if ((*time) >= 15) 
		{
            (*time) = 0;
            TestMenu.current[TestMenu.index[TestMenu.level]].longaddfun(1);
        }
    }
}

/*
***函数名：HandleLongPressSub
***功能：长按减少功能
***参数：
	time：一个指向整数的指针，用于跟踪按键被按下的时间长度。
***返回值：无
*/
void HandleLongPressSub(int* time) 
{
    if (Key_Long_Flag == 2) 
	{
        (*time)++;
        if ((*time) >= 15) 
		{
            (*time) = 0;
            TestMenu.current[TestMenu.index[TestMenu.level]].longsubfun(1);
        }
    }
}

/*
***函数名：timerMonitorMenu
***功能：检测按键无按下时计时到，触发事件
***参数：无
***返回值：无
*/
void timerMonitorMenu(void)
{
	if(Menu_timeout!=0)
	{
		Menu_timeout--;
		if(Menu_timeout == 0)
		{
			Menu_timeout =0;//超时置0
			
			if(TestMenu.level ==0)
			{
				MenuFlag=0;//关掉菜单
			
				g_trun_flag=1;//不显示
			
				TestMenu.index[TestMenu.level]=0;//索引归0
				
			}
			else if(TestMenu.level ==1)
			{
				/*在二级菜单操作的事情*/
				DataChangeFlag  = 0;

				FLASH_SAVE_Sign =1;//保存数据

				/*在一级级菜单操作的事情*/
				MenuFlag=0;//关掉菜单

				g_trun_flag=1;//不显示

				TestMenu.index[TestMenu.level]=0;//索引归0
			}
			upswitchUpDeviProFlag = 1;//上传所有保存到数据命令
			
			TestMenu.current = TestMenu.current[TestMenu.index[TestMenu.level]].prev->prev;
			TestMenu.level=0; // 菜单级别置0
		}
	}

}


/*
*********************************************************************************************************
*	函 数 名: setMenuTimeout
*	功能说明: 设置菜单超时。此函数用于初始化菜单超时状态，当菜单处于活动状态时，
*	           将超时标志设置为0，并将超时计数器设置为最大值。
*	形    参: Flag : 指向菜单超时标志的指针
*	           Menu_timeout : 指向菜单超时计数器的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void setMenuTimeout(uint8_t *Flag, uint32_t *Menu_timeout) 
{
    *Flag = 0;
    *Menu_timeout = MENU_MAXTIME;
}

