/**
  ******************************************************************************
  * @file    test_ui_com.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "keyboard.h"
#include "UI_COM/com_ui_info.h"
#include "test_win.h"
#include "7_test_ui_layout_1.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void update_menu_key_inf(WM_HMEM hWin);
static void test_win_cb(WM_MESSAGE * pMsg);

static void change_key_menu(int id);
static void test_win_f0_cb(KEY_MESSAGE *key_msg);
static void test_win_f1_cb(KEY_MESSAGE *key_msg);
static void test_win_f2_cb(KEY_MESSAGE *key_msg);
static void test_win_f3_cb(KEY_MESSAGE *key_msg);
static void test_win_f4_cb(KEY_MESSAGE *key_msg);
static void test_win_f5_cb(KEY_MESSAGE *key_msg);
static void test_win_f6_cb(KEY_MESSAGE *key_msg);

static void test_win_f1_1_cb(KEY_MESSAGE *key_msg);
static void test_win_f2_1_cb(KEY_MESSAGE *key_msg);
static void test_win_f3_1_cb(KEY_MESSAGE *key_msg);
static void test_win_f4_1_cb(KEY_MESSAGE *key_msg);

static void test_win_f1_2_cb(KEY_MESSAGE *key_msg);
static void test_win_f2_2_cb(KEY_MESSAGE *key_msg);
static void test_win_f3_2_cb(KEY_MESSAGE *key_msg);
static void test_win_f4_2_cb(KEY_MESSAGE *key_msg);

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  定时器句柄
  */
static	WM_HWIN timer_handle;
/**
  * @brief  测试界面的位置尺寸信息，根据不同屏幕进行初始化
  */
static WIDGET_POS_SIZE_T* test_win_pos_size_pool[SCREEN_NUM]=
{
    &_7_test_windows,/*4.3寸屏*/
    &_7_test_windows,/*5.6寸屏*/
    &_7_test_windows,/*7寸屏*/
};

/**
  * @brief  测试界面下的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_menu_key_pool[]=
{
    {"", F_KEY_DISPLAY	, KEY_F0 & _KEY_UP,	test_win_f0_cb },//f0
    {"", F_KEY_MODE		, KEY_F1 & _KEY_UP,	test_win_f1_cb },//f1
    {"", F_KEY_VOL	    , KEY_F2 & _KEY_UP,	test_win_f2_cb },//f2
    {"", F_KEY_RANGE    , KEY_F3 & _KEY_UP,	test_win_f3_cb },//f3
    {"", F_KEY_TIME	    , KEY_F4 & _KEY_UP,	test_win_f4_cb },//f4
    {"", F_KEY_MORE     , KEY_F5 & _KEY_UP,	test_win_f5_cb },//f5
    {"", F_KEY_BACK		, KEY_F6 & _KEY_UP,	test_win_f6_cb },//f6
};

/**
  * @brief  测试界面下ACW设置参数的菜单按键初始化数组
  */
static MENU_KEY_INFO_T test_ui_acw_menu_pool[][4]=
{
    {
        {"", F_KEY_MODE  , KEY_F1 & _KEY_UP, test_win_f1_1_cb},//f1
        {"", F_KEY_VOL   , KEY_F2 & _KEY_UP, test_win_f2_1_cb},//f2
        {"", F_KEY_RANGE , KEY_F3 & _KEY_UP, test_win_f3_1_cb},//f3
        {"", F_KEY_TIME  , KEY_F4 & _KEY_UP, test_win_f4_1_cb},//f4
    },
    {
        {"", F_KEY_UPPER , KEY_F1 & _KEY_UP, test_win_f1_2_cb},//f1
        {"", F_KEY_LOWER , KEY_F2 & _KEY_UP, test_win_f2_2_cb},//f2
        {"", F_KEY_NULL  , KEY_F3 & _KEY_UP, test_win_f3_2_cb},//f3
        {"", F_KEY_NULL  , KEY_F4 & _KEY_UP, test_win_f4_2_cb},//f4
    },
};


/**
  * @brief  测试界面显示的文本索引表
  */
static CS_INDEX test_ui_ele_buf[] =
{
	TEST_UI_FILE_NAME,
	TEST_UI_CUR_FILE_NAME,
	TEST_UI_STEP,
	TEST_UI_CUR_STEP,
	TEST_UI_WORK_MODE,
	TEST_UI_CUR_WORK_MODE,
	
	TEST_UI_ROAD01_NUM,
	TEST_UI_ROAD01_MODE,
	TEST_UI_ROAD01_STATUS,
	TEST_UI_ROAD01_VOLTAGE,
	TEST_UI_ROAD01_UPPER,
	TEST_UI_ROAD01_REAL,
	TEST_UI_ROAD01_TIME,
	TEST_UI_ROAD02_NUM,
	TEST_UI_ROAD02_MODE,
	TEST_UI_ROAD02_STATUS,
	TEST_UI_ROAD02_VOLTAGE,
	TEST_UI_ROAD02_UPPER,
	TEST_UI_ROAD02_REAL,
	TEST_UI_ROAD02_TIME,
	TEST_UI_ROAD03_NUM,
	TEST_UI_ROAD03_MODE,
	TEST_UI_ROAD03_STATUS,
	TEST_UI_ROAD03_VOLTAGE,
	TEST_UI_ROAD03_UPPER,
	TEST_UI_ROAD03_REAL,
	TEST_UI_ROAD03_TIME,
	TEST_UI_ROAD04_NUM,
	TEST_UI_ROAD04_MODE,
	TEST_UI_ROAD04_STATUS,
	TEST_UI_ROAD04_VOLTAGE,
	TEST_UI_ROAD04_UPPER,
	TEST_UI_ROAD04_REAL,
	TEST_UI_ROAD04_TIME,
};

/**
  * @brief 界面文本对象数组
  */
static TEXT_ELE_T test_ui_ele_pool[]=
{
	{{"文件名:", "FileName:"}, TEST_UI_FILE_NAME    },
	{{"DEFAULT", "DEFAULT"  }, TEST_UI_CUR_FILE_NAME},
	{{"步骤:"  , "Step:"    }, TEST_UI_STEP         },
	{{"01/01"  , "01/01"    }, TEST_UI_CUR_STEP     },
	{{"工作模式:","WorkMode:"}, TEST_UI_WORK_MODE   },
	{{"N"      , "N"        }, TEST_UI_CUR_WORK_MODE},
	
	{{"输出电压:","Voltage:"}, TEST_UI_VOLTAGE      },
	{{"5.000kV" , "5.000kV" }, TEST_UI_CUR_VOLTAGE  },
	{{"电流档位:","CUR.Range:"}, TEST_UI_RANGE      },
	{{"2mA"    , "2mA"      }, TEST_UI_CUR_RANGE    },
	{{"电流上限:","CUR.Upper:"}, TEST_UI_UPPER      },
	{{"2.000mA", "2.000mA"  }, TEST_UI_CUR_UPPER    },
	{{"电流下限:","CUR.Lower:"}, TEST_UI_LOWER      },
	{{"1.000mA", "1.000mA"  }, TEST_UI_CUR_LOWER    },
	{{"电弧侦测:","Arc:"    }, TEST_UI_ARC          },
	{{"10.00mA", "10.00mA"  }, TEST_UI_CUR_ARC      },
	{{"步间连续:","Continue:"}, TEST_UI_CONTINUE    },
	{{"开"     , "ON"       }, TEST_UI_CUR_CONTINUE },
	
	{{"测试:"  ,"Test:"     }, TEST_UI_TEST         },
	{{"ACW"    ,"ACW"       }, TEST_UI_TEST_1       },
	{{"->"     ,"->"        }, TEST_UI_TEST_2       },
	{{"DCW"    ,"DCW"       }, TEST_UI_TEST_3       },
	{{"->"     ,"->"        }, TEST_UI_TEST_4       },
	{{"IR"     ,"IR"        }, TEST_UI_TEST_5       },
	{{"->"     ,"->"        }, TEST_UI_TEST_6       },
	{{"端口:"  ,"Port:"     }, TEST_UI_PORT         },
	{{"浮地"   ,"Float"     }, TEST_UI_CUR_PORT     },
	{{"多路:"  ,"Roads:"    }, TEST_UI_ROADS        },
	{{"HHXX"   ,"HHXX"      }, TEST_UI_CUR_ROADS    },
	
	{{"01"     ,"01"        }, TEST_UI_ROAD01_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD01_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD01_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD01_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD01_REAL   },
	{{"823.4s" ,"823.4s"    }, TEST_UI_ROAD01_TIME   },
	{{"02"     ,"02"        }, TEST_UI_ROAD02_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD02_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD02_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD02_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD02_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD02_TIME   },
	{{"03"     ,"03"        }, TEST_UI_ROAD03_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD03_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD03_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD03_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD03_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD03_TIME   },
	{{"04"     ,"04"        }, TEST_UI_ROAD04_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD04_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD04_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD04_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD04_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD04_TIME   },
	{{"05"     ,"05"        }, TEST_UI_ROAD05_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD05_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD05_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD05_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD05_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD05_TIME   },
	{{"06"     ,"06"        }, TEST_UI_ROAD06_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD06_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD06_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD06_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD06_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD06_TIME   },
	{{"07"     ,"07"        }, TEST_UI_ROAD07_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD07_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD07_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD07_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD07_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD07_TIME   },
	{{"08"     ,"08"        }, TEST_UI_ROAD08_NUM    },
	{{"ACW"    ,"ACW"       }, TEST_UI_ROAD08_MODE   },
	{{"正在测试","Testing"  }, TEST_UI_ROAD08_STATUS },
	{{"5.000kV","5.000kV"   }, TEST_UI_ROAD08_VOLTAGE},
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD08_UPPER  },
	{{"2.000mA","2.000mA"   }, TEST_UI_ROAD08_REAL   },
	{{"123.4s" ,"123.4s"    }, TEST_UI_ROAD08_TIME   },
	
	{{"测试结果:","TestResult:"}, TEST_UI_RESULT     },
	{{"0001"   ,"0001"      }, TEST_UI_RES1_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES1_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES1_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES1_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES1_RESULT   },
    
	{{"0001"   ,"0001"      }, TEST_UI_RES2_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES2_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES2_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES2_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES2_RESULT   },
	{{"0001"   ,"0001"      }, TEST_UI_RES3_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES3_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES3_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES3_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES3_RESULT   },
	{{"0001"   ,"0001"      }, TEST_UI_RES4_NUM      },
	{{"5.000kV","5.000kV"   }, TEST_UI_RES4_VOL      },
	{{"2.000mA","2.000mA"   }, TEST_UI_RES4_CUR      },
	{{"123.4s" ,"123.4s"    }, TEST_UI_RES4_TIME     },
	{{"上限报警","High Fail"}, TEST_UI_RES4_RESULT   },
};
/**
  * @brief  测试窗口结构体初始化
  */
static MYUSER_WINDOW_T test_windows=
{
    {0},
    test_win_cb, update_menu_key_inf,
	{
        test_ui_ele_pool,COUNT_ARRAY_SIZE(test_ui_ele_pool),
        (CS_INDEX*)test_ui_ele_buf,COUNT_ARRAY_SIZE(test_ui_ele_buf)
    },
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  设置测试界面功能键F0回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f0_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F1回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f1_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F2回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F3回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f3_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F4回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f4_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  设置测试界面功能键F5回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f5_cb(KEY_MESSAGE *key_msg)
{
    change_key_menu(key_msg->user_data);
}
/**
  * @brief  设置测试界面功能键F6回调函数
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void test_win_f6_cb(KEY_MESSAGE *key_msg)
{
    back_win(key_msg->user_data);
}

static void test_win_f1_1_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f2_1_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f3_1_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f4_1_cb(KEY_MESSAGE *key_msg)
{
}

static void test_win_f1_2_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f2_2_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f3_2_cb(KEY_MESSAGE *key_msg)
{
}
static void test_win_f4_2_cb(KEY_MESSAGE *key_msg)
{
}
/**
  * @brief  改变菜单按键信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void change_key_menu(int id)
{
    static int i = 0;
    uint8_t n = ARRAY_SIZE(test_ui_acw_menu_pool);
    init_menu_key_info(test_ui_acw_menu_pool[i], ARRAY_SIZE(test_ui_acw_menu_pool[i]), id);
//     init_menu_key_info(test_ui_acw_menu_pool[i]);
    i = (i+1) % n;
}


/**
  * @brief  更新菜单键信息
  * @param  [in] key_msg 按键消息
  * @retval 无
  */
static void update_menu_key_inf(WM_HMEM hWin)
{
	init_menu_key_info(test_ui_menu_key_pool, 7, hWin);
}

/**
  * @brief  设置测试界面窗口句柄
  * @param  [in] hWin 窗口句柄
  * @retval 无
  */
static void set_test_windows_handle(WM_HWIN hWin)
{
	test_windows.handle = hWin;
}

/**
  * @brief  根据屏幕尺寸初始化界面的文本对象位置尺寸信息
  * @param  无
  * @retval 无
  */
static void init_test_ui_text_ele_pos_inf(void)
{
    switch(sys_par.screem_size)
    {
    case SCREEN_4_3INCH:
        break;
    case SCREEN_6_5INCH:
        break;
    default:
    case SCREEN_7INCH:
        _7_init_test_ui_layout1_text_ele_pos(test_ui_ele_pool);
        break;
    }
}
/**
  * @brief  重绘背景
  * @param  无
  * @retval 无
  */
static void test_win_paint_frame(void) 
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	GUI_SetBkColor(WINDOWS_BAK_COLOR);
	GUI_ClearRectEx(&r);
}
/**
  * @brief  测试界面回调函数
  * @param  [in] pMsg 回调函数指针
  * @retval 无
  */
static void test_win_cb(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	MYUSER_WINDOW_T* win;
    
	switch (pMsg->MsgId)
	{
		case WM_CREATE:
		{
            set_test_windows_handle(hWin);
            win = get_user_window_info(hWin);
            
			WM_SetFocus(hWin);/* 设置聚焦 */
			update_menu_key_inf(hWin);
			WM_SetCreateFlags(WM_CF_MEMDEV);//如果不开启显示效果非常差, 开启后刷屏很慢
			
			if(win != NULL)
			{
                init_test_ui_text_ele_pos_inf();
                init_window_text_ele_list(win);//初始化窗口文本对象链表
				init_window_text_ele(win);
			}
			timer_handle = WM_CreateTimer(hWin, id_base++, 100, 0);
			break;
		}
		case WM_TIMER:
		{
            static int time = 0;
            uint8_t buf[10];
            if(++time > 9999)
            {
                time = 0;
            }
            mysprintf(buf, unit_pool[TIM_U_s], 151, time);
			win = get_user_window_info(hWin);
			update_text_ele((CS_INDEX)TEST_UI_ROAD01_TIME, win, buf);
			update_text_ele((CS_INDEX)TEST_UI_ROAD02_TIME, win, buf);
			update_text_ele((CS_INDEX)TEST_UI_ROAD03_TIME, win, buf);
			update_text_ele((CS_INDEX)TEST_UI_ROAD04_TIME, win, buf);
			WM_RestartTimer(timer_handle, 100);
			break;
        }
        case WM_KEY:
            break;
		case WM_PAINT:
			test_win_paint_frame();
            draw_group_inf_area();
			draw_composition_7_1();
			break;
		case WM_NOTIFY_PARENT:
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}
/* Public functions ---------------------------------------------------------*/

/**
  * @brief  创建测试窗口
  * @param  [in] hWin 父窗口句柄
  * @retval 无
  */
void create_test_win(int hWin)
{    
    init_window_size(&test_windows, test_win_pos_size_pool[sys_par.screem_size]);
	create_user_window(&test_windows, &windows_list, hWin);//创建测试界面
}


/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
