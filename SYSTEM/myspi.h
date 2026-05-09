#ifndef __MYSPI_H
#define __MYSPI_H
 
#ifdef __cplusplus
 extern "C" {
#endif
/*

SDO   --->   PB4  主机输入
CS    --->   PA15  NSS
SCLK  --->   PB3
SDI   --->   PB5  主机输出
DRDY  --->   PA5
	 
*/
   
#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 *FLAG_TIMEOUT))
 
#define MAX31865_SPI_PORT 				GPIOA
#define MAX31865_IO_PORT  				GPIOB
				
// define SPI pins				
#define SPI_SCK           				GPIO_Pin_3    /* GPIOB3*/
#define SPI_MISO          				GPIO_Pin_4    /*GPIOB4*/
#define SPI_MOSI          				GPIO_Pin_5    /*GPIOB5*/
#define SPI_CS            				GPIO_Pin_15    /*GPIOA15*/
#define DRDY_PIN          				GPIO_Pin_5    /*GPIOA5*/
 
#define SPI_GPIO_CLK             	RCC_AHBPeriph_GPIOA
#define SPI_CS_GPIO_CLK          	RCC_AHBPeriph_GPIOB
#define DRDY_GPIO_CLK            	RCC_AHBPeriph_GPIOB
 
 
#define SPI_CS_HIGH              	GPIO_SetBits(MAX31865_IO_PORT, SPI_CS)
#define SPI_CS_LOW               	GPIO_ResetBits(MAX31865_IO_PORT, SPI_CS)
 
void  SPI_Inital(void);//MAX31865 硬件spi初始化
uint8_t SPI_Write(uint8_t *buffer, uint8_t nBytes);//SPI写数据
uint8_t SPI_Read(uint8_t *buffer, uint8_t nBytes); //SPI读数据 
uint8_t SPI_WriteByte(uint8_t data);//SPI字节写数据
uint8_t SPI_ReadByte(void);         //SPI字节读数据
void Set_DRDY_Pin_INPUT(void);//MAX31865 DRDY输入IO配置
uint8_t DRDY_Pin_Value(void);//读MAX31865  DRDY_Pin_Value高低电平
 
#ifdef __cplusplus
}
#endif
 
#endif
 