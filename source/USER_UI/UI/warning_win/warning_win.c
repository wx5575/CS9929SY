/**
  ******************************************************************************
  * @file    warning_win.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   警告框窗口
  ******************************************************************************
  */


#include "stm32f4xx.h"
#include "GUI.H"
#include "UI_COM/com_ui_info.h"
#include "string.h"
#include "keyboard.h"
#include "7_warning_win.h"
#include "warning_win.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void menu_key_cancle(int hWin);
static void menu_key_ok(int hWin);
static MYUSER_WINDOW_T warning_windows;
static void warning_win_cb(WM_MESSAGE * pMsg);

static void warning_win_f1_cb(KEY_MESSAGE *key_msg);
static void warning_win_f2_cb(KEY_MESSAGE *key_msg);
static void warning_win_f3_cb(KEY_MESSAGE *key_msg);
static void warning_win_f4_cb(KEY_MESSAGE *key_msg);
static void warning_win_f5_cb(KEY_MESSAGE *key_msg);
static void warning_win_f6_cb(KEY_MESSAGE *key_msg);

static void update_warning_ui_menu_key_inf(WM_HMEM hWin);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  警告框使用的初始化文本，在创建警告框前由父窗口进行初始化
	*/
static WARNING_INF warning_inf=
{
    {"警告","Warning"},///<标题
    {"确定要继续吗?","Are you sure you want to continue?"},///<内容
};
/**
  * @brief  警告框使用的文本对象定义
	*/
static TEXT_ELE_T warning_ui_ele_pool[]=
{
	{{"内容","CONTENT"}, WARNING_UI_CONTENT},
};

/**
  * @brief  警告对话框显示的文本索引表
  */
static CS_INDEX warning_ui_ele_buf[] =
{
	WARNING_UI_CONTENT,//内容
};
/** 当前使用界面对应的菜单键信息与响应函数 */
MENU_KEY_INFO_T 	cur_menu_key_info_[] =
{
    {"", F_KEY_NULL     , KEY_F1 & _KEY_UP, warning_win_f1_cb },//f3
    {"", F_KEY_NULL     , KEY_F2 & _KEY_UP, warning_win_f2_cb },//f3
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, warning_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, warning_win_f4_cb },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, warning_win_f5_cb },//f5
    {"", F_KEY_CANCLE   , KEY_F6 & _KEY_UP, warning_win_f6_cb },//f6
};
/**
  * @brief  主窗口结构体初始化
  */
static MYUSER_WINDOW_T warning_windows=
{
    {"警告","WARNING"},
    warning_win_cb,update_warning_ui_menu_key_inf,
	{
        warning_ui_ele_pool,COUNT_ARRAY_SIZE(warning_ui_ele_pool),
        (CS_INDEX*)warning_ui_ele_buf,COUNT_ARRAY_SIZE(warning_ui_ele_buf)
    },
};
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  f1键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  f2键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  f3键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  f4键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  f5键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f5_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);//确定键按下
}
/**
  * @brief  f6键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void warning_win_f6_cb(KEY_MESSAGE *key_msg)
{
    menu_key_cancle(key_msg->user_data);//取消键按下
}

/**
  * @brief  更新警告框窗口菜单键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_warning_ui_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(cur_menu_key_info_, ARRAY_SIZE(cur_menu_key_info_), hWin);
}

/**
  * @brief  当按下确认键时会调用这个函数，来关闭警告框并向父窗口发送确认消息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_ok(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	g_custom_msg.user_data = (int)&global_file;
//     upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
//     unregister_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool));
    back_win(hWin);//关闭对话框
}
/**
  * @brief  当按下取消键时会调用这个函数，来关闭警告框并向父窗口发送取消消息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void menu_key_cancle(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_CANCLE;
    back_win(hWin);//关闭对话框
}

/**
  * @brief  初始化警告框的内容显示
  * @param  无
  * @retval 无
  */
static void init_warning_content(void)
{
    warning_windows.win_name[CHINESE] = warning_inf.title.text[CHINESE];
    warning_windows.win_name[ENGLISH] = warning_inf.title.text[ENGLISH];
}
/**
  * @brief  文本回调函数，用来绑定定时器，实现自动关闭警告框
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void text_ele_cb(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
    
	switch (pMsg->MsgId)
	{
		case WM_TIMER:
		{
            hWin = WM_GetParent(hWin);
            back_win(hWin);//关闭对话框
			break;
		}
    }
}
/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void warning_win_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
        {
			break;
        }
        case WM_INIT_DIALOG:
        {
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            update_warning_ui_menu_key_inf(hWin);
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
            
            //无奈对话框不支持定时器，只能绕个弯子实现定时关闭对话框
            //利用一个普通控件来挂靠定定时器
            if(0 != warning_inf.dly_auto_close)
            {
                WM_HWIN handle;
                
                handle = TEXT_CreateEx(0, 0, 1, 1, hWin, WM_CF_HIDE, 0, ++id_base, "");
                WM_SetCallback(handle, text_ele_cb);//设置控件回调函数
                WM_CreateTimer(handle, 0, warning_inf.dly_auto_close, 0);//挂靠定时器
            }
            
			break;
        }
		case WM_PAINT:
			break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
		case WM_TIMER:
		{
            back_win(hWin);//关闭对话框
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  设置警告界面显示人警告内容
  * @param  [in] warning 警告信息
  * @retval 无
  */
void set_warning_ui_inf(WARNING_INF *warning)
{
    memcpy(&warning_inf, warning, sizeof(WARNING_INF));
    
    warning_windows.win_name[CHINESE] = warning_inf.title.text[CHINESE];
    warning_windows.win_name[ENGLISH] = warning_inf.title.text[ENGLISH];
    memcpy(&warning_windows.pos_size, &warning->win_pos_size, sizeof(warning->win_pos_size));
    memcpy(&warning_ui_ele_pool[WARNING_UI_CONTENT], &warning_inf.content, sizeof(TEXT_ELE_T));
    
    warning_ui_ele_pool[WARNING_UI_CONTENT].index = WARNING_UI_CONTENT;
}
/**
  * @brief  创建警告对话框
  * @param  [in] hWin 窗口句柄，作为创建窗口的父窗口句柄
  * @retval 无
  */
void create_warning_dialog(int hWin)
{
    init_warning_content();
    create_user_dialog(&warning_windows, &windows_list, hWin);//创建主界面
}







/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
