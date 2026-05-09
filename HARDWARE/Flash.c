#include "main.h"

//向FLASH保存用户参数
uint8_t FLASH_Save_User_Flag = 0;

//初始化用户参数的具体数据结构体
_FLASH_Save_User_Data FLASH_Save_User_Data;

//保存数据标志位
uint8_t FLASH_SAVE_Sign = 0;


void printUserParams(const _FLASH_Save_User_Data *params);

/********************************* 
功能：FLASH初始化
参数：无
返回：无
*********************************/
void FLASH_Init(void)
{
	/*解锁FLASH*/
	FLASH_Unlock();
	/*清除FLASH的标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_PGERR); // FLASH_FLAG_PGERR
	/*上锁FLASH*/
	FLASH_Lock();
}

/********************************
功能:用户参数恢复出厂设置
参数:无
返回:无
*********************************/
void user_parameter_Deinit(void)
{
	FLASH_Save_User_Data.switchMode  =0;//自动模式
	
	FLASH_Save_User_Data.switchOnTemp=-55;//默认开关启动温度为-40℃
	FLASH_Save_User_Data.switchOffTemp=125;//默认开关关闭温度为125℃
	
	FLASH_Save_User_Data.tempOffset =0;//默认温度补偿为0
	
	FLASH_Save_User_Data.highTempAlarm=1;//默认开启超高温报警
	FLASH_Save_User_Data.highMaxTemp=125;//默认125℃为超高温度最高值
	

	FLASH_Save_User_Data.lowTempAlarm=1;//默认开启超低温报警为
	FLASH_Save_User_Data.lowMinTemp=-55;//默认-40℃超低温度最低值
	

	FLASH_Save_User_Data.tempAlarmBuf=5;//默认温度缓冲区
	
	//初始化FLASH的状态标识位
	FLASH_Save_User_Data.Init_Flag = Init_Flag_deinit;
}
/*
功能:向FLASH写保存用户参数
参数:无
返回:无
*/
void Write_configuration(void)
{
	uint16_t dataLenght, q;
	uint16_t qq;

	/* 解锁 FLASH 控制块*/
	FLASH_Unlock();
	dataLenght = sizeof(FLASH_Save_User_Data) / 2; //计算该结构体有多少个字节
	
	/* 擦除起始地址为 0x8002000 的 FLASH 页 */
	FLASH_ErasePage(CONF_ADDR);


	for (q = 0; q < dataLenght; q++)
	{
		qq = *(((uint16_t *)&FLASH_Save_User_Data) + q); // ?????????
		FLASH_ProgramHalfWord(CONF_ADDR + q * 2, qq);	 // дFLASH

	}

	/* 锁定 FLASH 控制块*/
	FLASH_Lock();
}

/*
功能:从FLASH中读出用户参数
参数:无
返回:无
*/
void Read_configuration(void)
{
	uint16_t dataLenght, q;
	__IO uint16_t FLASH_DATA;
	
	//得到结构体大小
	dataLenght = sizeof(FLASH_Save_User_Data) / 2;

	//将FLSAH的数据读出来存到结构体里面
	for (q = 0; q < dataLenght; q++)
	{
		*(_FLASH_Save_User_Data *)((&FLASH_Save_User_Data)) = *(_FLASH_Save_User_Data *)(CONF_ADDR);
	}

	if (FLASH_Save_User_Data.Init_Flag != Init_Flag_deinit) //FLASH用户参数初始化默认标志位不正确  Init_Flag_deinit
	{
		user_parameter_Deinit(); //用户参数回复出厂设置
		Write_configuration();	 //向FLASH写保存用户参数
	}

}

/*
功能:回复出厂设置
参数:无
返回:无
*/
void Reset_function(void)
{
	user_parameter_Deinit(); 
	Write_configuration();	 
}

/*
功能:监测保存数据标志位，保存数据
参数:无
返回:无
*/
void FLASH_SAVE_Monitor(void)
{
	if (FLASH_SAVE_Sign == 1)
	{
		FLASH_SAVE_Sign = 0;
		Write_configuration(); // 向FLash指定位置写入数据
//		delay_ms(500);
//		Read_configuration();
//		printf("FLASH_Save_User_Data.switchOnTemp:%d\r\n",FLASH_Save_User_Data.switchOnTemp);
//		printf("FLASH_Save_User_Data.switchOnTemp:%d\r\n",FLASH_Save_User_Data.switchOffTemp);
//		printf("FLASH_Save_User_Data.Init_Flag:%d\r\n",FLASH_Save_User_Data.Init_Flag);
	}
}

/*
功能:对FLASH进行写入操作
参数:无
返回:无
*/
void Write_Flash(void)
{
	uint16_t dataLenght, q;
	uint16_t qq;
	
	/* 解锁 FLASH 控制块*/
	FLASH_Unlock();
	
	dataLenght = sizeof(FLASH_Save_User_Data) / 2; //计算该结构体有多少个字节
	
	/* 擦除起始地址为 0x8002000 的 FLASH 页 */
	FLASH_ErasePage(CONF_ADDR);

	//对FLASH闪存进行写操作
	for (q = 0; q < dataLenght; q++)
	{
		qq = *(((uint16_t *)&FLASH_Save_User_Data) + q); // ?????????
		FLASH_ProgramHalfWord(CONF_ADDR + q * 2, qq);	 // дFLASH
	}
	
	/* 锁定 FLASH 控制块*/
	FLASH_Lock();
}
