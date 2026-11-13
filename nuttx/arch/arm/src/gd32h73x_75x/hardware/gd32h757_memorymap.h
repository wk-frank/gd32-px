/****************************************************************************
 * arch/arm/src/gd32h73x_75x/hardware/gd32h757_memorymap.h
 *
 * GD32H757ZMT6 存储映射定义（M1 骨架）
 * 来源：GD32H757xx Datasheet / AN111，严禁照搬 STM32H7 定义。
 * 未经 User Manual 逐项核实的寄存器偏移均标注 TODO。
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H
#define __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H

#include <stdint.h>

/* 已核实片上存储基址与容量（GD32H757ZMT6） */

#define GD32_FLASH_BASE         0x08000000UL  /* 3840 KB On-chip Flash */
#define GD32_FLASH_SIZE         (3840UL * 1024UL)

#define GD32_ITCM_BASE          0x00000000UL  /* 可配置 ITCM（与 DTCM/AXI 共享 512 KB） */
#define GD32_DTCM_BASE          0x20000000UL  /* 可配置 DTCM（CPU 零等待，注意通用 DMA 不可达，仅 MDMA 可达） */
#define GD32_AXISRAM_BASE       0x24000000UL  /* 512 KB 固定 AXI SRAM（0x24000000 - 0x2407FFFF，DMA 可达） */
#define GD32_AXISRAM_SIZE       (512UL * 1024UL)

#define GD32_BKPSRAM_BASE       0x38800000UL  /* TODO: 对照 GD32H737/757/759 User Manual 核实 Backup SRAM 精确基址 */
#define GD32_BKPSRAM_SIZE       (4UL * 1024UL)

/* 外设总线基址（TODO: 对照 User Manual Memory Map 章节逐项核实） */

#define GD32_PERIPH_BASE        0x40000000UL
#define GD32_APB1_BASE          (GD32_PERIPH_BASE + 0x00000000UL) /* TODO: 核实 */
#define GD32_APB2_BASE          (GD32_PERIPH_BASE + 0x00010000UL) /* TODO: 核实 */
#define GD32_AHB1_BASE          (GD32_PERIPH_BASE + 0x00020000UL) /* TODO: 核实 */
#define GD32_AHB3_BASE          0x52000000UL                      /* TODO: 核实 */
#define GD32_APB4_BASE          0x58000000UL                      /* TODO: 核实 */

/* Cortex-M7 Cache 行大小 */

#define GD32_DCACHE_LINESIZE    32U
#define GD32_ICACHE_LINESIZE    32U

#endif /* __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H */
