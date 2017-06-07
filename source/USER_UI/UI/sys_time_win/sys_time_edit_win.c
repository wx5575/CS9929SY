
 /**
   ******************************************************************************
   * @file    file_ui_save.c
   * @author  王鑫
   * @version V0.0.1
   * @date    2017.4.18
   * @brief   文件保存界面
   ******************************************************************************
   */

#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "app.h"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "LISTWHEEL.h"
#include "GUI.h"
#include "cs99xx_mem_api.h"
#include "UI_COM/com_edit_api.h"
#include "UI_COM/com_ui_info.h"
#include "key_menu_win/key_menu_win.h"
#include "7_sys_time_edit_win.h"
#include "sys_time_edit_win.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#define LEFT      (1 << 0)
#define MIDDLE    (1 << 1)
#define RIGHT     (1 << 2)
#define TOP       (1 << 3)
#define BOTTOM    (1 << 4)
#define LIGHTBLUE 0xEE8822
#define GET_MODE  (WM_USER + 0)
#define TRANS0    0x11000000
#define TRANS1    0xEE000000
#define TRANS2    0xFF000000

static void _cbCallback(WM_MESSAGE * pMsg);
static void direct_key_up(int data);
static void direct_key_down(int data);
static void direct_key_left(int data);
static void direct_key_right(int data);
static void menu_key_ok(int hWin);
void init_day_wheel(uint16_t days);
static void sel_cur_time_list_wheel(void);


typedef struct {
    WM_HWIN hWin;
    GUI_MEMDEV_Handle hMemOverlay;
    GUI_MEMDEV_Handle hMemRBorder;
    GUI_MEMDEV_Handle hMemLBorder;
    const GUI_FONT GUI_UNI_PTR * pFont;
    GUI_COLOR font_color_sel;///<字体颜色选中的
    GUI_COLOR font_color_unsel;///<背景颜色未选中的
}WHEEL;

typedef uint32_t (*INIT_FUN)(int);
typedef struct{
    uint8_t *notice[2];
    INIT_FUN init_fun;
    WHEEL wheel;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint32_t size;
    uint32_t num;///<编号
    uint32_t index;///<索引
}SYS_TIME_T;
SYS_TIME_T *g_cur_sys_time;

#define START_YEAR      2014
#define END_YEAR        2050
uint32_t init_listwheel_year_string(int hWin)
{
    uint32_t size = END_YEAR - START_YEAR + 1;
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_year;
    
    this_year = get_rtc_year();
    
    if(END_YEAR >= this_year)
    {
        size = END_YEAR - this_year + 1;
    }
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%4d", this_year + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    if(this_year >= START_YEAR)
    {
        size = this_year - START_YEAR;
    }
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%4d", START_YEAR + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return (END_YEAR - START_YEAR + 1);
}

uint32_t init_listwheel_month_string(int hWin)
{
    uint32_t size = 12;
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_month;
    
    this_month = get_rtc_month();
    size = 12 - this_month + 1;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", this_month + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    for(i = 1; i < this_month; i++)
    {
        sprintf((char*)buf, "%02d", i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return 12;
}

uint16_t get_days_of_month(uint16_t year, uint16_t month)
{
    uint16_t flag = 0;
    uint16_t days = 0;
    uint32_t size[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
                        /*1  2  3  4  5  6  7  8  9 10 11 12*/
    
    /* 闰年 */
    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        flag = 1;
    }
    
    if(month == 2)
    {
        days = size[month] + flag;
    }
    else
    {
        days = size[month];
    }
    
    return days;
}

uint32_t init_listwheel_day_string(int hWin)
{
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_month;
    uint16_t this_year;
    uint16_t this_day;
    uint16_t n = 0;
    uint16_t days = 0;
    
    this_month = get_rtc_month();
    this_year = get_rtc_year();
    this_day = get_rtc_day();
    
    days =  get_days_of_month(this_year, this_month);
    
    n = days - this_day + 1;
    
    for(i = 0; i < n; i++)
    {
        sprintf((char*)buf, "%02d", this_day + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    for(i = 1; i < this_day; i++)
    {
        sprintf((char*)buf, "%02d", i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return days;
}
uint32_t init_listwheel_hour_string(int hWin)
{
    uint32_t size = 24;
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_hour = 0;
    
    this_hour = get_rtc_hour();
    
    size = 24 - this_hour;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", this_hour + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    size = this_hour;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return 24;
}
uint32_t init_listwheel_minute_string(int hWin)
{
    uint32_t size = 60;
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_minute = 0;
    
    this_minute = get_rtc_minute();
    
    size = 60 - this_minute;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", this_minute + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    size = this_minute;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return 60;
}
uint32_t init_listwheel_second_string(int hWin)
{
    uint32_t size = 60;
    int32_t i = 0;
    uint8_t buf[5] = {0};
    uint16_t this_second = 0;
    
    this_second = get_rtc_second();
    
    size = 60 - this_second;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", this_second + i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    size = this_second;
    
    for(i = 0; i < size; i++)
    {
        sprintf((char*)buf, "%02d", i);
        LISTWHEEL_AddString(hWin, (const char*)buf);
    }
    
    return 60;
}

SYS_TIME_T sys_time_t[]=
{
    {{"年","Year"    },init_listwheel_year_string},//年
    {{"月","Month"   },init_listwheel_month_string},//月
    {{"日","Day"     },init_listwheel_day_string},//日
    {{"时","Hour"    },init_listwheel_hour_string},//时
    {{"分","Minute"  },init_listwheel_minute_string},//分
    {{"秒","Second"  },init_listwheel_second_string},//秒
};
enum{
    SYS_T_YEAY,
    SYS_T_MONTH,
    SYS_T_DAY,
    SYS_T_HOUR,
    SYS_T_MINUTE,
    SYS_T_SECOND,
};

static WIDGET_POS_SIZE_T* sys_time_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_sys_time_windows,/*4.3寸屏*/
    &_7_sys_time_windows,/*5.6寸屏*/
    &_7_sys_time_windows,/*7寸屏*/
};

 MENU_KEY_INFO_T 	sys_time_key_info[] =
{
    {"" , F_KEY_NULL    , KEY_F1 & _KEY_UP, 0 },//f1
    {"" , F_KEY_NULL    , KEY_F2 & _KEY_UP, 0 },//f2
    {"" , F_KEY_NULL    , KEY_F3 & _KEY_UP, 0 },//f3
    {"" , F_KEY_NULL    , KEY_F4 & _KEY_UP, 0 },//f4
    {"" , F_KEY_OK      , KEY_F5 & _KEY_UP, menu_key_ok },//f5 
    {"" , F_KEY_BACK    , KEY_F6 & _KEY_UP, back_win},//f6
};

static FUNCTION_KEY_INFO_T syst_time_key_pool[]={
	{KEY_UP		, direct_key_up     },
	{KEY_DOWN	, direct_key_down   },
	{KEY_LEFT	, direct_key_left   },
	{KEY_RIGHT	, direct_key_right  },
    
	{KEY_UP     & _KEY_LONG, direct_key_up     },
	{KEY_DOWN	& _KEY_LONG, direct_key_down   },
	{KEY_LEFT	& _KEY_LONG, direct_key_left   },
	{KEY_RIGHT	& _KEY_LONG, direct_key_right  },
    
	{CODE_LEFT	, direct_key_down   },
	{CODE_RIGH	, direct_key_up     },
};

void env_langulag_sys_key(int data)
{
     register_system_key_fun(syst_time_key_pool, ARRAY_SIZE(syst_time_key_pool), data);
}

static void update_menu_key_inf(WM_HMEM hWin)
{
    init_menu_key_info(sys_time_key_info, ARRAY_SIZE(sys_time_key_info), hWin);//刷新菜单键显示
    register_system_key_fun(syst_time_key_pool, ARRAY_SIZE(syst_time_key_pool), hWin);
}

static CS_INDEX com_ele_table[]=
{
	ELE_RANGE_NAME,///<主界面的通信状态
	ELE_RANGE_NOTICE,///<主界面的系统时间
};
static MYUSER_WINDOW_T sys_time_window=
{
    {"系统时间","System Time"},
    _cbCallback,update_menu_key_inf,
    {0},
    {0},
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)com_ele_table,ARRAY_SIZE(com_ele_table)
    },
};

static uint32_t get_listwheel_value(SYS_TIME_T *inf)
{
    WM_HMEM hWin;
    uint32_t index;
    uint32_t value = 0;
    uint8_t buf[10] = {0};
    
    hWin = inf->wheel.hWin;
    
    if(hWin != 0)
    {
        index = LISTWHEEL_GetSel(hWin);
        LISTWHEEL_GetItemText(hWin, index, (char*)buf, 10);
        value = atoi((const char*)buf);
    }
    
    return value;
}

static void set_sys_time(void)
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    
    year = get_listwheel_value(&sys_time_t[SYS_T_YEAY]);
    month = get_listwheel_value(&sys_time_t[SYS_T_MONTH]);
    day = get_listwheel_value(&sys_time_t[SYS_T_DAY]);
    
    hour = get_listwheel_value(&sys_time_t[SYS_T_HOUR]);
    minute = get_listwheel_value(&sys_time_t[SYS_T_MINUTE]);
    second = get_listwheel_value(&sys_time_t[SYS_T_SECOND]);
    
    rtc_set_time(year, month, day, hour, minute, second);
    
}
static void menu_key_ok(int hWin)
{
    set_sys_time();
//    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
//    save_sys_par();//保存系统参数
//    unregister_system_key_fun(syst_time_key_pool, ARRAY_SIZE(syst_time_key_pool));
//    back_win(hWin);//关闭对话框
}

static void check_day_legality(void)
{
    uint32_t year;
    uint32_t month;
    uint32_t days;
    
    year = get_listwheel_value(&sys_time_t[SYS_T_YEAY]);
    month = get_listwheel_value(&sys_time_t[SYS_T_MONTH]);
    
    days = get_days_of_month(year, month);
    init_day_wheel(days);
}
static void direct_key_up(int data)
{
    int32_t i = 0;
    uint32_t size = 0;
    WM_HMEM hWin;
    
    hWin = g_cur_sys_time->wheel.hWin;
    
    if(hWin == 0)
    {
        return;
    }
    
    size = g_cur_sys_time->size;
    i = LISTWHEEL_GetSel(hWin);
    
    if(i == 0)
    {
        i = size - 1;
    }
    else
    {
        i--;
    }
    
    LISTWHEEL_SetSel(hWin, i);
    LISTWHEEL_MoveToPos(hWin, i);
    g_cur_sys_time->index = i;
    
    /* 如果当前设置的是年月就检查天的合法性 */
    if(g_cur_sys_time == &sys_time_t[SYS_T_MONTH]
        || g_cur_sys_time == &sys_time_t[SYS_T_YEAY])
    {
        check_day_legality();
    }
}

static void direct_key_down(int data)
{
    int32_t i = 0;
    WM_HMEM hWin;
    uint32_t size = 0;
    
    hWin = g_cur_sys_time->wheel.hWin;
    size = g_cur_sys_time->size;
    i = LISTWHEEL_GetSel(hWin);
    
    if(i >= size - 1)
    {
        i = 0;
    }
    else
    {
        i++;
    }
    
    LISTWHEEL_SetSel(hWin, i);
    LISTWHEEL_MoveToPos(hWin, i);
    g_cur_sys_time->index = i;
    
    /* 如果当前设置的是年月就检查天的合法性 */
    if(g_cur_sys_time == &sys_time_t[SYS_T_MONTH]
        || g_cur_sys_time == &sys_time_t[SYS_T_YEAY])
    {
        check_day_legality();
    }
}

static void sel_cur_time_list_wheel(void)
{
    WM_HMEM hWin;
    GUI_COLOR color;
    
    check_day_legality();
    
    hWin = g_cur_sys_time->wheel.hWin;
    color = g_cur_sys_time->wheel.font_color_sel;
    
    LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, color);
    
    set_com_text_ele_inf(ELE_RANGE_NOTICE, g_cur_win, g_cur_sys_time->notice);
    update_com_text_ele(ELE_RANGE_NOTICE, g_cur_win, g_cur_sys_time->notice[SYS_LANGUAGE]);
}
static void unsel_cur_time_list_wheel(void)
{
    WM_HMEM hWin;
    GUI_COLOR color;
    
    hWin = g_cur_sys_time->wheel.hWin;
    color = g_cur_sys_time->wheel.font_color_unsel;
    
    LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, color);
}
static void direct_key_left(int data)
{
    uint32_t size = 0;
    
    size = g_cur_sys_time->num;
    
    if(size > 0)
    {
        unsel_cur_time_list_wheel();
        g_cur_sys_time--;
        sel_cur_time_list_wheel();
    }
}

static void direct_key_right(int data)
{
    uint32_t size = 0;
    
    size = g_cur_sys_time->num;
    
    if(size < 5)
    {
        unsel_cur_time_list_wheel();
        g_cur_sys_time++;
        sel_cur_time_list_wheel();
    }
}


#define TRANS0    0x11000000
#define TRANS1    0xEE000000
#define TRANS2    0xFF000000
#define LIGHTBLUE 0xEE8822
static void _CreateDecoration(int xSize, int ySize, int LineHeight, WHEEL * pWheel)
{
    GUI_MEMDEV_Handle hMemPrev;
    GUI_MEMDEV_Handle hMemRBorder;
    GUI_MEMDEV_Handle hMemLBorder;
    GUI_MEMDEV_Handle hMemOverlay;
    
    //
    // Create left border
    //
    hMemLBorder = GUI_MEMDEV_CreateFixed(0, 0, 4, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
    hMemPrev = GUI_MEMDEV_Select(hMemLBorder);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawVLine(0, 0, ySize - 1);
    GUI_SetColor(0x00CCCCCC);
    GUI_FillRect(1, 0, 3, ySize - 1);
    //
    // Create right border
    //
    hMemRBorder = GUI_MEMDEV_CreateFixed(0, 0, 4, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
    GUI_MEMDEV_Select(hMemRBorder);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawVLine(3, 0, ySize - 1);
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_FillRect(0, 0, 2, ySize - 1);
    //
    // Create overlay device
    //
    hMemOverlay = GUI_MEMDEV_CreateFixed(0, 0, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
    GUI_MEMDEV_Select(hMemOverlay);
    //
    // Gray gradients at top
    //
    GUI_DrawGradientV(0, 0, xSize - 1, (ySize * 2) / 10 - 1, TRANS0, TRANS1);
    GUI_DrawGradientV(0, (ySize * 2) / 10, xSize - 1,
                    (ySize / 2) - (LineHeight / 2) - 1, TRANS1, TRANS2);
    //
    // Gray gradients at bottom
    //
    GUI_DrawGradientV(0, (ySize / 2) + (LineHeight / 2),
                xSize - 1, (ySize / 2) + (LineHeight / 2) + 2, 0x88000000, TRANS2);
    GUI_DrawGradientV(0, (ySize / 2) + (LineHeight / 2) + 3,
                xSize - 1, ySize - (ySize * 2) / 10 - 1, TRANS2, TRANS1);
    GUI_DrawGradientV(0, ySize - (ySize * 2) / 10, xSize - 1, ySize - 1,                          TRANS1,     TRANS0);
    GUI_DrawGradientV(0, ySize - 1, xSize - 1, ySize - 1, 0x33000000, 0x33000000);
    //
    // Translucent glass effect in the middle
    //
    GUI_SetColor(0x80000000);
    GUI_DrawHLine((ySize / 2) - (LineHeight / 2), 0, xSize - 1);
    GUI_SetColor(0x44000000);
    GUI_DrawHLine((ySize / 2) + (LineHeight / 2) - 1, 0, xSize - 1);
    GUI_DrawGradientV(1, (ySize / 2) - (LineHeight / 2) + 1, xSize - 2, (ySize / 2) - 1,
                        0x88FFFFFF, 0x55AAAAAA);
    GUI_DrawGradientV(1, (ySize / 2),
                xSize - 2, (ySize / 2) + (LineHeight / 2) - 2, 0xBB000000, 0xBB000000);
    GUI_MEMDEV_Select(hMemPrev);
    //
    // Store result
    //
    pWheel->hMemLBorder = hMemLBorder;
    pWheel->hMemRBorder = hMemRBorder;
    pWheel->hMemOverlay = hMemOverlay;
}

static int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    WM_HWIN   hWin;
    WHEEL   * pWheel;
    int       xPos;
    int       yPos;
    int       xSizeWin;
    int       ySizeWin;
    int       xSizeDev;
    
    hWin = pDrawItemInfo->hWin;
    
    if(hWin == 0)
    {
        return 0;
    }
    
    //
    // Process messages
    //
    switch (pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_GET_XSIZE:
            //
            // Return x-size of item
            //
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
        case WIDGET_ITEM_GET_YSIZE:
            //
            // Return y-size of item
            //
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
        case WIDGET_ITEM_DRAW:
            //
            // Draw item
            //
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
        case WIDGET_ITEM_DRAW_BACKGROUND:
            xSizeWin = WM_GetWindowSizeX(hWin);
            ySizeWin = WM_GetWindowSizeY(hWin);
            //
            // Draw background
            //
            GUI_SetColor(0x00FFFFFF);
            GUI_FillRect(4, 0, xSizeWin - 5, ySizeWin - 1);
            break;
        case WIDGET_ITEM_DRAW_OVERLAY:
            LISTWHEEL_GetUserData(hWin, &pWheel, sizeof(pWheel));
            xPos     = WM_GetWindowOrgX(hWin);
            yPos     = WM_GetWindowOrgY(hWin);
            xSizeWin = WM_GetWindowSizeX(hWin);
            xSizeDev = GUI_MEMDEV_GetXSize(pWheel->hMemRBorder);
            //
            // Draw border
            //
            GUI_MEMDEV_WriteAt(pWheel->hMemLBorder, xPos, yPos);
            GUI_MEMDEV_WriteAt(pWheel->hMemRBorder, xPos + xSizeWin - xSizeDev, yPos);
            //
            // Draw overlay
            //
            GUI_MEMDEV_WriteAt(pWheel->hMemOverlay, xPos, yPos);
            break;
    }
    return 0;
}

static int _CreateListWheel(int x, int y, int xSize, int ySize,
                            int Id, char ** apText, int NumItems, int TextAlign,
                            WM_HWIN hParent, WHEEL * pWheel)
{
    const GUI_FONT GUI_UNI_PTR * pFont;
    WM_HWIN hWin;
    int i;
    int LineHeight;
    
    //
    // Create LISTWHEEL object
    //
    pFont         = &GUI_Fonthz_24;
    LineHeight    = 30;
    pWheel->pFont = pFont;
    pWheel->font_color_sel = GUI_RED;
    pWheel->font_color_unsel = LIGHTBLUE;
    hWin = LISTWHEEL_CreateUser(x, y, xSize, ySize, hParent, WM_CF_SHOW | WM_CF_HASTRANS,
                                0, Id, NULL, sizeof(void *));
    
    if(hWin == 0)
    {
        return 0;
    }
    
    LISTWHEEL_SetFont(hWin, &GUI_Fonthz_24);
    LISTWHEEL_SetTextAlign(hWin, TextAlign);
    LISTWHEEL_SetSnapPosition(hWin, (ySize - LineHeight) / 2);
    LISTWHEEL_SetOwnerDraw(hWin, _OwnerDraw);
    LISTWHEEL_SetUserData(hWin, &pWheel, sizeof(pWheel));
    LISTWHEEL_SetLineHeight(hWin, LineHeight);
    LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, pWheel->font_color_unsel);
    
    for (i = 0; i < NumItems; i++)
    {
        LISTWHEEL_AddString(hWin, *(apText + i));
    }
    
    if (TextAlign & GUI_TA_RIGHT)
    {
        LISTWHEEL_SetRBorder(hWin, 10);
    }
    
    //
    // Create overlay devices
    //
    _CreateDecoration(xSize, ySize, LineHeight, pWheel);
    //
    // Fill WHEEL structure
    //
    pWheel->hWin = hWin;
    return 0;
}


void init_day_wheel(uint16_t days)
{
    WM_HMEM hWin;
    uint32_t n = 0;
    uint8_t buf[10] = {0};
    int32_t i = 0;
    uint16_t day;
    uint16_t index;
    uint32_t size;
    
    hWin = sys_time_t[SYS_T_DAY].wheel.hWin;
    
    if(hWin == 0)
    {
        return;
    }
    
    n = sys_time_t[SYS_T_DAY].size;
    
    index = LISTWHEEL_GetSel(hWin);
    LISTWHEEL_GetItemText(hWin, index, (char*)buf, 10);
    day = atoi((const char*)buf);
    
    if(day > days)
    {
        day = 1;
    }
    
    if(n != days)
    {
        SYS_TIME_T *inf = &sys_time_t[SYS_T_DAY];
        
        WM_DeleteWindow(hWin);
        hWin = g_cur_win->handle;
        _CreateListWheel(inf->x, inf->y, inf->w, inf->h, id_base++, NULL, 0,
                GUI_TA_VCENTER | GUI_TA_HCENTER, hWin, &inf->wheel);
        hWin = sys_time_t[SYS_T_DAY].wheel.hWin;
        
        size = days - day + 1;
        
        for(i = 0; i < size; i++)
        {
            sprintf((char*)buf, "%02d", day + i);
            LISTWHEEL_AddString(hWin, (const char*)buf);
        }
        
        size = day;
        
        for(i = 1; i < size; i++)
        {
            sprintf((char*)buf, "%02d", i);
            LISTWHEEL_AddString(hWin, (const char*)buf);
        }
        
        LISTWHEEL_SetSel(hWin, 0);
        inf->size = days;
    }
}

static void create_listwheel(SYS_TIME_T *inf, WM_HWIN hWin, int32_t index)
{
    _CreateListWheel(inf->x, inf->y, inf->w, inf->h, id_base++, NULL, 0,
            GUI_TA_VCENTER | GUI_TA_HCENTER, hWin, &inf->wheel);
    inf->num = index;
    inf->size = inf->init_fun(inf->wheel.hWin);
}
static void init_time_list_wheel_pos_size(void)
{
    SYS_TIME_T *inf;
    uint16_t i = 0;
    uint16_t n = ARRAY_SIZE(sys_time_t);
    uint16_t t_w[6] = {80,50,50,50,50,50};
    uint16_t x = 5;
    uint16_t y = 60;
    uint16_t h = 150;
    uint32_t w_sum = 0;
    uint32_t offset = 20;
    
    for(i = 0; i < n; i++)
    {
        inf = &sys_time_t[i];
        inf->x = x + w_sum + offset;
        inf->y = y;
        inf->w = t_w[i];
        inf->h = h;
        w_sum += inf->w;
        
        if(i == 2)
        {
            w_sum += 20;
        }
    }
}

static void _RoundUpCorners(GUI_MEMDEV_Handle hMem, int Side, int r, int vMask)
{
    GUI_MEMDEV_Handle hMemPrev;
    I32 rr;
    int i;
    int y;
    int x;
    int xs;
    int xe;
    int xSize;
    int ySize;
    int x1;
    int y1;

    xSize    = GUI_MEMDEV_GetXSize(hMem);
    ySize    = GUI_MEMDEV_GetYSize(hMem);
    hMemPrev = GUI_MEMDEV_Select(hMem);
    x1       = xSize - 1;
    y1       = ySize - 1;
    rr       = (I32)r * r;
    y        = r;
    GUI_SetColor(0xFF000000);
    
    for (i = 0; i <= r; i++, y--)
    {
        x = (int)sqrt((float)(rr - (I32)y * y));
        
        if (Side & LEFT)
        {
            xs = 0;
            xe = r - x - 1;
            
            if (x != r)
            {
                if (vMask & TOP)
                {
                    GUI_DrawHLine(i, xs, xe);
                }
                
                if (vMask & BOTTOM)
                {
                    GUI_DrawHLine(y1 - i, xs, xe);
                }
            }
        }
        
        if (Side & RIGHT)
        {
            xs = x1 - r + x + 1;
            xe = x1;
            
            if (x != r)
            {
                if (vMask & TOP)
                {
                    GUI_DrawHLine(i,      xs, xe);
                }
                
                if (vMask & BOTTOM)
                {
                    GUI_DrawHLine(y1 - i, xs, xe);
                }
            }
        }
    }
    
    GUI_MEMDEV_Select(hMemPrev);
}
void init_time_listwheel(WM_HWIN hWin)
{
    uint16_t i = 0;
    uint16_t n = ARRAY_SIZE(sys_time_t);
    
    init_time_list_wheel_pos_size();
    
    for(i = 0; i < 3; i++)
    {
        create_listwheel(&sys_time_t[i], hWin, i);
    }
    
    _RoundUpCorners(sys_time_t[0].wheel.hMemOverlay, LEFT,  4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[0].wheel.hMemLBorder, LEFT,  4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[2].wheel.hMemOverlay, RIGHT, 4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[2].wheel.hMemRBorder, RIGHT, 4, TOP | BOTTOM);
    
    for(i = 3; i < n; i++)
    {
        create_listwheel(&sys_time_t[i], hWin, i);
    }
    
    _RoundUpCorners(sys_time_t[3].wheel.hMemOverlay, LEFT,  4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[3].wheel.hMemRBorder, LEFT,  4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[5].wheel.hMemOverlay, RIGHT, 4, TOP | BOTTOM);
    _RoundUpCorners(sys_time_t[5].wheel.hMemRBorder, RIGHT, 4, TOP | BOTTOM);
    
    g_cur_sys_time = &sys_time_t[0];
}

void del_all_mem_device(void)
{
    uint16_t i = 0;
    uint16_t n = ARRAY_SIZE(sys_time_t);
    
    for(i = 0; i < n; i++)
    {
        GUI_MEMDEV_Delete(sys_time_t[i].wheel.hMemOverlay);
        GUI_MEMDEV_Delete(sys_time_t[i].wheel.hMemLBorder);
        GUI_MEMDEV_Delete(sys_time_t[i].wheel.hMemRBorder);
        
        sys_time_t[i].wheel.hMemOverlay = 0;
        sys_time_t[i].wheel.hMemLBorder = 0;
        sys_time_t[i].wheel.hMemRBorder = 0;
    }
}
static void init_com_text_ele_dis_inf(WM_HWIN hWin)
{
	MYUSER_WINDOW_T* win;
    UI_ELE_DISPLAY_INFO_T dis_info=
    {
        0/*base_x*/,0/*base_y*/,0/*x*/,200/*y*/,10/*width*/,30/*height*/,10,
        &GUI_Fonthz_20, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
    };
    
    win = get_user_window_info(hWin);
    
    dis_info.pos_size.x = 10;
    dis_info.pos_size.y = win->pos_size.height - 45;
    dis_info.pos_size.width = 70;
    dis_info.pos_size.height = 45;
    dis_info.max_len = 100;
    dis_info.font[CHINESE] = &GUI_Fonthz_20;
    dis_info.font_color = GUI_BLACK;
    dis_info.back_color = GUI_INVALID_COLOR;
    dis_info.align = GUI_TA_LEFT;
    
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NAME);//范围
    dis_info.pos_size.x += dis_info.pos_size.width;
    dis_info.pos_size.width = win->pos_size.width - 15 -  dis_info.pos_size.width;
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NOTICE);//提示信息
}

/**
  * @brief  回调函数
  * @param  [in] pMsg 窗口消息指针
  * @retval 无
  */
static void _cbCallback(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
        case WM_NOTIFY_PARENT:
            break;
        case WM_PAINT:
            break;
		case CM_EDIT_WIDGET_CHANGE:
		{
            update_menu_key_inf(hWin);
			break;
		}
        case WM_INIT_DIALOG:
        {
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            
            init_time_listwheel(hWin);
            update_menu_key_inf(hWin);
            init_window_edit_ele(win);
            init_com_text_ele_dis_inf(hWin);//初始化公共文本对象的显示信息
            init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
            {
                uint8_t *buf[2] = {"提示:"  ,"Notice:"};
                update_com_text_ele(ELE_RANGE_NAME, g_cur_win, buf[SYS_LANGUAGE]);
            }
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            sel_cur_time_list_wheel();
            break;
        }
		case WM_DELETE:
		{
            del_all_mem_device();
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
        case WM_KEY:
        {
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
        }
        default:
            WM_DefaultProc(pMsg);
    }
}
/**
  * @brief  创建对话框用来保存文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_sys_time_dialog(int hWin)
{
    init_window_size(&sys_time_window, sys_time_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&sys_time_window, &windows_list, hWin);
}



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
