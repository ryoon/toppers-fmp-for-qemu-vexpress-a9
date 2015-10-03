/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H

/*
 *  start.S
 */
#undef start
#undef start_master

/*
 *  target_config.c 
 */
#undef target_mprc_initialize
#undef target_initialize
#undef target_exit
#undef x_config_int
#undef default_int_handler

/*
 *  target_support.S
 */
#undef target_exc_handler
#undef target_ret_exc
#undef target_ret_int
#undef irq_handler

/*
 *  kernel_cfg.c
 */
#undef p_inh_table
#undef p_ipm_mask_table
#undef p_inh_ipm_table
#undef p_tacb_table

/*
 *  trace_config.c
 */
#undef log_dsp_enter
#undef log_dsp_leave
#undef log_inh_enter
#undef log_inh_leave
#undef log_exc_enter
#undef log_exc_leave


#ifdef TOPPERS_LABEL_ASM

/*
 *  start.S
 */
#undef _start
#undef _start_master

/*
 *  target_config.c 
 */
#undef _target_mprc_initialize
#undef _target_initialize
#undef _target_exit
#undef _x_config_int
#undef _default_int_handler

/*
 *  target_support.S
 */
#undef _target_exc_handler
#undef _target_ret_exc
#undef _target_ret_int
#undef _irq_handler

/*
 *  kernel_cfg.c
 */
#undef _p_inh_table
#undef _p_ipm_mask_table
#undef _p_inh_ipm_table
#undef _p_tacb_table

/*
 *  trace_config.c
 */
#undef _log_dsp_enter
#undef _log_dsp_leave
#undef _log_inh_enter
#undef _log_inh_leave
#undef _log_exc_enter
#undef _log_exc_leave


#endif /* TOPPERS_LABEL_ASM */

#include "arm_gcc/common/core_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
