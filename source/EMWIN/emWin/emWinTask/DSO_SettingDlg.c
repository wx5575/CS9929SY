/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器设置对话框
*	文件名称 : DSO_SettingDlg.c
*	版    本 : V1.0
*	说    明 : 主要设置如下5个配置
*              1. 设置是否隐藏幅值窗口，状态窗口和时基窗口。
*              2. 设置是否隐藏FFT计算的幅频响应波形。
*              3. 设置水平和垂直测量游标的显示。
*              4. 设置水平和垂直测量游标的移动步数。
*              5. 设置5种波形显示。
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
#define ID_CHECKBOX_0  (GUI_ID_USER + 0x01)
#define ID_CHECKBOX_1  (GUI_ID_USER + 0x02)
#define ID_RADIO_0  (GUI_ID_USER + 0x03)
#define ID_RADIO_1  (GUI_ID_USER + 0x04)
#define ID_RADIO_2  (GUI_ID_USER + 0x05)


/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
static uint32_t CheckBoxHideFFT = 0;     /* 第一次打开时默认未隐藏FFT波形显示 */
static uint32_t CheckBoxHideWindow = 0;  /* 第一次打开时默认未隐藏窗口 */
static uint32_t RadioCursorDisp = 0;     /* 第一次打开时默认不显示水平和垂直测量游标 */
static uint32_t RadioCursorStep = 3;     /* 第一次打开时默认选择水平和垂直游标都是移动10个像素 */
static uint32_t RadioLineStyle = 0;      /* 第一次打开时默认选择波形是实线绘制 */

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 15, 22, 465, 23, 0, 0x0, 0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_0, 15, 103, 195, 63, 0, 0x1403, 0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_1, 15, 194, 156, 84, 0, 0x1404, 0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_2, 15, 304, 167, 109, 0, 0x1405, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 15, 56, 374, 23, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 设置对话框的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

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
			FRAMEWIN_SetText(hItem, "Settings");
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
			FRAMEWIN_SetTitleHeight(hItem, 28);

			//
			// 初始化ID_CHECKBOX_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
			CHECKBOX_SetText(hItem, "Hide AmplitudeWindow, StatusWindow, ScaleWindow");
			CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
			CHECKBOX_SetState(hItem, CheckBoxHideWindow); /* 设置默认状态 */
						
			//
			// 初始化ID_CHECKBOX_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
			CHECKBOX_SetText(hItem, "Hide RFFT2048 Display");
			CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
			CHECKBOX_SetState(hItem, CheckBoxHideFFT); /* 设置默认状态 */

			//
			// 初始化ID_RADIO_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
			RADIO_SetText(hItem, "Hide Cursor", 0);
			RADIO_SetText(hItem, "Display HorizontalCursor", 1);
			RADIO_SetText(hItem, "Display VerticalCursor", 2);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioCursorDisp); /* 设置默认状态 */

			//
			// 初始化ID_RADIO_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
			RADIO_SetText(hItem, "MoveCursorStep=1", 0);
			RADIO_SetText(hItem, "MoveCursorStep=2", 1);
			RADIO_SetText(hItem, "MoveCursorStep=5", 2);
			RADIO_SetText(hItem, "MoveCursorStep=10", 3);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioCursorStep);  /* 设置默认状态 */
			
			//
			// 初始化ID_RADIO_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_2);
			RADIO_SetText(hItem, "LineStyle-Solid", 0);
			RADIO_SetText(hItem, "LineStyle-Dash", 1);
			RADIO_SetText(hItem, "LineStyle-Dot", 2);
			RADIO_SetText(hItem, "LineStyle-DashDot", 3);
			RADIO_SetText(hItem, "LineStyle-DashDotDot", 4);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioLineStyle); /* 设置默认状态 */
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				/* 复选框消息 */
				case ID_CHECKBOX_0: 
					switch(NCode) 
					{
						/* 复选框点击消息 */
						case WM_NOTIFICATION_CLICKED:
							break;
						
						/* 复选框点击释放后消息 */
						case WM_NOTIFICATION_RELEASED:
							CheckBoxHideWindow = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0));
							/* 选中复选框则隐藏幅值窗口，状态窗口和时基窗口 */
							if(CheckBoxHideWindow == 1)
							{
								WM_HideWindow(hWinAmp);
								WM_HideWindow(hWinStatus);
								WM_HideWindow(hWinScale);
							}
							/* 不选中则显示出来 */
							else
							{
								WM_ShowWindow(hWinAmp);
								WM_ShowWindow(hWinStatus);
								WM_ShowWindow(hWinScale);
							}
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;
				
				/* 复选框消息 */
				case ID_CHECKBOX_1: 
					switch(NCode) 
					{
						/* 复选框点击消息 */
						case WM_NOTIFICATION_CLICKED:
							break;
						
						/* 复选框点击释放后消息 */
						case WM_NOTIFICATION_RELEASED:
							CheckBoxHideFFT = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1));
							g_ucFFTDispFlag = CheckBoxHideFFT;
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;

				/* 单选按钮消息 */
				case ID_RADIO_0: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							/* 将水平和垂直游标的状态赋值给全局变量hWinCoursorFlag */
							RadioCursorDisp = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
							hWinCoursorFlag = RadioCursorDisp;
							break;
					}
					break;
				
				/* 单选按钮状态 */
				case ID_RADIO_1: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
						/* 水平和垂直游标的移动步数赋值给全局变量 */
						RadioCursorStep = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_1));
						if(RadioCursorStep == 0)
						{
							/* 每次移动1步 */
							g_usCursorStep = 1;
						}
						else if(RadioCursorStep == 1)
						{
							/* 每次移动2步 */
							g_usCursorStep = 2;			
						}
						else if(RadioCursorStep == 2)
						{
							/* 每次移动5步 */
							g_usCursorStep = 5;			
						}
						else if(RadioCursorStep == 3)
						{
							/* 每次移动10步 */
							g_usCursorStep = 10;			
						}
						break;
					}
					break;
				
				/* 单选按钮状态 */
				case ID_RADIO_2:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							/* 波形显示方式赋值给全局变量 */
							RadioLineStyle = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_2));
							g_ucLineStyle = RadioLineStyle;
							break;
					}
					break;
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO_CreateSettingsDlg
*	功能说明: 创建设置对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateSettingsDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
