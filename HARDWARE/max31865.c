#include "main.h"



/*实际显示温度数据*/
float T;

//临时数据，用于计算平均值
float T_temp[5];


/*标志位*/
volatile uint8_t time_over_flag=0;//1s到达标志位
uint8_t time_count=0;//1s到达次数


/***********************************************
调用方式：MAX31865_Init()
SDO   --->   MISO   ->     PB4
CS    --->   CS     ->     PA15
SCLK  --->   SCK    ->     PB3
SDI   --->   MOSI   ->     PB5
DRDY  --->   PA5
函数说明：MAX31865 初始化,软件模拟
************************************************/

void MAX31865_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ;
 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA ,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB ,ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = MAX31865_CS_Pin;  //PA15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //输出  
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;  //推挽
	GPIO_Init(MAX31865_CS_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MAX31865_SDI_Pin;  //PB3 | PB5
	GPIO_Init(MAX31865_SDI_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MAX31865_SCLK_Pin;  //PB3 | PB5
	GPIO_Init(MAX31865_SCLK_Port,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MAX31865_SDO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(MAX31865_SDO_Port,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = MAX31865_DRDY_Pin;
//	GPIO_Init(MAX31865_DRDY_Port,&GPIO_InitStructure);
	
	MAX31865_CS_SET;
	MAX31865_SCLK_SET;
}
 
/***********************************************
调用方式：MAX31865_Write()
返回值：
函数说明：MAX31865 写寄存器,addr:寄存器地址,data:数据
************************************************/
void MAX31865_Write(unsigned char addr, unsigned char data)
{
	unsigned char i;

	MAX31865_CS_CLR;
	for(i=0;i<8;i++)  //写地址
	{
		MAX31865_SCLK_CLR;
		if(addr&0x80) 
			MAX31865_SDI_SET;
		else 
			MAX31865_SDI_CLR;
		MAX31865_SCLK_SET;
		addr<<=1;
	}
	for(i=0;i<8;i++)  //写数据
	{
		MAX31865_SCLK_CLR;
		if(data&0x80) 
			MAX31865_SDI_SET;
		else 
			MAX31865_SDI_CLR;
		MAX31865_SCLK_SET;
		data<<=1;
	}
	MAX31865_CS_SET;
}
/***********************************************
调用方式：MAX31865_Read()
返回值：  data
函数说明：MAX31865 读寄存器 ,addr:寄存器地址
************************************************/
unsigned char MAX31865_Read(unsigned char addr)
{
	unsigned char i;
	unsigned char data=0;
	
	MAX31865_CS_CLR;
	for(i=0;i<8;i++)  //写地址
	{
		MAX31865_SCLK_CLR;
		if(addr&0x80) 
			MAX31865_SDI_SET;
		else 
			MAX31865_SDI_CLR;

		MAX31865_SCLK_SET;
		addr<<=1;
	}
	for(i=0;i<8;i++)  //读数据
	{
		MAX31865_SCLK_CLR;
		data<<=1;		
		MAX31865_SCLK_SET;		
		if(MAX31865_SDO_READ) 
			data|=0x01;
		else 
			data|=0x00;
	}
	MAX31865_CS_SET;
	return data;
}

/***********************************************
调用方式：MAX31865_Cfg()
返回值：  
函数说明：MAX31865 配置
************************************************/
void MAX31865_Cfg(void)
{
	//BIAS ON,自动，3线，50HZ  根据文件修改四线还是三线
	MAX31865_Write(0x80, 0xD3); 
	
}

/***********************************************
调用方式：MAX31865_GetTemp()
返回值：  
函数说明：MAX31865 获取温度
************************************************/
float MAX31865_GetTemp(void)
{ 
	unsigned int data;
	float Rt;
	float Rt0 = 100;  //PT100-> 100   PT1000 -> 1000  修改处	
	float Z1,Z2,Z3,Z4,temp;
	float a = 3.9083e-3;
	float b = -5.775e-7;
	float rpoly;
		//MAX31865_Write(0x80, 0xD3);
	data=MAX31865_Read(0x01)<<8;
	data|=MAX31865_Read(0x02);
	data>>=1;  //去掉Fault位
	Rt=(float)data/32768.0*RREF;
	Z1 = -a;
	Z2 = a*a-4*b;
	Z3 = 4*b/Rt0;
	Z4 = 2*b;
	temp = Z2+Z3*Rt;
	temp = (sqrt(temp)+Z1)/Z4;
	if(temp>=0)
	{
		return temp;
	}
	rpoly = Rt;
	temp = -242.02;
	temp += 2.2228 * rpoly;
	rpoly *= Rt;  // square
	temp += 2.5859e-3 * rpoly;
	rpoly *= Rt;  // ^3
	temp -= 4.8260e-6 * rpoly;
	rpoly *= Rt;  // ^4
	temp -= 2.8183e-8 * rpoly;
	rpoly *= Rt;  // ^5
	temp += 1.5243e-10 * rpoly;

	return temp;
}

/*
功能：成功读取或读取失败
参数：T，温度缓冲区
返回值：当前测量温度
*/
uint8_t PT100_Read_Temp(float *temp_buf)
{
	float temp;
	temp = MAX31865_GetTemp();
	return temp;
	
//	if(temp>ERROR_MAX || temp<ERROR_MIN)
//	{
//		//*T   = temp;
//		return 1;//读取失败
//	}
//	else
//		return 0;//读取成功
	
//	if(MAX31865_Read(0x00) == 0xD1 )
//	{
//		*T = MAX31865_GetTemp();
//		return 0;//成功读取
//	}
//	else
//	{
//		MAX31865_Cfg();
//		return 1;//读取失败
//	
//	}
}

