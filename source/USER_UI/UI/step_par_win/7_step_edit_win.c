
/**
  ******************************************************************************
  * @file    7_step_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.13
  * @brief   7寸屏步骤编辑窗口布局信息
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



#define STEP_EDIT_UI_WIDTH     690
#define STEP_EDIT_UI_HEIGHT    455
/**
  * @brief  7寸屏步骤编辑窗口位置尺寸初始化信息
  */
WIDGET_POS_SIZE_T _7_step_edit_windows=
{
	0/*x*/,0/*y*/,STEP_EDIT_UI_WIDTH/*width*/,STEP_EDIT_UI_HEIGHT/*height*/,
};

#define FONT    &GUI_Fonthz_24

/**
  * @brief  7寸屏步骤编辑窗口编辑对象自动布局结构初始化信息
  */
EDIT_ELE_AUTO_LAYOUT_T  _7_step_par_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    120/*name_w*/,150/*edit_w*/,25/*uint_w*/,30/*height*/,
    8/*rows*/,2/*column*/,45/*row_spacing*/,320/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER,20/*max_len*/
};



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
