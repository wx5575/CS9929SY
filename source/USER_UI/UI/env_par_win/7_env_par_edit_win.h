/**
  ******************************************************************************
  * @file    7_sys_manage_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   系统管理界面
  ******************************************************************************
  */
#ifndef __7_ENV_PAR_EDIT_WIN_H__
#define __7_ENV_PAR_EDIT_WIN_H__


extern EDIT_ELE_AUTO_LAYOUT_T   _7_env_par_edit_ele_auto_layout_inf;
extern WIDGET_POS_SIZE_T        _7_env_par_windows;
extern EDIT_ELE_DISPLAY_INF     env_par_ele_pos_pool[];

extern void _7_init_env_par_win_edit_ele_pos(EDIT_ELE_T *pool);

#endif //__7_ENV_PAR_EDIT_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
