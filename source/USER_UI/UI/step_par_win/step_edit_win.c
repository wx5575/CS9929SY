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
#include "warning_win/warning_win.h"
#include "cs99xx_mem_api.h"
#include "ui_com/com_edit_api.h"
#include "env_par_win/env_par_edit_win.h"
#include "sys_time_win/sys_time_edit_win.h"
#include "password_win/password_win.h"
#include "step_par_win/step_edit_win.h"
#include "step_par_win/7_step_edit_win.h"

static void cb_step_edit_windows(WM_MESSAGE* pMsg);
static void update_key_inf(WM_HMEM hWin);
static void direct_key_down(int data);
static void direct_key_up(int data);

static WIDGET_POS_SIZE_T* step_edit_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_edit_windows,/*4.3寸屏*/
    &_7_step_edit_windows,/*5.6寸屏*/
    &_7_step_edit_windows,/*7寸屏*/
};

/* 系统界面下按键菜单 */
static MENU_KEY_INFO_T 	step_edit_menu_key_info[] =
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP,	0 },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP,	0 },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP,	0 },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP,	0 },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP,	0 },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win },//f6
};


static FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
	{CODE_LEFT	, direct_key_down   },
	{CODE_RIGH	, direct_key_up     },
};
typedef enum{
    STEP_EDIT_UI_STEP,///<测试步骤
    STEP_EDIT_UI_MODE,///<测试模式
    STEP_EDIT_UI_END,
}STEP_EDIT_UI_INDEX;

static CS_INDEX step_par_index[]=
{
    STEP_EDIT_UI_STEP,
    STEP_EDIT_UI_MODE,
};

static void step_edit_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}

static void step_edit_menu_key()
{
    MENU_KEY_INFO_T * info = step_edit_menu_key_info;
    uint32_t size = ARRAY_SIZE(step_edit_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}

static _WIDGET_ELEMENT_ step_par_ele_pool[]={
    {
        {"测试步骤:","TestStep:"}, /* 名称 */
        STEP_EDIT_UI_STEP,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {step_edit_sys_key,step_edit_menu_key,keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试模式:","TestMode:"}, /* 名称 */
        STEP_EDIT_UI_MODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {MODE_END/*heigh*/,ACW/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {step_edit_sys_key,step_edit_menu_key,keyboard_fun_num,},/*key_inf*/
    },
//    {
//        {"测试模式:","TestMode:"}, /* 名称 */
//        STEP_EDIT_UI_MODE,/* 通过枚举索引 */
//        {0},/* 默认值 */
//        {NULL, 1/*数据字节数*/},/* 数据指针 */
//        {language_pool, ARRAY_SIZE(language_pool)},/* 资源表 */
//        {ELE_DROPDOWN, E_INT_T},/*类型*/
//        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
//        {0/*heigh*/,0/*low*/,{"Language","Language"}/*notice*/},/*range*/
//        {env_langulag_sys_key,env_langulag_menu_key,keyboard_fun_num,},/*key_inf*/
//    },
};

static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win);

static MYUSER_WINDOW_T step_edit_windows=
{
    {"步骤编辑","step_edit_windows"},
    cb_step_edit_windows, update_key_inf,
	{0},
    {
        step_par_ele_pool,ARRAY_SIZE(step_par_ele_pool),
        (CS_INDEX*)step_par_index,ARRAY_SIZE(step_par_index),
        init_create_file_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_group_com_ele_table,ARRAY_SIZE(range_group_com_ele_table),
        init_create_file_edit_win_com_ele,
    },
};

static UN_STRUCT step_par;
void set_step_par_window_ele_data(UN_STRUCT *step)
{
    
}

static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    set_step_par_window_ele_data(&step_par);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_step_par_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}

static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

static void update_menu_key_inf(WM_HMEM hWin)
{
	step_edit_menu_key();
}

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_WHITE);
	GUI_ClearRectEx(&r);
}


static void direct_key_up(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

static void direct_key_down(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}

static void cb_step_edit_windows(WM_MESSAGE* pMsg)
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
		 case WM_KEY:
            break;
		case WM_PAINT:
			_PaintFrame();
            draw_group_inf_area();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

void create_step_edit_ui(int id)
{
    init_window_size(&step_edit_windows, step_edit_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&step_edit_windows, &windows_list);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
