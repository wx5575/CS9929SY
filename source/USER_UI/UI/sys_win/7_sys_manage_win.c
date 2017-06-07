/**
  ******************************************************************************
  * @file    7_sys_manage_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
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


#define SYS_UI_WIDTH     690
#define SYS_UI_HEIGHT    455

WIDGET_POS_SIZE_T _7_sys_windows=
{
	0/*x*/,0/*y*/,SYS_UI_WIDTH/*width*/,SYS_UI_HEIGHT/*height*/,
};

#define SYS_BASE_X      200
#define SYS_BASE_Y      100
#define SYS_ELE_W       200

/* 界面控件自动布局结构 */
TEXT_ELE_AUTO_LAYOUT_T _7_sys_auto_layout_inf=
{
   SYS_BASE_X/*base_x*/,SYS_BASE_Y/*base_y*/,SYS_ELE_W/*width*/,30/*height*/,
    10/*rows*/,1/*column*/,30/*row_spacing*/,0/*column_spacing*/,
    {&GUI_Fonthz_20}, GUI_WHITE, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER,20
};


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
