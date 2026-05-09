#include "main.h"                  // Device header

#include "main.h"
#include "cmd_str_handle.h"
#include "math.h"
//定义最大数组长度
#define StrExtFloat_flen 64
//定义最大字符串长度
#define StrExtFloat_slen 512

//从字符串中获取到的浮点数缓冲区
float num[64] = {0.};

/*****************************************************
函数功能:对比字符串，str1跟str2对比
输入参数:str1:字符串1;str2:字符串2
输出参数:
执行命令结果:
1:str1包含str2
*****************************************************/
uint8_t strVS(uint8_t* str1,const uint8_t* str2)
{
	uint8_t i = 0;
	while(str2[i] != 0)
	{
		if(str1[i] != str2[i])
		{
			return 0;
		}
		i++;
	}
	
	if(str1[i] == 0 ||str1[i] == ':' ||str1[i] == ','||(str1[i]>='1'&&str1[i]<='9')||str1[i] == '\r')//判断末尾
	{
		return 1;
	}else
	{
		return 0;
	}
}

/*****************************************************
函数功能:截取字符串
输入参数:str1:截取到的字符串;str2:被截取的字符串
输出参数:
*****************************************************/
uint8_t  intercept_str(uint8_t* str1,uint8_t* str2)
{
	uint8_t i = 0;
	
	while(str2[i] != 0)
	{
		if(str2[i] != '"')
		{
			str1[i] = str2[i];
		}
		else
		{
			str1[i] = '\0';
		}
		i++;
	}
	return 1;
}
/*****************************************************
函数功能:将1个字符转换为16进制数字
输入参数:chr:字符,0~9/A~F/a~F					                 
输出参数:chr对应的16进制数值									   	               
*****************************************************/
uint8_t chr2hex(uint8_t chr)
{
 if(chr>='0'&&chr<='9')return chr-'0';
 if(chr>='A'&&chr<='F')return (chr-'A'+10);
 if(chr>='a'&&chr<='f')return (chr-'a'+10); 
 return 0;
}

/*****************************************************
函数功能:判断字符是不是数字0~9
输入参数:字符				                 
输出参数:1:是数字0~9;0:不是数字0~9;							   	               
*****************************************************/
uint8_t chrisnum(uint8_t chr)
{
	if(chr>='0'&&chr<='9')
	{
		return 1;
	}
	else if((chr>='A'&&chr<='F')||(chr>='a'&&chr<='f'))
	{
		return 0;
	}
	return 0;
}
/*****************************************************
函数功能:将1-15数字转换成字符串
输入参数:hexDigit：数字，strBuffer：接受BUF bufferSize  大小             
输出参数:1:是数字0~9;0:不是数字0~9;							   	               
*****************************************************/
void hex2str(uint8_t hexDigit, uint8_t *strBuffer, uint8_t bufferSize) 
{
    char digit;
	
    // 检查缓冲区大小是否在有效范围内
    if (bufferSize > 2 || bufferSize < 0) {
        return;
    }

    // 计算十位上的数字
    digit = hexDigit / 10;
    strBuffer[0] = digit + '0'; // 将数字转换为字符

    // 计算个位上的数字
    digit = hexDigit % 10;
    strBuffer[1] = digit + '0'; // 将数字转换为字符
}
/*****************************************************
函数功能:截取字符串中的连续数字
输入参数:包含数字的字符串，必须以数字开头
输出参数:将截取到的数字字符转换成数字
*****************************************************/

uint16_t chr_convert_num(uint8_t* buf)
{
	uint8_t i = 0,j = 0,flig = 1;//临时变量
	uint8_t temp[]={0};		//用于保存字符串中的数字
	uint8_t len = 0;				//数字个数
	uint16_t num = 0;			//将获取到的数值组合成新的的数字
	uint16_t num_buf = 0;	//临时变量
	
	while(flig)
	{
		if(chrisnum(buf[i]) == 1)//如果字符串的头一个字符是否为数字，把数字存到数组中
		{
			temp[i]=chr2hex(buf[i]);//将字符串中的数字字符转换成数字并保存到数组中
			i++;
		}
		else//如果检测到字符不是数字，退出检测
		{
			len = i;//数字的个数
			flig = 0;//
		}
	}	
	//将temp[]中的数字，合成一个整数
	for(j = 0;j < len;j++)
	{
		num_buf = temp[j]*pow(10,(len-1-j));
		num = (num+num_buf);
	}
	return num;
}



/*****************************************************
函数功能:将1个16进制数字转换为字符
输入参数:hex:16进制数字,0~15;			                 
输出参数:hex对应字符
*****************************************************/
uint8_t hex2chr(uint8_t hex)
{
 if(hex<=9)return hex+'0';
 if(hex>=10&&hex<=15)return (hex-10+'A'); 
 return '0';
}
/*****************************************************
函数功能:得到字符串的长度
输入参数:str:需要计算长度的字符串	                 
输出参数:len:字符串的长度，
注意:当字符串长度大于65535的时候返回0;	   	               
*****************************************************/
uint16_t strLen(const uint8_t* str)
{
	unsigned int len = 0;
	while(str[len] != 0)
	{
		len++;
		if(len == 65535)
		{
			return 0;
		}
	}
	return len;
}

/*****************************************************
函数功能:3位以内的整数填充到数组里面
输入参数:Index数组下标，value:3位以内的整数
输出参数:无
*****************************************************/
//uint16_t InitToArray(uint8_t Index,int value)
//{
//	if(value>=100&&value<=999)//判断数字位三位数
//	{
//		sendCommandBuf[Index] = (value/100%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = (value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = (value%10)+'0';
//		Index++;
//	}
//	else if(value>=10&&value<=99)//判断数字位两位数
//	{
//		sendCommandBuf[Index] = (value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = (value%10)+'0';
//		Index++;
//	}
//	else if(value>=0&&value<=9)//判断数字位一位数
//	{
//		sendCommandBuf[Index] = (value%10)+'0';
//		Index++;
//	}
//	return Index;
//}
///*****************************************************
//函数功能:3位以内的浮点数填充到数组里面,小数点为一位
//输入参数:Index数组下标，value:3位以内的浮点数
//输出参数:无
//*****************************************************/
//uint16_t FloatToArray(uint8_t Index,float value)
//{
//	if(value>=100&&value<1000)//判断数字位三位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value/100%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//	}
//	else if(value>=10&&value<100)//判断数字位两位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//	}
//	else if(value>=0&&value<10)//判断数字位一位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//	}
//	return Index;
//}
///*****************************************************
//函数功能:3位以内的浮点数填充到数组里面,小数点为两位
//输入参数:Index数组下标，value:3位以内的浮点数
//输出参数:无
//*****************************************************/
//uint16_t FloatToArray2(uint8_t Index,float value)
//{
//	if(value>=100&&value<1000)//判断数字位三位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value/100%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*100)%10)+'0';
//		Index++;
//	}
//	else if(value>=10&&value<100)//判断数字位两位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value/10%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*100)%10)+'0';
//		Index++;
//	}
//	else if(value>=0&&value<10)//判断数字位一位数
//	{
//		sendCommandBuf[Index] = ((uint16_t)value%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = '.';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*10)%10)+'0';
//		Index++;
//		sendCommandBuf[Index] = ((uint16_t)(value*100)%10)+'0';
//		Index++;
//	}
//	return Index;
//}


/*****************************************************
函数功能:将字符串填充到上传数组中
输入参数:Index:上传数组下标;buf:填充的字符串
输出参数:无
*****************************************************/
//uint8_t StrToArray(uint8_t arrayIndex,const uint8_t *buf)
//{
//	unsigned int i;
//	unsigned int len = strLen(buf);
//	
//	for(i = 0;i < len;i++)
//	{
//		sendCommandBuf[arrayIndex + i] = buf[i];
//	}
//	return len+arrayIndex;
//}
/*****************************************************
函数功能:将字符串2的数据填充到字符串1中
输入参数:buf2:源数据,buf1:被填充的数据
输出参数:无
*****************************************************/
void StrToSwitchName(uint8_t *buf1,const uint8_t *buf2)
{
	unsigned int i;
	unsigned int len = strLen(buf2);
	
	for(i = 0;i < len;i++)
	{
		buf1[i] = buf2[i];
	}
	buf1[len] = '\0';
}


/*****************************************************
函数功能:获取三个浮点类型数中的最小值
输入参数:传入三个浮点类型数
输出参数:返回最小值
*****************************************************/
float vsMinValue(float Value1,float Value2,float Value3)
{
	float MinValue;
	if (Value1 < Value2) 
	{
		MinValue = Value1;
	} 
	else 
	{
		MinValue = Value2;
	}
	if (MinValue > Value3) 
	{
		MinValue = Value3;
	}
	return MinValue;
}
/*****************************************************
函数功能:获取三个浮点类型数中的中间值
输入参数:传入三个浮点类型数
输出参数:返回中间值
*****************************************************/
float vsSecondValue(float a,float b,float c)
{
	return a > b ? (b > c ? b : ( a > c ? c : a)) : ( a > c ? a: (b > c ? c : a));
}
/*****************************************************
函数功能:获取三个浮点类型数中的最大值
输入参数:传入三个浮点类型数
输出参数:返回最大值
*****************************************************/
float vsMaxValue(float Value1,float Value2,float Value3)
{
	float MaxValue;
	if (Value1 > Value2) 
	{
		MaxValue = Value1;
	} 
	else 
	{
		MaxValue = Value2;
	}
	if (MaxValue < Value3) 
	{
		MaxValue = Value3;
	}
	return MaxValue;
}

/*****************************************************
函数功能:获得三个数的最大数索引
输入参数:输入三个数
输出参数:无
*****************************************************/
uint8_t vsMaxValueIndex(float Value1,float Value2,float Value3)
{
	float MaxValue;
	uint8_t MaxValueIndex;
	if (Value1 > Value2) 
	{
		MaxValue = Value1;
		MaxValueIndex = 1;
	} 
	else 
	{
		MaxValue = Value2;
		MaxValueIndex = 2;
	}
	if (MaxValue < Value3) 
	{
		MaxValue = Value3;
		MaxValueIndex = 3;
	}
	return MaxValueIndex;
}
/*****************************************************
函数功能:获得三个数的最小数索引
输入参数:输入三个数
输出参数:无
*****************************************************/
uint8_t vsMinValueIndex(float Value1,float Value2,float Value3)
{
	float MinValue;
	uint8_t MinValueIndex;
	if (Value1 < Value2) 
	{
		MinValue = Value1;
		MinValueIndex = 'A';
	} 
	else 
	{
		MinValue = Value2;
		MinValueIndex = 'B';
	}
	if (MinValue > Value3) 
	{
		MinValue = Value3;
		MinValueIndex = 'C';
	}
	return MinValueIndex;
}
/*****************************************************
函数功能:获得三个数的中间数索引
输入参数:输入三个数
输出参数:无
*****************************************************/
uint8_t vsSecondValueIndex(float a,float b,float c)
{
	uint8_t SecondValueIndex;
	float temp;
	temp = a > b ? (b > c ? b : ( a > c ? c : a)) : ( a > c ? a: (b > c ? c : a));
	if(temp == a)
	{
		SecondValueIndex = 1;
	}
	if(temp == b)
	{
		SecondValueIndex = 2;
	}
	if(temp == c)
	{
		SecondValueIndex = 3;
	}
	return SecondValueIndex;
}


/*****************************************************
函数功能:通过相序索引判断相序号,1对应A;2对应B;3对应C;
输入参数:Index:发送数组缓冲区索引;value:相序索引
输出参数:发送数组缓冲区索引
*****************************************************/
//uint16_t SerialNumber(uint8_t Index,uint8_t value)
//{
//	if(value == 1)
//	{
//		sendCommandBuf[Index] = 'A';//电流最低相
//		Index++;
//	}
//	else if(value == 2)
//	{
//		sendCommandBuf[Index] = 'B';//电流最低相
//		Index++;
//	}
//	else if(value == 3)
//	{
//		sendCommandBuf[Index] = 'C';//电流最低相
//		Index++;
//	}
//	return Index;
//}
/*****************************************************
函数功能:从字符串中获得浮点型数
输入参数:*num:获取浮点数的指针;*Str:源字符串
输出参数:无
*****************************************************/
void StrExtFloat(float *num,uint8_t* Str)
{
	//遍历深度
	int Fflag = 0;
	int i;
	float fpoint;
	int Fnum = 0;				//数字个数
	uint8_t num_start = 0;	//整数标志位
	uint8_t num_point = 0;	//小数点标志位
	//遍历到字符串尾部
	while ( *Str != '\0' )
	{	
		Fflag++;
		//防止查询超过边界
		if(Fflag>StrExtFloat_slen)
		{
			break;
		}
		//判断是不是数字
		if(*Str >='0' && *Str <= '9')
		{
			//printf("%c",*Str);
			//判断数字存在
			num_start = 1;
			//判断是否存在小数点
			if(num_point >= 1)
			{
				num_point++;
				//当前小数部分的数值
				 fpoint = *Str - '0';
				for(i = 1;i<num_point;i++)
				{
					fpoint = fpoint/10.;
				}
				//加入小数部分
				num[Fnum+1] = num[Fnum+1] + fpoint;
			}
			else
			{
				//加入整数部分
				num[Fnum+1] = num[Fnum+1]*10+(*Str - '0');
			}
		}
		else if(*Str == '.') //判断为小数点
		{
			if(num_start==1)//发现存在小数点
			{
				num_point=1;
			}
		}
		else //判断为其他字符
		{
			if (num_start == 1)
			{
				Fnum++;//统计个数加一
			}
			//清空字符统计与小数点统计
			num_start = 0;
			num_point = 0;
		}
		//指针移动
		*(Str++);
	}
	
	//如果不是以字符结尾
	if (num_start == 1)
	{
		Fnum++;//统计个数加一
	}
	//放入提取到的数字个数
	num[0] = Fnum;
}
/*****************************************************
函数功能:从字符串中获得浮点型数
输入参数:*Str:源字符串
输出参数:获得的浮点型数
*****************************************************/
float StrToFloat(uint8_t* Str)
{
	float gatNum = 0;		//小数点标志位
	num[1] = 0;
	StrExtFloat(num,Str);
	gatNum = num[1];
	return gatNum;
}






