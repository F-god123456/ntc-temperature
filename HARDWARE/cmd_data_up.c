#include "main.h"



void updateEnvironmentCommand(void)
{
	updataTempCommand();
}


/*
***函数名：updataSwitchNewProCommand
***参数：无
***功能：该函数用于更新与开关、湿度和温度相关的命令。
***返回值：无
*/
void updataSwitchNewProCommand(void)
{
	updataSwitchStatusCommand();
	updataSensorStatusCommand();
	if(Sensor_State!=1)
		updataTempCommand();
}


/*
***函数名：updataSwitchALLDataCommand
***参数：无
***功能：该函数用于更新与开关相关的所有数据命令。
***返回值：无
*/
void updataSwitchALLDataCommand(void)
{
	updataSwitchModeCommand();
	updataSwitchOnTempCommand();
	updataSwitchOffTempCommand();
	updataSwitchtempOffsetCommand();
	updataSwitchhighTempAlarmCommand();
	updataSwitchhighMaxTempCommand();
	updataSwitchlowTempAlarmCommand();
	updataSwitchlowMinTempCommand();
	updataSwitchtempAlarmBufCommand();
}


/********************************
功能:上传温度命令到云端
参数:无
返回:无
*********************************/
void updataTempCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 't';
	sendCommandBuf[8]  = 'e';
	sendCommandBuf[9]  = 'm';
	sendCommandBuf[10] = 'p';
	sendCommandBuf[11] = ',';
	
	sprintf((char *)&sendCommandBuf[12], "%.1f", T);
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);
}
/********************************
功能:上传开关命令到云端
参数:无
返回:无
*********************************/
void updataSwitchStatusCommand(void)
{

	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 's';
	sendCommandBuf[8]  = 'w';
	sendCommandBuf[9]  = 'i';
	sendCommandBuf[10] = 't';
	sendCommandBuf[11] = 'c';
	sendCommandBuf[12] = 'h';
	sendCommandBuf[13] = 'S';
	sendCommandBuf[14] = 't';
	sendCommandBuf[15] = 'a';
	sendCommandBuf[16] = 't';
	sendCommandBuf[17] = 'u';
	sendCommandBuf[18] = 's';
	sendCommandBuf[19] = '1';
	sendCommandBuf[20] = ',';
	
	sendCommandBuf[21] = hex2chr(switch_state);//状态只有0和1
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传开关工作模式到云端
参数:无
返回:无
*********************************/
void updataSwitchModeCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 's';
	sendCommandBuf[8]  = 'w';
	sendCommandBuf[9]  = 'i';
	sendCommandBuf[10] = 't';
	sendCommandBuf[11] = 'c';
	sendCommandBuf[12] = 'h';
	sendCommandBuf[13] = 'M';
	sendCommandBuf[14] = 'o';
	sendCommandBuf[15] = 'd';
	sendCommandBuf[16] = 'e';

	sendCommandBuf[17] = ',';
	sendCommandBuf[18] = hex2chr(FLASH_Save_User_Data.switchMode);//状态只有0和1
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);
	
}
/********************************
功能:上传开关开始温度到云端
参数:无
返回:无
*********************************/
void updataSwitchOnTempCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 's';
	sendCommandBuf[8]  = 'w';
	sendCommandBuf[9]  = 'i';
	sendCommandBuf[10] = 't';
	sendCommandBuf[11] = 'c';
	sendCommandBuf[12] = 'h';
	sendCommandBuf[13] = 'O';
	sendCommandBuf[14] = 'n';
	sendCommandBuf[15] = 'T';
	sendCommandBuf[16] = 'e';
	sendCommandBuf[17] = 'm';
	sendCommandBuf[18] = 'p';
	sendCommandBuf[19] = ',';
	
	if(FLASH_Save_User_Data.switchOnTemp>0)
	{
		sprintf((char *)&sendCommandBuf[20], "%.1f", FLASH_Save_User_Data.switchOnTemp);
	}
	else
	{
		sprintf((char *)&sendCommandBuf[20], "%.1f", FLASH_Save_User_Data.switchOnTemp);
	}
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);
	
}

/********************************
功能:上传开关停止温度到云端
参数:无
返回:无
*********************************/
void updataSwitchOffTempCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 's';
	sendCommandBuf[8]  = 'w';
	sendCommandBuf[9]  = 'i';
	sendCommandBuf[10] = 't';
	sendCommandBuf[11] = 'c';
	sendCommandBuf[12] = 'h';
	sendCommandBuf[13] = 'O';
	sendCommandBuf[14] = 'f';
	sendCommandBuf[15] = 'f';
	sendCommandBuf[16] = 'T';
	sendCommandBuf[17] = 'e';
	sendCommandBuf[18] = 'm';
	sendCommandBuf[19] = 'p';
	sendCommandBuf[20] = ',';
	
	if(FLASH_Save_User_Data.switchOffTemp>0)
	{
		sprintf((char *)&sendCommandBuf[21], "%.1f", FLASH_Save_User_Data.switchOffTemp);
	}
	else
	{
		//sendCommandBuf[21] = '-';
		sprintf((char *)&sendCommandBuf[21], "%.1f", FLASH_Save_User_Data.switchOffTemp);
	}
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}
/********************************
功能:上传温度补偿到云端
参数:无
返回:无
*********************************/
void updataSwitchtempOffsetCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 't';
	sendCommandBuf[8]  = 'e';
	sendCommandBuf[9]  = 'm';
	sendCommandBuf[10] = 'p';
	sendCommandBuf[11] = 'O';
	sendCommandBuf[12] = 'f';
	sendCommandBuf[13] = 'f';
	sendCommandBuf[14] = 's';
	sendCommandBuf[15] = 'e';
	sendCommandBuf[16] = 't';
	sendCommandBuf[17] = ',';
	
	sprintf((char *)&sendCommandBuf[18], "%0.1f", FLASH_Save_User_Data.tempOffset);
	
	uart1_send(sendCommandBuf);

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}
/********************************
功能:上传超高温报警开关到云端
参数:无
返回:无
*********************************/
void updataSwitchhighTempAlarmCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 'h';
	sendCommandBuf[8]  = 'i';
	sendCommandBuf[9]  = 'g';
	sendCommandBuf[10] = 'h';
	sendCommandBuf[11] = 'T';
	sendCommandBuf[12] = 'e';
	sendCommandBuf[13] = 'm';
	sendCommandBuf[14] = 'p';
	sendCommandBuf[15] = 'A';
	sendCommandBuf[16] = 'l';
	sendCommandBuf[17] = 'a';
	sendCommandBuf[18] = 'r';
	sendCommandBuf[19] = 'm';
	sendCommandBuf[20] = ',';
	
	sprintf((char *)&sendCommandBuf[21], "%d", FLASH_Save_User_Data.highTempAlarm);
	
	uart1_send(sendCommandBuf);

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}
/********************************
功能:上传超高温度最高值到云端
参数:无
返回:无
*********************************/
void updataSwitchhighMaxTempCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 'h';
	sendCommandBuf[8]  = 'i';
	sendCommandBuf[9]  = 'g';
	sendCommandBuf[10] = 'h';
	sendCommandBuf[11] = 'M';
	sendCommandBuf[12] = 'a';
	sendCommandBuf[13] = 'x';
	sendCommandBuf[14] = 'T';
	sendCommandBuf[15] = 'e';
	sendCommandBuf[16] = 'm';
	sendCommandBuf[17] = 'p';
	sendCommandBuf[18] = ',';
	
	sprintf((char *)&sendCommandBuf[19], "%0.1f", FLASH_Save_User_Data.highMaxTemp);
	
	uart1_send(sendCommandBuf);

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}




/********************************
功能:上传超低温度报警开关到云端
参数:无
返回:无
*********************************/
void updataSwitchlowTempAlarmCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 'l';
	sendCommandBuf[8]  = 'o';
	sendCommandBuf[9]  = 'w';
	sendCommandBuf[10] = 'T';
	sendCommandBuf[11] = 'e';
	sendCommandBuf[12] = 'm';
	sendCommandBuf[13] = 'p';
	sendCommandBuf[14] = 'A';
	sendCommandBuf[15] = 'l';
	sendCommandBuf[16] = 'a';
	sendCommandBuf[17] = 'r';
	sendCommandBuf[18] = 'm';
	sendCommandBuf[19] = ',';
	
	sprintf((char *)&sendCommandBuf[20], "%d", FLASH_Save_User_Data.lowTempAlarm);
	
	uart1_send(sendCommandBuf);

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传超低温度最低值到云端
参数:无
返回:无
*********************************/
void updataSwitchlowMinTempCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 'l';
	sendCommandBuf[8]  = 'o';
	sendCommandBuf[9]  = 'w';
	sendCommandBuf[10] = 'M';
	sendCommandBuf[11] = 'i';
	sendCommandBuf[12] = 'n';
	sendCommandBuf[13] = 'T';
	sendCommandBuf[14] = 'e';
	sendCommandBuf[15] = 'm';
	sendCommandBuf[16] = 'p';
	sendCommandBuf[17] = ',';
	
	sprintf((char *)&sendCommandBuf[18], "%0.1f", FLASH_Save_User_Data.lowMinTemp);
	
	uart1_send(sendCommandBuf);

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传温度报警缓存区到云端
参数:无
返回:无
*********************************/
void updataSwitchtempAlarmBufCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 't';
	sendCommandBuf[8]  = 'e';
	sendCommandBuf[9]  = 'm';
	sendCommandBuf[10] = 'p';
	sendCommandBuf[11] = 'A';
	sendCommandBuf[12] = 'l';
	sendCommandBuf[13] = 'a';
	sendCommandBuf[14] = 'r';
	sendCommandBuf[15] = 'm';
	sendCommandBuf[16] = 'B';
	sendCommandBuf[17] = 'u';
	sendCommandBuf[18] = 'f';
	sendCommandBuf[19] = ',';
	

	sprintf((char *)&sendCommandBuf[20], "%0.1f", FLASH_Save_User_Data.tempAlarmBuf);

	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传超高温报警事件到云端
参数:无
返回:无
*********************************/
void updataOverTempAlarmCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'o';
	sendCommandBuf[10] = 'v';
	sendCommandBuf[11] = 'e';
	sendCommandBuf[12] = 'r';
	sendCommandBuf[13] = 'T';
	sendCommandBuf[14] = 'e';
	sendCommandBuf[15] = 'm';
	sendCommandBuf[16] = 'p';
	sendCommandBuf[17] = 'A';
	sendCommandBuf[18] = 'l';
	sendCommandBuf[19] = 'a';
	sendCommandBuf[20] = 'r';
	sendCommandBuf[21] = 'm';
	sendCommandBuf[22] = ',';
	
	sprintf((char *)&sendCommandBuf[23], "%0.1f",T);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}


/********************************
功能:上传超低温报警事件到云端
参数:无
返回:无
*********************************/
void updataLowTempAlarmCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'l';
	sendCommandBuf[10] = 'o';
	sendCommandBuf[11] = 'w';
	sendCommandBuf[12] = 'T';
	sendCommandBuf[13] = 'e';
	sendCommandBuf[14] = 'm';
	sendCommandBuf[15] = 'p';
	sendCommandBuf[16] = 'A';
	sendCommandBuf[17] = 'l';
	sendCommandBuf[18] = 'a';
	sendCommandBuf[19] = 'r';
	sendCommandBuf[20] = 'm';
	sendCommandBuf[21] = ',';
	sprintf((char *)&sendCommandBuf[22], "%0.1f", T);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传超高温恢复报警事件到云端
参数:无
返回:无
*********************************/
void updataOverTempAlarmRecoverCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'o';
	sendCommandBuf[10] = 'v';
	sendCommandBuf[11] = 'e';
	sendCommandBuf[12] = 'r';
	sendCommandBuf[13] = 'T';
	sendCommandBuf[14] = 'e';
	sendCommandBuf[15] = 'm';
	sendCommandBuf[16] = 'p';
	sendCommandBuf[17] = 'A';
	sendCommandBuf[18] = 'l';
	sendCommandBuf[19] = 'a';
	sendCommandBuf[20] = 'r';
	sendCommandBuf[21] = 'm';
	sendCommandBuf[22] = 'R';
	sendCommandBuf[23] = 'e';
	sendCommandBuf[24] = 'c';
	sendCommandBuf[25] = 'o';
	sendCommandBuf[26] = 'v';
	sendCommandBuf[27] = 'e';
	sendCommandBuf[28] = 'r';
	sendCommandBuf[29] = ',';
	sprintf((char *)&sendCommandBuf[30], "%0.1f", T);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}


/********************************
功能:上传超低温恢复报警事件到云端
参数:无
返回:无
*********************************/
void updataLowTempAlarmRecoverCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'l';
	sendCommandBuf[10] = 'o';
	sendCommandBuf[11] = 'w';
	sendCommandBuf[12] = 'T';
	sendCommandBuf[13] = 'e';
	sendCommandBuf[14] = 'm';
	sendCommandBuf[15] = 'p';
	sendCommandBuf[16] = 'A';
	sendCommandBuf[17] = 'l';
	sendCommandBuf[18] = 'a';
	sendCommandBuf[19] = 'r';
	sendCommandBuf[20] = 'm';
	sendCommandBuf[21] = 'R';
	sendCommandBuf[22] = 'e';
	sendCommandBuf[23] = 'c';
	sendCommandBuf[24] = 'o';
	sendCommandBuf[25] = 'v';
	sendCommandBuf[26] = 'e';
	sendCommandBuf[27] = 'r';
	sendCommandBuf[28] = ',';
	sprintf((char *)&sendCommandBuf[29], "%0.1f", T);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

/********************************
功能:上传传感器错误事件到云端
参数:无
返回:无
*********************************/
void updataSensorFaultCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'S';
	sendCommandBuf[10] = 'e';
	sendCommandBuf[11] = 'n';
	sendCommandBuf[12] = 's';
	sendCommandBuf[13] = 'o';
	sendCommandBuf[14] = 'r';
	sendCommandBuf[15] = 'F';
	sendCommandBuf[16] = 'a';
	sendCommandBuf[17] = 'u';
	sendCommandBuf[18] = 'l';
	sendCommandBuf[19] = 't';

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);
}


/********************************
功能:上传传感器恢复正常事件到云端
参数:无
返回:无
*********************************/
void updataSensorRecoverCommand(void)
{
	//AT+EVENT:SensorRecover\r\n
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'S';
	sendCommandBuf[10] = 'e';
	sendCommandBuf[11] = 'n';
	sendCommandBuf[12] = 's';
	sendCommandBuf[13] = 'o';
	sendCommandBuf[14] = 'r';
	
	sendCommandBuf[15] = 'R';
	sendCommandBuf[16] = 'e';
	sendCommandBuf[17] = 'c';
	sendCommandBuf[18] = 'o';
	sendCommandBuf[19] = 'v';
	sendCommandBuf[20] = 'e';
	sendCommandBuf[21] = 'r';
	
	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);
}


/********************************
功能:上传电源状态变化事件到云端
参数:无
返回:无
*********************************/
void updataPowerStatusCommand(void)
{
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'E';
	sendCommandBuf[4]  = 'V';
	sendCommandBuf[5]  = 'E';
	sendCommandBuf[6]  = 'N';
	sendCommandBuf[7]  = 'T';
	sendCommandBuf[8]  = ':';
	
	sendCommandBuf[9]  = 'p';
	sendCommandBuf[10] = 'o';
	sendCommandBuf[11] = 'w';
	sendCommandBuf[12] = 'e';
	sendCommandBuf[13] = 'r';
	sendCommandBuf[14] = 'S';
	sendCommandBuf[15] = 't';
	sendCommandBuf[16] = 'a';
	sendCommandBuf[17] = 't';
	sendCommandBuf[18] = 'u';
	sendCommandBuf[19] = 's';
	sendCommandBuf[20] = ',';
	sendCommandBuf[21] = hex2chr(power.State);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}



/********************************
功能:上传传感器状态到云端
参数:无
返回:无
*********************************/
void updataSensorStatusCommand(void)
{
	//AT+PRO:sensorStatus,1\r\n
	sendCommandBuf[0]  = 'A';
	sendCommandBuf[1]  = 'T';
	sendCommandBuf[2]  = '+';
	sendCommandBuf[3]  = 'P';
	sendCommandBuf[4]  = 'R';
	sendCommandBuf[5]  = 'O';
	sendCommandBuf[6]  = ':';
	
	sendCommandBuf[7]  = 's';
	sendCommandBuf[8]  = 'e';
	sendCommandBuf[9]  = 'n';
	sendCommandBuf[10] = 's';
	sendCommandBuf[11] = 'o';
	sendCommandBuf[12] = 'r';
	
	sendCommandBuf[13] = 'S';
	sendCommandBuf[14] = 't';
	sendCommandBuf[15] = 'a';
	sendCommandBuf[16] = 't';
	sendCommandBuf[17] = 'u';
	sendCommandBuf[18] = 's';
	sendCommandBuf[19] = ',';
	sendCommandBuf[20] = hex2chr(Sensor_State);

	uart1_send(sendCommandBuf); // 

	uart1_send((uint8_t *)"\r");
	
	memset(sendCommandBuf, 0, sendCommandBufLen);

}

