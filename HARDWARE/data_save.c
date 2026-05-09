#include "main.h"


/***********************************************
功能：开关模式保存到FLASH
参数：无
返回：无
***********************************************/
void switchModeSave(uint8_t SW_Mode)
{
	FLASH_Save_User_Data.switchMode = SW_Mode;
	FLASH_SAVE_Sign=1;
}
/***********************************************
功能：开关启动温度保存到FLASH
参数：无
返回：无
***********************************************/
void switchOnTempSave(double Temp)
{
	FLASH_Save_User_Data.switchOnTemp = Temp;
	FLASH_SAVE_Sign=1;
}

/***********************************************
功能：开关停止温度保存到FLASH
参数：无
返回：无
***********************************************/
void switchOffTempSave(double Temp)
{
	FLASH_Save_User_Data.switchOffTemp = Temp;
	FLASH_SAVE_Sign=1;
}

/***********************************************
功能：开关温度补偿保存到FLASH
参数：无
返回：无
***********************************************/
void switchtempOffsetSave(double TempOffset)
{
	FLASH_Save_User_Data.tempOffset = TempOffset;
	FLASH_SAVE_Sign=1;
}

/***********************************************
功能：超高温报警开关保存到FLASH
参数：无
返回：无
***********************************************/
void switchhighTempAlarmSave(bool highTempAlarm)
{
	FLASH_Save_User_Data.highTempAlarm = highTempAlarm;
	FLASH_SAVE_Sign=1;
}

/***********************************************
功能：超高温度最高值保存到FLASH
参数：无
返回：无
***********************************************/
void switchhighMaxTempSave(double highMaxTemp)
{
	FLASH_Save_User_Data.highMaxTemp = highMaxTemp;
	FLASH_SAVE_Sign=1;
}




/***********************************************
功能：超低温度报警开关保存到FLASH
参数：无
返回：无
***********************************************/
void switchlowTempAlarmSave(bool lowTempAlarm)
{
	FLASH_Save_User_Data.lowTempAlarm = lowTempAlarm;
	FLASH_SAVE_Sign=1;

}
/***********************************************
功能：超低温度报警开关保存到FLASH
参数：无
返回：无
***********************************************/
void switchlowMinTempSave(double lowMinTemp)
{
	FLASH_Save_User_Data.lowMinTemp = lowMinTemp;
	FLASH_SAVE_Sign=1;

}

/***********************************************
功能：温度报警缓存区保存到FLASH
参数：无
返回：无
***********************************************/
void switchtempAlarmBufSave(double tempAlarmBuf)
{
	FLASH_Save_User_Data.tempAlarmBuf = tempAlarmBuf;
	FLASH_SAVE_Sign=1;

}

