/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2009 by Embedded and Real-Time Systems Laboratory
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
 *  SP804 timer driver definitions. Here we only implement functions for
 *  the first of the two timers in a SP804 (but using the second is trivial)
 */
#ifndef TOPPERS_SP804_TIMER_H
#define TOPPERS_SP804_TIMER_H

#include "vexpressa9.h" /* for SP804_IRQ */

/*
 *  Timer interrupts number (used by sp804.cfg)
 */
#define INTNO_TIMER_PRC1     (SP804_IRQ)
#define INHNO_TIMER_PRC1     (0x10000 | INTNO_TIMER_PRC1)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  Type definition for the internal representation of the timer value
 */
typedef uint32_t CLOCK;

/*
 *  Convertion between the internal representation of the timer value
 *  and miliseconds/microseconds. One tick equals to 1ms.
 */
#define TIMER_CLOCK          MPCORE_SP804_LOAD_1MS // cycles
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)       (((SYSUTM) clock) * 1000U / TIMER_CLOCK)

/*
 *  Timer initialization
 */
extern void target_timer_initialize(intptr_t exinf);

/*
 * Timer termination
 */
extern void target_timer_terminate(intptr_t exinf);

/*
 *  Get the current timer value
 */
extern CLOCK target_timer_get_current(void);

/*
 *  Check for pending timer interrupts
 */
extern bool_t target_timer_probe_int(void);

/*
 *  Timer interrupt handler
 */
extern void target_timer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_SP804_TIMER_H */
