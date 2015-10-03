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
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 *
 *  @(#) $Id: chip_support.S 955 2012-10-30 07:31:22Z ertl-honda $
 */

/*
 *  �������åȰ�¸�⥸�塼�� ������֥��������ARM�ѡ�
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t�����������ޥ��� */
#define ULONG_C(val)	(val)		/* ulong_t�����������ޥ��� */
#include "kernel_impl.h"
#include "target_asm.inc"
#include "target_ipi.h"
#include "offset.h"

/*
 *  ���٥�Υ������åȥ����ƥ��¸�ν����
 *
 *  �������ȥ��åץ⥸�塼�����ǡ��������������˸ƤӽФ���롥
 */
    ATEXT
    AALIGN(2)
    AGLOBAL(hardware_init_hook)
ALABEL(hardware_init_hook)
#ifdef TOPPERS_ENA_PERF
    stmfd   sp!, {lr}
    bl      perf_boot_init
    ldmfd   sp!, {lr}
#endif /* TOPPERS_ENA_PERF */
    bx  lr

/*
 * ����ߥϥ�ɥ�
 */
    ATEXT
    AALIGN(2)
    AGLOBAL(irq_handler)
ALABEL(irq_handler)
    /*
     * IRQ�⡼�ɤǼ¹Ԥ����
     */
    sub   lr, lr, #4 /* ����襢�ɥ쥹  */

    /*
     * lr(�����) spsr(cpsr_svc)�򥹡��ѡ��Х������⡼�ɤΥ����å���
     * ��¸��
     */
    srsdb #CPSR_SVC!

    /*
     *  ��������Υ⡼��(�����ѡ��Х������⡼��)��
     *  �ܹԤ�������ƥ����Ȥ���¸����
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)
    stmfd sp!, {r0-r3,ip,lr}

    /*
     *  PCB�μ���
     */
    my_pcb r0, r3

#if TTYPE_KLOCK != G_KLOCK
    /*
     *  �ץ����å�����γ�٥��å��ξ��ϥ��å���������Ƥ��뤫�����å�����
     *  �������Ƥ���Х��å���������롥
     */
ALABEL(release_lock)
    ldr   r3, [r0, #PCB_p_firstlock]  /* p_firstlock ����� */
    cmp   r3, #0                      /* NULL�ʤ齪λ       */
    beq   release_lock_end
    mov   r2, #0
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev
    str   r2, [r0, #PCB_p_firstlock]  /* p_firstlock �� NULL �� */
    ldr   r3, [r0, #PCB_p_secondlock] /* p_secondlock ����� */
    cmp   r3, #0                      /* NULL�ʤ齪λ       */
    mcr   p15, 0, r2, c7, c10, 5      /* data_memory_barrier */
    str   r2, [r3]
    mcr   p15, 0, r2, c7, c10, 4      /* data_sync_barrier */
    sev
    str   r2, [r0, #PCB_p_firstlock]  /* p_secondlock �� NULL �� */
ALABEL(release_lock_end)
#endif /* TTYPE_KLOCK != G_KLOCK */

    mov   lr, sp         /* ���λ����Υ����å��������Τ������ */

    /*
     *  �������װ���Ƚ�ꡥ
     */
    ldr   r2, =(DIC_CPUIF_IAR)
    ldr   r12, [r2]
    mov   r1, r12, lsl #22  /* ����10�ӥåȰʳ��򥯥ꥢ */
    mov   r1, r1,  lsr #22

   /*
    *  spurious�����(INT_ID=1023(0x03ff))�ξ�硤����߽�����λ
    */
   ldr   r2, =(0x03ff)
   cmp   r2, r1
   beq   ret_int_2

#ifdef TOPPERS_SAFEG_SECURE
   /*
    *  TrustZone���Ѥ���Ȥ��ϡ�INT_ID=1022(0x03fe)��
    *  spurious����ߤȤʤ롥
    */
   ldr   r2, =(0x03fe)
   cmp   r2, r1
   beq   ret_int_2
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef USE_IPI_DIS_HANDER_BYPASS
    cmp     r1, #IPINO_DIS
    ldreq   r2,=DIC_CPUIF_EOI
    streq   r12,[r2]                /* EOI�쥸�����ؤν񤭹��� */
    beq     ret_int_ipi_dispatch
#endif /* USE_IPI_DIS_HANDER_BYPASS */

    /*
     *  ¿�ų���ߤ�Ƚ��
     */
    ldr   r3, [r0, #PCB_excpt_nest_count]  /* �㳰������ߥͥ��Ȳ������� */
    add   r2, r3, #1                       /* �㳰������ߥͥ��Ȳ���򹹿� */
    str   r2, [r0, #PCB_excpt_nest_count]
    cmp   r3, #0
    bne   irq_handler_1

    /*
     * �����ܤ��㳰�ʳ���ߡˤʤ饹���å����ѹ�����
     */
    ldr   sp, [r0, #PCB_istkpt]

ALABEL(irq_handler_1)
    stmfd sp!, {lr}     /* �����ѤΥ����å��ݥ��󥿤���¸ */

    /*
     *  ����ߥϥ�ɥ�Υ��ɥ쥹�����
     */
    ldr   r3, [r0, #PCB_p_inh_tbl]   /* ����ߥϥ�ɥ�ơ��֥���ɤ߹��� */
    ldr   r0, [r3, r1, lsl #2]       /* r0<-����ߥϥ�ɥ� */

    /*
     *  ������װ��γ����ͥ���٤���(��ǥ���)�����ͥ���٥ޥ����򥻥å�
     *  ���롥�ޤ����κݡ��ϥ�ɥ�¹�����(��ǥ���)�����ͥ���٥ޥ�����
     *  ��¸���롥
     */
    ldr   lr, =(DIC_CPUIF_RII)
    ldr   r2, [lr]
    ldr   r3, =(DIC_CPUIF_PMR) /* �����ȯ�����γ����ͥ���٥ޥ�������� */
    ldr   lr, [r3]
    str   r2, [r3]   /* �����������ͥ���٥ޥ����򥻥åȤ��� */
    stmfd sp!, {lr}  /* �����ȯ�����γ����ͥ���٥ޥ����򥹥��å�����¸ */

    stmfd sp!,{r12}      /* DIC_CPUIF_IAR ��������������Ƥ���¸  */
    stmfd sp!,{r1}       /* inhno ����¸  */

    /*
     * �����ߵ���
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_ALWAYS_SET)

#ifdef LOG_INH_ENTER
    stmfd sp!,{r0}
    mov   r0, r1         /* inhno ��ѥ�᡼����    */
    bl    log_inh_enter  /* log_inh_enter��ƤӽФ� */
    ldmfd sp!,{r0}
#endif /* LOG_INH_ENTER */

    /*
     * ����ߥϥ�ɥ�θƤӽФ�
     */
    mov   lr,  pc
    bx    r0

ALABEL(target_ret_int)

    ldmfd sp!,{r0}       /* inhno ������  */
    ldmfd sp!,{r12}      /* DIC_CPUIF_IAR ��������������Ƥ���¸  */

    ldr   r1,=DIC_CPUIF_EOI
    str   r12,[r1]               /* EOI�쥸�����ؤν񤭹��� */

#ifdef LOG_INH_LEAVE
    bl    log_inh_leave  /* log_inh_leave��ƤӽФ� */
#endif /* LOG_INH_LEAVE */

    /*
     * �����ͥ�����γ���ߤ�ػߤ���
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)

    /*
     * �����ͥ���٥ޥ����򸵤��᤹
     */
    ldmfd sp!,{r1}                       /* ���γ����ͥ���٥ޥ�������� */
    ldr   r0, =(DIC_CPUIF_PMR)           /* DIC�˥��å� */
    str   r1, [r0]

    /*
     * �����å��ݥ��󥿤�����
     */
    ldmfd sp!,{r2}              /* ���Υ����å��ݥ��󥿤���� */
    mov   sp, r2

    /*
     * ��ν�����ARM��¸���Ǽ¹�
     */
    b     ret_int


/*
 * �������åȰ�¸���㳰��������
 * ARMv6/7��ARMv4�ȸƤӽФ�����郎�ۤʤ뤿�����դ��뤳�ȡ�
 * �����ѡ��Х������⡼�ɤǸƤӽФ���롥
 */
    ATEXT
    AGLOBAL(target_exc_handler)
ALABEL(target_exc_handler)
    mov   lr, sp         /* ���λ����Υ����å��������Τ������ */

    ldr   r0, =(DIC_CPUIF_PMR) /* �㳰ȯ�����γ����ͥ���٥ޥ�������� */
    ldr   r3, [r0]
    asr   r3, r3, #DIC_CPUIF_PMR_OFFSET /* ����ɽ�����ѹ� */
    sub   r3, r3, #(IPM_BIT_MASK)
    stmfd sp!, {r3}  /* �㳰ȯ�����γ����ͥ���٥ޥ����򥹥��å�����¸ */

    /*
     *  ����ƥ�����Ƚ��Τ��ᡤexcpt_nest_count �򥹥��å�����¸��
     *  �����å�����¸���������ߤ�excpt_nest_count��-1����ȼ����Ǥ��뤬��
     *  �����å����Ѥ�Ǥ����������ǥХå������Ԥ��䤹���Τǡ������å���
     *  ��¸���롥
     */
    my_pcb r0, r3
    ldr   r3, [r0, #PCB_excpt_nest_count]
    stmfd sp!, {r3}

    mov   r3,  sp         /* �㳰�ե졼�����Ϥ���¸ */

    /*
     * �����ͥ��������CPU�㳰��Ƚ�ꤹ��
     *
     * �����ͥ��������CPU�㳰�ϡ������ͥ�¹��桤������ߥ��å����֡�
     * CPU���å����֡������ͥ�������γ���ߥϥ�ɥ�¹����ȯ������
     * CPU�㳰�Ǥ��롥ARM�ξ��ϡ�SPSR��I/F�ե饰�Τ����줫��'1'�ʤ顤
     * �������롥
     */
    ldr   r1, [sp, #(9*4)]              /* �㳰�ե졼�फ��spsr����� */
    tst   r1, #(CPSR_FIQ_BIT AOR CPSR_IRQ_BIT)
    bne   target_kernel_unc_exc_handler /* �����ͥ��������CPU�㳰�ν����� */

    /*
     *  �㳰ȯ�����Υ���ƥ����Ȥ�Ƚ��
     */
    ldr   r1, [r0, #PCB_excpt_nest_count]
    add   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]
    cmp   r1, #1
    bne   target_exc_handler_1

    /*
     * ����������ƥ����Ȥ�CPU�㳰��ȯ���������
     * �����å����󥿥�������ƥ����Ȥ��ڤ��ؤ��롥
     */
    ldr   sp, [r0, #PCB_istkpt]

ALABEL(target_exc_handler_1)
    stmfd sp!, {lr}     /* �����ѤΥ����å��ݥ��󥿤���¸ */

    /*
     *  locspnid����¸
     */
    my_pcb r0, r1
    ldr    r1, [r0,#PCB_locspnid]
    stmfd  sp!,{r1}

    /*
     * �����ȯ�����γ����ͥ���٥ޥ����򥹥��å�����¸
     * ����ߥϥ�ɥ�Ƚи��롼�����ͭ���뤿�����¸
     */
    ldr   r12, =(DIC_CPUIF_PMR) /* �㳰ȯ�����γ����ͥ���٥ޥ�������� */
    ldr   r1, [r12]
    stmfd sp!, {r1}  /* �㳰ȯ�����γ����ͥ���٥ޥ����򥹥��å�����¸ */

    /*
     *  CPU�㳰�ϥ�ɥ�ε�ư���Ϥ����
     */
    ldr   r12, [r0, #PCB_p_exch_tbl]  /* ����ߥϥ�ɥ�ơ��֥���ɤ߽Ф� */
    ldr   r1, [r12, r2, lsl #2]       /* r1<-�㳰�ϥ�ɥ�                 */

    stmfd sp!,{r2}      /* excno ����¸ */

    /*
     * �����ߵ���
     * CPU���������㳰�ϥ�ɥ���̥롼����ǸƤӽФ����ᡤñ���SVC�⡼��
     * �ˤ�������Ǥ褤��
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_ALWAYS_SET)

#ifdef LOG_EXC_ENTER
    stmfd sp!,{r1, r3}
    mov   r0, r2         /* excno ��ѥ�᡼����    */
    bl    log_exc_enter  /* log_exc_enter��ƤӽФ� */
    ldmfd sp!,{r1, r3}
#endif /* LOG_EXC_ENTER */

    /*
     * CPU�㳰�ϥ�ɥ�θƤӽФ�
     *
     * �㳰�ե졼�����Ƭ������Ȥ����Ϥ�
     */
    mov   r0,  r3
    mov   lr,  pc
    bx    r1

ALABEL(target_ret_exc)
    ldmfd sp!,{r0}       /* excno ������  */
#ifdef LOG_EXC_LEAVE
    bl    log_exc_leave  /* log_exc_leave��ƤӽФ� */
#endif /* LOG_EXC_LEAVE */

    /*
     * �����ͥ�����γ���ߤ�ػߤ���
     */
    msr   cpsr_cxsf, #(CPSR_SVC AOR CPSR_CPULOCK AOR CPSR_ALWAYS_SET)

    /*
     * �����ͥ���٥ޥ����򸵤��᤹
     */
    ldmfd sp!,{r1}                       /* ���γ����ͥ���٥ޥ�������� */
    ldr   r0, =(DIC_CPUIF_PMR)           /* DIC�˥��å� */
    str   r1, [r0]

    /*
     * CPU�㳰ȯ������locspnid��r0��
     */
    ldmfd sp!,{r0}

    /*
     * �����å��ݥ��󥿤�����
     */
    ldmfd sp!,{r2}              /* ���Υ����å��ݥ��󥿤���� */
    mov   sp, r2

    /*
     * ��ν�����ARM��¸���Ǽ¹�
     */
    b     ret_exc


/*
 * �����ͥ��������CPU�㳰�ν���������
 */
ALABEL(target_kernel_unc_exc_handler)
    /*
     * �����ȯ�����Υ���ƥ����Ȥ�Ƚ��
     */
    ldr   r1, [r0, #PCB_excpt_nest_count]
    add   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]
    cmp   r1, #1
    bne   target_kernel_unc_exc_handler_1

    /*
     * ����������ƥ����Ȥ�CPU�㳰��ȯ���������
     * �����å����󥿥�������ƥ����Ȥ��ڤ��ؤ��롥
     */
    ldr   sp, [r0, #PCB_istkpt]

ALABEL(target_kernel_unc_exc_handler_1)
    stmfd sp!, {lr}     /* �����ѤΥ����å��ݥ��󥿤���¸ */

    /*
     *  CPU�㳰�ϥ�ɥ�ε�ư���Ϥ����
     */
    ldr   r1, [r0, #PCB_p_exch_tbl]   /* ����ߥϥ�ɥ�ơ��֥���ɤ߽Ф� */
    ldr   r1, [r1, r2, lsl #2]        /* r1<-�㳰�ϥ�ɥ�                 */

   /*
    * �����ƥ���֡ʥ���ƥ����ȤϽ����ˤ�CPU�㳰ȯ�����ξ��֤�
    */
    ldr   r0, [r3, #(9*4)]     /* CPU�㳰ȯ������CPSR�μ��� */
    and   r0, r0, #(CPSR_IRQ_BIT AOR CPSR_FIQ_BIT)
    orr   r0, r0, #(CPSR_SVC AOR CPSR_ALWAYS_SET)
    msr   cpsr_cxsf, r0

    /*
     * CPU�㳰�ϥ�ɥ�θƤӽФ�
     * �㳰�ե졼�����Ƭ������Ȥ����Ϥ�
     */
    mov   r0, r3
    mov   lr, pc
    bx    r1

    /*
     *  �㳰������ߤΥͥ��ȥ�����ȡ�excpt_nest_count)�Υǥ������
     */
    my_pcb r0, r3
    ldr   r1, [r0, #PCB_excpt_nest_count]
    sub   r1, r1, #1
    str   r1, [r0, #PCB_excpt_nest_count]

    /*
     * �����å��ݥ��󥿤�����
     */
    ldmfd sp!, {r2}             /* ���Υ����å��ݥ��󥿤���� */
    mov   sp, r2

    /*
     * CPU�㳰���������
     */
    ldmfd sp!,{r0-r3,ip,lr}
    rfeia sp!