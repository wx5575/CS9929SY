/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI�������ֵ���ͷ�ļ�
*	�ļ����� : MainTask.c
*	��    �� : V3.0
*	˵    �� : GUI����������
*	�޸ļ�¼ :
*		�汾��    ����          ����        ˵��
*		V1.0    2015-01-05    Eric2013      �׷�
*	    V2.0    2015-03-27    Eric2013     1. �����̼��⵽V1.5.0
*                                          2. ����BSP�弶֧�ְ� 
*                                          3. ����fatfs��0.11
*                                          4. ����STemWin��5.26��ע������ʹ�õ��Ƿ��Ż��汾
*                                             �Ż��汾�ͷ��Ż��汾������������ӣ�
*                                             (ʵ�ʲ���ST��˾���������ֿ�STemWin526_CM4_OS_Keil_ot.lib��STemWin526_CM4_OS_Keil.lib������)
*                                             http://bbs.armfly.com/read.php?tid=7442          
*                                          5. ����Ϊ�µ��ĵ㴥��У׼�㷨�����ز���Ŵ����������
*                                          6. ���7��800*480�ֱ��ʵ�����֧�֡�
*	    V3.0    2015-12-18  Eric2013       1. ����BSP�弶֧�ְ� 
*                                          2. ����STemWin��5.28
*                                          3. ���4.3���5�����������֧�֡����Ǵ����̵�ǰ��֧��800*480�ֱ��ʡ�
*                                          4. �޸���bsp_adc.c�ļ����TIM1��ʼ������
*                                          5. �޸���bsp_dac.c�ļ����DAC��ʼ������
*	
*	Copyright (C), 2015-2020, ���������� www.armfly.com
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
*	                                  �궨��
*********************************************************************************************************
*/							   
#define LCD_YSIZE 480
#define LCD_XSIZE 630

/* ���岨�ε���ʾ�������� 600*400 */
#define DSOSCREEN_STARTX     40     /* ������ʾ��X��ʼλ�� */
#define DSOSCREEN_STARTY     40	    /* ������ʾ��Y��ʼλ�� */
#define DSOSCREEN_ENDX      639     /* ������ʾ��X����λ�� */
#define DSOSCREEN_ENDY      439     /* ������ʾ��Y����λ�� */
#define DSOSCREEN_LENGTH    600     /* ������ʾ��Y����λ�� */

#define ATT_COUNT	        10      /* ����֧�ֵ�10�ַ�ֵ��λ */  
 
#define TIME_COUNT	        21      /* ����֧�ֵ�21�ֲ����� */

#define WM_TextUpDate  WM_USER + 1  /* �Զ���һ���ص���������Ϣ�����ڽ������ݵĸ��� */

/*
*********************************************************************************************************
*	                                  ����
*********************************************************************************************************
*/
extern const char *g_AttText[];                  /* 10�ַ��ȵ�λ */
extern const char *g_TimeTable[];                /* �����ʣ���2Msps��1Ksps ��Ӧ��ʱ�� */
extern const char *g_MeasureTable[];             /* ʾ������ǰʵ��֧�ֵĲ���ֵ */
extern const uint16_t g_CursorUintTable[][2];    /* �����α�������ʾ��ʽ */
extern const uint16_t g_AttTable[ATT_COUNT][2];  /* ������˥�������� */

extern GUI_RECT rClient;       /* ������ʾʾ������logo */
extern GUI_RECT rRunMode;      /* ������ʾ����״̬�����к���ͣ */
extern GUI_RECT rTrigMode;     /* ������ʾ����ģʽ���Զ��������������� */
extern GUI_RECT rTrigValue;    /* ������ʾ�Զ��������Ĵ������� */
extern GUI_RECT rButState;     /* ��ǰ������Ҫ���ڵ�״̬ */
extern GUI_RECT rRefPos;       /* ʾ��������ನ�βο���λ������ */
extern GUI_RECT rTrigPos;      /* 6KB���ݲ�ѯ  */

extern const GUI_POINT aPointsTrigBrowser[3];  /* 6k�����У�������ʾ����ʼλ�� */
extern const GUI_POINT aPointsTrig[7];         /* ������ʾ���Ҳ���ϴ���ֵ��ͷ */
extern const GUI_POINT aPoints[5];             /* ������ʾ�������ϲ�����ʾ�Ĳο�λ�� */

extern uint8_t  g_ucLineStyle;  /* Ĭ����ʵ�߻��Ʋ��� */
extern int8_t  Ch1AmpId;        /* ��g_AttText��ѡ��ÿ�������ʾ�ķ�ֵ */            
extern int8_t  TimeBaseId;      /* ѡ����Ӧ������ */

extern uint8_t g_ucMeasureFlag[30];  /* ʾ����֧�ֵ�30�ֲ���ֵ��־ */

/* �洢ƽ��ֵ�����ֵ��Ƶ�ʣ���Сֵ�����ֵ�ı��� */
extern float32_t   g_WaveMean;
extern float32_t   g_WavePkPk;
extern float32_t   g_WaveFreq;
extern float32_t   g_WaveMax;
extern float32_t   g_WaveMin;

/* ����ˮƽ�����ʹ�ֱ�����α꣬�����ǳ�ʼ����Ĭ��ֵ */
extern uint16_t g_usCursorStep;
extern int16_t  g_sCursorHA;
extern int16_t  g_sCursorHB;
extern int16_t  g_sCursorVA;
extern int16_t  g_sCursorVB;
extern float32_t  g_WaveCursorA;
extern float32_t  g_WaveCursorB;

extern uint8_t hWinRunStopFlag;  /* 0:��ʾ���У�1����ʾ��ͣ */

extern uint16_t TriggerFlag;     /* 0�������Զ�������1,2��������ͨ���� */

/* ����ˮƽ�����ʹ�ֱ�����α� */
extern uint16_t  g_usTriPos;    
extern uint16_t  g_usTriStep;
extern int16_t   g_usCurTriPos;
extern int16_t   g_usCurTriStep;
extern float32_t g_ufTrigValue;
extern uint16_t  g_usTrigValue;  
extern uint32_t  g_ulTrigTimeLine;


extern uint16_t  g_usRefPos;          /* ���Ĳο�λ�ã�Ĭ�Ͽ��������м�λ�� */
extern uint8_t hWinButStateFlag;      /* 8�ְ�������״̬ */
extern uint16_t g_usWaveBuf[1024*6];  /* ʾ�������� */
extern uint16_t g_usWaveBuf1[1024*6];

extern uint8_t hWinCoursorFlag;     /* 0:����ʾ�������ڣ�1����ʾ�������� */
extern uint8_t  g_ucFFTDispFlag;    /* FFT������ʾ��0����ʾ��ʾ��1����ʾ����ʾ */
extern uint8_t  g_ucFirFlter_Step100KHz;   /* 0����ʾ��ִ���˲���1����ʾ100KHz��2����ʾ200KHz�����Դ����� */
extern uint8_t  g_ucFirFlter_Step10KHz;    /* 0����ʾ��ִ���˲���1����ʾ10KHz�� 2����ʾ20KHz�����Դ����� */
extern uint8_t  g_ucFirFlter_Step1KHz;     /* 0����ʾ��ִ���˲���1����ʾ1KHz��  2����ʾ2KHz�����Դ����� */
extern uint8_t  g_ucWaveRefreshFlag;       /* 0����ʾ��ִ�в��������ˢ�£�1����ʾִ�в��������ˢ�� */
/*
*********************************************************************************************************
*	                                  fatfs
*********************************************************************************************************
*/
/* ���ⲿ�ļ����õ�fatfs���� */
extern FRESULT result;
extern FIL file;
extern FILINFO finfo;
extern DIR DirInf;
extern UINT bw;
extern FATFS fs;
extern FATFS fs_nand;
extern FATFS fs_usb;
extern char *_acBuffer2;

/* BMPͼƬ���� */
extern void _WriteByte2File(U8 Data, void * p);

/* ����BMPͼƬ����ʾ */
uint8_t  _ShowBMP(const char * sFilename); 

/*
*********************************************************************************************************
*	                                  ���ھ��
*********************************************************************************************************
*/
extern WM_HWIN hWinAmp;         /* ������ʾ���ȵĴ��� */
extern WM_HWIN hWinStatus;      /* ������ʾƵ�ʣ�ƽ��ֵ�����ֵ�����ֵ����Ϣ */
extern WM_HWIN hWinScale;       /* ������ʾ������ */

/* 8����ť��� */
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
*	                                  ���ⲿ�ļ����õĺ���
*********************************************************************************************************
*/
/* �������ȣ�ʱ����״̬���� */
extern WM_HWIN CreateWindowAmplitude(void); 
extern WM_HWIN CreateWindowScale(void);
extern WM_HWIN CreateWindowStatus(void);

/* ʾ���������ʼ�� */
extern void DSO_Init(uint8_t ucCreateFlag);

/* ��ť�����ĶԻ��� */
extern WM_HWIN DSO_CreateMeasureDlg(void);
extern WM_HWIN DSO_CreateInspectorDlg(void);
extern WM_HWIN DSO_CreateAcquireDlg(void);
extern WM_HWIN DSO_CreateTrigerDlg(void);
extern WM_HWIN DSO_CreateMathDlg(void);
extern WM_HWIN DSO_CreateSettingsDlg(void);
extern WM_HWIN DSO_CreateDacDlg(void);
extern WM_HWIN DSO_CreateReturnDlg(void);

/* ʾ����������� */
extern void DSO_DrawBakFrame(void);
extern void DSO_DrawCursorH(void);
extern void DSO_DrawCursorV(void);

/* Fir �˲� */
extern void DSO_FirFilter_Step100KHz(void);
extern void DSO_FirFilter_Step10KHz(void);
extern void DSO_FirFilter_Step1KHz(void);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
