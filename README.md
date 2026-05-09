# ThermoGuard - NTC 温度控制器

## 概述

ThermoGuard-NTC 是一款基于 **STM32F0** 微控制器的智能温度控制器，采用 **DS18B20** 数字温度传感器进行温度采集，通过 **4G 通信模块** 实现远程监控和控制。设备具备温度控制、超限报警、数据显示、远程参数设置等功能，适用于需要恒温控制或温度监控的工业及民用场景。

## 硬件架构

| 模块 | 说明 |
|------|------|
| **主控芯片** | STM32F0 系列 ARM Cortex-M0 |
| **温度传感器** | DS18B20（单总线数字温度传感器，-55°C ~ +125°C） |
| **通信方式** | UART1 串口（外接 4G 模块，波特率 115200bps） |
| **显示** | 4位 8段数码管（共用 GPIOA 口） |
| **继电器** | GPIOF Pin6，控制加热/制冷设备 |
| **蜂鸣器** | GPIOF Pin7，温度报警提示 |
| **按键** | 4 个物理按键：确认(OK)、上(UP)、下(DOWN)、返回(RETURN) |
| **电源管理** | 主电源检测 + 备用电池自动切换 |
| **存储** | 片内 Flash（0x0800FC00），保存用户配置参数 |
| **看门狗** | 独立看门狗 IWDG，保证系统稳定运行 |

## 软件结构

```
├── User/               # 主程序入口
│   ├── main.c          # main 函数，系统初始化与主循环
│   └── main.h          # 头文件包含与全局声明
├── HARDWARE/           # 硬件驱动与业务逻辑
│   ├── DS18B20.c/h     # DS18B20 温度传感器驱动（单总线）
│   ├── digitaltube.c/h # 数码管显示驱动
│   ├── relay.c/h       # 继电器控制
│   ├── beep.c/h        # 蜂鸣器报警
│   ├── key.c/h         # 按键扫描
│   ├── menu.c/h        # 菜单系统（支持多层菜单、长按调节）
│   ├── process.c/h     # 温度传感数据处理、控制逻辑、报警逻辑
│   ├── cmd_handle.c/h  # AT 命令接收与解析
│   ├── cmd_data_up.c/h # 数据上报（主动上报 + 事件上报）
│   ├── cmd_str_handle.c/h # 命令字符串处理工具
│   ├── Flash.c/h       # Flash 读写与配置存储
│   ├── data_save.c/h   # 配置参数保存接口
│   ├── led.c/h         # LED 指示灯
│   ├── led_auto.c/h    # 自动/手动模式指示灯
│   ├── led_signal.c/h  # 信号指示灯
│   ├── battery.c/h     # 备用电池管理
│   ├── power.c/h       # 主电源状态检测
│   ├── iwdg.c/h        # 独立看门狗
│   ├── Debug.h         # 调试宏
│   ├── max31865.c/h    # (预留) PT100 传感器驱动
│   ├── SHT4X.c/h       # (预留) SHT4X 温湿度传感器
│   └── DS18B20.h       # DS18B20 头文件
├── SYSTEM/             # 系统级驱动
│   ├── USART1.c/h      # 串口通信（与 4G 模块交互）
│   ├── Timer.c/h       # 系统定时器
│   ├── delay.c/h       # 延时函数
│   └── myspi.c/h       # SPI 通信
├── CORE/               # CMSIS 核心文件
│   ├── stm32f0xx.h     # STM32F0 寄存器定义
│   ├── system_stm32f0xx.c/h
│   ├── stm32f0xx_it.c/h    # 中断服务
│   └── stm32f0xx_conf.h   # 外设配置
├── STM32F03x_FWLib/    # STM32F0 标准外设库
├── ARM/                # ARM CMSIS 库（DSP/Math）
└── README.md           # 本文件
```

## 核心功能

### 1. 温度测量

- 通过 DS18B20 传感器每 1 秒采集一次温度
- 每 5 秒计算一次平均值作为当前温度
- 支持温度偏差校准（tempOffset，可正可负）
- 温度范围限制：-40°C ~ 125°C

### 2. 温度控制（自动模式）

根据用户设定的 **开启温度** 和 **关闭温度** 控制继电器：

- **散热模式**（开启温度 > 关闭温度）：温度超过开启温度时打开继电器（启动散热），低于关闭温度时关闭
- **加热模式**（开启温度 < 关闭温度）：温度低于开启温度时打开继电器（启动加热），高于关闭温度时关闭

### 3. 温度报警

| 功能 | 说明 |
|------|------|
| **高温报警** | 超过设定上限（highMaxTemp）时触发，蜂鸣器响 |
| **低温报警** | 低于设定下限（lowMinTemp）时触发，蜂鸣器响 |
| **报警回差** | 温度恢复到"阈值 ± 回差值(tempAlarmBuf)"后自动解除报警 |
| **远程消警** | 支持通过远程命令消除报警 |

### 4. 传感器故障检测

- 连续 30 次读取失败判定为传感器故障
- 上报传感器故障事件（SensorFault）
- 传感器恢复正常后上报恢复事件（SensorRecover）
- 故障状态每小时最多上报一次，避免频繁上报

### 5. 用户配置存储

所有用户配置保存在 STM32F0 片内 Flash（地址 0x0800FC00）：

| 参数 | 类型 | 说明 |
|------|------|------|
| switchMode | uint8_t | 控制模式：0=自动，1=手动 |
| switchOnTemp | double | 开启温度 |
| switchOffTemp | double | 关闭温度 |
| tempOffset | double | 温度校准偏移量 |
| highTempAlarm | bool | 高温报警开关 |
| highMaxTemp | double | 高温报警阈值 |
| lowTempAlarm | bool | 低温报警开关 |
| lowMinTemp | double | 低温报警阈值 |
| tempAlarmBuf | double | 报警回差 |

### 6. 远程通信（AT 命令协议）

设备通过串口与 4G 模块通信，使用 AT 命令格式：

#### 主动上报（AT+PRO）

上报设备状态到云平台：

| 命令 | 示例 | 说明 |
|------|------|------|
| `AT+PRO:temp,<value>` | `AT+PRO:temp,25.5` | 上报当前温度 |
| `AT+PRO:switchStatus1,<0/1>` | `AT+PRO:switchStatus1,1` | 上报继电器状态 |
| `AT+PRO:switchMode,<0/1>` | `AT+PRO:switchMode,0` | 上报控制模式 |
| `AT+PRO:sensorStatus,<0/1>` | `AT+PRO:sensorStatus,0` | 上报传感器状态（0=正常，1=异常） |

#### 事件上报（AT+EVENT）

上报异常事件到云平台：

| 命令 | 说明 |
|------|------|
| `AT+EVENT:overTempAlarm,<temp>` | 高温报警事件 |
| `AT+EVENT:lowTempAlarm,<temp>` | 低温报警事件 |
| `AT+EVENT:overTempAlarmRecover,<temp>` | 高温恢复事件 |
| `AT+EVENT:lowTempAlarmRecover,<temp>` | 低温恢复事件 |
| `AT+EVENT:SensorFault` | 传感器故障事件 |
| `AT+EVENT:SensorRecover` | 传感器恢复事件 |
| `AT+EVENT:powerStatus,<0/1>` | 电源状态变化事件 |

#### 远程控制（下发命令）

| 命令 | 示例 | 说明 |
|------|------|------|
| `AT+PRO:switchStatus1,<0/1>` | 手动控制继电器开关 |
| `AT+PRO:switchMode,<0/1>` | 切换自动/手动模式 |
| `AT+PRO:switchOnTemp,<value>` | 设置开启温度 |
| `AT+PRO:switchOffTemp,<value>` | 设置关闭温度 |
| `AT+PRO:tempOffset,<value>` | 设置温度校准 |
| `AT+PRO:highTempAlarm,<0/1>` | 开关高温报警 |
| `AT+PRO:highMaxTemp,<value>` | 设置高温阈值 |
| `AT+PRO:lowTempAlarm,<0/1>` | 开关低温报警 |
| `AT+PRO:lowMinTemp,<value>` | 设置低温阈值 |
| `AT+PRO:tempAlarmBuf,<value>` | 设置报警回差 |
| `AT+SER:upDeviPro` | 请求上报所有参数 |
| `AT+SER:upNewPro` | 请求上报实时数据 |
| `AT+SER:eliminateAlarm` | 消除报警 |
| `AT+SER:mqtt,<0/1>` | MQTT 连接状态通知（0=断开，1=成功） |

### 7. 数码管显示

- 4 位 8 段数码管显示当前温度（带小数点）
- 菜单模式下显示参数设置值
- 支持正负温度显示和错误代码显示

### 8. 菜单系统

通过 4 个按键操作的多层菜单：

- **OK 键**：进入菜单 / 确认选择
- **UP 键**：向上翻 / 增加数值
- **DOWN 键**：向下翻 / 减小数值
- **RETURN 键**：返回上级

长按 UP/DOWN 可实现数值连续快速调节。
60 秒无操作自动退出菜单，返回温度显示界面。

### 9. 电源管理

- 实时检测主电源通断状态
- 主电源断电时自动切换备用电池
- 上报电源状态变化事件到云平台
- 备用电池防反接保护，延时关闭策略

### 10. 系统稳定性

- 独立看门狗（IWDG）保证死机后自动复位
- 参数变化后自动保存到 Flash，防止丢失
- 串口命令缓冲和校验，防止误触发

## 版本历史

| 日期 | 版本 | 说明 |
|------|------|------|
| 2025-08-04 | v1.0 | 初始版本，完成基本温度控制、报警、远程通信功能 |

## 开发环境

- **MCU**：STM32F0 系列
- **IDE**：Keil MDK 或任意 ARM 开发工具
- **编译器**：ARMCC / GCC for ARM
- **库**：STM32F0 标准外设库
