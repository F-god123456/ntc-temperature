#ifndef  __DATA_SAVE_H__
#define  __DATA_SAVE_H__


extern void switchModeSave(uint8_t SW_Mode);//开关模式保存到FLASH
extern void switchOnTempSave(double Temp);//开关启动温度保存到FLASH
extern void switchOffTempSave(double Temp);//开关停止温度保存到FLASH
extern void switchtempOffsetSave(double TempOffset);//开关温度补偿保存到FLASH
extern void switchhighTempAlarmSave(bool highTempAlarm);//超高温报警开关保存到FLASH
extern void switchhighMaxTempSave(double highMaxTemp);//超高温度最高值保存到FLASH
extern void switchlowTempAlarmSave(bool lowTempAlarm);//超低温度报警开关保存到FLASH
extern void switchlowMinTempSave(double lowMinTemp);//超低温度报警开关保存到FLASH
extern void switchtempAlarmBufSave(double tempAlarmBuf);//温度报警缓存区保存到FLASH
#endif
