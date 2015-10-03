/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2010 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: start.S 30 2008-12-14 17:03:08Z ertl-honda $
 */

/*
 *   �����ͥ��ѤΥ������ȥ��åץ⥸�塼��(ARM��)
 */
#define    TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t�����������ޥ��� */
#define ULONG_C(val)	(val)		/* ulong_t�����������ޥ��� */
#include "kernel_impl.h"
#include "target_asm.inc"

/*
 *  Ʊ���ѤΥޥ��å��ʥ�С�
 */
#define MAGIC_START 0x87654321

    .text
    .code 32
    .align  2
    .global start
start:
    /*
     *  �ץ����å��⡼�ɤν����
     *  �����ѡ��Х������⡼�ɡ�FIQ/IRQ�ػ�
     */
    msr  cpsr, #(CPSR_SVC|CPSR_INTLOCK) 

    /*
     *  �����å��ݥ��󥿤ȡ��ե졼��ݥ��󥿤ν����
     *  sl(�����å���¥쥸����r10)�����ꤹ��ɬ�פ����뤫?
     *  gcc�ϥǥ��ե���ȤǤ�-mapcs�ʤΤ�ɬ�פʤ�
     */
    my_istkpt sp, r0   /* FMP */
    mov  fp, #0        /* ARM�⡼���ѥե졼��ݥ���(r11) */
    mov  r7, #0        /* Thumb�⡼���ѥե졼��ݥ���    */

    /*
     *  hardware_init_hook�θƤӽФ� ��0�Ǥʤ�����
     *
     *  �������åȥϡ��ɥ������˰�¸����ɬ�פʽ����������������
     *  �ϡ�hardware_init_hook �Ȥ����ؿ����Ѱդ���
     */
    ldr   r0, =hardware_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

    /*
     *  ����0�ʳ��Ͻ�����Ԥ�
     */
    my_core_index r0
    cmp  r0, #0
    bne  slave_wait

    /*
     *  ����0���ѿ�������
     */
    ldr  r0, =start_sync
    mov  r1, #0
    str  r1,[r0]

    /*
     * ����ѥ���ν�����롼�����ƤӽФ�
     */
    b __cs3_start_c


    /*
     *  software_init_hook ��ƽФ���0 �Ǥʤ�����
     *
     *  ���եȥ������Ķ����ä˥饤�֥��ˤ˰�¸����ɬ�פʽ������
     *  ����������ϡ�software_init_hook �Ȥ����ؿ����Ѱդ����
     *  �褤��
     */
    .global start_master
start_master:
    ldr   r0, =software_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

    /*
     *  target_mprc_initialize �θƤӽФ�
     *
     *  sta_ker �����˥ޥ����ץ����å��ΤߤǹԤ�ɬ�פΤ���������¹�
     */
start_6:
    bl   target_mprc_initialize

    /*
     *  �����ͥ��ư����
     */
start_7:
    ldr  r0, =start_sync
    ldr  r1, =MAGIC_START
    str  r1, [r0]
    bl   sta_ker

    /*
     *  ����0�ʳ��ν�����Ԥ��롼����
     */
slave_wait:
    ldr  r0, =start_sync
slave_wait_1:
    ldr  r2, =MAGIC_START
    ldr  r1, [r0]
    cmp  r1, r2
    bne  slave_wait_1
    bl   sta_ker