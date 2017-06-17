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
#include "cs99xx_mem_api.h"
#include "UI_COM/com_ui_info.h"
#include "KEY_MENU_WIN/key_menu_win.h"
#include "WARNING_WIN/warning_win.h"
#include "7_result_win.h"
#include "result_win.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色

/* Private function prototypes -----------------------------------------------*/

static LISTVIEW_Handle list_h;
static void result_win_cb(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);
//void pop_warning_dialog(int hWin);
//void into_save_file_dialog(int hWin);
static void result_exist_f1_cb(KEY_MESSAGE *key_msg);
static void result_exist_f2_cb(KEY_MESSAGE *key_msg);
static void result_exist_f3_cb(KEY_MESSAGE *key_msg);
static void result_exist_f4_cb(KEY_MESSAGE *key_msg);
static void result_exist_f5_cb(KEY_MESSAGE *key_msg);
static void result_exist_f6_cb(KEY_MESSAGE *key_msg);

static void result_no_exist_f1_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f2_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f3_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f4_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f5_cb(KEY_MESSAGE *key_msg);
static void result_no_exist_f6_cb(KEY_MESSAGE *key_msg);
static void direct_key_up_cb(KEY_MESSAGE *key_msg);
static void direct_key_down_cb(KEY_MESSAGE *key_msg);
static void update_cur_row_menu_key_st(WM_HWIN hWin);

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  结果管理窗口位置尺寸信息,根据不同屏幕尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* result_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_result_windows,/*4.3寸屏*/
    &_7_result_windows,/*5.6寸屏*/
    &_7_result_windows,/*7寸屏*/
};

MYUSER_WINDOW_T result_windows =
{
    {"File_window"},
    result_win_cb,update_menu_key_inf,
	0,0, 0,
};

static MENU_KEY_INFO_T 	result_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, result_exist_f1_cb },//f1
    {"", F_KEY_READ		, KEY_F2 & _KEY_UP, result_exist_f2_cb },//f2
    {"", F_KEY_EDIT		, KEY_F3 & _KEY_UP, result_exist_f3_cb },//f3
    {"", F_KEY_DEL		, KEY_F4 & _KEY_UP, result_exist_f4_cb },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP, result_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, result_exist_f6_cb },//f6
};
static MENU_KEY_INFO_T 	result_no_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP, result_no_exist_f1_cb },//f1
    {"", F_KEY_NEW		, KEY_F2 & _KEY_UP, result_no_exist_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, result_no_exist_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, result_no_exist_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, result_no_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, result_no_exist_f6_cb },//f6
};
static FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up_cb		},
	{KEY_DOWN	, direct_key_down_cb 	},
	{CODE_LEFT	, direct_key_down_cb	 },
	{CODE_RIGH	, direct_key_up_cb      },
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  结果存在菜单功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果存在菜单功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}


/**
  * @brief  结果不存在菜单功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  结果不存在菜单功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void result_no_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  向上功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_up_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_DecSel(list_h);
    update_cur_row_menu_key_st(key_msg->user_data);
}
/**
  * @brief  向下功能键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_down_cb(KEY_MESSAGE *key_msg)
{
	LISTVIEW_IncSel(list_h);
    update_cur_row_menu_key_st(key_msg->user_data);
}

/**
  * @brief  更新功能键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}
/**
  * @brief  更新功能键和菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}
/**
  * @brief  更新当前行的菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 步骤存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        size = ARRAY_SIZE(result_exist_menu_key_info);
        info = result_exist_menu_key_info;
	}
	/* 步骤不存在 */
	else
	{
        size = ARRAY_SIZE(result_no_exist_menu_key_info);
        info = result_no_exist_menu_key_info;
	}
    
    
	init_menu_key_info(info, size, hWin);//刷新菜单键显示
}
/**
  * @brief  更新菜单键信息
  * @param  无
  * @retval 无
  */
static void update_menu_key_inf(WM_HMEM hWin)
{
    update_cur_row_menu_key_st(hWin);
}
/**
  * @brief  窗口重绘
  * @param  无
  * @retval 无
  */
static void result_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}



/**
  * @brief  创建结果列表
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static WM_HWIN create_result_listview(WM_HWIN hWin)
{
    WM_HWIN handle = 0;
    
    switch(sys_par.screem_size)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            handle = _7_create_result_listview(hWin);
            break;
    }
    
    return handle;
}
/**
  * @brief  结果管理窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void result_win_cb(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		/*自定义消息 当子窗口被关闭后 收到由子窗口发给父窗口的消息 */
		case CM_CHILD_W_MSG:
		{
//			msg = *(CUSTOM_MSG_T*)pMsg->Data.v;//拷贝消息
//             WM_SendMessageNoPara(hWin, CM_W_DIS_MSG);
            
//			dispose_child_win_msg(&msg, hWin);
			break;
		}
		case WM_CREATE:
			WM_SetFocus(hWin);/* 设置聚焦 */
            list_h = create_result_listview(hWin);
            update_key_inf(hWin);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			result_win_paint_frame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建结果管理窗口
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
void create_result_win(int hWin)
{
    init_window_size(&result_windows, result_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&result_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
