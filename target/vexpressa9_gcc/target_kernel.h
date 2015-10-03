/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
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
 *  This file is included by kernel.h just after including
 *  t_stddef.h.
 */
#ifndef TOPPERS_TARGET_KERNEL_H
#define TOPPERS_TARGET_KERNEL_H

#include "vexpressa9.h"

/*
 *  Method for managing the system time
 *
 *  Note: the SP804 timers only use 1 interrupt so we have to use the
 *  same timer for all cores. The Cortex-A9 private timers and watchdogs
 *  have 1 interrupt per core so we can use them as local timers.
 */
#ifdef CA9_SP804_TIMER
#define TOPPERS_SYSTIM_GLOBAL /* use a global timer for all CPUs */
#else /* CA9_PRIVATE_TIMER or CA9_PRIVATE_WDT */
#define TOPPERS_SYSTIM_LOCAL /* use timers that are local to each CPU*/
#endif

/*
 *  Master processor
 */
#define TOPPERS_MASTER_PRCID  1

/*
 *  Processor ID that manages the system time when using a global timer
 */
#define TOPPERS_SYSTIM_PRCID  TOPPERS_MASTER_PRCID

/*
 * Default number of processors (this value can be overwritten in the
 * application's Makefile with the desired number of processors)
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID  4
#endif /* TNUM_PRCID */

/*
 *  Definition of the CLASS IDs which are used to assign tasks to processors
 *  statically at initialization.
 */
#define TCL_1          1 /* Assign tasks in TCL_1 to processor 1 */
#define TCL_2          2 /* Assign tasks in TCL_2 to processor 2 */
#define TCL_3          3 /* Assign tasks in TCL_3 to processor 3 */
#define TCL_4          4 /* Assign tasks in TCL_4 to processor 4 */

#define TCL_1_ONLY     5 /* The same but the task cannot migrate */
#define TCL_2_ONLY     6 /* The same but the task cannot migrate */
#define TCL_3_ONLY     7 /* The same but the task cannot migrate */
#define TCL_4_ONLY     8 /* The same but the task cannot migrate */
#define TCL_SYSTIM_PRC 9 /* Used for global system timer event handler */

/*
 *  Include shared definitions with the chip-dependent code
 */
#include "arm_gcc/mpcore/chip_kernel.h"

#endif /* TOPPERS_TARGET_KERNEL_H */
