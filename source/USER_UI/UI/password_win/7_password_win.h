/**
  ******************************************************************************
  * @file    7_sys_manage_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   系统管理界面
  ******************************************************************************
  */
#ifndef __7_PASSWORD_WIN_H__
#define __7_PASSWORD_WIN_H__

extern WIDGET_POS_SIZE_T        _7_password_windows;
extern TEXT_ELE_AUTO_LAYOUT_T   _7_pwd_text_ele_auto_layout_inf;
extern EDIT_ELE_AUTO_LAYOUT_T   _7_pwd_edit_ele_auto_layout_inf;

extern void _7_init_password_win_edit_ele_pos(EDIT_ELE_T *pool);
extern void _7_init_pwd_ui_text_ele_pos(TEXT_ELE_T *pool);

#endif //__7_PASSWORD_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
