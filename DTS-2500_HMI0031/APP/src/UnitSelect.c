/**
  ******************************************************************************
  * @file    UnitSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-23 14:47:31
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��λѡ��
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "UnitSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				3
#define MAX_PARAMETER_PUTIN_BIT			2

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_FH_UNIT = 0,
	OBJECT_WY_UNIT,
	OBJECT_BX_UNIT,
}OBJECT_UNIT_SELECT_TypeDef;

typedef enum
{
	INDEX_FH_UNIT = 0,
	INDEX_WY_UNIT,
	INDEX_BX_UNIT,
}INDEX_UNIT_SELECT_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//��������
	
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_PARAMETER_CNT];
	
	FunctionalState enableArrow;					//ʹ�ܼ�ͷ
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}UNIT_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pUnitSelectName[] =
{
	"����ͨ����",
	"λ��ͨ����",
	"����ͨ����",
};

const char * const pUnitFH[] =
{
	"kN",	//0
	"N",	//1
};

const char * const pUnitWY[] = 
{
	"mm",	//0
	"cm",	//1
	"dm",	//2
	"m",	//3
};

const char * const pUnitBX[] = 
{
	"mm",	//0
	"cm",	//1
	"dm",	//2
	"m",	//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UNIT_SELECT_TypeDef g_unitSelect;

/* Private function prototypes -----------------------------------------------*/
static void UnitSelectInit( void );
static void UnitSelectParameterConfig( void );
static void UnitSelectReadParameter( void );
static void GUI_UnitSelect( void );
static void Traverse_UnitSelect( void );
static void UnitSelectMoveIndexProcess( void );
static void UnitSelectMoveCursorProcess( void );
static void UnitSelectShortcutCycleTask( void );
static void UnitSelectKeyProcess( void );
static void UnitSelectLeavePageCheckCycle( void );
static void UnitSelectIndexCrossTheBorderProcess( void );
static void UnitSelectStatusProcess( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadUnitSelectPage
 * Description    : ��λѡ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadUnitSelectPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	UnitSelectInit();
	
	/* �������� */
	UnitSelectParameterConfig();
	
	/* ��ȡ���� */
	UnitSelectReadParameter();
	
	/* ����ֵԽ�紦�� */
	UnitSelectIndexCrossTheBorderProcess();
	
	/* ��GUI��� */
	GUI_UnitSelect();
	
	/* ���� */
	Traverse_UnitSelect();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_unitSelect.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* �ƶ�����ֵ */
		UnitSelectMoveIndexProcess();
		
		/* ״̬���� */
		UnitSelectStatusProcess();
		
		/* �ƶ���� */
		UnitSelectMoveCursorProcess();
		
		/* ��ݲ˵� */
		UnitSelectShortcutCycleTask();
		
		/* �������� */
		UnitSelectKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		UnitSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectInit( void )
{
	g_unitSelect.isIndexMove = NO;		
	g_unitSelect.recordIndex = 0xff;
	g_unitSelect.refreshShortcut = ENABLE;
	g_unitSelect.leavePage.flagLeavePage = RESET;
	g_unitSelect.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectParameterConfig( void )
{
	/* ���� */
	g_unitSelect.pTitle = "��λѡ��";
	
	/* �Կ���� */
	g_unitSelect.curParameterNum = MAX_PARAMETER_CNT;
	
	/* ����ֵ */
	g_unitSelect.indexArray[INDEX_FH_UNIT] = OBJECT_FH_UNIT;
	g_unitSelect.indexArray[INDEX_WY_UNIT] = OBJECT_WY_UNIT;
	g_unitSelect.indexArray[INDEX_BX_UNIT] = OBJECT_BX_UNIT;
	
	/* �������� */
	g_unitSelect.pParameterNameArray[INDEX_FH_UNIT] = pUnitSelectName[0];
	g_unitSelect.pParameterNameArray[INDEX_WY_UNIT] = pUnitSelectName[1];
	g_unitSelect.pParameterNameArray[INDEX_BX_UNIT] = pUnitSelectName[2];
	
	/* �����˵��������� */
	g_unitSelect.twoLevelMenu[INDEX_FH_UNIT].parameterCnt 	= 2;
	g_unitSelect.twoLevelMenu[INDEX_WY_UNIT].parameterCnt 	= 4;
	g_unitSelect.twoLevelMenu[INDEX_BX_UNIT].parameterCnt 	= 4;
	
	/* �����˵����� */
	g_unitSelect.twoLevelMenu[INDEX_FH_UNIT].parameterType 	= NONE_USE_USER_DEFINE;
	g_unitSelect.twoLevelMenu[INDEX_WY_UNIT].parameterType 	= NONE_USE_USER_DEFINE;
	g_unitSelect.twoLevelMenu[INDEX_BX_UNIT].parameterType 	= NONE_USE_USER_DEFINE;
	
	/* �����˵������� */
	g_unitSelect.twoLevelMenu[INDEX_FH_UNIT].pParameterNameArray 	= pUnitFH;
	g_unitSelect.twoLevelMenu[INDEX_WY_UNIT].pParameterNameArray 	= pUnitWY;
	g_unitSelect.twoLevelMenu[INDEX_BX_UNIT].pParameterNameArray 	= pUnitBX;
}

/*------------------------------------------------------------
 * Function Name  : GetUnitSelectIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetUnitSelectIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		if (g_unitSelect.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetUnitSelectIndex(OBJECT_FH_UNIT);
	if (index != 0xff)
	{
		if (pHmi->fhUnit == 0)	
		{
			strcpy(g_unitSelect.parameterData[index],pUnitFH[FH_UNIT_kN]);
		}
		else
		{
			strcpy(g_unitSelect.parameterData[index],pUnitFH[FH_UNIT_N]);
		}
		
		g_unitSelect.twoLevelMenu[index].index = pHmi->fhUnit;
	}
	
	index = GetUnitSelectIndex(OBJECT_WY_UNIT);
	if (index != 0xff)
	{
		switch (pHmi->wyUnit)	
		{
			case WY_UNIT_MM:
				strcpy(g_unitSelect.parameterData[index],pUnitWY[WY_UNIT_MM]);
				break;
			case WY_UNIT_CM:
				strcpy(g_unitSelect.parameterData[index],pUnitWY[WY_UNIT_CM]);
				break;
			case WY_UNIT_DM:
				strcpy(g_unitSelect.parameterData[index],pUnitWY[WY_UNIT_DM]);
				break;
			case WY_UNIT_M:
				strcpy(g_unitSelect.parameterData[index],pUnitWY[WY_UNIT_M]);
				break;
		}
		
		g_unitSelect.twoLevelMenu[index].index = pHmi->wyUnit;
	}
	
	index = GetUnitSelectIndex(OBJECT_BX_UNIT);
	if (index != 0xff)
	{
		switch (pHmi->bxUnit)	
		{
			case BX_UNIT_MM:
				strcpy(g_unitSelect.parameterData[index],pUnitBX[BX_UNIT_MM]);
				break;
			case BX_UNIT_CM:
				strcpy(g_unitSelect.parameterData[index],pUnitBX[BX_UNIT_CM]);
				break;
			case BX_UNIT_DM:
				strcpy(g_unitSelect.parameterData[index],pUnitBX[BX_UNIT_DM]);
				break;
			case BX_UNIT_M:
				strcpy(g_unitSelect.parameterData[index],pUnitBX[BX_UNIT_M]);
				break;
		}
		
		g_unitSelect.twoLevelMenu[index].index = pHmi->bxUnit;
	}
}	

/*------------------------------------------------------------
 * Function Name  : UnitSelectWriteParameter
 * Description    : д����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectWriteParameter( void )
{
	uint8_t index = 0;
	
	index = GetUnitSelectIndex(OBJECT_FH_UNIT);
	if (index != 0xff)
	{
		pHmi->fhUnit = g_unitSelect.twoLevelMenu[index].index;
	}
	
	index = GetUnitSelectIndex(OBJECT_WY_UNIT);
	if (index != 0xff)
	{
		pHmi->wyUnit = g_unitSelect.twoLevelMenu[index].index;
	}
	
	index = GetUnitSelectIndex(OBJECT_BX_UNIT);
	if (index != 0xff)
	{
		pHmi->bxUnit = g_unitSelect.twoLevelMenu[index].index;
	}
	
	pcm_save();
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectIndexCrossTheBorderProcess
 * Description    : ��������ֵԽ�紦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectIndexCrossTheBorderProcess( void )
{
	uint8_t i;
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		if (g_unitSelect.twoLevelMenu[i].index >= g_unitSelect.twoLevelMenu[i].parameterCnt)
		{
			g_unitSelect.twoLevelMenu[i].index = 0;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigUnitSelectParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigUnitSelectParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 150;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		g_unitSelect.oneLevelMenu[i].x = startX;
		g_unitSelect.oneLevelMenu[i].y = startY;
		g_unitSelect.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_unitSelect.oneLevelMenu[i].backColor = COLOR_BACK;
		g_unitSelect.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_unitSelect.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_unitSelect.oneLevelMenu[i].lenth = 102;
		g_unitSelect.oneLevelMenu[i].width = 30;
		g_unitSelect.oneLevelMenu[i].fontSize = 24;
		g_unitSelect.oneLevelMenu[i].rowDistance = 24;
		g_unitSelect.oneLevelMenu[i].columnDistance = 48;
		g_unitSelect.oneLevelMenu[i].lineWidth = 2;
		
		g_unitSelect.twoLevelMenu[i].x = g_unitSelect.oneLevelMenu[i].x + g_unitSelect.oneLevelMenu[i].lenth + 36;
		g_unitSelect.twoLevelMenu[i].y = g_unitSelect.oneLevelMenu[i].y;
		g_unitSelect.twoLevelMenu[i].pointColor = g_unitSelect.oneLevelMenu[i].pointColor;
		g_unitSelect.twoLevelMenu[i].backColor = g_unitSelect.oneLevelMenu[i].backColor;
		g_unitSelect.twoLevelMenu[i].lenth = 102;
		g_unitSelect.twoLevelMenu[i].fontSize = 24;
		g_unitSelect.twoLevelMenu[i].rowDistance = 2;
		g_unitSelect.twoLevelMenu[i].columnDistance = 0;
		g_unitSelect.twoLevelMenu[i].lineWidth = 2;				
		g_unitSelect.twoLevelMenu[i].width = g_unitSelect.twoLevelMenu[i].parameterCnt * \
												(g_unitSelect.twoLevelMenu[i].fontSize + g_unitSelect.twoLevelMenu[i].rowDistance) + \
												g_unitSelect.twoLevelMenu[i].rowDistance + 2 * g_unitSelect.twoLevelMenu[i].lineWidth;
		g_unitSelect.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_unitSelect.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_unitSelect.oneLevelMenu[i].width + g_unitSelect.oneLevelMenu[i].rowDistance - \
				  g_unitSelect.oneLevelMenu[i].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelectDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_UnitSelectDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_unitSelect.oneLevelMenu[index].x;
	rectangle.y = g_unitSelect.oneLevelMenu[index].y;
	rectangle.lenth = g_unitSelect.oneLevelMenu[index].lenth;
	rectangle.width = g_unitSelect.oneLevelMenu[index].width;
	rectangle.lineWidth = g_unitSelect.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_unitSelect.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelectRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_UnitSelectRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		GUI_UnitSelectDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelectDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_UnitSelectDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_unitSelect.oneLevelMenu[index].x - 5 * g_unitSelect.oneLevelMenu[index].fontSize;
	const uint16_t y = g_unitSelect.oneLevelMenu[index].y + g_unitSelect.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_unitSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_unitSelect.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_unitSelect.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelectOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_UnitSelectOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		GUI_UnitSelectDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelect
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_UnitSelect( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_unitSelect.pTitle);
	
	ConfigUnitSelectParameterRectangleFrameCoordinate();
	
	GUI_UnitSelectRectangleFrame();
	
	GUI_UnitSelectOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_UnitSelectOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_UnitSelectOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_unitSelect.oneLevelMenu[index].x + g_unitSelect.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_unitSelect.oneLevelMenu[index].y + g_unitSelect.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_unitSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_unitSelect.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_unitSelect.oneLevelMenu[index].lenth - 2 * g_unitSelect.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_unitSelect.oneLevelMenu[index].width - 2 * g_unitSelect.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_unitSelect.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_UnitSelectOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_UnitSelectOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_unitSelect.curParameterNum; ++i)
	{
		Show_UnitSelectOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_UnitSelect
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_UnitSelect( void )
{
	Show_UnitSelectOneLevelMenuContent();
}	

/*------------------------------------------------------------
 * Function Name  : UnitSelectMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_unitSelect.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_unitSelect.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_unitSelect.curParameterNum;
	indexObj.pNowIndex = &g_unitSelect.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_unitSelect.nowIndex != g_unitSelect.recordIndex)
	{		
		g_unitSelect.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectStatusProcess
 * Description    : ״̬����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectStatusProcess( void )
{
	uint8_t index = g_unitSelect.nowIndex;
	
	g_unitSelect.nowIndex %= g_unitSelect.curParameterNum;
	
	switch ( g_unitSelect.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_unitSelect.enableArrow = ENABLE;
			break;
		
		default:
			g_unitSelect.enableArrow = DISABLE;
			break;
	}
	
}

/*------------------------------------------------------------
 * Function Name  : Show_UnitSelectOneRowOneLevelMenuArrow
 * Description    : ��ʾһ�в�����ͷ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_UnitSelectOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_unitSelect.oneLevelMenu[index].x + g_unitSelect.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_unitSelect.oneLevelMenu[index].y + g_unitSelect.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_unitSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Clear_UnitSelectOneRowOneLevelMenuArrow
 * Description    : ���һ�в�����ͷ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_UnitSelectOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_unitSelect.oneLevelMenu[index].x + g_unitSelect.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_unitSelect.oneLevelMenu[index].y + g_unitSelect.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_unitSelect.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectRestoreShowOneMenu
 * Description    : ��ԭ��ʾһ���˵���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectRestoreShowOneMenu( uint8_t index )
{
	g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].pointColor = g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordPointColor;
	g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].backColor = g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordBackColor;
	
	Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.recordIndex);	
	Clear_UnitSelectOneRowOneLevelMenuArrow(g_unitSelect.recordIndex);
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectShowOneMenuWithCursor
 * Description    : ��ʾһ�������Ĳ˵���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectShowOneMenuWithCursor( uint8_t index )
{
	g_unitSelect.oneLevelMenu[index].pointColor = COLOR_SELECT_POINT;
	g_unitSelect.oneLevelMenu[index].backColor = COLOR_SELECT_BACK;
	
	g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].pointColor = COLOR_SELECT_POINT;
	g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].backColor = COLOR_SELECT_BACK;
	
	Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.nowIndex);
	
	if (g_unitSelect.enableArrow == ENABLE)
	{
		Show_UnitSelectOneRowOneLevelMenuArrow(g_unitSelect.nowIndex);
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectMoveCursorProcess( void )
{
	if (g_unitSelect.isIndexMove == YES)
	{		
		if (g_unitSelect.recordIndex != 0xff)
		{
			UnitSelectRestoreShowOneMenu(g_unitSelect.recordIndex);
			
			UnitSelectShowOneMenuWithCursor(g_unitSelect.nowIndex);
		}
		else
		{
			UnitSelectShowOneMenuWithCursor(g_unitSelect.nowIndex);
		}
		
		g_unitSelect.recordIndex = g_unitSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_unitSelect.refreshShortcut == ENABLE)
	{
		g_unitSelect.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectIndexUpdate
 * Description    : ����ֵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectIndexUpdate( void )
{
	g_unitSelect.nowIndex++;
	g_unitSelect.nowIndex %= g_unitSelect.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectUpdateStatus( void )
{
	UnitSelectIndexUpdate();
	
	g_unitSelect.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectKeyProcess( void )
{
	uint8_t index = g_unitSelect.nowIndex;	
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_unitSelect.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_unitSelect.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* �����ݲ˵� */
					GUI_ClearShortcutMenu();
					
					/* ��ԭһ���˵� */
					g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].pointColor = \
						g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordPointColor;
					g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].backColor = \
						g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordBackColor;
					
					Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.recordIndex);
					
					/* ���ö����˵� */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_unitSelect.twoLevelMenu[index].x;
					pMenu->y = g_unitSelect.twoLevelMenu[index].y;
					pMenu->nowIndex = g_unitSelect.twoLevelMenu[index].index;
					pMenu->maxUpY = g_unitSelect.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_unitSelect.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_unitSelect.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_unitSelect.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_unitSelect.twoLevelMenu[index].lenth;
					pMenu->width = g_unitSelect.twoLevelMenu[index].width;
					pMenu->pointColor = g_unitSelect.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_unitSelect.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_unitSelect.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_unitSelect.twoLevelMenu[index].pointColor;
					pMenu->nums = g_unitSelect.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_unitSelect.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_unitSelect.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{
						g_unitSelect.twoLevelMenu[index].index = pMenu->nowIndex;
						
						strcpy(g_unitSelect.parameterData[index],pMenu->pParameterNameArray[pMenu->nowIndex]);
					}
					
					UnitSelectUpdateStatus();				
				}
				break;
			
			case KEY_ENTER:
				break;
				
			case KEY_F3:
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				g_unitSelect.leavePage.flagLeavePage = SET;
				g_unitSelect.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F4:
			case KEY_ESC:
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				g_unitSelect.leavePage.flagLeavePage = SET;
				g_unitSelect.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectLeavePageCheckCycle( void )
{
	if (g_unitSelect.leavePage.flagLeavePage == SET)
	{
		if (g_unitSelect.leavePage.flagSaveData == SET)
		{			
			UnitSelectWriteParameter();
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
