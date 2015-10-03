/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_ipi.h 955 2012-10-30 07:31:22Z ertl-honda $
 */

/*
 *  プロセッサ間割込みドライバ（MPCORE用）
 */

#ifndef TOPPERS_CHIP_IPI_H
#define TOPPERS_CHIP_IPI_H

#include "mpcore.h"

/*
 *  ディスパッチ要求用プロセッサ間割込みの割込み番号
 */
#define INTNO_IPI_DIS_PRC1   (0x00010000 | IPINO_DIS)
#define INTNO_IPI_DIS_PRC2   (0x00020000 | IPINO_DIS)
#define INTNO_IPI_DIS_PRC3   (0x00030000 | IPINO_DIS)
#define INTNO_IPI_DIS_PRC4   (0x00040000 | IPINO_DIS)

/*
 *  カーネル終了用のプロセッサ間割込みの割込み番号
 */
#define INTNO_IPI_EXT_PRC1   (0x00010000 | IPINO_EXT)
#define INTNO_IPI_EXT_PRC2   (0x00020000 | IPINO_EXT)
#define INTNO_IPI_EXT_PRC3   (0x00030000 | IPINO_EXT)
#define INTNO_IPI_EXT_PRC4   (0x00040000 | IPINO_EXT)

/*
 *  ディスパッチ要求用のプロセッサ間割込みの割込みハンドラ番号
 */
#define INHNO_IPI_DIS_PRC1   (0x00010000 | IPINO_DIS)
#define INHNO_IPI_DIS_PRC2   (0x00020000 | IPINO_DIS)
#define INHNO_IPI_DIS_PRC3   (0x00030000 | IPINO_DIS)
#define INHNO_IPI_DIS_PRC4   (0x00040000 | IPINO_DIS)

/*
 *  カーネル終了用のプロセッサ間割込みの割込みハンドラ番号
 */
#define INHNO_IPI_EXT_PRC1   (0x00010000 | IPINO_EXT)
#define INHNO_IPI_EXT_PRC2   (0x00020000 | IPINO_EXT)
#define INHNO_IPI_EXT_PRC3   (0x00030000 | IPINO_EXT)
#define INHNO_IPI_EXT_PRC4   (0x00040000 | IPINO_EXT)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ディスパッチ用プロセッサ間割込みの発行
 */
Inline void
target_ipi_raise(uint_t prcid)
{
	dic_swi((1<<(prcid-1)), IPINO_DIS);
}

/*
 *  ディスパッチ用プロセッサ間割込みのクリア
 */
Inline void
target_ipi_clear(void)
{

}

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  カーネル終了処理用のプロセッサ間割込みハンドラ
 */
extern void _kernel_ipi_ext_handler(void);
#endif /* USE_IPI_DIS_HANDER_BYPASS */

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_CHIP_IPI_H */
