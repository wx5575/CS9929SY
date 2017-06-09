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
#include "ui_com/com_edit_api.h"
#include "app.h"
#include "7_restore_setting_win.h"
#include "restore_setting_win.h"

static void restore_setting_win_cb(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);
static void menu_key_ok(int hWin);
static void menu_key_cancle(int hWin);

static WIDGET_POS_SIZE_T* restore_setting_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_restore_setting_windows,/*4.3寸屏*/
    &_7_restore_setting_windows,/*5.6寸屏*/
    &_7_restore_setting_windows,/*7寸屏*/
};

TEXT_ELE_AUTO_LAYOUT_T  *restore_setting_text_ele_auto_layout[]=
{
    &_7_restore_setting_text_ele_auto_layout_inf,//4.3寸屏
    &_7_restore_setting_text_ele_auto_layout_inf,//5.6寸屏
    &_7_restore_setting_text_ele_auto_layout_inf,//7寸屏
};


static void restore_setting_win_f1_cb(KEY_MESSAGE *key_msg);
static void restore_setting_win_f2_cb(KEY_MESSAGE *key_msg);
static void restore_setting_win_f3_cb(KEY_MESSAGE *key_msg);
static void restore_setting_win_f4_cb(KEY_MESSAGE *key_msg);
static void restore_setting_win_f5_cb(KEY_MESSAGE *key_msg);
static void restore_setting_win_f6_cb(KEY_MESSAGE *key_msg);
static MENU_KEY_INFO_T 	restore_setting_menu_key_info[] =
{
    {"", F_KEY_NULL     , KEY_F1 & _KEY_UP, restore_setting_win_f1_cb },//f1
    {"", F_KEY_NULL     , KEY_F2 & _KEY_UP, restore_setting_win_f2_cb },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, restore_setting_win_f3_cb },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, restore_setting_win_f4_cb },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, restore_setting_win_f5_cb },//f5
    {"", F_KEY_CANCLE   , KEY_F6 & _KEY_UP, restore_setting_win_f6_cb },//f6
};

static void restore_setting_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
static void restore_setting_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
static void restore_setting_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
static void restore_setting_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
static void restore_setting_win_f5_cb(KEY_MESSAGE *key_msg)
{
    menu_key_ok(key_msg->user_data);
}
static void restore_setting_win_f6_cb(KEY_MESSAGE *key_msg)
{
    menu_key_cancle(key_msg->user_data);
}

static void menu_key_ok(int hWin)
{
    init_instrument_data();//初始化仪器数据
    back_win(hWin);//关闭对话框
}

static void menu_key_cancle(int hWin)
{
    back_win(hWin);//关闭对话框
}
/**
  * @brief  界面要显示的文本对象索引枚举定义
  */
enum{
    RESTORE_SETTING_NOTICE,
};
/**
  * @brief  界面要显示的文本对象索引表
  */
static CS_INDEX restore_setting_text_ele_table[]=
{
    RESTORE_SETTING_NOTICE,
};
/**
  * @brief  界面的文本对象池
  */
static TEXT_ELE_T restore_setting_ui_text_ele_pool[]=
{
	{
        {"恢复出厂设置会导致用户数据丢失.\n\n"
          "确定要继续吗?",
          "Restoring factory settings results in loss of user data.\n"
          "Are you sure you want to continue?"
        },
        RESTORE_SETTING_NOTICE
    },
};

static void init_create_restore_setting_text_ele(MYUSER_WINDOW_T* win)
{
    init_window_text_ele_list(win);
    init_window_text_ele_dis_inf(win, restore_setting_text_ele_auto_layout[sys_par.screem_size]);
    init_window_text_ele(win);
}
/**
  * @brief  密码界面窗口结构
  */
MYUSER_WINDOW_T restore_setting_windows=
{
    {"恢复出厂设置","Restore Factory Setting"},
    restore_setting_win_cb, update_menu_key_inf,
	{
        restore_setting_ui_text_ele_pool, ARRAY_SIZE(restore_setting_ui_text_ele_pool),
        (CS_INDEX*)restore_setting_text_ele_table, ARRAY_SIZE(restore_setting_text_ele_table),
        init_create_restore_setting_text_ele
    },
};

static void update_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(restore_setting_menu_key_info, ARRAY_SIZE(restore_setting_menu_key_info), hWin);//刷新菜单键显示
}

static void restore_setting_win_cb(WM_MESSAGE* pMsg)
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
            update_win_menu_key(win);
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
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
		default:
			WM_DefaultProc(pMsg);
	}
}

void create_restor_factory_setting_dialog(int hWin)
{
    init_window_size(&restore_setting_windows, restore_setting_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_dialog(&restore_setting_windows, &windows_list, hWin);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
