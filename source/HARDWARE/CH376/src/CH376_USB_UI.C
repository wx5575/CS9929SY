
#include "app.h"
#include "CH376_USB_UI.H"


uint8_t USB_DEVICE_INIT(void)
{
	uint8_t stat;
    
	stat = mInitCH376Host( );  /* 初始化CH376 */
	
	return stat;
}

uint8_t init_ch376(void)
{
    return USB_DEVICE_INIT();
}
uint8_t WAIT_DEVICE_CONNECT(void)
{
	int8_t stat,i,ERRCount;
	ERRCount = 0;
__WaitUsbDevice:
	  if(ERRCount > MAX_RETERY) return 0;
		ERRCount++;                //错误计数加1
	
		while (CH376DiskConnect() != USB_INT_SUCCESS)
		{ 
			mDelaymS(100 );/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		}
		mDelaymS(250);  /* 延时,可选操作,有的USB存储器需要几十毫秒的延时 */
		mDelaymS(250); 

		/* 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的 */
		for (i = 0; i < 100; i ++ )
		{  /* 最长等待时间,100*50mS */
			stat = CH376DiskMount();  /* 初始化磁盘并测试磁盘是否就绪 */
			if (stat == USB_INT_SUCCESS)
			{
				break;  /* 准备好 */
			}
			else if (stat == ERR_DISK_DISCON)
			{
				break;  /* 检测到断开,重新检测并计时 */
			}
			if (CH376GetDiskStatus() >= DEF_DISK_MOUNTED && i >= 5)
			{
				break;  /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
			}
		}
		if (stat == ERR_DISK_DISCON)
		{  /* 检测到断开,重新检测并计时 */
			goto __WaitUsbDevice;
		}
		if (CH376GetDiskStatus( ) < DEF_DISK_MOUNTED)
		{  /* 未知USB设备,例如USB键盘、打印机等 */
			return UNKNOWDEVICE;
		}
		return 1;

}

uint8_t OPEN_FILE(char *fliepath)
{
	char	SrcName[64];
	
	strcpy(SrcName, fliepath);
	mDelaymS(80);
	return(CH376FileOpenPath((uint8_t *)SrcName));  /* 打开文件,该文件在根目录下 */
}

