
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
/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  系统时间控件索引枚举定义
  */
typedef enum{
    SYS_T_YEAY,
    SYS_T_MONTH,
    SYS_T_DAY,
    SYS_T_HOUR,
    SYS_T_MINUTE,
    SYS_T_SECOND,
}SYS_TIME_NIDEX;

/**
  * @brief  wheel控件结构定义
  */
typedef struct {
    WM_HWIN hWin;///<wheel句柄
    GUI_MEMDEV_Handle hMemOverlay;///<wheel覆盖层句柄
    GUI_MEMDEV_Handle hMemRBorder;///<wheel右border句柄
    GUI_MEMDEV_Handle hMemLBorder;///<wheel左border句柄
    const GUI_FONT GUI_UNI_PTR * pFont;///<字体
    GUI_COLOR font_color_sel;///<字体颜色选中的
    GUI_COLOR font_color_unsel;///<背景颜色未选中的
}WHEEL;
/**
  * @brief  时间设置控件结构定义
  */
typedef struct{
    uint8_t *notice[2];///<提示
    uint32_t (*init_fun)(int);///<初始化函数
    WHEEL wheel;///<继承wheel
    uint16_t x;///<x坐标
    uint16_t y;///<y坐标
    uint16_t w;///<wheel宽度
    uint16_t h;///<wheel高度
    uint32_t size;///<wheel包含的标签个数
    uint32_t num;///<编号
    uint32_t index;///<索引
}SYS_TIME_T;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
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

#define START_YEAR      2014 ///<年设置开始年份
#define END_YEAR        2050 ///<年设置结束年份
/* Private function prototypes -----------------------------------------------*/


static void sys_time_edit_win_cb(WM_MESSAGE * pMsg);
static void sys_time_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void sys_time_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void sys_time_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void sys_time_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void check_day_legality(void);
static void init_sys_time_win_fun_key(WM_HMEM hWin);
static uint32_t init_listwheel_second_string(int hWin);
static uint32_t init_listwheel_minute_string(int hWin);
static uint32_t init_listwheel_hour_string(int hWin);
static uint32_t init_listwheel_day_string(int hWin);
static uint32_t init_listwheel_month_string(int hWin);
static uint32_t init_listwheel_year_string(int hWin);

static void sys_time_f1_cb(KEY_MESSAGE *key_msg);
static void sys_time_f2_cb(KEY_MESSAGE *key_msg);
static void sys_time_f3_cb(KEY_MESSAGE *key_msg);
static void sys_time_f4_cb(KEY_MESSAGE *key_msg);
static void sys_time_f5_cb(KEY_MESSAGE *key_msg);
static void sys_time_f6_cb(KEY_MESSAGE *key_msg);

static void sys_time_menu_key_ok(int hWin);
static void init_day_wheel(uint16_t days);
static void sel_cur_time_list_wheel(void);
static void unsel_cur_time_list_wheel(void);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  当前时间设置控件结构指针
  */
static SYS_TIME_T *g_cur_sys_time;

/**
  * @brief  系统时间窗口结构
  */
static MYUSER_WINDOW_T sys_time_window=
{
    {"系统时间","System Time"},
    sys_time_edit_win_cb,init_sys_time_win_fun_key,
    {0},
    {0},
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table)
    },
};
/**
  * @brief  系统窗口与屏幕尺寸位置相关的数组
  */
static WIDGET_POS_SIZE_T* sys_time_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_sys_time_windows,/*4.3寸屏*/
    &_7_sys_time_windows,/*5.6寸屏*/
    &_7_sys_time_windows,/*7寸屏*/
};

/**
  * @brief  系统时间设置控件数组
  */
static SYS_TIME_T sys_time_t[]=
{
    {{"年","Year"    },init_listwheel_year_string},//年
    {{"月","Month"   },init_listwheel_month_string},//月
    {{"日","Day"     },init_listwheel_day_string},//日
    {{"时","Hour"    },init_listwheel_hour_string},//时
    {{"分","Minute"  },init_listwheel_minute_string},//分
    {{"秒","Second"  },init_listwheel_second_string},//秒
};

/**
  * @brief  设置系统时间使用的菜单键初始化数组
  */
static MENU_KEY_INFO_T 	sys_time_key_info[] =
{
    {"" , F_KEY_NULL    , KEY_F1 & _KEY_UP, sys_time_f1_cb },//f1
    {"" , F_KEY_NULL    , KEY_F2 & _KEY_UP, sys_time_f2_cb },//f2
    {"" , F_KEY_NULL    , KEY_F3 & _KEY_UP, sys_time_f3_cb },//f3
    {"" , F_KEY_NULL    , KEY_F4 & _KEY_UP, sys_time_f4_cb },//f4
    {"" , F_KEY_OK      , KEY_F5 & _KEY_UP, sys_time_f5_cb },//f5
    {"" , F_KEY_BACK    , KEY_F6 & _KEY_UP, sys_time_f6_cb },//f6
};
/**
  * @brief  系统功能按键信息初始化数组
  */
static FUNCTION_KEY_INFO_T syst_time_key_pool[]={
	{KEY_UP		, sys_time_direct_key_up_cb     },
	{KEY_DOWN	, sys_time_direct_key_down_cb   },
	{KEY_LEFT	, sys_time_direct_key_left_cb   },
	{KEY_RIGHT	, sys_time_direct_key_right_cb  },
    
	{KEY_UP     & _KEY_LONG, sys_time_direct_key_up_cb     },
	{KEY_DOWN	& _KEY_LONG, sys_time_direct_key_down_cb   },
	{KEY_LEFT	& _KEY_LONG, sys_time_direct_key_left_cb   },
	{KEY_RIGHT	& _KEY_LONG, sys_time_direct_key_right_cb  },
    
	{CODE_LEFT	, sys_time_direct_key_down_cb   },
	{CODE_RIGH	, sys_time_direct_key_up_cb     },
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  菜单键f1的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f2的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f3的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f4的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f5的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f5_cb(KEY_MESSAGE *key_msg)
{
    sys_time_menu_key_ok(key_msg->user_data);
}
/**
  * @brief  菜单键f6的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}


/**
  * @brief  向上键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_direct_key_up_cb(KEY_MESSAGE *key_msg)
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

/**
  * @brief  向下键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_direct_key_down_cb(KEY_MESSAGE *key_msg)
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
/**
  * @brief  向左键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_direct_key_left_cb(KEY_MESSAGE *key_msg)
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
/**
  * @brief  向右键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_time_direct_key_right_cb(KEY_MESSAGE *key_msg)
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

/**
  * @brief  更新系统功能键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void init_sys_time_win_fun_key(WM_HMEM hWin)
{
    init_menu_key_info(sys_time_key_info, ARRAY_SIZE(sys_time_key_info), hWin);//刷新菜单键显示
    register_system_key_fun(syst_time_key_pool, ARRAY_SIZE(syst_time_key_pool), hWin);
}

/**
  * @brief  初始化年listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了年的个数
  */
static uint32_t init_listwheel_year_string(int hWin)
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

/**
  * @brief  初始化月listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了月的个数
  */
static uint32_t init_listwheel_month_string(int hWin)
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

/**
  * @brief  获取月的天数，根据年月确定月的天数
  * @param  [in] year 年
  * @param  [in] month 月
  * @retval 月的天数
  */
static uint16_t get_days_of_month(uint16_t year, uint16_t month)
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

/**
  * @brief  初始化日listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了日的个数
  */
static uint32_t init_listwheel_day_string(int hWin)
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
/**
  * @brief  初始化时listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了时的个数
  */
static uint32_t init_listwheel_hour_string(int hWin)
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
/**
  * @brief  初始化分listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了分的个数
  */
static uint32_t init_listwheel_minute_string(int hWin)
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
/**
  * @brief  初始化秒listwheel控件
  * @param  [in] hWin 窗口句柄
  * @retval 包含了秒的个数
  */
static uint32_t init_listwheel_second_string(int hWin)
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



/**
  * @brief  获取listwheel控件的值
  * @param  [in] inf 系统时间控件结构信息
  * @retval 控件内容转换的值
  */
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

/**
  * @brief  设置系统时间
  * @param  无
  * @retval 无
  */
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
/**
  * @brief  按下确认键后设置系统时间
  * @param  [in] 窗口句柄
  * @retval 无
  */
static void sys_time_menu_key_ok(int hWin)
{
    set_sys_time();
}
/**
  * @brief  检查天的合法性，根据年和月来判断天是否是正确的，不正确进行修正
  * @param  无
  * @retval 无
  */
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
/**
  * @brief  选择当前的时间设置控件
  * @param  无
  * @retval 无
  */
static void sel_cur_time_list_wheel(void)
{
    WM_HMEM hWin;
    GUI_COLOR color;
    
    check_day_legality();//检察天的合法性
    
    hWin = g_cur_sys_time->wheel.hWin;
    color = g_cur_sys_time->wheel.font_color_sel;
    
    LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, color);
    
    set_com_text_ele_inf(COM_RANGE_NOTICE, g_cur_win, g_cur_sys_time->notice);
    update_com_text_ele(COM_RANGE_NOTICE, g_cur_win, g_cur_sys_time->notice[SYS_LANGUAGE]);
}
/**
  * @brief  取消选择当前的时间设置控件
  * @param  无
  * @retval 无
  */
static void unsel_cur_time_list_wheel(void)
{
    WM_HMEM hWin;
    GUI_COLOR color;
    
    hWin = g_cur_sys_time->wheel.hWin;
    color = g_cur_sys_time->wheel.font_color_unsel;
    
    LISTWHEEL_SetTextColor(hWin, LISTWHEEL_CI_SEL, color);
}
/**
  * @brief  给wheel控件创建蒙板
  * @param  [in] xSize 宽
  * @param  [in] ySize 高
  * @param  [in] LineHeight 行高
  * @param  [in] wheel 控件结构
  * @retval 无
  */
static void create_decoration(int xSize, int ySize, int LineHeight, WHEEL * pWheel)
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

/**
  * @brief  给wheel控件创建蒙板
  * @param  [in] pDrawItemInfo 给emwin提供的自绘函数
  * @retval int 
  */
static int owner_draw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
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

/**
  * @brief  创建listwheel
  * @param  [in] x 坐标
  * @param  [in] y 坐标
  * @param  [in] xSize 宽度
  * @param  [in] ySize 高度
  * @param  [in] Id 创建时提供的ID值
  * @param  [in] apText 创建时提供的标签数组
  * @param  [in] NumItems 创建时提供的标签数组中标签个数
  * @param  [in] TextAlign 文本对齐方式
  * @param  [in] hParent 父窗口句柄
  * @param  [in] pWheel wheel结构
  * @retval 无
  */
static void create_list_wheel(int x, int y, int xSize, int ySize,
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
        return;
    }
    
    LISTWHEEL_SetFont(hWin, &GUI_Fonthz_24);
    LISTWHEEL_SetTextAlign(hWin, TextAlign);
    LISTWHEEL_SetSnapPosition(hWin, (ySize - LineHeight) / 2);
    LISTWHEEL_SetOwnerDraw(hWin, owner_draw);
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
    create_decoration(xSize, ySize, LineHeight, pWheel);
    //
    // Fill WHEEL structure
    //
    pWheel->hWin = hWin;
}


/**
  * @brief  初始化天的wheel控件
  * @param  [in] days 天数
  * @retval 无
  */
static void init_day_wheel(uint16_t days)
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
        create_list_wheel(inf->x, inf->y, inf->w, inf->h, id_base++, NULL, 0,
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
/**
  * @brief  创建系统时间listwheel控件
  * @param  [in] inf 系统时间结构
  * @param  [in] hWin 窗口句柄
  * @param  [in] index inf在数组中的索引
  * @retval 无
  */
static void create_sys_time_listwheel(SYS_TIME_T *inf, WM_HWIN hWin, int32_t index)
{
    create_list_wheel(inf->x, inf->y, inf->w, inf->h, id_base++, NULL, 0,
            GUI_TA_VCENTER | GUI_TA_HCENTER, hWin, &inf->wheel);
    inf->num = index;
    inf->size = inf->init_fun(inf->wheel.hWin);
}
/**
  * @brief  初始化系统时间listwheel控件的位置时间信息
  * @param  无
  * @retval 无
  */
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
/**
  * @brief  对listwheel控件进行圆角处理
  * @param  hMem 内存设备句柄
  * @param  Side 左右的标记 LEFT RIGHT
  * @param  r 圆角半径
  * @param  vMask 上下标记 TOP BOTTOM
  * @retval 无
  */
static void round_up_corners(GUI_MEMDEV_Handle hMem, int Side, int r, int vMask)
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
/**
  * @brief  初始化创建所有的listwheel控件
  * @param  hWin 窗口句柄
  * @retval 无
  */
static void init_create_time_listwheel(WM_HWIN hWin)
{
    uint16_t i = 0;
    uint16_t n = ARRAY_SIZE(sys_time_t);
    
    init_time_list_wheel_pos_size();
    
    for(i = 0; i < 3; i++)
    {
        create_sys_time_listwheel(&sys_time_t[i], hWin, i);
    }
    
    round_up_corners(sys_time_t[0].wheel.hMemOverlay, LEFT,  4, TOP | BOTTOM);
    round_up_corners(sys_time_t[0].wheel.hMemLBorder, LEFT,  4, TOP | BOTTOM);
    round_up_corners(sys_time_t[2].wheel.hMemOverlay, RIGHT, 4, TOP | BOTTOM);
    round_up_corners(sys_time_t[2].wheel.hMemRBorder, RIGHT, 4, TOP | BOTTOM);
    
    for(i = 3; i < n; i++)
    {
        create_sys_time_listwheel(&sys_time_t[i], hWin, i);
    }
    
    round_up_corners(sys_time_t[3].wheel.hMemOverlay, LEFT,  4, TOP | BOTTOM);
    round_up_corners(sys_time_t[3].wheel.hMemRBorder, LEFT,  4, TOP | BOTTOM);
    round_up_corners(sys_time_t[5].wheel.hMemOverlay, RIGHT, 4, TOP | BOTTOM);
    round_up_corners(sys_time_t[5].wheel.hMemRBorder, RIGHT, 4, TOP | BOTTOM);
    
    g_cur_sys_time = &sys_time_t[0];
}
/**
  * @brief  删除所有的listwheel控件，如果不删除就会造成内存泄漏
  * @param  无
  * @retval 无
  */
static void del_all_mem_device(void)
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
/**
  * @brief  窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void sys_time_edit_win_cb(WM_MESSAGE * pMsg)
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
            init_sys_time_win_fun_key(hWin);
			break;
		}
        case WM_INIT_DIALOG:
        {
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            
            init_create_time_listwheel(hWin);
            init_sys_time_win_fun_key(hWin);
            init_window_edit_ele(win);
            init_window_com_ele_list(win);//初始化窗口文本对象链表
            init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
            init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
            {
                uint8_t *buf[2] = {"提示:"  ,"Notice:"};
                update_com_text_ele(COM_RANGE_NAME, g_cur_win, buf[SYS_LANGUAGE]);
            }
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            sel_cur_time_list_wheel();
            break;
        }
		case WM_DELETE:
		{
            del_all_mem_device();//这里必须删除内存设备，否则内存泄漏
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
/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建对话框用来设置系统时间
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_sys_time_dialog(int hWin)
{
    init_window_size(&sys_time_window, sys_time_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&sys_time_window, &windows_list, hWin);
}



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
