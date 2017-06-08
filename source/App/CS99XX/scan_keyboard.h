/**
  ******************************************************************************
  * @file    scan_keyboard.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   按键扫描
  ******************************************************************************
  */
#ifndef __SCAN_KEYBOARD_H__
#define __SCAN_KEYBOARD_H__


#include "stm32f4xx.h"
#include "cs99xx_struct.h"

typedef enum{
	MENU_KEY_EN,//菜单键使能
	MENU_KEY_DIS,//菜单键失能
}MENU_KEY_ST_ENUM;

typedef struct
{
	void (*fun)(int);///<按键处理函数
	int user_data;///<用户数据
    int custom_data;///<用户定制数据
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


KEY_DISPOSE_EXT		 KEY_FUNCATION		key_funcation;
KEY_DISPOSE_EXT		 KEY_DISPOSE_FUN 	golbal_key_info;
extern void dispose_funcation_key(uint32_t key);
extern void init_funcation_key_dispose_fun(KEY_FUNCATION *info);
extern void register_key_dispose_fun(uint32_t key, KEY_DISPOSE_FUN *fun);
extern void enable_function_key(uint32_t key, uint8_t st);
extern uint8_t get_function_key_st(uint32_t key, CS_ERR *err);
extern void disable_system_fun_key_fun(void);

#endif //__SCAN_KEYBOARD_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
