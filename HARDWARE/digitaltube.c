#include "main.h"


/***************************************************

    数码管分布                   引脚分布
        A                NIXI_DIG1    PA8
     ---------           NIXI_DIG2    PA11
    |         |          NIXI_DIG3    PA12
   F|         |B         NIXI_DIG4    PA15
    |    G    |          NIXI_A       PA0
    |---------|          NIXI_B       PA1
    |         |          NIXI_C       PA2
   E|         |C         NIXI_D       PA3
    |    D    |          NIXI_E       PA4
    |---------| .DP      NIXI_F       PA5
                         NIXI_G       PA6
                         NIXI_DP      PA7

*****************************************************/


#define DATA_SET(x,y)    (x->BSRR) = y
#define DATA_RESET(x,y)  (x->BRR)  = y


#define PLACE_SET(x,y)    (x->BSRR) = y
#define PLACE_RESET(x,y)  (x->BRR)  = y

typedef enum {
    HideDecimalPoint=0,    // 不显示小数点
    ShowFirstDecimal=1,    // 显示第一位小数点
    ShowSecondDecimal=2,   // 显示第二位小数点
    ShowThirdDecimal=3     // 显示第三位小数点
} DecimalDisplayOption;


uint8_t Show_Place;                  //当前显示位号
uint8_t Show_Data_Buf[Position];     //4位数显数据缓冲区

uint8_t g_DP_Pos=0;//显示小数点位置或显示小数点状态值

static GPIO_InitTypeDef GPIO_InitStruct;

/*************************************************************
功能：数码管引脚初始化
输入：无
返回：无
*************************************************************/
void NIXI_GPIO_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = NIXI_PIN_A | NIXI_PIN_B | NIXI_PIN_C | NIXI_PIN_D | NIXI_PIN_E | NIXI_PIN_F | NIXI_PIN_G | NIXI_PIN_DP |
                               NIXI_PIN_DIG1 | NIXI_PIN_DIG2| NIXI_PIN_DIG3|NIXI_PIN_DIG4 ;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
    GPIO_Init(NIXI_PORT, &GPIO_InitStruct);

}

/*************************************************************
功能：数码管写入位选择
参数：
        place:写入位号
返回：无
*************************************************************/
void NIXI_Set_Place(uint8_t place)
{
    if(place>4)
        return ;
    switch(place)
    {
        case 0:
            PLACE_SET(NIXI_PORT,NIXI_PIN_DIG1);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG2);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG3);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG4);
            break;
        case 1:

            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG1);
            PLACE_SET(NIXI_PORT,NIXI_PIN_DIG2);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG3);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG4);
            break;
        case 2:
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG1);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG2);
            PLACE_SET(NIXI_PORT,NIXI_PIN_DIG3);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG4);
            break;
        case 3:

            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG1);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG2);
            PLACE_RESET(NIXI_PORT,NIXI_PIN_DIG3);
            PLACE_SET(NIXI_PORT,NIXI_PIN_DIG4);
            break;
    }
}



/*************************************************************
功能：数码管写数据
参数：
        data :要写入的数据
        place: 要写入的位号
返回：NULL
*************************************************************/
void NIXI_Set_Data(uint8_t data, uint8_t place)
{

    uint8_t show_dp;
    // 1. 将 data 转换成要显示的段码（不包括小数点）
    uint32_t seg_code = 0;

    // 2. 先关闭所有段，防止串扰
    DATA_RESET(NIXI_PORT, SHOW_DATA_ALL);

    switch (data)
    {
        case 0:  seg_code = SHOW_DATA_0;  break;
        case 1:  seg_code = SHOW_DATA_1;  break;
        case 2:  seg_code = SHOW_DATA_2;  break;
        case 3:  seg_code = SHOW_DATA_3;  break;
        case 4:  seg_code = SHOW_DATA_4;  break;
        case 5:  seg_code = SHOW_DATA_5;  break;
        case 6:  seg_code = SHOW_DATA_6;  break;
        case 7:  seg_code = SHOW_DATA_7;  break;
        case 8:  seg_code = SHOW_DATA_8;  break;
        case 9:  seg_code = SHOW_DATA_9;  break;

        case ' ': seg_code = SHOW_DATA_NONE; break;
        case '-': seg_code = SHOW_DATA_ACROSS; break;
        case '.': seg_code = SHOW_DATA_DP; break;
        case 'B': seg_code = SHOW_DATA_B; break;
        case 'E': seg_code = SHOW_DATA_E; break;
        case 'F': seg_code = SHOW_DATA_F; break;
        case 'S': seg_code = SHOW_DATA_S; break;
        case 'H': seg_code = SHOW_DATA_H; break;
        case 'R': seg_code = SHOW_DATA_R; break;

        default:
            seg_code = SHOW_DATA_NONE; // 未知字符不显示
            break;
    }

    // 3. 处理小数点：是否要显示 DP
    // 只在特定位置（温度的小数点位置、湿度小数点）才显示
    show_dp = 0;

    if (place == 0 && g_DP_Pos == ShowFirstDecimal)
        show_dp = 1;
    else if (place == 1 && g_DP_Pos == ShowSecondDecimal)
        show_dp = 1;
    else if (place == 2 && g_DP_Pos == ShowThirdDecimal)
        show_dp = 1;

    // 如果需要显示小数点，则 OR 上 SHOW_DATA_DP
    if (show_dp)
    {
        seg_code |= SHOW_DATA_DP;
    }

    // 4. 将段码输出到端口
    DATA_SET(NIXI_PORT, seg_code);
}
extern uint32_t systick_timer_elapsed_us(uint32_t start_val) ;
/*************************************************************
功能：数码管写入
参数：
    place:写入位号
    data:写入的数据
返回：无
*************************************************************/
void NIXI_Show(uint8_t place,uint8_t data)
{
    NIXI_Set_Place(place);
    NIXI_Set_Data(data,place);
}


/*
***功能：显示整数数字
***参数：显示数字
***返回：无
*/
void NIXI_number_Show(double num)
{
    if(num>=0)
    {
        if(num<10)//个位数   1
        {
            g_DP_Pos = HideDecimalPoint;
            num*=10;  //15
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = ' ';
            Show_Data_Buf[2] = ' ';
            Show_Data_Buf[3] = (uint8_t)num/10;
        }
        else if(num>=10 &&num<100)//十位数   16
        {
            g_DP_Pos = HideDecimalPoint;
            num*=10;  //163
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = ' ';
            Show_Data_Buf[2] = num/100;
            Show_Data_Buf[3] = (uint8_t)(num / 10) % 10;
        }
        else if(num>=100)//百位数 165
        {
            g_DP_Pos = HideDecimalPoint;
            num*=10;  //1650
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = num/1000;
            Show_Data_Buf[2] = (uint8_t)(num/100)%10;
            Show_Data_Buf[3] = (uint8_t)(num / 10) % 10;
        }
    }
    else
    {
        if(-(num)<10)//个位数   1
        {
            g_DP_Pos = HideDecimalPoint;
            num*=-10;  //15
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = ' ';
            Show_Data_Buf[2] = '-';
            Show_Data_Buf[3] = (uint8_t)num/10;
        }
        else if(-(num)>=10 &&-(num)<100)//十位数   16
        {
            g_DP_Pos = HideDecimalPoint;
            num*=-10;  //163
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = '-';
            Show_Data_Buf[2] = num/100;
            Show_Data_Buf[3] = (uint8_t)(num / 10) % 10;
        }
        else if(-(num)>=100)//百位数 165
        {
            g_DP_Pos = HideDecimalPoint;
            num*=-10;  //1650
            Show_Data_Buf[0] = '-';
            Show_Data_Buf[1] = num/1000;
            Show_Data_Buf[2] = (uint8_t)(num/100)%10;
            Show_Data_Buf[3] = (uint8_t)(num / 10) % 10;
        }
    }
}


/*
***功能：显示带小数的数字
***参数：显示数字
***返回：无
*/
void NIXI_revealDecimal_Show(double data)
{
    uint32_t temp;

    if(data>=0)
    {
        if(data<10)//个位数   1.5
        {
            g_DP_Pos = ShowThirdDecimal;
            temp=data*10;  //15
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = ' ';
            Show_Data_Buf[2] = (uint8_t)temp/10;
            Show_Data_Buf[3] = (uint8_t)fmod(temp, 10);//固定为小数点
        }
        else if(data>=10 &&data<100)//十位数   16.3
        {
            g_DP_Pos = ShowThirdDecimal;
            temp=data*10;  //163
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = temp/100;
            Show_Data_Buf[2] = (uint8_t)(temp / 10) % 10;
            Show_Data_Buf[3] = (uint8_t)fmod(temp, 10);//固定为小数点
        }
        else if(data>=100)//百位数 165.2
        {
            g_DP_Pos = ShowThirdDecimal;
            temp=data*10;  //163

            Show_Data_Buf[0] = temp/1000;
            Show_Data_Buf[1] = (uint8_t)(temp/100)%10;
            Show_Data_Buf[2] = (temp / 10) % 10;
            Show_Data_Buf[3] = (uint8_t)fmod(temp, 10);//固定为小数点
        }
    }
    else
    {
        if((-data)<10)//个位数   1.5
        {
            g_DP_Pos = ShowThirdDecimal;
            temp=data*-10;  //15
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = '-';
            Show_Data_Buf[2] = (uint8_t)temp/10;
            Show_Data_Buf[3] = (uint8_t)fmod(temp, 10);//固定为小数点
        }
        else if((-data)>=10 &&(-data)<100)//十位数   16.3
        {
            g_DP_Pos = ShowThirdDecimal;
            temp=data*-10;  //-13
            Show_Data_Buf[0] = '-';
            Show_Data_Buf[1] = temp/100;
            Show_Data_Buf[2] = (uint8_t)(temp / 10) % 10;
            Show_Data_Buf[3] = (uint8_t)fmod(temp, 10);//固定为小数点
        }
        else if((-data)>=100)//百位数 -165
        {
            g_DP_Pos = HideDecimalPoint;
            temp=data*-10;  //-163
            Show_Data_Buf[0] = '-';
            Show_Data_Buf[1] = temp/1000;
            Show_Data_Buf[2] = (uint8_t)(temp/100)%10;
            Show_Data_Buf[3] = (uint8_t)(temp / 10) % 10;//固定为小数点
        }
    }
    return ;
}

/*
***功能：显示湿度
***参数：湿度数据
***返回：无
*/
void NIXI_Humid_Show(double data)
{

    if(data>=0)
    {
        if(data<10)//个位数   1.5H
        {
            g_DP_Pos=ShowSecondDecimal;
            data*=10;  //15
            Show_Data_Buf[0] = ' ';
            Show_Data_Buf[1] = (uint8_t)data/10;
            Show_Data_Buf[2] = (uint8_t)fmod(data, 10);//固定为小数点
            Show_Data_Buf[3] = 'H';
        }
        else if(data>=10 &&data<100)//十位数   16.3
        {
            g_DP_Pos=ShowSecondDecimal;
            data*=10;  //163
            Show_Data_Buf[0] = data/100;
            Show_Data_Buf[1] = (uint8_t)(data / 10) % 10;
            Show_Data_Buf[2] = (uint8_t)fmod(data, 10);//固定为小数点
            Show_Data_Buf[3] = 'H';
        }
        else if(data>=100)//百位数 165.2
        {
            g_DP_Pos=HideDecimalPoint;
            data*=10;  //163
            Show_Data_Buf[0] = data/1000;
            Show_Data_Buf[1] = (uint8_t)(data/100)%10;
            Show_Data_Buf[2] = (uint8_t)(data / 10) % 10;//固定为小数点
            Show_Data_Buf[3] = 'H';
        }
    }
    return ;

}

/*
***功能：显示开启温度
***参数：无
***返回：无
*/
void NIXI_StartTemperature_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示关闭温度
***参数：无
***返回：无
*/
void NIXI_StopTemperature_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示温度偏移值
***参数：无
***返回：无
*/
void NIXI_tempOffset_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示超温报警值
***参数：无
***返回：无
*/
void NIXI_highTempAlarm_Show(int num)
{
    NIXI_number_Show(num);
}

/*
***功能：显示超温上限值
***参数：无
***返回：无
*/
void NIXI_highMaxTemp_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示低温报警值
***参数：无
***返回：无
*/
void NIXI_lowTempAlarm_Show(int num)
{
    NIXI_number_Show(num);
}

/*
***功能：显示低温下限值
***参数：无
***返回：无
*/
void NIXI_lowMinTemp_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示温度缓冲预警值
***参数：无
***返回：无
*/
void NIXI_tempAlarmBuf_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示湿度偏移值
***参数：无
***返回：无
*/
void NIXI_humidOffset_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示超湿报警值
***参数：无
***返回：无
*/
void NIXI_hignHumidAlarm_Show(int num)
{
    NIXI_number_Show(num);
}

/*
***功能：显示超湿上限值
***参数：无
***返回：无
*/
void NIXI_highMaxHumid_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示低湿报警值
***参数：无
***返回：无
*/
void NIXI_lowHumidAlarm_Show(int num)
{
    NIXI_number_Show(num);
}

/*
***功能：显示低湿下限值
***参数：无
***返回：无
*/
void NIXI_lowMinHumid_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}
/*
***功能：显示湿度缓冲值
***参数：无
***返回：无
*/
void NIXI_humidAlarmBuf_Show(double num)
{
    NIXI_revealDecimal_Show(num);
}

/*
***功能：显示错误信息
***参数：无
***返回：无
*/
void NIXI_Error_Show(void)
{
    g_DP_Pos = HideDecimalPoint;
    Show_Data_Buf[0] = 'E';
    Show_Data_Buf[1] = 'R';
    Show_Data_Buf[2] = 'R';
    Show_Data_Buf[3] =  1;

    return ;

}


/*
***功能：显示传感器读取失败
***参数：无
***返回：无
*/
void NIXI_No_data_read_Show(void)
{
    g_DP_Pos = HideDecimalPoint;
    Show_Data_Buf[0] = '-';
    Show_Data_Buf[1] = '-';
    Show_Data_Buf[2] = '-';
    Show_Data_Buf[3] = '-';

    return ;
}


/*
***功能：显示温度
***参数：无
***返回：无
*/
void EnvConditions_Show(void)
{

    if(MenuFlag!=1)
    {

        if(head_read_successs_flag == 1)
        {
            NIXI_No_data_read_Show();
        }
        else if(Sensor_State == 1 || Sensor_Fail_Count==30)
        {
            NIXI_Error_Show();
            return ;
        }
        else
        {
            NIXI_revealDecimal_Show(T);
        }
    }

}
