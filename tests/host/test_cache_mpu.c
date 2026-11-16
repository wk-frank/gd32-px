#include <assert.h>
#include <stdio.h>
#include "gd32h757_cache_mpu.h"
#include "gd32h757_pmu_safety.h"
#include "board.h"
#include "board_config.h"

extern int gd32_board_early_init(void);

typedef struct
{
  uint8_t  motor_index;
  uint8_t  timer_instance;
  uint8_t  timer_channel;
  uint32_t timer_base;
  uint32_t irq_vector;
  uint32_t gpio_af_cfg;
} gd32_pwm_channel_map_t;

extern const gd32_pwm_channel_map_t g_gd32_pwm_channels[DIRECT_PWM_OUTPUT_CHANNELS];

int main(void)
{
  /* 1. AXI SRAM 与 AHB SRAM0/1 通用 DMA 可达性校验 */
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE, 64U) == true);
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE + GD32_AXISRAM_SIZE - 32U, 32U) == true);
  assert(gd32h757_is_dma_capable_ram(GD32_SRAM0_BASE, 16384U) == true);
  assert(gd32h757_is_dma_capable_ram(GD32_SRAM1_BASE, 16384U) == true);

  /* 2. DTCM / ITCM / Backup SRAM 严禁被通用 DMA0/1 直接访问 */
  assert(gd32h757_is_dma_capable_ram(GD32_DTCM_BASE, 64U) == false);
  assert(gd32h757_is_dma_capable_ram(GD32_ITCM_BASE, 64U) == false);
  assert(gd32h757_is_dma_capable_ram(GD32_BKPSRAM_BASE, 64U) == false);
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE, 0U) == false);
  assert(gd32h757_is_dma_capable_ram(UINTPTR_MAX - 8U, 32U) == false);

  /* 3. 32-byte D-Cache 对齐计算校验 */
  uintptr_t aligned_start = 0U;
  size_t aligned_len = 0U;
  gd32h757_dcache_align_range(0x24000005UL, 40U, &aligned_start, &aligned_len);
  assert(aligned_start == 0x24000000UL);
  assert(aligned_len == 64U);
  assert(GD32_IS_CACHELINE_ALIGNED(aligned_start));

  /* 4. AN111 PMU 供电防呆校验：仅允许内部 LDO，拒绝 SMPS 误置位 */
  assert(gd32h757_pmu_validate_supply(GD32H7_SUPPLY_INTERNAL_LDO) == true);
  assert(gd32h757_pmu_validate_supply(GD32H7_SUPPLY_SMPS_FORBIDDEN) == false);

  /* 5. 板级早期安全启动与 600MHz 时钟树自检 */
  assert(gd32_board_early_init() == 0);

  /* 6. 8 路 PWM/DShot 通道无引脚冲突与合法端口校验 */
  for (int i = 0; i < DIRECT_PWM_OUTPUT_CHANNELS; ++i)
    {
      assert(g_gd32_pwm_channels[i].motor_index == (uint8_t)(i + 1));
      uint32_t port = (g_gd32_pwm_channels[i].gpio_af_cfg & GPIO_PORT_MASK) >> GPIO_PORT_SHIFT;
      assert(port <= 6U); /* 严格位于 PA..PG LQFP144 范围内 */
      for (int j = i + 1; j < DIRECT_PWM_OUTPUT_CHANNELS; ++j)
        {
          uint32_t p1 = g_gd32_pwm_channels[i].gpio_af_cfg & (GPIO_PORT_MASK | GPIO_PIN_MASK);
          uint32_t p2 = g_gd32_pwm_channels[j].gpio_af_cfg & (GPIO_PORT_MASK | GPIO_PIN_MASK);
          assert(p1 != p2);
        }
    }

  printf("M1/M3 host unit tests PASS: cache_mpu, PMU LDO lock, 600MHz PLL0, early_init & 8ch PWM map verified.\n");
  return 0;
}
