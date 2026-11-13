/****************************************************************************
 * arch/arm/src/gd32h73x_75x/gd32h757_cache_mpu.c
 *
 * GD32H757 Cortex-M7 D-Cache 对齐与 DMA 可达性校验实现
 ****************************************************************************/

#include "gd32h757_cache_mpu.h"

bool gd32h757_is_dma_capable_ram(uintptr_t addr, size_t len)
{
  if (len == 0U)
    {
      return false;
    }

  /* 防止无符号加法溢出 */
  if (addr > (UINTPTR_MAX - len))
    {
      return false;
    }

  uintptr_t end = addr + len;
  uintptr_t axi_end = GD32_AXISRAM_BASE + GD32_AXISRAM_SIZE;

  return (addr >= GD32_AXISRAM_BASE) && (end <= axi_end);
}

void gd32h757_dcache_align_range(uintptr_t addr, size_t len,
                                 uintptr_t *aligned_start,
                                 size_t *aligned_len)
{
  if (aligned_start == NULL || aligned_len == NULL || len == 0U)
    {
      return;
    }

  uintptr_t mask = (uintptr_t)(GD32_DCACHE_LINESIZE - 1U);
  uintptr_t start = addr & ~mask;
  uintptr_t end = (addr + len + mask) & ~mask;

  *aligned_start = start;
  *aligned_len = (size_t)(end - start);
}
