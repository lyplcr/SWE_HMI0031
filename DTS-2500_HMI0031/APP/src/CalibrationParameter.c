/**
  ******************************************************************************
  * @file    CalibrationParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-12-6 15:31:46
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   校准参数页面
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "CalibrationParameter.h"
#include "ChannelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_DATA_BIT					8
#define MAX_FIELD_NUM					2

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_SERIAL = 0,
	OBJECT_CHECK_POINT,
}OBJECT_CALIBRATION_PARAMETER_TypeDef;

typedef enum
{
	INDEX_SERIAL = 0,
	INDEX_CHECK_POINT,
}INDEX_CALIBRATION_PARAMETER_TypeDef;

typedef struct
{
	char parameterData[MAX_FIELD_NUM][MAX_DATA_BIT+1];
}CALIBRATION_PARAMETER_FIELD_DATA_TypeDef;

typedef enum
{
	AREA_SEGMENT = 0,
	AREA_TABLE,
}CALIBRATION_PARAMETER_AREA_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_CALIBRATION_SEGS][MAX_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTitle[MAX_FIELD_NUM];
	CALIBRATION_PARAMETER_FIELD_DATA_TypeDef fieldData[MAX_CALIBRATION_SEGS];
	char tableSegments[MAX_DATA_BIT+1];				//标定表分段数
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTable;
	const char *pParameterNameArray[MAX_FIELD_NUM];	
	uint8_t putinNum;	
	ALIGN_TYPE_TypeDef align[MAX_FIELD_NUM];		//对齐方式
	uint8_t indexArray[MAX_FIELD_NUM];				//句柄索引
	BoolStatus isIndexMove;							//索引值移动
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页	
	uint8_t fieldNum;								//字段个数
	FH_UINT_TypeDef fhChannelUnit;					//负荷通道单位
	WY_UINT_TypeDef	wyChannelUnit;					//位移通道单位
	BX_UINT_TypeDef	bxChannelUnit;					//变形通道单位
	SMPL_NAME_TypeDef curChannel;					//当前通道
	CALIBRATION_PARAMETER_AREA_TypeDef area;
	FunctionalState exchangeNewSegment;				//切换新的分段点
}CALIBRATION_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pCalibrationParameterFieldName[] = 
{
	"序号",			//0
	"检测点(kN)",	//1
	"检测点(N)",		//2
	"检测点(mm)",	//3
	"检测点(cm)",	//4
	"检测点(dm)",	//5
	"检测点(m)",		//6
};

const char * const pCalibrationParameterWarn[] = 
{
	"标定点分段数输入范围：1 ~ 10",		//0
	"标定点不能为0！",					//1
	"力值检测点错误(不是递增趋势)！",		//2
	"位移检测点错误(不是递增趋势)！",		//3
	"变形检测点错误(不是递增趋势)！",		//4
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CALIBRATION_PARAMETER_TypeDef g_calibrationParameter;
static CALIBRATION_POINT_TypeDef g_calibrationPoint;

/* Private function prototypes -----------------------------------------------*/
static void CalibrationParameterInit( void );
static void CalibrationParameterConfig( void );
static void CalibrationParameterReadParameter( void );
static void GUI_CalibrationParameter( void );
static void Traverse_CalibrationParameter( void );
static void CalibrationParameterPutinProcess( void );
static void CalibrationParameterMoveIndexProcess( void );
static void CalibrationParameterStatusProcess( void );
static void CalibrationParameterMoveCursorProcess( void );
static void CalibrationParameterShortcutCycleTask( void );
static void CalibrationParameterKeyProcess( void );
static void CalibrationParameterLeavePageCheckCycle( void );
static TestStatus CalibrationParameterCheckDataCycle( TABLE_INDEX_TypeDef *pTableIndex );
static void CalibrationParameterWriteParameter( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadCalibrationParameterPage
 * Description    : 校准参数页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadCalibrationParameterPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	CalibrationParameterInit();
	
	/* 参数配置 */
	CalibrationParameterConfig();
	
	/* 获取参数 */
	CalibrationParameterReadParameter();
	
	/* 画GUI框架 */
	GUI_CalibrationParameter();
	
	/* 遍历 */
	Traverse_CalibrationParameter();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_calibrationParameter.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		CalibrationParameterPutinProcess();
		
		/* 移动索引值 */
		CalibrationParameterMoveIndexProcess();
		
		/* 状态处理 */
		CalibrationParameterStatusProcess();
		
		/* 移动光标 */
		CalibrationParameterMoveCursorProcess();
		
		/* 快捷菜单 */
		CalibrationParameterShortcutCycleTask();
		
		/* 按键处理 */
		CalibrationParameterKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		CalibrationParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterInit( void )
{
	g_calibrationParameter.recordIndex = 0xff;
	g_calibrationParameter.isIndexMove = NO;
	g_calibrationParameter.refreshShortcut = ENABLE;
	g_calibrationParameter.leavePage.flagLeavePage = RESET;
	g_calibrationParameter.leavePage.flagSaveData = RESET;
	
	g_calibrationParameter.fhChannelUnit = GetFH_SmplUnit();
	g_calibrationParameter.wyChannelUnit = GetWY_SmplUnit();
	g_calibrationParameter.bxChannelUnit = GetBX_SmplUnit();
	g_calibrationParameter.curChannel = GetChannelSelectChannel();
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationParameterFieldIndex
 * Description    : 获取字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetCalibrationParameterFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_calibrationParameter.fieldNum; ++i)
	{
		if (g_calibrationParameter.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterConfig( void )
{
	uint8_t i;
	
	/* 标题 */
	switch ( g_calibrationParameter.curChannel )
	{
		case SMPL_FH_NUM:
			g_calibrationParameter.pTitle = "负荷通道 | 标定参数设置";	
			break;
		case SMPL_WY_NUM:
			g_calibrationParameter.pTitle = "位移通道 | 标定参数设置";		
			break;
		case SMPL_BX_NUM:
			g_calibrationParameter.pTitle = "变形通道 | 标定参数设置";		
			break;
		default:
			break;
	}
	
	/* 字段个数 */
	g_calibrationParameter.fieldNum = MAX_FIELD_NUM;
	
	/* 索引 */
	g_calibrationParameter.indexArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
	g_calibrationParameter.indexArray[INDEX_CHECK_POINT] 	= OBJECT_CHECK_POINT;
	
	/* 字段名 */
	g_calibrationParameter.pParameterNameArray[INDEX_SERIAL] 		= pCalibrationParameterFieldName[0];
	switch ( g_calibrationParameter.curChannel )
	{
		case SMPL_FH_NUM:
			if (g_calibrationParameter.fhChannelUnit == FH_UNIT_kN)
			{
				g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationParameterFieldName[1];
			}
			else
			{
				g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationParameterFieldName[2];
			}	
			break;
		case SMPL_WY_NUM:
			switch ( g_calibrationParameter.wyChannelUnit )
			{
				case WY_UNIT_MM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[3];	
					break;
				case WY_UNIT_CM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[4];	
					break;
				case WY_UNIT_DM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[5];	
					break;
				case WY_UNIT_M:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[6];	
					break; 
				default:
					break;
			}	
			break;
		case SMPL_BX_NUM:
			switch ( g_calibrationParameter.bxChannelUnit )
			{
				case BX_UNIT_MM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[3];	
					break;
				case BX_UNIT_CM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[4];	
					break;
				case BX_UNIT_DM:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[5];	
					break;
				case BX_UNIT_M:
					g_calibrationParameter.pParameterNameArray[INDEX_CHECK_POINT] = pCalibrationParameterFieldName[6];	
					break; 
				default:
					break;
			}	
			break;
		default:
			break;
	}
	
	/* 数据对齐 */
	g_calibrationParameter.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_calibrationParameter.align[INDEX_CHECK_POINT] = ALIGN_LEFT;
	
	/* 数据保存类型 */
	for (i=0; i<MAX_CALIBRATION_SEGS; ++i)
	{
		g_calibrationParameter.oneLevelMenu[i][INDEX_SERIAL].saveType 		= TYPE_INT;
		g_calibrationParameter.oneLevelMenu[i][INDEX_CHECK_POINT].saveType 	= TYPE_INT;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationParameterSegments
 * Description    : 获取分段数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetCalibrationParameterSegments( void )
{
	return (uint8_t)ustrtoul(g_calibrationParameter.tableSegments,0,10);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterAccordKeyIndexConvCoordinateIndex
 * Description    : 根据按键索引值转换为坐标索引
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TABLE_INDEX_TypeDef CalibrationParameterAccordKeyIndexConvCoordinateIndex( uint8_t keyIndex )
{
	TABLE_INDEX_TypeDef dataIndex = {0};
	uint8_t rowNum = GetCalibrationParameterSegments();
	
	dataIndex.rowIndex = keyIndex % rowNum;
	
	dataIndex.colIndex = keyIndex / rowNum + 1;	//加1表示加上序号列
	
	return dataIndex;
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterReadParameter( void )
{
	uint8_t calibrationSegments = 0;
	uint8_t segmentIndex = 0,fieldIndex = 0;
	float tempf = 0;
	uint8_t serial = 1;
	
	calibrationSegments = smpl_tab_num_get(g_calibrationParameter.curChannel);
		
	if ((calibrationSegments < 2) || (calibrationSegments > MAX_CALIBRATION_SEGS + 1))
	{
		calibrationSegments = 2;
	}				
	calibrationSegments--;	//去除零点
		
	if (g_calibrationParameter.exchangeNewSegment == DISABLE)	
	{	
		numtochar(MAX_DATA_BIT,calibrationSegments,g_calibrationParameter.tableSegments);
	}
	else	/* 切换分段数 */
	{
		calibrationSegments = GetCalibrationParameterSegments();
	}
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* 读序号 */
		fieldIndex = GetCalibrationParameterFieldIndex(OBJECT_SERIAL);
		numtochar(MAX_DATA_BIT,serial,g_calibrationParameter.fieldData[segmentIndex].parameterData[fieldIndex]);
		serial++;
		
		if (g_calibrationParameter.exchangeNewSegment == DISABLE)
		{
			fieldIndex = GetCalibrationParameterFieldIndex(OBJECT_CHECK_POINT);
			tempf = smpl_tab_value_get(g_calibrationParameter.curChannel,segmentIndex+1);
			
			/* 读力值 */
			switch ( g_calibrationParameter.curChannel )
			{
				case SMPL_FH_NUM:
					if (g_calibrationParameter.fhChannelUnit == FH_UNIT_kN)
					{
						tempf /= 1000;
					}	
					break;
				case SMPL_WY_NUM:
					switch ( g_calibrationParameter.wyChannelUnit )
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
					break;
				case SMPL_BX_NUM:
					switch ( g_calibrationParameter.bxChannelUnit )
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
					break;
				default:
					break;
			}
			
			numtochar(MAX_DATA_BIT,(int32_t)tempf,g_calibrationParameter.fieldData[segmentIndex].parameterData[fieldIndex]);
		}
	}
	
	g_calibrationParameter.exchangeNewSegment = DISABLE;
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterWriteParameter( void )
{
	uint8_t i;
	uint8_t calibrationSegments = 0;
	int32_t tempi32 = 0;
	uint8_t handle = 0;
	
	calibrationSegments = GetCalibrationParameterSegments();
	
	handle = GetCalibrationParameterFieldIndex(OBJECT_CHECK_POINT);
	
	for (i=0; i<calibrationSegments; ++i)
	{
		tempi32 = ustrtoul(g_calibrationParameter.fieldData[i].parameterData[handle],0,10);
		
		switch ( g_calibrationParameter.curChannel )
		{
			case SMPL_FH_NUM:
				if (g_calibrationParameter.fhChannelUnit == FH_UNIT_kN)
				{
					tempi32 *= 1000;
				}	
				break;
			case SMPL_WY_NUM:
				switch ( g_calibrationParameter.wyChannelUnit )
				{
					case WY_UNIT_MM:						
						break;
					case WY_UNIT_CM:
						tempi32 *= 10; 	
						break;
					case WY_UNIT_DM:
						tempi32 *= 100;	
						break;
					case WY_UNIT_M:
						tempi32 *= 1000;	
						break; 
					default:
						break;
				}	
				break;
			case SMPL_BX_NUM:
				switch ( g_calibrationParameter.bxChannelUnit )
				{
					case BX_UNIT_MM:						
						break;
					case BX_UNIT_CM:
						tempi32 *= 10; 	
						break;
					case BX_UNIT_DM:
						tempi32 *= 100;	
						break;
					case BX_UNIT_M:
						tempi32 *= 1000;	
						break; 
					default:
						break;
				}	
				break;
			default:
				break;
		}
		
		g_calibrationPoint.calibrationValue[i] = tempi32;
	}
	
	g_calibrationPoint.calibrationSegments = GetCalibrationParameterSegments();
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationPoint
 * Description    : 获取标定点数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CALIBRATION_POINT_TypeDef *GetCalibrationPoint( void )
{
	return &g_calibrationPoint;
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationParameterOnwFieldRectangleFrameCoordinate
 * Description    : 配置界面一个字段GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationParameterOnwFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_calibrationParameter.indexArray[fieldIndex];
	
	/* 通用属性 */
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].x = x;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].y = y;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].pointColor = COLOR_POINT;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].backColor = COLOR_BACK;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].recordBackColor = COLOR_BACK;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].width = 30;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].fontSize = 24;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].rowDistance = 0;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].columnDistance = 0;
	g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			break;
		case OBJECT_CHECK_POINT:
			g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].lenth = 126;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableOnwRowRectangleFrameCoordinate
 * Description    : 配置界面一行GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationParameterOnwRowRectangleFrameCoordinate( uint8_t rowIndex, uint16_t startX, uint16_t startY )	
{
	uint8_t fieldIndex = 0;
	uint16_t x = startX;
	uint16_t y = startY;
	
	for (fieldIndex=0; fieldIndex<g_calibrationParameter.fieldNum; ++fieldIndex)
	{
		ConfigCalibrationParameterOnwFieldRectangleFrameCoordinate(rowIndex,fieldIndex,x,y);
		
		x += g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].lenth - g_calibrationParameter.oneLevelMenu[rowIndex][fieldIndex].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationParameterOtherFieldRectangleFrameCoordinate
 * Description    : 配置其他字段GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationParameterOtherFieldRectangleFrameCoordinate( uint16_t startX, uint16_t startY )
{
	uint16_t x = startX;
	uint16_t y = startY;
	
	x += g_calibrationParameter.oneLevelMenu[0][0].lineWidth;
	y += g_calibrationParameter.oneLevelMenu[0][0].lineWidth;
	
	x += 96;
	y -= 48;
	g_calibrationParameter.oneLevelMenuTable.x = x;
	g_calibrationParameter.oneLevelMenuTable.y = y;
	g_calibrationParameter.oneLevelMenuTable.pointColor = COLOR_POINT;
	g_calibrationParameter.oneLevelMenuTable.backColor = COLOR_BACK;
	g_calibrationParameter.oneLevelMenuTable.recordPointColor = COLOR_POINT;
	g_calibrationParameter.oneLevelMenuTable.recordBackColor = COLOR_BACK;
	g_calibrationParameter.oneLevelMenuTable.lenth = 102;
	g_calibrationParameter.oneLevelMenuTable.width = 30;
	g_calibrationParameter.oneLevelMenuTable.fontSize = 24;
	g_calibrationParameter.oneLevelMenuTable.rowDistance = 0;
	g_calibrationParameter.oneLevelMenuTable.columnDistance = 0;
	g_calibrationParameter.oneLevelMenuTable.lineWidth = 2;
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationParameterRectangleFrameCoordinate( void )
{	
	const uint16_t startX = 294;
	const uint16_t startY = 100;
	uint16_t x = startX;
	uint16_t y = startY;
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	
	ConfigCalibrationParameterOnwRowRectangleFrameCoordinate(0,x,y);
	
	memcpy(g_calibrationParameter.oneLevelMenuTitle,g_calibrationParameter.oneLevelMenu,MAX_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
	
	y += (g_calibrationParameter.oneLevelMenuTitle[0].width - g_calibrationParameter.oneLevelMenuTitle[0].lineWidth);
	
	for (i=0; i<calibrationSegments; ++i)
	{
		ConfigCalibrationParameterOnwRowRectangleFrameCoordinate(i,x,y);
		y += g_calibrationParameter.oneLevelMenu[i][0].width - g_calibrationParameter.oneLevelMenu[i][0].lineWidth;
	}
	
	ConfigCalibrationParameterOtherFieldRectangleFrameCoordinate(startX,startY);
}

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportDrawOneRowTitleRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameterDrawOneRowTitleRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_calibrationParameter.fieldNum; ++fieldIndex)
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
 * Function Name  : GUI_CalibrationTableDrawOneRowSegmentRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameterDrawOneRowSegmentRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = pMenu->x;
	rectangle.y = pMenu->y;
	rectangle.lenth = pMenu->lenth;
	rectangle.width = pMenu->width;
	rectangle.lineWidth = pMenu->lineWidth;
	rectangle.lineColor = pMenu->pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameterRectangleFrame( void )
{
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	
	GUI_CalibrationParameterDrawOneRowTitleRectangleFrame(g_calibrationParameter.oneLevelMenuTitle);
	
	for (i=0; i<calibrationSegments; ++i)
	{
		GUI_CalibrationParameterDrawOneRowTitleRectangleFrame(g_calibrationParameter.oneLevelMenu[i]);
	}	
	
	GUI_CalibrationParameterDrawOneRowSegmentRectangleFrame(&g_calibrationParameter.oneLevelMenuTable);
}

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTableField
 * Description    : 界面字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameterField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint8_t fieldLen = 0;
	uint16_t lenth = 0;
	
	for (fieldIndex=0; fieldIndex<g_calibrationParameter.fieldNum; ++fieldIndex)
	{
		fieldLen = strlen(g_calibrationParameter.pParameterNameArray[fieldIndex]) * \
				   (g_calibrationParameter.oneLevelMenuTitle[fieldIndex].fontSize>>1);
		lenth = g_calibrationParameter.oneLevelMenuTitle[fieldIndex].lenth - \
				2 * g_calibrationParameter.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		x = g_calibrationParameter.oneLevelMenuTitle[fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_calibrationParameter.oneLevelMenuTitle[fieldIndex].lineWidth;
		y = g_calibrationParameter.oneLevelMenuTitle[fieldIndex].y + \
			g_calibrationParameter.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		pointColor = g_calibrationParameter.oneLevelMenuTitle[fieldIndex].pointColor;
		backColor = g_calibrationParameter.oneLevelMenuTitle[fieldIndex].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationParameter.pParameterNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTableSegmentsField
 * Description    : 界面分段字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameterSegmentsField( void )
{
	const uint16_t x = g_calibrationParameter.oneLevelMenuTable.x - 4 * g_calibrationParameter.oneLevelMenuTable.fontSize;
	const uint16_t y = g_calibrationParameter.oneLevelMenuTable.y + g_calibrationParameter.oneLevelMenuTable.lineWidth + 1;
	const uint16_t pointColor = g_calibrationParameter.oneLevelMenuTable.pointColor;
	const uint16_t backColor = g_calibrationParameter.oneLevelMenuTable.backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"分段数：");
}	

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTable
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_calibrationParameter.pTitle);
	
	ConfigCalibrationParameterRectangleFrameCoordinate();
	
	GUI_CalibrationParameterRectangleFrame();
	
	GUI_CalibrationParameterField();
	
	GUI_CalibrationParameterSegmentsField();
}

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationParameterOneFieldContent
 * Description    : 显示一个字段内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationParameterOneFieldContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_calibrationParameter.oneLevelMenu[indexRow][indexField].x + \
				 g_calibrationParameter.oneLevelMenu[indexRow][indexField].lineWidth + 1;
	uint16_t y = g_calibrationParameter.oneLevelMenu[indexRow][indexField].y + \
				 g_calibrationParameter.oneLevelMenu[indexRow][indexField].lineWidth + 1;
	const uint16_t pointColor = g_calibrationParameter.oneLevelMenu[indexRow][indexField].pointColor;
	const uint16_t backColor = g_calibrationParameter.oneLevelMenu[indexRow][indexField].backColor;
	const uint16_t lenth = g_calibrationParameter.oneLevelMenu[indexRow][indexField].lenth - \
							g_calibrationParameter.oneLevelMenu[indexRow][indexField].lineWidth * 2 - 2;
	const uint16_t width = g_calibrationParameter.oneLevelMenu[indexRow][indexField].width - \
							g_calibrationParameter.oneLevelMenu[indexRow][indexField].lineWidth * 2 - 2;
	const uint16_t fontLenth = strlen(g_calibrationParameter.fieldData[indexRow].parameterData[indexField]) * \
						(g_calibrationParameter.oneLevelMenu[indexRow][indexField].fontSize>>1);
	const uint16_t rectLenth = g_calibrationParameter.oneLevelMenu[indexRow][indexField].lenth - \
						(2 * g_calibrationParameter.oneLevelMenu[indexRow][indexField].lineWidth);
	
	lcd_fill(x,y,lenth,width,backColor);
	
	if (g_calibrationParameter.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationParameter.fieldData[indexRow].parameterData[indexField]);
}	

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationParameterOneRowOneLevelMenuContent
 * Description    : 显示一行参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationParameterOneRowOneLevelMenuContent( uint8_t indexRow )
{
	uint8_t i;
	
	for (i=0; i<g_calibrationParameter.fieldNum; ++i)
	{
		Show_CalibrationParameterOneFieldContent(indexRow,i);
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationParameterOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationParameterOneLevelMenuContent( void )
{
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	
	for (i=0; i<calibrationSegments; ++i)
	{
		Show_CalibrationParameterOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationParameterSegmentsFieldContent
 * Description    : 显示分段数内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationParameterSegmentsFieldContent( void )
{
	uint16_t x = g_calibrationParameter.oneLevelMenuTable.x + g_calibrationParameter.oneLevelMenuTable.lineWidth + 1;
	uint16_t y = g_calibrationParameter.oneLevelMenuTable.y + g_calibrationParameter.oneLevelMenuTable.lineWidth + 1;
	uint16_t pointColor = g_calibrationParameter.oneLevelMenuTable.pointColor;
	uint16_t backColor = g_calibrationParameter.oneLevelMenuTable.backColor;
	uint16_t fontLenth = strlen(g_calibrationParameter.tableSegments) * \
						(g_calibrationParameter.oneLevelMenuTable.fontSize>>1);
	uint16_t rectLenth = g_calibrationParameter.oneLevelMenuTable.lenth - \
						(2 * g_calibrationParameter.oneLevelMenuTable.lineWidth);
	const uint16_t lenth = g_calibrationParameter.oneLevelMenuTable.lenth - \
							g_calibrationParameter.oneLevelMenuTable.lineWidth * 2 - 2;
	const uint16_t width = g_calibrationParameter.oneLevelMenuTable.width - \
							g_calibrationParameter.oneLevelMenuTable.lineWidth * 2 - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	x += ((rectLenth - fontLenth) >> 1);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationParameter.tableSegments);
}

/*------------------------------------------------------------
 * Function Name  : Traverse_CalibrationParameter
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_CalibrationParameter( void )
{
	Show_CalibrationParameterOneLevelMenuContent();
	
	Show_CalibrationParameterSegmentsFieldContent();
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_calibrationParameter.nowIndex;
	TABLE_INDEX_TypeDef tableIndex;
	
	tableIndex = CalibrationParameterAccordKeyIndexConvCoordinateIndex(index);
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	if (g_calibrationParameter.area == AREA_SEGMENT)
	{
		pPutin->skin.backColor = g_calibrationParameter.oneLevelMenuTable.recordBackColor;
	}
	else
	{
		pPutin->skin.backColor = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].recordBackColor;
	}
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	if (g_calibrationParameter.area == AREA_SEGMENT)
	{
		pPutin->x = g_calibrationParameter.oneLevelMenuTable.x +\
					g_calibrationParameter.oneLevelMenuTable.lineWidth + 1;
		pPutin->y = g_calibrationParameter.oneLevelMenuTable.y + \
					g_calibrationParameter.oneLevelMenuTable.lineWidth + 1;
	}
	else
	{
		pPutin->x = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].x +\
					g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth + 1;
		pPutin->y = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].y + \
					g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth + 1;
	}
	pPutin->AllowPutinBit = MAX_DATA_BIT;
	pPutin->FillBit = MAX_DATA_BIT;
	if (g_calibrationParameter.area == AREA_SEGMENT)
	{
		pPutin->putinFrameLenth = g_calibrationParameter.oneLevelMenuTable.lenth -\
								2 * g_calibrationParameter.oneLevelMenuTable.lineWidth - 2;
		pPutin->putinFrameWidth = g_calibrationParameter.oneLevelMenuTable.width - \
								2 * g_calibrationParameter.oneLevelMenuTable.lineWidth - 2;
	}
	else
	{
		pPutin->putinFrameLenth = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lenth -\
								2 * g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth - 2;
		pPutin->putinFrameWidth = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].width - \
								2 * g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth - 2;
	}
	pPutin->PutinNum = &g_calibrationParameter.putinNum;
	if (g_calibrationParameter.area == AREA_SEGMENT)
	{
		pPutin->SaveType = g_calibrationParameter.oneLevelMenuTable.saveType;
		pPutin->FontSize = g_calibrationParameter.oneLevelMenuTable.fontSize;
	}
	else
	{
		pPutin->SaveType = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].saveType;
		pPutin->FontSize = g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].fontSize;
	}
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterSegmentCursorProcess
 * Description    : 分段数光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterSegmentCursorProcess( FunctionalState showCursor )
{
	switch ( showCursor )
	{
		case ENABLE:
			g_calibrationParameter.oneLevelMenuTable.pointColor = COLOR_SELECT_POINT;
			g_calibrationParameter.oneLevelMenuTable.backColor = COLOR_SELECT_BACK;	
			break;
		case DISABLE:
			g_calibrationParameter.oneLevelMenuTable.pointColor = g_calibrationParameter.oneLevelMenuTable.recordPointColor;
			g_calibrationParameter.oneLevelMenuTable.backColor = g_calibrationParameter.oneLevelMenuTable.recordBackColor;
			break;
	}
	
	Show_CalibrationParameterSegmentsFieldContent();
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterableCursorProcess
 * Description    : 表格区光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterableCursorProcess( FunctionalState showCursor, uint8_t index )
{
	TABLE_INDEX_TypeDef nowIndex;	
	
	nowIndex = CalibrationParameterAccordKeyIndexConvCoordinateIndex(index);
	
	switch ( showCursor )
	{
		case ENABLE:
			g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].pointColor = COLOR_SELECT_POINT;
			g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].backColor = COLOR_SELECT_BACK;
			break;
		case DISABLE:
			g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].pointColor \
			= g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].recordPointColor;
			
			g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].backColor \
			= g_calibrationParameter.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].recordBackColor;
			break;
	}
	
	Show_CalibrationParameterOneFieldContent(nowIndex.rowIndex,nowIndex.colIndex);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterMoveIndexProcess( void )
{
	uint32_t keyVal = 0;
	INDEX_MANAGE_TypeDef indexObj;
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	TABLE_INDEX_TypeDef tableIndex;
	FlagStatus enchangeStatus = RESET;
	
	if (IsPressKey() == YES)
	{
		keyVal = GetKeyVal();
		
		if (g_calibrationParameter.area == AREA_SEGMENT)
		{
			if (keyVal == KEY_DOWN)
			{
				enchangeStatus = SET;
				
				/* 清除分段数光标 */
				CalibrationParameterSegmentCursorProcess(DISABLE);
				
				/* 加载数据区光标 */
				g_calibrationParameter.isIndexMove = YES;
				g_calibrationParameter.area = AREA_TABLE;
				g_calibrationParameter.nowIndex = 0;
				g_calibrationParameter.recordIndex = 0xff;
			}
		}
		else
		{
			tableIndex = CalibrationParameterAccordKeyIndexConvCoordinateIndex(g_calibrationParameter.nowIndex);
			
			if ((keyVal==KEY_UP) && (tableIndex.rowIndex==0))	//首行
			{
				enchangeStatus = SET;
				
				/* 清除数据区光标 */
				CalibrationParameterableCursorProcess(DISABLE,g_calibrationParameter.nowIndex);
				
				/* 加载分段数光标 */
				g_calibrationParameter.isIndexMove = YES;
				g_calibrationParameter.area = AREA_SEGMENT;
				g_calibrationParameter.nowIndex = 0;
				g_calibrationParameter.recordIndex = 0xff;
			}
		}
	}
	
	if (enchangeStatus == SET)
	{
		return;
	}
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = calibrationSegments;
	indexObj.colNum = g_calibrationParameter.fieldNum - 1;
	indexObj.sumNum = indexObj.rowNum * indexObj.colNum;
	indexObj.pNowIndex = &g_calibrationParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_calibrationParameter.nowIndex != g_calibrationParameter.recordIndex)
	{		
		g_calibrationParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterStatusProcess( void )
{
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	
	/* 索引值溢出处理 */
	if (g_calibrationParameter.nowIndex >= calibrationSegments * (g_calibrationParameter.fieldNum - 1))
	{
		g_calibrationParameter.nowIndex = 0;
	}
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterMoveCursorProcess( void )
{
	if (g_calibrationParameter.isIndexMove == YES)
	{		
		g_calibrationParameter.isIndexMove = NO;
		
		if (g_calibrationParameter.area == AREA_SEGMENT)
		{
			CalibrationParameterSegmentCursorProcess(ENABLE);	
		}
		else
		{		
			if (g_calibrationParameter.recordIndex != 0xff)
			{
				CalibrationParameterableCursorProcess(DISABLE,g_calibrationParameter.recordIndex);
			}
			
			CalibrationParameterableCursorProcess(ENABLE,g_calibrationParameter.nowIndex);
		}
		
		g_calibrationParameter.recordIndex = g_calibrationParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_calibrationParameter.refreshShortcut == ENABLE)
	{
		g_calibrationParameter.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[84];
		pShortCut->pContent[3] = pTwoLevelMenu[67];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterCheckSegmentPoint
 * Description    : 检测分段数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus CalibrationParameterCheckSegmentPoint( uint8_t segments )
{
	if ((segments<1) || (segments>MAX_CALIBRATION_SEGS))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pCalibrationParameterWarn[0]);
		
		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterIndexUpdate( void )
{
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	uint8_t recordIndex = g_calibrationParameter.nowIndex;
	
	g_calibrationParameter.nowIndex++;
	
	g_calibrationParameter.nowIndex %= (calibrationSegments * (g_calibrationParameter.fieldNum - 1));
	
	if (g_calibrationParameter.nowIndex == recordIndex)
	{
		g_calibrationParameter.recordIndex = 0xff;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterUpdateStatus( void )
{
	CalibrationParameterIndexUpdate();
	
	g_calibrationParameter.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : CheckPointForceAutoFill
 * Description    : 检测点力值自动填充
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CheckPointForceAutoFill( uint8_t calibrationPoint )
{
	uint8_t i = 0;
	uint8_t fieldIndex = 0;
	float averageForce = 0,averageDisplacement = 0,averageDeform = 0;
	float tureMaxForce = 0;
	float maxForce = 0,maxDisplacement = 0,maxDeform = 0;
	float curForce = 0,curDisplacement = 0,curDeform = 0;
	uint8_t cof = 0;
	const uint32_t PointCoef[] 		= {10,20,40,60,80,100};
	const uint32_t PointCoef_1T[] 	= {1,2,3,4,5,6,8,10};
	const uint32_t PointCoef_30T[] 	= {30,50,80,120,150,200,250,300};
	const uint32_t PointCoef_200T[] = {200,400,600,800,1000,1200,1600,2000};
	const uint32_t PointCoef_300T[] = {300,400,800,1200,1600,2000,2500,3000};
	const uint32_t PointCoef_8Point[] = {10,20,30,40,50,60,80,100};
	
	if ((calibrationPoint<1) || (calibrationPoint>MAX_CALIBRATION_SEGS))
	{
		return;
	}
	
	fieldIndex = GetCalibrationParameterFieldIndex(OBJECT_CHECK_POINT);
	
	switch ( g_calibrationParameter.curChannel )
	{
		case SMPL_FH_NUM:
			tureMaxForce = smpl_ctrl_full_p_get(g_calibrationParameter.curChannel);
			
			maxForce = tureMaxForce;
			
			if ( g_calibrationParameter.fhChannelUnit == FH_UNIT_kN )
			{
				maxForce /= 1000;
			}
			averageForce = maxForce / 100;
			
			switch ( calibrationPoint )
			{
				case 6:
					for (i=0; i<calibrationPoint; ++i)
					{
						curForce = (int32_t)(averageForce * PointCoef[i]);
						
						numtochar(MAX_DATA_BIT,(int32_t)curForce,g_calibrationParameter.fieldData[i].parameterData[fieldIndex]);
					}
					break;
					
				case 8:
					for (i=0; i<calibrationPoint; ++i)
					{	
						switch ( (int32_t)tureMaxForce )
						{
							case 10000:
								if ( g_calibrationParameter.fhChannelUnit == FH_UNIT_kN )
								{
									curForce = PointCoef_1T[i];
								}
								else
								{
									curForce = PointCoef_1T[i] * 1000;
								}
								break;
							case 300000:
								if ( g_calibrationParameter.fhChannelUnit == FH_UNIT_kN )
								{
									curForce = PointCoef_30T[i];
								}
								else
								{
									curForce = PointCoef_30T[i] * 1000;
								}
								break;
							case 2000000:
								if ( g_calibrationParameter.fhChannelUnit == FH_UNIT_kN )
								{
									curForce = PointCoef_200T[i];
								}
								else
								{
									curForce = PointCoef_200T[i] * 1000;
								}
								break;
							case 3000000:
								if ( g_calibrationParameter.fhChannelUnit == FH_UNIT_kN )
								{
									curForce = PointCoef_300T[i];
								}
								else
								{
									curForce = PointCoef_300T[i] * 1000;
								}
								break;
							default:
								curForce = (int32_t)(averageForce * PointCoef_8Point[i]);
								break;
						}
						
						numtochar(MAX_DATA_BIT,(int32_t)curForce,g_calibrationParameter.fieldData[i].parameterData[fieldIndex]);
					}
					break;
					
				default:
					for (i=0; i<calibrationPoint; ++i)
					{
						cof += 10;
						curForce = averageForce * cof;
						
						numtochar(MAX_DATA_BIT,(int32_t)curForce,g_calibrationParameter.fieldData[i].parameterData[fieldIndex]);
					}
					break;				
							
			}	
			break;
		case SMPL_WY_NUM:
			maxDisplacement = smpl_ctrl_full_p_get(g_calibrationParameter.curChannel);
			
			switch ( g_calibrationParameter.wyChannelUnit )
			{
				case WY_UNIT_MM:					
					break;
				case WY_UNIT_CM:
					maxDisplacement /= 10;	
					break;
				case WY_UNIT_DM:
					maxDisplacement /= 100;	
					break;
				case WY_UNIT_M:
					maxDisplacement /= 1000;	
					break; 
				default:
					break;
			}
			
			averageDisplacement = (int32_t)(maxDisplacement / calibrationPoint);
			
			for (i=0; i<calibrationPoint; ++i)
			{
				curDisplacement = averageDisplacement * (i + 1);
				
				numtochar(MAX_DATA_BIT,(int32_t)curDisplacement,g_calibrationParameter.fieldData[i].parameterData[fieldIndex]);
			}
			/* 最后一点为最大值 */
			numtochar(MAX_DATA_BIT,(int32_t)maxDisplacement,g_calibrationParameter.fieldData[calibrationPoint-1].parameterData[fieldIndex]);
			break;
		case SMPL_BX_NUM:
			maxDeform = smpl_ctrl_full_p_get(g_calibrationParameter.curChannel);
			
			switch ( g_calibrationParameter.bxChannelUnit )
			{
				case BX_UNIT_MM:					
					break;
				case BX_UNIT_CM:
					maxDeform /= 10;	
					break;
				case BX_UNIT_DM:
					maxDeform /= 100;	
					break;
				case BX_UNIT_M:
					maxDeform /= 1000;	
					break; 
				default:
					break;
			}
			
			averageDeform = (int32_t)(maxDeform / calibrationPoint);
			
			for (i=0; i<calibrationPoint; ++i)
			{
				curDeform = averageDeform * (i + 1);
				
				numtochar(MAX_DATA_BIT,(int32_t)curDeform,g_calibrationParameter.fieldData[i].parameterData[fieldIndex]);
			}	
			/* 最后一点为最大值 */
			numtochar(MAX_DATA_BIT,(int32_t)maxDeform,g_calibrationParameter.fieldData[calibrationPoint-1].parameterData[fieldIndex]);
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterKeyProcess( void )
{
	uint8_t index = g_calibrationParameter.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	TABLE_INDEX_TypeDef tableIndex = CalibrationParameterAccordKeyIndexConvCoordinateIndex(index);
	
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
						if (g_calibrationParameter.area == AREA_SEGMENT)
						{
							/* 校验分段点 */
							if (CalibrationParameterCheckSegmentPoint(*GetPutinIntDataAddr()) == FAILED)
							{
								;
							}
							else
							{
								switch ( g_calibrationParameter.oneLevelMenuTable.saveType )
								{
									case TYPE_INT:
										numtochar(g_calibrationParameter.putinNum,*GetPutinIntDataAddr(),g_calibrationParameter.tableSegments);
										
										/* 自动填充力值 */
										CheckPointForceAutoFill(GetCalibrationParameterSegments());
										
										g_calibrationParameter.exchangeNewSegment = ENABLE;
										break;
									default:
										break;
								}
							}
							g_calibrationParameter.leavePage.flagLeavePage = SET;
							g_calibrationParameter.leavePage.flagSaveData = RESET;
							
							return;	
						}	
						else						
						{
							switch ( g_calibrationParameter.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].saveType )
							{
								case TYPE_INT:
									numtochar(g_calibrationParameter.putinNum,*GetPutinIntDataAddr(),g_calibrationParameter.fieldData[tableIndex.rowIndex].parameterData[tableIndex.colIndex]);
									break;
								default:
									break;
							}

							CalibrationParameterUpdateStatus();
						}
						break;						
					
					default:
						break;
				}
				break;
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_calibrationParameter.leavePage.flagLeavePage = SET;
						g_calibrationParameter.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						if (g_calibrationParameter.area == AREA_SEGMENT)
						{
							g_calibrationParameter.isIndexMove = YES;
						}
						else
						{
							g_calibrationParameter.recordIndex = 0xff;
						}
						g_calibrationParameter.refreshShortcut = ENABLE;
						break;
					
					default:						
						break;
				}
				break;
					
			case KEY_F3:
				SetPage(GetChannelSelectNextPage());
				g_calibrationParameter.leavePage.flagLeavePage = SET;
				g_calibrationParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_calibrationParameter.leavePage.flagLeavePage = SET;
				g_calibrationParameter.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationParameterLeavePageCheckCycle( void )
{
	TABLE_INDEX_TypeDef tableIndex;
	
	if (g_calibrationParameter.leavePage.flagLeavePage == SET)
	{
		if (g_calibrationParameter.leavePage.flagSaveData == SET)
		{
			if (FAILED == CalibrationParameterCheckDataCycle(&tableIndex) )
			{
				SetPage(CALIBRATION_PARAMETER);
				PopWindowsProcessCycle();
				
				return;
			}
			
			CalibrationParameterWriteParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationParameterCheckDataCycle
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus CalibrationParameterCheckDataCycle( TABLE_INDEX_TypeDef *pTableIndex )
{
	uint8_t rowIndex;
	uint8_t calibrationSegments = GetCalibrationParameterSegments();
	uint8_t fieldIndex = 0;
	int32_t sourceCheckPoint = 0,targetCheckPoint = 0;
	
	pTableIndex->rowIndex = 0;
	pTableIndex->colIndex = 0;
	
	if (calibrationSegments < 1)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pCalibrationParameterWarn[1]);
		
		return FAILED;
	}
	
	fieldIndex =  GetCalibrationParameterFieldIndex(OBJECT_CHECK_POINT);
	
	for (rowIndex=0; rowIndex<calibrationSegments-1; ++rowIndex)
	{	
		sourceCheckPoint = (int32_t)ustrtoul(g_calibrationParameter.fieldData[rowIndex].parameterData[fieldIndex],0,10);
		targetCheckPoint = (int32_t)ustrtoul(g_calibrationParameter.fieldData[rowIndex+1].parameterData[fieldIndex],0,10);
		if (abs(sourceCheckPoint) >= abs(targetCheckPoint))
		{
			pTableIndex->rowIndex = rowIndex;
			pTableIndex->colIndex = fieldIndex;
			
			switch ( g_calibrationParameter.curChannel )
			{
				case SMPL_FH_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pCalibrationParameterWarn[2]);	
					break;
				case SMPL_WY_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pCalibrationParameterWarn[3]);	
					break;
				case SMPL_BX_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pCalibrationParameterWarn[4]);	
					break;
				default:
					break;
			}
			
			return FAILED;
		}
	}
	
	return PASSED;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
