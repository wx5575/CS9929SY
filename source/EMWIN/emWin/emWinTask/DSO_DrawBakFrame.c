/*
*********************************************************************************************************
*	                                  
*	模块名称 : 绘制示波器的方格
*	文件名称 : DSO_DrawBakFrame.c
*	版    本 : V1.0
*	说    明 : 绘制示波器的方格
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
*	函 数 名: CreateWindowAmplitude
*	功能说明: 创建幅值窗口
*	形    参：无         	
*	返 回 值: 所创建窗口的句柄
*********************************************************************************************************
*/
void DSO_DrawBakFrame(void) 
{
	uint16_t x;
	uint16_t y;
	
	/* 第一步：绘制虚线背景方格*******************************************************************************/
	/* 填充背景 */
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(DSOSCREEN_STARTX,  /* Upper left X-position. */
			      DSOSCREEN_STARTY,  /* Upper left Y-position. */
	              DSOSCREEN_ENDX,    /* Lower right X-position. */
	              DSOSCREEN_ENDY);   /* Lower right Y-position. */
	
	GUI_SetColor(GUI_WHITE);
	
	/* 绘制水平刻度点 */
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
	
	
	/* 绘制垂直刻度点 */
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
	
	/* 绘制最后脚上的那个点 */
	GUI_DrawPixel(DSOSCREEN_ENDX - 1, DSOSCREEN_ENDY - 1);
	
	/* 绘制垂直中心刻度点 */
	for (y = 0; y < 41; y++)							
	{	 
		GUI_DrawPixel(DSOSCREEN_STARTX - 1 + (300), DSOSCREEN_STARTY + (y * 10));
		GUI_DrawPixel(DSOSCREEN_STARTX + 1 + (300), DSOSCREEN_STARTY + (y * 10));
	}
	
	GUI_DrawPixel(DSOSCREEN_STARTX - 1 + (300), DSOSCREEN_ENDY);
	GUI_DrawPixel(DSOSCREEN_STARTX + 1 + (300), DSOSCREEN_ENDY);
	
	
	/* 绘制水平中心刻度点 */
	for (x = 0; x < 61; x++)								
	{	 
		GUI_DrawPixel(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_STARTY - 1 + (200));
 		GUI_DrawPixel(DSOSCREEN_STARTX + (x * 10), DSOSCREEN_STARTY + 1 + (200));
	}
	 
 	GUI_DrawPixel(DSOSCREEN_ENDX, DSOSCREEN_STARTY - 1 + (200));
  	GUI_DrawPixel(DSOSCREEN_ENDX, DSOSCREEN_STARTY + 1 + (200));
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
