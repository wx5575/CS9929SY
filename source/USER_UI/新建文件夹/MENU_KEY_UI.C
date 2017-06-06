/**
  ******************************************************************************
  * @file    MENU_KEY_UI.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   菜单键界面
  ******************************************************************************
  */

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

static MENU_KEY_EN_ST 	_menu_key_en_st;
static void set_menu_key_dis_info(const _MENU_KEY_INFO_T_ *info);
static void update_menu_key_text(void);
/** 通用的菜单键名称 使用枚举进行索引 */
static const MENU_KEY_INFO_T all_menu_key_info_[]=
{
	{{"确定", "Ok"		}, F_KEY_OK			},
	{{"取消", "Cancle"	}, F_KEY_CANCLE		},
	{{"下一项", "NEXT"	}, F_KEY_NEXT		},
	{{"删除", "Delete"	}, F_KEY_DEL		},
	{{"退格", "BackSp."	}, F_KEY_BACKSPACE	},
	{{"开头", "Home"	}, F_KEY_HOME		},
	{{"结尾", "End"		}, F_KEY_END		},
	{{"选定", "SELECTED"}, F_KEY_SELECTED	},
	{{"选择", "SELECT"	}, F_KEY_SELECT		},
	
	{{"文件", "FILE"	}, F_KEY_FILE		},
	{{"步骤", "STEP"	}, F_KEY_STEP		},
	{{"系统", "SYSTEM"	}, F_KEY_SYS		},
	{{"结果", "RESULT"	}, F_KEY_RESULT		},
	{{"更多", "MORE"	}, F_KEY_MORE		},
	{{"F6"	,	"F6"	}, F_KEY_F6			},
	{{""	,	""		}, F_KEY_NULL		},
	
	{{"存贮", "Save"	}, F_KEY_SAVE		},
	{{"新建", "New"		}, F_KEY_NEW		},
	{{"读取", "Read"	}, F_KEY_READ		},
	{{"编辑", "Edit"	}, F_KEY_EDIT		},
	{{"返回", "Back"	}, F_KEY_BACK		},
	{{"显示", "Dis."	}, F_KEY_DISPLAY	},
	{{"测试", "Test"	}, F_KEY_TEST   	},
	{{"帮助", "Help"	}, F_KEY_HELP   	},
	{{"电压", "Voltage"	}, F_KEY_VOL    	},
	{{"电流", "Current"	}, F_KEY_CUR   	    },
	{{"档位", "Range"	}, F_KEY_RANGE   	},
	{{"上限", "Upper"	}, F_KEY_UPPER   	},
	{{"下限", "Lower"	}, F_KEY_LOWER   	},
	{{"时间", "Time"	}, F_KEY_TIME   	},
	{{"模式", "Mode"	}, F_KEY_MODE   	},
};

/** 菜单键要响应的键值数组 */
uint32_t g_menu_key_value[]={
	KEY_F0 & _KEY_UP,
	KEY_F1 & _KEY_UP,
	KEY_F2 & _KEY_UP,
	KEY_F3 & _KEY_UP,
	KEY_F4 & _KEY_UP,
	KEY_F5 & _KEY_UP,
	KEY_F6 & _KEY_UP,
};

void set_menu_key_struct_info(_MENU_KEY_INFO_T_ * info, int data, int size)
{
	int32_t i = 0;
	
	for(i = 0; i < size; i++)
	{
		info[i].user_data = data;
	}
}
uint8_t affirm_menu_key_index(_MENU_KEY_INFO_T_ * info)
{
    int32_t i = 0;
    
    for(i = 0; i < 7; i++)
    {
        if(info->key_value == g_menu_key_value[i])
        {
            return i;
        }
    }
    
    return 0xff;
}
void init_one_menu_key_info(_MENU_KEY_INFO_T_ *info)
{
	uint8_t i = 0;
    KEY_DISPOSE_FUN fun;
    
    if(info == NULL)
    {
        return;
    }
    
    i = affirm_menu_key_index(info);
    
    if(i == 0xff)
    {
        return;
    }
    
    fun.fun = info->key_up_dispose_fun;
    fun.user_data = info->user_data;
    fun.en = info->en;
    
    _menu_key_en_st.st[i] = info->en;
    
    if(info->en)
    {
        register_key_dispose_fun(info->key_value, NULL);
    }
    else
    {
        register_key_dispose_fun(info->key_value, &fun);
    }
    
    ///定制按键不在这里进行赋值
    if(info->index != F_KEY_CUSTOM)
    {
        info->name = get_com_menu_key_name(info->index);
    }
    
    ///设置菜单键显示信息
    set_menu_key_dis_info(info);
	
	update_menu_key_text();
}
void init_menu_key_info(_MENU_KEY_INFO_T_ * info)
{
	int32_t i = 0;
	uint32_t n = 7;
    KEY_DISPOSE_FUN fun;
    
	for(i = 0; i < n; i++)
	{
		_menu_key_en_st.st[i] = info[i].en;
		info[i].key_value = g_menu_key_value[i];
        fun.fun = info[i].key_up_dispose_fun;
        fun.user_data = info[i].user_data;
        fun.en = info[i].en;
        
		if(info[i].en)
		{
			register_key_dispose_fun(info[i].key_value, 0);
		}
		else
		{
			register_key_dispose_fun(info[i].key_value, &fun);
		}
		
		///定制按键不在这里进行赋值
		if(info[i].index != F_KEY_CUSTOM)
		{
			info[i].name = get_com_menu_key_name(info[i].index);
		}
		
		///设置菜单键显示信息
		set_menu_key_dis_info(&info[i]);
	}
	
	update_menu_key_text();
}

enum{
ID_FRAMEWIN_0 = (GUI_ID_USER + 0x00),
ID_TITLE_TEXT,
ID_TEXT_F0,
ID_TEXT_F1,
ID_TEXT_F2,
ID_TEXT_F3,
ID_TEXT_F4,
ID_TEXT_F5,
ID_TEXT_F6,
};


#define MENU_KEY_WIN_BASE_X		690
#define MENU_KEY_WIN_BASE_Y		0 //(6 * 10)
#define MENU_KEY_WIN_WIDTH		110
#define MENU_KEY_WIN_HEIGHT		480 // (480 - MENU_KEY_AREA_BASE_Y - 28 - 30 - 25-5)

#define MENU_KEY_NUM			7 /* 按键个数 */
#define MENU_KEY_O				3
#define MENU_KEY_AREA_BASE_X	0 //(CHILD_WIN_BASE_X + 690)
#define MENU_KEY_AREA_BASE_Y	0 //(6 * 10 - CHILD_WIN_BASE_Y) //使用6的倍数
#define MENU_KEY_AREA_WIDTH		MENU_KEY_WIN_WIDTH //110 //(800 - MENU_KEY_AREA_BASE_X)
#define MENU_KEY_AREA_HEIGHT	MENU_KEY_WIN_HEIGHT //(480 - MENU_KEY_AREA_BASE_Y - 28 - CHILD_WIN_BASE_Y)

#define MENU_KEY_BASE_X(N)		(MENU_KEY_AREA_BASE_X)
#define MENU_KEY_BASE_Y(N)		(MENU_KEY_AREA_BASE_Y + (MENU_KEY_HEIGHT + MENU_KEY_O) * (N - 1))
#define MENU_KEY_WIDTH			(MENU_KEY_AREA_WIDTH - 2 * MENU_KEY_O)
#define MENU_KEY_HEIGHT			((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))

#define MENU_KEY_BASE_POS(N)	MENU_KEY_BASE_X(N), MENU_KEY_BASE_Y(N)

static WIDGET_POS_INFO widget_pos_info[]=
{
{ID_TEXT_F0 ,  MENU_KEY_BASE_POS(1), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},

{ID_TEXT_F1	,  MENU_KEY_BASE_POS(2), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
{ID_TEXT_F2	,  MENU_KEY_BASE_POS(3), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
{ID_TEXT_F3	,  MENU_KEY_BASE_POS(4), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
{ID_TEXT_F4	,  MENU_KEY_BASE_POS(5), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
{ID_TEXT_F5	,  MENU_KEY_BASE_POS(6), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
{ID_TEXT_F6	,  MENU_KEY_BASE_POS(7), MENU_KEY_O, MENU_KEY_O, MENU_KEY_WIDTH  , MENU_KEY_HEIGHT},
};

#define KEY_DOWN_COLOR		GUI_DARKBLUE //GUI_DARKGRAY//
#define KEY_UP_COLOR		0x804000 //GUI_LIGHTGRAY//GUI_LIGHTBLUE //
#define KEY_EN_COLOR		KEY_UP_COLOR
#define KYE_EN_FONT_COLOR	GUI_WHITE
#define KEY_DIS_COLOR		KEY_UP_COLOR
#define KYE_DIS_FONT_COLOR	GUI_GRAY
#define MENU_KEY_WIDGET_ALIGN	GUI_TA_HCENTER | GUI_TA_VCENTER
static _USER_WIDGET_INFO user_widget_info[]=
{
	{{0}, ID_TEXT_F0, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F1, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F2, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F3, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F4, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F5, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
	{{0}, ID_TEXT_F6, &GUI_Fonthz_29, GUI_WHITE, KEY_UP_COLOR, MENU_KEY_WIDGET_ALIGN},
};

void change_color_key_down(WM_HWIN handle) 
{
	TEXT_SetBkColor(handle, KEY_DOWN_COLOR);
}
void change_color_key_up(WM_HWIN handle) 
{
	TEXT_SetBkColor(handle, KEY_UP_COLOR);
}

static MENU_KEY_INFO menu_key_info_pool[]=
{
	{KEY_F0 & _KEY_UP  , 0, ID_TEXT_F0, change_color_key_up,},
	{KEY_F1 & _KEY_UP  , 0, ID_TEXT_F1, change_color_key_up,},
	{KEY_F2 & _KEY_UP  , 0, ID_TEXT_F2, change_color_key_up},
	{KEY_F3 & _KEY_UP  , 0, ID_TEXT_F3, change_color_key_up},
	{KEY_F4 & _KEY_UP  , 0, ID_TEXT_F4, change_color_key_up},
	{KEY_F5 & _KEY_UP  , 0, ID_TEXT_F5, change_color_key_up},
	{KEY_F6 & _KEY_UP  , 0, ID_TEXT_F6, change_color_key_up,},
	{KEY_F0 & _KEY_DOWN, 0, ID_TEXT_F0, change_color_key_down},
	{KEY_F1 & _KEY_DOWN, 0, ID_TEXT_F1, change_color_key_down},
	{KEY_F2 & _KEY_DOWN, 0, ID_TEXT_F2, change_color_key_down},
	{KEY_F3 & _KEY_DOWN, 0, ID_TEXT_F3, change_color_key_down},
	{KEY_F4 & _KEY_DOWN, 0, ID_TEXT_F4, change_color_key_down},
	{KEY_F5 & _KEY_DOWN, 0, ID_TEXT_F5, change_color_key_down},
	{KEY_F6 & _KEY_DOWN, 0, ID_TEXT_F6, change_color_key_down},
};

static void update_menu_key_text(void)
{
	uint32_t i = 0;
	
	for(i = 0; i < MENU_KEY_NUM; i++)
	{
		if(_menu_key_en_st.st[i] == MENU_KEY_DIS)
		{
			TEXT_SetBkColor(user_widget_info[i].handle, KEY_DIS_COLOR);
			TEXT_SetTextColor(user_widget_info[i].handle, KYE_DIS_FONT_COLOR);
		}
		else
		{
			TEXT_SetBkColor(user_widget_info[i].handle, KEY_EN_COLOR);
			TEXT_SetTextColor(user_widget_info[i].handle, KYE_EN_FONT_COLOR);
		}
		
		TEXT_SetText(user_widget_info[i].handle, (const char *)user_widget_info[i].text[SYS_LANGUAGE]);
	}
}

static void set_menu_key_dis_info(const _MENU_KEY_INFO_T_ *info)
{
	int32_t i = 0;
	
	for(i = 0; i < MENU_KEY_NUM; i++)
	{
		if(g_menu_key_value[i] == info->key_value)
		{
			break;
		}
	}
	
	if(i == MENU_KEY_NUM)
	{
		return;
	}
	
	user_widget_info[i].text[0] = info->name;
	user_widget_info[i].text[1] = info->name;
}

/**
* get_com_menu_key_name
*@Description-获取菜单功能键的名称
*@param [in] 菜单键索引
*@return 菜单键名称 没有找到索引按键就返回空字符串
*/
uint8_t *get_com_menu_key_name(uint32_t index)
{
	int32_t i = 0;
	uint32_t size = COUNT_ARRAY_SIZE(all_menu_key_info_);
	
	/* 查找索引的按键 */
	for(i = 0; i < size; i++)
	{
		if(all_menu_key_info_[i].index == index)
		{
			break;
		}
	}
	
	/* 没找到返回空字符串 */
	if(i == size)
	{
		return "";
	}
	
	return all_menu_key_info_[i].text[SYS_LANGUAGE];
}

static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_ClearRectEx(&r);
}
static void display_text_widget(WM_HWIN hwin)
{
	int32_t i = 0;
	uint32_t num = sizeof(user_widget_info) / sizeof(user_widget_info[0]);
	uint32_t pos_pool_size = sizeof(widget_pos_info) / sizeof(widget_pos_info[0]);
	
	init_widget_pos_info(user_widget_info, num, widget_pos_info, pos_pool_size);/* 初始化控件位置信息 */
	
	for(i = 0; i < num; i++)
	{
		init_user_widget_info(&user_widget_info[i], hwin);
	}
}

static void _cbLanguage0(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
		{
			uint32_t widget_pool_size = COUNT_ARRY_SIZE(user_widget_info);
			uint32_t n = COUNT_ARRY_SIZE(menu_key_info_pool);
			/* 设置聚焦 */
			WM_SetFocus(hWin);
			display_text_widget(hWin);/* 显示文本控件 */
			init_key_widget_info(user_widget_info, widget_pool_size, menu_key_info_pool, n);
			break;
		}
		case WM_TIMER:
			break;
		 case WM_KEY:
            break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


static void test_ui_menu_key_dispose(uint32_t key_value)
{
	int i = 0;
	uint32_t n = sizeof(menu_key_info_pool) / sizeof(menu_key_info_pool[0]);
	
	for(i = 0; i < n; i++)
	{
		if(menu_key_info_pool[i].key_value == key_value)
		{
			if(NULL != menu_key_info_pool[i].fun)
			{
				if(_menu_key_en_st.st[i % 6] == MENU_KEY_EN)
				{
					menu_key_info_pool[i].fun(menu_key_info_pool[i].handle);
					WM_Exec();
				}
			}
			break;
		}
	}
}

void _create_menu_key_win(void)
{
	int x = MENU_KEY_WIN_BASE_X;
	int y = MENU_KEY_WIN_BASE_Y;
	
	WM_CreateWindowAsChild(x, y, MENU_KEY_WIN_WIDTH, MENU_KEY_WIN_HEIGHT, WM_HBKWIN, WM_CF_MEMDEV_ON_REDRAW | WM_CF_SHOW, _cbLanguage0, 0);//WM_CF_SHOW
	set_global_fun_key_dispose(test_ui_menu_key_dispose);
}
