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


void _7_init_key_menu_win_text_ele_pos(TEXT_ELE_T *pool)
{
#define KEY_UP_COLOR    0x804000 //GUI_LIGHTGRAY//GUI_LIGHTBLUE //
#define KEY_EN_COLOR    KEY_UP_COLOR
#define MK_FONTS        &GUI_Fonthz_29
#define MK_ALIGN        GUI_TA_HCENTER | GUI_TA_VCENTER
#define MK_F_COLOR      GUI_WHITE
#define MK_B_X          0
#define MK_B_Y          0
#define MK_W            (MENU_KEY_AREA_WIDTH - 2 * MENU_KEY_O)
#define MK_H            ((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))
#define MK_F0_X         MK_B_X + MENU_KEY_O
#define MK_F1_X         MK_B_X + MENU_KEY_O
#define MK_F2_X         MK_B_X + MENU_KEY_O
#define MK_F3_X         MK_B_X + MENU_KEY_O
#define MK_F4_X         MK_B_X + MENU_KEY_O
#define MK_F5_X         MK_B_X + MENU_KEY_O
#define MK_F6_X         MK_B_X + MENU_KEY_O
#define MK_F0_Y         MK_B_Y + MENU_KEY_O
#define MK_F1_Y         MK_F0_Y + MENU_KEY_O + MK_H
#define MK_F2_Y         MK_F1_Y + MENU_KEY_O + MK_H
#define MK_F3_Y         MK_F2_Y + MENU_KEY_O + MK_H
#define MK_F4_Y         MK_F3_Y + MENU_KEY_O + MK_H
#define MK_F5_Y         MK_F4_Y + MENU_KEY_O + MK_H
#define MK_F6_Y         MK_F5_Y + MENU_KEY_O + MK_H
    
    UI_ELE_DISPLAY_INFO_T inf;
    
    /* 多路编号 */
    inf.base_x = MK_B_X;
    inf.base_y = MK_B_Y;
    inf.align = MK_ALIGN;
    inf.back_color = KEY_UP_COLOR;
    inf.font = MK_FONTS;
    inf.font_color = MK_F_COLOR;
    inf.pos_size.height = MK_H;
    inf.pos_size.width = MK_W;
    
    inf.pos_size.x = MK_F0_X;
    inf.pos_size.y = MK_F0_Y;
    
    memcpy(&pool[KEY_MENU_F0].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F1_X;
    inf.pos_size.y = MK_F1_Y;
    
    memcpy(&pool[KEY_MENU_F1].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F2_X;
    inf.pos_size.y = MK_F2_Y;
    
    memcpy(&pool[KEY_MENU_F2].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F3_X;
    inf.pos_size.y = MK_F3_Y;
    
    memcpy(&pool[KEY_MENU_F3].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F4_X;
    inf.pos_size.y = MK_F4_Y;
    
    memcpy(&pool[KEY_MENU_F4].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F5_X;
    inf.pos_size.y = MK_F5_Y;
    
    memcpy(&pool[KEY_MENU_F5].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    inf.pos_size.x = MK_F6_X;
    inf.pos_size.y = MK_F6_Y;
    
    memcpy(&pool[KEY_MENU_F6].dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}
///**
//  * @brief  菜单键窗口文本控件显示相关信息
//  */
//UI_ELE_DISPLAY_INFO_T key_ui_ele_dis_info[]=
//{
//	//按键菜单 F0
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F0_X/*x*/,MK_F0_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F1
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F1_X/*x*/,MK_F1_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F2
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F2_X/*x*/,MK_F2_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F3
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F3_X/*x*/,MK_F3_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F4
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F4_X/*x*/,MK_F4_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F5
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F5_X/*x*/,MK_F5_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//	//按键菜单 F6
//	{
//		MK_B_X/*base_x*/,MK_B_Y/*base_y*/,MK_F6_X/*x*/,MK_F6_Y/*y*/,MK_W/*width*/,MK_H/*height*/,100,
//		MK_FONTS, MK_F_COLOR, KEY_UP_COLOR,MK_ALIGN
//	},
//};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
