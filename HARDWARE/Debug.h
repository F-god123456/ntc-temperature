#ifndef __DEBUG_H__
#define __DEBUG_H__


//完整的调试信息：设备类型，当前源文件名，当前源函数名，当前源代码行号，格式化输出内容，格式化参数
#define DEBUG_L(format,argent...)  printf("Node:S, File: "__FILE__", Func: %s(), Line: %03d: "format"", __func__, __LINE__, ##argent) 

//简洁的调试信息：设备类型，当前源函数名，格式化输出内容，格式化参数
#define DEBUG_M(format,argent...)  printf("Node:S, Func: %s(), "format"", __func__, ##argent)

//最简洁的调试信息：格式化输出内容，格式化参数
#define DEBUG_S(format,argent...)  printf(""format"",##argent)



#endif


