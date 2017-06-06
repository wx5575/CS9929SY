
#define UI_COM_INFO_GLOBALS
#include "ui_com_info.h"
#include "keyboard.h"
#include "scan_keyboard.h"


void myGUI_DrawRectEx(const GUI_RECT * pRect)
{
	GUI_DrawLine(pRect->x0, pRect->y0, pRect->x1, pRect->y0);
	GUI_DrawLine(pRect->x0, pRect->y1, pRect->x1, pRect->y1);
	GUI_DrawLine(pRect->x0, pRect->y0, pRect->x0, pRect->y1);
	GUI_DrawLine(pRect->x1, pRect->y0, pRect->x1, pRect->y1);
}

void init_user_widget_info(_USER_WIDGET_INFO *info, WM_HWIN hwin)
{
	int x0;
	int y0;
	int xSize;
	int ySize;
	WM_HWIN hParent;
	int WinFlags;
	int ExFlags;
	int Id;
	const char * pText;
	WM_HWIN    _hTitle;
	int align;/* 对齐方式 */
	const GUI_FONT * pFont;//字体
	
	x0 = info->pos->base_x + info->pos->x;
	y0 = info->pos->base_y + info->pos->y;
	xSize = info->pos->width;
	ySize = info->pos->height;
	hParent = hwin;//WM_HBKWIN;
	WinFlags = WM_CF_LATE_CLIP | WM_CF_SHOW;
	ExFlags = 0;
	Id = info->id;
	pText = (const char *)info->text[SYS_LANGUAGE];
	align = info->align;
	pFont = info->pFont;
	
	_hTitle = TEXT_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, pText);
	TEXT_SetTextAlign(_hTitle, align);
	TEXT_SetFont(_hTitle, pFont);
	TEXT_SetBkColor(_hTitle, info->bk_color);
	TEXT_SetTextColor(_hTitle, info->font_color);
	
	info->handle = _hTitle;
}

_USER_WIDGET_INFO *find_user_widget_info(_USER_WIDGET_INFO *info, uint16_t pool_size, uint16_t id)
{
	int32_t i = 0;
	uint32_t num = pool_size;//sizeof(user_widget_info) / sizeof(user_widget_info[0]);
	
	for(i = 0; i < num; i++)
	{
		if(id == info[i].id)
		{
			return &info[i];
		}
	}
	
	return NULL;
}
/*
* 函数名:
* 描述:找到指定ID的控件的位置信息
* 输入:id
* 输出:无
* 返回:WIDGET_POS_INFO *
*/
WIDGET_POS_INFO *find_widget_pos_info(WIDGET_POS_INFO *pos_pool,uint32_t pool_size, uint16_t id)
{
	int32_t i = 0;
	
	if(pos_pool == NULL)
	{
		return NULL;
	}
	
	for(i = 0; i < pool_size; i++)
	{
		if(id == pos_pool[i].id)
		{
			return &pos_pool[i];
		}
	}
	
	return NULL;
}

void init_widget_pos_info(_USER_WIDGET_INFO *widget_info, uint16_t widget_num, WIDGET_POS_INFO *pos_info, uint16_t pos_info_num)
{
	int32_t i = 0;
	uint32_t widget_pool_size = widget_num;//sizeof(user_widget_info) / sizeof(user_widget_info[0]);
	uint32_t pos_pool_size = pos_info_num;//sizeof(widget_pos_info) / sizeof(widget_pos_info[0]);
	WIDGET_POS_INFO *pos;
	
	for(i = 0; i < widget_pool_size; i++)
	{
		pos = find_widget_pos_info(pos_info, pos_pool_size, widget_info[i].id);
		
		if(NULL != pos)
		{
			widget_info[i].pos = pos;
		}
	}
}

TEXT_Handle get_text_widget_handle(_USER_WIDGET_INFO *widget_info, uint16_t widget_num, int Id)
{
	_USER_WIDGET_INFO *info;
// 	int32_t i = 0;
	uint32_t num = widget_num;//sizeof(user_widget_info) / sizeof(user_widget_info[0]);
	
	info = find_user_widget_info(widget_info, num, Id);
	
	if(NULL == info)
	{
		return WM_HMEM_NULL;
	}
	
	return info->handle;
}

// void set_direct_key_fun(_FUNCTION_KEY_INFO_T_ *info)
// {
// 	int32_t i = 0;
// 	uint32_t key_v_buf[]={
// 		KEY_UP,
// 		KEY_DOWN,
// 		KEY_LEFT,
// 		KEY_RIGHT,
// 		KEY_UNLOCK  ,
// 		KEY_OFFSET  ,
// 		KEY_ENTER	,
// 		KEY_EXIT	,
// 	};
// 	uint32_t size = sizeof(key_v_buf) / sizeof(key_v_buf[0]);
// 	
// // 	for(i = 0; i < size; i++)
// // 	{
// // 		if(info[i].key_value == key_v_buf[i])
// // 		{
// // 			register_key_dispose_fun(key_v_buf[i], info[i].key_up_dispose_fun);
// // 		}
// // 	}
// }


void init_key_widget_info(_USER_WIDGET_INFO *pool_info, uint16_t info_num, MENU_KEY_INFO *key_info, uint16_t key_info_size)
{
	uint32_t n = key_info_size;//sizeof(menu_key_info_pool) / sizeof(menu_key_info_pool[0]);
	int32_t i = 0;
	uint32_t widget_pool_size = info_num;//sizeof(user_widget_info) / sizeof(user_widget_info[0]);
	_USER_WIDGET_INFO *info = NULL;
	 
	for(i = 0; i < n; i++)
	{
		info = find_user_widget_info(pool_info, widget_pool_size, key_info[i].id);
		
		if(NULL != info)
		{
			if(key_info[i].id == info->id)
			{
				key_info[i].handle = info->handle;
			}
		}
	}
}

void (*global_fun_key_dispose)(uint32_t key_value);
void (*global_menu_key_dispose_fun)();
int global_key_dispose_user_data;//按键处理函数使用的用户数据


void set_global_fun_key_dispose(void (*fun)(uint32_t key_value))
{
	global_fun_key_dispose = fun;
}


PAR_SET_KEY_SERVER_INFO * find_edit_handle(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle)
{
	uint32_t n = size;
	uint32_t i = 0;
	
	for(i = 0; i < n; i++)
	{
		if(handle == info[i].handle)
		{
			return &info[i];
		}
	}
	
	return NULL;
}

int get_set_par_menu_key_index(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle)
{
	PAR_SET_KEY_SERVER_INFO * t_info;
	
	t_info = find_edit_handle(info, size, handle);
	
	if(NULL != t_info)
	{
		return t_info->menu_key_index;
	}
	
	return 0;
}

void *get_edit_widget_key_custom_fun(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle)
{
	PAR_SET_KEY_SERVER_INFO * t_info;
	
	t_info = find_edit_handle(info, size, handle);
	
	if(NULL != t_info)
	{
		return t_info->fun_key;
	}
	
	return NULL;
}
PAR_SET_KEY_SERVER_INFO * find_march_edit_widget(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, int id)
{
	uint32_t n = size;
	uint32_t i = 0;
	
	for(i = 0; i < n; i++)
	{
		if(id == info[i].id)
		{
			return &info[i];
		}
	}
	
	return NULL;
}

void set_par_set_key_server_handle(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, int id, WM_HWIN handle)
{
	PAR_SET_KEY_SERVER_INFO * t_info;
	
	t_info = find_march_edit_widget(info, size, id);
	
	if(NULL != t_info)
	{
		t_info->handle = handle;
	}
}

int get_par_set_key_server_array_index(PAR_SET_KEY_SERVER_INFO *info, uint32_t size, WM_HWIN handle)
{
	uint32_t n = size;
	uint32_t i = 0;
	
	for(i = 0; i < n; i++)
	{
		if(handle == info[i].handle)
		{
			return i;
		}
	}
	
	return 0;
}

