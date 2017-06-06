/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ����������ĳ�ʼ��
*	�ļ����� : DSO_Init.c
*	��    �� : V1.0
*	˵    �� : ʾ������ʼ��
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


/* 8����ť */
BUTTON_Handle hButton0;
BUTTON_Handle hButton1;
BUTTON_Handle hButton2;
BUTTON_Handle hButton3;
BUTTON_Handle hButton4;
BUTTON_Handle hButton5;
BUTTON_Handle hButton6;
BUTTON_Handle hButton7;

/*
*********************************************************************************************************
*	�� �� ��: DSO_Init
*	����˵��: ʾ�����������ʼ��
*	��    �Σ�ucCreateFlag  1:��ʾ��Ҫ���������ʹ��ڵȡ�
*                           0:����Ҫ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO_Init(uint8_t ucCreateFlag)
{
	char buf[10];
	uint32_t ulTrigPos;
	
	
	/* ��1����ˢ�±���*********************************************************************/
	GUI_SetBkColor(0x905040);
  	GUI_Clear();
	
	/* ��2������ʾ��������Ϣ***************************************************************/
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font8x16x1x2);
	GUI_DispStringInRect("Eric2013", &rClient, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	 /* ����K2 :���ò�����ʾ���л���ͣ */
	if(hWinRunStopFlag == 0)
	{
		GUI_DispStringInRect("Run", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else
	{
		GUI_DispStringInRect("Stop", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	
	/* ����K3 :������ͨ������ʽ���Զ����� */
	if(TriggerFlag == 0)
	{
		GUI_DispStringInRect("Auto", &rTrigMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else
	{
		GUI_DispStringInRect("Trig", &rTrigMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	
	/* ��3������ʾ�Զ������Ĵ�����ѹ**********************************************************/
	g_ufTrigValue = 240 - g_usTriPos;
	g_ufTrigValue = g_ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
	sprintf(buf, "%5.3fV", g_ufTrigValue);
	
	GUI_DispStringInRect(buf, &rTrigValue, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	/* ��ʾ�����ش����ı�־ */
	GUI_DrawHLine(rTrigValue.y1-10, rTrigValue.x0+10, rTrigValue.x0 + 19);
	GUI_DrawLine(rTrigValue.x0 + 19, rTrigValue.y1-10, rTrigValue.x0+30, rTrigValue.y0+8);
	GUI_DrawHLine(rTrigValue.y0+8, rTrigValue.x0+31, rTrigValue.x0 + 41);
	
	/* ��4��������ҡ�˰����ĵ���״̬����������ʾ����******************************************/
	if(hWinButStateFlag == 0)
	{
		GUI_DispStringInRect("ChangeSampleFreq", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 1)
	{
		GUI_DispStringInRect("ChangeAmplitude", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 2)
	{
		GUI_DispStringInRect("ChangeRefPos", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 3)
	{
		GUI_DispStringInRect("ChangeCursorVA", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 4)
	{
		GUI_DispStringInRect("ChangeCursorVB", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 5)
	{
		GUI_DispStringInRect("ChangeCursorHA", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 6)
	{
		GUI_DispStringInRect("ChangeCursorHB", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(hWinButStateFlag == 7)
	{
		GUI_DispStringInRect("ChangeTrigger", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	
    /* ��5����ʵ�ֲ��εķ���***************************************************************/
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(210, 6, 470, 33); 
	GUI_SetColor(GUI_YELLOW);
	GUI_DrawHLine(20,220, 220+239);
	GUI_DrawHLine(21,220, 220+239);
	
	GUI_SetColor(0x0040f0);
	
	/* �Զ�����ģʽ */
	if(TriggerFlag == 0)
	{
		ulTrigPos = (g_usCurTriStep + g_usCurTriPos) * 240;	
	}
	/* ��ͨ����ģʽ */
	else
	{
		ulTrigPos = (2772 + g_usCurTriStep)*240;
	}
	
	/* ��������������ݵĴ���λ�����������ϵĴ���ͼ��λ�� */
	ulTrigPos = ulTrigPos / 6144;	
	GUI_FillPolygon(&aPointsTrigBrowser[0], GUI_COUNTOF(aPointsTrigBrowser), ulTrigPos+220, 13);
	
	/* ��¼ר�ŵĴ���λ�� */
	GUI_SetColor(GUI_RED);
	GUI_DrawPixel(326, 20);
	GUI_DrawPixel(326, 21);
	
	GUI_DrawPixel(327, 20);
	GUI_DrawPixel(327, 21);
	
	GUI_DrawPixel(328, 20);
	GUI_DrawPixel(328, 21);
	
	GUI_DrawPixel(329, 20);
	GUI_DrawPixel(329, 21);
	
	GUI_DrawPixel(330, 20);
	GUI_DrawPixel(330, 21);

	/* ��6����������ʾ���ı߿�*************************************************************/
	GUI_SetColor(0XEBCD9E);
	GUI_DrawRect(DSOSCREEN_STARTX - 1,   /* Upper left X-position. */
			      DSOSCREEN_STARTY - 1,  /* Upper left Y-position. */
	              DSOSCREEN_ENDX + 1,    /* Lower right X-position. */
	              DSOSCREEN_ENDY + 1);   /* Lower right Y-position. */

	GUI_SetColor(0XB37F63);
	GUI_DrawRect(DSOSCREEN_STARTX - 2,   /* Upper left X-position. */
			      DSOSCREEN_STARTY - 2,  /* Upper left Y-position. */
	              DSOSCREEN_ENDX + 2,    /* Lower right X-position. */
	              DSOSCREEN_ENDY + 2);   /* Lower right Y-position. */
				  
	/* ������Ҫ�Ƿ���Ҫ���´��������ʹ��� */
	if(ucCreateFlag == 1)
	{
		/* ��7��������״̬����*************************************************************/
		hWinAmp = CreateWindowAmplitude();
		hWinStatus = CreateWindowStatus();
		hWinScale = CreateWindowScale();
		
		/* 
		 * ������ʱ�����书���Ǿ���ָ�����ں���ָ�����ڷ�����Ϣ��
		 * �ö�ʱ����ָ������������� 
		 */
		WM_CreateTimer(hWinStatus,   /* ������Ϣ�Ĵ��ڵľ�� */
					   0, 	         /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
					   500,          /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
					   0);	         /* ��������ʹ�ã�ӦΪ0 */

		
		/* ��6����������Ҫ�İ�ť*************************************************************/
		hButton0 = BUTTON_Create(670, 40, 100, 45, GUI_ID_BUTTON0, WM_CF_SHOW);
		BUTTON_SetText(hButton0, "Measure");
		BUTTON_SetFont(hButton0, &GUI_Font20B_ASCII);

		hButton1 = BUTTON_Create(670, 90, 100, 45, GUI_ID_BUTTON1, WM_CF_SHOW);
		BUTTON_SetText(hButton1, "Inspector");
		BUTTON_SetFont(hButton1, &GUI_Font20B_ASCII);

		hButton2 = BUTTON_Create(670, 140, 100, 45, GUI_ID_BUTTON2, WM_CF_SHOW);
		BUTTON_SetText(hButton2, "Acquire");
		BUTTON_SetFont(hButton2, &GUI_Font20B_ASCII);
		
		hButton3 = BUTTON_Create(670, 190, 100, 45, GUI_ID_BUTTON3, WM_CF_SHOW);
		BUTTON_SetText(hButton3, "Trigger");
		BUTTON_SetFont(hButton3, &GUI_Font20B_ASCII);

		hButton4 = BUTTON_Create(670, 240, 100, 45, GUI_ID_BUTTON4, WM_CF_SHOW);
		BUTTON_SetText(hButton4, "Math");
		BUTTON_SetFont(hButton4, &GUI_Font20B_ASCII);
		
		hButton5 = BUTTON_Create(670, 290, 100, 45, GUI_ID_BUTTON5, WM_CF_SHOW);
		BUTTON_SetText(hButton5, "Settings");
		BUTTON_SetFont(hButton5, &GUI_Font20B_ASCII);

		hButton6 = BUTTON_Create(670, 340, 100, 45, GUI_ID_BUTTON6, WM_CF_SHOW);
		BUTTON_SetText(hButton6, "DAC");
		BUTTON_SetFont(hButton6, &GUI_Font20B_ASCII);
		
		hButton7 = BUTTON_Create(670, 390, 100, 45, GUI_ID_BUTTON7, WM_CF_SHOW);
		BUTTON_SetText(hButton7, "Return");
		BUTTON_SetFont(hButton7, &GUI_Font20B_ASCII);
	}
	
	/* ��8������ʾ�ο�����*************************************************************/
	GUI_SetColor(GUI_YELLOW);
	GUI_FillPolygon(&aPoints[0], GUI_COUNTOF(aPoints), 5, g_usRefPos);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 10, g_usRefPos - 10);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
