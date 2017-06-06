/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ����Math�Ի���
*	�ļ����� : DSO_MathDlg.c
*	��    �� : V1.0
*	˵    �� : �˶Ի�����Ҫ����80��Fir��ͨ�˲����Ľ�ֹƵ��ѡ��
*              1. Fs(������) = 6Mspsʱ����Scale������ʾ500ns/3ʱ��Fc(��ֹƵ��)��ѡ��
*              2. Fs(������) = 300Kspsʱ����Scale������ʾ10us/3ʱ��Fc(��ֹƵ��)��ѡ��
*              3. Fs(������) = 30Kspsʱ����Scale������ʾ100us/3ʱ��Fc(��ֹƵ��)��ѡ��
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
#define ID_RADIO_0     (GUI_ID_USER + 0x01)
#define ID_TEXT_0      (GUI_ID_USER + 0x02)
#define ID_TEXT_1      (GUI_ID_USER + 0x03)
#define ID_TEXT_2      (GUI_ID_USER + 0x04)
#define ID_TEXT_3      (GUI_ID_USER + 0x05)
#define ID_RADIO_1     (GUI_ID_USER + 0x06)
#define ID_RADIO_2     (GUI_ID_USER + 0x07)

/*
*********************************************************************************************************
*	                                  ����
*********************************************************************************************************
*/
static uint32_t Radio_Step100KHz = 0;   /* ��һ�δ�ʱĬ�ϲ������˲� */
static uint32_t Radio_Step10KHz = 0;    /* ��һ�δ�ʱĬ�ϲ������˲�  */
static uint32_t Radio_Step1KHz = 0;     /* ��һ�δ�ʱĬ�ϲ������˲�  */

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { FRAMEWIN_CreateIndirect, "Math", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 18, 66, 102, 218, 0, 0x140b, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 310, 7, 192, 24, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 23, 38, 160, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 287, 36, 159, 17, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_3, 567, 36, 168, 17, 0, 0x64, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_1, 283, 73, 87, 218, 0, 0x140b, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_2, 563, 69, 114, 219, 0, 0x140b, 0 },

};

/*
*********************************************************************************************************
*	�� �� ��: _cbDialog
*	����˵��: �����Ի���Ļص�����
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
		FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
		FRAMEWIN_SetTitleHeight(hItem, 28);

		//
		// ��ʼ�� 'Text'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
		TEXT_SetText(hItem, "80 Order Fir Lowpass");
		
		//
		// ��ʼ�� 'Text'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetText(hItem, "Fs=6Msps,Scale=500ns/3");
		TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
		
		//
		// ��ʼ�� 'Text'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetText(hItem, "Fs=300Ksps,Scale=10us/3");
		TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
		
		//
		// ��ʼ�� 'Text'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetText(hItem, "Fs=30Ksps,Scale=100us/3");
		TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
		
		//
		// ��ʼ�� 'Radio'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
		RADIO_SetText(hItem, "Fc=100KHz", 1);
		RADIO_SetText(hItem, "Fc=200KHz", 2);
		RADIO_SetText(hItem, "Fc=300KHz", 3);
		RADIO_SetText(hItem, "Fc=400KHz", 4);
		RADIO_SetText(hItem, "Fc=500KHz", 5);
		RADIO_SetText(hItem, "Fc=600KHz", 6);
		RADIO_SetText(hItem, "Fc=700KHz", 7);
		RADIO_SetText(hItem, "Fc=800KHz", 8);
		RADIO_SetText(hItem, "Fc=900KHz", 9);
		RADIO_SetText(hItem, "Fc=1000KHz", 10);
		RADIO_SetValue(hItem, Radio_Step100KHz); /* ����Ĭ��״̬ */
		
		//
		// ��ʼ�� 'Text'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetText(hItem, "Fc=10KHz", 1);
		RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
		RADIO_SetText(hItem, "Fc=20KHz", 2);
		RADIO_SetText(hItem, "Fc=30KHz", 3);
		RADIO_SetText(hItem, "Fc=40KHz", 4);
		RADIO_SetText(hItem, "Fc=50KHz", 5);
		RADIO_SetText(hItem, "Fc=60KHz", 6);
		RADIO_SetText(hItem, "Fc=70KHz", 7);
		RADIO_SetText(hItem, "Fc=80KHz", 8);
		RADIO_SetText(hItem, "Fc=90KHz", 9);
		RADIO_SetText(hItem, "Fc=100KHz", 10);
		RADIO_SetValue(hItem, Radio_Step10KHz); /* ����Ĭ��״̬ */
		
		//
		// ��ʼ�� 'Radio'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_2);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetText(hItem, "Fc=1KHz", 1);
		RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
		RADIO_SetText(hItem, "Fc=2KHz", 2);
		RADIO_SetText(hItem, "Fc=3KHz", 3);
		RADIO_SetText(hItem, "Fc=4KHz", 4);
		RADIO_SetText(hItem, "Fc=5KHz", 5);
		RADIO_SetText(hItem, "Fc=6KHz", 6);
		RADIO_SetText(hItem, "Fc=7KHz", 7);
		RADIO_SetText(hItem, "Fc=8KHz", 8);
		RADIO_SetText(hItem, "Fc=9KHz", 9);
		RADIO_SetText(hItem, "Fc=10KHz", 10);
		RADIO_SetValue(hItem, Radio_Step1KHz); /* ����Ĭ��״̬ */
		break;
		
	case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) 
		{
			/* ��ѡ��ť��Ϣ */
			case ID_RADIO_0: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(������) = 6Mspsʱ����Scale������ʾ500ns/3ʱ��Fc(��ֹƵ��)��ѡ�� */
						Radio_Step100KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
						g_ucFirFlter_Step100KHz = Radio_Step100KHz;
						break;
				}
				break;
				
				/* ��ѡ��ť��Ϣ */
			case ID_RADIO_1: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(������) = 300Kspsʱ����Scale������ʾ10us/3ʱ��Fc(��ֹƵ��)��ѡ�� */
						Radio_Step10KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_1));
						g_ucFirFlter_Step10KHz = Radio_Step10KHz;
						break;
				}
				break;
				
				/* ��ѡ��ť��Ϣ */
			case ID_RADIO_2: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(������) = 30Kspsʱ����Scale������ʾ100us/3ʱ��Fc(��ֹƵ��)��ѡ�� */
						Radio_Step1KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_2));
						g_ucFirFlter_Step1KHz = Radio_Step1KHz;
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
*	�� �� ��: DSO_CreateMeasureDlg
*	����˵��: �����Ի���
*	��    ��: ��        	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
WM_HWIN DSO_CreateMathDlg(void) 
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
