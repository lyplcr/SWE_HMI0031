/**
  ******************************************************************************
  * @file    CurveShow.c
  * @author  SY
  * @version V1.0.0
  * @date    2016-1-21 13:15:16
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   显示曲线程序
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "CurveShow.h"
#include "TestReport.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint8_t curPage;								//当前页数
	uint8_t sumPage;								//总页数
	TEST_TYPE_TypeDef testType;						//试验类型
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页	
}CURVE_SHOW_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CURVE_SHOW_TypeDef g_curveShow;

extern COORDINATE_POINT_TypeDef g_coordinatePoint;
static COORDINATE_POINT_TypeDef * const pCurve = &g_coordinatePoint;

/* Private function prototypes -----------------------------------------------*/
static void CurveShowInit( void );
static void CurveShowReadParameter( void );
static void GUI_CurveShow( void );
static void CurveShowConfig( void );
static void CurveShowKeyProcess( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadCurveShowPage
 * Description    : 曲线显示页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadCurveShowPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	CurveShowInit();
	
	/* 参数配置 */
	CurveShowConfig();
	
	/* 获取参数 */
	CurveShowReadParameter();
	
	/* 画GUI框架 */
	GUI_CurveShow();
	
	/* 遍历 */

	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_curveShow.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		
		
		/* 移动光标 */
		
		
		/* 快捷菜单 */
		
		
		/* 按键处理 */
		CurveShowKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		
	}
}

/*------------------------------------------------------------
 * Function Name  : CurveShowInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowInit( void )
{
	g_curveShow.sumPage = 0;
	
	g_curveShow.testType = (TEST_TYPE_TypeDef)GetSelectReportTestType();
	
	g_curveShow.refreshShortcut = ENABLE;
	g_curveShow.leavePage.flagLeavePage = RESET;
	g_curveShow.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : CurveShowConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowConfig( void )
{
	g_curveShow.pTitle = "显示曲线";
}

/*------------------------------------------------------------
 * Function Name  : CurveShowReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowReadParameter( void )
{
	REPORT_TypeDef readReport;
	
	if (FR_OK != report_read(g_curveShow.testType,GetSelectReportFileNameAddr(),&readReport) )
	{
		g_curveShow.leavePage.flagLeavePage = SET;
		SetPage(DETAIL_REPORT_PAGE);
		
		return;
	}
	
	if ( readReport.sample_serial == 0 )
	{
		g_curveShow.leavePage.flagLeavePage = SET;
		SetPage(DETAIL_REPORT_PAGE);
		
		return;
	}
	
	g_curveShow.sumPage = readReport.sample_serial;
	
	if ((g_curveShow.curPage==0) || (g_curveShow.curPage>g_curveShow.sumPage))
	{
		g_curveShow.curPage = 1;
	}
	
	{
		FRESULT result = ReadCoordinatePoint(MMC,g_curveShow.testType,g_curveShow.curPage,\
							GetSelectReportFileNameAddr(),pCurve);	
		
		if (result != FR_OK)
		{
			g_curveShow.leavePage.flagLeavePage = SET;
			SetPage(DETAIL_REPORT_PAGE);
			
			return;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_CurveShowDrawCoordinate
 * Description    : 画坐标系GUI界面
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CurveShowDrawCoordinate( void )
{
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	pCoordinate->x = 120;
	pCoordinate->y = 94;
	pCoordinate->rowSpace = 50;
	pCoordinate->columnSpace = 50;
	pCoordinate->xLenth = 600;
	pCoordinate->yLenth = 300;
	pCoordinate->fillFieldLenth = 5;
	pCoordinate->emptyFieldLenth = 5;
	pCoordinate->lineWidth = 1;
	pCoordinate->rowFieldNum = 6;
	pCoordinate->columnFieldNum = 12;
	pCoordinate->mainBackColor = COLOR_BACK;
	pCoordinate->windowsBackColor = COLOR_BACK;
	pCoordinate->rowLineColor = FOREST_GREEN;
	pCoordinate->columnLineColor = FOREST_GREEN;
	pCoordinate->fontPointColor = ORANGE;
	pCoordinate->fontBackColor = COLOR_BACK;
	pCoordinate->xLinePointColor = FRESH_GREEN;
	pCoordinate->yLinePointColor = FRESH_GREEN;
	
	pCoordinate->maxForce = pCurve->maxValueY * pCurve->yScalingCoefficient;
	pCoordinate->maxTime = pCurve->maxValueX * pCurve->xScalingCoefficient;
	
	switch (pCurve->xUint)
	{
		case 0:
			pCoordinate->pXUnit = "(S)";
			break;
		case 1:
			pCoordinate->pXUnit = "(MS)";
			break;
	}
	
	switch (pCurve->yUint)
	{
		case 0:
			pCoordinate->pYUnit = "(kN)";
			break;
		case 1:
			pCoordinate->pYUnit = "(N)";
			break;
	}
	
	GUI_DrawCoordinate(pCoordinate);
}	

/*------------------------------------------------------------
 * Function Name  : InitCurveShowCoordinateDrawLine
 * Description    : 初始化坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitCurveShowCoordinateDrawLine( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	pDrawLine->status = STATUS_DRAW_LINE_IDLE;
	pDrawLine->start = RESET;
	pDrawLine->enableRedraw = DISABLE;
	pDrawLine->originX = pCoordinate->x;
	pDrawLine->originY = pCoordinate->y + pCoordinate->yLenth;
	pDrawLine->lenthX = pCoordinate->xLenth;
	pDrawLine->lenthY = pCoordinate->yLenth;
	pDrawLine->maxForce = pCurve->maxValueY;
	pDrawLine->maxTime = pCurve->maxValueX;
	pDrawLine->nowTimePoint = pCurve->nowUsePointNum;
	pDrawLine->lineColor = RED;
	pDrawLine->timeScalingCoefficient = pCurve->xScalingCoefficient;
	pDrawLine->forceScalingCoefficient = pCurve->yScalingCoefficient;
	pDrawLine->recordPointFreq = pCurve->recordPointFreq;
	pDrawLine->pDrawCoordinate = NULL;
				
	InitCoordinateDrawLine(pDrawLine);
	
	{
		uint32_t i;
		
		for (i=0; i<pCurve->maxPointNum; ++i)
		{
			pDrawLine->force[i] = pCurve->force[i];
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_CurveShow
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CurveShow( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_curveShow.pTitle);
	
	GUI_CurveShowDrawCoordinate();
	
	InitCurveShowCoordinateDrawLine();
	
	{
		COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
		
		CoordinateRedrawLine(pDrawLine);
	}
}

/*------------------------------------------------------------
 * Function Name  : CurveShowKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ESC:
				SetPage(DETAIL_REPORT_PAGE);
				g_curveShow.leavePage.flagLeavePage = SET;
				g_curveShow.leavePage.flagSaveData = RESET;
				break;
		}
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
