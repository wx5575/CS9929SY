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


// #define FONT    &GUI_Fonthz_24

// DISPLAY_INF env_par_ele_pos_pool[]=
// {
//     /* 语言 */
//     {
//         /*name*/
//         {
//             120/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
//             GUI_INVALID_COLOR/*backcolor*/, GUI_TA_RIGHT/*align*/,10/*max_len*/
//         },
//         /*edit*/
//         {
//             150/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
//             GUI_INVALID_COLOR/*backcolor*/, GUI_TA_CENTER/*align*/,14/*max_len*/
//         },
//         /*unit*/
//         {
//             25/*width*/,30/*height*/,FONT/*font*/,GUI_BLACK/*fontcolor*/,
//             GUI_INVALID_COLOR/*backcolor*/, GUI_TA_LEFT/*align*/,10/*max_len*/
//         },
//     },/*dis*/
// };
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
