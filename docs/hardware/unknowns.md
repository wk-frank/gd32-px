# 硬件未知项与签核状态清单（`docs/hardware/unknowns.md`）

> 状态更新（2026-11-16）：全自主商业飞控参考硬件 `gd32_h757-fc-v1` 已完成全套器件选型、LQFP144 零冲突引脚分配、6 页原理图定义与 KiCad 网表生成。此前遗留的 `SDIO0`/`CAN1`/`VBUS`/`PI5..6` 冲突已全部从架构上根除。

## 1. 已冻结并完成交叉核实的硬件参数

| 类别 | 冻结方案 | 核实依据 |
| :--- | :--- | :--- |
| 主控与封装 | **GD32H757ZMT6**（LQFP144，仅使用 `PA0..PG15`、`PH0..PH1` 与专用 `USBHS0_DM/DP`） | [GD32H757 Datasheet](https://uploadcdn.oneyac.com/attachments/files/brand_pdf/gigadevice/43/39/GD32H757VMJ6.pdf) |
| 时钟与电源 | 25.000 MHz `HXTAL` + 32.768 kHz `LXTAL`；PMU 内部 LDO 模式（0.9V VCORE）+ 三路隔离稳压 | [GigaDevice AN111](https://www.gd32mcu.com/download/down/document_id/455/path_type/1) & `gd32h759im-eval/board.h` |
| 历史冲突消解 | ① M5–M8 改用 `TIMER3` (`PD12..15` AF2)，避开 `SDIO0` (`PC8..12`) 与不存在的 `PI` 端口；② 第二路 CAN 改用 `CAN2` (`PF6/PF7` AF2)，避开 `I2C0`/`SPI1`；③ USB 改用专用 `USBHS0_DM/DP` + `PA8` VBUS 检测，避开 `PA9` Console；④ 功率计改用 `I2C3` (`PF14/PF15` AF4)，避开 `PC9` | [gd32_nuttx pinmap](https://github.com/GD32-MCU-IOT/gd32_nuttx/blob/master/arch/arm/src/gd32h73x_75x/hardware/gd32h759_pinmap.h) + Datasheet |

## 2. 投板打样与首次上电前必做签核项（非软件阻塞项）

1. **PCB 封装与电源引脚逐一核对**：EDA 绘制 PCB 封装时，须对照 `GD32H757xx Datasheet` Table `GD32H757Zx LQFP144 pin definitions` 核对全部 `VDD`、`VSS`、`VDDA`、`VSSA`、`VREF+`、`VCAP`、`VUSBHS`、`PDR_ON`（Pin 143）物理引脚号。
2. **DMAMUX 请求通道号实测确认**：在取得 `GD32H737/757/759 User Manual` DMA/DMAMUX 章节后，将 `SPI0/1/3`、`USART1/2/5`、`UART6/7`、`TIMER0/3`（DShot）对应的精确 DMAMUX 请求 ID 填入 M2 DMA 映射表。
3. **正式商业产品 ID 申请**：`firmware.prototype` 当前使用私有开发测试 ID `65501`，正式商业出货前须向 [PX4-Bootloader](https://docs.px4.io/main/zh/hardware/board_support_guide) 注册或使用自有闭源 QGC 定制地面站 ID 体系，以及合规 USB `VID/PID`。
