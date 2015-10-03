/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2008-2010 by Embedded and Real-Time Systems Laboratory
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
 *  Target-dependent part of the system services. Defines the
 *  number of serial ports available.
 */
#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

#include "vexpressa9.h"
#include "arm_gcc/mpcore/mpcore.h"

/*
 *  Enable trace (use the ENABLE_TRACE flag at the app's Makefile)
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  Name that appears in the initialization message to
 *  show the target system
 */
#define TARGET_NAME    "VEXPRESS-A9x4"

/*
 *  System log low-level character output
 */
extern void target_fput_log(char_t c);

/*
 *  Number of UARTs (used in target_serial.cfg and arm_primecell_uart.c)
 *
 *  If global output (G_SYSLOG) is selected only one UART will be used.
 *  Otherwise, each core will have its own UART.
 *
 *  Note: When SafeG is not used, G_SYSLOG can be defined by the user's
 *  application. When using SafeG, G_SYSLOG is defined (see Makefile.target)
 *  by default to make things easier. For more complicated scenarios
 *  (e.g., each world using 2 UARTS) you have to adapt this file manually.
 */
#ifdef G_SYSLOG
#define TNUM_PORT       1
#define TNUM_SIOP       1
#else
#define TNUM_PORT       TNUM_PRCID
#define TNUM_SIOP       TNUM_PRCID
#endif /* G_SYSLOG */

/*
 *  UART base address definitions (used in arm_primecell_uart.c)
 *
 *  Note: When using SafeG, G_SYSLOG is defined (see Makefile.target) so
 *  we select a single UART for each world (e.g. Secure uses UART1
 *  and Nonsecure uses UART0).
 */
#ifdef TOPPERS_NOSAFEG
#define     UART0_BASE  VEXPRESSA9_UART0_BASE
#define     UART1_BASE  VEXPRESSA9_UART1_BASE
#define     UART2_BASE  VEXPRESSA9_UART2_BASE
#define     UART3_BASE  VEXPRESSA9_UART3_BASE
#elif defined(TOPPERS_SAFEG_SECURE)
#define     UART0_BASE  VEXPRESSA9_UART1_BASE
#elif defined(TOPPERS_SAFEG_NONSECURE)
#define     UART0_BASE  VEXPRESSA9_UART0_BASE
#else
#error "Check your Makefile.target"
#endif

/*
 *  UART interrupt handler definitions (used in target_serial.cfg)
 *     INHNO: interrupt handler number
 *     INTNO: interrupt number
 *     INTPRI: interrupt priority (lower means higher priority)
 *     INTATR: interrupt attributes (0 means not enabled at the beginning)
 *
 *  Note: When using SafeG, G_SYSLOG is defined (see Makefile.target) so
 *  we select a single UART for each world (e.g. Secure uses UART0
 *  and Nonsecure uses UART1).
 */
#ifdef TOPPERS_NOSAFEG
#define    INHNO_SIO0  VEXPRESSA9_UART0_IRQ
#define    INTNO_SIO0  VEXPRESSA9_UART0_IRQ
#define    INTPRI_SIO0    -2
#define    INTATR_SIO0     0U
#define    INHNO_SIO1  VEXPRESSA9_UART1_IRQ
#define    INTNO_SIO1  VEXPRESSA9_UART1_IRQ
#define    INTPRI_SIO1    -2
#define    INTATR_SIO1     0U
#define    INHNO_SIO2  VEXPRESSA9_UART2_IRQ
#define    INTNO_SIO2  VEXPRESSA9_UART2_IRQ
#define    INTPRI_SIO2    -2
#define    INTATR_SIO2     0U
#define    INHNO_SIO3  VEXPRESSA9_UART3_IRQ
#define    INTNO_SIO3  VEXPRESSA9_UART3_IRQ
#define    INTPRI_SIO3    -2
#define    INTATR_SIO3     0U
#elif defined(TOPPERS_SAFEG_SECURE)
#define    INHNO_SIO0  VEXPRESSA9_UART1_IRQ
#define    INTNO_SIO0  VEXPRESSA9_UART1_IRQ
#define    INTPRI_SIO0    -19
#define    INTATR_SIO0     0U
#elif defined(TOPPERS_SAFEG_NONSECURE)
#define    INHNO_SIO0  VEXPRESSA9_UART0_IRQ
#define    INTNO_SIO0  VEXPRESSA9_UART0_IRQ
#define    INTPRI_SIO0    -2
#define    INTATR_SIO0     0U
#else
#error "Check your Makefile.target"
#endif

/*
 *  Bits per second UART value for 38400bps
 */
#define UART_IBRD_38400   0x0c
#define UART_FBRD_38400   0x00

#endif /* TOPPERS_TARGET_SYSSVC_H */
