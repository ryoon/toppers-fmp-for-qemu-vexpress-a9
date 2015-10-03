/*
 *  Versatile Express Cortex-A9 porting
 *  2012 (c) Daniel Sangorrin (daniel.sangorrin@gmail.com)
 */
#ifndef TOPPERS_TARGET_ASM_INC
#define TOPPERS_TARGET_ASM_INC

#ifdef TOPPERS_ENABLE_UBOOT
#define UBOOT_LOAD_ADDRESS               0x60000000 /* reset vector address */
#define UBOOT_HOLDING_PEN_POINTER        0x80000000 /* pointer to magic holding pen release value */
#define UBOOT_HOLDING_PEN_RELEASE_VALUE  0x87654321 /* magic holding pen release value */
#endif /* TOPPERS_ENABLE_UBOOT */

#include "chip_asm.inc"

#endif /* TOPPERS_TARGET_ASM_INC */
