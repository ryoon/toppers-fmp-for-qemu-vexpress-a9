$ 
$     パス2のチップ依存テンプレート（AT91SKYEYE用）
$ 

$ 
$ 有効なCPU例外ハンドラ番号
$ 
$EXCNO_VALID = { 
	0x10001,0x10002,0x10003,0x10004,0x10006;
	0x20001,0x20002,0x20003,0x20004,0x20006;
	0x30001,0x30002,0x30003,0x30004,0x30006;
	0x40001,0x40002,0x40003,0x40004,0x40006
}$

$ 
$  有効な割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号
$ 
$INTNO_VALID = { 
	0x10000,0x10001,...,0x1001f;
	0x20000,0x20001,...,0x2001f;
	0x30000,0x30001,...,0x3001f;
	0x40000,0x40001,...,0x4001f
}$

$INHNO_VALID = INTNO_VALID$

$ 
$  ATT_ISRで使用できる割込み番号とそれに対応する割込みハンドラ番号
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INTで使用できる割込みハンドラ番号
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$
$INTPRI_CFGINT_VALID = { -1,-2,...,-7 }$

$ 
$  割込み属性中のターゲット依存に用いるビット
$ 
$TARGET_INTATR = TA_HIGHLEVEL$


$ 
$ プロセッサに対する定義
$ 

$ グローバルタイマ時のタイムイベント用データのセクション指定
$PRC_SECTION_TEVT[0] = ""$

$ 
$ プロセッサ1に関する指定
$ 
$ PCBのセクション指定
$PRC_SECTION_PCB[1]  = "__attribute__((section(\".bss._kernel_prc1s_pcb\"),nocommon))"$
$ 非タスクコンテキスト用スタックのセクション指定
$PRC_SECTION_ISTACK[1]  = "__attribute__((section(\".bss._kernel_prc1p_istack\"),nocommon))"$
$ タイムイベント関連データのセクション指定
$PRC_SECTION_TEVT[1] = "__attribute__((section(\".bss._kernel_prc1s_tevt\"),nocommon))"$

$ 
$ プロセッサ2に関する指定
$ 
$PRC_SECTION_PCB[2] = "__attribute__((section(\".bss._kernel_prc2s_pcb\"),nocommon))"$
$PRC_SECTION_ISTACK[2]  = "__attribute__((section(\".bss._kernel_prc2p_istack\"),nocommon))"$
$PRC_SECTION_TEVT[2] = "__attribute__((section(\".bss._kernel_prc2s_tevt\"),nocommon))"$

$ 
$ プロセッサ3に関する指定
$ 
$PRC_SECTION_PCB[3] = "__attribute__((section(\".bss._kernel_prc3s_pcb\"),nocommon))"$
$PRC_SECTION_ISTACK[3]  = "__attribute__((section(\".bss._kernel_prc3p_istack\"),nocommon))"$
$PRC_SECTION_TEVT[3] = "__attribute__((section(\".bss._kernel_prc3s_tevt\"),nocommon))"$

$ 
$ プロセッサ4に関する指定
$ 
$PRC_SECTION_PCB[4] = "__attribute__((section(\".bss._kernel_prc4s_pcb\"),nocommon))"$
$PRC_SECTION_ISTACK[4]  = "__attribute__((section(\".bss._kernel_prc4p_istack\"),nocommon))"$
$PRC_SECTION_TEVT[4] = "__attribute__((section(\".bss._kernel_prc4s_tevt\"),nocommon))"$

$ 
$  オブジェクト関連のメモリの配置先指定（プロセッサ）
$ 

$ 
$  PCBの配置先指定
$ 
$FUNCTION GENERATE_PCB$
	PCB _kernel_prc$ARGV[1]$_pcb $PRC_SECTION_PCB[ARGV[1]]$;$NL$
$END$

$ 
$  ISTACKの配置先指定
$ 
$FUNCTION GENERATE_ISTACK$
	static STK_T					_kernel_prc$ARGV[1]$_istack[COUNT_STK_T(DEFAULT_PRC$ARGV[1]$_ISTKSZ)] $PRC_SECTION_ISTACK[ARGV[1]]$;$NL$
$END$

$ 
$  ISTACK_ICSの配置先指定
$ 
$FUNCTION GENERATE_ISTACK_ICS$
	static STK_T				_kernel_prc$ARGV[1]$_istack[COUNT_STK_T($ICS.ISTKSZ[ARGV[2]]$)] $PRC_SECTION_ISTACK[ARGV[1]]$;$NL$
$END$

$ 
$  TMEVTN_HEAPの配置先指定
$ 
$FUNCTION GENERATE_TMEVT_HEAP$
	TMEVTN   _kernel_prc$ARGV[1]$_tmevt_heap[TNUM_TSKID + TNUM_CYCID + TNUM_ALMID] $PRC_SECTION_TEVT[ARGV[1]]$;$NL$
$END$

$ 
$  グローバルタイマ方式でのタイムイベントヒープの配置先指定
$ 
$FUNCTION GENERATE_TMEVT_HEAP_SYSTIM_GLOBAL$
	TMEVTN   _kernel_tmevt_heap[TNUM_TSKID + TNUM_CYCID + TNUM_ALMID] $PRC_SECTION_TEVT[0]$;$NL$
$END$

$ 
$  TEVTCBの配置先指定
$ 
$FUNCTION GENERATE_TEVTCB$
	TEVTCB   _kernel_prc$ARGV[1]$_tevtcb $PRC_SECTION_TEVT[ARGV[1]]$;$NL$
$END$

$ 
$  グローバルタイマ方式でのタイムイベントコントロールブロックの配置先指定
$ 
$FUNCTION GENERATE_TEVTCB_SYSTIM_GLOBAL$
	TEVTCB   _kernel_tevtcb $PRC_SECTION_TEVT[0]$;$NL$
$END$


$ 
$  クラスに対する定義
$ 

$ 
$  クラス数
$ 
$NUM_CLASS = 9$

$ 
$ クラス TCL_1（クラスID 1）に関する指定
$ 

$  初期割付けプロセッサ(ID指定)
$CLASS_AFFINITY_INI[1] = 1$

$  マイグレーション可能なプロセッサのマスク指定
$CLASS_AFFINITY_MASK[1] = 0x0000000f$

$  オブジェクトロック
& P_LOCKの場合に使用するオブジェクトロックを持つプロセッサのIDを指定
$CLASS_OBJ_LOCK[1] = 1$

$  タスクスタックのセクション指定
$CLASS_SECTION_TSKSTK[1] = "__attribute__((section(\".bss._kernel_prc1s_tstack\"),nocommon))"$

$  タスクコントロールブロックのセクション指定
$CLASS_SECTION_TSKCB[1] = "__attribute__((section(\".bss._kernel_prc1s_tskcb\"),nocommon))"$

$  オブジェクトコントロールブロックのセクション指定
$CLASS_SECTION_OBJCB[1] = "__attribute__((section(\".bss._kernel_prc1s_objcb\"),nocommon))"$

$ 
$ クラス TCL_2（クラスID 2）に関する指定
$ 
$CLASS_AFFINITY_INI[2] = 2$
$CLASS_AFFINITY_MASK[2] = 0x0000000f$
$CLASS_OBJ_LOCK[2] = 2$

$CLASS_SECTION_TSKSTK[2] = "__attribute__((section(\".bss._kernel_prc2s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[2] = "__attribute__((section(\".bss._kernel_prc2s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[2] = "__attribute__((section(\".bss._kernel_prc2s_objcb\"),nocommon))"$

$ 
$ クラス TCL_3（クラスID 3） に関する指定
$ 
$CLASS_AFFINITY_INI[3] = 3$
$CLASS_AFFINITY_MASK[3] = 0x0000000f$
$CLASS_OBJ_LOCK[3] = 3$

$CLASS_SECTION_TSKSTK[3] = "__attribute__((section(\".bss._kernel_prc3s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[3] = "__attribute__((section(\".bss._kernel_prc3s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[3] = "__attribute__((section(\".bss._kernel_prc3s_objcb\"),nocommon))"$

$ 
$ クラス TCL_4（クラスID 4） に関する指定
$ 
$CLASS_AFFINITY_INI[4] = 4$
$CLASS_AFFINITY_MASK[4] = 0x0000000f$
$CLASS_OBJ_LOCK[4] = 4$

$CLASS_SECTION_TSKSTK[4] = "__attribute__((section(\".bss._kernel_prc4s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[4] = "__attribute__((section(\".bss._kernel_prc4s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[4] = "__attribute__((section(\".bss._kernel_prc4s_objcb\"),nocommon))"$

$ 
$ クラス TCL_1_ONLY（クラスID 5）に関する指定
$ 
$CLASS_AFFINITY_INI[5] = 1$
$CLASS_AFFINITY_MASK[5] = 0x00000001$
$CLASS_OBJ_LOCK[5] = 1$

$CLASS_SECTION_TSKSTK[5] = "__attribute__((section(\".bss._kernel_prc1s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[5] = "__attribute__((section(\".bss._kernel_prc1s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[5] = "__attribute__((section(\".bss._kernel_prc1s_objcb\"),nocommon))"$

$ 
$ クラス TCL_2_ONLY（クラスID 6）に関する指定
$ 
$CLASS_AFFINITY_INI[6] = 2$
$CLASS_AFFINITY_MASK[6] = 0x00000002$
$CLASS_OBJ_LOCK[6] = 2$

$CLASS_SECTION_TSKSTK[6] = "__attribute__((section(\".bss._kernel_prc2s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[6] = "__attribute__((section(\".bss._kernel_prc2s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[6] = "__attribute__((section(\".bss._kernel_prc2s_objcb\"),nocommon))"$

$ 
$ クラス TCL_3_ONLY（クラスID 7） に関する指定
$ 
$CLASS_AFFINITY_INI[7] = 3$
$CLASS_AFFINITY_MASK[7] = 0x00000004$
$CLASS_OBJ_LOCK[7] = 3$

$CLASS_SECTION_TSKSTK[7] = "__attribute__((section(\".bss._kernel_prc3s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[7] = "__attribute__((section(\".bss._kernel_prc3s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[7] = "__attribute__((section(\".bss._kernel_prc3s_objcb\"),nocommon))"$

$ 
$ クラス TCL_4_ONLY（クラスID 8） に関する指定
$ 
$CLASS_AFFINITY_INI[8] = 4$
$CLASS_AFFINITY_MASK[8] = 0x00000008$
$CLASS_OBJ_LOCK[8] = 4$

$CLASS_SECTION_TSKSTK[8] = "__attribute__((section(\".bss._kernel_prc4s_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[8] = "__attribute__((section(\".bss._kernel_prc4s_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[8] = "__attribute__((section(\".bss._kernel_prc4s_objcb\"),nocommon))"$

$ 
$ クラス TCL_SYSTIM_PRC（クラスID 9） に関する指定
$ 
$CLASS_AFFINITY_INI[9] = TOPPERS_SYSTIM_PRCID$
$CLASS_AFFINITY_MASK[9] = 0x1 << (TOPPERS_SYSTIM_PRCID - 1) $
$CLASS_OBJ_LOCK[9] = TOPPERS_SYSTIM_PRCID$

$CLASS_SECTION_TSKSTK[9] = "__attribute__((section(\".bss._kernel_systim_tstack\"),nocommon))"$
$CLASS_SECTION_TSKCB[9] = "__attribute__((section(\".bss._kernel_systim_tskcb\"),nocommon))"$
$CLASS_SECTION_OBJCB[9] = "__attribute__((section(\".bss._kernel_systim_objcb\"),nocommon))"$

$ 
$  オブジェクト関連のメモリの配置先指定（クラス）
$ 

$ 
$  タスクスタックの配置先指定
$ 
$FUNCTION GENERATE_TSKSTK$
	static STK_T _kernel_stack_$ARGV[1]$[COUNT_STK_T($TSK.STKSZ[ARGV[1]]$)]  $CLASS_SECTION_TSKSTK[TSK.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  TCBの配置先指定
$ 
$FUNCTION GENERATE_TCB$
	TCB _kernel_tcb_$ARGV[1]$$TAB$$CLASS_SECTION_TSKCB[TSK.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  SEMCBの配置先指定
$ 
$FUNCTION GENERATE_SEMCB$
	SEMCB _kernel_semcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[SEM.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  FLGCBの配置先指定
$ 
$FUNCTION GENERATE_FLGCB$
	FLGCB _kernel_flgcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[FLG.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  DTQMBの配置先指定
$ 
$FUNCTION GENERATE_DTQMB$
	static DTQMB _kernel_dtqmb_$DTQ.DTQID[ARGV[1]]$[$DTQ.DTQCNT[ARGV[1]]$] $CLASS_SECTION_OBJCB[DTQ.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  DTQCBの配置先指定
$ 
$FUNCTION GENERATE_DTQCB$
	DTQCB _kernel_dtqcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[DTQ.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  PDQMBの配置先指定
$ 
$FUNCTION GENERATE_PDQMB$
	static PDQMB _kernel_pdqmb_$PDQ.PDQID[ARGV[1]]$[$PDQ.PDQCNT[ARGV[1]]$]  $CLASS_SECTION_OBJCB[PDQ.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  PDQCBの配置先指定
$ 
$FUNCTION GENERATE_PDQCB$
	PDQCB _kernel_pdqcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[PDQ.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  MBXCBの配置先指定
$ 
$FUNCTION GENERATE_MBXCB$
	MBXCB _kernel_mbxcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[MBX.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  MPF_Tの配置先指定
$ 
$FUNCTION GENERATE_MPF_T$
	static MPF_T _kernel_mpf_$MPF.MPFID[ARGV[1]]$[($MPF.BLKCNT[ARGV[1]]$) * COUNT_MPF_T($MPF.BLKSZ[mpfid]$)]  $CLASS_SECTION_OBJCB[MPF.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  MPFMBの配置先指定
$ 
$FUNCTION GENERATE_MPFMB$
	static MPFMB _kernel_mpfmb_$MPF.MPFID[ARGV[1]]$[$MPF.BLKCNT[ARGV[1]]$] $CLASS_SECTION_OBJCB[MPF.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  MPFCBの配置先指定
$ 
$FUNCTION GENERATE_MPFCB$
	MPFCB _kernel_mpfcb_$ARGV[1]$ $CLASS_SECTION_OBJCB[MPF.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  CYCCBの配置先指定
$ 
$FUNCTION GENERATE_CYCCB$
	CYCCB _kernel_cyccb_$ARGV[1]$$TAB$$CLASS_SECTION_OBJCB[CYC.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  ALMCBの配置先指定
$ 
$FUNCTION GENERATE_ALMCB$
	ALMCB _kernel_almcb_$ARGV[1]$$TAB$$CLASS_SECTION_OBJCB[ALM.CLASS[ARGV[1]]]$;$NL$
$END$

$ 
$  SPNCBの配置先指定
$ 
$FUNCTION GENERATE_SPNCB$
	SPNCB _kernel_spncb_$ARGV[1]$ $CLASS_SECTION_OBJCB[SPN.CLASS[ARGV[1]]]$;$NL$
$END$


$ 
$  コア依存テンプレートのインクルード（ARM用）
$ 
$INCLUDE"../../arch/arm_gcc/common/core.tf"$

$ 
$  割込み優先度テーブル
$ 

$FOREACH prcid RANGE(1, TNUM_PRCID)$

const PRI _kernel_prc$prcid$_inh_ipm_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF (inhno & 0xffff0000) == (prcid << 16) $
		$IF LENGTH(INH.INHNO[inhno]) && (CLASS_AFFINITY_INI[INH.CLASS[inhno]] == prcid)$
		  $TAB$$INT.INTPRI[inhno]$,
		$ELSE$
		  $TAB$0,
		$END$
		$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
	$END$
$END$
};$NL$
$NL$

$END$

$ 
$  割込み優先度テーブルアクセステーブル
$ 
const PRI* const _kernel_p_inh_ipm_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_inh_ipm_table
$END$
$NL$};$NL$
$NL$

$ 
$  割込みマスクテーブル
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const uint32_t _kernel_prc$prcid$_ipm_mask_table[8]={$NL$
$FOREACH intpri { 0,-1,...,-7 }$
	$intmask = 0$
	$FOREACH intno (INT.ID_LIST)$
		$IF CLASS_AFFINITY_INI[INT.CLASS[intno]] == prcid $
			$IF INT.INTPRI[intno] >= intpri $
				$intmask = intmask | (1 << (INT.INTNO[intno] & 0x0000FFFF))$
			$END$
		$END$
	 $END$
 $TAB$UINT32_C($FORMAT("0x%08x", intmask)$), /* Priority $+intpri$ */$NL$
$END$
};$NL$
$NL$
$END$

$ 
$  割込みマスクテーブルアクセステーブル
$ 
const uint32_t* const _kernel_p_ipm_mask_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_ipm_mask_table
$END$
$NL$};$NL$
$NL$

$ 
$  割込みハンドラテーブル
$ 
$FOREACH prcid RANGE(1, TNUM_PRCID)$

const FP _kernel_prc$prcid$_inh_table[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$
	$IF (inhno & 0xffff0000) == (prcid << 16) $
		$IF LENGTH(INH.INHNO[inhno]) && (CLASS_AFFINITY_INI[INH.CLASS[inhno]] == prcid)$
			$TAB$(FP)($INH.INTHDR[inhno]$),
		$ELSE$
			$TAB$(FP)(_kernel_default_int_handler),
		$END$
		$SPC$$FORMAT("/* 0x%x */", +inhno)$$NL$
	$END$
$END$
};$NL$
$NL$

$END$

$ 
$  割込みハンドラテーブルアクセステーブル
$ 
const FP* const _kernel_p_inh_table[TNUM_PRCID] = {$NL$
$JOINEACH prcid RANGE(1, TNUM_PRCID) ",\n"$
	$TAB$_kernel_prc$prcid$_inh_table
$END$
$NL$};$NL$
$NL$
