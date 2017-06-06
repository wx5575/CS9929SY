
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


uint8_t g_long_file_name[100];//���ļ���
uint8_t g_tar_name[50];//���ļ���

/*
 * ��������check_usb_flash
 * ����  ��u�̼��
 * ����  ��void
 * ���  ����
 * ����  ����
 */
int32_t check_usb_flash(void)
{
    int32_t i = 0;
    int32_t s = 0;
    
    
    /* ���ڼ�⵽USB�豸��,���ȴ�100*50mS,��Ҫ�����ЩMP3̫��,���ڼ�⵽USB�豸��������DISK_MOUNTED��,���ȴ�5*50mS,��Ҫ���DiskReady������ */
    while(1)
    {
        for (i = 0; i < 100; i ++ )
		{
            /* ��ȴ�ʱ��,100*50mS */
			s = CH376DiskMount();  /* ��ʼ�����̲����Դ����Ƿ���� */
			if (s == USB_INT_SUCCESS)
			{
				break;  /* ׼���� */
			}
			else if (s == ERR_DISK_DISCON)
			{
				break;  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			}
            
			if (CH376GetDiskStatus() >= DEF_DISK_MOUNTED && i >= 5)
			{
				break;  /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
			}
		}
        
        /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
		if (s == ERR_DISK_DISCON)
		{
			continue;
		}
        /* δ֪USB�豸,����USB���̡���ӡ���� */
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
			//u�����ӳɹ�
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
	
	// ���Ʋ�ת�����ļ�����LongNameBuf���ļ��������� 
	for ( j = 0; ModelName[j] != 0; j++ )
	{
		LongNameBuf[j*2] = ModelName[j]; // ��Ӣ���ַ�ת��Ϊ���ֽڵ�С��UNICODE���� 
		LongNameBuf[j*2 + 1] = 0x00;
	}
	
	LongNameBuf[j*2] = 0x00;   // ĩβ������0��ʾ����
	LongNameBuf[j*2 + 1] = 0x00; // ���Ƹó��ļ�����ANSI�����ȫ·���Ķ��ļ���(8+3��ʽ),���ļ����ɳ��ļ�������(ֻҪ����ͻ,��ʵ��㶨)
	s = CH376CreateLongName(file_Shortname, LongNameBuf); // �½����г��ļ������ļ� L_NAME
	
	return (s);
}

uint8_t set_file_create_time(uint8_t* file_name, uint16_t date, uint16_t time)
{
	FAT_DIR_INFO fdi;
	uint8_t res = 0;
	
	res = CH376FileOpenPath((uint8_t*)file_name);/* ���ļ� */
	
	if(0x14 != res)
	{
		return 0xff;
	}
	
	//1.��CMD1H_DIR_INFO_READ��ȡ��ǰ�ļ��ṹ���ڴ�
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.ͨ��CMD01_RD_USB_DATA0������ڴ滺������������
	CH376ReadBlock((uint8_t*)&fdi);
	//3.ͨ��CMD_DIR_INFO_READ�����ȡFAT_DIR_INFO�ṹ��������
	//4.ͨ��CMD20_WR_OFS_DATA�������ڲ�������ָ��ƫ�Ƶ�ַд���޸ĺ������
	
	fdi.DIR_CrtTime = time;/* 0EH,�ļ�������ʱ�� */
	fdi.DIR_CrtDate = date;/* 10H,�ļ����������� */
	
	fdi.DIR_WrtTime = time;/* 16H,�ļ��޸�ʱ��,�ο�ǰ��ĺ� MAKE_FILE_TIME */
	fdi.DIR_WrtDate = date;/* 18H,�ļ��޸�����,�ο�ǰ��ĺ� MAKE_FILE_DATE */
	
	CH376WriteOfsBlock((uint8_t*)&fdi, 0, sizeof fdi );
	//5.ͨ��CMD_DIR_INFO_SAVE������USB�洢�豸
	CH376DirInfoSave( );
	
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.ͨ��CMD01_RD_USB_DATA0������ڴ滺������������
	CH376ReadBlock((uint8_t*)&fdi);
	
	CH376FileClose(TRUE);
	
	return 0x14;
}
uint8_t set_file_update_time(uint8_t* file_name, uint16_t date, uint16_t time)
{
	FAT_DIR_INFO fdi;
	uint8_t res = 0;
	
	res = CH376FileOpenPath((uint8_t*)file_name);/* ���ļ� */
	
	if(0x14 != res)
	{
		return 0xff;
	}
	
	//1.��CMD1H_DIR_INFO_READ��ȡ��ǰ�ļ��ṹ���ڴ�
	CH376SendCmdDatWaitInt( CMD1H_DIR_INFO_READ , 0XFF );
	//2.ͨ��CMD01_RD_USB_DATA0������ڴ滺������������
	CH376ReadBlock((uint8_t*)&fdi);
	//3.ͨ��CMD_DIR_INFO_READ�����ȡFAT_DIR_INFO�ṹ��������
	//4.ͨ��CMD20_WR_OFS_DATA�������ڲ�������ָ��ƫ�Ƶ�ַд���޸ĺ������
	
	fdi.DIR_WrtTime = time;/* 16H,�ļ��޸�ʱ��,�ο�ǰ��ĺ� MAKE_FILE_TIME */
	fdi.DIR_WrtDate = date;/* 18H,�ļ��޸�����,�ο�ǰ��ĺ� MAKE_FILE_DATE */
	
	CH376WriteOfsBlock((uint8_t*)&fdi, 0, sizeof fdi );
	//5.ͨ��CMD_DIR_INFO_SAVE������USB�洢�豸
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
    
    /* ����Ŀ¼ */
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
	
// 	res = CH376FileOpenPath((uint8_t*)tar_name);/* ���ļ� */
// 	
// 	if(0x14 != res)
// 	{
// 		return 0xff;
// 	}
    
// 	/* д����� */
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
    
    res = CH376FileOpenPath((uint8_t*)g_tar_name);/* ���ļ� */
    
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
    
    res = CH376FileOpenPath((uint8_t*)g_tar_name);/* ���ļ� */
    
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
            set_main_win_progbar_show();//��ʾ������
            
            for(i = 0; i < c; i++)
            {
                set_main_win_progbar_value(i * 100 / c);//���ý���������
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
            
            delete_main_win_progbar();//ɾ��������
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
// 	/* ����ļ��������ڣ��ʹ����ļ� */
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


UINT32	total;		/* ��¼��ǰ������FILE_DATA_BUF�е����ݳ��� */
UINT32	NewSize;	/* ��ʱ���� */
UINT8  FileDataBuf[1024*3+256];	/* �ļ�������,̫С���ٶ��� */

// /* ��׼��д��U�̵��������ݽ��м��л���,��ϳɴ����ݿ�ʱ��ͨ��CH376����д��U�� */
// /* �������ĺô���: ����ٶ�(��Ϊ�����ݿ�д��ʱЧ�ʸ�), ����U�����(U���ڲ����ڴ���������,����Ƶ����д) */

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

/* force = 0 ���Զ�ˢ��(��黺�����е����ݳ���,����д��,��������ʱ���ڻ�������), force != 0 ��ǿ��ˢ��(���ܻ������е������ж��ٶ�д��,ͨ����ϵͳ�ػ�ǰӦ��ǿ��д��) */
uint8_t mFlushBufferToDisk(uint8_t force)
{
	UINT8	s = 0x14;
	
	/* ǿ��ˢ�� */
	if ( force )
	{
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;
		/* ���������е��ֽ���ת��Ϊ������(����DEF_SECTOR_SIZE),���ȼ���DEF_SECTOR_SIZE-1����ȷ��д��������ͷ���� */
		/* ������ */
		if ( s )
		{  
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* ������Ϊ��λ���ļ�д������,д�뻺�����е���������,��������ͷ */
			if(s != 0X14)
			{
				return 0xff;
			}
			
			/*��ЩU�̿��ܻ�Ҫ����д���ݺ�ȴ�һ����ܼ�������,����,�����ĳЩU���з������ݶ�ʧ����,������ÿ��д�����ݺ���
			����ʱ�ټ��� */
			mDelaymS( 1 );  /* д����ʱ,��ѡ��,�����U�̲���Ҫ 1 1111 1111
															  10 0000 0001*/
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* ���ղ���д��U�̵���ͷ���ݸ��Ƶ���������ͷ�� */
			total &= DEF_SECTOR_SIZE - 1;
			/* ��������ֻʣ�¸ղ���д��U�̵���ͷ����,���������ڻ���������Ϊ�˷����Ժ��������׷������ */
		}
		
		NewSize = CH376GetFileSize( );  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
		
		if ( total )
		{
			NewSize -= DEF_SECTOR_SIZE - total;  /* ������Ϊ��λ,����ͷ����,������������ļ�����(��Ч���ݵĳ���) */
		}
		
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* ����ȷ�ĵ�ǰ�ļ�����д��CH376�ڴ� */
		
		s = CH376SecWrite( FileDataBuf, 0, NULL ); 
		/* д0����,ʵ����ˢ���ļ�����,��CH376�ڴ��е�ǰ�ļ�������Ϣ����д��U�̻���SD���� */
		if(s != 0X14)
		{
			return 0xff;
		}
		
		s = CH376SecLocate( 0xFFFFFFFF );
		if(s != 0X14)
		{
			return 0xff;
		}
		
	/*���»ص�ԭ�ļ���β��,������Ϊ��λ,���Ի�����ļ�β������ͷ����,���������д�����ݽ�����β����ͷ����,��������ͷ����
	��һ�ݸ��������ڻ�������,��������� */
	}
	/* �������е����ݿ�Ҫ����,����Ӧ���Ƚ��������е�ԭ������д��U�� */
	else if ( total + 2 * DEF_SECTOR_SIZE  >= sizeof( FileDataBuf) )
	{
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );
		/* ���������е��ֽ���ת��Ϊ������(����DEF_SECTOR_SIZE),������ͷ�����Ȳ���,������Ϊ��λ���ļ�д������ */
		if(s != 0X14)
		{
			return 0xff;
		}
		
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );
		/* ���ղ�δд��U�̵���ͷ���ݸ��Ƶ���������ͷ�� */
		total &= DEF_SECTOR_SIZE - 1;  /* ��������ֻʣ�¸ղ�δд��U�̵���ͷ���� */
/* 
ע��,�˴�U�̻���SD���е��ļ�������Ȼ����ǰ��ֵ,����ʱͻȻ�ϵ�,��ô��Ȼ�����ϵ���������U��/SD����,���ļ�����û�а�����Щ��
��,���³���Ӧ�ó����޷�ȡ������ */
	}
	
	return s;
}



