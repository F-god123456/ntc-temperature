#ifndef __MAX31865_H
#define __MAX31865_H
 
/*
SDO   --->   MISO   ->     PB14
CS    --->   CS     ->     PB12
SCLK  --->   SCK    ->     PB13
SDI   --->   MOSI   ->     PB15
DRDY  --->   PA5
*/


#define PTX_SW    1    // 1:pt100   0：pt1000

/* MAX31865参考电阻 */
//#if     PTX_SW    	//为pt100
//#define RREF            (430)  //400Ω
//#define RTD_R           100U          
//#else          		//为pt1000
//#define RREF            (4300)  //400Ω
//#define RTD_R           1000U 
//#endif
#define RREF            (430)  //400Ω

/* MAX31865控制口 */
#define MAX31865_SDO_Pin              GPIO_Pin_14
#define MAX31865_SDO_Port			  GPIOB

#define MAX31865_CS_Pin               GPIO_Pin_12
#define MAX31865_CS_Port              GPIOB

#define MAX31865_SCLK_Pin             GPIO_Pin_13
#define MAX31865_SCLK_Port            GPIOB

#define MAX31865_SDI_Pin              GPIO_Pin_15
#define MAX31865_SDI_Port             GPIOB


#define MAX31865_CS_SET           GPIO_WriteBit(MAX31865_CS_Port,MAX31865_CS_Pin,Bit_SET)
#define MAX31865_CS_CLR      	  GPIO_WriteBit(MAX31865_CS_Port,MAX31865_CS_Pin,Bit_RESET)
#define MAX31865_SCLK_SET    	  GPIO_WriteBit(MAX31865_SCLK_Port,MAX31865_SCLK_Pin,Bit_SET)
#define MAX31865_SCLK_CLR    	  GPIO_WriteBit(MAX31865_SCLK_Port,MAX31865_SCLK_Pin,Bit_RESET)
#define MAX31865_SDI_SET    	  GPIO_WriteBit(MAX31865_SDI_Port,MAX31865_SDI_Pin,Bit_SET)
#define MAX31865_SDI_CLR     	  GPIO_WriteBit(MAX31865_SDI_Port,MAX31865_SDI_Pin,Bit_RESET)
#define MAX31865_SDO_READ    	  GPIO_ReadInputDataBit(MAX31865_SDO_Port,MAX31865_SDO_Pin)

/*温度范围*/
#define T_MAX    125
#define T_MIN   -55


extern float T;//当前温度
extern float T_temp[5];//5秒内记录的温度，用于计算平均值
extern volatile uint8_t time_over_flag;//1s到达标志位
extern uint8_t time_count;//1s到达次数
 
extern void MAX31865_Init(void); //MAX31865 初始化,软件模拟
extern void MAX31865_Cfg(void);  //MAX31865 配置
extern float MAX31865_GetTemp(void);//MAX31865 获取温度
extern unsigned char MAX31865_Read(unsigned char addr);//读取当前温度
//uint8_t PT100_Read_Temp(float* T);//成功读取或失败未读取
extern uint8_t PT100_Read_Temp(float *temp_buf);
#endif


