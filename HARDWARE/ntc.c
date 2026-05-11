#include "ntc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_misc.h"
#include "delay.h"


/*实际显示温度数据*/
float T;

//临时数据，用于计算平均值
float T_temp[5];
volatile unsigned int  ADC_Buf[NUMSAMP+1];      //放单片机ADC采集原始值
volatile unsigned int  ADC_BufCnt=0;            //采集量变量

/*标志位*/
volatile uint8_t time_over_flag=0;//1s到达标志位
uint8_t time_count=0;//1s到达次数

/**************************************************************************************
 * 描  述 : 初始化NTC_ADC + NTC_GPIO_PORT, NTC_ADC_CHANNEL，配置为连续转换+中断模式
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 使能ADC时钟和GPIO时钟 */
	RCC_APB2PeriphClockCmd(NTC_ADC_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(NTC_GPIO_CLK, ENABLE);

	/* ADC分频：PCLK/4 = 48MHz/4 = 12MHz */
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	/* 配置NTC_GPIO_PIN为模拟输入 */
	GPIO_InitStructure.GPIO_Pin = NTC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL
	GPIO_Init(NTC_GPIO_PORT, &GPIO_InitStructure);

	/* 复位ADC寄存器 */
	ADC_DeInit(NTC_ADC);

	/* ADC配置 */
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							/* 连续转换 */
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* 软件触发 */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(NTC_ADC, &ADC_InitStructure);

	/* 配置NTC_GPIO_PIN为ADC采样通道，采样周期239.5 */
	ADC_ChannelConfig(NTC_ADC, NTC_ADC_CHANNEL, ADC_SampleTime_239_5Cycles);

	/* 使能EOC中断 */
	ADC_ITConfig(NTC_ADC, ADC_IT_EOC, ENABLE);

	/* ADC校准 */
	ADC_GetCalibrationFactor(NTC_ADC);

	/* 使能ADC */
	ADC_Cmd(NTC_ADC, ENABLE);
	while (!ADC_GetFlagStatus(NTC_ADC, ADC_FLAG_ADRDY));

	/* NVIC配置 */
	NVIC_InitStructure.NVIC_IRQChannel = NTC_ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 软件触发，启动连续转换 */
	ADC_StartOfConversion(NTC_ADC);
}

/**************************************************************************************
 * 描  述 : NTC初始化（调用NTC_ADC初始化，配置NTC_GPIO_PIN为ADC采样通道）
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void NTC_Init(void)
{
    ADC1_Init();
}

/**************************************************************************************
 * 描  述 : NTC_ADC中断服务函数 —— 累加ADC值，满ADC_SAMPLE_COUNT次后计算平均和电压
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void ADC1_IRQHandler(void)
{
	unsigned int adcPtr;
	if (ADC_GetITStatus(NTC_ADC, ADC_IT_EOC) != RESET)
	{
		adcPtr= ADC_GetConversionValue(NTC_ADC);	// 累加ADC值
		ADC_Buf[ADC_BufCnt++]=adcPtr; 

		if (ADC_BufCnt >= NUMSAMP)	// 达到指定采样次数
		{
			 ADC_BufCnt=0;	
		}

		ADC_ClearITPendingBit(NTC_ADC, ADC_IT_EOC);	// 清除EOC中断标志
	}
}


/**************************************************************************************
 * 描  述 : 采集处理函数（多次采样滤波 + 多项式拟合）
 * 入  参 : temp - 输出温度值(C)指针
 * 返回值 : 0 - 成功  -1 - ADC值超出有效范围
 **************************************************************************************/
int HandleADCTemp(float *temp)
{
	unsigned long val = 0;
	unsigned int Temp_signal;
	unsigned int i;

	if (temp == NULL)
	{
		return -1;
	}

    //软件过滤采集的原始值的前NUMHEAD和最后NUMHEAD个数据，将剩余数据累加
	for (i = NUMHEAD; i < (NUMSAMP - NUMHEAD); i++)
	{
		val = val + ADC_Buf[i];
	}

    //对采集的累加值求平均
	Temp_signal = val / (NUMSAMP - 2 * NUMHEAD);

	// ADC值有效范围判断：128 < 值 < 3955
	if (Temp_signal <= 128 || Temp_signal >= 3955)//128-- 120摄氏度   3955-- -40摄氏度
	{
		return -1;
	}

	//下面是曲线拟合得到的温度与采集原始值之间的关系，仅供参考
	*temp = -(7.12E-016)*pow(Temp_signal,5) + (7.644E-012)*pow(Temp_signal,4)
	       - (3.401E-08)*pow(Temp_signal,3) + (7.898E-05)*pow(Temp_signal,2)
	       - (1.182E-01)*Temp_signal + (1.19E+02);

	return 0;
}
 

/*************************************END OF FILE******************************/
