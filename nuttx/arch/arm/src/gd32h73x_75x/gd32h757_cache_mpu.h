/****************************************************************************
 * arch/arm/src/gd32h73x_75x/gd32h757_cache_mpu.h
 *
 * GD32H757 Cortex-M7 D-Cache 一致性与 DMA 内存区域校验接口
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_CACHE_MPU_H
#define __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_CACHE_MPU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "hardware/gd32h757_memorymap.h"

#define GD32_DMA_ALIGNED __attribute__((aligned(GD32_DCACHE_LINESIZE)))

#define GD32_IS_CACHELINE_ALIGNED(addr) \
  ((((uintptr_t)(addr)) & (GD32_DCACHE_LINESIZE - 1U)) == 0U)

#define GD32_ALIGN_UP_CACHELINE(sz) \
  (((size_t)(sz) + (GD32_DCACHE_LINESIZE - 1U)) & ~((size_t)(GD32_DCACHE_LINESIZE - 1U)))

#ifdef __cplusplus
extern "C" {
#endif

/* 判断给定内存区间是否位于通用 DMA 可达的 AXI SRAM（排除仅 MDMA 可达的 DTCM/ITCM） */
bool gd32h757_is_dma_capable_ram(uintptr_t addr, size_t len);

/* 计算 DMA 传输前/后需要 clean 或 invalidate 的对齐边界 */
void gd32h757_dcache_align_range(uintptr_t addr, size_t len,
                                 uintptr_t *aligned_start,
                                 size_t *aligned_len);

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_ARM_SRC_GD32H73X_75X_GD32H757_CACHE_MPU_H */
