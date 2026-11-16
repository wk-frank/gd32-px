/****************************************************************************
 * boards/gd32/h757-fc-v1/src/timer_config.c
 *
 * GD32H757ZMT6 8 路 PWM / 双向 DShot600 定时器与通道映射表
 *   M1..M4 -> TIMER0 CH0..3 (PE9, PE11, PE13, PE14, APB2 300MHz)
 *   M5..M8 -> TIMER3 CH0..3 (PD12, PD13, PD14, PD15, APB1 300MHz timer clk)
 ****************************************************************************/

#include <stdint.h>
#include "board_config.h"

typedef struct
{
  uint8_t  motor_index;     /* 1..8 */
  uint8_t  timer_instance;  /* 0 = TIMER0, 3 = TIMER3 */
  uint8_t  timer_channel;   /* 0..3 */
  uint32_t timer_base;
  uint32_t irq_vector;
  uint32_t gpio_af_cfg;
} gd32_pwm_channel_map_t;

const gd32_pwm_channel_map_t g_gd32_pwm_channels[DIRECT_PWM_OUTPUT_CHANNELS] =
{
  { 1, 0, 0, GD32_TIMER0_BASE, GD32_IRQ_TIMER0_CHANNEL, GPIO_MOTOR1_CH },
  { 2, 0, 1, GD32_TIMER0_BASE, GD32_IRQ_TIMER0_CHANNEL, GPIO_MOTOR2_CH },
  { 3, 0, 2, GD32_TIMER0_BASE, GD32_IRQ_TIMER0_CHANNEL, GPIO_MOTOR3_CH },
  { 4, 0, 3, GD32_TIMER0_BASE, GD32_IRQ_TIMER0_CHANNEL, GPIO_MOTOR4_CH },
  { 5, 3, 0, GD32_TIMER3_BASE, GD32_IRQ_TIMER3,         GPIO_MOTOR5_CH },
  { 6, 3, 1, GD32_TIMER3_BASE, GD32_IRQ_TIMER3,         GPIO_MOTOR6_CH },
  { 7, 3, 2, GD32_TIMER3_BASE, GD32_IRQ_TIMER3,         GPIO_MOTOR7_CH },
  { 8, 3, 3, GD32_TIMER3_BASE, GD32_IRQ_TIMER3,         GPIO_MOTOR8_CH },
};
