/**
  ******************************************************************************
  * @file    key_menu_win.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#ifndef __KEY_MENU_WIN_H__
#define __KEY_MENU_WIN_H__

#include "scan_keyboard.h"
#include "cs99xx_struct.h"

/**
  * @brief  菜单键项枚举索引
  */
typedef enum{
    KEY_MENU_F0,
    KEY_MENU_F1,
    KEY_MENU_F2,
    KEY_MENU_F3,
    KEY_MENU_F4,
    KEY_MENU_F5,
    KEY_MENU_F6,
}KEY_MENU_ENUM;

/**
  * @brief  标准菜单键名称的枚举索引
  */
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
    F_KEY_DETAIL,///<详细
    F_KEY_INSTER,///<插入
    F_KEY_FORWARD,///<前移
    F_KEY_BACKWARD,///<后移
}MENU_KEY_INDEX_ENUM;

/** 系统标准菜单键初始化信息结构 */
typedef struct{
	uint8_t *text[2];///<初始化菜单键文本，包含中英文
	CS_INDEX index;///<对初始化信息结构数组的索引
}STAND_MENU_KEY_INFO_T;

#define MENU_KEY_NUM			7 /* 按键个数 */

/** 按键回调函数指针 */
typedef void (*KEY_CB_FUN)(int);

/** 功能按键结构 */
typedef struct{
	uint32_t key_value;///<键值
	KEY_CB_FUN key_up_dispose_fun;///<处理函数
	MENU_KEY_ST_ENUM en;///<按键使能
	int user_data;///<传递给键盘处理函数的用户数据
    int custom_data;///<用户定制数据，用于在定制菜单信息时使用
}FUNCTION_KEY_INFO_T;

/** 菜单键的配置信息 */
typedef struct{
	uint8_t *name;///<按键名称
	CS_INDEX index;///<标准菜单键盘索引
    FUNCTION_KEY_INFO_T fun_key;///<功能键信息
}MENU_KEY_INFO_T;

extern void unregister_system_key_fun(FUNCTION_KEY_INFO_T info[], uint32_t n);
extern void register_system_key_fun(FUNCTION_KEY_INFO_T info[], uint32_t n, int data);
extern void create_key_menu_window(void);
extern void init_menu_key_info(MENU_KEY_INFO_T * info, uint32_t n, int data);
extern void display_menu_key(void);
extern void set_menu_function_status(uint32_t key_value, MENU_KEY_ST_ENUM st);

#endif //__KEY_MENU_WIN_H__


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
