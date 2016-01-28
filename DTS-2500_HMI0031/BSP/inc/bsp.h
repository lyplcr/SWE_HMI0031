/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ustdlib.h"
#include "isqrt.h"
#include "ustring.h"

#include "config.h"
#include "MY_DEBUG.H"

#include "diskio.h"
#include "ff.h"

#include "bsp_timer.h"
#include "bsp_uart.h"
#include "bsp_io.h"
#include "bsp_pcf8563.h"
#include "bsp_sdio_sd.h"
#include "bsp_crc16.h"
#include "bsp_date.h"
#include "bsp_ra8875.h"
#include "bsp_lcd.h"
#include "bsp_flash.h"
#include "bsp_spi_flash.h"
#include "bsp_uid.h"
#include "bsp_lan8720.h"
#include "bsp_print.h"
#include "bsp_rtc.h"


/* Exported define -----------------------------------------------------------*/
#define BEEP_RING_ONE()  				SetBeep(10,5,1) 	//按键声
#define BEEP_RING_WARN()  				SetBeep(10,10,2) 	//输入警告声
#define BEEP_WARN_PCM()					SetBeep(10,10,1) 	//上下位机警告
#define BEEP_START()					SetBeep(20,20,1)	//开机声
#define BEEP_TEST_START()				SetBeep(10,10,1)	//倒计时试验开始提示声
#define BEEP_COUNT_DOWN()				SetBeep(5,5,1)		//倒计时倒数提示声

/* FATFS设备 */
#define MMC		0
#define USB		1

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void BSP_Init( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
