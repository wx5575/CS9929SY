/**
  ******************************************************************************
  * @file    7_key_menu_win.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   菜单键界面
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "GUI.H"
#include "fonts.h"
#include "7_key_menu_win.h"
#include "key_menu_win.h"
#include "string.h"


#define MENU_KEY_WIN_BASE_X		690
#define MENU_KEY_WIN_BASE_Y		0
#define MENU_KEY_WIN_WIDTH		110
#define MENU_KEY_WIN_HEIGHT		480 // (480 - MENU_KEY_AREA_BASE_Y - 28 - 30 - 25-5)
#define MENU_KEY_O				3
#define MENU_KEY_AREA_WIDTH		MENU_KEY_WIN_WIDTH //110 //(800 - MENU_KEY_AREA_BASE_X)
#define MENU_KEY_AREA_HEIGHT	MENU_KEY_WIN_HEIGHT //(480 - MENU_KEY_AREA_BASE_Y - 28 - CHILD_WIN_BASE_Y)
#define MENU_KEY_HEIGHT			((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))



/**
  * @brief  7寸屏按键菜单窗口尺寸
  */
WIDGET_POS_SIZE_T _7_key_menu_windows=
{
	MENU_KEY_WIN_BASE_X/*x*/,MENU_KEY_WIN_BASE_Y/*y*/,
    MENU_KEY_WIN_WIDTH/*width*/,MENU_KEY_WIN_HEIGHT/*height*/,
};

#define MK_B_X          MENU_KEY_O
#define MK_B_Y          MENU_KEY_O
#define MK_W            (MENU_KEY_AREA_WIDTH - 2 * MENU_KEY_O)
#define MK_H            ((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))
#define MK_FONTS_CN     &GUI_Fonthz_29
#define MK_FONTS_EN     &GUI_Fonthz_24
#define MK_ALIGN        GUI_TA_HCENTER | GUI_TA_VCENTER
#define MK_F_COLOR      GUI_WHITE
#define KEY_UP_COLOR    0x804000 //GUI_LIGHTGRAY//GUI_LIGHTBLUE //
#define KEY_EN_COLOR    KEY_UP_COLOR

/* 界面控件自动布局结构 */
TEXT_ELE_AUTO_LAYOUT_T _7_key_menu_auto_layout_inf=
{
   MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_W/*width*/,MK_H/*height*/,
    7/*rows*/,1/*column*/,MK_H + MENU_KEY_O/*row_spacing*/,0/*column_spacing*/,
    {MK_FONTS_CN, MK_FONTS_EN}, MK_F_COLOR, KEY_UP_COLOR, MK_ALIGN,20
};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
