# 硬件未知项与待核实清单（`docs/hardware/unknowns.md`）

> 状态更新（2026-11-13）：已采用《参考硬件选型与原理图设计规范》（`docs/hardware/reference-schematic.md`）固化全套器件选型与总线规划。当前仅余以下 **打样/实物核对项**，在完成核对前禁止上电烧录。

## 1. 已在参考方案中固化的选型

- **MCU 与供电**：GD32H757ZMT6（LQFP144），内部 LDO 模式（3.3 V -> 0.9 V VCORE），25 MHz HXTAL + 32.768 kHz LXTAL，目标 600 MHz。
- **传感器**：ICM-42688-P（`SPI0`）+ BMI088（`SPI1`）双冗余 IMU、BMP390（`I2C0`）气压计、BMM150（`I2C0`）磁力计、FM25V02A（`SPI3`）FRAM、INA226（`I2C2`）功率计。
- **执行器与接口**：8 路 PWM/DShot（`TIMER0` + `TIMER7`）、6 路 UART、双路 CAN-FD（`CAN0`/`CAN1`）、`SDIO0`、`USBFS` OTG。

## 2. 仍需在原理图/PCB 冻结前核实的引脚冲突与 AF 项（P0）

1. **SDIO0 与 UART3/UART4 引脚冲突消解**：`PC10/PC11/PC12/PD2` 在 LQFP144 上同时复用为 `SDIO0_D2/D3/CLK/CMD` 与 `UART3/UART4`；实际原理图冻结时须将 `GPS1` / `RC_IN` 重映射至不冲突的 `UART6`（`PE7/PE8`）与 `UART7`（`PE0/PE1`）或 `USART2/5` 备用脚。
2. **CAN1 与 I2C0 引脚冲突消解**：`PB5/PB6` 复用需避开 `I2C0`（`PB6/PB7`），建议将 `I2C0` 移至备用脚或 `CAN1` 使用 `PB12/PB13`（若与 SPI1 冲突则改用 `CAN2`）。
3. **VBUS 检测引脚**：`PA9` 已分配给 `USART0_TX`（NSH Console），USB `VBUS` 检测须改用独立 GPIO（如 `PC0` 或分压电阻接 `PF2`），避免与串口冲突。
4. **AF 复用编号最终核对**：`pin-matrix.csv` 中所有 `AFx` 须在获取 `GD32H757xx Datasheet` 完整 Pin Definitions 表格后逐脚签名确认。
