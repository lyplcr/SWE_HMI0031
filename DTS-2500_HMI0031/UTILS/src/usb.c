/**
  ******************************************************************************
  * @file    usb.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   USB应用层文件
  ******************************************************************************
  * @attention
  *		USE_USB_OTG_HS:通过“Target Option”-- “C/C++”中宏定义。
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "usb.h"
#include "usbh_bsp_msc.h"

/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FATFS fs;            // Work area (file system object) for logical drive 


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : bsp_InitUSB
 * Description    : USB初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void bsp_InitUSB( void )
{	
	  /* Init Host Library */
	#ifdef USE_USB_OTG_FS
		USBH_Init(&USB_OTG_Core,
			USB_OTG_FS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
	#else
		USBH_Init(&USB_OTG_Core,
			USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
	#endif
	
	USB_ReadyCycle();
	
	f_mount(&fs,"1:/",1);	/* 注册设备 */
	
	USB_ReadyCycle();
}

/*------------------------------------------------------------
 * Function Name  : LinkUSBCycle
 * Description    : 连接USB循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LinkUSBCycle( void )
{
	USBH_Process(&USB_OTG_Core, &USB_Host);
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
