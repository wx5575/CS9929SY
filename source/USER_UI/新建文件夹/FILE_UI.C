
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "malloc.h"
#include "ff.h"
#include "OS.H"
#include "ui_com_info.h"
#include "cs99xx_struct.h"
#include "stdio.h"
#include "string.h"
#include "LISTVIEW.H"
#include "dialog_info.h"
#include "7_INCH_ui/com_ui_info.h"


static LISTVIEW_Handle list_h;

static void _cbInsertCard(WM_MESSAGE* pMsg);
static void init_menu_key_inf(WM_HMEM hWin);
MYUSER_WINDOW_T _7_1_FileWindows=
{
	0/*x*/,0/*y*/,690/*width*/,455/*height*/,_cbInsertCard,init_menu_key_inf,
	0,0, 0,
};

#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色

static FILE_T 	temp_edit_file = {"123", 1,G_MODE,1,1};/* 临时的编辑文件 */

void into_save_as_file_dialog(int p)
{
	init_file_name_data(temp_edit_file.name);
	init_work_mode_data(&temp_edit_file.work_mode);
	init_buzzer_time_data(&temp_edit_file.buzzer_time);
	create_new_file_dialog(p);
}

static _MENU_KEY_INFO_T_ 	cur_menu_key_info[][7] = {
	/* 测试界面下的按键菜单 */
	{
		{"", F_KEY_DISPLAY	,	0						},//f0
		{"", F_KEY_SAVE		,	into_save_as_file_dialog/*create_file_save_dialog*/	},//f1
		{"", F_KEY_NEW		,	create_file_new_dialog	},//f2
		{"", F_KEY_READ		,	0						},//f3
		{"", F_KEY_EDIT		,	0						},//f4
		{"", F_KEY_DEL		,	0						},//f5
		{"", F_KEY_BACK		,	into_test_ui			},//f6
	},
};

static void init_menu_key_inf(WM_HMEM hWin)
{
	set_menu_key_struct_info(cur_menu_key_info[0], hWin, 7);
	init_menu_key_info(cur_menu_key_info[0], 7);
}

enum{
ID_FRAMEWIN_1 = (GUI_ID_USER + 0x00),
ID_TEXT_F0,
ID_TEXT_F1,
ID_TEXT_F2,
ID_TEXT_F3,
ID_TEXT_F4,
ID_TEXT_F5,
ID_TEXT_F6,
ID_FILE_LIST_VIEW,
	
/* 界面记忆组信息栏 */
ID_GROUP_INFO_BASE,
ID_GROUP_INFO_END = ID_GROUP_INFO_BASE + 10,
	
};
enum{
	ID_GROUP_ELE_01,
	ID_GROUP_ELE_02,
	ID_GROUP_ELE_03,
	ID_GROUP_ELE_04,
	ID_GROUP_ELE_05,
	ID_GROUP_ELE_06,
};



// #define MENU_KEY_NUM			7 /* 按键个数 */
// #define MENU_KEY_O				2
// #define MENU_KEY_AREA_BASE_X	(CHILD_WIN_BASE_X + 690)
// #define MENU_KEY_AREA_BASE_Y	0 //(6 * 10 - CHILD_WIN_BASE_Y) //使用6的倍数
// #define MENU_KEY_AREA_WIDTH		(800 - MENU_KEY_AREA_BASE_X)
// #define MENU_KEY_AREA_HEIGHT	(480 - MENU_KEY_AREA_BASE_Y - 28 - CHILD_WIN_BASE_Y)

// #define MENU_KEY_BASE_X(N)		(MENU_KEY_AREA_BASE_X)
// #define MENU_KEY_BASE_Y(N)		(MENU_KEY_AREA_BASE_Y + (MENU_KEY_HEIGHT + MENU_KEY_O) * (N - 1))
// #define MENU_KEY_WIDTH			(MENU_KEY_AREA_WIDTH - 2 * MENU_KEY_O)
// #define MENU_KEY_HEIGHT			((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))

// #define MENU_KEY_BASE_POS(N)	MENU_KEY_BASE_X(N), MENU_KEY_BASE_Y(N)
// static WIDGET_POS_INFO file_ui_text_widget_pos_info[]=
// {
// {ID_TEXT_F0, MENU_KEY_AREA_BASE_X, MENU_KEY_AREA_BASE_Y, 0, 0, MENU_KEY_AREA_WIDTH, MENU_KEY_AREA_HEIGHT},

// {ID_TEXT_F1	,  MENU_KEY_BASE_POS(1), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
// {ID_TEXT_F2	,  MENU_KEY_BASE_POS(2), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
// {ID_TEXT_F3	,  MENU_KEY_BASE_POS(3), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
// {ID_TEXT_F4	,  MENU_KEY_BASE_POS(4), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
// {ID_TEXT_F5	,  MENU_KEY_BASE_POS(5), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
// {ID_TEXT_F6	,  MENU_KEY_BASE_POS(6), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},

// #define 	GROUP_INFO_AREA_X		(0 + 0)
// #define 	GROUP_INFO_AREA_Y		30 //(30 - CHILD_WIN_BASE_Y)
// #define 	GROUP_INFO_AREA_WIDTH  	800
// #define 	GROUP_INFO_AREA_HEIGHT	30

// #define		GROUP_INFO_ELE_X(N)		10 + 80 * (N > 1) + 80 * (N > 2) + 80 * (N > 3) + 100 * (N > 4) + 175 * (N > 5) //(10 + 120 * (N-1))
// #define		GROUP_INFO_ELE_Y(N)		4 //(30 + (STEP_INFO_ELE_HEIGHT + 3) * (N - 1 - n))

// #define 	GROUP_INFO_BASE_POS		GROUP_INFO_AREA_X, GROUP_INFO_AREA_Y
// #define 	GROUP_INFO_POS(N)		GROUP_INFO_ELE_X(N), GROUP_INFO_ELE_Y(N)
// #define 	GROUP_INFO_ELE_WIDTH(N)	75 * (N < 5) + 170 * (N == 5) + 80 * ( N == 6) 
// #define 	GROUP_INFO_ELE_HEIGHT	25
// #define		GROUP_INFO_ELE_SIZE(N)		GROUP_INFO_ELE_WIDTH(N), GROUP_INFO_ELE_HEIGHT
// /* 记忆组信息栏 */
// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_01, GROUP_INFO_BASE_POS, GROUP_INFO_POS(1), GROUP_INFO_ELE_SIZE(1)},
// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_02, GROUP_INFO_BASE_POS, GROUP_INFO_POS(2), GROUP_INFO_ELE_SIZE(2)},

// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_03, GROUP_INFO_BASE_POS, GROUP_INFO_POS(3), GROUP_INFO_ELE_SIZE(3)},
// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_04, GROUP_INFO_BASE_POS, GROUP_INFO_POS(4), GROUP_INFO_ELE_SIZE(4)},

// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_05, GROUP_INFO_BASE_POS, GROUP_INFO_POS(5), GROUP_INFO_ELE_SIZE(5)},
// {ID_GROUP_INFO_BASE + ID_GROUP_ELE_06, GROUP_INFO_BASE_POS, GROUP_INFO_POS(6), GROUP_INFO_ELE_SIZE(6)},
// };

// static _USER_WIDGET_INFO file_ui_text_widget_info[]=
// {
// 	0
// 	#define MENU_KEY_WIDGET_ALIGN	GUI_TA_CENTER | GUI_TA_VCENTER
// 	{{""	        	}, ID_TEXT_F0, &GUI_Fonthz_29, GUI_WHITE, GUI_WHITE   , MENU_KEY_WIDGET_ALIGN},
// 	{{"存贮", "Save"	}, ID_TEXT_F1, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
// 	{{"新建", "New"		}, ID_TEXT_F2, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
// 	{{"读取", "Read"	}, ID_TEXT_F3, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
// 	{{"编辑", "Edit"	}, ID_TEXT_F4, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
// 	{{"删除", "Delete"	}, ID_TEXT_F5, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
// 	{{"返回", "Back"	}, ID_TEXT_F6, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	
	/* 记忆组信息栏 */ // ID_GROUP_INFO_BASE ID_GROUP_ELE_11 
// 	{{"容量:", "Content:"		}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_01, &GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// 	{{"30", "30"				}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_02, &GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// 	
// 	{{"已用:", "USED:"			}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_03, &GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// 	{{"00", "00"				}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_04, &GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// 	
// 	{{"当前文件编号:", "Cur.FileID:"	}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_05, &GUI_Fonthz_24, GUI_BLACK, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// 	{{"00", "00"		}, ID_GROUP_INFO_BASE + ID_GROUP_ELE_06, &GUI_Fonthz_24, GUI_RED, GUI_INVALID_COLOR, GUI_TA_LEFT | GUI_TA_TOP},
// };

// static MENU_KEY_INFO menu_key_info_pool[]=
// {
// 	{KEY_F1 & _KEY_DOWN, 0, ID_TEXT_F1, change_color_key_down},
// 	{KEY_F2 & _KEY_DOWN, 0, ID_TEXT_F2, change_color_key_down},
// 	{KEY_F3 & _KEY_DOWN, 0, ID_TEXT_F3, change_color_key_down},
// 	{KEY_F4 & _KEY_DOWN, 0, ID_TEXT_F4, change_color_key_down},
// 	{KEY_F5 & _KEY_DOWN, 0, ID_TEXT_F5, change_color_key_down},
// 	{KEY_F6 & _KEY_DOWN, 0, ID_TEXT_F6, change_color_key_down},
// 	{KEY_F1 & _KEY_UP  , 0, ID_TEXT_F1, change_color_key_up, create_file_save_dialog},
// 	{KEY_F2 & _KEY_UP  , 0, ID_TEXT_F2, change_color_key_up},
// 	{KEY_F3 & _KEY_UP  , 0, ID_TEXT_F3, change_color_key_up},
// 	{KEY_F4 & _KEY_UP  , 0, ID_TEXT_F4, change_color_key_up},
// 	{KEY_F5 & _KEY_UP  , 0, ID_TEXT_F5, change_color_key_up},
// 	{KEY_F6 & _KEY_UP  , 0, ID_TEXT_F6, change_color_key_up, into_test_ui},
// };


/* 记忆组信息区 */
// static const GUI_RECT group_info_area	=
// {
// GROUP_INFO_BASE_POS,
// GROUP_INFO_AREA_X + GROUP_INFO_AREA_WIDTH,
// GROUP_INFO_AREA_Y + GROUP_INFO_AREA_HEIGHT
// };

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}

extern void display_global_widget(WM_HWIN hwin);
// static char *list_buf[][5]=
// {
// 	{"001","DEFAULT","N","10","2016.8.6 14:56:10"},
// };
void display_file_ui_text_widget(WM_HWIN hwin)
{
// 	int32_t i = 0;
// 	uint32_t num = sizeof(file_ui_text_widget_info) / sizeof(file_ui_text_widget_info[0]);
// 	uint32_t pos_pool_size = sizeof(file_ui_text_widget_pos_info) / sizeof(file_ui_text_widget_pos_info[0]);
// 	
// 	init_widget_pos_info(file_ui_text_widget_info, num, file_ui_text_widget_pos_info, pos_pool_size);/* 初始化控件位置信息 */
// 	
// 	for(i = 0; i < num; i++)
// 	{
// 		init_user_widget_info(&file_ui_text_widget_info[i], hwin);
// 	}
}

void dis_one_file_info(FILE_T *file)
{
	uint8_t list_buf[5][20] = {0};
	int32_t i = 0;
	uint16_t row = 0;

	if(file->name[0] == 0)
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
	sprintf((char *)list_buf[i++], "%d", file->total_steps);
	sprintf((char *)list_buf[i++], "%s", file->date);//get_time_str(0));
	
	for(i = 1; i < 5; i++)
	{
		LISTVIEW_SetItemText(list_h, i, row, (const char*)list_buf[i]);
	}
}
void new_file_name(FILE_T *file)
{
	uint8_t list_buf[5][20] = {0};
	uint16_t total_row = 0;
	int32_t i = 0;
	
	total_row = LISTVIEW_GetNumRows(list_h);
	sprintf((char *)list_buf[0], "%02d", total_row+1/*file->num*/);
	sprintf((char *)list_buf[1], "%s", file->name);
	sprintf((char *)list_buf[2], "%s", work_mode_pool[file->work_mode%2]);
	sprintf((char *)list_buf[3], "%d", file->total_steps);
	sprintf((char *)list_buf[4], "%s", get_time_str(0));
	
	LISTVIEW_AddRow(list_h, 0);
	
	for(i = 0; i < 5; i++)
	{
		LISTVIEW_SetItemText(list_h, i, total_row, (const char*)list_buf[i]);
	}
}

void update_file_dis(void)
{
	int32_t i = 0;
	
	for(i = 1; i < FILE_MAX_NUM; i++)
	{
		file_pool[i].num = i;
		dis_one_file_info(&file_pool[i]);
	}
}

static void _create_dialog_(WM_HWIN hWin)
{
	int32_t i = 0;
	uint8_t buf[20] = {0};
	HEADER_Handle hHeader;
	SCROLLBAR_Handle hScrollbar;
	
	list_h = LISTVIEW_CreateEx(0, 0, 690, 450, hWin, WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW, 0, ID_FILE_LIST_VIEW);
	hScrollbar = SCROLLBAR_CreateAttached(list_h, SCROLLBAR_CF_VERTICAL);
	
	LISTVIEW_SetFont(list_h, &GUI_Fonthz_24);
	
	hHeader = LISTVIEW_GetHeader(list_h);
	HEADER_SetFont(hHeader,&GUI_Fonthz_24);
	HEADER_SetHeight(hHeader,28);
// 	GUI_ID_HSCROLL 用于水平滚动条，而
// 	GUI_ID_VSCROLL 则用于垂直滚动条。
// 	SCROLLBAR_CF_VERTICAL 创建垂直滚动条 （默认为水平滚动条）。
// 	SCROLLBAR_CF_FOCUSSABLE 为滚动条指定输入焦点。
	LISTVIEW_AddColumn(list_h, 80	, SELE_STR("编号","Edit")			, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 170	, SELE_STR("文件名","FileName")		, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 80	, SELE_STR("模式","Mode")			, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 80	, SELE_STR("总步数","Total")		, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(list_h, 262	, SELE_STR("建立时间","CreateDate")	, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
// 	SCROLLBAR_CreateAttached(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0),SCROLLBAR_CF_VERTICAL);
    SCROLLBAR_SetWidth(hScrollbar,18);
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_THUMB, GUI_BLACK);//缩略图0x00A0A0A0
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_SHAFT, GUI_WHITE);  //轴
	SCROLLBAR_SetColor(hScrollbar, SCROLLBAR_CI_ARROW, GUI_GRAY);  //箭头
	WIDGET_SetEffect(hScrollbar, &WIDGET_Effect_None);
	
	
	for(i = 0; i < 30; i++)
	{
		LISTVIEW_AddRow(list_h, 0);
		sprintf((char *)buf, "%02d", i + 1);
		LISTVIEW_SetItemText(list_h, 0,i, (const char*)buf);
	}
	
	update_file_dis();
	
	LISTVIEW_SetSel(list_h, 0);
// 	total_row = LISTVIEW_GetNumRows(list_h);
// 	for(i = 0; i < total_row; i++)
// 	{
// 		sprintf((char *)buf, "%03d", i + 1);
// 		LISTVIEW_SetItemText(list_h, 0,i, buf);
// 	}
	
// 	LISTVIEW_IncSel(list_h);
    LISTVIEW_SetGridVis(list_h, 1);
	LISTVIEW_SetBkColor(list_h, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
	LISTVIEW_SetRowHeight(list_h, 30);
// 	LISTVIEW_SetHeaderHeight(list_h, 30);
// 	LISTVIEW_SetAutoScrollV(list_h, 1);
// 	display_file_ui_text_widget(hWin);
// 	{
// 		uint32_t widget_pool_size = sizeof(file_ui_text_widget_info) / sizeof(file_ui_text_widget_info[0]);
// 		uint32_t n = sizeof(menu_key_info_pool) / sizeof(menu_key_info_pool[0]);
// 		
// 		init_key_widget_info(file_ui_text_widget_info, widget_pool_size, menu_key_info_pool, n);
// 	}
// 	hScrollbar = SCROLLBAR_CreateEx(600, 0, 20, 300, list_h, WM_CF_SHOW, 0, ID_FILE_LIST_VIEW + 1);
	
// 	set_menu_key_info(file_ui_menu_key_info_);
}

static int cur_sel_row;

static MENU_KEY_EN_ST menu_key_en_st[]=
{
	/* 文件存在时的菜单键的使能情况 */
	{
		MENU_KEY_DIS,
		MENU_KEY_DIS,
		MENU_KEY_EN,
		MENU_KEY_EN,
		MENU_KEY_EN,
		MENU_KEY_EN,
	},
	/* 文件不存在时的菜单键的使能情况 */
	{
		MENU_KEY_EN,
		MENU_KEY_EN,
		MENU_KEY_DIS,
		MENU_KEY_DIS,
		MENU_KEY_DIS,
		MENU_KEY_EN,
	},
};

// static MENU_KEY_EN_ST* get_cur_menu_key_st(void)
// {
// 	MENU_KEY_EN_ST *st;
// 	
// 	cur_sel_row = LISTVIEW_GetSel(list_h);
// 	
// 	/* 文件存在 */
// 	if(CS_YES == is_file_exist(cur_sel_row + 1))
// 	{
// 		st = &menu_key_en_st[0];
// 	}
// 	/* 文件不存在 */
// 	else
// 	{
// 		st = &menu_key_en_st[1];
// 	}
// 	
// 	return st;
// }

static void direct_key_up(int data)
{
	LISTVIEW_DecSel(list_h);
	init_menu_key_info(cur_menu_key_info[0], 7);
}

static void direct_key_down(int data)
{
	LISTVIEW_IncSel(list_h);
	init_menu_key_info(cur_menu_key_info[0], 7);
}

static _FUNCTION_KEY_INFO_T_ 	direct_key_pool[4]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
};

static void dispose_child_win_msg(CUSTOM_MSG_T * msg)
{
	switch(msg->id)
	{
		case CM_FILE_UI_SAVE://文件界面
			if(msg->msg == CM_DIALOG_RETURN_OK)
			{
				int row = 0;
				FILE_T *f;
				
				f = (FILE_T*)msg->user_data;
				
				row = LISTVIEW_GetSel(list_h);
				
				f->num = row + 1;
				if(f->num < FILE_MAX_NUM)
				{
					strcpy((char *)f->date, (const char*)get_time_str(0));
					file_pool[f->num] = *f;
				}
			}
			else
			{
			}
			break;
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
}

static void _cbInsertCard(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		/*自定义消息 当子窗口被关闭后 收到由子窗口发给父窗口的消息 */
		case CM_CHILD_W_MSG:
		{
			CUSTOM_MSG_T msg;
			
			init_menu_key_info(cur_menu_key_info[0],7);
			set_direct_key_fun(direct_key_pool);
			
			msg = *(CUSTOM_MSG_T*)pMsg->Data.v;//拷贝消息
			dispose_child_win_msg(&msg);
			update_file_dis();
			
			break;
		}
		case WM_CREATE:
			/* 设置聚焦 */
			WM_SetFocus(hWin);
			_create_dialog_(hWin);
			
			set_menu_key_struct_info(cur_menu_key_info[0], hWin, 6);
			init_menu_key_info(cur_menu_key_info[0],7);
			set_direct_key_fun(direct_key_pool);
			break;
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			_PaintFrame();
			GUI_SetColor(GUI_GRAY);
// 			GUI_FillRectEx(&group_info_area);
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
extern MYUSER_WINDOW_T _7_1_test_windows;
void createFileManagerUI(int id)
{
// 	WM_DeleteWindow(id);
// 	if(_7_1_FileWindows.handle == 0)
	{
		create_user_window(&_7_1_FileWindows, &windows_list);//创建文件管理界面
	}
// 	else
// 	{
// 		WM_HideWindow(_7_1_test_windows.handle);
// 		WM_ShowWindow(_7_1_FileWindows.handle);
// 		_7_1_FileWindows.init_menu_key(_7_1_FileWindows.handle);
// 	}
}
