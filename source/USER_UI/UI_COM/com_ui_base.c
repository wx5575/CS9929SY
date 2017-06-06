/**
  ******************************************************************************
  * @file    scan_keyboard.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   按键扫描
  ******************************************************************************
  */
#include "com_ui_base.h"



/**
  * @brief  单位信息池
  */
const uint8_t *unit_pool[]=
{
	"","s","mV","V","kV","uA","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","uF",
};
/**
  * @brief  工作模式
  */
const uint8_t *work_mode_pool[2]={"N","G"};
/**
  * @brief  语言
  */
const uint8_t *language_pool[2]={"中文","English"};
/**
  * @brief  测试模式
  */
const uint8_t *mode_pool[10]={"", "ACW", "DCW", "IR ", "GR ", "BBD", "CC "};


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
