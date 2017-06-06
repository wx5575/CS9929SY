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
#include "env_par_edit_win.h"


WIDGET_POS_SIZE_T _7_env_par_windows=
{
	150/*x*/, 50/*y*/, 400/*width*/, 320/*height*/,
};


#define FONT    &GUI_Fonthz_24

/* 界面编辑对象自动布局结构 */
EDIT_ELE_AUTO_LAYOUT_T _7_env_par_edit_ele_auto_layout_inf=
{
    10/*base_x*/,50/*base_y*/,
    120/*name_w*/,150/*edit_w*/,25/*uint_w*/,30/*height*/,
    3/*rows*/,1/*column*/,50/*row_spacing*/,0/*column_spacing*/,
    FONT, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER,20/*max_len*/
};

void _7_init_env_par_win_edit_ele_pos(_WIDGET_ELEMENT_ *pool)
{
    CONFIGURABLE_PROPERTY inf;
    
    inf.width = 120;
    inf.height = 30;
    inf.font = FONT;
    inf.font_color = GUI_BLACK;
    inf.back_color = GUI_INVALID_COLOR;
    inf.align = GUI_TA_RIGHT;
    inf.max_len = 10;
    
    /* 语言 */
    inf.width = 120;
    inf.max_len = 10;
    inf.align = GUI_TA_RIGHT;
    memcpy(&pool[ENV_PAR_LANGUAGE].dis.name, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 150;
    inf.max_len = 14;
    inf.align = GUI_TA_CENTER;
    memcpy(&pool[ENV_PAR_LANGUAGE].dis.edit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
    inf.width = 25;
    inf.max_len = 10;
    inf.align = GUI_TA_LEFT;
    memcpy(&pool[ENV_PAR_LANGUAGE].dis.unit, &inf, sizeof(CONFIGURABLE_PROPERTY));
    
}
EDIT_ELE_DISPLAY_INF env_par_ele_pos_pool[]=
{
    /* 语言 */
    {
        /*name*/
        {
            120/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
            GUI_INVALID_COLOR/*backcolor*/, GUI_TA_RIGHT/*align*/,10/*max_len*/
        },
        /*edit*/
        {
            150/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
            GUI_INVALID_COLOR/*backcolor*/, GUI_TA_CENTER/*align*/,14/*max_len*/
        },
        /*unit*/
        {
            25/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
            GUI_INVALID_COLOR/*backcolor*/, GUI_TA_LEFT/*align*/,10/*max_len*/
        },
    },/*dis*/
};
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
