/**
  ******************************************************************************
  * @file    key_menu_win.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "keyboard.h"
#include "rtc_config.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "fonts.h"
#include "ff.h"
#include "OS.H"
#include "UI_COM/com_ui_info.h"
#include "7_key_menu_win.h"
#include "string.h"
#include "key_menu_win.h"
#include "scan_keyboard.h"

#define MENU_KEY_WIN_BASE_X		690
#define MENU_KEY_WIN_BASE_Y		0
#define MENU_KEY_WIN_WIDTH		110
#define MENU_KEY_WIN_HEIGHT		480 // (480 - MENU_KEY_AREA_BASE_Y - 28 - 30 - 25-5)


#define MENU_KEY_O				3

#define MENU_KEY_AREA_WIDTH		MENU_KEY_WIN_WIDTH //110 //(800 - MENU_KEY_AREA_BASE_X)
#define MENU_KEY_AREA_HEIGHT	MENU_KEY_WIN_HEIGHT //(480 - MENU_KEY_AREA_BASE_Y - 28 - CHILD_WIN_BASE_Y)

#define MENU_KEY_HEIGHT			((MENU_KEY_AREA_HEIGHT - (MENU_KEY_NUM + 1) * MENU_KEY_O) / (MENU_KEY_NUM))

#define KEY_DOWN_COLOR		    GUI_DARKBLUE //GUI_DARKGRAY//
#define KEY_UP_COLOR		    0x804000 //GUI_LIGHTGRAY//GUI_LIGHTBLUE //
#define KEY_EN_BACK_COLOR		KEY_UP_COLOR
#define KEY_EN_FONT_COLOR	    GUI_WHITE
#define KEY_DIS_BACK_COLOR		KEY_UP_COLOR
#define KEY_DIS_FONT_COLOR	    GUI_GRAY

/**
* @brief  窗口的位置尺寸信息数组，根据不同的屏幕进行设置
  */
static WIDGET_POS_SIZE_T* key_menu_win_pos_size[SCREEN_NUM]=
{
    &_7_key_menu_windows,/*4.3寸屏*/
    &_7_key_menu_windows,/*5.6寸屏*/
    &_7_key_menu_windows,/*7寸屏*/
};

/**
  * @brief  标准菜单键信息定义 使用枚举进行索引
  */
static const STAND_MENU_KEY_INFO_T all_menu_key_info_[]=
{
	{{"确定", "Ok"		}, F_KEY_OK			},
	{{"取消", "Cancle"	}, F_KEY_CANCLE		},
	{{"下一项", "Next"	}, F_KEY_NEXT		},
	{{"删除", "Delete"	}, F_KEY_DEL		},
	{{"退格", "BackSp."	}, F_KEY_BACKSPACE	},
	{{"开头", "Home"	}, F_KEY_HOME		},
	{{"结尾", "End"		}, F_KEY_END		},
	{{"选定", "SELECTED"}, F_KEY_SELECTED	},
	{{"选择", "SELECT"	}, F_KEY_SELECT		},
	
	{{"文件", "File"	}, F_KEY_FILE		},
	{{"步骤", "Step"	}, F_KEY_STEP		},
	{{"系统", "System"	}, F_KEY_SYS		},
	{{"结果", "Result"	}, F_KEY_RESULT		},
	{{"更多", "More"	}, F_KEY_MORE		},
	{{"F0"	,	"F0"	}, F_KEY_F0			},
	{{"F1"	,	"F1"	}, F_KEY_F1			},
	{{"F2"	,	"F2"	}, F_KEY_F2			},
	{{"F3"	,	"F3"	}, F_KEY_F3			},
	{{"F4"	,	"F4"	}, F_KEY_F4			},
	{{"F5"	,	"F5"	}, F_KEY_F5			},
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
	{{"清空", "Clear"	}, F_KEY_CLEAR   	},
	{{"详细", "Detail"	}, F_KEY_DETAIL   	},
	{{"插入", "Inster"	}, F_KEY_INSTER   	},
	{{"前移", "Forward"	}, F_KEY_FORWARD   	},
	{{"后移", "Backward"}, F_KEY_BACKWARD   },
};
/**
  * @brief  菜单键信息信息结构 是一个内部数据类型
  */
typedef struct {
	int down_key_val;/* 按下键值 */
	int up_key_val;/* 抬起键值 */
	void (*down_fun)();/* 按下处理函数 */
	void (*up_fun)();/* 抬起处理函数 */
}MENU_KEY_INFO;

static void change_color_key_up(WM_HWIN handle);
static void change_color_key_down(WM_HWIN handle);
/**
  * @brief  菜单键信息信息结构初始化数组 默认的按键按下抬起处理函数是改变菜单键的颜色
  */
static MENU_KEY_INFO menu_key_info_pool[]=
{
	{KEY_F0 & _KEY_DOWN  , KEY_F0 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F1 & _KEY_DOWN  , KEY_F1 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F2 & _KEY_DOWN  , KEY_F2 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F3 & _KEY_DOWN  , KEY_F3 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F4 & _KEY_DOWN  , KEY_F4 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F5 & _KEY_DOWN  , KEY_F5 & _KEY_UP  , change_color_key_down, change_color_key_up,},
	{KEY_F6 & _KEY_DOWN  , KEY_F6 & _KEY_UP  , change_color_key_down, change_color_key_up,},
};




static void key_menu_cb(WM_MESSAGE* pMsg);
static void scan_menu_key_dispose(uint32_t key_value);

extern UI_ELE_DISPLAY_INFO_T key_ui_ele_dis_info[];

/**
  * @brief  菜单键窗口文本控件表，在这个表中的控件才会在窗口中显示
  */
static CS_INDEX key_ui_ele_table[] =
{
    KEY_MENU_F0,
    KEY_MENU_F1,
    KEY_MENU_F2,
    KEY_MENU_F3,
    KEY_MENU_F4,
    KEY_MENU_F5,
    KEY_MENU_F6,
};
/**
  * @brief  菜单键窗口文本控件结构初始化池
  */
TEXT_ELE_T key_ui_ele_pool[]=
{
	{{"F0","F0"}, KEY_MENU_F0   },
	{{"F1","F1"}, KEY_MENU_F1   },
	{{"F2","F2"}, KEY_MENU_F2   },
	{{"F3","F3"}, KEY_MENU_F3   },
	{{"F4","F4"}, KEY_MENU_F4   },
	{{"F5","F5"}, KEY_MENU_F5   },
	{{"F6","F6"}, KEY_MENU_F6   },
};
static void init_create_key_menu_text_ele(MYUSER_WINDOW_T* win);
/**
  * @brief  菜单键窗口定义
  */
static MYUSER_WINDOW_T key_menu_windows=
{
    {"菜单键窗口","KeyMenuWindow"},
    key_menu_cb,NULL,
	{
        key_ui_ele_pool,COUNT_ARRAY_SIZE(key_ui_ele_pool),
        (CS_INDEX*)key_ui_ele_table, ARRAY_SIZE(key_ui_ele_table),
        init_create_key_menu_text_ele
    },
};

TEXT_ELE_AUTO_LAYOUT_T  *key_menu_text_ele_auto_layout[]=
{
    &_7_key_menu_auto_layout_inf,//4.3寸屏
    &_7_key_menu_auto_layout_inf,//5.6寸屏
    &_7_key_menu_auto_layout_inf,//7寸屏
};
static void init_create_key_menu_text_ele(MYUSER_WINDOW_T* win)
{
    init_window_text_ele_list(win);
    init_window_text_ele_dis_inf(win, key_menu_text_ele_auto_layout[sys_par.screem_size]);
    init_window_text_ele(win);
}
/**
  * @brief  设置菜单键窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_key_windows_handle(WM_HWIN hWin)
{
	key_menu_windows.handle = hWin;
}

/**
  * @brief  更新菜单按键名称
  * @param  无
  * @retval 无
  */
void display_menu_key(void)
{
	CS_LIST* t_node = NULL;
	CS_LIST* list = &key_menu_windows.text.list_head;
	TEXT_ELE_T *node = NULL;
	
	list_for_each(t_node, list)
	{
		node = list_entry(t_node, TEXT_ELE_T, list );
        update_text_ele((CS_INDEX)node->index, &key_menu_windows, (const uint8_t*)node->text[SYS_LANGUAGE]);
	}
}

/**
  * @brief  获取菜单功能键的名称
  * @param  [in] index 菜单键索引
  * @retval 标准菜单键名称
  */
void get_stand_menu_key_name(uint32_t index, uint8_t *name[], CS_ERR *err)
{
	int32_t i = 0;
	uint32_t size = COUNT_ARRAY_SIZE(all_menu_key_info_);
	
    *err = CS_ERR_NONE;
    
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
        *err = CS_ERR_INDEX_INVALID;
		return;
	}
    
	name[CHINESE] = all_menu_key_info_[i].text[CHINESE];
	name[ENGLISH] = all_menu_key_info_[i].text[ENGLISH];
}

/**
  * @brief  根据键值来获取菜单键索引
  * @param  [in] key_value 键值
  * @param  [out] err CS_ERR_NONE 成功获取索引值; CS_ERR_KEY_VALUE_INVALID 键值无效
  * @retval 菜单键索引
  */
static CS_INDEX get_key_inf_index(uint32_t key_value, CS_ERR *err)
{
    int32_t i = 0;
    
    for(i = 0; i < 7; i++)
    {
        if(key_value == menu_key_info_pool[i].down_key_val
            || key_value == menu_key_info_pool[i].up_key_val)
        {
            *err = CS_ERR_NONE;
            return (CS_INDEX)i;
        }
    }
    
    *err = CS_ERR_KEY_VALUE_INVALID;
    
    return (CS_INDEX)0;
}
/**
  * @brief  注销系统功能键
  * @param  [in] info 系统功能键数组地址
  * @param  [in] n 系统功能键数组中元素个数
  * @retval 菜单键索引
  */
void unregister_system_key_fun(FUNCTION_KEY_INFO_T info[], uint32_t n)
{
	int32_t i = 0;
    KEY_DISPOSE_FUN fun;
	
	for(i = 0; i < n; i++)
	{
        fun.fun = NULL;
        fun.msg.user_data = 0;
        fun.en = MENU_KEY_DIS;
        
        register_key_dispose_fun(info[i].key_value, &fun);
	}
}
/**
  * @brief  注册系统功能键
  * @param  [in] info 系统功能键数组地址
  * @param  [in] n 系统功能键数组中元素个数
  * @retval 菜单键索引
  */
void register_system_key_fun(FUNCTION_KEY_INFO_T info[], uint32_t n, int data)
{
	int32_t i = 0;
    KEY_DISPOSE_FUN fun;
	
	for(i = 0; i < n; i++)
	{
        info[i].msg.user_data = data;
        
        fun.fun = info[i].key_up_dispose_fun;
        fun.msg.user_data = info[i].msg.user_data;
        fun.en = info[i].en;
        
        register_key_dispose_fun(info[i].key_value, &fun);
	}
}

static void update_menu_key_enable_color(uint32_t key_value, MENU_KEY_ST_ENUM st)
{
    CS_INDEX index;
    CS_ERR err;
    
    index = get_key_inf_index(key_value, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    //按键失能
    if(st == MENU_KEY_DIS)
    {
        key_ui_ele_pool[index].dis_info.back_color = KEY_DIS_BACK_COLOR;
        key_ui_ele_pool[index].dis_info.font_color = KEY_DIS_FONT_COLOR;
    }
    //按键使能
    else
    {
        key_ui_ele_pool[index].dis_info.back_color = KEY_EN_BACK_COLOR;
        key_ui_ele_pool[index].dis_info.font_color = KEY_EN_FONT_COLOR;
    }
}
/**
  * @brief  设置菜单功能键的状态
  * @param  [in] key_value 要设置的按键键值
  * @param  [in] st 按键状态
  * @retval 菜单键索引
  */
void set_menu_function_status(uint32_t key_value, MENU_KEY_ST_ENUM st)
{
    enable_function_key(key_value, st);
    update_menu_key_enable_color(key_value, st);
}
/**
  * @brief  初始化菜单键
  * @param  [in] info 初始化信息数组
  * @param  [in] n 菜单键信息数组中的元素个数
  * @param  [in] data 用户数据
  * @retval 无
  */
void init_menu_key_info(MENU_KEY_INFO_T * info, uint32_t n, int data)
{
    int32_t i = 0;
    KEY_DISPOSE_FUN fun;
    CS_INDEX index;
    uint8_t *name[2];
    CS_ERR err;
    
	for(i = 0; i < n; i++)
	{
        index = get_key_inf_index(info[i].fun_key.key_value, &err);
        
        if(err != CS_ERR_NONE)
        {
            continue;
        }
        
        info[i].fun_key.msg.user_data = data;
        fun.fun = info[i].fun_key.key_up_dispose_fun;
        memcpy(&fun.msg, &info[i].fun_key.msg, sizeof(fun.msg));
        fun.en = info[i].fun_key.en;
        
        set_menu_function_status(info[i].fun_key.key_value, info[i].fun_key.en);
        register_key_dispose_fun(info[i].fun_key.key_value, &fun);
        
		///定制按键名称设置
		if(info[i].index == F_KEY_CUSTOM)
		{
            key_ui_ele_pool[index].text[CHINESE] = info[i].name;
            key_ui_ele_pool[index].text[ENGLISH] = info[i].name;
		}
        //校准按键名称设置
        else
        {
            get_stand_menu_key_name(info[i].index, name, &err);
            
            if(err == CS_ERR_NONE)
            {
                info[i].name = name[CHINESE];
                key_ui_ele_pool[index].text[CHINESE] = name[CHINESE];
                key_ui_ele_pool[index].text[ENGLISH] = name[ENGLISH];
            }
        }
	}
    
	display_menu_key();//刷新菜单键显示
}
/**
  * @brief  重绘界面背景
  * @param  无
  * @retval 无
  */
static void _PaintFrame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_ClearRectEx(&r);
}
/**
  * @brief  菜单键界面回调函数
  * @param  [in] pMsg 窗口消息
  * @retval 无
  */
static void key_menu_cb(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	MYUSER_WINDOW_T* win;
	
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
		{
            set_key_windows_handle(hWin);
            WM_SetStayOnTop(hWin, 1);//窗口保持在顶部
			win = get_user_window_info(hWin);
			
            init_create_win_all_ele(win);
//			if(win != NULL)
//			{
//                init_key_menu_ui_text_ele_pos_inf();//初始化文本对象的位置信息
//                init_window_text_ele_list(win);//初始化窗口文本对象链表
//				init_window_text_ele(win);
//			}
            
            set_global_fun_key_dispose(scan_menu_key_dispose);
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

/**
  * @brief  菜单键按下时改变控件的背景色
  * @param  [in] handle 按下的菜单键句柄
  * @retval 无
  */
static void change_color_key_down(WM_HWIN handle)
{
	TEXT_SetBkColor(handle, KEY_DOWN_COLOR);
}
/**
  * @brief  菜单键抬起时改变控件的背景色
  * @param  [in] handle 抬起的菜单键句柄
  * @retval 无
  */
static void change_color_key_up(WM_HWIN handle) 
{
	TEXT_SetBkColor(handle, KEY_UP_COLOR);
}
/**
  * @brief  菜单键抬起时改变控件的背景色
  * @param  [in] handle 抬起的菜单键句柄
  * @retval 无
  */
uint8_t get_menu_key_index_and_fun(uint32_t key_value, void(**fun)(), CS_ERR *err)
{
    int32_t i = 0;
    uint8_t size = ARRAY_SIZE(menu_key_info_pool);
    
    *err = CS_ERR_NONE;
    
    for(i = 0; i < size; i++)
    {
        if(menu_key_info_pool[i].down_key_val == key_value)
        {
            if(NULL != menu_key_info_pool[i].down_fun)
            {
                *fun = menu_key_info_pool[i].down_fun;
            }
            break;
        }
        else if(menu_key_info_pool[i].up_key_val == key_value)
        {
            if(NULL != menu_key_info_pool[i].up_fun)
            {
                *fun = menu_key_info_pool[i].up_fun;
            }
            break;
        }
    }
    
    if(i < size)
    {
        return i;
    }
    
    *err = CS_ERR_KEY_VALUE_INVALID;
    
    return 0xff;
}
/**
  * @brief  创建菜单按键窗口
  * @param  无
  * @retval 无
  */
static void scan_menu_key_dispose(uint32_t key_value)
{
    TEXT_ELE_T* node = NULL;
    uint8_t st = 0xff;
    CS_INDEX index = 0xff;
    void (*fun)();
    CS_ERR err;
    
    fun = NULL;
    
    index = get_menu_key_index_and_fun(key_value, &fun, &err);//根据按键值获取对应的菜单键信息
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
	
    st = get_function_key_st(key_value, &err);//获取菜单按键的使能状态
    
    if(CS_ERR_NONE != err || st == MENU_KEY_DIS)
    {
        return;
    }
    
    node = get_text_ele_node((CS_INDEX)index, &key_menu_windows.text.list_head, &err);
    
    if(node != NULL && fun != NULL && err == CS_ERR_NONE)
    {
        fun(node->handle);//执行按键回调函数
        WM_Exec();
    }
}
/**
  * @brief  创建菜单按键窗口
  * @param  无
  * @retval 无
  */
void create_key_menu_window(void)
{
	CS_LIST *list = &windows_list;//窗口链表
    
    init_window_size(&key_menu_windows, key_menu_win_pos_size[sys_par.screem_size]);
    
    create_user_window(&key_menu_windows, list, 0);//创建菜单界面
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
