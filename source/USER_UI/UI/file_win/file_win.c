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
#include "file_edit_win.h"
#include "cs99xx_mem_api.h"
#include "UI_COM/com_ui_info.h"
#include "KEY_MENU_WIN/key_menu_win.h"
#include "WARNING_WIN/warning_win.h"
#include "7_file_ui.h"
#include "file_win.h"


#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色

static LISTVIEW_Handle list_h;
static FUNCTION_KEY_INFO_T 	sys_key_pool[];

void pop_warning_dialog(int hWin);
void into_save_file_dialog(int hWin);
static void _cbInsertCard(WM_MESSAGE* pMsg);
static void update_menu_key_inf(WM_HMEM hWin);

static WIDGET_POS_SIZE_T* file_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_file_windows,/*4.3寸屏*/
    &_7_file_windows,/*5.6寸屏*/
    &_7_file_windows,/*7寸屏*/
};

MYUSER_WINDOW_T FileWindows=
{
    {"File_window"},
    _cbInsertCard,update_menu_key_inf,
	0,0, 0,
};


/* 文件存在的按键菜单 */
static MENU_KEY_INFO_T 	file_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP,	into_save_file_dialog },//f1
    {"", F_KEY_READ		, KEY_F2 & _KEY_UP,	0},//f2
    {"", F_KEY_EDIT		, KEY_F3 & _KEY_UP,	create_edit_file_dialog	},//f3
    {"", F_KEY_DEL		, KEY_F4 & _KEY_UP,	0},//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP,	0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win    			},//f6
};

/* 文件不存在的按键菜单 */
static MENU_KEY_INFO_T 	file_no_exist_menu_key_info[] =
{
    {"", F_KEY_SAVE		, KEY_F1 & _KEY_UP,	create_save_file_dialog },//f1
    {"", F_KEY_NEW		, KEY_F2 & _KEY_UP,	create_new_file_dialog  },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP,	0},//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP,	0},//f4
    {"", F_KEY_NULL     , KEY_F5 & _KEY_UP,	0},//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win    			},//f6
};
static void pop_warning_dialog_for_save_file(int hWin)
{
    const uint16_t WAR_WIN_TX = 10;//警告文本的X坐标
    const uint16_t WAR_WIN_TY = 20;//警告文本的Y坐标
    const uint16_t WAR_WIN_W = 400;//警告对话框的宽度
    const uint16_t WAR_WIN_H = 200;//警告对话框的高度
    const uint16_t WAR_WIN_X = 150;//警告对话框在父窗口中的X坐标
    const uint16_t WAR_WIN_Y = 50;//警告对话框在父窗口中的Y坐标
    
    WARNING_INF w_inf =
    {
        /* 标题 */
        {{"警告","Warning"}, 0 },
        /* 内容 */
        {
            {"该操作会覆盖文件数据.\n\n确定要继续吗?\n",
            "This will overwrite the file data.\n\n"
            "Do you want to continue?"}, 2,
            0/*base_x*/,0/*base_y*/,
            {WAR_WIN_TX,WAR_WIN_TY,WAR_WIN_W - 20,WAR_WIN_H - (WAR_WIN_TY + 10)},/*pos_size*/
            100/*max_len*/,
            {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_CENTER | GUI_TA_VCENTER
        },
        {WAR_WIN_X, WAR_WIN_Y, WAR_WIN_W, WAR_WIN_H},/*win_pos_size*/
        0,/*dly_auto_close xx ms后自动关闭 0表示不自动关闭*/
    };
    
    set_custom_msg_id(CM_WARNING_INTO_SAVE_FILE);
    set_warning_ui_inf(&w_inf);
    create_warning_dialog(hWin);
}
static void into_save_file_dialog(int hWin)
{
    int row = 0;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        pop_warning_dialog_for_save_file(hWin);
    }
	/* 文件不存在 */
	else
	{
        create_save_file_dialog(hWin);
    }
}
static void pop_warning_dialog(int hWin)
{
    int row = 0;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        
    }
	/* 文件不存在 */
	else
	{
        
    }
    
    create_warning_dialog(hWin);
}

static void update_cur_row_menu_key_st(WM_HWIN hWin)
{
    int row = 0;
    uint8_t size = 0;
    MENU_KEY_INFO_T *info = NULL;
    
	row = LISTVIEW_GetSel(list_h);
	
	/* 文件存在 */
	if(CS_TRUE == is_file_exist(row + 1))
	{
        size = ARRAY_SIZE(file_exist_menu_key_info);
        info = file_exist_menu_key_info;
	}
	/* 文件不存在 */
	else
	{
        size = ARRAY_SIZE(file_no_exist_menu_key_info);
        info = file_no_exist_menu_key_info;
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
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

static void dis_one_file_info(TEST_FILE *file)
{
	uint8_t list_buf[5][20] = {0};
	int32_t i = 0;
	uint16_t row = 0;
    CS_ERR err;
    
    err = check_file_data(file);
	
    if(err != CS_ERR_NONE)
    {
		return;
    }
    
	if(file->num >= 1)
	{
		row = file->num - 1;
	}
	
	i = 1;
	sprintf((char *)list_buf[i++], "%s", file->name);
	sprintf((char *)list_buf[i++], "%s", work_mode_pool[file->work_mode%2]);
	sprintf((char *)list_buf[i++], "%d", file->total);
	sprintf((char *)list_buf[i++], "%s", file->date);//get_time_str(0));
	
	for(i = 1; i < 5; i++)
	{
		LISTVIEW_SetItemText(list_h, i, row, (const char*)list_buf[i]);
	}
}

static void new_file_name(TEST_FILE *file)
{
	uint8_t list_buf[5][20] = {0};
	uint16_t total_row = 0;
	int32_t i = 0;
	
	total_row = LISTVIEW_GetNumRows(list_h);
	sprintf((char *)list_buf[0], "%02d", total_row+1/*file->num*/);
	sprintf((char *)list_buf[1], "%s", file->name);
	sprintf((char *)list_buf[2], "%s", work_mode_pool[file->work_mode%2]);
	sprintf((char *)list_buf[3], "%d", file->total);
	sprintf((char *)list_buf[4], "%s", get_time_str(0));
	
	LISTVIEW_AddRow(list_h, 0);
	
	for(i = 0; i < 5; i++)
	{
		LISTVIEW_SetItemText(list_h, i, total_row, (const char*)list_buf[i]);
	}
}

static void update_file_dis(void)
{
	int32_t i = 0;
	
	for(i = 1; i < MAX_FILES; i++)
	{
		file_pool[i].num = i;
		dis_one_file_info(&file_pool[i]);
	}
}

static WM_HWIN create_file_listview(WM_HWIN hWin)
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
            handle = _7_create_file_listview(hWin);
            break;
    }
    
    return handle;
}

static void _create_dialog_(WM_HWIN hWin)
{
    list_h = create_file_listview(hWin);
	update_file_dis();
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
	{CODE_LEFT	, direct_key_down   },
	{CODE_RIGH	, direct_key_up	    },
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
static void dispose_child_win_msg(CUSTOM_MSG_T * msg, WM_HWIN hWin)
{
    uint8_t flag = 0;
    
	switch(msg->id)
	{
        case CM_WARNING_INTO_SAVE_FILE:
        {
            /* 警告对话框点击了确定后就创建存贮文件对话框 */
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
                create_save_file_dialog(hWin);
                flag = 1;
            }
            /* 警告对话框点击了取消就返回到文件管理窗口，并更新一下按键信息 */
            
            break;
        }
		case CM_FILE_UI_SAVE://文件界面
        {
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				TEST_FILE *f;
				
				f = (TEST_FILE*)msg->user_data;
				
				row = LISTVIEW_GetSel(list_h);
				
				f->num = row + 1;
				if(f->num < MAX_FILES)
				{
					strcpy((char *)f->date, (const char*)get_time_str(0));
					file_pool[f->num] = *f;
                    save_file(f->num);
				}
                
                update_file_dis();
			}
			else
			{
			}
			break;
        }
		case CM_FILE_UI_NEW:
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
			}
			else
			{
			}
			break;
		case CM_FILE_UI_READ://文件读取
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
			}
			else
			{
			}
			break;
		case CM_FILE_UI_EDIT://文件编辑
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
			}
			else
			{
			}
			break;
		case CM_FILE_UI_DEL://文件删除
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
			}
			else
			{
			}
			break;
	}
    
    if(0 == flag)
    {
        update_key_inf(hWin);
    }
}

static void _cbInsertCard(WM_MESSAGE* pMsg)
{
    static CUSTOM_MSG_T msg;
	WM_HWIN hWin = pMsg->hWin;
    
	switch (pMsg->MsgId)
	{
		/*自定义消息 当子窗口被关闭后 收到由子窗口发给父窗口的消息 */
		case CM_CHILD_W_MSG:
		{
			msg = *(CUSTOM_MSG_T*)pMsg->Data.v;//拷贝消息
//             WM_SendMessageNoPara(hWin, CM_W_DIS_MSG);
            
			dispose_child_win_msg(&msg, hWin);
			break;
		}
		case WM_CREATE:
			WM_SetFocus(hWin);/* 设置聚焦 */
			_create_dialog_(hWin);
            update_key_inf(hWin);
            break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			_PaintFrame();
// 			GUI_SetColor(GUI_GRAY);
// 			GUI_FillRectEx(&group_info_area);
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

void createFileManagerUI(int id)
{
    init_window_size(&FileWindows, file_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&FileWindows, &windows_list);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
