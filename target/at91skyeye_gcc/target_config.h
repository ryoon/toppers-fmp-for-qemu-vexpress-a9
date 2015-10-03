/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007-2011 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 926 2012-03-30 10:18:29Z ertl-honda $
 */

/*
 *  チップ依存モジュール（AT91SKYEYE用）
 *
 *  カーネルのチップ依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 *
 *  UMA前提でデータ構造を生成
 *  -> 要素毎の配列とする． 
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  チップシステムのハードウェア資源の定義
 */
#include "at91skyeye.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  サポートしているSKYEYのバージョン
 */
#define SUPPORT_SKYEYE_VER  0x1024
#define SUPPORT_DEVM_VER    0x2020

/*
 *  スピンロック機能の方式
 */
#ifndef TTYPE_SPN
#define TTYPE_SPN NATIVE_SPN
//#define TTYPE_SPN EMULATE_SPN
#endif /* TTYPE_SPN */

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 */
#define TMAX_NATIVE_SPN   4

/*
 *  ロックのID
 */
#if TTYPE_KLOCK == G_KLOCK

/* ジャイアントロック方式 */
#define GIANT_LOCK  0x00

#else /* TTYPE_KLOCK == P_KLOCK */

/* プロセッサロック方式 */
#define PRC1_TSK_LOCK  0x00
#define PRC2_TSK_LOCK  0x02
#define PRC3_TSK_LOCK  0x04
#define PRC4_TSK_LOCK  0x06
#define PRC1_OBJ_LOCK  0x01
#define PRC2_OBJ_LOCK  0x03
#define PRC3_OBJ_LOCK  0x05
#define PRC4_OBJ_LOCK  0x07

#endif /* TTYPE_KLOCK == G_KLOCK */

/*
 *  ベクタールーチンを持たない場合のベクターアドレスの先頭番地
 */ 
#define VECTOR_START  0x00

/*
 *  データセクションの初期化をしない
 */
#define TOPPERS_OMIT_DATA_INIT

/*
 *  FMPカーネル動作時のメモリマップと関連する定義
 */
#define RAM_START       RAM_BASE_ADDRESSS

#define FIQ_DATA_SIZE   256U

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_PRC1_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC2_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC3_ISTKSZ      (1024 * 8)   /* 8KB */
#define DEFAULT_PRC4_ISTKSZ      (1024 * 8)   /* 8KB */
                                                   
/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    20
#define SIL_DLY_TIM2    10

/*
 *  割込みハンドラ番号に関する定義
 */ 
#define TMIN_INHNO 0U
#define TMAX_INHNO 31U
#define TNUM_INH   32U

/*
 *  割込み番号に関する定義
 */ 
#define TMIN_INTNO 0U
#define TMAX_INTNO 31U
#define TNUM_INT   32U

#ifndef TOPPERS_MACRO_ONLY

/*
 *  自プロセッサのAPCBへのポインタの取得
 *  インクルード順序の関係上，target_pcb.h には記述できないためここで定義する．
 */
Inline APCB*
get_my_p_apcb(void)
{
	return(&((get_my_p_tpcb())->arm_pcb));
}

/*
 * マスタプロセッサの判定
 */
Inline bool_t
x_sense_mprc(void){
	if(x_prc_index() == (TOPPERS_MASTER_PRCID - 1)) {
		return(true);
	}
	else{
		return(false);
	}
}

/*
 * マルチプロセッサ用機能
 */

#if TTYPE_KLOCK == G_KLOCK
/*
 *  ジャイアントロックの初期化（G_LOCKの場合）
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
	*p_giant_lock = GIANT_LOCK;
}

#endif /* TTYPE_KLOCK == G_KLOCK */

#if TTYPE_KLOCK == P_KLOCK

/*
 *  タスクロックの初期化（P_LOCKの場合）
 */
Inline void
x_initialize_tsk_lock(LOCK *p_tsk_lock)
{
	if (x_prc_index() == 0) {
		*p_tsk_lock = PRC1_TSK_LOCK;
	}
	else if (x_prc_index() == 1) {
		*p_tsk_lock = PRC2_TSK_LOCK;
	}
	else if (x_prc_index() == 2) {
		*p_tsk_lock = PRC3_TSK_LOCK;
	}
	else if (x_prc_index() == 3) {
		*p_tsk_lock = PRC4_TSK_LOCK;
	}
}

/*
 *  オブジェクトロックの初期化
 */
Inline void
x_initialize_obj_lock(LOCK *p_obj_lock)
{
	if (x_prc_index() == 0) {
		*p_obj_lock = PRC1_OBJ_LOCK;
	}
	else if (x_prc_index() == 1) {
		*p_obj_lock = PRC2_OBJ_LOCK;
	}
	else if (x_prc_index() == 2) {
		*p_obj_lock = PRC3_OBJ_LOCK;
	}
	else if (x_prc_index() == 3) {
		*p_obj_lock = PRC4_OBJ_LOCK;
	}
}

#endif /* TTYPE_KLOCK == P_KLOCK */

/*
 *  割込み番号のマスク（下位16bitのみが有効）
 */
#define INTNO_MASK(intno) (intno&0xffffU)

/*
 *  割込み番号の接続プロセッサのプロセッサID（上位16bit）
 */
#define INTNO_PRCID(intno) (intno >> 16U)

/*
 *  割込み番号の範囲の判定
 *
 *  ビットパターンを求めるのを容易にするために，8は欠番になっている．
 *  intnoは符号無し変数なので，TMIN_INTNOが0の場合は下限チェックはしない．
 */
#if TMIN_INTNO == 0
#define VALID_INTNO(prcid, intno) (((INTNO_MASK(intno)) <= TMAX_INTNO) && (INTNO_PRCID(intno) == prcid))
#else /* !TMIN_INTNO == 0 */
#define VALID_INTNO(prcid, intno) (((TMIN_INTNO <= (INTNO_MASK(intno)) && (INTNO_MASK(intno)) <= TMAX_INTNO)) \
                                    && (INTNO_PRCID(intno) == prcid))
#endif /* TMIN_INTNO == 0 */
#define VALID_INTNO_DISINT(prcid, intno)	VALID_INTNO(prcid, intno)
#define VALID_INTNO_CFGINT(prcid, intno)	VALID_INTNO(prcid, intno)

/*
 *  割込みハンドラの登録用テーブル（kernel_cfg.c）
 */
extern const FP* const p_inh_table[TNUM_PRCID];

/*
 *  割込みハンドラの優先度（kernel_cfg.c）
 */
extern const PRI* const p_inh_ipm_table[TNUM_PRCID];

/*
 *  割込みハンドラの設定
 */
Inline void
x_define_inh(INHNO inhno, FP int_entry, uint_t affinity_mask)
{

}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr)

/*
 *  TOPPERS標準割込み処理モデルの実現
 */
               
/*
 *  割込み優先度マスク操作ライブラリ
 *
 *  AT91SAM7Sは割込み優先度マスクをIRC内でハードウェア的に持つが，
 *  ソフトウェア側から値を読み書きできないため，割込み要求禁止フラ
 *  グにより割込み優先度マスクを実現する
 */
/*
 *  割込み優先度毎のマスクパターン（kernel_cfg.h）
 */
extern const uint32_t* const p_ipm_mask_table[TNUM_PRCID];

#define my_ipm_mask_table (p_ipm_mask_table[x_prc_index()])

#endif /* TOPPERS_MACRO_ONLY */  

/*
 *  IPMをimp_mask_tableのインデックスに変換するマクロ
 */
#define INDEX_IPM(ipm)  (-(ipm))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  (モデル上の)割込み優先度マスクの設定
 * 
 *  指定された優先度に設定された割込み要求禁止フラグのテーブルの値と（モデ
 *  ル上の）各割込みの割込み要求禁止フラグの状態を保持した変数の値との
 *  ORをIRCの割込み要求禁止フラグにセットし，設定した優先度を内部変数
 *  ipmに保存する．
 */
Inline void
x_set_ipm(PRI intpri)
{
	TPCB *p_tpcb;
	uint32_t ipm_mask;

	p_tpcb = get_my_p_tpcb();
	ipm_mask = my_ipm_mask_table[INDEX_IPM(intpri)];

	/*
	 *  AT91SAM7Sの割込みコントローラはイネーブルレジスタと
	 *  クリアーレジスタがあるため，一旦全ての割込みを禁止してから，
	 *  特定の割込みのみ許可する必要がある
	 */
	/* 全割込み禁止 */
	at91skyeye_disable_int(~0U);
	/* マスク指定されていない割込みのみ許可 */
	at91skyeye_enable_int(~(ipm_mask|(p_tpcb->idf)));
	p_tpcb->ipm = intpri;
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (モデル上の)割込み優先度マスクの参照
 *
 *  ipmの値を返す
 */
Inline PRI
x_get_ipm(void)
{
	TPCB *p_tpcb;

	p_tpcb = get_my_p_tpcb();

	return(p_tpcb->ipm);
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  割込み属性が設定されているかを判別するための変数
 */
extern uint32_t	bitpat_cfgint;

/*
 * （モデル上の）割込み要求禁止フラグのセット
 *
 *  指定された，割込み番号の割込み要求禁止フラグのセットして，割込みを
 *  禁止する．また，（モデル上の）割込み要求禁止フラグを管理するidfの対
 *  応するビットををセットする．
 *  割込み要求をマスクする機能をサポートしていない場合には，falseを返す
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	TPCB *p_tpcb = get_my_p_tpcb();

	if ((p_tpcb->bitpat_cfgint & INTNO_BITPAT(INTNO_MASK(intno))) == 0U) {
		return(false);
	}
	at91skyeye_disable_int(INTNO_BITPAT(INTNO_MASK(intno)));
	p_tpcb->idf |= INTNO_BITPAT(INTNO_MASK(intno));
	return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/* 
 * (モデル上の)割り要求禁止フラグの解除
 *
 * 指定された，割込み番号の割込み要求禁止フラグのクリアして，割込みを
 * 許可する．また，（モデル上の）割込み要求禁止フラグを管理するidfの対
 * 応するビットををクリアする．
 * 割込み要求をマスクする機能をサポートしていない場合には，falseを返す
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	TPCB *p_tpcb = get_my_p_tpcb();
	uint32_t ipm_mask = my_ipm_mask_table[INDEX_IPM(p_tpcb->ipm)];

	if ((p_tpcb->bitpat_cfgint & INTNO_BITPAT(INTNO_MASK(intno))) == 0U) {
		return(false);
	}
	p_tpcb->idf &= ~INTNO_BITPAT(INTNO_MASK(intno));
	/* 全割込み禁止 */
	at91skyeye_disable_int(~0U);
	/* マスク指定されていない割込みのみ許可 */
	at91skyeye_enable_int(~(ipm_mask|(p_tpcb->idf)));

	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 * 割込み要求のクリア
 */
Inline void
x_clear_int(INTNO intno)
{
	at91skyeye_clear_int(INTNO_BITPAT(INTNO_MASK(intno)));
}

#define t_clear_int(intno) x_clear_int(intno) 
#define i_clear_int(intno) x_clear_int(intno) 


/*
 *  割込み要求のチェック
 */
Inline bool_t
x_probe_int(INTNO intno)
{
    return(at91skyeye_probe_int(INTNO_BITPAT(INTNO_MASK(intno))));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  割込み要求ラインの属性の設定
 *
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);

/*
 *  割込みハンドラの入り口で必要なIRC操作
 *
 *  AT91SAM7Sでは，必要な処理はない
 */
Inline void
i_begin_int(INTNO intno)
{
    
}

/*
 *  割込みハンドラの出口で必要なIRC操作
 *
 *  AT91SAM7Sでは，必要な処理はない
 */
Inline void
i_end_int(INTNO intno)
{
    
}

/*
 *  ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void target_exit(void) NoReturn;

/*
 *  割込みハンドラ（chip_support.S）
 */
extern void interrupt_handler(void);

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);


/*
 *  マルチプロセッサ向けの機能
 */

/*
 * プロセッサ間割込みに使用する割込み番号（IRQ0）
 */
#define INTERPROCLVL 16

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  コア依存モジュール（ARM用）
 */
#include "core_config.h"

#ifndef TOPPERS_MACRO_ONLY
/*
 *  ロック取得解放関数
 *  x_lock_cpu()/x_unlock_cpu()を使うため，コア依存モジュール（ARM用）を
 *  読み込んでから定義する． 
 */
/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	while(true) {
		sil_wrw_mem((void *)MUTEX_ID_REG, *p_lock);
		if(sil_rew_mem((void *)MUTEX_CNT_REG) == 1){
			/* ロック取得成功 */
			ARM_MEMORY_CHANGED;
			return;
		}
		/* 割込みの許可 */
		x_unlock_cpu();
		x_lock_cpu();
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

#if TTYPE_KLOCK != G_KLOCK

/*
 *  2段目以上のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline bool_t
x_acquire_nested_lock(LOCK *p_lock)
{
	PCB		*p_pcb;

	while(true) {
		sil_wrw_mem((void *)MUTEX_ID_REG, *p_lock);
		if(sil_rew_mem((void *)MUTEX_CNT_REG) == 1){
			/* ロック取得成功 */
			ARM_MEMORY_CHANGED;
			return(false);
		}
		/* 割込みの許可 */
		x_unlock_cpu();
		x_lock_cpu();
		/*
		 * マイグレーションする可能性があるのでここで毎回取得  
		 * 非タスクコンテキストの場合，マイグレーションしないため，
		 * while前に実行してもよいが，1回でロックがとれた場合，
		 * 効率が悪いので，ここで取得する．
		 */
		p_pcb = get_my_p_pcb(); 
		if (p_pcb->p_firstlock == NULL) {
			return(true);
		}
	}
}

#define t_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)
#define i_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)

#endif /* TTYPE_KLOCK != G_KLOCK */

/*
 *  ロックの解放（タスク・非タスクコンテキスト共用）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	ARM_MEMORY_CHANGED;
	sil_wrw_mem((void *)MUTEX_ID_REG, *p_lock);
	sil_wrw_mem((void *)MUTEX_CNT_REG, 1);
}

/*
 *  割込を許可しないロック取得関数
 */
Inline void 
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	while(true) {
		sil_wrw_mem((void *)MUTEX_ID_REG, *p_lock);
		if(sil_rew_mem((void *)MUTEX_CNT_REG) == 1){
			/* ロック取得成功 */
			ARM_MEMORY_CHANGED;
			return;
		}
	}
}

/*
 *  スピンロック機能のネイティブロック方式で使用する関数
 */

/*
 *  スピンロックの型（ネイティブロックの場合）
 */
typedef uint32_t SPNLOCK;

/*
 *  スピンロックIDから実際のロックのIDへの変換
 */
#define TO_NATIVE_ID(spnid)		(spnid + 19)

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, SPNLOCK *p_spn_lock)
{
	*p_spn_lock = TO_NATIVE_ID(spnid);
	sil_wrw_mem((void *)MUTEX_ID_REG, *p_spn_lock);
	sil_wrw_mem((void *)MUTEX_CNT_REG, 1);
}

/*
 *  スピンロックの取得
 */
Inline void
x_lock_spin(SPNLOCK *p_spn_lock)
{
	x_acquire_lock(p_spn_lock);
}

#define t_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)
#define i_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)

/*
 *  スピンロックの取得の試行
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	sil_wrw_mem((void *)MUTEX_ID_REG, *p_spn_lock);
	if (sil_rew_mem((void *)MUTEX_CNT_REG) != 1) {
		/* 失敗 */
		return(true);
	}
	else {
		/* 成功 */
		ARM_MEMORY_CHANGED;
		return(false);
	}
}

/*
 *  スピンロックの返却
 */
Inline void
x_unlock_spin(SPNLOCK *spn_lock)
{
	x_release_lock(spn_lock);
}

#ifdef TOPPERS_ENABLE_GCOV_PART
/*
 *  GCOV一部取得用ライブラリ
 */
/*
 *  GCOV初期化関数
 */
extern void gcov_init(void);

/*
 *  GCOV中断関数
 */
extern void gcov_pause(void);

/*
 * GCOV再開関数
 */
extern void gcov_resume(void);

/*
 *  GCOV出力関数
 */
extern void gcov_dump(void);
#endif /* TOPPERS_ENABLE_GCOV_PART */
#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_TARGET_CONFIG_H */
