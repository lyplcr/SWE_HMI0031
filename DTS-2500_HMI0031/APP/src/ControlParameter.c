/**
  ******************************************************************************
  * @file    ControlParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-19 14:00:32
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   控制参数设置
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "ControlParameter.h"
#include "ChannelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				9
#define MAX_CONTROL_PARAMETER_PUTIN_BIT	8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_SYSTEM_MAX_VALUE = 0,
	OBJECT_LOAD_START_VALUE,
	OBJECT_CURVE_SHOW_START_VALUE,
	OBJECT_BREAK_START_FORCE,
	OBJECT_BREAK_DOWM_POINT,
	OBJECT_BREAK_TYPE,
	OBJECT_FORCE_DECAY_RATE,
	OBJECT_MAX_FORCE_DIFF,
	OBJECT_YIELD_DISTURB_THRESHOLD,
}OBJECT_CONTROL_PARAMETER_TypeDef;

typedef enum
{
	INDEX_SYSTEM_MAX_VALUE = 0,
	INDEX_LOAD_START_VALUE,
	INDEX_CURVE_SHOW_START_VALUE,
	INDEX_BREAK_START_FORCE,
	INDEX_BREAK_DOWM_POINT,
	INDEX_BREAK_TYPE,
	INDEX_FORCE_DECAY_RATE,
	INDEX_MAX_FORCE_DIFF,
	INDEX_YIELD_DISTURB_THRESHOLD,
}INDEX_CONTROL_PARAMETER_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_CONTROL_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pParameterUnitArray[MAX_PARAMETER_CNT];
	TEST_TYPE_TypeDef testType;
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	FH_UINT_TypeDef fhChannelUnit;					//负荷通道单位
	WY_UINT_TypeDef	wyChannelUnit;					//位移通道单位
	BX_UINT_TypeDef	bxChannelUnit;					//变形通道单位
	SMPL_NAME_TypeDef curChannel;					//当前通道
	FunctionalState enablePutin;					//使能输入
	FunctionalState enableArrow;					//使能箭头
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}CONTROL_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pCntrolParameterName[] =
{
	"系统最大力：",		//0
	"加载起始力：",		//1
	"曲线显示阈值：",	//2
	"起判力值：",		//3
	"下降点数：",		//4
	"判破方式：",		//5
	"A:力值衰减了：",	//6
	"B:与最大力差值：",	//7
	"位移满量程：",		//8
	"变形满量程：",		//9
	"加载起始位移：",	//10
	"加载起始变形：",	//11
	"屈服干扰阈值：",	//12
};

const char * const ControlParamErrInfoKN[] = 
{
	"系统最大力输入范围：",		//0
	"(10 ~ 10000 kN)",
	"加载起始力输入范围：",		//2
	"(1 ~ 100 kN)",
	"曲线显示阈值输入范围：",	//4
	"范围：(1 ~ 100 kN)",
	"破碎起判力值输入范围：",	//6
	"(1 ~ 1000 kN)",	
	"判破下降点数输入范围：",	//8
	"(1 ~ 100)",
	"与最大力差值输入范围：",	//10
	"(0.1 ~ 100 kN)",	
	"力值衰减了输入范围：",		//12
	"(1 ~ 100)",
	"屈服干扰阈值输入范围：",	//14
	"(0 ~ 60000)",		
};

const char * const ControlParamErrInfoN[] = 
{
	"系统最大力输入范围：",		//0
	"(10000 ~ 100000 N)",
	"加载起始力输入范围：",		//2
	"(1 ~ 10000 N)",
	"曲线显示阈值输入范围：",	//4
	"范围：(1 ~ 10000 N)",
	"破碎起判力值输入范围：",	//6
	"(1 ~ 10000 N)",	
	"判破下降点数输入范围：",	//8
	"(1 ~ 100)",
	"与最大力差值输入范围：",	//10
	"(1 ~ 10000 N)",	
	"力值衰减了输入范围：",		//12
	"(1 ~ 100)",
};

const char * const ControlParamErrInfo[] = 
{
	"位移满量程输入范围：",		//0
	"(1 ~ 10000)",
	"变形满量程输入范围：",		//2
	"(1 ~ 10000)",
	"加载起始位移不能为0！",		//4
	"加载起始变形不能为0！",		//5
};

const char * const ControlParameterCondition[] = 
{
	"条件A",
	"条件B",
};	

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CONTROL_PARAMETER_TypeDef g_controlParameter;

/* Private function prototypes -----------------------------------------------*/
static void ControlParameterInit( void );
static void ControlParameterConfig( void );
static void GUI_ControlParameter( void );
static void ControlParameterReadParameter( void );
static void Traverse_ControlParameter( void );
static void ControlParameterPutinProcess( void );
static void ControlParameterMoveIndexProcess( void );
static void ControlParameterMoveCursorProcess( void );
static void ControlParameterShortcutCycleTask( void );
static void ControlParameterKeyProcess( void );
static void ControlParameterWriteParameter( void );
static void ControlParameterLeavePageCheckCycle( void );
static TestStatus ControlParameterCheckData( uint8_t index );
static void ControlParameterStatusProcess( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadControlParameterPage
 * Description    : 控制参数页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadControlParameterPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	ControlParameterInit();
	
	/* 参数配置 */
	ControlParameterConfig();
	
	/* 获取参数 */
	ControlParameterReadParameter();
	
	/* 画GUI框架 */
	GUI_ControlParameter();
	
	/* 遍历 */
	Traverse_ControlParameter();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_controlParameter.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		ControlParameterPutinProcess();
		
		/* 移动索引值 */
		ControlParameterMoveIndexProcess();
		
		/* 状态处理 */
		ControlParameterStatusProcess();
		
		/* 移动光标 */
		ControlParameterMoveCursorProcess();
		
		/* 快捷菜单 */
		ControlParameterShortcutCycleTask();
		
		/* 按键处理 */
		ControlParameterKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ControlParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterInit
 * Description    : 控制参数初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterInit( void )
{	
	g_controlParameter.isIndexMove = NO;		
	g_controlParameter.recordIndex = 0xff;
	g_controlParameter.refreshShortcut = ENABLE;
	g_controlParameter.leavePage.flagLeavePage = RESET;
	g_controlParameter.leavePage.flagSaveData = RESET;
	
	g_controlParameter.fhChannelUnit = GetFH_SmplUnit();
	g_controlParameter.wyChannelUnit = GetWY_SmplUnit();
	g_controlParameter.bxChannelUnit = GetBX_SmplUnit();
	g_controlParameter.curChannel = GetChannelSelectChannel();
}

/*------------------------------------------------------------
 * Function Name  : GetControlParameterIndex
 * Description    : 获取控制参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetControlParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		if (g_controlParameter.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterConfig
 * Description    : 控制参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterConfig( void )
{
	/* 标题 */
	g_controlParameter.pTitle = "控制参数设置";
	
	switch ( g_controlParameter.curChannel )
	{
		case SMPL_FH_NUM:
			/* 个数 */
			g_controlParameter.curParameterNum = MAX_PARAMETER_CNT - 1;	
		
			/* 索引值 */
			g_controlParameter.indexArray[INDEX_SYSTEM_MAX_VALUE] 		= OBJECT_SYSTEM_MAX_VALUE;
			g_controlParameter.indexArray[INDEX_LOAD_START_VALUE] 		= OBJECT_LOAD_START_VALUE;
			g_controlParameter.indexArray[INDEX_CURVE_SHOW_START_VALUE] = OBJECT_CURVE_SHOW_START_VALUE;
			g_controlParameter.indexArray[INDEX_BREAK_START_FORCE] 		= OBJECT_BREAK_START_FORCE;
			g_controlParameter.indexArray[INDEX_BREAK_DOWM_POINT] 		= OBJECT_BREAK_DOWM_POINT;
			g_controlParameter.indexArray[INDEX_BREAK_TYPE] 			= OBJECT_BREAK_TYPE;
			g_controlParameter.indexArray[INDEX_FORCE_DECAY_RATE] 		= OBJECT_FORCE_DECAY_RATE;
			g_controlParameter.indexArray[INDEX_MAX_FORCE_DIFF] 		= OBJECT_MAX_FORCE_DIFF;
			g_controlParameter.indexArray[INDEX_YIELD_DISTURB_THRESHOLD] = OBJECT_YIELD_DISTURB_THRESHOLD;
			
			/* 参数名称 */
			g_controlParameter.pParameterNameArray[INDEX_SYSTEM_MAX_VALUE] 			= pCntrolParameterName[0];
			g_controlParameter.pParameterNameArray[INDEX_LOAD_START_VALUE] 			= pCntrolParameterName[1];
			g_controlParameter.pParameterNameArray[INDEX_CURVE_SHOW_START_VALUE] 	= pCntrolParameterName[2];
			g_controlParameter.pParameterNameArray[INDEX_BREAK_START_FORCE] 		= pCntrolParameterName[3];
			g_controlParameter.pParameterNameArray[INDEX_BREAK_DOWM_POINT] 			= pCntrolParameterName[4];
			g_controlParameter.pParameterNameArray[INDEX_BREAK_TYPE] 				= pCntrolParameterName[5];
			g_controlParameter.pParameterNameArray[INDEX_FORCE_DECAY_RATE] 			= pCntrolParameterName[6];
			g_controlParameter.pParameterNameArray[INDEX_MAX_FORCE_DIFF] 			= pCntrolParameterName[7];
			g_controlParameter.pParameterNameArray[INDEX_YIELD_DISTURB_THRESHOLD] 	= pCntrolParameterName[12];

			/* 单位 */
			if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
			{
				g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 			= pUnitType[1];
				g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE] 			= pUnitType[1];
				g_controlParameter.pParameterUnitArray[INDEX_CURVE_SHOW_START_VALUE] 	= pUnitType[1];
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_START_FORCE] 		= pUnitType[1];
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_DOWM_POINT] 			= "NULL";
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_TYPE] 				= "NULL";
				g_controlParameter.pParameterUnitArray[INDEX_FORCE_DECAY_RATE] 			= pUnitType[10];
				g_controlParameter.pParameterUnitArray[INDEX_MAX_FORCE_DIFF] 			= pUnitType[1];
				g_controlParameter.pParameterUnitArray[INDEX_YIELD_DISTURB_THRESHOLD] 	= "NULL";
			}
			else
			{
				g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 			= pUnitType[0];
				g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE] 			= pUnitType[0];
				g_controlParameter.pParameterUnitArray[INDEX_CURVE_SHOW_START_VALUE] 	= pUnitType[0];
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_START_FORCE] 		= pUnitType[0];
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_DOWM_POINT] 			= "NULL";
				g_controlParameter.pParameterUnitArray[INDEX_BREAK_TYPE] 				= "NULL";
				g_controlParameter.pParameterUnitArray[INDEX_FORCE_DECAY_RATE] 			= pUnitType[10];
				g_controlParameter.pParameterUnitArray[INDEX_MAX_FORCE_DIFF] 			= pUnitType[0];
				g_controlParameter.pParameterUnitArray[INDEX_YIELD_DISTURB_THRESHOLD] 	= "NULL";
			}
			
			/* 数据保存类型 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_CURVE_SHOW_START_VALUE].saveType 	= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_START_FORCE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_DOWM_POINT].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_TYPE].saveType 				= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_FORCE_DECAY_RATE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_MAX_FORCE_DIFF].saveType 			= TYPE_FLOAT;	
			g_controlParameter.oneLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].saveType = TYPE_INT;

			/* 小数点位数 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_CURVE_SHOW_START_VALUE].pointBit 	= 0;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_START_FORCE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_DOWM_POINT].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_BREAK_TYPE].pointBit 				= 0;
			g_controlParameter.oneLevelMenu[INDEX_FORCE_DECAY_RATE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_MAX_FORCE_DIFF].pointBit 			= 1;
			g_controlParameter.oneLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].pointBit	= 0;
			
			/* 二级菜单类型 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_CURVE_SHOW_START_VALUE].parameterType = IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_START_FORCE].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_DOWM_POINT].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_TYPE].parameterType 			= NONE_USE_USER_DEFINE;
			g_controlParameter.twoLevelMenu[INDEX_FORCE_DECAY_RATE].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_MAX_FORCE_DIFF].parameterType 		= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].parameterType= IMMEDIATELY_PUTIN_NONE;
			
			/* 二级菜单参数名 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_CURVE_SHOW_START_VALUE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_START_FORCE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_DOWM_POINT].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_TYPE].pParameterNameArray = ControlParameterCondition;
			g_controlParameter.twoLevelMenu[INDEX_FORCE_DECAY_RATE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_MAX_FORCE_DIFF].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].pParameterNameArray = NULL;
			
			/* 二级菜单参数个数 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_CURVE_SHOW_START_VALUE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_START_FORCE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_DOWM_POINT].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_BREAK_TYPE].parameterCnt = 2;
			g_controlParameter.twoLevelMenu[INDEX_FORCE_DECAY_RATE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_MAX_FORCE_DIFF].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_YIELD_DISTURB_THRESHOLD].parameterCnt = 0;
			break;
		case SMPL_WY_NUM:
			/* 个数 */
			g_controlParameter.curParameterNum = 1;	
		
			/* 索引值 */
			g_controlParameter.indexArray[INDEX_SYSTEM_MAX_VALUE] 					= OBJECT_SYSTEM_MAX_VALUE;
			g_controlParameter.indexArray[INDEX_LOAD_START_VALUE] 					= OBJECT_LOAD_START_VALUE;
			
			/* 参数名称 */
			g_controlParameter.pParameterNameArray[INDEX_SYSTEM_MAX_VALUE] 			= pCntrolParameterName[8];
			g_controlParameter.pParameterNameArray[INDEX_LOAD_START_VALUE] 			= pCntrolParameterName[10];
		
			/* 单位 */
			switch ( g_controlParameter.wyChannelUnit )
			{
				case WY_UNIT_MM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[4];	
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[4];
					break;
				case WY_UNIT_CM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[12];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[12];
					break;
				case WY_UNIT_DM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[13];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[13];
					break;
				case WY_UNIT_M:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[14];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[14];
					break; 
				default:
					break;
			}
			
			/* 数据保存类型 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].saveType 		= TYPE_FLOAT;

			/* 小数点位数 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].pointBit 		= 1;
			
			/* 二级菜单类型 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterType 	= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterType 	= IMMEDIATELY_PUTIN_NONE;
			
			/* 二级菜单参数名 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].pParameterNameArray = NULL;
			
			/* 二级菜单参数个数 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterCnt = 0;
			break;
		case SMPL_BX_NUM:
			/* 个数 */
			g_controlParameter.curParameterNum = 1;	
		
			/* 索引值 */
			g_controlParameter.indexArray[INDEX_SYSTEM_MAX_VALUE] 					= OBJECT_SYSTEM_MAX_VALUE;
			g_controlParameter.indexArray[INDEX_LOAD_START_VALUE]					= OBJECT_LOAD_START_VALUE;
		
			/* 参数名称 */
			g_controlParameter.pParameterNameArray[INDEX_SYSTEM_MAX_VALUE] 			= pCntrolParameterName[9];
			g_controlParameter.pParameterNameArray[INDEX_LOAD_START_VALUE] 			= pCntrolParameterName[11];

			/* 单位 */
			switch ( g_controlParameter.bxChannelUnit )
			{
				case BX_UNIT_MM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[4];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[4];
					break;
				case BX_UNIT_CM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[12];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[12];
					break;
				case BX_UNIT_DM:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[13];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[13];
					break;
				case BX_UNIT_M:
					g_controlParameter.pParameterUnitArray[INDEX_SYSTEM_MAX_VALUE] 	= pUnitType[14];
					g_controlParameter.pParameterUnitArray[INDEX_LOAD_START_VALUE]  = pUnitType[14];
					break; 
				default:
					break;
			}
			
			/* 数据保存类型 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].saveType 		= TYPE_INT;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].saveType 		= TYPE_FLOAT;

			/* 小数点位数 */
			g_controlParameter.oneLevelMenu[INDEX_SYSTEM_MAX_VALUE].pointBit 		= 0;
			g_controlParameter.oneLevelMenu[INDEX_LOAD_START_VALUE].pointBit 		= 1;
			
			/* 二级菜单类型 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterType 	= IMMEDIATELY_PUTIN_NONE;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterType 	= IMMEDIATELY_PUTIN_NONE;
			
			/* 二级菜单参数名 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].pParameterNameArray = NULL;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].pParameterNameArray = NULL;
			
			/* 二级菜单参数个数 */
			g_controlParameter.twoLevelMenu[INDEX_SYSTEM_MAX_VALUE].parameterCnt = 0;
			g_controlParameter.twoLevelMenu[INDEX_LOAD_START_VALUE].parameterCnt = 0;
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigControlParameterParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigControlParameterParameterRectangleFrameCoordinate( void )
{
	uint16_t startX = 260;
	uint16_t startY = 74;
	uint8_t i;
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		g_controlParameter.oneLevelMenu[i].x = startX;
		g_controlParameter.oneLevelMenu[i].y = startY;
		g_controlParameter.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_controlParameter.oneLevelMenu[i].backColor = COLOR_BACK;
		g_controlParameter.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_controlParameter.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_controlParameter.oneLevelMenu[i].lenth = 162;
		g_controlParameter.oneLevelMenu[i].width = 30;
		g_controlParameter.oneLevelMenu[i].fontSize = 24;
		g_controlParameter.oneLevelMenu[i].rowDistance = 10;
		g_controlParameter.oneLevelMenu[i].columnDistance = 0;
		g_controlParameter.oneLevelMenu[i].lineWidth = 2;
		
		g_controlParameter.twoLevelMenu[i].x = g_controlParameter.oneLevelMenu[i].x + g_controlParameter.oneLevelMenu[i].lenth + 36;
		g_controlParameter.twoLevelMenu[i].y = g_controlParameter.oneLevelMenu[i].y;
		g_controlParameter.twoLevelMenu[i].pointColor = g_controlParameter.oneLevelMenu[i].pointColor;
		g_controlParameter.twoLevelMenu[i].backColor = g_controlParameter.oneLevelMenu[i].backColor;
		g_controlParameter.twoLevelMenu[i].lenth = 102;
		g_controlParameter.twoLevelMenu[i].fontSize = 24;
		g_controlParameter.twoLevelMenu[i].rowDistance = 2;
		g_controlParameter.twoLevelMenu[i].columnDistance = 0;
		g_controlParameter.twoLevelMenu[i].lineWidth = 2;				
		g_controlParameter.twoLevelMenu[i].width = g_controlParameter.twoLevelMenu[i].parameterCnt * \
												(g_controlParameter.twoLevelMenu[i].fontSize + g_controlParameter.twoLevelMenu[i].rowDistance) + \
												g_controlParameter.twoLevelMenu[i].rowDistance + 2 * g_controlParameter.twoLevelMenu[i].lineWidth;
		g_controlParameter.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_controlParameter.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_controlParameter.oneLevelMenu[i].width + g_controlParameter.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ControlParameterDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ControlParameterDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_controlParameter.oneLevelMenu[index].x;
	rectangle.y = g_controlParameter.oneLevelMenu[index].y;
	rectangle.lenth = g_controlParameter.oneLevelMenu[index].lenth;
	rectangle.width = g_controlParameter.oneLevelMenu[index].width;
	rectangle.lineWidth = g_controlParameter.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_controlParameter.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ControlParameterRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ControlParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		GUI_ControlParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ControlParameterDrawOneRowOneLevelMenu
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ControlParameterDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_controlParameter.oneLevelMenu[index].x - 8 * 24;
	const uint16_t y = g_controlParameter.oneLevelMenu[index].y + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_controlParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_controlParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_controlParameter.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ControlParameterOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ControlParameterOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		GUI_ControlParameterDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ControlParameter
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ControlParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_controlParameter.pTitle);
	
	ConfigControlParameterParameterRectangleFrameCoordinate();
	
	GUI_ControlParameterRectangleFrame();
	
	GUI_ControlParameterOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterReadParameter( void )
{
	float tempf = 0;
	uint8_t index = 0;
	uint8_t tempu8 = 0;
	
	switch ( g_controlParameter.curChannel )
	{
		case SMPL_FH_NUM:
			index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_full_p_get(g_controlParameter.curChannel);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf /= 1000;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}
			
			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_entry_get(g_controlParameter.curChannel);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf /= 1000;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}
			
			index = GetControlParameterIndex(OBJECT_CURVE_SHOW_START_VALUE);
			if (index != 0xff)
			{
				tempf = pHmi->start_force[g_controlParameter.curChannel];
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf /= 1000;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_START_FORCE);
			if (index != 0xff)
			{
				tempf = pHmi->break_judge_value[g_controlParameter.curChannel];
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf /= 1000;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_DOWM_POINT);
			if (index != 0xff)
			{
				tempu8 = pHmi->break_point[g_controlParameter.curChannel];

				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,tempu8,g_controlParameter.parameterData[index]);
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_TYPE);
			if (index != 0xff)
			{
				tempu8 = pHmi->break_condition[g_controlParameter.curChannel];
				
				if (tempu8 > WITH_MAX_FORCE_DIFFERENCE)
				{
					tempu8 = ATTENUATION_RATE;
				}
				
				g_controlParameter.twoLevelMenu[index].index = tempu8;			
				strcpy(g_controlParameter.parameterData[index],ControlParameterCondition[tempu8]);
			}
			
			index = GetControlParameterIndex(OBJECT_FORCE_DECAY_RATE);
			if (index != 0xff)
			{
				tempu8 = pHmi->attenuationRate[g_controlParameter.curChannel];
				
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,tempu8,g_controlParameter.parameterData[index]);
			}	
			
			index = GetControlParameterIndex(OBJECT_MAX_FORCE_DIFF);
			if (index != 0xff)
			{
				tempf = pHmi->break_max_value[g_controlParameter.curChannel];
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf /= 1000;
				}
				floattochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,g_controlParameter.oneLevelMenu[index].pointBit,tempf,g_controlParameter.parameterData[index]);
			}

			index = GetControlParameterIndex(OBJECT_YIELD_DISTURB_THRESHOLD);
			if (index != 0xff)
			{
				uint32_t temp = pHmi->yieldDisturbThreshold;
				
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,temp,g_controlParameter.parameterData[index]);
			}	
			break;
		case SMPL_WY_NUM:
			index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_full_p_get(g_controlParameter.curChannel);
				switch ( g_controlParameter.wyChannelUnit )
				{
					case WY_UNIT_MM:						
						break;
					case WY_UNIT_CM:
						tempf /= 10; 	
						break;
					case WY_UNIT_DM:
						tempf /= 100;	
						break;
					case WY_UNIT_M:
						tempf /= 1000;	
						break; 
					default:
						break;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}	
			
			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_entry_get(g_controlParameter.curChannel);
				switch ( g_controlParameter.wyChannelUnit )
				{
					case WY_UNIT_MM:						
						break;
					case WY_UNIT_CM:
						tempf /= 10; 	
						break;
					case WY_UNIT_DM:
						tempf /= 100;	
						break;
					case WY_UNIT_M:
						tempf /= 1000;	
						break; 
					default:
						break;
				}
				floattochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,g_controlParameter.oneLevelMenu[index].pointBit,tempf,g_controlParameter.parameterData[index]);
			}
			break;
		case SMPL_BX_NUM:
			index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_full_p_get(g_controlParameter.curChannel);
				switch ( g_controlParameter.bxChannelUnit )
				{
					case BX_UNIT_MM:						
						break;
					case BX_UNIT_CM:
						tempf /= 10; 	
						break;
					case BX_UNIT_DM:
						tempf /= 100;	
						break;
					case BX_UNIT_M:
						tempf /= 1000;	
						break; 
					default:
						break;
				}
				numtochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,(uint32_t)tempf,g_controlParameter.parameterData[index]);
			}

			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempf = smpl_ctrl_entry_get(g_controlParameter.curChannel);
				switch ( g_controlParameter.bxChannelUnit )
				{
					case BX_UNIT_MM:						
						break;
					case BX_UNIT_CM:
						tempf /= 10; 	
						break;
					case BX_UNIT_DM:
						tempf /= 100;	
						break;
					case BX_UNIT_M:
						tempf /= 1000;	
						break; 
					default:
						break;
				}
				floattochar(MAX_CONTROL_PARAMETER_PUTIN_BIT,g_controlParameter.oneLevelMenu[index].pointBit,tempf,g_controlParameter.parameterData[index]);
			}
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterWriteParameter( void )
{
	float tempf = 0;
	uint32_t tempu = 0;
	uint8_t index = 0;
	
	switch ( g_controlParameter.curChannel )
	{
		case SMPL_FH_NUM:
			{
				float maxForce = 0;
				
				index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
				if (index != 0xff)
				{
					maxForce = ustrtoul(g_controlParameter.parameterData[index],0,10);
					if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
					{
						maxForce *= 1000;
					}
					smpl_ctrl_full_p_set(g_controlParameter.curChannel,maxForce);
					smpl_ctrl_full_n_set(g_controlParameter.curChannel,-maxForce);
				}
			}
			
			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempu *= 1000;
				}
				smpl_ctrl_entry_set(g_controlParameter.curChannel,tempu);
			}
			
			index = GetControlParameterIndex(OBJECT_CURVE_SHOW_START_VALUE);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempu *= 1000;
				}
				pHmi->start_force[g_controlParameter.curChannel] = tempu;
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_START_FORCE);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempu *= 1000;
				}
				pHmi->break_judge_value[g_controlParameter.curChannel] = tempu;
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_DOWM_POINT);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);

				pHmi->break_point[g_controlParameter.curChannel] = tempu;
			}
			
			index = GetControlParameterIndex(OBJECT_BREAK_TYPE);
			if (index != 0xff)
			{
				uint8_t index = g_controlParameter.twoLevelMenu[index].index;
				
				pHmi->break_condition[g_controlParameter.curChannel] = index;
			}
			
			index = GetControlParameterIndex(OBJECT_FORCE_DECAY_RATE);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
				
				pHmi->attenuationRate[g_controlParameter.curChannel] = tempu;
			}
			
			index = GetControlParameterIndex(OBJECT_MAX_FORCE_DIFF);
			if (index != 0xff)
			{
				tempf = str2float(g_controlParameter.parameterData[index]);
				if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempf *= 1000;
				}
				pHmi->break_max_value[g_controlParameter.curChannel] = tempf;
			}	
			
			index = GetControlParameterIndex(OBJECT_YIELD_DISTURB_THRESHOLD);
			if (index != 0xff)
			{
				tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
				pHmi->yieldDisturbThreshold = tempu;
			}
			break;
		case SMPL_WY_NUM:
			{
				float maxValue = 0;
				
				index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
				if (index != 0xff)
				{
					maxValue = ustrtoul(g_controlParameter.parameterData[index],0,10);
					switch ( g_controlParameter.wyChannelUnit )
					{
						case WY_UNIT_MM:						
							break;
						case WY_UNIT_CM:
							maxValue *= 10; 	
							break;
						case WY_UNIT_DM:
							maxValue *= 100;	
							break;
						case WY_UNIT_M:
							maxValue *= 1000;	
							break; 
						default:
							break;
					}
					smpl_ctrl_full_p_set(g_controlParameter.curChannel,maxValue);
					smpl_ctrl_full_n_set(g_controlParameter.curChannel,-maxValue);
				}
			}
			
			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempf = str2float(g_controlParameter.parameterData[index]);
				switch ( g_controlParameter.wyChannelUnit )
				{
					case WY_UNIT_MM:						
						break;
					case WY_UNIT_CM:
						tempf *= 10; 	
						break;
					case WY_UNIT_DM:
						tempf *= 100;	
						break;
					case WY_UNIT_M:
						tempf *= 1000;	
						break; 
					default:
						break;
				}
				smpl_ctrl_entry_set(g_controlParameter.curChannel,tempf);
			}
			break;
		case SMPL_BX_NUM:
			{
				float maxValue = 0;
				
				index = GetControlParameterIndex(OBJECT_SYSTEM_MAX_VALUE);
				if (index != 0xff)
				{
					maxValue = ustrtoul(g_controlParameter.parameterData[index],0,10);
					switch ( g_controlParameter.bxChannelUnit )
					{
						case BX_UNIT_MM:						
							break;
						case BX_UNIT_CM:
							maxValue *= 10; 	
							break;
						case BX_UNIT_DM:
							maxValue *= 100;	
							break;
						case BX_UNIT_M:
							maxValue *= 1000;	
							break; 
						default:
							break;
					}
					smpl_ctrl_full_p_set(g_controlParameter.curChannel,maxValue);
					smpl_ctrl_full_n_set(g_controlParameter.curChannel,-maxValue);
				}
			}
			
			index = GetControlParameterIndex(OBJECT_LOAD_START_VALUE);
			if (index != 0xff)
			{
				tempf = str2float(g_controlParameter.parameterData[index]);
				switch ( g_controlParameter.bxChannelUnit )
				{
					case BX_UNIT_MM:						
						break;
					case BX_UNIT_CM:
						tempf *= 10; 	
						break;
					case BX_UNIT_DM:
						tempf *= 100;	
						break;
					case BX_UNIT_M:
						tempf *= 1000;	
						break; 
					default:
						break;
				}
				smpl_ctrl_entry_set(g_controlParameter.curChannel,tempf);
			}
			break;
		default:
			break;
	}
	
	pcm_save();
	prm_save();
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneRowOneLevelMenu
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ControlParameterOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_controlParameter.oneLevelMenu[index].x + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_controlParameter.oneLevelMenu[index].y + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_controlParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_controlParameter.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_controlParameter.oneLevelMenu[index].lenth - 2 * g_controlParameter.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_controlParameter.oneLevelMenu[index].width - 2 * g_controlParameter.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_controlParameter.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_ControlParameterOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ControlParameterOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		Show_ControlParameterOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_ControlParameterOneRowOneLevelMenuUnit
 * Description    : 显示一行参数单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ControlParameterOneRowOneLevelMenuUnit( uint8_t index )
{
	const uint16_t x = g_controlParameter.oneLevelMenu[index].x + g_controlParameter.oneLevelMenu[index].lenth - \
					   g_controlParameter.oneLevelMenu[index].lineWidth - 5 * 12;
	const uint16_t y = g_controlParameter.oneLevelMenu[index].y + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_controlParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_controlParameter.oneLevelMenu[index].backColor;
	
	if ( strcmp(g_controlParameter.pParameterUnitArray[index],"NULL") )
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_controlParameter.pParameterUnitArray[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_ControlParameterOneLevelMenuUnit
 * Description    : 显示一级菜单单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ControlParameterOneLevelMenuUnit( void )
{
	uint8_t i;
	
	for (i=0; i<g_controlParameter.curParameterNum; ++i)
	{
		Show_ControlParameterOneRowOneLevelMenuUnit(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ControlParameter
 * Description    : 遍历控制参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ControlParameter( void )
{
	Show_ControlParameterOneLevelMenuContent();
	
	Show_ControlParameterOneLevelMenuUnit();
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_controlParameter.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_controlParameter.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_controlParameter.enablePutin;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_controlParameter.oneLevelMenu[index].x + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_controlParameter.oneLevelMenu[index].y + g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_CONTROL_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_CONTROL_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_controlParameter.oneLevelMenu[index].lenth - 2 * g_controlParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_controlParameter.oneLevelMenu[index].width - 2 * g_controlParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_controlParameter.putinNum;
	pPutin->SaveType = g_controlParameter.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_controlParameter.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_controlParameter.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_controlParameter.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_controlParameter.curParameterNum;
	indexObj.pNowIndex = &g_controlParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_controlParameter.nowIndex != g_controlParameter.recordIndex)
	{		
		g_controlParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterStatusProcess( void )
{
	uint8_t index = g_controlParameter.nowIndex;
	
	if (g_controlParameter.nowIndex >= g_controlParameter.curParameterNum)
	{
		g_controlParameter.nowIndex = 0;
	}
	
	switch ( g_controlParameter.twoLevelMenu[index].parameterType )
	{
		case IMMEDIATELY_PUTIN_NONE:
		case IMMEDIATELY_PUTIN_SHIFT:
			g_controlParameter.enablePutin = ENABLE;
			break;
		
		default:
			g_controlParameter.enablePutin = DISABLE;
			break;
	}
	
	switch ( g_controlParameter.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_controlParameter.enableArrow = ENABLE;
			break;
		
		default:
			g_controlParameter.enableArrow = DISABLE;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : Clear_RControlParameterOneRowOneLevelMenuArrow
 * Description    : 清除一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_ControlParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_controlParameter.oneLevelMenu[index].x + \
					   g_controlParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_controlParameter.oneLevelMenu[index].y + \
					   g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_controlParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Show_ControlParameterOneRowOneLevelMenuArrow
 * Description    : 显示一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ControlParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_controlParameter.oneLevelMenu[index].x + \
					   g_controlParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_controlParameter.oneLevelMenu[index].y + \
					   g_controlParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterMoveCursorProcess( void )
{
	if (g_controlParameter.isIndexMove == YES)
	{		
		if (g_controlParameter.recordIndex != 0xff)
		{
			g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].pointColor = \
				g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].recordPointColor;
			g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].backColor = \
				g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].recordBackColor;
			
			Show_ControlParameterOneRowOneLevelMenuContent(g_controlParameter.recordIndex);
			Show_ControlParameterOneRowOneLevelMenuUnit(g_controlParameter.recordIndex);
			Clear_ControlParameterOneRowOneLevelMenuArrow(g_controlParameter.recordIndex);			
			
			g_controlParameter.oneLevelMenu[g_controlParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_controlParameter.oneLevelMenu[g_controlParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ControlParameterOneRowOneLevelMenuContent(g_controlParameter.nowIndex);
			Show_ControlParameterOneRowOneLevelMenuUnit(g_controlParameter.nowIndex);	
			
			if (g_controlParameter.enableArrow == ENABLE)
			{
				Show_ControlParameterOneRowOneLevelMenuArrow(g_controlParameter.nowIndex);
			}
		}
		else
		{
			g_controlParameter.oneLevelMenu[g_controlParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_controlParameter.oneLevelMenu[g_controlParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ControlParameterOneRowOneLevelMenuContent(g_controlParameter.nowIndex);
			Show_ControlParameterOneRowOneLevelMenuUnit(g_controlParameter.nowIndex);
			
			if (g_controlParameter.enableArrow == ENABLE)
			{
				Show_ControlParameterOneRowOneLevelMenuArrow(g_controlParameter.nowIndex);
			}
		}
		
		g_controlParameter.recordIndex = g_controlParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_controlParameter.refreshShortcut == ENABLE)
	{
		g_controlParameter.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[0] = pTwoLevelMenu[6];
		pShortCut->pContent[1] = pTwoLevelMenu[93];
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterIndexUpdate( void )
{
	uint8_t recordIndex = g_controlParameter.nowIndex;
	
	g_controlParameter.nowIndex++;
	g_controlParameter.nowIndex %= g_controlParameter.curParameterNum;
	
	if (g_controlParameter.nowIndex == recordIndex)
	{
		g_controlParameter.recordIndex = 0xff;
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterUpdateStatus( void )
{
	ControlParameterIndexUpdate();
	
	g_controlParameter.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterKeyProcess( void )
{
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	uint8_t index = g_controlParameter.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_controlParameter.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_controlParameter.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* 处理快捷菜单 */
					GUI_ClearShortcutMenu();
					
					/* 还原一级菜单 */
					g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].pointColor = \
						g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].recordPointColor;
					g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].backColor = \
						g_controlParameter.oneLevelMenu[g_controlParameter.recordIndex].recordBackColor;
					
					Show_ControlParameterOneRowOneLevelMenuContent(g_controlParameter.recordIndex);
					
					/* 配置二级菜单 */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_controlParameter.twoLevelMenu[index].x;
					pMenu->y = g_controlParameter.twoLevelMenu[index].y;
					pMenu->nowIndex = g_controlParameter.twoLevelMenu[index].index;
					pMenu->maxUpY = g_controlParameter.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_controlParameter.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_controlParameter.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_controlParameter.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_controlParameter.twoLevelMenu[index].lenth;
					pMenu->width = g_controlParameter.twoLevelMenu[index].width;
					pMenu->pointColor = g_controlParameter.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_controlParameter.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_controlParameter.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_controlParameter.twoLevelMenu[index].pointColor;
					pMenu->nums = g_controlParameter.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_controlParameter.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_controlParameter.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{
						g_controlParameter.twoLevelMenu[index].index = pMenu->nowIndex;
						
						strcpy(g_controlParameter.parameterData[index],pMenu->pParameterNameArray[pMenu->nowIndex]);
					}
					ControlParameterUpdateStatus();
				}
				break;
			
			case KEY_ENTER:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						return;
					
					case STATUS_EDIT_COMP:							
						switch ( g_controlParameter.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_controlParameter.putinNum,*GetPutinIntDataAddr(),g_controlParameter.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_controlParameter.putinNum,g_controlParameter.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_controlParameter.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_controlParameter.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				if (FAILED == ControlParameterCheckData(index) )
				{
					g_controlParameter.leavePage.flagLeavePage = SET;
					g_controlParameter.leavePage.flagSaveData = RESET;
					break;
				}
				ControlParameterUpdateStatus();
				break;
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_controlParameter.leavePage.flagLeavePage = SET;
						g_controlParameter.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						ControlParameterUpdateStatus();
						break;
					
					default:						
						break;
				}
				break;
			case KEY_F1:
				SetPage(CALIBRATION_TABLE);
				g_controlParameter.leavePage.flagLeavePage = SET;
				g_controlParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F2:
				SetPage(EXTEND_PARAMETER_PAGE);
				g_controlParameter.leavePage.flagLeavePage = SET;
				g_controlParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_controlParameter.leavePage.flagLeavePage = SET;
				g_controlParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_controlParameter.leavePage.flagLeavePage = SET;
				g_controlParameter.leavePage.flagSaveData = RESET;
				break;
			
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ControlParameterLeavePageCheckCycle( void )
{
	if (g_controlParameter.leavePage.flagLeavePage == SET)
	{
		if (g_controlParameter.leavePage.flagSaveData == SET)
		{
			uint8_t i;
			
			for (i=0; i<g_controlParameter.curParameterNum; ++i)
			{
				if (FAILED == ControlParameterCheckData(i) )
				{
					SetPage(CONTROL_PARAMETER);
					PopWindowsProcessCycle();
					
					return;
				}
			}
			
			ControlParameterWriteParameter();
			
			SetLssuedParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ControlParameterCheckData
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ControlParameterCheckData( uint8_t index )
{
	float tempf = 0;
	uint32_t tempu = 0;
	uint8_t handle = 0;
	
	if (index >= g_controlParameter.curParameterNum)
	{
		return PASSED;
	}
	
	handle = g_controlParameter.indexArray[index];
	
	switch (handle)
	{
		case OBJECT_SYSTEM_MAX_VALUE:
			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			
			switch ( g_controlParameter.curChannel )
			{
				case SMPL_FH_NUM:	
					if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
					{
						tempu *= 1000;
						
						if ((tempu < 10000) || (tempu > 10000000))
						{
							SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[0]);
							
							return FAILED;
						}
					}
					else
					{
						if ((tempu < 10000) || (tempu > 100000))
						{
							SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoN[0]);
							
							return FAILED;
						}
					}	
					break;
				case SMPL_WY_NUM:
					switch ( g_controlParameter.wyChannelUnit )
					{
						case WY_UNIT_MM:						
							break;
						case WY_UNIT_CM:
							tempu *= 10; 	
							break;
						case WY_UNIT_DM:
							tempu *= 100;	
							break;
						case WY_UNIT_M:
							tempu *= 1000;	
							break; 
						default:
							break;
					}
					
					if ((tempu < 1) || (tempu > 10000))
					{
						SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfo[0]);
						
						return FAILED;
					}	
					break;
				case SMPL_BX_NUM:
					switch ( g_controlParameter.bxChannelUnit )
					{
						case BX_UNIT_MM:						
							break;
						case BX_UNIT_CM:
							tempu *= 10; 	
							break;
						case BX_UNIT_DM:
							tempu *= 100;	
							break;
						case BX_UNIT_M:
							tempu *= 1000;	
							break; 
						default:
							break;
					}
			
					if ((tempu < 1) || (tempu > 10000))
					{
						SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfo[2]);
						
						return FAILED;
					}	
					break;
				default:
					break;
			}
			break;
		case OBJECT_LOAD_START_VALUE:
			switch ( g_controlParameter.curChannel )
			{
				case SMPL_FH_NUM:
					tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			
					if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
					{
						tempu *= 1000;
						
						if ((tempu < 1000) || (tempu > 100000))
						{
							SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[2]);
							
							return FAILED;
						}
					}
					else
					{
						if ((tempu < 1) || (tempu > 10000))
						{
							SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoN[2]);
							
							return FAILED;
						}
					}
					break;
				case SMPL_WY_NUM:
					tempf = str2float(g_controlParameter.parameterData[index]);
				
					switch ( g_controlParameter.wyChannelUnit )
					{
						case WY_UNIT_MM:						
							break;
						case WY_UNIT_CM:
							tempf *= 10; 	
							break;
						case WY_UNIT_DM:
							tempf *= 100;	
							break;
						case WY_UNIT_M:
							tempf *= 1000;	
							break; 
						default:
							break;
					}
					
					if ((fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE))
					{
						SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParamErrInfo[4]);
						
						return FAILED;
					}
					break;
				case SMPL_BX_NUM:
					tempf = str2float(g_controlParameter.parameterData[index]);
				
					switch ( g_controlParameter.bxChannelUnit )
					{
						case BX_UNIT_MM:						
							break;
						case BX_UNIT_CM:
							tempf *= 10; 	
							break;
						case BX_UNIT_DM:
							tempf *= 100;	
							break;
						case BX_UNIT_M:
							tempf *= 1000;	
							break; 
						default:
							break;
					}
					
					if ((fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE))
					{
						SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParamErrInfo[5]);
						
						return FAILED;
					}
					break;
				default:
					break;
			}
			break;
		case OBJECT_CURVE_SHOW_START_VALUE:
			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
			{
				tempu *= 1000;
				if ((tempu < 1000) || (tempu > 100000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[4]);
					
					return FAILED;
				}
			}
			else
			{
				if ((tempu < 1) || (tempu > 10000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoN[4]);
					
					return FAILED;
				}
			}
			break;
		case OBJECT_BREAK_START_FORCE:
			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
			{
				tempu *= 1000;
				if ((tempu < 1000) || (tempu > 1000000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[6]);
					
					return FAILED;
				}
			}
			else
			{
				if ((tempu < 1) || (tempu > 10000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoN[6]);
					
					return FAILED;
				}
			}
			break;
		case OBJECT_BREAK_DOWM_POINT:
			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			if ((tempu < 1) || (tempu > 100))
			{
				SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[8]);
				
				return FAILED;
			}
			break;
		
		case OBJECT_FORCE_DECAY_RATE:
			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
			if ((tempu < 1) || (tempu > 100))
			{
				SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[12]);
				
				return FAILED;
			}
			break;
		
		case OBJECT_MAX_FORCE_DIFF:
			tempf = str2float(g_controlParameter.parameterData[index]);
			if (g_controlParameter.fhChannelUnit == FH_UNIT_kN)
			{
				tempf *= 1000;
				if ((tempf < 100) || (tempf > 100000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[10]);
					
					return FAILED;
				}
			}
			else
			{
				if ((tempf < 1) || (tempf > 10000))
				{
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoN[10]);
					
					return FAILED;
				}
			}
			break;
			
		case OBJECT_YIELD_DISTURB_THRESHOLD:
//			tempu = ustrtoul(g_controlParameter.parameterData[index],0,10);
//			if (tempu > 60000)
//			{
//				SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParamErrInfoKN[14]);
//				
//				return FAILED;
//			}
			break;
		
		default:
			break;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : GetJudgeBreakType
 * Description    : 获取判破方式
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
JUDGE_BREAK_TYPE_TypeDef GetJudgeBreakType( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return ATTENUATION_RATE;
	}
	else
	{
		switch ( pHmi->break_condition[channel] ) 
		{
			case 0:		
				return ATTENUATION_RATE;
			case 1:
				return WITH_MAX_FORCE_DIFFERENCE;
			default:
				return ADJOIN_TWO_POINT_DIFFERENCE;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTargetAttenuationRate
 * Description    : 获取目标衰减率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetTargetAttenuationRate( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->attenuationRate[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetWithMaxForceDifference
 * Description    : 获取目标与最大力差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetTargetWithMaxForceDifference( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->break_max_value[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTargetBreakDownPoint
 * Description    : 获取判破下降点
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetTargetBreakDownPoint( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->break_point[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTargetAdjoinTwoPointDiff
 * Description    : 获取目标相邻量点差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetTargetAdjoinTwoPointDiff( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->break_border_value[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTargetBreakStartValue
 * Description    : 获取破碎起判力
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetTargetBreakStartValue( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->break_judge_value[channel];
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
