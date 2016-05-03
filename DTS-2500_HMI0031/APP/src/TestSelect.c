/**
  ******************************************************************************
  * @file    TestSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-13 14:25:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   试验选择文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TestSelect.h"
#include "ModelSelect.h"
#include "TestTypeSelect.h"

/* Private define ------------------------------------------------------------*/
#define MAX_TEST_CNT					9
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
	INDEX_KYSNJS = 0,
	INDEX_KYJZSJ,
	INDEX_KYHNT,
	INDEX_KYQQZ,
	INDEX_KYTY,
}REAL_TEST_INDEX_TypeDef;

typedef enum
{
	INDEX_KZ_MODEL_KZSNJS = 0,
	INDEX_KZ_MODEL_KZYJSNJ,
	INDEX_KZ_KZHNT,
	INDEX_KZ_MODEL_KZTY,
}INDEX_KZ_MODEL_TypeDef;

typedef enum
{
	INDEX_KL_MODEL_KLJSSW = 0,
}INDEX_KL_MODEL_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	TEST_TYPE_TypeDef indexArray[MAX_TEST_CNT];
	uint8_t indexTest[MAX_TEST_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_TEST_CNT];
	uint8_t testCnt;	
	uint8_t rowNum;
	uint8_t columnNum;
	const char *pParameterNameArray[MAX_TEST_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//索引值移动
	TEST_TYPE_TypeDef testType;
	LEAVE_PAGE_TypeDef leavePage;						
}TEST_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* 标题名称 */
const char * const pTestSelectName[] = 
{
	"1、水泥胶砂抗压",
	"2、建筑砂浆抗压",
	"3、混凝土抗压",
	"4、砌墙砖抗压",
	"5、通用抗压",		
};

const char * const pModelKZTestSelectName[] = 
{
	"1、水泥胶砂抗折",
	"2、压浆水泥浆抗折",
	"3、混凝土抗折",
	"4、通用抗折",		
};

const char * const pKLTestSelectName[] = 
{
	"1、金属室温拉伸",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TEST_SELECT_TypeDef g_testSelect;
static void TestSelectConfig( void );

/* Private function prototypes -----------------------------------------------*/
static void TestSelectInit( void );
static void GUI_TestSelect( void );
static void TestSelectMoveIndexProcess( void );
static void TestSelectMoveCursorProcess( void );
static uint8_t AccordingTestTypeConvIndex( TEST_TYPE_TypeDef type );
static TEST_TYPE_TypeDef AccordingIndexConvTestType( uint8_t index );
static void TestSelectKeyProcess( void );
static void TestSelectLeavePageCheckCycle( void );
static void TestSelectStatusProcess( void );
static void TestSelectReadParameter( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadTestSelectPage
 * Description    : 试验选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestSelectPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	TestSelectInit();
	
	/* 参数配置 */
	TestSelectConfig();	
	
	/* 读取参数 */
	TestSelectReadParameter();
	
	/* 画GUI框架 */
	GUI_TestSelect();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_testSelect.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		TestSelectMoveIndexProcess();
		
		/* 状态处理 */
		TestSelectStatusProcess();
		
		/* 移动光标 */
		TestSelectMoveCursorProcess();
		
		/* 按键处理 */
		TestSelectKeyProcess();
		
		/* 离开页 */
		TestSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestSelectInit
 * Description    : 试验选择初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectInit( void )
{
	g_testSelect.testType = (TEST_TYPE_TypeDef)pHmi->test_standard_index;
	
	g_testSelect.nowIndex = 0;
	g_testSelect.recordIndex = 0xff;
	
	g_testSelect.leavePage.flagLeavePage = RESET;
	g_testSelect.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : TestSelectShortcutCheckCycle
 * Description    : 快捷键检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue>0) && (keyValue<=g_testSelect.testCnt))
	{
		SetPage(TEST_PARAMETER_PAGE);
		g_testSelect.leavePage.flagLeavePage = SET;
		g_testSelect.leavePage.flagSaveData = SET;
		
		g_testSelect.nowIndex = keyValue - 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : AccordingTestTypeConvIndex
 * Description    : 根据试验类型转换为索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t AccordingTestTypeConvIndex( TEST_TYPE_TypeDef type )
{
	uint8_t i;
	uint8_t index = 0xff;
	
	for (i=0; i<g_testSelect.testCnt; ++i)
	{
		if (g_testSelect.indexArray[i] == type)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : AccordingIndexConvTestType
 * Description    : 根据索引值转换为试验类型
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TEST_TYPE_TypeDef AccordingIndexConvTestType( uint8_t index )
{	
	return g_testSelect.indexArray[index];
}

/*------------------------------------------------------------
 * Function Name  : TestSelectConfig
 * Description    : 试验选择配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectConfig( void )
{
	/* 标题 */
	g_testSelect.pTitle = "试验选择";
	
	switch ( GetSelectTestAttribute() )
	{
		case COMPRESSION_TEST:
			/* 试块个数 */
			g_testSelect.testCnt = 5;
			
			/* 行列数 */
			g_testSelect.rowNum = 5;
			g_testSelect.columnNum = 1;
			
			/* 索引值 */		
			g_testSelect.indexArray[INDEX_KYSNJS] 	= KYSNJS;	
			g_testSelect.indexArray[INDEX_KYJZSJ] 	= KYJZSJ;		
			g_testSelect.indexArray[INDEX_KYHNT] 	= KYHNT;					
			g_testSelect.indexArray[INDEX_KYQQZ] 	= KYQQZ;
			g_testSelect.indexArray[INDEX_KYTY] 	= KYTY;		
			
			/* 名称 */
			g_testSelect.pParameterNameArray[INDEX_KYSNJS] 	= pTestSelectName[0];	
			g_testSelect.pParameterNameArray[INDEX_KYJZSJ] 	= pTestSelectName[1];		
			g_testSelect.pParameterNameArray[INDEX_KYHNT] 	= pTestSelectName[2];				
			g_testSelect.pParameterNameArray[INDEX_KYQQZ] 	= pTestSelectName[3];
			g_testSelect.pParameterNameArray[INDEX_KYTY] 	= pTestSelectName[4];
			break;
		case BENDING_TEST:
			/* 试块个数 */
			g_testSelect.testCnt = 4;
			
			/* 行数 */
			g_testSelect.rowNum = 4;
			g_testSelect.columnNum = 1;
			
			/* 索引值 */		
			g_testSelect.indexArray[INDEX_KZ_MODEL_KZSNJS] 	= KZSNJS;	
			g_testSelect.indexArray[INDEX_KZ_MODEL_KZYJSNJ] = KZYJSNJ;	
			g_testSelect.indexArray[INDEX_KZ_KZHNT] 		= KZHNT;
			g_testSelect.indexArray[INDEX_KZ_MODEL_KZTY] 	= KZTY;			
			
			/* 名称 */
			g_testSelect.pParameterNameArray[INDEX_KZ_MODEL_KZSNJS] 	= pModelKZTestSelectName[0];	
			g_testSelect.pParameterNameArray[INDEX_KZ_MODEL_KZYJSNJ] 	= pModelKZTestSelectName[1];		
			g_testSelect.pParameterNameArray[INDEX_KZ_KZHNT] 			= pModelKZTestSelectName[2];	
			g_testSelect.pParameterNameArray[INDEX_KZ_MODEL_KZTY] 		= pModelKZTestSelectName[3];
			break;
		case STRETCH_TEST:
			/* 试块个数 */
			g_testSelect.testCnt = 1;
			
			/* 行数 */
			g_testSelect.rowNum = 1;
			g_testSelect.columnNum = 1;
			
			/* 索引值 */		
			g_testSelect.indexArray[INDEX_KL_MODEL_KLJSSW] 	= KLJSSW;			
			
			/* 名称 */
			g_testSelect.pParameterNameArray[INDEX_KL_MODEL_KLJSSW] = pKLTestSelectName[0];	
			break;
		case INVALID_TEST:
			break;
		default:
			break;
	}
}	

/*------------------------------------------------------------
 * Function Name  : TestSelectReadParameter
 * Description    : 读取参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectReadParameter( void )
{
	uint8_t index = 0;
	
	index = AccordingTestTypeConvIndex(g_testSelect.testType);
	
	if (index != 0xff)
	{
		g_testSelect.nowIndex = index;
	}
}	

/*------------------------------------------------------------
 * Function Name  : TestSelectLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectLeavePageCheckCycle( void )
{
	if (g_testSelect.leavePage.flagLeavePage == SET)
	{
		if (g_testSelect.leavePage.flagSaveData == SET)
		{
			pHmi->test_standard_index = AccordingIndexConvTestType(g_testSelect.nowIndex);
			pcm_save();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestSelectKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				SetPage(TEST_PARAMETER_PAGE);
				g_testSelect.leavePage.flagLeavePage = SET;
				g_testSelect.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				SetPage(MAIN_PAGE);
				g_testSelect.leavePage.flagLeavePage = SET;
				g_testSelect.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	TestSelectShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : ConfigTestSelectRectangleFrameCoordinate
 * Description    : 配置试验选择界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestSelectRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 60;
	const uint16_t INIT_START_Y = 74;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_testSelect.testCnt; ++i)
	{
		g_testSelect.oneLevelMenu[i].x = startX;
		g_testSelect.oneLevelMenu[i].y = startY;
		g_testSelect.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_testSelect.oneLevelMenu[i].backColor = COLOR_POINT;
		g_testSelect.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_testSelect.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_testSelect.oneLevelMenu[i].lenth = 320;
		g_testSelect.oneLevelMenu[i].width = 36;
		g_testSelect.oneLevelMenu[i].fontSize = 32;
		g_testSelect.oneLevelMenu[i].rowDistance = 25;
		g_testSelect.oneLevelMenu[i].columnDistance = 0;
		
		if (i == g_testSelect.rowNum - 1)
		{
			startX += g_testSelect.oneLevelMenu[i].lenth + 40;
			startY = INIT_START_Y;
		}
		else
		{
			startY += g_testSelect.oneLevelMenu[i].rowDistance + g_testSelect.oneLevelMenu[i].width;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestSelectDrawOneRectangleFrame
 * Description    : 试验选择一行界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestSelectDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_testSelect.oneLevelMenu[index].x;
	const uint16_t y = g_testSelect.oneLevelMenu[index].y;
	const uint16_t lenth = g_testSelect.oneLevelMenu[index].lenth;
	const uint16_t width = g_testSelect.oneLevelMenu[index].width;
	const uint16_t backColor = g_testSelect.oneLevelMenu[index].backColor;
	
	lcd_fill(x,y,lenth,width,backColor);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestSelectRectangleFrame
 * Description    : 试验选择界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestSelectRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_testSelect.testCnt; ++i)
	{
		GUI_TestSelectDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestSelectDrawOneRowOneLevelMenu
 * Description    : 一行参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestSelectDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_testSelect.oneLevelMenu[index].x + 16;
	const uint16_t y = g_testSelect.oneLevelMenu[index].y + 1;
	const uint16_t pointColor = g_testSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testSelect.oneLevelMenu[index].backColor;
	
	GUI_DispStr32At(x,y,pointColor,backColor,g_testSelect.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestSelectOneLevelMenu
 * Description    : 界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestSelectOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_testSelect.testCnt; ++i)
	{
		GUI_TestSelectDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestSelect
 * Description    : 试验选择界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestSelect( void )
{	
	GUI_DrawGeneralTitleRectangleFrame(g_testSelect.pTitle);
	
	ConfigTestSelectRectangleFrameCoordinate();
	
	GUI_TestSelectRectangleFrame();
	
	GUI_TestSelectOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : TestParameterMoveIndexProcess
 * Description    : 试验参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_testSelect.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_testSelect.rowNum;
	indexObj.colNum = g_testSelect.columnNum;
	indexObj.sumNum = g_testSelect.testCnt;
	indexObj.pNowIndex = &g_testSelect.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_testSelect.nowIndex != g_testSelect.recordIndex)
	{		
		g_testSelect.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestSelectMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectMoveCursorProcess( void )
{
	if (g_testSelect.isIndexMove == YES)
	{		
		if (g_testSelect.recordIndex != 0xff)
		{
			g_testSelect.oneLevelMenu[g_testSelect.recordIndex].pointColor = g_testSelect.oneLevelMenu[g_testSelect.recordIndex].recordPointColor;
			g_testSelect.oneLevelMenu[g_testSelect.recordIndex].backColor = g_testSelect.oneLevelMenu[g_testSelect.recordIndex].recordBackColor;
			
			GUI_TestSelectDrawOneRectangleFrame(g_testSelect.recordIndex);
			GUI_TestSelectDrawOneRowOneLevelMenu(g_testSelect.recordIndex);
			
			g_testSelect.oneLevelMenu[g_testSelect.nowIndex].pointColor = COLOR_POINT;
			g_testSelect.oneLevelMenu[g_testSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_TestSelectDrawOneRectangleFrame(g_testSelect.nowIndex);
			GUI_TestSelectDrawOneRowOneLevelMenu(g_testSelect.nowIndex);
		}
		else
		{
			g_testSelect.oneLevelMenu[g_testSelect.nowIndex].pointColor = COLOR_POINT;
			g_testSelect.oneLevelMenu[g_testSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_TestSelectDrawOneRectangleFrame(g_testSelect.nowIndex);
			GUI_TestSelectDrawOneRowOneLevelMenu(g_testSelect.nowIndex);
		}
		
		g_testSelect.recordIndex = g_testSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestSelectStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSelectStatusProcess( void )
{
	g_testSelect.nowIndex %= g_testSelect.testCnt;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
