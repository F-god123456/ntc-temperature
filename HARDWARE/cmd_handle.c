#include "main.h"
#include "cmd_handle.h"

//命令缓存区
uint8_t commandBuf[commandBufLen];
//命令缓存区的索引
uint8_t commandIndex = 0;
//收到命令的头部的标志位
uint8_t headCommandFlag = 0;
//发送命令的缓存区
uint8_t sendCommandBuf[sendCommandBufLen];

//PRO 命令
const uint8_t PROStr[] = "PRO";
//SER 命令
const uint8_t SERStr[] = "SER";
//EVENT 命令
const uint8_t EVENTStr[] = "EVENT";
//MQTT 命令
const uint8_t mqttStr[] ="mqtt";

//联网成功标志  1 ：成功  0：失败
uint8_t MqttSuccess = 0;

// 属性标识符
const uint8_t switchStatusStr[] = "switchStatus1";
const uint8_t switchModeStr[]   = "switchMode";
const uint8_t switchOnTempStr[] = "switchOnTemp";
const uint8_t switchOffTempStr[] = "switchOffTemp";
const uint8_t switchtempOffsetStr[] = "tempOffset";
const uint8_t switchhighTempAlarmStr[] = "highTempAlarm";
const uint8_t switchhighMaxTempStr[] = "highMaxTemp";
const uint8_t switchlowTempAlarmStr[] = "lowTempAlarm";
const uint8_t switchlowMinTempStr[] = "lowMinTemp";
const uint8_t switchtempAlarmBufStr[] = "tempAlarmBuf";
const uint8_t switchUpDeviProStr[] = "upDeviPro";
const uint8_t switchUpNewProStr[] = "upNewPro";
const uint8_t eliminateAlarmStr[] ="eliminateAlarm";

// 命令上传标志位
uint8_t upSwitchStatusFlag = 0;	      // 上传开关状态标志位
uint8_t upSwitchModeFlag = 0;	      // 上传开关模式标志位
uint8_t upSwitchOnTempFlag = 0;	      // 上传开关起始温度标志位  
uint8_t upSwitchOffTempFlag =0;       // 上传开关停止温度标志位 
uint8_t upSwitchtempOffsetFlag=0;     // 上传开关温度补偿标志位 
uint8_t upSwitchhighTempAlarmFlag=0;  // 上传开关超高温度报警开关标志位 
uint8_t upSwitchhighMaxTempFlag=0;    // 上传开关超高温度最高值标志位 
uint8_t upSwitchlowTempAlarmFlag=0;   // 上传开关超低温度报警开关标志位 
uint8_t upSwitchlowMinTempFlag=0;     // 上传开关超低温度最低值标志位 
uint8_t upSwitchtempAlarmBufFlag=0;   // 上传温度报警缓存区标志位
uint8_t upswitchUpDeviProFlag=0;  	  // 上传全部参数标志位
uint8_t upswitchUpNewProFlag=0;		  // //上传设备实时指令标志位
uint8_t updateEnvironmentFlag=0; 	  //上传环境指令标志位

static uint8_t index;//下标

uint8_t head_A_flag = 0;//头命令A标志位
uint8_t head_T_flag = 0;//头命令T标志位

//发送回车键标志位置1
uint8_t upEnter = 0;


uint8_t checkPRO(void);
uint8_t checkSER(void);
uint8_t checkMqtt(uint8_t* buf);


/*****************************************************
函数名称：
函数功能：上传设备属性
输入参数：无
输出参数：无
******************************************************/
void DeviProCmd_monitor(void)
{
	if(MqttSuccess==1)
	{
		//上传电源状态指令
		if(updataPowerStatusFlag!=0)
		{
			updataPowerStatusFlag=0;
			updataPowerStatusCommand();//上传
			upEnter = 1;//发送回车键标志位置1
		}
		//上传开关状态指令
		if(upSwitchStatusFlag != 0)
		{
			upSwitchStatusFlag = 0;
			updataSwitchStatusCommand();//上传全部开关状态
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传开关模式指令
		if(upSwitchModeFlag !=0)
		{
			upSwitchModeFlag = 0;
			updataSwitchModeCommand();//上传开关模式
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传开关开启温度指令
		if(upSwitchOnTempFlag !=0)
		{
			upSwitchOnTempFlag = 0;
			updataSwitchOnTempCommand();//上传开关开启温度
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传开关停止温度指令
		if(upSwitchOffTempFlag !=0)
		{
			upSwitchOffTempFlag = 0;
			updataSwitchOffTempCommand();//上传开关开启温度
			upEnter = 1;//发送回车键标志位置1
		}
			
		//上传开关温度补偿指令
		if(upSwitchtempOffsetFlag !=0)
		{
			upSwitchtempOffsetFlag = 0;
			updataSwitchtempOffsetCommand();//
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超高温度警报指令
		if(upSwitchhighTempAlarmFlag !=0)
		{
			upSwitchhighTempAlarmFlag = 0;
			updataSwitchhighTempAlarmCommand();//
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超高温度最大值指令
		if(upSwitchhighMaxTempFlag !=0)
		{
			upSwitchhighMaxTempFlag = 0;
			updataSwitchhighMaxTempCommand();//
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超低温报警指令
		if(upSwitchlowTempAlarmFlag !=0)
		{
			upSwitchlowTempAlarmFlag = 0;
			updataSwitchlowTempAlarmCommand();//
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超低温度值指令
		if(upSwitchlowMinTempFlag !=0)
		{
			upSwitchlowMinTempFlag = 0;
			updataSwitchlowMinTempCommand();//
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传温度报警缓存区指令
		if(upSwitchtempAlarmBufFlag !=0)
		{
			upSwitchtempAlarmBufFlag = 0;
			updataSwitchtempAlarmBufCommand();//上传
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超高温度报警指令
		if(SuperHeatFlag !=0 )
		{	
			SuperHeatFlag=0;
			updataOverTempAlarmCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超低温度报警指令
		if(LowTempFlag !=0 )
		{	
			LowTempFlag=0;
			updataLowTempAlarmCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超高温度恢复报警指令
		if(OverTempRecover_Flag !=0)
		{
			OverTempRecover_Flag=0;
			updataOverTempAlarmRecoverCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传超低温度恢复报警指令
		if(LowTempRecover_Flag !=0)
		{
			LowTempRecover_Flag=0;
			updataLowTempAlarmRecoverCommand();
			upEnter = 1;//发送回车键标志位置1
		}

		//上传传感器错误指令
		if(Sensor_Fault_Flag !=0)
		{
			Sensor_Fault_Flag =0;
			updataSensorFaultCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传传感器恢复正常指令
		if(Sensor_recover_Flag !=0)
		{
			Sensor_recover_Flag =0;
			updataSensorRecoverCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传传感器状态指令
		if(upSensorStatusFlag !=0)
		{
			upSensorStatusFlag =0;
			updataSensorStatusCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传环境温湿度状态指令
		if(updateEnvironmentFlag !=0)
		{
			updateEnvironmentFlag =0;
			updateEnvironmentCommand();
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传设备属性服务指令
		if(upswitchUpDeviProFlag!=0)
		{
			upswitchUpDeviProFlag = 0;
			updataSwitchALLDataCommand();//上传
			upEnter = 1;//发送回车键标志位置1
		}
		
		//上传设备实时指令
		if(upswitchUpNewProFlag!=0)
		{
			upswitchUpNewProFlag = 0;
			updataSwitchNewProCommand();//上传
			upEnter = 1;//发送回车键标志位置1
		}
		
		//发送回车
		if(upEnter != 0)
		{
			upEnter = 0;
			uart1_send((uint8_t *)"\n");//发送回车键
		}
	}
}
/*********************************************************************************
函数功能:解析湿度报警缓存区命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:humidAlarmBuf,10\r\n
*********************************************************************************/
uint8_t checktempAlarmBuf(uint8_t *buf)
{
	uint8_t length,i;
	uint8_t temp[5];
	double tempAlarmBuf;
	index =12;//','坐标位置
	if (strVS(buf, switchtempAlarmBufStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[index] == ',' && buf[index+1] != '-')
		{
			
			if(buf[index+2]=='.')//个位处理  如：2.5\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+3]);
				tempAlarmBuf=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+3]=='.')//十位处理  如：12.2\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+4]);
				tempAlarmBuf=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+4]=='.')//百位处理  如：199.9\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+3]);
				temp[3]=chr2hex(buf[index+5]);
				tempAlarmBuf=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[index+1+length]!='\r' && buf[index+1+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+1+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					tempAlarmBuf += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		else{
		
			if(buf[index+3]=='.')//个位处理  如：-2.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+4]);
				tempAlarmBuf=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+4]=='.')//十位处理  如：-12.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+5]);
				tempAlarmBuf=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+5]=='.')//百位处理  如：-199.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+4]);
				temp[3]=chr2hex(buf[index+6]);
				tempAlarmBuf=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[index+2+length]!='\r' && buf[index+2+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+2+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					tempAlarmBuf += temp[i] * pow(10, length - i - 1);
				}
			}
			tempAlarmBuf=-tempAlarmBuf;
		}
		
		//保存数据到FLASH
		switchtempAlarmBufSave(tempAlarmBuf);
		
		// 上传超低温度报警开关
		upSwitchtempAlarmBufFlag = 1;
		
		return 1;
	
	}
	return 0;
}


/*********************************************************************************
函数功能:解析超低温度最低值命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:lowMinTemp,30\r\n
*********************************************************************************/
uint8_t checklowMinTemp(uint8_t *buf)
{
	uint8_t temp[5];
	uint8_t length,i;
	double lowMinTemp;
	index =10;//','坐标位置
	if (strVS(buf, switchlowMinTempStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[index] == ',' && buf[index+1] != '-')
		{
			
			if(buf[index+2]=='.')//个位处理  如：AT+PRO:lowMinTemp,2.5\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+3]);
				lowMinTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+3]=='.')//十位处理  如：AT+PRO:lowMinTemp,12.2\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+4]);
				lowMinTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+4]=='.')//百位处理  如：AT+PRO:lowMinTemp,199.9\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+3]);
				temp[3]=chr2hex(buf[index+5]);
				lowMinTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else//AT+PRO:lowMinTemp,12\r\n
			{
				length=0;
				while(buf[index+1+length]!='\r' && buf[index+1+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+1+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					lowMinTemp += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		//负温度处理
		else//14
		{
			if(buf[index+3]=='.')//个位处理  如：-2.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+4]);
				lowMinTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+4]=='.')//十位处理  如：-12.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+5]);
				lowMinTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+5]=='.')//百位处理  如：-199.0\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+4]);
				temp[3]=chr2hex(buf[index+6]);
				lowMinTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[index+2+length]!='\r' && buf[index+2+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+2+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					lowMinTemp += temp[i] * pow(10, length - i - 1);
				}
			}
			lowMinTemp=-lowMinTemp;
			
		}
		//保存数据到FLASH
		switchlowMinTempSave(lowMinTemp);
		
		// 上传开关打开开始温度
		upSwitchlowMinTempFlag = 1;
		return 1;
	}
	return 0;
	
}
/*********************************************************************************
函数功能:解析超低温度报警开关命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:lowTempAlarm,1\r\n
*********************************************************************************/
uint8_t checklowTempAlarm(uint8_t *buf)
{

	int lowTempAlarm;
	index =12;//','坐标位置
	if (strVS(buf, switchlowTempAlarmStr) == 1) // 判断属性值
	{
		if (buf[index] == ',')
		{
			lowTempAlarm = chr2hex((buf[index+1])); // 获取开关索引
		}
		//保存数据到FLASH
		switchlowTempAlarmSave(lowTempAlarm);
		
		// 上传超低温度报警开关
		upSwitchlowTempAlarmFlag = 1;
		return 1;
	}
	return 0;
}

/*********************************************************************************
函数功能:解析开关超高温度最高值命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:highMaxTemp,30\r\n
*********************************************************************************/
uint8_t checkhighMaxTemp(uint8_t *buf)
{
	uint8_t temp[5];
	uint8_t length,i;
	double highMaxTemp=0;
	index =11;//','坐标位置
	if(strVS(buf, switchhighMaxTempStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[index] == ',' && buf[index+1] != '-')
		{
			
			if(buf[index+2]=='.')//个位处理  如：AT+PRO:highMaxTemp,2.4\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+3]);
				highMaxTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+3]=='.')//十位处理  如：AT+PRO:highMaxTemp,12.2\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+4]);
				highMaxTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+4]=='.')//百位处理  如：AT+PRO:highMaxTemp,100.5\r\n
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+3]);
				temp[3]=chr2hex(buf[index+5]);
				highMaxTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else//如：AT+PRO:highMaxTemp,100\r\n
			{
				length=0;
				while(buf[index+1+length]!='\r' && buf[index+1+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+1+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					
					highMaxTemp += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		//负温度处理
		else//14
		{
			if(buf[index+3]=='.')    //个位处理   如：AT+PRO:highMaxTemp,-2.2\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+4]);
				highMaxTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+4]=='.')//十位处理  如：AT+PRO:highMaxTemp,-12.6\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+5]);
				highMaxTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[index+5]=='.')//百位处理  如：AT+PRO:highMaxTemp,-120.4\r\n
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+4]);
				temp[3]=chr2hex(buf[index+6]);
				highMaxTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[index+2+length]!='\r' && buf[index+2+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+2+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					
					highMaxTemp += temp[i] * pow(10, length - i - 1);
				}
			}
			highMaxTemp=-highMaxTemp;
			
		}
		//保存数据到FLASH
		switchhighMaxTempSave(highMaxTemp);
		
		// 上传开关打开开始温度
		upSwitchhighMaxTempFlag = 1;
		return 1;
	}
	return 0;
}
/*********************************************************************************
函数功能:解析超高温报警开关命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:highTempAlarm,1\r\n
*********************************************************************************/
uint8_t checkhighTempAlarm(uint8_t *buf)
{
	bool highTempAlarm;
	index =13;//','坐标位置
	if (strVS(buf, switchhighTempAlarmStr) == 1) // 判断属性值
	{
		
		if (buf[index] == ',')
		{
			highTempAlarm = chr2hex((buf[index+1])); // 获取开关索引
		}
		//保存数据到FLASH
		switchhighTempAlarmSave(highTempAlarm);
		
		// 上传超高温报警开关
		upSwitchhighTempAlarmFlag = 1;
		return 1;
	}
	return 0;
}
/*********************************************************************************
函数功能:解析开关温度补偿命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:tempOffset,5.6\r\n
*********************************************************************************/
uint8_t checktempOffset(uint8_t *buf)
{
	uint8_t length,i;
	uint8_t temp[5];
	double tempOffset;
	index =10;//','坐标位置
	if (strVS(buf, switchtempOffsetStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[index] == ',' && buf[index+1] != '-')
		{
			
			if(buf[index+2]=='.')//个位处理  如：2.6
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+3]);
				tempOffset=temp[0]+(temp[1]/10.0);
			}
			else if(buf[index+3]=='.')//十位处理  如：12.6
			{
				temp[0]=chr2hex(buf[index+1]);
				temp[1]=chr2hex(buf[index+2]);
				temp[2]=chr2hex(buf[index+4]);
				tempOffset=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
//			else if(buf[index+4]=='.')//百位处理  如：199.9
//			{
//				temp[0]=chr2hex(buf[index+1]);
//				temp[1]=chr2hex(buf[index+2]);
//				temp[2]=chr2hex(buf[index+3]);
//				temp[3]=chr2hex(buf[index+5]);
//				tempOffset=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
//			}
			else{
			
				length=0;
				while(buf[index+1+length]!='\r' && buf[index+1+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+1+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					tempOffset += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		//负温度处理
		else//14
		{
			if(buf[index+3]=='.')//个位处理  如：-2.6
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+4]);
				tempOffset=-(temp[0]+(temp[1]/10.0));
			}
			else if(buf[index+4]=='.')//十位处理  如：-12.6
			{
				temp[0]=chr2hex(buf[index+2]);
				temp[1]=chr2hex(buf[index+3]);
				temp[2]=chr2hex(buf[index+5]);
				tempOffset=-((temp[0]*10)+temp[1]+(temp[2]/10.0));
			}
//			else if(buf[index+5]=='.')//百位处理  如：-199.9
//			{
//				temp[0]=chr2hex(buf[index+2]);
//				temp[1]=chr2hex(buf[index+3]);
//				temp[2]=chr2hex(buf[index+4]);
//				temp[3]=chr2hex(buf[index+6]);
//				tempOffset=-((temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0));
//			}
			else
			{
				length=0;
				while(buf[index+1+length]!='\r' && buf[index+1+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[index+1+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					tempOffset += temp[i] * pow(10, length - i - 1);
				}
				tempOffset=-tempOffset;
			}
			
		}
		//保存数据到FLASH
		switchtempOffsetSave(tempOffset);
		
		// 上传开关打开开始温度
		upSwitchtempOffsetFlag = 1;
		return 1;
	}
	return 0;

}
/*********************************************************************************
函数功能:解析开关关闭温度命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:switchOffTemp,32.6\r\n
*********************************************************************************/
uint8_t checkswitchOffTemp(uint8_t *buf)
{
	int i;
	uint8_t length;
	uint8_t temp[5];
	double switchOffTemp;
	if (strVS(buf, switchOffTempStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[13] == ',' && buf[14] != '-')
		{
			
			if(buf[15]=='.')//个位处理  如：2.6
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[16]);
				switchOffTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[16]=='.')//十位处理  如：12.6
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[15]);
				temp[2]=chr2hex(buf[17]);
				switchOffTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[17]=='.')//百位处理  如：199.9
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[15]);
				temp[2]=chr2hex(buf[16]);
				temp[3]=chr2hex(buf[18]);
				switchOffTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[14+length]!='\r' && buf[14+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[14+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					switchOffTemp += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		//负温度处理
		else//14
		{
			if(buf[16]=='.')//个位处理  如：-2.6
			{
				temp[0]=chr2hex(buf[15]);
				temp[1]=chr2hex(buf[17]);
				switchOffTemp=-(temp[0]+(temp[1]/10.0));
			}
			else if(buf[17]=='.')//十位处理  如：-12.6
			{
				temp[0]=chr2hex(buf[15]);
				temp[1]=chr2hex(buf[16]);
				temp[2]=chr2hex(buf[18]);
				switchOffTemp=-((temp[0]*10)+temp[1]+(temp[2]/10.0));
			}
			else if(buf[18]=='.')//百位处理  如：-199.9
			{
				temp[0]=chr2hex(buf[15]);
				temp[1]=chr2hex(buf[16]);
				temp[2]=chr2hex(buf[17]);
				temp[3]=chr2hex(buf[19]);
				switchOffTemp=-((temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0));
			}
			else
			{
				length=0;
				while(buf[14+length]!='\r' && buf[14+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[14+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					switchOffTemp += temp[i] * pow(10, length - i - 1);
				}
				switchOffTemp=-switchOffTemp;
			
			}
			
		}
		
		//保存数据到FLASH
		switchOffTempSave(switchOffTemp);
		
		// 上传开关打开开始温度
		upSwitchOffTempFlag = 1;
		return 1;
	}
	return 0;
}
/*********************************************************************************
函数功能:解析开关开启温度命令
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:switchOffTemp,32.6\r\n
*********************************************************************************/
uint8_t checkswitchOnTemp(uint8_t *buf)
{
	int i;
	uint8_t temp[5];
	double switchOnTemp;
	uint8_t length;
	if (strVS(buf, switchOnTempStr) == 1) // 判断属性值
	{
		//正温度处理
		if (buf[12] == ',' && buf[13] != '-')
		{
			if(buf[14]=='.')//个位处理  如：2.6
			{
				temp[0]=chr2hex(buf[13]);
				temp[1]=chr2hex(buf[15]);
				switchOnTemp=temp[0]+(temp[1]/10.0);
			}
			else if(buf[15]=='.')//十位处理  如：12.6
			{
				temp[0]=chr2hex(buf[13]);
				temp[1]=chr2hex(buf[14]);
				temp[2]=chr2hex(buf[16]);
				switchOnTemp=(temp[0]*10)+temp[1]+(temp[2]/10.0);
			}
			else if(buf[16]=='.')//百位处理  如：199.9
			{
				temp[0]=chr2hex(buf[13]);
				temp[1]=chr2hex(buf[14]);
				temp[2]=chr2hex(buf[15]);
				temp[3]=chr2hex(buf[17]);
				switchOnTemp=(temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0);
			}
			else
			{
				length=0;
				while(buf[13+length]!='\r' && buf[13+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[13+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					switchOnTemp += temp[i] * pow(10, length - i - 1);
				}
			}
		}
		//负温度处理
		else//14
		{
			if(buf[15]=='.')//个位处理  如：-2.6
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[16]);
				switchOnTemp=-(temp[0]+(temp[1]/10.0));
			}
			else if(buf[16]=='.')//十位处理  如：-12.6
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[15]);
				temp[2]=chr2hex(buf[17]);
				switchOnTemp=-((temp[0]*10)+temp[1]+(temp[2]/10.0));
			}
			else if(buf[17]=='.')//百位处理  如：-199.9
			{
				temp[0]=chr2hex(buf[14]);
				temp[1]=chr2hex(buf[15]);
				temp[2]=chr2hex(buf[16]);
				temp[3]=chr2hex(buf[18]);
				switchOnTemp=-((temp[0]*100)+(temp[1]*10)+temp[2]+(temp[3]/10.0));
			}
			else
			{
				length=0;
				while(buf[13+length]!='\r' && buf[13+(length+1)]!='\n')
					length++;
				for( i =0;i<length;i++)
				{
					temp[i]=chr2hex(buf[13+i]);
				}
				for ( i = 0; i < length; i++) 
				{
					switchOnTemp += temp[i] * pow(10, length - i - 1);
				}
				switchOnTemp=-switchOnTemp;
			}
			
		}
		
		//保存数据到FLASH
		switchOnTempSave(switchOnTemp);
		
		// 上传开关打开开始温度
		upSwitchOnTempFlag = 1;
		return 1;
	}
	return 0;

}

/*********************************************************************************
函数功能:解析开关的模式
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:switchMode,1\r\n
*********************************************************************************/
uint8_t checkswitchMode(uint8_t *buf)
{
	uint8_t switchModeVal;
	if (strVS(buf, switchModeStr) == 1) // 判断属性值
	{
		if (buf[10] == ',')
		{
			switchModeVal = chr2hex((buf[11])); // 获取开关索引
		}
		//保存数据到FLASH
		switchModeSave(switchModeVal);
		
		Led_Auto_Control(!switchModeVal);
		// 上传开关模式
		upSwitchModeFlag = 1;
		return 1;
	}
	return 0;

}
/*********************************************************************************
函数功能:解析开关的状态
参数:属性标识符字符串
返回:0 命令错误; 1 命令正确
命令说明:AT+PRO:switchStatus1,1\r\n
*********************************************************************************/
uint8_t checkrelayStatus(uint8_t *buf)
{
	uint8_t switchStatusVal;			  // 保存开关状态
	if (strVS(buf, switchStatusStr) == 1) // 判断属性值
	{
		if (buf[13] == ',')
		{
			switchStatusVal = chr2hex((buf[14])); // 获取开关索引
		}
		//控制开关通或断
		ControlRelay(switchStatusVal);
		// 上传全部开关状态
		upSwitchStatusFlag = 1;
		return 1;
	}
	return 0;
}

/*****************************************************
函数名称:监测PRO属性的命令
输入参数:无
输出参数:0:指令错误;1:指令正确
*****************************************************/
uint8_t checkPRO(void)
{
	//检测串口是否有命令：以"PRO"开头
	if(strVS(commandBuf,PROStr) == 1)//判断是否为命令类型属性
	{
		//设置开关继电器状态
		if(checkrelayStatus(commandBuf+4) == 1)
		{
			return 1;
		}
		//设置开关继电器模式
		if(checkswitchMode(commandBuf+4) == 1)
		{
			return 1;
		}
		//设置开关启动温度
		if(checkswitchOnTemp(commandBuf+4) == 1)
		{
			return 1;
		}
		//设置开关停止温度
		if(checkswitchOffTemp(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置开关温度补偿
		if(checktempOffset(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置超高温度报警开关
		if(checkhighTempAlarm(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置超高温度最高值
		if(checkhighMaxTemp(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置超低温度报警开关
		if(checklowTempAlarm(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置超低温度最低值
		if(checklowMinTemp(commandBuf+4) == 1)
		{
			return 1;
		}
		
		//设置温度报警缓存区
		if(checktempAlarmBuf(commandBuf+4) == 1)
		{
			return 1;
		}
	}
	return 0;
}
/*********************************************************************************
函数功能:解析开关上传的状态
参数:属性标识符字符串
返回值:0 命令错误; 1 命令正确
命令说明:AT+SER:upNewPro
*********************************************************************************/
uint8_t checkswitchUpDeviProStr(uint8_t *buf)
{
	if (strVS(buf, switchUpDeviProStr) == 1) // 判断属性值
	{
		upswitchUpDeviProFlag = 1; // 上传开关模式的所有参数

		return 1;
	}
	return 0;
}
/*********************************************************************************
函数功能:解析开关相关属性
参数:属性标识符字符串
返回值:0 命令错误; 1 命令正确
命令说明:AT+SER:upNewPro
*********************************************************************************/
uint8_t checkswitchUpNewProStr(uint8_t *buf)
{
	if (strVS(buf, switchUpNewProStr) == 1) // 判断属性值
	{
		upswitchUpNewProFlag = 1;
		return 1;
	}
	return 0;
}
/*********************************************************************************
函数功能:解析解除蜂鸣器报警
参数:属性标识符字符串
返回值:0 命令错误; 1 命令正确
命令说明:AT+SER:eliminateAlarm\r\n
*********************************************************************************/
uint8_t checkeliminateAlarmStr(uint8_t *buf)
{
	if (strVS(buf, eliminateAlarmStr) == 1) // 判断属性值
	{
		//设置蜂鸣器状态
		beep_state=0;
		//关掉蜂鸣器
		BEEP_OFF();
		
		return 1;
	}
	return 0;

}
/*****************************************************
函数名称:监测SER属性的命令
输入参数:无
输出参数:0:指令错误;1:指令正确
*****************************************************/
uint8_t checkSER(void)
{
	//检测串口是否有命令：以"PRO"开头
	if(strVS(commandBuf,SERStr) == 1)//判断是否为命令类型属性
	{
		//命令：AT+SER:upDeviPro
		if(checkswitchUpDeviProStr(commandBuf+4) == 1)
		{
			return 1;
		}
		//命令:AT+SER:upNewPro
		if(checkswitchUpNewProStr(commandBuf+4) == 1)
		{
			return 1;
		}
		//命令:AT+SER:eliminateAlarm\r\n
		if(checkeliminateAlarmStr(commandBuf+4) == 1)
		{
			return 1;
		}
		//接收联网与断网消息:AT+SER:mqtt,1
		if(checkMqtt(commandBuf+4) == 1)
		{
			return 1;
		}
	}
	return 0;
	
}
/*****************************************************
函数名称:监测MQTT的命令
输入参数:无
输出参数:0:指令错误;1:指令正确
*****************************************************/
uint8_t checkMqtt(uint8_t* buf)
{
	uint8_t val;
	if(strVS(buf,mqttStr) == 1)//判断属性值
	{
		//AT+SER:mqtt,1\r\n
		if((buf[4] == ',')&&((buf[5] == '0')||(buf[5] == '1'))&&(buf[6] == '\r'))
		{
			val = chr2hex((buf[5]));
			
			//联网失败
			if(val == 0)
			{
				MqttSuccess = 0;
				batteryClose();//关闭备用电池
			}
			//联网成功
			else if(val == 1)
			{
				MqttSuccess = 1;
				batteryOpen(); //打开备用电池
				batteryDelayedClose = 0;
			}
			return 1;
		}
	}
	return 0;
	
}
/*****************************************************
函数名称:执行命令
输入参数:无
输出参数:0:指令错误;1:指令正确
*****************************************************/
uint8_t executiveCommand(void)
{
	//AT+PRO
	if(checkPRO()==1)
	{
		return 1;
	}
	//AT+SET
	if(checkSER()==1)
	{
		return 1;
	}
	return 0;
}


/*****************************************************
函数名称:检测串口数据是否有命令
输入参数:无
输出参数:无
*****************************************************/
void checkCommand(void)
{
	while(head != trail)
	{
		//有数据
		if(headCommandFlag == 0)//头命令结束标志位
		{
			//没有收到命令的头部
			if(head_A_flag == 0)
			{
				if(rcvBuf[head] == 'A')//还没接收到A时
				{
					head_A_flag = 1;//头命令A标志位
				}
			}
			else if(head_T_flag == 0)
			{
				//收到A，还没有收到T
				if(rcvBuf[head] == 'T')
				{
					head_T_flag = 1;//头命令T标志位
				}
				else
				{
					head_A_flag = 0;//头命令A标志位
					head_T_flag = 0;//头命令T标志位
				}
			}
			else if((head_A_flag == 1) && (head_T_flag == 1) && rcvBuf[head] == '+')
			{
				//收到AT+
				headCommandFlag = 1;//头命令结束标志位
				commandIndex = 0;//命令缓存区的索引
			}
			head++;//解析命令数组索引//获取串口缓存的下一个数据
			if(head >= rcvBufLen)
			{
				head = head - rcvBufLen;
			}
		}
		else
		{
			//已经收到了命令的头部
			commandBuf[commandIndex] = rcvBuf[head];
			//获取串口缓存的下一个数据
			head++;//解析命令数组索引//获取串口缓存的下一个数据
			if(head >= rcvBufLen)
			{
				head = head - rcvBufLen;
			}
			if(commandIndex >= commandBufLen)
			{
				//命令长度超出了命令缓存区
				//丢弃命令缓存区的数据
				commandIndex = 0;//命令缓存区的索引
				headCommandFlag = 0; //头命令结束标志位
			}
			else
			{
				commandIndex++;//命令缓存区的索引
			}
			//判断命令结尾是否为回车换行
			if((commandBuf[commandIndex - 1] == 0x0A) && (commandBuf[commandIndex - 2] == 0x0D))   //判断数据是否为命令的尾部
			{
				commandBuf[commandIndex] = 0;//收到命令尾部，命令缓存区手动添加字符串结束符
				executiveCommand();//执行命令
				commandIndex = 0;//命令缓存区的索引
				headCommandFlag = 0; //头命令结束标志位
				head_A_flag = 0;//头命令A标志位
				head_T_flag = 0;//头命令T标志位
			}
		}
	}
}


/*****************************************************
函数名称：	
函数功能：命令处理								                 							                 
输入参数：无										                    
输出参数：无							   	                  
******************************************************/
void monitor(void)
{
	//命令检测
	checkCommand();
	
	//上传设备属性
    DeviProCmd_monitor();
	
	
	//上传回车换行键表示发送结束
    if(upEnter != 0)
    {
        upEnter = 0;
        // 发送回车键
        uart1_send((uint8_t *)"\n");
    }
}



