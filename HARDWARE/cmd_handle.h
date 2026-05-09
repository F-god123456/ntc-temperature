#ifndef __CMD_HANDLE_H__
#define __CMD_HANDLE_H__


extern uint8_t upEnter;
//发送命令缓存区的最大值
#define sendCommandBufLen 250
//命令缓存区的最大值
#define commandBufLen 250

//命令缓存区
extern uint8_t commandBuf[commandBufLen];
//命令缓存区的索引
extern uint8_t commandIndex;
//收到命令的头部的标志位
extern uint8_t headCommandFlag;
//发送命令的缓存区
extern uint8_t sendCommandBuf[sendCommandBufLen];
//联网成功标志  1 ：成功  0：失败
extern uint8_t MqttSuccess;


//标志位
extern uint8_t upSwitchStatusFlag ;//上传继电器开关状态值命令标志位
extern uint8_t upSwitchModeFlag;	//上传开关工作模式值命令标志位
extern uint8_t upSwitchOnTempFlag ;	//上传开启温度值命令标志位
extern uint8_t upSwitchOffTempFlag ;  //上传关闭温度值命令标志位
extern uint8_t upSwitchtempOffsetFlag;//上传温度偏移值命令标志位
extern uint8_t upSwitchhighTempAlarmFlag;//上传超高温度报警状态命令标志位
extern uint8_t upSwitchhighMaxTempFlag;  //上传超高温度报警值命令标志位
extern uint8_t upSwitchlowTempAlarmFlag;//上传低温报警值命令标志位
extern uint8_t upSwitchlowMinTempFlag;// 上传开关超低温度最低值标志位 
extern uint8_t upswitchUpDeviProFlag;  //上传开关模式的所有蚕食标志位
extern uint8_t upswitchUpNewProFlag;	//长传设备实时指令标志位
extern uint8_t updateEnvironmentFlag; 	  //上传环境指令标志位

void checkCommand(void);//检测串口数据是否有命令
void monitor(void);//命令处理		
uint8_t checkMqtt(uint8_t* buf);//监测MQTT的命令

#endif


