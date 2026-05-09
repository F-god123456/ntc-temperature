#ifndef __CMD_STR_HANDLE_H__
#define __CMD_STR_HANDLE_H__

#include "stm32f0xx.h"                  // Device header

void hex2str(uint8_t hexDigit, uint8_t *strBuffer, uint8_t bufferSize) ;//将1-15数字转换成字符串
uint8_t strVS(uint8_t* str1,const uint8_t* str2);//对比字符串
uint8_t  intercept_str(uint8_t* str1,uint8_t* str2);//截取字符串
uint8_t chr2hex(uint8_t chr);//将1个字符转换为16进制数字
uint8_t chrisnum(uint8_t chr);//判断字符是不是数字0~9
uint8_t hex2chr(uint8_t hex);//将1个16进制数字转换为字符
uint8_t StrToArray(uint8_t Index,const uint8_t *buf);//将字符串填充到上传数组中
uint8_t vsMinValueIndex(float Value1,float Value2,float Value3);//获得三个数的最小数索引
uint8_t vsMaxValueIndex(float Value1,float Value2,float Value3);//获得三个数的最大数索引
uint8_t vsSecondValueIndex(float a,float b,float c);//获得三个数的中间数索引

uint16_t strLen(const uint8_t* str);//得到字符串的长度
uint16_t chr_convert_num(uint8_t* buf);//截取字符串中的连续数字
uint16_t SerialNumber(uint8_t Index,uint8_t value);//通过相序索引判断相序号,1对应A;2对应B;3对应C;
uint16_t InitToArray(uint8_t Index,int value);//3位以内的整数填充到数组里面
uint16_t FloatToArray(uint8_t Index,float value);//3位以内的浮点数填充到数组里面,小数点为一位
uint16_t FloatToArray2(uint8_t Index,float value);//3位以内的浮点数填充到数组里面,小数点为两位

void StrExtFloat(float *num,uint8_t* Str);//从字符串中获得浮点型数
void StrToSwitchName(uint8_t *buf1,const uint8_t *buf2);//将字符串2的数据填充到字符串1中

float vsMaxValue(float VoltageA,float VoltageB,float VoltageC);//获取三个浮点类型数中的最大值
float vsMinValue(float VoltageA,float VoltageB,float VoltageC);//获取三个浮点类型数中的最小值
float vsSecondValue(float a,float b,float c);//获取三个浮点类型数中的中间值
float StrToFloat(uint8_t* Str);//从字符串中获得浮点型数

#endif



