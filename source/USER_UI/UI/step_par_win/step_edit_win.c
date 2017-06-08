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
#include "warning_win/warning_win.h"
#include "cs99xx_mem_api.h"
#include "ui_com/com_edit_api.h"
#include "env_par_win/env_par_edit_win.h"
#include "sys_time_win/sys_time_edit_win.h"
#include "password_win/password_win.h"
#include "step_par_win/step_edit_win.h"
#include "step_par_win/7_step_edit_win.h"
#include "type/cs99xx_type.h"

static void cb_step_edit_windows(WM_MESSAGE* pMsg);
static void update_key_inf(WM_HMEM hWin);
static void direct_key_down(int data);
static void direct_key_up(int data);
static void save_setting_step(void);

static UN_STRUCT tmp_step_par;

static WIDGET_POS_SIZE_T* step_edit_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_step_edit_windows,/*4.3寸屏*/
    &_7_step_edit_windows,/*5.6寸屏*/
    &_7_step_edit_windows,/*7寸屏*/
};

typedef enum{
    STEP_EDIT_UI_STEP,///<测试步骤
    STEP_EDIT_UI_MODE,///<测试模式
    STEP_EDIT_UI_END,
}STEP_EDIT_UI_INDEX;

static CS_INDEX step_par_index[]=
{
    STEP_EDIT_UI_STEP,
    STEP_EDIT_UI_MODE,
};

/* 按键菜单 */
static MENU_KEY_INFO_T 	edit_step_menu_key_info[] =
{
    {"", F_KEY_DEL		, KEY_F1 & _KEY_UP,	menu_key_backspace },//f1
    {"", F_KEY_CLEAR    , KEY_F2 & _KEY_UP,	clear_edit_ele },//f2
    {"", F_KEY_NULL		, KEY_F3 & _KEY_UP,	0 },//f3
    {"", F_KEY_NULL		, KEY_F4 & _KEY_UP,	0 },//f4
    {"", F_KEY_NULL		, KEY_F5 & _KEY_UP,	0 },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win },//f6
};
static MENU_KEY_INFO_T 	edit_mode_menu_key_info[] =
{
    {"", F_KEY_CUSTOM   , KEY_F1 & _KEY_UP,	0 },//f1
    {"", F_KEY_CUSTOM   , KEY_F2 & _KEY_UP,	0 },//f2
    {"", F_KEY_CUSTOM   , KEY_F3 & _KEY_UP,	0 },//f3
    {"", F_KEY_CUSTOM   , KEY_F4 & _KEY_UP,	0 },//f4
    {"", F_KEY_CUSTOM   , KEY_F5 & _KEY_UP,	0 },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	back_win },//f6
};


static FUNCTION_KEY_INFO_T 	sys_key_pool[]={
	{KEY_UP		, direct_key_up		},
	{KEY_DOWN	, direct_key_down 	},
	{CODE_LEFT	, direct_key_down   },
	{CODE_RIGH	, direct_key_up     },
};
/************************** step *************************************/
static void edit_step_direct_key_up(int data);
static void edit_step_direct_key_down(int data);

static FUNCTION_KEY_INFO_T 	edit_step_sys_key_pool[]={
	{KEY_UP		, edit_step_direct_key_up   },
	{KEY_DOWN	, edit_step_direct_key_down },
	{CODE_LEFT	, edit_step_direct_key_down },
	{CODE_RIGH	, edit_step_direct_key_up   },
};

static STEP_NUM get_cur_step(void)
{
    return *(STEP_NUM*)g_cur_edit_ele->data.data;
}

static void edit_step_direct_key_up(int data)
{
    uint32_t step = get_edit_num_value(g_cur_edit_ele);
    
    if(step < g_cur_file->total)
    {
        set_edit_num_value(g_cur_edit_ele, step + 1);
        upload_par_to_ram(g_cur_edit_ele);
    }
    else
    {
        step = get_cur_step();
        set_edit_num_value(g_cur_edit_ele, step);
    }
    
    direct_key_up(data);
}

static void edit_step_direct_key_down(int data)
{
    NODE_STEP *node;
    uint32_t step = get_edit_num_value(g_cur_edit_ele);
    
    if(step < g_cur_file->total)
    {
        save_setting_step();
        upload_par_to_ram(g_cur_edit_ele);
        load_steps_to_list(step, 1);
        
        node = get_g_cur_step();
        
        if(NULL != node)
        {
            g_cur_step = node;
            update_group_inf(g_cur_win);
        }
    }
    else
    {
        step = get_cur_step();
        set_edit_num_value(g_cur_edit_ele, step);
    }
    
    direct_key_down(data);
}

static void edit_step_sys_key(int data)
{
    register_system_key_fun(edit_step_sys_key_pool, ARRAY_SIZE(edit_step_sys_key_pool), data);
}

static void step_edit_sys_key(int hWin)
{
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), data);
}
/*********************************************************************/


static void edit_step_menu_key(int hWin)
{
    MENU_KEY_INFO_T * info = edit_step_menu_key_info;
    uint32_t size = ARRAY_SIZE(edit_step_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    
	init_menu_key_info(info, size, data);
}

static void select_test_mode(int data);
#define TRAN_TEST_MODE(mode, n)    (mode - n)
CUSTOM_MENU_KEY_INF mode_inf_pool[]=
{
    {ACW_STR , TRAN_TEST_MODE(ACW, 1), select_test_mode},
    {DCW_STR , TRAN_TEST_MODE(DCW, 1), select_test_mode},
    {IR_STR  , TRAN_TEST_MODE(IR , 1), select_test_mode},
    {GR_STR  , TRAN_TEST_MODE(GR , 1), select_test_mode},
    {BBD_STR , TRAN_TEST_MODE(BBD, 1), select_test_mode},
    {CC_STR  , TRAN_TEST_MODE(CC , 1), select_test_mode},
};

static void select_test_mode(int hWin)
{
    int data = golbal_key_info.custom_data;
    uint32_t size = g_cur_edit_ele->data.bytes;
    
    DROPDOWN_SetSel(hWin, data);
    data = TRAN_TEST_MODE(data , -1);
    DROPDOWN_SetUserData(hWin, &data, size);
}
void init_menu_key_custom_inf(CUSTOM_MENU_KEY_INF *cus_inf,
                            uint16_t cus_size,
                            _WIDGET_ELEMENT_ *edit,
                            MENU_KEY_INFO_T * inf,
                            uint16_t size)
{
    uint8_t ** table = edit->resource.table;
    uint16_t num = edit->resource.size;
    int32_t i = 0;
    int32_t j = 0;
    
    for(i = 0; i < num && i < size; i++)
    {
        if(inf[i].index == F_KEY_CUSTOM)
        {
            inf[i].name = table[i];
            
            /* 匹配定制信息 */
            for(j = 0; j < cus_size; j++)
            {
                /* 如果资源表中的内容与定制信息中的名称一致就初始化数据并退出循环 */
                if(0 == strcmp((const char*)table[i], (const char*)cus_inf[j].name))
                {
                    inf[i].fun_key.key_up_dispose_fun = cus_inf[j].fun;
                    inf[i].fun_key.custom_data = cus_inf[j].data;
                    
                    break;
                }
            }
        }
    }
}
static void edit_mode_menu_key(int hWin)
{
    MENU_KEY_INFO_T * info = edit_mode_menu_key_info;
    uint32_t size = ARRAY_SIZE(edit_mode_menu_key_info);
    int32_t data = g_cur_edit_ele->dis.edit.handle;
    CUSTOM_MENU_KEY_INF *cus_inf = mode_inf_pool;
    uint16_t cus_size = sizeof(mode_inf_pool);
    
    init_menu_key_custom_inf(cus_inf, cus_size, g_cur_edit_ele, info, size);
	init_menu_key_info(info, size, data);
}

static _WIDGET_ELEMENT_ step_par_ele_pool[]={
    {
        {"测试步骤:","TestStep:"}, /* 名称 */
        STEP_EDIT_UI_STEP,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, sizeof(STEP_NUM)/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_EDIT_NUM, E_INT_T},/*类型*/
        {0/*decs*/,2/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {99/*heigh*/,1/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {edit_step_sys_key, edit_step_menu_key, keyboard_fun_num,},/*key_inf*/
    },
    {
        {"测试模式:","TestMode:"}, /* 名称 */
        STEP_EDIT_UI_MODE,/* 通过枚举索引 */
        {0},/* 默认值 */
        {NULL, 1/*数据字节数*/},/* 数据指针 */
        {NULL, 0},/* 资源表 */
        {ELE_DROPDOWN, E_INT_T},/*类型*/
        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
        {MODE_END/*heigh*/,ACW/*low*/,{"Language","Language"}/*notice*/},/*range*/
        {step_edit_sys_key, edit_mode_menu_key, keyboard_fun_num,},/*key_inf*/
    },
//    {
//        {"测试模式:","TestMode:"}, /* 名称 */
//        STEP_EDIT_UI_MODE,/* 通过枚举索引 */
//        {0},/* 默认值 */
//        {NULL, 1/*数据字节数*/},/* 数据指针 */
//        {language_pool, ARRAY_SIZE(language_pool)},/* 资源表 */
//        {ELE_DROPDOWN, E_INT_T},/*类型*/
//        {0/*decs*/,20/*lon*/,NULL_U_NULL/*unit*/,},/*format*/
//        {0/*heigh*/,0/*low*/,{"Language","Language"}/*notice*/},/*range*/
//        {env_langulag_sys_key,env_langulag_menu_key,keyboard_fun_num,},/*key_inf*/
//    },
};

static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win);
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win);

static MYUSER_WINDOW_T step_edit_windows=
{
    {"步骤编辑","step_edit_windows"},
    cb_step_edit_windows, update_key_inf,
	{0},
    {
        step_par_ele_pool,ARRAY_SIZE(step_par_ele_pool),
        (CS_INDEX*)step_par_index,ARRAY_SIZE(step_par_index),
        init_create_step_edit_win_edit_ele,
    },
    {
        com_text_ele_pool,ARRAY_SIZE(com_text_ele_pool),
        (CS_INDEX*)range_group_com_ele_table,ARRAY_SIZE(range_group_com_ele_table),
        init_create_step_edit_win_com_ele,
    },
};

static void set_acw_par_win_ele_data(UN_STRUCT *step)
{
    
}
static void set_dcw_par_win_ele_data(UN_STRUCT *step)
{
    
}
static void set_ir_par_win_ele_data(UN_STRUCT *step)
{
    
}
static void set_gr_par_win_ele_data(UN_STRUCT *step)
{
    
}

void set_step_par_window_ele_data(UN_STRUCT *step)
{
    step_par_ele_pool[STEP_EDIT_UI_STEP].data.data = &step->com.step;
    step_par_ele_pool[STEP_EDIT_UI_MODE].data.data = &step->com.mode;
    step_par_ele_pool[STEP_EDIT_UI_MODE].resource.table = get_defined_mode_table();
    step_par_ele_pool[STEP_EDIT_UI_MODE].resource.size = get_defined_mode_num();
    
    switch(step->com.mode)
    {
        case ACW:
            set_acw_par_win_ele_data(step);
            break;
        case DCW:
            set_dcw_par_win_ele_data(step);
            break;
        case IR:
            set_ir_par_win_ele_data(step);
            break;
        case GR:
            set_gr_par_win_ele_data(step);
            break;
    }
}
static void init_create_step_edit_win_edit_ele(MYUSER_WINDOW_T* win)
{
    memcpy(&tmp_step_par, g_cur_step, sizeof(tmp_step_par));
    tmp_step_par.com.mode = TRAN_TEST_MODE(tmp_step_par.com.mode, 1);
    set_step_par_window_ele_data(&tmp_step_par);//初始化编辑对象的参数
    init_window_edit_ele_list(win);//初始化窗口编辑对象链表
    
    //初始化编辑对象显示信息
    init_window_edit_ele_dis_inf(win, &_7_step_par_edit_ele_auto_layout_inf);
    
    init_window_edit_ele(win);//初始化创建编辑对象
}

void update_group_inf(MYUSER_WINDOW_T* win)
{
    uint8_t buf[10] = {0};
    STEP_NUM step;
    
    set_group_text_ele_inf(COM_UI_CUR_FILE_NAME, win, g_cur_file->name);
    step = g_cur_step->one_step.com.step;
    sprintf((char*)buf, "%d/%d", step, g_cur_file->total);
    set_group_text_ele_inf(COM_UI_CUR_STEP, win, buf);
    strncpy((char*)buf, (const char*)work_mode_pool[g_cur_file->work_mode], 2);
    set_group_text_ele_inf(COM_UI_CUR_WORK_MODE, win, buf);
}
static void init_create_step_edit_win_com_ele(MYUSER_WINDOW_T* win)
{
    init_window_com_ele_list(win);//初始化窗口文本对象链表
    init_com_text_ele_dis_inf(win);//初始化公共文本对象的显示信息
    init_group_com_text_ele_dis_inf(win);//初始化记忆组对象的显示信息
    update_group_inf(win);
    init_window_com_text_ele(win);//初始化创建窗口中的公共文本对象
}

static void update_menu_key_inf(WM_HMEM hWin)
{
//	step_edit_menu_key();
}

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_WHITE);
	GUI_ClearRectEx(&r);
}

static void save_setting_step(void)
{
    FILE_NUM file_num;
    STEP_NUM step_num;
    
    file_num = g_cur_file->num;
    step_num = g_cur_step->one_step.com.step;
    
    memcpy(g_cur_step, &tmp_step_par, sizeof(tmp_step_par));
    save_one_step(g_cur_step, file_num, step_num);
}

static void direct_key_up(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
    save_setting_step();
}

static void direct_key_down(int data)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, _WIDGET_ELEMENT_, e_list);
    }
    select_edit_ele(g_cur_edit_ele);
    save_setting_step();
}

static void update_sys_key_inf(WM_HWIN hWin)
{
    register_system_key_fun(sys_key_pool, ARRAY_SIZE(sys_key_pool), hWin);
}

static void update_key_inf(WM_HWIN hWin)
{
    update_menu_key_inf(hWin);
    update_sys_key_inf(hWin);
}

static void cb_step_edit_windows(WM_MESSAGE* pMsg)
{
	MYUSER_WINDOW_T* win;
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case CM_CHILD_W_MSG:
        {
            update_key_inf(hWin);
            break;
        }
		case WM_CREATE:
            set_user_window_handle(hWin);
			win = get_user_window_info(hWin);
			WM_SetFocus(hWin);/* 设置聚焦 */
            
            init_create_win_all_ele(win);
            
            g_cur_edit_ele = get_cur_win_edit_ele_list_head();//获取当前窗口编辑表头节点
            select_edit_ele(g_cur_edit_ele);
            
            break;
		case WM_TIMER:
			break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
		case WM_PAINT:
			_PaintFrame();
            draw_group_inf_area();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

void create_step_edit_ui(int hWin)
{
    init_window_size(&step_edit_windows, step_edit_win_pos_size_pool[sys_par.screem_size]);
    
    create_user_window(&step_edit_windows, &windows_list, hWin);//创建文件管理界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
