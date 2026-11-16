/****************************************************************************
 * arch/arm/include/gd32h73x_75x/gd32h757_irq.h
 *
 * GD32H737/757/759 中断向量号定义（严格对齐 GD32-MCU-IOT/gd32_nuttx）
 ****************************************************************************/

#ifndef __ARCH_ARM_INCLUDE_GD32H73X_75X_GD32H757_IRQ_H
#define __ARCH_ARM_INCLUDE_GD32H73X_75X_GD32H757_IRQ_H

/* Cortex-M7 Processor Exceptions (0..15) */

#define GD32_IRQ_NMI               2
#define GD32_IRQ_HARDFAULT         3
#define GD32_IRQ_MEMFAULT          4
#define GD32_IRQ_BUSFAULT          5
#define GD32_IRQ_USAGEFAULT        6
#define GD32_IRQ_SVCALL            11
#define GD32_IRQ_DBGMONITOR        12
#define GD32_IRQ_PENDSV            14
#define GD32_IRQ_SYSTICK           15

/* External Interrupt Base */

#define GD32_IRQ_EXINT             16

/* Key Flight-Control Peripheral IRQs (Verified from gd32h73x_75x_irq.h) */

#define GD32_IRQ_WWDGT             (GD32_IRQ_EXINT + 0)
#define GD32_IRQ_RCU               (GD32_IRQ_EXINT + 5)
#define GD32_IRQ_EXTI0             (GD32_IRQ_EXINT + 6)
#define GD32_IRQ_EXTI1             (GD32_IRQ_EXINT + 7)
#define GD32_IRQ_EXTI2             (GD32_IRQ_EXINT + 8)   /* GPS1 PPS (PG2) */
#define GD32_IRQ_EXTI3             (GD32_IRQ_EXINT + 9)
#define GD32_IRQ_EXTI4             (GD32_IRQ_EXINT + 10)  /* ICM42688P DRDY (PC4) */
#define GD32_IRQ_DMA0_CH0          (GD32_IRQ_EXINT + 11)
#define GD32_IRQ_DMA0_CH1          (GD32_IRQ_EXINT + 12)
#define GD32_IRQ_DMA0_CH2          (GD32_IRQ_EXINT + 13)
#define GD32_IRQ_DMA0_CH3          (GD32_IRQ_EXINT + 14)
#define GD32_IRQ_DMA0_CH4          (GD32_IRQ_EXINT + 15)
#define GD32_IRQ_DMA0_CH5          (GD32_IRQ_EXINT + 16)
#define GD32_IRQ_DMA0_CH6          (GD32_IRQ_EXINT + 17)
#define GD32_IRQ_ADC0_1            (GD32_IRQ_EXINT + 18)
#define GD32_IRQ_EXTI5_9           (GD32_IRQ_EXINT + 23)  /* BMI088 Gyro DRDY (PC5) */
#define GD32_IRQ_TIMER0_BRK        (GD32_IRQ_EXINT + 24)
#define GD32_IRQ_TIMER0_UP         (GD32_IRQ_EXINT + 25)
#define GD32_IRQ_TIMER0_TRG_CMT    (GD32_IRQ_EXINT + 26)
#define GD32_IRQ_TIMER0_CHANNEL    (GD32_IRQ_EXINT + 27)  /* M1..M4 DShot/PWM */
#define GD32_IRQ_TIMER1            (GD32_IRQ_EXINT + 28)  /* PX4 32-bit HRT */
#define GD32_IRQ_TIMER2            (GD32_IRQ_EXINT + 29)  /* Heater + Buzzer */
#define GD32_IRQ_TIMER3            (GD32_IRQ_EXINT + 30)  /* M5..M8 DShot/PWM */
#define GD32_IRQ_I2C0_EV           (GD32_IRQ_EXINT + 31)
#define GD32_IRQ_I2C0_ER           (GD32_IRQ_EXINT + 32)
#define GD32_IRQ_I2C1_EV           (GD32_IRQ_EXINT + 33)
#define GD32_IRQ_I2C1_ER           (GD32_IRQ_EXINT + 34)
#define GD32_IRQ_SPI0              (GD32_IRQ_EXINT + 35)  /* ICM-42688-P */
#define GD32_IRQ_SPI1              (GD32_IRQ_EXINT + 36)  /* BMI088 */
#define GD32_IRQ_USART0            (GD32_IRQ_EXINT + 37)  /* NSH Console */
#define GD32_IRQ_USART1            (GD32_IRQ_EXINT + 38)  /* TELEM1 */
#define GD32_IRQ_USART2            (GD32_IRQ_EXINT + 39)  /* TELEM2 */
#define GD32_IRQ_EXTI10_15         (GD32_IRQ_EXINT + 40)  /* BMI088 Accel DRDY (PD10) */
#define GD32_IRQ_SDIO0             (GD32_IRQ_EXINT + 49)  /* MicroSD ULog */
#define GD32_IRQ_UART3             (GD32_IRQ_EXINT + 52)
#define GD32_IRQ_UART4             (GD32_IRQ_EXINT + 53)
#define GD32_IRQ_CAN0_TX           (GD32_IRQ_EXINT + 63)
#define GD32_IRQ_CAN0_RX0          (GD32_IRQ_EXINT + 64)
#define GD32_IRQ_CAN0_RX1          (GD32_IRQ_EXINT + 65)
#define GD32_IRQ_CAN0_EWMC         (GD32_IRQ_EXINT + 66)
#define GD32_IRQ_USBHS0_OTG        (GD32_IRQ_EXINT + 67)
#define GD32_IRQ_USART5            (GD32_IRQ_EXINT + 71)  /* ESC Telemetry */
#define GD32_IRQ_I2C2_EV           (GD32_IRQ_EXINT + 72)
#define GD32_IRQ_I2C2_ER           (GD32_IRQ_EXINT + 73)
#define GD32_IRQ_USBHS0_EP1_OUT    (GD32_IRQ_EXINT + 74)
#define GD32_IRQ_USBHS0_EP1_IN     (GD32_IRQ_EXINT + 75)
#define GD32_IRQ_USBHS0_WKUP       (GD32_IRQ_EXINT + 76)
#define GD32_IRQ_USBHS0            (GD32_IRQ_EXINT + 77)
#define GD32_IRQ_UART6             (GD32_IRQ_EXINT + 82)  /* GPS1 */
#define GD32_IRQ_UART7             (GD32_IRQ_EXINT + 83)  /* RC Input */
#define GD32_IRQ_SPI3              (GD32_IRQ_EXINT + 84)  /* FRAM */
#define GD32_IRQ_I2C3_EV           (GD32_IRQ_EXINT + 95)
#define GD32_IRQ_I2C3_ER           (GD32_IRQ_EXINT + 96)
#define GD32_IRQ_ADC2              (GD32_IRQ_EXINT + 108)
#define GD32_IRQ_CAN0_BUSOFF       (GD32_IRQ_EXINT + 113)
#define GD32_IRQ_CAN0_ERROR        (GD32_IRQ_EXINT + 114)
#define GD32_IRQ_CAN1_TX           (GD32_IRQ_EXINT + 115)
#define GD32_IRQ_CAN2_TX           (GD32_IRQ_EXINT + 121)
#define GD32_IRQ_CAN2_RX0          (GD32_IRQ_EXINT + 122)
#define GD32_IRQ_CAN2_RX1          (GD32_IRQ_EXINT + 123)
#define GD32_IRQ_CAN2_EWMC         (GD32_IRQ_EXINT + 124)
#define GD32_IRQ_CAN2_BUSOFF       (GD32_IRQ_EXINT + 125)
#define GD32_IRQ_CAN2_ERROR        (GD32_IRQ_EXINT + 126)

#define GD32_IRQ_NEXTINT           201
#define NR_VECTORS                 (GD32_IRQ_EXINT + GD32_IRQ_NEXTINT)
#define NR_IRQS                    NR_VECTORS /* 217 */

#endif /* __ARCH_ARM_INCLUDE_GD32H73X_75X_GD32H757_IRQ_H */
