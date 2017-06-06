
#include "Key_LED.h"


#define   NUM_OF_4094            (2)                 //4094����������
#define		C4094_DLY			(10)

#define  GPIO_PORT_IN1			GPIOC
#define  GPIO_PIN_IN1			GPIO_Pin_3

#define  GPIO_PORT_IN2			GPIOC
#define  GPIO_PIN_IN2			GPIO_Pin_2

#define  GPIO_PORT_IN3			GPIOC
#define  GPIO_PIN_IN3			GPIO_Pin_1

#define  GPIO_PORT_IN4			GPIOC
#define  GPIO_PIN_IN4			GPIO_Pin_0


#define  GPIO_PORT_START		GPIOF
#define  GPIO_PIN_START			GPIO_Pin_11

#define  GPIO_PORT_STOP			GPIOB
#define  GPIO_PIN_STOP			GPIO_Pin_2

#define  C4094_ST_PORT           GPIOA
#define  C4094_ST_PIN            GPIO_Pin_5
#define  C4094_ST_SET()          ((GPIO_TypeDef *)C4094_ST_PORT)->BSRRL = C4094_ST_PIN
#define  C4094_ST_CLR()          ((GPIO_TypeDef *)C4094_ST_PORT)->BSRRH = C4094_ST_PIN

#define  C4094_CLK_PORT          GPIOA
#define  C4094_CLK_PIN           GPIO_Pin_6
#define  C4094_CLK_SET()         ((GPIO_TypeDef *)C4094_CLK_PORT)->BSRRL = C4094_CLK_PIN
#define  C4094_CLK_CLR()         ((GPIO_TypeDef *)C4094_CLK_PORT)->BSRRH = C4094_CLK_PIN

#define  C4094_DATA_PORT         GPIOA 
#define  C4094_DATA_PIN          GPIO_Pin_4
#define  C4094_DATA_SET()        ((GPIO_TypeDef *)C4094_DATA_PORT)->BSRRL = C4094_DATA_PIN  
#define  C4094_DATA_CLR()        ((GPIO_TypeDef *)C4094_DATA_PORT)->BSRRH = C4094_DATA_PIN  

#define  PASSLED_POS             (1<<0)   
#define  FAILLED_POS             (1<<1)   
#define  FMQ_POS                 (1<<2) 

#define  TESTLED_PORT            GPIOI
#define  TESTLED_PIN             GPIO_Pin_2

static DEVICE_CTRL_INFO	device_ctrl_info;

void set_buzzer_on_time(uint32_t time)
{
	device_ctrl_info.buzzer_time = time;
	set_buzzer(BUZZER_ON);
}

uint32_t get_buzzer_time(void)
{
	return device_ctrl_info.buzzer_time;
}

uint32_t sub_buzzer_time(void)
{
	if(device_ctrl_info.buzzer_time > 0)
	{
		if(--device_ctrl_info.buzzer_time == 0)
		{
			set_buzzer(BUZZER_OFF);
		}
	}
	
	return 0;
}

static void C4094_Delay(uint32_t t)
{
	while(t--);
}

static  uint8_t   C4094_data_pool[NUM_OF_4094];   //������������
static  void  C4094_Output(void)
{
	uint8_t i;
    
	C4094_ST_CLR();
    
	for(i=0;i<(NUM_OF_4094*8);i++)
	{	
		C4094_CLK_CLR();
        
		if(C4094_data_pool[NUM_OF_4094-1-(i/8)] & (0x80>>(i%8)))
		{
			C4094_DATA_SET();
		}
		else
		{
			C4094_DATA_CLR();
		}
        
		C4094_Delay(C4094_DLY);
		C4094_CLK_SET();
		C4094_Delay(C4094_DLY);
	}
    
	C4094_ST_SET();
}

//C4094�������
/*******************************
��������  C4094_Write
��  ����  Index��оƬ������
          data�� Ҫ���������

����ֵ��  ��
********************************/
static void C4094_Write(uint8_t Index,uint8_t data)
{
	if(Index >= NUM_OF_4094)
	{
		return;
	}
	
	C4094_data_pool[Index] = data;
	C4094_Output();
}



//PASS�ƿ��ƺ���
/*******************************
��������  Set_LED_PASSLED
��  ����  state��Ŀ��״̬  ON or OFF
����ֵ��  ��
********************************/
void Set_LED_PASSLED(uint8_t state)
{
	if(state)
	{
		C4094_data_pool[1] |= PASSLED_POS;
	}
	else
	{
		C4094_data_pool[1] &= ~PASSLED_POS;
	}
	
	C4094_Output();
}

//FAIL�ƿ��ƺ���
/*******************************
��������  Set_LED_FAILLED
��  ����  state��Ŀ��״̬  ON or OFF
����ֵ��  ��
********************************/
void Set_LED_FAILLED(uint8_t state)
{
	if(state)
	{
		C4094_data_pool[1] |= FAILLED_POS;
	}
	else
	{
		C4094_data_pool[1] &= ~FAILLED_POS;
	}
	
	C4094_Output();
}

//TEST�ƿ��ƺ���
/*******************************
��������  Set_LED_TESTLED
��  ����  state��Ŀ��״̬  ON or OFF
����ֵ��  ��
********************************/
void Set_LED_TESTLED(uint8_t state)
{
	if(state)
	{
		((GPIO_TypeDef *)TESTLED_PORT)->BSRRL = TESTLED_PIN ;
	}
	else
	{
		((GPIO_TypeDef *)TESTLED_PORT)->BSRRH = TESTLED_PIN ;
	}
}

//���������ƺ���
/*******************************
��������  Set_BUZZER
��  ����  state��Ŀ��״̬  ON or OFF
����ֵ��  ��
********************************/
void set_buzzer(uint8_t state)
{
	if(state == BUZZER_ON)
	{
		C4094_data_pool[1] |= FMQ_POS;
	}
	else
	{
		C4094_data_pool[1] &= ~FMQ_POS;
	}
	
	C4094_Output();
}

//��ֵ��ȡ����
/*******************************
��������  KeyValue_Read
��  ����  ��
����ֵ��  32λ�ļ�ֵ
********************************/
uint32_t KeyValue_Read(void)
{
	uint8_t readtemp,i;
	uint32_t KeyValue = 0;
	for(i = 0; i < 8; i++)
	{
		C4094_Write(0, ~(1<<i));
        C4094_Delay(10);
		readtemp = 0;
		readtemp|=(GPIO_ReadInputDataBit(GPIO_PORT_IN1,GPIO_PIN_IN1)?0:0x01);
		readtemp|=(GPIO_ReadInputDataBit(GPIO_PORT_IN2,GPIO_PIN_IN2)?0:0x02);
		readtemp|=(GPIO_ReadInputDataBit(GPIO_PORT_IN3,GPIO_PIN_IN3)?0:0x04);
		readtemp|=(GPIO_ReadInputDataBit(GPIO_PORT_IN4,GPIO_PIN_IN4)?0:0x08);	
		KeyValue <<= 4;
		KeyValue |= readtemp;
	}
	
// 	KeyValue |= (GPIO_ReadInputDataBit(GPIO_PORT_START,GPIO_PIN_START)?0:0x10000000);
// 	KeyValue |= (GPIO_ReadInputDataBit(GPIO_PORT_STOP,GPIO_PIN_STOP)  ?0:0x00000001);
	return KeyValue;
}


/*
*********************************************************************************************************
*	�� �� ��: C4094_CTRLine_Init
*	����˵��: C4094��ʼ��Ӳ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void C4094_CTRLine_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG, ENABLE);
	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN1;
	GPIO_Init(GPIO_PORT_IN1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN2;
	GPIO_Init(GPIO_PORT_IN2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN3;
	GPIO_Init(GPIO_PORT_IN3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IN4;
	GPIO_Init(GPIO_PORT_IN4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_START;
	GPIO_Init(GPIO_PORT_START, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STOP;
	GPIO_Init(GPIO_PORT_STOP, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Pin = C4094_ST_PIN;
	GPIO_Init(C4094_ST_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = C4094_CLK_PIN;
	GPIO_Init(C4094_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = C4094_DATA_PIN;
	GPIO_Init(C4094_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TESTLED_PIN;
	GPIO_Init(TESTLED_PORT, &GPIO_InitStructure);
	
	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* ���� EXTI Line8 �� PG8 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource8);
	
	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line    = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}


void Key_LED_Control_Init(void)
{
	C4094_CTRLine_Init();
	C4094_data_pool[0] = 0;
	C4094_data_pool[1] = 0;
	C4094_Output();
}

extern void Key_Stop_Dispose(void);
void Key_Stop_Irq(void)
{
//	Key_Stop_Dispose();
}


