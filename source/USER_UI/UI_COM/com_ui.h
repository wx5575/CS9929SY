
#ifndef __COM_UI_H__
#define __COM_UI_H__

typedef struct{
	uint8_t *name[2];
	uint16_t x;
	uint16_t y;
	uint8_t type;//��Դ���� EDIT �ȵ�
	uint16_t width;
	uint16_t height;
	uint8_t *value[10];
	uint8_t unit;//��λ ֻ�ǵ�λ������
	void (*key_fun)(uint32_t);//���̷�����
	_MENU_KEY_INFO_T_ menu_key_info[6];//�˵�����Ϣ
	void *data;
}DIALOG_SOURCE;

static all_dialog_info[]=
{
	{{"�ļ���","FileName"}, 10/*x*/,40/*y*/,UI_EDIT, 300, 40, {"DEFAULT"}, U_NONE, keyboard_fun_str,
		{
			{F_KEY_OK,			menu_key_ok			, 0},//f1
			{F_KEY_CANCLE,		menu_key_cancle		, 0},//f2
			{F_KEY_NEXT,		menu_key_next		, 0},//f3
			{F_KEY_DEL,			menu_key_delete		, 0},//f4
			{F_KEY_HOME,		menu_key_backspace	, 0},//f5
			{F_KEY_END,			menu_key_selected	, 0},//f6
		},
	},
	
	{{"����ģʽ","WorkMode"}, 10/*x*/,50/*y*/,UI_DROP, 300, 40, {"N","G"}, U_NONE, keyboard_fun_num,
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
	uint8_t *name[2];//�Ի�������
	uint32_t handle;//�Ի�����
	uint32_t parent;//�����
	void *stc;//��Դ
	void *data;//����
}DIALOG_T;
DIALOG_T all_dialog[]=
{
	{{"�½��ļ�","NewFile"}, 0, 0, 0, 0},
};
#endif //__COM_UI_H__
