/**
  ******************************************************************************
  * @file    bsp.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   �ײ������ļ�
  ******************************************************************************
  * @attention
  *
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
 * Description    : ��ʼ���ײ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void BSP_Init( void )
{	
  	bsp_InitTimer();			/* ��ʼ����ʱ�� 		*/
  	bsp_InitCOM();				/* ��ʼ������ 		*/
	bsp_InitPrint();			/* ��ʼ����ӡ�� 		*/
	bsp_InitIO();				/* ��ʼ��IO			*/	
	bsp_InitPcf8563();			/* ��ʼ��ʱ�� 		*/
	bsp_InitSFlash();			/* ��ʼ��FLASH 		*/
	bsp_InitEthernet();			/* ��ʼ����̫��		*/
	bsp_InitUSB();				/* ��ʼ��USB			*/
	bsp_InitLCD();				/* ��ʼ��LCD			*/
	bsp_InitFont();				/* ��ʼ������		*/
	
	#ifdef ENABLE_BEEP
		BEEP_START();
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/