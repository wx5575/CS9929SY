/**
  ******************************************************************************
  * @file    app.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   用户程序
  ******************************************************************************
  */
#ifndef __APP_H__
#define __APP_H__

#include "app_cfg.h"
#include "os.h"
#include "ff.h"
#include "stm32f4xx.h"

#define     OS_MAX_QS       10


#ifndef NULL
#define NULL 0
#endif

typedef struct {
	uint8_t stop_scan_key;///< 停止扫描键盘
    uint8_t calibration;///<校准标记
}APP_FLAG;

#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif

APP_EXT		APP_FLAG	app_flag;

APP_EXT     OS_Q     KeyboardQSem;	/* 消息队列 */
APP_EXT     void     *QMsgTbl[OS_MAX_QS];/* 消息队列 */

APP_EXT 	OS_MUTEX    MemMutex; /* 访问memory的互斥互斥访问 */

APP_EXT     FRESULT fs_result;
APP_EXT     FATFS fs_struct;

//任务控制块
APP_EXT	OS_TCB StartTaskTCB;
APP_EXT	OS_TCB ScanKeyTaskTCB;
APP_EXT	OS_TCB EmwindemoTaskTCB;
APP_EXT	OS_TCB TouchTaskTCB;
APP_EXT	OS_TCB Ch376TaskTCB;

APP_EXT	OS_TMR 	timer_for_app;//服务应用程序的定时器

//任务堆栈
APP_EXT	CPU_STK		START_TASK_STK[START_STK_SIZE];
APP_EXT	CPU_STK		SCAN_KEY_TASK_STK[SCAN_KEY_STK_SIZE];
APP_EXT	CPU_STK		EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
APP_EXT	CPU_STK		CH376_TASK_STK[CH376_STK_SIZE];

//任务函数
extern void start_task(void *p_arg);
extern void emwindemo_task(void *p_arg);
extern void led0_task(void *p_arg);
extern void touch_task(void *p_arg);

extern void set_scan_key_custom_fun(void (*fun)());
extern void keyboard_fun_str(uint32_t key_value);
extern void keyboard_fun_num(uint32_t key_value);
extern void keyboard_fun_pwd(uint32_t key_value);

#endif // __APP_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
