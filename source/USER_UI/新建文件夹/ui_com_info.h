/**
  ******************************************************************************
  * @file    7_main_ui_layout.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   7寸屏的布局1
  ******************************************************************************
  */
#ifndef __UI_COM_INFO_H__
#define __UI_COM_INFO_H__

#include "stm32f4xx.h"
#include "GUI.H"
#include "TEXT.h"
#include "ui_com_info.h"
#include "scan_keyboard.h"
#include "7_INCH_ui/com_ui_info.h"

#define CHINESE				0
#define ENGLISH				1
#define SYS_LANGUAGE		CHINESE //ENGLISH //


#define WINDOWS_BAK_COLOR	GUI_BLUE	//窗口背景色




#define CHILD_WIN_BASE_X		0
#define CHILD_WIN_BASE_Y		(30 + 30)
#define CHILD_WIN_BASE_WIDTH	(800 - 110)
#define CHILD_WIN_BASE_HEIGHT	(480 - 25 - CHILD_WIN_BASE_Y)


#define COUNT_ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

#define FILE_NAME_LON		20 //文件名最大长度

/* 根据系统语言选择不同的字符串 */
#define SELE_STR(S1, S2)	(SYS_LANGUAGE==CHINESE?S1:S2)

#define COUNT_ARRY_SIZE(arr)	(sizeof(arr)/sizeof(arr[0]))

#pragma  diag_suppress 870 //去除第870号警告

typedef enum{
	F_KEY_OK,
	F_KEY_CANCLE,
	F_KEY_NEXT,
	F_KEY_DEL,
	F_KEY_BACKSPACE,
	F_KEY_HOME,
	F_KEY_END,
	F_KEY_SELECTED,
	F_KEY_SELECT,
	
	F_KEY_FILE,
	F_KEY_STEP,
	F_KEY_SYS,
	F_KEY_RESULT,
	F_KEY_MORE,
    F_KEY_F0,
    F_KEY_F1,
    F_KEY_F2,
    F_KEY_F3,
    F_KEY_F4,
    F_KEY_F5,
    F_KEY_F6,
    
	F_KEY_NULL,
	F_KEY_CUSTOM,///<自定义
	F_KEY_SAVE,
	F_KEY_NEW,
	F_KEY_READ,
	F_KEY_EDIT,
	F_KEY_BACK,
	F_KEY_DISPLAY,
    F_KEY_TEST,
    F_KEY_HELP,
    F_KEY_VOL,
    F_KEY_CUR,
    F_KEY_RANGE,
    F_KEY_UPPER,
    F_KEY_LOWER,
    F_KEY_TIME,
    F_KEY_MODE,
    F_KEY_CLEAR,
}MENU_KEY_INDEX_ENUM;

/* 用户自定义消息 */
typedef enum{
	CM_CHILD_W_MSG = 0XFFFFF,//子窗口发来的消息 
	CM_FILE_UI_SAVE,
	CM_FILE_UI_NEW,//文件界面
	CM_FILE_UI_READ,//文件读取
	CM_FILE_UI_EDIT,//文件编辑
	CM_FILE_UI_DEL,//文件删除
	CM_EDIT_WIDGET_CHANGE,//当编辑控件改变时程序监控到后向控件的父窗口发送这个消息，通常是为了选择按键服务程序
	CM_DISTROY_DIALOG,//销毁对话框时发给父窗口的消息
	
	CM_DIALOG_RETURN_OK,//子窗口返回时按下了OK键 或 ENTER
	CM_DIALOG_RETURN_CANCLE,//子窗口返回时按下了CANCLE键 或 EXIT
}CUSTOM_MSG;

/* 用户自定义消息结构 */
typedef struct{
	int id;//所在子窗口的自定义ID
	int msg;//命令
	int user_data;//用户数据
}CUSTOM_MSG_T;

typedef void (*KEY_FUN)(int);

// typedef struct{
// 	uint8_t *name;///<按键名称
// 	uint32_t index;//标准菜单键盘索引
// 	KEY_FUN 	key_up_dispose_fun;
// 	uint32_t key_value;///<键值
// 	int en;///<按键使能
// 	int user_data;//传递给键盘处理函数的用户数据
// }_MENU_KEY_INFO_T_;

/** 功能按键结构 */
typedef struct{
	uint32_t key_value;///<键值
	KEY_FUN 	key_up_dispose_fun;
	int en;///<按键使能
	int user_data;//传递给键盘处理函数的用户数据
}_FUNCTION_KEY_INFO_T_;

/** 自定义菜单按键结构 */
typedef struct{
	uint8_t *name;///<按键名称
	uint32_t key_value;///<键值
	KEY_DISPOSE_FUN 	key_up_dispose_fun;///<按键处理函数
	int user_data;///<传递给键盘处理函数的用户数据
	int en;///<按键使能
}CUSTOM_MENU_KEY_INFO_T;


typedef struct{
	uint16_t id;//控件所在父的id
	uint16_t base_x;//控件的相对x基坐标
	uint16_t base_y;//控件的相对y基坐标
	int16_t x;
	int16_t y;
	uint16_t width;
	uint16_t height;
}WIDGET_POS_INFO;





typedef struct{
	uint8_t *text[2];
	uint16_t id;
	const GUI_FONT * pFont;//字体
	GUI_COLOR	font_color;//字体颜色
	GUI_COLOR	bk_color;//背景颜色//GUI_INVALID_COLOR 透明
	int align;/* 对齐方式 */
	WIDGET_POS_INFO *pos;
	TEXT_Handle handle;/* 文本句柄 */
}_USER_WIDGET_INFO;

/** 用户窗口结构 */
typedef struct{
	WM_HMEM handle;///<窗口句柄
	_MENU_KEY_INFO_T_ *p_m_key;///< 菜单键信息
	int user_data;///<用户数据
}USER_WINDOW_T;

typedef void (*MENU_FUN)(uint32_t);
typedef struct {
	int key_value;/* 键值 */
	WM_HWIN handle;/* 控件句柄 */
	int id;/* 控件ID */
	void (*fun)();/* 处理函数 */
}MENU_KEY_INFO;


typedef struct{
	uint8_t *text[2];
	uint32_t index;//键值
}MENU_KEY_INFO_T;


typedef struct{
	int id;
	int menu_key_index;//此索引值，就是给每一个设置参数都可以任意指定需要的菜单键 cur_menu_key_info
	void (*fun_key)();/* 设置该变量的键盘服务函数 */
	WM_HWIN handle;/* edit控件的句柄 */
}PAR_SET_KEY_SERVER_INFO;

#ifdef   UI_COM_INFO_GLOBALS
#define  UI_COM_EXT
#else
#define  UI_COM_EXT  extern
#endif

UI_COM_EXT WM_HWIN cur_active_win_handle;
UI_COM_EXT void (*global_menu_key_dispose_fun)();
UI_COM_EXT CUSTOM_MSG_T 	    g_custom_msg;//用户自定义消息实体变量
UI_COM_EXT USER_WINDOW_T 	    *g_cur_window;//当前窗口
UI_COM_EXT _MENU_KEY_INFO_T_    *g_cur_menu_key_info;

extern uint32_t g_menu_key_value[];

extern void myGUI_DrawRectEx(const GUI_RECT * pRect);


extern void register_menu_key_info(CUSTOM_MENU_KEY_INFO_T *info);
extern void init_user_widget_info(_USER_WIDGET_INFO *info, WM_HWIN hwin);
extern void init_widget_pos_info(_USER_WIDGET_INFO *widget_info, uint16_t widget_num, WIDGET_POS_INFO *pos_info, uint16_t pos_info_num);

extern _USER_WIDGET_INFO *find_user_widget_info(_USER_WIDGET_INFO *info, uint16_t pool_size, uint16_t id);
extern WIDGET_POS_INFO *find_widget_pos_info(WIDGET_POS_INFO *pos_pool,uint32_t pool_size, uint16_t id);
extern TEXT_Handle get_text_widget_handle(_USER_WIDGET_INFO *widget_info, uint16_t widget_num, int Id);

extern void change_color_key_down(WM_HWIN handle);
extern void change_color_key_up(WM_HWIN handle);
extern void init_key_widget_info(_USER_WIDGET_INFO *pool_info, uint16_t info_num, MENU_KEY_INFO *key_info, uint16_t key_info_size);
extern void into_file_manager(int);
extern void into_test_ui(int);

extern void (*global_fun_key_dispose)(uint32_t key_value);
extern void set_global_fun_key_dispose(void (*fun)(uint32_t key_value));

extern void file_ui_menu_key_dispose(void);
extern void create_file_save_dialog(int);
extern void _cbLanguage(WM_MESSAGE* pMsg);

extern void init_menu_key_info(_MENU_KEY_INFO_T_ * info, uint8_t n);
extern void set_menu_key_struct_info(_MENU_KEY_INFO_T_ * info, int data, int size);

extern void _create_menu_key_win(void);

extern void set_direct_key_fun(_FUNCTION_KEY_INFO_T_ *info);
/************ 新建文件 ******************/
extern void create_file_new_dialog(int p);

extern void create_new_file_dialog(int p);
/****************************************/
extern void set_par_set_key_server_handle(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, int id, WM_HWIN handle);
extern PAR_SET_KEY_SERVER_INFO * find_march_edit_widget(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, int id);
extern void *get_edit_widget_key_custom_fun(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle);
extern int get_set_par_menu_key_index(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle);
extern PAR_SET_KEY_SERVER_INFO * find_edit_handle(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle);
extern int get_par_set_key_server_array_index(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle);

extern uint8_t *get_com_menu_key_name(uint32_t index);
extern void init_one_menu_key_info(_MENU_KEY_INFO_T_ *info);
extern void update_menu_key_inf_(void);

#endif //__COM_INFO_H__
