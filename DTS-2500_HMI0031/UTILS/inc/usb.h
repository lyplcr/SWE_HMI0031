/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_H
#define __USB_H

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_InitUSB( void );
void LinkUSBCycle( void );
ErrorStatus Get_USB_Status( void );


#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
