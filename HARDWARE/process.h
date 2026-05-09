#ifndef __PROCESS_H__
#define __PROCESS_H__


// 假设这是你的系统时间结构
typedef struct {
    uint32_t seconds;
    uint32_t milliseconds;
} SystemTime;


extern uint8_t Sensor_State;//0:传感器正常，1传感器异常

// 当前系统时间，用于记录和同步操作
extern SystemTime currentTime;
// 过温恢复标志，指示系统是否从过温状态恢复
extern uint8_t OverTempRecover_Flag;
// 低温恢复标志，指示系统是否从低温状态恢复
extern uint8_t LowTempRecover_Flag;
// 过热标志，指示系统是否检测到过热情况
extern uint8_t SuperHeatFlag;
// 低温标志，指示系统是否检测到低温情况
extern uint8_t LowTempFlag;
// 过湿标志，指示系统是否检测到过湿情况
extern uint8_t SuperHumidityFlag;
// 低湿标志，指示系统是否检测到低湿情况
extern uint8_t LowHumidityFlag;
// 传感器故障标志，指示是否有传感器发生故障
extern uint8_t Sensor_Fault_Flag;
// 成功读取标志，指示传感器数据是否成功读取
extern uint8_t successReadings;
// 过湿恢复标志，指示系统是否从过湿状态恢复
extern uint8_t OverHumidRecover_Flag;
// 低湿恢复标志，指示系统是否从低湿状态恢复
extern uint8_t LowHumidRecover_Flag;
// 传感器小时故障标志，用于故障倒计时
extern uint8_t Sensor_Hour_Falg;
//传感器恢复正常标志位
extern uint8_t Sensor_recover_Flag;
// 用于上传传感器状态标志位
extern uint8_t upSensorStatusFlag ;      
//读取失败次数
extern uint8_t Sensor_Fail_Count;
//第一次读取传感数据标志位
extern uint8_t head_read_successs_flag;
// 读取传感器数据，执行传感器数据采集操作
extern void Read_Sensor_Data(void);
// 处理环境传感器数据，对采集到的数据进行处理
extern void EnvSense_ProcessData(void);
// 读取温度数据，专门用于读取温度传感器的数据
extern void ReadTemperature(void);



#endif


