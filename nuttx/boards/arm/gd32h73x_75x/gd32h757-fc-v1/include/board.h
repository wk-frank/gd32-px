/****************************************************************************
 * boards/arm/gd32h73x_75x/gd32h757-fc-v1/include/board.h
 *
 * GD32H757ZMT6 商业飞控参考板卡板级头文件（gd32_h757-fc-v1）
 * 注：AF 编号与寄存器偏移须在合入 gd32_nuttx 后对照 Datasheet 最终签核。
 ****************************************************************************/

#ifndef __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H
#define __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H

/* 1. 晶振与 PMU 供电模式（参考硬件选型：25 MHz HXTAL + 内部 LDO） */

#define BOARD_HXTAL_FREQUENCY_HZ   25000000UL
#define BOARD_LXTAL_FREQUENCY_HZ   32768UL
#define BOARD_PMU_SUPPLY_LDO       1          /* 内部 LDO 供 0.9V VCORE，严禁未改硬件切 SMPS */

/* 2. 目标时钟树（25 MHz / 5 = 5 MHz -> PLL0N * 120 = 600 MHz VCO -> /1 = 600 MHz） */

#define BOARD_PLL0_PSC             5U
#define BOARD_PLL0_N               120U
#define BOARD_PLL0_P               1U
#define BOARD_PLL0_Q               12U        /* 50 MHz（可用于 SPI/SDIO 分频基准，待核实） */
#define BOARD_PLL0_R               2U

#define BOARD_SYSCLK_FREQUENCY_HZ  600000000UL
#define BOARD_AHB_FREQUENCY_HZ     300000000UL
#define BOARD_APB1_FREQUENCY_HZ    150000000UL
#define BOARD_APB2_FREQUENCY_HZ    150000000UL
#define BOARD_APB3_FREQUENCY_HZ    150000000UL
#define BOARD_APB4_FREQUENCY_HZ    150000000UL

/* 3. 串口与调试控制台 */

#define BOARD_CONSOLE_USART        0          /* USART0 (PA9/PA10), 115200 8N1 */
#define BOARD_MAVLINK_TELEM1_USART 1          /* USART1 (PA2/PA3 + CTS/RTS) */
#define BOARD_MAVLINK_TELEM2_USART 2          /* USART2 (PB10/PB11) */
#define BOARD_GPS_UART             6          /* 避开 SDIO0 冲突，建议使用 UART6 */
#define BOARD_RC_UART              7          /* 避开 SDIO0 冲突，建议使用 UART7 */
#define BOARD_ESC_TLM_USART        5          /* USART5 (PC7 RX) */

/* 4. SPI / I2C / CAN / SDIO 启用标识 */

#define BOARD_HAVE_ICM42688P_SPI0  1          /* SPI0: 主 IMU */
#define BOARD_HAVE_BMI088_SPI1     1          /* SPI1: 冗余 IMU */
#define BOARD_HAVE_FM25V02A_SPI3   1          /* SPI3: 256Kbit FRAM */
#define BOARD_HAVE_SENSOR_I2C0     1          /* I2C0: BMP390 + BMM150 */
#define BOARD_HAVE_EXTMAG_I2C1     1          /* I2C1: 外置磁力计 */
#define BOARD_HAVE_POWER_I2C2      1          /* I2C2: INA226 功率计 */
#define BOARD_HAVE_CAN0_FD         1
#define BOARD_HAVE_CAN1_FD         1
#define BOARD_HAVE_SDIO0           1

#endif /* __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H */
