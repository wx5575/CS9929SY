/**
  ******************************************************************************
  * @file    com_ui_info.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   这个文件定义了文本控件操作的函数
  ******************************************************************************
  */
/* Private macro -------------------------------------------------------------*/
#define COM_UI_GLOBALS

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ui_com/com_ui_info.h"
#include "string.h"
#include "stdio.h"
#include "OS.H"


/**
  * @brief  获取文本对象在控件池中的索引
  * @param  [in] index 用户窗口指针
  * @param  [in] pool 用户窗口指针
  * @param  [out] err 成功返回 CS_ERR_NONE, 失败返回 CS_ERR_ELE_INDEX_INVALID
  * @retval 索引的文本对象在文本对象数组中的下标索引
  */
CS_INDEX get_text_ele_index_in_pool(CS_INDEX index, ELE_POOL_INF *pool, CS_ERR *err)
{
    uint32_t size = pool->pool_size;
    int32_t i = 0;
    TEXT_ELE_T *text_pool = NULL;
    
    *err = CS_ERR_NONE;
    
    for(i = 0; i < size; i++)
    {
        text_pool = pool->pool;
        
        if(index == text_pool[i].index)
        {
            return (CS_INDEX)i;
        }
    }
    
    *err = CS_ERR_ELE_INDEX_INVALID;
    
    return (CS_INDEX)0;
}
/**
  * @brief  初始化窗口中文本对象的链表
  * @param  [in] win 用户窗口信息
  * @retval 无
  */
void init_window_text_ele_list(MYUSER_WINDOW_T *win)
{
	int32_t i = 0;
	uint32_t n = win->text.index_size;
	CS_LIST *list = &win->text.list_head;
	CS_LIST *new_node = NULL;
    CS_INDEX index;
    CS_INDEX temp;
    CS_ERR err;
    TEXT_ELE_T *ele_pool = NULL;
	
	list_init(list);//初始化文本链表
	
	for(i = 0; i < n; i++)
	{
        temp = win->text.index_pool[i];
        index = get_text_ele_index_in_pool(temp, &win->text, &err);
        
        if(err != CS_ERR_NONE)
        {
            continue;
        }
        
        ele_pool = win->text.pool;
		new_node = &ele_pool[index].list;
        
		list_add_tail(new_node, list);
	}
}
/**
  * @brief  根据自动布局的结构参数初始化界面中的文本对象链表的位置信息 
  * @param  [in] win 用户窗口指针
  * @param  [in] inf 自动布局结构参数
  * @retval 无
  */
void init_window_text_ele_dis_inf(MYUSER_WINDOW_T *win, TEXT_ELE_AUTO_LAYOUT_T* inf)
{
	int32_t row = 0;
	int32_t column = 0;
	CS_LIST *list = &win->text.list_head;
    WIDGET_POS_SIZE_T *pos;
    UI_ELE_DISPLAY_INFO_T *dis;
	CS_LIST* t_node = NULL;
	TEXT_ELE_T *node = NULL;
	
    list_for_each(t_node, list)
    {
        node = list_entry( t_node, TEXT_ELE_T, list );
        
        dis = &node->dis_info;
        pos = &node->dis_info.pos_size;
        
        dis->base_x = inf->base_x;
        dis->base_y = inf->base_y;
        pos->x = inf->column_spacing * column;
        pos->y = inf->row_spacing * row;
        pos->width = inf->width;
        pos->height = inf->height;
        
        memcpy(dis->font, inf->font, sizeof(inf->font));
        dis->font_color = inf->font_color;
        dis->back_color = inf->back_color;
        dis->align = inf->align;
        dis->max_len = inf->max_len;
        
        ++row;
        
        if(row > inf->rows)
        {
            row = 0;
            column++;
        }
    }
}
/**
  * @brief  根据自动布局的结构参数初始化界面中的编辑对象链表的位置信息
  * @param  [in] win 用户窗口指针
  * @param  [in] inf 自动布局结构参数
  * @retval 无
  */
void init_window_edit_ele_dis_inf(MYUSER_WINDOW_T *win, EDIT_ELE_AUTO_LAYOUT_T* inf)
{
	int32_t row = 0;
	int32_t column = 0;
	CS_LIST *list = &win->edit.list_head;
    EDIT_ELE_DISPLAY_INF *dis;
	CS_LIST* t_node = NULL;
	WIDGET_ELEMENT *node = NULL;
	
    list_for_each(t_node, list)
    {
        node = list_entry( t_node, WIDGET_ELEMENT, e_list );
        
        dis = &node->dis;
        
        dis->x = inf->base_x + inf->column_spacing * column;
        dis->y = inf->base_y + inf->row_spacing * row;
        dis->edit.width = inf->edit_w;
        dis->name.width = inf->name_w;
        dis->unit.width = inf->uint_w;
        
        dis->edit.height = inf->height;
        dis->name.height = inf->height;
        dis->unit.height = inf->height;
        
        dis->name.font = inf->font;
        dis->edit.font = inf->font;
        dis->unit.font = inf->font;
        
        dis->name.font_color = inf->font_color;
        dis->edit.font_color = inf->font_color;
        dis->unit.font_color = inf->font_color;
        
        dis->name.back_color = inf->back_color;
        dis->edit.back_color = inf->back_color;
        dis->unit.back_color = inf->back_color;
        
        dis->name.max_len = inf->max_len;
        dis->edit.max_len = inf->max_len;
        dis->unit.max_len = inf->max_len;
        
        dis->name.align = inf->align;
        dis->edit.align = inf->align;
        dis->unit.align = inf->align;
        
        ++row;
        
        if(row > inf->rows)
        {
            row = 0;
            column++;
        }
    }
}
/**
  * @brief  初始化界面中的公共文本对象的链表
  * @param  [in] win 用户窗口指针
  * @retval 无
  */
void init_window_com_ele_list(MYUSER_WINDOW_T *win)
{
	int32_t i = 0;
	uint32_t n = win->com.index_size;
	CS_LIST *list = &win->com.list_head;
	CS_LIST *new_node = NULL;
    uint32_t index = 0;
	TEXT_ELE_T *text_pool = NULL;
	
	list_init(list);//初始化文本链表
	
	for(i = 0; i < n; i++)
	{
        index = win->com.index_pool[i];
        text_pool = win->com.pool;
		new_node = &text_pool[index].list;
		list_add_tail(new_node, list);
	}
}
/**
  * @brief  更新窗口内编辑对象的名称文本
  * @param  [in] list 编辑对象链表
  * @retval 无
  */
static void update_win_edit_ele_text(CS_LIST *list)
{
    CS_LIST *t_node;
    WIDGET_ELEMENT *node;
    
	list_for_each(t_node, list)
	{
		node = list_entry( t_node, WIDGET_ELEMENT, e_list );
        
        TEXT_SetText(node->dis.name.handle, (const char *)node->name[SYS_LANGUAGE]);
	}
}
/**
  * @brief  更新窗口内文本对象的文本或公共文本对象的文本
  * @param  [in] list 文本对象链表
  * @retval 无
  */
static void update_win_text_ele_text(CS_LIST *list)
{
    CS_LIST *t_node;
    TEXT_ELE_T *node;
    
	list_for_each(t_node, list)
	{
		node = list_entry( t_node, TEXT_ELE_T, list );
        
        TEXT_SetText(node->handle, (const char *)node->text[SYS_LANGUAGE]);
	}
}
/**
  * @brief  更新窗口内所有的文本,包括:文本对象的文本，编辑对象的文本，公共文本对象的文本
  * @param  [in] win 用户窗口指针
  * @retval 无
  */
static void update_win_all_text(MYUSER_WINDOW_T *win)
{
    update_win_text_ele_text(&win->text.list_head);
    update_win_text_ele_text(&win->com.list_head);
    update_win_edit_ele_text(&win->edit.list_head);
}
/**
  * @brief  更新所有窗口的文本,不包括窗口标题，这个接口的用处是在改变系统语言时
  * @param  无
  * @retval 无
  */
void update_all_windows_text(void)
{
	CS_LIST *list = &windows_list;//窗口链表
    CS_LIST *tmp_list;
    MYUSER_WINDOW_T *win;
    
	list_for_each(tmp_list, list)
	{
		win = list_entry( tmp_list, MYUSER_WINDOW_T, w_list );
        
        update_win_all_text(win);
	}
}
/**
  * @brief  创建文本控件
  * @param  [in] win 用户窗口指针
  * @param  [in] node 文本对象指针
  * @retval 无
  */
static void create_text_widget(WM_HWIN hwin, TEXT_ELE_T *node)
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
	
	x0 = node->dis_info.base_x + node->dis_info.pos_size.x;
	y0 = node->dis_info.base_y + node->dis_info.pos_size.y;
	xSize = node->dis_info.pos_size.width;
	ySize = node->dis_info.pos_size.height;
	hParent = hwin;//WM_HBKWIN;
	WinFlags = WM_CF_LATE_CLIP | WM_CF_SHOW;
	ExFlags = 0;
	Id = id_base++;
	pText = (const char *)node->text[SYS_LANGUAGE];
	align = node->dis_info.align;
	pFont = SEL_FONT(node->dis_info.font);
	
	_hTitle = TEXT_CreateEx(x0, y0, xSize, ySize, hParent, WinFlags, ExFlags, Id, pText);
	TEXT_SetTextAlign(_hTitle, align);
	TEXT_SetFont(_hTitle, pFont);
	TEXT_SetBkColor(_hTitle, node->dis_info.back_color);
	TEXT_SetTextColor(_hTitle, node->dis_info.font_color);
	
	node->handle = _hTitle;
}
/**
  * @brief  获取文本对象的指针
  * @param  [in] index 文本对象在对象池中的索引
  * @param  [in] list 用户窗口指针
  * @param  [out] err 正常输出 CS_ERR_NONE, 异常输出 CS_ERR_INDEX_INVALID
  * @retval 文本对象的指针,如果找不到对应的文本对象返回NULL
  */
TEXT_ELE_T* get_text_ele_node(CS_INDEX index, CS_LIST* list, CS_ERR *err)
{
	CS_LIST* t_node = NULL;
	TEXT_ELE_T *node = NULL;
    
    *err = CS_ERR_NONE;
	
	list_for_each(t_node, list)
	{
		node = list_entry( t_node, TEXT_ELE_T, list );
		
		if(node->index == index)
		{
			return node;
		}
	}
	
    *err = CS_ERR_INDEX_INVALID;
    
	return NULL;
}
/**
  * @brief  显示文本对象
  * @param  [in] node 文本对象的指针
  * @param  [in] str 要显示的文本
  * @retval 无
  */
static void _show_text_ele(TEXT_ELE_T*node, const uint8_t*str)
{
	const char * pText = (void*)str;//文本
	WM_HWIN    handle;//句柄
	int align;/* 对齐方式 */
	const GUI_FONT * font;// 字体
	GUI_COLOR	font_color;// 字体颜色
	GUI_COLOR	back_color;// 背景颜色
	UI_ELE_DISPLAY_INFO_T *dis_info = &node->dis_info;
	
	handle = node->handle;
	align = dis_info->align;
	font = SEL_FONT(dis_info->font);
	back_color = dis_info->back_color;
	font_color = dis_info->font_color;
	
    if(str == NULL)
    {
        pText = (const char *)node->text[SYS_LANGUAGE];
    }
    
	TEXT_SetTextAlign(handle, align);
	TEXT_SetFont(handle, font);
	TEXT_SetBkColor(handle, back_color);
	TEXT_SetTextColor(handle, font_color);
	TEXT_SetText(handle, pText);
}
void set_text_ele_font_color(CS_INDEX index, MYUSER_WINDOW_T* win, GUI_COLOR color)
{
	TEXT_ELE_T *node = NULL;
    CS_ERR err;
	
	node = get_text_ele_node(index, &win->text.list_head, &err);//获取文本对象指针
	
	if(node == NULL || err != CS_ERR_NONE)
	{
		return;
	}
	
    node->dis_info.font_color = color;
}
/**
  * @brief  查找并显示文本对象，如果查找到就显示文本对象内容
  * @param  [in] index 文本对象的索引
  * @param  [in] win 文本对象所在窗口的指针
  * @param  [in] str 要显示的文本
  * @retval 无
  */
void update_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, const uint8_t *str)
{
	TEXT_ELE_T *node = NULL;
    CS_ERR err;
	
	node = get_text_ele_node(index, &win->text.list_head, &err);//获取文本对象指针
	
	if(node == NULL || err != CS_ERR_NONE)
	{
		return;
	}
	
	_show_text_ele(node, str);//显示文本对象
}
/**
  * @brief  查找并显示公共文本对象，如果查找到就显示文本对象内容
  * @param  [in] index 文本对象的索引
  * @param  [in] win 文本对象所在窗口的指针
  * @param  [in] str 要显示的文本
  * @retval 无
  */
void update_com_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str)
{
	TEXT_ELE_T *node = NULL;
    CS_ERR err;
	
	node = get_text_ele_node(index, &win->com.list_head, &err);//获取文本对象指针
	
	if(node == NULL || err != CS_ERR_NONE)
	{
		return;
	}
	
	_show_text_ele(node, str);//显示文本对象
}
/**
  * @brief  根据窗口数据信息创建出窗口中的所有文本控件
  * @param  [in] win 窗口信息
  * @retval 无
  */
void init_window_text_ele(MYUSER_WINDOW_T* win)
{
	TEXT_ELE_T *node;
	CS_LIST *index = NULL;
	CS_LIST *t_list = &win->text.list_head;//文本链表
	
	list_for_each(index, t_list)
	{
		node = list_entry(index, TEXT_ELE_T, list);
		create_text_widget(win->handle, node);//创建文本对象
	}
}
/**
  * @brief  初始化创建界面中出现的公共文本控件，为了处理相同文本控件在不同的界面反复出现
  * @brief  如:范围,提示信息
  * @param  [in] win 窗口信息
  * @retval 无
  */
void init_window_com_text_ele(MYUSER_WINDOW_T* win)
{
	TEXT_ELE_T *node;
	CS_LIST *index = NULL;
	CS_LIST *t_list = &win->com.list_head;//文本链表
	
    if(win == NULL)
    {
        return;
    }
	
	list_for_each(index, t_list)
	{
		node = list_entry(index, TEXT_ELE_T, list);
		create_text_widget(win->handle, node);//创建文本对象
	}
}
/**
  * @brief  创建用户窗口，创建用户窗口的同时要对用户窗口中的所有链表头进行初始化，创建后就设置为当前窗口
  * @param  [in] win_info 窗口对象指针
  * @param  [in] list_head 窗口链表头，所有创建的窗口都放入这个链表
  * @retval 无
  */
void create_user_window(MYUSER_WINDOW_T* win_info, CS_LIST *list_head, WM_HWIN h_parent)
{
	uint16_t x = win_info->pos_size.x;
	uint16_t y = win_info->pos_size.y;
	uint16_t width = win_info->pos_size.width;
	uint16_t height = win_info->pos_size.height;
	USER_CALLBACK cb_fun = win_info->call_back_fun;
    
    if(h_parent == 0)
    {
        h_parent = WM_HBKWIN;
    }
    
	list_add_tail(&win_info->w_list, list_head);//将创建的新窗口加入窗口链表中
    list_init(&win_info->text.list_head);//初始化文本对象链表
    list_init(&win_info->edit.list_head);//初始化编辑对象链表
    list_init(&win_info->com.list_head);//初始化公共文本对象链表
    
    set_cur_window(win_info);//将新建窗口设为当前窗口
    win_info->handle = WM_CreateWindowAsChild(x, y, width, height, h_parent, WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW, cb_fun, 0);//WM_CF_SHOW
}
/**
  * @brief  设置当前用户窗口尺寸
  * @param  [in] win_inf 窗口结构指针
  * @param  [in] pos_size_inf 尺寸结构指针
  * @retval 无
  */
void init_window_size(MYUSER_WINDOW_T* win_inf, WIDGET_POS_SIZE_T *pos_size_inf)
{
    memcpy(&win_inf->pos_size, pos_size_inf, sizeof(WIDGET_POS_SIZE_T));
}

/**
  * @brief  初始化并创建窗口内所有控件 只包含文本控件和编辑控件(编辑框、下拉框)
  * @param  [in] win 窗口结构指针
  * @retval 无
  */
void init_create_win_all_ele(MYUSER_WINDOW_T* win)
{
    if(NULL != win->text.init_create_fun)
    {
        win->text.init_create_fun(win);//创建文本对象
    }
    
    if(NULL != win->edit.init_create_fun)
    {
        win->edit.init_create_fun(win);//创建编辑对象
    }
    
    if(NULL != win->com.init_create_fun)
    {
        win->com.init_create_fun(win);//创建公共文本对象
    }
}
/**
  * @brief  设置当前用户窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void set_user_window_handle(WM_HWIN hWin)
{
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    g_cur_win->handle = hWin;
    CPU_CRITICAL_EXIT();
}

/**
  * @brief  设置当前用户窗口
  * @param  [in] win_info 窗口信息
  * @retval 无
  */
void set_cur_window(MYUSER_WINDOW_T* win_info)
{
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
	g_cur_win = win_info;//把创建的窗口作为当前窗口
    CPU_CRITICAL_EXIT();
}
/**
  * @brief  创建用户对话框窗口,同时要初始化所有的链表头，创建后就设置为当前窗口
  * @param  [in] win_info 窗口对象指针
  * @param  [in] list_head 窗口链表头
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_user_dialog(MYUSER_WINDOW_T* win_info, CS_LIST *list_head, WM_HWIN hWin)
{
	uint16_t x = win_info->pos_size.x;
	uint16_t y = win_info->pos_size.y;
	uint16_t width = win_info->pos_size.width;
	uint16_t height = win_info->pos_size.height;
	USER_CALLBACK cb_fun = win_info->call_back_fun;
    GUI_WIDGET_CREATE_INFO aDialogBox =
    {
        FRAMEWIN_CreateIndirect, "", 0, 0, 0, 0, 0, 0, 0, 0
    };
	
    aDialogBox.Id = ++id_base;
    aDialogBox.x0 = x;
    aDialogBox.y0 = y;
    aDialogBox.xSize = width;
    aDialogBox.ySize = height;
    aDialogBox.Flags = WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW;
	
	list_add_tail(&win_info->w_list, list_head);//将创建的新窗口加入窗口链表中
    list_init(&win_info->text.list_head);//初始化文本对象链表
    list_init(&win_info->edit.list_head);//初始化编辑对象链表
    list_init(&win_info->com.list_head);//初始化公共文本对象链表
    
    set_cur_window(win_info);
    win_info->handle = GUI_CreateDialogBox(&aDialogBox, 1, cb_fun, hWin, 0, 0);//非阻塞
    WM_ShowWindow(win_info->handle);
}
/**
  * @brief  删除文本对象链表中的所有节点
  * @param  [in] list_head 文本对象链表头
  * @retval 无
  */
static void delete_text_list_node(CS_LIST *list_head)
{
    TEXT_ELE_T *node;
    CS_LIST *index;
    
	list_for_each( index, list_head )
	{
		node = list_entry( index, TEXT_ELE_T, list );
        
		if(node->handle != 0)
		{
            WM_DeleteWindow(node->handle);//删除窗口控件
            node->handle = 0;//清除被删除窗口的句柄
		}
	}
}
/**
  * @brief  删除编辑对象链表中的所有节点
  * @param  [in] list_head 编辑对象链表头
  * @retval 无
  */
static void delete_edit_list_node(CS_LIST *list_head)
{
    WIDGET_ELEMENT *node;
    CS_LIST *index;
    
	list_for_each( index, list_head )
	{
		node = list_entry( index, WIDGET_ELEMENT, e_list );
        
		if(node->dis.name.handle != 0)
		{
            WM_DeleteWindow(node->dis.name.handle);//删除窗口控件
            node->dis.name.handle = 0;//清除被删除窗口的句柄
		}
		if(node->dis.edit.handle != 0)
		{
            WM_DeleteWindow(node->dis.edit.handle);//删除窗口控件
            node->dis.edit.handle = 0;//清除被删除窗口的句柄
		}
		if(node->dis.unit.handle != 0)
		{
            WM_DeleteWindow(node->dis.unit.handle);//删除窗口控件
            node->dis.unit.handle = 0;//清除被删除窗口的句柄
		}
	}
}
/**
  * @brief  删除窗口中所有的对象包括:文本对象，编辑对象，公共文本对象
  * @param  [in] win_info 窗口信息
  * @retval 无
  */
void delete_win_all_ele(MYUSER_WINDOW_T* win)
{
    delete_text_list_node(&win->com.list_head);
    delete_text_list_node(&win->text.list_head);
    delete_edit_list_node(&win->edit.list_head);
}
/**
  * @brief  设置全局当前编辑对象指针
  * @param  [in] node 编辑对象地址
  * @retval 无
  */
void set_cur_edit_ele(WIDGET_ELEMENT *node)
{
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    g_cur_edit_ele = node;
    CPU_CRITICAL_EXIT();
}
/**
  * @brief  删除当前用户界面窗口,并返回新的当前窗口地址，将窗口中所有的对象都删除，将全局的当前编辑对象
  * @brief  指针设为NULL,将删除的窗口的句柄清零
  * @param  [in] win_info 窗口对象指针
  * @retval 新窗口的地址
  */
void del_cur_window(void)
{
	CS_LIST *list_head = &windows_list;//窗口链表头
    MYUSER_WINDOW_T* win_info = g_cur_win;
	int res = 0;
    MYUSER_WINDOW_T* win = NULL;
	
	list_del(&win_info->w_list);//把窗口信息从窗口链表中删除
	res = list_empty(list_head);//判断上一级窗口是否为空
    
    //上一级窗口不为空就获取上一级窗口的地址
	if(!res)
	{
		win = list_entry(list_head->prev, MYUSER_WINDOW_T, w_list);//上一级窗口
	}
    //上一级窗口为空
	else
	{
		win = NULL;
	}
    
    set_cur_window(win);//把新窗口设置为当前窗口
    
    delete_win_all_ele(win_info);//删除窗口中所有控件
    set_cur_edit_ele(NULL);//将当前编辑对象置为空
    disable_system_fun_key_fun();//失能系统功能按键
	WM_DeleteWindow(win_info->handle);//删除窗口控件
	win_info->handle = 0;//清除被删除窗口的句柄
}
/**
  * @brief  显示用户窗口
  * @param  [in] win_info 窗口对象指针
  * @retval 无
  */
static void show_user_window(MYUSER_WINDOW_T* win_info)
{
    if(win_info == NULL)
    {
        return;
    }
    
    WM_ShowWindow(win_info->handle);
	win_info->init_key_fun(win_info->handle);//初始化按键
}
/**
  * @brief  返回上一级窗口,返回上级窗口要做的操作是1.删除当前窗口 2.显示新的当前窗口
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void back_win(int hWin)
{
    del_cur_window();
    show_user_window(g_cur_win);
}
/**
  * @brief  从窗口链表中获取窗口结构地址
  * @param  [in] hWin 窗口句柄
  * @retval 窗口结构地址，如果没有找到返回NULL
  */
MYUSER_WINDOW_T * get_user_window_info(WM_HWIN hWin)
{
	CS_LIST* index = NULL;
	MYUSER_WINDOW_T *node = NULL;
    CS_LIST* list = &windows_list;
	
	list_for_each( index, list )
	{
		node = list_entry( index, MYUSER_WINDOW_T, w_list );
		if(node->handle == hWin)
		{
			return node;
		}
	}
	
	return NULL;
}
/**
  * @brief  初始化对话框
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
void init_dialog(MYUSER_WINDOW_T * win)
{
    
    WM_HWIN hWin;
    
    hWin = win->handle;
    
    if(hWin == 0)
    {
        return;
    }
    
    FRAMEWIN_SetTitleHeight(hWin, 35);
    FRAMEWIN_SetTextColor(hWin, GUI_BLACK);
    FRAMEWIN_SetText(hWin, (const char*)win->win_name[SYS_LANGUAGE]);
    FRAMEWIN_SetFont(hWin, &GUI_Fonthz_26);
    FRAMEWIN_SetTextAlign(hWin, GUI_TA_CENTER);
//    FRAMEWIN_SetBarColor(hWin, 1, GUI_LIGHTBLUE);
    FRAMEWIN_SetBarColor(hWin, 1, 0x804000);
    FRAMEWIN_SetBarColor(hWin, 0, GUI_LIGHTBLUE);
//    FRAMEWIN_SetClientColor(hWin, GUI_DARKGREEN);
    FRAMEWIN_SetActive(hWin, 1);
}

static void _7_draw_group_inf_area(void)
{
    /** 记忆组信息 */
    static const GUI_RECT group_info_area =
    {
        0, 0, 800 - 110, 30
    };
    
    GUI_SetColor(GUI_GRAY);//GUI_LIGHTGRAY
    GUI_FillRectEx(&group_info_area);
}
void draw_group_inf_area(void)
{
    switch(sys_par.screem_size)
    {
        case SCREEN_4_3INCH:
            break;
        case SCREEN_6_5INCH:
            break;
        default:
        case SCREEN_7INCH:
            _7_draw_group_inf_area();
            break;
    }
}
/*******范围信息是很多界面者会用到的公共文本控件**********************/

CS_INDEX range_com_ele_table[COM_RANGE_ELE_NUM]=
{
	COM_RANGE_NAME,///<主界面的通信状态
	COM_RANGE_NOTICE,///<主界面的系统时间
};

CS_INDEX range_group_com_ele_table[COM_ELE_NUM]=
{
	COM_RANGE_NAME,///<主界面的通信状态
	COM_RANGE_NOTICE,///<主界面的系统时间
    COM_UI_FILE_NAME    ,///< 记忆组文件名
    COM_UI_CUR_FILE_NAME,///< 记忆组文件名内容
    COM_UI_STEP         ,///< 记忆组步骤信息
    COM_UI_CUR_STEP     ,///< 记忆组步骤信息内容
    COM_UI_WORK_MODE    ,///< 记忆组工作模式
    COM_UI_CUR_WORK_MODE,///< 记忆组工作模式内容
};
CS_INDEX group_com_ele_table[COM_GRUOP_ELE_NUM]=
{
    COM_UI_FILE_NAME    ,///< 记忆组文件名
    COM_UI_CUR_FILE_NAME,///< 记忆组文件名内容
    COM_UI_STEP         ,///< 记忆组步骤信息
    COM_UI_CUR_STEP     ,///< 记忆组步骤信息内容
    COM_UI_WORK_MODE    ,///< 记忆组工作模式
    COM_UI_CUR_WORK_MODE,///< 记忆组工作模式内容
};
/**
  * @brief  公共文本对象池
  */
TEXT_ELE_T com_text_ele_pool[COM_ELE_NUM]=
{
	{{"范 围:","Range:"}, COM_RANGE_NAME   },
	{{"提示"  ,"Notice"}, COM_RANGE_NOTICE },
    
	{{"文件名:", "FileName:" }, COM_UI_FILE_NAME    },
	{{"DEFAULT", "DEFAULT"   }, COM_UI_CUR_FILE_NAME},
	{{"步骤:"  , "Step:"     }, COM_UI_STEP         },
	{{"01/01"  , "01/01"     }, COM_UI_CUR_STEP     },
	{{"工作模式:","WorkMode:"}, COM_UI_WORK_MODE    },
	{{"N"      , "N"         }, COM_UI_CUR_WORK_MODE},
};

/**
  * @brief  设置公共文本对象的显示相关信息
  * @param  [in] inf 显示相关的信息
  * @param  [in] index 对象索引
  * @retval 无
  */
void set_com_text_ele_dis_inf(UI_ELE_DISPLAY_INFO_T *inf, CS_INDEX index)
{
    if(index >= COM_ELE_NUM)
    {
        return;
    }
    
    memcpy(&com_text_ele_pool[index].dis_info, inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}
/**
  * @brief  设置公共文本对象的显示文本内容
  * @param  [in] index 对象索引
  * @param  [in] win 窗口指针
  * @param  [in] str[] 文本数组
  * @retval 无
  */
void set_com_text_ele_inf(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str[])
{
	TEXT_ELE_T *node = NULL;
    CS_ERR err;
    static uint8_t buf[2][100];//这里使用了内部静态变量，是因为有外部指针指向了它
	
	node = get_text_ele_node(index, &win->com.list_head, &err);//获取文本对象指针
	
	if(node == NULL || err != CS_ERR_NONE)
	{
		return;
	}
    
    /* 使用安全的字符串拷贝 */
    strncpy((char*)buf[CHINESE], (void*)str[CHINESE], sizeof(buf[CHINESE]) - 1);
    strncpy((char*)buf[ENGLISH], (void*)str[ENGLISH], sizeof(buf[ENGLISH]) - 1);
    
    node->text[CHINESE] = buf[CHINESE];
    node->text[ENGLISH] = buf[ENGLISH];
}
/**
  * @brief  设置记忆组文本对象的显示文本内容
  * @param  [in] index 对象索引
  * @param  [in] win 窗口指针
  * @param  [in] str 文本
  * @retval 无
  */
void set_group_text_ele_inf(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str)
{
	TEXT_ELE_T *node = NULL;
    CS_ERR err;
    typedef struct{
        uint8_t name[20];
        uint8_t step[10];
        uint8_t workmode[3];
    }GROUP_INF;
    static GROUP_INF inf;
    uint8_t *p_buf = NULL;
    uint32_t size = 0;
    
	node = get_text_ele_node(index, &win->com.list_head, &err);//获取文本对象指针
	
	if(node == NULL || err != CS_ERR_NONE)
	{
		return;
	}
    
    if(index == COM_UI_CUR_FILE_NAME)
    {
        p_buf = inf.name;
        size = sizeof(inf.name);
    }
    else if(index == COM_UI_CUR_STEP)
    {
        p_buf = inf.step;
        size = sizeof(inf.step);
    }
    else if(index == COM_UI_CUR_WORK_MODE)
    {
        p_buf = inf.workmode;
        size = sizeof(inf.workmode);
    }
    else
    {
        return;
    }
    
    /* 使用安全的字符串拷贝 */
    strncpy((char*)p_buf, (void*)str, size - 1);
    strncpy((char*)p_buf, (void*)str, size - 1);
    
    node->text[CHINESE] = p_buf;
    node->text[ENGLISH] = p_buf;
}

/**
  * @brief  初始化公共文本对象的显示信息(坐标，尺寸
  * @param  [in] win 窗口指针
  * @retval 无
  */
void init_com_text_ele_dis_inf(MYUSER_WINDOW_T* win)
{
    UI_ELE_DISPLAY_INFO_T dis_info=
    {
        0/*base_x*/,0/*base_y*/,0/*x*/,200/*y*/,10/*width*/,30/*height*/,10,
        {&GUI_Fonthz_20}, GUI_BLACK, GUI_INVALID_COLOR,GUI_TA_LEFT | GUI_TA_TOP
    };
    
    dis_info.pos_size.x = 10;
    dis_info.pos_size.y = win->pos_size.height - 45;
    dis_info.pos_size.width = 70;
    dis_info.pos_size.height = 45;
    dis_info.max_len = 100;
    dis_info.font[CHINESE] = &GUI_Fonthz_20;
    dis_info.font_color = GUI_BLACK;
    dis_info.back_color = GUI_INVALID_COLOR;
    dis_info.align = GUI_TA_LEFT;
    
    set_com_text_ele_dis_inf(&dis_info, COM_RANGE_NAME);//范围
    dis_info.pos_size.x += dis_info.pos_size.width;
    dis_info.pos_size.width = win->pos_size.width - 15 -  dis_info.pos_size.width;
    set_com_text_ele_dis_inf(&dis_info, COM_RANGE_NOTICE);//提示信息
}

/**
  * @brief  初始化记忆组公共文本对象的显示信息(坐标，尺寸
  * @param  [in] win 窗口指针
  * @retval 无
  */
void init_group_com_text_ele_dis_inf(MYUSER_WINDOW_T* win)
{
    UI_ELE_DISPLAY_INFO_T inf;
    
    typedef struct{
        TEXT_ELE_T * name;///<多路编号
        TEXT_ELE_T * cur_name;///<多路测试模式
        TEXT_ELE_T * step;///<多路测试状态
        TEXT_ELE_T * cur_step;///<多路电压
        TEXT_ELE_T * work_mode;///<多路电流
        TEXT_ELE_T * cur_work_mode;///<多路真实电流
    }FILE_T;
    
    FILE_T file_t = {
        &com_text_ele_pool[COM_UI_FILE_NAME],
        &com_text_ele_pool[COM_UI_CUR_FILE_NAME],
        &com_text_ele_pool[COM_UI_STEP],
        &com_text_ele_pool[COM_UI_CUR_STEP],
        &com_text_ele_pool[COM_UI_WORK_MODE],
        &com_text_ele_pool[COM_UI_CUR_WORK_MODE],
    };
    FILE_T *pool = &file_t;
    
    #define GB_X		0 ///<记忆组
    #define GB_Y		0
    #define GB_H		30
    #define FN_W		120 ///<文件名
    #define FN_X		0
    #define C_FN_X		FN_X+FN_W ///<当前文件名
    #define C_FN_W		140
    #define STEP_X  	C_FN_X+C_FN_W+1 ///< 步骤
    #define STEP_W  	65
    #define CS_X  		STEP_X+STEP_W//当前步骤
    #define CS_W  		80
    #define WM_X  		CS_X+CS_W+1 ///<工作模式
    #define WM_W		120
    #define CWM_X		WM_X+WM_W
    #define CWM_W  		20
    #define TF_FONT     &GUI_Fonthz_24
    
    /* "文件名" */
    inf.base_x = GB_X;//x基坐标 
    inf.base_y = GB_Y;//y基坐标
    
    inf.font[CHINESE] = TF_FONT;//字体
    inf.max_len = 100;//最大长度
    inf.font_color = GUI_BLACK;//字体颜色
    inf.back_color = GUI_INVALID_COLOR;//背景颜色
    inf.align = GUI_TA_RIGHT | GUI_TA_VCENTER;//对齐方式
    inf.pos_size.height = GB_H;//高
    
    inf.pos_size.width = FN_W;//宽
    inf.pos_size.x = FN_X;//x相对坐标
    inf.pos_size.y = 0;//y相对坐标
    
    memcpy(&pool->name->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 文件名 */
    inf.align = GUI_TA_LEFT | GUI_TA_VCENTER;//对齐方式
    inf.pos_size.width = C_FN_W;//宽
    inf.pos_size.x = C_FN_X;//x相对坐标
    
    memcpy(&pool->cur_name->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* "步骤" */
    inf.pos_size.x = STEP_X;//x相对坐标
    inf.pos_size.width = STEP_W;//宽
    
    memcpy(&pool->step->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 步骤 */
    inf.pos_size.x = CS_X;//x相对坐标
    inf.pos_size.width = CS_W;//宽
    
    memcpy(&pool->cur_step->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* "工作模式" */
    inf.pos_size.x = WM_X;//x相对坐标
    inf.pos_size.width = WM_W;//宽
    
    memcpy(&pool->work_mode->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
    
    /* 工作模式 */
    inf.pos_size.x = CWM_X;//x相对坐标
    inf.pos_size.width = CWM_W;//宽
    
    memcpy(&pool->cur_work_mode->dis_info, &inf, sizeof(UI_ELE_DISPLAY_INFO_T));
}
/**
  * @brief  更新窗口中记忆组显示信息
  * @param  [in] win 窗口指针
  * @retval 无
  */
void update_group_inf(MYUSER_WINDOW_T* win)
{
    uint8_t buf[10] = {0};
    STEP_NUM step;
    
    set_group_text_ele_inf(COM_UI_CUR_FILE_NAME, win, g_cur_file->name);
    step = g_cur_step->one_step.com.step;
    sprintf((char*)buf, "%d/%d", step, g_cur_file->total);
    set_group_text_ele_inf(COM_UI_CUR_STEP, win, buf);
    strncpy((char*)buf, (const char*)work_mode_pool[g_cur_file->work_mode], 2);
    set_group_text_ele_inf(COM_UI_CUR_WORK_MODE, win, buf);
    
    update_com_text_ele((CS_INDEX)COM_UI_CUR_FILE_NAME, win, NULL);
    update_com_text_ele((CS_INDEX)COM_UI_CUR_STEP, win, NULL);
    update_com_text_ele((CS_INDEX)COM_UI_CUR_WORK_MODE, win, NULL);
}
/**************************************************************/
/**
  * @brief  设置全局自定义消息的ID
  * @param  [in] id 自定义消息ID
  * @retval 无
  */
void set_custom_msg_id(CUSTOM_MSG_ID id)
{
    g_custom_msg.id = id;
}

/**
  * @brief  更新窗口菜单键显示
  * @param  [in] win 窗口信息
  * @retval 无
  */
void update_win_menu_key(MYUSER_WINDOW_T* win)
{
    if(NULL != win && NULL != win->init_key_fun)
    {
        win->init_key_fun(win->handle);
    }
}
/**
  * @brief  发送带参消息给父窗口
  * @param  [in] hwin 子窗口句柄
  * @param  [in] id EMWIN消息ID
  * @param  [in] val 消息参数
  * @retval 无
  */
void send_msg_to_parent(WM_HWIN hwin, int id, int val)
{
    CPU_SR_ALLOC();
    
	WM_HWIN hParent;
	WM_MESSAGE Msg;
	
	Msg.MsgId = id;
	Msg.Data.v = val;
	hParent = WM_GetParent(hwin);//获取子窗口的父窗口句柄
    
	if(hParent != 0)
    {
        CPU_CRITICAL_ENTER();
		WM_SendMessage(hParent, &Msg);
        CPU_CRITICAL_EXIT();
    }
}
/**
  * @brief  发送无参消息给父窗口
  * @param  [in] hwin 子窗口句柄
  * @param  [in] id EMWIN消息ID
  * @retval 无
  */
void send_no_par_msg_to_parent(WM_HWIN hwin, int id)
{
	WM_HWIN hParent;
	
	hParent = WM_GetParent(hwin);//获取编辑控件的父窗口句柄
	if(hParent != 0)
		WM_SendMessageNoPara(hParent, id);
}
/*********************************************************************/
/**
  * @brief  全局的功能键处理函数指针
  */
void (*global_fun_key_dispose)(uint32_t key_value);
/**
  * @brief  设置全局的功能键处理函数指针
  * @param  [in] fun 功能键处理函数的指针
  * @retval 无
  */
void set_global_fun_key_dispose(void (*fun)(uint32_t))
{
	global_fun_key_dispose = fun;
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
