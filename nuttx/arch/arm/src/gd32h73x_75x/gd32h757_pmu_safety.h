/****************************************************************************
 * arch/arm/src/gd32h73x_75x/gd32h757_pmu_safety.h
 *
 * GD32H757 PMU 电源模式硬件防呆与 PLL0 600MHz 升频前置安全检查（遵循 AN111）
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_PMU_SAFETY_H
#define __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_PMU_SAFETY_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  GD32H7_SUPPLY_INTERNAL_LDO = 0,  /* gd32_h757-fc-v1 唯一允许模式 */
  GD32H7_SUPPLY_EXTERNAL_BYPASS,
  GD32H7_SUPPLY_SMPS_FORBIDDEN     /* 原理图未接 SMPS 电感，禁止启用以防烧毁 VCORE */
} gd32h7_supply_mode_t;

typedef struct
{
  uint32_t hxtal_hz;
  uint32_t pll0_psc;
  uint32_t pll0_n;
  uint32_t pll0_p_div;
  uint32_t ahb_div;
  uint32_t apb1_div;
  uint32_t apb2_div;
} gd32h7_clock_cfg_t;

#ifdef __cplusplus
extern "C" {
#endif

/* 校验供电拓扑是否与板级硬件一致（拒绝非法 SMPS 置位） */
bool gd32h757_pmu_validate_supply(gd32h7_supply_mode_t mode);

/* 计算并校验 PLL0 系统时钟与总线频率是否在 GD32H757 规格上限内 */
bool gd32h757_rcu_validate_pll0(const gd32h7_clock_cfg_t *cfg,
                                uint32_t *out_sysclk_hz,
                                uint32_t *out_hclk_hz);

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_PMU_SAFETY_H */
