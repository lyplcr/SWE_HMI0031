/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "initial.h"
#include "PageManage.h"


/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void PageProcessCycle( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : main
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
int main( void )
{	
	InitSystem();	
	
	while (1)
	{			
		PageProcessCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ExchangePageInit
 * Description    : �л�ҳ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExchangePageInit( void )
{
	/* ��ʼ����ֵ */
	InitKeyCheck();
	
	/* ��ʼ���쳣��־ */
	ClearExceptionFlag();
	
	bsp_StartTimer(GENERAL_TIMEOUT_TIMING,100);
	while (1)
	{	
		/* ͨѶѭ���壺
		   Ŀ�ģ����ڻ�������֮�󣬺�����յ����ݽ���������
				 ��ʱ����������ݲ�����ʵʱ�������ݣ�ͨ��ִ��
				һ�α�����������մ��ڻ��棬���½����µĲ�������
		*/
		comm_cycle();

		/* ����ѭ���壺
			�ڲ����������⡢����������롣��Ҫ�������ǰ�ٴ�
			����⡣
		*/
		PutinProcessCycle();
		
		if (GetCoreStatus() != STATUS_CORE_WARNING)
		{
			return;
		}
		
		if (bsp_CheckTimer(GENERAL_TIMEOUT_TIMING))
		{
			return;
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : PageProcessCycle
 * Description    : ҳ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PageProcessCycle( void )
{
	static pFunction pTCBPage = NULL;
	
	while (1)
	{
		ExchangePageInit();
		
		pTCBPage = GetPage();
		
		pTCBPage();
	}
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


