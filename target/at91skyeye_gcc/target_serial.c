/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007 by GJ Business Division RICOH COMPANY,LTD. JAPAN  
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
 *  @(#) $Id: target_serial.c 926 2012-03-30 10:18:29Z ertl-honda $
 */

/*
 *  シリアルI/Oデバイス（SIO）ドライバ（AT91SKYEYE用）
 */

#include <kernel.h>
#include <t_syslog.h>
#include "at91skyeye.h"
#include "target_serial.h"
#include <target_syssvc.h>

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block 
{
	void* us_thr;
	void* us_rpr;
	void* us_rcr;
	void* us_csr;
	uint_t intno;
}
SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block 
{
	const SIOPINIB  *p_siopinib;  /* シリアルI/Oポート初期化ブロック */
	intptr_t        exinf;        /* 拡張情報 */
	uint8_t usart_rev_buf;        /* 受信用バッファ */
	bool_t    openflag;           /* オープン済みフラグ */
	bool_t    sendflag;           /* 送信割込みイネーブルフラグ */
	bool_t    getready;           /* 文字を受信した状態 */
	bool_t    putready;           /* 文字を送信できる状態 */
};

/*
 * シリアルI/Oポート初期化ブロック
 */
static const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{
		(void*)(USART0_THR),
		(void*)(USART0_RPR),
		(void*)(USART0_RCR),
		(void*)(USART0_CSR),
		(uint_t)(INTNO_SIO_PRC1),
	},
#if TNUM_SIOP >= 2
	{
		(void*)(USART0_THR),
		(void*)(USART0_RPR),
		(void*)(USART0_RCR),
		(void*)(USART0_CSR),
		(uint_t)(INTNO_SIO_PRC2),
	},
#endif /* TNUM_SIOP >= 2 */
#if TNUM_SIOP >= 3
	{
		(void*)(USART0_THR),
		(void*)(USART0_RPR),
		(void*)(USART0_RCR),
		(void*)(USART0_CSR),
		(uint_t)(INTNO_SIO_PRC3),
	},
#endif /* TNUM_SIOP >= 3 */
#if TNUM_PORT >= 4
	{
		(void*)(USART0_THR),
		(void*)(USART0_RPR),
		(void*)(USART0_RCR),
		(void*)(USART0_CSR),
		(uint_t)(INTNO_SIO_PRC4),
	}
#endif /* TNUM_SIOP >= 4 */
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SEC_SERIAL1(static SIOPCB, siopcb1);
#if TNUM_SIOP >= 2
SEC_SERIAL2(static SIOPCB, siopcb2);
#endif /* TNUM_SIOP >= 2 */
#if TNUM_SIOP >= 3
SEC_SERIAL3(static SIOPCB, siopcb3);
#endif /* TNUM_SIOP >= 3 */
#if TNUM_SIOP >= 4
SEC_SERIAL4(static SIOPCB, siopcb4);
#endif /* TNUM_SIOP >= 4 */

static SIOPCB* const p_siopcb_table[TNUM_SIOP] = {
	&siopcb1
#if TNUM_SIOP >= 2
	,&siopcb2
#endif /* TNUM_SIOP >= 2 */
#if TNUM_SIOP >= 3
	,&siopcb3
#endif /* TNUM_SIOP >= 3 */
#if TNUM_SIOP >= 4
	,&siopcb4
#endif /* TNUM_SIOP >= 4 */
};

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((uint_t)((siopid) - 1))
#define get_siopcb(siopid)  (p_siopcb_table[INDEX_SIOP(siopid)])


void
at91skyeye_init_uart(uint_t siopid)
{
	SIOPCB *p_siopcb = get_siopcb(siopid);
      
	/* 受信データの格納先アドレスの設定 */
	sil_wrw_mem((void *)(USART0_RPR), (uint32_t)(&(p_siopcb->usart_rev_buf)));
	sil_wrw_mem((void *)(USART0_RCR), 0x0001);
}

/*
 * 文字を受信したか?
 */ 
Inline bool_t
uart_getready(SIOPCB *p_siopcb)
{
	return(sil_rew_mem((void*)(p_siopcb->p_siopinib->us_rcr)) == 0);
}

/*
 * 文字を送信できるか?
 */
Inline bool_t
uart_putready(SIOPCB *p_siopcb)
{
	return(true);
}

/*
 *  受信した文字の取り出し
 */
Inline uint8_t
uart_getchar(SIOPCB *p_siopcb)
{
	char_t c;
     
	c = p_siopcb->usart_rev_buf;
	sil_wrw_mem((void *)(p_siopcb->p_siopinib->us_rpr), (uint32_t)(&(p_siopcb->usart_rev_buf)));
	sil_wrw_mem((void *)(p_siopcb->p_siopinib->us_rcr), 0x0001);
    
	return(c);
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *p_siopcb, uint8_t c)
{
	sil_wrw_mem((void*)(p_siopcb->p_siopinib->us_thr),c);
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *p_siopcb)
{

}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *p_siopcb)
{

}


/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *p_siopcb)
{

}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *p_siopcb)
{

}


/*
 *  SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	uint_t  i = (uint_t)exinf;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	(p_siopcb_table[i])->p_siopinib = &(siopinib_table[i]);
	(p_siopcb_table[i])->openflag = false;
	(p_siopcb_table[i])->sendflag = false;
}

/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
at91skyeye_uart_opn_por(SIOPCB *p_siopcb, intptr_t exinf)
{   
	p_siopcb->exinf = exinf;
	p_siopcb->getready = p_siopcb->putready = false;
	p_siopcb->openflag = true;

	return(p_siopcb);
}


/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB  *p_siopcb = get_siopcb(siopid);
	bool_t    opnflg;
	ER      ercd;
    
	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = p_siopcb->openflag;

	/*
	 *  デバイス依存のオープン処理．
	 */
	at91skyeye_uart_opn_por(p_siopcb, exinf);

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		ercd = ena_int(p_siopcb->p_siopinib->intno);
		assert(ercd == E_OK);
	}
	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	/*
	 *  デバイス依存のクローズ処理．
	 */
	p_siopcb->openflag = false;
    
	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	dis_int(p_siopcb->p_siopinib->intno);
}


/*
 *  SIOの割込みハンドラ
 */
void
sio_handler(void)
{
	SIOPCB *p_siopcb = get_siopcb((x_prc_index() + 1));
	if (uart_getready(p_siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sio_irdy_rcv(p_siopcb->exinf);
	}
	if (uart_putready(p_siopcb)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
		sio_irdy_snd(p_siopcb->exinf);
	}
}


/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char_t c)
{    
	if (uart_putready(siopcb)){
		uart_putchar(siopcb, c);
		return(true);
	}
	return(false);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
	if (uart_getready(siopcb)) {
		return((int_t)(uint8_t) uart_getchar(siopcb));
	}
	return(-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_enable_send(siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_enable_rcv(siopcb);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		uart_disable_send(siopcb);
		break;
	  case SIO_RDY_RCV:
		uart_disable_rcv(siopcb);
		break;
	}
}
