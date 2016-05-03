/**
  ******************************************************************************
  * @file    CurveShow.c
  * @author  SY
  * @version V1.0.0
  * @date    2016-1-21 13:15:16
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��ʾ���߳���
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
	uint8_t curPage;								//��ǰҳ��
	uint8_t sumPage;								//��ҳ��
	TEST_TYPE_TypeDef testType;						//��������
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ	
}CURVE_SHOW_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pCurveShowCue[] = 
{
	"��ȡ����ʧ�ܣ�",			//0
	"��ȡ������ļ�ʧ�ܣ�",		//1
};

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
static void CurveShowShortcutCycleTask( void );
static void CurveShowLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadCurveShowPage
 * Description    : ������ʾҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadCurveShowPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	CurveShowInit();
	
	/* �������� */
	CurveShowConfig();
	
	/* ��ȡ���� */
	CurveShowReadParameter();
	
	/* ��GUI��� */
	GUI_CurveShow();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_curveShow.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ��ݲ˵� */
		CurveShowShortcutCycleTask();
		
		/* �������� */
		CurveShowKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		CurveShowLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : CurveShowInit
 * Description    : ��ʼ��
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
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowConfig( void )
{
	g_curveShow.pTitle = "��ʾ����";
}

/*------------------------------------------------------------
 * Function Name  : CurveShowReadParameter
 * Description    : ������
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
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pCurveShowCue[0]);			
		PopWindowsProcessCycle();
		
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
		
		if (result == FR_OK)
		{
			if (pCurve->nowUsePointNum > DECORD_COORDINATE_FORCE_NUM)
			{
				pCurve->nowUsePointNum = DECORD_COORDINATE_FORCE_NUM;
			}
			if (pCurve->maxPointNum > DECORD_COORDINATE_FORCE_NUM)
			{
				pCurve->maxPointNum = DECORD_COORDINATE_FORCE_NUM;
			}
		}
		else
		{
			g_curveShow.leavePage.flagLeavePage = SET;
			SetPage(DETAIL_REPORT_PAGE);
			
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pCurveShowCue[1]);
			
			PopWindowsProcessCycle();
		}
	}
	
	/* ��ӡ������Ϣ */
	ECHO(DEBUG_CURVE_SHOW,"�������%f\r\n",readReport.maxForce[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"����ǿ�ȣ�%f\r\n",readReport.maxStrength[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"������ʽ��%d\r\n",readReport.yieldJudgeMode);
	ECHO(DEBUG_CURVE_SHOW,"����������ֵ��%d\r\n",readReport.yieldDisturbThreshold);	
	ECHO(DEBUG_CURVE_SHOW,"����������%f\r\n",readReport.upYieldForce[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"����������%f\r\n",readReport.downYieldForce[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"������ǿ�ȣ�%f\r\n",readReport.upYieldStrength[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"������ǿ�ȣ�%f\r\n",readReport.downYieldStrength[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"����ģ����ʼ�ʣ�%f\r\n",readReport.elasticModulusStartRate);
	ECHO(DEBUG_CURVE_SHOW,"����ģ����ֹ�ʣ�%f\r\n",readReport.elasticModulusEndRate);
	ECHO(DEBUG_CURVE_SHOW,"�Ǳ�����������%f\r\n",readReport.nonProportionalExtensionForce[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"�Ǳ�������ǿ�ȣ�%f\r\n",readReport.nonProportionalExtensionStrength[g_curveShow.curPage-1]);
	ECHO(DEBUG_CURVE_SHOW,"����ģ����%f\r\n",readReport.elasticModulus[g_curveShow.curPage-1]);
	
	#if (DEBUG_CURVE_SHOW == DEBUG_ON)
	{
		uint32_t i;
		
		ECHO(DEBUG_CURVE_SHOW,"��ӡ������ֵ��\r\n");
		
		for (i=0; i<pCurve->nowUsePointNum; ++i)
		{
			ECHO(DEBUG_CURVE_SHOW,"%f\r\n",pCurve->force[i]);
			bsp_DelayMS(10);
		}
	
		ECHO(DEBUG_CURVE_SHOW,"��ӡ���߱��Σ�\r\n");
		
		for (i=0; i<pCurve->nowUsePointNum; ++i)
		{
			ECHO(DEBUG_CURVE_SHOW,"%f\r\n",pCurve->deform[i]);
			bsp_DelayMS(10);
		}
	}
	#endif
}

/*------------------------------------------------------------
 * Function Name  : GUI_CurveShowDrawCoordinate
 * Description    : ������ϵGUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CurveShowDrawCoordinate( void )
{
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	pCoordinate->xType = pCurve->xType;
	pCoordinate->yType = pCurve->yType;	
	pCoordinate->x = 120;
	pCoordinate->y = 80;
	pCoordinate->rowSpace = 30;
	pCoordinate->columnSpace = 60;
	pCoordinate->xLenth = 600;
	pCoordinate->yLenth = 300;
	pCoordinate->fillFieldLenth = 5;
	pCoordinate->emptyFieldLenth = 5;
	pCoordinate->lineWidth = 1;
	pCoordinate->rowFieldNum = 10;
	pCoordinate->columnFieldNum = 10;
	pCoordinate->mainBackColor = COLOR_BACK;
	pCoordinate->windowsBackColor = COLOR_BACK;
	pCoordinate->rowLineColor = CL_GREEN1;
	pCoordinate->columnLineColor = CL_BROWN1;
	pCoordinate->fontPointColor = ORANGE;
	pCoordinate->fontBackColor = COLOR_BACK;
	pCoordinate->xLinePointColor = FRESH_GREEN;
	pCoordinate->yLinePointColor = FRESH_GREEN;
	
	if ((pCoordinate->xType==COORDINATE_USE_DEFORM) && \
		(pCoordinate->yType==COORDINATE_USE_FORCE))
	{
		pCoordinate->pTitle = "������ -- ���Ρ�����";
	}
	else if ((pCoordinate->xType==COORDINATE_USE_TIME) && \
		(pCoordinate->yType==COORDINATE_USE_FORCE))
	{
		pCoordinate->pTitle = "������ -- ʱ�䡻����";
	}
	else
	{
		pCoordinate->pTitle = "��XXXX -- XXXX������";
	}
	
	pCoordinate->xMaxValue = pCurve->xMaxValue * pCurve->xScalingCoefficient;
	pCoordinate->yMaxValue = pCurve->yMaxValue * pCurve->yScalingCoefficient;
	
	switch (pCurve->xUint)
	{
		case COORDINATE_UNIT_S:
			pCoordinate->pXUnit = " (S)";
			break;
		case COORDINATE_UNIT_MS:
			pCoordinate->pXUnit = "(MS)";
			break;
		case COORDINATE_UNIT_N:
			pCoordinate->pXUnit = " (N)";
			break;
		case COORDINATE_UNIT_KN:
			pCoordinate->pXUnit = "(kN)";
			break;
		case COORDINATE_UNIT_MM:
			pCoordinate->pXUnit = "(mm)";
			break;
		case COORDINATE_UNIT_CM:
			pCoordinate->pXUnit = "(cm)";
			break;
		case COORDINATE_UNIT_DM:
			pCoordinate->pXUnit = "(dm)";
			break;
		case COORDINATE_UNIT_M:
			pCoordinate->pXUnit = " (m)";
			break;
		default:
			pCoordinate->pXUnit = "(ERR)";
			break;
	}
	
	switch (pCurve->yUint)
	{
		case COORDINATE_UNIT_S:
			pCoordinate->pYUnit = " (S)";
			break;
		case COORDINATE_UNIT_MS:
			pCoordinate->pYUnit = "(MS)";
			break;
		case COORDINATE_UNIT_N:
			pCoordinate->pYUnit = " (N)";
			break;
		case COORDINATE_UNIT_KN:
			pCoordinate->pYUnit = "(kN)";
			break;
		case COORDINATE_UNIT_MM:
			pCoordinate->pYUnit = "(mm)";
			break;
		case COORDINATE_UNIT_CM:
			pCoordinate->pYUnit = "(cm)";
			break;
		case COORDINATE_UNIT_DM:
			pCoordinate->pYUnit = "(dm)";
			break;
		case COORDINATE_UNIT_M:
			pCoordinate->pYUnit = " (m)";
			break;
		default:
			pCoordinate->pYUnit = "(ERR)";
			break;
	}
	
	GUI_DrawCoordinate(pCoordinate);
}	

/*------------------------------------------------------------
 * Function Name  : InitCurveShowCoordinateDrawLine
 * Description    : ��ʼ������ϵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitCurveShowCoordinateDrawLine( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	pDrawLine->xType = pCoordinate->xType;
	pDrawLine->yType = pCoordinate->yType;
	pDrawLine->baseIndex = 0;
	pDrawLine->status = STATUS_DRAW_LINE_IDLE;
	pDrawLine->start = RESET;
	pDrawLine->enableRedraw = DISABLE;
	pDrawLine->originX = pCoordinate->x;
	pDrawLine->originY = pCoordinate->y + pCoordinate->yLenth;
	pDrawLine->lenthX = pCoordinate->xLenth;
	pDrawLine->lenthY = pCoordinate->yLenth;
	pDrawLine->xMaxValue = pCurve->xMaxValue;
	pDrawLine->yMaxValue = pCurve->yMaxValue;
	pDrawLine->nowTimePoint = pCurve->nowUsePointNum;
	pDrawLine->lineColor = CRIMSON;
	pDrawLine->xScalingCoefficient = pCurve->xScalingCoefficient;
	pDrawLine->yScalingCoefficient = pCurve->yScalingCoefficient;
	pDrawLine->recordPointFreq = pCurve->recordPointFreq;
	pDrawLine->pDrawCoordinate = NULL;
	
	memset(pDrawLine->force,0x00,sizeof(float)*pCurve->maxPointNum);
	memset(pDrawLine->deform,0x00,sizeof(float)*pCurve->maxPointNum);
	
	{
		uint32_t i;
		
		for (i=0; i<pCurve->maxPointNum; ++i)
		{
			pDrawLine->force[i] = pCurve->force[i];
			pDrawLine->deform[i] = pCurve->deform[i];
		}
	}
}
#if 0
	/*------------------------------------------------------------
	 * Function Name  : CurveShowShowCircleMarkInfomation
	 * Description    : ��ʾԲ�α����Ϣ
	 * Input          : None
	 * Output         : None
	 * Return         : None
	 *------------------------------------------------------------*/
	static void CurveShowShowCircleMarkInfomation( void )
	{
		DRAW_CIRCLE_MARK_TypeDef circleMark;
		const uint16_t RADIUS = 3;		/* �뾶 */
		const uint16_t DISTANCE = 12;	/* Բ��Բ֮��ľ��� */
		uint32_t curNum = g_curveShow.curPage;
		uint32_t sumNum = g_curveShow.sumPage;
		uint32_t lenth = 0;
		
		/* Բ�α�����򳤶� */
		lenth = (2 * RADIUS + DISTANCE) *  sumNum - DISTANCE;
		
		circleMark.x = ((LCD_LENTH_X - lenth) >> 1);
		circleMark.y = PAGE_DI_POS_Y + 45;
		circleMark.distance = DISTANCE;
		circleMark.curSerial = curNum;
		circleMark.sumSerial = sumNum;
		circleMark.pointColor = CL_BLUE3;
		circleMark.backColor = COLOR_BACK;
		circleMark.radius = RADIUS;
		
		DrawCircleMark(&circleMark);
	}
#endif
	
/*------------------------------------------------------------
 * Function Name  : CurveShowShowCurPageInfomation
 * Description    : ��ʾ��ǰҳ��Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowShowCurPageInfomation( void )
{
	char buff[10];
	uint32_t pageNum = 0;
	const uint16_t back_color = COLOR_BACK;
	const uint16_t point_color = MIDDLEBLUE;
	const uint16_t posFirstX = PAGE_DI_POS_X + 60;
	const uint16_t posFirstY = PAGE_DI_POS_Y + 25;
	const uint16_t posSecondX = PAGE_GONG_POS_X - 54;
	const uint16_t posSecondY = PAGE_GONG_POS_Y + 25;
	
	if ( g_curveShow.sumPage )
	{	
		pageNum = g_curveShow.curPage;
		usprintf(buff,"%02d",pageNum);		
		GUI_DispStr24At(posFirstX,posFirstY,point_color,back_color,buff);
		
		GUI_DispStr24At(posSecondX-20,posSecondY,COLOR_POINT,back_color,"/");
		
		pageNum = g_curveShow.sumPage;
		usprintf(buff,"%02d",pageNum);		
		GUI_DispStr24At(posSecondX,posSecondY,point_color,back_color,buff);
	}
	else
	{
		GUI_DispStr24At(posSecondX,posSecondY,COLOR_POINT,back_color,"δ�ҵ����ߣ�");	
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_CurveShow
 * Description    : ����GUI
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
	
	CurveShowShowCurPageInfomation();
	
//	CurveShowShowCircleMarkInfomation();
}

/*------------------------------------------------------------
 * Function Name  : CurveShowKeyProcess
 * Description    : ��������
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
			case KEY_F3:
			case KEY_LEFT:
				if (g_curveShow.sumPage > 1)
				{
					if (g_curveShow.curPage)
					{
						g_curveShow.curPage--;
					}
					
					if (!g_curveShow.curPage)
					{
						g_curveShow.curPage = g_curveShow.sumPage;
					}
					
					g_curveShow.leavePage.flagLeavePage = SET;
					g_curveShow.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_F4:
			case KEY_RIGHT:
				if (g_curveShow.sumPage > 1)
				{
					g_curveShow.curPage++;
					
					if (g_curveShow.curPage > g_curveShow.sumPage)
					{
						g_curveShow.curPage = 1;
					}
					
					g_curveShow.leavePage.flagLeavePage = SET;
					g_curveShow.leavePage.flagSaveData = RESET;
				}
				break;			
			case KEY_ESC:
				SetPage(DETAIL_REPORT_PAGE);
				g_curveShow.leavePage.flagLeavePage = SET;
				g_curveShow.leavePage.flagSaveData = SET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CurveShowShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_curveShow.refreshShortcut == ENABLE)
	{
		g_curveShow.refreshShortcut = DISABLE;
		
		pShortCut->status = SHOW_F3 | SHOW_F4;
		
		pShortCut->pContent[2] = pTwoLevelMenu[50];
		pShortCut->pContent[3] = pTwoLevelMenu[66];
		
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : CurveShowLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CurveShowLeavePageCheckCycle( void )
{
	if (g_curveShow.leavePage.flagLeavePage == SET)
	{
		if (g_curveShow.leavePage.flagSaveData == SET)
		{
			g_curveShow.curPage = 1;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
