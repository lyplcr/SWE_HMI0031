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
#include "bsp_dwt.h"

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
uint32_t  BSP_CPU_ClkFreq (void);

/*
*********************************************************************************************************
*	函 数 名: ResetFunctionRunTime
*	功能说明: 复位函数运行时间
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
__STATIC_INLINE void ResetFunctionRunTime( void )
{
	DWT_CYCCNT = (unsigned int)0u;			
}

/*
*********************************************************************************************************
*	函 数 名: GetFunctionRunTime
*	功能说明: 获取函数运行时间（单位：us）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
__STATIC_INLINE float GetFunctionRunTime( void )
{
	uint32_t cycCnt = DWT_CYCCNT;
	float runTimeUS = 0;
	uint32_t systemClock = BSP_CPU_ClkFreq();
	const float FUNCTION_RUN_TIME = 0.029762;	//函数本身运行时间（实测）
	
	runTimeUS = cycCnt / (systemClock / 1000000.0f) - FUNCTION_RUN_TIME;
	
	return runTimeUS;			
}

/*
*********************************************************************************************************
*	函 数 名: PrintFunctionRunTime
*	功能说明: 打印函数运行时间
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
__STATIC_INLINE void PrintFunctionRunTime( void )
{
	float runTimeUS = GetFunctionRunTime();
	
	printf("runTimeUS: %f\r\n",runTimeUS);		
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
