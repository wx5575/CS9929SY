/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵ�״̬����
*	�ļ����� : DSO_StatusWindow.c
*	��    �� : V1.0
*	˵    �� : ʾ�����������ϵ�״̬���ڣ�������ʾ���ֵ�����ֵ��ƽ��ֵ����Сֵ��4��
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
static const GUI_WIDGET_CREATE_INFO _aDialogCreateStatus[] = 
{
    { WINDOW_CreateIndirect,     "",    0,    195,  420,  400, 60,0},
	{ TEXT_CreateIndirect,    "Mean",    GUI_ID_TEXT0,   33,  5,  70, 24, 0,0},
    { TEXT_CreateIndirect,    "Min",    GUI_ID_TEXT1,   33,  33, 70, 24, 0,0},
	{ TEXT_CreateIndirect,    "Pk-Pk",   GUI_ID_TEXT2,   233,  5,  70, 24, 0,0},
    { TEXT_CreateIndirect,    "Max",     GUI_ID_TEXT3,   233,  33, 70, 24, 0,0},
	
	{ TEXT_CreateIndirect,    "0.000V",   GUI_ID_TEXT4,   33+80,  5,  120, 24, 0,0},
    { TEXT_CreateIndirect,    "0.000V",   GUI_ID_TEXT5,   33+80,  33, 120, 24, 0,0},
	{ TEXT_CreateIndirect,    "0.000V",   GUI_ID_TEXT6,   233+80,  5,  120, 24, 0,0},
    { TEXT_CreateIndirect,    "0.000V",   GUI_ID_TEXT7,   233+80,  33, 120, 24, 0,0},
};

/*
*********************************************************************************************************
*	�� �� ��: PaintDialogStatus
*	����˵��: ״̬���ڵĻص������ػ���Ϣ
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PaintDialogStatus(WM_MESSAGE * pMsg)
{
	/* ��һ�������Ʊ���Ϊ��ɫ */
	GUI_SetBkColor(GUI_BLACK);
  	GUI_Clear();
	
	/* �ڶ��������ư�ɫ�߿� */
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRect( 0,      	/* Upper left X-position. */
			      0,      	/* Upper left Y-position. */
	              399,		/* Lower right X-position. */
	              59);   	/* Lower right Y-position. */
    GUI_DrawRect( 1,      	/* Upper left X-position. */
			      1,      	/* Upper left Y-position. */
	              398,		/* Lower right X-position. */
	              58);   	/* Lower right Y-position. */
	
	/* �������������ĸ�Բ�Ǿ��ε� */
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 3, 5, 30, 25, 4);
	
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 3, 33, 30, 53, 4);
	
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 203, 5, 230, 25, 4);
	
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRoundedRect( 203, 33, 230, 53, 4);
	
	/* ���Ĳ������ĸ�Բ�Ǿ��ε�����д���� */
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	
	GUI_DispCharAt('1', 10, 5);
	GUI_DispCharAt('1', 10, 33);
	
	GUI_DispCharAt('1', 210, 5);
	GUI_DispCharAt('1', 210, 33);
}

/*
*********************************************************************************************************
*	�� �� ��: InitDialogStatus()
*	����˵��: ״̬���ڵĻص�������ʼ����Ϣ
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogStatus(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    //
    //GUI_ID_TEXT0
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT3),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT4),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT4),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT5),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT5),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT6),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT6),&GUI_Font24_ASCII);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT7),0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT7),&GUI_Font24_ASCII);
}

/*
*********************************************************************************************************
*	�� �� ��: _cbCallbackStatus
*	����˵��: ״̬���ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbCallbackStatus(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
	char buf[10];
	
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialogStatus(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialogStatus(pMsg);
            break;
		
		case WM_TIMER:
		{
			uint8_t i =0;

			/* ƽ��ֵ */
			if(g_ucMeasureFlag[20] == 1)
			{
				sprintf(buf, "%5.3fV", g_WaveMean);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0 + i),  g_MeasureTable[20]);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT4 + i),  buf);
				i++;
			}

			/* ��Сֵ */
			if(g_ucMeasureFlag[15] == 1)
			{
				sprintf(buf, "%5.3fV", g_WaveMin);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0 + i),  g_MeasureTable[15]);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT4 + i),  buf);
				i++;
			}

			/* ���ֵ */
			if(g_ucMeasureFlag[12] == 1)
			{
				sprintf(buf, "%5.3fV", g_WavePkPk);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0 + i),  g_MeasureTable[12]);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT4 + i),  buf);
				i++;
			}

			/* ���ֵ */
			if(g_ucMeasureFlag[14] == 1)
			{
				sprintf(buf, "%5.3fV", g_WaveMax);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0 + i),  g_MeasureTable[14]);
				TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT4 + i),  buf);
				i++;
			}
			
			WM_RestartTimer(pMsg->Data.v, 500);
		}
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
*	�� �� ��: CreateWindowStatus
*	����˵��: ״̬���ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
WM_HWIN CreateWindowStatus(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateStatus, GUI_COUNTOF(_aDialogCreateStatus), &_cbCallbackStatus, 0, 0, 0);
	return hWin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
