/**
  ******************************************************************************
  * @file    file_edit_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   文件编辑窗口
  ******************************************************************************
  */
#ifndef __FILE_UI_SAVE_H__
#define __FILE_UI_SAVE_H__

typedef enum{
    FSAVE_UI_FNAME,///<文件名
    FSAVE_UI_WMODE,///<工作模式
    FSAVE_UI_BEEPT,///<蜂鸣时间
    FSAVE_UI_PASST,///<PASS时间
}FSAVE_UI_ENUM;


extern void create_save_file_dialog(int hWin);
extern void create_new_file_dialog(int hWin);
extern void create_edit_file_dialog(int hWin);

#endif //__FILE_UI_SAVE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
