/**
  ******************************************************************************
  * @file    coded_disc.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   码盘驱动程序
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "coded_disc.h"
#include "app.h"
#include "keyboard.h"

static uint32_t SEND_MSG_RIGH = CODE_RIGH;
static uint32_t SEND_MSG_LEDT = CODE_LEFT;

#define C_DISC_RIGH_PORT    GPIOF
#define C_DISC_RIGH_PIN     GPIO_Pin_7
#define C_DISC_LEFT_PORT    GPIOF
#define C_DISC_LEFT_PIN     GPIO_Pin_6

void coded_exit_config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/*使能外部中断复用时钟*/
    
    /*映射GPIOF的Pin6至EXTILine6*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource6);  
    EXTI_ClearITPendingBit(EXTI_Line6);        //清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;      //线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;      //开外部中断
    EXTI_Init(&EXTI_InitStructure);
    
    /*映射GPIOF的Pin7至EXTILine7*/
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,GPIO_PinSource7);
    EXTI_ClearITPendingBit(EXTI_Line7);//清外部线路0中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;//线路0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//触发模式为中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//开外部中断
    EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//嵌套分组为组3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//中断通道为通道0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢断优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//开中断
    NVIC_Init(&NVIC_InitStructure);
}

void coded_disc_init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_RIGH_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(C_DISC_RIGH_PORT, &GPIO_InitStructure);//初始化
    
    GPIO_InitStructure.GPIO_Pin = C_DISC_LEFT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(C_DISC_LEFT_PORT, &GPIO_InitStructure);//初始化
    
    coded_exit_config();
    NVIC_Config();
}

void send_coded_disc_msg(uint32_t *MSG)
{
    OS_ERR err;
    
    OSQPost(&KeyboardQSem, MSG, sizeof(uint32_t), OS_OPT_POST_FIFO, &err);
}
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        send_coded_disc_msg(&SEND_MSG_LEDT);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        send_coded_disc_msg(&SEND_MSG_RIGH);
    }
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}



/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
