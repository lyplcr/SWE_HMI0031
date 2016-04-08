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
#define MAX_EXTEND_PARAMETER_PUTIN_BIT	7

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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static EXTEND_PARAMETER_TypeDef g_extendParameter;

/* Private function prototypes -----------------------------------------------*/
static void ExtendParameterInit( void );
static void ExtendParameterConfig( void );
static void GUI_ExtendParameter( void );
static void ExtendParameterReadParameter( void );
static void ExtendParameterWriteParameter( void );


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
//	Traverse_ControlParameter();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_extendParameter.leavePage.flagLeavePage == RESET)
	{
//		/* 输入处理 */
//		ControlParameterPutinProcess();
//		
//		/* 移动索引值 */
//		ControlParameterMoveIndexProcess();
//		
//		/* 状态处理 */
//		ControlParameterStatusProcess();
//		
//		/* 移动光标 */
//		ControlParameterMoveCursorProcess();
//		
//		/* 快捷菜单 */
//		ControlParameterShortcutCycleTask();
//		
//		/* 按键处理 */
//		ControlParameterKeyProcess();
//		
//		/* 弹窗处理 */
//		PopWindowsProcessCycle();
//		
//		/* 离开页 */
//		ControlParameterLeavePageCheckCycle();
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
		g_extendParameter.oneLevelMenu[i].lenth = 150;
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
