/*
*********************************************************************************************************
*	                                  
*	模块名称 : 绘制示波器测量游标
*	文件名称 : DSO_DrawCursorV.c
*	版    本 : V1.0
*	说    明 : 绘制示波器的垂直测量游标，用于测量的时间。
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
*	函 数 名: DSO_DrawCursorV
*	功能说明: 绘制示波器的垂直测量游标，用于测量的时间。
*	形    参：无         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO_DrawCursorV(void) 
{
	char buf[60];
	
	/* 第1步：绘制用于测量的两个游标线********************************************************/
	GUI_SetColor(0x0040f0);
	
	/* 绘制竖线A */
	GUI_DrawVLine(g_sCursorVA, DSOSCREEN_STARTY, DSOSCREEN_ENDY);
	GUI_DrawVLine(g_sCursorVA+1, DSOSCREEN_STARTY, DSOSCREEN_ENDY);  
	GUI_FillRoundedRect(g_sCursorVA+5, 47, g_sCursorVA+16, 62, 3);

	/* 绘制竖线B */
	GUI_DrawVLine(g_sCursorVB, DSOSCREEN_STARTY, DSOSCREEN_ENDY);
	GUI_DrawVLine(g_sCursorVB-1, DSOSCREEN_STARTY, DSOSCREEN_ENDY); 
	GUI_FillRoundedRect(g_sCursorVB-16, 47, g_sCursorVB-5, 62, 3);
	
	/* 在竖线A和竖线B旁边的小圆圈中显示字母a和字母b */
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('a', g_sCursorVA+6, 44);
	GUI_DispCharAt('b', g_sCursorVB-15, 45);

	/* 第2步：绘制游标数据显示窗口********************************************************/
	GUI_SetColor(GUI_BLACK);
	GUI_FillRoundedRect(500, 55, 635, 135, 4);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRoundedRect( 499, 54, 636, 136, 4);
					 
	GUI_SetColor(0x0040f0);
	GUI_FillRoundedRect(75 + 433 , 47 + 15, 86 + 433, 62 + 15, 3);				 
	GUI_FillRoundedRect(75 + 433 , 45 + 45, 86 + 433, 60 + 45, 3);
					 
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('a', 509, 59);
	GUI_DispCharAt('b', 509, 88);

	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font16_1);
	GUI_DispCharAt('#', 513, 115);

	/* 第3步：计算竖线A的数值***********************************************************/
	g_WaveCursorA = (float)((g_sCursorVA - 340) * g_CursorUintTable[TimeBaseId][0])/1000;
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs", g_WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs", g_WaveCursorA);
	}

	/* 显示竖线A数值 */
	GUI_DispStringAt(buf, 523, 63);

	/* 第4步：计算竖线B的数值***********************************************************/
	g_WaveCursorB = (float)((g_sCursorVB - 340) * g_CursorUintTable[TimeBaseId][0])/1000;
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs", g_WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs", g_WaveCursorB);
	}
	
	/* 显示竖线B数值 */
	GUI_DispStringAt(buf, 523, 91);

	/* 第5步：计算竖线A竖线B的差值***********************************************************/
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs", (g_WaveCursorB - g_WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs",(g_WaveCursorB - g_WaveCursorA));
	}
	
	/* 显示竖线A和竖线B的差值 */
	GUI_DispStringAt(buf, 523, 115);

	/* 第6步：显示水平线的计算数值***********************************************************/
	GUI_DispCharAt('#', 509+72, 115);

	g_WaveCursorA = (float)((240 - g_sCursorHA) * g_AttTable[Ch1AmpId][1]) / 50000;
	sprintf(buf, "%5.3fV", g_WaveCursorA);
	GUI_DispStringAt(buf, 518+72, 63);

	g_WaveCursorB = (float)((240 - g_sCursorHB) * g_AttTable[Ch1AmpId][1]) / 50000;
	sprintf(buf, "%5.3fV", g_WaveCursorB);	
	GUI_DispStringAt(buf, 518+70, 91);

	sprintf(buf, "%5.3fV", g_WaveCursorA - g_WaveCursorB);
	GUI_DispStringAt(buf, 520+70, 115);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
