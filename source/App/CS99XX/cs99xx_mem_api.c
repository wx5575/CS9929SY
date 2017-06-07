/**
  ******************************************************************************
  * @file    file_ui_com.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   存储器应用接口
  ******************************************************************************
  */
#define _99xxMEM_GLOBALS

#include	<string.h>
#include    <stdio.h>
#include    "cs99xx_mem_api.h"

/**
  * @brief  保存文件
  * @param  [in] file_num 文件编号
  * @retval 正常 CS_ERR_NONE
  * @retval CS_ERR_DATA_OUT_OF_RANGE 文件编号超出范围
  */
CS_ERR save_file(const FILE_NUM file_num)
{
    if(file_num > MAX_FILES)
    {
        return CS_ERR_DATA_OUT_OF_RANGE;
    }
    
    save_file_flash(file_num);
    
    return CS_ERR_NONE;
}
/**
  * @brief  保存所有文件
  * @param  无
  * @retval 无
  */
void save_all_file(void)
{
    save_all_file_flash();
}
/**
  * @brief  读取文件
  * @param  [in] file_num 文件编号
  * @retval 正常 CS_ERR_NONE
  * @retval CS_ERR_DATA_OUT_OF_RANGE 文件编号超出范围
  */
CS_ERR  read_file(const FILE_NUM file_num)
{
    if(file_num > MAX_FILES)
    {
        return CS_ERR_DATA_OUT_OF_RANGE;
    }
    
    read_file_flash(file_num);
    
    return CS_ERR_NONE;
}
/**
  * @brief  读取所有文件
  * @param  无
  * @retval 无
  */
void read_all_file(void)
{
    read_all_file_flash();
}
/**
  * @brief  保存系统标记
  * @param  无
  * @retval 正常 CS_ERR_NONE
  */
CS_ERR save_sys_flag(void)
{
    save_sys_flag_flash();
    
    return CS_ERR_NONE;
}
/**
  * @brief  读取系统标记
  * @param  无
  * @retval 无
  */
void read_sys_flag(void)
{
    read_sys_flag_flash();
}
/**
  * @brief  读取系统参数
  * @param  无
  * @retval 无
  */
void read_sys_par(void)
{
    read_sys_par_flash();
}
/**
  * @brief  保存系统参数
  * @param  无
  * @retval 无
  */
CS_ERR save_sys_par(void)
{
    save_sys_par_flash();
    
    return CS_ERR_NONE;
}
/**
  * @brief  保存步骤已使用的标记
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_step_used_flag(const FILE_NUM file_num)
{
    save_step_used_flag_flash(file_num);
}
/**
  * @brief  读取步骤已使用的标记
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void read_step_used_flag(const FILE_NUM file_num)
{
    read_step_used_flag_flash(file_num);
}
/**
  * @brief  读取记忆组中步骤存储映射表
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void read_group_table(const FILE_NUM file_num)
{
    read_group_table_flash(file_num);
}
/**
  * @brief  保存记忆组中步骤存储映射表
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_group_table(const FILE_NUM file_num)
{
    save_group_table_flash(file_num);
}
/**
  * @brief  保存一步的参数
  * @param  [in] node 步骤数据
  * @param  [in] file_num 文件编号
  * @param  [in] step 步骤编号
  * @retval 无
  */
void save_one_step(NODE_STEP *node, const FILE_NUM file_num, const STEP_NUM step)
{
    save_one_step_flash(node, file_num, step);
}
/**
  * @brief  读一步的参数
  * @param  [in] node 步骤数据
  * @param  [in] file_num 文件编号
  * @param  [in] step 步骤编号
  * @retval 无
  */
void read_one_step(NODE_STEP *node, const FILE_NUM file_num, const STEP_NUM step)
{
    read_one_step_flash(node, file_num, step);
}
/**
  * @brief  保存记忆组信息
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_group_info(const FILE_NUM file_num)
{
    save_file(file_num);//保存文件
    save_group_table(file_num);//保存记忆组映射表
    save_step_used_flag(file_num);//保存步骤已使用标记
}
/**
  * @brief  读记忆组信息
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void read_group_info(const FILE_NUM file_num)
{
    read_file(file_num);//保存文件
    read_group_table(file_num);//保存记忆组映射表
    read_step_used_flag(file_num);//保存步骤已使用标记
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
