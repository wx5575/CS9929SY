/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI各个部分的总头文件
*	文件名称 : MainTask.c
*	版    本 : V3.0
*	说    明 : GUI界面主函数
*	修改记录 :
*		版本号    日期          作者        说明
*		V1.0    2015-01-05    Eric2013      首发
*	    V2.0    2015-03-27    Eric2013     1. 升级固件库到V1.5.0
*                                          2. 升级BSP板级支持包 
*                                          3. 升级fatfs到0.11
*                                          4. 升级STemWin到5.26，注意这里使用的是非优化版本
*                                             优化版本和非优化版本的区别看这个帖子：
*                                             (实际测试ST公司发布的两种库STemWin526_CM4_OS_Keil_ot.lib和STemWin526_CM4_OS_Keil.lib的区别)
*                                             http://bbs.armfly.com/read.php?tid=7442          
*                                          5. 更改为新的四点触摸校准算法，并重查编排触摸检测任务
*                                          6. 添加7寸800*480分辨率电容屏支持。
*	    V3.0    2015-12-18  Eric2013       1. 升级BSP板级支持包 
*                                          2. 升级STemWin到5.28
*                                          3. 添加4.3寸和5寸电容屏驱动支持。但是此例程当前仅支持800*480分辨率。
*                                          4. 修改了bsp_adc.c文件里的TIM1初始化错误。
*                                          5. 修改了bsp_dac.c文件里的DAC初始化错误。
*	
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MainTask_H
#define __MainTask_H

#include "stdlib.h"
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"
#include "MESSAGEBOX.h"

#include "ff.h"
#include "diskio.h"

#include "arm_math.h"
#include "arm_const_structs.h"

/*
*********************************************************************************************************
*	                                  宏定义
*********************************************************************************************************
*/							   
#define LCD_YSIZE 480
#define LCD_XSIZE 630

/* 定义波形的显示界面区域 600*400 */
#define DSOSCREEN_STARTX     40     /* 波形显示的X起始位置 */
#define DSOSCREEN_STARTY     40	    /* 波形显示的Y起始位置 */
#define DSOSCREEN_ENDX      639     /* 波形显示的X结束位置 */
#define DSOSCREEN_ENDY      439     /* 波形显示的Y结束位置 */
#define DSOSCREEN_LENGTH    600     /* 波形显示的Y结束位置 */

#define ATT_COUNT	        10      /* 定义支持的10种幅值单位 */  
 
#define TIME_COUNT	        21      /* 定义支持的21种采样率 */

#define WM_TextUpDate  WM_USER + 1  /* 自定义一个回调函数的消息，用于界面数据的更新 */

/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
extern const char *g_AttText[];                  /* 10种幅度单位 */
extern const char *g_TimeTable[];                /* 采样率，从2Msps到1Ksps 对应的时基 */
extern const char *g_MeasureTable[];             /* 示波器当前实际支持的测量值 */
extern const uint16_t g_CursorUintTable[][2];    /* 测量游标数据显示格式 */
extern const uint16_t g_AttTable[ATT_COUNT][2];  /* 采样率衰减倍数表 */

extern GUI_RECT rClient;       /* 用于显示示波器的logo */
extern GUI_RECT rRunMode;      /* 用于显示运行状态，运行和暂停 */
extern GUI_RECT rTrigMode;     /* 用于显示触发模式，自动触发和正常触发 */
extern GUI_RECT rTrigValue;    /* 用于显示自动和正常的触发数组 */
extern GUI_RECT rButState;     /* 当前按键需要调节的状态 */
extern GUI_RECT rRefPos;       /* 示波器最左侧波形参考的位置区域 */
extern GUI_RECT rTrigPos;      /* 6KB数据查询  */

extern const GUI_POINT aPointsTrigBrowser[3];  /* 6k数据中，波形显示的起始位置 */
extern const GUI_POINT aPointsTrig[7];         /* 波形显示区右侧边上触发值箭头 */
extern const GUI_POINT aPoints[5];             /* 波形显示区左侧边上波形显示的参考位置 */

extern uint8_t  g_ucLineStyle;  /* 默认是实线绘制波形 */
extern int8_t  Ch1AmpId;        /* 从g_AttText中选择每个方格表示的幅值 */            
extern int8_t  TimeBaseId;      /* 选择相应采样率 */

extern uint8_t g_ucMeasureFlag[30];  /* 示波器支持的30种测量值标志 */

/* 存储平均值，峰峰值，频率，最小值和最大值的变量 */
extern float32_t   g_WaveMean;
extern float32_t   g_WavePkPk;
extern float32_t   g_WaveFreq;
extern float32_t   g_WaveMax;
extern float32_t   g_WaveMin;

/* 用于水平测量和垂直测量游标，下面是初始化的默认值 */
extern uint16_t g_usCursorStep;
extern int16_t  g_sCursorHA;
extern int16_t  g_sCursorHB;
extern int16_t  g_sCursorVA;
extern int16_t  g_sCursorVB;
extern float32_t  g_WaveCursorA;
extern float32_t  g_WaveCursorB;

extern uint8_t hWinRunStopFlag;  /* 0:表示运行，1：表示暂停 */

extern uint16_t TriggerFlag;     /* 0：用于自动触发，1,2：用于普通触发 */

/* 用于水平测量和垂直测量游标 */
extern uint16_t  g_usTriPos;    
extern uint16_t  g_usTriStep;
extern int16_t   g_usCurTriPos;
extern int16_t   g_usCurTriStep;
extern float32_t g_ufTrigValue;
extern uint16_t  g_usTrigValue;  
extern uint32_t  g_ulTrigTimeLine;


extern uint16_t  g_usRefPos;          /* 左侧的参考位置，默认开机后是中间位置 */
extern uint8_t hWinButStateFlag;      /* 8种按键功能状态 */
extern uint16_t g_usWaveBuf[1024*6];  /* 示波器缓存 */
extern uint16_t g_usWaveBuf1[1024*6];

extern uint8_t hWinCoursorFlag;     /* 0:不显示测量窗口，1：显示测量窗口 */
extern uint8_t  g_ucFFTDispFlag;    /* FFT波形显示，0：表示显示，1：表示不显示 */
extern uint8_t  g_ucFirFlter_Step100KHz;   /* 0：表示不执行滤波，1：表示100KHz，2：表示200KHz，，以此类推 */
extern uint8_t  g_ucFirFlter_Step10KHz;    /* 0：表示不执行滤波，1：表示10KHz， 2：表示20KHz，，以此类推 */
extern uint8_t  g_ucFirFlter_Step1KHz;     /* 0：表示不执行滤波，1：表示1KHz，  2：表示2KHz，，以此类推 */
extern uint8_t  g_ucWaveRefreshFlag;       /* 0：表示不执行波形区域的刷新，1：表示执行波形区域的刷新 */
/*
*********************************************************************************************************
*	                                  fatfs
*********************************************************************************************************
*/
/* 供外部文件调用的fatfs变量 */
extern FRESULT result;
extern FIL file;
extern FILINFO finfo;
extern DIR DirInf;
extern UINT bw;
extern FATFS fs;
extern FATFS fs_nand;
extern FATFS fs_usb;
extern char *_acBuffer2;

/* BMP图片生成 */
extern void _WriteByte2File(U8 Data, void * p);

/* 用于BMP图片的显示 */
uint8_t  _ShowBMP(const char * sFilename); 

/*
*********************************************************************************************************
*	                                  窗口句柄
*********************************************************************************************************
*/
extern WM_HWIN hWinAmp;         /* 用于显示幅度的窗口 */
extern WM_HWIN hWinStatus;      /* 用于显示频率，平均值，峰峰值，最大值等信息 */
extern WM_HWIN hWinScale;       /* 用于显示采样率 */

/* 8个按钮句柄 */
extern BUTTON_Handle hButton0;
extern BUTTON_Handle hButton1;
extern BUTTON_Handle hButton2;
extern BUTTON_Handle hButton3;
extern BUTTON_Handle hButton4;
extern BUTTON_Handle hButton5;
extern BUTTON_Handle hButton6;
extern BUTTON_Handle hButton7;

/*
*********************************************************************************************************
*	                                  供外部文件调用的函数
*********************************************************************************************************
*/
/* 创建幅度，时基和状态窗口 */
extern WM_HWIN CreateWindowAmplitude(void); 
extern WM_HWIN CreateWindowScale(void);
extern WM_HWIN CreateWindowStatus(void);

/* 示波器界面初始化 */
extern void DSO_Init(uint8_t ucCreateFlag);

/* 按钮创建的对话框 */
extern WM_HWIN DSO_CreateMeasureDlg(void);
extern WM_HWIN DSO_CreateInspectorDlg(void);
extern WM_HWIN DSO_CreateAcquireDlg(void);
extern WM_HWIN DSO_CreateTrigerDlg(void);
extern WM_HWIN DSO_CreateMathDlg(void);
extern WM_HWIN DSO_CreateSettingsDlg(void);
extern WM_HWIN DSO_CreateDacDlg(void);
extern WM_HWIN DSO_CreateReturnDlg(void);

/* 示波器界面绘制 */
extern void DSO_DrawBakFrame(void);
extern void DSO_DrawCursorH(void);
extern void DSO_DrawCursorV(void);

/* Fir 滤波 */
extern void DSO_FirFilter_Step100KHz(void);
extern void DSO_FirFilter_Step10KHz(void);
extern void DSO_FirFilter_Step1KHz(void);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
