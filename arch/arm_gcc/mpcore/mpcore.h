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
 *  @(#) $Id: mpcore.h 955 2012-10-30 07:31:22Z ertl-honda $
 */

/*
 *  MPCORE のハードウェア資源の定義
 */

#ifndef TOPPERS_MPCORE_H
#define TOPPERS_MPCORE_H

#include <sil.h>

/*
 *  MPCORE のハードウェア資源のツール依存部の定義
 */
#include <mpcore_tool.h>


#ifndef TOPPERS_MACRO_ONLY

/*
 *  コプロのビット定義
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#if __TARGET_ARCH_ARM == 6
#define CP15_AUXILIARY_SA_BIT (1 << 5)
#define CP15_CPUID_BIT        (0x0f)
#else /* __TARGET_ARCH_ARM == 7 */
#define CP15_AUXILIARY_SA_BIT (1 << 6)
#define CP15_AUXILIARY_EX_BIT (1 << 7)
#define CP15_CPUID_BIT        (0x03)
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TTB0_RGN_S       (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

#if __TARGET_ARCH_ARM == 7
#define CP15_TTB0_IRGN_WBWA   ((1 << 6)|(0))
#endif /* __TARGET_ARCH_ARM == 7 */


/*
 *  コプロを用いたルーチン
 */

/*
 *  プロセッサINDEX（0オリジン）の取得
 */
Inline uint32_t
x_prc_index(void)
{
	uint32_t index;

	CP15_CPUID_READ(index);
	return((index & 0x0fU));
}

/*
 *  Data Synchronization Barrier
 *  ・先に発行された read と write の終了を待つ
 *  ・キャッシュ，ブランチプリディクション，TLBの操作の終了を待つ
 *  ・この命令の後に書かれた命令は実行されない
 */
Inline void
mpcore_data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  ・プログラムの記述に従って，先に書かれた命令でのメモリアクセスが
 *     終了するまで待つ．
 *
 */
Inline void
mpcore_data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}

/*
 *  TLBの無効化
 */
Inline void
mpcore_invalidate_unfied_tlb(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Dキャッシュの無効化
 */
Inline void
mpcore_dcache_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  Dキャッシュのクリーンと無効化
 */
Inline void
mpcore_dcache_clean_and_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_CLEAN_AND_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t  ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_CLEAN_AND_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  Iキャッシュの無効化
 */
Inline void
mpcore_icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
}

/*
 *  プリフェッチバッファをクリア
 */
Inline void
mpcore_pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  内蔵タイマ関連の定義
 */
#define MPCORE_TM_LR     (MPCORE_PMR_BASE + 0x0600)  /* ロードレジスタ           */
#define MPCORE_TM_COUNT  (MPCORE_PMR_BASE + 0x0604)  /* カウンターレジスタ       */
#define MPCORE_TM_CNT    (MPCORE_PMR_BASE + 0x0608)  /* コントロールレジスタ     */
#define MPCORE_TM_ISR    (MPCORE_PMR_BASE + 0x060C)  /* 割込みステータスレジスタ */

#define MPCORE_TM_CNT_EN        0x1
#define MPCORE_TM_CNT_AR        0x2
#define MPCORE_TM_CNT_IEN       0x4
#define MPCORE_TM_CNT_PS_OFFSET   8

#define MPCORE_TM_ISR_SCBIT    0x01

#if __TARGET_ARCH_ARM == 7
/*
 * Global Timer関連の定義（r1以上でサポート）
 */
#define MPCORE_GTC_COUNT_L (MPCORE_PMR_BASE + 0x0200)  /* カウンターレジスタL       */
#define MPCORE_GTC_COUNT_U (MPCORE_PMR_BASE + 0x0204)  /* カウンターレジスタH       */
#define MPCORE_GTC_CNT     (MPCORE_PMR_BASE + 0x0208)  /* コントロールレジスタ      */
#define MPCORE_GTC_COMPV_L (MPCORE_PMR_BASE + 0x0210)  /* コンペアバリューレジスタL */
#define MPCORE_GTC_COMPV_U (MPCORE_PMR_BASE + 0x0214)  /* コンペアバリューレジスタH */

#define MPCORE_GTC_CNT_TIMENA 0x01 /* タイマーイネーブル */

#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  Snoop Control Unit
 */
#define MPCORE_SCU_CTRL    (MPCORE_PMR_BASE + 0x0000)  /* コントロールレジスタ */
#define MPCORE_SCU_CONFIG  (MPCORE_PMR_BASE + 0x0004)  /* コンフィギュレーションレジスタ */
#define MPCORE_SCU_CPUST   (MPCORE_PMR_BASE + 0x0008)  /* CPUステータス */
#define MPCORE_SCU_IALL    (MPCORE_PMR_BASE + 0x000C)  /* インバリッドオール */
#if __TARGET_ARCH_ARM == 6
#define MPCORE_SCU_PM_CTRL (MPCORE_PMR_BASE + 0x0010)  /* パフォーマンスモニタ制御レジスタ */
#define MPCORE_SCU_PM_EVT0 (MPCORE_PMR_BASE + 0x0014)  /* モニタカウンタイベント0 */
#define MPCORE_SCU_PM_EVT1 (MPCORE_PMR_BASE + 0x0018)  /* モニタカウンタイベント1 */
#define MPCORE_SCU_PM_MN0  (MPCORE_PMR_BASE + 0x001C)  /* モニタカウンタ0 */
#define MPCORE_SCU_PM_MN1  (MPCORE_PMR_BASE + 0x0020)  /* モニタカウンタ1 */
#define MPCORE_SCU_PM_MN2  (MPCORE_PMR_BASE + 0x0024)  /* モニタカウンタ2 */
#define MPCORE_SCU_PM_MN3  (MPCORE_PMR_BASE + 0x0028)  /* モニタカウンタ3 */
#define MPCORE_SCU_PM_MN4  (MPCORE_PMR_BASE + 0x002C)  /* モニタカウンタ4 */
#define MPCORE_SCU_PM_MN5  (MPCORE_PMR_BASE + 0x0030)  /* モニタカウンタ5 */
#define MPCORE_SCU_PM_MN6  (MPCORE_PMR_BASE + 0x0034)  /* モニタカウンタ6 */
#define MPCORE_SCU_PM_MN7  (MPCORE_PMR_BASE + 0x0038)  /* モニタカウンタ7 */
#endif /* __TARGET_ARCH_ARM == 6 */
#define MPCORE_SCU_SNSAC   (MPCORE_PMR_BASE + 0x0054)  /* ノンセキュアアクセスコントロールレジスタ*/


#define MPCORE_SCU_CTRL_EN (1 << 0)   /* SCUイネーブル */

#define MPCORE_SCU_PM_CTRL_ENA  (1 << 0)    /* パフォーマンスモニタ有効化 */
#define MPCORE_SCU_PM_CTRL_RST  (1 << 1)    /* パフォーマンスモニタリセット */

#define MPCORE_SCU_PM_EVT_CYC_COUNT0_4 (31 << 0) /* サイクルカウント指定 */

#define MPCORE_SCU_IALL_WAYS   (0x0000ffffU)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  SCUの初期化
 */
extern void scu_init(void);

#if __TARGET_ARCH_ARM == 6
/*
 * SCUカウンタの初期化（MN0を使用）
 */
extern void scu_counter_init(void);

/*
 * SCUタイマのリセット
 */
Inline void
scu_counter_reset(void)
{
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST);
}

/*
 * SCUタイマの現在値を読む
 */
Inline uint_t
scu_counter_get_current(void)
{
	return (sil_rew_mem((void *)MPCORE_SCU_PM_MN0));
}
#endif /* __TARGET_ARCH_ARM == 6 */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  Distributed Interrupt Controller CPU Interface
 *
 *  優先度は，0x0〜0xF の16段階．
 *  0x0が最高優先度で，0xFが最低優先度．
 */
#define DIC_CPUIF_CR   (MPCORE_PMR_BASE + 0x0100)  /* コントロールレジスタ */
#define DIC_CPUIF_PMR  (MPCORE_PMR_BASE + 0x0104)  /* 優先度マスクレジスタ */
#define DIC_CPUIF_BPR  (MPCORE_PMR_BASE + 0x0108)  /* バイナリポイントレジスタ */
#define DIC_CPUIF_IAR  (MPCORE_PMR_BASE + 0x010C)  /* Interrupt Acknowledge Register */
#define DIC_CPUIF_EOI  (MPCORE_PMR_BASE + 0x0110)  /* End of Interrupt Register */
#define DIC_CPUIF_RII  (MPCORE_PMR_BASE + 0x0114)  /* Running Interrupt Register */

#define DIC_CPUIF_CR_EN          0x01 /* このビットは、non-secureでのみ有効 */
#define DIC_CPUIF_CR_EN_S        0x01
#define DIC_CPUIF_CR_EN_NS       0x02
#define DIC_CPUIF_CR_EN_ACKCTL   0x04
#define DIC_CPUIF_CR_FIQEN       0x08
#define DIC_CPUIF_CR_EN_SBPR     0x10

#if __TARGET_ARCH_ARM == 6
#define DIC_CPUIF_PMR_OFFSET 0x04
#define DIC_CPUIF_BPR_ALL    0x03  /* 全てのビットが有効 */
#define DIC_CPUIF_BPR_3      0x04  /* 3ビット有効        */
#define DIC_CPUIF_BPR_2      0x05  /* 2ビット有効        */
#define DIC_CPUIF_BPR_1      0x06  /* 1ビット有効        */
#elif __TARGET_ARCH_ARM == 7
#define DIC_CPUIF_PMR_OFFSET 0x03
#define DIC_CPUIF_BPR_ALL    0x00  /* 全てのビットが有効 */
#define DIC_CPUIF_BPR_4      0x03  /* 4ビット有効        */
#define DIC_CPUIF_BPR_3      0x04  /* 3ビット有効        */
#define DIC_CPUIF_BPR_2      0x05  /* 2ビット有効        */
#define DIC_CPUIF_BPR_1      0x06  /* 1ビット有効        */
#endif /* __TARGET_ARCH_ARM == 7 */


/*
 *  Distributed Interrupt Controller
 */
#define DIC_CR     (MPCORE_PMR_BASE + 0x1000)  /* コントロールレジスタ */
#define DIC_ICT    (MPCORE_PMR_BASE + 0x1004)  /* Interrupt Controller タイプレジスタ */
#if __TARGET_ARCH_ARM == 7
#define DIC_IID    (MPCORE_PMR_BASE + 0x1008)  /* Distributor Implementer ID */
#define DIC_SEC    (MPCORE_PMR_BASE + 0x1080)  /* 割込みセキュリティ */
#endif /* __TARGET_ARCH_ARM == 7 */
#define DIC_SE     (MPCORE_PMR_BASE + 0x1100)  /* 割込みイネーブルセット   */
#define DIC_CE     (MPCORE_PMR_BASE + 0x1180)  /* 割込みイネーブルクリアー */
#define DIC_SP     (MPCORE_PMR_BASE + 0x1200)  /* 割込みセットペンディング */
#define DIC_CP     (MPCORE_PMR_BASE + 0x1280)  /* 割込みクリアーペンディング */
#define DIC_IPR    (MPCORE_PMR_BASE + 0x1400)  /* 割込み優先度レジスタ */
#define DIC_ITR    (MPCORE_PMR_BASE + 0x1800)  /* 割込みターゲットレジスタ/CA9はSPIターゲットレジスタ */
#define DIC_ICR    (MPCORE_PMR_BASE + 0x1C00)  /* 割込みコンフィギュレーションレジスタ */
#define DIC_SIR    (MPCORE_PMR_BASE + 0x1F00)  /* ソフトウェア割込みレジスタ  */
#if __TARGET_ARCH_ARM == 7
#define DIC_PPIS   (MPCORE_PRM_BASE + 0x1D00)  /* PPIステータス */
#define DIC_SPIS   (MPCORE_PRM_BASE + 0x1D04)  /* SPIステータス */
#endif /* __TARGET_ARCH_ARM == 7 */

#define DIC_CR_ENABLE 0x01
#define DIC_CR_NS_ENABLE 0x02

/*
 *  割込み先のプロセッサの指定
 */
#define DIC_ITR_CPU0  0x01
#define DIC_ITR_CPU1  0x02
#define DIC_ITR_CPU2  0x04
#define DIC_ITR_CPU3  0x08

#define DIC_SIR_CPU0  0x01
#define DIC_SIR_CPU1  0x02
#define DIC_SIR_CPU2  0x04
#define DIC_SIR_CPU3  0x08
#define DIC_SIR_CPUS  0x0f

#define DIC_SIR_CPU_OFFSET 16

/*
 *  割込み優先度
 */
#define DIC_MAX_PRIORITY  0x00
#if __TARGET_ARCH_ARM == 6
#define DIC_MIN_PRIORITY  0x0f
#elif __TARGET_ARCH_ARM == 7
#define DIC_MIN_PRIORITY  0x1f
#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  コンフィギュレーションレジスタの設定値
 */
#define DIC_ICR_EDGE     0x03   /* エッジ割込み */
#define DIC_ICR_LEVEL    0x01   /* レベル割込み */

/*
 *  DICでサポートしている割込み数
 */
#define DIC_TMIN_INTNO      0U

#ifndef DIC_TMAX_INTNO
#define DIC_TMAX_INTNO    255U
#endif /* DIC_TMAX_INTNO */

#ifndef DIC_TNUM_INT
#define DIC_TNUM_INT      256U
#endif /* DIC_TNUM_INT */

/*
 *  グローバル割込みの開始番号
 */
#define TMIN_GLOBAL_INTNO   32U

/*
 * 割込み番号(DICでの番号)
 */
#define DIC_IRQNO_IPI0     0
#define DIC_IRQNO_IPI1     1
#define DIC_IRQNO_IPI2     2
#define DIC_IRQNO_IPI3     3
#define DIC_IRQNO_IPI4     4
#define DIC_IRQNO_IPI5     5
#define DIC_IRQNO_IPI6     6
#define DIC_IRQNO_IPI7     7
#define DIC_IRQNO_IPI8     8
#define DIC_IRQNO_IPI9     9
#define DIC_IRQNO_IPI10   10
#define DIC_IRQNO_IPI11   11
#define DIC_IRQNO_IPI12   12
#define DIC_IRQNO_IPI13   13
#define DIC_IRQNO_IPI14   14
#define DIC_IRQNO_IPI15   15

#define DIC_IRQNO_TM      29
#define DIC_IRQNO_WDT     30

#ifndef TOPPERS_MACRO_ONLY
/*
 *  DIC CPUインタフェース関連のドライバ
 */
/*
 *  CPUの割込み優先度マスクを設定
 */
Inline void
dic_cpu_set_priority(int pri)
{
	sil_wrw_mem((void *)DIC_CPUIF_PMR,
				(pri << DIC_CPUIF_PMR_OFFSET));
}

/*
 *  CPUの割込み優先度マスクを取得
 */
Inline int
dic_cpu_current_priority(void)
{
	return (sil_rew_mem((void *)(DIC_CPUIF_PMR)) >>
			DIC_CPUIF_PMR_OFFSET);
}

/*
 *  割込みハンドラの出口で必要なIRC操作
 */
Inline void
dic_cpu_end_int(uint8_t id)
{
	sil_wrw_mem((void *)(DIC_CPUIF_EOI), id);
}

/*
 *  DICのプロセッサの割込み優先度のどのビットを使用するか
 */
Inline void
dic_cpu_set_bp(int mask_bit)
{
	sil_wrw_mem((void *)DIC_CPUIF_BPR, mask_bit);
}

/*
 *  CPUインタフェースの初期化
 */
extern void dic_cpu_init(void);

/*
 *  CPUインタフェースの終了
 */
extern void dic_cpu_stop(void);

/*
 *  DIC DISインタフェース関連のドライバ
 */

/*
 *  割込み禁止
 */
extern void dic_disable_int(uint8_t id);

/*
 *  割込み許可
 */
extern void dic_enable_int(uint8_t id);

/*
 *  割込みペンディングクリア
 */
extern void dic_clear_pending(uint8_t id);

/*
 *  割込みペンディングセット
 */
extern void dic_set_pending(uint8_t id);

/*
 *  割込み要求のチェック
 */
extern bool_t dic_probe_int(uint8_t id);

/*
 *  割込み設定のセット
 */
extern void dic_config(uint8_t id, uint8_t config);

/*
 *  割込み優先度のセット
 *  内部表現で渡す．
 */
extern void dic_set_priority(uint8_t id, int pri);

/*
 *  DIC割込みターゲットの設定
 *  CPUはORで指定
 */
extern void dic_set_target(uint8_t id, uint8_t cpus);

/*
 *  DICインタフェースの初期化(SPIのみ初期化)
 */
extern void dic_init(void);

/*
 *  各コアで実行するDICインタフェースのSGIとPPIの初期化
 */
extern void dic_sgi_ppi_init(void);

/*
 * GICの割込みセキュリティ設定
 * 割込み番号とそのセキュリティで指定
 * 引数 ns (non secure bit) :
 *     0 -> secure(FIQ) setting
 *     1 -> non-secure(IRQ) setting
 */
extern void dic_set_security(int intno, unsigned int ns);

/*
 *  DICインタフェースの終了
 */
extern void dic_stop(void);

/*
 *  ソフトウェア割込みを発行
 */
Inline void
dic_swi(int cpu, int id)
{
	CP15_DATA_SYNC_BARRIER();
	sil_wrw_mem((void *)DIC_SIR, (cpu << DIC_SIR_CPU_OFFSET)|id);
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  SMPモードの有効化
 */
extern void mpcore_smp_mode_enable(void);

/*
 *  MMU関連
 */

/*
 * 変換テーブルへの設定内容
 * va   : 仮想アドレス
 * pa   : 物理アドレス
 * size : サイズ
 * s    : 共有指定
 * tex  : C Bとの組み合わせで変化
 * ap   : アクセス権
 * c    : キャッシュ
 * b    : バッファ
 */
typedef struct{
	uintptr_t   va;
	uintptr_t   pa;
	uint32_t    size;
	uint8_t     ns;
	uint8_t     s;
	uint8_t     tex;
	uint8_t     ap;
	uint8_t     c;
	uint8_t     b;
}MEMORY_ATTRIBUTE;

/*
 *  MMUの初期化
 */
extern void mpcore_mmu_init(void);

/*
 *  MMUによるメモリのマッピング
 */
extern void mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute);

/*
 *  I/Dキャッシュを両方を有効に
 */
extern void mpcore_cache_enable(void);

/*
 *  I/Dキャッシュを両方を無効に
 */
extern void mpcore_cache_disable(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_MPCORE_H */
