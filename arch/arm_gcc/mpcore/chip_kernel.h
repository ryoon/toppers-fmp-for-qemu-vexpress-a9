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
 *  @(#) $Id: chip_kernel.h 955 2012-10-30 07:31:22Z ertl-honda $
 */

/*
 *  kernel.hのチップ依存部（MPCORE用）
 *
 *  このインクルードファイルは，kernel.hでインクルードされる．他のファ
 *  イルから直接インクルードすることはない．このファイルをインクルード
 *  する前に，t_stddef.hがインクルードされるので，それらに依存してもよ
 *  い．
 */

#ifndef TOPPERS_CHIP_KERNEL_H
#define TOPPERS_CHIP_KERNEL_H

#include "mpcore.h"

/*
 *  ARMで共通な定義
 */
#include "arm_gcc/common/core_kernel.h"

/*
 *  サポートする機能の定義
 */
#define TOPPERS_TARGET_SUPPORT_ENA_INT			/* ena_intをサポートする */
#define TOPPERS_TARGET_SUPPORT_DIS_INT			/* dis_intをサポートする */
#ifdef TOPPERS_SYSTIM_LOCAL
#define TOPPERS_TARGET_SUPPORT_GET_UTM			/* get_utmをサポートする */
#endif /* TOPPERS_SYSTIM_LOCAL */

/*
 *  Customのアイドル処理を使用
 */
#define TOPPERS_CUSTOM_IDLE

/*
 *  割込み優先度の範囲
 */
#if __TARGET_ARCH_ARM == 6
#define TMIN_INTPRI   (-15)   /* 割込み優先度の最小値（最高値）*/
#else /* __TARGET_ARCH_ARM == 7 */
#define TMIN_INTPRI   (-31)   /* 割込み優先度の最小値（最高値）*/
#endif /* __TARGET_ARCH_ARM == 7 */
#define TMAX_INTPRI   (-1)    /* 割込み優先度の最大値（最低値）*/

/*
 *  タイムティックの定義
 */
#define TIC_NUME  1U   /* タイムティックの周期の分子 */
#define TIC_DENO  1U   /* タイムティックの周期の分母 */

#ifndef TOPPERS_MACRO_ONLY
#if __TARGET_ARCH_ARM == 7
/*
 *  性能評価用の関数
 *  CA9/MPCore の r1以降に搭載されている Global Timer を使用する．
 */

/* 性能計測用のカウンタのデータ型 */
typedef uint64_t PERFCNT;

/*
 *  パフォーマンスカウンタの初期化
 */
Inline void
x_init_pcc(void)
{
	/* カウント停止 */
	sil_wrw_mem((void *)MPCORE_GTC_CNT, 0);

	/* カウンターレジスタの初期化 */
	sil_wrw_mem((void *)MPCORE_GTC_COUNT_L, 0);
	sil_wrw_mem((void *)MPCORE_GTC_COUNT_U, 0);

	/* コンペアバリューレジスタの初期化 */
	sil_wrw_mem((void *)MPCORE_GTC_COMPV_L, 0xffffffff);
	sil_wrw_mem((void *)MPCORE_GTC_COMPV_U, 0xffffffff);

	/* カウント開始 */
	sil_wrw_mem((void *)MPCORE_GTC_CNT, MPCORE_GTC_CNT_TIMENA);
}

/*
 *  パフォーマンスカウンタの読み込み
 */
Inline void
x_get_pcc(PERFCNT *p_count)
{
	uint32_t count_l;
	uint32_t count_h_p, count_h;

	/*
	 * 32bitカウンタ2個で64bitカウンタを実現されているため，
	 * 桁上げを考慮した読み込みとする．
	 */
	count_h_p = sil_rew_mem((void *)MPCORE_GTC_COUNT_U);
	count_l = sil_rew_mem((void *)MPCORE_GTC_COUNT_L);
	count_h = sil_rew_mem((void *)MPCORE_GTC_COUNT_U);
	if (count_h_p != count_h) {
		count_l = sil_rew_mem((void *)MPCORE_GTC_COUNT_L);
	}

	*p_count = count_h;
	*p_count = *p_count << 32;
	*p_count += count_l;
}

/*
 *  パフォーマンスカウンタのリセット
 */
Inline void
x_rst_pcc(void)
{
	uint32_t tmp, tmp1;

	/* カウンタ停止 */
	tmp = sil_rew_mem((void *)MPCORE_GTC_CNT);
	tmp1 = tmp & ~MPCORE_GTC_CNT_TIMENA;
	sil_wrw_mem((void *)MPCORE_GTC_CNT, tmp1);

	/* カウンターレジスタの初期化 */
	sil_wrw_mem((void *)MPCORE_GTC_COUNT_L, 0);
	sil_wrw_mem((void *)MPCORE_GTC_COUNT_U, 0);

	/* カウンタ再開 */
	sil_wrw_mem((void *)MPCORE_GTC_CNT, tmp);
}

/*
 *  カウンタ値のnsecへの変換
 */
Inline uint64_t
x_cnv_nsec(PERFCNT count) {
	return (count * (1000U / MPCORE_GTC_CLOCK_FREQ_MHZ));
}

#endif /* __TARGET_ARCH_ARM == 7 */
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS__KERNEL_H */
