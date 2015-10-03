/* This file is generated from core_rename.def by genrename. */

#ifndef TOPPERS_CORE_RENAME_H
#define TOPPERS_CORE_RENAME_H

/*
 *  start.S
 */
#define start_sync					_kernel_start_sync

/*
 *  core_support.S
 */
#define start_r						_kernel_start_r
#define ret_int						_kernel_ret_int
#define ret_exc						_kernel_ret_exc
#define vector_table				_kernel_vector_table
#define vector_ref_tbl				_kernel_vector_ref_tbl
#define undef_handler				_kernel_undef_handler
#define swi_handler					_kernel_swi_handler
#define prefetch_handler			_kernel_prefetch_handler
#define data_abort_handler			_kernel_data_abort_handler
#define interrupt_handler			_kernel_interrupt_handler
#define fiq_handler					_kernel_fiq_handler
#define dispatch					_kernel_dispatch
#define start_dispatch				_kernel_start_dispatch
#define exit_and_dispatch			_kernel_exit_and_dispatch
#define call_exit_kernel			_kernel_call_exit_kernel
#define default_exc_handler			_kernel_default_exc_handler
#define dispatch_and_migrate		_kernel_dispatch_and_migrate
#define exit_and_migrate			_kernel_exit_and_migrate
#define ret_int_ipi_dispatch		_kernel_ret_int_ipi_dispatch

/*
 *  core_config.c
 */
#define arm_vector_ref_tbl_add		_kernel_arm_vector_ref_tbl_add
#define arm_saved_exch_add			_kernel_arm_saved_exch_add
#define core_initialize				_kernel_core_initialize
#define core_terminate				_kernel_core_terminate
#define xlog_sys					_kernel_xlog_sys
#define x_install_exc				_kernel_x_install_exc


/*
 *  kernel_cfg.c
 */
#define p_accb_table				_kernel_p_accb_table
#define p_exch_table				_kernel_p_exch_table

#ifdef TOPPERS_LABEL_ASM

/*
 *  start.S
 */
#define _start_sync					__kernel_start_sync

/*
 *  core_support.S
 */
#define _start_r					__kernel_start_r
#define _ret_int					__kernel_ret_int
#define _ret_exc					__kernel_ret_exc
#define _vector_table				__kernel_vector_table
#define _vector_ref_tbl				__kernel_vector_ref_tbl
#define _undef_handler				__kernel_undef_handler
#define _swi_handler				__kernel_swi_handler
#define _prefetch_handler			__kernel_prefetch_handler
#define _data_abort_handler			__kernel_data_abort_handler
#define _interrupt_handler			__kernel_interrupt_handler
#define _fiq_handler				__kernel_fiq_handler
#define _dispatch					__kernel_dispatch
#define _start_dispatch				__kernel_start_dispatch
#define _exit_and_dispatch			__kernel_exit_and_dispatch
#define _call_exit_kernel			__kernel_call_exit_kernel
#define _default_exc_handler		__kernel_default_exc_handler
#define _dispatch_and_migrate		__kernel_dispatch_and_migrate
#define _exit_and_migrate			__kernel_exit_and_migrate
#define _ret_int_ipi_dispatch		__kernel_ret_int_ipi_dispatch

/*
 *  core_config.c
 */
#define _arm_vector_ref_tbl_add		__kernel_arm_vector_ref_tbl_add
#define _arm_saved_exch_add			__kernel_arm_saved_exch_add
#define _core_initialize			__kernel_core_initialize
#define _core_terminate				__kernel_core_terminate
#define _xlog_sys					__kernel_xlog_sys
#define _x_install_exc				__kernel_x_install_exc


/*
 *  kernel_cfg.c
 */
#define _p_accb_table				__kernel_p_accb_table
#define _p_exch_table				__kernel_p_exch_table

#endif /* TOPPERS_LABEL_ASM */


#endif /* TOPPERS_CORE_RENAME_H */
