/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "bsp.h"
#include "diskio.h"
#include "usbh_bsp_msc.h"	

#define MMC		0
#define USB		1

#define SECTOR_SIZE 		512U /* Block Size in Bytes */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	
	switch ( pdrv )
	{
		case MMC:
			if (SD_Init() == SD_OK)
			{
				return RES_OK;           //初始化成功
			}
			break;
		case USB:
 			if(HCD_IsDeviceConnected(&USB_OTG_Core))
 			{
 				stat &= ~STA_NOINIT;
 			}
			break;
	}
		
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	SD_Error Status = SD_OK;
	BYTE stat = USBH_MSC_OK;
	DRESULT res;
	
	switch ( pdrv )
	{
		case MMC:
			if (count == 1)
			{
				Status = SD_ReadBlock(buff, sector << 9 , SECTOR_SIZE);
			}
			else
			{
				Status = SD_ReadMultiBlocks(buff, sector << 9 , SECTOR_SIZE, count);
			}
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}

			#ifdef SD_DMA_MODE
				/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
				Status = SD_WaitReadOperation();
				if (Status != SD_OK)
				{
					return RES_ERROR;
				}

				while(SD_GetStatus() != SD_TRANSFER_OK);
			#endif
				
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}

			return RES_OK;
			
		case USB:
 			if (HCD_IsDeviceConnected(&USB_OTG_Core))
 			{
 				do
 				{
 					stat = USBH_MSC_Read10(&USB_OTG_Core, buff,sector,512 * count);
 					USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);

 					if (!HCD_IsDeviceConnected(&USB_OTG_Core))
 					{
 						break;
 					}
 				}
 				while (stat == USBH_MSC_BUSY );
 			}

 			if (stat == USBH_MSC_OK)
 			{
 				res = RES_OK;
 			}
 			else
 			{
 				res = RES_ERROR;
 			}
			return res;			
	}
			
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	SD_Error Status = SD_OK;
	BYTE stat = USBH_MSC_OK;
	DRESULT res;

	switch ( pdrv )
	{
		case MMC:
			if (count == 1)
			{
				Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);

				if (Status != SD_OK)
				{
					return RES_ERROR;
				}

				#ifdef SD_DMA_MODE
					/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
					Status = SD_WaitReadOperation();
					if (Status != SD_OK)
					{
						return RES_ERROR;
					}
					while(SD_GetStatus() != SD_TRANSFER_OK);
				#endif
				res = RES_OK;
			}
			else
			{
				/* 此处存在疑问： 扇区个数如果写 count ，将导致最后1个block无法写入 */
				//Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);
				Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count + 1);

				if (Status != SD_OK)
				{
					return RES_ERROR;
				}

				#ifdef SD_DMA_MODE
					/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
					Status = SD_WaitReadOperation();
					if (Status != SD_OK)
					{
						return RES_ERROR;
					}
					while(SD_GetStatus() != SD_TRANSFER_OK);
				#endif
				res = RES_OK;
			}
				
			return RES_OK;
			
		case USB:
 			if (HCD_IsDeviceConnected(&USB_OTG_Core))
 			{
 				do
 				{
 					stat = USBH_MSC_Write10(&USB_OTG_Core,(BYTE*)buff,sector, 512 * count);
 					USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

 					if(!HCD_IsDeviceConnected(&USB_OTG_Core))
 					{
 						break;
 					}
 				}
 				while(stat == USBH_MSC_BUSY );

 			}

 			if (stat == USBH_MSC_OK)
 			{
 				res = RES_OK;
 			}
 			else
 			{
 				res = RES_ERROR;
 			}
			return res;
	}
	
	return RES_ERROR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* get_fattime   Functions                                               */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void)
{
	tTime time;
	
	date_get(&time);
	time_get(&time);
    
	return  ((time.usYear-1980) << 25)    	// Year = 2007
            | (time.ucMon << 21)            // Month = June
            | (time.ucMday << 16)           // Day = 5
            | (time.ucHour << 11)           // Hour = 11
            | (time.ucMin << 5)            	// Min = 38
            | (time.ucSec >> 1)             // Sec = 0
            ;
}

