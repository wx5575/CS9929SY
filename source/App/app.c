/**
  ******************************************************************************
  * @file    app.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   用户应用程序
  ******************************************************************************
  */
#define APP_GLOBALS
#include "app.h"
#include "sys.h"
#include "board_support_package.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "mem_alloc.h"
#include "led.h"
#include "timer.h"
#include "GUI.h"
#include "ff.h"
#include "bsp.h"
#include "includes.h"
#include "WM.h"
#include "Key_LED.h"
#include "ff.h"
#include "rtc_config.h"
#include "keyboard.h"
#include "os.h"
#include "GUI.H"
#include "EDIT.h"
#include "scan_keyboard.h"
#include "LCD_RA8875.H"
#include "CODED_DISC/coded_disc.h"
#include "ui_com/com_edit_api.h"
#include "spi_flash.h"
#include "cs99xx_mem_api.h"
#include "CH376_USB_UI.H"
#include "cs99xx_usb_manager.h"
#include "WM.h"
#include "DIALOG.h"
#include "password_win/password_win.h"
#include "type/cs99xx_type.h"

uint8_t g_key_char;
extern WM_HWIN hDlg;
static void File_Init(void)
{	
	fs_result = f_mount(&fs_struct, "0:/", 0);
	if (fs_result != FR_OK)
	{
		return;
	}
}

static timer_for_app_callback(void)
{
	/* 服务蜂鸣器 */
	sub_buzzer_time();
	
	
}
static  void  AppObjCreate (void)
{
	OS_ERR	  err;
	
	/* 创建同步信号量 */ 
//    	BSP_OS_SemCreate(&SEM_SYNCH, 0,	(CPU_CHAR *)"SEM_SYNCH");
	
	/* 创建消息队列为键盘服务 */
	OSQCreate(&KeyboardQSem, "keyboard_queue", OS_MAX_QS, &err);
    OSMutexCreate(&MemMutex, "MemMutex", &err);
	/* 创建定时器 */
	OSTmrCreate((OS_TMR		*)&timer_for_app,		//定时器对象
                (CPU_CHAR	*)"tmr1",		//定时器名称
                (OS_TICK	 )1,			//1*10=10ms
                (OS_TICK	 )1,          //1*10=10ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期定时器模式
                (OS_TMR_CALLBACK_PTR)timer_for_app_callback,//回调函数
                (void	    *)0,			//参数
                (OS_ERR	    *)&err);		//错误码
	OSTmrStart(&timer_for_app, &err);	//启动定时器
}

void soft_init(void)
{
    CS_ERR err;
    TEST_FILE *f;
    
	File_Init();
    check_type();
    read_sys_flag();
    
    if(sys_flag.mem_init)
    {
        sys_flag.mem_init = 0;
        save_sys_flag();
        init_all_file();
        
        f = get_file_inf(0, &err);
        
        if(err == CS_ERR_NONE)
        {
            g_cur_file = f;
        }
    }
    
    read_sys_par();
    check_sys_par(&err);
    
    if(err != CS_ERR_NONE)
    {
        init_sys_par();
    }
    
    read_all_file();
    
    set_cur_file(sys_flag.last_file_num);//将最近使用的文件设为当前文件
    read_group_info(sys_flag.last_file_num);
    
    if(CS_ERR_NONE != check_file_data(&file_pool[0]))
    {
        init_file_data(&file_pool[0], 0);
        save_file(0);
    }
}

void ch376_task(void *p_arg)
{
	OS_ERR err;
    uint8_t strong_brush_time = 0;//强刷计时
    uint8_t strong_brush_flag = 0;//强刷标记
    
    init_ch376();
	OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_PERIODIC,&err);//延时5s
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_PERIODIC,&err);//延时5ms
        
        if(++strong_brush_time > 5)
        {
            strong_brush_time = 0;
            strong_brush_flag = 1;
        }
        else
        {
            strong_brush_flag = 0;
        }
		
        set_cur_ch376_chip(1);
		if(check_connect_usb(strong_brush_flag))
		{
			usb2_server_task();
		}
        
        set_cur_ch376_chip(2);
		if(check_connect_usb(strong_brush_flag))
		{
			usb2_server_task();
		}
	}
}

static void AppTaskScanKey(void *p_arg);
//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();//CPU初始化
	
	BSP_Tick_Init();//OS系统心跳时钟初始化
	
	bsp_init();//硬件初始化
    
    soft_init();
    
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif
	
	AppObjCreate();
	BUZZER_ON_T(1000);//蜂鸣器启动1000ms
	
	OS_CRITICAL_ENTER();	//进入临界区
	//STemWin Demo任务
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,
				 (CPU_CHAR*   )"Emwindemo task",
                 (OS_TASK_PTR )emwindemo_task,
                 (void*       )0,
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void*       )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//USB任务
	OSTaskCreate((OS_TCB*     )&Ch376TaskTCB,
				 (CPU_CHAR*   )"Ch376Task",
                 (OS_TASK_PTR )ch376_task,
                 (void*       )0,
                 (OS_PRIO	  )CH376_TASK_PRIO,
                 (CPU_STK*    )&CH376_TASK_STK[0],
                 (CPU_STK_SIZE)CH376_STK_SIZE/10,
                 (CPU_STK_SIZE)CH376_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void*       )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//键盘扫描任务
	OSTaskCreate((OS_TCB*     )&ScanKeyTaskTCB,
				 (CPU_CHAR*   )"ScanKey task",
                 (OS_TASK_PTR )AppTaskScanKey,
                 (void*       )0,
                 (OS_PRIO	  )SCAN_KEY_TASK_PRIO,
                 (CPU_STK*    )&SCAN_KEY_TASK_STK[0],
                 (CPU_STK_SIZE)SCAN_KEY_STK_SIZE/10,
                 (CPU_STK_SIZE)SCAN_KEY_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void*       )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);				 
// 	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务
    
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel(&StartTaskTCB, (OS_ERR*)&err);//删除启动任务
}
void set_framewin_skin(void)
{    
    FRAMEWIN_SKINFLEX_PROPS FRAMEWIN_pProps;
    
    FRAMEWIN_GetSkinFlexProps(&FRAMEWIN_pProps, FRAMEWIN_SKINFLEX_PI_ACTIVE);
//    FRAMEWIN_pProps.aColorFrame[0] = GUI_BLUE;
//    FRAMEWIN_pProps.aColorFrame[1] = GUI_LIGHTBLUE;
//    FRAMEWIN_pProps.aColorFrame[2] = GUI_LIGHTBLUE;
//    
//    FRAMEWIN_pProps.aColorTitle[0] = GUI_LIGHTBLUE;
//    FRAMEWIN_pProps.aColorTitle[1] = GUI_WHITE;
    FRAMEWIN_SetSkinFlexProps(&FRAMEWIN_pProps, FRAMEWIN_SKINFLEX_PI_ACTIVE);
    FRAMEWIN_SetSkinFlexProps(&FRAMEWIN_pProps, FRAMEWIN_SKINFLEX_PI_INACTIVE);
}
//EMWINDEMO任务
void my123(void);
void task_txt(void);
void main_ui(void);
void task_234(void);
void task_auto(void);
void EditTask(void);
void main_in7_ui(void);
void main_ui_enter(void);
void MainTask(void);
void emwindemo_task(void *p_arg)
{
	/* 开启所有窗口使用内存设备 */
    WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	
	GUI_UC_SetEncodeUTF8();/* 使能UTF8解码 */
	
	/* 使能控件的皮肤色 */
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
    CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    MENU_SetDefaultSkin(MENU_SKIN_FLEX);
    MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	set_framewin_skin();
    
	main_ui_enter();//主界面入口
//    MainTask();
	
	while(1)
	{
		GUI_Delay(100);
	}
}

//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//LED0任务
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}

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
	static uint8_t count_time = 0;
	static uint8_t key_index = 0;
	static uint8_t c = 0;
	static uint32_t key_back = KEY_NONE;
	static uint8_t index_back = 0xff;
	static uint8_t next_bit = 0;
	static uint8_t cour = 0;
	static uint8_t flag = 0;
	static uint8_t input_st = 0;
	static uint8_t temp_cur_num = 0;
    WM_HMEM handle = 0;
	
    handle = get_cur_edit_handle();
	
	if(key != KEY_NONE)
	{
		count_time = 0;
		
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
		if(++count_time > 80)
		{
			key_back = KEY_NONE;
			c = 0;
			count_time = 0;
			next_bit = 1;
			input_st = 2;
		}
	}
	
	key_index = 0xff;
	
	switch(key)
	{
		case KEY_0:
			key_index = 0;// * 4 + c;
			break;
		case KEY_1:
			key_index = 1;// * 4 + c;
			break;
		case KEY_2:
			key_index = 2;// * 4 + c;
			break;
		case KEY_3:
			key_index = 3;// * 4 + c;
			break;
		case KEY_4:
			key_index = 4;// * 4 + c;
			break;
		case KEY_5:
			key_index = 5;// * 4 + c;
			break;
		case KEY_6:
			key_index = 6;// * 4 + c;
			break;
		case KEY_7:
			key_index = 7;// * 4 + c;
			break;
		case KEY_8:
			key_index = 8;// * 4 + c;
			break;
		case KEY_9:
			key_index = 9;// * 4 + c;
			break;
		case KEY_POINT:
			key_index = 10;// * 4;
			break;
		default:
			break;
	}
	
	if(key_index != 0xff)
	{
		uint8_t buf[20] = {0};
		uint8_t temp = 0;
        
		if(handle == 0)
		{
            return;
		}
		
		EDIT_GetText(handle, (char*)buf, 20);
		cour = EDIT_GetCursorCharPos(handle);
		
		if(key_index != index_back)
		{
			index_back = key_index;
			EDIT_SetCursorAtChar(handle, cour + 1);
		}
		
		cour = EDIT_GetCursorCharPos(handle);
		
		temp_cur_num = bufx[key_index];
		temp = num_buf[key_index] + c;
		
		g_key_char = (ui_flag.shift_flag?KEY_ALL_VALUE:KEY_ALL_VALUE_)[temp];
//		 GUI_SendKeyMsg('~', 1);
//		 EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0), "123");
//		 WM_SendMessageNoPara(hDlg, 0x481);
		buf[cour] = g_key_char;
		EDIT_SetText(handle, (const char*)buf);
		EDIT_SetCursorAtChar(handle, ++cour);
		flag = 1;
		count_time = 0;
		input_st = 1;
	}
	
	if(input_st == 1)
	{
		if(flag)
		{
			flag = 0;
			EDIT_SetCursorAtChar(handle, cour - 1);
		}
		else if(next_bit)
		{
			next_bit = 0;
		}
	}
	else if(input_st == 2)
	{
		EDIT_SetCursorAtChar(handle, cour);
		input_st = 0;
	}
}

void keyboard_password(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	uint8_t index = 0xff;
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
    
	switch(key)
	{
		case KEY_0:index=0;break;
		case KEY_1:index=1;break;
		case KEY_2:index=2;break;
		case KEY_3:index=3;break;
		case KEY_4:index=4;break;
		case KEY_5:index=5;break;
		case KEY_6:index=6;break;
		case KEY_7:index=7;break;
		case KEY_8:index=8;break;
		case KEY_9:index=9;break;
        default:index = 0xff;break;
	}
	
	if(index != 0xff)
	{
        EDIT_GetText(handle, (char*)buf, 10);
        cour = EDIT_GetCursorCharPos(handle);
        max_len = pwd_max_len();
        add_a_char_into_edit_str(ASCII_NUM[index], cour, max_len);
        len = strlen((const char*)buf);
        
        if(len < max_len)
        {
            strcat((char*)buf, "*");
            EDIT_SetText(handle, (const char*)buf);
            EDIT_SetCursorAtChar(handle, cour + 1);
        }
	}
}

void keyboard_num(uint32_t key)
{
	#define ASCII_NUM 	"0123456789."
	
	switch(key)
	{
		case KEY_0:
			GUI_SendKeyMsg(ASCII_NUM[0], 1);
			break;
		case KEY_1:
			GUI_StoreKeyMsg(ASCII_NUM[1], 1);
			break;
		case KEY_2:
			GUI_SendKeyMsg(ASCII_NUM[2], 1);
			break;
		case KEY_3:
			GUI_SendKeyMsg(ASCII_NUM[3], 1);
			break;
		case KEY_4:
			GUI_SendKeyMsg(ASCII_NUM[4], 1);
			break;
		case KEY_5:
			GUI_SendKeyMsg(ASCII_NUM[5], 1);
			break;
		case KEY_6:
			GUI_SendKeyMsg(ASCII_NUM[6], 1);
			break;
		case KEY_7:
			GUI_SendKeyMsg(ASCII_NUM[7], 1);
			break;
		case KEY_8:
			GUI_SendKeyMsg(ASCII_NUM[8], 1);
			break;
		case KEY_9:
			GUI_SendKeyMsg(ASCII_NUM[9], 1);
			break;
		case KEY_POINT:
			GUI_SendKeyMsg(ASCII_NUM[10], 1);
			break;
		default:
			break;
	}
}

void set_scan_key_custom_fun(void (*fun)())
{
	scan_key_custom_fun = fun;
	app_flag.stop_scan_key = 1;
}

void keyboard_fun_str(uint32_t key_value)
{
	keyboard_str(key_value);
}

void keyboard_fun_num(uint32_t key_value)
{
	keyboard_num(key_value);
}

void keyboard_fun_pwd(uint32_t key_value)
{
    keyboard_password(key_value);
}
static void AppTaskScanKey(void *p_arg)
{
	uint32_t key_value = KEY_NONE;
	OS_ERR	err;
	
	(void)p_arg;		/* 避免编译器告警 */
	init_keyboard();
	set_scan_key_custom_fun(NULL);
	OSTaskSuspend(&ScanKeyTaskTCB, &err);
// 	_create_menu_key_win();
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 10, 0, &err);
		report_key_value();
		key_value =  get_key_value();
		
// 		if(NULL != scan_direct_key_fun)
// 		{
// 			scan_direct_key_fun(key_value);
// 		}
		
// 		scan_direct_key_function(key_value);
		//系统功能按键处理
		dispose_funcation_key(key_value);
		
		/* 根据不同界面定制按键扫描函数 */
		if(NULL != scan_key_custom_fun)
		{
			scan_key_custom_fun(key_value);
		}
		
		/* 菜单按键扫描 */
		if(NULL != global_fun_key_dispose)
		{
			global_fun_key_dispose(key_value);
		}
	}					 
}

/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/
