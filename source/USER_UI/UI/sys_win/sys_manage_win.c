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
#include "7_sys_manage_win.h"
#include "ui_com/com_edit_api.h"
#include "env_par_win/env_par_edit_win.h"
#include "sys_time_win/sys_time_edit_win.h"
#include "password_win/password_win.h"

static void cb_sys_windows(WM_MESSAGE* pMsg);
static void update_key_inf(WM_HMEM hWin);
static FUNCTION_KEY_INFO_T 	sys_key_pool[];

extern WIDGET_POS_SIZE_T _7_sys_windows;
static WIDGET_POS_SIZE_T* sys_win_pos_size_pool[4]=
{
    &_7_sys_windows,/*4.3寸屏*/
    &_7_sys_windows,/*5.6寸屏*/
    &_7_sys_windows,/*7寸屏*/
};

typedef enum{
    SYS_UI_ENV_PAR,///<环境参数
    SYS_UI_TIME,
    SYS_UI_RIGHT,///<系统权限
    SYS_UI_CAL,///<系统校准
    SYS_UI_MODULE,///<终统模块管理
    SYS_UI_RE_FACT_SETTING,///<恢复出厂设置
    SYS_UI_END,
}SYS_UI_INDEX;

CS_INDEX sys_ui_index[]=
{
    SYS_UI_ENV_PAR,// 环境参数
    SYS_UI_TIME,
    SYS_UI_RIGHT,// 系统权限
    SYS_UI_CAL,// 系统校准
    SYS_UI_MODULE,// 终统模块管理
    SYS_UI_RE_FACT_SETTING,// 恢复出厂设置
    SYS_UI_END,
};
UI_ELE_DISPLAY_INFO_T sys_ui_ele_dis_info_pool[SYS_UI_END];
TEXT_ELE_T sys_ui_ele_pool[]=
{
    {{"环境参数"    ,"Env. Par."         }, SYS_UI_ENV_PAR        },
	{{"系统时间"    ,"System Time"       }, SYS_UI_TIME           },
    {{"系统权限"    ,"System Privileges" }, SYS_UI_RIGHT          },
    {{"校准管理"    ,"Cal. Management"   }, SYS_UI_CAL            },
    {{"模块管理"    ,"Module Management" }, SYS_UI_MODULE         },
    {{"恢复出厂设置","Rest.Fact.Settings"}, SYS_UI_RE_FACT_SETTING},
};
MYUSER_WINDOW_T sys_windows=
{
    {"sys_windows"},
    cb_sys_windows, update_key_inf,
	{
        sys_ui_ele_pool, ARRAY_SIZE(sys_ui_ele_pool),
        (CS_INDEX*)sys_ui_index,ARRAY_SIZE(sys_ui_index)
    },
};

// #define WINDOWS_BAK_COLOR	GUI_GRAY //GUI_BLUE	//窗口背景色

static void into_sub_set_win(int hWin);
/* 系统界面下按键菜单 */
static MENU_KEY_INFO_T 	sys_menu_key_info[] =
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP,	0 },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP,	0 },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP,	0 },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP,	0 },//f4
    {"", F_KEY_OK		, KEY_F5 & _KEY_UP,	into_sub_set_win },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win },//f6
};
static void into_sub_set_win(int hWin)
{
    switch(g_cur_text_ele->index)
    {
        case SYS_UI_ENV_PAR:
            create_env_par_dialog(hWin);
            break;
        case SYS_UI_TIME:
            create_sys_time_dialog(hWin);
            break;
        case SYS_UI_RIGHT:
            create_password_ui(hWin);
            break;
        case SYS_UI_CAL:
            break;
        case SYS_UI_MODULE:
            break;
        case SYS_UI_RE_FACT_SETTING:
            create_restor_factory_setting_dialog(hWin);
            break;
    }
}
static void update_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(sys_menu_key_info, ARRAY_SIZE(sys_menu_key_info), hWin);//刷新菜单键显示
}

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

static void _create_dialog_(WM_HWIN hWin)
{
    
}

static void direct_key_up(int data)
{
    dis_select_text_ele(g_cur_text_ele);
    
    if(&g_cur_win->text.list_head != g_cur_text_ele->list.prev)
    {
        g_cur_text_ele = list_entry(g_cur_text_ele->list.prev, TEXT_ELE_T, list);
    }
    
    select_text_ele(g_cur_text_ele);
}

static void direct_key_down(int data)
{
    dis_select_text_ele(g_cur_text_ele);
    
    if(&g_cur_win->text.list_head != g_cur_text_ele->list.next)
    {
        g_cur_text_ele = list_entry(g_cur_text_ele->list.next, TEXT_ELE_T, list);
    }
    
    select_text_ele(g_cur_text_ele);
}

static FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
	{CODE_LEFT	, direct_key_down   },
	{CODE_RIGH	, direct_key_up     },
};

static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}

static void cb_sys_windows(WM_MESSAGE* pMsg)
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
			_create_dialog_(hWin);
            update_key_inf(hWin);
            init_window_text_ele_list(win);
            init_window_text_ele_dis_inf(win, &_7_sys_auto_layout_inf);//初始化窗口文本对象链表
			init_window_text_ele(win);
            
            g_cur_text_ele = get_cur_win_text_ele_list_head();
            select_text_ele(g_cur_text_ele);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

void create_sys_manager_ui(int id)
{
    init_window_size(&sys_windows, sys_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&sys_windows, &windows_list);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
