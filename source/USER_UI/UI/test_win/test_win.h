/**
  ******************************************************************************
  * @file    test_ui_com.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   文件保存界面
  ******************************************************************************
  */
#ifndef __TEST_UI_COM_H__
#define __TEST_UI_COM_H__


/** 
  * @brief 测试界面的文本对象
  */
typedef enum {
/** @defgroup 测试界面
  * @{
  */
	TEST_UI_FILE_NAME,///<测试界面“文件名”文本
	TEST_UI_CUR_FILE_NAME,///<测试界面当前文件名
	TEST_UI_STEP,///<测试界面"步骤"文本
	TEST_UI_CUR_STEP,///<测试界面当前步骤xx/xx
	TEST_UI_WORK_MODE,///<测试界面"工作模式"文本
	TEST_UI_CUR_WORK_MODE,///<测试界面当前工作模式 N/G
	
	TEST_UI_VOLTAGE,///<测试界面"输出电压"文本
	TEST_UI_CUR_VOLTAGE,///<测试界面当前的输出电压
	TEST_UI_RANGE,///<测试界面当前步"电流档位"文本
	TEST_UI_CUR_RANGE,///<测试界面当前电流档位
	TEST_UI_UPPER,///<测试界面"电流上限"文本
	TEST_UI_CUR_UPPER,///<测试界面当前电流上限值
	TEST_UI_LOWER,///<测试界面“电流下限”文本
	TEST_UI_CUR_LOWER,///<测试界面电流下限
	TEST_UI_ARC,///<测试界面"电弧侦测"文本
	TEST_UI_CUR_ARC,///<测试界面当前电弧侦测
	TEST_UI_CONTINUE,///<测试界面"步间连续"文本
	TEST_UI_CUR_CONTINUE,///<测试界面当前步间连续
	
	TEST_UI_TEST,///<"测试信息"文本 ACW->DCW->IR
	TEST_UI_TEST_1,///<测试信息中的第一部分"ACW"
	TEST_UI_TEST_2,///<测试信息中的第二部分"->"
	TEST_UI_TEST_3,///<测试信息中的第三部分"DCW"
	TEST_UI_TEST_4,///<测试信息中的第四部分"->"
	TEST_UI_TEST_5,///<测试信息中的第五部分"IR"
	TEST_UI_TEST_6,///<测试信息中的第六部分"->"
	TEST_UI_PORT,///<测试"端口"文本
	TEST_UI_CUR_PORT,///<当前测试端口设置 接地/浮地
	TEST_UI_ROADS,///<"多路信息"文本
	TEST_UI_CUR_ROADS,///<多路信息 XXXHHH
	
	TEST_UI_ROAD01_NUM,///<第一路的编号
	TEST_UI_ROAD01_MODE,///<第一路的模式
	TEST_UI_ROAD01_STATUS,///<第一路的状态
	TEST_UI_ROAD01_VOLTAGE,///<第一路的输出电压
	TEST_UI_ROAD01_UPPER,///<第一路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD01_REAL,///<第一路的真实电流
	TEST_UI_ROAD01_TIME,///<第一路的测试时间
	
	TEST_UI_ROAD02_NUM,///<第二路的测试编号
	TEST_UI_ROAD02_MODE,///<第二路的模式
	TEST_UI_ROAD02_STATUS,///<第二路的状态
	TEST_UI_ROAD02_VOLTAGE,///<第二路的输出电压
	TEST_UI_ROAD02_UPPER,///<第二路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD02_REAL,///<第二路的真实电流
	TEST_UI_ROAD02_TIME,///<第二路的测试时间
	
	TEST_UI_ROAD03_NUM,///<第三路的测试编号
	TEST_UI_ROAD03_MODE,///<第三路的模式
	TEST_UI_ROAD03_STATUS,///<第三路的状态
	TEST_UI_ROAD03_VOLTAGE,///<第三路的输出电压
	TEST_UI_ROAD03_UPPER,///<第三路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD03_REAL,///<第三路的真实电流
	TEST_UI_ROAD03_TIME,///<第三路的测试时间
	
	TEST_UI_ROAD04_NUM,///<第四路的测试编号
	TEST_UI_ROAD04_MODE,///<第四路的模式
	TEST_UI_ROAD04_STATUS,///<第四路的状态
	TEST_UI_ROAD04_VOLTAGE,///<第四路的输出电压
	TEST_UI_ROAD04_UPPER,///<第四路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD04_REAL,///<第四路的真实电流
	TEST_UI_ROAD04_TIME,///<第四路的测试时间
	
	TEST_UI_ROAD05_NUM,///<第五路的测试编号
	TEST_UI_ROAD05_MODE,///<第五路的模式
	TEST_UI_ROAD05_STATUS,///<第五路的状态
	TEST_UI_ROAD05_VOLTAGE,///<第五路的输出电压
	TEST_UI_ROAD05_UPPER,///<第五路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD05_REAL,///<第五路的真实电流
	TEST_UI_ROAD05_TIME,///<第五路的测试时间
	
	TEST_UI_ROAD06_NUM,///<第六路的测试编号
	TEST_UI_ROAD06_MODE,///<第六路的模式
	TEST_UI_ROAD06_STATUS,///<第六路的状态
	TEST_UI_ROAD06_VOLTAGE,///<第六路的输出电压
	TEST_UI_ROAD06_UPPER,///<第六路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD06_REAL,///<第六路的真实电流
	TEST_UI_ROAD06_TIME,///<第六路的测试时间
	
	TEST_UI_ROAD07_NUM,///<第七路的测试编号
	TEST_UI_ROAD07_MODE,///<第七路的模式
	TEST_UI_ROAD07_STATUS,///<第七路的状态
	TEST_UI_ROAD07_VOLTAGE,///<第七路的输出电压
	TEST_UI_ROAD07_UPPER,///<第七路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD07_REAL,///<第七路的真实电流
	TEST_UI_ROAD07_TIME,///<第七路的测试时间

	TEST_UI_ROAD08_NUM,///<第八路的测试编号
	TEST_UI_ROAD08_MODE,///<第八路的模式
	TEST_UI_ROAD08_STATUS,///<第八路的状态
	TEST_UI_ROAD08_VOLTAGE,///<第八路的输出电压
	TEST_UI_ROAD08_UPPER,///<第八路的测试电流 在等待测试时显示的是电流上限
	TEST_UI_ROAD08_REAL,///<第八路的真实电流
	TEST_UI_ROAD08_TIME,///<第八路的测试时间
	
	TEST_UI_RESULT,///<"测试结果"文本
	TEST_UI_RES1_NUM,///<第一条测试结果编号
	TEST_UI_RES1_VOL,///<第一条测试结果电压
	TEST_UI_RES1_CUR,///<第一条测试结果电流
	TEST_UI_RES1_TIME,///<第一条测试结果时间
	TEST_UI_RES1_RESULT,///<第一条测试结果结果
	TEST_UI_RES2_NUM,///<第二条测试结果编号
	TEST_UI_RES2_VOL,///<第二条测试结果电压
	TEST_UI_RES2_CUR,///<第二条测试结果电压
	TEST_UI_RES2_TIME,///<第二条测试结果时间
	TEST_UI_RES2_RESULT,///<第二条测试结果结果
	TEST_UI_RES3_NUM,///<第三条测试结果编号
	TEST_UI_RES3_VOL,///<第三条测试结果电压
	TEST_UI_RES3_CUR,///<第三条测试结果电流
	TEST_UI_RES3_TIME,///<第三条测试结果时间
	TEST_UI_RES3_RESULT,///<第三条测试结果
	TEST_UI_RES4_NUM,///<第四条测试结果编号
	TEST_UI_RES4_VOL,///<第四条测试结果电压
	TEST_UI_RES4_CUR,///<第四条测试结果电流
	TEST_UI_RES4_TIME,///<第四条测试结果时间
	TEST_UI_RES4_RESULT,///<第四条测试结果结果
/**
  * @}
  */
}TEST_UI_INDEX;


extern void create_test_win(int hWin);


#endif //__TEST_UI_COM_H__


/************************ (C) COPYRIGHT Nanjing Changsheng *****END OF FILE****/
