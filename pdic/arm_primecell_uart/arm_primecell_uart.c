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
 *  @(#) $Id: arm_primecell_uart.c 961 2012-12-04 00:29:28Z ertl-sangorrin $
 */

/*
 *   ARM PrimeCell UART用 簡易SIOドライバ
 */

#include <sil.h>
#include "target_syssvc.h"
#include "arm_primecell_uart.h"

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
    void  *dr;    /* データレジスタ */
    void  *rsr;   /* 受信ステータスレジスタ */
    void  *fr;    /* フラグレジスタ */
    void  *ibrd;  /* 整数ボーレートレジスタ */
    void  *fbrd;  /* 分数ボーレートレジスタ */
    void  *lcr_h; /* ラインコントロールレジスタ(H) */
    void  *cr;    /* コントロールレジスタ */
    void  *imsc;  /* 割込みマスクレジスタ */
    uint8_t lcr_h_def; /* ラインコントロールレジスタの設定値 */
    uint8_t ibrd_def;  /* 整数ボーレートレジスタの設定値 */
    uint8_t fbrd_def;  /* 分数ボーレートレジスタの設定値 */
    uint32_t intno;    /* 割込み番号 */
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
    const SIOPINIB  *siopinib;  /* シリアルI/Oポート初期化ブロック */
    intptr_t  exinf;            /* 拡張情報 */
    bool_t    openflag;           /* オープン済みフラグ */
    bool_t    sendflag;           /* 送信割込みイネーブルフラグ */
    bool_t    getready;           /* 文字を受信した状態 */
    bool_t    putready;           /* 文字を送信できる状態 */
};

/*
 *  シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {(void *)(UART0_BASE + UART_DR),
     (void *)(UART0_BASE + UART_RSR),
     (void *)(UART0_BASE + UART_FR),
     (void *)(UART0_BASE + UART_IBRD),
     (void *)(UART0_BASE + UART_FBRD),
     (void *)(UART0_BASE + UART_LCR_H),
     (void *)(UART0_BASE + UART_CR),
     (void *)(UART0_BASE + UART_IMSC),
     UART_LCR_H_WLEN_8,
     UART_IBRD_38400,
     UART_FBRD_38400,
     INTNO_SIO0,
    },
#if TNUM_PORT >= 2
    {(void *)(UART1_BASE + UART_DR),
     (void *)(UART1_BASE + UART_RSR),
     (void *)(UART1_BASE + UART_FR),
     (void *)(UART1_BASE + UART_IBRD),
     (void *)(UART1_BASE + UART_FBRD),
     (void *)(UART1_BASE + UART_LCR_H),
     (void *)(UART1_BASE + UART_CR),
     (void *)(UART1_BASE + UART_IMSC),
     UART_LCR_H_WLEN_8,
     UART_IBRD_38400,
     UART_FBRD_38400,
     INTNO_SIO1,
    },
#endif /* TNUM_PORT >= 2 */
#if TNUM_PORT >= 3
    {(void *)(UART2_BASE + UART_DR),
     (void *)(UART2_BASE + UART_RSR),
     (void *)(UART2_BASE + UART_FR),
     (void *)(UART2_BASE + UART_IBRD),
     (void *)(UART2_BASE + UART_FBRD),
     (void *)(UART2_BASE + UART_LCR_H),
     (void *)(UART2_BASE + UART_CR),
     (void *)(UART2_BASE + UART_IMSC),
     UART_LCR_H_WLEN_8,
     UART_IBRD_38400,
     UART_FBRD_38400,
     INTNO_SIO2,
    },
#endif /* TNUM_PORT >= 3 */
#if TNUM_PORT >= 4
    {(void *)(UART3_BASE + UART_DR),
     (void *)(UART3_BASE + UART_RSR),
     (void *)(UART3_BASE + UART_FR),
     (void *)(UART3_BASE + UART_IBRD),
     (void *)(UART3_BASE + UART_FBRD),
     (void *)(UART3_BASE + UART_LCR_H),
     (void *)(UART3_BASE + UART_CR),
     (void *)(UART3_BASE + UART_IMSC),
     UART_LCR_H_WLEN_8,
     UART_IBRD_38400,
     UART_FBRD_38400,
     INTNO_SIO3,
    }
#endif /* TNUM_PORT >= 4 */
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((uint_t)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  文字を受信できるか？
 */
Inline bool_t
arm_primecell_uart_getready(SIOPCB *p_siopcb)
{
    return((sil_rew_mem(p_siopcb->siopinib->fr) & UART_FR_RXFE) != UART_FR_RXFE);
}

/*
 *  文字を送信できるか？
 */
Inline bool_t
arm_primecell_uart_putready(SIOPCB *p_siopcb)
{
    return((sil_rew_mem(p_siopcb->siopinib->fr) & UART_FR_TXFF) != UART_FR_TXFF);
}

/*
 *  受信した文字の取出し
 */
Inline char_t
arm_primecell_uart_getchar(SIOPCB *p_siopcb)
{
    return((char_t)sil_rew_mem(p_siopcb->siopinib->dr));
}

/*
 *  送信する文字の書込み
 */
Inline void
arm_primecell_uart_putchar(SIOPCB *p_siopcb, char_t c)
{
    sil_wrw_mem(p_siopcb->siopinib->dr, c);
}

/*
 *  送信割込み許可
 */
Inline void
arm_primecell_uart_enable_send(SIOPCB *p_siopcb)
{
    sil_wrw_mem(p_siopcb->siopinib->imsc,
                sil_rew_mem(p_siopcb->siopinib->imsc) | UART_IMSC_TXIM);
}

/*
 *  送信割込み禁止
 */
Inline void
arm_primecell_uart_disable_send(SIOPCB *p_siopcb)
{
    sil_wrw_mem(p_siopcb->siopinib->imsc,
				sil_rew_mem(p_siopcb->siopinib->imsc) & ~UART_IMSC_TXIM);
}


/*
 *  受信割込み許可
 */
Inline void
arm_primecell_uart_enable_rcv(SIOPCB *p_siopcb)
{
	sil_wrw_mem(p_siopcb->siopinib->imsc,
				sil_rew_mem(p_siopcb->siopinib->imsc) | UART_IMSC_RXIM);
}

/*
 *  受信割込み禁止
 */
Inline void
arm_primecell_uart_disable_rcv(SIOPCB *p_siopcb)
{
	sil_wrw_mem(p_siopcb->siopinib->imsc,
				sil_rew_mem(p_siopcb->siopinib->imsc) & ~UART_IMSC_RXIM);
}

/*
 *  SIOドライバの初期化
 */
void
arm_primecell_uart_initialize(void)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
		p_siopcb->siopinib = &(siopinib_table[i]);
		p_siopcb->openflag = false;
		p_siopcb->sendflag = false;
	}
}

/*
 *  オープンしているポートがあるか？
 */
bool_t
arm_primecell_uart_openflag(ID siopid)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(siopid);

	return(p_siopcb->openflag);
}

/*
 *  Reset the PrimeCell UART PL011 hardware to a default status
 */
void
arm_primecell_uart_reset(ID siopid)
{
        char_t __attribute__((__unused__)) c;  /* for removing warning */
        int index;

        index = INDEX_SIOP(siopid);

        /* UART停止 */
        sil_wrw_mem(siopinib_table[index].cr, 0x00);

        /* エラーフラグをクリア */
        sil_wrw_mem(siopinib_table[index].rsr, 0x00);

        /* FIFOを空にする */
        while((sil_rew_mem(siopinib_table[index].fr) & UART_FR_RXFE) != UART_FR_RXFE) {
                /* バッファからの読み込み */
                c = sil_rew_mem(siopinib_table[index].dr);
        }

        /* ボーレートを設定 */
        sil_wrw_mem(siopinib_table[index].ibrd, UART_IBRD_38400);
        sil_wrw_mem(siopinib_table[index].fbrd, UART_FBRD_38400);

        /* データフォーマットと，FIFOのモードを設定 */
        sil_wrw_mem(siopinib_table[index].lcr_h, UART_LCR_H_WLEN_8);

        /* UART再開 */
        sil_wrw_mem(siopinib_table[index].cr,
                    (sil_rew_mem(siopinib_table[index].cr)
                    | UART_CR_RXE | UART_CR_TXE | UART_CR_UARTEN));
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
arm_primecell_uart_opn_por(ID siopid, intptr_t exinf)
{
        SIOPCB *p_siopcb;

        arm_primecell_uart_reset(siopid);

        p_siopcb = get_siopcb(siopid);
        p_siopcb->exinf = exinf;
        p_siopcb->getready = p_siopcb->putready = false;
        p_siopcb->openflag = true;

        return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
arm_primecell_uart_cls_por(SIOPCB *p_siopcb)
{
    /* UART停止 */
	sil_wrw_mem(p_siopcb->siopinib->cr, 0x00);
    p_siopcb->openflag = false;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
arm_primecell_uart_snd_chr(SIOPCB *p_siopcb, char_t c)
{
    if (arm_primecell_uart_putready(p_siopcb)){
        arm_primecell_uart_putchar(p_siopcb, c);
        return(true);
    }
    return(false);
}

/*
 * Put a character in polling mode
 */
void
arm_primecell_uart_pol_putc(ID siopid, char_t c)
{
    int index = INDEX_SIOP(siopid);

    /* Delay until the buffer has space */
    while((sil_rew_mem(siopinib_table[index].fr) & UART_FR_TXFF) == UART_FR_TXFF);
    /* Write the character to the data register */
    sil_wrw_mem(siopinib_table[index].dr, c);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
arm_primecell_uart_rcv_chr(SIOPCB *p_siopcb)
{
	if (arm_primecell_uart_getready(p_siopcb)) {
		return((int_t)(uint8_t) arm_primecell_uart_getchar(p_siopcb));
	}
	return(-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
arm_primecell_uart_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
      case SIO_RDY_SND:
        arm_primecell_uart_enable_send(p_siopcb);
        break;
      case SIO_RDY_RCV:
        arm_primecell_uart_enable_rcv(p_siopcb);
        break;
    }
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
arm_primecell_uart_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
    switch (cbrtn) {
      case SIO_RDY_SND:
        arm_primecell_uart_disable_send(p_siopcb);
        break;
      case SIO_RDY_RCV:
        arm_primecell_uart_disable_rcv(p_siopcb);
        break;
    }
}

/*
 *  シリアルI/Oポートに対する割込み処理
 */
static void
arm_primecell_uart_isr_siop(SIOPCB *p_siopcb)
{
    if (arm_primecell_uart_getready(p_siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        arm_primecell_uart_irdy_rcv(p_siopcb->exinf);
    }
    if (arm_primecell_uart_putready(p_siopcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        arm_primecell_uart_irdy_snd(p_siopcb->exinf);
    }
}

/*
 *  SIOの割込みサービスルーチン
 */
void
arm_primecell_uart_isr(intptr_t exinf)
{
    arm_primecell_uart_isr_siop(get_siopcb(exinf));
}

/*
 *  SIOPCBへのポインタからポート番号を返す
 */
ID
arm_primecell_uart_get_siopid(SIOPCB *p_siopcb)
{
	return (ID)((p_siopcb - siopcb_table) + 1);
}

/*
 *  割込み番号を返す
 */
uint8_t
arm_primecell_uart_get_intno(SIOPCB *p_siopcb)
{
	return p_siopcb->siopinib->intno;
}
