/****************************************************************************
 * boards/arm/gd32h73x_75x/gd32h757-fc-v1/src/gd32_boot.c
 *
 * GD32H757ZMT6 飞控板卡早期启动钩子（安全优先：上电立即禁止电机输出并锁定 LDO 模式）
 ****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "board.h"
#include "gd32h757_pmu_safety.h"

/* 弱符号桩：在未链接完整 gd32_nuttx GPIO 驱动时允许主机侧单元测试验证启动逻辑 */
__attribute__((weak)) int gd32_gpio_config(uint32_t cfgset)
{
  (void)cfgset;
  return 0;
}

int gd32_board_early_init(void)
{
  /* 1. 安全门禁第一优先级：拉高 PE10 (FMU_PWM_OEn = 1)，物理三态关断 8 路电机缓冲器 */
  (void)gd32_gpio_config(GPIO_PWM_BUFFER_OEN);

  /* 2. 校验 PMU 供电配置（必须为内部 LDO 模式，拒绝任何 SMPS 置位） */
  if (!gd32h757_pmu_validate_supply(GD32H7_SUPPLY_INTERNAL_LDO))
    {
      return -1;
    }

  /* 3. 校验 PLL0 600MHz 时钟树参数合法性 */
  gd32h7_clock_cfg_t clk = {
    .hxtal_hz   = GD32_BOARD_HXTAL,
    .pll0_psc   = 5U,
    .pll0_n     = 120U,
    .pll0_p_div = 1U,
    .ahb_div    = 2U,
    .apb1_div   = 2U,
    .apb2_div   = 1U,
  };
  uint32_t sysclk = 0U;
  uint32_t hclk = 0U;
  if (!gd32h757_rcu_validate_pll0(&clk, &sysclk, &hclk) ||
      sysclk != GD32_SYSCLK_FREQUENCY || hclk != GD32_HCLK_FREQUENCY)
    {
      return -2;
    }

  /* 4. 默认初始化片选为高电平，开启传感器 3.3V 低噪声 LDO 与外设 5V 限流开关 */
  (void)gd32_gpio_config(GPIO_SPI0_CS_ICM42688);
  (void)gd32_gpio_config(GPIO_SPI1_CS_BMI088_ACC);
  (void)gd32_gpio_config(GPIO_SPI1_CS_BMI088_GYR);
  (void)gd32_gpio_config(GPIO_SPI3_CS_FRAM);
  (void)gd32_gpio_config(GPIO_VDD_3V3_SENSORS_EN);
  (void)gd32_gpio_config(GPIO_VDD_5V_PERIPH_EN);
  (void)gd32_gpio_config(GPIO_CAN0_SILENT);
  (void)gd32_gpio_config(GPIO_CAN2_SILENT);
  (void)gd32_gpio_config(GPIO_SBUS_INV_SEL);
  (void)gd32_gpio_config(GPIO_LED_RED_N);
  (void)gd32_gpio_config(GPIO_LED_GREEN_N);
  (void)gd32_gpio_config(GPIO_LED_BLUE_N);

  return 0;
}
