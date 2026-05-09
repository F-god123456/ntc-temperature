#ifndef __DIGITALTUBE_H
#define __DIGITALTUBE_H


#define Position     4              //数据位个数




//3位数码管脚定义vcc
#define NIXI_PORT          GPIOA
#define NIXI_PIN_A         GPIO_Pin_0
#define NIXI_PIN_B         GPIO_Pin_1
#define NIXI_PIN_C         GPIO_Pin_2
#define NIXI_PIN_D         GPIO_Pin_3
#define NIXI_PIN_E         GPIO_Pin_4
#define NIXI_PIN_F         GPIO_Pin_5
#define NIXI_PIN_G         GPIO_Pin_6
#define NIXI_PIN_DP        GPIO_Pin_7

//GND
#define NIXI_PIN_DIG1      GPIO_Pin_8
#define NIXI_PIN_DIG2      GPIO_Pin_11
#define NIXI_PIN_DIG3      GPIO_Pin_12
#define NIXI_PIN_DIG4      GPIO_Pin_15


/**************************************
		共阴极下    1亮0灭
		共阳极下    1灭0亮
**************************************/
#define SHOW_DATA_ALL       ((uint16_t)0x00FF)//8.
#define SHOW_DATA_0         ((uint16_t)0x003F)//0
#define SHOW_DATA_1         ((uint16_t)0x0006)//1
#define SHOW_DATA_2         ((uint16_t)0x005B)//2
#define SHOW_DATA_3         ((uint16_t)0x004F)//3
#define SHOW_DATA_4         ((uint16_t)0x0066)//4
#define SHOW_DATA_5         ((uint16_t)0x006D)//5
#define SHOW_DATA_6         ((uint16_t)0x007D)//6
#define SHOW_DATA_7         ((uint16_t)0x0007)//7
#define SHOW_DATA_8         ((uint16_t)0x007F)//8
#define SHOW_DATA_9         ((uint16_t)0x0067)//9
#define SHOW_DATA_DP        ((uint16_t)0x0080)//.
#define SHOW_DATA_NONE      ((uint16_t)0x0000)//清空
#define SHOW_DATA_ACROSS    ((uint16_t)0x0040)//-
#define SHOW_DATA_B         ((uint16_t)0x007C)//b
#define SHOW_DATA_E         ((uint16_t)0x0079)//E
#define SHOW_DATA_F         ((uint16_t)0x0071)//F
#define SHOW_DATA_S         ((uint16_t)0x006D)//S.
#define SHOW_DATA_H         ((uint16_t)0x0076)//H.
#define SHOW_DATA_R         ((uint16_t)0x0070 )//R 77  ；70 r


extern uint8_t Show_Place;         //显示的位置
extern uint8_t Show_Data_Buf[Position];   //4位数码管数据缓存区
extern uint8_t g_DP_Pos;//显示小数点位置或不显示小数点状态值



extern void ToggleTHDisplay(void);//5s翻转在数码管显示温湿度
extern void NIXI_GPIO_Init(void);//数码管脚初始化
extern void NIXI_Show(uint8_t place,uint8_t data);//数码管显示
extern void NIXI_revealDecimal_Show(double data);//带小数点显示数字
extern void NIXI_StartTemperature_Show(double num);//显示开关启动温度
extern void NIXI_StopTemperature_Show(double num);//显示开关关闭温度
extern void NIXI_tempOffset_Show(double num);//显示温度补偿
extern void NIXI_highTempAlarm_Show(int num);//显示超高温度报警
extern void NIXI_highMaxTemp_Show(double num);//显示超高温度最高值
extern void NIXI_lowTempAlarm_Show(int num);//显示超低温度报警
extern void NIXI_lowMinTemp_Show(double num);//显示超低温度最低值
extern void NIXI_tempAlarmBuf_Show(double num);//显示温度报警缓存区
extern void EnvConditions_Show(void);//显示开关关闭温度
#endif




