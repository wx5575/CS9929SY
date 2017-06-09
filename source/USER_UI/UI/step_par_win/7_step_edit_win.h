
/**
  ******************************************************************************
  * @file    file_ui_save.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "app.h"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "UI_COM/com_ui_info.h"
#include "step_edit_win.h"
#include "ui_com/com_edit_api.h"
#include "key_menu_win/key_menu_win.h"


extern WIDGET_POS_SIZE_T        _7_step_edit_windows;
extern EDIT_ELE_AUTO_LAYOUT_T   _7_step_par_edit_ele_auto_layout_inf;


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
