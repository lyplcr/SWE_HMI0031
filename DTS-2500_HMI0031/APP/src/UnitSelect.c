/**
  ******************************************************************************
  * @file    UnitSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-23 14:47:31
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   单位选择
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
}OBJECT_UNIT_SELECT_TypeDef;

typedef enum
{
	INDEX_KY_UNIT = 0,
}INDEX_UNIT_SELECT_TypeDef;

typedef enum
{
	INDEX_KN = 0,
	INDEX_N,
}KY_UNIT_INDEX_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pBoolSelect[2];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}UNIT_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pUnitSelectName[] =
{
	"抗压单位：",
};

const char * const pUnitSelectParameterName[] =
{
	"kN",
	"N",
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

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadUnitSelectPage
 * Description    : 单位选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadUnitSelectPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	UnitSelectInit();
	
	/* 参数配置 */
	UnitSelectParameterConfig();
	
	/* 获取参数 */
	UnitSelectReadParameter();
	
	/* 画GUI框架 */
	GUI_UnitSelect();
	
	/* 遍历 */
	Traverse_UnitSelect();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_unitSelect.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		UnitSelectMoveIndexProcess();
		
		/* 移动光标 */
		UnitSelectMoveCursorProcess();
		
		/* 快捷菜单 */
		UnitSelectShortcutCycleTask();
		
		/* 按键处理 */
		UnitSelectKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		UnitSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectInit
 * Description    : 初始化
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
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectParameterConfig( void )
{
	/* 试块个数 */
	g_unitSelect.curParameterNum = MAX_PARAMETER_CNT;
	
	/* 索引值 */
	g_unitSelect.indexArray[INDEX_KY_UNIT] = OBJECT_KY_UNIT;
	
	/* 参数名称 */
	g_unitSelect.pParameterNameArray[INDEX_KY_UNIT] = pUnitSelectName[0];
	
	g_unitSelect.pBoolSelect[INDEX_KN] = pUnitSelectParameterName[0];
	g_unitSelect.pBoolSelect[INDEX_N] = pUnitSelectParameterName[1];
	
	/* 标题 */
	g_unitSelect.pTitle = "单位选择";
}

/*------------------------------------------------------------
 * Function Name  : GetUnitSelectIndex
 * Description    : 获取索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetUnitSelectIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
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
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetUnitSelectIndex(OBJECT_KY_UNIT);
	if (index != 0xff)
	{
		if ( pHmi->unit )	//自动打印
		{
			strcpy(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[INDEX_N]);
		}
		else
		{
			strcpy(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[INDEX_KN]);
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : UnitSelectWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectWriteParameter( void )
{
	uint8_t index = 0;
	
	index = GetUnitSelectIndex(OBJECT_KY_UNIT);
	if (index != 0xff)
	{
		if ( strcmp(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[INDEX_N]) == 0 )
		{
			pHmi->unit = 1;
		}
		else
		{
			pHmi->unit = 0;
		}
	}
	
	pcm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigUnitSelectParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigUnitSelectParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 445;
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
		g_unitSelect.oneLevelMenu[i].lenth = 30;
		g_unitSelect.oneLevelMenu[i].width = 30;
		g_unitSelect.oneLevelMenu[i].fontSize = 24;
		g_unitSelect.oneLevelMenu[i].rowDistance = 48;
		g_unitSelect.oneLevelMenu[i].columnDistance = 48;
		g_unitSelect.oneLevelMenu[i].lineWidth = 2;
		
		startX += g_unitSelect.oneLevelMenu[i].lenth + g_unitSelect.oneLevelMenu[i].columnDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_UnitSelectDrawOneRectangleFrame
 * Description    : 参数界面GUI
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
 * Description    : 参数界面GUI
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
 * Description    : 参数界面GUI
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
 * Description    : 参数界面一级菜单
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
 * Description    : 界面GUI
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
 * Description    : 显示一行试验参数
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
 * Description    : 显示一级菜单内容
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
 * Description    : 遍历
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
 * Description    : 参数移动索引值
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
 * Function Name  : UnitSelectMoveCursorProcess
 * Description    : 移动光标处理
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
			g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].pointColor = g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordPointColor;
			g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].backColor = g_unitSelect.oneLevelMenu[g_unitSelect.recordIndex].recordBackColor;
			
			Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.recordIndex);		
			
			g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.nowIndex);	
		}
		else
		{
			g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_unitSelect.oneLevelMenu[g_unitSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_UnitSelectOneRowOneLevelMenuContent(g_unitSelect.nowIndex);
		}
		
		g_unitSelect.recordIndex = g_unitSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectShortcutCycleTask
 * Description    : 快捷菜单任务
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
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectIndexUpdate( void )
{
	g_unitSelect.recordIndex = 0xff;
}

/*------------------------------------------------------------
 * Function Name  : UnitSelectUpdateStatus
 * Description    : 更新状态
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
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UnitSelectKeyProcess( void )
{
	uint8_t index = g_unitSelect.nowIndex;	
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				if ( strcmp(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[YES]) == 0 )
				{
					strcpy(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[NO]);
				}
				else
				{
					strcpy(g_unitSelect.parameterData[index],g_unitSelect.pBoolSelect[YES]);
				}
				UnitSelectUpdateStatus();
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
 * Description    : 离开页检测
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
