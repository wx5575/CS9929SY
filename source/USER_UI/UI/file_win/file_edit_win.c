
/**
  ******************************************************************************
  * @file    file_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件编辑窗口
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
#include "UI_COM/com_ui_info.h"
#include "file_edit_win.h"
#include "7_file_edit_win.h"
#include "ui_com/com_edit_api.h"
#include "key_menu_win/key_menu_win.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void file_edit_win_cb(WM_MESSAGE * pMsg);
static FUNCTION_KEY_INFO_T file_edit_sys_key_pool[];
extern EDIT_ELE_T edit_file_ele_pool[];
static void file_edit_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void file_edit_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void file_edit_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void file_edit_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void menu_key_ok(WM_HWIN);
static void menu_key_cancle(WM_HWIN);

static void edit_name_f1_cb(KEY_MESSAGE *key_msg);
static void edit_name_f2_cb(KEY_MESSAGE *key_msg);
static void edit_name_f3_cb(KEY_MESSAGE *key_msg);
static void edit_name_f4_cb(KEY_MESSAGE *key_msg);
static void edit_name_f5_cb(KEY_MESSAGE *key_msg);
static void edit_name_f6_cb(KEY_MESSAGE *key_msg);

static void edit_work_mode_f1_cb(KEY_MESSAGE *key_msg);
static void edit_work_mode_f2_cb(KEY_MESSAGE *key_msg);
static void edit_work_mode_f3_cb(KEY_MESSAGE *key_msg);
static void edit_work_mode_f4_cb(KEY_MESSAGE *key_msg);

static void edit_beep_time_f1_cb(KEY_MESSAGE *key_msg);
static void edit_beep_time_f2_cb(KEY_MESSAGE *key_msg);
static void edit_beep_time_f3_cb(KEY_MESSAGE *key_msg);
static void edit_beep_time_f4_cb(KEY_MESSAGE *key_msg);

static void edit_pass_time_f1_cb(KEY_MESSAGE *key_msg);
static void edit_pass_time_f2_cb(KEY_MESSAGE *key_msg);
static void edit_pass_time_f3_cb(KEY_MESSAGE *key_msg);
static void edit_pass_time_f4_cb(KEY_MESSAGE *key_msg);

static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win);

static void fname_sys_key(WM_HWIN data);
static void fwmode_sys_key(WM_HWIN data);
static void fpasstime_sys_key(WM_HWIN data);
static void fbeeptime_sys_key(WM_HWIN data);
static void fname_menu_key(WM_HWIN data);
static void fwmode_menu_key(WM_HWIN data);
static void fpasstime_menu_key(WM_HWIN data);
static void fbeeptime_menu_key(WM_HWIN data);

static void set_fwmode_n(WM_HWIN hWin);
static void set_fwmode_g(WM_HWIN hWin);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  文件编辑窗口的位置尺寸信息池，对应不同尺寸的屏幕
  */
static WIDGET_POS_SIZE_T* file_edit_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_file_edit_windows,/*4.3寸屏*/
    &_7_file_edit_windows,/*5.6寸屏*/
    &_7_file_edit_windows,/*7寸屏*/
};
/**
  * @brief  文件名编辑菜单键信息数组
  */
static MENU_KEY_INFO_T 	fname_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, edit_name_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_name_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, edit_name_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, edit_name_f4_cb },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, edit_name_f5_cb },//f5
    {"", F_KEY_CANCLE   , KEY_F6 & _KEY_UP, edit_name_f6_cb },//f6
};
/**
  * @brief  工作模式编辑菜单键信息数组
  */
static MENU_KEY_INFO_T 	fwmode_menu_key_info[] =
{
    {"N", F_KEY_CUSTOM, KEY_F1 & _KEY_UP, edit_work_mode_f1_cb },//f1
    {"G", F_KEY_CUSTOM, KEY_F2 & _KEY_UP, edit_work_mode_f2_cb },//f2
    {"" , F_KEY_NULL  , KEY_F3 & _KEY_UP, edit_work_mode_f3_cb },//f3
    {"" , F_KEY_NULL  , KEY_F4 & _KEY_UP, edit_work_mode_f4_cb },//f4
};

/**
  * @brief  蜂鸣时间编辑菜单键信息数组
  */
static MENU_KEY_INFO_T 	fbeept_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, edit_beep_time_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, edit_beep_time_f2_cb },//f2
    {"" , F_KEY_NULL    , KEY_F3 & _KEY_UP, edit_beep_time_f3_cb },//f3
    {"" , F_KEY_NULL    , KEY_F4 & _KEY_UP, edit_beep_time_f4_cb },//f4
};
/**
  * @brief  PASS时间编辑菜单键信息数组
  */
static MENU_KEY_INFO_T 	fpasst_menu_key_info[] =
{
    {"", F_KEY_DEL  , KEY_F1 & _KEY_UP, edit_pass_time_f1_cb },//f1
    {"", F_KEY_CLEAR, KEY_F2 & _KEY_UP, edit_pass_time_f2_cb },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, edit_pass_time_f3_cb },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, edit_pass_time_f4_cb },//f4
};

/**
  * @brief  文件编辑窗口系统按键初始化数组
  */
static FUNCTION_KEY_INFO_T file_edit_sys_key_pool[]=
{
	{KEY_UP		, file_edit_direct_key_up_cb		 },
	{KEY_DOWN	, file_edit_direct_key_down_cb	 },
	{KEY_LEFT	, file_edit_direct_key_left_cb	 },
	{KEY_RIGHT	, file_edit_direct_key_right_cb	 },
    
	{CODE_RIGH	, file_edit_direct_key_up_cb      },
	{CODE_LEFT	, file_edit_direct_key_down_cb	 },
};
/**
  * @brief  文件编辑窗口的编辑对象初始化数组
  */
static EDIT_ELE_T edit_file_ele_pool[]=
{
    {
        {"文 件 名:","FileName:"}, /* 名称 */
        FSAVE_UI_FNAME,/* 通过枚举索引 */
        {"DEFAULT","DEFAULT"},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-12个字符 \n(A-Z a-z 0-9 空格 + / - .)","1-12 characters\n(A-Z a-z 0-9 space + / - .)"}/*notice*/},/*range*/
        {fname_sys_key,fname_menu_key,keyboard_fun_str,},/*key_inf*/
        0,/*dis*/
    },
    {
        {"工作模式:","WorkMode:"}, /* 名称 */
        FSAVE_UI_WMODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"WorkMode","WorkMode"}/*notice*/},/*range*/
        {fwmode_sys_key,fwmode_menu_key,keyboard_fun_num,},/*key_inf*/
        0,/*dis*/
    },
    {
        {"蜂鸣时间:","BeepTime:"}, /* 名称 */
        FSAVE_UI_BEEPT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"BeepTime","BeepTime"}/*notice*/},/*range*/
        {fbeeptime_sys_key,fbeeptime_menu_key,keyboard_fun_num,},/*key_inf*/
        0,/*dis*/
    },
    {
        {"PASS时间:","PassTime:"}, /* 名称 */
        FSAVE_UI_PASST,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
        {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"PassTime","PassTime"}/*notice*/},/*range*/
        {fpasstime_sys_key,fpasstime_menu_key,keyboard_fun_num,},/*key_inf*/
        0,/*dis*/
    },
};
/**
  * @brief  文件保存窗口的编辑对象索引数组
  */
static CS_INDEX fsave_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
/**
  * @brief  文件新建窗口的编辑对象索引数组
  */
static CS_INDEX fnew_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
/**
  * @brief  文件编辑窗口的编辑对象索引数组
  */
static CS_INDEX fedit_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
/**
  * @brief  文件保存窗口的数据结构定义
  */
static MYUSER_WINDOW_T save_file_window=
{
    {"存贮文件","Save As File"},
    file_edit_win_cb,NULL,
    {
        0
    },
    {
        edit_file_ele_pool, ARRAY_SIZE(edit_file_ele_pool),
        (CS_INDEX*)fsave_ui_ele_table,ARRAY_SIZE(fsave_ui_ele_table),
        init_create_file_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_file_edit_win_com_ele,
    },
};
/**
  * @brief  文件新建窗口的数据结构定义
  */
static MYUSER_WINDOW_T new_file_window=
{
    {"新建文件","New File"},
    file_edit_win_cb,NULL,
    {
        0
    },
    {
        edit_file_ele_pool, ARRAY_SIZE(edit_file_ele_pool),
        (CS_INDEX*)fnew_ui_ele_table,ARRAY_SIZE(fnew_ui_ele_table),
        init_create_file_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_file_edit_win_com_ele,
    },
};
/**
  * @brief  文件编辑窗口的数据结构定义
  */
static MYUSER_WINDOW_T edit_file_windows=
{
    {"编辑文件","Edit File"},
    file_edit_win_cb, NULL,
    {
        0
    },
    {
        edit_file_ele_pool, ARRAY_SIZE(edit_file_ele_pool),
        (CS_INDEX*)fedit_ui_ele_table,ARRAY_SIZE(fedit_ui_ele_table),
        init_create_file_edit_win_edit_ele,
    },
    {
        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_file_edit_win_com_ele,
    },
};


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  编辑文件名功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  编辑文件名功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  编辑文件名功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑文件名功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑文件名功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f5_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);
}
/**
  * @brief  编辑文件名功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_name_f6_cb(KEY_MESSAGE *key_msg)
{
    menu_key_cancle(key_msg->user_data);
}
/**
  * @brief  编辑工作模式功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_work_mode_f1_cb(KEY_MESSAGE *key_msg)
{
    set_fwmode_n(key_msg->user_data);
}
/**
  * @brief  编辑工作模式功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_work_mode_f2_cb(KEY_MESSAGE *key_msg)
{
    set_fwmode_g(key_msg->user_data);
}
/**
  * @brief  编辑工作模式功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_work_mode_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑工作模式功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_work_mode_f4_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  编辑蜂鸣时间功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_beep_time_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  编辑蜂鸣时间功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_beep_time_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  编辑蜂鸣时间功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_beep_time_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑蜂鸣时间功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_beep_time_f4_cb(KEY_MESSAGE *key_msg)
{
}

/**
  * @brief  编辑PASS时间功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_pass_time_f1_cb(KEY_MESSAGE *key_msg)
{
    menu_key_backspace(key_msg->user_data);
}
/**
  * @brief  编辑PASS时间功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_pass_time_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_edit_ele(key_msg->user_data);
}
/**
  * @brief  编辑PASS时间功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_pass_time_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑PASS时间功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_pass_time_f4_cb(KEY_MESSAGE *key_msg)
{
}


/**
  * @brief  向上方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void file_edit_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, EDIT_ELE_T, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

/**
  * @brief  向下方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void file_edit_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, EDIT_ELE_T, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

/**
  * @brief  向左方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void file_edit_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}

/**
  * @brief  向右方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void file_edit_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}



/**
  * @brief  注册文件名编辑系统键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fname_sys_key(WM_HWIN hWin)
{
    register_system_key_fun(file_edit_sys_key_pool, ARRAY_SIZE(file_edit_sys_key_pool), hWin);
}
/**
  * @brief  注册工作模式编辑系统键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fwmode_sys_key(WM_HWIN hWin)
{
    register_system_key_fun(file_edit_sys_key_pool, ARRAY_SIZE(file_edit_sys_key_pool), hWin);
}
/**
  * @brief  注册pass时间编辑系统键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fpasstime_sys_key(WM_HWIN hWin)
{
    register_system_key_fun(file_edit_sys_key_pool, ARRAY_SIZE(file_edit_sys_key_pool), hWin);
}
/**
  * @brief  注册蜂鸣时间编辑系统键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fbeeptime_sys_key(WM_HWIN hWin)
{
    register_system_key_fun(file_edit_sys_key_pool, ARRAY_SIZE(file_edit_sys_key_pool), hWin);
}
/**
  * @brief  注册文件名编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fname_menu_key(WM_HWIN hWin)
{
    MENU_KEY_INFO_T * info = fname_menu_key_info;
    uint32_t size = ARRAY_SIZE(fname_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}


/**
  * @brief  注册工作模式编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fwmode_menu_key(WM_HWIN hWin)
{
    MENU_KEY_INFO_T * info = fwmode_menu_key_info;
    uint32_t size = ARRAY_SIZE(fwmode_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}


/**
  * @brief  注册PASS时间编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fpasstime_menu_key(WM_HWIN hWin)
{
    MENU_KEY_INFO_T * info = fpasst_menu_key_info;
    uint32_t size = ARRAY_SIZE(fpasst_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  注册蜂鸣时间编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void fbeeptime_menu_key(WM_HWIN hWin)
{
    MENU_KEY_INFO_T * info = fbeept_menu_key_info;
    uint32_t size = ARRAY_SIZE(fbeept_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  注册设置工作模式为N模式编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_fwmode_n(WM_HWIN hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis.edit.handle, N_MODE);
}
/**
  * @brief  注册设置工作模式为G模式编辑菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_fwmode_g(WM_HWIN hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis.edit.handle, G_MODE);
}

/**
  * @brief  确认键回调函数调用，用来确认并关闭对话框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_ok(WM_HWIN hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	g_custom_msg.user_data = (int)&global_file;
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    unregister_system_key_fun(file_edit_sys_key_pool, ARRAY_SIZE(file_edit_sys_key_pool));
    back_win(hWin);//关闭对话框
}
/**
  * @brief  返回键回调函数调用，用来取消并关闭对话框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_cancle(WM_HWIN hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_CANCLE;
    back_win(hWin);//关闭对话框
}

/**
  * @brief  设置编辑对象的数据成员
  * @param  [in] f 文件结构信息
  * @retval 无
  */
static void set_file_par_window_ele_data(TEST_FILE *f)
{
    set_edit_ele_data(&edit_file_ele_pool[FSAVE_UI_FNAME], f->name);
    set_edit_ele_data(&edit_file_ele_pool[FSAVE_UI_WMODE], &f->work_mode);
    set_edit_ele_data(&edit_file_ele_pool[FSAVE_UI_BEEPT], &f->buzzer_time);
    set_edit_ele_data(&edit_file_ele_pool[FSAVE_UI_PASST], &f->pass_time);
}
/**
  * @brief  初始化并创建窗口编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    set_file_par_window_ele_data(&global_file);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_file_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}

/**
  * @brief  初始化并创建窗口公共文本对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

/**
  * @brief  回调函数
  * @param  [in] pMsg 窗口消息指针
  * @retval 无
  */
static void file_edit_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_EDIT_WIDGET_CHANGE:
		{
			break;
		}
        case WM_PAINT:
            break;
        case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
            break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
        default:
            WM_DefaultProc(pMsg);
    }
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建对话框用来保存文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_save_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_SAVE);
    init_window_size(&save_file_window, file_edit_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&save_file_window, &windows_list, g_cur_win->handle);//创建主界面
}
/**
  * @brief  创建对话框用来新建文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_new_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_NEW);
    init_window_size(&new_file_window, file_edit_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&new_file_window, &windows_list, g_cur_win->handle);//创建主界面
}

/**
  * @brief  创建对话框用来编辑文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_edit_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_EDIT);
    init_window_size(&edit_file_windows, file_edit_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&edit_file_windows, &windows_list, g_cur_win->handle);//创建主界面
}






/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
