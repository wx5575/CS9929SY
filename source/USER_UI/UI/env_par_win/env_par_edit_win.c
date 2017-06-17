
/**
  ******************************************************************************
  * @file    env_par_edit_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   环境变量编辑窗口
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
#include "cs99xx_mem_api.h"
#include "UI_COM/com_edit_api.h"
#include "UI_COM/com_ui_info.h"
#include "file_win/file_edit_win.h"
#include "key_menu_win/key_menu_win.h"
#include "7_env_par_edit_win.h"
#include "env_par_edit_win.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
    ENV_PAR_LANGUAGE,
}ENV_PAR_UI_INDEX;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void env_par_edit_win_cb(WM_MESSAGE * pMsg);
static void env_par_direct_key_up_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_left_cb(KEY_MESSAGE *key_msg);
static void env_par_direct_key_right_cb(KEY_MESSAGE *key_msg);
static void menu_key_ok(WM_HMEM hWin);

static void edit_language_f1_cb(KEY_MESSAGE *key_msg);
static void edit_language_f2_cb(KEY_MESSAGE *key_msg);
static void edit_language_f3_cb(KEY_MESSAGE *key_msg);
static void edit_language_f4_cb(KEY_MESSAGE *key_msg);
static void edit_language_f5_cb(KEY_MESSAGE *key_msg);
static void edit_language_f6_cb(KEY_MESSAGE *key_msg);

static void init_create_env_par_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_env_par_win_edit_ele(MYUSER_WINDOW_T* win);
static void reg_env_language_sys_key(WM_HMEM hWin);
static void env_language_menu_key(WM_HMEM hWin);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  环境变量设置窗口的位置尺寸信息
  */
static WIDGET_POS_SIZE_T* env_par_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_env_par_windows,/*4.3寸屏*/
    &_7_env_par_windows,/*5.6寸屏*/
    &_7_env_par_windows,/*7寸屏*/
};

/**
  * @brief  编辑语言使用的菜单键初始化信息
  */
static MENU_KEY_INFO_T 	env_par_language_menu_key_inf[] =
{
    {"中文"     , F_KEY_CUSTOM, KEY_F1 & _KEY_UP, edit_language_f1_cb },//f1
    {"English"  , F_KEY_CUSTOM, KEY_F2 & _KEY_UP, edit_language_f2_cb },//f2
    {""         , F_KEY_NULL  , KEY_F3 & _KEY_UP, edit_language_f3_cb },//f3
    {""         , F_KEY_NULL  , KEY_F4 & _KEY_UP, edit_language_f4_cb },//f4
    {""         , F_KEY_NULL  , KEY_F5 & _KEY_UP, edit_language_f5_cb },//f3
    {""         , F_KEY_BACK  , KEY_F6 & _KEY_UP, edit_language_f6_cb },//f4
};

/**
  * @brief  环境变量窗口系统按键初始化数组
  */
static FUNCTION_KEY_INFO_T env_par_sys_key_pool[]={
	{KEY_UP		, env_par_direct_key_up_cb		 },
	{KEY_DOWN	, env_par_direct_key_down_cb	 },
	{KEY_LEFT	, env_par_direct_key_left_cb	 },
	{KEY_RIGHT	, env_par_direct_key_right_cb	 },
    
	{CODE_LEFT	, env_par_direct_key_up_cb      },
	{CODE_RIGH	, env_par_direct_key_down_cb	 },
};

/**
  * @brief  环境变量窗口显示的编辑对象池
  */
static EDIT_ELE_T env_par_ele_pool[]={
    {
        {"语  言:","Language:"}, /* 名称 */
        ENV_PAR_LANGUAGE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {language_pool, ARRAY_SIZE(language_pool)},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {reg_env_language_sys_key,env_language_menu_key,keyboard_fun_num,},/*key_inf*/
    },
//     {
//         {"蜂鸣时间:","BeepTime:"}, /* 名称 */
//         FSAVE_UI_BEEPT,/* 通过枚举索引 */
//         {0},/* 默认值 */
//         {NULL, 2/*数据字节数*/},/* 数据指针 */
//         {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
//         {ELE_EDIT_NUM, E_INT_T},/*类型*/
//         {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
//         {9999/*heigh*/,0/*low*/,{"BeepTime","BeepTime"}/*notice*/},/*range*/
//         {fbeeptime_sys_key,fbeeptime_menu_key,keyboard_fun_num,},/*key_inf*/
//         &save_file_ele_pos_pool[FSAVE_UI_BEEPT],/*dis*/
//     },
//     {
//         {"PASS时间:","PassTime:"}, /* 名称 */
//         FSAVE_UI_PASST,/* 通过枚举索引 */
//         {0},/* 默认值 */
//         {NULL, 2/*数据字节数*/},/* 数据指针 */
//         {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
//         {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
//         {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
//         {9999/*heigh*/,0/*low*/,{"PassTime","PassTime"}/*notice*/},/*range*/
//         {fpasstime_sys_key,fpasstime_menu_key,keyboard_fun_num,},/*key_inf*/
//         &save_file_ele_pos_pool[FSAVE_UI_PASST],/*dis*/
//     },
};

/**
  * @brief  环境变量窗口显示的编辑对象索引表
  */
static CS_INDEX env_par_ui_ele_table[]=
{
    ENV_PAR_LANGUAGE,
};

/**
  * @brief  环境变量窗口结构定义
  */
static MYUSER_WINDOW_T env_par_window=
{
    {"环境参数","Env.Par"},
    env_par_edit_win_cb,NULL,
    {0},
    {
        env_par_ele_pool,ARRAY_SIZE(env_par_ele_pool),
        (CS_INDEX*)env_par_ui_ele_table,ARRAY_SIZE(env_par_ui_ele_table),
        init_create_env_par_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_com_ele_table,ARRAY_SIZE(range_com_ele_table),
        init_create_env_par_win_com_ele,
    },
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  设置系统语言为中文
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_env_language_chinese(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = CHINESE;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
}

/**
  * @brief  设置系统语言为英文
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_env_language_english(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = ENGLISH;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
}

/**
  * @brief  编辑语言时功能键F1的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f1_cb(KEY_MESSAGE *key_msg)
{
    set_env_language_chinese(key_msg->user_data);
}
/**
  * @brief  编辑语言时功能键F2的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f2_cb(KEY_MESSAGE *key_msg)
{
    set_env_language_english(key_msg->user_data);
}
/**
  * @brief  编辑语言时功能键F3的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F4的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F5的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  编辑语言时功能键F6的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void edit_language_f6_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);
}
/**
  * @brief  向上键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, EDIT_ELE_T, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}
/**
  * @brief  向下键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, EDIT_ELE_T, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}
/**
  * @brief  向左键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_left_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
/**
  * @brief  向右键功能键的回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void env_par_direct_key_right_cb(KEY_MESSAGE *key_msg)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}

/**
  * @brief  注册编辑语言时功能键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void reg_env_language_sys_key(WM_HMEM hWin)
{
    register_system_key_fun(env_par_sys_key_pool, ARRAY_SIZE(env_par_sys_key_pool), hWin);
}
/**
  * @brief  注册编辑语言时菜单键
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void env_language_menu_key(WM_HMEM hWin)
{
    MENU_KEY_INFO_T * info = env_par_language_menu_key_inf;
    uint32_t size = ARRAY_SIZE(env_par_language_menu_key_inf);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}
/**
  * @brief  按下确认键后调用这个函数进行保存数据并返回上级窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_ok(WM_HMEM hWin)
{
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    save_sys_par();//保存系统参数
    unregister_system_key_fun(env_par_sys_key_pool, ARRAY_SIZE(env_par_sys_key_pool));
    back_win(hWin);//关闭对话框
}

/**
  * @brief  设置环境变量参数的编辑对象数据
  * @param  [in] par 系统参数
  * @retval 无
  */
static void set_env_par_window_ele_data(SYS_PAR *par)
{
    set_edit_ele_data(&env_par_ele_pool[ENV_PAR_LANGUAGE], &par->language);
}

/**
  * @brief  初始化并创建编辑对象
  * @param  [in] win 窗口结构数据
  * @retval 无
  */
static void init_create_env_par_win_edit_ele(MYUSER_WINDOW_T* win)
{
    set_env_par_window_ele_data(&sys_par);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_env_par_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}

/**
  * @brief  初始化并创建公共文本对象
  * @param  [in] win 窗口结构数据
  * @retval 无
  */
static void init_create_env_par_win_com_ele(MYUSER_WINDOW_T* win)
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
static void env_par_edit_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_EDIT_WIDGET_CHANGE:
		{
            win = get_user_window_info(hWin);
            update_win_menu_key(win);
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
            select_edit_ele(g_cur_edit_ele);
            break;
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
        case WM_KEY:
        {
            break;
        }
        default:
            WM_DefaultProc(pMsg);
    }
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建对话框用来编辑环境变量
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_env_par_dialog(int hWin)
{
    init_window_size(&env_par_window, env_par_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&env_par_window, &windows_list, hWin);//创建主界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
