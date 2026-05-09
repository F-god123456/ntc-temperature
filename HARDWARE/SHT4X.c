#include "main.h" 


#define write 0
#define read  1

#define ACK_TIMEOUT 200 // 定义超时阈值，单位为微秒

/*实际显示温度、湿度数据*/
float T;
float H;

//临时数据，用于计算平均值
float T_temp[5];
float H_temp[5];


/*标志位*/
volatile uint8_t time_over_flag=0;//1s到达标志位
uint8_t time_count=0;//1s到达次数



uint32_t SHT45_Data1[2]={0};//存储从传感器读取的温度和湿度原始数据。


static GPIO_InitTypeDef GPIO_InitStructure;

/*
***函数名：IIC_Init_S
***功能：初始化I2C接口的GPIO引脚，包括开启相应的时钟、设置引脚模式、输出类型、速度和上下拉电阻。
***参数：
	RCC_CLK：表示需要使能的GPIO端口的时钟。这通常是一个宏定义，例如RCC_AHBPeriph_GPIOB，用于指定GPIOB端口的时钟。
*/
void IIC_Init_S(uint32_t RCC_CLK)
{					     

	RCC_AHBPeriphClockCmd(RCC_CLK, ENABLE );						
	   
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN|IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(IIC_GPIO_PORT,IIC_SCL_PIN|IIC_SDA_PIN);
}

/*
***函数名：SDA_Out
***功能：配置指定的GPIO引脚作为输出，通常用于I2C通信中的数据线（SDA）。
***参数：无参数。
*/
void SDA_Out(void)
{
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
}
/*
***函数名：SDA_In
***功能：配置指定的GPIO引脚作为输入，通常用于I2C通信中的数据线（SDA）。
***参数：无
*/
void SDA_In(void)
{
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStructure);
}

/*----------------------------------*/
/*									*/
/*	     产生IIC起始信号			*/
/*  								*/
/*----------------------------------*/
void IIC_Start(void)
{
	SDA_Out();
	IIC_Sda(1);	  	  
	IIC_Scl(1);
	Delay_us(4);
 	IIC_Sda(0);
	Delay_us(4);
	IIC_Scl(0);
}

/*----------------------------------*/
/*									*/
/*	    	产生IIC停止信号 		*/
/*									*/
/*----------------------------------*/
void IIC_Stop(void)
{
	SDA_Out();
	IIC_Scl(0);
	IIC_Sda(0);
 	Delay_us(4);
	IIC_Scl(1); 
	IIC_Sda(1);
	Delay_us(2);						   	
}

/*----------------------------------*/
/*									*/
/*	       等待应答					*/
/*	返回值	1：接收应答失败			*/
/*			0：接收应答成功			*/
/*----------------------------------*/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_In();      													
	IIC_Sda(1);Delay_us(1);	   
	IIC_Scl(1);Delay_us(1);	 
	while(READ_Sda)				
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();														
			return 1;
		}
	}
	IIC_Scl(0);
	return 0;  
} 

/*----------------------------------*/
/*									*/
/*			产生ACK应答				*/
/*									*/
/*----------------------------------*/
void IIC_Ack(void)
{
	IIC_Scl(0);
	SDA_Out();														
	IIC_Sda(0);Delay_us(2);
	IIC_Scl(1);Delay_us(2);
	IIC_Scl(1);
}

/*----------------------------------*/
/*									*/
/*			不产生ACK应答			*/
/*									*/
/*----------------------------------*/	    
void IIC_NAck(void)
{
	IIC_Scl(0);
	SDA_Out();							
    IIC_Sda(1);
	Delay_us(2);	
	IIC_Scl(1);
	Delay_us(2);
	IIC_Scl(1);
}		

/*----------------------------------*/
/*									*/
/*	 IIC发送一个字节			    */
/*	    @txd：	要发送的数据		*/
/*	    返回值：1，有应答			*/
/*		    	0，无应答			*/
/*----------------------------------*/		  
void IIC_Send_Byte(uint8_t txd)
{                        
		uint8_t t;   
		SDA_Out(); 	    												
		IIC_Scl(0);															
		for(t=0;t<8;t++)
		{              
			if((txd&0x80)>>7)									
				IIC_Sda(1);
			else
				IIC_Sda(0);
			txd<<=1; 	  											
			Delay_us(2);   														
			IIC_Scl(1);	
			Delay_us(2); 
			IIC_Scl(0);	
			Delay_us(2);
    }	 
} 	

/*
***函数名：IIC_Read_Byte
***返回类型：uint32_t，表示函数返回一个32位无符号整数，但在这个上下文中，返回值似乎没有被使用，因此函数实际上返回0。
***参数：
	 ack：一个布尔值，用于指示是否在每个字节读取后发送应答信号（ACK）。如果为0，则在读取每个字节后发送非应答信号（NACK），如果为非0，则发送应答信号（ACK）。
	 recvdata：一个指向缓冲区的指针，用于存储读取的数据。
	 num：要读取的字节数。	
*/
uint32_t  IIC_Read_Byte(uint8_t ack,uint8_t* recvdata,uint8_t num)
{
	uint32_t    data=0;
	while(num--)
	{
		*recvdata++ = IIC_Read_B(ack);
	}
	return  data;
}

/*
***函数名：IIC_Read_B
***返回类型：uint8_t，表示函数返回一个8位无符号整数，即读取到的数据字节。
***参数：
	ack：一个布尔值，用于指示是否发送应答信号。如果ack为0，则发送非应答信号（NACK），如果为非0，则发送应答信号（ACK）。
*/
uint8_t  IIC_Read_B(uint8_t ack)
{
	unsigned char i;
	uint8_t receive;
	SDA_In();								
	for(i=0;i<8;i++)
	{
		IIC_Scl(0); 				
		Delay_us(1);//2
		IIC_Scl(1);
		Delay_us(1);//无
		receive<<=1;
		if(READ_Sda)
				receive++;   	
		//Delay_us(1); 
	}
			
	if (!ack)
		IIC_NAck();	
	else 
		IIC_Ack(); 	
	IIC_Scl(0); 						
	return receive;
}
/*
***函数名：SHT45_Init
***功能：初始化IIC接口，配置相应的GPIO引脚用于IIC通信。
***参数：无参数。
*/
void SHT45_Init(void)             //初始化IIC        Pb11：SCL     PB10：SDA
{
	IIC_Init_S(RCC_AHBPeriph_GPIOB);
}


/* 读取压力数据寄存器*/ 
uint8_t SHT45_ReadPdata(uint8_t ack,uint8_t *arr,float* T,float* H )
{
	int32_t CRC1=0XFF;
	int32_t CRC2=0XFF;
	uint32_t timeout;
	
	//开启温度转换
	IIC_Start();														
	IIC_Send_Byte (0x88);	//0x44 <<1 +0
	delay_us(2);
	timeout = ACK_TIMEOUT;
	while(IIC_Wait_Ack ()!=0)
	{
		--timeout;
		if(timeout ==0)
		{
			return 1;
		}
		Delay_ms(1);
	}
	IIC_Send_Byte (0xFD);
	timeout = ACK_TIMEOUT;
	while(IIC_Wait_Ack ()!=0)
	{
		--timeout;
		if(timeout ==0)
		{
			return 2;
		}
		Delay_ms(1);
	} 
	IIC_Stop();
		
	delay_ms(10);
	//读取温度
	IIC_Start();
	IIC_Send_Byte (0x89);	//0x44 <<1 +1
	delay_us(2);
	timeout = ACK_TIMEOUT;
	while(IIC_Wait_Ack()!=0)
	{
		
		--timeout;
		if(timeout ==0)
		{
			return 3;
		}
		Delay_ms(1);
	} 
	IIC_Read_Byte(ack,arr,6);
	IIC_Stop();
	
	SHT45_Data1[0] =arr[0]<<8|arr[1];
	*T = (-45 + 175*(SHT45_Data1[0])/65535.0);
	CRC1= arr[2];
	SHT45_Data1[1] =arr[3]<<8|arr[4];
	*H =(-6 + 125 *( SHT45_Data1[1])/65535.0);
		
	if(*H > 100)
		*H = 100;
	if(*H < 0)
		*H = 0;
	
	CRC2= arr[5];
	
    return 0;          
}
