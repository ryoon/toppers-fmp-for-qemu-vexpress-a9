/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: target_config.c 843 2011-07-15 03:17:19Z ertl-honda $
 */

/*
 * チップ依存モジュール（AT91SKYEYE用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "target_serial.h"
#include "mp.h"
#include <stdlib.h>

#ifdef TOPPERS_ENABLE_TRACE
#include <stdio.h>
static FILE *trace_log_file;
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  SIL_LOC_SPN()を取得したプロセッサの管理
 */
uint32_t TOPPERS_spn_var;

/*
 *  文字列の出力（バージョンチェック用）
 */
static void
target_fput_str(char *c) {
	while(*c != '\0') {
		at91skyeye_putc(*c++);
	}
}

/*
 *  数字 to 文字変換(4bit用)
 */
static char
num_to_char(uint8_t num) {
	if (num <= 9) {
		return ('0' + num);
	}
	else {
		return ('a' + (num - 10));
	}
}

/*
 *  バージョン番号の出力
 */
static void
version_put(uint16_t version) {
	char c;

	c = num_to_char((version >> 12) & 0x0f);
	at91skyeye_putc(c);
	at91skyeye_putc('.');
	c = num_to_char((version >> 8) & 0x0f);
	at91skyeye_putc(c);
	c = num_to_char((version >> 4) & 0x0f);
	at91skyeye_putc(c);
	at91skyeye_putc('.');
	c = num_to_char((version) & 0x0f);
	at91skyeye_putc(c);
	at91skyeye_putc('.');
}

/*
 *  バージョンチェック
 */
static void
version_check(void) {

	/* SkyEye のバージョンを確認 */
	if (sil_rew_mem((void *)(SKYEYE_VER_REG)) != SUPPORT_SKYEYE_VER) {
		target_fput_str("SkyEye version is mismatch!!");
		at91skyeye_putc('\n');
		target_fput_str("Suppoted version is ");
		version_put(SUPPORT_SKYEYE_VER);
		at91skyeye_putc('\n');
		target_fput_str("Tool version is ");
		version_put(sil_rew_mem((void *)(SKYEYE_VER_REG)));
		at91skyeye_putc('\n');
		target_fput_str("Kernel Exit...");
		at91skyeye_putc('\n');
		target_exit();
	}

	/* DeviceManger Extension のバージョンを確認 */
	if ((sil_rew_mem((void *)(DEVM_VER_REG)) & 0xfff0)
		!= (SUPPORT_DEVM_VER & 0xfff0)) {
		target_fput_str("DeviceManager Extension version is mismatch!!");
		at91skyeye_putc('\n');
		target_fput_str("Suppoted version is ");
		version_put(SUPPORT_DEVM_VER);
		at91skyeye_putc('\n');
		target_fput_str("Tool version is ");
		version_put((sil_rew_mem((void *)(DEVM_VER_REG)) & 0xfff0));
		at91skyeye_putc('\n');
		target_fput_str("Kernel Exit...");
		at91skyeye_putc('\n');
		target_exit();
	}
}

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
void
target_mprc_initialize(void)
{

}

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	TPCB *p_tpcb = get_my_p_tpcb();
	uint32_t index = x_prc_index();

	/*
	 *  target_fput_log が可能になるようにUARTを初期化
	 */
	at91skyeye_init_uart(x_prc_index() + 1);

	/*
	 *  バージョンの確認
	 */
	version_check();

	/*
	 *  ARM依存の初期化
	 */
	core_initialize();
    
	/*
	 * 各割込みの割込み要求禁止フラグ全禁止
	 */
	p_tpcb->idf = ~0U;

	/*
	 * 割込み優先度マスクは0
	 */ 
	p_tpcb->ipm = 0U;

	/*
	 * 全ての割込みをマスク
	 */ 
	at91skyeye_disable_int(~0U);

	/*
	 * 全ての割込み要因をクリア
	 */
	at91skyeye_clear_int(~0U);

	/*
	 *  割込み属性が設定されているかを判別するための変数を初期化する．
	 */
	p_tpcb->bitpat_cfgint = 0U;
    
	/*
	 *  割込みマスクテーブルへのポインタの初期化
	 */
	p_tpcb->p_ipm_mask_tbl = p_ipm_mask_table[index];

	/*
	 *  割込みハンドラテーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_tbl = p_inh_table[index];

	/*
	 *  割込み優先度テーブルへのポインタの初期化
	 */
	p_tpcb->p_inh_ipm_tbl = p_inh_ipm_table[index];

#ifdef TOPPERS_ENABLE_TRACE
	if (x_sense_mprc()) {
		trace_log_file = fopen("kernel_trace.log","w");
	}
#endif /* TOPPERS_ENABLE_TRACE */
}

/*
 *  exit()の最後に呼び出される関数．SkyEyeを終了させる．
 *  マスタプロセッサ以外が呼び出す．
 */
extern void _exit(int) NoReturn;

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *  すべての割込みをマスクする．
	 */
	at91skyeye_disable_int(~0U);

	/*
	 *  ARM依存の終了処理
	 */
	core_terminate();

	/*
	 *  開発環境依存の終了処理
	 */
	at91skyeye_exit();

#ifdef TOPPERS_ENABLE_TRACE
	if (x_sense_mprc()) {
		fclose(trace_log_file);
	}
#endif /* TOPPERS_ENABLE_TRACE */

#ifdef TOPPERS_ENABLE_GCOV_PART
	_exit(0);
#else /* TOPPERS_ENABLE_GCOV_FULL */
	if (x_sense_mprc()) {
		exit(0);
	}
	else {
		_exit(0);
	}
#endif /* TOPPERS_ENABLE_GCOV_PART */
}

/*
 *  実行時間分布集計モジュールで用いる測定用タイマの初期化関数
 */
void
perf_timer_initialize(intptr_t exinf)
{
}

/*
 *  サイクルカウンタの取得
 */
void
cycle_counter_get(uint32_t *p_time)
{
	*p_time = cycle_counter_get_current();
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char_t c)
{
	if (c == '\n') {
		at91skyeye_putc('\r');
	}
	at91skyeye_putc(c);
}

#ifdef TOPPERS_ENABLE_TRACE
/*
 *  トレースログのファイル出力
 */
void
target_fput_log_file(char_t c)
{
	if (c == '\n') {
		fputc('\r', trace_log_file);
	}
	fputc(c, trace_log_file);
}
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  割込み要求ラインの属性の設定
 *
 *  FMPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 *
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	TPCB *p_tpcb = get_my_p_tpcb();

	assert(VALID_INTNO_CFGINT(ID_PRC(x_prc_index()), intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込み属性が設定されているかを判別するための変数の設定
	 */
	p_tpcb->bitpat_cfgint |= INTNO_BITPAT(INTNO_MASK(intno));
    
	/* 
	 * いったん割込みを禁止する
	 */    
	x_disable_int(intno);

	if ((intatr & TA_ENAINT) != 0U){
		(void)x_enable_int(intno);
	}    
}

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 * 未定義の割込みが入った場合の処理
 */
void
default_int_handler(void){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "Processor %d : Unregistered Interrupt occurs.", prc_id);
	target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */


extern void start_master(void);

/*
 *  コンパイラのスタートアップルーチンから呼び出される．
 *  sta_ker()を呼び出してカーネルをスタートさせる． 
 */
int
main(void)
{
	start_master();
	return 1;
}

#ifdef TOPPERS_ENABLE_GCOV_PART
/*
 *  GCOV一部取得用ライブラリ
 */

extern void __gcov_flush();

/*
 *  GCOV初期化関数
 *   カバレッジを.gcdaに出力し，SkyEyeで*.gcdaファイルを削除する．
 */
void
gcov_init(void)
{
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	__gcov_flush();
	sil_wrw_mem((void*)0xFFFE1020, 0);
	SIL_UNL_SPN();
}

/*
 *  GCOV中断関数
 *   カバレッジを.gcdaに出力し，SkyEyeで*.gcdaファイルを*.gcda.bakへリネームする．
 */
void
gcov_pause(void)
{
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	__gcov_flush();
	sil_wrw_mem((void*)0xFFFE1024, 0);
	SIL_UNL_SPN();
}

/*
 *  GCOV再開関数
 *   カバレッジを.gcdaに出力し，SkyEyeで*.gcda.bakファイルを*.gcdaへリネームする．
 */
void
gcov_resume(void)
{
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	__gcov_flush();
	sil_wrw_mem((void*)0xFFFE1028, 0);
	SIL_UNL_SPN();
}

/*
 *  GCOV出力関数
 *   カバレッジを.gcdaに出力する．
 */
void
gcov_dump(void)
{
	SIL_PRE_LOC;

	SIL_LOC_SPN();
	__gcov_flush();
	SIL_UNL_SPN();
}
#endif /* TOPPERS_ENABLE_GCOV_PART */
