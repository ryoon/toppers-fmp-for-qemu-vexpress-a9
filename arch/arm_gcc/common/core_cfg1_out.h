/*
 *  @(#) $Id: core_cfg1_out.h 1096 2015-02-14 10:48:14Z ertl-honda $
 */

/*
 *		cfg1_out.cをリンクするために必要なスタブの定義
 */

void sta_ker(void){}
void _kernel_target_mprc_initialize(void){}
uint_t _kernel_start_sync;
STK_T *const	_kernel_istkpt_table[TNUM_PRCID];

/*
 *  オフセットファイルを生成するための定義
 */
const uint8_t   MAGIC_1 = 0x12;
const uint16_t  MAGIC_2 = 0x1234;
const uint32_t  MAGIC_4 = 0x12345678;

const TCB TCB_enatex = {
	{ NULL, NULL }, NULL, 0U, 0U,
	false, false, true,
	0U, NULL, { NULL, NULL }, 0U,
#if TTYPE_KLOCK != G_KLOCK
	false, /* pend_relwai */
	false, /* pend_chgpri */
	0U,    /* pend_newpri */
#endif /* TTYPE_KLOCK != G_KLOCK */
	0U, {0U, NULL, NULL}, NULL, {{0U, 0U, 0U}}
};

const exc_frame_t exc_frame_cpsr = {
#if (__TARGET_ARCH_ARM == 4) || (__TARGET_ARCH_ARM == 5)
	0U, /* nest_count */
	0U, /* ipm */
	1U, /* cpsr */
	0U, /* r0 */
	0U, /* r1 */
	0U, /* r2 */
	0U, /* r3 */
	0U, /* r12 */
	0U, /* lr */
	0U, /* pc */
#else /* (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7) */
#ifdef USE_GIC_CPULOCK
	0U, /* saved_iipm */
	0U, /* lock_flag */
#endif /* USE_GIC_CPULOCK */
	0U, /* nest_count */
	0U, /* ipm */
#if defined(SUPPORT_VFPv3_NEON)
	0U, /* d0 */
	0U, /* d1 */
	0U, /* d2 */
	0U, /* d3 */
	0U, /* d4 */
	0U, /* d5 */
	0U, /* d6 */
	0U, /* d7 */
	0U, /* fpscr */
#endif /* SUPPORT_VFPv3_NEON */
	0U, /* r0 */
	0U, /* r1 */
	0U, /* r2 */
	0U, /* r3 */
	0U, /* r12 */
	0U, /* lr */
	0U, /* pc */
	1U, /* cpsr */
#endif /* (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7) */
};

