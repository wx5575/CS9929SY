/**
  ******************************************************************************
  * @file    cs99xx_flash_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   flash应用层代码
  ******************************************************************************
  */
#include "cs99xx_flash_manage.h"
#include "spi_flash.h"
#include "app.h"
#include "os.h"
#include "string.h"

/**
  * @brief  串行FLASH缓冲写函数
  * @param  [in] _pBuf 数据缓冲区
  * @param  [in] _uiWriteAddr 要写入的地址
  * @param  [in] _usWriteSize 要写入的字节数
  * @param  [in] chip FLASH 编号
  * @retval OS_ERR OS_ERR_NONE
  */
static void writebuffer_spi_flash(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize, uint8_t chip)
{
    OS_ERR err = OS_ERR_NONE;
    
    flash_cs_set_at45db161(chip);
    
    OSMutexPend(&MemMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    
    at45_buffer_write(_pBuf, _uiWriteAddr, _usWriteSize);
    
    OSMutexPost (&MemMutex, OS_OPT_POST_NONE, &err);
}

/**
  * @brief  串行FLASH缓冲读函数
  * @param  [in] _pBuf 数据缓冲区
  * @param  [in] _uiWriteAddr 要读的地址
  * @param  [in] _usWriteSize 要读的字节数
  * @param  [in] chip FLASH 编号
  * @retval OS_ERR OS_ERR_NONE
  */
static void readbuffer_spi_flash(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize, uint8_t chip)
{
    OS_ERR err = OS_ERR_NONE;
    
    flash_cs_set_at45db161(chip);
    
    OSMutexPend(&MemMutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    
    at45_buffer_read(_pBuf, _uiReadAddr, _uiSize);
    
    OSMutexPost (&MemMutex, OS_OPT_POST_NONE, &err);
}
/**
  * @brief  保存文件到FLASH
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_file_flash(const FILE_NUM file_num)
{
    writebuffer_spi_flash((uint8_t*)&file_pool[file_num],CONT_F_FILE_ADDR(file_num),
            F_FILE_OFFSET, SPI_FLASH_CHIP1);
}
/**
  * @brief  保存所有文件到FLASH
  * @param  无
  * @retval 无
  */
void save_all_file_flash(void)
{
    int32_t i = 0;
    
    for(i = 0; i < MAX_FILES; i++)
    {
        save_file_flash(i);
    }
}
/**
  * @brief  读文件从FLASH
  * @param  无
  * @retval 无
  */
void read_file_flash(const FILE_NUM file_num)
{
    if(file_num > MAX_FILES - 1)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)&file_pool[file_num],
            F_FILE_BASE + F_FILE_OFFSET * file_num,
            F_FILE_OFFSET, SPI_FLASH_CHIP1);
}
/**
  * @brief  读所有文件从FLASH
  * @param  无
  * @retval 无
  */
void read_all_file_flash(void)
{
    int32_t i = 0;
    
    for(i = 0; i < MAX_FILES; i++)
    {
        read_file_flash(i);
    }
}
/**
  * @brief  保存系统标记到FLASH
  * @param  无
  * @retval 无
  */
void save_sys_flag_flash(void)
{
    writebuffer_spi_flash((uint8_t*)&sys_flag,
            F_SYS_FLAG_BASE, F_SYS_FLAG_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  读系统标记从FLASH
  * @param  无
  * @retval 无
  */
void read_sys_flag_flash(void)
{
    readbuffer_spi_flash((uint8_t*)&sys_flag, F_SYS_FLAG_BASE, F_SYS_FLAG_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  保存系统参数到FLASH
  * @param  无
  * @retval 无
  */
void save_sys_par_flash(void)
{
	writebuffer_spi_flash((uint8_t*)&sys_par, F_SYS_PAR_BASE, F_SYS_PAR_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  读系统参数从FLASH
  * @param  无
  * @retval 无
  */
void read_sys_par_flash(void)
{
	readbuffer_spi_flash((uint8_t*)&sys_par, F_SYS_PAR_BASE, F_SYS_PAR_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  初始化记忆组步骤使用标记到FLASH
  * @param  无
  * @retval 无
  */
void init_group_step_used_flag_flash(void)
{
    int32_t i = 0;
    
    memset(step_used_flag, 0, F_FILE_USED_STEP_FLAG_OFFSET);
    
    for(i = 0; i < MAX_FILES; i++)
    {
        writebuffer_spi_flash((uint8_t*)step_used_flag,
                  F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * i,
                  F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
    }
}

/**
  * @brief  保存步骤使用标记到FLASH
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_step_used_flag_flash(FILE_NUM file_num)
{
    writebuffer_spi_flash((uint8_t*)step_used_flag,
                F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * file_num,
                F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
}
/**
  * @brief  读步骤使用标记从FLASH
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void read_step_used_flag_flash(const FILE_NUM file_num)
{
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)step_used_flag,
                F_FILE_USED_STEP_FLAG_BASE + F_FILE_USED_STEP_FLAG_OFFSET * file_num,
                F_FILE_USED_STEP_FLAG_OFFSET, SPI_FLASH_CHIP1);
}
/**
  * @brief  保存记忆组步骤映射表到FLASH
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void save_group_table_flash(const FILE_NUM file_num)
{
    if(file_num > MAX_FILES - 1)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)cur_group_table,
                F_FILE_STEP_TABLE_BASE + F_FILE_STEP_TABLE_OFFSET * file_num,
                F_FILE_STEP_TABLE_OFFSET, SPI_FLASH_CHIP1);
}
/**
  * @brief  读记忆组步骤映射表从FLASH
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void read_group_table_flash(const FILE_NUM file_num)
{
    readbuffer_spi_flash((uint8_t*)cur_group_table,
                  F_FILE_STEP_TABLE_BASE
                + F_GROP_TABLE_SIZE * file_num,
                  F_GROP_TABLE_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  保存一步测试参数到FLASH
  * @param  [in] node 步骤参数
  * @param  [in] file_num 文件编号
  * @param  [in] step 步骤编号
  * @retval 无
  */
void save_one_step_flash(NODE_STEP *node, const FILE_NUM file_num, const STEP_NUM step)
{
    uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    offset_addr = cur_group_table[step - 1];
    
    /* 判断映射地址是无效 */
    if(offset_addr == TABLE_VALUE_NULL)
    {
        return;
    }
    
    writebuffer_spi_flash((uint8_t*)&node->one_step,
                  F_GROUP_BASE + F_GROUP_OFFSET * file_num + F_STEP_OFFSET * offset_addr,
                  F_UN_SIZE, SPI_FLASH_CHIP1);
}
/**
  * @brief  保存一步测试参数到FLASH
  * @param  [in] node 步骤参数
  * @param  [in] file_num 文件编号
  * @param  [in] step 步骤编号
  * @retval 无
  */
void read_one_step_flash(NODE_STEP *node, const FILE_NUM file_num, const STEP_NUM step)
{
    uint16_t offset_addr = TABLE_VALUE_NULL;
    
    if(file_num > MAX_FILES)
    {
        return;
    }
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    offset_addr = cur_group_table[step - 1];
    
    /* 判断映射地址是无效 */
    if(offset_addr == TABLE_VALUE_NULL)
    {
        return;
    }
    
    readbuffer_spi_flash((uint8_t*)&node->one_step,
                  F_GROUP_BASE + F_GROUP_OFFSET * file_num + F_STEP_OFFSET * offset_addr,
                  F_UN_SIZE, SPI_FLASH_CHIP1);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
