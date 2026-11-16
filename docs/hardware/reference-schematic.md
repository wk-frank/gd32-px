# GD32H757ZMT6 商业闭源飞控硬件选型与原理图设计规范（`gd32_h757-fc-v1` Rev 1.0）

> **文档等级**：硬件基线冻结规范（6 张原理图分图 + 零冲突 LQFP144 引脚表 + 完整 BOM + KiCad 网表）  
> **基准核实**：所有端口范围（`PA0..PG15`, `PH0..PH1`）、专用 `USBHS0_DM/DP` 引脚、外设基址与复用功能（`AF0`–`AF12`）均已对照 [GD32H757xx Datasheet](https://uploadcdn.oneyac.com/attachments/files/brand_pdf/gigadevice/43/39/GD32H757VMJ6.pdf)、[GigaDevice AN111](https://www.gd32mcu.com/download/down/document_id/455/path_type/1) 及 [GD32-MCU-IOT/gd32_nuttx](https://github.com/GD32-MCU-IOT/gd32_nuttx) 交叉核实，并通过 `scripts/verify_design.py` 自动 DRC 零冲突检查。

## 1. 总体硬件架构与核心选型理由

1. **计算核心（U1：GD32H757ZMT6，LQFP-144）**：
   - Arm Cortex-M7 @ **600 MHz**，内置双精度 FPU、64 KB L1 Cache（32 KB I-Cache + 32 KB D-Cache）、16 区 MPU。
   - **3840 KB 片上 Flash**（前 128 KB 分配给独立 Bootloader，后 3712 KB 分配给 PX4 飞控固件与商业私有模块，远超 STM32H743/753 的 2048 KB 容量限制）。
   - **1024 KB 分层 SRAM**：512 KB AXI SRAM（`0x24000000`，通用 DMA 主力内存区）+ 512 KB 可配置 ITCM/DTCM/AXI（零等待控制环与中断栈）+ 32 KB AHB SRAM0/1（`0x30000000`）+ 4 KB Backup SRAM（`0x38800000`，保存 Crash Dump 与热启动状态）。
2. **异构双冗余 IMU + 全原生传感器链**：
   - **主 IMU（U_IMU1：TDK ICM-42688-P）**：独占 `SPI0`（24 MHz DMA）+ `PC4` EXTI4 DRDY，低噪声、低温漂，置于减震区中心。
   - **冗余 IMU（U_IMU2：Bosch BMI088）**：独占 `SPI1`（10 MHz DMA）+ 双片选（`PB12`/`PC0`）+ 双 DRDY（`PD10`/`PC5`），机械抗振抑制能力优异，与 ICM-42688-P 构成异构冗余。
   - **气压与磁力计**：板载 `I2C0`（`PB6/PB7`）挂载 **BMP390**（`0x76`）与 **BMM150**（`0x10`）；外置 `I2C1`（`PF1/PF0`）经 `J_GPS1` 接入机臂外置罗盘（`IST8310` / `BMM350`）。
   - **参数与日志双保险**：`SPI3`（`PE2/PE4/PE5/PE6`）独占挂载 **FM25V02A-GTR**（32 KB 铁电存储 FRAM），彻底消除 Flash 擦写停顿对控制周期的干扰；`SDIO0` 4-bit（`PC8..PC12, PD2`）连接自锁 MicroSD 卡槽记录高频 ULog。

## 2. 六张原理图分图详细设计（Sheet 1 ~ Sheet 6）

### Sheet 1：三级隔离电源树与 PMU 安全电路
- **双路 5V 理想二极管合路**：`J_PWR`（5.2V BEC）与 `J_USB`（Type-C VBUS）经 **LM66200DRLR**（`U_OR`）无损合路为 `VDD_5V_MAIN`，防止调试时 USB 倒灌电调/电池模块；`PA8` 通过 `10kΩ / 20kΩ` 分压独立检测 `USB_VBUS_SENSE`（不占用 `PA9` 调试串口）。
- **三路隔离稳压输出**：
  - `VDD_3V3_MCU`：**TLV75733PDBVR**（1 A）供 MCU `VDD`/`VUSBHS`/FRAM/CAN VIO；经磁珠 `FB1 (BLM18PG121SN1)` + `4.7 µF + 100 nF` 滤波后供给 `VDDA`/`VREF+`。
  - `VDD_3V3_SENSORS`：**TPS7A2033PDBVR**（6.5 µVRMS 超低噪声，300 mA），`EN` 脚受 `PD9`（`VDD_3V3_SENSORS_EN`）控制，支持传感器总线锁死时由软件断电硬复位。
  - `VDD_5V_PERIPH`：**TPS2553DBVR**（限流阈值设定为 1.5 A），受 `PD7` 使能控制，`FAULTn` 接 `PD8`（`VDD_5V_PERIPH_OCn`），外部 GPS/数传短路时瞬时限流保护飞控核心。
- **PMU 0.9V VCORE 硬件防呆**：严格遵循 [AN111](https://www.gd32mcu.com/download/down/document_id/455/path_type/1)，`VCAP` 引脚配置 `2 × 2.2 µF (0603 X7R)` 去耦电容，使用**片内 LDO 模式**，并在 `gd32h757_pmu_safety.c` 启动早期锁定 LDO 位，严禁置位 SMPS 位。

### Sheet 2：MCU 最小系统、时钟树、SWD 与专用 USBHS0
- **时钟网络**：
  - `Y1`：**25.000 MHz**（NDK `NX3225GA-25.000M`，±10 ppm）接 `PH0 (OSC_IN)` / `PH1 (OSC_OUT)`，配 `2 × 15 pF` C0G 电容；PLL0 配置 `PSC=5, N=120, P=1` 输出 **600 MHz**。
  - `Y2`：**32.768 kHz**（Abracon `ABS07`）接 `PC14` / `PC15`，配 `2 × 6.8 pF` C0G 电容；`VBAT` 经双肖特基 `BAT54C` 接 `VDD_3V3_MCU` 与板载备份电容。
- **USB 2.0 接口（关键硬件差异修正）**：根据 [GD32H757 Datasheet](https://uploadcdn.oneyac.com/attachments/files/brand_pdf/gigadevice/43/39/GD32H757VMJ6.pdf) 注释 `(3) USBHS0_DM and USBHS0_DP pins can only be used for USBHS`，直接将内置 PHY 专用引脚 `USBHS0_DM` / `USBHS0_DP` 经 **USBLC6-2SC6** ESD 保护接至 Type-C 母座（而非照搬 STM32H7 的 `PA11/PA12`！），Type-C `CC1`/`CC2` 各接 `5.1 kΩ` 下拉至 GND。
- **启动与调试**：`BOOT0` 串 `10 kΩ` 下拉至 GND 并引出 DFU 按键至 `3.3V`；`J_DBG`（SH-6P）引出 `PA9/PA10`（`USART0` NSH Console）与 `PA13/PA14`（SWD）。

### Sheet 3：异构双 IMU、恒温加热与环境传感器
- `U_IMU1`（ICM-42688-P）接 `SPI0`（`PA4` CS、`PA5` SCK、`PA6` MISO、`PA7` MOSI、`PC4` DRDY），电源引脚就近放置 `100 nF + 2.2 µF` X7R。
- `U_IMU2`（BMI088）接 `SPI1`（`PB12` ACC_CS、`PC0` GYR_CS、`PB13` SCK、`PB14` MISO、`PB15` MOSI、`PD10` ACC_DRDY、`PC5` GYR_DRDY）。
- **恒温槽电路**：`PB4`（`TIMER2_CH0`）驱动 `Q1`（AO3400A，栅极 `100 kΩ` 下拉保证复位默认关闭加热），漏极接 4 颗并联 `120 Ω`（等效 `30 Ω`）贴片电阻环绕双 IMU 布局。
- `U_BARO`（BMP390）与 `U_MAG`（BMM150）挂载于 `I2C0`（`PB6` SCL、`PB7` SDA，`2.2 kΩ` 上拉至 `VDD_3V3_SENSORS`）。

### Sheet 4：FRAM 铁电存储与 MicroSD 高速日志接口
- `U_FRAM`（FM25V02A-GTR）接 `SPI3`（`PE2` SCK、`PE4` CSn、`PE5` MISO、`PE6` MOSI），`WPn` 与 `HOLDn` 接 `10 kΩ` 上拉至 `VDD_3V3_MCU`。
- `J_SD`（Molex `502570-0893`）接 `SDIO0`（`PC8..PC11` D0..D3、`PC12` CK 串 `22 Ω` 阻尼匹配、`PD2` CMD、`PC13` CDn），总线经 **IP4252CZ16** 完成 EMI 滤波、ESD 防护与上拉，独立 `47 µF + 100 nF` 储能电容抑制写卡电流尖峰。

### Sheet 5：8 路硬件门控 PWM/DShot 与双路车规 CAN-FD
- **电机安全门控输出**：
  - M1–M4 由高级定时器 `TIMER0`（`PE9, PE11, PE13, PE14` AF1）产生，支持互补/DMAMUX 双向 DShot600；
  - M5–M8 由通用定时器 `TIMER3`（`PD12, PD13, PD14, PD15` AF2）产生，彻底避开 `SDIO0`（`PC8..12`）引脚！
  - 8 路信号统一进入 **SN74LVC8T245PWR**（`U_BUF1`），其 `OEn` 由 `PE10` 控制并接 **`10 kΩ` 硬件上拉至 3.3V**，输出端各串 `33 Ω` 电阻并接 `47 kΩ` 下拉电阻 + **USBLC6-4SC6** ESD 保护，确保 MCU 复位、Bootloader 驻留或 HardFault 时电机信号物理强制为低电平。
- **双路 CAN-FD**：
  - `CAN0`（`PD0/PD1` AF9，`PD3` Silent）与 `CAN2`（`PF6/PF7` AF2，`PD4` Silent）彻底避开 `I2C0` 与 `SPI1` 引脚冲突，各配一颗 **TCAN1044VDRBRQ1**（3.3V VIO 直连 MCU，5V 驱动总线，带 `2 × 60.4 Ω + 4.7 nF` 分裂端接）。

### Sheet 6：6 路串口、硬件可编程 RC 反相器与人机交互
- **RC 全协议自适应电路**：外部 RC 输入经 `1 kΩ` 限流保护进入异或门 **SN74LVC1G86DBVR**（`U_XOR`）输入 A，输入 B 接 `PG3`（`SBUS_INV_SEL`），输出接 `PE0`（`UART7_RX`）；无需依赖芯片内部 UART 极性反转位即可同时支持 SBUS（反相 100 kbps）与 CRSF/ELRS/GHST（正相 420 kbps+）。
- **通信与模拟监控连接器**：`TELEM1`（`PA0..PA3` 带流控）、`TELEM2`（`PB10/PB11`）、`GPS1`（`PE7/PE8` + `PF0/PF1` + `PG2` PPS + `PC2/PC3` 安全开关）、`ESC_TLM`（`PC6/PC7`）、`POWER1`（`PF14/PF15` `I2C3` + `PF11` `VBAT_ADC` + `PF12` `IBAT_ADC` + `PF13` `5V_ADC`）、RGB 状态灯（`PE3/PE12/PE15`）及蜂鸣器（`PB0` `TIMER2_CH2`）。
