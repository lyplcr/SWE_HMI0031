/**
  ******************************************************************************
  * @file    CalibrationTable.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-25 10:21:37
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   标定表
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "CalibrationTable.h"
#include "ControlParameter.h"
#include "ChannelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_CALIBRATION_SEGS			10
#define MAX_DATA_BIT					8
#define MAX_FIELD_NUM					3


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_SERIAL = 0,
	OBJECT_CHECK_POINT,
	ONJECT_FORCE_CODE,
}OBJECT_CALIBRATION_TABLE_TypeDef;

typedef enum
{
	INDEX_SERIAL = 0,
	INDEX_CHECK_POINT,
	INDEX_FORCE_CODE,
}INDEX_CALIBRATION_TABLE_TypeDef;

typedef struct
{
	char parameterData[MAX_FIELD_NUM][MAX_DATA_BIT+1];
}CALIBRATION_TABLE_FIELD_DATA_TypeDef;

typedef enum
{
	AREA_SEGMENT = 0,
	AREA_TABLE,
}CALIBRATION_TABLE_AREA_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_CALIBRATION_SEGS][MAX_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTitle[MAX_FIELD_NUM];
	CALIBRATION_TABLE_FIELD_DATA_TypeDef fieldData[MAX_CALIBRATION_SEGS];
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
	CALIBRATION_TABLE_AREA_TypeDef area;
	FunctionalState exchangeNewSegment;				//切换新的分段点
}CALIBRATION_TABLE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pCalibrationTableFieldName[] = 
{
	"序号",			//0
	"检测点(kN)",	//1
	"检测点(N)",		//2
	"力码",			//3
	"检测点(mm)",	//4
	"检测点(cm)",	//5
	"检测点(dm)",	//6
	"检测点(m)",		//7
	"位移码",		//8
	"变形码",		//9
};

const char * const ControlParameterWarn[] = 
{
	"标定表分段数输入范围：1 ~ 10",		//0
	"标定点不能为0！",					//1
	"检测点力值错误(不是递增趋势)！",		//2	
	"力码错误(不是递增趋势)！",			//3		
	"检测点位移值错误，",				//4
	"不是递增趋势！",					//5	
	"位移码错误(不是递增趋势)！",		//6	
	"检测点变形值错误，",				//7
	"不是递增趋势！",					//8
	"变形码错误(不是递增趋势)！",		//9
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_CALIBRATION_TABLE",zidata = "RAM_CALIBRATION_TABLE"
	static CALIBRATION_TABLE_TypeDef g_calibrationTable;
#pragma arm section


/* Private function prototypes -----------------------------------------------*/
static void CalibrationTableInit( void );
static void CalibrationTableConfig( void );
static void CalibrationTableReadParameter( void );
static void GUI_CalibrationTable( void );
static void Traverse_CalibrationTable( void );
static void CalibrationTablePutinProcess( void );
static void CalibrationTableMoveIndexProcess( void );
static void CalibrationTableStatusProcess( void );
static void CalibrationTableMoveCursorProcess( void );
static void CalibrationTableShortcutCycleTask( void );
static void CalibrationTableKeyProcess( void );
static void CalibrationTableLeavePageCheckCycle( void );
static TestStatus CalibrationTableCheckDataCycle( TABLE_INDEX_TypeDef *pTableIndex );


/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadCalibrationTablePage
 * Description    : 标定表
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadCalibrationTablePage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	CalibrationTableInit();
	
	/* 参数配置 */
	CalibrationTableConfig();
	
	/* 获取参数 */
	CalibrationTableReadParameter();
	
	/* 画GUI框架 */
	GUI_CalibrationTable();
	
	/* 遍历 */
	Traverse_CalibrationTable();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_calibrationTable.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		CalibrationTablePutinProcess();
		
		/* 移动索引值 */
		CalibrationTableMoveIndexProcess();
		
		/* 状态处理 */
		CalibrationTableStatusProcess();
		
		/* 移动光标 */
		CalibrationTableMoveCursorProcess();
		
		/* 快捷菜单 */
		CalibrationTableShortcutCycleTask();
		
		/* 按键处理 */
		CalibrationTableKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		CalibrationTableLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableInit( void )
{
	g_calibrationTable.recordIndex = 0xff;
	g_calibrationTable.isIndexMove = NO;
	g_calibrationTable.refreshShortcut = ENABLE;
	g_calibrationTable.leavePage.flagLeavePage = RESET;
	g_calibrationTable.leavePage.flagSaveData = RESET;
	
	g_calibrationTable.fhChannelUnit = GetFH_SmplUnit();
	g_calibrationTable.wyChannelUnit = GetWY_SmplUnit();
	g_calibrationTable.bxChannelUnit = GetBX_SmplUnit();
	g_calibrationTable.curChannel = GetChannelSelectChannel();
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationTableFieldIndex
 * Description    : 获取字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetCalibrationTableFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_calibrationTable.fieldNum; ++i)
	{
		if (g_calibrationTable.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetCalibrationTableSegments
 * Description    : 获取标定表分段数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetCalibrationTableSegments( void )
{
	return (uint8_t)ustrtoul(g_calibrationTable.tableSegments,0,10);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableConfig( void )
{
	uint8_t i;
	
	/* 标题 */
	switch ( g_calibrationTable.curChannel )
	{
		case SMPL_FH_NUM:
			g_calibrationTable.pTitle = "负荷通道标定表";		
			break;
		case SMPL_WY_NUM:
			g_calibrationTable.pTitle = "位移通道标定表";		
			break;
		case SMPL_BX_NUM:
			g_calibrationTable.pTitle = "变形通道标定表";		
			break;
		default:
			break;
	}
	
	/* 字段个数 */
	g_calibrationTable.fieldNum = MAX_FIELD_NUM;
	
	/* 索引 */
	g_calibrationTable.indexArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
	g_calibrationTable.indexArray[INDEX_CHECK_POINT] 	= OBJECT_CHECK_POINT;
	g_calibrationTable.indexArray[INDEX_FORCE_CODE] 	= ONJECT_FORCE_CODE;
	
	/* 字段名 */
	g_calibrationTable.pParameterNameArray[INDEX_SERIAL] 		= pCalibrationTableFieldName[0];
	switch ( g_calibrationTable.curChannel )
	{
		case SMPL_FH_NUM:
			if (g_calibrationTable.fhChannelUnit == FH_UNIT_kN)
			{
				g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[1];
			}
			else
			{
				g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[2];
			}
			g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[3];	
			break;
		case SMPL_WY_NUM:
			switch ( g_calibrationTable.wyChannelUnit )
			{
				case WY_UNIT_MM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[4];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[8];
					break;
				case WY_UNIT_CM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[5];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[8];	
					break;
				case WY_UNIT_DM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[6];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[8];	
					break;
				case WY_UNIT_M:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[7];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[8];	
					break; 
				default:
					break;
			}	
			break;
		case SMPL_BX_NUM:
			switch ( g_calibrationTable.bxChannelUnit )
			{
				case BX_UNIT_MM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[4];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[9];
					break;
				case BX_UNIT_CM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[5];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[9];	
					break;
				case BX_UNIT_DM:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[6];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[9];	
					break;
				case BX_UNIT_M:
					g_calibrationTable.pParameterNameArray[INDEX_CHECK_POINT] 	= pCalibrationTableFieldName[7];
					g_calibrationTable.pParameterNameArray[INDEX_FORCE_CODE] 	= pCalibrationTableFieldName[9];	
					break; 
				default:
					break;
			}	
			break;
		default:
			break;
	}

	/* 数据对齐 */
	g_calibrationTable.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_calibrationTable.align[INDEX_CHECK_POINT] = ALIGN_LEFT;
	g_calibrationTable.align[INDEX_FORCE_CODE] 	= ALIGN_LEFT;
	
	/* 数据保存类型 */
	for (i=0; i<MAX_CALIBRATION_SEGS; ++i)
	{
		g_calibrationTable.oneLevelMenu[i][INDEX_SERIAL].saveType 		= TYPE_INT;
		g_calibrationTable.oneLevelMenu[i][INDEX_CHECK_POINT].saveType 	= TYPE_INT;
		g_calibrationTable.oneLevelMenu[i][INDEX_FORCE_CODE].saveType 	= TYPE_INT;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableAccordKeyIndexConvCoordinateIndex
 * Description    : 根据按键索引值转换为坐标索引
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TABLE_INDEX_TypeDef CalibrationTableAccordKeyIndexConvCoordinateIndex( uint8_t keyIndex )
{
	TABLE_INDEX_TypeDef dataIndex = {0};
	uint8_t rowNum = GetCalibrationTableSegments();
	
	dataIndex.rowIndex = keyIndex % rowNum;
	
	dataIndex.colIndex = keyIndex / rowNum + 1;	//加1表示加上序号列
	
	return dataIndex;
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationTableReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableReadParameter( void )
{
	uint8_t calibrationSegments = 0;
	uint8_t saveCalibrationSegments = 0;		//已保存的标定表分段数
	uint8_t segmentIndex = 0,fieldIndex = 0;
	float tempf = 0;
	uint8_t serial = 1;
	int32_t code = 0;
	
	calibrationSegments = smpl_tab_num_get(g_calibrationTable.curChannel);
		
	if ((calibrationSegments < 2) || (calibrationSegments > MAX_CALIBRATION_SEGS + 1))
	{
		calibrationSegments = 2;
	}				
	calibrationSegments--;	//去除零点
		
	saveCalibrationSegments = calibrationSegments;
	
	if (g_calibrationTable.exchangeNewSegment == DISABLE)	
	{	
		numtochar(MAX_DATA_BIT,calibrationSegments,g_calibrationTable.tableSegments);
	}
	else	/* 切换分段数 */
	{
		calibrationSegments = GetCalibrationTableSegments();
		g_calibrationTable.exchangeNewSegment = DISABLE;
	}
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* 读序号 */
		fieldIndex = GetCalibrationTableFieldIndex(OBJECT_SERIAL);
		numtochar(MAX_DATA_BIT,serial,g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex]);
		serial++;
		
		if (segmentIndex < saveCalibrationSegments)
		{
			/* 读力值 */
			fieldIndex = GetCalibrationTableFieldIndex(OBJECT_CHECK_POINT);
			tempf = smpl_tab_value_get(g_calibrationTable.curChannel,segmentIndex+1);
			
			switch ( g_calibrationTable.curChannel )
			{
				case SMPL_FH_NUM:
					if (g_calibrationTable.fhChannelUnit == FH_UNIT_kN)
					{
						tempf /= 1000;
					}	
					break;
				case SMPL_WY_NUM:
					switch ( g_calibrationTable.wyChannelUnit )
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
					switch ( g_calibrationTable.bxChannelUnit )
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
			
			numtochar(MAX_DATA_BIT,(int32_t)tempf,g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex]);
			
			/* 读码值 */
			fieldIndex = GetCalibrationTableFieldIndex(ONJECT_FORCE_CODE);
			code = smpl_tab_code_get(g_calibrationTable.curChannel,segmentIndex+1);
			numtochar(MAX_DATA_BIT,code,g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableWriteParameter( void )
{
	uint8_t calibrationSegments = 0;
	uint8_t segmentIndex = 0,fieldIndex = 0;
	int32_t force = 0;
	int32_t code = 0;
	
	calibrationSegments = GetCalibrationTableSegments();
	if ((calibrationSegments<1) || (calibrationSegments>MAX_CALIBRATION_SEGS))
	{
		return;
	}
	
	calibrationSegments++;		//加上零点
	smpl_tab_num_set(g_calibrationTable.curChannel,calibrationSegments);
	smpl_tab_value_set(g_calibrationTable.curChannel,0,0);
	smpl_tab_code_set(g_calibrationTable.curChannel,0,0);
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* 写力值 */
		fieldIndex = GetCalibrationTableFieldIndex(OBJECT_CHECK_POINT);
		force = (int32_t)ustrtoul(g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex],0,10);
		
		switch ( g_calibrationTable.curChannel )
		{
			case SMPL_FH_NUM:
				if (g_calibrationTable.fhChannelUnit == FH_UNIT_kN)
				{
					force *= 1000;
				}	
				break;
			case SMPL_WY_NUM:
				switch ( g_calibrationTable.wyChannelUnit )
				{
					case WY_UNIT_MM:								
						break;
					case WY_UNIT_CM:
						force *= 10;	
						break;
					case WY_UNIT_DM:
						force *= 100;	
						break;
					case WY_UNIT_M:
						force *= 1000;	
						break; 
					default:
						break;
				}	
				break;
			case SMPL_BX_NUM:
				switch ( g_calibrationTable.bxChannelUnit )
				{
					case BX_UNIT_MM:								
						break;
					case BX_UNIT_CM:
						force *= 10;	
						break;
					case BX_UNIT_DM:
						force *= 100;	
						break;
					case BX_UNIT_M:
						force *= 1000;	
						break; 
					default:
						break;
				}	
				break;
			default:
				break;
		}
		
		smpl_tab_value_set(g_calibrationTable.curChannel,segmentIndex+1,force);
		
		/* 写码值 */
		fieldIndex = GetCalibrationTableFieldIndex(ONJECT_FORCE_CODE);
		code = (int32_t)ustrtoul(g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex],0,10);
		smpl_tab_code_set(g_calibrationTable.curChannel,segmentIndex+1,code);
	}
	
	prm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableOnwFieldRectangleFrameCoordinate
 * Description    : 配置界面一个字段GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationTableOnwFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_calibrationTable.indexArray[fieldIndex];
	
	/* 通用属性 */
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].x = x;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].y = y;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].pointColor = COLOR_POINT;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].backColor = COLOR_BACK;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].recordBackColor = COLOR_BACK;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].width = 30;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].fontSize = 24;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].rowDistance = 0;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].columnDistance = 0;
	g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lenth = 54;
			break;
		case OBJECT_CHECK_POINT:
			g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lenth = 126;
			break;
		case ONJECT_FORCE_CODE:
			g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lenth = 102;
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
static void ConfigCalibrationTableOnwRowRectangleFrameCoordinate( uint8_t rowIndex, uint16_t startX, uint16_t startY )	
{
	uint8_t fieldIndex = 0;
	uint16_t x = startX;
	uint16_t y = startY;
	
	for (fieldIndex=0; fieldIndex<g_calibrationTable.fieldNum; ++fieldIndex)
	{
		ConfigCalibrationTableOnwFieldRectangleFrameCoordinate(rowIndex,fieldIndex,x,y);
		
		x += g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lenth - g_calibrationTable.oneLevelMenu[rowIndex][fieldIndex].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableOtherFieldRectangleFrameCoordinate
 * Description    : 配置其他字段GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationTableOtherFieldRectangleFrameCoordinate( uint16_t startX, uint16_t startY )
{
	uint16_t x = startX;
	uint16_t y = startY;
	
	x += g_calibrationTable.oneLevelMenu[0][0].lineWidth;
	y += g_calibrationTable.oneLevelMenu[0][0].lineWidth;
	
	x += 96;
	y -= 48;
	g_calibrationTable.oneLevelMenuTable.x = x;
	g_calibrationTable.oneLevelMenuTable.y = y;
	g_calibrationTable.oneLevelMenuTable.pointColor = COLOR_POINT;
	g_calibrationTable.oneLevelMenuTable.backColor = COLOR_BACK;
	g_calibrationTable.oneLevelMenuTable.recordPointColor = COLOR_POINT;
	g_calibrationTable.oneLevelMenuTable.recordBackColor = COLOR_BACK;
	g_calibrationTable.oneLevelMenuTable.lenth = 102;
	g_calibrationTable.oneLevelMenuTable.width = 30;
	g_calibrationTable.oneLevelMenuTable.fontSize = 24;
	g_calibrationTable.oneLevelMenuTable.rowDistance = 0;
	g_calibrationTable.oneLevelMenuTable.columnDistance = 0;
	g_calibrationTable.oneLevelMenuTable.lineWidth = 2;
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationTableRectangleFrameCoordinate( void )
{	
	const uint16_t startX = 230;
	const uint16_t startY = 100;
	uint16_t x = startX;
	uint16_t y = startY;
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	
	ConfigCalibrationTableOnwRowRectangleFrameCoordinate(0,x,y);
	
	memcpy(g_calibrationTable.oneLevelMenuTitle,g_calibrationTable.oneLevelMenu,MAX_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
	
	y += (g_calibrationTable.oneLevelMenuTitle[0].width - g_calibrationTable.oneLevelMenuTitle[0].lineWidth);
	
	for (i=0; i<calibrationSegments; ++i)
	{
		ConfigCalibrationTableOnwRowRectangleFrameCoordinate(i,x,y);
		y += g_calibrationTable.oneLevelMenu[i][0].width - g_calibrationTable.oneLevelMenu[i][0].lineWidth;
	}
	
	ConfigCalibrationTableOtherFieldRectangleFrameCoordinate(startX,startY);
}

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportDrawOneRowTitleRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationTableDrawOneRowTitleRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_calibrationTable.fieldNum; ++fieldIndex)
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
static void GUI_CalibrationTableDrawOneRowSegmentRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
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
static void GUI_CalibrationTableRectangleFrame( void )
{
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	
	GUI_CalibrationTableDrawOneRowTitleRectangleFrame(g_calibrationTable.oneLevelMenuTitle);
	
	for (i=0; i<calibrationSegments; ++i)
	{
		GUI_CalibrationTableDrawOneRowTitleRectangleFrame(g_calibrationTable.oneLevelMenu[i]);
	}	
	
	GUI_CalibrationTableDrawOneRowSegmentRectangleFrame(&g_calibrationTable.oneLevelMenuTable);
}

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTableField
 * Description    : 界面字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationTableField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint8_t fieldLen = 0;
	uint16_t lenth = 0;
	
	for (fieldIndex=0; fieldIndex<g_calibrationTable.fieldNum; ++fieldIndex)
	{
		fieldLen = strlen(g_calibrationTable.pParameterNameArray[fieldIndex]) * \
				   (g_calibrationTable.oneLevelMenuTitle[fieldIndex].fontSize>>1);
		lenth = g_calibrationTable.oneLevelMenuTitle[fieldIndex].lenth - \
				2 * g_calibrationTable.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		x = g_calibrationTable.oneLevelMenuTitle[fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_calibrationTable.oneLevelMenuTitle[fieldIndex].lineWidth;
		y = g_calibrationTable.oneLevelMenuTitle[fieldIndex].y + \
			g_calibrationTable.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		pointColor = g_calibrationTable.oneLevelMenuTitle[fieldIndex].pointColor;
		backColor = g_calibrationTable.oneLevelMenuTitle[fieldIndex].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationTable.pParameterNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTableSegmentsField
 * Description    : 界面分段字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationTableSegmentsField( void )
{
	const uint16_t x = g_calibrationTable.oneLevelMenuTable.x - 4 * g_calibrationTable.oneLevelMenuTable.fontSize;
	const uint16_t y = g_calibrationTable.oneLevelMenuTable.y + g_calibrationTable.oneLevelMenuTable.lineWidth + 1;
	const uint16_t pointColor = g_calibrationTable.oneLevelMenuTable.pointColor;
	const uint16_t backColor = g_calibrationTable.oneLevelMenuTable.backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"分段数：");
}	

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTable
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_CalibrationTable( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_calibrationTable.pTitle);
	
	ConfigCalibrationTableRectangleFrameCoordinate();
	
	GUI_CalibrationTableRectangleFrame();
	
	GUI_CalibrationTableSegmentsField();
	
	GUI_CalibrationTableField();
}

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationTableOneFieldContent
 * Description    : 显示一个字段内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationTableOneFieldContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_calibrationTable.oneLevelMenu[indexRow][indexField].x + \
				 g_calibrationTable.oneLevelMenu[indexRow][indexField].lineWidth + 1;
	uint16_t y = g_calibrationTable.oneLevelMenu[indexRow][indexField].y + \
				 g_calibrationTable.oneLevelMenu[indexRow][indexField].lineWidth + 1;
	const uint16_t pointColor = g_calibrationTable.oneLevelMenu[indexRow][indexField].pointColor;
	const uint16_t backColor = g_calibrationTable.oneLevelMenu[indexRow][indexField].backColor;
	const uint16_t lenth = g_calibrationTable.oneLevelMenu[indexRow][indexField].lenth - \
							g_calibrationTable.oneLevelMenu[indexRow][indexField].lineWidth * 2 - 2;
	const uint16_t width = g_calibrationTable.oneLevelMenu[indexRow][indexField].width - \
							g_calibrationTable.oneLevelMenu[indexRow][indexField].lineWidth * 2 - 2;
	const uint16_t fontLenth = strlen(g_calibrationTable.fieldData[indexRow].parameterData[indexField]) * \
						(g_calibrationTable.oneLevelMenu[indexRow][indexField].fontSize>>1);
	const uint16_t rectLenth = g_calibrationTable.oneLevelMenu[indexRow][indexField].lenth - \
						(2 * g_calibrationTable.oneLevelMenu[indexRow][indexField].lineWidth);
	
	lcd_fill(x,y,lenth,width,backColor);
	
	if (g_calibrationTable.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationTable.fieldData[indexRow].parameterData[indexField]);
}	

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationTableOneRowOneLevelMenuContent
 * Description    : 显示一行参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationTableOneRowOneLevelMenuContent( uint8_t indexRow )
{
	uint8_t i;
	
	for (i=0; i<g_calibrationTable.fieldNum; ++i)
	{
		Show_CalibrationTableOneFieldContent(indexRow,i);
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationTableOneLevelMenuContent( void )
{
	uint8_t i;
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	
	for (i=0; i<calibrationSegments; ++i)
	{
		Show_CalibrationTableOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_CalibrationTableSegmentsFieldContent
 * Description    : 显示分段数内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_CalibrationTableSegmentsFieldContent( void )
{
	uint16_t x = g_calibrationTable.oneLevelMenuTable.x + g_calibrationTable.oneLevelMenuTable.lineWidth + 1;
	uint16_t y = g_calibrationTable.oneLevelMenuTable.y + g_calibrationTable.oneLevelMenuTable.lineWidth + 1;
	uint16_t pointColor = g_calibrationTable.oneLevelMenuTable.pointColor;
	uint16_t backColor = g_calibrationTable.oneLevelMenuTable.backColor;
	uint16_t fontLenth = strlen(g_calibrationTable.tableSegments) * \
						(g_calibrationTable.oneLevelMenuTable.fontSize>>1);
	uint16_t rectLenth = g_calibrationTable.oneLevelMenuTable.lenth - \
						(2 * g_calibrationTable.oneLevelMenuTable.lineWidth);
	const uint16_t lenth = g_calibrationTable.oneLevelMenuTable.lenth - \
							g_calibrationTable.oneLevelMenuTable.lineWidth * 2 - 2;
	const uint16_t width = g_calibrationTable.oneLevelMenuTable.width - \
							g_calibrationTable.oneLevelMenuTable.lineWidth * 2 - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	x += ((rectLenth - fontLenth) >> 1);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_calibrationTable.tableSegments);
}

/*------------------------------------------------------------
 * Function Name  : Traverse_CalibrationTable
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_CalibrationTable( void )
{
	Show_CalibrationTableOneLevelMenuContent();
	
	Show_CalibrationTableSegmentsFieldContent();
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTablePutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTablePutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_calibrationTable.nowIndex;
	TABLE_INDEX_TypeDef tableIndex;
	
	tableIndex = CalibrationTableAccordKeyIndexConvCoordinateIndex(index);
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	if (g_calibrationTable.area == AREA_SEGMENT)
	{
		pPutin->skin.backColor = g_calibrationTable.oneLevelMenuTable.recordBackColor;
	}
	else
	{
		pPutin->skin.backColor = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].recordBackColor;
	}
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	if (g_calibrationTable.area == AREA_SEGMENT)
	{
		pPutin->x = g_calibrationTable.oneLevelMenuTable.x +\
					g_calibrationTable.oneLevelMenuTable.lineWidth + 1;
		pPutin->y = g_calibrationTable.oneLevelMenuTable.y + \
					g_calibrationTable.oneLevelMenuTable.lineWidth + 1;
	}
	else
	{
		pPutin->x = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].x +\
					g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth + 1;
		pPutin->y = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].y + \
					g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth + 1;
	}
	pPutin->AllowPutinBit = MAX_DATA_BIT;
	pPutin->FillBit = MAX_DATA_BIT;
	if (g_calibrationTable.area == AREA_SEGMENT)
	{
		pPutin->putinFrameLenth = g_calibrationTable.oneLevelMenuTable.lenth -\
								2 * g_calibrationTable.oneLevelMenuTable.lineWidth - 2;
		pPutin->putinFrameWidth = g_calibrationTable.oneLevelMenuTable.width - \
								2 * g_calibrationTable.oneLevelMenuTable.lineWidth - 2;
	}
	else
	{
		pPutin->putinFrameLenth = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lenth -\
								2 * g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth - 2;
		pPutin->putinFrameWidth = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].width - \
								2 * g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].lineWidth - 2;
	}
	pPutin->PutinNum = &g_calibrationTable.putinNum;
	if (g_calibrationTable.area == AREA_SEGMENT)
	{
		pPutin->SaveType = g_calibrationTable.oneLevelMenuTable.saveType;
		pPutin->FontSize = g_calibrationTable.oneLevelMenuTable.fontSize;
	}
	else
	{
		pPutin->SaveType = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].saveType;
		pPutin->FontSize = g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].fontSize;
	}
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableSegmentCursorProcess
 * Description    : 分段数光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableSegmentCursorProcess( FunctionalState showCursor )
{
	switch ( showCursor )
	{
		case ENABLE:
			g_calibrationTable.oneLevelMenuTable.pointColor = COLOR_SELECT_POINT;
			g_calibrationTable.oneLevelMenuTable.backColor = COLOR_SELECT_BACK;	
			break;
		case DISABLE:
			g_calibrationTable.oneLevelMenuTable.pointColor = g_calibrationTable.oneLevelMenuTable.recordPointColor;
			g_calibrationTable.oneLevelMenuTable.backColor = g_calibrationTable.oneLevelMenuTable.recordBackColor;
			break;
	}
	
	Show_CalibrationTableSegmentsFieldContent();
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationTableTableCursorProcess
 * Description    : 表格区光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableTableCursorProcess( FunctionalState showCursor, uint8_t index )
{
	TABLE_INDEX_TypeDef nowIndex;	
	
	nowIndex = CalibrationTableAccordKeyIndexConvCoordinateIndex(index);
	
	switch ( showCursor )
	{
		case ENABLE:
			g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].pointColor = COLOR_SELECT_POINT;
			g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].backColor = COLOR_SELECT_BACK;
			break;
		case DISABLE:
			g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].pointColor \
			= g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].recordPointColor;
			
			g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].backColor \
			= g_calibrationTable.oneLevelMenu[nowIndex.rowIndex][nowIndex.colIndex].recordBackColor;
			break;
	}
	
	Show_CalibrationTableOneFieldContent(nowIndex.rowIndex,nowIndex.colIndex);
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableMoveIndexProcess( void )
{
	uint32_t keyVal = 0;
	INDEX_MANAGE_TypeDef indexObj;
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	TABLE_INDEX_TypeDef tableIndex;
	FlagStatus enchangeStatus = RESET;
	
	if (IsPressKey() == YES)
	{
		keyVal = GetKeyVal();
		
		if (g_calibrationTable.area == AREA_SEGMENT)
		{
			if (keyVal == KEY_DOWN)
			{
				enchangeStatus = SET;
				
				/* 清除分段数光标 */
				CalibrationTableSegmentCursorProcess(DISABLE);
				
				/* 加载数据区光标 */
				g_calibrationTable.isIndexMove = YES;
				g_calibrationTable.area = AREA_TABLE;
				g_calibrationTable.nowIndex = 0;
				g_calibrationTable.recordIndex = 0xff;
			}
		}
		else
		{
			tableIndex = CalibrationTableAccordKeyIndexConvCoordinateIndex(g_calibrationTable.nowIndex);
			
			if ((keyVal==KEY_UP) && (tableIndex.rowIndex==0))	//首行
			{
				enchangeStatus = SET;
				
				/* 清除数据区光标 */
				CalibrationTableTableCursorProcess(DISABLE,g_calibrationTable.nowIndex);
				
				/* 加载分段数光标 */
				g_calibrationTable.isIndexMove = YES;
				g_calibrationTable.area = AREA_SEGMENT;
				g_calibrationTable.nowIndex = 0;
				g_calibrationTable.recordIndex = 0xff;
			}
		}
	}
	
	if (enchangeStatus == SET)
	{
		return;
	}
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = calibrationSegments;
	indexObj.colNum = g_calibrationTable.fieldNum - 1;
	indexObj.sumNum = indexObj.rowNum * indexObj.colNum;
	indexObj.pNowIndex = &g_calibrationTable.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_calibrationTable.nowIndex != g_calibrationTable.recordIndex)
	{		
		g_calibrationTable.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableStatusProcess( void )
{
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	
	/* 索引值溢出处理 */
	if (g_calibrationTable.nowIndex >= calibrationSegments * (g_calibrationTable.fieldNum - 1))
	{
		g_calibrationTable.nowIndex = 0;
	}
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationTableMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableMoveCursorProcess( void )
{
	if (g_calibrationTable.isIndexMove == YES)
	{		
		g_calibrationTable.isIndexMove = NO;
		
		if (g_calibrationTable.area == AREA_SEGMENT)
		{
			CalibrationTableSegmentCursorProcess(ENABLE);	
		}
		else
		{		
			if (g_calibrationTable.recordIndex != 0xff)
			{
				CalibrationTableTableCursorProcess(DISABLE,g_calibrationTable.recordIndex);
			}
			
			CalibrationTableTableCursorProcess(ENABLE,g_calibrationTable.nowIndex);
		}
		
		g_calibrationTable.recordIndex = g_calibrationTable.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_calibrationTable.refreshShortcut == ENABLE)
	{
		g_calibrationTable.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F1 | SHOW_F2| SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[0] = pTwoLevelMenu[4];
		pShortCut->pContent[1] = pTwoLevelMenu[19];
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus CalibrationTableCheckSegmentPoint( uint8_t segments )
{
	if ((segments<1) || (segments>MAX_CALIBRATION_SEGS))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[0]);
		
		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableIndexUpdate( void )
{
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	uint8_t colNum = g_calibrationTable.nowIndex / calibrationSegments + 1;
	uint8_t recordIndex = g_calibrationTable.nowIndex;
	
	if (colNum == GetCalibrationTableFieldIndex(OBJECT_CHECK_POINT))
	{
		g_calibrationTable.nowIndex += calibrationSegments;
	}
	else
	{
		g_calibrationTable.nowIndex -= calibrationSegments;
		g_calibrationTable.nowIndex += 1;
	}
	
	g_calibrationTable.nowIndex %= (calibrationSegments * (g_calibrationTable.fieldNum - 1));
	
	if (g_calibrationTable.nowIndex == recordIndex)
	{
		g_calibrationTable.recordIndex = 0xff;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableUpdateStatus( void )
{
	CalibrationTableIndexUpdate();
	
	g_calibrationTable.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTablePagePrint( void )
{
	ShowPrintingShortcutMenu();
	
	if (PrintCalibrationTable(g_calibrationTable.curChannel,g_calibrationTable.fhChannelUnit) == ERROR)
	{
		g_calibrationTable.leavePage.flagLeavePage = SET;
		g_calibrationTable.leavePage.flagSaveData = RESET;
	}
	else
	{
		g_calibrationTable.refreshShortcut = ENABLE;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableKeyProcess( void )
{
	uint8_t index = g_calibrationTable.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	TABLE_INDEX_TypeDef tableIndex = CalibrationTableAccordKeyIndexConvCoordinateIndex(index);
	
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
						if (g_calibrationTable.area == AREA_SEGMENT)
						{
							/* 校验分段点 */
							if (CalibrationTableCheckSegmentPoint(*GetPutinIntDataAddr()) == FAILED)
							{
								;
							}
							else
							{
								switch ( g_calibrationTable.oneLevelMenuTable.saveType )
								{
									case TYPE_INT:
										numtochar(g_calibrationTable.putinNum,*GetPutinIntDataAddr(),g_calibrationTable.tableSegments);
										g_calibrationTable.exchangeNewSegment = ENABLE;
										break;
									default:
										break;
								}
							}
							g_calibrationTable.leavePage.flagLeavePage = SET;
							g_calibrationTable.leavePage.flagSaveData = RESET;
							
							return;	
						}	
						else						
						{
							switch ( g_calibrationTable.oneLevelMenu[tableIndex.rowIndex][tableIndex.colIndex].saveType )
							{
								case TYPE_INT:
									numtochar(g_calibrationTable.putinNum,*GetPutinIntDataAddr(),g_calibrationTable.fieldData[tableIndex.rowIndex].parameterData[tableIndex.colIndex]);
									break;
								default:
									break;
							}

							CalibrationTableUpdateStatus();
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
						g_calibrationTable.leavePage.flagLeavePage = SET;
						g_calibrationTable.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						if (g_calibrationTable.area == AREA_SEGMENT)
						{
							g_calibrationTable.isIndexMove = YES;
						}
						else
						{
							g_calibrationTable.recordIndex = 0xff;
						}
						g_calibrationTable.refreshShortcut = ENABLE;
						break;
					
					default:						
						break;
				}
				break;
				
			case KEY_F1:
				SetPage(CONTROL_PARAMETER);
				g_calibrationTable.leavePage.flagLeavePage = SET;
				g_calibrationTable.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F2:
//			case KEY_PRINT:
				CalibrationTablePagePrint();
				break;
					
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_calibrationTable.leavePage.flagLeavePage = SET;
				g_calibrationTable.leavePage.flagSaveData = SET;
				break;
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_calibrationTable.leavePage.flagLeavePage = SET;
				g_calibrationTable.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableLeavePageCheckCycle( void )
{
	TABLE_INDEX_TypeDef tableIndex;
	
	if (g_calibrationTable.leavePage.flagLeavePage == SET)
	{
		if (g_calibrationTable.leavePage.flagSaveData == SET)
		{
			if (FAILED == CalibrationTableCheckDataCycle(&tableIndex) )
			{
				SetPage(CALIBRATION_TABLE);
				PopWindowsProcessCycle();
				
				return;
			}
			
			CalibrationTableWriteParameter();
			
			SetLssuedParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableCheckDataCycle
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus CalibrationTableCheckDataCycle( TABLE_INDEX_TypeDef *pTableIndex )
{
	uint8_t rowIndex;
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	uint8_t fieldIndex = 0;
	int32_t sourceCheckPoint = 0,targetCheckPoint = 0;
	
	pTableIndex->rowIndex = 0 ;
	pTableIndex->colIndex = 0;
	
	if (calibrationSegments < 1)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[1]);
		
		return FAILED;
	}
	
	fieldIndex =  GetCalibrationTableFieldIndex(OBJECT_CHECK_POINT);
	
	for (rowIndex=0; rowIndex<calibrationSegments-1; ++rowIndex)
	{	
		sourceCheckPoint = (int32_t)ustrtoul(g_calibrationTable.fieldData[rowIndex].parameterData[fieldIndex],0,10);
		targetCheckPoint = (int32_t)ustrtoul(g_calibrationTable.fieldData[rowIndex+1].parameterData[fieldIndex],0,10);
		if (abs(sourceCheckPoint) >= abs(targetCheckPoint))
		{
			pTableIndex->rowIndex = rowIndex;
			pTableIndex->colIndex = fieldIndex;
			
			switch ( g_calibrationTable.curChannel )
			{
				case SMPL_FH_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[2]);	
					break;
				case SMPL_WY_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParameterWarn[4]);	
					break;
				case SMPL_BX_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,2,&ControlParameterWarn[7]);		
					break;
				default:
					break;
			}
			
			return FAILED;
		}
	}
	
	fieldIndex =  GetCalibrationTableFieldIndex(ONJECT_FORCE_CODE);
	
	for (rowIndex=0; rowIndex<calibrationSegments-1; ++rowIndex)
	{	
		sourceCheckPoint = (int32_t)ustrtoul(g_calibrationTable.fieldData[rowIndex].parameterData[fieldIndex],0,10);
		targetCheckPoint = (int32_t)ustrtoul(g_calibrationTable.fieldData[rowIndex+1].parameterData[fieldIndex],0,10);
		if (abs(sourceCheckPoint) >= abs(targetCheckPoint))
		{
			pTableIndex->rowIndex = rowIndex;
			pTableIndex->colIndex = fieldIndex;
			
			switch ( g_calibrationTable.curChannel )
			{
				case SMPL_FH_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[3]);	
					break;
				case SMPL_WY_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[6]);	
					break;
				case SMPL_BX_NUM:
					SetPopWindowsInfomation(POP_PCM_CUE,1,&ControlParameterWarn[9]);		
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
