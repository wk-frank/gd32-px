/****************************************************************************
 * arch/arm/src/gd32h73x_75x/gd32h757_cache_mpu.c
 *
 * GD32H757 Cortex-M7 D-Cache 对齐与通用 DMA 可达性校验实现
 * 通用 DMA0/DMA1 可访问 AXI SRAM (0x24000000, 512KB) 与 AHB SRAM0/1 (0x30000000, 32KB)，
 * 严禁将通用外设 DMA 指向 DTCM (0x20000000) 或 ITCM (0x00000000)。
 ****************************************************************************/

#include "gd32h757_cache_mpu.h"

bool gd32h757_is_dma_capable_ram(uintptr_t addr, size_t len)
{
  if (len == 0U || addr > (UINTPTR_MAX - len))
    {
      return false;
    }

  uintptr_t end = addr + len;

  uintptr_t axi_end = GD32_AXISRAM_BASE + GD32_AXISRAM_SIZE;
  if (addr >= GD32_AXISRAM_BASE && end <= axi_end)
    {
      return true;
    }

  uintptr_t ahb_sram_end = GD32_SRAM0_BASE + GD32_SRAM0_SIZE + GD32_SRAM1_SIZE;
  if (addr >= GD32_SRAM0_BASE && end <= ahb_sram_end)
    {
      return true;
    }

  return false;
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
