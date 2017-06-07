/**
  ******************************************************************************
  * @file    com_ui_info.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   测试界面文本控件对应文本数组中的索引枚举定义
  ******************************************************************************
  */
#ifndef __COM_UI_INFO_H__
#define __COM_UI_INFO_H__

/* Includes ------------------------------------------------------------------*/
#include "cs_list.h"
#include "GUI.H"
#include "WM.h"
#include "TEXT.h"
#include "stm32f4xx.h"
#include "cs99xx_struct.h"
#include "UI/KEY_MENU_WIN/key_menu_win.h"
#include "fonts.h"
#include "ui_com/com_ui_base.h"
#include "tools.h"

#pragma  diag_suppress 870 //消除870号编译警告

#define SYS_LANGUAGE		sys_par.language //CHINESE //ENGLISH //

/* 选择字体 如果当前系统字体为空就使用系统字体 不为空就正常使用 */
#define SEL_FONT(font)      font[SYS_LANGUAGE]==NULL?font[CHINESE]:font[SYS_LANGUAGE]
#define SELE_STR(S1, S2)	(SYS_LANGUAGE==CHINESE?S1:S2) ///< 根据系统语言选择不同的字符串

#define WINDOWS_BAK_COLOR	GUI_BLUE	//GUI_GRAY ///< 窗口背景色

/** 
  * @brief 编辑控件结构
  */
typedef struct WIDGET_ELEMENT_  _WIDGET_ELEMENT_;

/** 
  * @brief 用户窗口结构
  */
typedef struct MYUSER_WINDOW    MYUSER_WINDOW_T;
/** 
  * @brief 控件位置尺寸结构
  */
typedef struct{
	uint16_t x;///<x坐标
	uint16_t y;///<y坐标
	uint16_t width;///<宽
	uint16_t height;///<高
}WIDGET_POS_SIZE_T;
/** 
  * @brief 控件显示信息结构
  */
typedef struct UI_ELE_DISPLAY_INFO{
	uint16_t base_x;///< x基坐标
	uint16_t base_y;///<y基坐标
    WIDGET_POS_SIZE_T pos_size;///<窗口的位置尺寸
	uint8_t max_len;///< 最大长度
    const GUI_FONT * font[LANGUAGE_NUM];//不同语言可以使用不同的字体
	GUI_COLOR	font_color;///<字体颜色
	GUI_COLOR	back_color;///<背景颜色
	int align;///< 对齐方式
}UI_ELE_DISPLAY_INFO_T;
/** 
  * @brief 文本控件信息结构
  */
typedef struct TEST_UI_ELE{
	uint8_t *text[LANGUAGE_NUM];///< 内容
	CS_INDEX index;///<索引
	UI_ELE_DISPLAY_INFO_T dis_info;///<显示信息
	WM_HMEM	 handle;///< 句柄
	CS_LIST list;///< 文本链表
}TEXT_ELE_T;

/** 
  * @brief 用户回调函数指针类型
  */
typedef void (*USER_CALLBACK)(WM_MESSAGE *);

/** 
  * @brief 控件对象池结构
  */
typedef struct{
    void* pool;///<控件池类型
    uint32_t pool_size;///<控件池大小
    CS_INDEX *index_pool;///<控件索引池
    uint32_t index_size;///<索引池大小
    void (*init_create_fun)(MYUSER_WINDOW_T* win);///< 初始化并创建控件对象池中被索引的控件函数
	CS_LIST list_head;///< 控件链表头
}ELE_POOL_INF;

/** 
  * @brief 用户窗口结构
  */
struct MYUSER_WINDOW{
    uint8_t *win_name[LANGUAGE_NUM];///<窗口名称
	USER_CALLBACK call_back_fun;///<回调函数
	void (*init_key_fun)(WM_HMEM);///<窗口配套的按键更新函数
    ELE_POOL_INF text;///<文本控件索引池
    ELE_POOL_INF edit;///<编辑控件索引池
    ELE_POOL_INF com;///<公共文本控件索引池
    
    WIDGET_POS_SIZE_T pos_size;///<窗口的位置尺寸
	WM_HMEM	handle;///< 窗口句柄
	CS_LIST w_list;///< 窗口链表
};

/** 
  * @brief 对象类型枚举
  */
typedef enum{
	ELE_TEXT,///< 文本 
	ELE_EDIT_NUM,///< 数字编辑框 
	ELE_EDIT_STR,///< 字符串编辑框 
	ELE_DROPDOWN,///< 下拉选择框 
}ELE_TYPE;

/** 
  * @brief 数据类型
  */
typedef enum{
	E_INT_T,///< 整数
	E_FLOAT_T,///< 浮点数
	E_STRING_T,///< 字符串数据
	E_STRUCT_T,///< 结构体数据
}DATA_TYPE;

/** 
  * @brief 数据所占的空间size 
  */
typedef enum{
	D_1_BYTES = 1,///< 1字节数据 
	D_2_BYTES = 2,///< 2字节数据 
	D_4_BYTES = 4,///< 4字节数据 
	D_8_BYTES = 8,///< 8字节数据 
	D_N_BYTES,///< 数据字节数不定,用于字符串数据和结构体数据 
}BYTE_NUM;

/** 
  * @brief 编辑控件可配置属性
  */
typedef struct{
    uint16_t width;///<宽
    uint16_t height;///<高
    const GUI_FONT *font;///<字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int align;///<对齐方式
    uint32_t max_len;///<最大长度
    WM_HMEM handle;///<句柄
}CONFIGURABLE_PROPERTY;

/** 
  * @brief 编辑控件相关显示内容信息
  */
typedef struct{
    CONFIGURABLE_PROPERTY name;///< 名称
    CONFIGURABLE_PROPERTY edit;///< 编辑
    CONFIGURABLE_PROPERTY unit;///< 单位
    uint16_t x;///< 所在窗口的 x 坐标
    uint16_t y;///< 所在窗口的 x 坐标
}EDIT_ELE_DISPLAY_INF;

/** 
  * @brief 编辑控件结构 包含3个子控件 名称+编辑控件+单位信息
  */
struct WIDGET_ELEMENT_{
	uint8_t *name[LANGUAGE_NUM];///< 名称
	CS_INDEX index;///< 通过枚举索引
	uint8_t *value[10];///< 默认值
	
    /* 数据 */
    struct{
        void *data;///< 数据指针
        uint8_t bytes;///< 字节数
    }data;
    
    /* 资源 */
    struct{
        void *table;///< 资源表
        uint8_t size;///< 资源表的条数
    }resource;
    
    /* 类型 */
    struct{
        ELE_TYPE type;///< 对象类型
        DATA_TYPE data_type;///< 数据类型
    }type;
    
    /* 格式 */
    struct{
        uint8_t decs;///< 小数点位数
        uint8_t lon;///< 数据显示长度,如果是浮点数包含小数点，必须小于10
        UNIT_T unit;///< 单位
    }format;
    
    /* 范围 */
    struct{
        uint32_t high;///< 上限
        uint32_t low;///< 下限 
        uint8_t *notice[LANGUAGE_NUM];///< 提示信息包含中英文
    }range;
    
    /* 按键信息 包含系统键 菜单键 键盘服务函数 */
    struct{
        void (*fun_sys_key)();///< 方向键信息更新函数
        void (*fun_menu_key)();//菜单键更新函数
        void (*fun_key)();///< 设置该变量的键盘服务函数
    }key_inf;
    
    EDIT_ELE_DISPLAY_INF dis;///<显示相关的配置信息
    
	CS_LIST e_list;///< 编辑控件链表
};
/** 
  * @brief 警告提示信息结构
  */
typedef struct{
    TEXT_ELE_T title;///<标题
    TEXT_ELE_T content;///<内容
    WIDGET_POS_SIZE_T win_pos_size;///<窗口位置尺寸
    uint32_t dly_auto_close;///<延时自动关闭,计时为0表示不关闭，非0时间到自动关闭,单位ms
}WARNING_INF;
/** 
  * @brief 界面文本对象自动布局结构
  */
typedef struct{
    uint16_t base_x;///<x基坐标
    uint16_t base_y;///<y基坐标
    uint16_t width;///<文本控件的宽度
    uint16_t height;///<文本控件的高度
    uint8_t rows;///<最大行数
    uint8_t columns;///<最大列数
    uint8_t row_spacing;///<行距
    uint8_t column_spacing;///<列距
    const GUI_FONT * font[LANGUAGE_NUM];//不同语言可以使用不同的字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int align;///对齐方式
    uint32_t max_len;///最大长度
}TEXT_ELE_AUTO_LAYOUT_T;
/** 
  * @brief 界面编辑对象自动布局结构
  */
typedef struct{
    uint16_t base_x;///<x基坐标
    uint16_t base_y;///<y基坐标
    uint16_t name_w;///<名称文本控件的宽度
    uint16_t edit_w;///<编辑控件的宽度
    uint16_t uint_w;///<单位文本控件的宽度
    uint16_t height;///<控件的高度
    uint8_t rows;///<最大行数
    uint8_t columns;///<最大列数
    uint8_t row_spacing;///<行距
    uint8_t column_spacing;///<列距
    const GUI_FONT *font;///<字体
    GUI_COLOR font_color;///<字体颜色
    GUI_COLOR back_color;///<背景颜色
    int align;///<对齐方式
    uint32_t max_len;///<最大长度
}EDIT_ELE_AUTO_LAYOUT_T;
/*********************************************************************/
/** 
  * @brief 范围信息是很多界面者会用到的公共文本控件
  */
typedef enum {
	ELE_RANGE_NAME,///<主界面的通信状态
	ELE_RANGE_NOTICE,///<主界面的系统时间
    
    COM_ELE_NUM,///<公共控件的个数
}COM_TEXT_INDEX;
extern TEXT_ELE_T com_text_ele_pool[COM_ELE_NUM];
/*********************************************************************/
/** 
  * @brief 用户自定义消息
  */
typedef enum{
	CM_CHILD_W_MSG = 0XFFFFF,//子窗口发来的消息 
    CM_UPDATE_USB_ST,///<更新usb的状态
	CM_FILE_UI_SAVE,
	CM_FILE_UI_NEW,//文件界面
	CM_FILE_UI_READ,//文件读取
	CM_FILE_UI_EDIT,//文件编辑
	CM_FILE_UI_DEL,//文件删除
	CM_WARNING_UI,//警告框
    CM_WARNING_INTO_SAVE_FILE,///<进入存贮文件的警告框
    CM_WARNING_INTO_DELETE_FILE,///<进入存贮文件的警告框
	CM_EDIT_WIDGET_CHANGE,//当编辑控件改变时程序监控到后向控件的父窗口发送这个消息，通常是为了选择按键服务程序
	CM_DISTROY_DIALOG,//销毁对话框时发给父窗口的消息
	
	CM_DIALOG_RETURN_OK,//子窗口返回时按下了OK键 或 ENTER
	CM_DIALOG_RETURN_CANCLE,//子窗口返回时按下了CANCLE键 或 EXIT
}CUSTOM_MSG_ID;
/** 
  * @brief 用户自定义消息结构
  */
typedef struct{
	CUSTOM_MSG_ID id;///<所在子窗口的自定义ID
	int msg;///<命令
	int user_data;///<用户数据
}CUSTOM_MSG_T;
/** 
  * @brief 用户界面使用的全局标记
  */
typedef struct{
    uint8_t shift_flag;///< SHIFT键按下标记
    uint8_t key_lock_flag;///< SHIFT键按下标记
}UI_FLAG;

#ifdef   COM_UI_GLOBALS
#define  COM_UI_EXT
#else
#define  COM_UI_EXT  extern
#endif

COM_UI_EXT CS_LIST 				windows_list;///<窗口链表
COM_UI_EXT MYUSER_WINDOW_T      *g_cur_win;///<当前窗口指针
COM_UI_EXT _WIDGET_ELEMENT_     *g_cur_edit_ele;///<当前编辑对象
COM_UI_EXT TEXT_ELE_T           *g_cur_text_ele;///<当前文本对象
COM_UI_EXT CUSTOM_MSG_T 	    g_custom_msg;///<用户自定义消息实体变量
COM_UI_EXT volatile UI_FLAG     ui_flag;///<界面使用全局标记
COM_UI_EXT uint32_t             id_base;///<全局控件ID变量

extern void init_window_text_ele(MYUSER_WINDOW_T* win);
extern void update_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, const uint8_t *str);
extern void set_text_ele_font_color(CS_INDEX index, MYUSER_WINDOW_T* win, GUI_COLOR color);
extern void create_user_window(MYUSER_WINDOW_T* win_info, CS_LIST *list_head);
extern void init_window_size(MYUSER_WINDOW_T* win_inf, WIDGET_POS_SIZE_T *pos_size_inf);
extern void init_create_win_all_ele(MYUSER_WINDOW_T* win);
extern void create_user_dialog(MYUSER_WINDOW_T* win_info, CS_LIST *list_head, WM_HWIN hWin);
extern void set_user_window_handle(WM_HWIN hWin);
extern void set_cur_window(MYUSER_WINDOW_T* win_info);
extern void set_cur_edit_ele(_WIDGET_ELEMENT_ *node);
extern void show_user_window(MYUSER_WINDOW_T* win_info);
extern void back_win(int id);
extern MYUSER_WINDOW_T * get_user_window_info(WM_HWIN hWin);
extern TEXT_ELE_T* get_text_ele_node(CS_INDEX index, CS_LIST* list, CS_ERR *err);
extern void set_com_text_ele_dis_inf(UI_ELE_DISPLAY_INFO_T *inf, CS_INDEX index);
extern void update_com_text_ele(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str);
extern void init_window_com_text_ele(MYUSER_WINDOW_T* win);
extern void init_window_text_ele_list(MYUSER_WINDOW_T *win);
extern void init_window_edit_ele_list(MYUSER_WINDOW_T *win);
extern void init_window_text_ele_dis_inf(MYUSER_WINDOW_T *win, TEXT_ELE_AUTO_LAYOUT_T *inf);
extern void init_window_edit_ele_dis_inf(MYUSER_WINDOW_T *win, EDIT_ELE_AUTO_LAYOUT_T* inf);
extern void set_custom_msg_id(CUSTOM_MSG_ID id);
extern void update_win_menu_key(MYUSER_WINDOW_T* win);
extern void init_dialog(MYUSER_WINDOW_T * win);
extern void send_msg_to_parent(WM_HWIN hwin, int id, int val);
extern void send_no_par_msg_to_parent(WM_HWIN hwin, int id);
extern void update_all_windows_text(void);
extern void (*global_fun_key_dispose)(uint32_t key_value);
extern void set_global_fun_key_dispose(void (*fun)(uint32_t));
extern void set_com_text_ele_inf(CS_INDEX index, MYUSER_WINDOW_T* win, uint8_t *str[]);

#endif //__COM_UI_INFO_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
