# GD32-PX (`gd32_h757-fc-v1`) — 基于 GD32H757ZMT6 + Apache NuttX + PX4 v1.17.0 的商业飞控工程

> **目标仓库**：`https://github.com/wk-frank/gd32-px`  
> **软件基线**：PX4-Autopilot `v1.17.0` (`d6f12ad1c4f7`) + NuttX 子模块 (`fb2fadf6f599`) + `GD32-MCU-IOT/gd32_nuttx` (`a71c8d5`) 最小可审计移植层  
> **硬件基线**：`gd32_h757-fc-v1`（GigaDevice **GD32H757ZMT6** LQFP-144，Cortex-M7 @ 600 MHz，3840 KB Flash，1024 KB SRAM，零引脚冲突商业级硬件方案）

## 1. 核心特性与全自主硬件选型

- **100% 保留 PX4 v1.17.0 原生飞控算法与中间件**：完整保留 EKF2、姿态/角速度/位置控制、Control Allocation、Commander、Navigator、uORB、WorkQueue、参数系统、MAVLink 2 与 QGroundControl 兼容性。
- **全自主商业硬件配置（零冲突 LQFP144 映射）**：
  - **主控与存储**：GD32H757ZMT6（128 KB Bootloader + 3712 KB App Flash；512 KB AXI SRAM `0x24000000` + 512 KB ITCM/DTCM/AXI + 32 KB AHB SRAM0/1 + 4 KB Backup SRAM）。
  - **电源与时钟**：25.000 MHz HXTAL + 32.768 kHz LXTAL；遵循 GigaDevice AN111 锁定**片内 LDO 供电模式**（防 SMPS 误置位烧片），配 LM66200 理想二极管双电源合路、TLV75733 数字稳压、TPS7A2033（`PD9` 可控硬复位）超低噪声传感器稳压与 TPS2553（`PD7/PD8`）1.5A 外设限流开关。
  - **异构双冗余 IMU 与环境传感**：`SPI0` 独占 **TDK ICM-42688-P**（24 MHz DMA + `PC4` DRDY），`SPI1` 独占 **Bosch BMI088**（10 MHz DMA + 双片选/双 DRDY），配 `PB4`（`TIMER2_CH0`）恒温加热槽；`I2C0` 挂载 **BMP390** 气压计 + **BMM150** 磁力计；`I2C1` 外扩机臂罗盘 **IST8310/BMM350**；`I2C3` 挂载 **INA226** 数字功率计。
  - **零延迟参数与高速黑匣子**：`SPI3` 独占 **Infineon FM25V02A-GTR**（32 KB FRAM）存参数/航点；`SDIO0` 4-bit（`PC8..12, PD2` + IP4252CZ16）配自锁 MicroSD 记录 ULog。
  - **8 路硬件门控电机输出与双 CAN-FD**：`TIMER0`（M1–M4: `PE9/11/13/14`）+ `TIMER3`（M5–M8: `PD12..15`）经 **SN74LVC8T245** 输出，`PE10`（10 kΩ 硬件上拉）确保复位与未解锁阶段物理禁止输出；`CAN0`（`PD0/PD1`）+ `CAN2`（`PF6/PF7`）配双路 **TCAN1044V-Q1**（5 Mbps CAN-FD）；内置高速 PHY 专用 `USBHS0_DM/DP` 直连 Type-C；`UART7` 配 **SN74LVC1G86**（`PG3` 控制）硬件兼容 CRSF/ELRS 与反相 SBUS。

## 2. 仓库目录结构

```text
.
├── docs/
│   ├── architecture/ADR-0001.md          # 架构决策记录
│   ├── bringup/{M0.md,M1.md}             # M0/M1 阶段报告与验收记录
│   ├── hardware/
│   │   ├── reference-schematic.md        # 6 张原理图分图详细设计规范
│   │   ├── pin-matrix.csv                # 60 信号 LQFP144 零冲突引脚映射表
│   │   ├── bom.csv                       # 32 项完整量产级物料清单 (BOM)
│   │   └── unknowns.md                   # 投板签核项说明
│   ├── licenses/THIRD-PARTY.md           # 商业闭源合规与 SBOM 清单
│   ├── porting-gap.md                    # 对照 gd32_nuttx 源码核实的差距矩阵
│   └── validation/results/               # 实测检查与单元测试日志
├── hardware/schematic/
│   ├── gd32_h757_fc_v1.kicad_sch         # KiCad 7/8 原理图工程文件
│   └── gd32_h757_fc_v1.net               # 标准 KiCad S-expression 网表
├── nuttx/
│   ├── arch/arm/include/gd32h73x_75x/    # 已核实的 217 向量 IRQ 定义
│   ├── arch/arm/src/gd32h73x_75x/        # Kconfig、MemoryMap、PinMap、Cache/MPU、PMU 安全锁
│   └── boards/arm/gd32h73x_75x/gd32h757-fc-v1/  # NuttX board.h、flash.ld、defconfig、gd32_boot.c
├── px4/boards/gd32/h757-fc-v1/           # PX4 v1.17.0 板级配置、8路PWM/DShot映射与启动脚本
├── scripts/
│   ├── check_env.sh                      # 交叉编译工具链环境自检
│   ├── verify_design.py                  # 硬件 DRC 零冲突与跨层一致性自动校验器
│   ├── check_m1_static.sh                # 主机侧 C11 严格编译、单元测试与全量校验入口
│   └── push_to_github.sh                 # 一键推送至 https://github.com/wk-frank/gd32-px
└── tests/host/test_cache_mpu.c           # Cache/DMA、PMU 防呆、时钟树与 PWM 映射单元测试
```

## 3. 快速验证与推送命令

```bash
# 1. 运行硬件引脚零冲突 DRC、KiCad 网表校验与 C11 主机单元测试
bash scripts/check_m1_static.sh

# 2. 在具备外网权限的终端一键推送全部分支至 GitHub
bash scripts/push_to_github.sh
```
