/**
  ******************************************************************************
  * @file    7_main_ui_layout.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   7寸屏的布局1
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "fonts.h"
#include "app.h"
#include "scan_keyboard.h"
#include "IMAGE.H"
#include "UI_COM/com_ui_info.h"
#include "test_win.h"
#include "string.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SIZE_7INCH_WIDTH	800
#define SIZE_7INCH_HEIGH	480
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


// 	0/*x*/,0/*y*/,690/*width*/,455/*height*/,
WIDGET_POS_SIZE_T _7_test_windows=
{
	0/*x*/, 0/*y*/, 690/*width*/, 455/*height*/,
};

#define PB_X 40		///<多路信息区基坐标
#define PB_Y 50
#define TP_W 270	///<每路信息区的宽度
#define TP_XO 50	///<相临信息区的X坐标偏移
#define TP_YO 40	///<相临信息区的Y坐标偏移
#define TP_H 168	///<每路信息区的高度

#define PB1_X (PB_X)	///<第一路坐标
#define PB1_Y (PB_Y)
#define PB2_X (PB_X + TP_W + TP_XO) ///<第二路坐标
#define PB2_Y (PB_Y)
#define PB3_X (PB_X) 	///<第三路坐标
#define PB3_Y (PB_Y + TP_H + TP_YO)
#define PB4_X (PB_X + TP_W + TP_XO)	///<第四路坐标
#define PB4_Y (PB_Y + TP_H + TP_YO)

static const GUI_RECT test_port1_area = {PB1_X, PB1_Y, PB1_X + TP_W, PB1_Y + TP_H};
static const GUI_RECT test_port2_area = {PB2_X, PB2_Y, PB2_X + TP_W, PB2_Y + TP_H};
static const GUI_RECT test_port3_area = {PB3_X, PB3_Y, PB3_X + TP_W, PB3_Y + TP_H};
static const GUI_RECT test_port4_area = {PB4_X, PB4_Y, PB4_X + TP_W, PB4_Y + TP_H};

static void myGUI_DrawRectEx(const GUI_RECT * pRect)
{
	GUI_DrawLine(pRect->x0, pRect->y0, pRect->x1, pRect->y0);
	GUI_DrawLine(pRect->x0, pRect->y1, pRect->x1, pRect->y1);
	GUI_DrawLine(pRect->x0, pRect->y0, pRect->x0, pRect->y1);
	GUI_DrawLine(pRect->x1, pRect->y0, pRect->x1, pRect->y1);
}
/**
  * @brief  绘制7寸屏第1种布局框
  * @param  无
  * @retval 无
  */
void draw_composition_7_1(void)
{
	GUI_SetColor(GUI_WHITE);
	GUI_SetPenSize(2);
	myGUI_DrawRectEx(&test_port1_area);
	myGUI_DrawRectEx(&test_port2_area);
	myGUI_DrawRectEx(&test_port3_area);
	myGUI_DrawRectEx(&test_port4_area);
}

typedef struct{
    TEXT_ELE_T * name;///<多路编号
    TEXT_ELE_T * cur_name;///<多路测试模式
    TEXT_ELE_T * step;///<多路测试状态
    TEXT_ELE_T * cur_step;///<多路电压
    TEXT_ELE_T * work_mode;///<多路电流
    TEXT_ELE_T * cur_work_mode;///<多路真实电流
}FILE_T;

typedef struct{
    TEXT_ELE_T * num;///<多路编号
    TEXT_ELE_T * mode;///<多路测试模式
    TEXT_ELE_T * status;///<多路测试状态
    TEXT_ELE_T * vol;///<多路电压
    TEXT_ELE_T * cur;///<多路电流
    TEXT_ELE_T * real;///<多路真实电流
    TEXT_ELE_T * time;///<多路时间
}ROAD_T;

static void _7_test_ui_init_file_inf(uint16_t base_x, uint16_t base_y, FILE_T *pool)
{
    UI_ELE_DISPLAY_INFO_T inf;
    
    #define GB_X		0 ///<记忆组
    #define GB_Y		0
    #define GB_H		30
    #define FN_W		120 ///<文件名
    #define FN_X		0
    #define C_FN_X		FN_X+FN_W ///<当前文件名
    #define C_FN_W		140
    #define STEP_X  	C_FN_X+C_FN_W+1 ///< 步骤
    #define STEP_W  	65
    #define CS_X  		STEP_X+STEP_W//当前步骤
    #define CS_W  		80
    #define WM_X  		CS_X+CS_W+1 ///<工作模式
    #define WM_W		120
    #define CWM_X		WM_X+WM_W
    #define CWM_W  		20
    #define TF_FONT     &GUI_Fonthz_24
    
    /* "文件名" */
    inf.base_x = base_x;//x基坐标 
    inf.base_y = base_y;//y基坐标
    
    inf.font = TF_FONT;//字体
    inf.max_len = 100;//最大长度
    inf.font_color = GUI_BLACK;//字体颜色
    inf.back_color = GUI_INVALID_COLOR;//背景颜色
    inf.align = GUI_TA_RIGHT | GUI_TA_VCENTER;//对齐方式
    inf.pos_size.height = GB_H;//高
    
    inf.pos_size.width = FN_W;//宽
    inf.pos_size.x = FN_X;//x相对坐标
    inf.pos_size.y = 0;//y相对坐标
    
    memcpy(&pool->name->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 文件名 */
    inf.align = GUI_TA_LEFT | GUI_TA_VCENTER;//对齐方式
    inf.pos_size.width = C_FN_W;//宽
    inf.pos_size.x = C_FN_X;//x相对坐标
    
    memcpy(&pool->cur_name->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* "步骤" */
    inf.pos_size.x = STEP_X;//x相对坐标
    inf.pos_size.width = STEP_W;//宽
    
    memcpy(&pool->step->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 步骤 */
    inf.pos_size.x = CS_X;//x相对坐标
    inf.pos_size.width = CS_W;//宽
    
    memcpy(&pool->cur_step->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* "工作模式" */
    inf.pos_size.x = WM_X;//x相对坐标
    inf.pos_size.width = WM_W;//宽
    
    memcpy(&pool->work_mode->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 工作模式 */
    inf.pos_size.x = CWM_X;//x相对坐标
    inf.pos_size.width = CWM_W;//宽
    
    memcpy(&pool->cur_work_mode->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}


static void init_one_road_pos_size_inf(uint16_t base_x, uint16_t base_y, ROAD_T *road)
{
    UI_ELE_DISPLAY_INFO_T inf;
    
    #define ROAD_FONTS		&GUI_Fonthz_29
    #define RB_H	30
    #define RN_X	0
    #define RN_Y	0
    #define RN_W	50
    #define RM_X	RN_X + RN_W + 1	//模式
    #define RM_Y	RN_Y
    #define RM_W	50
    #define RS_X	RM_X + RM_W + 1	//状态
    #define RS_Y	RN_Y
    #define RS_W	TP_W - RM_W - RN_W - 4
    
    #define RV_X	10					//电压
    #define RV_Y	RB_H + 30
    #define RV_W	120	
    #define RC_X	RV_X + RV_W + 20	//电流
    #define RC_Y	RB_H + 30
    #define RC_W	RV_W
    #define RR_X	RC_X				//真实电流
    #define RR_Y	RB_H * 2 + 50
    #define RR_W	RV_W
    #define RT_X	RV_X				//时间
    #define RT_Y	RB_H * 2 + 50
    #define RT_W	95
    
    /* 多路编号 */
    inf.base_x = base_x;
    inf.base_y = base_y;
    inf.max_len = 20;//最大长度
    inf.align = GUI_TA_CENTER | GUI_TA_TOP;
    inf.back_color = GUI_INVALID_COLOR;
    inf.font = ROAD_FONTS;
    inf.font_color = GUI_WHITE;
    inf.pos_size.height = RB_H;
    inf.pos_size.width = RN_W;
    inf.pos_size.x = RN_X;
    inf.pos_size.y = RN_Y;
    
    memcpy(&road->num->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 测试模式 */
    inf.align = GUI_TA_LEFT | GUI_TA_TOP;
    inf.pos_size.width = RM_W;
    inf.pos_size.x = RM_X;
    inf.pos_size.y = RM_Y;
    
    memcpy(&road->mode->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 测试状态 */
    inf.align = GUI_TA_CENTER | GUI_TA_TOP;
    inf.pos_size.width = RS_W;
    inf.pos_size.x = RS_X;
    inf.pos_size.y = RS_Y;
    
    memcpy(&road->status->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 输出电压 */
    inf.align = GUI_TA_LEFT | GUI_TA_TOP;
    inf.pos_size.width = RV_W;
    inf.pos_size.x = RV_X;
    inf.pos_size.y = RV_Y;
    
    memcpy(&road->vol->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 回路电流 */
    inf.align = GUI_TA_LEFT | GUI_TA_TOP;
    inf.pos_size.width = RC_W;
    inf.pos_size.x = RC_X;
    inf.pos_size.y = RC_Y;
    
    memcpy(&road->cur->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 回路真实电流 */
    inf.align = GUI_TA_LEFT | GUI_TA_TOP;
    inf.pos_size.width = RR_W;
    inf.pos_size.x = RR_X;
    inf.pos_size.y = RR_Y;
    
    memcpy(&road->real->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 测试时间 */
    inf.align = GUI_TA_LEFT | GUI_TA_TOP;
    inf.pos_size.width = RT_W;
    inf.pos_size.x = RT_X;
    inf.pos_size.y = RT_Y;
    
    memcpy(&road->time->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}
void _7_init_test_ui_layout1_text_ele_pos(TEXT_ELE_T *pool)
{
    FILE_T file_t;
    ROAD_T road_pool[4];
    
    file_t.name = &pool[TEST_UI_FILE_NAME];
    file_t.cur_name = &pool[TEST_UI_CUR_FILE_NAME];
    file_t.step = &pool[TEST_UI_STEP];
    file_t.cur_step = &pool[TEST_UI_CUR_STEP];
    file_t.work_mode = &pool[TEST_UI_WORK_MODE];
    file_t.cur_work_mode = &pool[TEST_UI_CUR_WORK_MODE];
    
    road_pool[0].num    = &pool[TEST_UI_ROAD01_NUM];
    road_pool[0].mode   = &pool[TEST_UI_ROAD01_MODE];
    road_pool[0].status = &pool[TEST_UI_ROAD01_STATUS];
    road_pool[0].vol    = &pool[TEST_UI_ROAD01_VOLTAGE];
    road_pool[0].cur    = &pool[TEST_UI_ROAD01_UPPER];
    road_pool[0].real   = &pool[TEST_UI_ROAD01_REAL];
    road_pool[0].time   = &pool[TEST_UI_ROAD01_TIME];
    
    road_pool[1].num    = &pool[TEST_UI_ROAD02_NUM];
    road_pool[1].mode   = &pool[TEST_UI_ROAD02_MODE];
    road_pool[1].status = &pool[TEST_UI_ROAD02_STATUS];
    road_pool[1].vol    = &pool[TEST_UI_ROAD02_VOLTAGE];
    road_pool[1].cur    = &pool[TEST_UI_ROAD02_UPPER];
    road_pool[1].real   = &pool[TEST_UI_ROAD02_REAL];
    road_pool[1].time   = &pool[TEST_UI_ROAD02_TIME];
    
    road_pool[2].num    = &pool[TEST_UI_ROAD03_NUM];
    road_pool[2].mode   = &pool[TEST_UI_ROAD03_MODE];
    road_pool[2].status = &pool[TEST_UI_ROAD03_STATUS];
    road_pool[2].vol    = &pool[TEST_UI_ROAD03_VOLTAGE];
    road_pool[2].cur    = &pool[TEST_UI_ROAD03_UPPER];
    road_pool[2].real   = &pool[TEST_UI_ROAD03_REAL];
    road_pool[2].time   = &pool[TEST_UI_ROAD03_TIME];
    
    road_pool[3].num    = &pool[TEST_UI_ROAD04_NUM];
    road_pool[3].mode   = &pool[TEST_UI_ROAD04_MODE];
    road_pool[3].status = &pool[TEST_UI_ROAD04_STATUS];
    road_pool[3].vol    = &pool[TEST_UI_ROAD04_VOLTAGE];
    road_pool[3].cur    = &pool[TEST_UI_ROAD04_UPPER];
    road_pool[3].real   = &pool[TEST_UI_ROAD04_REAL];
    road_pool[3].time   = &pool[TEST_UI_ROAD04_TIME];
    
    /******************** 测试界面文件信息 ***********************************/
    
    _7_test_ui_init_file_inf(GB_X, GB_Y, &file_t);
    
#define R1B_X	PB1_X+2
#define R1B_Y	PB1_Y+5
#define R2B_X	PB2_X+2
#define R2B_Y	PB2_Y+5
#define R3B_X	PB3_X+2
#define R3B_Y	PB3_Y+5
#define R4B_X	PB4_X+2
#define R4B_Y	PB4_Y+5
    
    init_one_road_pos_size_inf(R1B_X, R1B_Y, &road_pool[0]);
    init_one_road_pos_size_inf(R2B_X, R2B_Y, &road_pool[1]);
    init_one_road_pos_size_inf(R3B_X, R3B_Y, &road_pool[2]);
    init_one_road_pos_size_inf(R4B_X, R4B_Y, &road_pool[3]);
}
///**
//  * @brief  显示信息 数组成员的顺序必须与引用数组顺序一致
//  */
//UI_ELE_DISPLAY_INFO_T test_ui_ele_dis_info_pool[]=
//{
//#define GB_X		0 ///<记忆组
//#define GB_Y		0
//#define GB_H		30
//#define FN_W		120 ///<文件名
//#define FN_X		0
//#define C_FN_X		FN_X+FN_W ///<当前文件名
//#define C_FN_W		140
//#define STEP_X  	C_FN_X+C_FN_W+1 ///< 步骤
//#define STEP_W  	65
//#define CS_X  		STEP_X+STEP_W//当前步骤
//#define CS_W  		80
//#define WM_X  		CS_X+CS_W+1 ///<工作模式
//#define WM_W		120
//#define CWM_X		WM_X+WM_W
//#define CWM_W  		20
//	
//	//文件名
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,FN_X/*x*/,GB_Y/*y*/,FN_W/*width*/,GB_H/*height*/,10,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_RIGHT | GUI_TA_VCENTER
//	},
//	//当前的文件名
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,C_FN_X/*x*/,GB_Y/*y*/,C_FN_W/*width*/,GB_H/*height*/,15,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_VCENTER
//	},
//	//步骤
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,STEP_X/*x*/,GB_Y/*y*/,STEP_W/*width*/,GB_H/*height*/,10,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_VCENTER
//	},
//	//当前步骤
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,CS_X/*x*/,GB_Y/*y*/,CS_W/*width*/,GB_H/*height*/,10,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_VCENTER
//	},
//	//工作模式
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,WM_X/*x*/,GB_Y/*y*/,WM_W/*width*/,GB_H/*height*/,10,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_VCENTER
//	},
//	//当前工作模式
//	{
//		GB_X/*base_x*/,GB_Y/*base_y*/,CWM_X/*x*/,GB_Y/*y*/,CWM_W/*width*/,GB_H/*height*/,10,
//		&GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_VCENTER
//	},
//#define SI_FONTS		&GUI_Fonthz_20
//#define SIB_X 10  ///< 步骤信息基坐标
//#define SIB_Y 30
//#define SIB_H 30
//#define SIVOL_X  0 ///<步骤信息输出电压坐标
//#define SIVOL_Y  SIB_Y+SIB_H*1
//#define SIVOL_W  90
//#define CSIVOL_X  SIVOL_X+SIVOL_W+1
//#define CSIVOL_W  80
//	//输出电压
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIVOL_X/*x*/,SIVOL_Y/*y*/,SIVOL_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前输出电压
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIVOL_X/*x*/,SIVOL_Y/*y*/,CSIVOL_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define SIR_X  0 ///<步骤信息电流档位坐标
//#define SIR_Y  SIB_Y+SIB_H*2
//#define SIR_W  90
//#define CSIR_X  SIR_X+SIR_W+1
//#define CSIR_W  80
//	//电流档位
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIR_X/*x*/,SIR_Y/*y*/,SIR_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前电流档位
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIR_X/*x*/,SIR_Y/*y*/,CSIR_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define SIH_X  0 ///<步骤信息电流上限坐标
//#define SIH_Y  SIB_Y+SIB_H*3
//#define SIH_W  90
//#define CSIH_X  SIH_X+SIH_W+1
//#define CSIH_W  80
//	//电流上限
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIH_X/*x*/,SIH_Y/*y*/,SIH_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前电流上限
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIH_X/*x*/,SIH_Y/*y*/,CSIH_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define SIL_X  0 ///<步骤信息电流上限坐标
//#define SIL_Y  SIB_Y+SIB_H*4
//#define SIL_W  90
//#define CSIL_X  SIL_X+SIL_W+1
//#define CSIL_W  80
//	//电流下限
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIL_X/*x*/,SIL_Y/*y*/,SIL_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前电流下限
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIL_X/*x*/,SIL_Y/*y*/,CSIL_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define SIA_X  0 ///<步骤信息电流上限坐标
//#define SIA_Y  SIB_Y+SIB_H*5
//#define SIA_W  90
//#define CSIA_X  SIA_X+SIA_W+1
//#define CSIA_W  80
//	//电弧侦测
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIA_X/*x*/,SIA_Y/*y*/,SIA_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前电弧侦测
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIA_X/*x*/,SIA_Y/*y*/,CSIA_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define SIC_X  0 ///<步骤信息电流上限坐标
//#define SIC_Y  SIB_Y+SIB_H*6
//#define SIC_W  90
//#define CSIC_X  SIC_X+SIC_W+1
//#define CSIC_W  80
//	//步间连续
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,SIC_X/*x*/,SIC_Y/*y*/,SIC_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前步间连续
//	{
//		SIB_X/*base_x*/,SIB_Y/*base_y*/,CSIC_X/*x*/,SIC_Y/*y*/,CSIC_W/*width*/,SIB_H/*height*/,10,
//		SI_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	
//	
//#define SSI_FONTS		&GUI_Fonthz_24
//#define SSB_X 	10  ///< 步骤信息基坐标
//#define SSB_Y 	390
//#define SSB_H 	30
//#define TF_X	0	///< 系统信息栏测试预告
//#define TF_Y	0
//#define TF_W	80
//#define TF1_X	TF_X+TF_W	///< 系统信息栏测试预告1
//#define TF1_W	40
//#define TF2_X	TF1_X+TF1_W	///< 系统信息栏测试预告2
//#define TF2_W	30
//	//测试预告
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF_X/*x*/,TF_Y/*y*/,TF_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//测试预告1
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF1_X/*x*/,TF_Y/*y*/,TF1_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//测试预告2
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF2_X/*x*/,TF_Y/*y*/,TF2_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define TF3_X	TF2_X+TF2_W	///< 系统信息栏测试预告3
//#define TF3_W	40
//#define TF4_X	TF3_X+TF3_W	///< 系统信息栏测试预告4
//#define TF4_W	30
//#define TF5_X	TF4_X+TF4_W	///< 系统信息栏测试预告5
//#define TF5_W	40
//#define TF6_X	TF5_X+TF5_W	///< 系统信息栏测试预告6
//#define TF6_W	30
//	//测试预告3
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF3_X/*x*/,TF_Y/*y*/,TF3_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//测试预告4
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF4_X/*x*/,TF_Y/*y*/,TF4_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//测试预告5
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF5_X/*x*/,TF_Y/*y*/,TF5_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//测试预告6
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,TF6_X/*x*/,TF_Y/*y*/,TF6_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	
//#define PO_X	TF6_X+TF6_W + 1	///< 系统信息栏测试预告6
//#define PO_Y	0
//#define PO_W	75
//#define CPO_X	PO_X+PO_W
//#define CPO_W	70
//	//端口
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,PO_X/*x*/,PO_Y/*y*/,PO_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前端口
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,CPO_X/*x*/,PO_Y/*y*/,CPO_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define MR_X	CPO_X+CPO_W + 1	///< 系统信息栏测试预告6
//#define MR_Y	0
//#define MR_W	80
//#define CMR_X	MR_X+MR_W
//#define CMR_W	60
//	//多路
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,MR_X/*x*/,MR_Y/*y*/,MR_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//当前多路
//	{
//		SSB_X/*base_x*/,SSB_Y/*base_y*/,CMR_X/*x*/,MR_Y/*y*/,CMR_W/*width*/,SSB_H/*height*/,10,
//		SSI_FONTS, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//#define ROAD_FONTS		&GUI_Fonthz_29
//#define R1B_X	PB1_X+2
//#define R1B_Y	PB1_Y+5
//#define RB_H	30
//#define RN_X	0
//#define RN_Y	0
//#define RN_W	50
//#define RM_X	RN_X + RN_W + 1	//模式
//#define RM_Y	RN_Y
//#define RM_W	50
//#define RS_X	RM_X + RM_W + 1	//状态
//#define RS_Y	RN_Y
//#define RS_W	TP_W - RM_W - RN_W - 4

//#define RV_X	10					//电压
//#define RV_Y	RB_H + 30
//#define RV_W	120	
//#define RC_X	RV_X + RV_W + 20	//电流
//#define RC_Y	RB_H + 30
//#define RC_W	RV_W
//#define RR_X	RC_X				//真实电流
//#define RR_Y	RB_H * 2 + 50
//#define RR_W	RV_W
//#define RT_X	RV_X				//时间
//#define RT_Y	RB_H * 2 + 50
//#define RT_W	95
//	//第一路编号
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RN_X/*x*/,RN_Y/*y*/,RN_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第一路模式
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RM_X/*x*/,RM_Y/*y*/,RM_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第一路状态
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RS_X/*x*/,RS_Y/*y*/,RS_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第一路电压
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RV_X/*x*/,RV_Y/*y*/,RV_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第一路电流
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RC_X/*x*/,RC_Y/*y*/,RC_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第一路真实电流
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RR_X/*x*/,RR_Y/*y*/,RR_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第一路时间
//	{
//		R1B_X/*base_x*/,R1B_Y/*base_y*/,RT_X/*x*/,RT_Y/*y*/,RT_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_RIGHT | GUI_TA_TOP
//	},
//	//第二路编号
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RN_X/*x*/,RN_Y/*y*/,RN_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第二路模式
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RM_X/*x*/,RM_Y/*y*/,RM_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第二路状态
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RS_X/*x*/,RS_Y/*y*/,RS_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第二路电压
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RV_X/*x*/,RV_Y/*y*/,RV_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第二路电流
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RC_X/*x*/,RC_Y/*y*/,RC_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第二路真实电流
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RR_X/*x*/,RR_Y/*y*/,RR_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第二路时间
//	{
//		R2B_X/*base_x*/,R2B_Y/*base_y*/,RT_X/*x*/,RT_Y/*y*/,RT_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_RIGHT | GUI_TA_TOP
//	},
//#define R3B_X	PB3_X+2
//#define R3B_Y	PB3_Y+5

//	//第三路编号
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RN_X/*x*/,RN_Y/*y*/,RN_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第三路模式
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RM_X/*x*/,RM_Y/*y*/,RM_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第三路状态
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RS_X/*x*/,RS_Y/*y*/,RS_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第三路电压
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RV_X/*x*/,RV_Y/*y*/,RV_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第三路电流
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RC_X/*x*/,RC_Y/*y*/,RC_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第三路真实电流
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RR_X/*x*/,RR_Y/*y*/,RR_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第三路时间
//	{
//		R3B_X/*base_x*/,R3B_Y/*base_y*/,RT_X/*x*/,RT_Y/*y*/,RT_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_RIGHT | GUI_TA_TOP
//	},
//#define R4B_X	PB4_X+2
//#define R4B_Y	PB4_Y+5

//	//第四路编号
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RN_X/*x*/,RN_Y/*y*/,RN_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第四路模式
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RM_X/*x*/,RM_Y/*y*/,RM_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第四路状态
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RS_X/*x*/,RS_Y/*y*/,RS_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_CENTER | GUI_TA_TOP
//	},
//	//第四路电压
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RV_X/*x*/,RV_Y/*y*/,RV_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第四路电流
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RC_X/*x*/,RC_Y/*y*/,RC_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第四路真实电流
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RR_X/*x*/,RR_Y/*y*/,RR_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
//	},
//	//第四路时间
//	{
//		R4B_X/*base_x*/,R4B_Y/*base_y*/,RT_X/*x*/,RT_Y/*y*/,RT_W/*width*/,RB_H/*height*/,10,
//		ROAD_FONTS, GUI_WHITE, GUI_INVALID_COLOR,GUI_TA_RIGHT | GUI_TA_TOP
//	},
//};

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
