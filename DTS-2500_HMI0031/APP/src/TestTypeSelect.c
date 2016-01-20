/**
  ******************************************************************************
  * @file    TestTypeSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-12-24 15:26:15
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   试验类型选择程序
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
	INDEX_COMPRESSION_TEST = 0,	/* 压缩试验 */
	INDEX_BENDING_TEST,			/* 弯曲试验 */
	INDEX_STRETCH_TEST,			/* 拉伸试验 */
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
	BoolStatus isIndexMove;							//索引值移动
	LEAVE_PAGE_TypeDef leavePage;			
	PAGE_NAME_TypeDef sourcePage;					//源页
	PAGE_NAME_TypeDef targetPage;					//目标页
	PAGE_NAME_TypeDef nextPage;						//下一页
	TEST_ATTRIBUTE_TypeDef selChannel;
}TEST_TYPE_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTestTypeSelectParameterName[] =
{
	"压缩试验",
	"弯曲试验",
	"拉伸试验",
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
 * Description    : 试验类型选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestTypeSelectPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	TestTypeSelectInit();
	
	/* 参数配置 */
	TestTypeSelectConfig();
	
	/* 获取参数 */
	TestTypeSelectReadParameter();
	
	/* 画GUI框架 */
	GUI_TestTypeSelect();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_testTypeSelect.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		TestTypeSelectMoveIndexProcess();
		
		/* 移动光标 */
		TestTypeSelectMoveCursorProcess();
		
		/* 快捷菜单 */
		TestTypeSelectShortcutCheckCycle();
		
		/* 按键处理 */
		TestTypeSelectKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		TestTypeSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SetTestTypeSelectSourcePage
 * Description    : 设置通道选择来源页
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
 * Description    : 设置通道选择目标页
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
 * Description    : 获取选择的试验属性
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
 * Description    : 初始化
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
 * Description    : 配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestTypeSelectConfig( void )
{
	/* 标题 */
	g_testTypeSelect.pTitle = "试验类型选择";
	
	/* 试块个数 */
	g_testTypeSelect.menuCnt = TEST_TYPE_SELECT_MENU_CNT;
	
	/* 索引值 */		
	g_testTypeSelect.indexArray[INDEX_COMPRESSION_TEST] = COMPRESSION_TEST;	
	g_testTypeSelect.indexArray[INDEX_BENDING_TEST] 	= BENDING_TEST;	
	g_testTypeSelect.indexArray[INDEX_STRETCH_TEST] 	= STRETCH_TEST;
	
	/* 名称 */
	g_testTypeSelect.pParameterNameArray[INDEX_COMPRESSION_TEST]	= pTestTypeSelectParameterName[0];	
	g_testTypeSelect.pParameterNameArray[INDEX_BENDING_TEST] 		= pTestTypeSelectParameterName[1];	
	g_testTypeSelect.pParameterNameArray[INDEX_STRETCH_TEST] 		= pTestTypeSelectParameterName[2];
}

/*------------------------------------------------------------
 * Function Name  : TestTypeSelectReadParameter
 * Description    : 读参数
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
 * Description    : 配置界面GUI坐标
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
 * Description    : 一行界面GUI
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
 * Description    : 画界面GUI
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
 * Description    : 界面GUI
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
 * Description    : 移动索引值
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
 * Description    : 移动光标处理
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
 * Description    : 快捷键检测
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
 * Description    : 按键处理
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
 * Description    : 离开页检测
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
 * Description    : 获取试验的属性
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
