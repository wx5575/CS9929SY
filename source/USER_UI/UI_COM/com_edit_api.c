/**
  ******************************************************************************
  * @file    com_edit_api.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   菜单键界面
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "com_edit_api.h"
#include "cs99xx_struct.h"
#include "cs_list.h"
#include "com_ui_info.h"
#include "ui/file_win/file_edit_win.h"
#include "GUI.H"
#include "EDIT.H"
#include "TEXT.H"
#include "DROPDOWN.H"
#include "string.h"
#include "tools.h"
#include "stdlib.h"
#include "stdio.h"
#include "keyboard.h"

#define SELETED_COLOR	GUI_LIGHTBLUE

static void create_win_edit_ele(EDIT_ELE_T *ele, WM_HWIN hParent);

/**
  * @brief  获取编辑对象在控件池中的索引值
  * @param  [in] edit_pool 控件池
  * @param  [in] pool_size 控件池的大小
  * @param  [in] index 控件索引值，是控件索引池中的元素
  * @param  [out] err 成功返回 CS_ERR_NONE, 失败返回 CS_ERR_ELE_INDEX_INVALID
  * @retval CS_INDEX 控件 在控件池中对应的数组下标索引值
  */
CS_INDEX get_edit_ele_index(EDIT_ELE_T *edit_pool, uint32_t pool_size, CS_INDEX index, CS_ERR*err)
{
    int32_t i = 0;
    
    for(i = 0; i < pool_size; i++)
    {
        /* 索引值 匹配到了控件池中的索引值 返回控件池的对应的下标 */
        if(edit_pool[i].index == index)
        {
            *err = CS_ERR_NONE;
            return (CS_INDEX)i;
        }
    }
    
    *err = CS_ERR_INDEX_INVALID;
    
    return (CS_INDEX)0;
}

EDIT_ELE_T* get_edit_ele_inf(EDIT_ELE_T *edit_pool, uint32_t pool_size, CS_INDEX index, CS_ERR*err)
{
    EDIT_ELE_T *ele = NULL;
    CS_INDEX tmp_index;
    
    tmp_index = get_edit_ele_index(edit_pool, pool_size, index, err);
    
    if(*err == CS_ERR_NONE)
    {
        ele = &edit_pool[tmp_index];
    }
    
    return ele;
}

/**
  * @brief  初始化窗口中文本对象的链表
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
void init_window_edit_ele_list(MYUSER_WINDOW_T *win)
{
    EDIT_ELE_T  *edit_pool;
    uint32_t pool_size = 0;
    CS_INDEX *index_pool;
    uint32_t index_size = 0;
    CS_INDEX index;
    CS_ERR err = CS_ERR_NONE;
    CS_LIST *list_head;
    int32_t i = 0;
    
    list_head = &win->edit.list_head;
    
    edit_pool = win->edit.pool;
    pool_size = win->edit.pool_size;
    
    index_pool = win->edit.index_pool;
    index_size = win->edit.index_size;
    
    list_init(list_head);
    
    /* 匹配所有的编辑对象并将其添加到编号对象链表中 */
    for(i = 0; i < index_size; i++)
    {
        index = get_edit_ele_index(edit_pool, pool_size, index_pool[i], &err);
        
        if(err != CS_ERR_NONE)
        {
            continue;
        }
        
        list_add_tail(&edit_pool[index].e_list, list_head);
    }
}
/**
  * @brief  初始化窗口中的编辑对象,会建立编辑对象链表
  * @param  [in] win 窗口信息
  * @retval 无
  */
void init_window_edit_ele(MYUSER_WINDOW_T* win)
{
    EDIT_ELE_T *node;
	CS_LIST *t_index = NULL;
	CS_LIST *t_list = &win->edit.list_head;//链表
    
    /* 初始化并创建编辑对象链表中的所有对象 */
	list_for_each(t_index, t_list)
	{
		node = list_entry(t_index, EDIT_ELE_T, e_list);
        create_win_edit_ele(node, win->handle);
	}
}
/**
  * @brief  设置编辑对象的数据指针
  * @param  [in] ele 编辑对象
  * @param  [in] data 数据指针
  * @retval 无
  */
void set_edit_ele_data(EDIT_ELE_T *ele, void *data)
{
    ele->data.data = data;
}

uint32_t get_edit_ele_value(EDIT_ELE_T* ele, uint8_t *buf)
{
    WM_HMEM handle = ele->dis.edit.handle;
    uint32_t max_len = ele->dis.edit.max_len + 1;
    uint32_t value = 0;
    uint8_t decs = ele->format.decs;
    uint32_t index = 0;
    uint8_t t_buf[20] = {0};
    
    switch(ele->type.type)
    {
        case ELE_TEXT:
            break;
        case ELE_EDIT_NUM:
            EDIT_GetText(handle, (char*)t_buf, max_len);
            value = atof((const char*)t_buf) * ten_power(decs);
            if(buf == NULL)
            {
                break;
            }
            EDIT_GetText(handle, (char*)buf, max_len);
            break;
        case ELE_EDIT_STR:
            EDIT_GetText(handle, (char*)buf, max_len);
            
            break;
        case ELE_DROPDOWN:
            DROPDOWN_GetUserData(handle, &value, ele->data.bytes);
            if(buf == NULL)
            {
                break;
            }
			index = DROPDOWN_GetSel(handle);
            DROPDOWN_GetItemText(handle, index, (char*)buf, max_len);
            break;
    }
    
    return value;
}

void set_edit_ele_value(EDIT_ELE_T* ele, uint32_t value, uint8_t *buf)
{
    WM_HMEM handle = ele->dis.edit.handle;
    uint8_t decs = ele->format.decs;
    uint8_t lon = ele->format.lon;
    uint8_t t_buf[20] = {0};
    
    switch(ele->type.type)
    {
        case ELE_TEXT:
            break;
        case ELE_EDIT_NUM:
            mysprintf(t_buf, NULL, decs + lon * 10 + 1 * 100, value);
            EDIT_SetText(handle, (const void*)t_buf);
            break;
        case ELE_EDIT_STR:
            EDIT_SetText(handle, (const void*)buf);
            break;
        case ELE_DROPDOWN:
            DROPDOWN_SetUserData(handle, &value, ele->data.bytes);
            break;
    }
}
void update_unit_dis(EDIT_ELE_T* ele)
{
	const char * pText;//文本
	WM_HWIN    handle;//句柄
	int align;/* 对齐方式 */
	const GUI_FONT * font;// 字体
	GUI_COLOR	font_color;// 字体颜色
	GUI_COLOR	back_color;// 背景颜色
    uint8_t unit = 0;
	
	handle = ele->dis.unit.handle;
	align = ele->dis.unit.align;
	font = ele->dis.unit.font;
	back_color = ele->dis.unit.back_color;
	font_color = ele->dis.unit.font_color;
	
    unit = ele->format.unit;
    pText = (const char *)unit_pool[unit];
    
	TEXT_SetTextAlign(handle, align);
	TEXT_SetFont(handle, font);
	TEXT_SetBkColor(handle, back_color);
	TEXT_SetTextColor(handle, font_color);
	TEXT_SetText(handle, pText);
}
void set_edit_num_value(EDIT_ELE_T* ele, uint32_t value)
{
    WM_HMEM handle = ele->dis.edit.handle;
    uint8_t buf[20] = {0};
    uint8_t decs = ele->format.decs;
    uint8_t lon = ele->format.lon;
    
    mysprintf(buf, NULL, decs + lon * 10 + 1 * 100, value);
    EDIT_SetMaxLen(handle, lon);
    EDIT_SetText(handle, (const void*)buf);
    update_unit_dis(ele);
}

void default_check_value_validity(EDIT_ELE_T* ele, uint32_t *value)
{
    uint32_t val = *value;
    
    if(val > ele->range.high)
    {
        *value = ele->range.high;
    }
    else if(val < ele->range.low)
    {
        *value = ele->range.low;
    }
}
/**
  * @brief  从控件中上载参数到内存空间,在设置完成时会通过这个函数来得到设置值
  *         这个函数会对得到的数据进行合法性判断，并进行修正
  * @param  [in] ele 编辑对象
  * @retval 无
  */
void upload_par_to_ram(EDIT_ELE_T* ele)
{
	void *data = ele->data.data;
    uint32_t size = ele->data.bytes;
    WM_HMEM handle = ele->dis.edit.handle;
    uint32_t max_len = ele->dis.edit.max_len + 1;
    uint8_t buf[20] = {0};
    uint32_t value = 0;
    
    if(ele == NULL)
    {
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    
    switch(ele->type.type)
    {
        case ELE_TEXT:
        {
            break;
        }
        case ELE_EDIT_NUM:
        {
            value = get_edit_ele_value(ele, NULL);
            
            if(ele->range.check_value_validity != NULL)
            {
                ele->range.check_value_validity(ele, &value);
            }
            else
            {
                default_check_value_validity(ele, &value);
            }
			
            memcpy(data, (const void*)&value, size);
			
            set_edit_num_value(ele, value);
            break;
        }
        case ELE_EDIT_STR:
        {
			EDIT_GetText(handle, (char*)buf, max_len);
            
            if(strlen((const char*)buf) > 0)
            {
                memcpy(data, (const void*)buf, max_len);
                
                if(ele->range.check_value_validity != NULL)
                {
                    ele->range.check_value_validity(ele, (uint32_t*)buf);
                }
            }
            else
            {
                EDIT_SetText(handle, (const void*)ele->data.data);
            }
            break;
        }
        case ELE_DROPDOWN:
        {
            DROPDOWN_GetUserData(handle, &value, size);
            
            /* 对数据进行检查 */
            if(ele->range.check_value_validity != NULL)
            {
                ele->range.check_value_validity(ele, &value);
            }
            
            memcpy(data, (const void*)&value, size);
            break;
        }
    }
    
    return;
}

/**
  * @brief  从编辑对象的数据元素中获取数据并存入指定的位置void *value中,
  * @brief  这个函数会在编辑对象被创建的时候调用，来对控件进行初始化
  * @param  [in] ele 编辑对象
  * @param  [out] valut 输出编辑对象的数据
  * @retval 无
  */
void load_par_to_buf(EDIT_ELE_T* ele, void *value)
{
	void *data = ele->data.data;
    uint32_t size = ele->data.bytes;
    
    if(ele == NULL)
    {
        return;
    }
    
    if(data == NULL)
    {
        return;
    }
    
    if(value == NULL)
    {
        return;
    }
	
	memcpy(value, (const void*)data, size);
}
/**
  * @brief  获取数据在编辑对象资源表数组中的索引
  * @param  [in] ele 编辑对象的信息数组
  * @param  [in] bytes 资源表数组元素的size
  * @param  [in] data 要匹配的数据
  * @retval 索引值
  */
CS_INDEX get_data_in_resource_table_index(EDIT_ELE_T *ele, uint8_t bytes, int32_t *data)
{
    int32_t i = 0;
    uint8_t *p8 = NULL;
    uint16_t *p16 = NULL;
    uint32_t *p32 = NULL;
    
    if(bytes == 1)
    {
        p8 = ele->resource.user_data;
    }
    else if(bytes == 2)
    {
        p16 = ele->resource.user_data;
    }
    else if(bytes == 4)
    {
        p32 = ele->resource.user_data;
    }
    else
    {
        return 0;
    }
    
    if(p8 != NULL)
    {
        for(i = 0; i < ele->resource.user_data_size; i++)
        {
            if(*data == p8[i])
            {
                return i;
            }
        }
        
        *data = p8[i];
    }
    else if(p16 != NULL)
    {
        for(i = 0; i < ele->resource.user_data_size; i++)
        {
            if(*data == p16[i])
            {
                return i;
            }
        }
        
        *data = p16[i];
    }
    else
    {
        for(i = 0; i < ele->resource.user_data_size; i++)
        {
            if(*data == p32[i])
            {
                return i;
            }
        }
        
        *data = p32[i];
    }
    
    return 0;
}
/**
  * @brief  创建一个编辑控件对象，根据编辑对象提供的信息来创建编辑对象，编辑对象的类型决定
  * @brief  创建一个什么样的对象 ELE_TEXT 文本 ELE_EDIT_NUM 数字编辑框 ELE_EDIT_STR 字符串编辑框
  * @brief  ELE_DROPDOWN 下拉框
  * @param  [in] ele 编辑对象的信息
  * @param  [in] hParent 父窗口句柄
  * @retval 无
  */
void create_one_edit_ele(EDIT_ELE_T *ele, WM_HWIN hParent)
{
    int32_t i = 0;
	WM_HWIN hwidget = 0;
    uint8_t buf[20] = {0};
    int32_t data = 0;
	
	switch(ele->type.type)
	{
		case ELE_TEXT:/* 文本 */
			break;
		case ELE_EDIT_NUM:/* 数字编辑框 */
		{
			hwidget = EDIT_CreateEx(ele->dis.x + ele->dis.name.width, ele->dis.y,
						ele->dis.edit.width, ele->dis.edit.height,
						hParent, WM_CF_SHOW,
						ele->dis.edit.align,
						id_base++,
						ele->dis.edit.max_len);
			
			ele->dis.edit.handle = hwidget;
			EDIT_SetFont(hwidget, ele->dis.edit.font);
			EDIT_SetBkColor(hwidget, EDIT_CI_ENABLED, ele->dis.edit.back_color);
			EDIT_SetTextColor(hwidget, EDIT_CI_ENABLED, ele->dis.edit.font_color);
			
			if(ele->data.data != NULL)
			{
                load_par_to_buf(ele, &data);
                mysprintf(buf, NULL, ele->format.decs + ele->format.lon * 10 + 1 * 100, data);
			}
			
			EDIT_SetText(hwidget, (const void*)buf);
			break;
		}
		case ELE_EDIT_STR:/* 字符串编辑框 */
		{
			hwidget = EDIT_CreateEx(ele->dis.x + ele->dis.name.width, ele->dis.y,
						ele->dis.edit.width, ele->dis.edit.height,
						hParent, WM_CF_SHOW,
						ele->dis.edit.align,
						id_base++,
						ele->dis.edit.max_len);
			
			ele->dis.edit.handle = hwidget;
			EDIT_SetFont(hwidget, ele->dis.edit.font);
			EDIT_SetBkColor(hwidget, EDIT_CI_ENABLED, ele->dis.edit.back_color);
			EDIT_SetTextColor(hwidget, EDIT_CI_ENABLED, ele->dis.edit.font_color);
			
			EDIT_SetText(hwidget, (const char*)ele->value[0]);
			
			if(ele->data.data != NULL)
				EDIT_SetText(hwidget, (const char*)ele->data.data);
			break;
		}
		case ELE_DROPDOWN:/* 下拉选择框 */
		{
            uint32_t size = ele->data.bytes;
            
			hwidget = DROPDOWN_CreateUser(ele->dis.x + ele->dis.name.width, ele->dis.y,
									ele->dis.edit.width, ele->dis.edit.height,
									hParent, WM_CF_SHOW,
									ele->dis.edit.align,
									id_base++, size);//使用用户数据来传递设置的数据
            
			ele->dis.edit.handle = hwidget;
			
            if(ele->resource.table != NULL)
            {
                for(i = 0; i < ele->resource.size; i++)
                {
                    DROPDOWN_AddString(hwidget, ((const char**)ele->resource.table)[i]);
                }
            }
            
// 			DROPDOWN_AddString(hwidget, (const char*)ele->value[1]);
			DROPDOWN_SetFont(hwidget, ele->dis.edit.font);
			DROPDOWN_SetAutoScroll(hwidget, 1);
			DROPDOWN_SetTextAlign(hwidget, ele->dis.edit.align);
			
//			if(ele->data.data != NULL)
//            {
//                load_par_to_buf(ele, &data);
//				DROPDOWN_SetSel(hwidget, data);
//            }
            if(ele->resource.user_data != NULL)
            {
                CS_INDEX index;
                load_par_to_buf(ele, &data);
                index = get_data_in_resource_table_index(ele, size, &data);
                
                DROPDOWN_SetUserData(hwidget, &data, size);
				DROPDOWN_SetSel(hwidget, index);
            }
			break;
		}
	}
}
/**
  * @brief  创建一个名称对象,就是一个文本对象
  * @param  [in] ele 编辑对象的结构信息
  * @param  [in] hParent 父窗口句柄
  * @retval 无
  */
void create_one_name_ele(EDIT_ELE_T *ele, WM_HWIN hParent)
{
	WM_HWIN hwidget;
	
	hwidget = TEXT_CreateEx(ele->dis.x, ele->dis.y,
							ele->dis.name.width, ele->dis.name.height,
							hParent, WM_CF_SHOW,
							ele->dis.name.align,
							id_base++,
							(const char*)ele->name[SYS_LANGUAGE]);
	ele->dis.name.handle = hwidget;
	TEXT_SetFont(hwidget, ele->dis.name.font);
	TEXT_SetBkColor(hwidget, ele->dis.name.back_color);
	TEXT_SetTextColor(hwidget, ele->dis.name.font_color);
}
/**
  * @brief  创建单位对象，就是一个文本对象
  * @param  [in] ele 编辑对象的结构信息
  * @param  [in] hParent 父窗口句柄
  * @retval 无
  */
void create_one_unit_ele(EDIT_ELE_T *ele, WM_HWIN hParent)
{
	WM_HWIN hwidget;
	
    hwidget = TEXT_CreateEx(ele->dis.x + ele->dis.name.width + ele->dis.edit.width,
                            ele->dis.y,
                            ele->dis.unit.width, ele->dis.unit.height,
                            hParent, WM_CF_SHOW,
                            ele->dis.unit.align,
                            id_base++,
                            (const char*)unit_pool[ele->format.unit]);
    ele->dis.unit.handle = hwidget;
    TEXT_SetFont(hwidget, ele->dis.unit.font);
    TEXT_SetBkColor(hwidget, ele->dis.unit.back_color);
    TEXT_SetTextColor(hwidget, ele->dis.unit.font_color);
}
/**
  * @brief  创建编辑对象，包含了三部分:名称+编辑控件+单位
  * @param  [in] ele 编辑对象的结构信息
  * @param  [in] hParent 父窗口句柄
  * @retval 无
  */
void create_win_edit_ele(EDIT_ELE_T *ele, WM_HWIN hParent)
{
    create_one_name_ele(ele, hParent);//创建名称对象
    create_one_edit_ele(ele, hParent);//创建编辑对象
    create_one_unit_ele(ele, hParent);//创建单位对象
}

/**
  * @brief  获取当前窗口文本对象的链表头
  * @param  无
  * @retval 无
  */
TEXT_ELE_T *get_cur_win_text_ele_list_head(void)
{
    if(g_cur_win == NULL)
    {
        return NULL;
    }
    
    if(list_empty(&g_cur_win->text.list_head))
    {
        return NULL;
    }
    
    return list_entry(g_cur_win->text.list_head.next, TEXT_ELE_T, list);
}
/**
  * @brief  获取当前窗口编辑对象的链表头
  * @param  无
  * @retval 无
  */
EDIT_ELE_T *get_cur_win_edit_ele_list_head(void)
{
    if(g_cur_win == NULL)
    {
        return NULL;
    }
    
    if(list_empty(&g_cur_win->edit.list_head))
    {
        return NULL;
    }
    
    return list_entry(g_cur_win->edit.list_head.next, EDIT_ELE_T, e_list);
}

void update_ele_range_text(EDIT_ELE_T *ele)
{
    uint8_t*str[2] = {0};//ele->range.notice[SYS_LANGUAGE];
    uint8_t buf[100] = {0};
    uint8_t buf1[100] = {0};
    uint8_t buf2[100] = {0};
    int32_t i = 0;
    
    str[CHINESE] = ele->range.notice[CHINESE];
    str[ENGLISH] = ele->range.notice[ENGLISH];
    
    switch(ele->type.type)
    {
        case ELE_TEXT:
            break;
        case ELE_EDIT_NUM:
        {
            if(ele->range.provided_dis_range_fun != NULL)
            {
                ele->range.provided_dis_range_fun(ele);
            }
            else
            {
                mysprintf(buf1, unit_pool[ele->format.unit], ele->format.decs + ele->format.lon * 10 + 1 * 100, ele->range.low);
                mysprintf(buf2, unit_pool[ele->format.unit], ele->format.decs + ele->format.lon * 10 + 1 * 100, ele->range.high);
                sprintf((char*)buf, "%s - %s", buf1, buf2);
                sprintf((char*)buf1, "%s %s", buf, str[CHINESE]);
                sprintf((char*)buf2, "%s %s", buf, str[ENGLISH]);
                str[CHINESE] = buf1;
                str[ENGLISH] = buf2;
            }
            break;
        }
        case ELE_EDIT_STR:
            break;
        case ELE_DROPDOWN:
            for(i = 0; i < ele->resource.size; i++)
            {
                strcat((char*)buf, ((const char**)ele->resource.table)[i]);
                strcat((char*)buf, " ");
            }
            
            str[CHINESE] = buf;
            str[ENGLISH] = buf;
            break;
    }
    
    set_com_text_ele_inf((CS_INDEX)COM_RANGE_NOTICE, g_cur_win, str);
    update_com_text_ele((CS_INDEX)COM_RANGE_NOTICE, g_cur_win, str[SYS_LANGUAGE]);
}
/**
  * @brief  改变编辑对象中编辑控件的背景色，根据编辑控件的类型来区别处理
  * @param  [in] ele 编辑对象信息
  * @param  [in] color 要设置的背景色
  * @retval 无
  */
static void change_edit_ele_edit_bkcolor(EDIT_ELE_T *ele, GUI_COLOR color)
{
    switch(ele->type.type)
    {
        case ELE_TEXT:
            TEXT_SetBkColor(ele->dis.edit.handle, SELETED_COLOR);
            break;
        case ELE_EDIT_NUM:
        case ELE_EDIT_STR:
            EDIT_SetBkColor(ele->dis.edit.handle, EDIT_CI_ENABLED, color);
            break;
        case ELE_DROPDOWN:
            DROPDOWN_SetBkColor(ele->dis.edit.handle, 1, color);
            break;
    }
}
/**
  * @brief  改变编辑对象中名称的背景色
  * @param  [in] ele 编辑对象信息
  * @param  [in] color 要设置的背景色
  * @retval 无
  */
static void change_edit_ele_name_bkcolor(EDIT_ELE_T *ele, GUI_COLOR color)
{
	TEXT_SetBkColor(ele->dis.name.handle, color);
}
/**
  * @brief  改变编辑对象中单位的背景色
  * @param  [in] ele 编辑对象信息
  * @param  [in] color 要设置的背景色
  * @retval 无
  */
static void change_edit_ele_unit_bkcolor(EDIT_ELE_T *ele, GUI_COLOR color)
{
	TEXT_SetBkColor(ele->dis.unit.handle, color);
}
/**
  * @brief  选中编辑对象，选中后要进行一系列的操作，改变背景颜色，范围信息，系统按键
  * @brief  菜单键，键盘服务函数，并将焦点放到该控件上来
  * @param  [in] ele 编辑对象信息
  * @retval 无
  */
void select_edit_ele(EDIT_ELE_T *ele)
{
    if(ele == NULL)
    {
        return;
    }
    
    change_edit_ele_name_bkcolor(ele, SELETED_COLOR);
//     change_edit_ele_edit_bkcolor(ele, SELETED_COLOR);
    change_edit_ele_unit_bkcolor(ele, SELETED_COLOR);
    
    update_ele_range_text(ele);
    
    if(ele->key_inf.fun_sys_key != NULL)
    {
        ele->key_inf.fun_sys_key(ele->dis.edit.handle);
    }
    if(ele->key_inf.fun_menu_key != NULL)
    {
        ele->key_inf.fun_menu_key(ele->dis.edit.handle);
    }
    
    set_scan_key_custom_fun(ele->key_inf.fun_key);
    
    WM_SetFocus(ele->dis.edit.handle);
}
/**
  * @brief  取消选中编辑对象，取消选中后要 恢复背景颜色，将数据上载到内存中
  * @param  [in] ele 编辑对象信息
  * @param  [in] flag LOAD_TO_RAM表示将数据上载到内存，UNLOAD_TO_RAM表示不上载数据，多设置为LOAD_TO_RAM，在设置密码时用到UNLOAD_TO_RAM
  * @retval 无
  */
void dis_select_edit_ele(EDIT_ELE_T *ele, LOAD_DATA_FLAG flag)
{
    change_edit_ele_name_bkcolor(ele, ele->dis.name.back_color);
    change_edit_ele_edit_bkcolor(ele, ele->dis.edit.back_color);
    change_edit_ele_unit_bkcolor(ele, ele->dis.unit.back_color);
    
    if(flag == LOAD_TO_RAM)
    {
        upload_par_to_ram(ele);//将数据上载到内存中
    }
}
/**
  * @brief  选中文本对象，取消选中后要 改变背景颜色
  * @param  [in] ele 文本对象信息
  * @retval 无
  */
void select_text_ele(TEXT_ELE_T *ele)
{
    TEXT_SetBkColor(ele->handle, SELETED_COLOR);
    TEXT_SetTextColor(ele->handle, GUI_RED);
}
/**
  * @brief  取消选中文本对象，取消选中后要 恢复背景颜色
  * @param  [in] ele 文本对象信息
  * @retval 无
  */
void dis_select_text_ele(TEXT_ELE_T *ele)
{
    TEXT_SetBkColor(ele->handle, ele->dis_info.back_color);
    TEXT_SetTextColor(ele->handle, ele->dis_info.font_color);
}
/**
  * @brief  获取当前编辑对象中编辑控件的句柄
  * @param  无
  * @retval WM_HMEM 控件句柄
  */
WM_HMEM get_cur_edit_handle(void)
{
    if(g_cur_edit_ele == NULL)
    {
        return 0;
    }
    
    return g_cur_edit_ele->dis.edit.handle;
}

/**
  * @brief  清空编辑对象内容
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void clear_edit_ele(int hWin)
{
    EDIT_SetText(hWin, "");
}
/**
  * @brief  向当前窗口控件发送一个九九退格键消息
  * @param  [in] hWin 窗口控件句柄
  * @retval 无
  */
void menu_key_backspace(int hWin)
{
	GUI_SendKeyMsg(GUI_KEY_BACKSPACE, 1);
}

/**
  * @brief  删除当前编辑的字符串中光标所在的位置
  * @param  [in] hWin 窗口控件句柄
  * @retval 无
  */
void del_a_char_from_edit_str(uint8_t cur)
{
    uint8_t *str;
    uint8_t len;
    uint8_t buf[10];
    
    if(cur == 0)
    {
        return;
    }
    
    str = g_cur_edit_ele->data.data;
    len = strlen((const char*)str);
    strncpy((char*)buf, (const char*)str, sizeof(buf) - 1);
    
    str[cur - 1] = 0;
    
    if(len > cur)
    {
        strncat((char*)str, (const char*)&buf[cur], len - cur);
    }
}

/**
  * @brief  在当前编辑的字符串中光标所在的位置增加一个字符
  * @param  [in] hWin 窗口控件句柄
  * @retval 无
  */
void add_a_char_into_edit_str(uint8_t ch, uint8_t cur, uint8_t max_len)
{
    uint8_t *str;
    uint8_t len;
    uint8_t buf[10];
    
    str = g_cur_edit_ele->data.data;
    len = strlen((const char*)str);
    if(len >= max_len)
    {
        return;
    }
    
    strncpy((char*)buf, (const char*)str, sizeof(buf) - 1);
    
    str[cur] = ch;
    str[cur + 1] = 0;
    
    if(len > cur)
    {
        strncat((char*)str, (const char*)&buf[cur], len - cur);
    }
}
/**
  * @brief  初始化菜单键定制信息
  * @param  [in] cus_inf 菜单定制信息数组
  * @param  [in] cus_size 菜单定制信息数组元素个数
  * @param  [in] cus_inf 编辑控件信息
  * @param  [in] inf 菜单按键信息数组
  * @param  [in] size 菜单按键信息数组元素个数
  * @retval 无
  */
void init_menu_key_custom_inf(CUSTOM_MENU_KEY_INF *cus_inf,
                            uint16_t cus_size,
                            EDIT_ELE_T *edit,
                            MENU_KEY_INFO_T * inf,
                            uint16_t size)
{
    uint8_t ** table = edit->resource.table;
    uint16_t num = edit->resource.size;
    int32_t i = 0;
    int32_t j = 0;
    
    for(i = 0; i < num && i < size; i++)
    {
        if(inf[i].index == F_KEY_CUSTOM)
        {
            inf[i].name = table[i];
            
            /* 匹配定制信息 */
            for(j = 0; j < cus_size; j++)
            {
                /* 如果资源表中的内容与定制信息中的名称一致就初始化数据并退出循环 */
                if(0 == strncmp((const char*)table[i], (const char*)cus_inf[j].name, 20))
                {
                    inf[i].fun_key.key_up_dispose_fun = cus_inf[j].fun;
                    inf[i].fun_key.msg.custom_data = cus_inf[j].data;
                    
                    break;
                }
            }
        }
    }
    
    for(; i < size; i++)
    {
        if(inf[i].index == F_KEY_CUSTOM)
        {
            inf[i].name = "";
        }
    }
}

/**
  * @brief  设置下拉框的开关变量为打开状态
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void set_sw_status_on(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = SW_ON;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
}

/**
  * @brief  设置下拉框的开关变量为关闭状态
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void set_sw_status_off(WM_HMEM hWin)
{
    uint8_t size = g_cur_edit_ele->data.bytes;
    uint32_t value = SW_OFF;
    WM_HMEM handle = g_cur_edit_ele->dis.edit.handle;
    
    DROPDOWN_SetSel(handle, value);
    DROPDOWN_SetUserData(handle, &value, size);
    upload_par_to_ram(g_cur_edit_ele);//数据更新到内存
}

/**
  * @brief  公用向上键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
void com_edit_win_direct_key_up_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.prev)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev, EDIT_ELE_T, e_list);
    }
    else
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.prev->prev, EDIT_ELE_T, e_list);
    }
    
    select_edit_ele(g_cur_edit_ele);
}
/**
  * @brief  公用的编辑窗口向下键的回调函数
  * @param  [in] key_msg 回调函数携带的按键消息
  * @retval 无
  */
void com_edit_win_direct_key_down_cb(KEY_MESSAGE *key_msg)
{
    dis_select_edit_ele(g_cur_edit_ele, LOAD_TO_RAM);
    
    if(&g_cur_win->edit.list_head != g_cur_edit_ele->e_list.next)
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next, EDIT_ELE_T, e_list);
    }
    else
    {
        g_cur_edit_ele = list_entry(g_cur_edit_ele->e_list.next->next, EDIT_ELE_T, e_list);
    }
    
    select_edit_ele(g_cur_edit_ele);
}

/**
  * @brief  初始化开关类型编辑对象的资源信息
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
void init_sw_type_edit_ele_resource_inf(EDIT_ELE_T* ele)
{
    ele->resource.table = sw_pool[SYS_LANGUAGE];
    ele->resource.size = ARRAY_SIZE(sw_pool[SYS_LANGUAGE]);
}

/**
  * @brief  设置当前窗口编辑控件的索引表信息
  * @param  [in] step 步骤参数结构
  * @retval 无
  */
void set_g_cur_win_edit_index_inf(CS_INDEX *index_pool, uint32_t size)
{
    g_cur_win->edit.index_pool = index_pool;
    g_cur_win->edit.index_size = size;
}

/**
  * @brief  注册步骤编辑对象的数据
  * @param  [in] index 编辑对象在编辑对象池中的索引
  * @param  [in] data 数据的地址
  * @retval 无
  */
void reg_step_ele_data(CS_INDEX index, void *data, uint8_t bytes)
{
    CS_INDEX tmp_index;
    CS_ERR err;
    EDIT_ELE_T *pool;
    EDIT_ELE_T* ele;
    uint32_t size;
    
    pool = g_cur_win->edit.pool;
    size = g_cur_win->edit.pool_size;
    tmp_index = get_edit_ele_index(pool, size, index, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    ele = &pool[tmp_index];
    
    ele->data.data = data;
    ele->data.bytes = bytes;
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
