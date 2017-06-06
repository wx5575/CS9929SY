
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/** 任务优先级 */
typedef enum{
	START_TASK_PRIO = 3, //启动任务优先级
	TOUCH_TASK_PRIO,//TOUCH任务
	SCAN_KEY_TASK_PRIO,//SCAN_KEY_TASK_PRIO任务
	EMWINDEMO_TASK_PRIO,//EMWINDEMO任务
	CH376_TASK_PRIO,//EMWINDEMO任务
}TASK_PRIO;
/** 任务堆栈大小 */
typedef enum{
	START_STK_SIZE = 1024,///<启动任务优先级
	SCAN_KEY_STK_SIZE = 2048,///<键盘任务
	CH376_STK_SIZE = 2048,///<CH376任务
	EMWINDEMO_STK_SIZE = 1024*5,///<EMWINDEMO任务
}TASK_STK_SIZE;




#endif //__APP_CFG_H__
