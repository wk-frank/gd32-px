# GD32H757ZMT6 × PX4 v1.17.0 / NuttX 移植差距分析（M0）

## 1. 基线与外部仓库核查结论

| 维度 | 核查对象 | 核查结果 | 证据/来源 |
| :--- | :--- | :--- | :--- |
| PX4 软件基线 | `PX4/PX4-Autopilot` `v1.17.0` | 稳定发布版，commit `d6f12ad1c4f7`，NuttX 子模块 `fb2fadf6f599` | [PX4 v1.17 Release Notes](https://docs.px4.io/main/en/releases/1.17) / [Issue #29346](https://github.com/PX4/PX4-Autopilot/issues/29346) |
| Apache NuttX 主线 | `apache/nuttx` | `arm` 目录下仅有 `gd32f4`、`gd32w51x`，**无 GD32H7 支持** | [NuttX ARM Platforms](https://github.com/apache/nuttx/blob/master/Documentation/platforms/arm/gd32f4/index.rst) |
| GD32 下游 NuttX | `GD32-MCU-IOT/gd32_nuttx` | **已存在 `arch/arm/src/gd32h73x_75x/`** 与 `boards/arm/gd32h73x_75x/gd32h759im-eval/`（commit `a71c8d5`，PR #46 `H7_lib`，Apache-2.0） | [GD32-MCU-IOT/gd32_nuttx](https://github.com/GD32-MCU-IOT/gd32_nuttx) |
| GD32H757ZMT6 规格 | GigaDevice 官方资料 | Cortex-M7 @ 600 MHz，3840 KB Flash，1024 KB SRAM（512 KB AXI SRAM `0x24000000` + 512 KB 可配置 ITCM/DTCM/AXI + 4 KB Backup SRAM），64 KB L1 Cache（I/D-Cache） | [GD32H757 Datasheet](https://www.endrich.com/Datenbl%C3%A4tter/Aktive%20Komponenten/GD32H757.pdf) / [GD32H7xx 产品页](https://www.gigadevice.com.cn/product/mcu/high-performance-mcus/gd32h7xx-series) |
| 本地构建环境 | 当前沙箱 VM | 仅有 `git`、`python3` 3.11.2、`gcc/g++` 12.2.0、`make`；**缺少 `arm-none-eabi-gcc`、`cmake`、`ninja`、`kconfig-mconf` 及外网克隆权限** | `scripts/check_env.sh`（M0 实测） |

## 2. 子系统差距矩阵（对比 FMUv6X / STM32H7 与 `gd32_nuttx`）

| 子系统 / 外设 | `gd32_nuttx` 现状（已核实） | PX4 v1.17.0 需求 | 差距等级 | 主要工作量与注意事项 |
| :--- | :--- | :--- | :--- | :--- |
| 启动 / 链接脚本 / 内存 | 有 `gd32h759im-eval` 参考，需核对 3840KB Flash 与 ITCM/DTCM 划分 | `px4_fmu-v6x` 具备独立 Bootloader 分区、ITCM/DTCM 分配及 CI ITCM 检查 | **P0** | 编写 GD32H757ZMT6 链接脚本，区分 AXI SRAM（DMA 可达）与 DTCM/ITCM |
| 时钟 / PMU / RCU | 需核实 `gd32h73x_75x` 内部 RCU/PMU（LDO/SMPS/Bypass） | 600 MHz 主频、USB 48MHz、SDIO/SPI/CAN-FD 独立时钟源 | **P0** | 必须按 AN111 核实 PMU 供电模式（错误配置 SMPS 有烧毁风险），核实 PLL0/1/2 |
| 中断 / NVIC / SysTick | 有 `gd32h73x_75x_irq.c` | NuttX 系统节拍 + 高精度单调时钟 `hrt` | **P1** | 核对 GD32H757 全量 IRQ 向量号，禁止复用 STM32H7 IRQ 表 |
| D-Cache / MPU / DMA | 有 `gd32h73x_75x_dma.{c,h}`（DMAMUX 待核实） | DMA 缓冲区 32-byte cache-line 对齐或 MPU Non-Cacheable 区 | **P0** | 不可全量关闭 D-Cache；需实现 DMA clean/invalidate 与 MDMA/DMA0/1 映射 |
| GPIO / EXTI / UART | 有 `gd32h73x_75x_gpio.{c,h}`；UART 待核实 | NSH Console、MAVLink、GPS、RC（CRSF/SBUS） | **P1** | 验证串口 DMA 收发、引脚 AF 复用与低延迟中断 |
| SPI（IMU 核心） | **待核实**（首轮可见文件列表未见 `spi.c`） | 高频 IMU 读取（1–8 kHz）+ DRDY EXTI + DMA | **P0** | 需补齐/审计 SPI+DMA+Cache 一致性驱动与多总线仲裁 |
| I2C / ADC / PWM / DShot | 有 `gd32h73x_75x_i2c.{c,h}`；ADC/PWM/DShot **缺失/待核实** | 磁力计/气压计 I2C、电源电压电流 ADC、4~8 路 PWM/DShot | **P0** | 基于 GD32H7 高级/通用定时器实现 PX4 `io_timers` 与 DShot DMA |
| FDCAN / SDIO / USB | **未核实 / 缺失** | DroneCAN（3 路 CAN-FD）、ULog SD 卡、USB CDC ACM | **P1** | M2 后期至 M4/M5 按产品需要逐步补齐 |
| PX4 Arch / `hrt` / Bootloader | **完全缺失**（无 `boards/gd32/`） | `px4_arch`、`hrt` 硬件定时器、`mtd`/参数存储、Bootloader | **P0** | M3 核心工作：建立 `boards/gd32/h757-fc-v1` 与 PX4-Bootloader GD32 适配 |
