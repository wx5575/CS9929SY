/**
  ******************************************************************************
  * @file    key_server.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.14
  * @brief   提供按键服务，字符输入，数字输入、密码输入、按键查询任务
  ******************************************************************************
  */
#ifndef __KEY_SERVER_H__
#define __KEY_SERVER_H__


#include "stm32f4xx.h"

extern void scan_key_task(void);

extern void set_shift_status(uint8_t st);
extern uint8_t get_shift_status(void);
extern void set_key_lock_flag(uint8_t flag);
extern uint8_t get_key_lock_flag(void);
extern void set_scan_key_custom_fun(void (*fun)());
extern void keyboard_fun_str(uint32_t key_value);
extern void keyboard_fun_num(uint32_t key_value);
extern void keyboard_fun_pwd(uint32_t key_value);
extern void keyboard_test_port(uint32_t key);

#endif //__KEY_SERVER_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
