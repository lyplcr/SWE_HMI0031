/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-12-7 08:52:19
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   力值标定页
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "ForceCalibration.h"
#include "ChannelSelect.h"
#include "CalibrationParameter.h"
#include "printer.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_CALIBRATION_POINT_SEGS		10
#define MAX_DATA_BIT					8
#define MAX_FIELD_NUM					5

#define MAX_ONE_PAGE_SHOW_NUM			8	

/* 示值窗 */
#define INDICATE_WINDOWS_NUMS				2		//示值窗个数
#define	INDICATE_WINDOWS_ROW_NUMS			2		//示值窗行数
#define INDICATE_WINDOWS_FORCE_CHAR_NUM		8		
#define INDICATE_WINDOWS_SPEED_CHAR_NUM		6

/* 状态栏 */
#define STATUS_BAR_NUMS						5	

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	REFRESH_FORCE = 0,
	REFRESH_SPEED,
	REFRESH_COMMUNICATION_STATUS,
	REFRESH_CALIBRATION_STATUS,
	REFRESH_CODE,
	REFRESH_TIME,
	REFRESH_PERIPHERAL_STATUS,
}REFRESH_TERM_TypeDef;

typedef enum
{
	OBJECT_SERIAL = 0,
	OBJECT_CHECK_POINT,
	OBJECT_FORCE,
	OBJECT_CODE,
	OBJECT_ERROR,
}OBJECT_FORCE_CALIBRATION_TypeDef;

typedef enum
{
	INDEX_SERIAL = 0,
	INDEX_CHECK_POINT,
	INDEX_FORCE,
	INDEX_CODE,
	INDEX_ERROR,
}INDEX_FORCE_CALIBRATION_TypeDef;

typedef struct
{
	char parameterData[MAX_DATA_BIT+1];
}FORCE_CALIBRATION_FIELD_DATA_TypeDef;

typedef enum
{
	OBJECT_WINDOWS_FORCE = 0,	
	OBJECT_WINDOWS_SPEED,		
}OBJECT_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	INDEX_WINDOWS_FORCE = 0,	
	INDEX_WINDOWS_SPEED,		
}INDEX_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	INDEX_COMM_STATUS = 0,
	INDEX_CALIBRATION_STATUS,
	INDEX_BAR_CODE,
	INDEX_TIME,
	INDEX_STATUS,
}INDEX_STATUS_BAR_NAME_TypeDef;

typedef enum
{
	OBJECT_COMM_STATUS = 0,
	OBJECT_CALIBRATION_STATUS,
	OBJECT_BAR_CODE,
	OBJECT_TIME,
	OBJECT_PERIPHERAL_STATUS,
}OBJECT_STATUS_BAR_NAME_TypeDef;

typedef struct
{
	char force[INDICATE_WINDOWS_FORCE_CHAR_NUM+1];
	char speed[INDICATE_WINDOWS_SPEED_CHAR_NUM+1];
}INDICATE_WINDOWS_FIELD_DATA_TypeDef;

typedef struct
{
	uint8_t indexIndicateWindowsArray[INDICATE_WINDOWS_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuIndicateWindows[INDICATE_WINDOWS_ROW_NUMS][INDICATE_WINDOWS_NUMS];
	const char *pIndicateWindowsTitleNameArray[INDICATE_WINDOWS_NUMS];
	uint8_t indicateWindowNum;						//示值窗字段个数
	INDICATE_WINDOWS_FIELD_DATA_TypeDef indicateWindowsData[INDICATE_WINDOWS_NUMS];
	
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTitle[MAX_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTable[MAX_CALIBRATION_POINT_SEGS][MAX_FIELD_NUM];
	FORCE_CALIBRATION_FIELD_DATA_TypeDef fieldData[MAX_CALIBRATION_POINT_SEGS][MAX_FIELD_NUM]; 
	const char *pParameterNameArray[MAX_FIELD_NUM];	
	ALIGN_TYPE_TypeDef align[MAX_FIELD_NUM];		//对齐方式
	uint8_t indexFieldArray[MAX_FIELD_NUM];			//句柄索引
	
	uint8_t indexStatusBarArray[STATUS_BAR_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuStatusBar[STATUS_BAR_NUMS];
	const char *pStatusBarTitleNameArray[STATUS_BAR_NUMS];
	uint8_t statusBarNum;							//状态栏个数
	
	const char * pTitle;							//标题
	uint8_t curPageSampleNum;						//当前页试块个数
	uint8_t sumSampleNum;							//全部试块个数
	uint8_t curPage;								//当前页数
	uint8_t sumPage;								//总页数
	uint8_t fieldNum;								//字段个数
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页	
	FH_UINT_TypeDef fhChannelUnit;					//负荷通道单位
	WY_UINT_TypeDef	wyChannelUnit;					//位移通道单位
	BX_UINT_TypeDef	bxChannelUnit;					//变形通道单位
	SMPL_NAME_TypeDef curChannel;					//当前通道
}FORCE_CALIBRATION_TypeDef;

typedef struct
{
	FlagStatus startCalibration;					//开始标志
	FlagStatus endCalibration;						//结束标志
	uint8_t curCompletePieceSerial;					//已完成试块编号
	FlagStatus flagCalibrationComplete;				//校准完成标志
	FlagStatus flagTakePoint;						//打点
	FlagStatus cancelPoint;							//撤销
	CALIBRATION_STATUS_TypeDef calibrationStatus;
	uint8_t serial[MAX_CALIBRATION_POINT_SEGS];
	int32_t checkValue[MAX_CALIBRATION_POINT_SEGS];
	float realValue[MAX_CALIBRATION_POINT_SEGS];
	int32_t code[MAX_CALIBRATION_POINT_SEGS];
	float deviation[MAX_CALIBRATION_POINT_SEGS];
}FORCE_CALIBRATION_BODY_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pForceCalibrationFieldName[] = 
{
	"序号",			//0
	"检测点(kN)",	//1
	"检测点(N)",		//2
	"力值(kN)",		//3
	"力值(N)",		//4
	"力码",			//5
	"误差(%)",		//6
	"检测点(mm)",	//7
	"检测点(cm)",	//8
	"检测点(dm)",	//9
	"检测点(m)",		//10
	"位移值(mm)",	//11
	"位移值(cm)",	//12
	"位移值(dm)",	//13
	"位移值(m)",		//14
	"变形值(mm)",	//15
	"变形值(cm)",	//16
	"变形值(dm)",	//17
	"变形值(m)",		//18
	"位移码",		//19
	"变形码",		//20
};

const char * const pForceCalibrationIndecateWindowsTitleName[] = 
{
	"力值(kN)",		//0
	"力值(N)",		//1
	"速度(kN/s)",	//2
	"速度(N/s)",	//3
	"位移(mm)",		//4
	"位移(cm)",		//5
	"位移(dm)",		//6
	"位移(m)",		//7
	"变形(mm)",		//8
	"变形(cm)",		//9
	"变形(dm)",		//10
	"变形(m)",		//11
	"速度(mm/s)",	//12
	"速度(cm/s)",	//13
	"速度(dm/s)",	//14
	"速度(m/s)",	//15
};

const char * const pCalibrationStatusBarTitleName[] = 
{
	"通讯状态：",	//0
	"标定状态：",	//1
	"码值：",		//2
	"时间：",		//3
	"",				//4
};

const char * const pForceCalibrationWarning[] = 
{
	"当前处于脱机状态！",			//0
	"标定点个数不能为0！",			//1
	"是否写入标定表？",				//2
	"标定数据不是递增趋势！",		//3
	"标定表已更新！",				//4
	"发送力值清零命令失败！",		//5
	"发送位移清零命令失败！",		//6
	"发送变形清零命令失败！",		//7
	"系统不支持当前配置的机型！",		//8
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_FORCE_CALIBRATION",zidata = "RAM_FORCE_CALIBRATION"
	static FORCE_CALIBRATION_TypeDef g_ForceCalibration;
	static FORCE_CALIBRATION_BODY_TypeDef g_CalibrationBody;
	static CALIBRATION_RSULT_PRINT_TypeDef g_CalibrationResult;
#pragma arm section


/* Private function prototypes -----------------------------------------------*/
static void ForceCalibationInit( void );
static void ForceCalibationConfig( void );
static void ForceCalibrationReadParameter( void );
static void GUI_ForceCalibration( void );
static void Traverse_ForceCalibration( void );
static void ForceCalibrationShortcutCycleTask( void );
static void ForceCalibrationKeyProcess( void );
static void ForceCalibrationCtrlCoreCycle( void );
static void ReloadForceCalibrationArea( void );
static void ForceCalibrationLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadForceCalibationPage
 * Description    : 力值标定页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadForceCalibationPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	ForceCalibationInit();
	
	/* 参数配置 */
	ForceCalibationConfig();
	
	/* 获取参数 */
	ForceCalibrationReadParameter();
	
	/* 画GUI框架 */
	GUI_ForceCalibration();
	
	/* 遍历 */
	Traverse_ForceCalibration();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_ForceCalibration.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 快捷菜单 */
		ForceCalibrationShortcutCycleTask();
		
		/* 按键处理 */
		ForceCalibrationKeyProcess();
		
		/* 控制核心 */
		ForceCalibrationCtrlCoreCycle();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ForceCalibrationLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibationInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibationInit( void )
{
	g_ForceCalibration.fhChannelUnit = GetFH_SmplUnit();
	g_ForceCalibration.wyChannelUnit = GetWY_SmplUnit();
	g_ForceCalibration.bxChannelUnit = GetBX_SmplUnit();
	g_ForceCalibration.curChannel = GetChannelSelectChannel();
	
	InitInterfaceElement();
	
	g_ForceCalibration.refreshShortcut = ENABLE;
	g_ForceCalibration.leavePage.flagLeavePage = RESET;
	g_ForceCalibration.leavePage.flagSaveData = RESET;
	
	g_ForceCalibration.curPageSampleNum = 0;
	g_ForceCalibration.sumSampleNum = 0;	
	g_ForceCalibration.sumPage = 1;
	
	g_CalibrationBody.startCalibration = RESET;
	g_CalibrationBody.endCalibration = RESET;
}

/*------------------------------------------------------------
 * Function Name  : GetForceCalibationFieldIndex
 * Description    : 获取字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_ForceCalibration.fieldNum; ++i)
	{
		if (g_ForceCalibration.indexFieldArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetForceCalibationIndicateWindowsFieldIndex
 * Description    : 获取示值窗字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationIndicateWindowsFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_ForceCalibration.indicateWindowNum; ++i)
	{
		if (g_ForceCalibration.indexIndicateWindowsArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetForceCalibationStatusBarFieldIndex
 * Description    : 获取状态栏字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationStatusBarFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_ForceCalibration.statusBarNum; ++i)
	{
		if (g_ForceCalibration.indexStatusBarArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationStatus
 * Description    : 获取校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static CALIBRATION_STATUS_TypeDef GetCalibrationStatus( void )
{
	return g_CalibrationBody.calibrationStatus;
}

/*------------------------------------------------------------
 * Function Name  : SetCalibrationStatus
 * Description    : 设置校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetCalibrationStatus( CALIBRATION_STATUS_TypeDef coreStatus )
{
	g_CalibrationBody.calibrationStatus = coreStatus;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibationConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibationConfig( void )
{
	uint8_t i;
	
	/* 标题 */
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			if (GetPageName() == FORCE_CALIBRATION_PAGE)
			{
				g_ForceCalibration.pTitle = "力值校准";
			}
			else
			{
				g_ForceCalibration.pTitle = "力值检定";
			}

			/* 字段名 */
			g_ForceCalibration.pParameterNameArray[INDEX_SERIAL] 		= pForceCalibrationFieldName[0];
			if (g_ForceCalibration.fhChannelUnit == FH_UNIT_kN)
			{
				g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[1];
				g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[3];
			}
			else
			{
				g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[2];
				g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[4];
			}
			
			g_ForceCalibration.pParameterNameArray[INDEX_CODE] 			= pForceCalibrationFieldName[5];
			g_ForceCalibration.pParameterNameArray[INDEX_ERROR] 		= pForceCalibrationFieldName[6];
			break;
		case SMPL_WY_NUM:
			if (GetPageName() == FORCE_CALIBRATION_PAGE)
			{
				g_ForceCalibration.pTitle = "位移校准";
			}
			else
			{
				g_ForceCalibration.pTitle = "位移检定";
			}

			/* 字段名 */
			g_ForceCalibration.pParameterNameArray[INDEX_SERIAL] 		= pForceCalibrationFieldName[0];
			switch ( g_ForceCalibration.wyChannelUnit )
			{
				case WY_UNIT_MM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[7];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[11];	
					break;
				case WY_UNIT_CM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[8];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[12];		
					break;
				case WY_UNIT_DM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[9];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[13];		
					break;
				case WY_UNIT_M:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[10];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[14];		
					break; 
				default:
					break;
			}
			
			g_ForceCalibration.pParameterNameArray[INDEX_CODE] 			= pForceCalibrationFieldName[19];
			g_ForceCalibration.pParameterNameArray[INDEX_ERROR] 		= pForceCalibrationFieldName[6];
			break;
		case SMPL_BX_NUM:
			if (GetPageName() == FORCE_CALIBRATION_PAGE)
			{
				g_ForceCalibration.pTitle = "变形校准";
			}
			else
			{
				g_ForceCalibration.pTitle = "变形检定";
			}

			/* 字段名 */
			g_ForceCalibration.pParameterNameArray[INDEX_SERIAL] 		= pForceCalibrationFieldName[0];
			switch ( g_ForceCalibration.bxChannelUnit )
			{
				case BX_UNIT_MM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[7];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[15];	
					break;
				case BX_UNIT_CM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[8];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[16];		
					break;
				case BX_UNIT_DM:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[9];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[17];		
					break;
				case BX_UNIT_M:
					g_ForceCalibration.pParameterNameArray[INDEX_CHECK_POINT] 	= pForceCalibrationFieldName[10];
					g_ForceCalibration.pParameterNameArray[INDEX_FORCE] 		= pForceCalibrationFieldName[18];		
					break; 
				default:
					break;
			}
			
			g_ForceCalibration.pParameterNameArray[INDEX_CODE] 			= pForceCalibrationFieldName[20];
			g_ForceCalibration.pParameterNameArray[INDEX_ERROR] 		= pForceCalibrationFieldName[6];
			break;
		default:
			break;
	}
	
	/* 示值窗索引 */
	g_ForceCalibration.indexIndicateWindowsArray[INDEX_WINDOWS_FORCE] 		= OBJECT_WINDOWS_FORCE;
	g_ForceCalibration.indexIndicateWindowsArray[INDEX_WINDOWS_SPEED] 		= OBJECT_WINDOWS_SPEED;
	
	/* 字段索引值 */
	g_ForceCalibration.indexFieldArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
	g_ForceCalibration.indexFieldArray[INDEX_CHECK_POINT] 	= OBJECT_CHECK_POINT;
	g_ForceCalibration.indexFieldArray[INDEX_FORCE] 		= OBJECT_FORCE;
	g_ForceCalibration.indexFieldArray[INDEX_CODE] 			= OBJECT_CODE;
	g_ForceCalibration.indexFieldArray[INDEX_ERROR] 		= OBJECT_ERROR;
	
	/* 状态栏索引 */
	g_ForceCalibration.indexStatusBarArray[INDEX_COMM_STATUS] 			= OBJECT_COMM_STATUS;
	g_ForceCalibration.indexStatusBarArray[INDEX_CALIBRATION_STATUS] 	= OBJECT_CALIBRATION_STATUS;
	g_ForceCalibration.indexStatusBarArray[INDEX_BAR_CODE] 				= OBJECT_BAR_CODE;
	g_ForceCalibration.indexStatusBarArray[INDEX_TIME] 					= OBJECT_TIME;
	g_ForceCalibration.indexStatusBarArray[INDEX_STATUS] 				= OBJECT_PERIPHERAL_STATUS;
	
	/* 示值窗标题名 */
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			if (g_ForceCalibration.fhChannelUnit == FH_UNIT_kN)
			{
				g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[0];
				g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[2];
			}
			else
			{
				g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[1];
				g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[3];
			}	
			break;
		case SMPL_WY_NUM:
			switch ( g_ForceCalibration.wyChannelUnit )
			{
				case WY_UNIT_MM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[4];	
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[12];
					break;
				case WY_UNIT_CM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[5];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[13];
					break;
				case WY_UNIT_DM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[6];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[14];
					break;
				case WY_UNIT_M:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[7];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[15];
					break; 
				default:
					break;
			}
			break;
		case SMPL_BX_NUM:
			switch ( g_ForceCalibration.bxChannelUnit )
			{
				case BX_UNIT_MM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[8];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[12];
					break;
				case BX_UNIT_CM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[9];	
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[13];
					break;
				case BX_UNIT_DM:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[10];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[14];
					break;
				case BX_UNIT_M:
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pForceCalibrationIndecateWindowsTitleName[11];
					g_ForceCalibration.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pForceCalibrationIndecateWindowsTitleName[15];
					break; 
				default:
					break;
			}	
			break;
		default:
			break;
	}
		
	/* 状态栏标题 */
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_COMM_STATUS] 			= pCalibrationStatusBarTitleName[0];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_CALIBRATION_STATUS] 	= pCalibrationStatusBarTitleName[1];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_BAR_CODE] 			= pCalibrationStatusBarTitleName[2];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_TIME] 				= pCalibrationStatusBarTitleName[3];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_STATUS] 				= pCalibrationStatusBarTitleName[4];
	
	/* 字段个数 */
	g_ForceCalibration.fieldNum = MAX_FIELD_NUM;
	g_ForceCalibration.indicateWindowNum = INDICATE_WINDOWS_NUMS;	
	g_ForceCalibration.statusBarNum = STATUS_BAR_NUMS;
	
	/* 小数点位数 */
	for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
	{
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_SERIAL].pointBit 		= 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_CHECK_POINT].pointBit = 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_FORCE].pointBit 		= 2;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_CODE].pointBit 		= 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_ERROR].pointBit 		= 2;
	}
	
	/* 数据对齐 */
	g_ForceCalibration.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_ForceCalibration.align[INDEX_CHECK_POINT] = ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_FORCE] 		= ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_CODE] 		= ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_ERROR] 		= ALIGN_LEFT;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationReadParameter( void )
{
	CALIBRATION_POINT_TypeDef *pCalibrationPoint = NULL;
	uint8_t rowIndex = 0;
	uint8_t remain = 0;
	uint8_t baseIndex = 0;
	uint8_t fieldIndex = 0;
	float force = 0,disPlacement = 0,deform = 0;
	uint8_t dotNum = 0;
	int32_t tempu = 0;
	float tempf = 0;
	
	pCalibrationPoint = GetCalibrationPoint();

	/* 从校准参数界面获取原始数据 */
	for (rowIndex=0; rowIndex<pCalibrationPoint->calibrationSegments; ++rowIndex)
	{
		g_CalibrationBody.serial[rowIndex] = rowIndex + 1;
		
		g_CalibrationBody.checkValue[rowIndex] = pCalibrationPoint->calibrationValue[rowIndex];
	}
	
	/* 总个数 */
	g_ForceCalibration.sumSampleNum = pCalibrationPoint->calibrationSegments;
	
	if (g_ForceCalibration.sumSampleNum == 0)
	{
		return;
	}
	
	/* 总页数 */
	remain = g_ForceCalibration.sumSampleNum % MAX_ONE_PAGE_SHOW_NUM;
	if (remain)
	{
		g_ForceCalibration.sumPage = g_ForceCalibration.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM + 1;
	}
	else
	{
		g_ForceCalibration.sumPage = g_ForceCalibration.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM;
	}	
	
	/* 当前页 */
	if ((g_ForceCalibration.curPage==0) || (g_ForceCalibration.curPage>g_ForceCalibration.sumPage))
	{
		g_ForceCalibration.curPage = 1;
	}
	
	/* 当前页个数 */
	if (g_ForceCalibration.curPage == g_ForceCalibration.sumPage)
	{
		if (remain)
		{
			g_ForceCalibration.curPageSampleNum = remain;
		}
		else
		{
			g_ForceCalibration.curPageSampleNum = MAX_ONE_PAGE_SHOW_NUM;
		}
	}
	else
	{
		g_ForceCalibration.curPageSampleNum = MAX_ONE_PAGE_SHOW_NUM;
	}
	
	/* 基索引 */
	baseIndex = (g_ForceCalibration.curPage - 1) * MAX_ONE_PAGE_SHOW_NUM;
	
	/* 序号 */
	fieldIndex = GetForceCalibationFieldIndex(OBJECT_SERIAL);
	for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
	{
		tempu = g_CalibrationBody.serial[baseIndex+rowIndex];
		
		numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
	}
	
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			/* 检测点 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CHECK_POINT);
			
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{		
				force = g_CalibrationBody.checkValue[baseIndex+rowIndex];	
				if (g_ForceCalibration.fhChannelUnit == FH_UNIT_kN)
				{
					force /= 1000;
				}
				
				numtochar(MAX_DATA_BIT,(int32_t)force,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);	
			}	
			
			/* 力值 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				force = g_CalibrationBody.realValue[baseIndex+rowIndex];
				
				if (g_ForceCalibration.fhChannelUnit == FH_UNIT_kN)
				{
					force /= 1000;
				}
				
				dotNum = g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].pointBit;
				floattochar(MAX_DATA_BIT,dotNum,force,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			
			/* 力码 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				tempu = g_CalibrationBody.code[baseIndex+rowIndex];
				
				numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			break;
		case SMPL_WY_NUM:
			/* 检测点 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CHECK_POINT);
			
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				disPlacement = g_CalibrationBody.checkValue[baseIndex+rowIndex];	
				
				switch ( g_ForceCalibration.wyChannelUnit )
				{
					case WY_UNIT_MM:							
						break;
					case WY_UNIT_CM:
						disPlacement /= 10;	
						break;
					case WY_UNIT_DM:
						disPlacement /= 100;	
						break;
					case WY_UNIT_M:
						disPlacement /= 1000;	
						break; 
					default:
						break;
				}
				numtochar(MAX_DATA_BIT,(int32_t)disPlacement,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);	
			}
		
			/* 位移值 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
			
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				disPlacement = g_CalibrationBody.realValue[baseIndex+rowIndex];
				
				switch ( g_ForceCalibration.wyChannelUnit )
				{
					case WY_UNIT_MM:
							
						break;
					case WY_UNIT_CM:
						disPlacement /= 10;	
						break;
					case WY_UNIT_DM:
						disPlacement /= 100;	
						break;
					case WY_UNIT_M:
						disPlacement /= 1000;	
						break; 
					default:
						break;
				}				
				
				dotNum = g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].pointBit;
				floattochar(MAX_DATA_BIT,dotNum,disPlacement,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			
			/* 位移码 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				tempu = g_CalibrationBody.code[baseIndex+rowIndex];
				
				numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			break;
		case SMPL_BX_NUM:
			/* 检测点 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CHECK_POINT);
			
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				deform = g_CalibrationBody.checkValue[baseIndex+rowIndex];	
				switch ( g_ForceCalibration.bxChannelUnit )
				{
					case BX_UNIT_MM:							
						break;
					case BX_UNIT_CM:
						deform /= 10;	
						break;
					case BX_UNIT_DM:
						deform /= 100;	
						break;
					case BX_UNIT_M:
						deform /= 1000;	
						break; 
					default:
						break;
				}
				numtochar(MAX_DATA_BIT,(int32_t)deform,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);		
			}
			
			/* 变形值 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				deform = g_CalibrationBody.realValue[baseIndex+rowIndex];
				
				switch ( g_ForceCalibration.bxChannelUnit )
				{
					case BX_UNIT_MM:
							
						break;
					case BX_UNIT_CM:
						deform /= 10;	
						break;
					case BX_UNIT_DM:
						deform /= 100;	
						break;
					case BX_UNIT_M:
						deform /= 1000;	
						break; 
					default:
						break;
				}				
				
				dotNum = g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].pointBit;
				floattochar(MAX_DATA_BIT,dotNum,deform,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			
			/* 变形码 */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				tempu = g_CalibrationBody.code[baseIndex+rowIndex];
				
				numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			break;
		default:
			break;
	}
	
	/* 误差 */
	fieldIndex = GetForceCalibationFieldIndex(OBJECT_ERROR);
	for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
	{
		tempf = g_CalibrationBody.deviation[baseIndex+rowIndex];
		
		dotNum = g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].pointBit;
		floattochar(MAX_DATA_BIT,dotNum,tempf,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
	}
}	

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationOneFieldRectangleFrameCoordinate
 * Description    : 配置界面一个字段GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationOneFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_ForceCalibration.indexFieldArray[fieldIndex];
	
	/* 通用属性 */
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].x = x;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].y = y;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].pointColor = COLOR_POINT;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].backColor = COLOR_BACK;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].recordBackColor = COLOR_BACK;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].width = 28;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].fontSize = 24;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].rowDistance = 0;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].columnDistance = 0;
	g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth = 54;
			break;
		case OBJECT_CHECK_POINT:
			g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth = 126;
			break;
		
		case OBJECT_FORCE:
			g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth = 126;
			break;
		case OBJECT_CODE:
			g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth = 102;
			break;
		case OBJECT_ERROR:
			g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth = 102;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate
 * Description    : 配置示值窗GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationIndicateWindowsOneRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_ForceCalibration.indexIndicateWindowsArray[fieldIndex];
	
	g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x = x;
	g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y = y;
	if (!rowIndex)
	{
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor = COLOR_POINT;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor = DARKBLUE;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordBackColor = DARKBLUE;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width = 30;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize = 24;
	}
	else
	{
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor = COLOR_POINT;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor = LGRAY;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordBackColor = LGRAY;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width = 60;
		g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize = 48;
	}
	g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].rowDistance = 0;
	g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].columnDistance = 0;
	g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_WINDOWS_FORCE:
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 400;
			break;
		case OBJECT_WINDOWS_SPEED:
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 402;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationIndicateWindowsRectangleFrameCoordinate
 * Description    : 配置示值窗GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationIndicateWindowsRectangleFrameCoordinate( void )
{
	uint8_t fieldIndex;
	const uint16_t START_X = 0;
	const uint16_t START_Y = 2;
	uint16_t x = START_X;
	uint16_t y = START_Y;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.indicateWindowNum; ++fieldIndex)
	{
		y = START_Y;
		
		ConfigForceCalibrationIndicateWindowsOneRectangleFrameCoordinate(0,fieldIndex,x,y);
		
		y += g_ForceCalibration.oneLevelMenuIndicateWindows[0][fieldIndex].width - \
			 g_ForceCalibration.oneLevelMenuIndicateWindows[0][fieldIndex].lineWidth;
		
		ConfigForceCalibrationIndicateWindowsOneRectangleFrameCoordinate(1,fieldIndex,x,y);
		
		x += g_ForceCalibration.oneLevelMenuIndicateWindows[0][fieldIndex].lenth - \
			g_ForceCalibration.oneLevelMenuIndicateWindows[0][fieldIndex].lineWidth; 
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationOnwRowRectangleFrameCoordinate
 * Description    : 配置界面一行GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationOnwRowRectangleFrameCoordinate( uint8_t rowIndex, uint16_t startX, uint16_t startY )	
{
	uint8_t fieldIndex = 0;
	uint16_t x = startX;
	uint16_t y = startY;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.fieldNum; ++fieldIndex)
	{
		ConfigForceCalibrationOneFieldRectangleFrameCoordinate(rowIndex,fieldIndex,x,y);
		
		x += g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lenth - g_ForceCalibration.oneLevelMenuTable[rowIndex][fieldIndex].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationDrawOneIndicateWindowsRectangleFrame
 * Description    : 画1个示值窗
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationDrawOneIndicateWindowsRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.indicateWindowNum; ++fieldIndex)
	{
		rectangle.x = pMenu[fieldIndex].x;
		rectangle.y = pMenu[fieldIndex].y;
		rectangle.lenth = pMenu[fieldIndex].lenth;
		rectangle.width = pMenu[fieldIndex].width;
		rectangle.lineWidth = pMenu[fieldIndex].lineWidth;
		rectangle.lineColor = pMenu[fieldIndex].pointColor;
		
		GUI_DrawRectangleFrame(&rectangle);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationDrawIndicateWindowsRectangleFrame
 * Description    : 画示值窗
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationDrawIndicateWindowsRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<INDICATE_WINDOWS_ROW_NUMS; ++i)
	{
		GUI_ForceCalibrationDrawOneIndicateWindowsRectangleFrame(g_ForceCalibration.oneLevelMenuIndicateWindows[i]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationIndicateWindowOneField
 * Description    : 表格菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationIndicateWindowOneField( uint8_t rowIndex )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint16_t lenth = 0;
	uint16_t width = 0;
	uint8_t fieldLen = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.indicateWindowNum; ++fieldIndex)
	{		
		x = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x + \
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		y = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y + \
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		
		fieldLen = strlen(g_ForceCalibration.pIndicateWindowsTitleNameArray[fieldIndex]) * \
				   (g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize>>1);
		lenth = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth - \
				2 * g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		width = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width - \
				2 * g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		
		pointColor = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor;
		backColor = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor;
		
		lcd_fill(x,y,lenth,width,backColor);
		
		x = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		y = g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y + \
			g_ForceCalibration.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth + 1;
		
		if (!rowIndex)
		{
			GUI_DispStr24At(x,y,pointColor,backColor,g_ForceCalibration.pIndicateWindowsTitleNameArray[fieldIndex]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationIndicateWindowField
 * Description    : 示值窗菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationIndicateWindowField( void )
{
	uint8_t rowIndex = 0;
	
	for (rowIndex=0; rowIndex<INDICATE_WINDOWS_ROW_NUMS; ++rowIndex)
	{		
		GUI_ForceCalibrationIndicateWindowOneField(rowIndex);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_ForceCalibrationOneIndicateWindowsContent
 * Description    : 显示一个示值窗内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ForceCalibrationOneIndicateWindowsContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_ForceCalibration.oneLevelMenuIndicateWindows[indexRow][indexField].x + \
				 g_ForceCalibration.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint16_t y = g_ForceCalibration.oneLevelMenuIndicateWindows[indexRow][indexField].y + \
				 g_ForceCalibration.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint16_t pointColor = FUCHSIA_RED;//g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].pointColor;
	uint16_t backColor = g_ForceCalibration.oneLevelMenuIndicateWindows[indexRow][indexField].backColor;
//	uint16_t lenth = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lenth - \
//					2 * g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
//	uint16_t width = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].width - \
//					2 * g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint8_t handle = g_ForceCalibration.indexIndicateWindowsArray[indexField];
	float tempf = 0;
	
//	lcd_fill(x,y,lenth,width,backColor);
	y += 6;
	
	switch ( handle )
	{
		case OBJECT_WINDOWS_FORCE:
			switch ( g_ForceCalibration.curChannel )
			{
				case SMPL_FH_NUM:
					tempf = GetInterfaceElementForce();	
					RefreshDynamicForce(x+104,y,pointColor,backColor,tempf);	
					break;
				case SMPL_WY_NUM:
					tempf = GetInterfaceElementDisPlacement();	
					RefreshDynamicDisplacement(x+104,y,pointColor,backColor,tempf);	
					break;
				case SMPL_BX_NUM:
					tempf = GetInterfaceElementDeform();	
					RefreshDynamicDeform(x+104,y,pointColor,backColor,tempf);	
					break;
				default:
					break;
			}
			
				
			break;
		case OBJECT_WINDOWS_SPEED:
			switch ( g_ForceCalibration.curChannel )
			{
				case SMPL_FH_NUM:
					tempf = GetInterfaceElementFHSpeed();	
					break;
				case SMPL_WY_NUM:
					tempf = GetInterfaceElementWYSpeed();	
					break;
				case SMPL_BX_NUM:
					tempf = GetInterfaceElementBXSpeed();	
					break;
				default:
					break;
			}
			
			RefreshDynamicSpeed(x+104,y,pointColor,backColor,tempf);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationRectangleFrameCoordinate( void )
{	
	uint16_t x = 157;
	uint16_t y = 100;
	uint8_t i;
	
	y += 30;
	ConfigForceCalibrationOnwRowRectangleFrameCoordinate(0,x,y);
	
	memcpy(g_ForceCalibration.oneLevelMenuTitle,g_ForceCalibration.oneLevelMenuTable,MAX_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
	
	y += (g_ForceCalibration.oneLevelMenuTitle[0].width - g_ForceCalibration.oneLevelMenuTitle[0].lineWidth);
	
	for (i=0; i<g_ForceCalibration.curPageSampleNum; ++i)
	{
		ConfigForceCalibrationOnwRowRectangleFrameCoordinate(i,x,y);
		y += g_ForceCalibration.oneLevelMenuTable[i][0].width - g_ForceCalibration.oneLevelMenuTable[i][0].lineWidth;
	}
}	

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationDrawOneRowTitleRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationDrawOneRowTitleRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.fieldNum; ++fieldIndex)
	{
		rectangle.x = pMenu[fieldIndex].x;
		rectangle.y = pMenu[fieldIndex].y;
		rectangle.lenth = pMenu[fieldIndex].lenth;
		rectangle.width = pMenu[fieldIndex].width;
		rectangle.lineWidth = pMenu[fieldIndex].lineWidth;
		rectangle.lineColor = pMenu[fieldIndex].pointColor;
		
		GUI_DrawRectangleFrame(&rectangle);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationRectangleFrame( void )
{
	uint8_t i;
	
	GUI_ForceCalibrationDrawOneRowTitleRectangleFrame(g_ForceCalibration.oneLevelMenuTitle);
	
	for (i=0; i<g_ForceCalibration.curPageSampleNum; ++i)
	{
		GUI_ForceCalibrationDrawOneRowTitleRectangleFrame(g_ForceCalibration.oneLevelMenuTable[i]);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationField
 * Description    : 界面字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint8_t fieldLen = 0;
	uint16_t lenth = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.fieldNum; ++fieldIndex)
	{
		fieldLen = strlen(g_ForceCalibration.pParameterNameArray[fieldIndex]) * \
				   (g_ForceCalibration.oneLevelMenuTitle[fieldIndex].fontSize>>1);
		lenth = g_ForceCalibration.oneLevelMenuTitle[fieldIndex].lenth - \
				2 * g_ForceCalibration.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		x = g_ForceCalibration.oneLevelMenuTitle[fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_ForceCalibration.oneLevelMenuTitle[fieldIndex].lineWidth;
		y = g_ForceCalibration.oneLevelMenuTitle[fieldIndex].y + \
			g_ForceCalibration.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		pointColor = g_ForceCalibration.oneLevelMenuTitle[fieldIndex].pointColor;
		backColor = g_ForceCalibration.oneLevelMenuTitle[fieldIndex].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,g_ForceCalibration.pParameterNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationShowFrame
 * Description    : 显示框线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationShowFrame( void )
{
	const uint16_t START_Y = 90;
	const uint16_t LENTH = 2;
	const uint16_t WIDTH = 337;
	
	lcd_fill(0,START_Y,LENTH,WIDTH,COLOR_POINT);
	lcd_fill(LCD_LENTH_X-2,START_Y,LENTH,WIDTH,COLOR_POINT);
}

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationStatusBarOneRectangleFrameCoordinate
 * Description    : 配置状态栏GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationStatusBarOneRectangleFrameCoordinate( uint8_t fieldIndex, uint16_t x, uint16_t y )
{
	uint8_t handle = g_ForceCalibration.indexStatusBarArray[fieldIndex];
	
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].x = x;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].y = y;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].pointColor = COLOR_POINT;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].backColor = LIGHT_GRAY2;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].recordPointColor = COLOR_POINT;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].recordBackColor = LIGHT_GRAY2;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].width = 22;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].fontSize = 16;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].rowDistance = 0;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].columnDistance = 0;
	g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_COMM_STATUS:
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth = 150;
			break;
		case OBJECT_CALIBRATION_STATUS:
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth = 150;
			break;
		case OBJECT_BAR_CODE:
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth = 150;		
			break;
		case OBJECT_TIME:
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth = 254;	
			break;
		case OBJECT_PERIPHERAL_STATUS:
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth = 104;		
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigForceCalibrationStatusBarRectangleFrameCoordinate
 * Description    : 配置状态栏GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationStatusBarRectangleFrameCoordinate( void )
{
	uint8_t fieldIndex;
	const uint16_t START_X = 0;
	const uint16_t START_Y = LCD_WIDTH_Y - 54;
	uint16_t x = START_X;
	uint16_t y = START_Y;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.statusBarNum; ++fieldIndex)
	{
		ConfigForceCalibrationStatusBarOneRectangleFrameCoordinate(fieldIndex,x,y);
		
		x += g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth - g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth; 
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationDrawStatusBarRectangleFrame
 * Description    : 画状态栏框线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationDrawStatusBarRectangleFrame( void )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.statusBarNum; ++fieldIndex)
	{
		rectangle.x = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].x;
		rectangle.y = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].y;
		rectangle.lenth = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth;
		rectangle.width = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].width;
		rectangle.lineWidth = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		rectangle.lineColor = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].pointColor;
		
		GUI_DrawRectangleFrame(&rectangle);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationStatusBarField
 * Description    : 表格菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationStatusBarField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint16_t lenth = 0;
	uint16_t width = 0;
	
	for (fieldIndex=0; fieldIndex<g_ForceCalibration.statusBarNum; ++fieldIndex)
	{		
		x = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].x + \
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		y = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].y + \
			g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		
		lenth = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lenth - \
				2 * g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		width = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].width - \
				2 * g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		
		pointColor = COLOR_BACK;//g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].pointColor;
		backColor = g_ForceCalibration.oneLevelMenuStatusBar[fieldIndex].backColor;
		
		lcd_fill(x,y,lenth,width,backColor);
		
		GUI_DispStr16At(x+2,y+1,pointColor,backColor,g_ForceCalibration.pStatusBarTitleNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageOneStatusBarContent
 * Description    : 显示一个状态栏内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ForceCalibrationOneStatusBarContent( uint8_t indexField )
{
	uint16_t x = g_ForceCalibration.oneLevelMenuStatusBar[indexField].x + \
				 g_ForceCalibration.oneLevelMenuStatusBar[indexField].lineWidth;
	uint16_t y = g_ForceCalibration.oneLevelMenuStatusBar[indexField].y + \
				 g_ForceCalibration.oneLevelMenuStatusBar[indexField].lineWidth;
	uint16_t pointColor = FUCHSIA_RED;//g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].pointColor;
	uint16_t backColor = g_ForceCalibration.oneLevelMenuStatusBar[indexField].backColor;
//	uint16_t lenth = g_mainPage.oneLevelMenuStatusBar[indexField].lenth - \
//					2 * g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
//	uint16_t width = g_mainPage.oneLevelMenuStatusBar[indexField].width - \
//					2 * g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
	uint8_t handle = g_ForceCalibration.indexStatusBarArray[indexField];
	uint8_t fontSize = g_ForceCalibration.oneLevelMenuStatusBar[indexField].fontSize;
	
//	lcd_fill(x,y,lenth,width,backColor);
	
	x += strlen(g_ForceCalibration.pStatusBarTitleNameArray[indexField]) * (fontSize>>1) + 10;
	y += 1;
	
	switch ( handle )
	{
		case OBJECT_COMM_STATUS:	
			RefreshDynamicCommunicationStatus(x,y,pointColor,backColor,GetInterfaceLinkStatus());
			break;
		case OBJECT_CALIBRATION_STATUS:
			RefreshDynamicCalibrationStatus(x,y,pointColor,backColor,GetInterfaceCalibrationStatus());
			break;
		case OBJECT_BAR_CODE:
			RefreshDynamicCode(x,y,pointColor,backColor,GetInterfaceElementCode(g_ForceCalibration.curChannel));		
			break;
		case OBJECT_TIME:
			RefreshDynamicSystemTime(x,y,pointColor,backColor);
			break;
		case OBJECT_PERIPHERAL_STATUS:
			RefreshDynamicUSBIco(x,y,pointColor,backColor,GetInterfaceElementUSBStatus());
			RefreshDynamicEthernetIco(x+24,y,pointColor,backColor,GetInterfaceElementEthernetConnectStatus());
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibrationDrawCalibrationTable
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibrationDrawCalibrationTable( void )
{
	ConfigForceCalibrationRectangleFrameCoordinate();
	GUI_ForceCalibrationRectangleFrame();
	GUI_ForceCalibrationField();
}

/*------------------------------------------------------------
 * Function Name  : GUI_ForceCalibration
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibration( void )
{
	/* 示值窗 */
	ConfigForceCalibrationIndicateWindowsRectangleFrameCoordinate();
	GUI_ForceCalibrationDrawIndicateWindowsRectangleFrame();
	GUI_ForceCalibrationIndicateWindowField();
	
	/* 配置标定表格 */
	GUI_ForceCalibrationDrawCalibrationTable();
	
	/* 中间框线 */
	GUI_ForceCalibrationShowFrame();
	
	/* 状态栏 */
	ConfigForceCalibrationStatusBarRectangleFrameCoordinate();
	GUI_ForceCalibrationDrawStatusBarRectangleFrame();
	GUI_ForceCalibrationStatusBarField();
}

/*------------------------------------------------------------
 * Function Name  : Show_ForceCalibrationOneFieldContent
 * Description    : 显示一个字段内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ForceCalibrationOneFieldContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].x + g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lineWidth;
	uint16_t y = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].y + g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lineWidth;
	uint16_t pointColor = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].pointColor;
	uint16_t backColor = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].backColor;
	uint16_t fontLenth = strlen(g_ForceCalibration.fieldData[indexRow][indexField].parameterData) * \
						(g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].fontSize>>1);
	uint16_t rectLenth = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lenth - \
						(2 * g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lineWidth);
	uint16_t lenth = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lenth -\
					 2 * g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lineWidth;
	uint16_t width = g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].width -\
					 2 * g_ForceCalibration.oneLevelMenuTable[indexRow][indexField].lineWidth;
	
	lcd_fill(x,y,lenth,width,backColor);	
	
	if (g_ForceCalibration.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_ForceCalibration.fieldData[indexRow][indexField].parameterData);
}	

/*------------------------------------------------------------
 * Function Name  : Show_ForceCalibrationTestResultTable
 * Description    : 显示结果表格(rowNum：从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ForceCalibrationTestResultTable( uint8_t rowNum, uint8_t handle )
{
	uint8_t indexField = GetForceCalibationFieldIndex(handle);
	
	if (indexField != 0xff)
	{
		if ((rowNum>=1) && (rowNum<=MAX_ONE_PAGE_SHOW_NUM))
		{
			Show_ForceCalibrationOneFieldContent(rowNum-1,indexField);
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : GetForceCalibrationPageNum
 * Description    : 获取已做试块页号
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetForceCalibrationPageNum( void )
{
	uint8_t serial = 0;
	uint8_t remain = 0;
	
	if (g_ForceCalibration.sumPage == 0)
	{
		return 0;
	}
	
	if (g_CalibrationBody.curCompletePieceSerial == 0)
	{
		return 1;
	}
	
	serial = g_CalibrationBody.curCompletePieceSerial / MAX_ONE_PAGE_SHOW_NUM;
	
	remain = g_CalibrationBody.curCompletePieceSerial % MAX_ONE_PAGE_SHOW_NUM;
	if (remain)
	{
		serial += 1;
	}
	
	return serial;
}

/*------------------------------------------------------------
 * Function Name  : GetCompletePieceSerialInCurrentPageSerial
 * Description    : 获取已做试块编号在当前页编号
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetCompletePieceSerialInCurrentPageSerial( void )
{
	uint8_t serial = 0;
	
	serial = g_CalibrationBody.curCompletePieceSerial % MAX_ONE_PAGE_SHOW_NUM;
	if (serial == 0)
	{
		if (g_CalibrationBody.curCompletePieceSerial)
		{
			serial = g_ForceCalibration.curPageSampleNum;
		}
		else
		{
			serial = 0;
		}
	}
	
	return serial;
}

/*------------------------------------------------------------
 * Function Name  : IsCurrentPointReachToPageTail
 * Description    : 判断当前点是否已到达页尾
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus IsCurrentPointReachToPageTail( void )
{
	if (g_ForceCalibration.curPageSampleNum == 0)
	{
		return NO;
	}
	
	if (GetCompletePieceSerialInCurrentPageSerial() == g_ForceCalibration.curPageSampleNum)
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ForceCalibration
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ForceCalibration( void )
{
	uint8_t indexRow = 0;
	uint8_t sampleNum = 0;
	uint8_t pageNum = 0;
	
	/* 遍历序号 */
	for (indexRow=1; indexRow<=g_ForceCalibration.curPageSampleNum; ++indexRow)
	{
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_SERIAL);
	}
	
	/* 遍历检测点 */
	for (indexRow=1; indexRow<=g_ForceCalibration.curPageSampleNum; ++indexRow)
	{
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_CHECK_POINT);
	}
	
	pageNum = GetForceCalibrationPageNum();
	if (g_ForceCalibration.curPage < pageNum)
	{
		sampleNum = g_ForceCalibration.curPageSampleNum;
	}
	else if (g_ForceCalibration.curPage == pageNum)
	{
		sampleNum = GetCompletePieceSerialInCurrentPageSerial();
	}
	else
	{
		return;
	}
	
	for (indexRow=1; indexRow<=sampleNum; ++indexRow)
	{		
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_FORCE);
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_CODE);
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_ERROR);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_ForceCalibration.refreshShortcut == ENABLE)
	{
		g_ForceCalibration.refreshShortcut = DISABLE;
			
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
		switch ( GetCalibrationStatus() )
		{
			case STATUS_CALIBRATION_IDLE:
				pShortCut->status = SHOW_F1 | SHOW_F4;
				
				if (GetPageName() == FORCE_CALIBRATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[89];	
				}
				else if (GetPageName() == FORCE_VERIFICATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[90];	
				}				
				pShortCut->pContent[3] = pTwoLevelMenu[60];
				break;
			case STATUS_CALIBRATION_PROCESS:
			case STATUS_CALIBRATION_BACKHAUL:
				pShortCut->status = SHOW_F1 | SHOW_F3 | SHOW_F4;
				
				if (GetPageName() == FORCE_CALIBRATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[91];	
				}
				else if (GetPageName() == FORCE_VERIFICATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[92];	
				}
				pShortCut->pContent[2] = pTwoLevelMenu[65];
				pShortCut->pContent[3] = pTwoLevelMenu[47];
				break;
			case STATUS_CALIBRATION_END:
				pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
			
				if (GetPageName() == FORCE_CALIBRATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[89];	
				}
				else if (GetPageName() == FORCE_VERIFICATION_PAGE)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[90];	
				}
				pShortCut->pContent[1] = pTwoLevelMenu[19];
				pShortCut->pContent[2] = pTwoLevelMenu[85];
				pShortCut->pContent[3] = pTwoLevelMenu[60];
				break;
		}
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationPrintCalibrationResult
 * Description    : 打印标定结果
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationPrintCalibrationResult( void )
{
	uint8_t calibrationNum = g_CalibrationBody.curCompletePieceSerial;
	
	if (calibrationNum == 0)
	{
		return;
	}
	
	g_CalibrationResult.channel = g_ForceCalibration.curChannel;
	g_CalibrationResult.calibrationPointNums = calibrationNum;
	g_CalibrationResult.pCheckForce = g_CalibrationBody.checkValue;
	g_CalibrationResult.pRealForce = g_CalibrationBody.realValue;
	g_CalibrationResult.pCode = g_CalibrationBody.code;
	g_CalibrationResult.pDeviation = g_CalibrationBody.deviation;
	
	ShowPrintingShortcutMenu();
	
	PrintForceCalibrationResultTable(&g_CalibrationResult);
	
	g_ForceCalibration.refreshShortcut = ENABLE;
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationKeyF2Process
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationKeyF2Process( void )
{
	if (GetCalibrationStatus() != STATUS_CALIBRATION_END)
	{
		return;
	}
	
	ForceCalibrationPrintCalibrationResult();
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationCheckCalibrationDateIsAvail
 * Description    : 检测标定数据是否有效
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ForceCalibrationCheckCalibrationDateIsAvail( void )
{
	uint8_t i;
	uint8_t calibrationNum = g_CalibrationBody.curCompletePieceSerial;
	
	if (calibrationNum == 0)
	{
		return FAILED;
	}
	
	if (calibrationNum == 1)
	{
		return PASSED;
	}
	
	for (i=0; i<calibrationNum-1; ++i)
	{
		if (abs(g_CalibrationBody.code[i]) > abs(g_CalibrationBody.code[i+1]))
		{
			return FAILED;
		}
	}
	
	return PASSED;
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationChangeCalibrationTableProcess
 * Description    : 修正标定表
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ForceCalibrationChangeCalibrationTableProcess( void )
{
	uint8_t calibrationSegments = 0;
	uint8_t segmentIndex = 0;
	
	if (ForceCalibrationCheckCalibrationDateIsAvail() == FAILED)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[3]);
		
		return FAILED;
	}
	
	calibrationSegments = g_CalibrationBody.curCompletePieceSerial;
	
	calibrationSegments++;	//加上0点
	
	smpl_tab_num_set(g_ForceCalibration.curChannel,calibrationSegments);
	smpl_tab_value_set(g_ForceCalibration.curChannel,0,0);
	smpl_tab_code_set(g_ForceCalibration.curChannel,0,0);
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* 写值 */
		smpl_tab_value_set(g_ForceCalibration.curChannel,segmentIndex+1,g_CalibrationBody.checkValue[segmentIndex]);
		
		/* 写码 */		
		smpl_tab_code_set(g_ForceCalibration.curChannel,segmentIndex+1,g_CalibrationBody.code[segmentIndex]);
	}
	
	prm_save();
	
	return PASSED;
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationChangeCalibrationTableProcess
 * Description    : 修正标定表
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationChangeCalibrationTable( void )
{
	if (g_CalibrationBody.curCompletePieceSerial == 0)
	{
		return;
	}
	
	SetSelectPopWindowsInfomation(POP_PCM_ASKING,1,&pForceCalibrationWarning[2],pSelectMenuCue);
	
	PopWindowsProcessCycle();
	
	if ( GetPopWindowsSelectResult() == YES)
	{
		if (ForceCalibrationChangeCalibrationTableProcess() == PASSED)
		{
			SetLssuedParameter();
			
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[4]);	
		}
	}
	
	g_ForceCalibration.leavePage.flagLeavePage = SET;
	g_ForceCalibration.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationKeyF3Process
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationKeyF3Process( void )
{
	switch ( GetCalibrationStatus() )
	{
		case STATUS_CALIBRATION_PROCESS:
		case STATUS_CALIBRATION_BACKHAUL:
			g_CalibrationBody.flagTakePoint = SET;
			break;
		case STATUS_CALIBRATION_END:
			ForceCalibrationChangeCalibrationTable();
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationKeyF4Process
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationKeyF4Process( void )
{
	switch ( GetCalibrationStatus() )
	{
		case STATUS_CALIBRATION_IDLE:
		case STATUS_CALIBRATION_END:
			SetPage(CALIBRATION_PARAMETER);
			g_ForceCalibration.leavePage.flagLeavePage = SET;
			g_ForceCalibration.leavePage.flagSaveData = SET;
			break;
		case STATUS_CALIBRATION_PROCESS:
		case STATUS_CALIBRATION_BACKHAUL:
			g_CalibrationBody.cancelPoint = SET;
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : PageTurningLeft
 * Description    : 向左翻页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationTurningPageLeft( void )
{
	if (g_ForceCalibration.sumPage > 1)
	{
		if (g_ForceCalibration.curPage)
		{
			g_ForceCalibration.curPage--;
		}
		
		if (!g_ForceCalibration.curPage)
		{
			g_ForceCalibration.curPage = g_ForceCalibration.sumPage;
		}
		
		ReloadForceCalibrationArea();
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageTurningPageRight
 * Description    : 向右翻页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationTurningPageRight( void )
{
	if (g_ForceCalibration.sumPage > 1)
	{
		g_ForceCalibration.curPage++;
		
		if (g_ForceCalibration.curPage > g_ForceCalibration.sumPage)
		{
			g_ForceCalibration.curPage = 1;
		}
		
		ReloadForceCalibrationArea();
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F1:
				switch ( GetCalibrationStatus() )
				{
					case STATUS_CALIBRATION_IDLE:
					case STATUS_CALIBRATION_END:
						g_CalibrationBody.startCalibration = SET;
						break;
					case STATUS_CALIBRATION_PROCESS:
					case STATUS_CALIBRATION_BACKHAUL:
						g_CalibrationBody.endCalibration = SET;
						break;
					default:
						break;
				}
				break;
			case KEY_F2:
//			case KEY_PRINT:
				ForceCalibrationKeyF2Process();
				break;
			case KEY_F3:
				ForceCalibrationKeyF3Process();
				break;
			case KEY_F4:
				ForceCalibrationKeyF4Process();
				break;
			case KEY_LEFT:
				switch ( GetCalibrationStatus() )
				{
					case STATUS_CALIBRATION_IDLE:
					case STATUS_CALIBRATION_END:
						ForceCalibrationTurningPageLeft();
						break;
					case STATUS_CALIBRATION_PROCESS:
					case STATUS_CALIBRATION_BACKHAUL:						
						break;
					default:
						break;
				}
				break;
			case KEY_RIGHT:
				switch ( GetCalibrationStatus() )
				{
					case STATUS_CALIBRATION_IDLE:
					case STATUS_CALIBRATION_END:
						ForceCalibrationTurningPageRight();
						break;
					case STATUS_CALIBRATION_PROCESS:
					case STATUS_CALIBRATION_BACKHAUL:						
						break;
					default:
						break;
				}
				break;
			case KEY_ESC:
				SetPage(SYSTEM_SET);
				g_ForceCalibration.leavePage.flagLeavePage = SET;
				g_ForceCalibration.leavePage.flagSaveData = SET;
				break;
			
			case KEY_FORCE_TARE:		
				if (SendChannelTareCmd(SMPL_FH_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[5]);								
			
					g_ForceCalibration.leavePage.flagLeavePage = SET;
					g_ForceCalibration.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_DISPLACE_TARE:
				if (SendChannelTareCmd(SMPL_WY_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[6]);								
			
					g_ForceCalibration.leavePage.flagLeavePage = SET;
					g_ForceCalibration.leavePage.flagSaveData = RESET;
				}
				break;
			
			case KEY_DEFORMATE_TARE:
				if (SendChannelTareCmd(SMPL_BX_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[7]);								
			
					g_ForceCalibration.leavePage.flagLeavePage = SET;
					g_ForceCalibration.leavePage.flagSaveData = RESET;
				}
				break;	
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationCheckWarn
 * Description    : 检测警告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationCheckWarn( void )
{	
	switch ( GetCalibrationStatus() )
	{
		case STATUS_CALIBRATION_IDLE:
		case STATUS_CALIBRATION_END:
			/* 检测下位机警告 */
			if (CheckPrmWarning() == YES)
			{	
				g_ForceCalibration.leavePage.flagLeavePage = SET;
				g_ForceCalibration.leavePage.flagSaveData = RESET;
			}
			break;
		default:
			break;
	}	
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentCalibrationStatus
 * Description    : 设置动态内容校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentCalibrationStatus( void )
{
	SetInterfaceCalibrationStatus(GetCalibrationStatus());
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentTask
 * Description    : 设置动态内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetForceCalibrationDynamicContentTask( void )
{
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			SetDynamicContentForce(g_ForceCalibration.fhChannelUnit);
			SetDynamicContentFHSpeed(g_ForceCalibration.fhChannelUnit);	
			break;
		case SMPL_WY_NUM:
			SetDynamicContentDispalcement(g_ForceCalibration.wyChannelUnit);
			SetDynamicContentWYSpeed(g_ForceCalibration.wyChannelUnit);
			break;
		case SMPL_BX_NUM:
			SetDynamicContentDeform(g_ForceCalibration.bxChannelUnit);
			SetDynamicContentBXSpeed(g_ForceCalibration.bxChannelUnit);
			break;
		default:
			break;
	}
	SetDynamicContentLinkStatus();
	SetDynamicContentCalibrationStatus();
	SetDynamicContentCode(g_ForceCalibration.curChannel);
	SetDynamicContentUSBStatus();
	SetDynamicContentNETStatus();
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicContent
 * Description    : 刷新动态内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void RefreshForceCalibrationDynamicContent( void )
{
	static uint8_t s_refreshIndex = REFRESH_FORCE;
	const uint8_t endIndex = REFRESH_PERIPHERAL_STATUS;
	uint8_t indexField = 0;
	
	s_refreshIndex++;
	s_refreshIndex %= endIndex + 1;
	
	switch ( s_refreshIndex )
	{
		case REFRESH_FORCE:
			indexField = GetForceCalibationIndicateWindowsFieldIndex(OBJECT_WINDOWS_FORCE);
			Show_ForceCalibrationOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_SPEED:
			indexField = GetForceCalibationIndicateWindowsFieldIndex(OBJECT_WINDOWS_SPEED);
			Show_ForceCalibrationOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_COMMUNICATION_STATUS:
			indexField = GetForceCalibationStatusBarFieldIndex(OBJECT_COMM_STATUS);
			Show_ForceCalibrationOneStatusBarContent(indexField);
			break;
		case REFRESH_CALIBRATION_STATUS:
			indexField = GetForceCalibationStatusBarFieldIndex(OBJECT_CALIBRATION_STATUS);
			Show_ForceCalibrationOneStatusBarContent(indexField);
			break;
		case REFRESH_CODE:
			indexField = GetForceCalibationStatusBarFieldIndex(OBJECT_BAR_CODE);
			Show_ForceCalibrationOneStatusBarContent(indexField);
			break;
		case REFRESH_TIME:
			indexField = GetForceCalibationStatusBarFieldIndex(OBJECT_TIME);
			Show_ForceCalibrationOneStatusBarContent(indexField);
			break;
		case REFRESH_PERIPHERAL_STATUS:
			indexField = GetForceCalibationStatusBarFieldIndex(OBJECT_PERIPHERAL_STATUS);
			Show_ForceCalibrationOneStatusBarContent(indexField);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationAllowRunCalibration
 * Description    : 允许运行校准
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FunctionalState ForceCalibrationAllowRunCalibration( void )
{
	/* 检测机型 */
	if (CheckCurrentModel() == FAILED)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[8]);
		
		return DISABLE;
	}
	
	/* 检测联机状态 */
	if (GetLinkStatus() == LINK_UNLINK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[0]);
		
		return DISABLE;
	}
	
	/* 检测个数 */
	if (g_ForceCalibration.sumSampleNum == 0)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[1]);
		
		return DISABLE;
	}
	
	return ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ClearTestResultArea
 * Description    : 擦除试验结果区域
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ClearTestResultArea( uint16_t color )
{
	lcd_fill(150,100,530,270,color);
}

/*------------------------------------------------------------
 * Function Name  : ReloadForceCalibrationArea
 * Description    : 重新加载标定结果区域
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReloadForceCalibrationArea( void )
{
	ClearTestResultArea(COLOR_BACK);
	
	/* 参数初始化 */
	ForceCalibationInit();
	
	/* 参数配置 */
	ForceCalibationConfig();
	
	/* 获取参数 */
	ForceCalibrationReadParameter();
	
	/* 画GUI框架 */
	GUI_ForceCalibrationDrawCalibrationTable();
	
	/* 遍历 */
	Traverse_ForceCalibration();
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationExecuteStartBody
 * Description    : 校准开始
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationExecuteStartBody( void )
{
	g_CalibrationBody.curCompletePieceSerial = 0;
	g_CalibrationBody.flagCalibrationComplete = RESET;
	g_CalibrationBody.flagTakePoint = RESET;
	g_CalibrationBody.cancelPoint = RESET;
	
	g_ForceCalibration.curPage = 1;
	ReloadForceCalibrationArea();

	g_ForceCalibration.refreshShortcut = ENABLE;
	
	SetCalibrationStatus(STATUS_CALIBRATION_PROCESS);
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationExecuteEndBody
 * Description    : 校准结束
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationExecuteEndBody( void )
{
	if (g_CalibrationBody.curCompletePieceSerial)
	{
		SetCalibrationStatus(STATUS_CALIBRATION_END);
	}
	else
	{
		SetCalibrationStatus(STATUS_CALIBRATION_IDLE);
	}	
	
	g_ForceCalibration.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationWriteForce
 * Description    : 写入力值(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationWriteForce( uint8_t index, float force )
{
	uint8_t fieldIndex = 0;
	uint8_t dotNum = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		if (g_ForceCalibration.fhChannelUnit == FH_UNIT_kN)
		{
			force /= 1000;
		}
		
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		if (fabs(force) > 9999999)
		{
			strcpy(g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData,"--------");
			
			return;
		}
		
		dotNum = g_ForceCalibration.oneLevelMenuTable[index-1][fieldIndex].pointBit;
		
		floattochar(MAX_DATA_BIT,dotNum,force,g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationWriteDisPlacement
 * Description    : 写入位移(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationWriteDisPlacement( uint8_t index, float disPlacement )
{
	uint8_t fieldIndex = 0;
	uint8_t dotNum = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		switch ( g_ForceCalibration.wyChannelUnit )
		{
			case WY_UNIT_MM:
					
				break;
			case WY_UNIT_CM:
				disPlacement /= 10;	
				break;
			case WY_UNIT_DM:
				disPlacement /= 100;	
				break;
			case WY_UNIT_M:
				disPlacement /= 1000;	
				break; 
			default:
				break;
		}
			
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		if (fabs(disPlacement) > 9999999)
		{
			strcpy(g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData,"--------");
			
			return;
		}
		
		dotNum = g_ForceCalibration.oneLevelMenuTable[index-1][fieldIndex].pointBit;
		
		floattochar(MAX_DATA_BIT,dotNum,disPlacement,g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationWriteDeform
 * Description    : 写入变形(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationWriteDeform( uint8_t index, float deform )
{
	uint8_t fieldIndex = 0;
	uint8_t dotNum = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		switch ( g_ForceCalibration.bxChannelUnit )
		{
			case BX_UNIT_MM:
					
				break;
			case BX_UNIT_CM:
				deform /= 10;	
				break;
			case BX_UNIT_DM:
				deform /= 100;	
				break;
			case BX_UNIT_M:
				deform /= 1000;	
				break; 
			default:
				break;
		}
			
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		if (fabs(deform) > 9999999)
		{
			strcpy(g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData,"--------");
			
			return;
		}
		
		dotNum = g_ForceCalibration.oneLevelMenuTable[index-1][fieldIndex].pointBit;
		
		floattochar(MAX_DATA_BIT,dotNum,deform,g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationClearForce
 * Description    : 清除力值(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationClearForce( uint8_t index )
{
	uint8_t fieldIndex = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_FORCE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData[0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationWriteCode
 * Description    : 写入码值(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationWriteCode( uint8_t index, int32_t code )
{
	uint8_t fieldIndex = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{		
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		if (abs(code) > 999999)
		{
			strcpy(g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData,"--------");
			
			return;
		}
		
		numtochar(MAX_DATA_BIT,code,g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationClearCode
 * Description    : 清除码值(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationClearCode( uint8_t index )
{
	uint8_t fieldIndex = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData[0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationWriteDeviation
 * Description    : 写入误差(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationWriteDeviation( uint8_t index, float deviation )
{
	uint8_t fieldIndex = 0;
	uint8_t dotNum = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{		
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_ERROR);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		if (fabs(deviation) > 9999999)
		{
			strcpy(g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData,"--------");
			
			return;
		}
		
		dotNum = g_ForceCalibration.oneLevelMenuTable[index-1][fieldIndex].pointBit;
		
		floattochar(MAX_DATA_BIT,dotNum,deviation,g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData);
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationClearDeviation
 * Description    : 清除误差(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationClearDeviation( uint8_t index )
{
	uint8_t fieldIndex = 0;
	
	if ((index>=1) && (index<=MAX_CALIBRATION_POINT_SEGS))
	{
		fieldIndex = GetForceCalibationFieldIndex(OBJECT_ERROR);
		if (fieldIndex == 0xff)
		{
			return;
		}
		
		g_ForceCalibration.fieldData[index-1][fieldIndex].parameterData[0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationGetDeviation
 * Description    : 获取力值校准界面误差
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float ForceCalibrationGetDeviation( float standardForce, float realForce )
{
	float deviation = 0;
	
	deviation = (realForce - standardForce) / standardForce * 100;
	
	return deviation;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationTakePointCoreCycle
 * Description    : 打点循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationTakePointProcess( void )
{
	float curValue = 0,standardForce = 0;
	int32_t code = 0;
	float deviation = 0;
		
	if (g_CalibrationBody.flagTakePoint == SET)
	{
		g_CalibrationBody.flagTakePoint = RESET;
		
		if (g_CalibrationBody.flagCalibrationComplete == SET)
		{
			return;
		}
		
		g_CalibrationBody.curCompletePieceSerial++;
		
		/* 处理值 */
		curValue = get_smpl_value(g_ForceCalibration.curChannel);		
		g_CalibrationBody.realValue[g_CalibrationBody.curCompletePieceSerial-1] = curValue;
		switch ( g_ForceCalibration.curChannel )
		{
			case SMPL_FH_NUM:
				ForceCalibrationWriteForce(GetCompletePieceSerialInCurrentPageSerial(),curValue);	
				break;
			case SMPL_WY_NUM:
				ForceCalibrationWriteDisPlacement(GetCompletePieceSerialInCurrentPageSerial(),curValue);	
				break;
			case SMPL_BX_NUM:
				ForceCalibrationWriteDeform(GetCompletePieceSerialInCurrentPageSerial(),curValue);	
				break;
			default:
				break;
		}
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_FORCE);
		
		/* 处理力码 */
		code = GetSammpleCode(g_ForceCalibration.curChannel);
		g_CalibrationBody.code[g_CalibrationBody.curCompletePieceSerial-1] = code;
		ForceCalibrationWriteCode(GetCompletePieceSerialInCurrentPageSerial(),code);
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_CODE);
		
		/* 处理误差 */
		standardForce = g_CalibrationBody.checkValue[g_CalibrationBody.curCompletePieceSerial-1];
		deviation = ForceCalibrationGetDeviation(standardForce,curValue);
		g_CalibrationBody.deviation[g_CalibrationBody.curCompletePieceSerial-1] = deviation;
		ForceCalibrationWriteDeviation(GetCompletePieceSerialInCurrentPageSerial(),deviation);
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_ERROR);
		
		/* 到达页尾 */
		if (IsCurrentPointReachToPageTail() == YES)
		{
			if (g_ForceCalibration.curPage != g_ForceCalibration.sumPage)
			{
				g_ForceCalibration.curPage++;
				
				ReloadForceCalibrationArea();
			}
		}
		
		if (g_CalibrationBody.curCompletePieceSerial >= g_ForceCalibration.sumSampleNum)
		{
			g_CalibrationBody.flagCalibrationComplete = SET;
			
			#if 0
				if (GetPageName() == FORCE_CALIBRATION_PAGE)
				{
					SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"校准结束，请卸载！");
				}
				else if (GetPageName() == FORCE_VERIFICATION_PAGE)
				{
					SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"检定结束，请卸载！");
				}
			#endif
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationCancelPointProcess
 * Description    : 撤销点循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationCancelPointProcess( void )
{	
	if(g_CalibrationBody.cancelPoint == SET)
	{
		g_CalibrationBody.cancelPoint = RESET;
		
		if (g_CalibrationBody.curCompletePieceSerial == 0)
		{
			return;
		}
		
		if (g_CalibrationBody.flagCalibrationComplete == SET)
		{
			g_ForceCalibration.refreshShortcut = ENABLE;
			
			g_CalibrationBody.flagCalibrationComplete = RESET;
		}
		
		/* 处理力值 */	
		g_CalibrationBody.realValue[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearForce(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_FORCE);
		
		/* 处理力码 */
		g_CalibrationBody.code[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearCode(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_CODE);
		
		/* 处理误差 */
		g_CalibrationBody.deviation[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearDeviation(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_ERROR);
		
		g_CalibrationBody.curCompletePieceSerial--;
		
		/* 到达页首 */
		if (g_CalibrationBody.curCompletePieceSerial)
		{
			if (GetForceCalibrationPageNum() != g_ForceCalibration.curPage)
			{
				/* 已做试块在上一页的页尾 */
				if (IsCurrentPointReachToPageTail() == YES)
				{
					return;
				}
				
				if (g_ForceCalibration.curPage > 1)
				{
					g_ForceCalibration.curPage--;
					
					ReloadForceCalibrationArea();
				}
			}
		}
	}
}
		
/*------------------------------------------------------------
 * Function Name  : ForceCalibrationTakePointCoreCycle
 * Description    : 打点循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationTakePointCoreCycle( void )
{	
	/* 打点处理 */
	ForceCalibrationTakePointProcess();
	
	/* 撤销一点处理 */
	ForceCalibrationCancelPointProcess();
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationExecuteCoreCycle
 * Description    : 执行核心循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationExecuteCoreCycle( void )
{
	switch ( GetCalibrationStatus() )
	{
		case STATUS_CALIBRATION_IDLE:
			if (g_CalibrationBody.startCalibration == SET)
			{
				g_CalibrationBody.startCalibration = RESET;
				
				if (ForceCalibrationAllowRunCalibration() == ENABLE)
				{
					ForceCalibrationExecuteStartBody();
				}
				else
				{
					g_ForceCalibration.leavePage.flagLeavePage = SET;
					g_ForceCalibration.leavePage.flagSaveData = RESET;
				}
			}
			break;
		case STATUS_CALIBRATION_PROCESS:
		case STATUS_CALIBRATION_BACKHAUL:
			ForceCalibrationTakePointCoreCycle();
			
			if (g_CalibrationBody.endCalibration == SET)
			{
				g_CalibrationBody.endCalibration = RESET;
			
				ForceCalibrationExecuteEndBody();
			}
			break;
		case STATUS_CALIBRATION_END:
			if (g_CalibrationBody.startCalibration == SET)
			{
				SetCalibrationStatus(STATUS_CALIBRATION_IDLE);
			}
			break;
			
		default:			
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationLeavePageCheckCycle( void )
{
	if (g_ForceCalibration.leavePage.flagLeavePage == SET)
	{
		if (g_ForceCalibration.leavePage.flagSaveData == SET)
		{
			memset(&g_CalibrationBody,0x00,sizeof(FORCE_CALIBRATION_BODY_TypeDef));
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationCtrlCoreCycle
 * Description    : 控制核心循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationCtrlCoreCycle( void )
{
	while (!bsp_CheckTimer(MAIN_CYCLE_TIMING));
	
	/* 上位机下发命令(包含采样) */
	PCM_CmdSendCycle();
	
	/* 校准执行 */
	ForceCalibrationExecuteCoreCycle();
	
	/* 系统警告检测 */
	ForceCalibrationCheckWarn();
	
	/* 脱机检测 */
	CheckOfflineCycle();
	
	/* 设置动态内容 */
	SetForceCalibrationDynamicContentTask();
	
	/* 刷新动态内容 */
	RefreshForceCalibrationDynamicContent();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
