/****************************************************************************
 * arch/arm/src/gd32h73x_75x/gd32h757_pmu_safety.c
 *
 * GD32H757 PMU 电源模式防呆与 PLL0 600MHz 频率合法性校验实现
 ****************************************************************************/

#include <stddef.h>
#include "gd32h757_pmu_safety.h"

#define GD32H757_MAX_SYSCLK_HZ      600000000UL
#define GD32H757_MAX_HCLK_HZ        300000000UL
#define GD32H757_MAX_APB1_HZ        150000000UL
#define GD32H757_MAX_APB2_HZ        300000000UL
#define GD32H757_MIN_HXTAL_HZ       4000000UL
#define GD32H757_MAX_HXTAL_HZ       50000000UL

bool gd32h757_pmu_validate_supply(gd32h7_supply_mode_t mode)
{
  /* gd32_h757-fc-v1 硬件采用内部 LDO 供 0.9V VCORE，严禁软件误开启 SMPS */
  return (mode == GD32H7_SUPPLY_INTERNAL_LDO);
}

bool gd32h757_rcu_validate_pll0(const gd32h7_clock_cfg_t *cfg,
                                uint32_t *out_sysclk_hz,
                                uint32_t *out_hclk_hz)
{
  if (cfg == NULL || cfg->pll0_psc == 0U || cfg->pll0_n == 0U ||
      cfg->pll0_p_div == 0U || cfg->ahb_div == 0U ||
      cfg->apb1_div == 0U || cfg->apb2_div == 0U)
    {
      return false;
    }

  if (cfg->hxtal_hz < GD32H757_MIN_HXTAL_HZ ||
      cfg->hxtal_hz > GD32H757_MAX_HXTAL_HZ)
    {
      return false;
    }

  uint64_t ref_hz = (uint64_t)cfg->hxtal_hz / (uint64_t)cfg->pll0_psc;
  if (ref_hz < 1000000ULL || ref_hz > 16000000ULL)
    {
      return false;
    }

  uint64_t vco_hz = ref_hz * (uint64_t)cfg->pll0_n;
  uint64_t sysclk_hz = vco_hz / (uint64_t)cfg->pll0_p_div;
  uint64_t hclk_hz = sysclk_hz / (uint64_t)cfg->ahb_div;
  uint64_t apb1_hz = hclk_hz / (uint64_t)cfg->apb1_div;
  uint64_t apb2_hz = hclk_hz / (uint64_t)cfg->apb2_div;

  if (sysclk_hz > GD32H757_MAX_SYSCLK_HZ ||
      hclk_hz > GD32H757_MAX_HCLK_HZ ||
      apb1_hz > GD32H757_MAX_APB1_HZ ||
      apb2_hz > GD32H757_MAX_APB2_HZ)
    {
      return false;
    }

  if (out_sysclk_hz != NULL)
    {
      *out_sysclk_hz = (uint32_t)sysclk_hz;
    }

  if (out_hclk_hz != NULL)
    {
      *out_hclk_hz = (uint32_t)hclk_hz;
    }

  return true;
}
