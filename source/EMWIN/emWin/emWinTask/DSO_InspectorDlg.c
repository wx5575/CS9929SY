/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器Wave Inspector对话框
*	文件名称 : DSO_InspectorDlg.c
*	版    本 : V1.0
*	说    明 : 示波器Wave Inspector对话框，用于波形查询设置。
*              对话框使用GUIBulder5.22生成。
*	修改记录 :
*		版本号    日期          作者         说明
*		V1.0    2015-01-05    Eric2013       首发
*	
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                  自定义控件ID
*********************************************************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0  (GUI_ID_USER + 0x01)

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 10, 15, 152, 29, 0, 0x64, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;

	switch (pMsg->MsgId) 
	{
		/* 对话框关闭后使能一次波形刷新，防止在波形暂停状态下(通过K2按键将波形暂停)
		   关闭对话框并返回的时候，波形区域没有刷新，一片空白。
		*/
		case WM_DELETE:
			g_ucWaveRefreshFlag = 1;
			break;
		
		case WM_INIT_DIALOG:
			//
			// 初始化 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_24B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetTextColor(hItem, 0x00FFFFFF);
			FRAMEWIN_SetText(hItem, "Inspector");
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
			FRAMEWIN_SetTitleHeight(hItem, 28);
		
			//
			// 初始化 'Text'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetText(hItem, "NO Application");
			TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
			break;

		case WM_NOTIFY_PARENT:
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO_CreateInspectorDlg
*	功能说明: 创建对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateInspectorDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
