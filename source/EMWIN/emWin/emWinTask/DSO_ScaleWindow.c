/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵ�ʱ������
*	�ļ����� : DSO_ScaleWindow.c
*	��    �� : V1.0
*	˵    �� : ��ʾʾ������ǰ��ʱ��
*	�޸ļ�¼ :
*		�汾��    ����          ����         ˵��
*		v1.0    2015-01-05    Eric2013       �׷�
*	
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateScale[] = 
{
    { WINDOW_CreateIndirect,     "",       0,             600,  445,  200,  35, 0},
	{ TEXT_CreateIndirect,      "Scale",   GUI_ID_TEXT0,   33,    7,   70,  24, 0, 0},
	{ TEXT_CreateIndirect,      "500ns / 3",   GUI_ID_TEXT1,  103,    7,   100,  24, 0, 0},
};

/*
*********************************************************************************************************
*	�� �� ��: PaintDialogScale()
*	����˵��: ʱ�����ڵĻص������ػ�
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PaintDialogScale(WM_MESSAGE * pMsg)
{
	GUI_SetBkColor(GUI_BLACK);
  	GUI_Clear();
	
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRect( 0,      /* Upper left X-position. */
			      0,      /* Upper left Y-position. */
	              199,    /* Lower right X-position. */
	              34);    /* Lower right Y-position. */
    GUI_DrawRect( 1,      /* Upper left X-position. */
			      1,      /* Upper left Y-position. */
	              198,    /* Lower right X-position. */
	              33);    /* Lower right Y-position. */
	
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 3, 7, 30, 29, 4);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 10, 6);
}

/*
*********************************************************************************************************
*	�� �� ��: InitDialogScale
*	����˵��: ʱ�����ڵĻص������ĳ�ʼ��
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogScale(WM_MESSAGE * pMsg)
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
*	�� �� ��: _cbCallbackScale()
*	����˵��: ʱ�����ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbCallbackScale(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
		 case WM_TextUpDate:
			TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1), g_TimeTable[TimeBaseId]);
			break;
		 
		case WM_PAINT:
            PaintDialogScale(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialogScale(pMsg);
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
*	�� �� ��: CreateWindowScale
*	����˵��: ʱ�����ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
WM_HWIN CreateWindowScale(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateScale, GUI_COUNTOF(_aDialogCreateScale), &_cbCallbackScale, 0, 0, 0);
	return hWin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
