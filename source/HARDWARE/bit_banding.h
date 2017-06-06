// Header	:
// File Name:bit_banding.h
// Author	:***
// Date		:2014/04/04
// Explain	:	
#ifndef __BIT_BANDING_H__
#define __BIT_BANDING_H__


//M4�ں˵�GPIO�Ĵ�������ַ��
#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx) || defined (STM32F401xx) || defined (STM32F40XX) || defined (STM32F427X)
//��Щ�궨����ʹ�����¿⺯��ʱ�������ж���

#include "stm32f4xx.h"

#define GPIO_REG_ADDR_BASE		0x40020000ul
#define GPIO_REG_ADDR_OFFSET	0x400ul

#define GPIO_ODR_OFFSET			0x14ul
#define GPIO_IDR_OFFSET			0x10ul

#endif

//M3�ں˵�GPIO�Ĵ�������ַ
#if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_XL) || defined (STM32F10X_CL) 

#include "stm32f10x.h"

#define GPIO_REG_ADDR_BASE		0x40010800ul
#define GPIO_REG_ADDR_OFFSET	0x400ul

#define GPIO_ODR_OFFSET			0x0Cul
#define GPIO_IDR_OFFSET			0x08ul

#endif


//�������Ļ���ַ��ĩ��ַ����M3��M4����һ���ģ�λ��ַ�ļ��㹫ʽҲ��һ����

//����������Ļ���ַ
#define BIT_BAND_ALIAS_PERIPHERAL_BASE	0x42000000ul

#define BIT_BAND_ALIAS_PERIPHERAL_END	0x43FFFFFFul

//SRAM����������ַ
#define BIT_BAND_ALIAS_SRAM_BASE		0x22000000ul
//
#define BIT_BAND_ALIAS_SRAM_END			0x23FFFFFFul


//��bitband���� λ��ַ�ļ��㹫ʽ
#define BIT_WORD_ADDR(ByteAddr, BitNumber)	((ByteAddr&0xF0000000) + 0x2000000 + (ByteAddr&0x000FFFFF)*32 + (BitNumber&0x0000000F)*4)


//����ַת����ָ�룬�������ܶ�д��ַ�е�ֵ
#define CONVERT_TO_PTR(BitWordAddr)		( *( (volatile unsigned long *)BitWordAddr ) )

//BitNumber<16
//���
#define PAout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOA_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PBout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOB_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PCout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOC_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PDout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOD_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PEout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOE_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PFout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOF_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PGout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOG_BASE + GPIO_ODR_OFFSET, BitNumber) )
#define PHout(BitNumber)	CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOH_BASE + GPIO_ODR_OFFSET, BitNumber) )
//������
#define PAin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOA_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PBin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOB_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PCin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOC_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PDin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOD_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PEin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOE_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PFin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOF_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PGin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOG_BASE + GPIO_IDR_OFFSET, BitNumber) )
#define PHin(BitNumber)		CONVERT_TO_PTR( BIT_WORD_ADDR(GPIOH_BASE + GPIO_IDR_OFFSET, BitNumber) )



#define START_PIN   PGin(7)
#define STOP_PIN    PGin(8)

#endif

