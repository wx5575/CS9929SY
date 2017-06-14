/**
  ******************************************************************************
  * @file    sys_manage_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.11
  * @brief   系统管理界面
  ******************************************************************************
  */
#ifndef __PASSWORD_WIN_H__
#define __PASSWORD_WIN_H__

typedef enum{
    PASSWORD_OLD,///<原始密码
    PASSWORD_NEW,///<新密码
    PASSWORD_CONFIRM,///<确认密码
}PWD_UI_ENUM;

/**
  * @brief  密码界面的文本索引枚举
  */
typedef enum{
    PWD_UI_OLD_NOTICE,///<原始密码的判断结果文本 "密码不正确"
    PWD_UI_NEW_NOTICE,///<新密码的判断结果文本 "密码不能为空"
    PWD_UI_CONFIRM_NOTICE,///<确认密码的判断结果文本 "密码不一"
    PWD_UI_CHANGE_RESULT,///<密码修改的结果 "密码修改成功"
}PWD_TEXT_INDEX;

void create_password_ui(int hWin);

#endif //__PASSWORD_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
