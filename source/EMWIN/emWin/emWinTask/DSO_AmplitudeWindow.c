/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面上的幅值窗口
*	文件名称 : DSO_AmplititudeWindow.c
*	版    本 : V1.0
*	说    明 : 显示示波器当前的幅值
*	修改记录 :
*		版本号    日期          作者         说明
*		v1.0    2015-01-05    Eric2013       首发
*	
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"

/*
*********************************************************************************************************
*	                   GUI_WIDGET_CREATE_INFO类似数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateAmp[] = 
{
    { WINDOW_CreateIndirect,     "",      0,              0,     445,  190, 35, 0},
	{ TEXT_CreateIndirect,      "Amp",    GUI_ID_TEXT0,  33,     7,   40, 24, 0, 0},
	{ TEXT_CreateIndirect,      "1.00V",  GUI_ID_TEXT1,  90,     7,  100, 24, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialogAmp
*	功能说明: 幅值窗口重绘消息中的函数
*	形    参：pMsg  指针地址      	
*	返 回 值: 无
*********************************************************************************************************
*/
static void PaintDialogAmp(WM_MESSAGE * pMsg)
{
	/* 第一步：绘制背景为黑色 */
	GUI_SetBkColor(GUI_BLACK);
  	GUI_Clear();
	
	/* 第二步：绘制白色边框，绘制两次 */
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRect( 0,        /* Upper left X-position.  */
				  0,        /* Upper left Y-position.  */
				  189,      /* Lower right X-position. */
	              34);      /* Lower right Y-position. */
	
    GUI_DrawRect( 1,      /* Upper left X-position.  */
			      1,      /* Upper left Y-position.  */
	              188,    /* Lower right X-position. */
	              33);    /* Lower right Y-position. */
	
	/* 第三步：填充一个黄色圆角矩形 */
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 3,  /* Upper left X-position */
	                     7,  /* Upper left Y-position. */
	                    30,  /* Lower right X-position. */
	                    29,  /* Lower right Y-position. */
	                    4);
	
	/* 第四步：在黄色的圆角上填写一个数字 */
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 10, 6);
}

/*
*********************************************************************************************************
*	函 数 名: PaintDialogAmp
*	功能说明: 幅值窗口初始化的消息处理
*	形    参：pMsg  指针地址      	
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogAmp(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;

	//
	//GUI_ID_TEXT0
	//
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0),0x00ffff);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Font24_ASCII);

	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1),0x00ffff);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_Font24_ASCII);
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackAmp()
*	功能说明: 幅值窗口回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackAmp(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
	
    switch (pMsg->MsgId) 
    {
        case WM_TextUpDate:
			TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1), g_AttText[Ch1AmpId]);
			break;
			
		case WM_PAINT:
            PaintDialogAmp(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialogAmp(pMsg);
            break;
		
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateWindowAmplitude
*	功能说明: 创建幅值窗口
*	形    参：无         	
*	返 回 值: 所创建窗口的句柄
*********************************************************************************************************
*/
WM_HWIN CreateWindowAmplitude(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateAmp, GUI_COUNTOF(_aDialogCreateAmp), &_cbCallbackAmp, 0, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
