/**
  ******************************************************************************
  * @file    7_env_par_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   菜单键界面
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
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"



WIDGET_POS_SIZE_T _7_sys_time_windows=
{
	80/*x*/, 50/*y*/, 400/*width*/, 300/*height*/,
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
