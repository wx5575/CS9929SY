/**
  ******************************************************************************
  * @file    dialog_info.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   用户应用程序
  ******************************************************************************
  */
#ifndef __DIALOG_INFO_H__
#define __DIALOG_INFO_H__


#include "stm32f4xx.h"
#include "GUI.H"
#include "TEXT.h"
#include "scan_keyboard.h"
#include "fonts.h"
#include "ui_com_info.h"
#include "test_ui_base.h"
#include "cs99xx_struct.h"


#ifndef NULL
#define NULL 0
#endif

#define SELETED_COLOR	GUI_LIGHTBLUE

enum{
	ELE_FILE_NAME,			/* 文件名 */
	ELE_FILE_WORK_MODE,		/* 工作模式 */
	ELE_FILE_BUZZER_TIME,	/* 蜂鸣时间 */
};

/** 对象类型 */
// typedef enum{
// 	ELE_TEXT,///< 文本 
// 	ELE_EDIT_NUM,///< 数字编辑框 
// 	ELE_EDIT_STR,///< 字符串编辑框 
// 	ELE_DROPDOWN,///< 下拉选择框 
// }ELE_TYPE;

/** 数据类型 */
// typedef enum{
// 	E_INT_T,///< 整数
// 	E_FLOAT_T,///< 浮点数
// 	E_STRING_T,///< 字符串数据
// 	E_STRUCT_T,///< 结构体数据
// }DATA_TYPE;

// /** 编辑类型 */
// typedef enum{
// 	TEXT_TYPE,///<显示文本控件 他需要进入编辑模式才能进行编辑
// 	EDIT_TYPE,///<显示编辑控件 他可以直接进行编辑
// }ELE_EDIT_TYPE;

// /** 数据字节数 */
// typedef enum{
// 	D_1_BYTES = 1,///< 1字节数据 
// 	D_2_BYTES = 2,///< 2字节数据 
// 	D_4_BYTES = 4,///< 4字节数据 
// 	D_8_BYTES = 8,///< 8字节数据 
// 	D_N_BYTES,///< 数据字节数不定,用于字符串数据和结构体数据 
// }DATA_BYTE_NUM;///< 数据所占的军空间size 

/** */
// typedef struct RANGE_ELEMENT_{
// 	uint8_t *content[2];/* 内容 */
// 	uint16_t x;/* 所在窗口的 x 坐标 */
// 	uint16_t y;/* 所在窗口的 x 坐标 */
// 	uint16_t width;/* 宽 */
// 	uint16_t height;/* 高 */
// 	const GUI_FONT * pFont;//字体
// 	GUI_COLOR	font_color;//字体颜色
// 	GUI_COLOR	bk_color;//背景颜色
// 	int align;/* 对齐方式 */
// 	WM_HMEM	 handle;/* 句柄 */
// 	uint32_t id;/* ID */
// 	uint8_t max_len;/* 最大长度 */
// }RANGE_ELEMENT;

/** 控件显示信息结构体包含了三部分信息 名称:值 单位 */
typedef struct{
	uint16_t x;/* 所在窗口的 x 坐标 */
	uint16_t y;/* 所在窗口的 x 坐标 */
	uint16_t width[3];/* 宽 */
	uint16_t height[3];/* 高 */
	const GUI_FONT * pFont[3];//字体
	GUI_COLOR	font_color[3];//字体颜色
	GUI_COLOR	bk_color[3];//背景颜色
	int align[3];/* 对齐方式 */
}WIDGET_DISPLAY_3INFO;

typedef struct __WIDGET_ELEMENT_{
	uint8_t *name[2];/* 名称 */
	uint8_t *value[10];/* 值 */
	uint8_t unit;/* 单位 */
	void *data;/* 数据指针 */
	void *resource;/* 资源表 */
	uint8_t resource_size;/* 资源表的条数 */
	WIDGET_DISPLAY_3INFO *dis_info;///< 显示相关信息
	WM_HMEM	 handle[3];/* 句柄 */
	uint32_t id[3];/* ID */
	uint8_t max_len[3];/* 最大长度 */
	uint16_t index;/* 通过枚举索引 */
	ELE_TYPE type;/* 对象类型 */
	ELE_EDIT_TYPE edit_type;///<对象的编辑类型
	DATA_TYPE data_type;/* 数据类型 */
	uint8_t bytes;/* 字节数 */
	uint8_t decs;/* 小数点位数 */
	uint8_t lon;/* 数据显示长度,如果是浮点数包含小数点 */
	uint32_t hight_limit;/* 上限 */
	uint32_t low_limit;/* 下限 */
	RANGE_ELEMENT *range;/* 范围 */
	uint8_t *notice;/* 提示信息 */
	_FUNCTION_KEY_INFO_T_ *p_direct_key;/* 方向键信息 */
	_MENU_KEY_INFO_T_ *p_m_key;/* 菜单键信息 */
	void (*fun_key)();/* 设置该变量的键盘服务函数 */
	struct __WIDGET_ELEMENT_ *next;/* 下一个 */
	struct __WIDGET_ELEMENT_ *prev;/* 上一个 */
}WIDGET_ELEMENT;

typedef struct{
	struct __WIDGET_ELEMENT_ *head;/* 下一个 */
	struct __WIDGET_ELEMENT_ *tail;/* 上一个 */
}DIALOG_ELE_LIST;







/**********************************************************************/
enum{
	DIALOG_NEW_FILE,/* 新建文件对话框 */
};

typedef struct{
	uint8_t *name[2];/* 名称 */
	uint16_t x;/* 所在窗口的 x 坐标 */
	uint16_t y;/* 所在窗口的 y 坐标 */
	uint16_t width;/* 宽 */
	uint16_t height;/* 高 */
	uint16_t head_height;/* 对话框头的高度 */
	uint32_t id;/* ID */
	WM_HMEM	 handle;/* 句柄 */
	uint16_t index;/* 索引 */
	const GUI_FONT * pFont;//字体
	GUI_COLOR	font_color;//字体颜色
	GUI_COLOR	bk_color;//背景颜色
	_FUNCTION_KEY_INFO_T_ *p_direct_key;/* 方向键信息 */
	_MENU_KEY_INFO_T_ *p_m_key;/* 菜单键信息 */
}DIALOG_INFO_T;

/**********************************************************************/


#ifdef   DIALOG_INFO_GLOBALS
#define  DIALOG_INFO_EXT
#else
#define  DIALOG_INFO_EXT  extern
#endif

DIALOG_INFO_EXT WIDGET_ELEMENT	widget_element[];
DIALOG_INFO_EXT DIALOG_ELE_LIST	cur_dia_ele_list;/* 当前的Dialog 中元素的链表 */
DIALOG_INFO_EXT DIALOG_INFO_T	dialog_info_t[];
DIALOG_INFO_EXT DIALOG_INFO_T	*p_cur_dialog;/* 指向当前的对话框信息结构 */
DIALOG_INFO_EXT WIDGET_ELEMENT	*g_cur_dia_ele;/* 指向当前正在编辑的对话框对象 */

extern void init_new_file_dialog_ele_list(TEST_FILE *file);
extern void init_serve_dialog_menu_key(DIALOG_INFO_T *dialog);
extern void create_text_element(DIALOG_INFO_T *dialog, WIDGET_ELEMENT *ele);
extern void delete_edit_element(WM_HWIN *hWin);
extern void init_file_name_data(void *name);
extern void init_work_mode_data(void *work_mode);
extern void init_buzzer_time_data(void *time);
extern void create_range_text_element(DIALOG_INFO_T *dialog, WIDGET_ELEMENT *ele);
extern void menu_key_dialog_edit(int p);

#endif //__DIALOG_INFO_H__
