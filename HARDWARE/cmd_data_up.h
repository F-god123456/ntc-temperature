#ifndef __CMD_DATA_UP_
#define __CMD_DATA_UP_

extern void updataSensorFaultCommand(void);//上传传感器错误事件到云端
extern void updataOverTempAlarmCommand(void);//上传超高温报警事件到云端
extern void updataLowTempAlarmCommand(void);//上传超低温报警事件到云端
extern void updataOverTempAlarmRecoverCommand(void);//上传超高温恢复报警事件到云端
extern void updataLowTempAlarmRecoverCommand(void);//上传超低温恢复报警事件到云端

extern void updataTempCommand(void);//上传温度命令到云端
extern void updataSwitchALLDataCommand(void);//更新与开关相关的所有数据命令。
extern void updataSwitchStatusCommand(void);//上传开关命令到云端
extern void updataSwitchModeCommand(void);//上传工作模式到云端
extern void updataSwitchOnTempCommand(void);//上传开关开始温度到云端
extern void updataSwitchOffTempCommand(void);//上传开关停止温度到云端
extern void updataSwitchtempOffsetCommand(void);//上传温度补偿到云端
extern void updataSwitchhighTempAlarmCommand(void);//上传超高温报警开关到云端
extern void updataSwitchhighMaxTempCommand(void);//上传超高温度最高值到云端
extern void updataSensorRecoverCommand(void);
extern void updataSwitchlowTempAlarmCommand(void);//上传超低温度报警开关到云端
extern void updataSwitchlowMinTempCommand(void);//上传超低温度最低值到云端

extern void updataSwitchtempAlarmBufCommand(void);//上传温度报警缓存区到云端

extern void updataSwitchNewProCommand(void);//用于更新与开关、温度相关的命令。
extern void updataPowerStatusCommand(void);//上传电源状态变化事件到云端
extern void updataSensorStatusCommand(void);//上传传感器状态到云端
extern void updateEnvironmentCommand(void);
#endif


