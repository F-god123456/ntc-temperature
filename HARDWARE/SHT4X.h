#ifndef __SHT4X_H__
#define __SHT4X_H__



//IO方向设置产考F4手册
/**********************************begin**************************************/
//#define SDA_In()  {GPIOB->MODER&=0xFFCFFFFF;}  
//#define SDA_Out() {GPIOB->MODER&=0xFFCFFFFF;GPIOB->MODER|=1<<20;}
/**********************************end************************************/

#define IIC_SCL_PIN									GPIO_Pin_11
#define IIC_SDA_PIN									GPIO_Pin_10
#define IIC_GPIO_PORT								GPIOB

#define Delay_ms(x)    								delay_ms(x)
#define Delay_us(x)    								delay_us(x)

//IO操作函数	 
#define IIC_Scl(x)   								GPIO_WriteBit(IIC_GPIO_PORT, IIC_SCL_PIN, (BitAction)x)
#define IIC_Sda(x)  								GPIO_WriteBit(IIC_GPIO_PORT, IIC_SDA_PIN, (BitAction)x)
#define READ_Sda   	  								GPIO_ReadInputDataBit(IIC_GPIO_PORT, IIC_SDA_PIN)


extern float T;//当前温度
extern float H;//当前湿度

extern float T_temp[5];//5秒内记录的温度，用于计算平均值
extern float H_temp[5];//5秒内记录的湿度，用于计算平均值

extern uint32_t SHT45_Data1[2];//存储从传感器读取的温度和湿度原始数据。
extern volatile uint8_t time_over_flag;//1s到达标志位
extern uint8_t time_count;//1s到达次数

//IIC所有操作函数
void IIC_Init_S(uint32_t RCC_CLK);   												//初始化IIC的IO口				 
void IIC_Start(void);																//发送IIC开始信号
void IIC_Stop(void);	  															//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);													//IIC发送一个字节
void IIC_Ack(void);																	//IIC发送ACK信号
void IIC_NAck(void);																//IIC不发送ACK信号										  
uint8_t  IIC_Wait_Ack(void); 														//IIC等待ACK信号
uint8_t  IIC_Read_B(uint8_t ack);													//IIC读取一个字节
uint32_t  IIC_Read_Byte(uint8_t ack,uint8_t* recvdata,uint8_t num);					//IIC读取n个字节


/*初始化*/
void SHT45_Init(void);//模块初始化函数

/*读取温湿度*/
uint8_t SHT45_ReadPdata(uint8_t ack,uint8_t *arr,float* T,float* H );//读取温湿度函数






#endif


