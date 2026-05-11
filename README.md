# ThermoGuard-NTC 温度控制器

基于 **STM32F030C8** 的智能温度控制器，采用 **NTC 热敏电阻 + ADC** 采集温度，通过 **4G 模块** 实现远程监控和控制。

---

## 硬件架构

| 模块 | 说明 |
|------|------|
| **主控** | STM32F030C8 (Cortex-M0, 48MHz) |
| **温度传感器** | NTC 热敏电阻，PB0(ADC_IN8) 连续转换，12位 |
| **通信** | USART1 (PA9-TX, PA10-RX)，115200-8N1 |
| **显示** | 4位8段LED数码管，GPIOA段码+位选多路复用 |
| **继电器** | PF6，控制加热/制冷设备 |
| **蜂鸣器** | PF7，温度报警提示 |
| **按键** | 5个物理按键：Menu/OK(PB9)、上(PB1)、下(PB2)、返回(PB3)、开/关(PB4) |
| **指示灯** | 自动模式LED(PB8)、普通LED(PB1) |
| **电池** | PB6，备用电池控制 |
| **电源检测** | PB5，主电源通断检测 |
| **存储** | 片内Flash (0x0800FC00)，保存用户配置 |
| **看门狗** | 独立看门狗 IWDG (约25.6s超时) |

---

## 软件结构

```
├── User/
│   ├── main.c              # 主函数：初始化 + 主循环(每20ms)
│   └── main.h              # 全局头文件包含
├── HARDWARE/               # 硬件驱动 + 业务逻辑
│   ├── ntc.c/h             # NTC驱动：ADC采样、滤波、5阶多项式拟合
│   ├── digitaltube.c/h     # 数码管显示驱动
│   ├── relay.c/h           # 继电器控制 (PF6)
│   ├── beep.c/h            # 蜂鸣器驱动 (PF7)
│   ├── key.c/h             # 5按键扫描、消抖、FIFO事件队列
│   ├── menu.c/h            # 菜单系统 (8项参数配置)
│   ├── process.c/h         # 核心处理：温度校准、报警检测、自动控制
│   ├── cmd_handle.c/h      # AT命令接收/解析 + 事件上传调度
│   ├── cmd_data_up.c/h     # 命令构建与发送
│   ├── cmd_str_handle.c/h  # 字符串处理工具
│   ├── Flash.c/h           # Flash读写与配置持久化
│   ├── data_save.c/h       # 配置保存接口
│   ├── led.c/h             # 普通LED驱动
│   ├── led_auto.c/h        # 自动模式指示灯 (PB8)
│   ├── led_signal.c/h      # 信号指示灯
│   ├── battery.c/h         # 备用电池管理
│   ├── power.c/h           # 主电源状态检测
│   ├── iwdg.c/h            # 独立看门狗
│   └── Debug.h             # 调试打印宏
├── SYSTEM/
│   ├── USART1.c/h          # 串口驱动 (115200, 环形缓冲区)
│   ├── Timer.c/h           # 定时器: TIM3(10ms按键) TIM14(3ms显示) TIM16(20ms主循环)
│   └── delay.c/h           # SysTick延时
├── CORE/                   # CMSIS核心文件
├── STM32F03x_FWLib/        # 标准外设库
└── ARM/                    # CMSIS库
```

---

## 核心功能

### 1. 温度采集

- **采样**：ADC1连续转换，每次640个样本，去头尾40个后取平均
- **转换**：5阶多项式拟合 ADC → °C，有效范围 -40°C ~ 120°C
- **节奏**：每1秒采集一次，每5秒滑动平均后更新显示值 `T`
- **校准**：支持用户可配偏移量 `tempOffset`
- **故障检测**：连续30次ADC超范围(≤128 或 ≥3955)判为传感器故障
  - 故障时停止报警检测和自动控制，继电器保持当前状态
  - 每小时重试上报，最多10次

### 2. 显示

- 4位LED数码管显示当前温度（含小数点、负号）
- 传感器故障时显示 `ERR`
- 菜单模式显示 `S--1` ~ `S--8`

### 3. 按键操作

| 按键 | 短按 | 长按 |
|------|------|------|
| Menu/OK (PB9) | 菜单内确认 | 非菜单时进入菜单 |
| 上 (PB1) | 菜单上移 / 参数+0.1°C | 参数+1.0°C |
| 下 (PB2) | 菜单下移 / 参数-0.1°C | 参数-1.0°C |
| 返回 (PB3) | 菜单返回上级 | — |
| 开/关 (PB4) | 翻转继电器 | 切换自动/手动模式 |

### 4. 继电器控制

- **手动模式**：短按开/关键翻转
- **自动模式**：根据开启/关闭温度自动控制
  - 散热模式（开启 > 关闭）：超开启温度→ON，低于关闭温度→OFF
  - 加热模式（开启 < 关闭）：低于开启温度→ON，超关闭温度→OFF
  - 中间区间为滞回区，保持原状态

### 5. 温度报警

| 报警 | 触发条件 | 恢复条件 |
|------|----------|----------|
| 高温报警 | T > `highMaxTemp` | T ≤ `highMaxTemp` - `tempAlarmBuf` |
| 低温报警 | T < `lowMinTemp` | T ≥ `lowMinTemp` + `tempAlarmBuf` |

报警时蜂鸣器响，上报事件到云端。支持远程消警。

**⚠️注意**：报警期间按键被拦截，需按2次任意键关闭蜂鸣器后才能正常操作。

### 6. 菜单系统（8项可配置参数）

| 菜单 | 参数 | 说明 |
|------|------|------|
| S--1 | switchOnTemp | 开启温度 |
| S--2 | switchOffTemp | 关闭温度 |
| S--3 | tempOffset | 温度偏移校准 |
| S--4 | highTempAlarm | 高温报警开关 |
| S--5 | highMaxTemp | 高温报警阈值 |
| S--6 | lowTempAlarm | 低温报警开关 |
| S--7 | lowMinTemp | 低温报警阈值 |
| S--8 | tempAlarmBuf | 报警回差 |

60秒无操作自动退出并保存到Flash。

### 7. AT指令协议

#### 主动上报（设备 → 云端）

| 命令 | 触发时机 |
|------|----------|
| `AT+PRO:temp,<值>` | 传感器恢复、收到 upNewPro/upDeviPro 指令时 |
| `AT+PRO:switchStatus1,<0/1>` | 继电器状态变化 |
| `AT+PRO:switchMode,<0/1>` | 模式切换 |
| `AT+PRO:sensorStatus,<0/1>` | 传感器故障/恢复 |
| `AT+EVENT:overTempAlarm,<值>` | 高温报警触发 |
| `AT+EVENT:overTempAlarmRecover,<值>` | 高温报警恢复 |
| `AT+EVENT:lowTempAlarm,<值>` | 低温报警触发 |
| `AT+EVENT:lowTempAlarmRecover,<值>` | 低温报警恢复 |
| `AT+EVENT:SensorFault` | 传感器故障 |
| `AT+EVENT:SensorRecover` | 传感器恢复 |
| `AT+EVENT:powerStatus,<0/1>` | 主电源通断 |

**⚠️注意**：温度不上报，仅在事件触发时附带上传。

#### 远程控制（云端 → 设备）

| 命令 | 功能 |
|------|------|
| `AT+PRO:switchStatus1,<0/1>` | 远程开关继电器 |
| `AT+PRO:switchMode,<0/1>` | 切换自动/手动 |
| `AT+PRO:switchOnTemp,<值>` | 设置开启温度 |
| `AT+PRO:switchOffTemp,<值>` | 设置关闭温度 |
| `AT+PRO:tempOffset,<值>` | 设置温度偏移 |
| `AT+PRO:highTempAlarm,<0/1>` | 高温报警开关 |
| `AT+PRO:highMaxTemp,<值>` | 设置高温阈值 |
| `AT+PRO:lowTempAlarm,<0/1>` | 低温报警开关 |
| `AT+PRO:lowMinTemp,<值>` | 设置低温阈值 |
| `AT+PRO:tempAlarmBuf,<值>` | 设置报警回差 |
| `AT+SER:upDeviPro` | 上传全部配置参数 |
| `AT+SER:upNewPro` | 上传实时数据（温度+状态） |
| `AT+SER:eliminateAlarm` | 远程消除报警 |
| `AT+SER:mqtt,<0/1>` | MQTT连接状态通知 |

### 8. 电源管理

- PB5检测主电源通断（250周期消抖）
- 断电时上报事件，切换备用电池
- 约15秒后关断电池（延时策略）
- MQTT连接成功（`AT+SER:mqtt,1`）才开启电池供电

### 9. 系统稳定性

- IWDG看门狗，约25.6s超时自动复位
- 参数变化后自动保存到Flash
- 串口环形缓冲区抗数据溢出
- 传感器故障时自动切断温度控制和报警

---

## 开发环境

| 项目 | 说明 |
|------|------|
| MCU | STM32F030C8 |
| IDE | Keil MDK μVision |
| 编译器 | ARMCC V5.06 |
| 库 | STM32F0 标准外设库 |

---

## 版本历史

| 日期 | 版本 | 说明 |
|------|------|------|
| 2026-05-09 | v1.02 | 改为NTC热敏电阻+ADC，新增 `ntc.c/h` |
| 2025-08-04 | v1.0 | 初始版本（DS18B20传感器） |
