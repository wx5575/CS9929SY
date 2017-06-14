/**
  ******************************************************************************
  * @file    key_server.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.14
  * @brief   提供按键服务，字符输入，数字输入、密码输入、按键查询任务
  ******************************************************************************
  */
#include "string.h"
#include "os.h"
#include "GUI.H"
#include "EDIT.h"
#include "tools.h"
#include "ui_com/com_edit_api.h"
#include "keyboard.h"
#include "key_fun_manage.h"

/** 
  * @brief 用户界面使用的全局标记
  */
typedef struct{
    uint8_t shift_flag;///< SHIFT键按下标记
    uint8_t key_lock_flag;///< 键盘锁按下标记
}UI_FLAG;
static volatile UI_FLAG ui_flag;///<界面使用全局标记

void set_shift_status(uint8_t st)
{
    ui_flag.shift_flag = st;
}
uint8_t get_shift_status(void)
{
    return ui_flag.shift_flag;
}
void set_key_lock_flag(uint8_t flag)
{
    ui_flag.key_lock_flag = flag;
}
uint8_t get_key_lock_flag(void)
{
    return ui_flag.key_lock_flag;
}
/**
  * @brief  获取键值对应按键的索引值
  * @param  [in] key 键值
  * @param  [in] flag 1表示对小数点进行索引，0表示不对小数点进行索引
  * @retval 按键的索引值
  */
uint8_t get_key_value_index(uint32_t key, uint8_t flag)
{
	uint8_t key_index = 0xff;
    
	switch(key)
	{
		case KEY_0:
			key_index = 0;
			break;
		case KEY_1:
			key_index = 1;
			break;
		case KEY_2:
			key_index = 2;
			break;
		case KEY_3:
			key_index = 3;
			break;
		case KEY_4:
			key_index = 4;
			break;
		case KEY_5:
			key_index = 5;
			break;
		case KEY_6:
			key_index = 6;
			break;
		case KEY_7:
			key_index = 7;
			break;
		case KEY_8:
			key_index = 8;
			break;
		case KEY_9:
			key_index = 9;
			break;
		case KEY_POINT:
            if(flag)
            {
                key_index = 10;
            }
			break;
		default:
			break;
	}
    
    return key_index;
}
/**
  * @brief  处理字符输入时使用的按键服务函数
  * @param  [in] key 键值
  * @retval 无
  */
void keyboard_str(uint32_t key)
{
#define KEY_ALL_VALUE "0+/-1ABC2DEF3GHI4JKL5MNO6PQR7STU8VWX9YZ ."
#define KEY_ALL_VALUE_ "0+/-1abc2def3ghi4jkl5mno6pqr7stu8vwx9yz ."
	uint8_t bufx[]={
	4,4,4,4,4,//0 1 2 3 4
	4,4,4,4,4,//5 6 7 8 9
	1,		  //.
	};
	uint8_t num_buf[]={
		0 ,  4,  8, 12, 16,//0 1 2 3 4
		20, 24, 28, 32, 36,//5 6 7 8 9
		40, 			   //.
	};
	static uint8_t key_timeout = 0;
	static uint8_t c = 0;
	static uint32_t key_back = KEY_NONE;
	static uint8_t index_back = 0xff;
	static uint8_t cour = 0;
	static uint8_t input_st = 0;
	static uint8_t temp_cur_num = 0;
    WM_HMEM handle = 0;
    uint8_t key_char;
    uint8_t key_index = 0;
    uint8_t temp;
	
    
    handle = get_cur_edit_handle();
	
    if(handle == 0)
    {
        return;
    }
	
	if(key != KEY_NONE)
	{
		key_timeout = 0;
		
		if(key == key_back)
		{
			c = (c + 1) % temp_cur_num;//buf[key_index];//4;
		}
		else
		{
			key_back = key;
			c = 0;
		}
	}
	
	if(input_st == 1)
	{
		if(++key_timeout > 80)
		{
			key_back = KEY_NONE;
            index_back = 0xff;
			c = 0;
			key_timeout = 0;
			input_st = 0;
            EDIT_SetCursorAtChar(handle, cour + 1);
		}
	}
    
	key_index = get_key_value_index(key, 1);
	
	if(key_index == 0xff)
	{
        return;
    }
    
    cour = EDIT_GetCursorCharPos(handle);//获取光标位置
    
    /* 第一次按下某个键 0-9 . */
    if(index_back == 0xFF)
    {
        index_back = key_index;
    }
    /* 连续按下不同的按键 */
    else if(key_index != index_back)
    {
        index_back = key_index;
        EDIT_SetCursorAtChar(handle, cour + 1);//先将光标后移一位
    }
    /* 连续按下同一个按键 */
    else
    {
        GUI_SendKeyMsg(GUI_KEY_DELETE, 1);//先删除这个光标位置的字符
    }
    
    cour = EDIT_GetCursorCharPos(handle);//获取光标位置
    
    temp_cur_num = bufx[key_index];
    temp = num_buf[key_index] + c;
    
    key_char = (ui_flag.shift_flag? KEY_ALL_VALUE : KEY_ALL_VALUE_)[temp];
    
    GUI_SendKeyMsg(key_char, 1);//在光标位置插入一个字符
//        GUI_StoreKeyMsg(key_char, 1);//在光标位置插入一个字符
    key_timeout = 0;
    input_st = 1;
    EDIT_SetCursorAtChar(handle, cour);//保持光标位置不动
}
/**
  * @brief  处理密码输入时使用的按键服务函数
  * @param  [in] key 键值
  * @retval 无
  */
void keyboard_password(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	uint8_t key_index = 0xff;
	static uint8_t cour = 0;
    WM_HMEM handle = 0;
    uint8_t buf[20] = {0};
    uint8_t max_len;
    uint8_t len;
	
    
    handle = get_cur_edit_handle();
	
    if(handle == 0)
    {
        return;
    }
    
	key_index = get_key_value_index(key, 0);//排除小数点因此传入0
    
	if(key_index == 0xff)
	{
        return;
    }
    
    max_len = pwd_max_len();
    EDIT_GetText(handle, (char*)buf, 10);
    len = strlen((const char*)buf);
    
    if(len < max_len)
    {
        cour = EDIT_GetCursorCharPos(handle);
        add_a_char_into_edit_str(ASCII_NUM[key_index], cour, max_len);
        GUI_StoreKeyMsg('*', 1);//会移动光标
    }
}
/**
  * @brief  处理数字和小数点输入时使用的按键服务函数
  * @param  [in] key 键值
  * @retval 无
  */
void keyboard_num(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	uint8_t key_index = 0xff;
    
	key_index = get_key_value_index(key, 1);//包含小数点因此传入1
    
    if(key_index == 0xff)
    {
        return;
    }
    
	GUI_StoreKeyMsg(ASCII_NUM[key_index], 1);
}
/**
  * @brief  设置按键服务函数
  * @param  [in] fun 按键服务函数
  * @retval 无
  */
void set_scan_key_custom_fun(void (*fun)())
{
	scan_key_custom_fun = fun;
}

/**
  * @brief  提供字符输入的按键服务函数
  * @param  [in] key_value 键值
  * @retval 无
  */
void keyboard_fun_str(uint32_t key_value)
{
	keyboard_str(key_value);
}

/**
  * @brief  提供数字小数点输入的按键服务函数
  * @param  [in] key_value 键值
  * @retval 无
  */
void keyboard_fun_num(uint32_t key_value)
{
	keyboard_num(key_value);
}

/**
  * @brief  提供密码输入的按键服务函数
  * @param  [in] key_value 键值
  * @retval 无
  */
void keyboard_fun_pwd(uint32_t key_value)
{
    keyboard_password(key_value);
}
/**
  * @brief  按键扫描任务
  * @param  无
  * @retval 无
  */
void scan_key_task(void)
{
	uint32_t key_value = KEY_NONE;
	OS_ERR	err;
	
	init_keyboard();
	set_scan_key_custom_fun(NULL);
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 10, 0, &err);
		report_key_value();
		key_value =  get_key_value();
		
		//系统功能按键处理
		dispose_funcation_key(key_value);
		
		/* 根据不同界面定制按键扫描函数 */
		if(NULL != scan_key_custom_fun)
		{
			scan_key_custom_fun(key_value);
		}
		
		/* 菜单按键扫描服务函数 */
		if(NULL != global_fun_key_dispose)
		{
			global_fun_key_dispose(key_value);
		}
	}					 
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
