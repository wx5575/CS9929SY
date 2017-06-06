/**
  ******************************************************************************
  * @file    cs99xx_mem_api.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   存储器应用接口
  ******************************************************************************
  */
#ifndef __CS99XX_MEM_API_H__
#define __CS99XX_MEM_API_H__

#include    "stm32f4xx.h"
#include    "cs99xx_flash_manage.h"
#include    "cs99xx_struct.h"

/******************* 全局变量声明 *********************/
#ifdef   _99xxMEM_GLOBALS
#define  _99xxMEM_EXT
#else
#define  _99xxMEM_EXT  extern
#endif

/***********************************************************/

extern CS_ERR save_file(const FILE_NUM file_num);
extern void save_all_file(void);
extern CS_ERR  read_file(const FILE_NUM file_num);
extern void read_all_file(void);

extern CS_ERR save_sys_flag(void);
extern void read_sys_flag(void);
extern void read_sys_par(void);
extern CS_ERR save_sys_par(void);

extern void save_step_used_flag(const FILE_NUM file_num);
extern void read_step_used_flag(const FILE_NUM file_num);
extern void read_group_table(const FILE_NUM file_num);
extern void save_group_table(const FILE_NUM file_num);
extern void save_one_step(NODE_STEP *node, const STEP_NUM step);
extern void save_group_info(const FILE_NUM file_num);

#endif //__CS99XX_MEM_API_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
