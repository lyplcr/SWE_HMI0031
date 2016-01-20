/**
  ******************************************************************************
  * @file    TestTypeSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-12-24 15:26:15
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��������ѡ�����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TestTypeSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define TEST_TYPE_SELECT_MENU_CNT		3

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	INDEX_COMPRESSION_TEST = 0,	/* ѹ������ */
	INDEX_BENDING_TEST,			/* �������� */
	INDEX_STRETCH_TEST,			/* �������� */
}INDEX_TEST_TYPE_SELECT_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[TEST_TYPE_SELECT_MENU_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[TEST_TYPE_SELECT_MENU_CNT];
	uint8_t menuCnt;	
	const char *pParameterNameArray[TEST_TYPE_SELECT_MENU_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	LEAVE_PAGE_TypeDef leavePage;			
	PAGE_NAME_TypeDef sourcePage;					//Դҳ
	PAGE_NAME_TypeDef targetPage;					//Ŀ��ҳ
	PAGE_NAME_TypeDef nextPage;						//��һҳ
	TEST_ATTRIBUTE_TypeDef selChannel;
}TEST_TYPE_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTestTypeSelectParameterName[] =
{
	"ѹ������",
	"��������",
	"��������",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TEST_TYPE_SELECT_TypeDef g_testTypeSelect;

/* Private function prototypes -----------------------------------------------*/
static void TestTypeSelectInit( void );
static void TestTypeSelectConfig( void );
static void TestTypeSelectReadParameter( void );
static void GUI_TestTypeSelect( void );
static void TestTypeSelectMoveIndexProcess( void );
static void TestTypeSelectMoveCursorProcess( void );
static void TestTypeSelectShortcutCheckCycle( void );
static void TestTypeSelectKeyProcess( void );
static void TestTypeSelectLeavePageCheckCycle( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadTestTypeSelectPage
 * Description    : ��������ѡ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestTypeSelectPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	TestTypeSelectInit();
	
	/* �������� */
	TestTypeSelectConfig();
	
	/* ��ȡ���� */
	TestTypeSelectReadParameter();
	
	/* ��GUI��� */
	GUI_TestTypeSelect();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_testTypeSelect.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* �ƶ�����ֵ */
		TestTypeSelectMoveIndexProcess();
		
		/* �ƶ���� */
		TestTypeSelectMoveCursorProcess();
		
		/* ��ݲ˵� */
		TestTypeSelectShortcutCheckCycle();
		
		/* �������� */
		TestTypeSelectKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		TestTypeSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SetTestTypeSelectSourcePage
 * Description    : ����ͨ��ѡ����Դҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetTestTypeSelectSourcePage( PAGE_NAME_TypeDef sourcePage )
{
	g_testTypeSelect.sourcePage = sourcePage;
}

/*------------------------------------------------------------
 * Function Name  : SetTestTypeSelectTargetPage
 * Description    : ����ͨ��ѡ��Ŀ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetTestTypeSelectTargetPage( PAGE_NAME_TypeDef targetPage )
{
	g_testTypeSelect.targetPage = targetPage;
}

/*------------------------------------------------------------
 * Function Name  : GetSelectTestAttribute
 * Description    : ��ȡѡ�����������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TEST_ATTRIBUTE_TypeDef GetSelectTestAttribute( void )
{
	return g_testTypeSelect.selChannel;
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectInit( void )
{
	g_testTypeSelect.recordIndex = 0xff;
	
	g_testTypeSelect.isIndexMove = NO;
	
	g_testTypeSelect.leavePage.flagLeavePage = RESET;
	g_testTypeSelect.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectConfig
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectConfig( void )
{
	/* ���� */
	g_testTypeSelect.pTitle = "��������ѡ��";
	
	/* �Կ���� */
	g_testTypeSelect.menuCnt = TEST_TYPE_SELECT_MENU_CNT;
	
	/* ����ֵ */		
	g_testTypeSelect.indexArray[INDEX_COMPRESSION_TEST] = COMPRESSION_TEST;	
	g_testTypeSelect.indexArray[INDEX_BENDING_TEST] 	= BENDING_TEST;	
	g_testTypeSelect.indexArray[INDEX_STRETCH_TEST] 	= STRETCH_TEST;
	
	/* ���� */
	g_testTypeSelect.pParameterNameArray[INDEX_COMPRESSION_TEST]	= pTestTypeSelectParameterName[0];	
	g_testTypeSelect.pParameterNameArray[INDEX_BENDING_TEST] 		= pTestTypeSelectParameterName[1];	
	g_testTypeSelect.pParameterNameArray[INDEX_STRETCH_TEST] 		= pTestTypeSelectParameterName[2];
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectReadParameter( void )
{	
	;
}	

/*------------------------------------------------------------
 * Function Name  : ConfigTestTypeSelectRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestTypeSelectRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 240;
	const uint16_t INIT_START_Y = 105;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_testTypeSelect.menuCnt; ++i)
	{
		g_testTypeSelect.oneLevelMenu[i].x = startX;
		g_testTypeSelect.oneLevelMenu[i].y = startY;
		g_testTypeSelect.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_testTypeSelect.oneLevelMenu[i].backColor = COLOR_POINT;
		g_testTypeSelect.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_testTypeSelect.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_testTypeSelect.oneLevelMenu[i].lenth = 320;
		g_testTypeSelect.oneLevelMenu[i].width = 64;
		g_testTypeSelect.oneLevelMenu[i].fontSize = 32;
		g_testTypeSelect.oneLevelMenu[i].rowDistance = 40;
		g_testTypeSelect.oneLevelMenu[i].columnDistance = 0;
		
		startY += g_testTypeSelect.oneLevelMenu[i].rowDistance + g_testTypeSelect.oneLevelMenu[i].width;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestTypeSelectDrawOneRectangleFrame
 * Description    : һ�н���GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestTypeSelectDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_testTypeSelect.oneLevelMenu[index].x;
	const uint16_t y = g_testTypeSelect.oneLevelMenu[index].y;
	const uint16_t lenth = g_testTypeSelect.oneLevelMenu[index].lenth;
	const uint16_t width = g_testTypeSelect.oneLevelMenu[index].width;
	const uint16_t pointColor = g_testTypeSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testTypeSelect.oneLevelMenu[index].backColor;
	uint16_t fontLenth = strlen(g_testTypeSelect.pParameterNameArray[index]);
	uint8_t fontSize = g_testTypeSelect.oneLevelMenu[index].fontSize;
	uint16_t xOffset = 0;
	uint16_t yOffset = 0;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	xOffset = (lenth - fontLenth * (fontSize >> 1)) >> 1;
	yOffset = (width - fontSize) >> 1;
	
	GUI_DispStr32At(x+xOffset,y+yOffset,pointColor,backColor,g_testTypeSelect.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestTypeSelectRectangleFrame
 * Description    : ������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestTypeSelectRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_testTypeSelect.menuCnt; ++i)
	{
		GUI_TestTypeSelectDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestTypeSelect
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestTypeSelect( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_testTypeSelect.pTitle);
	
	ConfigTestTypeSelectRectangleFrameCoordinate();
	
	GUI_TestTypeSelectRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_testTypeSelect.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_testTypeSelect.menuCnt;
	indexObj.colNum = 1;
	indexObj.sumNum = indexObj.rowNum * indexObj.colNum;
	indexObj.pNowIndex = &g_testTypeSelect.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_testTypeSelect.nowIndex != g_testTypeSelect.recordIndex)
	{		
		g_testTypeSelect.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectMoveCursorProcess( void )
{
	if (g_testTypeSelect.isIndexMove == YES)
	{		
		if (g_testTypeSelect.recordIndex != 0xff)
		{
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.recordIndex].pointColor = g_testTypeSelect.oneLevelMenu[g_testTypeSelect.recordIndex].recordPointColor;
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.recordIndex].backColor = g_testTypeSelect.oneLevelMenu[g_testTypeSelect.recordIndex].recordBackColor;
			
			GUI_TestTypeSelectDrawOneRectangleFrame(g_testTypeSelect.recordIndex);
			
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.nowIndex].pointColor = COLOR_POINT;
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_TestTypeSelectDrawOneRectangleFrame(g_testTypeSelect.nowIndex);
		}
		else
		{
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.nowIndex].pointColor = COLOR_POINT;
			g_testTypeSelect.oneLevelMenu[g_testTypeSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_TestTypeSelectDrawOneRectangleFrame(g_testTypeSelect.nowIndex);
		}
		
		g_testTypeSelect.recordIndex = g_testTypeSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectShortcutCheckCycle
 * Description    : ��ݼ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue > 0) && (keyValue <= g_testTypeSelect.menuCnt))
	{
		g_testTypeSelect.leavePage.flagLeavePage = SET;
		g_testTypeSelect.leavePage.flagSaveData = SET;
		
		g_testTypeSelect.nowIndex = keyValue - 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				g_testTypeSelect.leavePage.flagLeavePage = SET;
				g_testTypeSelect.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_testTypeSelect.leavePage.flagLeavePage = SET;
				g_testTypeSelect.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	TestTypeSelectShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectLeavePageCheckCycle( void )
{
	if (g_testTypeSelect.leavePage.flagLeavePage == SET)
	{
		if (g_testTypeSelect.leavePage.flagSaveData == SET)
		{
			switch ( g_testTypeSelect.indexArray[g_testTypeSelect.nowIndex] )
			{
				case COMPRESSION_TEST:
					g_testTypeSelect.selChannel = COMPRESSION_TEST;
					SetPage(g_testTypeSelect.targetPage);
					break;
				case BENDING_TEST:
					g_testTypeSelect.selChannel = BENDING_TEST;
					SetPage(g_testTypeSelect.targetPage);
					break;
				case STRETCH_TEST:
					g_testTypeSelect.selChannel = STRETCH_TEST;
					SetPage(g_testTypeSelect.targetPage);
					break;
			}
		}
		else
		{
			SetPage(g_testTypeSelect.sourcePage);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTestAttribute
 * Description    : ��ȡ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TEST_ATTRIBUTE_TypeDef GetTestAttribute( uint8_t testIndex )
{
	TEST_ATTRIBUTE_TypeDef testAttribute;
	
	switch (testIndex)
	{
		case KYSNJS:
		case KYJZSJ:
		case KYHNT:
		case KZHNT:
		case KYQQZ:
		case KYZJDH:
		case KYTY:
			testAttribute = COMPRESSION_TEST;
			break;
		case KZSNJS:
		case KZYJSNJ:
		case KZTY:
			testAttribute = BENDING_TEST;
			break;
		case KLJSSW:
			testAttribute = STRETCH_TEST;
			break;
		default:
			testAttribute = COMPRESSION_TEST;
			break;
	}
	
	return testAttribute;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
