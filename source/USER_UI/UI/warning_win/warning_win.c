/**
  ******************************************************************************
  * @file    scan_keyboard.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键扫描
  ******************************************************************************
  */


#include "warning_win.h"
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "fonts.h"
#include "app.h"
#include "scan_keyboard.h"
#include "IMAGE.H"
#include "UI_COM/com_ui_info.h"
#include "string.h"
#include "7_warning_win.h"

static void menu_key_cancle(int hWin);
static void menu_key_ok(int hWin);
static MYUSER_WINDOW_T warning_windows;
static void WarningUIcb(WM_MESSAGE * pMsg);


static WARNING_INF warning_inf=
{
    {"警告","Warning"},
    {"确定要继续吗?","Are you sure you want to continue?"},
};

/** 当前使用界面对应的菜单键信息与响应函数 */
MENU_KEY_INFO_T 	cur_menu_key_info_[] =
{
    {"", F_KEY_NULL     , KEY_F1 & _KEY_UP, 0                       },//f3
    {"", F_KEY_NULL     , KEY_F2 & _KEY_UP, 0                       },//f3
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, 0                       },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, 0                       },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, menu_key_ok             },//f5
    {"", F_KEY_CANCLE   , KEY_F6 & _KEY_UP, menu_key_cancle         },//f6
};

TEXT_ELE_T warning_ui_ele_pool[]=
{
	{{"内容","CONTENT"}, WARNING_UI_CONTENT},
};

void set_warning_ui_inf(WARNING_INF *warning)
{
    memcpy(&warning_inf, warning, sizeof(WARNING_INF));
    
    warning_windows.win_name[CHINESE] = warning_inf.title.text[CHINESE];
    warning_windows.win_name[ENGLISH] = warning_inf.title.text[ENGLISH];
    memcpy(&warning_windows.pos_size, &warning->win_pos_size, sizeof(warning->win_pos_size));
    memcpy(&warning_ui_ele_pool[WARNING_UI_CONTENT], &warning_inf.content, sizeof(TEXT_ELE_T));
    
    warning_ui_ele_pool[WARNING_UI_CONTENT].index = WARNING_UI_CONTENT;
    
    memcpy(&warning_inf, warning, sizeof(warning_inf));
}
/** 警告对话框显示的文本索引表 */
static CS_INDEX warning_ui_ele_buf[] =
{
	WARNING_UI_CONTENT,//内容
};

static void update_warning_ui_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(cur_menu_key_info_, ARRAY_SIZE(cur_menu_key_info_), hWin);
}

static void menu_key_ok(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	g_custom_msg.user_data = (int)&global_file;
//     upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
//     unregister_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool));
    back_win(hWin);//关闭对话框
}
static void menu_key_cancle(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_CANCLE;
    back_win(hWin);//关闭对话框
}
/**
  * @brief  主窗口结构体初始化
  */
static MYUSER_WINDOW_T warning_windows=
{
    {"警告","WARNING"},
    WarningUIcb,update_warning_ui_menu_key_inf,
	{
        warning_ui_ele_pool,COUNT_ARRAY_SIZE(warning_ui_ele_pool),
        (CS_INDEX*)warning_ui_ele_buf,COUNT_ARRAY_SIZE(warning_ui_ele_buf)
    },
};

void init_warning_content(void)
{
    warning_windows.win_name[CHINESE] = warning_inf.title.text[CHINESE];
    warning_windows.win_name[ENGLISH] = warning_inf.title.text[ENGLISH];
    
//    warning_ui_ele_pool[WARNING_UI_NOTICE].text[CHINESE] = warning_inf.gist[CHINESE];
//    warning_ui_ele_pool[WARNING_UI_NOTICE].text[ENGLISH] = warning_inf.gist[ENGLISH];
//    warning_ui_ele_pool[WARNING_UI_CONTENT].text[CHINESE] = warning_inf.content[CHINESE];
//    warning_ui_ele_pool[WARNING_UI_CONTENT].text[ENGLISH] = warning_inf.content[ENGLISH];
}
///**
//  * @brief  重绘背景
//  * @param  无
//  * @retval 无
//  */
//static void _PaintFrame(void) 
//{
//    GUI_RECT r;
//    WM_GetClientRect(&r);
//    GUI_SetBkColor(warning_ui_ele_pool[WARNING_UI_CONTENT].dis_info.back_color);
//    GUI_ClearRectEx(&r);
//    
//}

/**
  * @brief  根据屏幕尺寸初始化界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_warning_ui_text_ele_pos_inf(void)
{
    switch(sys_par.screem_size)
    {
    case SCREEN_4_3INCH:
        break;
    case SCREEN_6_5INCH:
        break;
    default:
    case SCREEN_7INCH:
//        _7_init_warning_win_text_ele_pos(warning_ui_ele_pool);
        break;
    }
}

static void TUIcb(WM_MESSAGE * pMsg)
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
static void WarningUIcb(WM_MESSAGE * pMsg)
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
            init_warning_ui_text_ele_pos_inf();
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
            
            //无奈对话框不支持定时器，只能绕个弯子实现定时关闭对话框
            //利用一个普通控件来挂靠定定时器
            if(0 != warning_inf.dly_auto_close)
            {
                WM_HWIN handle;
                
                handle = TEXT_CreateEx(0, 0, 1, 1, hWin, WM_CF_HIDE, 0, ++id_base, "");
                WM_SetCallback(handle, TUIcb);//设置控件回调函数
                WM_CreateTimer(handle, 0, warning_inf.dly_auto_close, 0);//挂靠定时器
            }
            
			break;
        }
		case WM_PAINT:
// 			_PaintFrame();
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
		case WM_KEY:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

void create_warning_dialog(int hWin)
{
    init_warning_content();
//    init_window_size(&warning_windows, warning_windows_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&warning_windows, &windows_list, hWin);//创建主界面
}







/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
