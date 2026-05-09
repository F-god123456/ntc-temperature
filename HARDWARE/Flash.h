#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

//单片机FLASH地址
#define CONF_ADDR                      0x0800FC00

//FLASH用户参数初始化默认标志位
#define Init_Flag_deinit       0x02		//FLASH的状态标识位
#define User_Num               10			//遥控器个数
#define SWITCH_Num             16			//开关个数
#define SWITCH_Name_Num        30			//开关名字字符长度
#define SWITCH_Status_Num      1  			//开关值字符长度



//保存数据标志位
extern uint8_t FLASH_SAVE_Sign;

//#pragma pack(2)
//用户参数的具体数据
typedef struct
{
	uint8_t switchMode;  //开关模式
	
	double 	switchOnTemp; //开启温度
	double  switchOffTemp;//关闭温度	
	
	double  tempOffset;	  //温度偏移量
	
	bool	highTempAlarm;//超高温度开关
	double  highMaxTemp	; //超高温度值

	bool    lowTempAlarm; //超低温度开关
	double  lowMinTemp;	  //超低温度值
	
	double  tempAlarmBuf;  //温度缓冲区
	
	//如果结构体内存是单数打开k变量注释，如果是双数注释k变量
	//char k;
	//FLASH的状态标识位
	char Init_Flag;       
	
}_FLASH_Save_User_Data;

//向FLASH保存用户参数标志位
extern uint8_t FLASH_Save_User_Flag;           

//外部声明用户参数的具体数据结构体
extern _FLASH_Save_User_Data FLASH_Save_User_Data;

//FLASH初始化
void FLASH_Init(void);

//*用户参数回复出厂设置
void user_parameter_Deinit(void);

//向FLASH写保存用户参数
void Write_configuration(void);

//从FLASH中读出用户参数
void Read_configuration(void);

//回复出厂设置
void Reset_function(void);

//监测保存数据标志位，保存数据
void FLASH_SAVE_Monitor(void);

//对FLASH进行写操作
void Write_Flash(void);




#endif


