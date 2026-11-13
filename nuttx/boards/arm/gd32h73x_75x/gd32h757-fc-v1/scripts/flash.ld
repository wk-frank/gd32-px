/****************************************************************************
 * boards/arm/gd32h73x_75x/gd32h757-fc-v1/scripts/flash.ld
 *
 * GD32H757ZMT6 最小启动链接脚本（M1 骨架）
 * Flash: 3840 KB @ 0x08000000（预留 Bootloader 偏移前暂从 0x08000000 起）
 * SRAM:  512 KB AXI SRAM @ 0x24000000 + 512 KB 共享 ITCM/DTCM/AXI
 *        本骨架默认按 128 KB ITCM + 256 KB DTCM + 128 KB 扩展 AXI（待核实选项字节）
 ****************************************************************************/

MEMORY
{
  /* TODO: M3/M5 引入 PX4 Bootloader 后调整 ORIGIN 与 LENGTH（如 +128K） */
  FLASH    (rx)  : ORIGIN = 0x08000000, LENGTH = 3840K
  ITCM     (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
  DTCM     (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
  AXISRAM  (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
  BKPSRAM  (rwx) : ORIGIN = 0x38800000, LENGTH = 4K
}

OUTPUT_ARCH(arm)
ENTRY(__start)

SECTIONS
{
  .text :
  {
    _stext = ABSOLUTE(.);
    *(.vectors)
    *(.text .text.*)
    *(.rodata .rodata.*)
    . = ALIGN(4);
    _etext = ABSOLUTE(.);
  } > FLASH

  _eronly = ABSOLUTE(.);

  .data :
  {
    _sdata = ABSOLUTE(.);
    *(.data .data.*)
    . = ALIGN(4);
    _edata = ABSOLUTE(.);
  } > DTCM AT > FLASH

  .bss :
  {
    _sbss = ABSOLUTE(.);
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = ABSOLUTE(.);
  } > DTCM

  /* 通用 DMA 缓冲区必须置于 AXI SRAM 并按 32-byte cache line 对齐 */
  .dma_buffer (NOLOAD) :
  {
    . = ALIGN(32);
    _sdma_buffer = ABSOLUTE(.);
    *(.dma_buffer .dma_buffer.*)
    . = ALIGN(32);
    _edma_buffer = ABSOLUTE(.);
  } > AXISRAM
}
