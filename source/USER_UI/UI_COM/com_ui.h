
#ifndef __COM_UI_H__
#define __COM_UI_H__

typedef struct{
	uint8_t *name[2];
	uint16_t x;
	uint16_t y;
	uint8_t type;//资源类型 EDIT 等等
	uint16_t width;
	uint16_t height;
	uint8_t *value[10];
	uint8_t unit;//单位 只是单位的索引
	void (*key_fun)(uint32_t);//键盘服务函数
	_MENU_KEY_INFO_T_ menu_key_info[6];//菜单键信息
	void *data;
}DIALOG_SOURCE;

static all_dialog_info[]=
{
	{{"文件名","FileName"}, 10/*x*/,40/*y*/,UI_EDIT, 300, 40, {"DEFAULT"}, U_NONE, keyboard_fun_str,
		{
			{F_KEY_OK,			menu_key_ok			, 0},//f1
			{F_KEY_CANCLE,		menu_key_cancle		, 0},//f2
			{F_KEY_NEXT,		menu_key_next		, 0},//f3
			{F_KEY_DEL,			menu_key_delete		, 0},//f4
			{F_KEY_HOME,		menu_key_backspace	, 0},//f5
			{F_KEY_END,			menu_key_selected	, 0},//f6
		},
	},
	
	{{"工作模式","WorkMode"}, 10/*x*/,50/*y*/,UI_DROP, 300, 40, {"N","G"}, U_NONE, keyboard_fun_num,
		{
			{F_KEY_OK,			menu_key_ok			, 0},//f1
			{F_KEY_CANCLE,		menu_key_cancle		, 0},//f2
			{F_KEY_NEXT,		menu_key_next		, 0},//f3
			{F_KEY_DEL,			menu_key_delete		, 0},//f4
			{F_KEY_HOME,		menu_key_backspace	, 0},//f5
			{F_KEY_END,			menu_key_selected	, 0},//f6
		},
	},
	
	
};

typedef struct{
	uint8_t *name[2];//对话框名称
	uint32_t handle;//对话框句柄
	uint32_t parent;//父句柄
	void *stc;//资源
	void *data;//数据
}DIALOG_T;
DIALOG_T all_dialog[]=
{
	{{"新建文件","NewFile"}, 0, 0, 0, 0},
};
#endif //__COM_UI_H__
