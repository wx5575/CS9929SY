/**
  ******************************************************************************
  * @file    MAIN_UI.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   7寸屏的布局1
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "malloc.h"
#include "ff.h"
#include "OS.H"
#include "app.h"
#include "scan_keyboard.h"
#include "IMAGE.H"
#include "7_INCH_ui/com_ui_info.h"
// #include "dialog_info.h"

void task_234(void);
USER_WINDOW_T main_window;///<主窗口


void into_test_ui(int d)
{
    g_cur_win = del_user_window(g_cur_win);
    show_cur_win(g_cur_win);
}

void main_ui(void)
{
	while(1)
	{
		GUI_Delay(2);
		
// 		if(NULL != golbal_key_info)
// 		{
// 			golbal_key_info(g_cur_menu_key_info->user_data);
// 			golbal_key_info = NULL;
// 		}
	}
}
