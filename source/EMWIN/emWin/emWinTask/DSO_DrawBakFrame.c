/*
*********************************************************************************************************
*	                                  
*	ģ������ : ����ʾ�����ķ���
*	�ļ����� : DSO_DrawBakFrame.c
*	��    �� : V1.0
*	˵    �� : ����ʾ�����ķ���
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
*	�� �� ��: CreateWindowAmplitude
*	����˵��: ������ֵ����
*	��    �Σ���         	
*	�� �� ֵ: ���������ڵľ��
*********************************************************************************************************
*/
void DSO_DrawBakFrame(void) 
{
	uint16_t x;
	uint16_t y;
	
	/* ��һ�����������߱�������*******************************************************************************/
	/* ��䱳�� */
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(DSOSCREEN_STARTX,  /* Upper left X-position. */
			      DSOSCREEN_STARTY,  /* Upper left Y-position. */
	              DSOSCREEN_ENDX,    /* Lower right X-position. */
	              DSOSCREEN_ENDY);   /* Lower right Y-position. */
	
	GUI_SetColor(GUI_WHITE);
	
	/* ����ˮƽ�̶ȵ� */
	for (y = 0; y < 9; y++)								
	{
		for (x = 0; x < 61; x++)
		{
			GUI_DrawPoint(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_STARTY + (y * 50));
		}
	}
	
	for (x = 0; x < 61; x++)
	{
		GUI_DrawPoint(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_ENDY);
	}
	
	
	/* ���ƴ�ֱ�̶ȵ� */
	for (x = 0; x < 12; x++)							
	{
		for (y = 0; y < 41; y++)
		{
			GUI_DrawPoint(DSOSCREEN_STARTX + (x * 50), DSOSCREEN_STARTY + (y * 10));
		}
	}
	
	for (y = 0; y < 41; y++)
	{
		GUI_DrawPixel(DSOSCREEN_ENDX, DSOSCREEN_STARTY + (y * 10));
	}
	
	/* ���������ϵ��Ǹ��� */
	GUI_DrawPixel(DSOSCREEN_ENDX - 1, DSOSCREEN_ENDY - 1);
	
	/* ���ƴ�ֱ���Ŀ̶ȵ� */
	for (y = 0; y < 41; y++)							
	{	 
		GUI_DrawPixel(DSOSCREEN_STARTX - 1 + (300), DSOSCREEN_STARTY + (y * 10));
		GUI_DrawPixel(DSOSCREEN_STARTX + 1 + (300), DSOSCREEN_STARTY + (y * 10));
	}
	
	GUI_DrawPixel(DSOSCREEN_STARTX - 1 + (300), DSOSCREEN_ENDY);
	GUI_DrawPixel(DSOSCREEN_STARTX + 1 + (300), DSOSCREEN_ENDY);
	
	
	/* ����ˮƽ���Ŀ̶ȵ� */
	for (x = 0; x < 61; x++)								
	{	 
		GUI_DrawPixel(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_STARTY - 1 + (200));
 		GUI_DrawPixel(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_STARTY + 1 + (200));
	}
	 
 	GUI_DrawPixel(DSOSCREEN_ENDX, DSOSCREEN_STARTY - 1 + (200));
  	GUI_DrawPixel(DSOSCREEN_ENDX, DSOSCREEN_STARTY + 1 + (200));
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
