/**
  ******************************************************************************
  * @file    7_file_layout1.c
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

/**
  * @brief  7寸屏文件窗口尺寸
  */
WIDGET_POS_SIZE_T _7_file_windows=
{
	0/*x*/,0/*y*/,690/*width*/,455/*height*/,
};
/**
  * @brief  创建7寸屏文件管理使用的listview控件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
WM_HWIN _7_create_file_listview(WM_HWIN hWin)
{
    LISTVIEW_Handle list_h;
	HEADER_Handle hHeader;
	SCROLLBAR_Handle hScrollbar;
	int32_t i = 0;
	uint8_t buf[20] = {0};
    
	list_h = LISTVIEW_CreateEx(0, 0, 690, 455, hWin, WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW, 0, id_base++);
	hScrollbar = SCROLLBAR_CreateAttached(list_h, SCROLLBAR_CF_VERTICAL);
	
	LISTVIEW_SetFont(list_h, &GUI_Fonthz_24);
	
	hHeader = LISTVIEW_GetHeader(list_h);
	HEADER_SetFont(hHeader,&GUI_Fonthz_24);
	HEADER_SetHeight(hHeader,28);
	LISTVIEW_AddColumn(list_h, 60	, SELE_STR("编号","NO.")			, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 190	, SELE_STR("文件名","FileName")		, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 80	, SELE_STR("模式","Mode")			, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 80	, SELE_STR("总步数","Total")		, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 262	, SELE_STR("建立时间","CreateDate")	, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
    SCROLLBAR_SetWidth(hScrollbar,18);
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_THUMB, GUI_BLACK);//缩略图0x00A0A0A0
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_SHAFT, GUI_WHITE);  //轴
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_ARROW, GUI_GRAY);  //箭头
	WIDGET_SetEffect(hScrollbar, &WIDGET_Effect_None);
	
	for(i = 0; i < 30; i++)
	{
		LISTVIEW_AddRow(list_h, 0);
		sprintf((char *)buf, "%02d", i + 1);
		LISTVIEW_SetItemText(list_h, 0,i, (const char*)buf);
	}
    
	LISTVIEW_SetSel(list_h, 0);
    LISTVIEW_SetGridVis(list_h, 1);
	LISTVIEW_SetBkColor(list_h, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
	LISTVIEW_SetRowHeight(list_h, 30);
    
    return list_h;
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
