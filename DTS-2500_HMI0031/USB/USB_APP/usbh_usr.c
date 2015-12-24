/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the usb host library user callbacks
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"




USB_OTG_CORE_HANDLE   USB_OTG_Core;
USBH_HOST             USB_Host;


/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/
const char *MSG_HOST_INIT        = "> Host Library Initialized\r\n";
const char *MSG_DEV_ATTACHED     = "> Device Attached \r\n";
const char *MSG_DEV_DISCONNECTED = "> Device Disconnected\r\n";
const char *MSG_DEV_ENUMERATED   = "> Enumeration completed \r\n";
const char *MSG_DEV_HIGHSPEED    = "> High speed device detected\r\n";
const char *MSG_DEV_FULLSPEED    = "> Full speed device detected\r\n";
const char *MSG_DEV_LOWSPEED     = "> Low speed device detected\r\n";
const char *MSG_DEV_ERROR        = "> Device fault \r\n";

const char *MSG_MSC_CLASS        = "> Mass storage device connected\r\n";
const char *MSG_HID_CLASS        = "> HID device connected\r\n";
const char *MSG_DISK_SIZE        = "> Size of the disk in MBytes: \r\n";
const char *MSG_LUN              = "> LUN Available in the device:\r\n";
const char *MSG_ROOT_CONT        = "> Exploring disk flash ...\r\n";
const char *MSG_WR_PROTECT       = "> The disk is write protected\r\n";
const char *MSG_UNREC_ERROR      = "> UNRECOVERED ERROR STATE\r\n";

static uint8_t status_usb_insert = 0;

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/

//获取USB插入状态
ErrorStatus Get_USB_Status( void )
{
	if ( status_usb_insert )
	{
		return SUCCESS;
	}
	return ERROR;
}


/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
	static uint8_t startup = 0;

	if(startup == 0 )
	{
		startup = 1;
		#ifdef DEBUG_USB
			#ifdef USE_USB_OTG_HS
				printf("> USB OTG HS MSC Host\r\n");
			#else
				printf("> USB OTG FS MSC Host\r\n");
			#endif
			printf("> USB Host library started\r\n");
			printf("> USB Host Library v2.1.0\r\n" );
		#endif
		
	}
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
	#ifdef DEBUG_USB
		printf("> Device Attached\r\n");
	#endif
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
	#ifdef DEBUG_USB
		printf("> UNRECOVERED ERROR STATE\r\n");
	#endif
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
	#ifdef DEBUG_USB
		printf("> Device Disconnected\r\n");
	#endif
	status_usb_insert = 0;
}
/**
* @brief  USBH_USR_ResetUSBDevice
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
	/* callback for USB-Reset */
	#ifdef DEBUG_USB
		printf("> USBH_USR_ResetDevice\r\n");
	#endif
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
	{
		#ifdef DEBUG_USB
			printf("> High speed device detected\r\n");
		#endif
	}
	else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
	{
		#ifdef DEBUG_USB
			printf("> Full speed device detected\r\n");
		#endif
	}
	else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
	{

		#ifdef DEBUG_USB
			printf("> Low speed device detected\r\n");
		#endif
	}
	else
	{
		#ifdef DEBUG_USB
			printf("> Device fault\r\n");
		#endif
	}
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs = DeviceDesc;
	
	hs = hs;

	#ifdef DEBUG_USB
		printf("> VID : %04Xh\r\n" , (uint32_t)(*hs).idVendor);
		printf("> PID : %04Xh\r\n" , (uint32_t)(*hs).idProduct);
	#endif
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}

/*------------------------------------------------------------
 * Function Name  : USB_ReadyCycle
 * Description    : USB准备
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USB_ReadyCycle( void )
{
	uint32_t num = 0;
	const uint32_t USB_STATUS_CYCLE_NUM = 50000;		//USB循环体必须达到此次数才能操作
	
	while (num < USB_STATUS_CYCLE_NUM)
	{
		num++;
		
		USBH_Process(&USB_OTG_Core, &USB_Host);	//执行一定次数才可以改变读写U盘状态位
	}
}

/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id;
	
	id = itfDesc;

	if((*id).bInterfaceClass  == 0x08)
	{
		#ifdef DEBUG_USB
			printf("> Mass storage device connected\r\n");
		#endif
	}
	else if((*id).bInterfaceClass  == 0x03)
	{
		#ifdef DEBUG_USB
			printf("> HID device connected\r\n");
		#endif
	}
	
	status_usb_insert = 1;
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  Manufacturer String
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	#ifdef DEBUG_USB
		printf("> Manufacturer : %s\r\n", (char *)ManufacturerString);
	#endif
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
	#ifdef DEBUG_USB
		printf("> Product : %s\r\n", (char *)ProductString);
	#endif
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNum_String
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	#ifdef DEBUG_USB
		printf( "> Serial Number : %s\r\n", (char *)SerialNumString);
	#endif
}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
	/* Enumeration complete */
	#ifdef DEBUG_USB
		printf ("> Enumeration completed\r\n");
	#endif
}


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{	
	#ifdef DEBUG_USB
		printf ("> Device not supported\r\n");
	#endif
}

/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
#if 1
	/* HOST_ENUMERATION 和 HOST_CLASS_REQUEST
		在枚举成功和类请求之间等待用户输入。
		此处直接返回OK，无需等待。
	*/
	return USBH_USR_RESP_OK;
#else
	USBH_USR_Status usbh_usr_status;

	usbh_usr_status = USBH_USR_NO_RESP;

	#if 0
	/*Key B3 is in polling mode to detect user action */
	if(STM_EVAL_PBGetState(Button_KEY) == RESET)
	{
		usbh_usr_status = USBH_USR_RESP_OK;
	}
	#endif
	return usbh_usr_status;
#endif
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
	#ifdef DEBUG_USB
		printf("> Overcurrent detected\r\n");
	#endif
}

/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
	return 0;
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
	//USBH_USR_ApplicationState = USH_USR_FS_INIT;
}


/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

