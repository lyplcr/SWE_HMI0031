/**
  ******************************************************************************
  * @file    usb.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   USBӦ�ò��ļ�
  ******************************************************************************
  * @attention
  *		USE_USB_OTG_HS:ͨ����Target Option��-- ��C/C++���к궨�塣
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

static void USB_ReadyCycle( void );

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : bsp_InitUSB
 * Description    : USB��ʼ��
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
	
	f_mount(&fs,"1:/",1);	/* ע���豸 */
	
	USB_ReadyCycle();
}

/*------------------------------------------------------------
 * Function Name  : LinkUSBCycle
 * Description    : ����USBѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LinkUSBCycle( void )
{
	USBH_Process(&USB_OTG_Core, &USB_Host);
}

/*------------------------------------------------------------
 * Function Name  : Get_USB_Status
 * Description    : ��ȡUSB����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus Get_USB_Status( void )
{
	if ( HCD_IsDeviceConnected(&USB_OTG_Core) )
	{
		return SUCCESS;
	}
	return ERROR;
}

/*------------------------------------------------------------
 * Function Name  : USB_ReadyCycle
 * Description    : USB׼��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void USB_ReadyCycle( void )
{
	uint32_t num = 0;
	const uint32_t USB_STATUS_CYCLE_NUM = 50000;		//USBѭ�������ﵽ�˴������ܲ���
	
	while (num < USB_STATUS_CYCLE_NUM)
	{
		num++;
		
		USBH_Process(&USB_OTG_Core, &USB_Host);	//ִ��һ�������ſ��Ըı��дU��״̬λ
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
