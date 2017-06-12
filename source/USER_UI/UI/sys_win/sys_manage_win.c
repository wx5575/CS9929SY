/**
  ******************************************************************************
  * @file    sys_manage_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.18
  * @brief   系统管理界面
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
#include "restore_setting_win/restore_setting_win.h"

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  系统窗口文本控件索引枚举定义
  */
typedef enum{
    SYS_UI_ENV_PAR,///<环境参数
    SYS_UI_TIME,
    SYS_UI_RIGHT,///<系统权限
    SYS_UI_CAL,///<系统校准
    SYS_UI_MODULE,///<终统模块管理
    SYS_UI_RE_FACT_SETTING,///<恢复出厂设置
    SYS_UI_END,
}SYS_WIN_TEXT_INDEX_ENUM;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void sys_manage_win_cb(WM_MESSAGE* pMsg);
static void update_sys_fun_key_inf(WM_HMEM hWin);
static FUNCTION_KEY_INFO_T 	sys_win_sys_key_pool[];
static void update_sys_fun_key_inf(WM_HWIN hWin);
static void into_sub_set_win(int hWin);

static void sys_win_f1_cb(KEY_MESSAGE *key_msg);
static void sys_win_f2_cb(KEY_MESSAGE *key_msg);
static void sys_win_f3_cb(KEY_MESSAGE *key_msg);
static void sys_win_f4_cb(KEY_MESSAGE *key_msg);
static void sys_win_f5_cb(KEY_MESSAGE *key_msg);
static void sys_win_f6_cb(KEY_MESSAGE *key_msg);
static void sys_win_direct_key_down_cb(KEY_MESSAGE *key_msg);
static void sys_win_direct_key_up_cb(KEY_MESSAGE *key_msg);
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  系统窗口位置尺寸，与屏幕尺寸相关的数组
  */
static WIDGET_POS_SIZE_T* sys_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_sys_windows,/*4.3寸屏*/
    &_7_sys_windows,/*5.6寸屏*/
    &_7_sys_windows,/*7寸屏*/
};
/**
  * @brief  系统窗口文本索引表
  */
static CS_INDEX sys_win_text_index_table[]=
{
    SYS_UI_ENV_PAR,// 环境参数
    SYS_UI_TIME,
    SYS_UI_RIGHT,// 系统权限
    SYS_UI_CAL,// 系统校准
    SYS_UI_MODULE,// 终统模块管理
    SYS_UI_RE_FACT_SETTING,// 恢复出厂设置
    SYS_UI_END,
};
/**
  * @brief  系统窗口文本控件池
  */
static TEXT_ELE_T sys_ui_ele_pool[]=
{
    {{"环境参数"    ,"Env. Par."         }, SYS_UI_ENV_PAR        },
	{{"系统时间"    ,"System Time"       }, SYS_UI_TIME           },
    {{"系统权限"    ,"System Privileges" }, SYS_UI_RIGHT          },
    {{"校准管理"    ,"Cal. Management"   }, SYS_UI_CAL            },
    {{"模块管理"    ,"Module Management" }, SYS_UI_MODULE         },
    {{"恢复出厂设置","Rest.Fact.Settings"}, SYS_UI_RE_FACT_SETTING},
};

/**
  * @brief  系统窗口菜单键信息初始化数组
  */
static MENU_KEY_INFO_T 	sys_manage_menu_key_inf[] =
{
    {"", F_KEY_NULL		, KEY_F1 & _KEY_UP, sys_win_f1_cb },//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, sys_win_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, sys_win_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, sys_win_f4_cb },//f4
    {"", F_KEY_OK		, KEY_F5 & _KEY_UP, sys_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, sys_win_f6_cb },//f6
};
/**
  * @brief  系统窗口功能键信息初始化数组
  */
static FUNCTION_KEY_INFO_T 	sys_win_sys_key_pool[]={
	{KEY_UP		, sys_win_direct_key_up_cb		},
	{KEY_DOWN	, sys_win_direct_key_down_cb 	},
	{CODE_LEFT	, sys_win_direct_key_down_cb   },
	{CODE_RIGH	, sys_win_direct_key_up_cb     },
};
/**
  * @brief  系统窗口结构定义
  */
static MYUSER_WINDOW_T sys_windows=
{
    {"系统管理", "SysManage"},
    sys_manage_win_cb, update_sys_fun_key_inf,
	{
        sys_ui_ele_pool, ARRAY_SIZE(sys_ui_ele_pool),
        (CS_INDEX*)sys_win_text_index_table,ARRAY_SIZE(sys_win_text_index_table)
    },
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  菜单键f1的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f2的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f3的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f4的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  菜单键f5的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f5_cb(KEY_MESSAGE *key_msg)
{
    into_sub_set_win(key_msg->user_data);
}
/**
  * @brief  菜单键f6的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  向上键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_text_ele(g_cur_text_ele);
    
    if(&g_cur_win->text.list_head != g_cur_text_ele->list.prev)
    {
        g_cur_text_ele = list_entry(g_cur_text_ele->list.prev, TEXT_ELE_T, list);
    }
    
    select_text_ele(g_cur_text_ele);
}

/**
  * @brief  向下键的回调函数
  * @param  [in] key_msg 按键信息
  * @retval 无
  */
static void sys_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    dis_select_text_ele(g_cur_text_ele);
    
    if(&g_cur_win->text.list_head != g_cur_text_ele->list.next)
    {
        g_cur_text_ele = list_entry(g_cur_text_ele->list.next, TEXT_ELE_T, list);
    }
    
    select_text_ele(g_cur_text_ele);
}

/**
  * @brief  进入子设置窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
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

/**
  * @brief  系统管理窗口重绘
  * @param  无
  * @retval 无
  */
static void sys_manage_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}
/**
  * @brief  更新系统功能键信息
  * @param  hWin 窗口句柄
  * @retval 无
  */
static void update_sys_fun_key_inf(WM_HWIN hWin)
{
    init_menu_key_info(sys_manage_menu_key_inf, ARRAY_SIZE(sys_manage_menu_key_inf), hWin);//刷新菜单键显示
    register_system_key_fun(sys_win_sys_key_pool, ARRAY_SIZE(sys_win_sys_key_pool), hWin);
}

/**
  * @brief  系统管理窗口回调函数
  * @param  pMsg 窗口消息
  * @retval 无
  */
static void sys_manage_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case CM_CHILD_W_MSG:
        {
            update_sys_fun_key_inf(hWin);
            break;
        }
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            update_sys_fun_key_inf(hWin);
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
			sys_manage_win_paint_frame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建系统管理窗口
  * @param  hWin 父窗口句柄
  * @retval 无
  */
void create_sys_manager_win(int hWin)
{
    init_window_size(&sys_windows, sys_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&sys_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
