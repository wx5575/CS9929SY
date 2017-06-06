/**
  ******************************************************************************
  * @file    scan_keyboard.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键扫描
  ******************************************************************************
  */

#ifndef __WARNING_WIN_H__
#define __WARNING_WIN_H__

#include "stm32f4xx.h"
#include "UI_COM/com_ui_info.h"


typedef enum {
	WARNING_UI_CONTENT,///<内容
}MAIN_UI_INDEX;

extern void create_warning_dialog(int hWin);
extern void set_warning_ui_inf(WARNING_INF *warning);

#endif //__WARNING_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
