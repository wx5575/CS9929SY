/**
  ******************************************************************************
  * @file    app.c
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   用户应用程序
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#define APP_GLOBALS
#include "os.h"
#include "GUI.H"
#include "WM.h"
#include "DIALOG.h"
#include "sys.h"
#include "mem_alloc.h"
#include "bsp.h"
#include "board.h"
#include "Key_LED.h"
#include "CH376_USB_UI.H"
#include "key_fun_manage.h"
#include "cs99xx_mem_api.h"
#include "cs99xx_usb_manager.h"
#include "type/cs99xx_type.h"
#include "key_server.h"
#include "app.h"

void AppTaskScanKey(void *p_arg);

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


void AppTaskScanKey(void *p_arg)
{
	OS_ERR err;
    
    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
    scan_key_task();
    while(1);
}

/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/
