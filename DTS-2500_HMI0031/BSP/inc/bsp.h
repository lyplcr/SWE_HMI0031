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
#define BEEP_RING_ONE()  				SetBeep(10,5,1) 	//������
#define BEEP_RING_WARN()  				SetBeep(10,10,2) 	//���뾯����
#define BEEP_WARN_PCM()					SetBeep(10,10,1) 	//����λ������
#define BEEP_START()					SetBeep(20,20,1)	//������
#define BEEP_TEST_START()				SetBeep(10,10,1)	//����ʱ���鿪ʼ��ʾ��
#define BEEP_COUNT_DOWN()				SetBeep(5,5,1)		//����ʱ������ʾ��

/* FATFS�豸 */
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
*	�� �� ��: ResetFunctionRunTime
*	����˵��: ��λ��������ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
__STATIC_INLINE void ResetFunctionRunTime( void )
{
	DWT_CYCCNT = (unsigned int)0u;			
}

/*
*********************************************************************************************************
*	�� �� ��: GetFunctionRunTime
*	����˵��: ��ȡ��������ʱ�䣨��λ��us��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
__STATIC_INLINE float GetFunctionRunTime( void )
{
	uint32_t cycCnt = DWT_CYCCNT;
	float runTimeUS = 0;
	uint32_t systemClock = BSP_CPU_ClkFreq();
	const float FUNCTION_RUN_TIME = 0.029762;	//������������ʱ�䣨ʵ�⣩
	
	runTimeUS = cycCnt / (systemClock / 1000000.0f) - FUNCTION_RUN_TIME;
	
	return runTimeUS;			
}

/*
*********************************************************************************************************
*	�� �� ��: PrintFunctionRunTime
*	����˵��: ��ӡ��������ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
__STATIC_INLINE void PrintFunctionRunTime( void )
{
	float runTimeUS = GetFunctionRunTime();
	
	printf("runTimeUS: %f\r\n",runTimeUS);		
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
