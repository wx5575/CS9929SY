
#define USB_MANAGER_GLOBALS
#include "cs99xx_usb_manager.h"
#include "CH376_USB_UI.H"
#include "cs99xx_struct.h"
#include "rtc_config.h"
#include "stdio.h"
#include "app.h"
#include "main_win/main_win.h"
#include "GUI.H"
#include "mem_alloc.h"


uint8_t g_long_file_name[100];//长文件名
uint8_t g_tar_name[50];//短文件名

/*
 * 函数名：check_usb_flash
 * 描述  ：u盘检查
 * 输入  ：void
 * 输出  ：无
 * 返回  ：无
 */
int32_t check_usb_flash(void)
{
    int32_t i = 0;
    int32_t s = 0;
    
    
    /* 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的 */
    while(1)
    {
        for (i = 0; i < 100; i ++ )
		{
            /* 最长等待时间,100*50mS */
			s = CH376DiskMount();  /* 初始化磁盘并测试磁盘是否就绪 */
			if (s == USB_INT_SUCCESS)
			{
				break;  /* 准备好 */
			}
			else if (s == ERR_DISK_DISCON)
			{
				break;  /* 检测到断开,重新检测并计时 */
			}
            
			if (CH376GetDiskStatus() >= DEF_DISK_MOUNTED && i >= 5)
			{
				break;  /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
			}
		}
        
        /* 检测到断开,重新检测并计时 */
		if (s == ERR_DISK_DISCON)
		{
			continue;
		}
        /* 未知USB设备,例如USB键盘、打印机等 */
		if (CH376GetDiskStatus( ) < DEF_DISK_MOUNTED)
		{
			return -1;
		}
        
        return 0;
    }
}


uint8_t check_connect_usb(uint8_t strong_brush)
{
	if(1 == Query376Interrupt() || strong_brush)
	{
		if(CH376DiskConnect() == USB_INT_SUCCESS)
		{
			//u盘连接成功
			if(check_usb_flash() == 0)
			{
                set_cur_ch376_status(1);
                update_usb_dis_status();
			}
		}
		else
		{
            set_cur_ch376_status(0);
            update_usb_dis_status();
		}
	}
	
	return get_cur_ch376_status();
}




unsigned int Creat_LongFile_Name(unsigned char *file_Shortname,unsigned char *ModelName)
{
	unsigned int j;
	unsigned int s;
	uint8_t LongNameBuf[256];
	
	// 复制并转换长文件名到LongNameBuf长文件名缓冲区 
	for ( j = 0; ModelName[j] != 0; j++ )
	{
		LongNameBuf[j*2] = ModelName[j]; // 将英文字符转换为两字节的小端UNICODE编码 
		LongNameBuf[j*2 + 1] = 0x00;
	}
	
	LongNameBuf[j*2] = 0x00;   // 末尾用两个0表示结束
	LongNameBuf[j*2 + 1] = 0x00; // 复制该长文件名的ANSI编码的全路径的短文件名(8+3格式),短文件名由长文件名生成(只要不冲突,其实随便定)
	s = CH376CreateLongName(file_Shortname, LongNameBuf); // 新建具有长文件名的文件 L_NAME
	
	return (s);
}

uint8_t set_file_create_time(uint8_t* file_name, uint16_t date, uint16_t time)
{
	FAT_DIR_INFO fdi;
	uint8_t res = 0;
	
	res = CH376FileOpenPath((uint8_t*)file_name);/* 打开文件 */
	
	if(0x14 != res)
	{
		return 0xff;
	}
	
	//1.用CMD1H_DIR_INFO_READ读取当前文件结构到内存
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.通过CMD01_RD_USB_DATA0命令从内存缓冲区读出数据
	CH376ReadBlock((uint8_t*)&fdi);
	//3.通过CMD_DIR_INFO_READ命令读取FAT_DIR_INFO结构到缓冲区
	//4.通过CMD20_WR_OFS_DATA命令向内部缓冲区指定偏移地址写入修改后的数据
	
	fdi.DIR_CrtTime = time;/* 0EH,文件创建的时间 */
	fdi.DIR_CrtDate = date;/* 10H,文件创建的日期 */
	
	fdi.DIR_WrtTime = time;/* 16H,文件修改时间,参考前面的宏 MAKE_FILE_TIME */
	fdi.DIR_WrtDate = date;/* 18H,文件修改日期,参考前面的宏 MAKE_FILE_DATE */
	
	CH376WriteOfsBlock((uint8_t*)&fdi, 0, sizeof fdi );
	//5.通过CMD_DIR_INFO_SAVE命令向USB存储设备
	CH376DirInfoSave( );
	
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.通过CMD01_RD_USB_DATA0命令从内存缓冲区读出数据
	CH376ReadBlock((uint8_t*)&fdi);
	
	CH376FileClose(TRUE);
	
	return 0x14;
}
uint8_t set_file_update_time(uint8_t* file_name, uint16_t date, uint16_t time)
{
	FAT_DIR_INFO fdi;
	uint8_t res = 0;
	
	res = CH376FileOpenPath((uint8_t*)file_name);/* 打开文件 */
	
	if(0x14 != res)
	{
		return 0xff;
	}
	
	//1.用CMD1H_DIR_INFO_READ读取当前文件结构到内存
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.通过CMD01_RD_USB_DATA0命令从内存缓冲区读出数据
	CH376ReadBlock((uint8_t*)&fdi);
	//3.通过CMD_DIR_INFO_READ命令读取FAT_DIR_INFO结构到缓冲区
	//4.通过CMD20_WR_OFS_DATA命令向内部缓冲区指定偏移地址写入修改后的数据
	
	fdi.DIR_WrtTime = time;/* 16H,文件修改时间,参考前面的宏 MAKE_FILE_TIME */
	fdi.DIR_WrtDate = date;/* 18H,文件修改日期,参考前面的宏 MAKE_FILE_DATE */
	
	CH376WriteOfsBlock((uint8_t*)&fdi, 0, sizeof fdi );
	//5.通过CMD_DIR_INFO_SAVE命令向USB存储设备
	CH376DirInfoSave( );
	
	CH376FileClose(TRUE);
	
	return 0x14;
}
uint8_t SetFileCreateTime(uint8_t* file_name)
{
	uint16_t year = get_rtc_year();
	uint16_t month = get_rtc_month();
	uint16_t day = get_rtc_day();
	uint16_t hour = get_rtc_hour();
	uint16_t minute = get_rtc_minute();
	uint16_t second = get_rtc_second();
	
	return set_file_create_time(file_name, MAKE_FILE_DATE(year, month, day), MAKE_FILE_TIME(hour, minute, second));
}

uint8_t create_long_file_name(uint8_t *file_name, uint8_t *path, uint8_t *tar_name, uint8_t *long_file_name)
{
    uint8_t len = 0;
    uint8_t res = 0;
    
    tar_name[0] = 0;
    long_file_name[0] = 0;
//     strcat((char*)tar_name, "/");
    strcat((char*)tar_name, (const char*)path);
//     strcat((char*)tar_name, "/");
    
    len = strlen((const char*)file_name);
	
    if(len > 12)
    {
        strncat((char*)tar_name, (const char*)file_name, 8);
        strncat((char*)tar_name, (const char*)(file_name + (len - 4)), 4);
    }
    else
    {
        strncat((char*)tar_name, (const char*)file_name, len);
    }
    
    strcpy((char*)long_file_name, (const char*)file_name);
    return res;
}
uint8_t create_file_usb_flash(uint8_t *file_name, uint8_t *path, uint8_t *tar_name, uint8_t *long_file_name)
{
	uint8_t res;
    
    /* 创建目录 */
//     strcpy((char*)tar_name, "/");
    tar_name[0] = 0;
    strcat((char*)tar_name, (const char*)path);
    res = CH376DirCreatePath((uint8_t*)tar_name);
    
    if(res != USB_INT_SUCCESS)
    {
        return res;
    }
	
    create_long_file_name(file_name, path, tar_name, long_file_name);
	res = Creat_LongFile_Name((uint8_t *)tar_name, (uint8_t *)long_file_name);
	
	if(res == 0x14)
	{
		res = SetFileCreateTime((uint8_t*)tar_name);
	}
	
// 	res = CH376FileOpenPath((uint8_t*)tar_name);/* 打开文件 */
// 	
// 	if(0x14 != res)
// 	{
// 		return 0xff;
// 	}
    
// 	/* 写入标题 */
//     strcpy((char*)buf, Test_File_TITLE);
//     res = CH376ByteWrite((uint8_t *)buf, strlen((const char*)buf), NULL );
// 	
// 	if(res == 0x14)
// 	{
// 	}
	
//     CH376FileClose(TRUE);
	
	return res;
}

uint8_t new_file_in_usb_flash(uint8_t *file_name, uint8_t *path)
{
    uint8_t name_buf[100];
    uint8_t path_buf[100];
    
    strcpy((char*)name_buf, (const char*)file_name);
    strcpy((char*)path_buf, (const char*)path);
    
	return create_file_usb_flash(name_buf, path_buf, g_tar_name, g_long_file_name);
}

uint8_t close_file_in_usb_disk(void)
{
    return CH376FileClose(TRUE);
}
uint8_t open_file_in_usb_disk(uint8_t *file_name, uint8_t *path)
{
    uint8_t name_buf[100];
    uint8_t path_buf[100];
    uint8_t res = 0;
    
    strcpy((char*)name_buf, (const char*)file_name);
    strcpy((char*)path_buf, (const char*)path);
    
    create_long_file_name(name_buf, path_buf, g_tar_name, g_long_file_name);
    
    res = CH376FileOpenPath((uint8_t*)g_tar_name);/* 打开文件 */
    
	return res;
}

uint8_t write_file_in_usb_disk(uint8_t *file_name, uint8_t *path)
{
    uint8_t name_buf[100];
    uint8_t path_buf[100];
    uint8_t res = 0;
    
    strcpy((char*)name_buf, (const char*)file_name);
    strcpy((char*)path_buf, (const char*)path);
    
    create_long_file_name(name_buf, path_buf, g_tar_name, g_long_file_name);
    
    res = CH376FileOpenPath((uint8_t*)g_tar_name);/* 打开文件 */
    
	return res;
}

void usb1_server_task(void)
{
    int i = 0;
    
    i ++;
    
}

void set_usb_disk_task(uint8_t cmd)
{
    if(get_ch376_status(CH376_USB_1))
    {
        usb_exe_task = cmd;
    }
    
    if(get_ch376_status(CH376_USB_2))
    {
        usb_exe_task = cmd;
    }
}

uint8_t *ex_addr;
uint32_t count;

void write_byte_2_file(uint8_t data, void *p)
{
    if(ex_addr != NULL)
    {
        ex_addr[count++] = data;
    }
}
void read_dis_ram(uint16_t _usX, uint16_t _usY, uint16_t *buf);
void usb2_server_task(void)
{
    uint8_t res = 0;
    int32_t i = 0;
    uint8_t buf[50]={"a12345678936.bmp"};
    uint32_t c = 0;
    
	switch(usb_exe_task)
	{
		case USB_SCREEN_CAPTURE:
            res = open_file_in_usb_disk(buf, "/");
            
            if(0x14 != res)
            {
                res = new_file_in_usb_flash(buf, "/");
                
                if(0x14 != res)
                {
                    usb_exe_task = USB_TASK_NULL;
                    return;
                }
                
                res = open_file_in_usb_disk(buf, "/");
                
                if(0x14 != res)
                {
                    usb_exe_task = USB_TASK_NULL;
                    return;
                }
            }
            
            ex_addr = malloc_ex_mem(2*800*480);
            count = 0;
            GUI_BMP_Serialize(write_byte_2_file, NULL);
            
            c = count / (1024 * 3);
            set_main_win_progbar_show();//显示进度条
            
            for(i = 0; i < c; i++)
            {
                set_main_win_progbar_value(i * 100 / c);//设置进度条进度
                res = CH376LongNameWrite((uint8_t *)(ex_addr + (1024 * 3) * i), (1024 * 3));
                
                if(0x14 != res)
                {
                    res = res;
                }
            }
            
            if(count > ((1024 * 3) * i))
            {
                res = CH376LongNameWrite((uint8_t *)(ex_addr + (1024 * 3) * i),
                                count - (1024 * 3) * i);
                
                if(0x14 != res)
                {
                    res = res;
                }
            }
            
            delete_main_win_progbar();//删除进度条
            free_ex_mem(ex_addr);
            close_file_in_usb_disk();
			usb_exe_task = USB_TASK_NULL;
			break;
    }
}


// void usb_server_task(void)
// {
// 	static RESULT_STR *p_result = NULL;
//     uint8_t err = 0;
// 	
// 	/* 如果文件名不存在，就创建文件 */
// 	if(g_tar_name[0] == 0)
// 	{
// 		create_file_save_result();
// 	}
// 	
// 	switch(usb_exe_task)
// 	{
// 		case USB_LEADING_OUT_RESULT:
// 			leading_out_result_usb();
// 			app_flag.leading_watch_dog = 1;
// 			usb_exe_task = USB_TASK_NULL;
// 			break;
// 		case USB_SAVE_ONE_RESULT:
// 			if(p_result != NULL)
// 			{
// 				usb_save_one_result_ch376(p_result);
// 				p_result = NULL;
// 			}
// 			usb_exe_task = USB_TASK_NULL;
// 			break;
// 		case USB_LEADING_OUT_SYS_DATA:
// 			leading_out_sub_task();
// 			usb_exe_task = USB_TASK_NULL;
// 			break;
// 		case USB_LEADING_IN_SYS_DATA:
// 			leading_in_sub_task();
// 			usb_exe_task = USB_TASK_NULL;
// 			break;
// 		default:
// 			p_result = OSQPend(ResultQSem,10,&err);
// 			if(p_result != NULL)
// 			{
// 				usb_save_one_result_ch376(p_result);
// 				p_result = NULL;
// 				usb_exe_task = USB_TASK_NULL;
// 			}
// 			break;
// 	}
// }


UINT32	total;		/* 记录当前缓冲在FILE_DATA_BUF中的数据长度 */
UINT32	NewSize;	/* 临时变量 */
UINT8  FileDataBuf[1024*3+256];	/* 文件缓冲区,太小则速度慢 */

// /* 将准备写入U盘的零碎数据进行集中缓冲,组合成大数据块时再通过CH376真正写入U盘 */
// /* 这样做的好处是: 提高速度(因为大数据块写入时效率高), 减少U盘损耗(U盘内部的内存寿命有限,不宜频繁擦写) */

void write_data_to_disk_buf(uint8_t *buf, uint32_t num)
{
	if(total >= sizeof( FileDataBuf))
	{
		total = 0;
		return;
	}
	
	if(total + num  >= sizeof( FileDataBuf))
	{
		mFlushBufferToDisk(1);
	}
	
	memcpy((char*)(FileDataBuf + total), buf, num);
	total += num;
}
void clear_disk_info(void)
{
	total = 0;
	NewSize = 0;
}

/* force = 0 则自动刷新(检查缓冲区中的数据长度,满则写盘,不满则暂时放在缓冲区中), force != 0 则强制刷新(不管缓冲区中的数据有多少都写盘,通常在系统关机前应该强制写盘) */
uint8_t mFlushBufferToDisk(uint8_t force)
{
	UINT8	s = 0x14;
	
	/* 强制刷新 */
	if ( force )
	{
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;
		/* 将缓冲区中的字节数转换为扇区数(除以DEF_SECTOR_SIZE),长度加上DEF_SECTOR_SIZE-1用于确保写入最后的零头数据 */
		/* 有数据 */
		if ( s )
		{  
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* 以扇区为单位向文件写入数据,写入缓冲区中的所有数据,含最后的零头 */
			if(s != 0X14)
			{
				return 0xff;
			}
			
			/*有些U盘可能会要求在写数据后等待一会才能继续操作,所以,如果在某些U盘中发生数据丢失现象,建议在每次写入数据后稍
			作延时再继续 */
			mDelaymS( 1 );  /* 写后延时,可选的,大多数U盘不需要 1 1111 1111
															  10 0000 0001*/
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* 将刚才已写入U盘的零头数据复制到缓冲区的头部 */
			total &= DEF_SECTOR_SIZE - 1;
			/* 缓冲区中只剩下刚才已写入U盘的零头数据,继续保留在缓冲区中是为了方便以后在其后面追加数据 */
		}
		
		NewSize = CH376GetFileSize( );  /* 读取当前文件长度,如果没有零头数据,那么文件长度是DEF_SECTOR_SIZE的倍数 */
		
		if ( total )
		{
			NewSize -= DEF_SECTOR_SIZE - total;  /* 以扇区为单位,有零头数据,计算出真正的文件长度(有效数据的长度) */
		}
		
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* 将正确的当前文件长度写入CH376内存 */
		
		s = CH376SecWrite( FileDataBuf, 0, NULL ); 
		/* 写0长度,实际是刷新文件长度,将CH376内存中当前文件长度信息真正写入U盘或者SD卡中 */
		if(s != 0X14)
		{
			return 0xff;
		}
		
		s = CH376SecLocate( 0xFFFFFFFF );
		if(s != 0X14)
		{
			return 0xff;
		}
		
	/*重新回到原文件的尾部,以扇区为单位,所以会忽略文件尾部的零头数据,下面如果再写入数据将覆盖尾部零头数据,不过该零头数据
	有一份副本保留在缓冲区中,所以请放心 */
	}
	/* 缓冲区中的数据快要满了,所以应该先将缓冲区中的原有数据写入U盘 */
	else if ( total + 2 * DEF_SECTOR_SIZE  >= sizeof( FileDataBuf) )
	{
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );
		/* 将缓冲区中的字节数转换为扇区数(除以DEF_SECTOR_SIZE),最后的零头数据先不管,以扇区为单位向文件写入数据 */
		if(s != 0X14)
		{
			return 0xff;
		}
		
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );
		/* 将刚才未写入U盘的零头数据复制到缓冲区的头部 */
		total &= DEF_SECTOR_SIZE - 1;  /* 缓冲区中只剩下刚才未写入U盘的零头数据 */
/* 
注意,此处U盘或者SD卡中的文件长度仍然是以前的值,即此时突然断电,那么虽然物理上的数据已在U盘/SD卡中,但文件长度没有包括这些数
据,导致常归应用程序无法取得数据 */
	}
	
	return s;
}



