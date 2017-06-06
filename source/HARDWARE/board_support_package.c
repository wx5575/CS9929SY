/**
  ******************************************************************************
  * @file    board_support_package.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.5.12
  * @brief   板级支持
  ******************************************************************************
  */
#include "board_support_package.h"
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
#include "mem_alloc.h"
#include "led.h"
#include "timer.h"
#include "GUI.h"
#include "ff.h"
#include "Key_LED.h"

void bsp_init(void)
{
    CS99xx_Peripheral_Config();
    CS99xx_GPIO_Config();
//	mem_init(SRAMEX);
//	mem_init(SRAMIN);
	Key_LED_Control_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//中断分组配置
	ra8875_bsp_Init();
	rtc_init();
    coded_disc_init();
    init_flash();
}

/*
 * 函数名：CS99xx_GPIO_Config
 * 描述  ：整机IO配置
 * 输入  ：空
 * 输出  ：空
 */
void CS99xx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD
						 | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH
						 | RCC_AHB1Periph_GPIOI, ENABLE);
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* A0,A1,A2,A3,A4,A5,A6,A7,A8,A9 */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FSMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
	                            GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* A10,A11,A12,A13,A14,A15 */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* A16,A17,A18 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* A19,A20 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* D0,D1,D2,D3 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* D4,D5,D6,D7,D8,D9,D10,D11,D12 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* D13,D14,D15 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	
	/* NE1 (CS) */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* NE2 (CS) */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* NE3 (CS) */
// 	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_FSMC);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* NE4 (CS) */
// 	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* NOE (RD), NWE (WR) */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/***************** 板子画错了 *******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	//校准开关
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/******************************************************************************************/
	/*	接按键及状态灯的 MC4094 引脚配置 */
	/* KEY_IN1 C3
	   KEY_IN2 C2
	   KEY_IN3 C1
	   KEY_IN4 C0 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* 4094-CLK KEY_EXT1 A6
	   4094-ST  KEY_EXT2 A5
	   4094-DATA KEY_EXT3 A4 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	/*******************************************************************************************/
	
	
	/* 码盘中断引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//L
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//R
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource3);
	
	/*短路中断*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//short_int
	GPIO_Init(GPIOG, &GPIO_InitStructure);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource15);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//short_int
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//arc_int
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//short_int
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource10);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//gfi_int
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource8);
	
	/* 启动键和停止键引脚 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;//S , E
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource8);
	
	/******************************************************************************************/
	/* 主板继电器的 MC4094 引脚配置 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	/******************************************************************************************/
	/* SD 卡 引脚配置 */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

	/* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PD.02 CMD line */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure PC.12 pin: CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*****************************************************************************************/
	/* SPI FLASH 引脚配置 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */
	/* SPI CS1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* SPI CS2, CS3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* SPI CS4 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	/* SPI CLK, DO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* SPI WP */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	/* SPI DI */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  /* 上拉电阻使能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	GPIO_SetBits(GPIOC,GPIO_Pin_5);
	GPIO_SetBits(GPIOH,GPIO_Pin_5);
}


/*
 * 函数名：CS99xx_Peripheral_Config
 * 描述  ：整机外设配置
 * 输入  ：空
 * 输出  ：空
 */
void CS99xx_Peripheral_Config(void)
{
	FSMC_NORSRAMInitTypeDef  init;
	FSMC_NORSRAMTimingInitTypeDef  timingWrite;
	FSMC_NORSRAMTimingInitTypeDef  timingRead;

	/******************************************************************************************/
	/* LCD FSMC 外设配置 */
	/* 使能FSMC时钟 */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	
	timingWrite.FSMC_AddressSetupTime = 4;
	timingWrite.FSMC_AddressHoldTime = 0;
	timingWrite.FSMC_DataSetupTime = 6;
	timingWrite.FSMC_BusTurnAroundDuration = 1;
	timingWrite.FSMC_CLKDivision = 0;
	timingWrite.FSMC_DataLatency = 0;
	timingWrite.FSMC_AccessMode = FSMC_AccessMode_A;
	
	timingRead.FSMC_AddressSetupTime = 4;
	timingRead.FSMC_AddressHoldTime =  0;
	timingRead.FSMC_DataSetupTime = 8;
	timingRead.FSMC_BusTurnAroundDuration = 1;
	timingRead.FSMC_CLKDivision = 0;
	timingRead.FSMC_DataLatency = 0;
	timingRead.FSMC_AccessMode = FSMC_AccessMode_A;

	/*
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable
	*/
	init.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	/* 注意旧库无这个成员 */
	init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

	init.FSMC_ReadWriteTimingStruct = &timingRead;
	init.FSMC_WriteTimingStruct = &timingWrite;

	FSMC_NORSRAMInit(&init);

	/* - BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
	
	
	/******************************************************************************************/
	/* 外部 SRAM FSMC 外设配置 */
	timingWrite.FSMC_AddressSetupTime = 3;		/* 设置为2会出错; 3正常 */
	timingWrite.FSMC_AddressHoldTime = 0;
	timingWrite.FSMC_DataSetupTime = 2;			/* 设置为1出错，2正常 */
	timingWrite.FSMC_BusTurnAroundDuration = 1;
	timingWrite.FSMC_CLKDivision = 0;
	timingWrite.FSMC_DataLatency = 0;
	timingWrite.FSMC_AccessMode = FSMC_AccessMode_A;

	init.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	/* 注意旧库无这个成员 */
	init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	
	init.FSMC_ReadWriteTimingStruct = &timingWrite;
	init.FSMC_WriteTimingStruct = &timingWrite;
	
	FSMC_NORSRAMInit(&init);
	
	/* - BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

