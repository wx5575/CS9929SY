
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
#include "app.h"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "DROPDOWN.H"

static WM_HWIN hDlg;
static WM_HWIN cur_dropdown_handle;

static void menu_key_ok(int);
static void menu_key_cancle(int);
static void menu_key_delete(int p);
static void menu_key_backspace(int p);
static void menu_key_home(int p);
static void menu_key_end(int p);

static void menu_key_next(int p)
{
	GUI_SendKeyMsg(GUI_KEY_TAB, 1);
}

static void menu_key_selected(int p)
{
	int n = 0;
	n = DROPDOWN_GetSel(cur_dropdown_handle);
	DROPDOWN_SetSel(cur_dropdown_handle, n);
	DROPDOWN_Collapse(cur_dropdown_handle);
	WM_SetFocus(cur_dropdown_handle);
}

static void menu_key_select(int p)
{
	int n = 0;
	n = DROPDOWN_GetSel(cur_dropdown_handle);
	DROPDOWN_SetSel(cur_dropdown_handle, n);
	DROPDOWN_Collapse(cur_dropdown_handle);
	WM_SetFocus(cur_dropdown_handle);
}


/* 当前使用界面对应的菜单键信息与响应函数 */
static _MENU_KEY_INFO_T_ 	cur_menu_key_info[][6] = {
	/* 文件名 */
	{
		{"", F_KEY_OK,			menu_key_ok			},//f1
		{"", F_KEY_CANCLE,		menu_key_cancle		},//f2
		{"", F_KEY_NEXT,		menu_key_next		},//f3
		{"", F_KEY_DEL,			menu_key_delete		},//f4
		{"", F_KEY_HOME,		menu_key_backspace	},//f5
		{"", F_KEY_END,			menu_key_selected	},//f6
	},
	/* 工作模式 */
	{
		{"", F_KEY_OK,			menu_key_ok			},//f1
		{"", F_KEY_CANCLE,		menu_key_cancle		},//f2
		{"", F_KEY_NEXT,		menu_key_next		},//f3
		{"", F_KEY_SELECT,		menu_key_select		},//f4
		{"", F_KEY_NULL,		0					},//f5
		{"", F_KEY_NULL,		0					},//f6
	},
	/* 蜂鸣时间 */
	{
		{"", F_KEY_OK,			menu_key_ok			},//f1
		{"", F_KEY_CANCLE,		menu_key_cancle		},//f2
		{"", F_KEY_NEXT,		menu_key_next		},//f3
		{"", F_KEY_NULL,		0					},//f4
		{"", F_KEY_NULL,		0					},//f5
		{"", F_KEY_NULL,		0					},//f6
	},
	/* PASS保持时间 */
	{
		{"", F_KEY_OK,			menu_key_ok			},//f1
		{"", F_KEY_CANCLE,		menu_key_cancle		},//f2
		{"", F_KEY_NEXT,		menu_key_next		},//f3
		{"", F_KEY_NULL,		0					},//f4
		{"", F_KEY_NULL,		0					},//f5
		{"", F_KEY_NULL,		0					},//f6
	},
};

enum{
ID_FRAMEWIN_FILE_SAVE = (GUI_ID_USER + 0x00),
ID_DIALOG_FILE_NAME_TEXT,
ID_DIALOG_FILE_PAR_EDIT,
ID_DIALOG_FILE_NUM_EDIT,
ID_DIALOG_FILE_NAME_V_EDIT,

ID_DIALOG_FILE_WORK_MODE_NAME_TEXT,
ID_DIALOG_FILE_WORK_MODE_V_DROPDOWN,

ID_DIALOG_FILE_BUZZER_NAME_TEXT,
ID_DIALOG_FILE_BUZZER_V_EDIT,

ID_DIALOG_FILE_PASS_T_NAME_TEXT,
ID_DIALOG_FILE_PASS_T_V_EDIT,
	
ID_DIALOG_FILE_NOTICE_TEXT,
};

static PAR_SET_KEY_SERVER_INFO par_set_key_server_info[]=
{
	{ID_DIALOG_FILE_NAME_V_EDIT				, 0, keyboard_fun_str},
	{ID_DIALOG_FILE_WORK_MODE_V_DROPDOWN	, 1, keyboard_fun_num},
	{ID_DIALOG_FILE_BUZZER_V_EDIT			, 1, keyboard_fun_num},
	{ID_DIALOG_FILE_PASS_T_V_EDIT			, 1, keyboard_fun_num},
};

static void send_msg_to_parent(WM_HWIN hwin, int id, int val)
{
	WM_HWIN hParent;
	WM_MESSAGE Msg;
	
	Msg.MsgId = id;
	Msg.Data.v = val;
	hParent = WM_GetParent(hwin);//获取编辑控件的父窗口句柄
	
	if(hParent != 0)
		WM_SendMessage(hParent, &Msg);
}

static void menu_key_ok(int p)
{
	g_custom_msg.id = CM_FILE_UI_NEW;
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	g_custom_msg.user_data = (int)&global_file;
	GUI_EndDialog(hDlg, 1);
}

static void menu_key_cancle(int p)
{
	g_custom_msg.id = CM_FILE_UI_NEW;
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	GUI_EndDialog(hDlg, 1);
}

static void menu_key_delete(int p)
{
	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
// 	GUI_SendKeyMsg(GUI_KEY_DELETE, 1);
}

static void menu_key_backspace(int p)
{
// 	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
	static int flag;
	int n = 0;
	int num = 0;
	num = DROPDOWN_GetNumItems(cur_dropdown_handle) - 1;
	n = DROPDOWN_GetSel(cur_dropdown_handle);
	
	if(flag == 0)
	{
		if(n == num)
		{
			flag = 1;
			DROPDOWN_DecSelExp(cur_dropdown_handle);
			DROPDOWN_DecSel(cur_dropdown_handle);
		}
		else
		{
			DROPDOWN_IncSelExp(cur_dropdown_handle);
			DROPDOWN_IncSel(cur_dropdown_handle);
		}
	}
	else
	{
		if(n == 0)
		{
			flag = 0;
			DROPDOWN_IncSelExp(cur_dropdown_handle);
			DROPDOWN_IncSel(cur_dropdown_handle);
		}
		else
		{
			DROPDOWN_DecSelExp(cur_dropdown_handle);
			DROPDOWN_DecSel(cur_dropdown_handle);
		}
	}
}

static void menu_key_home(int p)
{
	GUI_SendKeyMsg(GUI_KEY_SPACE, 1);
}

static void menu_key_end(int p)
{
// 	DROPDOWN_IncSel(cur_dropdown_handle);
	DROPDOWN_IncSelExp(cur_dropdown_handle);
}

/* 消除第177号警告 static函数只是为了初始化数据结构而没有直接调用引出的warning */
void dis_warning_177_new(void)
{
	menu_key_home(0);
	menu_key_end(0);
}

static const uint8_t *_widget_dialog_info_buf[][2]=
{
	{"新建文件"	, "CREATE FILE"},
	{"文件名:"	, "FileName"},
	{"工作模式:", "WorkMode"},
	{"蜂鸣时间:", "BuzzerTime"},
	{"APSS保持时间:", "PassHoldTime"},
	{"提示:文件名最大程度12字符","Notice:File name of the maximum length of 12 characters"},
	{"提示:按选择键进行选择不同模式.","Notice:Select different modes by selecting key."},
	{"提示:(0-999.9s).","Notice:(0-999.9s)."},
	{"提示:(0-999.9s).","Notice:(0-999.9s)."},
};

#define DIALOG_NAME_INDEX			0
#define DIALOG_FILE_NAME_INDEX		1
#define WORKMODE_INDEX				2
#define BUZZER_TIME_INDEX			3
#define PASS_HOLD_TIME_INDEX		4
#define FILE_NAME_NOTICE_INDEX		5
#define WORKMODE_NOTICE_INDEX		6
#define BUZZER_NOTICE_INDEX			7
// #define PASS_HOLD_TIME_INDEX		8


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{FRAMEWIN_CreateIndirect, "新建文件", ID_FRAMEWIN_FILE_SAVE, 150, 50, 400, 320, 0, 0x0, 0 },
	
	{TEXT_CreateIndirect, "文件名:", ID_DIALOG_FILE_NAME_TEXT		, 10, 10, 100, 30, 0, 0x64, 0 },
	{EDIT_CreateIndirect, "DEFAULT", ID_DIALOG_FILE_NAME_V_EDIT		, 150, 5, 170,30, 0, 0x64, 0 },
	
	{TEXT_CreateIndirect, "工作模式:", ID_DIALOG_FILE_WORK_MODE_NAME_TEXT	, 10, 40, 170, 30, 0, 0x64, 0 },
	{DROPDOWN_CreateIndirect, "Dropdown", ID_DIALOG_FILE_WORK_MODE_V_DROPDOWN,  180, 40, 100,60, 0, 0x0, 0 },
	
	{TEXT_CreateIndirect, "蜂鸣时间:", ID_DIALOG_FILE_BUZZER_NAME_TEXT		, 10, 70, 170, 30, 0, 0x64, 0 },
	{EDIT_CreateIndirect, "003.0s", ID_DIALOG_FILE_BUZZER_V_EDIT			, 180, 70, 100,30, 0, 0x64, 0 },
	
	{TEXT_CreateIndirect, "PASS保持时间:", ID_DIALOG_FILE_PASS_T_NAME_TEXT	, 10, 100, 170, 30, 0, 0x64, 0 },
	{EDIT_CreateIndirect, "003.0s", ID_DIALOG_FILE_PASS_T_V_EDIT			, 180, 100, 100,30, 0, 0x64, 0 },
	
	{TEXT_CreateIndirect, "NOTICE:", ID_DIALOG_FILE_NOTICE_TEXT	, 10, 140, 170, 30, 0, 0x64, 0 },
};

static void direct_key_up(int data)
{
	int n = COUNT_ARRY_SIZE(par_set_key_server_info);
	int index = 0;
	
// 	index = get_par_set_key_server_array_index(par_set_key_server_info, n, cur_edit_handle);
	
	if(index > 0)
	{
		WM_SetFocus(par_set_key_server_info[index - 1].handle);
	}
}

static void direct_key_down(int data)
{
	int n = COUNT_ARRY_SIZE(par_set_key_server_info);
	int index = 0;
	
// 	index = get_par_set_key_server_array_index(par_set_key_server_info, n, cur_edit_handle);
	
	if(index + 1 < n)
	{
		WM_SetFocus(par_set_key_server_info[index + 1].handle);
	}
}

static _FUNCTION_KEY_INFO_T_ 	direct_key_pool[4]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
};

/*****************************************************************
** FunctionName:void InitDialog(WM_MESSAGE * pMsg)
** Function: to initialize the Dialog items
** 
** callthis function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    WM_HWIN hwidget;
	
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin, &GUI_Fonthz_24);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
//     FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
//     FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin, 35);
	FRAMEWIN_SetText(hWin, (const char*)_widget_dialog_info_buf[DIALOG_NAME_INDEX][SYS_LANGUAGE]);
	
    //
    //GUI_ID_TEXT0
    //
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_NAME_TEXT);
    TEXT_SetFont(hwidget, &GUI_Fonthz_24);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_NAME_V_EDIT);
    EDIT_SetFont(hwidget, &GUI_Fonthz_24);
	TEXT_SetBkColor(hwidget, GUI_BLUE);
	set_par_set_key_server_handle(par_set_key_server_info, COUNT_ARRY_SIZE(par_set_key_server_info),
							ID_DIALOG_FILE_NAME_V_EDIT, hwidget);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_WORK_MODE_NAME_TEXT);
    TEXT_SetFont(hwidget, &GUI_Fonthz_24);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_WORK_MODE_V_DROPDOWN);
    DROPDOWN_AddString(hwidget, "N");
    DROPDOWN_AddString(hwidget, "G");
    DROPDOWN_SetFont(hwidget, &GUI_Fonthz_24);
	DROPDOWN_SetAutoScroll(hwidget, 1);
	cur_dropdown_handle = hwidget;
	set_par_set_key_server_handle(par_set_key_server_info, COUNT_ARRY_SIZE(par_set_key_server_info),
							ID_DIALOG_FILE_WORK_MODE_V_DROPDOWN, hwidget);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_BUZZER_NAME_TEXT);
    TEXT_SetFont(hwidget, &GUI_Fonthz_24);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_BUZZER_V_EDIT);
    EDIT_SetFont(hwidget, &GUI_Fonthz_24);
	set_par_set_key_server_handle(par_set_key_server_info, COUNT_ARRY_SIZE(par_set_key_server_info),
							ID_DIALOG_FILE_BUZZER_V_EDIT, hwidget);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_PASS_T_NAME_TEXT);
    TEXT_SetFont(hwidget, &GUI_Fonthz_24);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_PASS_T_V_EDIT);
    EDIT_SetFont(hwidget, &GUI_Fonthz_24);
	set_par_set_key_server_handle(par_set_key_server_info, COUNT_ARRY_SIZE(par_set_key_server_info),
							ID_DIALOG_FILE_PASS_T_V_EDIT, hwidget);
	
	hwidget = WM_GetDialogItem(hWin, ID_DIALOG_FILE_NOTICE_TEXT);
    TEXT_SetFont(hwidget, &GUI_Fonthz_24);
}

/*********************************************************************
*
* Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_EDIT_WIDGET_CHANGE:
		{
// 			int index = 0;
			void (*fun)(int);
			
// 			index = get_set_par_menu_key_index(par_set_key_server_info,
// 							COUNT_ARRY_SIZE(par_set_key_server_info), cur_edit_handle);
			init_menu_key_info(cur_menu_key_info[0],7);
// 			fun = (void (*)(int))get_edit_widget_key_custom_fun(par_set_key_server_info,
// 							COUNT_ARRY_SIZE(par_set_key_server_info), cur_edit_handle);
			set_scan_key_custom_fun(fun);
			break;
		}
        case WM_PAINT:
//             PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
			init_menu_key_info(cur_menu_key_info[0],7);
// 			set_direct_key_fun(direct_key_pool);
            break;
		case WM_DELETE:
		{
			send_msg_to_parent(hDlg, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
        case WM_KEY:
        {
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        }
        case WM_NOTIFY_PARENT:
        {
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            
            switch (Id)
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                    GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode == WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        }
        default:
            WM_DefaultProc(pMsg);
    }
}

/*********************************************************************
*
* MainTask
*
**********************************************************************
*/
void create_file_new_dialog(int p)
{
// 	set_direct_key_fun(0);
// 	hDlg = GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hWin, 0, 0);//阻塞
	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, p, 0, 0);//非阻塞
}

