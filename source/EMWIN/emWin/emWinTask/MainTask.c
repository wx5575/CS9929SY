/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器设计主函数
*	文件名称 : MainTask.c
*	版    本 : V3.0
*	说    明 : 示波器主界面设计
*	修改记录 :
*		版本号    日期          作者           说明
*		V1.0    2015-01-05     Eric2013        首发 
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
#include "includes.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                  数组定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
理想情况下衰减倍数表，ADC和DAC外部参考是3.3V，这里没有进行校准。
(1) 每个方格对应的是50个点，表示1V 。
(2) 3.3V 对应ADC数值4095  4095/3.3 = 1240.9;
    对应关系就是 50个点阵 == 1V == ADC计数的1240.9
    由此可得1V的衰减因子是4095/3.3/50 = 24.818，由这个关系可推得下面的数组。
*********************************************************************************************************
*/
const uint16_t g_AttTable[ATT_COUNT][2] =
{	
/*  {除数*放大100倍, 每大格电压单位mv}  被除数也得更加相应情况进行放大 这里做了四舍五入 */
    {24818,	10000}, /* 即248.18 * 100 ==  10000mv 跟上面的推导是对应的 */
	{12409,	5000},  /* 即124.09 * 100 ==  5000mv  */
	{4964,	2000},
	{2482,	1000},
	{1241,	500},
	{496,	200},
	{248,	100},
	{124,	50},
	{50,	20},
	{25,	10},
};

/* 10种幅度单位**************************************************************************************/
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

/* 每秒采集的数据数量，用于采集低速信号显示使用***************************************************************/
/*  
   500sps   1.5KB数据  
   200sps   600B数据   
   100sps   300B数据
   50sps    150B数据
   20sps    60B数据
   10sps    30B数据
   5sps     15B数据
   2sps     6B数据
   1sps     3B数据
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

/* 每秒采集的数据数量，用于采集低速信号TFT显示使用***************************************************************/
/* 波形显示的延迟时间设置 */
const uint16_t g_RefreshDelay[] =
{
	85, /* 下面每组5个 */
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

/* 用于配制定时器TIM1的周期和分频****************************************************************************/
/* 
	单通道采样率， 2.8Msps到1sps 
    三通道采样率， 8.4Msps到1sps
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

/* 用于配置定时器TIM1的周期和分频****************************************************************************/
/* 采用5.1f格式显示 ，比如222.2us 依次类推 */
const uint16_t g_CursorUintTable[][2] =
{  
	/********5.1F********************************/
	//除数*1000   数据显示时的格式
	{357,   1},  // 600 * 0.36us = 216.0us -- 0.36us.
	{500,   1},  // 600 * 0.5us  = 300.0us -- 0.5us.
	{1000,  1},  // 600 * 1us    = 600.0us -- 1.0us.
	
	/* 以下的除数均没有放大，但最后的结果需要除以1000 */
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

/* 分辨率，符号/前半部分表示单ADC通道的分辨率，后半部分的3表示要除以3才是实际分辨率******************************/
/* 采样率，从2Msps到1Ksps */
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

/* 表示当前实际支持的测量值******************************************************************************** */
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
*	                                  变量
*********************************************************************************************************
*/
uint8_t  g_ucWaveRefreshFlag = 0;        /* 0：表示不执行波形区域的刷新，1：表示执行波形区域的刷新 */
uint8_t  g_ucFirFlter_Step100KHz = 0;    /* 0：表示不执行滤波，1：表示100KHz，2：表示200KHz，，以此类推 */
uint8_t  g_ucFirFlter_Step10KHz = 0;;    /* 0：表示不执行滤波，1：表示10KHz， 2：表示20KHz，，以此类推 */
uint8_t  g_ucFirFlter_Step1KHz = 0;;     /* 0：表示不执行滤波，1：表示1KHz，  2：表示2KHz，，以此类推 */
uint8_t  g_ucFFTDispFlag = 0;  /* FFT波形显示，0：表示显示，1：表示不显示 */
uint8_t  g_ucLineStyle = 0;    /* 默认是实线绘制波形 */
uint8_t  g_ucSlowWaveFlag = 0; /* 低采样率时，波形慢速刷新标志 */
uint32_t TempTime;             /* 低采样率时，波形刷新起始位置调整 */
uint16_t TriggerFlag = 0;      /* 0：用于自动触发，1,2：用于普通触发 */
WM_HWIN hWinAmp;               /* 用于显示幅度的窗口 */
WM_HWIN hWinStatus;            /* 用于显示频率，平均值，峰峰值，最大值等信息 */
WM_HWIN hWinScale;             /* 用于显示采样率 */
uint8_t hWinCoursorFlag = 0;   /* 0:不显示测量窗口，1：显示测量窗口 */
uint8_t hWinRunStopFlag = 0;   /* 0:表示运行，1：表示暂停 */
uint8_t hWinButStateFlag = 0;  /* 8种按键功能状态 */
int8_t  TimeBaseId = 1;        /* 采样率时基 */
int8_t  Ch1AmpId = 3;          /* 3:表示默认选择每个方格表示1V的模式 */
uint16_t  g_usRefPos = 240;    /* 左侧的参考位置，默认开机后是中间位置 */

/* 用于水平测量和垂直测量游标 */
uint16_t  g_usTriPos = 190;     /* 上电后默认的触发值是1V */
uint16_t  g_usTriStep = 1;
int16_t   g_usCurTriPos;
int16_t   g_usCurTriStep = 0;
float32_t g_ufTrigValue;
uint16_t  g_usTrigValue = 1241; /* adc的数值1241对应1V */
uint32_t  g_ulTrigTimeLine = 1000;
 
/* 用于水平测量和垂直测量游标，下面是初始化的默认值 */
uint16_t g_usCursorStep = 5;
int16_t  g_sCursorVA = 190;   
int16_t  g_sCursorVB = 490;
int16_t  g_sCursorHA = 90;
int16_t  g_sCursorHB = 390;

float32_t   g_WaveCursorA = 0;
float32_t   g_WaveCursorB = 0;

/* 存储平均值，峰峰值，频率，最小值和最大值的变量 */
float32_t   g_WaveMean = 0;
float32_t   g_WavePkPk = 0;
float32_t   g_WaveFreq = 0;
float32_t   g_WaveMax = 0;
float32_t   g_WaveMin = 0;

/* 通过按键调节ADC采样率进入低速采样时，记录下此时ADC的DMA传输位置 */
uint32_t g_ulSlowRefreshWait;

/* 一共有30个可用的测量选项， 1：表示选择了，0表示没有选择 */
uint8_t g_ucMeasureFlag[30] = {0};

/* 用于波形刷新的缓存 */
GUI_POINT aPointsRe[600];  
uint16_t g_usWaveBuf[1024*6];
uint16_t g_usWaveBuf1[1024*6];

/* 输入和输出缓冲 */
#define TEST_LENGTH_SAMPLES 2048 
static float32_t testInput_fft_2048[TEST_LENGTH_SAMPLES];
static float32_t testOutput_fft_2048[TEST_LENGTH_SAMPLES];

/* 用于函数GUI_MEMDEV_Draw的调用 */
typedef struct 
{
	uint16_t *aY;
} PARAM;

PARAM Param;

/* 声明一个矩形方框 */
GUI_RECT Rect ={DSOSCREEN_STARTX, DSOSCREEN_STARTY, DSOSCREEN_ENDX, DSOSCREEN_ENDY};

/*
*********************************************************************************************************
*	                                 用于绘制多边形的数组
*********************************************************************************************************
*/

/* 波形显示区左侧边上波形显示的参考位置 */
const GUI_POINT aPoints[5] = 
{
	{  0, -10},
	{ 22, -10},
	{ 32,   0},
	{ 22,  10},
	{ 0,   10}
};

/* 波形显示区右侧边上触发值箭头 */
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

/* 6k数据中，波形显示的起始位置 */
const GUI_POINT aPointsTrigBrowser[3] = 
{   
    { -6, 0},
	{  6, 0},
	{ 0, 6}
};

/*
*********************************************************************************************************
*	                                 GUI_RECT类型数组
*********************************************************************************************************
*/
GUI_RECT rClient    = {0,   0,  75, 38};     /* 用于显示示波器的logo */
GUI_RECT rRunMode   = {76,  0, 140, 38};     /* 用于显示运行状态，运行和暂停 */
GUI_RECT rTrigMode  = {141, 0, 205, 38};     /* 用于显示触发模式，自动触发和普通触发 */
GUI_RECT rTrigValue = {471, 0, 640, 38};     /* 用于显示自动和正常的触发数组 */
GUI_RECT rButState  = {641, 0, 799, 38};     /* 当前按键需要调节的状态 */
GUI_RECT rRefPos    = {0,  29, 39, 449};     /* 示波器最左侧波形参考的位置区域 */
GUI_RECT rTrigPos   = {210, 6, 470, 33};     /* 6KB数据查询：自动触发，普通触发 */

/*
*********************************************************************************************************
*	函 数 名: _cbBkWin
*	功能说明: 桌面窗口的回调函数 
*	形    参：pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBkWin(WM_MESSAGE * pMsg) 
{
	WM_HWIN hWin;
	int NCode, Id;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		/* 桌面背景重绘 */
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
				
				 /* 按钮0：打开Measure对话框 */
				 case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateMeasureDlg();
                            break;
                    }
					break;
					
				 /* 按钮1：打开Inspector对话框 */
				 case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateInspectorDlg();
                            break;
                    }
					break;
				
				/* 按钮2：打开Acquire对话框 */
				 case GUI_ID_BUTTON2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateAcquireDlg();
                            break;
                    }
					break;
				
				/* 按钮3：打开Triger对话框 */
				 case GUI_ID_BUTTON3:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateTrigerDlg();
                            break;
                    }
					break;
					
				 /* 按钮4：打开Math对话框 */
				 case GUI_ID_BUTTON4:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateMathDlg();
                            break;
                    }
					break;
				
				/* 按钮5：打开Settings对话框 */
				case GUI_ID_BUTTON5:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateSettingsDlg();
                            break;
                    }	
                    break;
					
				/* 按钮6：打开DAC对话框 */
				case GUI_ID_BUTTON6:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateDacDlg();
                            break;
                    }
					break;
					
				/* 按钮7：打开Return对话框 */
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
*	函 数 名: _Draw
*	功能说明: 绘制波形和波形的背景显示
*	形    参：p  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _Draw(void * p) 
{
	uint16_t i;
	uint32_t temp;
	
	PARAM * pParam = (PARAM *)p;
	
	/* 第1步：绘制示波器方格 ***********************************************************************/
	DSO_DrawBakFrame();
	
	/* 第2步：实现数据的修正，以便显示到LCD上******************************************************/
	for(i = 0; i < DSOSCREEN_LENGTH; i++)
	{
		temp = pParam->aY[i] * 100 / g_AttTable[Ch1AmpId][0];
	    temp = temp + g_usRefPos-40;
		pParam->aY[i] = 400 - temp + (g_usRefPos-240)*2;
		aPointsRe[i].y = pParam->aY[i];
	}
	
	/* 第3步：在LCD上显示波形**********************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
#if 0	
	GUI_DrawGraph((I16 * )pParam->aY,   
	              DSOSCREEN_LENGTH,     
				  DSOSCREEN_STARTX, 	 
				  DSOSCREEN_STARTY);    
#endif	
	
	/* 波形显示方式1：按实线样式绘制线条 （默认）*/
	if(g_ucLineStyle == 0)
	{
		GUI_SetLineStyle(GUI_LS_SOLID);
	}
	/* 波形显示方式2：按虚线样式绘制线条 */
	else if(g_ucLineStyle == 1)
	{
		GUI_SetLineStyle(GUI_LS_DASH);
	}
	/* 波形显示方式3：按圆点样式绘制线条 */
	else if(g_ucLineStyle == 2)
	{
		GUI_SetLineStyle(GUI_LS_DOT);
	}
	/* 波形显示方式4：按虚线和圆点交替的样式绘制线条 */
	else if(g_ucLineStyle == 3)
	{
		GUI_SetLineStyle(GUI_LS_DASHDOT);
	}
	/* 波形显示方式5：按虚线和双点交替的样式绘制线条 */
	else if(g_ucLineStyle == 4)
	{
		GUI_SetLineStyle(GUI_LS_DASHDOTDOT);
	}
	
	/* 单通道采样率从2.8Msps到500sps采用下面的方式显示 */
	if(TimeBaseId < 13)
	{
		GUI_DrawPolyLine(aPointsRe,              /* 指向要显示的折线 */
		                 GUI_COUNTOF(aPointsRe), /* 点列表中指定的点数量 */
		                 DSOSCREEN_STARTX,       /* 原点的X位置 */
		                 DSOSCREEN_STARTY);      /* 原点的Y位置 */
	}
	/* 其余采用下面的方式显示 */
	else
	{
		/* 小于600个采样点时显示方式，600个点是一屏能够显示的数据个数 */
		if(TempTime < 600)
		{
			GUI_DrawPolyLine(&aPointsRe[600 - TempTime], TempTime, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
		}
		/* 大于等于600个采样点时显示方式 */
		else
		{
			GUI_DrawPolyLine(aPointsRe, 600, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
		}
	}
	
	/* 第4步：在LCD上显示FFT幅频**********************************************************************/
	/* 在自动触发模式，采样率为2.8Msps到5Ksps时(单通道)才显示FFT波形 */
	if((g_ucFFTDispFlag == 0)&&(TriggerFlag == 0)&&(TimeBaseId < 10))
	{
		/* 求FFT变换结果的幅值 */
		for(i = 0; i < DSOSCREEN_LENGTH; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/1024; 
		}
		
		/* 直流分量大小 */
		testInput_fft_2048[0] = testInput_fft_2048[0]/2;
		
		/* 求得幅频做一下转换，方便在LCD上显示 */
		for(i = 0; i < DSOSCREEN_LENGTH-2; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/5.0f;
			aPointsRe[i+2].y = 370 - testInput_fft_2048[i] ;
		}
		
		GUI_SetColor(GUI_RED);
		GUI_DrawPolyLine(&aPointsRe[2], 598, DSOSCREEN_STARTX, DSOSCREEN_STARTY);
	}
	
	
	/* 波形显示完后恢复实线显示 */
	GUI_SetLineStyle(GUI_LS_SOLID);

	/* 第5步：显示触发游标的位置*************************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
	/* 显示触发游标 */
	GUI_FillPolygon(&aPointsTrig[0],          /* 指向要显示和填充的多边形 */
	                GUI_COUNTOF(aPointsTrig), /* 点列表中指定的点数量 */
	                628,                      /* 原点的X位置 */
	                g_usTriPos);              /* 原点的Y位置 */
	
	/* 重新设置触发值后，显示游标水平线1s时间 */
	if((GUI_GetTime() - g_ulTrigTimeLine) < 1000)
	{
		GUI_DrawHLine(g_usTriPos, 40, 626);
	}
	
	/* 第6步：垂直测量，用于测量波形的时间************************************************************************/
	if(hWinCoursorFlag == 1)
	{
		DSO_DrawCursorH();
	}
	
	/* 第7步：水平测量，用于测量幅值差值************************************************************************/
	if(hWinCoursorFlag == 2)
	{
		DSO_DrawCursorV();
	}
}

/*
*********************************************************************************************************
*	函 数 名: _Draw
*	功能说明: 绘制波形和波形的背景显示
*	形    参：p  指针地址         	
*	返 回 值: 无
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
	
	   
	/* 指向波形缓存地址 */
	Param.aY = g_usWaveBuf;
	
	/* 开启后默认显示的是峰峰值，最大值，最小值，平均值 */
	g_ucMeasureFlag[12] = 1;
	g_ucMeasureFlag[14] = 1;
	g_ucMeasureFlag[15] = 1;
	g_ucMeasureFlag[20] = 1;
	
	/* 初始化波形显示的横坐标 */
	for(i = 0; i < 600; i++)
	{
		aPointsRe[i].x = i;
	}
	
	/* 实数序列FFT长度 */
	fftSize = 2048; 
	/* 正变换 */
    ifftFlag = 0; 
	
	/* 初始化结构体S中的参数 */
 	arm_rfft_fast_init_f32(&S, fftSize);
	
	while(1) 
	{
		g_WaveMin = 4095;
		g_WaveMax = 0.0f;
		g_WaveMean = 0.0f;
		
		/* 没有选择触发时的波形显示 */
		/*
			1. 每次至少要保证采集6KB的数据，也就是单通道2Ksps（500us）时。
		       
		       //每次刷新前采集完600个数据
		    2. 2Ksps    6KB数据    //100ms刷新一次
			   1Ksps    3KB数据    //200ms刷新一次
			   
			   //移动式的显示数据 
			   500sps   1.5KB数据  
			   200sps   600B数据   
			   100sps   300B数据
		       50sps    150B数据
		       20sps    60B数据
		       10sps    30B数据
		       5sps     15B数据
		       2sps     6B数据
		       1sps     3B数据
		*/
		if(TriggerFlag == 0)
		{
			/* 读取的是ADC3的位置 */
			usCurPos = 10240 - DMA2_Stream1->NDTR;
			
			/* 前10种采样率采用直接刷新的方式进行显示****************************************/
			if(TimeBaseId < 10)
			{
				/*ADC1，ADC2，ADC3都有10k 16bit的数据缓存区。而我们每次采集时先确定当前DMA已经传输到的位置，
				  然后取这个位置之前的2K数据。
				*/
				/* 第一种情况：当前DMA传输位置在小于2k的地方 */
				if(usCurPos < 2048)
				{
					//j = 2048 - usCurPos;
					//j = 10240 - j;
					j = 8192 + usCurPos;
					
					/* 获取2k数据的前部分  */
					for(i = j; i < 10240; i++)
					{
						k = (i-j)*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i];
					}
					
					j = (2048 - usCurPos)*3;
					/* 获取2K数据的后部分 */
					for(i = 0; i < usCurPos; i++)
					{
						k = i*3;
						g_usWaveBuf1[j+k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[j+k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[j+k+2] = ADC3ConvertedValue[i];
					}		
				}
				/* 第二种情况：当前DMA传输位置在大于等于2k的地方 */
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
				
				/* 通过软件检测实现上升沿触发，并保留最后600的数据不做检测，用于直接显示 
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
				
				/* 记录满足上升沿触发的位置 */
				g_usCurTriPos = j;
				
				/* 变量g_usCurTriStep用来实现6K数据的移动，防止出现小于0 */
				if((g_usCurTriPos + g_usCurTriStep) < 0)
				{
					/* 将g_usCurTriStep取最接近g_usCurTriPos + g_usCurTriStep = 0 */
					temp = g_usCurTriPos/100*100;
					g_usCurTriStep = -temp;
				}
				
				/* 共采集6*1024 = 6144个数据，移动到最后至少还有剩600个数据，即6144-600=5544。
				   防止g_usCurTriPos + g_usCurTriStep的位置超过5544。
				*/
				if((g_usCurTriPos + g_usCurTriStep) > 5544)
				{
					/* 将g_usCurTriStep取最接近g_usCurTriPos + g_usCurTriStep < 5544的数值 */
					temp = (5544 - g_usCurTriPos)/100*100;
					g_usCurTriStep = temp;
				}
				
				/* 从位置g_usCurTriPos+g_usCurTriStep开始的600个数据全部赋值给g_usWaveBuf */
				for(i = 0; i < DSOSCREEN_LENGTH; i++)
				{
					g_usWaveBuf[i] =g_usWaveBuf1[i+g_usCurTriPos+g_usCurTriStep];
					g_WaveMean += g_usWaveBuf[i];
				}
				
				/* 执行2048点浮点FFT运算*******************************************************/
				for(i=0; i<2048; i++)
				{
					testInput_fft_2048[i] = g_usWaveBuf1[i];
				}
				
				/* 2048点实序列快速FFT, 浮点 */ 
				arm_rfft_fast_f32(&S, testInput_fft_2048, testOutput_fft_2048, ifftFlag);
				
				/* 求DSOSCREEN_LENGTH = 600点的幅值 */ 
				arm_cmplx_mag_f32(testOutput_fft_2048, testInput_fft_2048, DSOSCREEN_LENGTH);
				
				/* 执行650点，80阶Fir滤波器******************************************************/
				/* 单通道2Msps */
				if((TimeBaseId == 1)&&(g_ucFirFlter_Step100KHz !=0 ))
				{
					DSO_FirFilter_Step100KHz();
				}
					
				/* 单通道100Ksps */
				if((TimeBaseId == 5)&&(g_ucFirFlter_Step10KHz !=0 ))
				{
					DSO_FirFilter_Step10KHz();
				}
					
				/* 单通道10Ksps */
				if((TimeBaseId == 8)&&(g_ucFirFlter_Step1KHz !=0 ))
				{
					DSO_FirFilter_Step1KHz();
				}
			}
			/* 前10种采样率之后采用直接刷新的方式进行显示****************************************/
			else
			{
				/* g_ucSlowWaveFlag = 0 表示启动慢速显示模式，也就是滚动显示 */
			    if(g_ucSlowWaveFlag == 0)
				{
					/* TempTime表示DMA当前传输到的位置 */
					TempTime = 10240 - DMA2_Stream1->NDTR;
				
					/* g_ulSlowRefreshWait表示按键调节频率后DMA传输到的位置 
					   通过TempTime和g_ulSlowRefreshWait的差值确定刚开始显示
					   波形时600个数据的起始位置。(刚开始显示波形有一个从右向左的移动过程)
					*/
					if(TempTime >= g_ulSlowRefreshWait)
					{
						TempTime = TempTime - g_ulSlowRefreshWait;
					}
					else
					{
						TempTime = 10240 + TempTime - g_ulSlowRefreshWait;
					}
					
					/* 当差值大于600的时候，停止移动过程，开始整体刷新 */
					if(TempTime >= 600)
					{
						g_ucSlowWaveFlag = 1;
					}
				}
				
				/* 第一种情况：当前DMA传输位置在小于200的地方 */
				if(usCurPos < 200)
				{
					//j = 200 - usCurPos;
					//j = 10240 - j;
					j = 10040 + usCurPos;
					
					/* 获取200个数据的前部分  */
					for(i = j; i < 10240; i++)
					{
						k = (i-j)*3;
						g_usWaveBuf1[k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[k+2] = ADC3ConvertedValue[i];
					}
					
					j = (200 - usCurPos)*3;
					/* 获取200个数据的后部分  */
					for(i = 0; i < usCurPos; i++)
					{
						k = i*3;
						g_usWaveBuf1[j+k] = ADC1ConvertedValue[i];
						g_usWaveBuf1[j+k+1] = ADC2ConvertedValue[i];
						g_usWaveBuf1[j+k+2] = ADC3ConvertedValue[i];
					}
					
				}
				/* 第二种情况：当前DMA传输位置在大于等于200的地方 */
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

				/* 将获取的数据赋值给g_usWaveBuf */
				for(i = 0; i < DSOSCREEN_LENGTH; i++)
				{
					g_usWaveBuf[i] =g_usWaveBuf1[i];
					g_WaveMean += g_usWaveBuf[i];
				}
			
			}
			
		}
		/* 选择 普通触发 模式后的波形显示 
		   普通触发模式一般情况下是不刷新显示的，只有被触发的情况在才做界面的刷新。
		*/
		else
		{
			/* 此标志用来表示允许或者暂停波形显示，1表示暂停，0表示运行 */
			hWinRunStopFlag = 1;
			
			/* TriggerFlag = 2 表示达到触发条件后，已经采集完触发值前后的1K数据（单通道） */
			if(TriggerFlag == 2)
			{
				/* 允许波形显示 */
				hWinRunStopFlag = 0;
				
				/* 采集的时候是采集了单通道前后各1024的数据，三通道总共就是1024*6=6144
                   6144/2 = 3072 第3072个就是触发值，下面是取了3072的前300个和后300个在屏
				   上进行显示，通过变量g_usCurTriStep配合按键可以实现波形的移动观察。
				*/
				for(i = 2772; i < 3372; i++)
				{
					g_usWaveBuf[i-2772] = g_usWaveBuf1[i+g_usCurTriStep] ;
				}
				
				/* TriggerFlag = 1 表示等待下次触发 */
				TriggerFlag = 1;
				
				/* 使能ADC3的看门狗 */
				ADC_ITConfig(ADC3, ADC_IT_AWD, ENABLE);
			}
		}
		
		/* 求600个数值的最大值和最小值 */
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
			
		/* 求平均值 */
		g_WaveMean = g_WaveMean / 1000 *3.3f / 4095;
		
		/* 求最大值 */
		g_WaveMax =  g_WaveMax * 3.3f / 4095;
		
		/* 求最小值 */
		g_WaveMin = g_WaveMin *3.3f / 4095;
		
		/* 求峰峰值 */
		g_WavePkPk = g_WaveMax - g_WaveMin;
		
				
		/* 记录刷新前的时刻 */
		t0 = GUI_GetTime();
		
		/* hWinRunStopFlag = 0 表示运行态 
		   g_ucWaveRefreshFlag = 1也表示运行态，用于使用K2按键将波形暂停后波形的刷新。
		*/
		if((hWinRunStopFlag == 0)||(g_ucWaveRefreshFlag == 1))
		{
			/* 采用分片存储空间绘制 */
			/* 矩形框，回调函数，回调函数的参数 ，0或存储设备的分段数量, O或者GUI_MEMDEV_HASTRANS */
			GUI_MEMDEV_Draw(&Rect, _Draw, &Param, 0, GUI_MEMDEV_NOTRANS);
			g_ucWaveRefreshFlag = 0;  /* 0：表示不执行波形区域的刷新，1：表示执行波形区域的刷新 */
		}

		/* 求差值，根据不同的执行时间差，执行不同的延迟时间 */
		t1 = GUI_GetTime() - t0;
		printf("%d\r\n",  t1);
		if (t1 < g_RefreshDelay[TimeBaseId]) 
		{  
			GUI_Delay(g_RefreshDelay[TimeBaseId] - t1);
		}
		/* 防止规定的时间内没有完成波形的刷新，而此时又来了GUI消息，需要通过下面函数进行处理
	       要不会出现GUI消息得不到处理的情况。
		   
		   刷新定时器消息的话，用WM_Exec是不行的。
		*/
		else
		{
			GUI_Delay(1);
			//GUI_Exec();
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		
		/* 按键消息处理 */
		uKeyCode = bsp_GetKey();
		
		/* 判断是否有按键按下 */
		if(uKeyCode != KEY_NONE)
		{

			switch (uKeyCode)
			{
			    /* 按键K1 用于实现屏幕截图功能 ********************************************************/	
				case KEY_1_DOWN:		
				    /* 隐藏主界面的窗口 */
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

					/* 执行触摸校准 */
					TOUCH_Calibration();
				
					 /* 显示主界面的窗口 */
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
					
					/************由于按键不够用，将截图功能取消***********/
 					//BSP_OS_SemPost(&SEM_SYNCH);  
					break;
				
				/* 按键K2 用于实现波形的暂停和运行 **************************************************/
				case KEY_2_DOWN:		
					hWinRunStopFlag ^= 1;
					WM_InvalidateArea(&rRunMode);
					break;
				
				/* 按键K3 用于切换自动触发和普通触发 **********************************************/
                case KEY_3_DOWN:		
					/* TriggerFlag = 0 表示当前用的是自动触发模式 */
					if(TriggerFlag == 0)
					{
						/* 从自动触发切换到普通触发 */
						TriggerFlag = 1;
						
						g_usCurTriStep = 0;
						/* 更新触发位置 */
						WM_InvalidateArea(&rTrigPos);
						
						/*  进入触发后重新设置ADC触发值为1V 
						    切换后重新设置采样率位2Msps 
						*/
						ADC_DMA_Close();
						ADC_DMA_Open();
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						/* 进入触发界面后，默认设置触发是1V，也就是1241 */
						ADC_AnalogWatchdogThresholdsConfig(ADC3, 1241, 0);
					}
					else
					{
						/* TriggerFlag = 0 表示当前用的是自动触发模式 */
						TriggerFlag = 0;
						/* 波形显示 */
						hWinRunStopFlag = 0;
						
						/* 返回后，不再需要看门狗设置触发 */
						ADC_AnalogWatchdogThresholdsConfig(ADC3, 4095, 0);
					}
					
					/* 两种触发模式切换后，都将数值设置为默认 */
					g_usTriPos = 190;     
					g_usTrigValue = 1241; 
					WM_InvalidateArea(&rTrigValue);
					WM_InvalidateArea(&rTrigMode);
					
					/* 刚切换到普通触发模式显示一次波形 */
					if(TriggerFlag == 1)
					{
						TriggerFlag = 2;
					}
					break;
             
				/* 摇杆上键按下 *************************************************************/
                case JOY_DOWN_U:	
					/* 调节幅度放大 */
					if(hWinButStateFlag == 1) /* 幅度调节 */
					{
						Ch1AmpId++;
						if(Ch1AmpId == ATT_COUNT)
						{
							Ch1AmpId = ATT_COUNT - 1;
						}
						WM_SendMessageNoPara(hWinAmp, WM_TextUpDate);
					}
					/* 调节参考电压的上移 */
					else if(hWinButStateFlag == 2) 
					{
						g_usRefPos -= 10;
						if(g_usRefPos<= 40)
						{
							g_usRefPos = 40;
						}
						WM_InvalidateArea(&rRefPos);
					}
					/* 调节水平横线A上移 */
					else if(hWinButStateFlag == 5)
					{
						g_sCursorHA -= g_usCursorStep;
						if(g_sCursorHA <= 40)
						{
							g_sCursorHA = 40;
						}
					}
					/* 调节水平横线B上移  */
					else if(hWinButStateFlag == 6) 
					{
						g_sCursorHB -= g_usCursorStep;
						if(g_sCursorHB <= g_sCursorHA)
						{
							g_sCursorHB = g_sCursorHA;
						}
					}
					/* 调节自动触发和普通触发的触发值 */
					else if(hWinButStateFlag == 7) 
					{
						/* 调整触发箭头的位置，方便查看 */
						g_usTriPos -= g_usTriStep;
						if(g_usTriPos <= DSOSCREEN_STARTY)
						{
							g_usTriPos = DSOSCREEN_STARTY;
						}
						
						/* 根据调节的触发箭头位置获得触发值 */
						g_ufTrigValue = 240 - g_usTriPos;
						g_ufTrigValue = g_ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_ufTrigValue = g_ufTrigValue * 4095 / 3.3f;
						g_usTrigValue = g_ufTrigValue;
						
						/* 更新触发值的显示并获得当前的时间，从而实现调节触发值后，显现一条水平的直线，时长1s*/
						g_ulTrigTimeLine = GUI_GetTime();
						WM_InvalidateArea(&rTrigValue);
						
						/* 更新后，更新一下波形显示，仅限普通触发方式 */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* 如果是普通触发模式，更新ADC3的看门狗阀值 */
							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_usTrigValue, 0);
						}
					}
					
					/* 使用K2按键将波形暂停后，通过变量g_ucWaveRefreshFlag置1进行波形刷新 */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* 摇杆下键按下 *************************************************************/
				case JOY_DOWN_D:	
					/* 调节幅度衰减 */
					if(hWinButStateFlag == 1) 
					{
						Ch1AmpId--;
						if(Ch1AmpId < 0)
						{
							Ch1AmpId = 0;
						}
						WM_SendMessageNoPara(hWinAmp, WM_TextUpDate);
					}
					/* 调节参考电压的下移 */
					else if(hWinButStateFlag == 2) /* 幅度调节 */
					{
						g_usRefPos += 10;
						if(g_usRefPos >= 440)
						{
							g_usRefPos = 440;
						}
						WM_InvalidateArea(&rRefPos);
					}
					/* 调节水平横线A下移 */
					if(hWinButStateFlag == 5) 
					{
						g_sCursorHA += g_usCursorStep;
						if(g_sCursorHA >= g_sCursorHB)
						{
							g_sCursorHA = g_sCursorHB;
						}
					}
					 /* 调节水平横线B下移  */
					else if(hWinButStateFlag == 6)
					{
						g_sCursorHB += g_usCursorStep;
						if(g_sCursorHB >= 440)
						{
							g_sCursorHB = 440;
						}
					}
					/* 调节自动触发和普通触发的触发值 */
					else if(hWinButStateFlag == 7) 
					{
						/* 调整触发箭头的位置，方便查看 */
						g_usTriPos += g_usTriStep;
						if(g_usTriPos >= DSOSCREEN_ENDY)
						{
							g_usTriPos = DSOSCREEN_ENDY;
						}
						
						/* 根据调节的触发箭头位置获得触发值 */
						g_ufTrigValue = 240 - g_usTriPos;
						g_ufTrigValue = g_ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_ufTrigValue = g_ufTrigValue * 4095 / 3.3f;
						g_usTrigValue = g_ufTrigValue;
						
						/* 更新触发值的显示并获得当前的时间，从而实现调节触发值后，显现一条水平的直线，时长1s*/
						g_ulTrigTimeLine = GUI_GetTime();
						WM_InvalidateArea(&rTrigValue);
						
						/* 更新后，更新一下波形显示，仅限普通触发方式 */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* 如果是普通触发模式，更新ADC3的看门狗阀值 */
							ADC_AnalogWatchdogThresholdsConfig(ADC3, g_usTrigValue, 0);
						}
					}
					
					/* 使用K2按键将波形暂停后，通过变量g_ucWaveRefreshFlag置1进行波形刷新 */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* 摇杆左键按下 *************************************************************/
				case JOY_DOWN_L:	
					/* 调节ADC的采样率 */
					if(hWinButStateFlag == 0) 
					{
						TimeBaseId++;
						if(TimeBaseId == TIME_COUNT)
						{
							TimeBaseId = TIME_COUNT - 1;
						}
						ADC_DMA_Close();
						ADC_DMA_Open();
						
						/* 如果是低频的慢速显示需要记录当前的DMA出传输位置 */
						g_ulSlowRefreshWait = 10240 - DMA2_Stream1->NDTR;
						TempTime = 0;
						
						/* 更新当前采样时基显示 */
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						//WM_Exec();
						
						/* g_ucSlowWaveFlag = 0 表示启动慢速显示模式，也就是滚动显示
						   在低频采样时，设置g_ucSlowWaveFlag = 0 才有效。这里为了方便直接
						   设置为0；
						*/
						g_ucSlowWaveFlag = 0;
					}
					/* 调节垂直线A左移 */
					else if(hWinButStateFlag == 3) 
					{
						g_sCursorVA -= g_usCursorStep;
						if(g_sCursorVA <= 40)
						{
							g_sCursorVA = 40;
						}
					}
					/* 调节垂直线B左移 */
					else if(hWinButStateFlag == 4) 
					{
						g_sCursorVB -= g_usCursorStep;
						if(g_sCursorVB <= g_sCursorVA)
						{
							g_sCursorVB = g_sCursorVA;
						}
					}
					/* 自动触发模式和普通触发模式6K数据的显示起始位置设定 */
					else if(hWinButStateFlag == 7) 
					{
						/* 自动触发模式：6K数据的显示起始位置设定 */
						if(TriggerFlag == 0)
						{
							/* 左移100个数据 */
							g_usCurTriStep -= 100;	
						}
						/* 普通触发模式：6K数据的显示起始位置设定 */
						else
						{
							/* 左移100个数据 */
							g_usCurTriStep -= 100;
							/* 理论上这里的2700应该取2772，但每次的偏移是100，直接取2700比较方便 */
							if(g_usCurTriStep <= -2700)
							{
								g_usCurTriStep = -2700;
							}
							
							/* 通过设置为2来更新波形显示 */
							TriggerFlag = 2;
						}
					
						/* 更新当前的数据位置的箭头 */
						WM_InvalidateArea(&rTrigPos);
					}
			
					/* 使用K2按键将波形暂停后，通过变量g_ucWaveRefreshFlag置1进行波形刷新 */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;
					
				/* 摇杆右键按下 *************************************************************/
				case JOY_DOWN_R:		
					/* 调节ADC的采样率 */
					if(hWinButStateFlag == 0) 
					{
						TimeBaseId--;
						if(TimeBaseId < 0)
						{
							TimeBaseId = 0;
						}
						ADC_DMA_Close();
						ADC_DMA_Open();
						/* 如果是低频的慢速显示需要记录当前的DMA出传输位置 */
						g_ulSlowRefreshWait = 10240 - DMA2_Stream1->NDTR;
						TempTime = 0;
						
						/* 更新当前采样时基显示 */
						WM_SendMessageNoPara(hWinScale, WM_TextUpDate);
						//WM_Exec();
						
						/* g_ucSlowWaveFlag = 0 表示启动慢速显示模式，也就是滚动显示
						   在低频采样时，设置g_ucSlowWaveFlag = 0 才有效。这里为了方便直接
						   设置为0；
						*/
						g_ucSlowWaveFlag = 0;
					}
					/* 调节垂直线A右移 */
					else if(hWinButStateFlag == 3) 
					{
						g_sCursorVA += g_usCursorStep;
						if(g_sCursorVA >= g_sCursorVB)
						{
							g_sCursorVA = g_sCursorVB;
						}
					}
					/* 调节垂直线B右移 */
					else if(hWinButStateFlag == 4)
					{
						g_sCursorVB += g_usCursorStep;
						if(g_sCursorVB >= 640)
						{
							g_sCursorVB = 640;
						}
					}
					/* 自动触发模式和普通触发模式6K数据的显示起始位置设定 */
					else if(hWinButStateFlag == 7) 
					{
						/* 自动触发模式：6K数据的显示起始位置设定 */
						if(TriggerFlag == 0)
						{
							/* 右移100个数据 */
							g_usCurTriStep += 100;						
						}
						/* 普通触发模式：6K数据的显示起始位置设定 */
						else
						{
							/* 右移100个数据 */
							g_usCurTriStep += 100;
							/* 自动触发模式支持的数据显示是6K=6144，但是为了方便，
							   我们只观看前后各3000的数据，共6000。下面的数据理论上
							   也是取2772，由于偏移是100，取2700更方便。
							*/
							if(g_usCurTriStep >= 2700) 
							{
								g_usCurTriStep = 2700;
							}
							
							/* 通过设置为2来更新波形显示 */
							TriggerFlag = 2;
						}
						
						/* 更新当前的数据位置的箭头 */
						WM_InvalidateArea(&rTrigPos);
					}
					
					/* 使用K2按键将波形暂停后，通过变量g_ucWaveRefreshFlag置1进行波形刷新 */
					if(hWinRunStopFlag == 1)
					{
						g_ucWaveRefreshFlag = 1;  
					}
					break;

				/* 摇杆OK键按下 *************************************************************/
				case JOY_DOWN_OK:
					/* OK键用于实现8种功能的切换 */
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
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参：无       	
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{
	/* 开启所有窗口使用内存设备 */
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	
	/* 设置皮肤色 *************************************************************/
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
	* 设置桌面窗口的回调函数
	*/
	WM_SetCallback(WM_HBKWIN, _cbBkWin);
	
	/* 初始化 DSO */
	DSO_Init(1);
	
	/* 波形显示和处理 */
	DSO_Graph();																								 
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
