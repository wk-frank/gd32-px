# GD32H757ZMT6 商业飞控参考硬件选型与原理图设计规范（`gd32_h757-fc-v1`）

> **设计定位**：对标 Pixhawk FMUv6X 核心架构的商业级四旋翼/多旋翼飞控参考硬件方案。  
> **安全声明**：本文件为参考选型与原理图设计规范；实际 PCB 打样与首次上电前，必须对照 [GD32H757xx Datasheet](https://www.endrich.com/Datenbl%C3%A4tter/Aktive%20Komponenten/GD32H757.pdf) 逐脚核对 LQFP144 复用功能（AF）与电源引脚，严禁未核对直接通电或带桨测试。

## 1. 核心计算与电源/时钟树

- **主控 MCU**：**GD32H757ZMT6**（LQFP144，Arm Cortex-M7 @ 600 MHz，3840 KB Flash，1024 KB SRAM，64 KB L1 I/D-Cache，3× CAN-FD）。
- **主时钟源**：
  - `HXTAL`：**25.000 MHz** 温补/高精度无源晶振（配 12–18 pF 负载电容，`OSC_IN`/`OSC_OUT`），经 PLL0 倍频至 **600 MHz SYSCLK**、**300 MHz AHB**、**150 MHz APB1/APB2**；PLL1/PLL2 生成 48 MHz（USB/SDIO）及专用 SPI/CAN 时钟。
  - `LXTAL`：**32.768 kHz** 晶振，供 RTC 与备份域时间保持。
- **电源树（遵循 [AN111](https://www.gd32mcu.com/download/down/document_id/455/path_type/1) 安全规范）**：
  - **PMU 供电模式**：采用 **内部 LDO 模式**（3.3 V 输入 -> 内部 LDO 输出 0.9 V `VCORE`），作为首版最稳妥方案，规避 SMPS 误配置烧毁芯片风险。
  - **三路隔离 LDO**：
    1. `VDD_MCU`（3.3 V / 500 mA）：专供 MCU 数字域与 IO；
    2. `VDD_ANA_IMU`（3.3 V 超低噪声 LDO，如 TPS7A20 / LP5907）：专供 IMU1/IMU2、气压计与 `VDDA/VREF+`；
    3. `VDD_5V_PERIPH`（5 V / 2 A 同步降压 + 限流开关）：供 GPS、TELEM1/2、RC 接收机与 CAN 收发器，并带独立 `nVDD_5V_PERIPH_EN` 使能与过流检测。

## 2. 传感器与存储选型（100% 复用 PX4 v1.17.0 原生驱动）

| 子系统 | 选型型号 | 接口总线 | 选型依据 |
| :--- | :--- | :--- | :--- |
| **主 IMU（减震/高频）** | **TDK ICM-42688-P** | `SPI0` + DMA + DRDY EXTI | PX4 v1.17.0 主力低噪声 6 轴 IMU，支持最高 8 kHz 采样与外部时钟同步 |
| **冗余 IMU** | **Bosch BMI088** | `SPI1` + DMA + 双 CS/DRDY | 加速度计/陀螺仪物理分离，抗高频振动能力强，与 ICM-42688-P 形成异构双冗余 |
| **IMU 恒温加热** | 贴片功率电阻 + N-MOSFET | `TIMER2_CH1` PWM | 稳定传感器零偏，缩短冷启动温度漂移 |
| **气压计** | **Bosch BMP390** | `I2C0` | 高精度低漂移，PX4 v1.17.0 原生支持且配合 `SENS_BAR_AUTOCAL` |
| **板载/外置磁力计** | **Bosch BMM150**（板载）+ 外置 **IST8310/BMM350** | `I2C0`（板载）/ `I2C1`（外置 GPS 口） | 兼容 PX4 v1.17.0 原生磁力计驱动与校准流 |
| **非易失参数存储** | **Infineon FM25V02A**（256 Kbit FRAM） | `SPI3` | 零擦写延迟、10^14 次耐久，保障 PX4 参数/航点掉电不丢失 |
| **黑匣子日志存储** | MicroSD 卡槽 | `SDIO0`（4-bit）+ CD 引脚 | 支持 ULog 高速日志记录，缓冲区置于 AXI SRAM |

## 3. 执行器、通信与安全电路

- **电机输出（8 路 PWM / 双向 DShot600）**：
  - `M1–M4`：`TIMER0` CH1–CH4（支持高级定时器互补/DMA 突发，适配 DShot600）；
  - `M5–M8`：`TIMER7` CH1–CH4（可扩展八旋翼/舵机）。
  - 信号链：MCU -> `74LVC244` / 电平缓冲与 33 Ω 阻尼电阻 -> ESD 保护，**上电默认下拉禁止输出**。
- **串口规划（6 路全功能覆盖）**：
  - `USART0`（`PA9`/`PA10`）：**NSH Debug Console**（115200 8N1）；
  - `USART1`（`PA2`/`PA3` + CTS/RTS）：**TELEM1**（MAVLink 2 数传）；
  - `USART2`：**TELEM2**（机载伴飞计算机 / uXRCE-DDS）；
  - `UART3`：**GPS1**（u-blox M9N/F9P）；
  - `UART4`：**RC_IN**（CRSF/GHST/SBUS，带可控硬件反相/单线切换）；
  - `USART5`：**ESC Telemetry**（电调电流/转速/温度回传）。
- **双路 CAN-FD（DroneCAN）**：`CAN0`、`CAN1` 配 **TCAN1044V** 收发器（3.3 V IO 兼容，支持 5 Mbps CAN-FD 及 120 Ω 可选端接）。
- **USB Type-C**：`USBFS/HS` OTG（`PA11`/`PA12` FS 作为 QGC MAVLink CDC-ACM 与 Bootloader 烧录口）+ `VBUS` 分压检测。
- **电源监控**：`I2C2` 接 **TI INA226 / INA238** 数字功率计 + `ADC0` 模拟 VBAT/IBAT 备份采样。
- **调试与启动**：标准 10-pin Cortex-M SWD 接口（`PA13` SWDIO、`PA14` SWCLK、`NRST`），`BOOT0` 下拉 10 kΩ 并引出按键以支持出厂系统 Bootloader 救砖。
