/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�������������
*	�ļ����� : MainTask.c
*	��    �� : V3.0
*	˵    �� : ʾ�������������
*	�޸ļ�¼ :
*		�汾��    ����          ����           ˵��
*		V1.0    2015-01-05     Eric2013        �׷� 
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
#include "includes.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                  ���鶨��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
���������˥��������ADC��DAC�ⲿ�ο���3.3V������û�н���У׼��
(1) ÿ�������Ӧ����50���㣬��ʾ1V ��
(2) 3.3V ��ӦADC��ֵ4095  4095/3.3 = 1240.9;
    ��Ӧ��ϵ���� 50������ == 1V == ADC������1240.9
    �ɴ˿ɵ�1V��˥��������4095/3.3/50 = 24.818���������ϵ���Ƶ���������顣
*********************************************************************************************************
*/
const uint16_t g_AttTable[ATT_COUNT][2] =
{	
/*  {����*�Ŵ�100��, ÿ����ѹ��λmv}  ������Ҳ�ø�����Ӧ������зŴ� ���������������� */
    {24818,	10000}, /* ��248.18 * 100 ==  10000mv ��������Ƶ��Ƕ�Ӧ�� */
	{12409,	5000},  /* ��124.09 * 100 ==  5000mv  */
	{4964,	2000},
	{2482,	1000},
	{1241,	500},
	{496,	200},
	{248,	100},
	{124,	50},
	{50,	20},
	{25,	10},
};

/* 10�ַ��ȵ�λ**************************************************************************************/
const char *g_AttText[] =
{
    "10.0V",
	"5.00V",
	"2.00V",
	"1.00V",
	"500mV",
	"200mV",
	"100mV",
	"50.0mV",
	"20.0mV",
	"10.0mV",
};

/* ÿ��ɼ����������������ڲɼ������ź���ʾʹ��***************************************************************/
/*  
   500sps   1.5KB����  
   200sps   600B����   
   100sps   300B����
   50sps    150B����
   20sps    60B����
   10sps    30B����
   5sps     15B����
   2sps     6B����
   1sps     3B����
*/
const uint16_t g_SlowRefreshWait[] =
{
	1500,
	600,
	300,
	150,
	60,
	30,
	15,
	6,
	3
};

/* ÿ��ɼ����������������ڲɼ������ź�TFT��ʾʹ��***************************************************************/
/* ������ʾ���ӳ�ʱ������ */
const uint16_t g_RefreshDelay[] =
{
	85, /* ����ÿ��5�� */
	85,
	85,
	85,
	85,
	
	85,
	85,
	85,
	85,
	85,
	
	85,
	100,
	200,
	85,
	85,
	
	85,
	85,
	85,
	85,
	85,
	85,
};

/* �������ƶ�ʱ��TIM1�����ںͷ�Ƶ****************************************************************************/
/* 
	��ͨ�������ʣ� 2.8Msps��1sps 
    ��ͨ�������ʣ� 8.4Msps��1sps
*/
const uint32_t g_SampleFreqTable[][2] =
{
	{2800000,  1},     //2.8Msps
	{2000000,  1},     //2Msps
	{1000000,  1},     //1Msps
	{500000,   1},     //500Ksps
	{200000,   1},     //200Ksps
	
	{100000,   1},     //100Ksps
	{50000,    1},     //50Ksps
	{20000,    1},     //20Ksps
	{10000,    1},     //10Ksps
	{5000,     1},     //5Ksps
	
	{16800,      5},    //2Ksps
	{16800,     10},    //1Ksps
	{16800,     20},    //500sps
	{16800,     50},    //200sps
	{16800,     100},   //100sps
	
	{16800,     200},   //50sps
	{16800,     500},   //20sps
	{16800,     1000},  //10sps
	{16800,     2000},  //5sps
	{16800,     5000},  //2sps
	{16800,     10000}, //1sps
};

/* �������ö�ʱ��TIM1�����ںͷ�Ƶ****************************************************************************/
/* ����5.1f��ʽ��ʾ ������222.2us �������� */
const uint16_t g_CursorUintTable[][2] =
{  
	/********5.1F********************************/
	//����*1000   ������ʾʱ�ĸ�ʽ
	{357,   1},  // 600 * 0.36us = 216.0us -- 0.36us.
	{500,   1},  // 600 * 0.5us  = 300.0us -- 0.5us.
	{1000,  1},  // 600 * 1us    = 600.0us -- 1.0us.
	
	/* ���µĳ�����û�зŴ󣬵����Ľ����Ҫ����1000 */
	/********5.3F********************************/
	{2,  2},  // 600 * 2us    = 1.200ms -- 0.002ms. 
	{5,  2},  // 600 * 5us    = 3.000ms -- 0.005ms.
	{10, 2},  // 600 * 10us   = 6.000ms -- 0.010ms.
	
	/********5.2F********************************/
	{20,  3},  // 600 * 20us   = 12.00ms -- 0.02ms.
	{50,  3},  // 600 * 50us   = 30.00ms -- 0.05ms.
	{100, 3},  // 600 * 100us  = 60.00ms -- 0.10ms.
	
	/********5.1F********************************/
	{200,  4},  // 600 * 200us  = 120.0ms -- 0.2ms.
	{500,  4},  // 600 * 500us  = 300.0ms -- 0.5ms.
	{1000, 4},  // 600 * 1000us = 600.0ms -- 1.0ms.
	
	/********5.3F********************************/
	{2,  5},  // 600 * 2ms  = 1.200s -- 0.002s.
	{5,  5},  // 600 * 5ms  = 3.000s -- 0.005s.
	{10, 5},  // 600 * 10ms = 6.000ms -- 0.010ms.
	
	/********5.2F********************************/
	{20,  6},  // 600 * 20ms   = 12.00s -- 0.02s.
	{50,  6},  // 600 * 50ms   = 30.00s -- 0.05s.
	{100, 6},  // 600 * 100ms  = 60.00s -- 0.10s.
	
	/********5.1F********************************/
	{200,  7},  // 600 * 200us  = 120.0s -- 0.2s.
	{500,  7},  // 600 * 500us  = 300.0s -- 0.5s.
	{1000, 7},  // 600 * 1000us = 600.0s -- 1.0s.
};

/* �ֱ��ʣ�����/ǰ�벿�ֱ�ʾ��ADCͨ���ķֱ��ʣ���벿�ֵ�3��ʾҪ����3����ʵ�ʷֱ���******************************/
/* �����ʣ���2Msps��1Ksps */
const char *g_TimeTable[] =
{
	"357ns / 3",
	"500ns / 3",
	"1us / 3",
	"2us / 3",
	"5us / 3",
	"10us / 3",
	
	"20us / 3",
	"50us / 3",
	"100us / 3",
	"200us / 3",
	"500us / 3",
	
	"1ms / 3",
	"2ms / 3",
	"5ms / 3",
	"10ms / 3",
	"20ms / 3",

	"50ms / 3",
	"100ms / 3",
	"200ms / 3",
	"500ms / 3",
	"1s / 3",
};

/* ��ʾ��ǰʵ��֧�ֵĲ���ֵ******************************************************************************** */
const char *g_MeasureTable[] =
{
	"Snapshot",
	"Freq",
	"Period",
	"RiseTime",
	"FallTime",
	"Delay",
	"Phase",
	"PPW",
	"NPW",
	"PDC",
	
	"NDC",
	"BurWidth",
	"Pk-Pk",
	"Amp",
	"Max",
	"Min",
	"High",
	"Low",
	"PosOver",
	"NegOver",
	
	"Mean",
	"CMean",
	"RMS",
	"CRMS",
	"PPC",
	"NPC",
	"REC",
	"FEC",
	"Area",
	"Value",
};

/*
*********************************************************************************************************
*	                                  ����
*********************************************************************************************************
*/
uint8_t  g_ucWaveRefreshFlag = 0;        /* 0����ʾ��ִ�в��������ˢ�£�1����ʾִ�в��������ˢ�� */
uint8_t  g_ucFirFlter_Step100KHz = 0;    /* 0����ʾ��ִ���˲���1����ʾ100KHz��2����ʾ200KHz�����Դ����� */
uint8_t  g_ucFirFlter_Step10KHz = 0;;    /* 0����ʾ��ִ���˲���1����ʾ10KHz�� 2����ʾ20KHz�����Դ����� */
uint8_t  g_ucFirFlter_Step1KHz = 0;;     /* 0����ʾ��ִ���˲���1����ʾ1KHz��  2����ʾ2KHz�����Դ����� */
uint8_t  g_ucFFTDispFlag = 0;  /* FFT������ʾ��0����ʾ��ʾ��1����ʾ����ʾ */
uint8_t  g_ucLineStyle = 0;    /* Ĭ����ʵ�߻��Ʋ��� */
uint8_t  g_ucSlowWaveFlag = 0; /* �Ͳ�����ʱ����������ˢ�±�־ */
uint32_t TempTime;             /* �Ͳ�����ʱ������ˢ����ʼλ�õ��� */
uint16_t TriggerFlag = 0;      /* 0�������Զ�������1,2��������ͨ���� */
WM_HWIN hWinAmp;               /* ������ʾ���ȵĴ��� */
WM_HWIN hWinStatus;            /* ������ʾƵ�ʣ�ƽ��ֵ�����ֵ�����ֵ����Ϣ */
WM_HWIN hWinScale;             /* ������ʾ������ */
uint8_t hWinCoursorFlag = 0;   /* 0:����ʾ�������ڣ�1����ʾ�������� */
uint8_t hWinRunStopFlag = 0;   /* 0:��ʾ���У�1����ʾ��ͣ */
uint8_t hWinButStateFlag = 0;  /* 8�ְ�������״̬ */
int8_t  TimeBaseId = 1;        /* ������ʱ�� */
int8_t  Ch1AmpId = 3;          /* 3:��ʾĬ��ѡ��ÿ�������ʾ1V��ģʽ */
uint16_t  g_usRefPos = 240;    /* ���Ĳο�λ�ã�Ĭ�Ͽ��������м�λ�� */

/* ����ˮƽ�����ʹ�ֱ�����α� */
uint16_t  g_usTriPos = 190;     /* �ϵ��Ĭ�ϵĴ���ֵ��1V */
uint16_t  g_usTriStep = 1;
int16_t   g_usCurTriPos;
int16_t   g_usCurTriStep = 0;
float32_t g_ufTrigValue;
uint16_t  g_usTrigValue = 1241; /* adc����ֵ1241��Ӧ1V */
uint32_t  g_ulTrigTimeLine = 1000;
 
/* ����ˮƽ�����ʹ�ֱ�����α꣬�����ǳ�ʼ����Ĭ��ֵ */
uint16_t g_usCursorStep = 5;
int16_t  g_sCursorVA = 190;   
int16_t  g_sCursorVB = 490;
int16_t  g_sCursorHA = 90;
int16_t  g_sCursorHB = 390;

float32_t   g_WaveCursorA = 0;
float32_t   g_WaveCursorB = 0;

/* �洢ƽ��ֵ�����ֵ��Ƶ�ʣ���Сֵ�����ֵ�ı��� */
float32_t   g_WaveMean = 0;
float32_t   g_WavePkPk = 0;
float32_t   g_WaveFreq = 0;
float32_t   g_WaveMax = 0;
float32_t   g_WaveMin = 0;

/* ͨ����������ADC�����ʽ�����ٲ���ʱ����¼�´�ʱADC��DMA����λ�� */
uint32_t g_ulSlowRefreshWait;

/* һ����30�����õĲ���ѡ� 1����ʾѡ���ˣ�0��ʾû��ѡ�� */
uint8_t g_ucMeasureFlag[30] = {0};

/* ���ڲ���ˢ�µĻ��� */
GUI_POINT aPointsRe[600];  
uint16_t g_usWaveBuf[1024*6];
uint16_t g_usWaveBuf1[1024*6];

/* ������������ */
#define TEST_LENGTH_SAMPLES 2048 
static float32_t testInput_fft_2048[TEST_LENGTH_SAMPLES];
static float32_t testOutput_fft_2048[TEST_LENGTH_SAMPLES];

/* ���ں���GUI_MEMDEV_Draw�ĵ��� */
typedef struct 
{
	uint16_t *aY;
} PARAM;

PARAM Param;

/* ����һ�����η��� */
GUI_RECT Rect ={DSOSCREEN_STARTX, DSOSCREEN_STARTY, DSOSCREEN_ENDX, DSOSCREEN_ENDY};

/*
*********************************************************************************************************
*	                                 ���ڻ��ƶ���ε�����
*********************************************************************************************************
*/

/* ������ʾ�������ϲ�����ʾ�Ĳο�λ�� */
const GUI_POINT aPoints[5] = 
{
	{  0, -10},
	{ 22, -10},
	{ 32,   0},
	{ 22,  10},
	{ 0,   10}
};

/* ������ʾ���Ҳ���ϴ���ֵ��ͷ */
const GUI_POINT aPointsTrig[7] = 
{
	{  0, 0},
	{ 6, -6},
	{ 6, -1},
	{ 12, -1},
	{ 12, 1},
	{ 6,  1},
	{ 6,  6},
};

/* 6k�����У�������ʾ����ʼλ�� */
const GUI_POINT aPointsTrigBrowser[3] = 
{   
    { -6, 0},
	{  6, 0},
	{ 0, 6}
};

/*
*********************************************************************************************************
*	                                 GUI_RECT��������
*********************************************************************************************************
*/
GUI_RECT rClient    = {0,   0,  75, 38};     /* ������ʾʾ������logo */
GUI_RECT rRunMode   = {76,  0, 140, 38};     /* ������ʾ����״̬�����к���ͣ */
GUI_RECT rTrigMode  = {141, 0, 205, 38};     /* ������ʾ����ģʽ���Զ���������ͨ���� */
GUI_RECT rTrigValue = {471, 0, 640, 38};     /* ������ʾ�Զ��������Ĵ������� */
GUI_RECT rButState  = {641, 0, 799, 38};     /* ��ǰ������Ҫ���ڵ�״̬ */
GUI_RECT rRefPos    = {0,  29, 39, 449};     /* ʾ��������ನ�βο���λ������ */
GUI_RECT rTrigPos   = {210, 6, 470, 33};     /* 6KB���ݲ�ѯ���Զ���������ͨ���� */

/*
*********************************************************************************************************
*	�� �� ��: _cbBkWin
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbBkWin(WM_MESSAGE * pMsg) 
{
	WM_HWIN hWin;
	int NCode, Id;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		/* ���汳���ػ� */
		case WM_PAINT:
			DSO_Init(0);
			break;
		
		case WM_TIMER:
			break;
		
		case WM_INIT_DIALOG:
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
				
				 /* ��ť0����Measure�Ի��� */
				 case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateMeasureDlg();
                            break;
                    }
					break;
					
				 /* ��ť1����Inspector�Ի��� */
				 case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateInspectorDlg();
                            break;
                    }
					break;
				
				/* ��ť2����Acquire�Ի��� */
				 case GUI_ID_BUTTON2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateAcquireDlg();
                            break;
                    }
					break;
				
				/* ��ť3����Triger�Ի��� */
				 case GUI_ID_BUTTON3:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateTrigerDlg();
                            break;
                    }
					break;
					
				 /* ��ť4����Math�Ի��� */
				 case GUI_ID_BUTTON4:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateMathDlg();
                            break;
                    }
					break;
				
				/* ��ť5����Settings�Ի��� */
				case GUI_ID_BUTTON5:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateSettingsDlg();
                            break;
                    }	
                    break;
					
				/* ��ť6����DAC�Ի��� */
				case GUI_ID_BUTTON6:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateDacDlg();
                            break;
                    }
					break;
					
				/* ��ť7����Return�Ի��� */
				case GUI_ID_BUTTON7:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateReturnDlg();
                            break;
                    }
					break;
			}
			break;
			
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _Draw
*	����˵��: ���Ʋ��κͲ��εı�����ʾ
*	��    �Σ�p  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _Draw(void * p) 
{
	uint16_t i;
	uint32_t temp;
	
	PARAM * pParam = (PARAM *)p;
	
	/* ��1��������ʾ�������� ***********************************************************************/
	DSO_DrawBakFrame();
	
	/* ��2����ʵ�����ݵ��������Ա���ʾ��LCD��******************************************************/
	for(i = 0; i < DSOSCREEN_LENGTH; i++)
	{
		temp = pParam->aY[i] * 100 / g_AttTable[Ch1AmpId][0];
	    temp = temp + g_usRefPos-40;
		pParam->aY[i] = 400 - temp + (g_usRefPos-240)*2;
		aPointsRe[i].y = pParam->aY[i];
	}
	
	/* ��3������LCD����ʾ����**********************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
#if 0	
	GUI_DrawGraph((I16 * )pParam->aY,   
	              DSOSCREEN_LENGTH,     
				  DSOSCREEN_STARTX, 	 
				  DSOSCREEN_STARTY);    
#endif	
	
	/* ������ʾ��ʽ1����ʵ����ʽ�������� ��Ĭ�ϣ�*/
	if(g_ucLineStyle == 0)
	{
		GUI_SetLineStyle(GUI_LS_SOLID);
	}
	/* ������ʾ��ʽ2����������ʽ�������� */
	else if(g_ucLineStyle == 1)
	{
		GUI_SetLineStyle(GUI_LS_DASH);
	}
	/* ������ʾ��ʽ3����Բ����ʽ�������� */
	else if(g_ucLineStyle == 2)
	{
		GUI_SetLineStyle(GUI_LS_DOT);
	}
	/* ������ʾ��ʽ4�������ߺ�Բ�㽻�����ʽ�������� */
	else if(g_ucLineStyle == 3)
	{
		GUI_SetLineStyle(GUI_LS_DASHDOT);
	}
	/* ������ʾ��ʽ5�������ߺ�˫�㽻�����ʽ�������� */
	else if(g_ucLineStyle == 4)
	{
		GUI_SetLineStyle(GUI_LS_DASHDOTDOT);
	}
	
	/* ��ͨ�������ʴ�2.8Msps��500sps��������ķ�ʽ��ʾ */
	if(TimeBaseId < 13)
	{
		GUI_DrawPolyLine(aPointsRe,              /* ָ��Ҫ��ʾ������ */
		                 GUI_COUNTOF(aPointsRe), /* ���б���ָ���ĵ����� */
		                 DSOSCREEN_STARTX,       /* ԭ���Xλ�� */
		                 DSOSCREEN_STARTY);      /* ԭ���Yλ�� */
	}
	/* �����������ķ�ʽ��ʾ */
	else
	{
		/* С��600��������ʱ��ʾ��ʽ��600������һ���ܹ���ʾ�����ݸ��� */
		if(TempTime < 600)
		{
			GUI_DrawPolyLine(&aPointsRe[600 - TempTime], TempTime, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
		}
		/* ���ڵ���600��������ʱ��ʾ��ʽ */
		else
		{
			GUI_DrawPolyLine(aPointsRe, 600, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
		}
	}
	
	/* ��4������LCD����ʾFFT��Ƶ**********************************************************************/
	/* ���Զ�����ģʽ��������Ϊ2.8Msps��5Kspsʱ(��ͨ��)����ʾFFT���� */
	if((g_ucFFTDispFlag == 0)&&(TriggerFlag == 0)&&(TimeBaseId < 10))
	{
		/* ��FFT�任����ķ�ֵ */
		for(i = 0; i < DSOSCREEN_LENGTH; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/1024; 
		}
		
		/* ֱ��������С */
		testInput_fft_2048[0] = testInput_fft_2048[0]/2;
		
		/* ��÷�Ƶ��һ��ת����������LCD����ʾ */
		for(i = 0; i < DSOSCREEN_LENGTH-2; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/5.0f;
			aPointsRe[i+2].y = 370 - testInput_fft_2048[i] ;
		}
		
		GUI_SetColor(GUI_RED);
		GUI_DrawPolyLine(&aPointsRe[2], 598, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
	}
	
	
	/* ������ʾ���ָ�ʵ����ʾ */
	GUI_SetLineStyle(GUI_LS_SOLID);

	/* ��5������ʾ�����α��λ��*************************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
	/* ��ʾ�����α� */
	GUI_FillPolygon(&aPointsTrig[0],          /* ָ��Ҫ��ʾ�����Ķ���� */
	                GUI_COUNTOF(aPointsTrig), /* ���б���ָ���ĵ����� */
	                628,                      /* ԭ���Xλ�� */
	                g_usTriPos);              /* ԭ���Yλ�� */
	
	/* �������ô���ֵ����ʾ�α�ˮƽ��1sʱ�� */
	if((GUI_GetTime() - g_ulTrigTimeLine) < 1000)
	{
		GUI_DrawHLine(g_usTriPos, 40, 626);
	}
	
	/* ��6������ֱ���������ڲ������ε�ʱ��************************************************************************/
	if(hWinCoursorFlag == 1)
	{
		DSO_DrawCursorH();
	}
	
	/* ��7����ˮƽ���������ڲ�����ֵ��ֵ************************************************************************/
	if(hWinCoursorFlag == 2)
	{
		DSO_DrawCursorV();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _Draw
*	����˵��: ���Ʋ��κͲ��εı�����ʾ
*	��    �Σ�p  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DSO_Graph(void) 
{
	uint16_t  i,j,k;  
	uint32_t t0, t1;
	uint8_t uKeyCode;
	uint16_t usCurPos;
	int16_t temp;
	arm_rfft_fast_instance_f32 S;
	uint32_t fftSize = 2048; 
	uint32_t ifftFlag = 0; 
	
	   
	/* ָ���λ����ַ */
	Param.aY = g_usWaveBuf;
	
	/* ������Ĭ����ʾ���Ƿ��ֵ�����ֵ����Сֵ��ƽ��ֵ */
	g_ucMeasureFlag[12] = 1;
	g_ucMeasureFlag[14] = 1;
	g_ucMeasureFlag[15] = 1;
	g_ucMeasureFlag[20] = 1;
	
	/* ��ʼ��������ʾ�ĺ����� */
	for(i = 0; i < 600; i++)
	{
		aPointsRe[i].x = i;
	}
	
	/* ʵ������FFT���� */
	fftSize = 2048; 
	/* ���任 */
    ifftFlag = 0; 
	
	/* ��ʼ���ṹ��S�еĲ��� */
 	arm_rfft_fast_init_f32(&S, fftSize);
	
	while(1) 
	{
		g_WaveMin = 4095;
		g_WaveMax = 0.0f;
		g_WaveMean = 0.0f;
		
		/* û��ѡ�񴥷�ʱ�Ĳ�����ʾ */
		/*
			1. ÿ������Ҫ��֤�ɼ�6KB�����ݣ�Ҳ���ǵ�ͨ��2Ksps��500us��ʱ��
		       
		       //ÿ��ˢ��ǰ�ɼ���600������
		    2. 2Ksps    6KB����    //100msˢ��һ��
			   1Ksps    3KB����    //200msˢ��һ��
			   
			   //�ƶ�ʽ����ʾ���� 
			   500sps   1.5KB����  
			   200sps   600B����   
			   100sps   300B����
		       50sps    150B����
		       20sps    60B����
		       10sps    30B����
		       5sps     15B����
		       2sps     6B����
		       1sps     3B����
		*/
		if(TriggerFlag == 0)
		{
			/* ��ȡ����ADC3��λ�� */
			usCurPos = 10240 - DMA2_Stream1->NDTR;
			
			/* ǰ10�ֲ����ʲ���ֱ��ˢ�µķ�ʽ������ʾ****************************************/
			if(TimeBaseId < 10)
			{
				/*ADC1��ADC2��ADC3����10k 16bit�����ݻ�������������ÿ�βɼ�ʱ��ȷ����ǰDMA�Ѿ����䵽��λ�ã�
				  Ȼ��ȡ���λ��֮ǰ��2K���ݡ�
				*/
				/* ��һ���������ǰDMA����λ����С��2k�ĵط� */
				if(usCurPos < 2048)
				{
					//j = 2048 - usCurPos;
					//j = 10240 - j;
					j = 8192 + usCurPos;
					
					/* ��ȡ2k���ݵ�ǰ����  */
					for(i = j; i < 10240; i++)
					{
						k = (i-j)*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i];
					}
					
					j = (2048 - usCurPos)*3;
					/* ��ȡ2K���ݵĺ󲿷� */
					for(i = 0; i < usCurPos; i++)
					{
						k = i*3;
						g_usWaveBuf1[j+k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[j+k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[j+k+2] = ADC3ConvertedValue[i];
					}		
				}
				/* �ڶ����������ǰDMA����λ���ڴ��ڵ���2k�ĵط� */
				else
				{
					usCurPos = usCurPos - 2048;
					for(i = 0; i < 2048; i++)
					{
						k = i*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i+usCurPos];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i+usCurPos];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i+usCurPos];
					}	
				}
				
				/* ͨ��������ʵ�������ش��������������600�����ݲ�����⣬����ֱ����ʾ 
				   6144-600 = 5544;
				*/
				j = 0;
				for(i = 0; i < 5544; i++)
				{
					j++;
					if((g_usWaveBuf1[i]>g_usTrigValue)&&(g_usWaveBuf1[i+1]<g_usTrigValue))
					{	
						break;
					}
				}
				
				/* ��¼���������ش�����λ�� */
				g_usCurTriPos = j;
				
				/* ����g_usCurTriStep����ʵ��6K���ݵ��ƶ�����ֹ����С��0 */
				if((g_usCurTriPos + g_usCurTriStep) < 0)
				{
					/* ��g_usCurTriStepȡ��ӽ�g_usCurTriPos + g_usCurTriStep = 0 */
					temp = g_usCurTriPos/100*100;
					g_usCurTriStep = -temp;
				}
				
				/* ���ɼ�6*1024 = 6144�����ݣ��ƶ���������ٻ���ʣ600�����ݣ���6144-600=5544��
				   ��ֹg_usCurTriPos + g_usCurTriStep��λ�ó���5544��
				*/
				if((g_usCurTriPos + g_usCurTriStep) > 5544)
				{
					/* ��g_usCurTriStepȡ��ӽ�g_usCurTriPos + g_usCurTriStep < 5544����ֵ */
					temp = (5544 - g_usCurTriPos)/100*100;
					g_usCurTriStep = temp;
				}
				
				/* ��λ��g_usCurTriPos+g_usCurTriStep��ʼ��600������ȫ����ֵ��g_usWaveBuf */
				for(i = 0; i < DSOSCREEN_LENGTH; i++)
				{
					g_usWaveBuf[i] =g_usWaveBuf1[i+g_usCurTriPos+g_usCurTriStep];
					g_WaveMean += g_usWaveBuf[i];
				}
				
				/* ִ��2048�㸡��FFT����*******************************************************/
				for(i=0; i<2048; i++)
				{
					testInput_fft_2048[i] = g_usWaveBuf1[i];
				}
				
				/* 2048��ʵ���п���FFT, ���� */ 
				arm_rfft_fast_f32(&S, testInput_fft_2048, testOutput_fft_2048, ifftFlag);
				
				/* ��DSOSCREEN_LENGTH = 600��ķ�ֵ */ 
				arm_cmplx_mag_f32(testOutput_fft_2048, testInput_fft_2048, DSOSCREEN_LENGTH);
				
				/* ִ��650�㣬80��Fir�˲���******************************************************/
				/* ��ͨ��2Msps */
				if((TimeBaseId == 1)&&(g_ucFirFlter_Step100KHz !=0 ))
				{
					DSO_FirFilter_Step100KHz();
				}
					
				/* ��ͨ��100Ksps */
				if((TimeBaseId == 5)&&(g_ucFirFlter_Step10KHz !=0 ))
				{
					DSO_FirFilter_Step10KHz();
				}
					
				/* ��ͨ��10Ksps */
				if((TimeBaseId == 8)&&(g_ucFirFlter_Step1KHz !=0 ))
				{
					DSO_FirFilter_Step1KHz();
				}
			}
			/* ǰ10�ֲ�����֮�����ֱ��ˢ�µķ�ʽ������ʾ****************************************/
			else
			{
				/* g_ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��Ҳ���ǹ�����ʾ */
			    if(g_ucSlowWaveFlag == 0)
				{
					/* TempTime��ʾDMA��ǰ���䵽��λ�� */
					TempTime = 10240 - DMA2_Stream1->NDTR;
				
					/* g_ulSlowRefreshWait��ʾ��������Ƶ�ʺ�DMA���䵽��λ�� 
					   ͨ��TempTime��g_ulSlowRefreshWait�Ĳ�ֵȷ���տ�ʼ��ʾ
					   ����ʱ600�����ݵ���ʼλ�á�(�տ�ʼ��ʾ������һ������������ƶ�����)
					*/
					if(TempTime >= g_ulSlowRefreshWait)
					{
						TempTime = TempTime - g_ulSlowRefreshWait;
					}
					else
					{
						TempTime = 10240 + TempTime - g_ulSlowRefreshWait;
					}
					
					/* ����ֵ����600��ʱ��ֹͣ�ƶ����̣���ʼ����ˢ�� */
					if(TempTime >= 600)
					{
						g_ucSlowWaveFlag = 1;
					}
				}
				
				/* ��һ���������ǰDMA����λ����С��200�ĵط� */
				if(usCurPos < 200)
				{
					//j = 200 - usCurPos;
					//j = 10240 - j;
					j = 10040 + usCurPos;
					
					/* ��ȡ200�����ݵ�ǰ����  */
					for(i = j; i < 10240; i++)
					{
						k = (i-j)*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i];
					}
					
					j = (200 - usCurPos)*3;
					/* ��ȡ200�����ݵĺ󲿷�  */
					for(i = 0; i < usCurPos; i++)
					{
						k = i*3;
						g_usWaveBuf1[j+k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[j+k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[j+k+2] = ADC3ConvertedValue[i];
					}
					
				}
				/* �ڶ����������ǰDMA����λ���ڴ��ڵ���200�ĵط� */
				else
				{
					usCurPos = usCurPos - 200;
					for(i = 0; i < 200; i++)
					{
						k = i*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i+usCurPos];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i+usCurPos];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i+usCurPos];
					}	
				}

				/* ����ȡ�����ݸ�ֵ��g_usWaveBuf */
				for(i = 0; i < DSOSCREEN_LENGTH; i++)
				{
					g_usWaveBuf[i] =g_usWaveBuf1[i];
					g_WaveMean += g_usWaveBuf[i];
				}
			
			}
			
		}
		/* ѡ�� ��ͨ���� ģʽ��Ĳ�����ʾ 
		   ��ͨ����ģʽһ��������ǲ�ˢ����ʾ�ģ�ֻ�б�����������ڲ��������ˢ�¡�
		*/
		else
		{
			/* �˱�־������ʾ���������ͣ������ʾ��1��ʾ��ͣ��0��ʾ���� */
			hWinRunStopFlag = 1;
			
			/* TriggerFlag = 2 ��ʾ�ﵽ�����������Ѿ��ɼ��괥��ֵǰ���1K���ݣ���ͨ���� */
			if(TriggerFlag == 2)
			{
				/* ��������ʾ */
				hWinRunStopFlag = 0;
				
				/* �ɼ���ʱ���ǲɼ��˵�ͨ��ǰ���1024�����ݣ���ͨ���ܹ�����1024*6=6144
                   6144/2 = 3072 ��3072�����Ǵ���ֵ��������ȡ��3072��ǰ300���ͺ�300������
				   �Ͻ�����ʾ��ͨ������g_usCurTriStep��ϰ�������ʵ�ֲ��ε��ƶ��۲졣
				*/
				for(i = 2772; i < 3372; i++)
				{
					g_usWaveBuf[i-2772] = g_usWaveBuf1[i+g_usCurTriStep] ;
				}
				
				/* TriggerFlag = 1 ��ʾ�ȴ��´δ��� */
				TriggerFlag = 1;
				
				/* ʹ��ADC3�Ŀ��Ź� */
				ADC_ITConfig(ADC3, ADC_IT_AWD, ENABLE);
			}
		}
		
		/* ��600����ֵ�����ֵ����Сֵ */
		for (i = 0; i < DSOSCREEN_LENGTH; i++) 
		{
			if(g_usWaveBuf[i] < g_WaveMin)
			{
				g_WaveMin = g_usWaveBuf[i];
			}
			
			if(g_usWaveBuf[i] > g_WaveMax)
			{
				g_WaveMax = g_usWaveBuf[i];
			}
			
		}
			
		/* ��ƽ��ֵ */
		g_WaveMean = g_WaveMean / 1000 *3.3f / 4095;
		
		/* �����ֵ */
		g_WaveMax =  g_WaveMax * 3.3f / 4095;
		
		/* ����Сֵ */
		g_WaveMin = g_WaveMin *3.3f / 4095;
		
		/* ����ֵ */
		g_WavePkPk = g_WaveMax - g_WaveMin;
		
				
		/* ��¼ˢ��ǰ��ʱ�� */
		t0 = GUI_GetTime();
		
		/* hWinRunStopFlag = 0 ��ʾ����̬ 
		   g_ucWaveRefreshFlag = 1Ҳ��ʾ����̬������ʹ��K2������������ͣ���ε�ˢ�¡�
		*/
		if((hWinRunStopFlag == 0)||(g_ucWaveRefreshFlag == 1))
		{
			/* ���÷�Ƭ�洢�ռ���� */
			/* ���ο򣬻ص��������ص������Ĳ��� ��0��洢�豸�ķֶ�����, O����GUI_MEMDEV_HASTRANS */
			GUI_MEMDEV_Draw(&Rect, _Draw, &Param, 0, GUI_MEMDEV_NOTRANS);
			g_ucWaveRefreshFlag = 0;  /* 0����ʾ��ִ�в��������ˢ�£�1����ʾִ�в��������ˢ�� */
		}

		/* ���ֵ�����ݲ�ͬ��ִ��ʱ��ִ�в�ͬ���ӳ�ʱ�� */
		t1 = GUI_GetTime() - t0;
		printf("%d\r\n",  t1);
		if (t1 < g_RefreshDelay[TimeBaseId]) 
		{  
			GUI_Delay(g_RefreshDelay[TimeBaseId] - t1);
		}
		/* ��ֹ�涨��ʱ����û����ɲ��ε�ˢ�£�����ʱ������GUI��Ϣ����Ҫͨ�����溯�����д���
	       Ҫ�������GUI��Ϣ�ò�������������
		   
		   ˢ�¶�ʱ����Ϣ�Ļ�����WM_Exec�ǲ��еġ�
		*/
		else
		{
			GUI_Delay(1);
			//GUI_Exec();
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		
		/* ������Ϣ���� */
		uKeyCode = bsp_GetKey();
		
		/* �ж��Ƿ��а������� */
		if(uKeyCode != KEY_NONE)
		{

			switch (uKeyCode)
			{
			    /* ����K1 ����ʵ����Ļ��ͼ���� ********************************************************/	
				case KEY_1_DOWN:		
				    /* ����������Ĵ��� */
					WM_HideWindow(hWinAmp);
					WM_HideWindow(hWinStatus);
					WM_HideWindow(hWinScale);
					WM_HideWindow(hButton0);
					WM_HideWindow(hButton1);
					WM_HideWindow(hButton2);
					WM_HideWindow(hButton3);
					WM_HideWindow(hButton4);
					WM_HideWindow(hButton5);
					WM_HideWindow(hButton6);
					WM_HideWindow(hButton7);
				
					GUI_SetBkColor(GUI_BLUE); 
					GUI_Clear(); 
					WM_Exec();

					/* ִ�д���У׼ */
					TOUCH_Calibration();
				
					 /* ��ʾ������Ĵ��� */
		 			WM_ShowWindow(hWinAmp);
					WM_ShowWindow(hWinStatus);
					WM_ShowWindow(hWinScale);
					WM_ShowWindow(hButton0);
					WM_ShowWindow(hButton1);
					WM_ShowWindow(hButton2);
					WM_ShowWindow(hButton3);
				    WM_ShowWindow(hButton4);
					WM_ShowWindow(hButton5);
					WM_ShowWindow(hButton6);
					WM_ShowWindow(hButton7);
					DSO_Init(0);
					WM_Exec();
					
					/************���ڰ��������ã�����ͼ����ȡ��***********/
 					//BSP_OS_SemPost(&SEM_SYNCH);  
					break;
				
				/* ����K2 ����ʵ�ֲ��ε���ͣ������ **************************************************/
				case KEY_2_DOWN:		
					hWinRunStopFlag ^= 1;
					WM_InvalidateArea(&rRunMode);
					break;
				
				/* ����K3 �����л��Զ���������ͨ���� **********************************************/
                case KEY_3_DOWN:		
					/* TriggerFlag = 0 ��ʾ��ǰ�õ����Զ�����ģʽ */
					if(TriggerFlag == 0)
					{
						/* ���Զ������л�����ͨ���� */
						TriggerFlag = 1;
						
						g_usCurTriStep = 0;
						/* ���´���λ�� */
						WM_InvalidateArea(&rTrigPos);
						
						/*  ���봥������������ADC����ֵΪ1V 
						    �л����������ò�����λ2Msps 
						*/
						ADC_DMA_Close();
						ADC_DMA_Open();
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						/* ���봥�������Ĭ�����ô�����1V��Ҳ����1241 */
						ADC_AnalogWatchdogThresholdsConfig(ADC3, 1241, 0);
					}
					else
					{
						/* TriggerFlag = 0 ��ʾ��ǰ�õ����Զ�����ģʽ */
						TriggerFlag = 0;
						/* ������ʾ */
						hWinRunStopFlag = 0;
						
						/* ���غ󣬲�����Ҫ���Ź����ô��� */
						ADC_AnalogWatchdogThresholdsConfig(ADC3, 4095, 0);
					}
					
					/* ���ִ���ģʽ�л��󣬶�����ֵ����ΪĬ�� */
					g_usTriPos = 190;     
					g_usTrigValue = 1241; 
					WM_InvalidateArea(&rTrigValue);
					WM_InvalidateArea(&rTrigMode);
					
					/* ���л�����ͨ����ģʽ��ʾһ�β��� */
					if(TriggerFlag == 1)
					{
						TriggerFlag = 2;
					}
					break;
             
				/* ҡ���ϼ����� *************************************************************/
                case JOY_DOWN_U:	
					/* ���ڷ��ȷŴ� */
					if(hWinButStateFlag == 1) /* ���ȵ��� */
					{
						Ch1AmpId++;
						if(Ch1AmpId == ATT_COUNT)
						{
							Ch1AmpId = ATT_COUNT - 1;
						}
						WM_SendMessageNoPara(hWinAmp, WM_TextUpDate);
					}
					/* ���ڲο���ѹ������ */
					else if(hWinButStateFlag == 2) 
					{
						g_usRefPos -= 10;
						if(g_usRefPos<= 40)
						{
							g_usRefPos = 40;
						}
						WM_InvalidateArea(&rRefPos);
					}
					/* ����ˮƽ����A���� */
					else if(hWinButStateFlag == 5)
					{
						g_sCursorHA -= g_usCursorStep;
						if(g_sCursorHA <= 40)
						{
							g_sCursorHA = 40;
						}
					}
					/* ����ˮƽ����B����  */
					else if(hWinButStateFlag == 6) 
					{
						g_sCursorHB -= g_usCursorStep;
						if(g_sCursorHB <= g_sCursorHA)
						{
							g_sCursorHB = g_sCursorHA;
						}
					}
					/* �����Զ���������ͨ�����Ĵ���ֵ */
					else if(hWinButStateFlag == 7) 
					{
						/* ����������ͷ��λ�ã�����鿴 */
						g_usTriPos -= g_usTriStep;
						if(g_usTriPos <= DSOSCREEN_STARTY)
						{
							g_usTriPos = DSOSCREEN_STARTY;
						}
						
						/* ���ݵ��ڵĴ�����ͷλ�û�ô���ֵ */
						g_ufTrigValue = 240 - g_usTriPos;
						g_ufTrigValue = g_ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_ufTrigValue = g_ufTrigValue * 4095 / 3.3f;
						g_usTrigValue = g_ufTrigValue;
						
						/* ���´���ֵ����ʾ����õ�ǰ��ʱ�䣬�Ӷ�ʵ�ֵ��ڴ���ֵ������һ��ˮƽ��ֱ�ߣ�ʱ��1s*/
						g_ulTrigTimeLine = GUI_GetTime();
						WM_InvalidateArea(&rTrigValue);
						
						/* ���º󣬸���һ�²�����ʾ��������ͨ������ʽ */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* �������ͨ����ģʽ������ADC3�Ŀ��Ź���ֵ */
							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_usTrigValue, 0);
						}
					}
					
					/* ʹ��K2������������ͣ��ͨ������g_ucWaveRefreshFlag��1���в���ˢ�� */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* ҡ���¼����� *************************************************************/
				case JOY_DOWN_D:	
					/* ���ڷ���˥�� */
					if(hWinButStateFlag == 1) 
					{
						Ch1AmpId--;
						if(Ch1AmpId < 0)
						{
							Ch1AmpId = 0;
						}
						WM_SendMessageNoPara(hWinAmp, WM_TextUpDate);
					}
					/* ���ڲο���ѹ������ */
					else if(hWinButStateFlag == 2) /* ���ȵ��� */
					{
						g_usRefPos += 10;
						if(g_usRefPos >= 440)
						{
							g_usRefPos = 440;
						}
						WM_InvalidateArea(&rRefPos);
					}
					/* ����ˮƽ����A���� */
					if(hWinButStateFlag == 5) 
					{
						g_sCursorHA += g_usCursorStep;
						if(g_sCursorHA >= g_sCursorHB)
						{
							g_sCursorHA = g_sCursorHB;
						}
					}
					 /* ����ˮƽ����B����  */
					else if(hWinButStateFlag == 6)
					{
						g_sCursorHB += g_usCursorStep;
						if(g_sCursorHB >= 440)
						{
							g_sCursorHB = 440;
						}
					}
					/* �����Զ���������ͨ�����Ĵ���ֵ */
					else if(hWinButStateFlag == 7) 
					{
						/* ����������ͷ��λ�ã�����鿴 */
						g_usTriPos += g_usTriStep;
						if(g_usTriPos >= DSOSCREEN_ENDY)
						{
							g_usTriPos = DSOSCREEN_ENDY;
						}
						
						/* ���ݵ��ڵĴ�����ͷλ�û�ô���ֵ */
						g_ufTrigValue = 240 - g_usTriPos;
						g_ufTrigValue = g_ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_ufTrigValue = g_ufTrigValue * 4095 / 3.3f;
						g_usTrigValue = g_ufTrigValue;
						
						/* ���´���ֵ����ʾ����õ�ǰ��ʱ�䣬�Ӷ�ʵ�ֵ��ڴ���ֵ������һ��ˮƽ��ֱ�ߣ�ʱ��1s*/
						g_ulTrigTimeLine = GUI_GetTime();
						WM_InvalidateArea(&rTrigValue);
						
						/* ���º󣬸���һ�²�����ʾ��������ͨ������ʽ */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* �������ͨ����ģʽ������ADC3�Ŀ��Ź���ֵ */
							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_usTrigValue, 0);
						}
					}
					
					/* ʹ��K2������������ͣ��ͨ������g_ucWaveRefreshFlag��1���в���ˢ�� */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* ҡ��������� *************************************************************/
				case JOY_DOWN_L:	
					/* ����ADC�Ĳ����� */
					if(hWinButStateFlag == 0) 
					{
						TimeBaseId++;
						if(TimeBaseId == TIME_COUNT)
						{
							TimeBaseId = TIME_COUNT - 1;
						}
						ADC_DMA_Close();
						ADC_DMA_Open();
						
						/* ����ǵ�Ƶ��������ʾ��Ҫ��¼��ǰ��DMA������λ�� */
						g_ulSlowRefreshWait = 10240 - DMA2_Stream1->NDTR;
						TempTime = 0;
						
						/* ���µ�ǰ����ʱ����ʾ */
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						//WM_Exec();
						
						/* g_ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��Ҳ���ǹ�����ʾ
						   �ڵ�Ƶ����ʱ������g_ucSlowWaveFlag = 0 ����Ч������Ϊ�˷���ֱ��
						   ����Ϊ0��
						*/
						g_ucSlowWaveFlag = 0;
					}
					/* ���ڴ�ֱ��A���� */
					else if(hWinButStateFlag == 3) 
					{
						g_sCursorVA -= g_usCursorStep;
						if(g_sCursorVA <= 40)
						{
							g_sCursorVA = 40;
						}
					}
					/* ���ڴ�ֱ��B���� */
					else if(hWinButStateFlag == 4) 
					{
						g_sCursorVB -= g_usCursorStep;
						if(g_sCursorVB <= g_sCursorVA)
						{
							g_sCursorVB = g_sCursorVA;
						}
					}
					/* �Զ�����ģʽ����ͨ����ģʽ6K���ݵ���ʾ��ʼλ���趨 */
					else if(hWinButStateFlag == 7) 
					{
						/* �Զ�����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
						if(TriggerFlag == 0)
						{
							/* ����100������ */
							g_usCurTriStep -= 100;	
						}
						/* ��ͨ����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
						else
						{
							/* ����100������ */
							g_usCurTriStep -= 100;
							/* �����������2700Ӧ��ȡ2772����ÿ�ε�ƫ����100��ֱ��ȡ2700�ȽϷ��� */
							if(g_usCurTriStep <= -2700)
							{
								g_usCurTriStep = -2700;
							}
							
							/* ͨ������Ϊ2�����²�����ʾ */
							TriggerFlag = 2;
						}
					
						/* ���µ�ǰ������λ�õļ�ͷ */
						WM_InvalidateArea(&rTrigPos);
					}
			
					/* ʹ��K2������������ͣ��ͨ������g_ucWaveRefreshFlag��1���в���ˢ�� */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* ҡ���Ҽ����� *************************************************************/
				case JOY_DOWN_R:		
					/* ����ADC�Ĳ����� */
					if(hWinButStateFlag == 0) 
					{
						TimeBaseId--;
						if(TimeBaseId < 0)
						{
							TimeBaseId = 0;
						}
						ADC_DMA_Close();
						ADC_DMA_Open();
						/* ����ǵ�Ƶ��������ʾ��Ҫ��¼��ǰ��DMA������λ�� */
						g_ulSlowRefreshWait = 10240 - DMA2_Stream1->NDTR;
						TempTime = 0;
						
						/* ���µ�ǰ����ʱ����ʾ */
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						//WM_Exec();
						
						/* g_ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��Ҳ���ǹ�����ʾ
						   �ڵ�Ƶ����ʱ������g_ucSlowWaveFlag = 0 ����Ч������Ϊ�˷���ֱ��
						   ����Ϊ0��
						*/
						g_ucSlowWaveFlag = 0;
					}
					/* ���ڴ�ֱ��A���� */
					else if(hWinButStateFlag == 3) 
					{
						g_sCursorVA += g_usCursorStep;
						if(g_sCursorVA >= g_sCursorVB)
						{
							g_sCursorVA = g_sCursorVB;
						}
					}
					/* ���ڴ�ֱ��B���� */
					else if(hWinButStateFlag == 4)
					{
						g_sCursorVB += g_usCursorStep;
						if(g_sCursorVB >= 640)
						{
							g_sCursorVB = 640;
						}
					}
					/* �Զ�����ģʽ����ͨ����ģʽ6K���ݵ���ʾ��ʼλ���趨 */
					else if(hWinButStateFlag == 7) 
					{
						/* �Զ�����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
						if(TriggerFlag == 0)
						{
							/* ����100������ */
							g_usCurTriStep += 100;						
						}
						/* ��ͨ����ģʽ��6K���ݵ���ʾ��ʼλ���趨 */
						else
						{
							/* ����100������ */
							g_usCurTriStep += 100;
							/* �Զ�����ģʽ֧�ֵ�������ʾ��6K=6144������Ϊ�˷��㣬
							   ����ֻ�ۿ�ǰ���3000�����ݣ���6000�����������������
							   Ҳ��ȡ2772������ƫ����100��ȡ2700�����㡣
							*/
							if(g_usCurTriStep >= 2700) 
							{
								g_usCurTriStep = 2700;
							}
							
							/* ͨ������Ϊ2�����²�����ʾ */
							TriggerFlag = 2;
						}
						
						/* ���µ�ǰ������λ�õļ�ͷ */
						WM_InvalidateArea(&rTrigPos);
					}
					
					/* ʹ��K2������������ͣ��ͨ������g_ucWaveRefreshFlag��1���в���ˢ�� */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;

				/* ҡ��OK������ *************************************************************/
				case JOY_DOWN_OK:
					/* OK������ʵ��8�ֹ��ܵ��л� */
					hWinButStateFlag++;
					if(hWinButStateFlag == 8)
					{
						hWinButStateFlag = 0;
					}
					WM_InvalidateArea(&rButState);
					break;
			}		
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: MainTask
*	����˵��: GUI������
*	��    �Σ���       	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MainTask(void) 
{
	/* �������д���ʹ���ڴ��豸 */
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	
	/* ����Ƥ��ɫ *************************************************************/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	
	/* 
	* �������洰�ڵĻص�����
	*/
	WM_SetCallback(WM_HBKWIN, _cbBkWin);
	
	/* ��ʼ�� DSO */
	DSO_Init(1);
	
	/* ������ʾ�ʹ��� */
	DSO_Graph();																								 
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
