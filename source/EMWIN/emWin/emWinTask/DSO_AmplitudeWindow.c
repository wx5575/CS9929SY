/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵķ�ֵ����
*	�ļ����� : DSO_AmplititudeWindow.c
*	��    �� : V1.0
*	˵    �� : ��ʾʾ������ǰ�ķ�ֵ
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
*	                   GUI_WIDGET_CREATE_INFO��������
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
*	�� �� ��: PaintDialogAmp
*	����˵��: ��ֵ�����ػ���Ϣ�еĺ���
*	��    �Σ�pMsg  ָ���ַ      	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PaintDialogAmp(WM_MESSAGE * pMsg)
{
	/* ��һ�������Ʊ���Ϊ��ɫ */
	GUI_SetBkColor(GUI_BLACK);
  	GUI_Clear();
	
	/* �ڶ��������ư�ɫ�߿򣬻������� */
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRect( 0,        /* Upper left X-position.  */
				  0,        /* Upper left Y-position.  */
				  189,      /* Lower right X-position. */
	              34);      /* Lower right Y-position. */
	
    GUI_DrawRect( 1,      /* Upper left X-position.  */
			      1,      /* Upper left Y-position.  */
	              188,    /* Lower right X-position. */
	              33);    /* Lower right Y-position. */
	
	/* �����������һ����ɫԲ�Ǿ��� */
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 3,  /* Upper left X-position */
	                     7,  /* Upper left Y-position. */
	                    30,  /* Lower right X-position. */
	                    29,  /* Lower right Y-position. */
	                    4);
	
	/* ���Ĳ����ڻ�ɫ��Բ������дһ������ */
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 10, 6);
}

/*
*********************************************************************************************************
*	�� �� ��: PaintDialogAmp
*	����˵��: ��ֵ���ڳ�ʼ������Ϣ����
*	��    �Σ�pMsg  ָ���ַ      	
*	�� �� ֵ: ��
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
*	�� �� ��: _cbCallbackAmp()
*	����˵��: ��ֵ���ڻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
*	�� �� ��: CreateWindowAmplitude
*	����˵��: ������ֵ����
*	��    �Σ���         	
*	�� �� ֵ: ���������ڵľ��
*********************************************************************************************************
*/
WM_HWIN CreateWindowAmplitude(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateAmp, GUI_COUNTOF(_aDialogCreateAmp), &_cbCallbackAmp, 0, 0, 0);
	return hWin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
