
#ifndef __MAIN_UI_COM_INFO_H__
#define __MAIN_UI_COM_INFO_H__


typedef enum {
	MAIN_UI_COM_ST,///<主界面的通信状态
	MAIN_UI_SYS_TIME,///<主界面的系统时间
    MAIN_UI_ELE_NUM,///<主界面文本对象个数
}MAIN_UI_INDEX;

extern void update_usb_dis_status(void);
extern void set_main_win_progbar_value(int value);
extern void delete_main_win_progbar(void);
extern void set_main_win_progbar_show(void);

#endif //__MAIN_UI_COM_INFO_H__
