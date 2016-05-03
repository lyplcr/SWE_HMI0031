/**
  ******************************************************************************
  * @file    bsp.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   底层驱动文件
  ******************************************************************************
  * 版本号  日期        作者         说明       
  *
  *	V1.0.1 	2016-2-19	SY			增加计算函数运行时间的函数，ResetFunctionRunTime()、
  *									GetFunctionRunTime()、PrintFunctionRunTime()
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "netconf.h"
#include "usb.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : BSP_Init
 * Description    : 初始化底层驱动
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void BSP_Init( void )
{	
  	bsp_InitTimer();			/* 初始化定时器 		*/
  	bsp_InitCOM();				/* 初始化串口 		*/
	bsp_InitPrint();			/* 初始化打印机 		*/
	bsp_InitIO();				/* 初始化IO			*/	
	bsp_InitPcf8563();			/* 初始化时钟 		*/
	bsp_InitSFlash();			/* 初始化FLASH 		*/
	bsp_InitEthernet();			/* 初始化以太网		*/
	bsp_InitUSB();				/* 初始化USB			*/
	bsp_InitLCD();				/* 初始化LCD			*/
	bsp_InitFont();				/* 初始化字体		*/
	bsp_InitDWT();				/* 初始化DWT 		*/
	
	#if (ENABLE_BEEP == 0x01U)
		BEEP_START();
	#endif
}

/*------------------------------------------------------------
 * Function Name  : BSP_CPU_ClkFreq
 * Description    : 获取时钟频率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);
	
    return ((uint32_t)rcc_clocks.HCLK_Frequency);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
