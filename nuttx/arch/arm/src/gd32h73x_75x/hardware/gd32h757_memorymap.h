/****************************************************************************
 * arch/arm/src/gd32h73x_75x/hardware/gd32h757_memorymap.h
 *
 * GD32H757ZMT6 片上存储与总线外设基址定义
 * 核实来源：GD32-MCU-IOT/gd32_nuttx (gd32h759im_memorymap.h) + GD32H757 Datasheet
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H
#define __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H

#include <stdint.h>

/* 1. 片上 Flash 与分层 SRAM 基址及容量（GD32H757ZMT6） */

#define GD32_ITCMSRAM_BASE      0x00000000UL  /* 可配置共享 TCM（默认 64KB~128KB） */
#define GD32_FLASH_BASE         0x08000000UL  /* 3840 KB 片上主 Flash */
#define GD32_FLASH_SIZE         (3840UL * 1024UL)
#define GD32_BL_FLASH_SIZE      (128UL * 1024UL)
#define GD32_APP_FLASH_BASE     (GD32_FLASH_BASE + GD32_BL_FLASH_SIZE)
#define GD32_APP_FLASH_SIZE     (GD32_FLASH_SIZE - GD32_BL_FLASH_SIZE)

#define GD32_DTCMSRAM_BASE      0x20000000UL  /* CPU 零等待 DTCM（通用 DMA0/1 不可达，仅 MDMA 可达） */
#define GD32_DTCM_BASE          GD32_DTCMSRAM_BASE
#define GD32_ITCM_BASE          GD32_ITCMSRAM_BASE

#define GD32_AXISRAM_BASE       0x24000000UL  /* 512 KB 固定 AXI SRAM（通用 DMA 主力区） */
#define GD32_AXISRAM_SIZE       (512UL * 1024UL)

#define GD32_SRAM0_BASE         0x30000000UL  /* 16 KB AHB SRAM0（DMA 可达） */
#define GD32_SRAM0_SIZE         (16UL * 1024UL)
#define GD32_SRAM1_BASE         0x30004000UL  /* 16 KB AHB SRAM1（DMA 可达） */
#define GD32_SRAM1_SIZE         (16UL * 1024UL)

#define GD32_BKPSRAM_BASE       0x38800000UL  /* 4 KB Backup SRAM（Crash Dump / 热启动区） */
#define GD32_BKPSRAM_SIZE       (4UL * 1024UL)

/* 2. AHB / APB 总线基址（已对照 gd32h759im_memorymap.h 核实） */

#define GD32_APB1_BUS_BASE      0x40000000UL
#define GD32_APB2_BUS_BASE      0x40010000UL
#define GD32_AHB1_BUS_BASE      0x40020000UL
#define GD32_AHB2_BUS_BASE      0x48000000UL
#define GD32_APB3_BUS_BASE      0x50000000UL
#define GD32_AHB3_BUS_BASE      0x51000000UL
#define GD32_APB4_BUS_BASE      0x58000000UL
#define GD32_AHB4_BUS_BASE      0x58020000UL

/* 3. 飞控关键外设组基址（已对照 gd32_nuttx 与 Datasheet 核实） */

#define GD32_TIMER_BASE         (GD32_APB1_BUS_BASE + 0x00000000UL)
#define GD32_TIMER1_BASE        (GD32_APB1_BUS_BASE + 0x00000000UL) /* 32-bit HRT */
#define GD32_TIMER2_BASE        (GD32_APB1_BUS_BASE + 0x00000400UL) /* Heater + Buzzer */
#define GD32_TIMER3_BASE        (GD32_APB1_BUS_BASE + 0x00000800UL) /* Motor 5..8 */
#define GD32_SPI1_BASE          (GD32_APB1_BUS_BASE + 0x00003800UL) /* BMI088 */
#define GD32_USART1_BASE        (GD32_APB1_BUS_BASE + 0x00004400UL) /* TELEM1 */
#define GD32_USART2_BASE        (GD32_APB1_BUS_BASE + 0x00004800UL) /* TELEM2 */
#define GD32_UART3_BASE         (GD32_APB1_BUS_BASE + 0x00004C00UL)
#define GD32_UART4_BASE         (GD32_APB1_BUS_BASE + 0x00005000UL)
#define GD32_I2C0_BASE          (GD32_APB1_BUS_BASE + 0x00005400UL) /* BMP390 + BMM150 */
#define GD32_I2C1_BASE          (GD32_APB1_BUS_BASE + 0x00005800UL) /* Ext Compass */
#define GD32_I2C2_BASE          (GD32_APB1_BUS_BASE + 0x00005C00UL)
#define GD32_I2C3_BASE          (GD32_APB1_BUS_BASE + 0x00006000UL) /* INA226 Power */
#define GD32_UART6_BASE         (GD32_APB1_BUS_BASE + 0x00007800UL) /* GPS1 */
#define GD32_UART7_BASE         (GD32_APB1_BUS_BASE + 0x00007C00UL) /* RC_IN */

#define GD32_TIMER0_BASE        (GD32_APB2_BUS_BASE + 0x00000000UL) /* Motor 1..4 */
#define GD32_USART0_BASE        (GD32_APB2_BUS_BASE + 0x00001000UL) /* NSH Console */
#define GD32_USART5_BASE        (GD32_APB2_BUS_BASE + 0x00001400UL) /* ESC Telemetry */
#define GD32_ADC_BASE           (GD32_APB2_BUS_BASE + 0x00002000UL)
#define GD32_SPI0_BASE          (GD32_APB2_BUS_BASE + 0x00003000UL) /* ICM-42688-P */
#define GD32_SPI3_BASE          (GD32_APB2_BUS_BASE + 0x00003400UL) /* FM25V02A FRAM */
#define GD32_CAN0_BASE          (GD32_APB2_BUS_BASE + 0x00008C00UL) /* Verified 0x40018C00 */
#define GD32_CAN1_BASE          (GD32_APB2_BUS_BASE + 0x0000A000UL) /* Verified 0x4001A000 */
#define GD32_CAN2_BASE          (GD32_APB2_BUS_BASE + 0x0000B000UL) /* Verified 0x4001B000 */

#define GD32_DMA_BASE           (GD32_AHB1_BUS_BASE + 0x00000000UL)
#define GD32_USBHS0_BASE        (GD32_AHB1_BUS_BASE + 0x00020000UL)
#define GD32_SDIO0_BASE         (GD32_AHB2_BUS_BASE + 0x00022400UL)

#define GD32_SYSCFG_BASE        (GD32_APB4_BUS_BASE + 0x00000400UL)
#define GD32_PMU_BASE           (GD32_APB4_BUS_BASE + 0x00005800UL)
#define GD32_GPIO_BASE          (GD32_AHB4_BUS_BASE + 0x00000000UL)
#define GD32_RCU_BASE           (GD32_AHB4_BUS_BASE + 0x00004400UL)

/* 4. Cortex-M7 Cache Line 尺寸 */

#define GD32_DCACHE_LINESIZE    32U
#define GD32_ICACHE_LINESIZE    32U

#endif /* __ARCH_ARM_SRC_GD32H73X_75X_HARDWARE_GD32H757_MEMORYMAP_H */
