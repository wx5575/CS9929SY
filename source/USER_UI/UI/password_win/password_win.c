/**
  ******************************************************************************
  * @file    file_ui_com.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件管理界面
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
#include "file_win/file_edit_win.h"
#include "ui_com/com_ui_info.h"
#include "ui_com/com_edit_api.h"
#include "warning_win/warning_win.h"
#include "cs99xx_mem_api.h"
#include "7_password_win.h"
#include "ui_com/com_edit_api.h"
#include "password_win.h"
#include "app.h"

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  密码类型结构定义
  */
typedef struct{
    uint8_t new_pwd[PWD_MAX_LEN + 1];///<新设密码缓冲区
    uint8_t old_pwd[PWD_MAX_LEN + 1];///<原始密码缓冲区
    uint8_t confirm_pwd[PWD_MAX_LEN + 1];///<确认密码缓冲区
    CS_BOOL old_pwd_flag;///<原始密码是否正确标记
    CS_BOOL new_pwd_flag;///<新设密码格式是否正确标记
    CS_BOOL confirm_pwd_flag;///<确认密码是否正确标记
}PWD_T;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void password_win_cb(WM_MESSAGE* pMsg);
static FUNCTION_KEY_INFO_T 	sys_key_pool[];
static void menu_key_ok(int hWin);

static void init_create_pwd_com_ele(MYUSER_WINDOW_T* win);
static void init_create_pwd_text_ele(MYUSER_WINDOW_T* win);
static void init_create_pwd_edit_ele(MYUSER_WINDOW_T* win);

static void old_new_pwd_win_f1_cb(KEY_MESSAGE *key_msg);
static void old_new_pwd_win_f2_cb(KEY_MESSAGE *key_msg);
static void old_new_pwd_win_f3_cb(KEY_MESSAGE *key_msg);
static void old_new_pwd_win_f4_cb(KEY_MESSAGE *key_msg);
static void old_new_pwd_win_f5_cb(KEY_MESSAGE *key_msg);
static void old_new_pwd_win_f6_cb(KEY_MESSAGE *key_msg);

static void confirm_pwd_win_f5_cb(KEY_MESSAGE *key_msg);

static void pwd_sys_key(WM_HMEM data);

static void pwd_menu_key(WM_HMEM hWin);
static void c_pwd_menu_key(WM_HMEM hWin);

static void direct_key_down_cb(KEY_MESSAGE *key_msg);
static void direct_key_up_cb(KEY_MESSAGE *key_msg);
static void direct_key_left_cb(KEY_MESSAGE *key_msg);
static void direct_key_right_cb(KEY_MESSAGE *key_msg);

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  密码管理窗口位置尺寸信息，根据不同的屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* password_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_password_windows,/*4.3寸屏*/
    &_7_password_windows,/*5.6寸屏*/
    &_7_password_windows,/*7寸屏*/
};
/**
  * @brief  密码管理窗口文本控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static TEXT_ELE_AUTO_LAYOUT_T  *pwd_text_ele_auto_layout[]=
{
    &_7_pwd_text_ele_auto_layout_inf,//4.3寸屏
    &_7_pwd_text_ele_auto_layout_inf,//5.6寸屏
    &_7_pwd_text_ele_auto_layout_inf,//7寸屏
};
/**
  * @brief  密码管理窗口编辑控件自动布局信息数组，根据不同的屏幕尺寸进行初始化
  */
static EDIT_ELE_AUTO_LAYOUT_T      *pwd_edit_ele_auto_layout[]=
{
    &_7_pwd_edit_ele_auto_layout_inf,//4.3寸屏
    &_7_pwd_edit_ele_auto_layout_inf,//5.6寸屏
    &_7_pwd_edit_ele_auto_layout_inf,//7寸屏
};

/**
  * @brief  原始密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	o_n_pwd_menu_key_info[] =
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, old_new_pwd_win_f1_cb },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, old_new_pwd_win_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, old_new_pwd_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, old_new_pwd_win_f4_cb },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, old_new_pwd_win_f5_cb ,MENU_KEY_DIS},//f5
    {"", F_KEY_BACK     , KEY_F6 & _KEY_UP, old_new_pwd_win_f6_cb },//f6
};

/**
  * @brief  确认密码 新设密码使用的菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	c_pwd_menu_key_info[] =
{
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, confirm_pwd_win_f5_cb },//f5
};

/**
  * @brief  窗口中使用到的编辑控件初始化数组
  */
static EDIT_ELE_T password_ele_pool[]=
{
    {
        {"原始密码:","Old PWD:"}, /* 名称 */
        PASSWORD_OLD,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-8个数字","1-8 Numbers"}/*notice*/},/*range*/
        {pwd_menu_key,pwd_sys_key,keyboard_fun_pwd,},/*key_inf*/
        0,/*dis*/
    },
    {
        {"新设密码:","New PWD:"}, /* 名称 */
        PASSWORD_NEW,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-8个数字","1-8 Numbers"}/*notice*/},/*range*/
        {pwd_menu_key,pwd_sys_key,keyboard_fun_pwd,},/*key_inf*/
        0,/*dis*/
    },
    {
        {"确认密码:","Confirm PWD:"}, /* 名称 */
        PASSWORD_CONFIRM,/* 通过枚举索引 */
        {"",""},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, E_STRING_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-8个数字","1-8 Numbers"}/*notice*/},/*range*/
        {c_pwd_menu_key,pwd_sys_key,keyboard_fun_pwd,},/*key_inf*/
        0,/*dis*/
    },
};

static PWD_T pdw_inf;

static FUNCTION_KEY_INFO_T sys_key_pool[]={
	{KEY_UP		, direct_key_up_cb		 },
	{KEY_DOWN	, direct_key_down_cb	 },
	{KEY_LEFT	, direct_key_left_cb	 },
	{KEY_RIGHT	, direct_key_right_cb	 },
    
	{CODE_LEFT	, direct_key_down_cb    },
	{CODE_RIGH	, direct_key_up_cb	     },
};


CS_INDEX password_ui_ele_table[]=
{
    PASSWORD_OLD,
    PASSWORD_NEW,
    PASSWORD_CONFIRM,
};

/**
  * @brief  密码界面要显示的文本对象索引表
  */
static CS_INDEX pwd_text_ele_table[]=
{
    PWD_UI_OLD_NOTICE,
    PWD_UI_NEW_NOTICE,
    PWD_UI_CONFIRM_NOTICE,
    PWD_UI_CHANGE_RESULT,
};
/**
  * @brief  密码界面的文本对象池
  */
static TEXT_ELE_T password_ui_text_ele_pool[]=
{
	{{"",""}, PWD_UI_OLD_NOTICE     },//原始密码的判断结果文本 "密码不正确"
	{{"",""}, PWD_UI_NEW_NOTICE     },//新密码的判断结果文本 "密码不能为空"
	{{"",""}, PWD_UI_CONFIRM_NOTICE   },//确认密码的判断结果文本 "密码不一"
	{{"",""}, PWD_UI_CHANGE_RESULT  },//密码修改的结果 "密码修改成功"
};

/**
  * @brief  密码界面窗口结构
  */
static MYUSER_WINDOW_T password_windows=
{
    {"系统密码","password_windows"},
    password_win_cb, NULL,
	{
        password_ui_text_ele_pool, ARRAY_SIZE(password_ui_text_ele_pool),
        (CS_INDEX*)pwd_text_ele_table, ARRAY_SIZE(pwd_text_ele_table),
        init_create_pwd_text_ele
    },
    {
        password_ele_pool, ARRAY_SIZE(password_ele_pool),
        (CS_INDEX*)password_ui_ele_table,ARRAY_SIZE(password_ui_ele_table),
        init_create_pwd_edit_ele
    },
    {
        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_pwd_com_ele
    },
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  清空编辑对象的密码，注意密码是在内存中的，不是在控件中显示的*
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void clear_pwd_edit_ele(int hWin)
{
    WM_HMEM handle = 0;
    uint8_t *str;
    
    str = g_cur_edit_ele->data.data;
    str[0] = 0;
    
    handle = g_cur_edit_ele->dis.edit.handle;
    clear_edit_ele(handle);
}
/**
  * @brief  退格编辑密码，注意密码是在内存中的，不是在控件中显示的*
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void backspace_pwd_edit_ele(int hWin)
{
    WM_HMEM handle = 0;
    uint8_t cur;
    
    handle = g_cur_edit_ele->dis.edit.handle;
    cur = EDIT_GetCursorCharPos(handle);
    
    del_a_char_from_edit_str(cur);
    menu_key_backspace(hWin);
}
/**
  * @brief  原始密码和新密码共用的功能键F1的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f1_cb(KEY_MESSAGE *key_msg)
{
    backspace_pwd_edit_ele(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F2的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f2_cb(KEY_MESSAGE *key_msg)
{
    clear_pwd_edit_ele(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F3的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  原始密码和新密码共用的功能键F4的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  原始密码和新密码共用的功能键F5的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f5_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);
}
/**
  * @brief  原始密码和新密码共用的功能键F6的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void old_new_pwd_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  确认密码使用的功能键F5的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void confirm_pwd_win_f5_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);
}

/**
  * @brief  原始密码和新密码的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pwd_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = o_n_pwd_menu_key_info;
    uint32_t size = ARRAY_SIZE(o_n_pwd_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  确认密码使用的菜单键初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void c_pwd_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = c_pwd_menu_key_info;
    uint32_t size = ARRAY_SIZE(c_pwd_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  检查原始密码
  * @param  无
  * @retval 无
  */
static void check_old_pwd(void)
{
    uint8_t *str[][2]=
    {
        {"密码正确","Correct"},
        {"密码不正确","Incorrect"},
    };
    
    // 密码正确
    if(0 == strcmp((const char*)sys_par.password, (const char*)pdw_inf.old_pwd))
    {
        pdw_inf.old_pwd_flag = CS_TRUE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_OLD_NOTICE, g_cur_win, GUI_BLACK);
        update_text_ele((CS_INDEX)PWD_UI_OLD_NOTICE, g_cur_win, str[0][SYS_LANGUAGE]);
    }
    // 密码错误
    else
    {
        pdw_inf.old_pwd_flag = CS_FALSE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_OLD_NOTICE, g_cur_win, GUI_RED);
        update_text_ele((CS_INDEX)PWD_UI_OLD_NOTICE, g_cur_win, str[1][SYS_LANGUAGE]);
    }
}

/**
  * @brief  检查新设密码
  * @param  无
  * @retval 无
  */
static void check_new_pwd(void)
{
    uint8_t *str[][2]=
    {
        {"格式正确","Correct"},
        {"不能为空","Non empty"},
    };
    
    // 密码格式正确
    if(0 != strcmp("", (const char*)pdw_inf.new_pwd))
    {
        pdw_inf.new_pwd_flag = CS_TRUE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_NEW_NOTICE, g_cur_win, GUI_BLACK);
        update_text_ele((CS_INDEX)PWD_UI_NEW_NOTICE, g_cur_win, str[0][SYS_LANGUAGE]);
    }
    // 密码格式错误
    else
    {
        pdw_inf.new_pwd_flag = CS_FALSE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_NEW_NOTICE, g_cur_win, GUI_RED);
        update_text_ele((CS_INDEX)PWD_UI_NEW_NOTICE, g_cur_win, str[1][SYS_LANGUAGE]);
    }
}
/**
  * @brief  检查确认密码
  * @param  无
  * @retval 无
  */
static void check_confirm_pwd(void)
{
    uint8_t *str[][2]=
    {
        {"密码正确","Correct"},
        {"密码不一致","Atypism"},
    };
    
    // 密码格式正确
    if(0 == strcmp((const char*)pdw_inf.new_pwd, (const char*)pdw_inf.confirm_pwd))
    {
        pdw_inf.confirm_pwd_flag = CS_TRUE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_CONFIRM_NOTICE, g_cur_win, GUI_BLACK);
        update_text_ele((CS_INDEX)PWD_UI_CONFIRM_NOTICE, g_cur_win, str[0][SYS_LANGUAGE]);
    }
    // 密码格式错误
    else
    {
        pdw_inf.confirm_pwd_flag = CS_FALSE;
        set_text_ele_font_color((CS_INDEX)PWD_UI_CONFIRM_NOTICE, g_cur_win, GUI_RED);
        update_text_ele((CS_INDEX)PWD_UI_CONFIRM_NOTICE, g_cur_win, str[1][SYS_LANGUAGE]);
    }
}
/**
  * @brief  清空检查原始密码的提示信息
  * @param  无
  * @retval 无
  */
static void clear_check_old_pwd_notice(void)
{
    update_text_ele((CS_INDEX)PWD_UI_OLD_NOTICE, g_cur_win, "");
}
/**
  * @brief  清空检查新设密码的提示信息
  * @param  无
  * @retval 无
  */
static void clear_check_new_pwd_notice(void)
{
    update_text_ele((CS_INDEX)PWD_UI_NEW_NOTICE, g_cur_win, "");
}
/**
  * @brief  清空检查确认密码的提示信息
  * @param  无
  * @retval 无
  */
static void clear_check_confirm_pwd_notice(void)
{
    update_text_ele((CS_INDEX)PWD_UI_CONFIRM_NOTICE, g_cur_win, "");
}
/**
  * @brief  清空设置密码结果的提示信息
  * @param  无
  * @retval 无
  */
static void clear_change_pwd_result_notice(void)
{
    update_text_ele((CS_INDEX)PWD_UI_CHANGE_RESULT, g_cur_win, "");
}
/**
  * @brief  向上功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, UNLOAD_TO_RAM);
    
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, EDIT_ELE_T, e_list);
    }
    
    select_edit_ele(g_cur_edit_ele);
    
    /* 清除密码提示信息 */
    if(g_cur_edit_ele->index == PASSWORD_OLD)
    {
        clear_check_old_pwd_notice();
        clear_check_new_pwd_notice();
        clear_check_confirm_pwd_notice();
        clear_change_pwd_result_notice();
    }
    else if(g_cur_edit_ele->index == PASSWORD_NEW)
    {
        clear_check_new_pwd_notice();
        clear_check_confirm_pwd_notice();
    }
}

/**
  * @brief  向下功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    uint8_t flag = 0;
    
    dis_select_edit_ele(g_cur_edit_ele, UNLOAD_TO_RAM);
    
    /* 检查原始密码 */
    if(g_cur_edit_ele->index == PASSWORD_OLD)
    {
        check_old_pwd();
        if(pdw_inf.old_pwd_flag == CS_TRUE)
        {
            flag = 1;
        }
    }
    else if(g_cur_edit_ele->index == PASSWORD_NEW)
    {
        check_new_pwd();
        if(pdw_inf.new_pwd_flag == CS_TRUE)
        {
            flag = 1;
        }
    }
    
    if(flag == 1)
    {
        if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
        {
            g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, EDIT_ELE_T, e_list);
        }
    }
    
    select_edit_ele(g_cur_edit_ele);
}


/**
  * @brief  向左功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_left_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
/**
  * @brief  向右功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}

/**
  * @brief  密码设置窗口中系统功能键的初始化
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void pwd_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}
/**
  * @brief  显示密码设置成功提示信息
  * @param  无
  * @retval 无
  */
static void dis_set_pwd_succeed(void)
{
    uint8_t *str[2] = {"密码修改成功.","Password changes succeeded."};
    
    //密码修改成功
    set_text_ele_font_color((CS_INDEX)PWD_UI_CHANGE_RESULT, g_cur_win, GUI_RED);
    update_text_ele((CS_INDEX)PWD_UI_CHANGE_RESULT, g_cur_win, str[SYS_LANGUAGE]);
}
/**
  * @brief  设置密码时按下确认键后调用这个函数，进行密码合法性判断，如果合法就保存密码
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_ok(int hWin)
{
    if(pdw_inf.old_pwd_flag == CS_TRUE && pdw_inf.new_pwd_flag == CS_TRUE)
    {
        check_confirm_pwd();
        
        if(pdw_inf.confirm_pwd_flag == CS_TRUE)
        {
            dis_set_pwd_succeed();//显示密码修改成功
            memcpy(sys_par.password, pdw_inf.new_pwd, sizeof(sys_par.password));
            save_sys_par();//保存新设密码
        }
    }
}

/**
  * @brief  设置密码窗口中编辑对象的数据指针
  * @param  [in] pwd 密码结构信息
  * @retval 无
  */
static void set_pwd_par_window_ele_data(PWD_T *pwd)
{
    set_edit_ele_data(&password_ele_pool[PASSWORD_OLD], pwd->old_pwd);
    set_edit_ele_data(&password_ele_pool[PASSWORD_NEW], pwd->new_pwd);
    set_edit_ele_data(&password_ele_pool[PASSWORD_CONFIRM], pwd->confirm_pwd);
}
/**
  * @brief  初始化并创建密码编辑对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_pwd_edit_ele(MYUSER_WINDOW_T* win)
{
    set_pwd_par_window_ele_data(&pdw_inf);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, pwd_edit_ele_auto_layout[sys_par.screem_size]);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}
/**
  * @brief  初始化其他的文本对象的显示信息
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_window_other_text_ele_dis_inf(MYUSER_WINDOW_T* win)
{
    password_ui_text_ele_pool[PWD_UI_CHANGE_RESULT].dis_info.base_x = 0;
    password_ui_text_ele_pool[PWD_UI_CHANGE_RESULT].dis_info.pos_size.x = 0;
    password_ui_text_ele_pool[PWD_UI_CHANGE_RESULT].dis_info.pos_size.width = win->pos_size.width;
    password_ui_text_ele_pool[PWD_UI_CHANGE_RESULT].dis_info.align = GUI_TA_CENTER;
}
/**
  * @brief  初始化并创建窗口中的文本对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_pwd_text_ele(MYUSER_WINDOW_T* win)
{
    init_window_text_ele_list(win);
    init_window_text_ele_dis_inf(win, pwd_text_ele_auto_layout[sys_par.screem_size]);
    init_window_other_text_ele_dis_inf(win);
    init_window_text_ele(win);
}
/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 窗口结构信息
  * @retval 无
  */
static void init_create_pwd_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

/**
  * @brief  清空密码数据，清空历史数据
  * @param  无
  * @retval 无
  */
static void clear_pwd_data(void)
{
    memset(&pdw_inf, 0, sizeof(pdw_inf));
}
/**
  * @brief  密码窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void password_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            
            init_dialog(win);
            clear_pwd_data();
            
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();
            if(g_cur_edit_ele != NULL)
            {
                select_edit_ele(g_cur_edit_ele);//选中当前编辑对象
            }
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			break;
		case WM_NOTIFY_PARENT:
			break;
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
		default:
			WM_DefaultProc(pMsg);
	}
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建密码窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_password_ui(int hWin)
{
    init_window_size(&password_windows, password_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_dialog(&password_windows, &windows_list, hWin);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
