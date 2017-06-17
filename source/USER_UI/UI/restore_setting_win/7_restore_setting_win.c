/**
  ******************************************************************************
  * @file    7_restore_setting_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   7寸屏恢复出厂设置窗口布局信息定义
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

#define RES_SET_WIN_W   350
#define RES_SET_WIN_H   200
/**
  * @brief  7寸屏恢复出厂设置窗口的位置尺寸信息
  */
WIDGET_POS_SIZE_T _7_restore_setting_windows=
{
	80/*x*/, 50/*y*/, RES_SET_WIN_W/*width*/, RES_SET_WIN_H/*height*/,
};

#define FONT    &GUI_Fonthz_20

#define RES_SET_BASE_T_X      10
#define RES_SET_BASE_T_Y      50
#define RES_SET_ELE_T_W       RES_SET_WIN_W - RES_SET_BASE_T_X * 2
#define RES_SET_ELE_T_H       RES_SET_WIN_H - RES_SET_BASE_T_Y - 10

/**
  * @brief  7寸屏恢复出厂设置窗口文本对象自动布局结构
  */
TEXT_ELE_AUTO_LAYOUT_T _7_restore_setting_text_ele_auto_layout_inf=
{
   RES_SET_BASE_T_X/*base_x*/,RES_SET_BASE_T_Y/*base_y*/,
    RES_SET_ELE_T_W/*width*/,RES_SET_ELE_T_H/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    {FONT}, GUI_BLACK, GUI_INVALID_COLOR,
    GUI_TA_CENTER | GUI_TA_VCENTER,100
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
