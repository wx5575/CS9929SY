/**
  ******************************************************************************
  * @file    tools.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   软件工具函数
  ******************************************************************************
  */
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "stm32f4xx.h"

#define COUNT_ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])
#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

extern uint32_t ten_power(u8 n);
extern void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value);


#endif //__TOOLS_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
