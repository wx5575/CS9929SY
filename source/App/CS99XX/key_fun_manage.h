/**
  ******************************************************************************
  * @file    key_fun_manage.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键功能管理,按键功能的注册和查询
  ******************************************************************************
  */
#ifndef __KEY_FUN_MANAGE_H__
#define __KEY_FUN_MANAGE_H__


/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "cs99xx_struct.h"

typedef enum{
	MENU_KEY_EN,//菜单键使能
	MENU_KEY_DIS,//菜单键失能
}MENU_KEY_ST_ENUM;

typedef struct{
	int user_data;///<传递给键盘处理函数的用户数据
    int custom_data;///<用户定制数据，用于在定制菜单信息时使用
}KEY_MESSAGE;

/** 按键回调函数指针 */
typedef void (*KEY_CB_FUN)(KEY_MESSAGE *);

typedef struct
{
	KEY_CB_FUN fun;///<按键处理函数
	KEY_MESSAGE msg;///<按键消息
	int en;///<使能按键
}KEY_DISPOSE_FUN;

typedef struct{
	KEY_DISPOSE_FUN	coded_right;
	KEY_DISPOSE_FUN	coded_left;
    
	KEY_DISPOSE_FUN	key_up;
	KEY_DISPOSE_FUN	key_down;
	KEY_DISPOSE_FUN	key_left;
	KEY_DISPOSE_FUN	key_right;
	
	KEY_DISPOSE_FUN	key_enter;
	KEY_DISPOSE_FUN	key_exit;
	KEY_DISPOSE_FUN	key_offset;
	KEY_DISPOSE_FUN	key_ulock;
	KEY_DISPOSE_FUN	key_shift;
	
	KEY_DISPOSE_FUN	key_f0;
	KEY_DISPOSE_FUN	key_f1;
	KEY_DISPOSE_FUN	key_f2;
	KEY_DISPOSE_FUN	key_f3;
	KEY_DISPOSE_FUN	key_f4;
	KEY_DISPOSE_FUN	key_f5;
	KEY_DISPOSE_FUN	key_f6;
    
	KEY_DISPOSE_FUN	key_clock_0;
	KEY_DISPOSE_FUN	key_clock_1;
	KEY_DISPOSE_FUN	key_f1_0;
	KEY_DISPOSE_FUN	key_f1_1;
	KEY_DISPOSE_FUN	key_f1_4;
	
}KEY_FUNCATION;


#ifdef   KEY_DISPOSE_GLOBALS
#define  KEY_DISPOSE_EXT
#else
#define  KEY_DISPOSE_EXT  extern
#endif

/**
  * @brief  全局的功能键处理函数指针
  */
KEY_DISPOSE_EXT		void (*global_fun_key_dispose)(uint32_t key_value);
KEY_DISPOSE_EXT		void (*scan_key_custom_fun)();
KEY_DISPOSE_EXT		void (*scan_direct_key_fun)();
KEY_DISPOSE_EXT		 KEY_FUNCATION		key_funcation;
KEY_DISPOSE_EXT		 KEY_DISPOSE_FUN 	golbal_key_info;
extern void dispose_funcation_key(uint32_t key);
extern void init_funcation_key_dispose_fun(KEY_FUNCATION *info);
extern void register_key_dispose_fun(uint32_t key, KEY_DISPOSE_FUN *fun);
extern void enable_function_key(uint32_t key, uint8_t st);
extern uint8_t get_function_key_st(uint32_t key, CS_ERR *err);
extern void disable_system_fun_key_fun(void);

#endif //__KEY_FUN_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
