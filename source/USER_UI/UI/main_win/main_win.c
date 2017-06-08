/**
  ******************************************************************************
  * @file    main_ui_com.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#include "main_win.h"
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
#include "7_main_win.h"
#include "key_menu_win/key_menu_win.h"
#include "image/logo.h"
#include "image/user_image.h"
#include "sys_win/sys_manage_win.h"
#include "file_win/file_win.h"
#include "test_win/test_win.h"
#include "FILE_SYS.H"
#include "cs99xx_usb_manager.h"
#include "step_par_win/step_win.h"
#include "result_win/result_win.h"

static	WM_HWIN progbar_handle;///<进度条
static	WM_HWIN timer_handle;///<定时器句柄
static	WM_HWIN U_FLASH_1_handle;///<U盘图标1句柄
static	WM_HWIN U_FLASH_2_handle;///<U盘图标2句柄
static	WM_HWIN KEY_LOCK_handle;///<键盘锁图标句柄
static	WM_HWIN KEY_CAPITAL_SMALL_handle;///<大小写图标句柄
static void main_ui_cb(WM_MESSAGE * pMsg);
static void update_main_ui_menu_key_inf(WM_HMEM hWin);

static void sys_shift_key_fun(int data);
static void sys_unlock_key_fun(int data);
static void screen_capture_key_fun(int data);
/**
  * @brief  系统按键信息
  */
static FUNCTION_KEY_INFO_T sys_key_pool[]=
{
	{KEY_SHIFT	        , sys_shift_key_fun     },
	{KEY_UNLOCK	        , sys_unlock_key_fun    },
	{KEY_F1 & KEY_0 , screen_capture_key_fun    },
};

/**
  * @brief  根据不同屏幕尺寸填入位置尺寸信息
  */
static WIDGET_POS_SIZE_T* main_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_main_windows,/*4.3寸屏*/
    &_7_main_windows,/*5.6寸屏*/
    &_7_main_windows,/*7寸屏*/
};

/** 主界面使用的菜单键信息的配置 */
static MENU_KEY_INFO_T 	main_ui_menu_key_inf[] = 
{
    {"", F_KEY_DISPLAY  , KEY_F0 & _KEY_UP, 0                     , MENU_KEY_DIS},//f0
    {"", F_KEY_FILE		, KEY_F1 & _KEY_UP, create_file_win       , MENU_KEY_EN },//f1
    {"", F_KEY_STEP		, KEY_F2 & _KEY_UP, create_step_par_win   , MENU_KEY_EN},//f2
    {"", F_KEY_SYS		, KEY_F3 & _KEY_UP, create_sys_manager_win, MENU_KEY_EN },//f3
    {"", F_KEY_RESULT   , KEY_F4 & _KEY_UP, create_result_win     , MENU_KEY_EN},//f4
    {"", F_KEY_TEST		, KEY_F5 & _KEY_UP, create_test_win       , MENU_KEY_EN},//f5
    {"", F_KEY_HELP		, KEY_F6 & _KEY_UP, 0                     , MENU_KEY_DIS },//f6
};

/**
  * @brief  主界面的文本对象池
  */
static TEXT_ELE_T main_ui_text_ele_pool[]=
{
	{{"本控","UNLINK"}, MAIN_UI_COM_ST },
	{{"2017-04-07 08:59:00","2017-04-07 08:59:00"}, MAIN_UI_SYS_TIME },
};
/**
  * @brief  根据屏幕尺寸初始化主界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_main_ui_text_ele_pos_inf(void)
{
    switch(sys_par.screem_size)
    {
    case SCREEN_4_3INCH:
        break;
    case SCREEN_6_5INCH:
        break;
    default:
    case SCREEN_7INCH:
        _7_init_main_ui_text_ele_pos(main_ui_text_ele_pool);
        break;
    }
}
/**
  * @brief  绘制主界面状态栏
  * @param  无
  * @retval 无
  */
static void draw_main_win_status_bar(void)
{
    GUI_SetColor(GUI_LIGHTGRAY);
    
    switch(sys_par.screem_size)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            GUI_FillRectEx(&_7_sys_st_info_area);
            break;
    }
}


/** 主界面显示的文本索引表 */
static CS_INDEX main_ui_text_ele_table[] =
{
	MAIN_UI_COM_ST,
	MAIN_UI_SYS_TIME,
};

/**
  * @brief  主窗口结构体初始化
  */
MYUSER_WINDOW_T main_windows=
{
    {"main_window"},
    main_ui_cb, update_main_ui_menu_key_inf,
	{
        main_ui_text_ele_pool, ARRAY_SIZE(main_ui_text_ele_pool),
        (CS_INDEX*)main_ui_text_ele_table, ARRAY_SIZE(main_ui_text_ele_table)
    },
};
/**
  * @brief  更新主界面的菜单键信息
  * @param  [in] hWin 主界面窗口句柄
  * @retval 无
  */
static void update_main_ui_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(main_ui_menu_key_inf, ARRAY_SIZE(main_ui_menu_key_inf), hWin);
}

void update_shift_bmp(void)
{
    if(ui_flag.shift_flag)
    {
        set_capital_letter_image(KEY_CAPITAL_SMALL_handle);
    }
    else
    {
        set_small_letter_image(KEY_CAPITAL_SMALL_handle);
    }
}
/**
  * @brief  系统SHIFT按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_shift_key_fun(int data)
{
    ui_flag.shift_flag = !ui_flag.shift_flag;
    update_shift_bmp();
}

void update_unlock_bmp(void)
{
    if(ui_flag.key_lock_flag)
    {
        set_key_lock_image(KEY_LOCK_handle);
    }
    else
    {
        set_key_unlock_image(KEY_LOCK_handle);
    }
}
/**
  * @brief  系统键盘锁按键回调函数
  * @param  [in] data 用户数据
  * @retval 无
  */
static void sys_unlock_key_fun(int data)
{
    ui_flag.key_lock_flag = !ui_flag.key_lock_flag;
    update_unlock_bmp();
}

/**
  * @brief  截屏按键的
  * @param  [in] data 用户数据
  * @retval 无
  */
#include "PROGBAR.h"
static void screen_capture_key_fun(int data)
{
    set_usb_disk_task(USB_SCREEN_CAPTURE);
    //创建进度条
    progbar_handle = PROGBAR_CreateEx(100, 455, 50, 20, data, WM_CF_HIDE, 0, id_base++);
}
void delete_main_win_progbar(void)
{
    WM_DeleteWindow(progbar_handle);//删除窗口控件
    progbar_handle = 0;//清除被删除窗口的句柄
}
void set_main_win_progbar_value(int32_t value)
{
    PROGBAR_SetValue(progbar_handle, value);
}
void set_main_win_progbar_show(void)
{
    WM_ShowWindow(progbar_handle);
}
/**
  * @brief  更新系统按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_system_key_inf(WM_HMEM hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

/**
  * @brief  设置主界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_main_windows_handle(WM_HWIN hWin)
{
	main_windows.handle = hWin;
}

/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  更新按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_main_ui_menu_key_inf(hWin);
    update_system_key_inf(hWin);
}

void send_mes_to_main_window(int msg)
{
    WM_SendMessageNoPara(main_windows.handle, CM_UPDATE_USB_ST);
}
void update_usb_dis_status(void)
{
    uint8_t st = 0;
    
    st = get_ch376_status(1);
    
    if(st == 1)
    {
        set_u_flash_1_ok_image(U_FLASH_1_handle);
    }
    else
    {
        set_u_flash_1_ng_image(U_FLASH_1_handle);
    }
    
    st = get_ch376_status(2);
    
    if(st == 1)
    {
        set_u_flash_2_ok_image(U_FLASH_2_handle);
    }
    else
    {
        set_u_flash_2_ng_image(U_FLASH_2_handle);
    }
    
    update_unlock_bmp();
    update_shift_bmp();
}

/**
  * @brief  主测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void main_ui_cb(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
        case CM_UPDATE_USB_ST:
            break;
		case WM_CREATE:
			set_main_windows_handle(hWin);
			win = get_user_window_info(hWin);
            
			WM_SetFocus(hWin);/* 设置聚焦 */
            update_key_inf(hWin);
            
            init_main_ui_text_ele_pos_inf();//初始化文本对象的位置信息
            init_window_text_ele_list(win);//初始化窗口文本对象链表
			init_window_text_ele(win);
			timer_handle = WM_CreateTimer(hWin, 0, 1000, 0);
            create_logo_image(hWin);
            U_FLASH_1_handle = create_u_flash_1_image(hWin, 5 + 25 * 0, 480 - 24);
            U_FLASH_2_handle = create_u_flash_2_image(hWin, 5 + 25 * 1, 480 - 24);
            KEY_LOCK_handle = create_key_lock_image(hWin, 5 + 25 * 2, 480 - 24);
            KEY_CAPITAL_SMALL_handle = create_capital_small_letter_image(hWin, 5 + 25 * 3, 480 - 24);
            
            set_u_flash_1_ng_image(U_FLASH_1_handle);
            set_u_flash_2_ng_image(U_FLASH_2_handle);
            set_key_unlock_image(KEY_LOCK_handle);
            set_small_letter_image(KEY_CAPITAL_SMALL_handle);
			break;
		case WM_PAINT:
			_PaintFrame();
            draw_main_win_status_bar();
			
            /* 唤醒按键扫描任务 */
			{
				OS_ERR	err;
				OSTaskResume(&ScanKeyTaskTCB, &err);
			}
			break;
		case WM_TIMER:
		{
            uint8_t *str = get_time_str(0);
			win = get_user_window_info(hWin);
            main_ui_text_ele_pool[MAIN_UI_SYS_TIME].text[CHINESE] = str;
            main_ui_text_ele_pool[MAIN_UI_SYS_TIME].text[ENGLISH] = str;
			update_text_ele((CS_INDEX)MAIN_UI_SYS_TIME, win, str);
			WM_RestartTimer(timer_handle, 1000);
			break;
		}
		case WM_KEY:
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/**
  * @brief  创建主界面
  * @param  无
  * @retval 无
  */
void create_main_windows(void)
{
    init_window_size(&main_windows, main_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&main_windows, &windows_list, 0);
}
/**
  * @brief  初始化用户窗口环境
  * @param  无
  * @retval 无
  */
void init_user_window_env(void)
{
	list_init(&windows_list);//初始化窗口链表
}
/**
  * @brief  7寸屏布局1的入口
  * @param  无
  * @retval 无
  */
void main_ui_enter(void)
{
	sys_par.screem_size = SCREEN_7INCH;
	id_base = GUI_ID_USER;//窗口控件ID
    init_user_window_env();
    create_key_menu_window();//创建按键界面
    create_main_windows();//创建主界面
	
	while(1)
	{
		GUI_Delay(1);//调用这个函数可以刷新界面
		
		/* 响应按键回调函数 */
        if(golbal_key_info.fun != NULL && golbal_key_info.en == MENU_KEY_EN)
        {
            golbal_key_info.fun(golbal_key_info.user_data);
            golbal_key_info.fun = NULL;
        }
	}
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
