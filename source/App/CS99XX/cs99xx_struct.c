/**
  ******************************************************************************
  * @file    cs99xx_struct.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   仪器相关结构体定义
  ******************************************************************************
  */

#define STRUCT_GLOBALS
#include "cs99xx_struct.h"
#include "string.h"
#include "rtc_config.h"
#include "cs99xx_mem_api.h"
#include "type/cs99xx_type.h"


/**
  * @brief  定义位图表用来查询步存储空间未使用位置
  */
static uint8_t  const  cs99xx_maptbl[256] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x00 to 0x0F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x10 to 0x1F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x20 to 0x2F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0x30 to 0x3F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x40 to 0x4F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x50 to 0x5F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x60 to 0x6F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,       /* 0x70 to 0x7F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x80 to 0x8F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x90 to 0x9F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xA0 to 0xAF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0xB0 to 0xBF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xC0 to 0xCF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0xD0 to 0xDF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xE0 to 0xEF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 0        /* 0xF0 to 0xFF */
};
/**
  * @brief  AC档位信息
  */
GEAR_STR ac_gear[]=
{
    {0},//0
    {"2~A"   , "2.000~A", "0.000~A", CUR_U_uA, 2000, 1, 2000, 0, 3},//2uA
    {"20~A"  , "20.00~A", "00.00~A", CUR_U_uA, 2000, 1, 2000, 0, 2},//20uA
    {"200~A" , "200.0~A", "000.0~A", CUR_U_uA, 2000, 1, 2000, 0, 1},//200uA
    {"2mA"   , "2.000mA", "0.000mA", CUR_U_mA, 2000, 1, 2000, 0, 3},
    {"10mA"  , "10.00mA", "00.00mA", CUR_U_mA, 1000, 1, 1000, 0, 2},
    {"20mA"  , "20.00mA", "00.00mA", CUR_U_mA, 2000, 1, 2000, 0, 2},
    {"50mA"  , "50.00mA", "00.00mA", CUR_U_mA, 5000, 1, 5000, 0, 2},
    {"100mA" , "100.0mA", "000.0mA", CUR_U_mA, 1000, 1, 1000, 0, 1},
    {"200mA" , "200.0mA", "000.0mA", CUR_U_mA, 2000, 1, 2000, 0, 1},
    {"1.25A" , "1.250A" , "1.250A" , CUR_U_A , 1250, 1, 1250, 0, 3},//2A
};
/**
  * @brief  DC档位信息
  */
GEAR_STR dc_gear[]=
{
    {0},//0
    {"2~A"   , "2.000~A", "0.000~A", CUR_U_uA, 2000, 1, 2000, 0, 3},//2uA
    {"20~A"  , "20.00~A", "00.00~A", CUR_U_uA, 2000, 1, 2000, 0, 2},//20uA
    {"200~A" , "200.0~A", "000.0~A", CUR_U_uA, 2000, 1, 2000, 0, 1},//200uA
    {"2mA"   , "2.000mA", "0.000mA", CUR_U_mA, 2000, 1, 2000, 0, 3},
    {"10mA"  , "10.00mA", "00.00mA", CUR_U_mA, 1000, 1, 1000, 0, 2},
    {"20mA"  , "20.00mA", "00.00mA", CUR_U_mA, 2000, 1, 2000, 0, 2},
    {"50mA"  , "50.00mA", "00.00mA", CUR_U_mA, 5000, 1, 5000, 0, 2},
    {"100mA" , "100.0mA", "000.0mA", CUR_U_mA, 1000, 1, 1000, 0, 1},
};
/**
  * @brief  IR档位信息
  */
GEAR_STR ir_gear[]=
{
    {0},//0
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 3},//IR_10MOHM
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 2},//IR_100MOHM
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 1},//IR_1GOHM
    {"" , "", "", RES_U_GOHM, 0, 0, 0, 0, 3},//IR_10GOHM
    {"" , "", "", RES_U_GOHM, 0, 0, 0, 0, 2},//IR_100GOHM
};
/**
  * @brief  默认文件定义
  */
TEST_FILE default_file = 
{
	0,
	"DEFAULT",/* name */
	N_MODE,	
	0,
	10,
	0,
	1,
    0,
};

/**
  * @brief  判断文件是否存在
  * @param  [in] file_num 文件编号
  * @retval CS_FALSE 文件不存在 CS_TRUE 文件存在
  */
CS_BOOL is_file_exist(FILE_NUM file_num)
{
	if(file_num >= MAX_FILES)
	{
		return CS_FALSE;
	}
	
	if(file_pool[file_num].name[0] == 0)
	{
		return CS_FALSE;
	}
    
	if(file_pool[file_num].name[0] > 0X80)
	{
		return CS_FALSE;
    }
	
	return CS_TRUE;
}
/**
  * @brief  检查步骤数据参数的合法性
  * @param  [in] step 步骤信息
  * @retval 正常CS_ERR_NONE
  */
CS_ERR check_step_data(UN_STRUCT *step)
{
    CS_ERR err = CS_ERR_NONE;
    
    return err;
}
/**
  * @brief  检查文件数据参数的合法性
  * @param  [in] file 文件指针
  * @retval 正常CS_ERR_NONE
  * @retval CS_ERR_FILE_NUM_INVALID 文件编号无效
  * @retval CS_ERR_FILE_NAME_INVALID 文件名非法
  * @retval CS_ERR_STRING_TOO_LONG 文件名太长
  * @retval CS_ERR_DATA_OUT_OF_RANGE 数据超出范围
  * @retval CS_ERR_DATE_STR_TOO_LONG 日期字符串太长
  */
CS_ERR check_file_data(TEST_FILE*file)
{
    CS_ERR err = CS_ERR_NONE;
    
    if(file->num > MAX_FILES)
    {
        err = CS_ERR_FILE_NUM_INVALID;
    }
    else if(file->name[0] == 0)
    {
        err = CS_ERR_FILE_NO_EXIST;
    }
    else if(file->name[0] > 0x80)
    {
        err = CS_ERR_FILE_NAME_INVALID;
    }
    else if(strlen((const char*)file->name) > NAME_LON)
    {
        err = CS_ERR_STRING_TOO_LONG;
    }
    else if(file->work_mode != N_MODE && file->work_mode != G_MODE)
    {
        err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    else if(file->total > MAX_STEPS)
    {
        err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    else if(file->buzzer_time > 9999)
    {
        err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    else if(file->pass_time > 9999)
    {
        err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    else if(strlen((const char*)file->date) > 22)
    {
        err = CS_ERR_DATE_STR_TOO_LONG;
    }
    
    return err;
}
/**
  * @brief  初始化文件数据
  * @param  [in] file 文件指针
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void init_file_data(TEST_FILE *file, FILE_NUM file_num)
{
    TEST_FILE f={0,"DEFAULT", N_MODE, 1, 0,0,"2017-5-10 17:59:59"};
    
    f.num = file_num;
    
    strcpy((char *)f.date, (const char*)get_time_str(0));
    
    memcpy(file, &f, sizeof(TEST_FILE));
}
/**
  * @brief  清除文件数据
  * @param  [in] file 文件指针
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void clear_file_data(TEST_FILE *file, FILE_NUM file_num)
{
    memset(file, 0, sizeof(TEST_FILE));
    file->num = file_num;
}
/**
  * @brief  清除所有文件数据
  * @param  无
  * @retval 无
  */
void clear_all_file_data(void)
{
    int32_t i = 0;
    
    memset(file_pool, 0, sizeof(file_pool));
    
    for(i = 0; i < MAX_FILES; i++)
    {
        file_pool[i].num = i;
    }
}
/**
  * @brief  初始化所有文件
  * @param  无
  * @retval 无
  */
void init_all_file(void)
{
    clear_all_file_data();
    init_file_data(&file_pool[0], 0);//初始化默认文件
    save_all_file();
}
/**
  * @brief  获取文件信息
  * @param  [in] file_num 文件编号
  * @param  [out] err 错误码 CS_ERR_NONE ，CS_ERR_DATA_OUT_OF_RANGE 
  * @retval TEST_FILE* 文件指针
  */
TEST_FILE* get_file_inf(FILE_NUM file_num, CS_ERR* err)
{
    *err = CS_ERR_NONE;
    
    if(file_num)
    {
        *err = CS_ERR_DATA_OUT_OF_RANGE;
        return NULL;
    }
    
    return &file_pool[file_num];
}
/**
  * @brief  检查系统参数的合法性
  * @param  [out] err 错误码 CS_ERR_NONE ，CS_ERR_DATA_OUT_OF_RANGE 
  * @retval 无
  */
void check_sys_par(CS_ERR *err)
{
    *err = CS_ERR_NONE;
    
    if(sys_par.screem_size > SCREEN_END)
    {
        *err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    
    if(sys_par.language != CHINESE && sys_par.language != ENGLISH)
    {
        *err = CS_ERR_DATA_OUT_OF_RANGE;
    }
    
    if(strlen((const char*)sys_par.password) > PWD_MAX_LEN)
    {
        *err = CS_ERR_DATA_OUT_OF_RANGE;
    }
}
/**
  * @brief  初始化系统参数
  * @param  无
  * @retval 无
  */
void init_sys_par(void)
{
    sys_par.screem_size = SCREEN_7INCH;
    sys_par.language = CHINESE;
    memcpy(sys_par.password, DEFAULT_PWD, sizeof(sys_par.password));
    save_sys_par();
}
/**
  * @brief  清空步骤空间使用标记
  * @param  无
  * @retval 无
  */
void clear_step_used_flag(void)
{
    memset(step_used_flag, 0, sizeof step_used_flag);
}
/**
  * @brief  清空记忆组中的步骤空间映射表
  * @param  无
  * @retval 无
  */
void clear_group_table(void)
{
    memset(cur_group_table, 0xff, sizeof cur_group_table);
}
/**
  * @brief  初始化文件表
  * @param  无
  * @retval 无
  */
void init_file_table(void)
{
	uint8_t i = 0;
    
	memset(file_pool, 0, sizeof(file_pool[0]) * MAX_FILES);
    
	for(i = 0; i < MAX_FILES; i++)
	{
		file_pool[i].num = i;
	}
}

/**
  * @brief  清空一个文件的数据
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void clear_one_file(FILE_NUM file_num)
{
    if(file_num >= MAX_FILES)
    {
        return;
    }
    
    memset(&file_pool[file_num], 0, sizeof file_pool[0]);
    
	file_pool[file_num].num = file_num;
}

/**
  * @brief  删除一个文件
  * @param  [in] file_num 文件编号
  * @retval 无
  */
void del_file(const FILE_NUM file_num)
{
    if(file_num >= MAX_FILES)
    {
        return;
    }
    
    clear_step_used_flag();
    clear_group_table();
    
    clear_one_file(file_num);
    
    save_group_info(file_num);
    
    if(g_cur_file->num == file_num)
    {
        g_cur_file = &file_pool[0];
        
        set_cur_file(g_cur_file->num);
    }
    
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
}

/**
  * @brief  删除所有的文件
  * @param  无
  * @retval 无
  */
void del_all_file(void)
{
    int32_t i = 0;
    
    default_file = file_pool[0];
    init_file_table();
    
    clear_step_used_flag();
    clear_group_table();
    
    for(i = 1; i < MAX_FILES; i++)
    {
        if(file_pool[i].name[0] != 0)
        {
            save_step_used_flag(i);
            save_group_table(i);
        }
    }
    
    file_pool[0] = default_file;
    g_cur_file = &file_pool[0];
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
    
    save_all_file();
    
    set_cur_file(0);
}
/**
  * @brief  初始化ACW的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_acw_step(NODE_STEP * p)
{
	ACW_STRUCT 	l_acw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	defined_cur_kinds(ACW, l_gear_buf, l_flag);
	
	memset(&l_acw, 0, sizeof l_acw);
	
	l_acw.step = p->one_step.com.step;
	l_acw.mode = ACW;
	l_acw.testing_voltage = ACW_VOL_L;
	l_acw.range = l_flag[0];
	l_acw.upper_limit = 500;
	l_acw.lower_limit = 0;
	l_acw.ac_real_cur = 0;
	l_acw.arc_sur = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_acw.output_freq = 1;/* 50hz */
	}
	/* Á¬ÐøÄ£Ê½ */
	else
	{
		l_acw.output_freq = 500;/* 50hz */
	}
	
	l_acw.rise_time = type_spe.rise_time_min_value;
	l_acw.testing_time = 30;
	l_acw.fall_time = 0;
	l_acw.interval_time = 0;
	l_acw.steps_pass = 1;
	l_acw.steps_cont = 0;
	
	l_acw.offset_cur = 0;
	l_acw.offset_real = 0;
	l_acw.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_acw.steps_pass = 0;
		l_acw.steps_cont = 1;
	}
    
    l_acw.port.num = type_spe.port_num;
    
	p->one_step.acw = l_acw;
}
/**
  * @brief  初始化CC的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_cc_step(NODE_STEP * p)
{
	CC_STRUCT 	l_cc;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	defined_cur_kinds(CC, l_gear_buf, l_flag);
	
	memset(&l_cc, 0, sizeof l_cc);
	
	l_cc.step = p->one_step.com.step;
	l_cc.mode = CC;
	l_cc.testing_voltage = CC_VOL_L;
	l_cc.range = l_flag[0];
	l_cc.upper_limit = 500;//ac_gear[get_cc_max_gear()].high_max;//;
	l_cc.lower_limit = 50;
	l_cc.ac_real_cur = 0;
	l_cc.cur_intensity = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_cc.output_freq = 1;/* 50hz */
	}
	else
	{
		l_cc.output_freq = 500;/* 50hz */
	}
	
	l_cc.rise_time = 0;
	l_cc.testing_time = 1;
	l_cc.fall_time = 0;
	l_cc.interval_time = 0;
	l_cc.steps_pass = 1;
	l_cc.steps_cont = 0;
	
	l_cc.offset_cur = 0;
	l_cc.offset_real = 0;
	l_cc.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_cc.steps_pass = 0;
		l_cc.steps_cont = 1;
	}
    
    l_cc.port.num = type_spe.port_num;
    
	p->one_step.cc = l_cc;
}
/**
  * @brief  初始化DCW的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_dcw_step(NODE_STEP * p)
{
	DCW_STRUCT 	l_dcw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	
	l_kinds = defined_cur_kinds(DCW, l_gear_buf, l_flag);
	memset(&l_dcw, 0, sizeof l_dcw);
	
	l_dcw.step = p->one_step.com.step;
	l_dcw.mode = DCW;
	l_dcw.testing_voltage = DCW_VOL_L;
	l_dcw.range = l_flag[l_kinds-1];
	l_dcw.upper_limit = 500;
	l_dcw.lower_limit = 0;
	l_dcw.charge_cur = 0;
	l_dcw.arc_sur = 0;
	l_dcw.delay_time = 0;
	l_dcw.rise_time = 0;
	l_dcw.stab_time = 0;
	l_dcw.testing_time = 30;
	l_dcw.fall_time = 0;
	l_dcw.interval_time = 0;
	l_dcw.steps_pass = 1;
	l_dcw.steps_cont = 0;
	
	l_dcw.offset_cur = 0;
	l_dcw.offset_result = 0;
	
	if(g_cur_file->work_mode == G_MODE)
	{
		l_dcw.steps_pass = 0;
		l_dcw.steps_cont = 1;
	}
    
    l_dcw.port.num = type_spe.port_num;
	p->one_step.dcw = l_dcw;
}
/**
  * @brief  初始化IR的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_ir_step(NODE_STEP * p)
{
	IR_STRUCT 	l_ir;
	
	memset(&l_ir, 0, sizeof l_ir);
	
	l_ir.step = p->one_step.com.step;
	l_ir.mode = IR;
	l_ir.testing_voltage = IR_VOL_L;
	l_ir.auto_shift_gears = 1;
	l_ir.upper_limit = 0;
	l_ir.lower_limit = IR_RES_L;
	l_ir.rise_time = 0;
	l_ir.testing_time = 30;
	l_ir.delay_time = 0;
	l_ir.interval_time = 0;
	l_ir.steps_pass = 1;
	l_ir.steps_cont = 0;
	
    l_ir.port.num = type_spe.port_num;
	p->one_step.ir = l_ir;
}
/**
  * @brief  初始化GR的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_gr_step(NODE_STEP * p)
{
	GR_STRUCT 	l_gr;
	const uint8_t *l_gear_buf[VOL_KINDS_MAX];
	uint8_t l_flag[VOL_KINDS_MAX];
	
	memset(l_flag, 0, sizeof l_flag);
	memset(&l_gr, 0, sizeof l_gr);
	defined_vol_kinds(GR, l_gear_buf, l_flag);
	
	l_gr.step = p->one_step.com.step;
	l_gr.mode = GR;
	l_gr.voltage_gear = l_flag[0];
	l_gr.testing_cur = GR_CUR_L;
	l_gr.upper_limit = 1000;
	l_gr.lower_limit = 0;
	l_gr.testing_time = 30;
	l_gr.interval_time = 0;
	l_gr.steps_pass = 1;
	l_gr.steps_cont = 0;
    
	if(type_spe.gr_hz_type == HZ_TYPE_GRADE)
	{
		l_gr.output_freq = 1;
	}
	else
	{
		l_gr.output_freq = 500;
	}
    
	l_gr.test_method = DC_GR_CONT;
	
	l_gr.offset_res = 0;
	l_gr.offset_result = 0;
	
	p->one_step.gr = l_gr;
}

/**
  * @brief  初始化BBD的步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_bbd_step(NODE_STEP * p)
{
	BBD_STRUCT 	l_bbd;
	
	memset(&l_bbd, 0, sizeof l_bbd);
	
	l_bbd.step = p->one_step.com.step;
	l_bbd.mode = BBD;
	l_bbd.testing_voltage = 100;
	l_bbd.open_ratio = 50;
	l_bbd.short_ratio = 120;
	l_bbd.cap_value = 0;
	l_bbd.testing_time = 30;
	l_bbd.interval_time = 0;
	l_bbd.steps_pass = 1;
	l_bbd.steps_cont = 0;
	l_bbd.output_freq = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_bbd.output_freq = 9;/* 400hz */
	}
	else
	{
		l_bbd.output_freq = 4000;/* 400hz */
	}
    
	l_bbd.gear = AC_20mA;
	l_bbd.get_cs_ok = 0;
	l_bbd.offset_cap = 0;
	l_bbd.offset_result = 0;
	
    l_bbd.port.num = type_spe.port_num;
	p->one_step.bbd = l_bbd;
}

/**
  * @brief  初始化步骤参数
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void init_mode(NODE_STEP *p)
{
	switch(p->one_step.com.mode)
	{
		case ACW:
			init_acw_step(p);
			break;
		case DCW:
			init_dcw_step(p);
			break;
		case IR:
			init_ir_step(p);
			break;
		case GR:
			init_gr_step(p);
			break;
		case BBD:
			init_bbd_step(p);
			break;
        case CC:
            init_cc_step(p);
            break;
	}
}
/**
  * @brief  插入一步到指定的位置
  * @param  [in/out] p 步骤参数 
  * @retval 无
  */
void insert_step(uint16_t pos, uint8_t mode)
{
	NODE_STEP node;
    uint16_t l_index;
    int16_t i = 0;
    uint16_t free_step = TABLE_VALUE_NULL;
    
    if(pos > (MAX_STEPS-1))
    {
        return;
    }
    
    if(g_cur_file == NULL)
    {
        return;
    }
    
    if(g_cur_file->total == 0)
    {
        clear_step_used_flag();
    }
    
    for(i = 0; i < (MAX_STEPS/8+1); i++)
    {
        l_index = step_used_flag[i];
        
        if(l_index != 0xff)
        {
            free_step = cs99xx_maptbl[l_index];
            step_used_flag[i] |= (1<<free_step);
            free_step = free_step + i * 8;
            break;
        }
    }
    
    if(free_step == TABLE_VALUE_NULL)
    {
        return;
    }
    
    for(i = g_cur_file->total; i > pos; i--)
    {
        cur_group_table[i] = cur_group_table[i - 1];
    }
    
    cur_group_table[pos] = free_step;
    
    g_cur_file->total++;
    
    node.one_step.com.step = pos + 1;
	node.one_step.com.mode = mode;
    
    init_mode(&node);
    save_one_step(&node, node.one_step.com.step);
}

/**
  * @brief  设置最近使用的记忆组文件编号
  * @param  无
  * @retval 无
  */
void set_cur_file(FILE_NUM file_num)
{
    sys_flag.last_file_num = file_num;
    save_sys_flag();
}
/**
  * @brief  初始化仪器数据
  * @param  无
  * @retval 无
  */
void init_instrument_data(void)
{
    init_all_file();//初始化所有文件
    set_cur_file(0);//将默认文件设为当前文件
    init_sys_par();//初始化系统参数
}
/************************ (C) COPYRIGHT Nanjing Changsheng 2017 *****END OF FILE****/
