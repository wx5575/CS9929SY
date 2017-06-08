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
#include "7_step_ui.h"
#include "step_win.h"
#include "type/cs99xx_type.h"
#include "step_edit_win.h"


static LISTVIEW_Handle list_h;
static void _cbInsertCard(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);
static void dis_all_steps(void);
static void update_cur_row_menu_key_st(WM_HWIN hWin);
static void new_one_step(int hwin);

static FUNCTION_KEY_INFO_T 	sys_key_pool[];

static WIDGET_POS_SIZE_T* step_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_windows,/*4.3寸屏*/
    &_7_step_windows,/*5.6寸屏*/
    &_7_step_windows,/*7寸屏*/
};

static MYUSER_WINDOW_T step_windows =
{
    {"步骤参数","Step Par."},
    _cbInsertCard, update_menu_key_inf,
	0,0, 0,
};

/* 步骤存在 */
static MENU_KEY_INFO_T 	step_exist_menu_key_info[] =
{
    {"", F_KEY_NEW      , KEY_F1 & _KEY_UP,	new_one_step},//f1
    {"", F_KEY_DETAIL   , KEY_F2 & _KEY_UP,	create_step_edit_ui},//f2
    {"", F_KEY_DEL		, KEY_F3 & _KEY_UP,	0},//f3
    {"", F_KEY_FORWARD  , KEY_F4 & _KEY_UP,	0},//f4
    {"", F_KEY_BACKWARD , KEY_F5 & _KEY_UP,	0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win},//f6
};
/* 步骤不存在 */
static MENU_KEY_INFO_T 	step_no_exist_menu_key_info[] =
{
    {"", F_KEY_NEW      , KEY_F1 & _KEY_UP,	new_one_step},//f1
    {"", F_KEY_NULL		, KEY_F2 & _KEY_UP,	0},//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP,	0},//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP,	0},//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP, 0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win},//f6
};
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
    update_cur_row_menu_key_st(hwin);
}

//static void into_save_file_dialog(int hWin)
//{
//    int row = 0;
//    
//	row = LISTVIEW_GetSel(list_h);
//	
//	/* 文件存在 */
//	if(CS_TRUE == is_file_exist(row + 1))
//	{
//        WARNING_INF w_inf =
//        {
//            {"警告","Warning"},
//            {"该操作会覆盖文件数据.","This will overwrite the file data."},
//            {"确定要继续吗?","Are you sure you want to continue?"},
//        };
//        set_custom_msg_id(CM_WARNING_INTO_SAVE_FILE);
//        set_warning_ui_inf(&w_inf);
//        create_warning_dialog(hWin);
//    }
//	/* 文件不存在 */
//	else
//	{
//        create_save_file_dialog(hWin);
//    }
//}
//static void pop_warning_dialog(int hWin)
//{
//    int row = 0;
//    
//	row = LISTVIEW_GetSel(list_h);
//	
//	/* 文件存在 */
//	if(CS_TRUE == is_file_exist(row + 1))
//	{
//        
//    }
//	/* 文件不存在 */
//	else
//	{
//        
//    }
//    
//    create_warning_dialog(hWin);
//}

static void update_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    
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
    
	init_menu_key_info(info, size, hWin);//刷新菜单键显示
}

static void update_menu_key_inf(WM_HMEM hWin)
{
    update_cur_row_menu_key_st(hWin);
}

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_ClearRectEx(&r);
}

//static void new_file_name(TEST_FILE *file)
//{
//	uint8_t list_buf[5][20] = {0};
//	uint16_t total_row = 0;
//	int32_t i = 0;
//	
//	total_row = LISTVIEW_GetNumRows(list_h);
//	sprintf((char *)list_buf[0], "%02d", total_row+1/*file->num*/);
//	sprintf((char *)list_buf[1], "%s", file->name);
//	sprintf((char *)list_buf[2], "%s", work_mode_pool[file->work_mode%2]);
//	sprintf((char *)list_buf[3], "%d", file->total_steps);
//	sprintf((char *)list_buf[4], "%s", get_time_str(0));
//	
//	LISTVIEW_AddRow(list_h, 0);
//	
//	for(i = 0; i < 5; i++)
//	{
//		LISTVIEW_SetItemText(list_h, i, total_row, (const char*)list_buf[i]);
//	}
//}

//static void update_file_dis(void)
//{
//	int32_t i = 0;
//	
//	for(i = 1; i < MAX_FILES; i++)
//	{
//		file_pool[i].num = i;
//		dis_one_file_info(&file_pool[i]);
//	}
//}

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

static uint32_t init_mode_listview_dis_inf(uint8_t buf[5][20], NODE_STEP *node)
{
    uint8_t mode;
    int32_t i = 0;
	UN_STRUCT *un = (void*)node;
    
    mode = node->one_step.com.mode;
    sprintf((char *)buf[i++], "%s", mode_pool[mode]);
    
    switch(mode)
    {
        case ACW:
            mysprintf(buf[i++], unit_pool[VOL_U_kV], 53, un->acw.testing_voltage);
            mysprintf(buf[i++], unit_pool[TIM_U_s], 51, un->acw.testing_time);
            
            break;
    }
    
    return i;
}
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
    
    n = init_mode_listview_dis_inf(list_buf, node);
    
	for(i = 0; i < n; i++)
	{
		LISTVIEW_SetItemText(list_h, i + 1, row, (const char*)list_buf[i]);
	}
}

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

static void init_step_win_listview(WM_HWIN hWin)
{
    list_h = create_step_listview(hWin);
    dis_all_steps();
}
static void direct_key_up(int data)
{
	LISTVIEW_DecSel(list_h);
    update_cur_row_menu_key_st(data);
}

static void direct_key_down(int data)
{
	LISTVIEW_IncSel(list_h);
    update_cur_row_menu_key_st(data);
}

static FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
	{CODE_LEFT	, direct_key_down	 },
	{CODE_RIGH	, direct_key_up      },
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
//static void dispose_child_win_msg(CUSTOM_MSG_T * msg, WM_HWIN hWin)
//{
//    uint8_t flag = 0;
//    
//	switch(msg->id)
//	{
//        case CM_WARNING_INTO_SAVE_FILE:
//        {
//            /* 警告对话框点击了确定后就创建存贮文件对话框 */
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//                create_save_file_dialog(hWin);
//                flag = 1;
//            }
//            /* 警告对话框点击了取消就返回到文件管理窗口，并更新一下按键信息 */
//            
//            break;
//        }
//		case CM_FILE_UI_SAVE://文件界面
//        {
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//				int row = 0;
//				TEST_FILE *f;
//				
//				f = (TEST_FILE*)msg->user_data;
//				
//				row = LISTVIEW_GetSel(list_h);
//				
//				f->num = row + 1;
//				if(f->num < MAX_FILES)
//				{
//					strcpy((char *)f->date, (const char*)get_time_str(0));
//					file_pool[f->num] = *f;
//                    save_file(f->num);
//				}
//                
//                update_file_dis();
//			}
//			else
//			{
//			}
//			break;
//        }
//		case CM_FILE_UI_NEW:
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//			}
//			else
//			{
//			}
//			break;
//		case CM_FILE_UI_READ://文件读取
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//			}
//			else
//			{
//			}
//			break;
//		case CM_FILE_UI_EDIT://文件编辑
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//			}
//			else
//			{
//			}
//			break;
//		case CM_FILE_UI_DEL://文件删除
//			if(msg->msg == CM_DIALOG_RETURN_OK)
//			{
//			}
//			else
//			{
//			}
//			break;
//	}
//    
//    if(0 == flag)
//    {
//        update_key_inf(hWin);
//    }
//}

static void _cbInsertCard(WM_MESSAGE* pMsg)
{
//    static CUSTOM_MSG_T msg;
    
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
			init_step_win_listview(hWin);
            update_key_inf(hWin);
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

void create_step_par_ui(int id)
{
    init_window_size(&step_windows, step_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&step_windows, &windows_list);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
