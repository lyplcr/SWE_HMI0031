/**
  ******************************************************************************
  * @file    CalibrationTable.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-25 10:21:37
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   �궨��
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
	char tableSegments[MAX_DATA_BIT+1];				//�궨��ֶ���
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTable;
	const char *pParameterNameArray[MAX_FIELD_NUM];
	uint8_t putinNum;	
	ALIGN_TYPE_TypeDef align[MAX_FIELD_NUM];		//���뷽ʽ
	uint8_t indexArray[MAX_FIELD_NUM];				//�������
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ	
	uint8_t fieldNum;								//�ֶθ���
	FH_UINT_TypeDef fhChannelUnit;					//����ͨ����λ
	WY_UINT_TypeDef	wyChannelUnit;					//λ��ͨ����λ
	BX_UINT_TypeDef	bxChannelUnit;					//����ͨ����λ
	SMPL_NAME_TypeDef curChannel;					//��ǰͨ��
	CALIBRATION_TABLE_AREA_TypeDef area;
	FunctionalState exchangeNewSegment;				//�л��µķֶε�
}CALIBRATION_TABLE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pCalibrationTableFieldName[] = 
{
	"���",			//0
	"����(kN)",	//1
	"����(N)",		//2
	"����",			//3
	"����(mm)",	//4
	"����(cm)",	//5
	"����(dm)",	//6
	"����(m)",		//7
	"λ����",		//8
	"������",		//9
};

const char * const ControlParameterWarn[] = 
{
	"�궨��ֶ������뷶Χ��1 ~ 10",		//0
	"�궨�㲻��Ϊ0��",					//1
	"������ֵ����(���ǵ�������)��",		//2	
	"�������(���ǵ�������)��",			//3		
	"����λ��ֵ����",				//4
	"���ǵ������ƣ�",					//5	
	"λ�������(���ǵ�������)��",		//6	
	"�������ֵ����",				//7
	"���ǵ������ƣ�",					//8
	"���������(���ǵ�������)��",		//9
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
 * Description    : �궨��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadCalibrationTablePage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	CalibrationTableInit();
	
	/* �������� */
	CalibrationTableConfig();
	
	/* ��ȡ���� */
	CalibrationTableReadParameter();
	
	/* ��GUI��� */
	GUI_CalibrationTable();
	
	/* ���� */
	Traverse_CalibrationTable();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_calibrationTable.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		CalibrationTablePutinProcess();
		
		/* �ƶ�����ֵ */
		CalibrationTableMoveIndexProcess();
		
		/* ״̬���� */
		CalibrationTableStatusProcess();
		
		/* �ƶ���� */
		CalibrationTableMoveCursorProcess();
		
		/* ��ݲ˵� */
		CalibrationTableShortcutCycleTask();
		
		/* �������� */
		CalibrationTableKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		CalibrationTableLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableInit
 * Description    : ��ʼ��
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
 * Description    : ��ȡ�ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetCalibrationTableFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��ȡ�궨��ֶ���
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
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableConfig( void )
{
	uint8_t i;
	
	/* ���� */
	switch ( g_calibrationTable.curChannel )
	{
		case SMPL_FH_NUM:
			g_calibrationTable.pTitle = "����ͨ���궨��";		
			break;
		case SMPL_WY_NUM:
			g_calibrationTable.pTitle = "λ��ͨ���궨��";		
			break;
		case SMPL_BX_NUM:
			g_calibrationTable.pTitle = "����ͨ���궨��";		
			break;
		default:
			break;
	}
	
	/* �ֶθ��� */
	g_calibrationTable.fieldNum = MAX_FIELD_NUM;
	
	/* ���� */
	g_calibrationTable.indexArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
	g_calibrationTable.indexArray[INDEX_CHECK_POINT] 	= OBJECT_CHECK_POINT;
	g_calibrationTable.indexArray[INDEX_FORCE_CODE] 	= ONJECT_FORCE_CODE;
	
	/* �ֶ��� */
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

	/* ���ݶ��� */
	g_calibrationTable.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_calibrationTable.align[INDEX_CHECK_POINT] = ALIGN_LEFT;
	g_calibrationTable.align[INDEX_FORCE_CODE] 	= ALIGN_LEFT;
	
	/* ���ݱ������� */
	for (i=0; i<MAX_CALIBRATION_SEGS; ++i)
	{
		g_calibrationTable.oneLevelMenu[i][INDEX_SERIAL].saveType 		= TYPE_INT;
		g_calibrationTable.oneLevelMenu[i][INDEX_CHECK_POINT].saveType 	= TYPE_INT;
		g_calibrationTable.oneLevelMenu[i][INDEX_FORCE_CODE].saveType 	= TYPE_INT;
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableAccordKeyIndexConvCoordinateIndex
 * Description    : ���ݰ�������ֵת��Ϊ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TABLE_INDEX_TypeDef CalibrationTableAccordKeyIndexConvCoordinateIndex( uint8_t keyIndex )
{
	TABLE_INDEX_TypeDef dataIndex = {0};
	uint8_t rowNum = GetCalibrationTableSegments();
	
	dataIndex.rowIndex = keyIndex % rowNum;
	
	dataIndex.colIndex = keyIndex / rowNum + 1;	//��1��ʾ���������
	
	return dataIndex;
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationTableReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableReadParameter( void )
{
	uint8_t calibrationSegments = 0;
	uint8_t saveCalibrationSegments = 0;		//�ѱ���ı궨��ֶ���
	uint8_t segmentIndex = 0,fieldIndex = 0;
	float tempf = 0;
	uint8_t serial = 1;
	int32_t code = 0;
	
	calibrationSegments = smpl_tab_num_get(g_calibrationTable.curChannel);
		
	if ((calibrationSegments < 2) || (calibrationSegments > MAX_CALIBRATION_SEGS + 1))
	{
		calibrationSegments = 2;
	}				
	calibrationSegments--;	//ȥ�����
		
	saveCalibrationSegments = calibrationSegments;
	
	if (g_calibrationTable.exchangeNewSegment == DISABLE)	
	{	
		numtochar(MAX_DATA_BIT,calibrationSegments,g_calibrationTable.tableSegments);
	}
	else	/* �л��ֶ��� */
	{
		calibrationSegments = GetCalibrationTableSegments();
		g_calibrationTable.exchangeNewSegment = DISABLE;
	}
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* ����� */
		fieldIndex = GetCalibrationTableFieldIndex(OBJECT_SERIAL);
		numtochar(MAX_DATA_BIT,serial,g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex]);
		serial++;
		
		if (segmentIndex < saveCalibrationSegments)
		{
			/* ����ֵ */
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
			
			/* ����ֵ */
			fieldIndex = GetCalibrationTableFieldIndex(ONJECT_FORCE_CODE);
			code = smpl_tab_code_get(g_calibrationTable.curChannel,segmentIndex+1);
			numtochar(MAX_DATA_BIT,code,g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : CalibrationTableWriteParameter
 * Description    : д����
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
	
	calibrationSegments++;		//�������
	smpl_tab_num_set(g_calibrationTable.curChannel,calibrationSegments);
	smpl_tab_value_set(g_calibrationTable.curChannel,0,0);
	smpl_tab_code_set(g_calibrationTable.curChannel,0,0);
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* д��ֵ */
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
		
		/* д��ֵ */
		fieldIndex = GetCalibrationTableFieldIndex(ONJECT_FORCE_CODE);
		code = (int32_t)ustrtoul(g_calibrationTable.fieldData[segmentIndex].parameterData[fieldIndex],0,10);
		smpl_tab_code_set(g_calibrationTable.curChannel,segmentIndex+1,code);
	}
	
	prm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigCalibrationTableOnwFieldRectangleFrameCoordinate
 * Description    : ���ý���һ���ֶ�GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigCalibrationTableOnwFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_calibrationTable.indexArray[fieldIndex];
	
	/* ͨ������ */
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
 * Description    : ���ý���һ��GUI����
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
 * Description    : ���������ֶ�GUI����
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
 * Description    : ���ý���GUI����
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
 * Description    : ����GUI
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
 * Description    : ����GUI
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
 * Description    : ����GUI
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
 * Description    : �����ֶ�����
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
 * Description    : ����ֶ��ֶ�����
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
	
	GUI_DispStr24At(x,y,pointColor,backColor,"�ֶ�����");
}	

/*------------------------------------------------------------
 * Function Name  : GUI_CalibrationTable
 * Description    : ����GUI
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
 * Description    : ��ʾһ���ֶ�����
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
 * Description    : ��ʾһ�в���
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
 * Description    : ��ʾһ���˵�����
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
 * Description    : ��ʾ�ֶ�������
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
 * Description    : ����
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
 * Description    : ���봦��
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
 * Description    : �ֶ�����괦��
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
 * Description    : �������괦��
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
 * Description    : �����ƶ�����ֵ
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
				
				/* ����ֶ������ */
				CalibrationTableSegmentCursorProcess(DISABLE);
				
				/* ������������� */
				g_calibrationTable.isIndexMove = YES;
				g_calibrationTable.area = AREA_TABLE;
				g_calibrationTable.nowIndex = 0;
				g_calibrationTable.recordIndex = 0xff;
			}
		}
		else
		{
			tableIndex = CalibrationTableAccordKeyIndexConvCoordinateIndex(g_calibrationTable.nowIndex);
			
			if ((keyVal==KEY_UP) && (tableIndex.rowIndex==0))	//����
			{
				enchangeStatus = SET;
				
				/* ������������ */
				CalibrationTableTableCursorProcess(DISABLE,g_calibrationTable.nowIndex);
				
				/* ���طֶ������ */
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
 * Description    : ״̬����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CalibrationTableStatusProcess( void )
{
	uint8_t calibrationSegments = GetCalibrationTableSegments();
	
	/* ����ֵ������� */
	if (g_calibrationTable.nowIndex >= calibrationSegments * (g_calibrationTable.fieldNum - 1))
	{
		g_calibrationTable.nowIndex = 0;
	}
}	

/*------------------------------------------------------------
 * Function Name  : CalibrationTableMoveCursorProcess
 * Description    : �ƶ���괦��
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
 * Description    : ��ݲ˵�����
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
 * Description    : ��������
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
 * Description    : ����ֵ����
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
 * Description    : ����״̬
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
 * Description    : ��������
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
 * Description    : ��������
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
							/* У��ֶε� */
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
 * Description    : �뿪ҳ���
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
 * Description    : �������
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
