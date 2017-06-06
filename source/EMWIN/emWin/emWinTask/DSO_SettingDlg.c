/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�������öԻ���
*	�ļ����� : DSO_SettingDlg.c
*	��    �� : V1.0
*	˵    �� : ��Ҫ��������5������
*              1. �����Ƿ����ط�ֵ���ڣ�״̬���ں�ʱ�����ڡ�
*              2. �����Ƿ�����FFT����ķ�Ƶ��Ӧ���Ρ�
*              3. ����ˮƽ�ʹ�ֱ�����α����ʾ��
*              4. ����ˮƽ�ʹ�ֱ�����α���ƶ�������
*              5. ����5�ֲ�����ʾ��
*              �Ի���ʹ��GUIBulder5.22���ɡ�
*	�޸ļ�¼ :
*		�汾��    ����          ����         ˵��
*		V1.0    2015-01-05    Eric2013       �׷�
*	
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                  �Զ���ؼ�ID
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
*	                                  ����
*********************************************************************************************************
*/
static uint32_t CheckBoxHideFFT = 0;     /* ��һ�δ�ʱĬ��δ����FFT������ʾ */
static uint32_t CheckBoxHideWindow = 0;  /* ��һ�δ�ʱĬ��δ���ش��� */
static uint32_t RadioCursorDisp = 0;     /* ��һ�δ�ʱĬ�ϲ���ʾˮƽ�ʹ�ֱ�����α� */
static uint32_t RadioCursorStep = 3;     /* ��һ�δ�ʱĬ��ѡ��ˮƽ�ʹ�ֱ�α궼���ƶ�10������ */
static uint32_t RadioLineStyle = 0;      /* ��һ�δ�ʱĬ��ѡ������ʵ�߻��� */

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO��������
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
*	�� �� ��: _cbDialog
*	����˵��: ���öԻ���Ļص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) 
	{
		/* �Ի���رպ�ʹ��һ�β���ˢ�£���ֹ�ڲ�����ͣ״̬��(ͨ��K2������������ͣ)
		   �رնԻ��򲢷��ص�ʱ�򣬲�������û��ˢ�£�һƬ�հס�
		*/
		case WM_DELETE:
			g_ucWaveRefreshFlag = 1;
			break;
		
		case WM_INIT_DIALOG:
			//
			// ��ʼ�� 'Framewin'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetFont(hItem, GUI_FONT_24B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetText(hItem, "Settings");
			FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
			FRAMEWIN_SetTitleHeight(hItem, 28);

			//
			// ��ʼ��ID_CHECKBOX_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
			CHECKBOX_SetText(hItem, "Hide AmplitudeWindow, StatusWindow, ScaleWindow");
			CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
			CHECKBOX_SetState(hItem, CheckBoxHideWindow); /* ����Ĭ��״̬ */
						
			//
			// ��ʼ��ID_CHECKBOX_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
			CHECKBOX_SetText(hItem, "Hide RFFT2048 Display");
			CHECKBOX_SetFont(hItem, GUI_FONT_16B_ASCII);
			CHECKBOX_SetState(hItem, CheckBoxHideFFT); /* ����Ĭ��״̬ */

			//
			// ��ʼ��ID_RADIO_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
			RADIO_SetText(hItem, "Hide Cursor", 0);
			RADIO_SetText(hItem, "Display HorizontalCursor", 1);
			RADIO_SetText(hItem, "Display VerticalCursor", 2);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioCursorDisp); /* ����Ĭ��״̬ */

			//
			// ��ʼ��ID_RADIO_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
			RADIO_SetText(hItem, "MoveCursorStep=1", 0);
			RADIO_SetText(hItem, "MoveCursorStep=2", 1);
			RADIO_SetText(hItem, "MoveCursorStep=5", 2);
			RADIO_SetText(hItem, "MoveCursorStep=10", 3);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioCursorStep);  /* ����Ĭ��״̬ */
			
			//
			// ��ʼ��ID_RADIO_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_2);
			RADIO_SetText(hItem, "LineStyle-Solid", 0);
			RADIO_SetText(hItem, "LineStyle-Dash", 1);
			RADIO_SetText(hItem, "LineStyle-Dot", 2);
			RADIO_SetText(hItem, "LineStyle-DashDot", 3);
			RADIO_SetText(hItem, "LineStyle-DashDotDot", 4);
			RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
			RADIO_SetValue(hItem, RadioLineStyle); /* ����Ĭ��״̬ */
			break;

		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				/* ��ѡ����Ϣ */
				case ID_CHECKBOX_0: 
					switch(NCode) 
					{
						/* ��ѡ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							break;
						
						/* ��ѡ�����ͷź���Ϣ */
						case WM_NOTIFICATION_RELEASED:
							CheckBoxHideWindow = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0));
							/* ѡ�и�ѡ�������ط�ֵ���ڣ�״̬���ں�ʱ������ */
							if(CheckBoxHideWindow == 1)
							{
								WM_HideWindow(hWinAmp);
								WM_HideWindow(hWinStatus);
								WM_HideWindow(hWinScale);
							}
							/* ��ѡ������ʾ���� */
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
				
				/* ��ѡ����Ϣ */
				case ID_CHECKBOX_1: 
					switch(NCode) 
					{
						/* ��ѡ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							break;
						
						/* ��ѡ�����ͷź���Ϣ */
						case WM_NOTIFICATION_RELEASED:
							CheckBoxHideFFT = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1));
							g_ucFFTDispFlag = CheckBoxHideFFT;
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;

				/* ��ѡ��ť��Ϣ */
				case ID_RADIO_0: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							/* ��ˮƽ�ʹ�ֱ�α��״̬��ֵ��ȫ�ֱ���hWinCoursorFlag */
							RadioCursorDisp = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
							hWinCoursorFlag = RadioCursorDisp;
							break;
					}
					break;
				
				/* ��ѡ��ť״̬ */
				case ID_RADIO_1: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
						/* ˮƽ�ʹ�ֱ�α���ƶ�������ֵ��ȫ�ֱ��� */
						RadioCursorStep = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_1));
						if(RadioCursorStep == 0)
						{
							/* ÿ���ƶ�1�� */
							g_usCursorStep = 1;
						}
						else if(RadioCursorStep == 1)
						{
							/* ÿ���ƶ�2�� */
							g_usCursorStep = 2;			
						}
						else if(RadioCursorStep == 2)
						{
							/* ÿ���ƶ�5�� */
							g_usCursorStep = 5;			
						}
						else if(RadioCursorStep == 3)
						{
							/* ÿ���ƶ�10�� */
							g_usCursorStep = 10;			
						}
						break;
					}
					break;
				
				/* ��ѡ��ť״̬ */
				case ID_RADIO_2:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							/* ������ʾ��ʽ��ֵ��ȫ�ֱ��� */
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
*	�� �� ��: DSO_CreateSettingsDlg
*	����˵��: �������öԻ���
*	��    ��: ��        	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
WM_HWIN DSO_CreateSettingsDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
