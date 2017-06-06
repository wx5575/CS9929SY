/* CH376芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include "stm32f4xx.h"
#include "HAL.H"


//CLK
#define CH376_SCK_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_13)
#define CH376_SCK_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_13)

//MOSI
#define CH376_SDI_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_15)
#define CH376_SDI_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_15)

//CS
#define CH376_CSS2_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_12)
#define CH376_CSS2_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_12)

#define CH376_CSS1_0()	GPIO_ResetBits(GPIOG,  GPIO_Pin_6)
#define CH376_CSS1_1()	GPIO_SetBits(GPIOG,  GPIO_Pin_6)

//MISO
#define CH376_SDO_HIGH()	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) != Bit_RESET)
//USB1_INT
#define CH376_INT1_HIGH()	(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10) != Bit_RESET)
//USB2_INT
#define CH376_INT2_HIGH()	(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9 ) != Bit_RESET)


void CH376_PORT_INIT( void );
void Spi376OutByte(UINT8 d);
UINT8 Spi376InByte( void );

typedef int (*FUN)(void);

typedef struct{
    char *name;//设备名称
    FUN css_en_fun;//片选使能函数
    FUN css_dis_fun;//片选去使能函数
    FUN read_int_fun;//读中断函数
    FUN server_fun;//用户的服务函数
    int status;//ch376的连接状态 1 连接 0未连接
    int init_st;//初始化状态
}CH376_DEVICE_T;

static int ch376_1_css_en(void)
{
    CH376_CSS1_0();
    return 0;
}

static int ch376_1_css_dis(void)
{
    CH376_CSS1_1();
    return 0;
}

static int ch376_1_read_int(void)
{
    return ((GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9 ) != Bit_RESET)? FALSE : TRUE);
}

/* ch376_1 */
CH376_DEVICE_T ch376_1={
    "ch376-1",
    ch376_1_css_en,
    ch376_1_css_dis,
    ch376_1_read_int,
};
static int ch376_2_css_en(void)
{
    CH376_CSS2_0();
    return 0;
}
static int ch376_2_css_dis(void)
{
    CH376_CSS2_1();
    return 0;
}
static int ch376_2_read_int(void)
{
    return ((GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10 ) != Bit_RESET)? FALSE : TRUE);
}

/* ch376_2 */
CH376_DEVICE_T ch376_2={
    "ch376-2",
    ch376_2_css_en,
    ch376_2_css_dis,
    ch376_2_read_int,
};

CH376_DEVICE_T *cur_ch376_chip = &ch376_1;

void set_cur_ch376_chip(unsigned char n)
{
    if(n == 1)
    {
        cur_ch376_chip = &ch376_1;
    }
    else
    {
        cur_ch376_chip = &ch376_2;
    }
}

void set_ch376_init_status(unsigned char st)
{
    cur_ch376_chip->init_st = st;
}

unsigned char get_ch376_status(unsigned char chip)
{
    if(chip == 1)
    {
        return ch376_1.status;
    }
    
    return ch376_2.status;
}

void set_cur_ch376_status(unsigned char st)
{
    cur_ch376_chip->status = st;
}

unsigned char get_cur_ch376_status(void)
{
    return cur_ch376_chip->status;
}

void mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
{
	UINT8 i;
    
	for (i = 0; i < 10; i++);
}

void xEndCH376Cmd( )
{
	cur_ch376_chip->css_dis_fun();
}

void xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
	cur_ch376_chip->css_dis_fun();/* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	mDelay0_5uS( );
	
	cur_ch376_chip->css_en_fun();/* SPI片选有效 */
	Spi376OutByte( mCmd );  /* 发出命令码 */
	
	/* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
	mDelay0_5uS( );
	mDelay0_5uS( );
	mDelay0_5uS( );
}

void xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	Spi376OutByte( mData );
}

UINT8 xReadCH376Data( void )  /* 从CH376读数据 */
{
	return( Spi376InByte());
}

/* 查询CH376中断(INT#低电平) */
UINT8	Query376Interrupt( void )
{
	return cur_ch376_chip->read_int_fun();// ? FALSE : TRUE);
}

static UINT8 mInitCH376Host_( void )
{
	UINT8	res;
    
    xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* 测试单片机与CH376之间的通讯接口 */
    xWriteCH376Data( 0x65 );
    res = xReadCH376Data();
    xEndCH376Cmd();
    
    /*
    通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),
    串口波特率,一直在复位,晶振不工作 
    */
    if ( res != 0x9A )
    {
        return( ERR_USB_UNKNOWN );  
    }
    
    xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* 设备USB工作模式 */
    xWriteCH376Data( 0x06 );
    mDelayuS(50 );
    res = xReadCH376Data( );
    xEndCH376Cmd( );
    
    /* 初始化成功 */
    if ( res == CMD_RET_SUCCESS )
    {
        return( USB_INT_SUCCESS );
    }
    /* 设置模式错误 */
    else
    {
        return( ERR_USB_UNKNOWN );
    }
}
/* 初始化CH376 */
UINT8 mInitCH376Host( void )
{
	UINT8	res;
	
	CH376_PORT_INIT( );  /* 接口硬件初始化 */
    
	mDelaymS(200);
	
    set_cur_ch376_chip(1);
    res = mInitCH376Host_();
    set_ch376_init_status(res);
	mDelaymS(200);
	
    set_cur_ch376_chip(2);
    res = mInitCH376Host_();
    set_ch376_init_status(res);
	mDelaymS(200);
	
    return( res );
}


#if 1
static void bsp_CfgSPIForCH376(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* 打开SPI时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; /* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; /* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; /* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; /* 片选控制方式：软件控制 */
    
	/* 设置波特率预分频系数 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7; /* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI2, &SPI_InitStructure);
    
	SPI_Cmd(SPI2, DISABLE); /* 先禁止SPI  */
	SPI_Cmd(SPI2, ENABLE); /* 使能SPI  */
}


void CH376_PORT_INIT( void )  
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开CPU的GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
	/* 配置片选 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
    /* CS1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
    /* CS2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* 配置中断引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
    /* INT1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;     
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* INT2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* 配置 SCK, MISO 、 MOSI */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* 配置 SCK, MISO 、 MOSI 为复用功能 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
    
    bsp_CfgSPIForCH376();//配置SPI外设
}

void Spi376OutByte( UINT8 mData )  /* 向CH376写数据 */
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    
    /* 通过SPI硬件发送1个字节 */
    SPI_I2S_SendData(SPI2, mData);
    
    /* 等待接收一个字节任务完成 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* 返回从SPI总线读到的数据 */
    SPI_I2S_ReceiveData(SPI2);
}

UINT8 Spi376InByte( void )  /* 从CH376读数据 */
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    
    /* 通过SPI硬件发送1个字节 */
    SPI_I2S_SendData(SPI2, 0xFF);
    
    /* 等待接收一个字节任务完成 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* 返回从SPI总线读到的数据 */
    return SPI_I2S_ReceiveData(SPI2);
}

#else

/* 使用软件模拟SPI读写时序,进行初始化 */
void CH376_PORT_INIT( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
    
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
    
    /* MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* NSS1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* NSS2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
    /* MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
    /* USB1_INT */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    /* USB2_INT */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
	CH376_CSS1_1();  /* 禁止SPI片选 */
	CH376_CSS2_1();  /* 禁止SPI片选 */
	CH376_SCK_1();
}


/* SPI输出8个位数据 */
void Spi376OutByte(UINT8 d)  
{  
	/* 如果是硬件SPI接口,应该是先将数据写入SPI数据寄存器,然后查询SPI状态寄存器以等待SPI字节传输完成 */
	UINT8	i;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();
		if ( d & 0x80 )
		{
			CH376_SDI_1();
		}
		else
		{
			CH376_SDI_0();
		}
        
        mDelay0_5uS();
		d <<= 1;  /* 数据位是高位在前 */
		CH376_SCK_1();  /* CH376在时钟上升沿采样输入 */
        mDelay0_5uS();
	}
}

/* SPI输入8个位数据 */
UINT8 Spi376InByte( void )
{  /* 如果是硬件SPI接口,应该是先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
	UINT8	i, d;
	d = 0;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();  /* CH376在时钟下降沿输出 */
        mDelay0_5uS();
		d <<= 1;  /* 数据位是高位在前 */
		if ( CH376_SDO_HIGH() ) d ++;
		CH376_SCK_1();
        mDelay0_5uS();
	}
	return( d );
}
#endif
