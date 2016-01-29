/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   主程序
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
 * Description    : 程序入口
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
 * Description    : 切换页初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExchangePageInit( void )
{
	/* 初始化键值 */
	InitKeyCheck();
	
	/* 初始化异常标志 */
	ClearExceptionFlag();
	
	bsp_StartTimer(GENERAL_TIMEOUT_TIMING,100);
	while (1)
	{	
		/* 通讯循环体：
		   目的：串口缓存满了之后，后面接收的数据将被丢弃。
				 此时，缓存的数据并不是实时采样数据，通过执行
				一次本函数，将清空串口缓存，重新接收新的采样数据
		*/
		comm_cycle();

		/* 按键循环体：
			内部包含警告检测、警告清除代码。需要进入界面前再次
			被检测。
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
 * Description    : 页处理
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


