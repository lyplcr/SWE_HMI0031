/**
  ******************************************************************************
  * @file    ExtendParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2016-4-7 16:06:34
  * @IDE	 V5.18.0.0
  * @Chip    STM32F407VET6
  * @brief   扩展参数设置
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "ExtendParameter.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				5
#define MAX_EXTEND_PARAMETER_PUTIN_BIT	8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_YIELD_JUDGE_MODE = 0,
	OBJECT_YIELD_DISTURB_THRESHOLD,
	OBJECT_COMPUTE_ELASTIC_MODULUS,
	OBJECT_ELASTIC_MODULUS_START_STRENGTH,
	OBJECT_ELASTIC_MODULUS_END_STRENGTH,
}OBJECT_EXTEND_PARAMETER_TypeDef;

typedef enum
{
	INDEX_YIELD_JUDGE_MODE = 0,
	INDEX_YIELD_DISTURB_THRESHOLD,
	INDEX_COMPUTE_ELASTIC_MODULUS,
	INDEX_ELASTIC_MODULUS_START_STRENGTH,
	INDEX_ELASTIC_MODULUS_END_STRENGTH,
}INDEX_EXTEND_PARAMETER_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_EXTEND_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pParameterUnitArray[MAX_PARAMETER_CNT];

	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	FunctionalState enablePutin;					//使能输入
	FunctionalState enableArrow;					//使能箭头
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}EXTEND_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const ExtendParameterNamePtr[] =
{
	"屈服判断方式：",		//0
	"屈服干扰阈值：",		//1
	"计算弹性模量：",		//2
	"弹性模量起始强度：",	//3
	"弹性模量终止强度：",	//4
};	

const char * const YieldJudgeModeNamePtr[] =
{
	"明显屈服",		//0
	"Rp0.2",		//1
	"无屈服",		//2
};	

const char * const ComputeLasticModulusNamePtr[] =
{
	"否",		//0
	"是",		//1
};	

const char * const ExtendParamErrInfo[] = 
{
	"弹性模量起始强度输入范围：",		//0
	"(1 ~ )",
	"弹性模量终止强度必须大于",
	"起始强度。",					//2
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static EXTEND_PARAMETER_TypeDef g_extendParameter;

/* Private function prototypes -----------------------------------------------*/
static void ExtendParameterInit( void );
static void ExtendParameterConfig( void );
static void GUI_ExtendParameter( void );
static void ExtendParameterReadParameter( void );
static void ExtendParameterWriteParameter( void );
static void Traverse_ExtendParameter( void );
static void ExtendParameterPutinProcess( void );
static void ExtendParameterMoveIndexProcess( void );
static void ExtendParameterStatusProcess( void );
static void ExtendParameterMoveCursorProcess( void );
static void ExtendParameterShortcutCycleTask( void );
static void ExtendParameterIndexUpdate( void );
static void ExtendParameterUpdateStatus( void );
static void ExtendParameterKeyProcess( void );
static void ExtendParameterLeavePageCheckCycle( void );
static TestStatus ExtendParameterCheckData( uint8_t index );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadExtendParameterPage
 * Description    : 扩展参数页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadExtendParameterPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	ExtendParameterInit();
	
	/* 参数配置 */
	ExtendParameterConfig();
	
	/* 获取参数 */
	ExtendParameterReadParameter();
	
	/* 画GUI框架 */
	GUI_ExtendParameter();
	
	/* 遍历 */
	Traverse_ExtendParameter();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_extendParameter.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		ExtendParameterPutinProcess();
		
		/* 移动索引值 */
		ExtendParameterMoveIndexProcess();
		
		/* 状态处理 */
		ExtendParameterStatusProcess();
		
		/* 移动光标 */
		ExtendParameterMoveCursorProcess();
		
		/* 快捷菜单 */
		ExtendParameterShortcutCycleTask();
		
		/* 按键处理 */
		ExtendParameterKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ExtendParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterInit( void )
{	
	g_extendParameter.isIndexMove = NO;		
	g_extendParameter.recordIndex = 0xff;
	g_extendParameter.refreshShortcut = ENABLE;
	g_extendParameter.leavePage.flagLeavePage = RESET;
	g_extendParameter.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : GetExtendParameterIndex
 * Description    : 获取参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetExtendParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		if (g_extendParameter.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterConfig( void )
{
	/* 标题 */
	g_extendParameter.pTitle = "扩展参数设置";
	
	/* 个数 */
	g_extendParameter.curParameterNum = MAX_PARAMETER_CNT;	

	/* 索引值 */
	g_extendParameter.indexArray[INDEX_YIELD_JUDGE_MODE] 				= OBJECT_YIELD_JUDGE_MODE;
	g_extendParameter.indexArray[INDEX_YIELD_DISTURB_THRESHOLD] 		= OBJECT_YIELD_DISTURB_THRESHOLD;
	g_extendParameter.indexArray[INDEX_COMPUTE_ELASTIC_MODULUS] 		= OBJECT_COMPUTE_ELASTIC_MODULUS;
	g_extendParameter.indexArray[INDEX_ELASTIC_MODULUS_START_STRENGTH] 	= OBJECT_ELASTIC_MODULUS_START_STRENGTH;
	g_extendParameter.indexArray[INDEX_ELASTIC_MODULUS_END_STRENGTH] 	= OBJECT_ELASTIC_MODULUS_END_STRENGTH;
	
	/* 参数名称 */
	g_extendParameter.pParameterNameArray[INDEX_YIELD_JUDGE_MODE] 					= ExtendParameterNamePtr[0];
	g_extendParameter.pParameterNameArray[INDEX_YIELD_DISTURB_THRESHOLD] 			= ExtendParameterNamePtr[1];
	g_extendParameter.pParameterNameArray[INDEX_COMPUTE_ELASTIC_MODULUS] 			= ExtendParameterNamePtr[2];
	g_extendParameter.pParameterNameArray[INDEX_ELASTIC_MODULUS_START_STRENGTH] 	= ExtendParameterNamePtr[3];
	g_extendParameter.pParameterNameArray[INDEX_ELASTIC_MODULUS_END_STRENGTH] 		= ExtendParameterNamePtr[4];

	/* 单位 */
	g_extendParameter.pParameterUnitArray[INDEX_YIELD_JUDGE_MODE] 				= "NULL";
	g_extendParameter.pParameterUnitArray[INDEX_YIELD_DISTURB_THRESHOLD] 		= "NULL";
	g_extendParameter.pParameterUnitArray[INDEX_COMPUTE_ELASTIC_MODULUS] 		= "NULL";
	g_extendParameter.pParameterUnitArray[INDEX_ELASTIC_MODULUS_START_STRENGTH] = pUnitType[15];
	g_extendParameter.pParameterUnitArray[INDEX_ELASTIC_MODULUS_END_STRENGTH] 	= pUnitType[15];
	
	/* 数据保存类型 */
	g_extendParameter.oneLevelMenu[INDEX_YIELD_JUDGE_MODE].saveType 				= TYPE_INT;
	g_extendParameter.oneLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].saveType 			= TYPE_INT;
	g_extendParameter.oneLevelMenu[INDEX_COMPUTE_ELASTIC_MODULUS].saveType 			= TYPE_INT;
	g_extendParameter.oneLevelMenu[INDEX_ELASTIC_MODULUS_START_STRENGTH].saveType 	= TYPE_INT;
	g_extendParameter.oneLevelMenu[INDEX_ELASTIC_MODULUS_END_STRENGTH].saveType 	= TYPE_INT;

	/* 小数点位数 */
	g_extendParameter.oneLevelMenu[INDEX_YIELD_JUDGE_MODE].pointBit 				= 0;
	g_extendParameter.oneLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].pointBit 			= 0;
	g_extendParameter.oneLevelMenu[INDEX_COMPUTE_ELASTIC_MODULUS].pointBit 			= 0;
	g_extendParameter.oneLevelMenu[INDEX_ELASTIC_MODULUS_START_STRENGTH].pointBit 	= 0;
	g_extendParameter.oneLevelMenu[INDEX_ELASTIC_MODULUS_END_STRENGTH].pointBit 	= 0;
	
	/* 二级菜单类型 */
	g_extendParameter.twoLevelMenu[INDEX_YIELD_JUDGE_MODE].parameterType 				= NONE_USE_USER_DEFINE;
	g_extendParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].parameterType 		= IMMEDIATELY_PUTIN_NONE;
	g_extendParameter.twoLevelMenu[INDEX_COMPUTE_ELASTIC_MODULUS].parameterType 		= NONE_USE_USER_DEFINE;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_START_STRENGTH].parameterType 	= IMMEDIATELY_PUTIN_NONE;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_END_STRENGTH].parameterType 	= IMMEDIATELY_PUTIN_NONE;
	
	/* 二级菜单参数名 */
	g_extendParameter.twoLevelMenu[INDEX_YIELD_JUDGE_MODE].pParameterNameArray 					= YieldJudgeModeNamePtr;
	g_extendParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].pParameterNameArray 			= NULL;
	g_extendParameter.twoLevelMenu[INDEX_COMPUTE_ELASTIC_MODULUS].pParameterNameArray 			= ComputeLasticModulusNamePtr;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_START_STRENGTH].pParameterNameArray 	= NULL;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_END_STRENGTH].pParameterNameArray 		= NULL;
	
	/* 二级菜单参数个数 */
	g_extendParameter.twoLevelMenu[INDEX_YIELD_JUDGE_MODE].parameterCnt 				= 3;
	g_extendParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].parameterCnt 			= 0;
	g_extendParameter.twoLevelMenu[INDEX_COMPUTE_ELASTIC_MODULUS].parameterCnt 			= 2;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_START_STRENGTH].parameterCnt 	= 0;
	g_extendParameter.twoLevelMenu[INDEX_ELASTIC_MODULUS_END_STRENGTH].parameterCnt 	= 0;
}

/*------------------------------------------------------------
 * Function Name  : ConfigExtendParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigExtendParameterRectangleFrameCoordinate( void )
{
	uint16_t startX = 260;
	uint16_t startY = 74;
	uint8_t i;
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		g_extendParameter.oneLevelMenu[i].x = startX;
		g_extendParameter.oneLevelMenu[i].y = startY;
		g_extendParameter.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_extendParameter.oneLevelMenu[i].backColor = COLOR_BACK;
		g_extendParameter.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_extendParameter.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_extendParameter.oneLevelMenu[i].lenth = 162;
		g_extendParameter.oneLevelMenu[i].width = 30;
		g_extendParameter.oneLevelMenu[i].fontSize = 24;
		g_extendParameter.oneLevelMenu[i].rowDistance = 10;
		g_extendParameter.oneLevelMenu[i].columnDistance = 0;
		g_extendParameter.oneLevelMenu[i].lineWidth = 2;
		
		g_extendParameter.twoLevelMenu[i].x = g_extendParameter.oneLevelMenu[i].x + g_extendParameter.oneLevelMenu[i].lenth + 36;
		g_extendParameter.twoLevelMenu[i].y = g_extendParameter.oneLevelMenu[i].y;
		g_extendParameter.twoLevelMenu[i].pointColor = g_extendParameter.oneLevelMenu[i].pointColor;
		g_extendParameter.twoLevelMenu[i].backColor = g_extendParameter.oneLevelMenu[i].backColor;
		g_extendParameter.twoLevelMenu[i].lenth = 102;
		g_extendParameter.twoLevelMenu[i].fontSize = 24;
		g_extendParameter.twoLevelMenu[i].rowDistance = 2;
		g_extendParameter.twoLevelMenu[i].columnDistance = 0;
		g_extendParameter.twoLevelMenu[i].lineWidth = 2;				
		g_extendParameter.twoLevelMenu[i].width = g_extendParameter.twoLevelMenu[i].parameterCnt * \
												(g_extendParameter.twoLevelMenu[i].fontSize + g_extendParameter.twoLevelMenu[i].rowDistance) + \
												g_extendParameter.twoLevelMenu[i].rowDistance + 2 * g_extendParameter.twoLevelMenu[i].lineWidth;
		g_extendParameter.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_extendParameter.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_extendParameter.oneLevelMenu[i].width + g_extendParameter.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ExtendParameterDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ExtendParameterDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_extendParameter.oneLevelMenu[index].x;
	rectangle.y = g_extendParameter.oneLevelMenu[index].y;
	rectangle.lenth = g_extendParameter.oneLevelMenu[index].lenth;
	rectangle.width = g_extendParameter.oneLevelMenu[index].width;
	rectangle.lineWidth = g_extendParameter.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_extendParameter.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ExtendParameterRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ExtendParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		GUI_ExtendParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ExtendParameterDrawOneRowOneLevelMenu
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ExtendParameterDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_extendParameter.oneLevelMenu[index].x - 9 * 24;
	const uint16_t y = g_extendParameter.oneLevelMenu[index].y + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_extendParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_extendParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_extendParameter.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ExtendParameterOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ExtendParameterOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		GUI_ExtendParameterDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ExtendParameter
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ExtendParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_extendParameter.pTitle);
	
	ConfigExtendParameterRectangleFrameCoordinate();
	
	GUI_ExtendParameterRectangleFrame();
	
	GUI_ExtendParameterOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetExtendParameterIndex(OBJECT_YIELD_JUDGE_MODE);
	if (index != 0xff)
	{
		uint8_t yieldJudgeMode = pHmi->yieldJudgeMode;
		
		if (yieldJudgeMode > NO_YIELD)
		{
			yieldJudgeMode = OBVIOUS_YIELD;
		}
		
		g_extendParameter.twoLevelMenu[index].index = yieldJudgeMode;	
		strcpy(g_extendParameter.parameterData[index],YieldJudgeModeNamePtr[yieldJudgeMode]);
	}
	
	index = GetExtendParameterIndex(OBJECT_YIELD_DISTURB_THRESHOLD);
	if (index != 0xff)
	{
		uint32_t yieldDisturbThreshold = pHmi->yieldDisturbThreshold;
		
		numtochar(MAX_EXTEND_PARAMETER_PUTIN_BIT,yieldDisturbThreshold,g_extendParameter.parameterData[index]);
	}
	
	index = GetExtendParameterIndex(OBJECT_COMPUTE_ELASTIC_MODULUS);
	if (index != 0xff)
	{
		uint32_t computeElasticModulus = pHmi->computeElasticModulus;
		
		if (computeElasticModulus > COMPUTE_LASTIC_MODULUS)
		{
			computeElasticModulus = NO_LASTIC_MODULUS;
		}
		
		g_extendParameter.twoLevelMenu[index].index = computeElasticModulus;
		strcpy(g_extendParameter.parameterData[index],ComputeLasticModulusNamePtr[computeElasticModulus]);
	}
	
	index = GetExtendParameterIndex(OBJECT_ELASTIC_MODULUS_START_STRENGTH);
	if (index != 0xff)
	{
		uint32_t elasticModulusStartStrength = pHmi->elasticModulusStartStrength;
		
		numtochar(MAX_EXTEND_PARAMETER_PUTIN_BIT,elasticModulusStartStrength,g_extendParameter.parameterData[index]);
	}
	
	index = GetExtendParameterIndex(OBJECT_ELASTIC_MODULUS_END_STRENGTH);
	if (index != 0xff)
	{
		uint32_t elasticModulusEndStrength = pHmi->elasticModulusEndStrength;
		
		numtochar(MAX_EXTEND_PARAMETER_PUTIN_BIT,elasticModulusEndStrength,g_extendParameter.parameterData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterWriteParameter( void )
{
	uint8_t index = 0;
	
	index = GetExtendParameterIndex(OBJECT_YIELD_JUDGE_MODE);
	if (index != 0xff)
	{
		pHmi->yieldJudgeMode = g_extendParameter.twoLevelMenu[index].index;
	}
	
	index = GetExtendParameterIndex(OBJECT_YIELD_DISTURB_THRESHOLD);
	if (index != 0xff)
	{
		pHmi->yieldDisturbThreshold = ustrtoul(g_extendParameter.parameterData[index],0,10);
	}
	
	index = GetExtendParameterIndex(OBJECT_COMPUTE_ELASTIC_MODULUS);
	if (index != 0xff)
	{
		pHmi->computeElasticModulus = g_extendParameter.twoLevelMenu[index].index;	
	}
	
	index = GetExtendParameterIndex(OBJECT_ELASTIC_MODULUS_START_STRENGTH);
	if (index != 0xff)
	{
		pHmi->elasticModulusStartStrength = ustrtoul(g_extendParameter.parameterData[index],0,10);
	}
	
	index = GetExtendParameterIndex(OBJECT_ELASTIC_MODULUS_END_STRENGTH);
	if (index != 0xff)
	{
		pHmi->elasticModulusEndStrength = ustrtoul(g_extendParameter.parameterData[index],0,10);
	}
	
	pcm_save();
}

/*------------------------------------------------------------
 * Function Name  : Show_ExtendParameterOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ExtendParameterOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_extendParameter.oneLevelMenu[index].x + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_extendParameter.oneLevelMenu[index].y + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_extendParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_extendParameter.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_extendParameter.oneLevelMenu[index].lenth - 2 * g_extendParameter.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_extendParameter.oneLevelMenu[index].width - 2 * g_extendParameter.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_extendParameter.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_ExtendParameterOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ExtendParameterOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		Show_ExtendParameterOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_ExtendParameterOneRowOneLevelMenuUnit
 * Description    : 显示一行参数单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ExtendParameterOneRowOneLevelMenuUnit( uint8_t index )
{
	const uint16_t x = g_extendParameter.oneLevelMenu[index].x + g_extendParameter.oneLevelMenu[index].lenth - \
					   g_extendParameter.oneLevelMenu[index].lineWidth - 5 * 12;
	const uint16_t y = g_extendParameter.oneLevelMenu[index].y + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_extendParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_extendParameter.oneLevelMenu[index].backColor;
	
	if ( strcmp(g_extendParameter.pParameterUnitArray[index],"NULL") )
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_extendParameter.pParameterUnitArray[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_ExtendParameterOneLevelMenuUnit
 * Description    : 显示一级菜单单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ExtendParameterOneLevelMenuUnit( void )
{
	uint8_t i;
	
	for (i=0; i<g_extendParameter.curParameterNum; ++i)
	{
		Show_ExtendParameterOneRowOneLevelMenuUnit(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ExtendParameter
 * Description    : 遍历参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ExtendParameter( void )
{
	Show_ExtendParameterOneLevelMenuContent();
	
	Show_ExtendParameterOneLevelMenuUnit();
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_extendParameter.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_extendParameter.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_extendParameter.enablePutin;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_extendParameter.oneLevelMenu[index].x + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_extendParameter.oneLevelMenu[index].y + g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_EXTEND_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_EXTEND_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_extendParameter.oneLevelMenu[index].lenth - 2 * g_extendParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_extendParameter.oneLevelMenu[index].width - 2 * g_extendParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_extendParameter.putinNum;
	pPutin->SaveType = g_extendParameter.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_extendParameter.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_extendParameter.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_extendParameter.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_extendParameter.curParameterNum;
	indexObj.pNowIndex = &g_extendParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_extendParameter.nowIndex != g_extendParameter.recordIndex)
	{		
		g_extendParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterStatusProcess( void )
{
	uint8_t index = g_extendParameter.nowIndex;
	
	if (g_extendParameter.nowIndex >= g_extendParameter.curParameterNum)
	{
		g_extendParameter.nowIndex = 0;
	}
	
	switch ( g_extendParameter.twoLevelMenu[index].parameterType )
	{
		case IMMEDIATELY_PUTIN_NONE:
		case IMMEDIATELY_PUTIN_SHIFT:
			g_extendParameter.enablePutin = ENABLE;
			break;
		
		default:
			g_extendParameter.enablePutin = DISABLE;
			break;
	}
	
	switch ( g_extendParameter.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_extendParameter.enableArrow = ENABLE;
			break;
		
		default:
			g_extendParameter.enableArrow = DISABLE;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : Clear_ExtendParameterOneRowOneLevelMenuArrow
 * Description    : 清除一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_ExtendParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_extendParameter.oneLevelMenu[index].x + \
					   g_extendParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_extendParameter.oneLevelMenu[index].y + \
					   g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_extendParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Show_ExtendParameterOneRowOneLevelMenuArrow
 * Description    : 显示一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ExtendParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_extendParameter.oneLevelMenu[index].x + \
					   g_extendParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_extendParameter.oneLevelMenu[index].y + \
					   g_extendParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterMoveCursorProcess( void )
{
	if (g_extendParameter.isIndexMove == YES)
	{		
		if (g_extendParameter.recordIndex != 0xff)
		{
			g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].pointColor = \
				g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].recordPointColor;
			g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].backColor = \
				g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].recordBackColor;
			
			Show_ExtendParameterOneRowOneLevelMenuContent(g_extendParameter.recordIndex);
			Show_ExtendParameterOneRowOneLevelMenuUnit(g_extendParameter.recordIndex);
			Clear_ExtendParameterOneRowOneLevelMenuArrow(g_extendParameter.recordIndex);			
			
			g_extendParameter.oneLevelMenu[g_extendParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_extendParameter.oneLevelMenu[g_extendParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ExtendParameterOneRowOneLevelMenuContent(g_extendParameter.nowIndex);
			Show_ExtendParameterOneRowOneLevelMenuUnit(g_extendParameter.nowIndex);	
			
			if (g_extendParameter.enableArrow == ENABLE)
			{
				Show_ExtendParameterOneRowOneLevelMenuArrow(g_extendParameter.nowIndex);
			}
		}
		else
		{
			g_extendParameter.oneLevelMenu[g_extendParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_extendParameter.oneLevelMenu[g_extendParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ExtendParameterOneRowOneLevelMenuContent(g_extendParameter.nowIndex);
			Show_ExtendParameterOneRowOneLevelMenuUnit(g_extendParameter.nowIndex);
			
			if (g_extendParameter.enableArrow == ENABLE)
			{
				Show_ExtendParameterOneRowOneLevelMenuArrow(g_extendParameter.nowIndex);
			}
		}
		
		g_extendParameter.recordIndex = g_extendParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_extendParameter.refreshShortcut == ENABLE)
	{
		g_extendParameter.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[0] = pTwoLevelMenu[6];
		pShortCut->pContent[1] = pTwoLevelMenu[4];
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterIndexUpdate( void )
{
	uint8_t recordIndex = g_extendParameter.nowIndex;
	
	g_extendParameter.nowIndex++;
	g_extendParameter.nowIndex %= g_extendParameter.curParameterNum;
	
	if (g_extendParameter.nowIndex == recordIndex)
	{
		g_extendParameter.recordIndex = 0xff;
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterUpdateStatus( void )
{
	ExtendParameterIndexUpdate();
	
	g_extendParameter.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterKeyProcess( void )
{
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	uint8_t index = g_extendParameter.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_extendParameter.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_extendParameter.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* 处理快捷菜单 */
					GUI_ClearShortcutMenu();
					
					/* 还原一级菜单 */
					g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].pointColor = \
						g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].recordPointColor;
					g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].backColor = \
						g_extendParameter.oneLevelMenu[g_extendParameter.recordIndex].recordBackColor;
					
					Show_ExtendParameterOneRowOneLevelMenuContent(g_extendParameter.recordIndex);
					
					/* 配置二级菜单 */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_extendParameter.twoLevelMenu[index].x;
					pMenu->y = g_extendParameter.twoLevelMenu[index].y;
					pMenu->nowIndex = g_extendParameter.twoLevelMenu[index].index;
					pMenu->maxUpY = g_extendParameter.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_extendParameter.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_extendParameter.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_extendParameter.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_extendParameter.twoLevelMenu[index].lenth;
					pMenu->width = g_extendParameter.twoLevelMenu[index].width;
					pMenu->pointColor = g_extendParameter.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_extendParameter.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_extendParameter.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_extendParameter.twoLevelMenu[index].pointColor;
					pMenu->nums = g_extendParameter.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_extendParameter.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_extendParameter.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{
						g_extendParameter.twoLevelMenu[index].index = pMenu->nowIndex;
						
						strcpy(g_extendParameter.parameterData[index],pMenu->pParameterNameArray[pMenu->nowIndex]);
					}
					ExtendParameterUpdateStatus();
				}
				break;
			
			case KEY_ENTER:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						return;
					
					case STATUS_EDIT_COMP:							
						switch ( g_extendParameter.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_extendParameter.putinNum,*GetPutinIntDataAddr(),g_extendParameter.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_extendParameter.putinNum,g_extendParameter.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_extendParameter.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_extendParameter.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				if (FAILED == ExtendParameterCheckData(index) )
				{
					g_extendParameter.leavePage.flagLeavePage = SET;
					g_extendParameter.leavePage.flagSaveData = RESET;
					break;
				}
				ExtendParameterUpdateStatus();
				break;
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_extendParameter.leavePage.flagLeavePage = SET;
						g_extendParameter.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						ExtendParameterUpdateStatus();
						break;
					
					default:						
						break;
				}
				break;
			case KEY_F1:
				SetPage(CALIBRATION_TABLE);
				g_extendParameter.leavePage.flagLeavePage = SET;
				g_extendParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F2:
				SetPage(CONTROL_PARAMETER);
				g_extendParameter.leavePage.flagLeavePage = SET;
				g_extendParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_extendParameter.leavePage.flagLeavePage = SET;
				g_extendParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_extendParameter.leavePage.flagLeavePage = SET;
				g_extendParameter.leavePage.flagSaveData = RESET;
				break;
			
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ExtendParameterLeavePageCheckCycle( void )
{
	if (g_extendParameter.leavePage.flagLeavePage == SET)
	{
		if (g_extendParameter.leavePage.flagSaveData == SET)
		{
			uint8_t i;
			
			for (i=0; i<g_extendParameter.curParameterNum; ++i)
			{
				if (FAILED == ExtendParameterCheckData(i) )
				{
					SetPage(EXTEND_PARAMETER_PAGE);
					PopWindowsProcessCycle();
					
					return;
				}
			}
			
			ExtendParameterWriteParameter();
			
			SetLssuedParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ExtendParameterCheckData
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ExtendParameterCheckData( uint8_t index )
{
	uint8_t handle;
	
	if (index >= g_extendParameter.curParameterNum)
	{
		return PASSED;
	}
	
	handle = g_extendParameter.indexArray[index];
	
	switch (handle)
	{
		case OBJECT_YIELD_JUDGE_MODE:
			
			break;
		case OBJECT_YIELD_DISTURB_THRESHOLD:
			
			break;
		case OBJECT_COMPUTE_ELASTIC_MODULUS:
			
			break;
		case OBJECT_ELASTIC_MODULUS_START_STRENGTH:
			{
				uint32_t startStrength = ustrtoul(g_extendParameter.parameterData[index],0,10);
				
				if (startStrength == 0)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ExtendParamErrInfo[0]);
							
					return FAILED;
				}
			}
			break;
		case OBJECT_ELASTIC_MODULUS_END_STRENGTH:
			{
				uint32_t endStrength = ustrtoul(g_extendParameter.parameterData[index],0,10);
				uint8_t startStrengthIndex = GetExtendParameterIndex(OBJECT_ELASTIC_MODULUS_START_STRENGTH);
				uint32_t startStrength = ustrtoul(g_extendParameter.parameterData[startStrengthIndex],0,10);
				
				if (startStrength >= endStrength)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ExtendParamErrInfo[2]);
							
					return FAILED;
				}
			}
			break;
		default:
			break;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : GetYieldJudgeMode
 * Description    : 获取屈服判断方式
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
YIELD_JUDGE_MODE_TypeDef GetYieldJudgeMode( void )
{
	uint8_t modeIndex = pHmi->yieldJudgeMode;
	
	if (modeIndex > NO_YIELD)
	{
		modeIndex = OBVIOUS_YIELD;
	}
	
	return (YIELD_JUDGE_MODE_TypeDef)modeIndex;
}

/*------------------------------------------------------------
 * Function Name  : GetYieldDisturbThreshold
 * Description    : 获取屈服干扰阈值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t GetYieldDisturbThreshold( void )
{
	return pHmi->yieldDisturbThreshold;
}

/*------------------------------------------------------------
 * Function Name  : GetComputeLasticModulusMode
 * Description    : 获取计算弹性模量方式
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
COMPUTE_LASTIC_MODULUS_TypeDef GetComputeLasticModulusMode( void )
{
	uint8_t modeIndex = pHmi->computeElasticModulus;
	
	if (modeIndex > COMPUTE_LASTIC_MODULUS)
	{
		modeIndex = NO_LASTIC_MODULUS;
	}
	
	return (COMPUTE_LASTIC_MODULUS_TypeDef)modeIndex;
}

/*------------------------------------------------------------
 * Function Name  : GetElasticModulusStartStrength
 * Description    : 获取弹性模量起始强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetElasticModulusStartStrength( void )
{
	return pHmi->elasticModulusStartStrength;
}

/*------------------------------------------------------------
 * Function Name  : GetElasticModulusEndStrength
 * Description    : 获取弹性模量终止强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetElasticModulusEndStrength( void )
{
	return pHmi->elasticModulusEndStrength;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
