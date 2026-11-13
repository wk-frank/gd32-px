/****************************************************************************
 * boards/arm/gd32h73x_75x/gd32h757-fc-v1/include/board.h
 *
 * GD32H757ZMT6 飞控板卡板级头文件（M1 骨架）
 * 声明：未取得原理图前，晶振频率、PMU 供电模式与引脚映射均标为 TODO。
 ****************************************************************************/

#ifndef __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H
#define __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H

/* TODO: 根据原理图确认实际 HXTAL 频率（常见 8/16/24/25 MHz） */
#define BOARD_HXTAL_FREQUENCY_HZ   25000000UL  /* TODO: 待原理图确认 */

/* TODO: 必须对照 AN111 与原理图确认 PMU 模式（LDO / SMPS / Direct） */
#define BOARD_PMU_SUPPLY_MODE_TODO 1

/* 目标系统主频与总线分频（待 User Manual RCU 章节核实后固定 PLL 参数） */
#define BOARD_SYSCLK_FREQUENCY_HZ  600000000UL
#define BOARD_AHB_FREQUENCY_HZ     300000000UL /* TODO: 核实 GD32H7 最大 AHB 频率 */
#define BOARD_APB1_FREQUENCY_HZ    150000000UL /* TODO: 核实 GD32H7 最大 APB1 频率 */
#define BOARD_APB2_FREQUENCY_HZ    150000000UL /* TODO: 核实 GD32H7 最大 APB2 频率 */

/* Console UART 引脚（TODO: 待原理图确认 USART 实例与 GPIO 引脚） */
#define BOARD_CONSOLE_UART_TODO    1

#endif /* __BOARDS_ARM_GD32H73X_75X_GD32H757_FC_V1_INCLUDE_BOARD_H */
