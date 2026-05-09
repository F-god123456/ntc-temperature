#include "main.h"


uint8_t SuperHeatFlag = 0;    //标志变量，用于指示是否检测到超高温状态。
uint8_t LowTempFlag = 0;      //标志变量，用于指示是否检测到低温状态。

uint8_t OverTempRecover_Flag=0;//标志变量，用于指示超高温状态是否已经恢复。
uint8_t LowTempRecover_Flag=0; //标志变量，用于指示低温状态是否已经恢复。


uint8_t highTempAlarmActive = 0;     // 定义全局变量来表示是否已经发生过超高温报警
uint8_t lowTempAlarmActive = 0;      // 定义全局变量来表示是否已经发生过超低温报警

uint8_t upSensorStatusFlag = 0;      // 用于上传传感器状态标志位

SystemTime currentTime;//存储系统时间
uint8_t error_flag=0;
uint8_t Sensor_State=0;//0:传感器正常，1传感器异常


/* 温度读取和传感器故障通知相关的变量*/
bool Mcu_Init =true;//记录MCU第一次上电时，读温湿度特殊动作
float _T;//临时温度

uint8_t Sensor_Fault_Flag=0;//传感器错误标志位
uint8_t Sensor_recover_Flag=0;//传感器恢复正常标志位
uint8_t Sensor_Fail_Count=0;//读取失败次数
uint8_t Sensor_failure_Cmd_Count =0;//发送故障命令次数
uint8_t Sensor_Hour_Falg=0;//故障倒计时标志位

/*
***功能：根据温度模式控制开关
***参数：T :温度
***返回：无
*/
void control_switch(float T)
{
	//散热模式
	if(FLASH_Save_User_Data.switchOnTemp > FLASH_Save_User_Data.switchOffTemp) 
	{
		if(T>FLASH_Save_User_Data.switchOnTemp && switch_state!=1)
		{
			//打开开关
			ControlRelay(1);
		}
		else if(T<=FLASH_Save_User_Data.switchOffTemp&& switch_state!=0)
		{
			//关闭开关
			ControlRelay(0);
		}
	}
	
	//升温模式
	if(FLASH_Save_User_Data.switchOnTemp < FLASH_Save_User_Data.switchOffTemp)
	{
		if(T<FLASH_Save_User_Data.switchOnTemp&& switch_state!=1)
		{
			//打开开关
			ControlRelay(1);
		}
		else if(T>=FLASH_Save_User_Data.switchOffTemp&& switch_state!=0)
		{
			//关闭开关
			ControlRelay(0);
		}
	}
}

/*
***功能：处理超高温度的逻辑
***参数：T :温度
***返回：无
*/
void HandleSuperHeat(float T) 
{
    // 若发生超高温度报警事件
    if (FLASH_Save_User_Data.highTempAlarm) // 超高温度开关开启情况下
	{ 
        // 如果当前温度超过最高温度并且没有激活报警
        if ( !highTempAlarmActive && T > FLASH_Save_User_Data.highMaxTemp ) 
		{
            // 置一个超高标志位
            SuperHeatFlag = 1;
		
            // 标记报警为激活状态
            highTempAlarmActive = 1;
			
			//设置蜂鸣器状态
			beep_state =1;
			
			//printf("超高温度报警\n\n");
			BEEP_ON();
        }
    }

    // 如果当前温度低于temp并且报警是激活状态
    if (highTempAlarmActive && T <= (FLASH_Save_User_Data.highMaxTemp - FLASH_Save_User_Data.tempAlarmBuf))		
	{
		OverTempRecover_Flag = 1;
        // 标记报警为非激活状态
        highTempAlarmActive = 0;		
		
		//设置蜂鸣器状态
		beep_state =0;
		
		BEEP_OFF();

    }
}

/*
***功能：处理超低温度的逻辑
***参数：T :温度
***返回：无
*/
void HandleLowTemp(float T) 
{
    // 若发生超低温度报警事件
    if (FLASH_Save_User_Data.lowTempAlarm) // 超低温度开关开启情况下
	{ 
        // 如果当前温度低于最低温度并且没有激活报警
        if ( !lowTempAlarmActive && T < FLASH_Save_User_Data.lowMinTemp ) 
		{
            // 置一个超低标志位
            LowTempFlag = 1;
			
            // 标记报警为激活状态
            lowTempAlarmActive = 1;
			//printf("超低温度报警\n\n");
			
			//设置蜂鸣器状态
			beep_state =1;
			
			BEEP_ON();
        }
    }

    // 如果当前温度低于temp并且报警是激活状态
    if (lowTempAlarmActive && T >= (FLASH_Save_User_Data.lowMinTemp + FLASH_Save_User_Data.tempAlarmBuf)) 
	{
		LowTempRecover_Flag = 1;
		
        // 标记报警为非激活状态
        lowTempAlarmActive = 0;
		
		//设置蜂鸣器状态
		beep_state =0;
		
		BEEP_OFF();
    }
}

/*
***功能：处理温度的逻辑
***参数：T :温度
***返回：无
*/
void HandleTemperature(float T) 
{
    // 处理超高温度
    HandleSuperHeat(T);
    // 处理超低温度
    HandleLowTemp(T);
}


/*
***功能：计算平均值的函数
***参数：
	array :待计算的数组
	size  ：长度
***返回：无
*/
float calculate_average(float array[], int size) 
{
	int i;
    float sum = 0.0;
    for ( i = 0; i < size; i++) 
	{
        sum += array[i];
    }
    return sum / size;
}



/*
***功能：将一个浮点数值赋给一个浮点数数组的所有元素
***参数：
	arr:数组
	length：数组长度
	value：值
***返回：无
*/
void fillArrayWithValue(float arr[], int length, float value) 
{
	int i;
    for ( i = 0; i < length; i++) 
	{
        arr[i] = value;
    }
}

/*
***功能：首次读取数据成功，将数据放进缓冲区
***参数：无
***返回：无
*/
void handle_head_read_success(void)
{
	T_temp[1]=T_temp[0];
	T_temp[2]=T_temp[0];
	T_temp[3]=T_temp[0];
	T_temp[4]=T_temp[0];
	_T=T_temp[0];
}
//uint8_t arr[6] = {0};
uint8_t head_read_successs_flag=1;


/*
***功能：求完5s内的温度平均值，更新数据到数码管显示
***参数：无
***返回：无
*/
void Read_Sensor_Data(void)
{
	float Temp;
	int ret;//结果
	
	if(time_over_flag==1)//1s时间到
	{
		//清除1s时间到标志位
		time_over_flag = 0;
		
		// 1.获取温度
		ret = HandleADCTemp(&Temp);
		
		// 2.判断传感器读取是否异常
		if(ret!=0)
		{
			ret= 1;//读取失败
		}
		else
		{
			ret= 0;//读取成功
			T_temp[time_count]=Temp;
			
		}
		
		// 3.根据读取结果进行判断
		if(ret!=0)//读取失败
		{
			if(Sensor_State==0 && Sensor_Fail_Count>=30)//故障状态  硬件故障超过30次以上时
			{
				Sensor_State =1;			//传感器异常状态
				//上传传感器状态
				upSensorStatusFlag = 1;
			}
			
			if(Sensor_Fail_Count<30)
			{
				Sensor_Fail_Count++;//传感器失败次数递加
			}
		}
		else//读取成功
		{
			Sensor_Fail_Count=0;
			
			//清空发送传感器故障失败命令次数（30ci算失败一次）
			Sensor_failure_Cmd_Count=0;
			
//			if(head_read_successs_flag)
//			{
//				head_read_successs_flag=0;
//				handle_head_read_success();
//			}
			
			if(head_read_successs_flag)
			{
				T_temp[0]=Temp;
				time_count=5;
				head_read_successs_flag=0;
				handle_head_read_success();
			}
			
			if(Sensor_State==1)
			{
				//上传传感器状态
				upSensorStatusFlag = 1;
				
				//上传温湿度
				updateEnvironmentFlag=1;
				
			}
			
			if(error_flag==1 && Sensor_State==1)
			{
				error_flag = 0;
				
				//上传传感器恢复状态
				Sensor_recover_Flag=1;
				
			}
			Sensor_State=0;

		}
		
		time_count++; //1s次数递增	
		
		if(time_count>=5)//5s到达
		{
			//显示到数码管上面
			_T =calculate_average(T_temp,5);
			time_count=0;
		}
	}
}


/*
***功能：处理传感器失败任务
***参数：无
***返回值：无
*/
void SensorError_Proress(void)
{
	if(Sensor_failure_Cmd_Count>=10)//当发送传感器故障失败命令次数大于10时
	{
		
		return ;
	}
	else
	{
		if(Sensor_Hour_Falg == 1)//当开启故障60分钟倒计时开关时
		{	
			
			return ;
		}
		else
		{
			if(Sensor_Fail_Count>=30)//故障状态  硬件故障超过30次以上时
			{
				//upSensorStatusFlag = 1;    //上传传感器状态
				
				error_flag =1;				//产生了读取异常错误
				
				head_read_successs_flag=0;	//若开机一直读取失败的话
				
				//Sensor_State =1;	 		//传感器异常状态
				
				//Sensor_Fail_Count =0;		//清除传感器失败次数
				
				Sensor_Fault_Flag =1;		//置位传感器错误标志位，发送故障错误命令
				
				Sensor_Hour_Falg  =1;		//开启故障60分钟重发开关
				
				Sensor_failure_Cmd_Count++;//记录发送传感器故障失败命令次数
			}
			else if(Sensor_Fail_Count <= 0)//正常状态
			{
				Sensor_Fail_Count = 0;//清除传感器失败次数
				
				Sensor_failure_Cmd_Count=0;;//清空发送传感器故障失败命令次数
			}
			else
			{
				return ;
			}
		}
	}
	return ;	
}


/*
***功能：矫正用户设定矫正数于环境温湿度
***参数：无
***返回：无
*/
void CalibrateT(void)
{
	if((FLASH_Save_User_Data.tempOffset+_T)>T_MAX)//温度范围-40至125摄氏度
	{
		T = T_MAX;
	}
	else if((FLASH_Save_User_Data.tempOffset+_T)<T_MIN)
	{
		T = T_MIN;
	}
	else
	{
		// 温度矫正
		T = FLASH_Save_User_Data.tempOffset+_T;
	}

}
/*
***功能：处理温度的当前值与用户设定值的功能
***参数：无
***返回：无
*/
void EnvSense_ProcessData(void) 
{
	//读取DS18B20的温度
	Read_Sensor_Data();

	//处理传感器读取失败任务
	SensorError_Proress();

	//矫正用户设置温度
	CalibrateT();
	
	if(Sensor_State == 0 )//读取状态正常情况下
	{
		// 处理温度超高/超低
		HandleTemperature(T);
		
		if(FLASH_Save_User_Data.switchMode == 0)//自动模式下
		{
			//根据温度控制开关
			control_switch(T);
		}
	}
}


