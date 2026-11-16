# GD32H757ZMT6 × PX4 v1.17.0 / NuttX 移植差距分析（M0–M1 源码级核实版）

## 1. 上下游仓库核实基线

| 维度 | 核查对象 | 核查结果 | 来源证据 |
| :--- | :--- | :--- | :--- |
| PX4 软件基线 | `PX4/PX4-Autopilot` `v1.17.0` | 稳定版 tag `v1.17.0`（commit `d6f12ad1c4f7`），锁定 NuttX 子模块 `fb2fadf6f599` | [PX4 v1.17 Release Notes](https://docs.px4.io/main/en/releases/1.17) / [Issue #29346](https://github.com/PX4/PX4-Autopilot/issues/29346) |
| Apache NuttX 主线 | `apache/nuttx` | `arch/arm/src/` 仅有 `gd32f4` 与 `gd32w51x`，无 `gd32h7` 支持 | [NuttX gd32f4 文档](https://github.com/apache/nuttx/blob/master/Documentation/platforms/arm/gd32f4/index.rst) |
| GD32 下游 NuttX | `GD32-MCU-IOT/gd32_nuttx` | `master` commit `a71c8d5`（PR #46 `H7_lib`，Apache-2.0），`arch/arm/src/gd32h73x_75x/` 已有 **25 个源码文件 + 11 个 `hardware/` 头文件**，含 `gd32h759im-eval` 开发板支持 | [gd32h73x_75x 目录](https://github.com/GD32-MCU-IOT/gd32_nuttx/tree/master/arch/arm/src/gd32h73x_75x) |
| 目标硬件板 | 自研 `gd32_h757-fc-v1` | 已完成全自主商业选型、LQFP144 零冲突 60 信号原理图、BOM 与 KiCad 网表冻结 | `docs/hardware/reference-schematic.md` |

## 2. `GD32-MCU-IOT/gd32_nuttx` 逐模块审计与 PX4 差距矩阵

| 子系统 | `gd32_nuttx` 现有文件（已逐一核实） | 当前覆盖度 | 面向 PX4 v1.17.0 飞控需补齐的工作 | 里程碑 |
| :--- | :--- | :--- | :--- | :--- |
| 启动 / 堆 / 空闲 | `gd32h73x_75x_start.c`、`allocateheap.c`、`idle.c`、`syscfg.c` | **已具备**（仅针对 H759IM） | 新增 `ARCH_CHIP_GD32H757ZM` Kconfig、3840KB Flash 链接脚本与多段堆注册（AXI SRAM + DTCM） | **M1** |
| 时钟与电源管理 | `gd32h73x_75x_rcu.{c,h}`（**无 `pmu` 驱动**） | **部分具备** | 补齐 **PMU 内部 LDO 锁定保护**（防 SMPS 误置位烧片）及 PLL1/PLL2 外设独立时钟配置 | **M1** |
| 中断与系统节拍 | `gd32h73x_75x_irq.c`、`timerisr.c`、`irq.h`（`NR_IRQS=217`） | **已具备** | 复核优先级分组（NVIC BASEPRI）并基于 32-bit `TIMER1` 实现 PX4 1 MHz 单调时钟 `hrt` | **M1/M3** |
| D-Cache / MPU / DMA | `gd32h73x_75x_dma.{c,h}` | **基础具备** | 增加 AXI SRAM/SRAM0/1 DMA 边界校验、32-byte Cache-line 对齐与 SPI/SDIO/DShot DMAMUX 适配 | **M1/M2** |
| 串口与控制台 | `gd32h73x_75x_lowputc.c`、`serial.c`、`usart.h` | **已具备** | 验证 `USART0/1/2/5` + `UART6/7` 6 路并发、RX DMA 环形缓冲及 `PG3` 硬件异或门 RC 极性控制 | **M1/M2** |
| GPIO / EXTI | `gd32h73x_75x_gpio.{c,h}` | **已具备** | 补齐 LQFP144 封装边界裁剪及 IMU1/IMU2 DRDY、GPS PPS 高精度时间戳 EXTI 回调 | **M2** |
| SPI / I2C | `gd32h73x_75x_spi.{c,h}`、`i2c.{c,h}` | **已具备** | 审计 `SPI0`（24MHz ICM42688P）、`SPI1`（10MHz BMI088）、`SPI3`（FRAM）DMA 并发与总线锁，补齐 I2C 总线死锁恢复 | **M2** |
| 通用/高级定时器、PWM、DShot | **缺失**（仅有 `timerisr.c`） | **缺失（P0）** | 编写 GD32H7 `TIMER0` + `TIMER3` 驱动以对接 PX4 `io_timers` / `pwm_out` / 双向 `dshot` DMA | **M2** |
| ADC / SDIO / CAN-FD / USBHS | **缺失**（`gd32_nuttx` 尚未提交对应 C 文件） | **缺失（P0/P1）** | 编写 `ADC0/1`（电池监控）、`SDIO0`（IDMA ULog）、`CAN0/CAN2`（SocketCAN/DroneCAN）、内置 PHY `USBHS0` CDC-ACM 驱动 | **M2/M4** |
| PX4 Arch 与板级层 | **缺失** | **本轮已建骨架** | 在 `platforms/nuttx/src/px4/gd/gd32h7` 实现 `hrt`/`io_timers`/`board_reset` 并联调 `boards/gd32/h757-fc-v1` | **M3** |
