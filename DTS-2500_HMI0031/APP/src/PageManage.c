/**
  ******************************************************************************
  * @file    PageManage.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-10 11:03:17
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   页处理程序
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "initial.h"
#include "TestParameter.h"
#include "TestSelect.h"
#include "SearchReport.h"
#include "TestReport.h"
#include "DetailReport.h"
#include "MainPage.h"
#include "SystemSet.h"
#include "ControlParameter.h"
#include "TimeDate.h"
#include "SoftActive.h"
#include "OnlineParameter.h"
#include "UserLogin.h"
#include "SystemPassword.h"
#include "AdvanceParameter.h"
#include "ChannelSelect.h"
#include "TestAfterDispose.h"
#include "ModelSelect.h"
#include "UnitSelect.h"
#include "SystemBackup.h"
#include "SystemRestore.h"
#include "SystemInfomation.h"
#include "HardTest.h"
#include "CalibrationTable.h"
#include "LinkMode.h"
#include "CalibrationParameter.h"
#include "ForceCalibration.h"
#include "TestTypeSelect.h"
#include "CurveShow.h"
#include "FileManager.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	PAGE_NAME_TypeDef pageName;
	pFunction curPage;			//当前页
	pFunction pageArray[MAX_PAGE_NUM];
}PAGE_TCB_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static PAGE_TCB_TypeDef g_pageTCB;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : InitPage
 * Description    : 初始化页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitPage( void )
{
	g_pageTCB.pageArray[SYS_SETUP] 					= LoadSystemSetupPage;
	g_pageTCB.pageArray[MAIN_PAGE] 					= LoadMainPage;
	g_pageTCB.pageArray[TEST_PARAMETER_PAGE] 		= LoadTestParameterPage;
	g_pageTCB.pageArray[TEST_SELECT] 				= LoadTestSelectPage;
	g_pageTCB.pageArray[SEARCH_REPORT_PAGE] 		= LoadReportSearchPage;
	g_pageTCB.pageArray[TEST_REPORT_PAGE] 			= LoadTestReportPage;
	g_pageTCB.pageArray[DETAIL_REPORT_PAGE] 		= LoadDetailReportPage;
	g_pageTCB.pageArray[USER_LOGIN_PAGE] 			= PrestrainLoadPasswordPage;
	g_pageTCB.pageArray[CHANNEL_SELECT_PAGE] 		= PrestrainLoadChannelSelectPage;
	g_pageTCB.pageArray[SYSTEM_SET] 				= LoadSystemSetPage;
	g_pageTCB.pageArray[CONTROL_PARAMETER] 			= LoadControlParameterPage;
	g_pageTCB.pageArray[CALIBRATION_TABLE] 			= LoadCalibrationTablePage;
	g_pageTCB.pageArray[DATE_TIME_PAGE] 			= LoadTimeDatePage;
	g_pageTCB.pageArray[SOFT_ACTIVE_PAGE] 			= LoadSoftActivePage;
	g_pageTCB.pageArray[ONLINE_PARAMETER_PAGE] 		= LoadOnlineParameterPage;
	g_pageTCB.pageArray[SYSTEM_PASSWORD_PAGE] 		= LoadSystemPasswordPage;
	g_pageTCB.pageArray[FORCE_CALIBRATION_PAGE] 	= LoadForceCalibationPage;
	g_pageTCB.pageArray[FORCE_VERIFICATION_PAGE] 	= LoadForceCalibationPage;
	g_pageTCB.pageArray[CALIBRATION_PARAMETER] 		= LoadCalibrationParameterPage;
	g_pageTCB.pageArray[TEST_AFTER_DISPOSE_PAGE]	= LoadTestAfterDisposePage;
	g_pageTCB.pageArray[ADVANCE_PARAMETER_SET_PAGE] = LoadAdvanceParameterPage;
	g_pageTCB.pageArray[MODEL_SELECT_PAGE] 			= LoadModelSelectPage;
	g_pageTCB.pageArray[UNIT_SELECT_PAGE] 			= LoadUnitSelectPage;
	g_pageTCB.pageArray[SYSTEM_BACKUP_PAGE] 		= LoadSystemBackupPage;
	g_pageTCB.pageArray[SYSTEM_RESTORE_PAGE] 		= LoadSystemRestorePage;
	g_pageTCB.pageArray[SYSTEM_INFOMATION_PAGE] 	= LoadSystemInfomationPage;
	g_pageTCB.pageArray[HARD_TEST_PAGE] 			= LoadHardTestPage;
	g_pageTCB.pageArray[LINK_PAGE] 					= LoadLinkModePage;
	g_pageTCB.pageArray[TEST_TYPE_SELECT_PAGE] 		= LoadTestTypeSelectPage;
	g_pageTCB.pageArray[CURVE_SHOW_PAGE] 			= LoadCurveShowPage;
	g_pageTCB.pageArray[FILE_MANAGER_PAGE] 			= LoadFileManagerPage;
	
	g_pageTCB.curPage = g_pageTCB.pageArray[SYS_SETUP];
	g_pageTCB.pageName = SYS_SETUP;
}

/*------------------------------------------------------------
 * Function Name  : SetPage
 * Description    : 设置页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPage( PAGE_NAME_TypeDef setPage )
{
	g_pageTCB.curPage = g_pageTCB.pageArray[setPage];
	g_pageTCB.pageName = setPage;
}

/*------------------------------------------------------------
 * Function Name  : GetPageName
 * Description    : 获取页名
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
PAGE_NAME_TypeDef GetPageName( void )
{
	return g_pageTCB.pageName;
}

/*------------------------------------------------------------
 * Function Name  : GetPage
 * Description    : 获取页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
pFunction GetPage( void )
{
	return g_pageTCB.curPage;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
