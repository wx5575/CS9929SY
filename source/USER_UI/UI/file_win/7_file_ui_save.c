/**
  ******************************************************************************
  * @file    7_file_ui_save.c
  * @author  王鑫
  * @version V1.0.0
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
#include "7_file_ui_save.h"
#include "file_edit_win.h"
#include "string.h"


WIDGET_POS_SIZE_T _7_file_save_windows=
{
	150/*x*/, 50/*y*/, 400/*width*/, 320/*height*/,
};


#define FONT    &GUI_Fonthz_24

#define FONT    &GUI_Fonthz_24

/* 界面编辑对象自动布局结构 */
EDIT_ELE_AUTO_LAYOUT_T _7_file_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    120/*name_w*/,120/*edit_w*/,25/*uint_w*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER,20/*max_len*/
};

void _7_init_file_save_win_edit_ele_pos(WIDGET_ELEMENT *pool)
{
    CONFIGURABLE_PROPERTY inf;
    
    inf.width = 120;
    inf.height = 30;
    inf.font = FONT;
    inf.font_color = GUI_BLACK;
    inf.back_color = GUI_INVALID_COLOR;
    inf.align = GUI_TA_RIGHT;
    inf.max_len = 10;
    
    /* 文件名 */
    memcpy(&pool[FSAVE_UI_FNAME].dis.name, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 210;
    inf.max_len = 14;
    memcpy(&pool[FSAVE_UI_FNAME].dis.edit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 25;
    inf.max_len = 10;
    inf.align = GUI_TA_LEFT;
    memcpy(&pool[FSAVE_UI_FNAME].dis.unit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    /* 工作模式 */
    inf.width = 120;
    inf.max_len = 10;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[FSAVE_UI_WMODE].dis.name, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 120;
    inf.max_len = 10;
    inf.align = GUI_TA_CENTER;
    memcpy(&pool[FSAVE_UI_WMODE].dis.edit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 25;
    inf.max_len = 10;
    inf.align = GUI_TA_LEFT;
    memcpy(&pool[FSAVE_UI_WMODE].dis.unit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    /* PASS时间 */
    inf.width = 120;
    inf.max_len = 10;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[FSAVE_UI_PASST].dis.name, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 120;
    inf.max_len = 5;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[FSAVE_UI_PASST].dis.edit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 25;
    inf.max_len = 10;
    inf.align = GUI_TA_LEFT;
    memcpy(&pool[FSAVE_UI_PASST].dis.unit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    /* 蜂鸣时间 */
    inf.width = 120;
    inf.max_len = 10;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[FSAVE_UI_BEEPT].dis.name, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 120;
    inf.max_len = 5;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[FSAVE_UI_BEEPT].dis.edit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 25;
    inf.max_len = 10;
    inf.align = GUI_TA_LEFT;
    memcpy(&pool[FSAVE_UI_BEEPT].dis.unit, &inf, sizeof(CONFIGURABLE_PROPERTY));
}






/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
