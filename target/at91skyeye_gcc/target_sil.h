/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_sil.h 881 2011-12-31 14:06:52Z ertl-honda $
 */

/*
 *  sil.hのチップ依存部（AT91SKYEYE用）
 *
 *  このインクルードファイルは，sil.hの先頭でインクルードされる．他のファ
 *  イルからは直接インクルードすることはない．このファイルをインクルー
 *  ドする前に，t_stddef.hがインクルードされるので，それらに依存しても
 *  よい．
 */

#ifndef TOPPERS_TARGET_SIL_H
#define TOPPERS_TARGET_SIL_H

/*
 *  チップのエンディアン
 */
#define SIL_ENDIAN_LITTLE             /* リトルエンディアン */

/*
 *  ARMで共通な定義
 */
#include "core_sil.h"

#ifndef TOPPERS_MACRO_ONLY

/*
 *  プロセッサIDハードウェアのアドレス
 */
#define TOPPERS_PRC_ID_ADDR 0xFFFFFFE0

/*
 *  プロセッサIDの取得
 */
Inline void
sil_get_pid(ID *p_prcid)
{
	/*
	 * 最適化を有効にするため，silでは呼び出していない
	 */
	*p_prcid = (*((uint32_t *)TOPPERS_PRC_ID_ADDR) + 1); 
}

/*
 *  システムインタフェースレイヤーで使用する Mutex の ID
 */
#define TOPPERS_SIL_LOC_MUTEX_ID  10

/*
 *  Mutex用ハードウェア
 */
#define TOPPERS_MUTEX_ID_REG      (0xFFFFFF00)
#define TOPPERS_MUTEX_CNT_REG     (0xFFFFFF04)

/*
 *  SIL_LOC_SPN()を取得したプロセッサの管理（target_config.c）
 */
extern uint32_t TOPPERS_spn_var;

/*
 *  スピンロックの取得（silが使用できないので直接アクセス）
 */
Inline uint32_t
TOPPERS_sil_loc_spn(void)
{
	uint32_t irq_fiq_mask;
	ID       prcid;

  retry:
	/* 全割込みロック状態へ*/
	irq_fiq_mask = TOPPERS_disint();

	/* スピンロックの取得 */
	*((volatile unsigned int *)TOPPERS_MUTEX_ID_REG) = TOPPERS_SIL_LOC_MUTEX_ID;

	if (*((volatile unsigned int *)TOPPERS_MUTEX_CNT_REG) != 1) {
		/* スピンロックが取得できなかった場合 */
		/* 割込みロック状態を前の状態へ */
		TOPPERS_set_fiq_irq(irq_fiq_mask);
		goto retry;
	}

	ARM_MEMORY_CHANGED;

	sil_get_pid(&prcid);
	TOPPERS_spn_var = prcid;

	return irq_fiq_mask;
}

/*
 *  スピンロックの返却
 */
Inline void
TOPPERS_sil_unl_spn(uint32_t irq_fiq_mask)
{
	TOPPERS_spn_var = 0;

	ARM_MEMORY_CHANGED;

	*((volatile unsigned int *)TOPPERS_MUTEX_ID_REG) = TOPPERS_SIL_LOC_MUTEX_ID;
	*((volatile unsigned int *)TOPPERS_MUTEX_CNT_REG) = 1;

	/* スピンロックの取得前の状態へ */
	TOPPERS_set_fiq_irq(irq_fiq_mask);
}

/*
 *  スピンロックの強制解放
 *  自プロセッサがスピンロックを取得している場合に解放する． 
 */
Inline void
TOPPERS_sil_force_unl_spn(void)
{
	ID       prcid;

	sil_get_pid(&prcid);
	if (TOPPERS_spn_var == prcid) {
		*((volatile unsigned int *)TOPPERS_MUTEX_ID_REG) = TOPPERS_SIL_LOC_MUTEX_ID;
		*((volatile unsigned int *)TOPPERS_MUTEX_CNT_REG) = 1;
	}
}

/*
 *  スピンロック
 */
#define SIL_LOC_SPN() ((void)(TOPPERS_irq_fiq_mask = TOPPERS_sil_loc_spn()))
#define SIL_UNL_SPN() (TOPPERS_sil_unl_spn(TOPPERS_irq_fiq_mask))

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_SIL_H */
