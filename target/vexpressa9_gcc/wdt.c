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
 *  This file contains a driver for the watchdog timers.
 *  We use them in "timer mode", where a counter decreases
 *  until reaching 0, generating an interrupt (number 30).
 *  If we put them in "watchdog mode" they would reset the
 *  CPU!!
 *
 */
#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"

/*
 *  Clear the timer interrupt request
 */
Inline void
target_timer_int_clear()
{
        /* clear pending bit */
        sil_wrw_mem((void *)MPCORE_WDT_ISR, MPCORE_WDT_ISR_SCBIT);
}

/*
 *  Timer initialization
 */
void
target_timer_initialize(intptr_t exinf)
{
        uint32_t cyc = TO_CLOCK(TIC_NUME, TIC_DENO);

        /* Stop the timer */
        sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);

        /* Set it to timer mode */
        sil_wrw_mem((void *)MPCORE_WDT_DR, 0x12345678);
        sil_wrw_mem((void *)MPCORE_WDT_DR, 0x87654321);

        /* Stop the timer */
        sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);

        /* Clear pending interrupts */
        target_timer_int_clear();

        /* Set the counter value (1ms per tick) */
        sil_wrw_mem((void *)MPCORE_WDT_LR, cyc - 1);

        /*
         * Set the scaler for 1ms per tick and start the timer
         */
        sil_wrw_mem((void *)MPCORE_WDT_CNT,
              (MPCORE_WDT_PS_1MS << MPCORE_WDT_CNT_PS_OFFSET) |
               MPCORE_WDT_CNT_IEN | MPCORE_WDT_CNT_AR | MPCORE_WDT_CNT_EN);
}

/*
 *  Timer termination
 */
void
target_timer_terminate(intptr_t exinf)
{
        /* Clear interrupts */
        target_timer_int_clear();

        /* Stop the timer */
        sil_wrw_mem((void *)MPCORE_WDT_CNT, 0x00);
}

/*
 *  Timer interrupt handler
 */
void
target_timer_handler(void)
{
        ID prcid;

        /* Clear interrupts */
        target_timer_int_clear();

        /* Signal the system tick */
        iget_pid(&prcid);
        i_begin_int((0x10000 << (prcid - 1))|DIC_IRQNO_WDT);
        signal_time();
        i_end_int((0x10000 << (prcid - 1))|DIC_IRQNO_WDT);
}
