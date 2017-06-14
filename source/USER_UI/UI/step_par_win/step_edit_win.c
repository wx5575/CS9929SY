/**
  ******************************************************************************
  * @file    step_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.13
  * @brief   步骤编辑窗口
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
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "UI_COM/com_ui_info.h"
#include "key_menu_win/key_menu_win.h"
#include "ui_com/com_ui_info.h"
#include "cs99xx_mem_api.h"
#include "ui_com/com_edit_api.h"
#include "step_par_win/7_step_edit_win.h"
#include "type/cs99xx_type.h"

#include "keyboard.h"
//#include "stm32f4xx.h"
//#include "rtc_config.h"
//#include "GUI.H"
//#include "WM.h"
//#include "DIALOG.h"
//#include "fonts.h"
//#include "ff.h"
//#include "OS.H"
#include "app.h"
//#include "cs99xx_struct.h"
//#include "FRAMEWIN.H"
//#include "EDIT.H"
/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  步骤编辑窗口中编辑对象的枚举索引
  */
typedef enum{
    STEP_EDIT_WIN_STEP,///<测试步骤
    STEP_EDIT_WIN_MODE,///<测试模式
    STEP_EDIT_WIN_VOL,///<输出电压
    STEP_EDIT_WIN_RANGE,///<电流档位
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    
    STEP_EDIT_WIN_ARC,
    STEP_EDIT_WIN_REAL_C,
    STEP_EDIT_WIN_FREQ,
    STEP_EDIT_WIN_RAISE_T,
    STEP_EDIT_WIN_TEST_T,
    STEP_EDIT_WIN_FALL_T,
    STEP_EDIT_WIN_INTER_T,
    STEP_EDIT_WIN_CONT,
    STEP_EDIT_WIN_PASS,
    STEP_EDIT_WIN_PORT,
    
    
    STEP_EDIT_UI_END,
}STEP_EDIT_UI_INDEX;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void step_edit_windows_cb(WM_MESSAGE* pMsg);
static void update_key_inf(WM_HMEM hWin);
static void step_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void step_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void save_setting_step(void);

static void edit_step_num_f1_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f2_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f3_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f4_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f5_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_f6_cb(KEY_MESSAGE *key_msg);

static void edit_mode_f1_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f2_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f3_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f4_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f5_cb(KEY_MESSAGE *key_msg);
static void edit_mode_f6_cb(KEY_MESSAGE *key_msg);

static void edit_range_f1_cb(KEY_MESSAGE *key_msg);
static void edit_range_f2_cb(KEY_MESSAGE *key_msg);
static void edit_range_f3_cb(KEY_MESSAGE *key_msg);
static void edit_range_f4_cb(KEY_MESSAGE *key_msg);
static void edit_range_f5_cb(KEY_MESSAGE *key_msg);
static void edit_range_f6_cb(KEY_MESSAGE *key_msg);

static void edit_step_num_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void edit_step_num_direct_key_enter_cb(KEY_MESSAGE *key_msg);

static void edit_mode_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void edit_mode_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void edit_mode_direct_key_enter_cb(KEY_MESSAGE *key_msg);

static void select_test_mode_key_cb(KEY_MESSAGE *key_msg);
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win);
static void edit_step_num_sys_key(WM_HMEM hWin);
static void step_edit_win_sys_key_init(WM_HMEM hWin);
static void edit_step_num_menu_key_init(WM_HMEM hWin);
static void edit_mode_menu_key_init(WM_HMEM hWin);
static void edit_mode_win_sys_key_init(WM_HMEM hWin);
static void set_step_par_window_ele_data(UN_STRUCT *step);
static void update_and_init_mode(void);
static void edit_range_menu_key_init(WM_HMEM hWin);

static void check_range_value_validity(EDIT_ELE_T* ele, uint32_t *value);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  为设置而定义的中间的步骤变量
  */
static NODE_STEP tmp_step_par;

/**
  * @brief  步骤编辑窗口的位置尺寸信息数组，根据不同的屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* step_edit_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_edit_windows,/*4.3寸屏*/
    &_7_step_edit_windows,/*5.6寸屏*/
    &_7_step_edit_windows,/*7寸屏*/
};

/**
  * @brief  步骤编辑窗口中要显示的编辑控件的索引表
  */
static CS_INDEX step_par_index[]=
{
    STEP_EDIT_WIN_STEP,
    STEP_EDIT_WIN_MODE,
    STEP_EDIT_WIN_VOL,
    STEP_EDIT_WIN_RANGE,
    STEP_EDIT_WIN_UPPER,///<电流上限
    STEP_EDIT_WIN_LOWER,///<电流下限
    
    STEP_EDIT_WIN_ARC,
    STEP_EDIT_WIN_REAL_C,
    STEP_EDIT_WIN_FREQ,
    STEP_EDIT_WIN_RAISE_T,
    STEP_EDIT_WIN_TEST_T,
    STEP_EDIT_WIN_FALL_T,
    STEP_EDIT_WIN_INTER_T,
    STEP_EDIT_WIN_CONT,
    STEP_EDIT_WIN_PASS,
    STEP_EDIT_WIN_PORT,
};
/**
  * @brief  编辑步骤编号时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_step_num_menu_key_init_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP, edit_step_num_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_step_num_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, edit_step_num_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, edit_step_num_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, edit_step_num_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_step_num_f6_cb },//f6
};
/**
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_mode_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, edit_mode_f1_cb },//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, edit_mode_f2_cb },//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, edit_mode_f3_cb },//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, edit_mode_f4_cb },//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, edit_mode_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_mode_f6_cb },//f6
};
/**
  * @brief  编辑测试模式时使用的菜单键初始化信息数组
  */
static MENU_KEY_INFO_T 	edit_range_menu_key_init_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP, edit_range_f1_cb },//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP, edit_range_f2_cb },//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP, edit_range_f3_cb },//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP, edit_range_f4_cb },//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP, edit_range_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, edit_range_f6_cb },//f6
};
/**
  * @brief  步骤编辑窗口系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	step_edit_win_sys_key_init_pool[]=
{
	{KEY_UP		, step_edit_win_direct_key_up_cb      },
	{KEY_DOWN	, step_edit_win_direct_key_down_cb 	},
	{CODE_LEFT	, step_edit_win_direct_key_down_cb    },
	{CODE_RIGH	, step_edit_win_direct_key_up_cb      },
};
/**
  * @brief  编辑步骤编号时使用的系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	edit_step_num_sys_key_init_pool[]=
{
	{KEY_UP		, edit_step_num_direct_key_up_cb    },
	{KEY_DOWN	, edit_step_num_direct_key_down_cb  },
	{CODE_LEFT	, edit_step_num_direct_key_down_cb  },
	{CODE_RIGH	, edit_step_num_direct_key_up_cb    },
	{KEY_ENTER	, edit_step_num_direct_key_enter_cb },
};
/**
  * @brief  编辑步骤编号时使用的系统功能键初始化信息数组
  */
static FUNCTION_KEY_INFO_T 	edit_mode_sys_key_init_pool[]=
{
	{KEY_UP		, edit_mode_direct_key_up_cb    },
	{KEY_DOWN	, edit_mode_direct_key_down_cb  },
	{CODE_LEFT	, edit_mode_direct_key_down_cb  },
	{CODE_RIGH	, edit_mode_direct_key_up_cb    },
	{KEY_ENTER	, edit_mode_direct_key_enter_cb },
};
/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF mode_inf_pool[]=
{
    {ACW_STR , ACW, select_test_mode_key_cb},
    {DCW_STR , DCW, select_test_mode_key_cb},
    {IR_STR  , IR , select_test_mode_key_cb},
    {GR_STR  , GR , select_test_mode_key_cb},
    {BBD_STR , BBD, select_test_mode_key_cb},
    {CC_STR  , CC , select_test_mode_key_cb},
};
/**
  * @brief  编辑测试模式时使用的定制菜单键信息初始化数组
  */
static CUSTOM_MENU_KEY_INF acw_range_inf_pool[]=
{
    {CUR_2uA_STR     , AC_2uA	, select_test_mode_key_cb},
    {CUR_20uA_STR    , AC_20uA	, select_test_mode_key_cb},
    {CUR_200uA_STR   , AC_200uA , select_test_mode_key_cb},
    {CUR_2mA_STR     , AC_2mA	, select_test_mode_key_cb},
    {CUR_10mA_STR    , AC_10mA	, select_test_mode_key_cb},
    {CUR_20mA_STR    , AC_20mA	, select_test_mode_key_cb},
    {CUR_50mA_STR    , AC_50mA	, select_test_mode_key_cb},
    {CUR_100mA_STR   , AC_100mA , select_test_mode_key_cb},
    {CUR_200mA_STR   , AC_200mA , select_test_mode_key_cb},
    {CUR_2A_STR      , AC_2A    , select_test_mode_key_cb},
};
/**
  * @brief  步骤编辑窗口中所有的编辑控件初始化池
  */
static EDIT_ELE_T step_par_ele_pool[]=
{
    {
        {"测试步骤:","TestStep:"}, /* 名称 */
        STEP_EDIT_WIN_STEP,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, sizeof(STEP_NUM)/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*decs*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {edit_step_num_sys_key, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试模式:","TestMode:"}, /* 名称 */
        STEP_EDIT_WIN_MODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {MODE_END/*heigh*/,ACW/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {edit_mode_win_sys_key_init, edit_mode_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出电压:","Voltage:"}, /* 名称 */
        STEP_EDIT_WIN_VOL,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,VOL_U_kV/*unit*/,},/*format*/
        {5000/*heigh*/,50/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电流档位:","Cur.Range:"}, /* 名称 */
        STEP_EDIT_WIN_RANGE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {MODE_END/*heigh*/,ACW/*low*/,{"",""}/*notice*/,check_range_value_validity},/*range*/
        {step_edit_win_sys_key_init, edit_range_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电流上限:","Up Limt:"}, /* 名称 */
        STEP_EDIT_WIN_UPPER,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电流下限:","LowLimt:"}, /* 名称 */
        STEP_EDIT_WIN_LOWER,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"电弧侦测:","ARC:"}, /* 名称 */
        STEP_EDIT_WIN_ARC,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"真实电流:","Real Cur.:"}, /* 名称 */
        STEP_EDIT_WIN_REAL_C,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出频率:","Frequence:"}, /* 名称 */
        STEP_EDIT_WIN_FREQ,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"上升时间:","Raise:"}, /* 名称 */
        STEP_EDIT_WIN_RAISE_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试时间:","TestTime:"}, /* 名称 */
        STEP_EDIT_WIN_TEST_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"下降时间:","FallTime:"}, /* 名称 */
        STEP_EDIT_WIN_FALL_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"间隔时间:","Inter.Time:"}, /* 名称 */
        STEP_EDIT_WIN_INTER_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"步间连续:","Step Con.:"}, /* 名称 */
        STEP_EDIT_WIN_INTER_T,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"步间PASS:","Step Pass:"}, /* 名称 */
        STEP_EDIT_WIN_PASS,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"输出端口:","Port:"}, /* 名称 */
        STEP_EDIT_WIN_PORT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {NULL, 0,NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {3/*decs*/,5/*lon*/,CUR_U_mA/*unit*/,},/*format*/
        {2000/*heigh*/,0/*low*/,{"",""}/*notice*/},/*range*/
        {step_edit_win_sys_key_init, edit_step_num_menu_key_init, keyboard_fun_num,},/*key_inf*/
    },
};

/**
  * @brief  步骤编辑窗口结构初始化定义
  */
static MYUSER_WINDOW_T step_edit_windows=
{
    {"步骤编辑","step_edit_windows"},
    step_edit_windows_cb, update_key_inf,
	{0},
    {
        step_par_ele_pool,ARRAY_SIZE(step_par_ele_pool),
        (CS_INDEX*)step_par_index,ARRAY_SIZE(step_par_index),
        init_create_step_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_group_com_ele_table,ARRAY_SIZE(range_group_com_ele_table),
        init_create_step_edit_win_com_ele,
    },
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  编辑步骤编号使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  编辑步骤编号使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  编辑步骤编号使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑步骤编号使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}
/**
  * @brief  编辑测试模式使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑测试模式使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  编辑电流档位使用的功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑电流档位使用的功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_range_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}
/**
  * @brief  获取当前步的步号数据
  * @param  无
  * @retval 当前步的步号
  */
static STEP_NUM get_cur_step(void)
{
    return *(STEP_NUM*)g_cur_edit_ele->data.data;
}
/**
  * @brief  重新初始化并创建窗口中的对象
  * @param  [in] step 要加载的步骤编号
  * @retval 无
  */
static void re_init_create_win_all_ele(STEP_NUM step)
{
    NODE_STEP *node;
    delete_win_all_ele(g_cur_win);
    load_steps_to_list(step, 1);
    
    node = get_g_cur_step();
    
    if(NULL != node)
    {
        memcpy(g_cur_step, node, sizeof(NODE_STEP));
        init_create_win_all_ele(g_cur_win);
        select_edit_ele(g_cur_edit_ele);//重新选重当前编辑控件
        update_group_inf(g_cur_win);
    }
}
/**
  * @brief  编辑步骤编号使用的确认键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_enter_cb(KEY_MESSAGE *key_msg)
{
    uint32_t new_step = get_edit_ele_value(g_cur_edit_ele,NULL);
    uint32_t old_step = get_cur_step();
    
    if(new_step != old_step && new_step != 0 && new_step <= g_cur_file->total)
    {
        re_init_create_win_all_ele(new_step);
    }
    else
    {
        set_edit_num_value(g_cur_edit_ele, old_step);
    }
}

/**
  * @brief  编辑步骤编号使用的向上键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_cb(key_msg);
    step_edit_win_direct_key_up_cb(key_msg);
}

/**
  * @brief  编辑步骤编号使用的向下键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_step_num_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    edit_step_num_direct_key_enter_cb(key_msg);
    step_edit_win_direct_key_down_cb(key_msg);
}

/**
  * @brief  编辑测试模式使用的确认键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_enter_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();
}

/**
  * @brief  更新并初始化测试模式
  * @param  无
  * @retval 无
  */
static void update_and_init_mode(void)
{
    uint8_t mode = get_edit_ele_value(g_cur_edit_ele, NULL);
    
    if(g_cur_step->one_step.com.mode != mode)
    {
        g_cur_step->one_step.com.mode = mode;
        init_mode(g_cur_step);
        save_setting_step();
        set_step_par_window_ele_data(&g_cur_step->one_step);//初始化编辑对象的参数
    }
}
/**
  * @brief  编辑测试模式使用的向上键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();
    step_edit_win_direct_key_up_cb(key_msg);
}

/**
  * @brief  编辑测试模式使用的向下键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_mode_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    update_and_init_mode();
    step_edit_win_direct_key_down_cb(key_msg);
}
/**
  * @brief  编辑步骤编号使用的系统功能按键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_step_num_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(edit_step_num_sys_key_init_pool, ARRAY_SIZE(edit_step_num_sys_key_init_pool), hWin);
}
/**
  * @brief  编辑测试模式使用的系统功能按键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_mode_win_sys_key_init(WM_HMEM hWin)
{
    register_system_key_fun(edit_mode_sys_key_init_pool, ARRAY_SIZE(edit_mode_sys_key_init_pool), hWin);
}
/**
  * @brief  步骤编辑窗口系统功能键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void step_edit_win_sys_key_init(WM_HMEM hWin)
{
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    register_system_key_fun(step_edit_win_sys_key_init_pool, ARRAY_SIZE(step_edit_win_sys_key_init_pool), data);
}
/**
  * @brief  编辑步骤编号使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_step_num_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_step_num_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_step_num_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}

/**
  * @brief  选择测试模式时使用的按键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void select_test_mode_key_cb(KEY_MESSAGE *key_msg)
{
    int data = key_msg->custom_data;
    int hWin = key_msg->user_data;
    uint32_t size = g_cur_edit_ele->data.bytes;
    CS_INDEX index;
    
    index = get_data_in_resource_table_index(g_cur_edit_ele, size, &data);
    
    DROPDOWN_SetSel(hWin, index);
    DROPDOWN_SetUserData(hWin, &data, size);
}
/**
  * @brief  编辑测试模式时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_mode_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_mode_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_mode_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    CUSTOM_MENU_KEY_INF *cus_inf = mode_inf_pool;
    uint16_t cus_size = ARRAY_SIZE(mode_inf_pool);
    
    init_menu_key_custom_inf(cus_inf, cus_size, g_cur_edit_ele, info, size);
	init_menu_key_info(info, size, data);
}
/**
  * @brief  编辑电流档位时使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void edit_range_menu_key_init(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = edit_range_menu_key_init_info;
    uint32_t size = ARRAY_SIZE(edit_range_menu_key_init_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    CUSTOM_MENU_KEY_INF *cus_inf = acw_range_inf_pool;
    uint16_t cus_size = sizeof(acw_range_inf_pool);
    
    init_menu_key_custom_inf(cus_inf, cus_size, g_cur_edit_ele, info, size);
	init_menu_key_info(info, size, data);
}

static uint8_t get_cur_step_mode(void)
{
    return g_cur_step->one_step.com.mode;
}

static void check_acw_range_value(EDIT_ELE_T* ele, uint32_t *range)
{
    uint8_t tmp = *range;
    ACW_STRUCT *acw = &g_cur_step->one_step.acw;
    uint32_t new_range = get_edit_ele_value(g_cur_edit_ele, NULL);
    uint32_t old_range = acw->range;
    EDIT_ELE_T* tmp_ele;
    
    if(new_range == old_range)
    {
        return;
    }
    
    if(tmp >= AC_GEAR_END)
    {
        *range = get_acw_max_gear();
        tmp = *range;
    }
    
    tmp_ele = &step_par_ele_pool[STEP_EDIT_WIN_UPPER];
    tmp_ele->format.unit = ac_gear[tmp].unit;
    tmp_ele->format.lon = ac_gear[tmp].lon;
    tmp_ele->format.decs = ac_gear[tmp].decs;
    tmp_ele->range.high = ac_gear[tmp].high_max;
    acw->upper_limit = ACW_UPPER_DEFAULT_VAL;
    set_edit_num_value(tmp_ele, acw->upper_limit);
    
    tmp_ele = &step_par_ele_pool[STEP_EDIT_WIN_LOWER];
    tmp_ele->format.unit = ac_gear[tmp].unit;
    tmp_ele->format.lon = ac_gear[tmp].lon;
    tmp_ele->format.decs = ac_gear[tmp].decs;
    tmp_ele->range.high = acw->upper_limit;
    acw->lower_limit = ACW_LOWER_DEFAULT_VAL;
    set_edit_num_value(tmp_ele, acw->lower_limit);
    
}
static void check_range_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint8_t mode;
    
    mode = get_cur_step_mode();
    
    switch(mode)
    {
        case ACW:
            check_acw_range_value(ele, value);
            break;
        case DCW:
            break;
    }
}
/**
  * @brief  设置ACW参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_acw_par_win_ele_data(UN_STRUCT *step)
{
    uint8_t mode = ACW;
    
    step_par_ele_pool[STEP_EDIT_WIN_VOL].data.data = &step->acw.testing_voltage;
    step_par_ele_pool[STEP_EDIT_WIN_RANGE].data.data = &step->acw.range;
    step_par_ele_pool[STEP_EDIT_WIN_RANGE].resource.table = get_defined_range_table(mode);
    step_par_ele_pool[STEP_EDIT_WIN_RANGE].resource.size = get_defined_range_num(mode);
    step_par_ele_pool[STEP_EDIT_WIN_RANGE].resource.user_data = get_defined_range_flag(mode);
    step_par_ele_pool[STEP_EDIT_WIN_RANGE].resource.user_data_size = get_defined_range_num(mode);
    
    step_par_ele_pool[STEP_EDIT_WIN_UPPER].data.data = &step->acw.upper_limit;
    step_par_ele_pool[STEP_EDIT_WIN_LOWER].data.data = &step->acw.lower_limit;
    
}
/**
  * @brief  设置DCW参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_dcw_par_win_ele_data(UN_STRUCT *step)
{
    
}

/**
  * @brief  设置IR参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_ir_par_win_ele_data(UN_STRUCT *step)
{
    
}

/**
  * @brief  设置GR参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_gr_par_win_ele_data(UN_STRUCT *step)
{
    
}

/**
  * @brief  根据测试模式来设置参数对应编辑控件的数据指针
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
static void set_step_par_window_ele_data(UN_STRUCT *step)
{
    step_par_ele_pool[STEP_EDIT_WIN_STEP].data.data = &step->com.step;
    step_par_ele_pool[STEP_EDIT_WIN_MODE].data.data = &step->com.mode;
    step_par_ele_pool[STEP_EDIT_WIN_MODE].resource.table = get_defined_mode_table();
    step_par_ele_pool[STEP_EDIT_WIN_MODE].resource.size = get_defined_mode_num();
    step_par_ele_pool[STEP_EDIT_WIN_MODE].resource.user_data = get_defined_mode_flag();
    step_par_ele_pool[STEP_EDIT_WIN_MODE].resource.user_data_size = get_defined_mode_num();
    
    switch(step->com.mode)
    {
        case ACW:
            set_acw_par_win_ele_data(step);
            break;
        case DCW:
            set_dcw_par_win_ele_data(step);
            break;
        case IR:
            set_ir_par_win_ele_data(step);
            break;
        case GR:
            set_gr_par_win_ele_data(step);
            break;
    }
}
/**
  * @brief  初始化并创建步骤编辑窗口中的编辑控件
  * @param  [in] win 窗口的结构数据
  * @retval 无
  */
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    memcpy(&tmp_step_par, g_cur_step, sizeof(tmp_step_par));
    g_cur_step = &tmp_step_par;
    set_step_par_window_ele_data(&g_cur_step->one_step);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_step_par_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}
/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    update_group_inf(win);
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

/**
  * @brief  窗口重绘
  * @param  无
  * @retval 无
  */
static void step_edit_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_WHITE);
	GUI_ClearRectEx(&r);
}
/**
  * @brief  保存正在设置的步骤
  * @param  无
  * @retval 无
  */
static void save_setting_step(void)
{
    FILE_NUM file_num;
    STEP_NUM step_num;
    
    file_num = g_cur_file->num;
    step_num = g_cur_step->one_step.com.step;
    
    save_one_step(g_cur_step, file_num, step_num);//保存数据
}

/**
  * @brief  向上键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, EDIT_ELE_T, e_list);
    }
    
    select_edit_ele(g_cur_edit_ele);
    save_setting_step();
}
/**
  * @brief  向下键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
static void step_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, EDIT_ELE_T, e_list);
    }
    
    select_edit_ele(g_cur_edit_ele);
    save_setting_step();
}

/**
  * @brief  更新系统按键信息
  * @param  [in] hWin窗口句柄
  * @retval 无
  */
static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(step_edit_win_sys_key_init_pool, ARRAY_SIZE(step_edit_win_sys_key_init_pool), hWin);
}
/**
  * @brief  更新按键信息
  * @param  [in] hWin窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_sys_key_inf(hWin);
}
/**
  * @brief  步骤参数设置窗口回调函数
  * @param  [in] pMsg窗口消息
  * @retval 无
  */
static void step_edit_windows_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case CM_CHILD_W_MSG:
        {
            update_key_inf(hWin);
            break;
        }
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            
            break;
		case WM_TIMER:
			break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
		case WM_PAINT:
			step_edit_win_paint_frame();
            draw_group_inf_area();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建步骤编辑窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_step_edit_win(int hWin)
{
    init_window_size(&step_edit_windows, step_edit_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&step_edit_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
