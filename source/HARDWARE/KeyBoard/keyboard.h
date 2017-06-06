/**
  ******************************************************************************
  * @file    keyboard.h
  * @author  ����
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   7�����Ĳ���1
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


#define CODE_RIGH     1 //������
#define CODE_LEFT     2 //������

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

KEY_EXT int16_t key_buzzer_time;/* ������������ */

#define BUTTON_FILTER_TIME 	(1)		/* ����ʱ�� */
#define BUTTON_LONG_TIME 	(100)		/* ����1�룬��Ϊ�����¼� */
enum{
    SINGLE_KEY,
    COMBINATION_KEY,
};
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			/* �˲��������� */
	uint8_t FilterTime;		/* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint16_t State;			/* ������ǰ״̬�����»��ǵ��� */
	uint32_t KeyCodeUp;		/* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */
	uint32_t KeyCodeDown;	/* �������µļ�ֵ����, 0��ʾ����ⰴ������ */
	uint32_t KeyCodeLong;	/* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */
    uint8_t type;           /* ������ ���� ��ϼ� */
}KEY_STRUCT;

extern void init_keyboard(void);
extern uint32_t scan_keyboard(void);
extern void report_key_value(void);
extern uint32_t get_key_value(void);

#endif //__KEYBOARD_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
