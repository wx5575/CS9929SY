/**
  ******************************************************************************
  * @file    cs99xx_struct.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   仪器相关结构体定义
  ******************************************************************************
  */

#ifndef __CS99XX_STRUCT_H__
#define __CS99XX_STRUCT_H__

#include "stm32f4xx.h"
#include "cs_list.h"

#define MAX_STEPS           (99)            ///<单个文件容纳最大步数
#define TEST_PORTS_MAX      (8 * 2)         ///<测试端口的最大数目 必须是8的倍数 
#define MAX_FILES			((uint16_t)31)  ///< 最大文件数目包括一个默认的文件 
#define NAME_LON			((uint16_t)14)  ///< 文件名的长度 

#define PWD_MAX_LEN         8               ///<密码的最大长度
#define DEFAULT_PWD         "888888"        ///<系统默认密码

/** 
  * @brief 用户数组对象索引,只提供类型，用来将用户的索引统一
  */
#define CS_INDEX    int32_t

/**
  * @brief  bool常量定义
  */
typedef enum{
	CS_FALSE,
	CS_TRUE,
}CS_BOOL;
/**
  * @brief  系统错误码定义
  */
typedef enum{
    CS_ERR_NONE = 0x1000,///<正常
    CS_ERR_FILE_NO_EXIST,///<文件不存在
    CS_ERR_FILE_NUM_INVALID,///<文件编号无效
    CS_ERR_FILE_NAME_INVALID,///<文件名无效
    CS_ERR_STRING_TOO_LONG,///<字符串太长
    CS_ERR_DATE_STR_TOO_LONG,///<日期字符串太长
    CS_ERR_DATA_OUT_OF_RANGE,///<数据越界
    CS_ERR_INDEX_INVALID,///<索引值无效
    CS_ERR_KEY_VALUE_INVALID,///<键值无效
    CS_ERR_ELE_INDEX_INVALID,///对象索引无效
}CS_ERR;
/**
  * @brief  文件编号
  */
typedef uint16_t FILE_NUM;
/**
  * @brief  步骤编号
  */
typedef uint16_t STEP_NUM;

/**
  * @brief  定义8个测试端口数据
  */
typedef struct {
	uint16_t port1:2;
	uint16_t port2:2;
	uint16_t port3:2;
	uint16_t port4:2;
	uint16_t port5:2;
	uint16_t port6:2;
	uint16_t port7:2;
	uint16_t port8:2;
}_TEST_PORT;
/**
  * @brief  测试端口结构定义
  */
typedef struct{
    _TEST_PORT ports[TEST_PORTS_MAX / 8];
    uint8_t num;
    uint8_t mode;/* 通信时使用 */
}TEST_PORT;

/**
  * @brief  ACW数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	real_cur;                ///< 真实电流上限值 */
	uint16_t	arc_sur;					///< 电弧侦测 可能是等级模式即整数 可能为电流模式即为浮点数 这个有电弧侦测模式决定 */
	uint16_t	output_freq;				///< 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;					///< 步间pass */
	uint8_t 	step_con;					///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint16_t 	offset_real;                ///< 偏移真实电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}ACW_STRUCT;

/**
  * @brief  CC数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	real_cur;                ///< 真实电流上限值 */
	uint16_t	cur_intensity;              ///< 电流强度 高 中 低 */
	uint16_t	output_freq;      			///< 输出频率选择 可以连续也可以是等级 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;         		///< 步间pass */
	uint8_t 	step_con; 				///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint16_t 	offset_real;                ///< 偏移真实电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}CC_STRUCT;

/**
  * @brief  DCW数据结构定义
  */
typedef struct {
	STEP_NUM     step;                      ///< 测试步 */
	uint8_t     mode;                       ///< 测试步 */
	uint16_t 	output_vol;            ///< 测试电压 */
	uint8_t 	range;                      ///< 电流档位   2uA ,  20uA , 200uA ,  2mA,   10mA */
	uint16_t	upper_limit;				///< 电流上限 */
	uint16_t	lower_limit;				///< 电流下限 */
	uint16_t	charge_cur;         		///< 充电电流 */
	uint16_t	arc_sur;   					///< 电弧侦测 */
    uint8_t     output_impedance;           ///< 输出阻抗 */
	uint16_t	delay_time;                 ///< 延时时间 延时时间到了才判断 */
	uint16_t	rise_time;                  ///< 上升时间 */
	uint16_t	stab_time;                  ///< 稳压时间 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	fall_time;                  ///< 下降时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;         		///< 步间pass */
	uint8_t		step_con; 				///< 步间连续 */
	TEST_PORT	port;                       ///< 端口 */
	
	uint16_t	offset_cur;                 ///< 偏移电流 */
	uint8_t 	offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}DCW_STRUCT;

/**
  * @brief  IR数据结构定义
  */
typedef struct {
	STEP_NUM 	step;                       ///< 测试步 */
	uint8_t 	mode;                       ///< 模式 */
	uint16_t 	output_vol;			///< 输出电压 */
	uint8_t 	auto_shift;           ///< 自动换挡 */
	uint32_t 	upper_limit;     			///< 电阻上限 */
	uint32_t 	lower_limit;	    		///< 电阻下限 */
	uint16_t	rise_time;	                ///< 上升时间 */
	uint16_t	test_time;	            ///< 测试时间 */
	uint16_t	delay_time;	                ///< 延时时间 */
	uint16_t	inter_time;	            ///< 间隔时间 */
	uint8_t 	step_pass;	        		///< 步间pass */
	uint8_t 	step_con;					///< 步间连续 */
	TEST_PORT	port;	                    ///< 端口 */
}IR_STRUCT;

/**
  * @brief  GR数据结构定义
  */
typedef struct {
	STEP_NUM 	step;
	uint8_t 	mode;
	uint8_t		voltage_gear;               ///< 电压档位 1 20mv 2 200mv 3 2000mv */
	uint16_t	output_cur;                ///< 输出电流 */
	uint16_t 	upper_limit;				///< 电阻上限 */
	uint16_t 	lower_limit;				///< 电阻下限 */
	uint16_t	test_time;               ///< 测试时间 */
	uint16_t	inter_time;              ///< 间隔时间 */
	uint8_t 	step_pass;					///< 步间pass */
	uint8_t 	step_con; 				///< 步间连续 */
	uint16_t	output_freq;				///< 输出频率 */
	uint16_t	test_method;                ///< 当脉冲测试模式使能了以后 0表示脉冲测试 1表示连续测试 */
	
	uint16_t	offset_res;                 ///< 偏移电阻 */
	uint8_t	    offset_result;              ///< 偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}GR_STRUCT;

/**
  * @brief  BBD数据结构定义
  */
typedef struct {
	STEP_NUM 	step; ///< 设置步 */
	uint8_t 	mode; ///< 模式 */
	uint16_t 	output_vol; ///< 输出电压 */
	uint16_t	open_ratio; ///< 开路比例 0，10%-100% */
	uint16_t	short_ratio; ///< 短路比例 100% - 500% */
	uint16_t 	upper_limit; ///< 电容上限 */
	uint16_t 	lower_limit; ///< 电容下限 */
	uint16_t	output_freq; ///<输出频率 */
	uint16_t	test_time; ///< 测试时间 */
	uint16_t	inter_time; ///< 间隔时间 */
	uint8_t 	step_pass; ///< 步间pass */
	uint8_t 	step_con; ///< 步间连续 */
	float		cap_value; ///< 电容值 */
	uint8_t 	gear; ///< 保存获取电容值时的电流档位 默认 20mA */
	TEST_PORT	port; ///< 端口 */
	
	uint8_t 	get_cs_ok; ///< 标记获取电容已经OK */
    
	float   	offset_cap; ///<偏移电阻 */
	uint8_t	    offset_result; ///<偏移电流测量结果 OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}BBD_STRUCT;

/**
  * @brief  公共数据结构定义
  */
typedef struct {
	STEP_NUM step; ///< 设置步
	uint8_t mode; ///< 模式
}COMMON;

/**
  * @brief  将每一步都定义为一个联合体实现统一操作
  */
typedef union{
	COMMON      com;///<公共参数
	ACW_STRUCT 	acw;///<ACW参数
	DCW_STRUCT 	dcw;///<DCW参数
	IR_STRUCT 	ir;///<IR参数
	GR_STRUCT 	gr;///<GR参数
	BBD_STRUCT  bbd;///<BBD参数
    CC_STRUCT   cc;///<CC参数
}UN_STRUCT,*PUN_STRUCT,**PPUN_STRUCT;

/**
  * @brief  步骤节点结构定义
  */
typedef struct Node_Step{
	UN_STRUCT 	one_step;///<步骤结构数据
	CS_LIST     list;///<链表
}NODE_STEP;
/**
  * @brief  测试步骤缓冲区
  */
#define STEP_LIST_SIZE      ((uint16_t)5) /* 存放最近连续步的个数 */
typedef struct TestGroup{
	NODE_STEP test_steps[STEP_LIST_SIZE];
}TESTGROUP;

/**
  * @brief  测试模式的枚举定义
  */
typedef enum{
    NUL     = 0,///<空
    ACW     = 1,///<ACW 模式
    DCW     = 2,///<DCW 模式
    IR      = 3,///<IR 模式
    GR      = 4,///<GR 模式
    BBD     = 5,///<BBD 模式
    CC      = 6,///<CC 模式
    MODE_END,
}MODE_ENUM;

/**
  * @brief  工件模式的枚举定义
  */
typedef enum{
	N_MODE,///<N模式
	G_MODE,///<G模式
}WORK_MODE_T;

/**
  * @brief  ARC 侦测枚举定义
  */
enum{
	ARC_CUR_MODEL 	= 0,    ///<电流模式
	ARC_GRADE_MODEL = 1,    ///<等级模式
};
/**
  * @brief  测试连接方法
  */
enum{
	FLOAT_MODE	= 0,        ///<浮空
	GND_MODE	= 1,        ///<接地
};
/**
  * @brief  GFI 开关
  */
enum{
	GFI_OFF		= 0,        ///<GFI关闭
	GFI_ON		= 1,        ///<GFI打开
};
/**
  * @brief  PLC 开关
  */
enum{
	PLC_OFF		= 0,        ///<PLC关闭
	PLC_ON		= 1,        ///<PLC打开
};
/**
  * @brief  电弧侦测模式
  */
enum{
	ARC_CUR_MODE,           ///<电弧侦测电流模式
	ARC_GRADE_MODE,         ///<电弧侦测档位模式
};
/**
  * @brief  直流GR的测试方法
  */
enum{
	DC_GR_PULSE = 0,        ///<直流接地脉冲测试
	DC_GR_CONT	= 1,        ///<直流接地连续测试
};
/**
  * @brief  AC档位枚举定义
  */
enum{
	AC_2uA		= 0X1,
	AC_20uA		= 0X2,
	AC_200uA	= 0X3,
	AC_2mA		= 0X4,
	AC_10mA		= 0X5,
	AC_20mA		= 0X6,
	AC_50mA		= 0x7,
	AC_100mA	= 0X8,
	AC_200mA	= 0X9,
	AC_2A	    = 0XA,
    AC_GEAR_END,
};
#define CUR_2uA_STR     "2uA"
#define CUR_20uA_STR    "20uA"
#define CUR_200uA_STR   "200uA"
#define CUR_2mA_STR     "2mA"
#define CUR_10mA_STR    "10mA"
#define CUR_20mA_STR    "20mA"
#define CUR_50mA_STR    "50mA"
#define CUR_100mA_STR   "100mA"
#define CUR_200mA_STR   "200mA"
#define CUR_2A_STR      "2A"



/**
  * @brief  DC档位枚举定义
  */
enum{
	DC_2uA		= 0X1,
	DC_20uA		= 0x2,
	DC_200uA	= 0X3,
	DC_2mA		= 0x4,
	DC_10mA		= 0X5,
	DC_20mA		= 0X6,
	DC_50mA		= 0X7,
	DC_100mA	= 0X8,
    DC_GEAR_END,
};
/**
  * @brief  IR档位枚举定义
  */
enum{
	IR_10MOHM	= 0X01,
	IR_100MOHM	= 0X02,
	IR_1GOHM	= 0x03,
	IR_10GOHM	= 0X04,
	IR_100GOHM	= 0X05,
};
/**
  * @brief  DC-GR电压档位枚举定义
  */
enum{
	DC_GR_20mV	= 0x2,
	DC_GR_200mV	= 0x3,
	DC_GR_2000mV= 0X4,
	DC_GR_12V	= 0X5,
	DC_GR_12V_100mA  = 0x6,
	DC_GR_12V_1000mA = 0x7,
};
/**
  * @brief  AC通信使用的档位枚举定义
  */
enum{
	AC_0_GEAR_COMM	= 0X0,
	AC_1_GEAR_COMM	= 0X1,
	AC_2_GEAR_COMM	= 0X2,
	AC_3_GEAR_COMM	= 0X3,
};
/**
  * @brief  DC通信使用的档位枚举定义
  */
enum{
	DC_0_GEAR_COMM	= 0X0,
	DC_1_GEAR_COMM	= 0X1,
	DC_2_GEAR_COMM	= 0X2,
	DC_3_GEAR_COMM	= 0X3,
	DC_4_GEAR_COMM	= 0X4,
	DC_5_GEAR_COMM	= 0X5,
};
/**
  * @brief  IR通信使用的档位枚举定义
  */
enum{
	IR_1_GEAR_COMM	= 0X1,
	IR_2_GEAR_COMM	= 0X2,
	IR_3_GEAR_COMM	= 0X3,
	IR_4_GEAR_COMM	= 0X4,
	IR_5_GEAR_COMM	= 0X5,
	IR_6_GEAR_COMM	= 0X6,
};
/**
  * @brief  大电容测试输出阻抗
  */
typedef enum{
    OUTPUT_IMPEDAN_OPT_1,///<第一组阻抗
    OUTPUT_IMPEDAN_OPT_2,///<第二组阻抗
}OUTPUT_IMPEDAN_OPTS;
/**
  * @brief  数据的单位枚举定义
  */
typedef enum{
    NULL_U_NULL,///<无单位
	TIM_U_s,///<秒
    VOL_U_mV,///<mV
    VOL_U_V,///< V
    VOL_U_kV,///< kV
    CUR_U_uA,///< uA
    CUR_U_mA,///< mA
    CUR_U_A,///< A
    RES_U_mOHM,///< mOhm
    RES_U_OHM,///< ohm
    RES_U_MOHM,///< Mohm
    RES_U_GOHM,///< Gohm
    CAP_U_pF,///< pF
    CAP_U_nF,///< nF
    CAP_U_uF,///< uF
    FREQ_U_Hz,///< Hz
}UNIT_T;

/**
  * @brief  文件结构定义
  */
typedef struct{
	FILE_NUM num;///<文件编号
	uint8_t name[NAME_LON + 1];///<文件名
	WORK_MODE_T work_mode;///<工作模式 N模式 G模式
	uint16_t total;///< 总测试步
	uint16_t buzzer_time;///<蜂鸣时间
	uint16_t pass_time;///<PASS时间
    uint16_t arc_mode;///<电弧侦测模式
	uint8_t date[22];///<存放日期时间 xxxx.xx.xx xx:xx:xx
}TEST_FILE;
/**
  * @brief  屏幕尺寸枚举定义
  */
typedef enum{
	SCREEN_4_3INCH,///<4.3寸屏
	SCREEN_6_5INCH,///<5.6寸屏
	SCREEN_7INCH,///<7寸屏
    SCREEN_NUM,///<支持的屏幕个数
}SCREEM_SIZE_T;
/**
  * @brief  显示布局枚举定义
  */
typedef enum{
	COMPOSITION_1,///<布局1
	COMPOSITION_2,///<布局2
	COMPOSITION_3,///<布局3
}UI_LAYOUT_T;
/**
  * @brief  系统支持的语言枚举定义
  */
typedef enum{
    CHINESE,///<中文
    ENGLISH,///<英文
    LANGUAGE_NUM,///<语言个数
}LANGUAGE_T;
/**
  * @brief  开关变量状态枚举定义
  */
typedef enum{
    SW_OFF,
    SW_ON,
}SW_STATUS;
/**
  * @brief  系统参数结构定义
  */
typedef struct{
	SCREEM_SIZE_T   screem_size;///< 屏幕尺寸 7inch 5.6inch 4.3inch
	UI_LAYOUT_T     ui_composition;///<界面布局
    LANGUAGE_T      language;///<语言
    uint8_t         password[PWD_MAX_LEN + 1];///<系统密码
    uint32_t        instrument_type;///<仪器型号
}SYS_PAR;
/**
  * @brief  系统标志结构定义
  */
typedef struct{
	uint32_t power_on_count;///<开机次数计数
	uint32_t mem_init;///<存储器初始化标志 非零时要初始化
	FILE_NUM last_file_num;///< 最近使用的记忆组文件编号
}SYS_FLAG;
/**
  * @brief  档位信息结构定义
  */
typedef struct {
    uint8_t* name;///<档位名称
    UNIT_T unit;///<单位
    uint32_t high_max;///<档位的上限最大值
    uint32_t high_min;///<档位的上限最小值
    uint32_t low_max;///<档位的下限最大值
    uint32_t low_min;///<档位的下限最小值
    uint8_t lon;///<最大长度，包括小数点
    uint8_t decs;///<档位的小数点位数
}GEAR_STR;

#ifdef   STRUCT_GLOBALS
#define  STRUCT_EXT
#else
#define  STRUCT_EXT  extern
#endif

#define     TABLE_VALUE_NULL                ((uint16_t)0xffff)

STRUCT_EXT uint8_t step_used_flag[MAX_STEPS/8 + 1 + 1];///<步骤已使用的标记列表
STRUCT_EXT uint16_t cur_group_table[MAX_STEPS];///<记忆组步骤地址映射表
STRUCT_EXT TEST_FILE *g_cur_file;///<当前文件
STRUCT_EXT TEST_FILE file_pool[MAX_FILES];///< 文件池
STRUCT_EXT TEST_FILE global_file;///< 全局文件实体用于界面通信
STRUCT_EXT SYS_PAR sys_par;///<系统参数
STRUCT_EXT SYS_FLAG sys_flag;///<系统参数
STRUCT_EXT NODE_STEP * g_cur_step;///<当前步指针
STRUCT_EXT TESTGROUP test_step_buf;///<测试步缓冲区
STRUCT_EXT CS_LIST list_head_99xx;///<测试步链表头

extern GEAR_STR ac_gear[];
extern GEAR_STR dc_gear[];
extern GEAR_STR ir_gear[];


extern CS_ERR check_step_data(NODE_STEP*step);
extern CS_BOOL is_file_exist(FILE_NUM file_num);
extern CS_ERR check_file_data(TEST_FILE*file);
extern void init_file_data(TEST_FILE *file, FILE_NUM file_num);
extern void clear_file_data(TEST_FILE *file, FILE_NUM file_num);
extern void clear_all_file_data(void);
extern void init_all_file(void);
extern TEST_FILE* get_file_inf(FILE_NUM file_num, CS_ERR* err);
extern void init_sys_par(void);
extern void check_sys_par(CS_ERR *err);
extern uint8_t pwd_max_len(void);
extern void set_cur_file(FILE_NUM file_num);
extern void init_instrument_data(void);
extern void swap_step(const STEP_NUM one, const STEP_NUM two);
extern void insert_step(uint16_t pos, uint8_t mode);
extern void del_step(STEP_NUM step);
extern void load_steps_to_list(const int16_t step, uint8_t step_num);
extern NODE_STEP *get_g_cur_step(void);
extern void init_mode(NODE_STEP *p);
extern void transform_test_port_to_str(TEST_PORT *port, uint8_t *buf);
extern void transform_str_to_test_port(TEST_PORT *port, uint8_t *buf);
extern uint16_t transform_arc_cur_to_grade(uint16_t arc_cur_val);
extern uint16_t transform_arc_grade_to_cur(uint16_t gear);

#endif //__CS99XX_STRUCT_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
