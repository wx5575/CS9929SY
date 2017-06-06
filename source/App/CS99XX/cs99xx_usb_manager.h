
#ifndef __CS99XX_USB_MANAGER_H__
#define __CS99XX_USB_MANAGER_H__

#include "stm32f4xx.h"


enum{
    CH376_USB_1 = 1,
    CH376_USB_2,
};

enum{
	USB_CON_ST_NG,
	USB_CON_ST_OK,
};

enum{
	USB_TASK_NULL,
	USB_LEADING_OUT_RESULT,//导出结果
	USB_LEADING_OUT_SYS_DATA,//导出系统数据
	USB_LEADING_IN_SYS_DATA,//导入系统数据
	USB_SAVE_ONE_RESULT,//保存一条结果,测试一条保存一条
    
    USB_SCREEN_CAPTURE,///<截屏
};

typedef struct{
	uint8_t usb_status;//usb设备的连接状态
}USB_MANAGER_T;


#ifdef   USB_MANAGER_GLOBALS
#define  USB_MANAGER_EXT
#else
#define  USB_MANAGER_EXT  extern
#endif

// USB_MANAGER_EXT USB_MANAGER_T usb_manager_t;
USB_MANAGER_EXT uint8_t usb_exe_task;




extern int32_t check_usb_flash(void);
extern uint8_t check_connect_usb(uint8_t strong_brush);
extern uint8_t check_connect_usb2(void);
extern void usb1_server_task(void);
extern void usb2_server_task(void);
extern int32_t leading_out_result_usb(void);
extern uint8_t get_usb_connect_st(void);
extern void set_exe_usb_leading_out_result_task(void);
extern void set_exe_usb_leading_out_sys_data_task(void);
extern void set_exe_usb_leading_in_sys_data_task(void);
extern void set_exe_usb_save_one_result_data_task(void);
extern uint8_t mFlushBufferToDisk(uint8_t force);
extern void write_data_to_disk_buf(uint8_t *buf, uint32_t num);
extern void clear_disk_info(void);
extern uint8_t create_file_save_result(void);
extern uint8_t new_file_in_usb_flash(uint8_t *file_name, uint8_t *path);
extern void set_usb_disk_task(uint8_t cmd);

#endif //__CS99XX_USB_MANAGER_H__
