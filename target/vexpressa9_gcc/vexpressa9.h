/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  Versatile Express Cortex-A9 porting
 *  2012 (c) Daniel Sangorrin (daniel.sangorrin@gmail.com)
 *
 *  This file contains a few definitions that are specific to
 *  the vexpress-CA9 board, such as the memory address, clock values,
 *  or number of interrupts.
 *
 */
#ifndef TOPPERS_VEXPRESSA9_H
#define TOPPERS_VEXPRESSA9_H

#include <sil.h>

/*
 * MACRO that I like for accessing registers in ARM32bitsLittleEndian
 * Note: in other platforms use sil.h functions
 */
#define REG(x) *((volatile uint32_t *)(x))

/*
 * CPU clocks
 */
#define VEXPRESS_CORE_CLK          400 /* 400 MHz - see Vexpress-A9x4 manual */
#define VEXPRESS_PERIPHCLKCLK      200 /* VEXPRESS_CORE_CLK/2 */
#define MPCORE_GTC_CLOCK_FREQ_MHZ  VEXPRESS_PERIPHCLKCLK

/*
 * Timer preescaler and load value to achieve 1ms per tick with cortex-a9
 * private timers and watchdogs. Note that the preescaler value must be
 * representable with 8 bits; and the load value must be a 32bit value.
 *
 * Private timers and watchdogs are clocked with PERIPHCLK which is 1/2 of
 * the CPU frequency (e.g. VEXPRESS_CORE_CLK). The formula is:
 *
 *    period = (preescaler + 1)(load + 1) / PERIPHCLK
 *      - period    = 1ms
 *      - periphclk = 200Mhz
 *
 *    For example:
 *      - preescaler = 0
 *      - load       = 199999
 */
#define MPCORE_WDT_PS_1MS    0U       // preescaler for private watchdog
#define MPCORE_WDT_LOAD_1MS  199999U  // load value for private watchdog

#define MPCORE_TM_PS_1MS     0U       // preescaler for private timer
#define MPCORE_TM_LOAD_1MS   199999U  // load value for private timer

/*
 * The SP804 timer is clocked by TIMCLK and depending on the SP804 Control
 * register it can preescale (divide) its frequency by 1,16,256. We will
 * assume here that it is not preescaled (i.e., by 1).
 *
 * At reset Vexpress SP804 timers (at least the ones in the motherboard)
 * are clocked at 32.768kHz but the clock can be changed to 1MHz
 * using the sytem controller (SP810). Therefore we can use a load
 * of 1000 (1e3 * 1e-6 = 1e-3 = 1ms)
 */
#define MPCORE_SP804_PS_1MS     1      // preescaler for SP804 timer
#define MPCORE_SP804_LOAD_1MS   1000U  // load value for SP804 timer

/*
 * Available UARTs
 */
#define VEXPRESSA9_UART0_BASE 0x10009000
#define VEXPRESSA9_UART1_BASE 0x1000A000
#define VEXPRESSA9_UART2_BASE 0x1000B000
#define VEXPRESSA9_UART3_BASE 0x1000C000
#define VEXPRESSA9_UART0_IRQ  37
#define VEXPRESSA9_UART1_IRQ  38
#define VEXPRESSA9_UART2_IRQ  39
#define VEXPRESSA9_UART3_IRQ  40

/*
 * Memory base address and size
 */
#define DDR_ADDR 0x60000000
#define DDR_SIZE 0x40000000 /* 1GB */

/*
 * TrustZone Protection Controller (TZPC BP147) registers
 *
 * Note: there is a register to set a small portion of usually static
 * memory as Secure (TZPC_SIZE_REG) and then 3 more registers where
 * each bits configures some device or bus as Secure (0) or Non-Secure(1).
 *
 */
#define TZPC_BASE           0x100E6000
#define TZPC_SIZE_REG      (TZPC_BASE + 0x000)

#define TZPC_DECPROT0_STAT (TZPC_BASE + 0x800)
#define TZPC_DECPROT0_SET  (TZPC_BASE + 0x804)
#define TZPC_DECPROT0_CLR  (TZPC_BASE + 0x808)
#define      TZPC_PL341_APB_BIT     (1 << 0)
#define      TZPC_PL354_APB_BIT     (1 << 1)
#define      TZPC_SCC_BIT           (1 << 2)
#define      TZPC_SP804_BIT         (1 << 4)
#define      TZPC_SP805_BIT         (1 << 5)
#define      TZPC_FAST_PL301_BIT    (1 << 9)
#define      TZPC_SLOW_PL301_BIT    (1 << 10)
#define      TZPC_SMC_TZASC_BIT     (1 << 13)
#define      TZPC_DEBUGAPB_BIT      (1 << 14)

#define TZPC_DECPROT1_STAT (TZPC_BASE + 0x80C)
#define TZPC_DECPROT1_SET  (TZPC_BASE + 0x810)
#define TZPC_DECPROT1_CLR  (TZPC_BASE + 0x814)
#define      TZPC_EXT_AXI_BIT       (1 << 0)
#define      TZPC_PL354_AXI_BIT     (1 << 1)
#define      TZPC_ENTIRE_APB_BIT    (1 << 3)
#define      TZPC_PL111_BIT         (1 << 4)
#define      TZPC_RAM_AXI_BIT       (1 << 5)
#define      TZPC_PL341_AXI_BIT     (1 << 6)
#define      TZPC_CA9COHERENCY_BIT  (1 << 8)
#define      TZPC_ENTIRE_AXI_BIT    (1 << 9)

#define TZPC_DECPROT2_STAT (TZPC_BASE + 0x818)
#define TZPC_DECPROT2_SET  (TZPC_BASE + 0x81C)
#define TZPC_DECPROT2_CLR  (TZPC_BASE + 0x820)
#define      TZPC_EXT_MASTER_TZ_OVERRIDE_BIT       (1 << 0)
#define      TZPC_DAP_AHB_AP_TZ_OVERRIDE_BIT       (1 << 1)
#define      TZPC_PL111_MASTER_TZ_OVERRIDE_BIT     (1 << 2)
#define      TZPC_SMC_TZASC_LOCKDOWN_BIT           (1 << 5)

/*
 * Test Chip SCC Register 1
 */
#define CFGRW1_REG
#define      CFGRW1_DBGEN_BIT   (1 << 8)    // invasive debug
#define      CFGRW1_NIDEN_BIT   (1 << 9)    // non-invasive debug
#define      CFGRW1_SPIDEN_BIT  (1 << 10)   // secure invasive debug
#define      CFGRW1_SNPIDEN_BIT (1 << 11)   // secure non-invasive debug
#define      CFGRW1_L2CC_BIT    (1 << 12)   // enable L2CC
#define      CFGRW1_TZASC_BIT   (1 << 13)   // enable TZASC
#define      CFGRW1_CP15DIS_BIT (0xF << 14) // CP15DISABLE[3:0] bus

/*
 * TrustZone Address Space Controller (TZASC TZC-380)
 */
#define TZASC_BASE 0x100EC000

/*
 *  MPCore Private Memory Region Base Address
 */
#define MPCORE_PMR_BASE  0x1E000000

/*
 *  SP804 Base address and interrupts
 */
#define VEXPRESS_DB_SP804_BASE   0x100E4000  /* Dautherboard timer*/
#define VEXPRESS_MB_SP804_BASE_A 0x1E011000  /* Motherboard timer 0-1 */
#define VEXPRESS_MB_SP804_BASE_B 0x1E012000  /* Motherboard timer 2-3 */

#define VEXPRESS_DB_SP804_IRQ    80 /* Dautherboard timer IRQ */
#define VEXPRESS_MB_SP804_IRQ_A  34 /* Motherboard timer 0-1 IRQ */
#define VEXPRESS_MB_SP804_IRQ_B  35 /* Motherboard timer 2-3 IRQ */

#define SP804_BASE VEXPRESS_DB_SP804_BASE /* choose one of the above */
#define SP804_IRQ  VEXPRESS_DB_SP804_IRQ  /* choose one of the above */

/*
 * System controller
 *
 * Note: we used it for setting the reference clock for TIMCLK to 1MHz
 * which is used by the SP804 timers.
 */
#define SYSCTL_BASE             0x10001000
#define SYSCTL_SCCTRL_BASE     (SYSCTL_BASE + 0)
#define     SCCTRL_TIMEREN0SEL_TIMCLK       (1 << 15)
#define     SCCTRL_TIMEREN1SEL_TIMCLK       (1 << 17)

/*
 *  Number of interrupts supported by the GICv1.0 in this board. Note
 *  that these values could be obtained dynamically from the corresponding
 *  GIC register.
 */
#define DIC_TMIN_INTNO    0U
#define DIC_TMAX_INTNO   95U // maximum interrupt ID
#define DIC_TNUM_INT     96U // maximum number of interrupts

#ifndef TOPPERS_MACRO_ONLY

/*
 *  Trace log definitions (TODO: not tested)
 *
 *  TRACE_GET_TIM returns microseconds and uses the lower part of the
 *  global timer counter (32bit). This means that only around 12.89 seconds
 *  (0xFFFFFFFF/333 us) of trace can be taken until the counter wraps around.
 *
 *  Note: For traces that take more time, comment the TRACE_HW_INIT and
 *  TRACE_GET_TIM. As defined in trace_config.h, get_my_current_time()
 *  will be used instead. This allows taking a very long trace log (up to
 *  49,7 days) but the resolution will be in the milliseconds order.
 */
#ifndef MPCORE_GTC_COUNT_L
#define MPCORE_GTC_COUNT_L (MPCORE_PMR_BASE + 0x0200)
#endif

#define TRACE_HW_INIT() x_init_pcc()
#define TRACE_GET_TIM() (*((volatile uint32_t *)(MPCORE_GTC_COUNT_L)) / (MPCORE_GTC_CLOCK_FREQ_MHZ))

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_VEXPRESSA9_H */
