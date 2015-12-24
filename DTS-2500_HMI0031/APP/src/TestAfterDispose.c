/**
  ******************************************************************************
  * @file    TestAfterDispose.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-22 10:33:314
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   试验后处理
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TestAfterDispose.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				2
#define MAX_PARAMETER_PUTIN_BIT			2

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_PRINT_REPORT_TYPE = 0,
	OBJECT_CURVE_STAY_TIME,
}OBJECT_TEST_AFTER_DISPOSE_TypeDef;

typedef enum
{
	INDEX_PRINT_REPORT_TYPE = 0,
	INDEX_CURVE_STAY_TIME,
}INDEX_TEST_AFTER_DISPOSE_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	PARAMETER_TYPE_TypeDef parameterType[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pBoolSelect[2];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	FunctionalState enablePutin;					//使能输入
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}TEST_AFTER_DISPOSE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTestAfterDisposeName[] =
{
	"自动打印报告：",
	"曲线停留时间(S)：",
};

const char * const pTestAfterDisposeParameterName[] =
{
	"×",
	"√",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TEST_AFTER_DISPOSE_TypeDef g_testAfterDispose;

/* Private function prototypes -----------------------------------------------*/
static void TestAfterDisposeInit( void );
static void TestAfterDisposeParameterConfig( void );
static uint8_t GetTestAfterDisposeIndex( uint8_t handle );
static void TestAfterDisposeReadParameter( void );
static void GUI_TestAfterDispose( void );
static void Traverse_TestAfterDispose( void );
static void TestAfterDisposeMoveIndexProcess( void );
static void TestAfterDisposeMoveCursorProcess( void );
static void TestAfterDisposeShortcutCycleTask( void );
static void TestAfterDisposeKeyProcess( void );
static void TestAfterDisposeLeavePageCheckCycle( void );
static void TestAfterDisposePutinProcess( void );
static void TestAfterDisposeStatusProcess( void );


/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadTestAfterDisposePage
 * Description    : 试验后处理页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestAfterDisposePage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	TestAfterDisposeInit();
	
	/* 参数配置 */
	TestAfterDisposeParameterConfig();
	
	/* 获取参数 */
	TestAfterDisposeReadParameter();
	
	/* 画GUI框架 */
	GUI_TestAfterDispose();
	
	/* 遍历 */
	Traverse_TestAfterDispose();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_testAfterDispose.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		TestAfterDisposePutinProcess();
		
		/* 移动索引值 */
		TestAfterDisposeMoveIndexProcess();
		
		/* 状态处理 */
		TestAfterDisposeStatusProcess();
		
		/* 移动光标 */
		TestAfterDisposeMoveCursorProcess();
		
		/* 快捷菜单 */
		TestAfterDisposeShortcutCycleTask();
		
		/* 按键处理 */
		TestAfterDisposeKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		TestAfterDisposeLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeInit( void )
{
	g_testAfterDispose.isIndexMove = NO;		
	g_testAfterDispose.recordIndex = 0xff;
	g_testAfterDispose.refreshShortcut = ENABLE;
	g_testAfterDispose.leavePage.flagLeavePage = RESET;
	g_testAfterDispose.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeParameterConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeParameterConfig( void )
{
	/* 标题 */
	g_testAfterDispose.pTitle = "试验后处理";
	
	/* 试块个数 */
	g_testAfterDispose.curParameterNum = MAX_PARAMETER_CNT;
	
	/* 索引值 */
	g_testAfterDispose.indexArray[INDEX_PRINT_REPORT_TYPE] 	= OBJECT_PRINT_REPORT_TYPE;
	g_testAfterDispose.indexArray[INDEX_CURVE_STAY_TIME] 	= OBJECT_CURVE_STAY_TIME;
	
	/* 参数名称 */
	g_testAfterDispose.pParameterNameArray[INDEX_PRINT_REPORT_TYPE] = pTestAfterDisposeName[0];
	g_testAfterDispose.pParameterNameArray[INDEX_CURVE_STAY_TIME] 	= pTestAfterDisposeName[1];
	
	g_testAfterDispose.pBoolSelect[NO] = pTestAfterDisposeParameterName[0];
	g_testAfterDispose.pBoolSelect[YES] = pTestAfterDisposeParameterName[1];
	
	/* 数据保存类型 */
	g_testAfterDispose.oneLevelMenu[INDEX_PRINT_REPORT_TYPE].saveType 		= TYPE_INT;
	g_testAfterDispose.oneLevelMenu[INDEX_CURVE_STAY_TIME].saveType 		= TYPE_INT;
	
	/* 二级菜单类型 */
	g_testAfterDispose.parameterType[INDEX_PRINT_REPORT_TYPE] 	= NONE_PARAMETER;
	g_testAfterDispose.parameterType[INDEX_CURVE_STAY_TIME] 	= IMMEDIATELY_PUTIN_NONE;
	
	
}

/*------------------------------------------------------------
 * Function Name  : GetTestAfterDisposeIndex
 * Description    : 获取索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetTestAfterDisposeIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_testAfterDispose.curParameterNum; ++i)
	{
		if (g_testAfterDispose.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetTestAfterDisposeIndex(OBJECT_PRINT_REPORT_TYPE);
	if (index != 0xff)
	{
		if ( pHmi->print_mode )	//自动打印
		{
			strcpy(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[YES]);
		}
		else
		{
			strcpy(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[NO]);
		}
	}
	
	index = GetTestAfterDisposeIndex(OBJECT_CURVE_STAY_TIME);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_PUTIN_BIT,pHmi->curveStayTime,g_testAfterDispose.parameterData[index]);
	}
}	

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeWriteParameter( void )
{
	uint8_t index = 0;
	uint8_t tempu8 = 0;
	
	index = GetTestAfterDisposeIndex(OBJECT_PRINT_REPORT_TYPE);
	if (index != 0xff)
	{
		if ( strcmp(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[YES]) == 0 )
		{
			pHmi->print_mode = 1;
		}
		else
		{
			pHmi->print_mode = 0;
		}
	}
	
	index = GetTestAfterDisposeIndex(OBJECT_CURVE_STAY_TIME);
	if (index != 0xff)
	{
		tempu8 = ustrtoul(g_testAfterDispose.parameterData[index],0,10);
		
		pHmi->curveStayTime = tempu8;
	}
	
	pcm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigTestAfterDisposeParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestAfterDisposeParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 445;
	const uint16_t START_Y = 150;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_testAfterDispose.curParameterNum; ++i)
	{
		g_testAfterDispose.oneLevelMenu[i].x = startX;
		g_testAfterDispose.oneLevelMenu[i].y = startY;
		g_testAfterDispose.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_testAfterDispose.oneLevelMenu[i].backColor = COLOR_BACK;
		g_testAfterDispose.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_testAfterDispose.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_testAfterDispose.oneLevelMenu[i].lenth = 30;
		g_testAfterDispose.oneLevelMenu[i].width = 30;
		g_testAfterDispose.oneLevelMenu[i].fontSize = 24;
		g_testAfterDispose.oneLevelMenu[i].rowDistance = 24;
		g_testAfterDispose.oneLevelMenu[i].columnDistance = 24;
		g_testAfterDispose.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_testAfterDispose.oneLevelMenu[i].width + g_testAfterDispose.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDisposeDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDisposeDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_testAfterDispose.oneLevelMenu[index].x;
	rectangle.y = g_testAfterDispose.oneLevelMenu[index].y;
	rectangle.lenth = g_testAfterDispose.oneLevelMenu[index].lenth;
	rectangle.width = g_testAfterDispose.oneLevelMenu[index].width;
	rectangle.lineWidth = g_testAfterDispose.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_testAfterDispose.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDisposeRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDisposeRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_testAfterDispose.curParameterNum; ++i)
	{
		GUI_TestAfterDisposeDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDisposeDrawOneRowOneLevelMenu
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDisposeDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_testAfterDispose.oneLevelMenu[index].x - 9 * g_testAfterDispose.oneLevelMenu[index].fontSize;
	const uint16_t y = g_testAfterDispose.oneLevelMenu[index].y + g_testAfterDispose.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_testAfterDispose.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testAfterDispose.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_testAfterDispose.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDisposeOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDisposeOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_testAfterDispose.curParameterNum; ++i)
	{
		GUI_TestAfterDisposeDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDisposeCue
 * Description    : 提示信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDisposeCue( void )
{	
	uint8_t index = 0;
	uint16_t x,y,pointColor,backColor;
	
	index = GetTestAfterDisposeIndex(OBJECT_PRINT_REPORT_TYPE);
	if (index != 0xff)
	{
		x = g_testAfterDispose.oneLevelMenu[index].x - \
			12 * g_testAfterDispose.oneLevelMenu[index].fontSize;
		y = g_testAfterDispose.oneLevelMenu[index].y - \
			g_testAfterDispose.oneLevelMenu[index].rowDistance - 12;
		pointColor = g_testAfterDispose.oneLevelMenu[index].pointColor;
		backColor = g_testAfterDispose.oneLevelMenu[index].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,"一组试验结束后：");
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestAfterDispose
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestAfterDispose( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_testAfterDispose.pTitle);
	
	ConfigTestAfterDisposeParameterRectangleFrameCoordinate();
	
	GUI_TestAfterDisposeRectangleFrame();
	
	GUI_TestAfterDisposeOneLevelMenu();
	
	GUI_TestAfterDisposeCue();
}

/*------------------------------------------------------------
 * Function Name  : Show_TestAfterDisposeOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestAfterDisposeOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_testAfterDispose.oneLevelMenu[index].x + g_testAfterDispose.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_testAfterDispose.oneLevelMenu[index].y + g_testAfterDispose.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_testAfterDispose.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testAfterDispose.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_testAfterDispose.oneLevelMenu[index].lenth - 2 * g_testAfterDispose.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_testAfterDispose.oneLevelMenu[index].width - 2 * g_testAfterDispose.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_testAfterDispose.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_TestAfterDisposeOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestAfterDisposeOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_testAfterDispose.curParameterNum; ++i)
	{
		Show_TestAfterDisposeOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TestAfterDispose
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TestAfterDispose( void )
{
	Show_TestAfterDisposeOneLevelMenuContent();
}	

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposePutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposePutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_testAfterDispose.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_testAfterDispose.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_testAfterDispose.enablePutin;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_testAfterDispose.oneLevelMenu[index].x + g_testAfterDispose.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_testAfterDispose.oneLevelMenu[index].y + g_testAfterDispose.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_testAfterDispose.oneLevelMenu[index].lenth - 2 * g_testAfterDispose.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_testAfterDispose.oneLevelMenu[index].width - 2 * g_testAfterDispose.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_testAfterDispose.putinNum;
	pPutin->SaveType = g_testAfterDispose.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_testAfterDispose.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_testAfterDispose.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_testAfterDispose.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_testAfterDispose.curParameterNum;
	indexObj.pNowIndex = &g_testAfterDispose.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_testAfterDispose.nowIndex != g_testAfterDispose.recordIndex)
	{		
		g_testAfterDispose.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeStatusProcess( void )
{
	uint8_t index = 0;
	
	g_testAfterDispose.enablePutin = DISABLE;
	
	index = GetTestAfterDisposeIndex(OBJECT_CURVE_STAY_TIME);
	
	if(index == g_testAfterDispose.nowIndex)
	{
		g_testAfterDispose.enablePutin = ENABLE;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeMoveCursorProcess( void )
{
	if (g_testAfterDispose.isIndexMove == YES)
	{		
		if (g_testAfterDispose.recordIndex != 0xff)
		{
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.recordIndex].pointColor = g_testAfterDispose.oneLevelMenu[g_testAfterDispose.recordIndex].recordPointColor;
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.recordIndex].backColor = g_testAfterDispose.oneLevelMenu[g_testAfterDispose.recordIndex].recordBackColor;
			
			Show_TestAfterDisposeOneRowOneLevelMenuContent(g_testAfterDispose.recordIndex);		
			
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_TestAfterDisposeOneRowOneLevelMenuContent(g_testAfterDispose.nowIndex);	
		}
		else
		{
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_testAfterDispose.oneLevelMenu[g_testAfterDispose.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_TestAfterDisposeOneRowOneLevelMenuContent(g_testAfterDispose.nowIndex);
		}
		
		g_testAfterDispose.recordIndex = g_testAfterDispose.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_testAfterDispose.refreshShortcut == ENABLE)
	{
		g_testAfterDispose.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeIndexUpdate( void )
{
	g_testAfterDispose.nowIndex++;
	g_testAfterDispose.nowIndex %= g_testAfterDispose.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeUpdateStatus( void )
{
	TestAfterDisposeIndexUpdate();
	
	g_testAfterDispose.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeKeyProcess( void )
{
	uint8_t index = g_testAfterDispose.nowIndex;	
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				if (g_testAfterDispose.parameterType[index] == NONE_PARAMETER)
				{
					if ( strcmp(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[YES]) == 0 )
					{
						strcpy(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[NO]);
					}
					else
					{
						strcpy(g_testAfterDispose.parameterData[index],g_testAfterDispose.pBoolSelect[YES]);
					}
				}
				
				if (g_testAfterDispose.parameterType[index] == IMMEDIATELY_PUTIN_NONE)
				{
					putinStatus = GetPutinStatus();
						
					switch ( putinStatus )
					{
						case STATUS_DISABLE_PUTIN:
							return;
						
						case STATUS_EDIT_COMP:							
							switch ( g_testAfterDispose.oneLevelMenu[index].saveType )
							{
								case TYPE_INT:
									numtochar(g_testAfterDispose.putinNum,*GetPutinIntDataAddr(),g_testAfterDispose.parameterData[index]);
									break;
								case TYPE_FLOAT:
									floattochar(g_testAfterDispose.putinNum,g_testAfterDispose.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_testAfterDispose.parameterData[index]);
									break;
								case TYPE_CHAR:
									if ( strcmp(GetPutinCharDataAddr(),"") )
									{
										strcpy(g_testAfterDispose.parameterData[index],GetPutinCharDataAddr());
									}
									break;
							}	
							break;						
						
						default:
							break;
					}
				}
				
				TestAfterDisposeUpdateStatus();
				break;
				
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_testAfterDispose.leavePage.flagLeavePage = SET;
				g_testAfterDispose.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F4:
			case KEY_ESC:
				SetPage(SYSTEM_SET);
				g_testAfterDispose.leavePage.flagLeavePage = SET;
				g_testAfterDispose.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestAfterDisposeLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestAfterDisposeLeavePageCheckCycle( void )
{
	if (g_testAfterDispose.leavePage.flagLeavePage == SET)
	{
		if (g_testAfterDispose.leavePage.flagSaveData == SET)
		{			
			TestAfterDisposeWriteParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : iSAutoPrintReport
 * Description    : 是否自动打印报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus iSAutoPrintReport( void )
{
	if (pHmi->print_mode)
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : GetCurveStayTime
 * Description    : 获取曲线停留时间 单位：s
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetCurveStayTime( void )
{
	return pHmi->curveStayTime;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
