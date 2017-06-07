
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
#include "cs99xx_mem_api.h"
#include "UI_COM/com_edit_api.h"
#include "UI_COM/com_ui_info.h"
#include "file_win/file_edit_win.h"
#include "key_menu_win/key_menu_win.h"
#include "7_env_par_edit_win.h"
#include "env_par_edit_win.h"

void _cbCallback(WM_MESSAGE * pMsg);
static FUNCTION_KEY_INFO_T sys_key_pool[];
extern _WIDGET_ELEMENT_ save_file_ele_pool[];
static void direct_key_up(int data);
static void direct_key_down(int data);
static void direct_key_left(int data);
static void direct_key_right(int data);
static void menu_key_ok(int);
//static void menu_key_backspace(int p);

MYUSER_WINDOW_T* get_save_file_win_struct_addr(void);
extern EDIT_ELE_DISPLAY_INF save_file_ele_pos_pool[];
extern MYUSER_WINDOW_T SaveFileWindows;

static WIDGET_POS_SIZE_T* env_par_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_env_par_windows,/*4.3寸屏*/
    &_7_env_par_windows,/*5.6寸屏*/
    &_7_env_par_windows,/*7寸屏*/
};

static void set_env_language_chinese(int hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis.edit.handle, CHINESE);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
}
static void set_env_language_english(int hWin)
{
    DROPDOWN_SetSel(g_cur_edit_ele->dis.edit.handle, ENGLISH);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    update_all_windows_text();
    init_dialog(g_cur_win);//为了更新对话框名字
}
static MENU_KEY_INFO_T 	env_langulag_key_info[] =
/* 语言 */
{
    {"中文"     , F_KEY_CUSTOM, KEY_F1 & _KEY_UP, set_env_language_chinese },//f1
    {"English"  , F_KEY_CUSTOM, KEY_F2 & _KEY_UP, set_env_language_english },//f2
    {""         , F_KEY_NULL  , KEY_F3 & _KEY_UP, 0 },//f3
    {""         , F_KEY_NULL  , KEY_F4 & _KEY_UP, 0 },//f4
    {""         , F_KEY_NULL  , KEY_F5 & _KEY_UP, 0 },//f3
    {""         , F_KEY_BACK  , KEY_F6 & _KEY_UP, menu_key_ok },//f4
};

static FUNCTION_KEY_INFO_T sys_key_pool[]={
	{KEY_UP		, direct_key_up		 },
	{KEY_DOWN	, direct_key_down	 },
	{KEY_LEFT	, direct_key_left	 },
	{KEY_RIGHT	, direct_key_right	 },
    
	{CODE_LEFT	, direct_key_up      },
	{CODE_RIGH	, direct_key_down	 },
};

static void env_langulag_sys_key(int data)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}

static void env_langulag_menu_key()
{
    MENU_KEY_INFO_T * info = env_langulag_key_info;
    uint32_t size = ARRAY_SIZE(env_langulag_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}


_WIDGET_ELEMENT_ env_par_ele_pool[]={
    {
        {"语  言:","Language:"}, /* 名称 */
        ENV_PAR_LANGUAGE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {language_pool, ARRAY_SIZE(language_pool)},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {0/*heigh*/,0/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {env_langulag_sys_key,env_langulag_menu_key,keyboard_fun_num,},/*key_inf*/
    },
//     {
//         {"蜂鸣时间:","BeepTime:"}, /* 名称 */
//         FSAVE_UI_BEEPT,/* 通过枚举索引 */
//         {0},/* 默认值 */
//         {NULL, 2/*数据字节数*/},/* 数据指针 */
//         {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
//         {ELE_EDIT_NUM, E_INT_T},/*类型*/
//         {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
//         {9999/*heigh*/,0/*low*/,{"BeepTime","BeepTime"}/*notice*/},/*range*/
//         {fbeeptime_sys_key,fbeeptime_menu_key,keyboard_fun_num,},/*key_inf*/
//         &save_file_ele_pos_pool[FSAVE_UI_BEEPT],/*dis*/
//     },
//     {
//         {"PASS时间:","PassTime:"}, /* 名称 */
//         FSAVE_UI_PASST,/* 通过枚举索引 */
//         {0},/* 默认值 */
//         {NULL, 2/*数据字节数*/},/* 数据指针 */
//         {work_mode_pool, ARRAY_SIZE(work_mode_pool)},/* 资源表 */
//         {ELE_EDIT_NUM, E_FLOAT_T},/*类型*/
//         {1/*decs*/,5/*lon*/,TIM_U_s/*unit*/,},/*format*/
//         {9999/*heigh*/,0/*low*/,{"PassTime","PassTime"}/*notice*/},/*range*/
//         {fpasstime_sys_key,fpasstime_menu_key,keyboard_fun_num,},/*key_inf*/
//         &save_file_ele_pos_pool[FSAVE_UI_PASST],/*dis*/
//     },
};

static CS_INDEX com_ele_table[]=
{
	ELE_RANGE_NAME,///<主界面的通信状态
	ELE_RANGE_NOTICE,///<主界面的系统时间
};

static void update_menu_key_inf(WM_HMEM hWin)
{
}

static ENV_PAR_UI_INDEX env_par_ui_ele_table[]=
{
    ENV_PAR_LANGUAGE,
};

static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win);
static MYUSER_WINDOW_T env_par_window=
{
    {"环境参数","Env.Par"},
    _cbCallback,update_menu_key_inf,
    {0},
    {
        env_par_ele_pool,ARRAY_SIZE(env_par_ele_pool),
        (CS_INDEX*)env_par_ui_ele_table,ARRAY_SIZE(env_par_ui_ele_table),
        init_create_file_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)com_ele_table,ARRAY_SIZE(com_ele_table),
        init_create_file_edit_win_com_ele,
    },
};

static void menu_key_ok(int hWin)
{
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
    save_sys_par();//保存系统参数
    unregister_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool));
    back_win(hWin);//关闭对话框
}

// static void menu_key_cancle(int hWin)
// {
//     back_win(hWin);//关闭对话框
// }

// static void menu_key_backspace(int p)
// {
// 	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
// }


static void direct_key_up(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
}

static void direct_key_down(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
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

static void init_com_text_ele_dis_inf(WM_HWIN hWin)
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
    dis_info.font[CHINESE] = &GUI_Fonthz_20;
    dis_info.font_color = GUI_BLACK;
    dis_info.back_color = GUI_INVALID_COLOR;
    dis_info.align = GUI_TA_LEFT;
    
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NAME);//范围
    dis_info.pos_size.x += dis_info.pos_size.width;
    dis_info.pos_size.width = win->pos_size.width - 15 -  dis_info.pos_size.width;
    set_com_text_ele_dis_inf(&dis_info, ELE_RANGE_NOTICE);//提示信息
}

static void set_env_par_window_ele_data(SYS_PAR *par)
{
    set_edit_ele_data(&env_par_ele_pool[ENV_PAR_LANGUAGE], &par->language);
}

/**
  * @brief  根据屏幕尺寸初始化界面的编辑对象位置尺寸信息
  * @param  无
  * @retval 无
  */
//static void init_env_par_ui_edit_ele_pos_inf(void)
//{
//    switch(sys_par.screem_size)
//    {
//    case SCREEN_4_3INCH:
//        break;
//    case SCREEN_6_5INCH:
//        break;
//    default:
//    case SCREEN_7INCH:
//        _7_init_env_par_win_edit_ele_pos(env_par_ele_pool);
//        break;
//    }
//}

static void init_create_file_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    set_env_par_window_ele_data(&sys_par);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_env_par_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}

static void init_create_file_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
	WM_HWIN hWin = win->handle;
    
    init_com_text_ele_dis_inf(hWin);//初始化公共文本对象的显示信息
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
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
            win = get_user_window_info(hWin);
            update_win_menu_key(win);
			break;
		}
        case WM_PAINT:
            break;
        case WM_INIT_DIALOG:
            set_user_window_handle(hWin);
            win = get_user_window_info(hWin);
            
            init_dialog(win);
            init_create_win_all_ele(win);
            
//            update_menu_key_inf(hWin);
//            set_env_par_window_ele_data(&sys_par);
//            init_env_par_ui_edit_ele_pos_inf();
//            init_window_edit_ele(win);
//            init_com_text_ele_dis_inf(hWin);//初始化公共文本对象的显示信息
//            init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            break;
		case WM_DELETE:
		{
			send_no_par_msg_to_parent(hWin, CM_CHILD_W_MSG);
			break;
		}
        case WM_KEY:
        {
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
void create_env_par_dialog(int hWin)
{
    init_window_size(&env_par_window, env_par_win_pos_size_pool[sys_par.screem_size]);
    create_user_dialog(&env_par_window, &windows_list, hWin);//创建主界面
}





/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
