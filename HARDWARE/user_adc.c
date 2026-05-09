#include "user_adc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "delay.h"

volatile unsigned int ADC_Buf[NUMSAMP + 1]; // 放单片机ADC采集原始值
volatile unsigned int ADC_BufCnt = 0;		// 采集量变量
volatile unsigned char NTC_tempFlag = 0;	// 负温标志

const unsigned int FTtable[55][2] = { // 温度与电阻阻值对应关系表格
	201, 3380,						  //-1度对应阻值33.80k
	202, 3552,						  //-2度对应阻值35.52k
	203, 3735,						  //-3度对应阻值37.35k
	204, 3927,
	205, 4131,
	206, 4345,
	207, 4571,
	208, 4810,
	209, 5062,
	210, 5328,
	211, 5609,
	212, 5902,
	213, 6218,
	214, 6549,
	215, 6898,
	216, 7267,
	217, 7657,
	218, 8070,
	219, 8506,
	220, 8968,
	221, 9457,
	222, 9976,
	223, 10525,
	224, 11108,
	225, 11728,
	226, 12387,
	227, 13088,
	228, 13835,
	229, 14632,
	230, 15483,
	231, 16393,
	232, 17368,
	233, 18414,
	234, 19537,
	235, 20745,
	236, 22047,
	237, 23450,
	238, 24965,
	239, 26602,
	240, 28373,
	241, 30289,
	242, 32362,
	243, 34605,
	244, 37028,
	245, 39643,
	246, 42455,
	247, 45470,
	248, 48686,
	249, 52091,
	250, 55664,
	251, 59369,
	252, 63147,
	253, 66917,
	254, 70566,
	255, 73950

};

const unsigned int Ttable[126][2] = { // 温度与电阻阻值对应关系表格
	0, 3212,						  // 0度对应阻值32.12k
	1, 3060,						  // 1度对应阻值30.60k
	2, 2913,						  // 2度对应阻值29.13k
	3, 2773,
	4, 2641,
	5, 2515,
	6, 2396,
	7, 2283,
	8, 2176,
	9, 2075,
	10, 1978,
	11, 1887,
	12, 1800,
	13, 1718,
	14, 1640,
	15, 1565,
	16, 1495,
	17, 1428,
	18, 1364,
	19, 1304,
	20, 1246,
	21, 1192,
	22, 1140,
	23, 1090,
	24, 1043,
	25, 1000, // 25度对应阻值10k
	26, 956,  // 26度对应阻值9.56k
	27, 915,
	28, 877,
	29, 840,
	30, 805,
	31, 771,
	32, 739,
	33, 709,
	34, 680,
	35, 652,
	36, 626,
	37, 601,
	38, 577,
	39, 554,
	40, 532,
	41, 511,
	42, 491,
	43, 472,
	44, 453,
	45, 436,
	46, 419,
	47, 403,
	48, 388,
	49, 373,
	50, 359,
	51, 345,
	52, 332,
	53, 320,
	54, 308,
	55, 297,
	56, 286,
	57, 275,
	58, 265,
	59, 256,
	60, 247,
	61, 238,
	62, 229,
	63, 221,
	64, 213,
	65, 206,
	66, 199,
	67, 192,
	68, 185,
	69, 179,
	70, 173,
	71, 167,
	72, 161,
	73, 156,
	74, 150,
	75, 145,
	76, 140,
	77, 136,
	78, 131,
	79, 127,
	80, 123,
	81, 119,
	82, 115,
	83, 111,
	84, 108,
	85, 104,
	86, 101, // 86度对应阻值1.01k
	87, 98,	 // 87度对应阻值0.98k
	88, 95,
	89, 92,
	90, 89,
	91, 86,
	92, 84,
	93, 81,
	94, 79,
	95, 76,
	96, 74,
	97, 72,
	98, 70,
	99, 68,
	100, 66,
	101, 64,
	102, 62,
	103, 60,
	104, 58,
	105, 57, // 105度对应阻值0.57k
	106, 55,
	107, 54,
	108, 52,
	109, 51,
	110, 49,
	111, 48,
	112, 46,
	113, 45,
	114, 44,
	115, 43,
	116, 42,
	117, 40,
	118, 39,
	119, 38,
	120, 37,
	121, 36,
	122, 35,
	123, 34,
	124, 33,
	125, 32

};

/* ADC 初始化 */
void ADC1_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							/* 单次转换 */
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* 软件触发 */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC 校准 */
	ADC_GetCalibrationFactor(ADC1);

	/* 使能 ADC */
	ADC_Cmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY))
		;
}

/* 配置 ADC 通道对应的 GPIO 为模拟输入 */
void ADC_GPIO_Config(uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 根据引脚选择 GPIO 端口时钟 */
	if (GPIO_Pin >= GPIO_Pin_0 && GPIO_Pin <= GPIO_Pin_7)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	else if (GPIO_Pin >= GPIO_Pin_8 && GPIO_Pin <= GPIO_Pin_15)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init((GPIO_Pin >= GPIO_Pin_8) ? GPIOB : GPIOA, &GPIO_InitStructure);
}

/* 配置 ADC 转换通道并读取转换结果 */
uint16_t ADC_ReadChannel(uint32_t ADC_Channel)
{
	/* 配置采样通道和采样时间 */
	ADC_ChannelConfig(ADC1, ADC_Channel, ADC_SampleTime_55_5Cycles);

	/* 软件触发启动转换 */
	ADC_StartOfConversion(ADC1);

	/* 等待转换结束 */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;

	/* 返回转换结果 */
	return ADC_GetConversionValue(ADC1);
}

/* 获取电压值（mV），参考电压 3.3V */
uint32_t ADC_GetVoltage_mV(uint16_t adc_value)
{
	/* ADC 12位分辨率: Voltage = (adc_value / 4096) * 3300 mV */
	return (uint32_t)((uint32_t)adc_value * 3300 / 4096);
}

/*******************************************************************************
 * 将采集的原始值计算为温度值
 * 入口参数：无
 * 返回值  ：温度值（有符号范围 -54 ~ 125）
 ******************************************************************************/
unsigned char HandleADC(void)
{
	unsigned char i;
	unsigned int j;
	unsigned int Temp_signal;
	unsigned char g_temperature = 0; // 定义温度变量
	float R;
	float voltage;
	unsigned long val = 0;
	unsigned int NTC_R; // 定义热敏电阻阻值变量

	ADC_ReadChannel(GPIO_Pin_0); // 第一次采集，丢弃使通道稳定

	// 连续采集10次取平均值
	val = 0;
	for (j = 0; j < 10; j++)
	{
		val += ADC_ReadChannel(GPIO_Pin_0);
	}
	Temp_signal = val / 10;

	// NTC端电压（单位V）
	voltage = (3.3 * Temp_signal) / 4096;
	// 硬件电路串联电阻10K，计算NTC电阻
	R = 1000 * voltage / (3.3 - voltage);
	// 把float浮点数转化为int型
	NTC_R = R;

	if (NTC_R < 3211) // 非负温
	{
		NTC_tempFlag = 0; // 非负温标志
		for (i = 125; i < 126; i--)
		{
			if (NTC_R >= Ttable[i][1] && NTC_R < Ttable[i - 1][1])
			{
				g_temperature = (Ttable[i][0] + 1);
			}
		}
	}
	else if (NTC_R > 3379) // 负温
	{
		NTC_tempFlag = 1; // 负温标志
		for (i = 54; i < 55; i--)
		{
			if (NTC_R >= FTtable[i - 1][1] && NTC_R < FTtable[i][1])
			{
				g_temperature = (FTtable[i - 1][0] - 200);
			}
		}
	}
	else // 0度
	{
		NTC_tempFlag = 0; // 非负温标志
		g_temperature = 0;
	}
	return g_temperature;
}
