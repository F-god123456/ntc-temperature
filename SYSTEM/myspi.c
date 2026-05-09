#include "main.h"



__IO uint32_t TIMEOUT=LONG_TIMEOUT;
/***********************************************
SDO   --->   PB4  主机输入
CS    --->   PA15  NSS
SCLK  --->   PB3
SDI   --->   PB5  主机输出
DRDY  --->   PA5
函数说明：MAX31865 硬件spi初始化
************************************************/

void  SPI_Inital(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 
	// GPIO
	RCC_AHBPeriphClockCmd(SPI_GPIO_CLK, ENABLE);//使能GPIOB
	RCC_AHBPeriphClockCmd(SPI_CS_GPIO_CLK, ENABLE);//使能GPIOA

	/*!< SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI

	/*!< GPIO configuration */  
	/*!< Configure SPI pins */
	GPIO_InitStructure.GPIO_Pin = SPI_SCK|SPI_MOSI|SPI_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(MAX31865_SPI_PORT, &GPIO_InitStructure);

	/*!< Configure SPI pin: CS */
	GPIO_InitStructure.GPIO_Pin = SPI_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(MAX31865_IO_PORT, &GPIO_InitStructure);  
	//set CS high
	GPIO_SetBits(MAX31865_IO_PORT, SPI_CS); 
	/*!< SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	/* SPI Peripheral Enable */
	SPI_Cmd(SPI1, ENABLE);
	/* Apply SPI configuration after enabling it */
	SPI_Init(SPI1, &SPI_InitStructure);
}
/***********************************************
调用方式： SPI_Write()
返回值：  
函数说明：SPI写数据
************************************************/
uint8_t SPI_Write(uint8_t *buffer, uint8_t nBytes)
{
	uint8_t i;
	for(i=0;i<nBytes;i++)
	{
		TIMEOUT = LONG_TIMEOUT;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{  
			if((TIMEOUT--) == 0) //not empty
				return (1);
		}
		SPI_SendData8(SPI1,buffer[i]);
		TIMEOUT = LONG_TIMEOUT;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{     
			if((TIMEOUT--) == 0) return (1);
		}
		SPI_ReceiveData8(SPI1);  
	}
	return(0);
}
/***********************************************
调用方式： SPI_Read()
返回值：  
函数说明：SPI读数据
************************************************/
uint8_t SPI_Read(uint8_t *buffer, uint8_t nBytes)
{
	uint8_t i;
	SPI_ReceiveData8(SPI1);
	for(i=0;i<nBytes;i++)
	{
		TIMEOUT = LONG_TIMEOUT;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{  
			if((TIMEOUT--) == 0) //not empty
				return (1);
		}
		SPI_SendData8(SPI1,0x00);
		TIMEOUT = LONG_TIMEOUT;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
			if((TIMEOUT--) == 0) return (1);
		}
		buffer[i]= SPI_ReceiveData8(SPI1);
	}
	return(0);
}
/***********************************************
调用方式： SPI_WriteByte()
返回值：  
函数说明：SPI字节写数据
************************************************/
uint8_t SPI_WriteByte(uint8_t data)
{
	TIMEOUT = LONG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{  
		if((TIMEOUT--) == 0) return (1);//not empty
	}
	SPI_SendData8(SPI1,data);
	TIMEOUT = LONG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((TIMEOUT--) == 0) return (1);
	}
	SPI_ReceiveData8(SPI1);
	SPI_ReceiveData8(SPI1);
	return(0);
}
/***********************************************
调用方式： SPI_WriteByte()
返回值：  
函数说明：SPI字节读数据
************************************************/
uint8_t SPI_ReadByte(void)
{
	SPI_ReceiveData8(SPI1);//SPI_I2S_GetFlagStatus
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  //not empty	
		SPI_SendData8(SPI1,0xff);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); 
		return (SPI_ReceiveData8(SPI1));
}
/***********************************************
调用方式： Set_DRDY_Pin_INPUT()
返回值：  
函数说明：MAX31865 DRDY输入IO配置
************************************************/
void Set_DRDY_Pin_INPUT(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
   
   RCC_APB2PeriphClockCmd(DRDY_GPIO_CLK, ENABLE); 
   GPIO_InitStructure.GPIO_Pin = DRDY_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入浮空
   GPIO_Init(MAX31865_IO_PORT, &GPIO_InitStructure);      
}
/***********************************************
调用方式： DRDY_Pin_Value()
返回值：  
函数说明： 读MAX31865  DRDY_Pin_Value高低电平
************************************************/
uint8_t DRDY_Pin_Value(void)
{
   return(GPIO_ReadOutputDataBit(MAX31865_IO_PORT, DRDY_PIN));      
}
