/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chip_config.c 955 2012-10-30 07:31:22Z ertl-honda $
 */

/*
 * ターゲット依存モジュール（MPCORE用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "mp.h"
#include "target_ipi.h"

#ifdef TOPPERS_ENA_PERF
extern PERFCNT perf_boot_time[];
#endif /* TOPPERS_ENA_PERF */

/*
 *  SILのスピンロック用変数
 */
uint32_t TOPPERS_spn_var;

#ifdef USE_IPI_DIS_HANDER_BYPASS
/*
 *  終了処理開始フラグ
 */
static bool_t ext_ker_reqflg;
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  str_ker() の実行前にマスタプロセッサのみ実行される初期化処理
 */
void
chip_mprc_initialize(void)
{
#ifdef TOPPERS_ENA_PERF
	x_get_pcc(&perf_boot_time[0]);
	x_rst_pcc();
#endif /* TOPPERS_ENA_PERF */

	/*
	 *  DICの初期化
	 */
	dic_init();

	/*
	 *  SCUの初期化
	 */
	scu_init();

	/*
	 *  SILのスピンロック用変数の初期化
	 */
	TOPPERS_spn_var = 0U;

#ifdef USE_IPI_DIS_HANDER_BYPASS
	/*
	 *  終了処理開始フラグの初期化
	 */
	ext_ker_reqflg = false;
#endif /* USE_IPI_DIS_HANDER_BYPASS */
}

/*
 *  ベクターテーブル（chip_support.S）
 */
extern void *vector_table;

/*
 *  チップ依存の初期化
 */
void
chip_initialize(void)
{
	TPCB *p_tpcb = get_my_p_tpcb();
	uint32_t index = x_prc_index();

#ifdef TOPPERS_ENA_PERF
	if (x_sense_mprc()) {
		x_get_pcc(&perf_boot_time[1]);
		x_rst_pcc();
	}
#endif /* TOPPERS_ENA_PERF */

#ifdef USE_THREAD_ID_PCB
	/*
	 *  Thread ID レジスタへのPCBへのポインタの設定
	 */
	CPU15_PONLY_THREAD_ID_WRITE((uint32_t)p_pcb_table[x_prc_index()]);
#endif /* USE_THREAD_ID_PCB */

	p_tpcb = get_my_p_tpcb();

	/*
	 * ARM依存の初期化
	 */
	core_initialize();

	/*
	 * キャッシュを無効に
	 */
	mpcore_cache_disable();

	/*
	 * MMUを有効に
	 */
	mpcore_mmu_init();

	/*
	 * SMPモードへ
	 */
	mpcore_smp_mode_enable();

	/*
	 * キャッシュを有効に
	 */
	mpcore_cache_enable();

	/*
	 *  DICの各コアのSGIとPPIの設定
	 */
	dic_sgi_ppi_init();

	/*
	 * DICのCPUインタフェース初期化
	 */
	dic_cpu_init();

	/*
	 *  割込みハンドラテーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_tbl = p_inh_table[index];

#if defined(TOPPERS_SAFEG_SECURE) | defined(TOPPERS_SAFEG_NONSECURE)
	CP15_SET_VBAR((uint32_t) &vector_table);
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef TOPPERS_ENA_PERF
	if (x_sense_mprc()) {
		x_get_pcc(&perf_boot_time[2]);
		x_rst_pcc();
	}
#endif /* TOPPERS_ENA_PERF */
}

/*
 *  チップ依存の終了処理
 */
void
chip_exit(void)
{
	/*
	 *  ARM依存の終了処理
	 */
	core_terminate();

	/*
	 *  DICのCPUインタフェースを停止
	 */
	dic_cpu_stop();

	/*
	 *  DICのDISインタフェースを停止
	 */
	if (x_sense_mprc()) {
		dic_stop();
	}
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  ASPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	assert(VALID_INTNO_CFGINT(ID_PRC(x_prc_index()), intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込み要求のマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	x_disable_int(intno);

	/*
	 * 属性を設定
	 */
	if ((intatr & TA_EDGE) != 0U) {
		dic_config(INTNO_MASK(intno), DIC_ICR_EDGE);
		x_clear_int(intno);
	}
	else {
		dic_config(INTNO_MASK(intno), DIC_ICR_LEVEL);
	}

	/*
	 * 割込み優先度マスクの設定
	 */
#ifdef TOPPERS_SAFEG_SECURE
	if (intpri > -16) {
		syslog_2(LOG_EMERG, "Error! Secure Priority smaller than -16 but intno 0x%x priority is %d.", intno, intpri);
		target_exit();
	}
#endif /* TOPPERS_SAFEG_SECURE */
	dic_set_priority(INTNO_MASK(intno), INT_IPM(intpri));

	/*
	 * ターゲットCPUの設定（グローバル割込みのみ）
	 */
	if (INTNO_MASK(intno) >= TMIN_GLOBAL_INTNO) {
		dic_set_target(INTNO_MASK(intno), (1 << x_prc_index()));
	}

	/*
	 *  Secureの設定
	 */
#ifdef TOPPERS_SAFEG_SECURE
	dic_set_security(INTNO_MASK(intno), 0);
#endif /* TOPPERS_SAFEG_SECURE */

	/*
	 * 割込みを許可
	 */
	if ((intatr & TA_ENAINT) != 0U) {
		(void)x_enable_int(intno);
	}
}

#ifndef OMIT_DEFAULT_INT_HANDLER

/*
 *  未定義の割込みが入った場合の処理
 */
void
default_int_handler(void){
	ID prc_id = ID_PRC(x_prc_index());
	
	syslog_1(LOG_EMERG, "Processor %d : Unregistered Interrupt occurs.", prc_id);
	target_exit();
}

#endif /* OMIT_DEFAULT_INT_HANDLER */

#ifdef USE_IPI_DIS_HANDER_BYPASS

/*
 *  カーネル終了処理要求
 */
void
ext_ker_request(void)
{
	ID prcid;
	volatile int i;

	/* すでに要求が出ていればリターン */
	if (ext_ker_reqflg) {
		return;
	}

	ext_ker_reqflg = true;

	for(prcid = 1; prcid <= TNUM_PRCID; prcid++){
		if (prcid != ID_PRC(x_prc_index())) {
			dic_swi((1<<(prcid-1)), IPINO_EXT);
			/*
			 * dic_swi()を連続して発行すると割込みを正しく付けないプロセッ
			 * サがいるためループを挿入
			 */
			for(i = 0; i < 10; i++);
		}
	}
}

/*
 *  カーネル終了処理用のプロセッサ間割込みハンドラ
 */
void
ipi_ext_handler(void)
{
	ext_ker();
}
#endif /* USE_IPI_DIS_HANDER_BYPASS */

#ifdef TOPPERS_ENA_PERF
void
perf_boot_init(void)
{
	if (x_sense_mprc()) {
		x_init_pcc();
		x_rst_pcc();
	}
}
#endif /* TOPPERS_ENA_PERF */
