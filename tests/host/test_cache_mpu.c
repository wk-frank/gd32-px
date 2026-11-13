#include <assert.h>
#include <stdio.h>
#include "gd32h757_cache_mpu.h"

int main(void)
{
  /* 1. AXI SRAM DMA 可达性校验 */
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE, 64U) == true);
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE + GD32_AXISRAM_SIZE - 32U, 32U) == true);

  /* 2. DTCM / ITCM 不应被误判为通用 DMA 可达区 */
  assert(gd32h757_is_dma_capable_ram(GD32_DTCM_BASE, 64U) == false);
  assert(gd32h757_is_dma_capable_ram(GD32_ITCM_BASE, 64U) == false);

  /* 3. 边界溢出与 0 长度保护 */
  assert(gd32h757_is_dma_capable_ram(GD32_AXISRAM_BASE, 0U) == false);
  assert(gd32h757_is_dma_capable_ram(UINTPTR_MAX - 8U, 32U) == false);

  /* 4. 32-byte cache line 对齐计算校验 */
  uintptr_t aligned_start = 0U;
  size_t aligned_len = 0U;
  gd32h757_dcache_align_range(0x24000005UL, 40U, &aligned_start, &aligned_len);
  assert(aligned_start == 0x24000000UL);
  assert(aligned_len == 64U);
  assert(GD32_IS_CACHELINE_ALIGNED(aligned_start));

  printf("M1 host unit test PASS: cache_mpu & memorymap checks succeeded.\n");
  return 0;
}
