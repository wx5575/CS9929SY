/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器测量对话框
*	文件名称 : DSO_MeasureDlg.c
*	版    本 : V1.0
*	说    明 : 支持30种数据测量功能，当前实现平均值，峰峰值，最小值和最大值4种数值测量。
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
#define ID_CHECKBOX_2  (GUI_ID_USER + 0x03)
#define ID_CHECKBOX_3  (GUI_ID_USER + 0x04)
#define ID_CHECKBOX_4  (GUI_ID_USER + 0x05)
#define ID_CHECKBOX_5  (GUI_ID_USER + 0x06)
#define ID_CHECKBOX_6  (GUI_ID_USER + 0x07)
#define ID_CHECKBOX_7  (GUI_ID_USER + 0x08)
#define ID_CHECKBOX_8  (GUI_ID_USER + 0x09)
#define ID_CHECKBOX_9  (GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_10  (GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_11  (GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_12  (GUI_ID_USER + 0x0D)
#define ID_CHECKBOX_13  (GUI_ID_USER + 0x0E)
#define ID_CHECKBOX_14  (GUI_ID_USER + 0x0F)
#define ID_CHECKBOX_15  (GUI_ID_USER + 0x10)
#define ID_CHECKBOX_16  (GUI_ID_USER + 0x12)
#define ID_CHECKBOX_17  (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_18  (GUI_ID_USER + 0x16)
#define ID_CHECKBOX_19  (GUI_ID_USER + 0x17)
#define ID_CHECKBOX_20  (GUI_ID_USER + 0x18)
#define ID_CHECKBOX_21  (GUI_ID_USER + 0x19)
#define ID_CHECKBOX_22  (GUI_ID_USER + 0x1A)
#define ID_CHECKBOX_23  (GUI_ID_USER + 0x1C)
#define ID_CHECKBOX_24  (GUI_ID_USER + 0x1D)
#define ID_CHECKBOX_25  (GUI_ID_USER + 0x1E)
#define ID_CHECKBOX_26  (GUI_ID_USER + 0x1F)
#define ID_CHECKBOX_27  (GUI_ID_USER + 0x20)
#define ID_CHECKBOX_28  (GUI_ID_USER + 0x21)
#define ID_CHECKBOX_29  (GUI_ID_USER + 0x22)

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 16, 11, 114, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 16, 50, 96, 21, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_2, 16, 90, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_3, 16, 130, 96, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_4, 16, 170, 97, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_5, 16, 210, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_6, 16, 250, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_7, 16, 290, 161, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_8, 16, 330, 171, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_9, 16, 370, 150, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_10, 266, 11, 157, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_11, 266, 50, 107, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_12, 266, 90, 121, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_13, 266, 130, 95, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_14, 266, 170, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_15, 266, 210, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_16, 266, 250, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_17, 266, 290, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_18, 266, 330, 153, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_19, 266, 370, 155, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_20, 533, 16, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_21, 533, 50, 109, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_22, 533, 90, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_23, 533, 130, 99, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_24, 533, 170, 162, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_25, 533, 210, 171, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_26, 533, 250, 147, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_27, 533, 290, 153, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_28, 533, 330, 80, 20, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_29, 533, 370, 103, 20, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 测量对话框的回调函数
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
		FRAMEWIN_SetTextColor(hItem, 0x00FFFFFF);
		FRAMEWIN_SetText(hItem, "Measure");
		FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
		FRAMEWIN_SetTitleHeight(hItem, 28);
		
		//
		// 初始化 'ID_CHECKBOX_0'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
		CHECKBOX_SetText(hItem, "Snapshot All");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[0]);
		
		//
		// 初始化 'ID_CHECKBOX_1'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
		CHECKBOX_SetText(hItem, "Freqency");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[1]);
// 		CHECKBOX_SetTextColor(hItem, 0x0000ff);    /* 标成红体表示当前支持的测量 */
		 
		//
		// 初始化 'ID_CHECKBOX_2'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
		CHECKBOX_SetText(hItem, "Period");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[2]);
		
		//
		// 初始化 'ID_CHECKBOX_3
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
		CHECKBOX_SetText(hItem, "Rise Time");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[3]);
		
		//
		// 初始化 'ID_CHECKBOX_4'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4);
		CHECKBOX_SetText(hItem, "Fall Time");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[4]);
		
		//
		// 初始化 'ID_CHECKBOX_5'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5);
		CHECKBOX_SetText(hItem, "Delay");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[5]);
		
		//
		// 初始化 'ID_CHECKBOX_6'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6);
		CHECKBOX_SetText(hItem, "Phase");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[6]);
		
		//
		// 初始化 'ID_CHECKBOX_7'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7);
		CHECKBOX_SetText(hItem, "Positive Pulse Width");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[7]);
		
		//
		// 初始化 'ID_CHECKBOX_8'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8);
		CHECKBOX_SetText(hItem, "Negative Pulse Width");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[8]);
		
		//
		// 初始化 'ID_CHECKBOX_9'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9);
		CHECKBOX_SetText(hItem, "Positive Duty Cycle");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[9]);
		
		//
		// 初始化 'ID_CHECKBOX_10'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10);
		CHECKBOX_SetText(hItem, "Negative Duty Cycle");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[10]);
		
		//
		// 初始化 'ID_CHECKBOX_11'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11);
		CHECKBOX_SetText(hItem, "Burst Width");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[11]);
		
		//
		// 初始化 'ID_CHECKBOX_12'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_12);
		CHECKBOX_SetText(hItem, "Peak-to-peak");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[12]);
		CHECKBOX_SetTextColor(hItem, 0x0000ff);    /* 标成红体表示当前支持的测量 */
		
		//
		// 初始化 'ID_CHECKBOX_13'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_13);
		CHECKBOX_SetText(hItem, "Amplitude");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[13]);
		
		//
		// 初始化 'ID_CHECKBOX_14'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_14);
		CHECKBOX_SetText(hItem, "Max");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[14]);
		CHECKBOX_SetTextColor(hItem, 0x0000ff);    /* 标成红体表示当前支持的测量 */
		
		//
		// 初始化 'ID_CHECKBOX_15'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_15);
		CHECKBOX_SetText(hItem, "Min");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[15]);
		CHECKBOX_SetTextColor(hItem, 0x0000ff);    /* 标成红体表示当前支持的测量 */
		
		//
		// 初始化 'ID_CHECKBOX_16'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_16);
		CHECKBOX_SetText(hItem, "High");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[16]);
		
		//
		// 初始化 'ID_CHECKBOX_17'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_17);
		CHECKBOX_SetText(hItem, "Low");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[17]);
		
		//
		// 初始化 'ID_CHECKBOX_18'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_18);
		CHECKBOX_SetText(hItem, "Positive Overshoot");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[18]);
		
		//
		// 初始化 'ID_CHECKBOX_19'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_19);
		CHECKBOX_SetText(hItem, "Negative Overshoot");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[19]);
		
		//
		// 初始化 'ID_CHECKBOX_20'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_20);
		CHECKBOX_SetText(hItem, "Mean");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[20]);
		CHECKBOX_SetTextColor(hItem, 0x0000ff);    /* 标成红体表示当前支持的测量 */
		
		//
		// 初始化 'ID_CHECKBOX_21'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_21);
		CHECKBOX_SetText(hItem, "Cycle Mean");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[21]);
		
		//
		// 初始化 'ID_CHECKBOX_22'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_22);
		CHECKBOX_SetText(hItem, "RMS");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[22]);
		
		//
		// 初始化 'ID_CHECKBOX_23'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_23);
		CHECKBOX_SetText(hItem, "Cycle RMS");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[23]);
		
		//
		// 初始化 'ID_CHECKBOX_24'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_24);
		CHECKBOX_SetText(hItem, "Positive Pulse Count");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[24]);
		
		//
		// 初始化 'ID_CHECKBOX_25'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_25);
		CHECKBOX_SetText(hItem, "Negative Pulse Count");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[25]);
		
		//
		// 初始化 'ID_CHECKBOX_26'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_26);
		CHECKBOX_SetText(hItem, "Rising Edge Count");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[26]);
		
		//
		// 初始化 'ID_CHECKBOX_27'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_27);
		CHECKBOX_SetText(hItem, "Falling Edge Count");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[27]);
		
		//
		// 初始化 'ID_CHECKBOX_28'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_28);
		CHECKBOX_SetText(hItem, "Area");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[28]);
		
		//
		// 初始化 'ID_CHECKBOX_29'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_29);
		CHECKBOX_SetText(hItem, "Cycle Area");
		CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
		CHECKBOX_SetState(hItem, g_ucMeasureFlag[29]);
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
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[0] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0));
						break;
					
					/* 复选框状态改变消息 */
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
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[1] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_2: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[2] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			
			/* 复选框消息 */
			case ID_CHECKBOX_3: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[3] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			
			/* 复选框消息 */
			case ID_CHECKBOX_4: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[4] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_4));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			
			/* 复选框消息 */
			case ID_CHECKBOX_5: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[5] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_5));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			
			/* 复选框消息 */
			case ID_CHECKBOX_6: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[6] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_6));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_7: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[7] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_7));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_8: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[8] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_8));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_9: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[9] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_9));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
			
			/* 复选框消息 */
			case ID_CHECKBOX_10: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[10] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_10));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_11: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[11] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_11));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_12: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[12] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_12));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_13: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[13] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_13));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_14: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[14] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_14));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_15: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[15] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_15));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_16: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[16] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_16));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_17: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[17] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_17));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_18: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[18] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_18));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_19: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[19] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_19));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_20: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[20] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_20));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_21: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[21] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_21));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_22: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[22] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_22));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_23: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[23] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_23));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_24: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[24] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_24));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_25: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[25] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_25));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_26: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[26] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_26));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_27: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[27] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_27));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_28: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[28] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_28));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
						break;
				}
				break;
				
			/* 复选框消息 */
			case ID_CHECKBOX_29: 
				switch(NCode) 
				{
					/* 复选框点击消息 */
					case WM_NOTIFICATION_CLICKED:
						break;
					
					/* 复选框释放消息 */
					case WM_NOTIFICATION_RELEASED:
						g_ucMeasureFlag[29] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_29));
						break;
					
					/* 复选框状态改变消息 */
					case WM_NOTIFICATION_VALUE_CHANGED:
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
*	函 数 名: DSO_CreateMeasureDlg
*	功能说明: 创建测量对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateMeasureDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
