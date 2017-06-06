/**
  ******************************************************************************
  * @file    keyboard.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   7寸屏的布局1
  ******************************************************************************
  */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "stm32f4xx.h"


#define KEY_START	    0XFFFFFF1F
#define KEY_STOP	    0XFFFFFF2F

#define KEY_0	        0XFFFDFFFF
#define KEY_1	        0XFFFFDFFF
#define KEY_2	        0XBFFFFFFF
#define KEY_3	        0XFFFFBFFF
#define KEY_4	        0XFFFFFDFF
#define KEY_5	        0XFBFFFFFF
#define KEY_6	        0XFFFFFBFF
#define KEY_7	        0XFFFFFFDF
#define KEY_8	        0XFFBFFFFF
#define KEY_9	        0XFFFFFFBF
#define KEY_POINT		0XFFFFFFFD
#define KEY_SHIFT		0XFFFBFFFF

#define KEY_EXIT	    0XFFDFFFFF
#define KEY_ENTER	    0XFDFFFFFF
#define KEY_OFFSET      0XDFFFFFFF
#define KEY_UNLOCK      0XFFFFFFFE

#define KEY_UP  	    0X7FFFFFFF
#define KEY_DOWN	    0XF7FFFFFF
#define KEY_LEFT        0XFFFFFFFB
#define KEY_RIGHT       0XFF7FFFFF

#define KEY_DIS_FORMAT  0XEFFFFFFF
#define KEY_F0			KEY_DIS_FORMAT
#define KEY_F1		    0XFEFFFFFF
#define KEY_F2		    0XFFEFFFFF
#define KEY_F3		    0XFFFEFFFF
#define KEY_F4		    0XFFFFEFFF
#define KEY_F5		    0XFFFFFEFF
#define KEY_F6		    0XFFFFFFEF


#define KEY_NONE	    0XFFFFFFFF

#define _KEY_DOWN		0XEFFFFFFE
#define _KEY_UP			0XEFFFFFFF
#define _KEY_LONG		0XEFFFFFF6


#define CODE_RIGH     1 //码盘右
#define CODE_LEFT     2 //码盘左

#define KEY_DELAY_NORMAL	(100)
#define KEY_DELAY_CAL		(100)

#ifndef NULL
#define NULL 0
#endif

#ifdef   KEY_GLOBALS
#define  KEY_EXT
#else
#define  KEY_EXT  extern
#endif

KEY_EXT int16_t key_buzzer_time;/* 按键蜂鸣长度 */

#define BUTTON_FILTER_TIME 	(1)		/* 消抖时间 */
#define BUTTON_LONG_TIME 	(100)		/* 持续1秒，认为长按事件 */
enum{
    SINGLE_KEY,
    COMBINATION_KEY,
};
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t Count;			/* 滤波器计数器 */
	uint8_t FilterTime;		/* 滤波时间(最大255,表示2550ms) */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint16_t State;			/* 按键当前状态（按下还是弹起） */
	uint32_t KeyCodeUp;		/* 按键弹起的键值代码, 0表示不检测按键弹起 */
	uint32_t KeyCodeDown;	/* 按键按下的键值代码, 0表示不检测按键按下 */
	uint32_t KeyCodeLong;	/* 按键长按的键值代码, 0表示不检测长按 */
    uint8_t type;           /* 按类型 单键 组合键 */
}KEY_STRUCT;

extern void init_keyboard(void);
extern uint32_t scan_keyboard(void);
extern void report_key_value(void);
extern uint32_t get_key_value(void);

#endif //__KEYBOARD_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
