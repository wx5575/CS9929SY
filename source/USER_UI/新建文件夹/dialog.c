
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "malloc.h"
#include "ff.h"
#include "OS.H"
#include "ui_com_info.h"
#include "app.h"
#include "cs99xx_struct.h"

#include "dialog.h"
#include "dialog_info.h"
#include "FRAMEWIN.H"
#include "EDIT.H"
#include "DROPDOWN.H"


enum{
	ID_FRAMEWIN_FILE_SAVE = (GUI_ID_USER + 0x00),
	TEXT_ID123,
};

void init_dialog_elements(WIDGET_ELEMENT *ele, WM_HWIN hWin)
{
	WM_HWIN hwidget;
	uint8_t index = 0;
	
	hwidget = TEXT_CreateEx(ele->dis_info->x, ele->dis_info->y,
							ele->dis_info->width[index], ele->dis_info->height[index],
							hWin, WM_CF_SHOW,
							ele->dis_info->align[index],
							ele->id[index],
							(const char*)ele->name[SYS_LANGUAGE]);
	ele->handle[index] = hwidget;
	TEXT_SetFont(hwidget, ele->dis_info->pFont[index]);
	TEXT_SetBkColor(hwidget, ele->dis_info->bk_color[index]);
	TEXT_SetTextColor(hwidget, ele->dis_info->font_color[index]);
	
	index = 1;
	hwidget = TEXT_CreateEx(ele->dis_info->x + ele->dis_info->width[0], ele->dis_info->y,
							ele->dis_info->width[index], ele->dis_info->height[index],
							hWin, WM_CF_SHOW,
							ele->dis_info->align[index],
							ele->id[index],
							(const char*)ele->value[0]);
	ele->handle[index] = hwidget;
	
	if(ele->data != NULL)
	switch(ele->type)
	{
		case ELE_TEXT:/* 文本 */
			break;
		case ELE_EDIT_STR:/* 字符串编辑框 */
			TEXT_SetText(hwidget, (const char*)ele->data);
			break;
		case ELE_EDIT_NUM:/* 数字编辑框 */
			break;
		case ELE_DROPDOWN:/* 下拉选择框 */
			TEXT_SetText(hwidget, ((const char**)ele->resource)[*(uint8_t*)ele->data]);
			break;
	}
	TEXT_SetFont(hwidget, ele->dis_info->pFont[index]);
	TEXT_SetBkColor(hwidget, ele->dis_info->bk_color[index]);
	TEXT_SetTextColor(hwidget, ele->dis_info->font_color[index]);
	
	index = 2;
	hwidget = TEXT_CreateEx(ele->dis_info->x + ele->dis_info->width[0] + ele->dis_info->width[1],
							ele->dis_info->y,
							ele->dis_info->width[index], ele->dis_info->height[index],
							hWin, WM_CF_SHOW,
							ele->dis_info->align[index],
							ele->id[index],
							(const char*)unit_pool[ele->unit]);
	ele->handle[index] = hwidget;
	TEXT_SetFont(hwidget, ele->dis_info->pFont[index]);
	TEXT_SetBkColor(hwidget, ele->dis_info->bk_color[index]);
	TEXT_SetTextColor(hwidget, ele->dis_info->font_color[index]);
}

void init_menu_key_info_dialog(WIDGET_ELEMENT *ele)
{
	init_menu_key_info(ele->p_m_key, 7);
	set_scan_key_custom_fun(ele->fun_key);
}

void add_dialog_elements(WM_HWIN hWin)
{
	WIDGET_ELEMENT *node;
	uint8_t flag = 0;
	
	init_new_file_dialog_ele_list(&file_pool[0]);
	
	for(flag = 0, node = cur_dia_ele_list.head;
		node && (!(flag == 1 && node == cur_dia_ele_list.head));
		node = node->next)
	{
		flag = 1;
		init_dialog_elements(node, hWin);
	}
	
	g_cur_dia_ele = cur_dia_ele_list.head;/* 初始化当前对话框对象 */
	set_scan_key_custom_fun(g_cur_dia_ele->fun_key);
// 	set_direct_key_fun(p_cur_dialog->p_direct_key);
	TEXT_SetBkColor(g_cur_dia_ele->handle[0], SELETED_COLOR);
	TEXT_SetBkColor(g_cur_dia_ele->handle[1], SELETED_COLOR);
	TEXT_SetBkColor(g_cur_dia_ele->handle[2], SELETED_COLOR);
	init_serve_dialog_menu_key(p_cur_dialog);
	
	create_range_text_element(p_cur_dialog, g_cur_dia_ele);
}
static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	
    //FRAMEWIN
    FRAMEWIN_SetFont(hWin, p_cur_dialog->pFont);
    FRAMEWIN_SetTitleHeight(hWin, p_cur_dialog->head_height);
	FRAMEWIN_SetText(hWin, (const char*)p_cur_dialog->name[SYS_LANGUAGE]);
// 	FRAMEWIN_SetBarColor(hWin, 1, GUI_BLUE);
// 	FRAMEWIN_SetBarColor(hWin, 0, GUI_BLUE);
	FRAMEWIN_SetClientColor(hWin, GUI_LIGHTGRAY);
	
	add_dialog_elements(hWin);
}

static void send_msg_to_parent(WM_HWIN hwin, int id, int val)
{
	WM_HWIN hParent;
	WM_MESSAGE Msg;
	
	Msg.MsgId = id;
	Msg.Data.v = val;
	hParent = WM_GetParent(hwin);//获取编辑控件的父窗口句柄
	
	if(hParent != 0)
	{
		WM_SendMessage(hParent, &Msg);
	}
}
/*********************************************************************
*
* Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    
    WM_HWIN hWin = pMsg->hWin;
    
    switch(pMsg->MsgId)
    {
		case CM_DIALOG_RETURN_OK:
		{
			break;
		}
		case WM_CREATE:
			/* 创建后设置聚焦 */
			WM_SetFocus(hWin);
			break;
        case WM_PAINT:
//             PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
			p_cur_dialog->handle = hWin;
            InitDialog(pMsg);
// 			init_menu_key_info(cur_menu_key_info[0], &menu_key_en_st[0]);
// 			set_direct_key_fun(direct_key_pool);
            break;
		case WM_DELETE:
		{
			send_msg_to_parent(hWin, CM_CHILD_W_MSG, (int)&g_custom_msg);
			break;
		}
        case WM_KEY:
        {
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        }
        case WM_NOTIFY_PARENT:
        {
            Id = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            
            switch (Id)
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
					{
						GUI_EndDialog(hWin, 0);
					}
                    break;
                case GUI_ID_CANCEL:
                    if(NCode == WM_NOTIFICATION_RELEASED)
					{
                        GUI_EndDialog(hWin, 0);
					}
                    break;
            }
            break;
        }
        default:
            WM_DefaultProc(pMsg);
    }
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{FRAMEWIN_CreateIndirect, "", 1232, 150, 50, 400, 320, 0, FRAMEWIN_CF_ACTIVE, 4},
	{FRAMEWIN_CreateIndirect, "", 1233, 50 , 0, 600, 450, 0, 0x0, 0 },
	{FRAMEWIN_CreateIndirect, "", 1234, 150, 50, 400, 320, 0, 0x0, 0 },
};

WM_HMEM create_new_dialog_400_320(int p)
{
	return GUI_CreateDialogBox(&_aDialogCreate[0], 1, &_cbCallback, p, 0, 0);//非阻塞
}

WM_HMEM create_new_dialog_600_480(int p)
{
	return GUI_CreateDialogBox(&_aDialogCreate[1], 1, &_cbCallback, p, 0, 0);//非阻塞
}

void create_new_file_dialog(int p)
{
// 	set_direct_key_fun(0);
	p_cur_dialog = &dialog_info_t[DIALOG_NEW_FILE];
	create_new_dialog_400_320(p);
}

