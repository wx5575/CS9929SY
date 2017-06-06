/**
  ******************************************************************************
  * @file    dialog_info.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   用户应用程序
  ******************************************************************************
  */
#define DIALOG_INFO_GLOBALS
// #include "dialog_info.h"
#include "app.h"
#include "DROPDOWN.H"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "DROPDOWN.H"
#include "string.h"
#include "keyboard.h"
#include "stdio.h"
#include "stdlib.h"
#include "tools.h"
#include "cs99xx_struct.h"


enum{
	GLOBAL_WIDGET_ID = (GUI_ID_USER + 0x00),
};
extern void init_menu_key_info_dialog(WIDGET_ELEMENT *ele);
static void menu_key_dialog_next(int p);
static void menu_key_dialog_prev(int p);
static void menu_key_ok(int p);
static void menu_key_dialog_ok(int p);
static void exit_edit_mode(void);
void menu_key_dialog_edit(int p);

void create_text_element(DIALOG_INFO_T *dialog, WIDGET_ELEMENT *ele)
{
	WM_HWIN hwidget;
	uint8_t index = 1;
	
	index = 1;
	hwidget = TEXT_CreateEx(ele->dis_info->x + ele->dis_info->width[0], ele->dis_info->y,
							ele->dis_info->width[index], ele->dis_info->height[index],
							dialog->handle, WM_CF_SHOW,
							ele->dis_info->align[index],
							ele->id[0],
							(const char*)ele->value[0]);
	ele->handle[index] = hwidget;
	TEXT_SetFont(hwidget, ele->dis_info->pFont[index]);
	TEXT_SetBkColor(hwidget, ele->dis_info->bk_color[index]);
	TEXT_SetTextColor(hwidget, ele->dis_info->font_color[index]);
	
	if(ele->data != NULL)
	switch(ele->type)
	{
		case ELE_TEXT:/* 文本 */
			break;
		case ELE_EDIT_STR:/* 字符串编辑框 */
		{
			uint8_t t_buf[20] = {0};
			
			read_par_to_buf(ele, t_buf);
			TEXT_SetText(hwidget, (const char*)t_buf);
			break;
		}
		case ELE_EDIT_NUM:/* 数字编辑框 */
		{
			uint8_t t_buf[20] = {0};
			uint16_t value = 0;
			
			read_par_to_buf(ele, &value);
			
			mysprintf(t_buf, NULL, 100 + ele->lon * 10 + ele->decs, value);
			TEXT_SetText(hwidget, (const char*)t_buf);
			break;
		}
		case ELE_DROPDOWN:/* 下拉选择框 */
		{
			uint8_t n = 0;
			
			read_par_to_buf(ele, &n);
			TEXT_SetText(hwidget, ((const char**)ele->resource)[n]);
			break;
		}
	}
}

void create_range_text_element(DIALOG_INFO_T *dialog, WIDGET_ELEMENT *ele)
{
	WM_HWIN hwidget;
	uint8_t buf[100] = {0};
	uint8_t high[20] = {0};
	uint8_t low[20] = {0};
	
	if(ele == NULL || ele->range == NULL)
	{
		return;
	}
	
	if(ele->range->handle != 0)
	{
// 	delete_edit_element(&ele->range->handle);
		return;
	}
	
	hwidget = TEXT_CreateEx(ele->range->x, ele->range->y,
							ele->range->width, ele->range->height,
							dialog->handle, WM_CF_SHOW,
							ele->range->align,
							ele->range->id,
							0);
	
	ele->range->handle = hwidget;
	
	if(ele->type == ELE_EDIT_NUM)
	{
		mysprintf(high, unit_pool[ele->unit], 100 + 10 * ele->lon + ele->decs, ele->hight_limit);
		mysprintf(low , unit_pool[ele->unit], 100 + 10 * ele->lon + ele->decs, ele->low_limit);
		sprintf((char*)buf, "%s:%s->%s",ele->range->content[SYS_LANGUAGE], low, high);
	}
	else if(ele->type == ELE_EDIT_STR)
	{
		sprintf((char*)buf, "%s:%s", ele->range->content[SYS_LANGUAGE], ele->notice);
	}
	else if(ele->type == ELE_DROPDOWN)
	{
		uint8_t i = 0;
		
		strcat((char*)buf, (const char*)ele->range->content[SYS_LANGUAGE]);
		strcat((char*)buf, ":");
		
		for(i = 0; i < ele->resource_size; i++)
		{
			strcat((char*)buf, ((const char**)ele->resource)[i]);
			if(i + 1 < ele->resource_size)
				strcat((char*)buf, ",");
		}
	}
	
	TEXT_SetText(hwidget, (const char*)buf);
	
	TEXT_SetFont(hwidget, ele->range->pFont);
	TEXT_SetBkColor(hwidget, ele->range->bk_color);
	TEXT_SetTextColor(hwidget, ele->range->font_color);
}

void create_edit_element(DIALOG_INFO_T *dialog, WIDGET_ELEMENT *ele)
{
	WM_HWIN hwidget = 0;
	uint8_t index = 1;
	
	switch(ele->type)
	{
		case ELE_TEXT:/* 文本 */
			break;
		case ELE_EDIT_NUM:/* 数字编辑框 */
		{
			hwidget = EDIT_CreateEx(ele->dis_info->x + ele->dis_info->width[0], ele->dis_info->y,
						ele->dis_info->width[index], ele->dis_info->height[index],
						dialog->handle, WM_CF_SHOW,
						ele->dis_info->align[index],
						ele->id[0] + 1,
						ele->max_len[index]);
			
			ele->handle[index] = hwidget;
			EDIT_SetFont(hwidget, ele->dis_info->pFont[index]);
			EDIT_SetBkColor(hwidget, EDIT_CI_ENABLED, ele->dis_info->bk_color[index]);
			EDIT_SetTextColor(hwidget, EDIT_CI_ENABLED, ele->dis_info->font_color[index]);
			
			EDIT_SetText(hwidget, "");
			
			if(ele->data != NULL)
			{
			}
			break;
		}
		case ELE_EDIT_STR:/* 字符串编辑框 */
		{
			hwidget = EDIT_CreateEx(ele->dis_info->x + ele->dis_info->width[0], ele->dis_info->y,
						ele->dis_info->width[index], ele->dis_info->height[index],
						dialog->handle, WM_CF_SHOW,
						ele->dis_info->align[index],
						ele->id[0] + 1,
						ele->max_len[index]);
			
			ele->handle[index] = hwidget;
			EDIT_SetFont(hwidget, ele->dis_info->pFont[index]);
			EDIT_SetBkColor(hwidget, EDIT_CI_ENABLED, ele->dis_info->bk_color[index]);
			EDIT_SetTextColor(hwidget, EDIT_CI_ENABLED, ele->dis_info->font_color[index]);
			
			EDIT_SetText(hwidget, (const char*)ele->value[0]);
			
			if(ele->data != NULL)
				EDIT_SetText(hwidget, (const char*)ele->data);
			break;
		}
		case ELE_DROPDOWN:/* 下拉选择框 */
		{
			hwidget = DROPDOWN_CreateEx(ele->dis_info->x + ele->dis_info->width[0], ele->dis_info->y,
									ele->dis_info->width[index], ele->dis_info->height[index],
									dialog->handle, WM_CF_SHOW,
									ele->dis_info->align[index],
									ele->id[0] + 1);
			
			ele->handle[index] = hwidget;
			
			DROPDOWN_AddString(hwidget, (const char*)ele->value[0]);
			DROPDOWN_AddString(hwidget, (const char*)ele->value[1]);
			DROPDOWN_SetFont(hwidget, ele->dis_info->pFont[index]);
			DROPDOWN_SetAutoScroll(hwidget, 1);
			DROPDOWN_SetTextAlign(hwidget, ele->dis_info->align[index]);
			
			if(ele->data != NULL)
				DROPDOWN_SetSel(hwidget, *(uint8_t*)ele->data);
			break;
		}
	}
}

void delete_edit_element(WM_HWIN *hWin)
{
	WM_DeleteWindow(*hWin);
	*hWin = 0;
}

// static void menu_key_selected(int p)
// {
// }

static void menu_key_select(int p)
{
	static int flag;
	int n = 0;
	int num = 0;
	WM_HWIN handle = g_cur_dia_ele->handle[1];
	
	num = DROPDOWN_GetNumItems(handle) - 1;
	n = DROPDOWN_GetSel(handle);
	
	if(flag == 0)
	{
		if(n == num)
		{
			flag = 1;
			DROPDOWN_DecSelExp(handle);
			DROPDOWN_DecSel(handle);
		}
		else
		{
			DROPDOWN_IncSelExp(handle);
			DROPDOWN_IncSel(handle);
		}
	}
	else
	{
		if(n == 0)
		{
			flag = 0;
			DROPDOWN_IncSelExp(handle);
			DROPDOWN_IncSel(handle);
		}
		else
		{
			DROPDOWN_DecSelExp(handle);
			DROPDOWN_DecSel(handle);
		}
	}
// 	DROPDOWN_IncSelExp(g_cur_dia_ele->handle[1]);
// 	GUI_SendKeyMsg(GUI_KEY_SPACE, 1);
// 	int n = 0;
// 	
// 	if(g_cur_dia_ele->type == ELE_DROPDOWN)
// 	{
// 		n = DROPDOWN_GetSel(g_cur_dia_ele->handle[1]);
// 		DROPDOWN_SetSel(g_cur_dia_ele->handle[1], n);
// 		DROPDOWN_Collapse(g_cur_dia_ele->handle[1]);
// // 		WM_SetFocus(g_cur_dia_ele->next->handle[1]);
// 	}
}

void init_serve_dialog_menu_key(DIALOG_INFO_T *dialog)
{
	init_menu_key_info(dialog->p_m_key,7);
}

void get_eidt_element_new_value(WIDGET_ELEMENT *ele)
{
	
	switch(ele->type)
	{
		case ELE_TEXT:/* 文本 */
			break;
		case ELE_EDIT_NUM:/* 数字编辑框 */
		{
			uint8_t t_buf[20] = {0};
			uint32_t value = 0;
			
			EDIT_GetText(ele->handle[1], (char*)t_buf, ele->max_len[1]);
			
			value = atof((const char*)t_buf) * ten_power(ele->decs);
			
			if(value > ele->hight_limit)
			{
				value = ele->hight_limit;
			}
			
			save_par_to_ram(ele, &value);
			break;
		}
		case ELE_EDIT_STR:/* 字符串编辑框 */
		{
			uint8_t t_buf[20] = {0};
			
			EDIT_GetText(ele->handle[1], (char*)t_buf, ele->max_len[1]);
			
			if(t_buf[0] != 0)
			{
				save_par_to_ram(ele, t_buf);
			}
			
			break;
		}
		case ELE_DROPDOWN:
		{
			uint8_t n = 0;
			
			n = DROPDOWN_GetSel(ele->handle[1]);
			save_par_to_ram(ele, &n);
			break;
		}
	}
}


static void menu_key_ok(int p)
{
// 	if(cur_edit_handle != 0)
	{
// 		get_eidt_element_new_value(g_cur_dia_ele);//获取编辑对象的新值
		exit_edit_mode();
	}
}

static void menu_key_cancle(int p)
{
	exit_edit_mode();
}

static void menu_key_delete(int p)
{
	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
// 	GUI_SendKeyMsg(GUI_KEY_DELETE, 1);
}

// static void menu_key_backspace(int p)
// {
// }

enum{
	FILE_NAME_I,
	WORK_MODE,
	BUZZER_TIME,
}MENU_KEY_ENUM_INDEX;

/* 当前使用界面对应的菜单键信息与响应函数 */
static _MENU_KEY_INFO_T_ 	cur_menu_key_info[][6] = {
	/* 文件名 */
	{
		{"", F_KEY_OK		, menu_key_ok		},//f1
		{"", F_KEY_CANCLE	, menu_key_cancle	},//f2
		{"", F_KEY_DEL		, menu_key_delete	},//f3
		{"", F_KEY_NULL		, 0					},//f4
		{"", F_KEY_NULL		, 0					},//f5
		{"", F_KEY_NULL		, 0					},//f6
	},
	/* 工作模式 */
	{
		{"", F_KEY_OK		, menu_key_ok		},//f1
		{"", F_KEY_CANCLE	, menu_key_cancle	},//f2
		{"", F_KEY_CUSTOM	, menu_key_select	},//f3
		{"", F_KEY_CUSTOM	, 0					},//f4
		{"", F_KEY_NULL		, 0					},//f5
		{"", F_KEY_NULL		, 0					},//f6
	},
	/* 蜂鸣时间 */
	{
		{"", F_KEY_OK		, menu_key_ok		},//f1
		{"", F_KEY_CANCLE	, menu_key_cancle	},//f2
		{"", F_KEY_NULL		, 0					},//f3
		{"", F_KEY_NULL		, 0					},//f4
		{"", F_KEY_NULL		, 0					},//f5
		{"", F_KEY_NULL		, 0					},//f6
	},
	/* PASS保持时间 */
	{
		{"", F_KEY_OK		, menu_key_ok		},//f1
		{"", F_KEY_CANCLE	, menu_key_cancle	},//f2
		{"", F_KEY_NULL		, 0					},//f3
		{"", F_KEY_NULL		, 0					},//f4
		{"", F_KEY_NULL		, 0					},//f5
		{"", F_KEY_NULL		, 0					},//f6
	},
};


static void direct_key_lock(int data)
{
}
static void direct_key_offset(int data)
{
}
// static void direct_key_enter(int data)
// {
// 	menu_key_ok(0);
// }
static void direct_key_exit(int data)
{
// 	GUI_SendKeyMsg(GUI_KEY_UP, 1);
	menu_key_dialog_prev(0);
}
static void direct_key_up(int data)
{
// 	GUI_SendKeyMsg(GUI_KEY_UP, 1);
	menu_key_dialog_prev(0);
}
static void direct_key_down(int data)
{
// 	GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
	menu_key_dialog_next(0);
}
static void direct_key_left(int data)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, 1);
}
static void direct_key_right(int data)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, 1);
}

static _FUNCTION_KEY_INFO_T_ 	direct_key_pool[]={
	{KEY_UP		, direct_key_up		 },
	{KEY_DOWN	, direct_key_down	 },
	{KEY_LEFT	, direct_key_left	 },
	{KEY_RIGHT	, direct_key_right	 },
	
	{KEY_UNLOCK	, direct_key_lock	 },
	{KEY_OFFSET	, direct_key_offset	 },
	{KEY_ENTER	, menu_key_dialog_edit },
	{KEY_EXIT	, direct_key_exit	 },
};

RANGE_ELEMENT range_element=
{
	{"范围","Range"},///< 内容
	10/*x*/,280/*y*/,
	380/* width */, 30/* height */,
	&GUI_Fonthz_24,///< 字体
	GUI_BLACK,///<字体颜色
	GUI_INVALID_COLOR,///< 背景颜色
	GUI_TA_LEFT,///< 对齐方式
	0,///< 句柄
	GLOBAL_WIDGET_ID+3*50,///< ID
	100,///< max_lon
};

uint8_t buf[10] = "abcde";
uint8_t vvv = 1;
uint16_t bz_time = 0;

void init_file_name_data(void *name)
{
	widget_element[ELE_FILE_NAME].data = name;
}

void init_work_mode_data(void *work_mode)
{
	widget_element[ELE_FILE_WORK_MODE].data = work_mode;
}

void init_buzzer_time_data(void *time)
{
	widget_element[ELE_FILE_BUZZER_TIME].data = time;
}

WIDGET_DISPLAY_3INFO widget_display_info[]=
{
	{
		10,50,/* x, y */
		{120,120,25},/* width */
		{30	,30	,30},/* height */
		{&GUI_Fonthz_24,&GUI_Fonthz_24,&GUI_Fonthz_24},//字体
		{GUI_BLACK, GUI_BLACK, GUI_BLACK},//字体颜色
		{GUI_INVALID_COLOR, GUI_INVALID_COLOR, GUI_INVALID_COLOR},//背景颜色
		{GUI_TA_RIGHT, GUI_TA_RIGHT, GUI_TA_LEFT},/* 对齐方式 */
	},
	{
		10,80,/* x, y */
		{120,120,25},/* width */
		{30	,30	,30},/* height */
		{&GUI_Fonthz_24,&GUI_Fonthz_24,&GUI_Fonthz_24},//字体
		{GUI_BLACK, GUI_BLACK, GUI_BLACK},//字体颜色
		{GUI_INVALID_COLOR, GUI_INVALID_COLOR, GUI_INVALID_COLOR},//背景颜色
		{GUI_TA_RIGHT, GUI_TA_CENTER, GUI_TA_LEFT},/* 对齐方式 */
	},
	{
		10,120,/* x, y */
		{120,120,25},/* width */
		{30	,30	,30},/* height */
		{&GUI_Fonthz_24,&GUI_Fonthz_24,&GUI_Fonthz_24},//字体
		{GUI_BLACK, GUI_BLACK, GUI_BLACK},//字体颜色
		{GUI_INVALID_COLOR, GUI_INVALID_COLOR, GUI_INVALID_COLOR},//背景颜色
		{GUI_TA_RIGHT, GUI_TA_RIGHT, GUI_TA_LEFT},/* 对齐方式 */
	},
};

WIDGET_ELEMENT	widget_element[]=
{
	{
		{"文 件 名:", "FileName"},/* name */{"Default","Default"},/* 默认值 */
		NULL_U_NULL,/* 单位 */buf,/* 数据 */
		NULL,/* 资源表 */0,/* 资源表条数 */&widget_display_info[0],
		{0,0,0},/* 句柄 */{GLOBAL_WIDGET_ID+3*0},/* ID */
		{10,10,10},/* max_lon */
		ELE_FILE_NAME,/* 枚举索引 */
		ELE_EDIT_STR,/* 对象类型 */TEXT_TYPE,/*/<对象编辑类型 */E_STRING_T,/* 数据类型 */
		D_N_BYTES/* bytes */,1/* decs */,5/* lon */,
		0/*high_limit*/,0/*low_limit*/,
		&range_element,/* 范围 */
		"0-9,A-Z",direct_key_pool,/*/< 方向键注册 */cur_menu_key_info[0],
		keyboard_fun_str,
	},
	{
		{"工作模式:", "WorkMode"},/* name */{"N","G"},/* 默认值 */
		NULL_U_NULL,/* 单位 */&vvv,/* 数据 */
		work_mode_pool,/* 资源表 */	COUNT_ARRAY_SIZE(work_mode_pool),/* 资源表条数 */
		&widget_display_info[1],
		{0,0,0},/* 句柄 */{GLOBAL_WIDGET_ID+3*1},/* ID */
		{10,10,10},/* max_lon */
		ELE_FILE_WORK_MODE,/* 枚举索引 */
		ELE_DROPDOWN,/* 对象类型 */	TEXT_TYPE,/*/<对象编辑类型*/E_INT_T,/* 数据类型 */
		D_1_BYTES/* bytes */,1/* decs */,5/* lon */,
		0/*high_limit*/,0/*low_limit*/,
		&range_element,/* 范围 */
		"",/* 提示信息 */
		direct_key_pool,cur_menu_key_info[1],
		keyboard_fun_num,
	},
	{
		{"蜂鸣时间:", "BuzzerTime"},/* name */{"000.0","000.0"},/* 默认值 */
		TIM_U_s,/* 单位 */&bz_time,/* 数据 */
        NULL,/* 资源表 */0,/* 资源表条数 */
		&widget_display_info[2],
		{0,0,0},/* 句柄 */
		{GLOBAL_WIDGET_ID+3*2},/* ID */
		{10,6,10},/* 字符串包含结束符 */
		ELE_FILE_NAME,/* 枚举索引 */
		ELE_EDIT_NUM,/* 对象类型 */TEXT_TYPE,/*/<对象编辑类型*/E_FLOAT_T,/* 数据类型 */
		D_2_BYTES/* bytes */,1/* decs */,5/* lon */,
		9999/*high_limit*/,0/*low_limit*/,
		&range_element,/* 范围 */
		"",/* 提示信息 */
		direct_key_pool,cur_menu_key_info[2],keyboard_fun_num,
	},
	{
		0
	},
};

void init_dialog_ele_list(DIALOG_ELE_LIST *list)
{
	list->head = NULL;
	list->tail = NULL;
}

void tail_insert_ele(DIALOG_ELE_LIST *list, WIDGET_ELEMENT *ele, void *data)
{
	ele->data = data;
	
	if(list->tail == NULL)
	{
		list->head = ele;
	}
	else
	{
		list->tail->next = ele;
		ele->prev = list->tail;
	}
	
	list->tail = ele;
	
	list->tail->next = list->head;
	list->head->prev = list->tail;
}

/** 新建文件对话框中元素设置路径 */
void new_file_dialog_ele_set_way1(TEST_FILE *file)
{
	init_dialog_ele_list(&cur_dia_ele_list);
	tail_insert_ele(&cur_dia_ele_list, &widget_element[ELE_FILE_NAME], file->name);
	tail_insert_ele(&cur_dia_ele_list, &widget_element[ELE_FILE_WORK_MODE], &file->work_mode);
	tail_insert_ele(&cur_dia_ele_list, &widget_element[ELE_FILE_BUZZER_TIME], &file->buzzer_time);
}

/** 初始化新建文件对话框元素链表 */
void init_new_file_dialog_ele_list(TEST_FILE *file)
{
	new_file_dialog_ele_set_way1(file);
	
	/* 调整坐标,完成自动布局 */
}

void distroy_dialog_ele(void)
{
	struct __WIDGET_ELEMENT_ *node;
	struct __WIDGET_ELEMENT_ *end = cur_dia_ele_list.tail;
	
	for(node = cur_dia_ele_list.head; ;node=node->next)
	{
		delete_edit_element(&g_cur_dia_ele->range->handle);
		delete_edit_element(&g_cur_dia_ele->handle[0]);
		delete_edit_element(&g_cur_dia_ele->handle[1]);
		delete_edit_element(&g_cur_dia_ele->handle[2]);
		
		if(node == end)
		{
			break;
		}
	}
}
/****************************对话框级别的操作****************************************/
static void menu_key_dialog_ok(int p)
{
// 	delete_edit_element(&g_cur_dia_ele->range->handle);
	distroy_dialog_ele();
	GUI_EndDialog(p_cur_dialog->handle, 0);
}
static void menu_key_dialog_cancle(int p)
{
	distroy_dialog_ele();
// 	delete_edit_element(&g_cur_dia_ele->range->handle);
	GUI_EndDialog(p_cur_dialog->handle, 1);
}

void default_menu_dispose_fun(int data)
{
// 	WIDGET_ELEMENT *ele = (WIDGET_ELEMENT *)data;
}

// void create_all_menu_key(WIDGET_ELEMENT *ele)
// {
// 	int32_t i = 0;
// 	int32_t j = 0;
// 	
// // 	for(i = 0; i < 6; i++)
// // 	{
// // 		if(F_KEY_CUSTOM == ele->p_m_key[i].index)
// // 		{
// // 			ele->p_m_key[i].name = ((uint8_t**)ele->resource)[j];
// // 			ele->p_m_key[i].key_up_dispose_fun = default_menu_dispose_fun;
// // 			ele->p_m_key[i].key_value = g_menu_key_value[i];
// // 			ele->p_m_key[i].user_data = *(uint32_t*)ele;
// // 			j++;
// // 		}
// // 		else
// // 		{
// // 			ele->p_m_key[i].name = get_com_menu_key_name(ele->p_m_key[i].index);
// // 		}
// // 	}
// }

static void select_menu_key_dialog_ele(int x)
{
	TEXT_SetBkColor(g_cur_dia_ele->handle[0], g_cur_dia_ele->dis_info->bk_color[1]);
	TEXT_SetBkColor(g_cur_dia_ele->handle[1], g_cur_dia_ele->dis_info->bk_color[1]);
	TEXT_SetBkColor(g_cur_dia_ele->handle[2], g_cur_dia_ele->dis_info->bk_color[1]);
	delete_edit_element(&g_cur_dia_ele->range->handle);
	
	if(x == 0)
	{
		g_cur_dia_ele = g_cur_dia_ele->next;
	}
	else
	{
		g_cur_dia_ele = g_cur_dia_ele->prev;
	}
	
	TEXT_SetBkColor(g_cur_dia_ele->handle[0], SELETED_COLOR);
	TEXT_SetBkColor(g_cur_dia_ele->handle[1], SELETED_COLOR);
	TEXT_SetBkColor(g_cur_dia_ele->handle[2], SELETED_COLOR);
	create_range_text_element(p_cur_dialog, g_cur_dia_ele);
	
	if(g_cur_dia_ele->edit_type == EDIT_TYPE)
	{
// 		create_all_menu_key(g_cur_dia_ele);
		init_menu_key_info_dialog(g_cur_dia_ele);
		set_scan_key_custom_fun(g_cur_dia_ele->fun_key);
	}

// 	cur_edit_handle = 0;
}
static void menu_key_dialog_next(int p)
{
	select_menu_key_dialog_ele(0);
}
static void menu_key_dialog_prev(int p)
{
	select_menu_key_dialog_ele(1);
}


static void exit_edit_mode(void)
{
	delete_edit_element(&g_cur_dia_ele->handle[1]);
	create_text_element(p_cur_dialog, g_cur_dia_ele);
	TEXT_SetBkColor(g_cur_dia_ele->handle[1], SELETED_COLOR);
	init_serve_dialog_menu_key(p_cur_dialog);
// 	cur_edit_handle = 0;
	enable_function_key(KEY_UP, ENABLE);
	enable_function_key(KEY_DOWN, ENABLE);
// 	register_key_dispose_fun(KEY_ENTER, menu_key_dialog_edit);
}

void menu_key_dialog_edit(int p)
{
	delete_edit_element(&g_cur_dia_ele->handle[1]);
// 	set_direct_key_fun(g_cur_dia_ele->p_direct_key);
	init_menu_key_info_dialog(g_cur_dia_ele);
	create_edit_element(p_cur_dialog, g_cur_dia_ele);
	create_range_text_element(p_cur_dialog, g_cur_dia_ele);
	WM_SetFocus(g_cur_dia_ele->handle[1]);
	enable_function_key(KEY_UP, DISABLE);
	enable_function_key(KEY_DOWN, DISABLE);
// 	register_key_dispose_fun(KEY_ENTER, menu_key_ok);
}

static _MENU_KEY_INFO_T_ 	dialog_menu_key_info[][6]=
{
	{
		{"", F_KEY_NULL, menu_key_dialog_ok			},//f1
		{"", F_KEY_NULL, menu_key_dialog_cancle		},//f2
		{"", F_KEY_NULL, menu_key_dialog_next		},//f3
		{"", F_KEY_NULL, menu_key_dialog_edit		},//f4
		{"", F_KEY_NULL, 0							},//f5
		{"", F_KEY_BACK, menu_key_dialog_ok			},//f6
	},
};

DIALOG_INFO_T	dialog_info_t[]=
{
	{
		{"新建文件", "New File"},/* 名称 */
		150, 50,/* 坐标 */
		400, 320,/* width height */
		35,/* head_height */
		GLOBAL_WIDGET_ID + 200,/* id */
		0,/* 句柄 */
		DIALOG_NEW_FILE,/* 索引 */
		&GUI_Fonthz_24,
		GUI_BLACK,
		GUI_LIGHTGRAY,
		direct_key_pool,
		dialog_menu_key_info[0],
	},
};

