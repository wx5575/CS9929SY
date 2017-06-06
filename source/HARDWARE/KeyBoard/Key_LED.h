#ifndef __KEY_LED__H__
#define __KEY_LED__H__


/*包含的头文件*/
#include <stdint.h>
#include "stm32f4xx.h"

typedef struct{
	uint32_t buzzer_time;/* 蜂鸣器定时 */
}DEVICE_CTRL_INFO;

#define   ON     (1)
#define   OFF    (0)

#define BUZZER_ON	1
#define BUZZER_OFF	0

#define BUZZER_ON_T(T)	{set_buzzer_on_time(T);} //蜂鸣器开启明时间


void Key_LED_Control_Init(void);
extern void Set_LED_PASSLED(uint8_t state);
extern void Set_LED_FAILLED(uint8_t state);
extern void Set_LED_TESTLED(uint8_t state);
extern void set_buzzer(uint8_t state);
extern uint32_t KeyValue_Read(void);
extern void set_buzzer_on_time(uint32_t time);

uint32_t get_buzzer_time(void);
uint32_t sub_buzzer_time(void);

#endif



