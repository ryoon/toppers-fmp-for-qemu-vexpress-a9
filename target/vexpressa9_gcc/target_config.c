/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2007-2012 by Embedded and Real-Time Systems Laboratory
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
 *  This file implements the following functions:
 *      - target_mprc_initialize (master processor initialization)
 *      - target_fput_log (low-level character output)
 *      - target_mmu_init (virtual memory initialization)
 *      - target_initialize (target initialization)
 *      - target_exit (target exit processing)
 *
 */
#include "kernel_impl.h"
#include <sil.h>
#include "vexpressa9.h"    /* for DDR_ADDR, SCCTRL_TIMEREN1SEL_TIMCLK */
#include "target_serial.h" /* for vexpressa9_xxx */

/*
 *  Master processor initialization before str_ker().
 *  (E.g.: initialize the interrupt controller or the SCU.)
 */
void
target_mprc_initialize(void)
{
        chip_mprc_initialize();
}

/*
 *  System log low-level (polling) character output
 */
void
target_fput_log(char c)
{
        if (c == '\n') {
                vexpressa9_pol_putc('\r');
        }
        vexpressa9_pol_putc(c);
}

/*
 * MMU initialization (called from the chip code)
 *
 * We create a simpe 1-to-1 (physical address = virtual address) memory
 * mapping and enable caching for the DDR memory.
 *
 * Note: when not using SafeG the default world is Secure. When using
 * SafeG, Secure world uses secure accesses by default. If you want to
 * access a shared memory region with non-secure access, you will have
 * to configure it manually.
 */
void
target_mmu_init(void)
{
        MEMORY_ATTRIBUTE m_attribute;

        /*
         * Basic initialization of the whole virtual memory space to
         * non-cachable, non-bufferable, strongly-ordered.
         */
        m_attribute.pa   = 0x00000000;
        m_attribute.va   = m_attribute.pa;
        m_attribute.size = 0x80000000; /* 2GB */
#if defined(TOPPERS_SAFEG_SECURE) || defined(TOPPERS_NOSAFEG)
        m_attribute.ns   = 0;     /* 0=Secure */
#endif
        m_attribute.s    = 1;     /* 1=Shared */
        m_attribute.ap   = 3;     /* Full access */
        m_attribute.tex  = 0;     /* Strongly Ordered */
        m_attribute.c    = 0;
        m_attribute.b    = 0;

        mmu_map_memory (&m_attribute);

        m_attribute.pa   = 0x80000000; /* 2GB */
        m_attribute.va   = m_attribute.pa;

        mmu_map_memory (&m_attribute);

        /*
         *  Enable caching for the DDR
         */
        m_attribute.pa   = DDR_ADDR;
        m_attribute.va   = m_attribute.pa;
        m_attribute.size = DDR_SIZE;
#if defined(TOPPERS_SAFEG_SECURE) || defined(TOPPERS_NOSAFEG)
        m_attribute.ns   = 0;     /* 0=Secure */
#endif
        m_attribute.s    = 1;     /* 1=Shared */
        m_attribute.ap   = 3;     /* Full access */
        m_attribute.tex  = 1;     /* Outer and Inner */
        m_attribute.c    = 1;     /* Inner Write-Back */
        m_attribute.b    = 1;     /* Write Allocate */

        mmu_map_memory(&m_attribute);
}

/*
 *  Target-dependent initialization
 */
#ifdef TOPPERS_SAFEG_SECURE
static void trustzone_configure(void)
{
#if 0   /* TODO: configure */

        /*
         * TZPC initialization
         *
         * Note: by default we set all values as non-secure. Depending on
         * the system, the System Engineer should select which bits are
         * set to 1 (non-secure) and 0 (secure).
         */
        REG(TZPC_SIZE_REG)     = 0;      // set all SDRAM to non secure
        REG(TZPC_DECPROT0_SET) = 0xFFFF; // set all devices as non-secure
        REG(TZPC_DECPROT1_SET) = 0xFFFF; // set all devices as non-secure
        REG(TZPC_DECPROT2_SET) = 0xFFFF; // set all devices as non-secure

        /*
         * TZASC initialization
         *
         * Note: not finished yet
         */
        REG(CFGRW1_REG) = REG(CFGRW1_REG) | CFGRW1_TZASC_BIT; // enable TZASC
        REG(0x100EC108) = 0xF0000000;

#endif
}
#endif /* TOPPERS_SAFEG_SECURE */

void
target_initialize(void)
{
        /*
         * Initialize chip
         */
        chip_initialize();

#ifdef TOPPERS_SAFEG_SECURE
        /*
         * Configure TrustZone devices and memory
         */
        trustzone_configure();
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef CA9_SP804_TIMER
        /*
         * Set the SP804 timer frequency used on the System Controller
         */
        REG(SYSCTL_SCCTRL_BASE) = REG(SYSCTL_SCCTRL_BASE) |
                SCCTRL_TIMEREN0SEL_TIMCLK | SCCTRL_TIMEREN1SEL_TIMCLK;
#endif /* CA9_SP804_TIMER */

        /*
         *  Initialize the UART. If we are using a
         *  global logger (shared by all cores) only
         *  the master processor (mprc) initializes it.
         */
#ifdef G_SYSLOG
        if (x_sense_mprc()) {
                vexpressa9_uart_reset();
        }
#else
        vexpressa9_uart_reset();
#endif /* G_SYSLOG */
}

/*
 *  Target-dependent exit processing
 */
void
target_exit(void)
{
        /*
         *  Chip exit processing
         */
        chip_exit();

        /*
         *  Endless loop
         */
        while(1);
}
