/**
  ******************************************************************************
  * @file    file_ui_com.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件管理界面
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

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
#include "7_step_ui.h"
#include "step_win.h"
#include "type/cs99xx_type.h"
#include "step_edit_win.h"

  
/* Private typedef -----------------------------------------------------------*/

/**
  * @brief  移动步骤的方向枚举定义 向前移 或 向后移
  */
typedef enum{
    MOVE_STEP_FORWARD,///<向前移动当前步
    MOVE_STEP_BACKWARD,///<向后移动当前步
}MOVE_STEP_DIRECTION;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void step_win_cb(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);
static void dis_all_steps(void);
static void update_step_cur_row_menu_key_st(WM_HWIN hWin);
static void new_one_step(int hwin);
static void dis_all_steps(void);
static void update_g_cur_step(void);
static void delete_g_cur_step(void);
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win);

static void step_exist_f1_cb(KEY_MESSAGE *key_msg);
static void step_exist_f2_cb(KEY_MESSAGE *key_msg);
static void step_exist_f3_cb(KEY_MESSAGE *key_msg);
static void step_exist_f4_cb(KEY_MESSAGE *key_msg);
static void step_exist_f5_cb(KEY_MESSAGE *key_msg);
static void step_exist_f6_cb(KEY_MESSAGE *key_msg);

static void step_no_exist_f1_cb(KEY_MESSAGE *key_msg);
static void step_no_exist_f2_cb(KEY_MESSAGE *key_msg);
static void step_no_exist_f3_cb(KEY_MESSAGE *key_msg);
static void step_no_exist_f4_cb(KEY_MESSAGE *key_msg);
static void step_no_exist_f5_cb(KEY_MESSAGE *key_msg);
static void step_no_exist_f6_cb(KEY_MESSAGE *key_msg);

static void direct_key_down_cb(KEY_MESSAGE *key_msg);
static void direct_key_up_cb(KEY_MESSAGE *key_msg);
static void move_step(MOVE_STEP_DIRECTION dir);
/* Private variables ---------------------------------------------------------*/
/**
  * @brief  步骤管理列表的句柄
  */
static LISTVIEW_Handle list_h;
/**
  * @brief  步骤管理窗口的位置尺寸信息，根据不同的屏尺寸进行初始化
  */
static WIDGET_POS_SIZE_T* step_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_windows,/*4.3寸屏*/
    &_7_step_windows,/*5.6寸屏*/
    &_7_step_windows,/*7寸屏*/
};
/**
  * @brief  步骤管理窗口的结构定义
  */
static MYUSER_WINDOW_T step_windows =
{
    {"步骤参数","Step Par."},
    step_win_cb, update_menu_key_inf,
    {0},
    {0},
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)group_com_ele_table,ARRAY_SIZE(group_com_ele_table),
        init_create_step_edit_win_com_ele,
    },
};

/**
  * @brief  步骤存在时的菜单键初始化数组
  */
static MENU_KEY_INFO_T 	step_exist_menu_key_info[] =
{
    {"", F_KEY_NEW      , KEY_F1 & _KEY_UP, step_exist_f1_cb },//f1
    {"", F_KEY_DETAIL   , KEY_F2 & _KEY_UP, step_exist_f2_cb },//f2
    {"", F_KEY_DEL		, KEY_F3 & _KEY_UP, step_exist_f3_cb },//f3
    {"", F_KEY_FORWARD  , KEY_F4 & _KEY_UP, step_exist_f4_cb },//f4
    {"", F_KEY_BACKWARD , KEY_F5 & _KEY_UP, step_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, step_exist_f6_cb },//f6
};
/**
  * @brief  步骤不存在时的菜单键初始化数组
  */
static MENU_KEY_INFO_T 	step_no_exist_menu_key_info[] =
{
    {"", F_KEY_NEW      , KEY_F1 & _KEY_UP, step_no_exist_f1_cb	},//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP, step_no_exist_f2_cb },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP, step_no_exist_f3_cb },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP, step_no_exist_f4_cb },//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, step_no_exist_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP, step_no_exist_f6_cb	},//f6
};

/**
  * @brief  系统功能按键的初始化数组
  */
static FUNCTION_KEY_INFO_T 	step_win_sys_key_pool[]={
	{KEY_UP		, direct_key_up_cb      },
	{KEY_DOWN	, direct_key_down_cb 	},
	{CODE_LEFT	, direct_key_down_cb    },
	{CODE_RIGH	, direct_key_up_cb      },
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  步骤存在时功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f1_cb(KEY_MESSAGE *key_msg)
{
    new_one_step(key_msg->user_data);
}
/**
  * @brief  步骤存在时功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f2_cb(KEY_MESSAGE *key_msg)
{
    delete_win_com_ele(g_cur_win);//删除当前窗口中的公共文本对象
    update_g_cur_step();
    update_group_inf(g_cur_win);
    create_step_edit_win(key_msg->user_data);
}
/**
  * @brief  步骤存在时功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f3_cb(KEY_MESSAGE *key_msg)
{
    delete_g_cur_step();
}
/**
  * @brief  步骤存在时功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f4_cb(KEY_MESSAGE *key_msg)
{
    move_step(MOVE_STEP_FORWARD);
}
/**
  * @brief  步骤存在时功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f5_cb(KEY_MESSAGE *key_msg)
{
    move_step(MOVE_STEP_BACKWARD);
}
/**
  * @brief  步骤存在时功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}
/**
 * @brief  步骤不存在时功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f1_cb(KEY_MESSAGE *key_msg)
{
    new_one_step(key_msg->user_data);
}

/**
 * @brief  步骤不存在时功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
 * @brief  步骤不存在时功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
 * @brief  步骤不存在时功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
 * @brief  步骤不存在时功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f5_cb(KEY_MESSAGE *key_msg)
{
}

/**
 * @brief  步骤不存在时功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void step_no_exist_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

/**
  * @brief  向上方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    uint32_t data = key_msg->user_data;
    
	LISTVIEW_DecSel(list_h);
    update_step_cur_row_menu_key_st(data);
    update_g_cur_step();
    update_group_inf(g_cur_win);
}

/**
  * @brief  向下方向键回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    uint32_t data = key_msg->user_data;
    
	LISTVIEW_IncSel(list_h);
    update_step_cur_row_menu_key_st(data);
    update_g_cur_step();
    update_group_inf(g_cur_win);
}

/**
  * @brief  初始化并创建窗口中的公共文本对象
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    update_group_inf(win);
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}
/**
  * @brief  新建一个测试步
  * @param  [in] hwin 窗口句柄
  * @retval 无
  */
static void new_one_step(int hwin)
{
    uint8_t mode;
    STEP_NUM step;
    
    step = g_cur_file->total;
    
    if(step >= MAX_STEPS)
    {
        return;
    }
    
    mode = get_first_mode();
    insert_step(step, mode);
    save_group_info(g_cur_file->num);
    dis_all_steps();
    LISTVIEW_SetSel(list_h, step);
    update_step_cur_row_menu_key_st(hwin);
    update_g_cur_step();
    update_group_inf(g_cur_win);
}
/**
  * @brief  移动测试步
  * @param  [in] dir 移动的方向 MOVE_STEP_FORWARD 向前; MOVE_STEP_BACKWARD 向后
  * @retval 无
  */
static void move_step(MOVE_STEP_DIRECTION dir)
{
    int row = 0;
    STEP_NUM one;
    STEP_NUM two;
    
	row = LISTVIEW_GetSel(list_h);
	
    if(dir == MOVE_STEP_FORWARD)
    {
        if(row == 0)
        {
            return;
        }
        
        one = row;
        two = row + 1;
    }
    else if(dir == MOVE_STEP_BACKWARD)
    {
        if(row + 1 > g_cur_file->total)
        {
            return;
        }
        
        one = row + 2;
        two = row + 1;
    }
    else
    {
        return;
    }
    
    swap_step(one, two);
    save_group_info(g_cur_file->num);
    dis_all_steps();
    
    LISTVIEW_SetSel(list_h, one - 1);
    update_step_cur_row_menu_key_st(g_cur_win->handle);
    update_g_cur_step();
    update_group_inf(g_cur_win);
}
/**
  * @brief  更新步骤表当前行的菜单显示
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_step_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    CS_ERR err;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 步骤存在 */
    if(g_cur_file->total >= row + 1)
    {
        size = ARRAY_SIZE(step_exist_menu_key_info);
        info = step_exist_menu_key_info;
    }
	/* 步骤不存在 */
    else
    {
        size = ARRAY_SIZE(step_no_exist_menu_key_info);
        info = step_no_exist_menu_key_info;
    }
    
    /* 是第一步*/
    if(row == 0)
    {
        set_menu_key_config_st(info, size, F_KEY_FORWARD, MENU_KEY_DIS, &err);
        
        /* 当前文件不至一步 */
        if(g_cur_file->total > 1)
        {
            set_menu_key_config_st(info, size, F_KEY_BACKWARD, MENU_KEY_EN, &err);
        }
    }
    /* 是最后一步 */
    if(row + 1 == g_cur_file->total)
    {
        set_menu_key_config_st(info, size, F_KEY_BACKWARD, MENU_KEY_DIS, &err);
        
        /* 当前文件不至一步 */
        if(g_cur_file->total > 1)
        {
            set_menu_key_config_st(info, size, F_KEY_FORWARD, MENU_KEY_EN, &err);
        }
    }
    /* 既不是第一步也不是最后一步 */
    if(row > 0 && (row + 1 < g_cur_file->total))
    {
        set_menu_key_config_st(info, size, F_KEY_FORWARD, MENU_KEY_EN, &err);
        set_menu_key_config_st(info, size, F_KEY_BACKWARD, MENU_KEY_EN, &err);
    }
    
	init_menu_key_info(info, size, hWin);//刷新菜单键显示
}
/**
  * @brief  清空步骤表
  * @param  无
  * @retval 无
  */
static void clear_step_listview(void)
{
    int32_t row = 0;
    int32_t column = 0;
    uint32_t n = MAX_STEPS;
    
    for(row = 0; row < n; row++)
    {
        for(column = 1; column < 4; column++)
        {
            LISTVIEW_SetItemText(list_h, column, row, "");
        }
    }
}
/**
  * @brief  删除当前步骤
  * @param  无
  * @retval 无
  */
static void delete_g_cur_step(void)
{
    int row = 0;
    NODE_STEP *node;
    uint32_t total = g_cur_file->total;
    
	row = LISTVIEW_GetSel(list_h);
	
    if(total < row + 1)
    {
        return;
    }
    
    del_step(row + 1);
    clear_step_listview();
    dis_all_steps();
    
    if(total == row + 1)
    {
        load_steps_to_list(row, 1);
        if(row > 0)
        {
            LISTVIEW_SetSel(list_h, row - 1);
        }
    }
    else
    {
        load_steps_to_list(row + 1, 1);
        LISTVIEW_SetSel(list_h, row);
    }
    
    node = get_g_cur_step();
    
    if(NULL != node)
    {
        g_cur_step = node;
    }
    
    update_group_inf(g_cur_win);
    update_step_cur_row_menu_key_st(g_cur_win->handle);
    save_group_info(g_cur_file->num);
}
/**
  * @brief  更新当前步骤
  * @param  无
  * @retval 无
  */
static void update_g_cur_step(void)
{
    int row = 0;
    NODE_STEP *node;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 步骤存在 */
    if(g_cur_file->total >= row + 1)
    {
        load_steps_to_list(row + 1, 1);
        node = get_g_cur_step();
        
        if(NULL != node)
        {
            g_cur_step = node;
        }
    }
}
/**
  * @brief  更新菜单信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_menu_key_inf(WM_HMEM hWin)
{
    update_step_cur_row_menu_key_st(hWin);
}
/**
  * @brief  步骤管理窗口重绘函数
  * @param  无
  * @retval 无
  */
static void step_win_pain_frame(void)
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_ClearRectEx(&r);
}

/**
  * @brief  创建步骤管理表
  * @param  [in] hWin 窗口句柄
  * @retval list 句柄
  */
static WM_HWIN create_step_listview(WM_HWIN hWin)
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
            handle = _7_create_step_listview(hWin);
            break;
    }
    
    return handle;
}

/**
  * @brief  初始化步骤参数在列表中的显示信息
  * @param  [out] buf 显示信息存放的缓冲区
  * @param  [in] node 步骤结构数据
  * @retval 参数个数
  */
static uint32_t init_step_dis_inf(uint8_t buf[5][20], NODE_STEP *node)
{
    uint8_t mode;
    int32_t i = 0;
	UN_STRUCT *un = (void*)node;
    
    mode = node->one_step.com.mode;
    sprintf((char *)buf[i++], "%s", mode_pool[mode]);
    
    switch(mode)
    {
        case ACW:
            mysprintf(buf[i++], unit_pool[VOL_U_kV] , 53, un->acw.output_vol);
            mysprintf(buf[i++], unit_pool[TIM_U_s]  , 51, un->acw.test_time);
            
            break;
        case DCW:
            mysprintf(buf[i++], unit_pool[VOL_U_kV] , 53, un->dcw.output_vol);
            mysprintf(buf[i++], unit_pool[TIM_U_s]  , 51, un->dcw.test_time);
            
            break;
        case IR:
            mysprintf(buf[i++], unit_pool[VOL_U_kV] , 53, un->ir.output_vol);
            mysprintf(buf[i++], unit_pool[TIM_U_s]  , 51, un->ir.test_time);
            
            break;
    }
    
    return i;
}
/**
  * @brief  在列表中的显示一步的信息
  * @param  [in] node 步骤结构数据
  * @param  [in] row 行号
  * @retval 无
  */
static void dis_one_step(NODE_STEP *node, int32_t row)
{
	uint8_t list_buf[5][20] = {0};
	int32_t i = 0;
    CS_ERR err;
    uint32_t n = 0;
    
    err = check_step_data(node);
	
    if(err != CS_ERR_NONE)
    {
		return;
    }
    
    n = init_step_dis_inf(list_buf, node);
    
  	for(i = 0; i < n; i++)
	{
		LISTVIEW_SetItemText(list_h, i + 1, row, (const char*)list_buf[i]);
	}
}

/**
  * @brief  在列表中的显示所有步的信息
  * @param  无
  * @retval 无
  */
static void dis_all_steps(void)
{
    int16_t i = 0;
    int16_t total = 0;
    NODE_STEP node;
    FILE_NUM file_num;
    
    file_num = g_cur_file->num;
    total = g_cur_file->total;
    
    for(i = 0; i < total; i++)
    {
        read_one_step(&node, file_num, i + 1);
        dis_one_step(&node, i);
    }
}

/**
  * @brief  初始化步骤管理列表显示所有步的信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void init_step_win_listview(WM_HWIN hWin)
{
    list_h = create_step_listview(hWin);//创建步骤管理表
    dis_all_steps();//显示出所有的步骤信息
}
/**
  * @brief  更新系统功能按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(step_win_sys_key_pool, ARRAY_SIZE(step_win_sys_key_pool), hWin);
}
/**
  * @brief  更新系统功能按键和菜单按键信息
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}
/**
  * @brief  步骤管理窗口回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void step_win_cb(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		/*自定义消息 当子窗口被关闭后 收到由子窗口发给父窗口的消息 */
		case CM_CHILD_W_MSG:
		{
            update_key_inf(hWin);
			win = get_user_window_info(hWin);
            update_g_cur_step();
            dis_all_steps();
            init_create_step_edit_win_com_ele(win);
			break;
		}
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
			init_step_win_listview(hWin);
            
            init_create_win_all_ele(win);
            update_key_inf(hWin);
            update_g_cur_step();
            update_group_inf(g_cur_win);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			step_win_pain_frame();
            draw_group_inf_area();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


/* Public functions ---------------------------------------------------------*/
/**
  * @brief  创建步骤管理窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_step_par_win(int hWin)
{
    init_window_size(&step_windows, step_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&step_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
