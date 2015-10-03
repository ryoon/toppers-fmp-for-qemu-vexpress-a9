/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2010 by Embedded and Real-Time Systems Laboratory
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
 *  This file contains calls to the ARM PL011 UART driver
 *  (pdic/arm_primecell_uart.c) in order to implement the FMP serial
 *  input/output (SIO) interface.
 */
#include <kernel.h>
#include <t_syslog.h>
#include "target_kernel.h"  /* for TOPPERS_MASTER_PRCID */
#include "target_serial.h"  /* for prototypes */
#include "pdic/arm_primecell_uart/arm_primecell_uart.h" /* arm_primexxx */

/*
 *  Target-dependent early system initialization functions
 */
#ifdef G_SYSLOG
#define my_siopid        1
#else
#define my_siopid        (x_prc_index() + 1)
#endif

void vexpressa9_uart_reset(void)
{
        arm_primecell_uart_reset(my_siopid);
}

void vexpressa9_pol_putc(char_t c)
{
        arm_primecell_uart_pol_putc(my_siopid, c);
}

void vexpressa9_uart_init(intptr_t exinf)
{
        arm_primecell_uart_initialize();
}

/*
 * SIO INTERFACE IMPLEMENTATION
 */

/*
 * Open the serial port
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB  *p_siopcb;
	bool_t    opnflg;
	ER      ercd;

	opnflg = arm_primecell_uart_openflag(siopid);
	p_siopcb = arm_primecell_uart_opn_por(siopid, exinf);

	if (!opnflg) {
		ercd = ena_int(arm_primecell_uart_get_intno(p_siopcb));
		assert(ercd == E_OK);
	}

	return(p_siopcb);
}

/*
 * Close the serial port
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	ID siopid = arm_primecell_uart_get_siopid(p_siopcb);

	arm_primecell_uart_cls_por(p_siopcb);
	if (!arm_primecell_uart_openflag(siopid)) {
		dis_int(arm_primecell_uart_get_intno(p_siopcb));
	}
}

/*
 *  SIO interrupt service routine (used in target_serial.cfg)
 */
void
sio_isr(intptr_t exinf)
{
	arm_primecell_uart_isr(exinf);
}

/*
 * Send a character through the serial port
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char_t c)
{
	return(arm_primecell_uart_snd_chr(siopcb, c));
}

/*
 * Receive a character through the serial port
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
	return(arm_primecell_uart_rcv_chr(siopcb));
}

/*
 *  Enable callbacks from the serial port hardware when a character is
 *  received (cbrtn=SIO_RDY_RCV) or sent (cbrtn==SIO_RDY_SND).
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	arm_primecell_uart_ena_cbr(siopcb, cbrtn);
}

/*
 *  Disable callbacks from the serial port hardware.
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	arm_primecell_uart_dis_cbr(siopcb, cbrtn);
}

/*
 *  Transmission-allowed callback from the serial port
 */
void
arm_primecell_uart_irdy_snd(intptr_t exinf)
{
	sio_irdy_snd(exinf);
}

/*
 *  Reception-notice callback from the serial port
 */
void
arm_primecell_uart_irdy_rcv(intptr_t exinf)
{
	sio_irdy_rcv(exinf);
}
