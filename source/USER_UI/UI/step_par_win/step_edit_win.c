
/**
  ******************************************************************************
  * @file    file_ui_save.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
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
#include "app.h"
#include "cs99xx_struct.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "UI_COM/com_ui_info.h"
#include "step_edit_win.h"
#include "ui_com/com_edit_api.h"
#include "key_menu_win/key_menu_win.h"

void _cbCallback(WM_MESSAGE * pMsg);
static FUNCTION_KEY_INFO_T sys_key_pool[];
extern _WIDGET_ELEMENT_ save_file_ele_pool[];
static void direct_key_up(int data);
static void direct_key_down(int data);
static void direct_key_left(int data);
static void direct_key_right(int data);
static void menu_key_ok(int);
static void menu_key_cancle(int);
static void menu_key_backspace(int p);

MYUSER_WINDOW_T* get_save_file_win_struct_addr(void);
extern DISPLAY_INF save_file_ele_pos_pool[];
extern MYUSER_WINDOW_T SaveFileWindows;
extern WIDGET_POS_SIZE_T _7_file_save_windows;

WIDGET_POS_SIZE_T* file_save_win_pos_size_pool[4]=
{
    &_7_step_par_windows,/*4.3寸屏*/
    &_7_step_par_windows,/*5.6寸屏*/
    &_7_step_par_windows,/*7寸屏*/
};

void clear_edit_ele(int hWin)
{
    if(g_cur_edit_ele->dis->edit.handle == NULL)
    {
        return;
    }
    
    EDIT_SetText(g_cur_edit_ele->dis->edit.handle, "");
}
static MENU_KEY_INFO_T 	fname_menu_key_info[] =
/* 文件名 */
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, menu_key_backspace    },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, clear_edit_ele        },//f2
    {"", F_KEY_NULL     , KEY_F3 & _KEY_UP, 0                     },//f3
    {"", F_KEY_NULL     , KEY_F4 & _KEY_UP, 0					    },//f4
    {"", F_KEY_OK       , KEY_F5 & _KEY_UP, menu_key_ok           },//f5
    {"", F_KEY_CANCLE   , KEY_F6 & _KEY_UP, menu_key_cancle       },//f6
};
void set_fwmode_n(int hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis->edit.handle, N_MODE);
}
void set_fwmode_g(int hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis->edit.handle, G_MODE);
}
static MENU_KEY_INFO_T 	fwmode_key_info[] =
/* 工作模式 */
{
    {"N", F_KEY_CUSTOM, KEY_F1 & _KEY_UP, set_fwmode_n        },//f1
    {"G", F_KEY_CUSTOM, KEY_F2 & _KEY_UP, set_fwmode_g        },//f2
    {"" , F_KEY_NULL  , KEY_F3 & _KEY_UP, 0                   },//f3
    {"" , F_KEY_NULL  , KEY_F4 & _KEY_UP, 0                   },//f4
};
static MENU_KEY_INFO_T 	fbeept_key_info[] =
/* 蜂鸣时间 */
{
    {"", F_KEY_DEL      , KEY_F1 & _KEY_UP, menu_key_backspace},//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP, clear_edit_ele    },//f2
    {"" , F_KEY_NULL    , KEY_F3 & _KEY_UP, 0                 },//f3
    {"" , F_KEY_NULL    , KEY_F4 & _KEY_UP, 0                 },//f4
};
static MENU_KEY_INFO_T 	fpasst_key_info[] =
/* PASS时间 */
{
    {"", F_KEY_DEL  , KEY_F1 & _KEY_UP, menu_key_backspace    },//f1
    {"", F_KEY_CLEAR, KEY_F2 & _KEY_UP, clear_edit_ele        },//f2
    {"", F_KEY_NULL , KEY_F3 & _KEY_UP, 0                     },//f3
    {"", F_KEY_NULL , KEY_F4 & _KEY_UP, 0                     },//f4
};

static FUNCTION_KEY_INFO_T sys_key_pool[]={
	{KEY_UP		, direct_key_up		 },
	{KEY_DOWN	, direct_key_down	 },
	{KEY_LEFT	, direct_key_left	 },
	{KEY_RIGHT	, direct_key_right	 },
    
	{CODE_LEFT	, direct_key_up      },
	{CODE_RIGH	, direct_key_down	 },
};

static void fname_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}

static void fwmode_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}
static void fpasstime_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}
static void fbeeptime_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}

static void fname_menu_key()
{
    MENU_KEY_INFO_T * info = fname_menu_key_info;
    uint32_t size = ARRAY_SIZE(fname_menu_key_info);
    int32_t data = g_cur_edit_ele->dis->edit.handle;
    
	init_menu_key_info(info, size, data);
}


static void fwmode_menu_key()
{
    MENU_KEY_INFO_T * info = fwmode_key_info;
    uint32_t size = ARRAY_SIZE(fwmode_key_info);
    int32_t data = g_cur_edit_ele->dis->edit.handle;
    
	init_menu_key_info(info, size, data);
}


static void fpasstime_menu_key()
{
    MENU_KEY_INFO_T * info = fpasst_key_info;
    uint32_t size = ARRAY_SIZE(fpasst_key_info);
    int32_t data = g_cur_edit_ele->dis->edit.handle;
    
	init_menu_key_info(info, size, data);
}
static void fbeeptime_menu_key()
{
    MENU_KEY_INFO_T * info = fbeept_key_info;
    uint32_t size = ARRAY_SIZE(fbeept_key_info);
    int32_t data = g_cur_edit_ele->dis->edit.handle;
    
	init_menu_key_info(info, size, data);
}

_WIDGET_ELEMENT_ save_file_ele_pool[]={
    {
        {"文 件 名:","FileName:"}, /* 名称 */
        FSAVE_UI_FNAME,/* 通过枚举索引 */
        {"DEFAULT","DEFAULT"},/* 默认值 */
        {NULL, D_N_BYTES},/* 数据指针 */
        {NULL,0},/* 资源表 */
        {ELE_EDIT_STR, EDIT_TYPE, E_STRING_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"1-12个字符 \n(A-Z a-z 0-9 空格 + / - .)","1-12 characters\n(A-Z a-z 0-9 space + / - .)"}/*notice*/},/*range*/
        {fname_sys_key,fname_menu_key,keyboard_fun_str,},/*key_inf*/
        &save_file_ele_pos_pool[FSAVE_UI_FNAME],/*dis*/
    },
    {
        {"工作模式:","WorkMode:"}, /* 名称 */
        FSAVE_UI_WMODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_DROPDOWN, EDIT_TYPE, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"WorkMode","WorkMode"}/*notice*/},/*range*/
        {fwmode_sys_key,fwmode_menu_key,keyboard_fun_num,},/*key_inf*/
        &save_file_ele_pos_pool[FSAVE_UI_WMODE],/*dis*/
    },
    {
        {"蜂鸣时间:","BeepTime:"}, /* 名称 */
        FSAVE_UI_BEEPT,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_EDIT_NUM, EDIT_TYPE, E_INT_T},/*类型*/
        {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"BeepTime","BeepTime"}/*notice*/},/*range*/
        {fbeeptime_sys_key,fbeeptime_menu_key,keyboard_fun_num,},/*key_inf*/
        &save_file_ele_pos_pool[FSAVE_UI_BEEPT],/*dis*/
    },
    {
        {"PASS时间:","PassTime:"}, /* 名称 */
        FSAVE_UI_PASST,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 2/*数据字节数*/},/* 数据指针 */
        {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
        {ELE_EDIT_NUM, EDIT_TYPE, E_FLOAT_T},/*类型*/
        {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
        {9999/*heigh*/,0/*low*/,{"PassTime","PassTime"}/*notice*/},/*range*/
        {fpasstime_sys_key,fpasstime_menu_key,keyboard_fun_num,},/*key_inf*/
        &save_file_ele_pos_pool[FSAVE_UI_PASST],/*dis*/
    },
};

COM_TEXT_INDEX com_ele_table[]=
{
	ELE_RANGE_NAME,///<主界面的通信状态
	ELE_RANGE_NOTICE,///<主界面的系统时间
};
static void update_menu_key_inf(WM_HMEM hWin)
{
}

static FSAVE_UI_ENUM fsave_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
static FSAVE_UI_ENUM fnew_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
static FSAVE_UI_ENUM fedit_ui_ele_table[]=
{
    FSAVE_UI_FNAME,
    FSAVE_UI_WMODE,
    FSAVE_UI_BEEPT,
    FSAVE_UI_PASST,
};
MYUSER_WINDOW_T SaveFileWindows=
{
    {"存贮文件","Save As File"},
    _cbCallback,update_menu_key_inf,
    {
        0
    },
    {
        save_file_ele_pool, ARRAY_SIZE(save_file_ele_pool),
        (CS_UI_INDEX*)fsave_ui_ele_table,ARRAY_SIZE(fsave_ui_ele_table)
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_UI_INDEX*)com_ele_table,ARRAY_SIZE(com_ele_table)
    },
};
MYUSER_WINDOW_T NewFileWindows=
{
    {"新建文件","New File"},
    _cbCallback,update_menu_key_inf,
    {
        0
    },
    {
        save_file_ele_pool, ARRAY_SIZE(save_file_ele_pool),
        (CS_UI_INDEX*)fnew_ui_ele_table,ARRAY_SIZE(fnew_ui_ele_table)
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_UI_INDEX*)com_ele_table,ARRAY_SIZE(com_ele_table)
    },
};
MYUSER_WINDOW_T EditFileWindows=
{
    {"编辑文件","Edit File"},
    _cbCallback,update_menu_key_inf,
    {
        0
    },
    {
        save_file_ele_pool, ARRAY_SIZE(save_file_ele_pool),
        (CS_UI_INDEX*)fedit_ui_ele_table,ARRAY_SIZE(fedit_ui_ele_table)
    },
    {
        com_text_ele_pool, ARRAY_SIZE(com_text_ele_pool),
        (CS_UI_INDEX*)com_ele_table,ARRAY_SIZE(com_ele_table)
    },
};

MYUSER_WINDOW_T* get_save_file_win_struct_addr(void)
{
    return &SaveFileWindows;
}

static void menu_key_ok(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_OK;
	g_custom_msg.user_data = (int)&global_file;
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    unregister_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool));
    back_win(hWin);//关闭对话框
}

static void menu_key_cancle(int hWin)
{
	g_custom_msg.msg = CM_DIALOG_RETURN_CANCLE;
    back_win(hWin);//关闭对话框
}

// static void menu_key_delete(int p)
// {
// 	GUI_SendKeyMsg(GUI_KEY_DELETE, 1);
// }

static void menu_key_backspace(int p)
{
	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
}


static void direct_key_up(int data)
{
    dis_select_edit_ele(g_cur_edit_ele);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

static void direct_key_down(int data)
{
    dis_select_edit_ele(g_cur_edit_ele);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

static void direct_key_left(int data)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}

static void direct_key_right(int data)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}

void init_com_text_ele_dis_inf(WM_HWIN hWin)
{
	MYUSER_WINDOW_T* win;
    UI_ELE_DISPLAY_INFO_T dis_info=
    {
        0/*base_x*/,0/*base_y*/,0/*x*/,200/*y*/,10/*width*/,30/*height*/,10,
        &GUI_Fonthz_20, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
    };
    
    win = get_user_window_info(hWin);
    
    dis_info.pos_size.x = 10;
    dis_info.pos_size.y = win->pos_size.height - 45;
    dis_info.pos_size.width = 70;
    dis_info.pos_size.height = 45;
    dis_info.max_len = 100;
    dis_info.font = &GUI_Fonthz_20;
    dis_info.font_color = GUI_BLACK;
    dis_info.back_color = GUI_INVALID_COLOR;
    dis_info.align = GUI_TA_LEFT;
    
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NAME);//范围
    dis_info.pos_size.x += dis_info.pos_size.width;
    dis_info.pos_size.width = win->pos_size.width - 15 -  dis_info.pos_size.width;
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NOTICE);//提示信息
}

void set_file_par_window_ele_data(TEST_FILE *f)
{
    set_edit_ele_data(&save_file_ele_pool[FSAVE_UI_FNAME], f->name);
    set_edit_ele_data(&save_file_ele_pool[FSAVE_UI_WMODE], &f->work_mode);
    set_edit_ele_data(&save_file_ele_pool[FSAVE_UI_BEEPT], &f->buzzer_time);
    set_edit_ele_data(&save_file_ele_pool[FSAVE_UI_PASST], &f->pass_time);
}
/**
  * @brief  回调函数
  * @param  [in] pMsg 窗口消息指针
  * @retval 无
  */
static void _cbCallback(WM_MESSAGE * pMsg)
{
	MYUSER_WINDOW_T* win;
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_EDIT_WIDGET_CHANGE:
		{
            update_menu_key_inf(hWin);
			break;
		}
        case WM_PAINT:
            break;
        case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(hWin);
            update_menu_key_inf(hWin);
            set_file_par_window_ele_data(&global_file);
            init_window_edit_ele(win);
            init_com_text_ele_dis_inf(hWin);//初始化公共文本对象的显示信息
            init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
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
/**
  * @brief  创建对话框用来保存文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_save_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_SAVE);
    init_window_size(&SaveFileWindows, file_save_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&SaveFileWindows, &windows_list, hWin);//创建主界面
}
/**
  * @brief  创建对话框用来新建文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_new_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_NEW);
    init_window_size(&NewFileWindows, file_save_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&NewFileWindows, &windows_list, hWin);//创建主界面
}

/**
  * @brief  创建对话框用来编辑文件
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void create_edit_file_dialog(int hWin)
{
    set_custom_msg_id(CM_FILE_UI_EDIT);
    init_window_size(&EditFileWindows, file_save_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&EditFileWindows, &windows_list, hWin);//创建主界面
}






/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
