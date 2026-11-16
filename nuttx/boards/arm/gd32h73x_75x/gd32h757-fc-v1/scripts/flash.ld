/****************************************************************************
 * boards/arm/gd32h73x_75x/gd32h757-fc-v1/scripts/flash.ld
 *
 * GD32H757ZMT6 飞控固件链接脚本（预留前 128KB 给 PX4 Bootloader，主固件占 3712KB）
 * 存储分区已对照 gd32h759im_memorymap.h 与 GD32H757 Datasheet 核实：
 *   FLASH   : 0x08020000, 3712K (Bootloader 位于 0x08000000..0x0801FFFF)
 *   ITCM    : 0x00000000,  128K (高频控制/向量表加速区)
 *   DTCM    : 0x20000000,  256K (零等待内核栈/.data/.bss，注意通用DMA不可达)
 *   AXISRAM : 0x24000000,  512K (通用DMA缓冲与主堆区，32-byte cache line对齐)
 *   SRAM0_1 : 0x30000000,   32K (AHB外设DMA缓冲专用区)
 *   BKPSRAM : 0x38800000,    4K (HardFault CrashDump与Bootloader通信区)
 ****************************************************************************/

MEMORY
{
  FLASH   (rx)  : ORIGIN = 0x08020000, LENGTH = 3712K
  ITCM    (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
  DTCM    (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
  AXISRAM (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
  SRAM0_1 (rwx) : ORIGIN = 0x30000000, LENGTH = 32K
  BKPSRAM (rwx) : ORIGIN = 0x38800000, LENGTH = 4K
}

OUTPUT_ARCH(arm)
ENTRY(__start)

SECTIONS
{
  .text :
  {
    _stext = ABSOLUTE(.);
    KEEP(*(.vectors))
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(4);
    _etext = ABSOLUTE(.);
  } > FLASH

  .ARM.exidx :
  {
    __exidx_start = ABSOLUTE(.);
    *(.ARM.exidx*)
    __exidx_end = ABSOLUTE(.);
  } > FLASH

  _eronly = ABSOLUTE(.);

  .itcmfunc :
  {
    _sitcm = ABSOLUTE(.);
    *(.itcmfunc .itcmfunc.*)
    . = ALIGN(4);
    _eitcm = ABSOLUTE(.);
  } > ITCM AT > FLASH

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
    . = ALIGN(8);
    _ebss = ABSOLUTE(.);
  } > DTCM

  /* 通用外设 DMA 静态缓冲区（强制置于 AXI SRAM 并 32-byte 对齐） */
  .dma_buffer (NOLOAD) :
  {
    . = ALIGN(32);
    _sdma_buffer = ABSOLUTE(.);
    *(.dma_buffer .dma_buffer.*)
    . = ALIGN(32);
    _edma_buffer = ABSOLUTE(.);
  } > AXISRAM

  /* AHB SRAM0/1 外设缓冲段 */
  .sram4 (NOLOAD) :
  {
    . = ALIGN(32);
    *(.sram4 .sram4.*)
    . = ALIGN(32);
  } > SRAM0_1

  /* 备份域 CrashDump / Bootloader RTC 标志段 */
  .Fk_bkp (NOLOAD) :
  {
    *(.Fk_bkp .Fk_bkp.*)
  } > BKPSRAM
}
