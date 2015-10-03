/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  This file contains the implementation of the driver for
 *  SP804 timers. We only use the 1st of 2 timers.
 *
 */
#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"
#include "vexpressa9.h" /* for SP804_BASE */

#ifndef CA9_SP804_TIMER
#error "CA9_SP804_TIMER should be defined in Makefile.target"
#endif

/*
 * SP804 Registers
 */
#define SP804_TIMER_1_BASE    (SP804_BASE + 0x00)
#define SP804_TIMER_2_BASE    (SP804_BASE + 0x20)

#define SP804_TIMER_LOAD      0x00  /* autoload value (>1) counts to 0 rw */
#define SP804_TIMER_VALUE     0x04  /* read current counter value ro */
#define SP804_TIMER_CTRL      0x08  /* control mode, enable timer, etc rw */
#define     SP804_TIMER_CTRL_ONESHOT      (1 << 0) /* oneshot mode */
#define     SP804_TIMER_CTRL_32BIT        (1 << 1) /* 32 bit counter */
#define     SP804_TIMER_CTRL_DIV1         (0 << 2) /* preescale: clock/1 */
#define     SP804_TIMER_CTRL_DIV16        (1 << 2) /* preescale: clock/16 */
#define     SP804_TIMER_CTRL_DIV256       (2 << 2) /* preescale: clock/256 */
#define     SP804_TIMER_CTRL_IE           (1 << 5) /* enable interrupt */
#define     SP804_TIMER_CTRL_PERIODIC     (1 << 6) /* periodic mode */
#define     SP804_TIMER_CTRL_ENABLE       (1 << 7) /* enable timer */
#define SP804_TIMER_INTCLR    0x0c /* clear interrupt wo */
#define SP804_TIMER_RIS       0x10 /* raw irq status (1=pending) ro */
#define SP804_TIMER_MIS       0x14 /* masked irq status (raw | ctrl_enable) ro */
#define SP804_TIMER_BGLOAD    0x18 /* background load (like LOAD but for the next round) rw */

#if MPCORE_SP804_PS_1MS == 1
#define SP804_TIMER_CTRL_DIV SP804_TIMER_CTRL_DIV1
#elif MPCORE_SP804_PS_1MS == 16
#define SP804_TIMER_CTRL_DIV SP804_TIMER_CTRL_DIV16
#elif MPCORE_SP804_PS_1MS == 256
#define SP804_TIMER_CTRL_DIV SP804_TIMER_CTRL_DIV256
#endif

#define TIM1REG(x) *((volatile uint32_t *)(SP804_TIMER_1_BASE + (x)))
#define TIM2REG(x) *((volatile uint32_t *)(SP804_TIMER_2_BASE + (x)))

/*
 *  Timer initialization (called from startup.c:call_local_inirtn)
 */
void target_timer_initialize(intptr_t exinf)
{
        /* Disable the timer */
        TIM1REG(SP804_TIMER_CTRL)   = 0;

        /* Clear pending interrupts */
        TIM1REG(SP804_TIMER_INTCLR) = 1;

        /* Set the counter value (1ms per tick) */
        TIM1REG(SP804_TIMER_BGLOAD) = MPCORE_SP804_LOAD_1MS;
        TIM1REG(SP804_TIMER_LOAD)   = MPCORE_SP804_LOAD_1MS;

        /*
         * Set the scaler for 1ms per tick and start the timer
         */
        TIM1REG(SP804_TIMER_CTRL)   = SP804_TIMER_CTRL_32BIT    |
                                      SP804_TIMER_CTRL_DIV      |
                                      SP804_TIMER_CTRL_PERIODIC |
                                      SP804_TIMER_CTRL_IE       |
                                      SP804_TIMER_CTRL_ENABLE;
}

/*
 * Timer termination
 */
void target_timer_terminate(intptr_t exinf)
{
        /* Stop the timer */
        TIM1REG(SP804_TIMER_CTRL)   = 0;

        /* Clear interrupts */
        TIM1REG(SP804_TIMER_INTCLR) = 1;
}

/*
 *  Get the current timer value
 */
CLOCK target_timer_get_current(void)
{
        return(TO_CLOCK(TIC_NUME, TIC_DENO) - TIM1REG(SP804_TIMER_VALUE));
}

/*
 *  Check for pending timer interrupts
 */
bool_t target_timer_probe_int(void)
{
        if (TIM1REG(SP804_TIMER_RIS) & 1U)
                return true;
        else
                return false;
}
/*
 *  Timer interrupt handler
 */
void target_timer_handler(void)
{
        ID prcid;

        /* Clear interrupts */
        TIM1REG(SP804_TIMER_INTCLR) = 1;

        /* Signal the system tick */
        iget_pid(&prcid);
        i_begin_int((0x10000 << (prcid - 1))|SP804_IRQ);
        signal_time();
        i_end_int((0x10000 << (prcid - 1))|SP804_IRQ);
}
