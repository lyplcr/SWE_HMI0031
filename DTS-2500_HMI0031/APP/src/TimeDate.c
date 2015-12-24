/**
  ******************************************************************************
  * @file    TimeDate.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-20 13:18:58
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   时间日期程序
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TimeDate.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				6
#define MAX_PARAMETER_PUTIN_BIT			2

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_YEAR = 0,
	OBJECT_MONTH,
	OBJECT_DATE,
	OBJECT_HOUR,
	OBJECT_MINITER,
	OBJECT_SECOND,
}OBJECT_TIME_DATE_TypeDef;

typedef enum
{
	INDEX_YEAR = 0,
	INDEX_MONTH,
	INDEX_DATE,
	INDEX_HOUR,
	INDEX_MINITER,
	INDEX_SECOND,
}INDEX_TIME_DATE_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pCutOffRuleNameArray[MAX_PARAMETER_CNT];	//分割线
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}TIME_DATE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTimeDateName[] =
{
	"年",
	"月",
	"日",
	"时",
	"分",
	"秒",
};

const char * const pCutOffRuleName[] =
{
	"-",
	"-",
	"-",
	":",
	":",
	":",
};

const char * const pTimeDateErrorCue[] =
{
	"月输入范围：1 ~ 12",		//0
	"日输入范围出错",			//1
	"时输入范围：0 ~ 23",		//2
	"分输入范围：0 ~ 59",		//3
	"秒输入范围：0 ~ 59",		//4
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TIME_DATE_TypeDef g_timeDate;

/* Private function prototypes -----------------------------------------------*/
static void TimeDateInit( void );
static void TimeDateParameterConfig( void );
static void TimeDateReadParameter( void );
static void GUI_TimeDate( void );
static void Traverse_TimeDate( void );
static void TimeDatePutinProcess( void );
static void TimeDateMoveIndexProcess( void );
static void TimeDateMoveCursorProcess( void );
static void TimeDateShortcutCycleTask( void );
static void TimeDateKeyProcess( void );
static void TimeDateLeavePageCheckCycle( void );
static TestStatus TimeDateCheckDataCycle( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadTimeDatePage
 * Description    : 时间日期页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTimeDatePage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	TimeDateInit();
	
	/* 参数配置 */
	TimeDateParameterConfig();
	
	/* 获取参数 */
	TimeDateReadParameter();
	
	/* 画GUI框架 */
	GUI_TimeDate();
	
	/* 遍历 */
	Traverse_TimeDate();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_timeDate.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		TimeDatePutinProcess();
		
		/* 移动索引值 */
		TimeDateMoveIndexProcess();
		
		/* 移动光标 */
		TimeDateMoveCursorProcess();
		
		/* 快捷菜单 */
		TimeDateShortcutCycleTask();
		
		/* 按键处理 */
		TimeDateKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		TimeDateLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TimeDateInit
 * Description    : 时间日期初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateInit( void )
{
	g_timeDate.isIndexMove = NO;		
	g_timeDate.recordIndex = 0xff;
	g_timeDate.refreshShortcut = ENABLE;
	g_timeDate.leavePage.flagLeavePage = RESET;
	g_timeDate.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : TimeDateParameterConfig
 * Description    : 时间日期参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateParameterConfig( void )
{
	/* 试块个数 */
	g_timeDate.curParameterNum = MAX_PARAMETER_CNT;
	
	/* 索引值 */
	g_timeDate.indexArray[INDEX_YEAR] 		= OBJECT_YEAR;
	g_timeDate.indexArray[INDEX_MONTH] 		= OBJECT_MONTH;
	g_timeDate.indexArray[INDEX_DATE] 		= OBJECT_DATE;
	g_timeDate.indexArray[INDEX_HOUR] 		= OBJECT_HOUR;
	g_timeDate.indexArray[INDEX_MINITER] 	= OBJECT_MINITER;
	g_timeDate.indexArray[INDEX_SECOND] 	= OBJECT_SECOND;
	
	/* 参数名称 */
	g_timeDate.pParameterNameArray[INDEX_YEAR] 		= pTimeDateName[0];
	g_timeDate.pParameterNameArray[INDEX_MONTH] 	= pTimeDateName[1];
	g_timeDate.pParameterNameArray[INDEX_DATE] 		= pTimeDateName[2];
	g_timeDate.pParameterNameArray[INDEX_HOUR] 		= pTimeDateName[3];
	g_timeDate.pParameterNameArray[INDEX_MINITER] 	= pTimeDateName[4];
	g_timeDate.pParameterNameArray[INDEX_SECOND] 	= pTimeDateName[5];
	
	/* 分割线名称*/
	g_timeDate.pCutOffRuleNameArray[INDEX_YEAR] 	= pCutOffRuleName[0];
	g_timeDate.pCutOffRuleNameArray[INDEX_MONTH] 	= pCutOffRuleName[1];
	g_timeDate.pCutOffRuleNameArray[INDEX_DATE] 	= pCutOffRuleName[2];
	g_timeDate.pCutOffRuleNameArray[INDEX_HOUR] 	= pCutOffRuleName[3];
	g_timeDate.pCutOffRuleNameArray[INDEX_MINITER] 	= pCutOffRuleName[4];
	g_timeDate.pCutOffRuleNameArray[INDEX_SECOND] 	= pCutOffRuleName[5];
	
	/* 标题 */
	g_timeDate.pTitle = "时间日期设置";
	
	/* 数据保存类型 */
	g_timeDate.oneLevelMenu[INDEX_YEAR].saveType 		= TYPE_INT;
	g_timeDate.oneLevelMenu[INDEX_MONTH].saveType 		= TYPE_INT;
	g_timeDate.oneLevelMenu[INDEX_DATE].saveType 		= TYPE_INT;
	g_timeDate.oneLevelMenu[INDEX_HOUR].saveType 		= TYPE_INT;
	g_timeDate.oneLevelMenu[INDEX_MINITER].saveType 	= TYPE_INT;
	g_timeDate.oneLevelMenu[INDEX_SECOND].saveType 		= TYPE_INT;	
}

/*------------------------------------------------------------
 * Function Name  : GetTimeDateIndex
 * Description    : 获取日期时间索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetTimeDateIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_timeDate.curParameterNum; ++i)
	{
		if (g_timeDate.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : TimeDateReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateReadParameter( void )
{
	tTime t;
	uint8_t index = 0;
	char buff[5];
	
	time_cycle();
	
	t = get_time();	 
	
	index = GetTimeDateIndex(OBJECT_YEAR);
	if (index != 0xff)
	{
		usprintf(buff,"%04d",t.usYear);		
		strcpy(g_timeDate.parameterData[index],&buff[2]);
	}
	
	index = GetTimeDateIndex(OBJECT_MONTH);
	if (index != 0xff)
	{
		usprintf(buff,"%02d",t.ucMon);		
		strcpy(g_timeDate.parameterData[index],buff);
	}
	
	index = GetTimeDateIndex(INDEX_DATE);
	if (index != 0xff)
	{
		usprintf(buff,"%02d",t.ucMday);		
		strcpy(g_timeDate.parameterData[index],buff);
	}
	
	index = GetTimeDateIndex(INDEX_HOUR);
	if (index != 0xff)
	{
		usprintf(buff,"%02d",t.ucHour);		
		strcpy(g_timeDate.parameterData[index],buff);
	}
	
	index = GetTimeDateIndex(INDEX_MINITER);
	if (index != 0xff)
	{
		usprintf(buff,"%02d",t.ucMin);		
		strcpy(g_timeDate.parameterData[index],buff);
	}
	
	index = GetTimeDateIndex(INDEX_SECOND);
	if (index != 0xff)
	{
		usprintf(buff,"%02d",t.ucSec);		
		strcpy(g_timeDate.parameterData[index],buff);
	}
}	

/*------------------------------------------------------------
 * Function Name  : TimeDateWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateWriteParameter( void )
{
	uint8_t index = 0;
	uint32_t tempu = 0;
	tTime t;
	
	index = GetTimeDateIndex(OBJECT_YEAR);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.usYear = 2000 + tempu;
	}
	
	index = GetTimeDateIndex(OBJECT_MONTH);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.ucMon = tempu;
	}
	
	index = GetTimeDateIndex(INDEX_DATE);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.ucMday = tempu;
	}
	
	index = GetTimeDateIndex(INDEX_HOUR);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.ucHour = tempu;
	}
	
	index = GetTimeDateIndex(INDEX_MINITER);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.ucMin = tempu;
	}
	
	index = GetTimeDateIndex(INDEX_SECOND);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		t.ucSec = tempu;
	}
	
	time_set(t);
}

/*------------------------------------------------------------
 * Function Name  : ConfigTimeDateParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTimeDateParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 190;
	const uint16_t START_Y = 240;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_timeDate.curParameterNum; ++i)
	{
		g_timeDate.oneLevelMenu[i].x = startX;
		g_timeDate.oneLevelMenu[i].y = startY;
		g_timeDate.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_timeDate.oneLevelMenu[i].backColor = COLOR_BACK;
		g_timeDate.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_timeDate.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_timeDate.oneLevelMenu[i].lenth = 30;
		g_timeDate.oneLevelMenu[i].width = 30;
		g_timeDate.oneLevelMenu[i].fontSize = 24;
		g_timeDate.oneLevelMenu[i].rowDistance = 48;
		g_timeDate.oneLevelMenu[i].columnDistance = 48;
		g_timeDate.oneLevelMenu[i].lineWidth = 2;
		
		startX += g_timeDate.oneLevelMenu[i].lenth + g_timeDate.oneLevelMenu[i].columnDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_timeDate.oneLevelMenu[index].x;
	rectangle.y = g_timeDate.oneLevelMenu[index].y;
	rectangle.lenth = g_timeDate.oneLevelMenu[index].lenth;
	rectangle.width = g_timeDate.oneLevelMenu[index].width;
	rectangle.lineWidth = g_timeDate.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_timeDate.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_timeDate.curParameterNum; ++i)
	{
		GUI_TimeDateDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateDrawOneRowOneLevelMenu
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_timeDate.oneLevelMenu[index].x + 3;
	const uint16_t y = g_timeDate.oneLevelMenu[index].y - 72;
	const uint16_t pointColor = g_timeDate.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_timeDate.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_timeDate.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_timeDate.curParameterNum; ++i)
	{
		GUI_TimeDateDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateCutOffCuleSingleOneLevelMenu
 * Description    : 单个分割线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateCutOffRuleSingleOneLevelMenu( uint8_t index )
{
	uint16_t tempLen = (g_timeDate.oneLevelMenu[index].columnDistance - (g_timeDate.oneLevelMenu[index].fontSize>>1)) >> 1;
	const uint16_t lenth = g_timeDate.oneLevelMenu[index].lenth;
	const uint16_t x = g_timeDate.oneLevelMenu[index].x + lenth + tempLen;
	const uint16_t y = g_timeDate.oneLevelMenu[index].y + g_timeDate.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_timeDate.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_timeDate.oneLevelMenu[index].backColor;
		
	GUI_DispStr24At(x,y,pointColor,backColor,g_timeDate.pCutOffRuleNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateCutOffCuleOneLevelMenu
 * Description    : 参数界面一级菜单分割线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDateCutOffRuleOneLevelMenu( void )
{	
	uint8_t i;
	uint8_t index = GetTimeDateIndex(OBJECT_DATE);
	
	for (i=0; i<g_timeDate.curParameterNum - 1; ++i)
	{
		if (index == i)
		{
			continue;
		}
		
		GUI_TimeDateCutOffRuleSingleOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDate
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TimeDate( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_timeDate.pTitle);
	
	ConfigTimeDateParameterRectangleFrameCoordinate();
	
	GUI_TimeDateRectangleFrame();
	
	GUI_TimeDateOneLevelMenu();
	
	GUI_TimeDateCutOffRuleOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_TimeDateDataOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TimeDateDataOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_timeDate.oneLevelMenu[index].x + g_timeDate.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_timeDate.oneLevelMenu[index].y + g_timeDate.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_timeDate.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_timeDate.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_timeDate.oneLevelMenu[index].lenth - 2 * g_timeDate.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_timeDate.oneLevelMenu[index].width - 2 * g_timeDate.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_timeDate.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_TimeDateDataOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TimeDateDataOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_timeDate.curParameterNum; ++i)
	{
		Show_TimeDateDataOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TimeDate
 * Description    : 遍历时间日期
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TimeDate( void )
{
	Show_TimeDateDataOneLevelMenuContent();
}	

/*------------------------------------------------------------
 * Function Name  : TimeDatePutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDatePutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_timeDate.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_timeDate.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_timeDate.oneLevelMenu[index].x + g_timeDate.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_timeDate.oneLevelMenu[index].y + g_timeDate.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_timeDate.oneLevelMenu[index].lenth - 2 * g_timeDate.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_timeDate.oneLevelMenu[index].width - 2 * g_timeDate.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_timeDate.putinNum;
	pPutin->SaveType = g_timeDate.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_timeDate.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : TimeDateMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_timeDate.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = 1;
	indexObj.colNum = g_timeDate.curParameterNum;
	indexObj.sumNum = g_timeDate.curParameterNum;
	indexObj.pNowIndex = &g_timeDate.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_timeDate.nowIndex != g_timeDate.recordIndex)
	{		
		g_timeDate.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TimeDateMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateMoveCursorProcess( void )
{
	if (g_timeDate.isIndexMove == YES)
	{		
		if (g_timeDate.recordIndex != 0xff)
		{
			g_timeDate.oneLevelMenu[g_timeDate.recordIndex].pointColor = g_timeDate.oneLevelMenu[g_timeDate.recordIndex].recordPointColor;
			g_timeDate.oneLevelMenu[g_timeDate.recordIndex].backColor = g_timeDate.oneLevelMenu[g_timeDate.recordIndex].recordBackColor;
			
			Show_TimeDateDataOneRowOneLevelMenuContent(g_timeDate.recordIndex);		
			
			g_timeDate.oneLevelMenu[g_timeDate.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_timeDate.oneLevelMenu[g_timeDate.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_TimeDateDataOneRowOneLevelMenuContent(g_timeDate.nowIndex);	
		}
		else
		{
			g_timeDate.oneLevelMenu[g_timeDate.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_timeDate.oneLevelMenu[g_timeDate.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_TimeDateDataOneRowOneLevelMenuContent(g_timeDate.nowIndex);
		}
		
		g_timeDate.recordIndex = g_timeDate.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TimeDateShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_timeDate.refreshShortcut == ENABLE)
	{
		g_timeDate.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : TimeDateIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateIndexUpdate( void )
{
	g_timeDate.nowIndex++;
	g_timeDate.nowIndex %= g_timeDate.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateUpdateStatus( void )
{
	TimeDateIndexUpdate();
	
	g_timeDate.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateKeyProcess( void )
{
	uint8_t index = g_timeDate.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						return;
					
					case STATUS_EDIT_COMP:							
						switch ( g_timeDate.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_timeDate.putinNum,*GetPutinIntDataAddr(),g_timeDate.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_timeDate.putinNum,g_timeDate.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_timeDate.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_timeDate.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
//				if (TimeDateCheckDataCycle() == FAILED)
//				{
//					g_timeDate.leavePage.flagLeavePage = SET;
//					g_timeDate.leavePage.flagSaveData = RESET;
//					break;
//				}
				
				TimeDateUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_timeDate.leavePage.flagLeavePage = SET;
						g_timeDate.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						TimeDateUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
				
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_timeDate.leavePage.flagLeavePage = SET;
				g_timeDate.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_timeDate.leavePage.flagLeavePage = SET;
				g_timeDate.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TimeDateCheckDataCycle
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus TimeDateCheckDataCycle( void )
{
	uint8_t index = 0;
	uint16_t tempu = 0;
	uint16_t year = 0,mouth = 0,day = 0;

	index = GetTimeDateIndex(OBJECT_YEAR);
	if (index != 0xff)
	{
		year = ustrtoul(g_timeDate.parameterData[index],0,10) + 2000;	
	}
	
	index = GetTimeDateIndex(OBJECT_MONTH);
	if (index != 0xff)
	{
		mouth = ustrtoul(g_timeDate.parameterData[index],0,10);	
		if ((mouth<1) || (mouth>12))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTimeDateErrorCue[0]);
						
			return FAILED;
		}
	}
	
	index = GetTimeDateIndex(INDEX_DATE);
	if (index != 0xff)
	{
		day = ustrtoul(g_timeDate.parameterData[index],0,10);	
		
		tempu = GetAMonthDays(year,mouth);
		if ((day<1) || (day>tempu))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTimeDateErrorCue[1]);
						
			return FAILED;
		}
	}
	
	index = GetTimeDateIndex(INDEX_HOUR);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		if (tempu > 23)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTimeDateErrorCue[2]);
						
			return FAILED;
		}
	}
	
	index = GetTimeDateIndex(INDEX_MINITER);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		if (tempu > 59)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTimeDateErrorCue[3]);
						
			return FAILED;
		}
	}
	
	index = GetTimeDateIndex(INDEX_SECOND);
	if (index != 0xff)
	{
		tempu = ustrtoul(g_timeDate.parameterData[index],0,10);	
		if (tempu > 59)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTimeDateErrorCue[4]);
						
			return FAILED;
		}
	}
	
	return PASSED;
}	

/*------------------------------------------------------------
 * Function Name  : TimeDateLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TimeDateLeavePageCheckCycle( void )
{
	if (g_timeDate.leavePage.flagLeavePage == SET)
	{
		if (g_timeDate.leavePage.flagSaveData == SET)
		{
			if (TimeDateCheckDataCycle() == FAILED)
			{
				SetPage(DATE_TIME_PAGE);
				PopWindowsProcessCycle();
				
				return;
			}
				
			TimeDateWriteParameter();
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
