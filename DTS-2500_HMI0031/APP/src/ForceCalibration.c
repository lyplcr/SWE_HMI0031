/**
  ******************************************************************************
  * @file    main.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-12-7 08:52:19
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��ֵ�궨ҳ
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

/* ʾֵ�� */
#define INDICATE_WINDOWS_NUMS				2		//ʾֵ������
#define	INDICATE_WINDOWS_ROW_NUMS			2		//ʾֵ������
#define INDICATE_WINDOWS_FORCE_CHAR_NUM		8		
#define INDICATE_WINDOWS_SPEED_CHAR_NUM		6

/* ״̬�� */
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
	uint8_t indicateWindowNum;						//ʾֵ���ֶθ���
	INDICATE_WINDOWS_FIELD_DATA_TypeDef indicateWindowsData[INDICATE_WINDOWS_NUMS];
	
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTitle[MAX_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTable[MAX_CALIBRATION_POINT_SEGS][MAX_FIELD_NUM];
	FORCE_CALIBRATION_FIELD_DATA_TypeDef fieldData[MAX_CALIBRATION_POINT_SEGS][MAX_FIELD_NUM]; 
	const char *pParameterNameArray[MAX_FIELD_NUM];	
	ALIGN_TYPE_TypeDef align[MAX_FIELD_NUM];		//���뷽ʽ
	uint8_t indexFieldArray[MAX_FIELD_NUM];			//�������
	
	uint8_t indexStatusBarArray[STATUS_BAR_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuStatusBar[STATUS_BAR_NUMS];
	const char *pStatusBarTitleNameArray[STATUS_BAR_NUMS];
	uint8_t statusBarNum;							//״̬������
	
	const char * pTitle;							//����
	uint8_t curPageSampleNum;						//��ǰҳ�Կ����
	uint8_t sumSampleNum;							//ȫ���Կ����
	uint8_t curPage;								//��ǰҳ��
	uint8_t sumPage;								//��ҳ��
	uint8_t fieldNum;								//�ֶθ���
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ	
	FH_UINT_TypeDef fhChannelUnit;					//����ͨ����λ
	WY_UINT_TypeDef	wyChannelUnit;					//λ��ͨ����λ
	BX_UINT_TypeDef	bxChannelUnit;					//����ͨ����λ
	SMPL_NAME_TypeDef curChannel;					//��ǰͨ��
}FORCE_CALIBRATION_TypeDef;

typedef struct
{
	FlagStatus startCalibration;					//��ʼ��־
	FlagStatus endCalibration;						//������־
	uint8_t curCompletePieceSerial;					//������Կ���
	FlagStatus flagCalibrationComplete;				//У׼��ɱ�־
	FlagStatus flagTakePoint;						//���
	FlagStatus cancelPoint;							//����
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
	"���",			//0
	"����(kN)",	//1
	"����(N)",		//2
	"��ֵ(kN)",		//3
	"��ֵ(N)",		//4
	"����",			//5
	"���(%)",		//6
	"����(mm)",	//7
	"����(cm)",	//8
	"����(dm)",	//9
	"����(m)",		//10
	"λ��ֵ(mm)",	//11
	"λ��ֵ(cm)",	//12
	"λ��ֵ(dm)",	//13
	"λ��ֵ(m)",		//14
	"����ֵ(mm)",	//15
	"����ֵ(cm)",	//16
	"����ֵ(dm)",	//17
	"����ֵ(m)",		//18
	"λ����",		//19
	"������",		//20
};

const char * const pForceCalibrationIndecateWindowsTitleName[] = 
{
	"��ֵ(kN)",		//0
	"��ֵ(N)",		//1
	"�ٶ�(kN/s)",	//2
	"�ٶ�(N/s)",	//3
	"λ��(mm)",		//4
	"λ��(cm)",		//5
	"λ��(dm)",		//6
	"λ��(m)",		//7
	"����(mm)",		//8
	"����(cm)",		//9
	"����(dm)",		//10
	"����(m)",		//11
	"�ٶ�(mm/s)",	//12
	"�ٶ�(cm/s)",	//13
	"�ٶ�(dm/s)",	//14
	"�ٶ�(m/s)",	//15
};

const char * const pCalibrationStatusBarTitleName[] = 
{
	"ͨѶ״̬��",	//0
	"�궨״̬��",	//1
	"��ֵ��",		//2
	"ʱ�䣺",		//3
	"",				//4
};

const char * const pForceCalibrationWarning[] = 
{
	"��ǰ�����ѻ�״̬��",			//0
	"�궨���������Ϊ0��",			//1
	"�Ƿ�д��궨��",				//2
	"�궨���ݲ��ǵ������ƣ�",		//3
	"�궨���Ѹ��£�",				//4
	"������ֵ��������ʧ�ܣ�",		//5
	"����λ����������ʧ�ܣ�",		//6
	"���ͱ�����������ʧ�ܣ�",		//7
	"ϵͳ��֧�ֵ�ǰ���õĻ��ͣ�",		//8
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
 * Description    : ��ֵ�궨ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadForceCalibationPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	ForceCalibationInit();
	
	/* �������� */
	ForceCalibationConfig();
	
	/* ��ȡ���� */
	ForceCalibrationReadParameter();
	
	/* ��GUI��� */
	GUI_ForceCalibration();
	
	/* ���� */
	Traverse_ForceCalibration();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_ForceCalibration.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ��ݲ˵� */
		ForceCalibrationShortcutCycleTask();
		
		/* �������� */
		ForceCalibrationKeyProcess();
		
		/* ���ƺ��� */
		ForceCalibrationCtrlCoreCycle();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		ForceCalibrationLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibationInit
 * Description    : ��ʼ��
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
 * Description    : ��ȡ�ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��ȡʾֵ���ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationIndicateWindowsFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��ȡ״̬���ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetForceCalibationStatusBarFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��ȡУ׼״̬
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
 * Description    : ����У׼״̬
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
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibationConfig( void )
{
	uint8_t i;
	
	/* ���� */
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			if (GetPageName() == FORCE_CALIBRATION_PAGE)
			{
				g_ForceCalibration.pTitle = "��ֵУ׼";
			}
			else
			{
				g_ForceCalibration.pTitle = "��ֵ�춨";
			}

			/* �ֶ��� */
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
				g_ForceCalibration.pTitle = "λ��У׼";
			}
			else
			{
				g_ForceCalibration.pTitle = "λ�Ƽ춨";
			}

			/* �ֶ��� */
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
				g_ForceCalibration.pTitle = "����У׼";
			}
			else
			{
				g_ForceCalibration.pTitle = "���μ춨";
			}

			/* �ֶ��� */
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
	
	/* ʾֵ������ */
	g_ForceCalibration.indexIndicateWindowsArray[INDEX_WINDOWS_FORCE] 		= OBJECT_WINDOWS_FORCE;
	g_ForceCalibration.indexIndicateWindowsArray[INDEX_WINDOWS_SPEED] 		= OBJECT_WINDOWS_SPEED;
	
	/* �ֶ�����ֵ */
	g_ForceCalibration.indexFieldArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
	g_ForceCalibration.indexFieldArray[INDEX_CHECK_POINT] 	= OBJECT_CHECK_POINT;
	g_ForceCalibration.indexFieldArray[INDEX_FORCE] 		= OBJECT_FORCE;
	g_ForceCalibration.indexFieldArray[INDEX_CODE] 			= OBJECT_CODE;
	g_ForceCalibration.indexFieldArray[INDEX_ERROR] 		= OBJECT_ERROR;
	
	/* ״̬������ */
	g_ForceCalibration.indexStatusBarArray[INDEX_COMM_STATUS] 			= OBJECT_COMM_STATUS;
	g_ForceCalibration.indexStatusBarArray[INDEX_CALIBRATION_STATUS] 	= OBJECT_CALIBRATION_STATUS;
	g_ForceCalibration.indexStatusBarArray[INDEX_BAR_CODE] 				= OBJECT_BAR_CODE;
	g_ForceCalibration.indexStatusBarArray[INDEX_TIME] 					= OBJECT_TIME;
	g_ForceCalibration.indexStatusBarArray[INDEX_STATUS] 				= OBJECT_PERIPHERAL_STATUS;
	
	/* ʾֵ�������� */
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
		
	/* ״̬������ */
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_COMM_STATUS] 			= pCalibrationStatusBarTitleName[0];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_CALIBRATION_STATUS] 	= pCalibrationStatusBarTitleName[1];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_BAR_CODE] 			= pCalibrationStatusBarTitleName[2];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_TIME] 				= pCalibrationStatusBarTitleName[3];
	g_ForceCalibration.pStatusBarTitleNameArray[INDEX_STATUS] 				= pCalibrationStatusBarTitleName[4];
	
	/* �ֶθ��� */
	g_ForceCalibration.fieldNum = MAX_FIELD_NUM;
	g_ForceCalibration.indicateWindowNum = INDICATE_WINDOWS_NUMS;	
	g_ForceCalibration.statusBarNum = STATUS_BAR_NUMS;
	
	/* С����λ�� */
	for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
	{
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_SERIAL].pointBit 		= 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_CHECK_POINT].pointBit = 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_FORCE].pointBit 		= 2;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_CODE].pointBit 		= 0;
		g_ForceCalibration.oneLevelMenuTable[i][INDEX_ERROR].pointBit 		= 2;
	}
	
	/* ���ݶ��� */
	g_ForceCalibration.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_ForceCalibration.align[INDEX_CHECK_POINT] = ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_FORCE] 		= ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_CODE] 		= ALIGN_LEFT;
	g_ForceCalibration.align[INDEX_ERROR] 		= ALIGN_LEFT;
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationReadParameter
 * Description    : ������
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

	/* ��У׼���������ȡԭʼ���� */
	for (rowIndex=0; rowIndex<pCalibrationPoint->calibrationSegments; ++rowIndex)
	{
		g_CalibrationBody.serial[rowIndex] = rowIndex + 1;
		
		g_CalibrationBody.checkValue[rowIndex] = pCalibrationPoint->calibrationValue[rowIndex];
	}
	
	/* �ܸ��� */
	g_ForceCalibration.sumSampleNum = pCalibrationPoint->calibrationSegments;
	
	if (g_ForceCalibration.sumSampleNum == 0)
	{
		return;
	}
	
	/* ��ҳ�� */
	remain = g_ForceCalibration.sumSampleNum % MAX_ONE_PAGE_SHOW_NUM;
	if (remain)
	{
		g_ForceCalibration.sumPage = g_ForceCalibration.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM + 1;
	}
	else
	{
		g_ForceCalibration.sumPage = g_ForceCalibration.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM;
	}	
	
	/* ��ǰҳ */
	if ((g_ForceCalibration.curPage==0) || (g_ForceCalibration.curPage>g_ForceCalibration.sumPage))
	{
		g_ForceCalibration.curPage = 1;
	}
	
	/* ��ǰҳ���� */
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
	
	/* ������ */
	baseIndex = (g_ForceCalibration.curPage - 1) * MAX_ONE_PAGE_SHOW_NUM;
	
	/* ��� */
	fieldIndex = GetForceCalibationFieldIndex(OBJECT_SERIAL);
	for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
	{
		tempu = g_CalibrationBody.serial[baseIndex+rowIndex];
		
		numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
	}
	
	switch ( g_ForceCalibration.curChannel )
	{
		case SMPL_FH_NUM:
			/* ���� */
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
			
			/* ��ֵ */
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
			
			/* ���� */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				tempu = g_CalibrationBody.code[baseIndex+rowIndex];
				
				numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			break;
		case SMPL_WY_NUM:
			/* ���� */
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
		
			/* λ��ֵ */
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
			
			/* λ���� */
			fieldIndex = GetForceCalibationFieldIndex(OBJECT_CODE);
			for (rowIndex=0; rowIndex<g_ForceCalibration.curPageSampleNum; ++rowIndex)
			{
				tempu = g_CalibrationBody.code[baseIndex+rowIndex];
				
				numtochar(MAX_DATA_BIT,tempu,g_ForceCalibration.fieldData[rowIndex][fieldIndex].parameterData);
			}
			break;
		case SMPL_BX_NUM:
			/* ���� */
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
			
			/* ����ֵ */
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
			
			/* ������ */
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
	
	/* ��� */
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
 * Description    : ���ý���һ���ֶ�GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigForceCalibrationOneFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_ForceCalibration.indexFieldArray[fieldIndex];
	
	/* ͨ������ */
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
 * Description    : ����ʾֵ��GUI����
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
 * Description    : ����ʾֵ��GUI����
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
 * Description    : ���ý���һ��GUI����
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
 * Description    : ��1��ʾֵ��
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
 * Description    : ��ʾֵ��
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
 * Description    : ���˵��ֶ�
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
 * Description    : ʾֵ���˵��ֶ�
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
 * Description    : ��ʾһ��ʾֵ������
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
 * Description    : ���ý���GUI����
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
 * Description    : ����GUI
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
 * Description    : ����GUI
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
 * Description    : �����ֶ�����
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
 * Description    : ��ʾ����
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
 * Description    : ����״̬��GUI����
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
 * Description    : ����״̬��GUI����
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
 * Description    : ��״̬������
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
 * Description    : ���˵��ֶ�
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
 * Description    : ��ʾһ��״̬������
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
 * Description    : ����GUI
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
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ForceCalibration( void )
{
	/* ʾֵ�� */
	ConfigForceCalibrationIndicateWindowsRectangleFrameCoordinate();
	GUI_ForceCalibrationDrawIndicateWindowsRectangleFrame();
	GUI_ForceCalibrationIndicateWindowField();
	
	/* ���ñ궨��� */
	GUI_ForceCalibrationDrawCalibrationTable();
	
	/* �м���� */
	GUI_ForceCalibrationShowFrame();
	
	/* ״̬�� */
	ConfigForceCalibrationStatusBarRectangleFrameCoordinate();
	GUI_ForceCalibrationDrawStatusBarRectangleFrame();
	GUI_ForceCalibrationStatusBarField();
}

/*------------------------------------------------------------
 * Function Name  : Show_ForceCalibrationOneFieldContent
 * Description    : ��ʾһ���ֶ�����
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
 * Description    : ��ʾ������(rowNum����1��ʼ)
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
 * Description    : ��ȡ�����Կ�ҳ��
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
 * Description    : ��ȡ�����Կ����ڵ�ǰҳ���
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
 * Description    : �жϵ�ǰ���Ƿ��ѵ���ҳβ
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
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ForceCalibration( void )
{
	uint8_t indexRow = 0;
	uint8_t sampleNum = 0;
	uint8_t pageNum = 0;
	
	/* ������� */
	for (indexRow=1; indexRow<=g_ForceCalibration.curPageSampleNum; ++indexRow)
	{
		Show_ForceCalibrationTestResultTable(indexRow,OBJECT_SERIAL);
	}
	
	/* �������� */
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
 * Description    : ��ݲ˵�����
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
 * Description    : ��ӡ�궨���
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
 * Description    : ��������
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
 * Description    : ���궨�����Ƿ���Ч
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
 * Description    : �����궨��
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
	
	calibrationSegments++;	//����0��
	
	smpl_tab_num_set(g_ForceCalibration.curChannel,calibrationSegments);
	smpl_tab_value_set(g_ForceCalibration.curChannel,0,0);
	smpl_tab_code_set(g_ForceCalibration.curChannel,0,0);
	
	for (segmentIndex=0; segmentIndex<calibrationSegments; ++segmentIndex)
	{
		/* дֵ */
		smpl_tab_value_set(g_ForceCalibration.curChannel,segmentIndex+1,g_CalibrationBody.checkValue[segmentIndex]);
		
		/* д�� */		
		smpl_tab_code_set(g_ForceCalibration.curChannel,segmentIndex+1,g_CalibrationBody.code[segmentIndex]);
	}
	
	prm_save();
	
	return PASSED;
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationChangeCalibrationTableProcess
 * Description    : �����궨��
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
 * Description    : ��������
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
 * Description    : ��������
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
 * Description    : ����ҳ
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
 * Description    : ���ҷ�ҳ
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
 * Description    : ��������
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
 * Description    : ��⾯��
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
			/* �����λ������ */
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
 * Description    : ���ö�̬����У׼״̬
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
 * Description    : ���ö�̬����
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
 * Description    : ˢ�¶�̬����
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
 * Description    : ��������У׼
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FunctionalState ForceCalibrationAllowRunCalibration( void )
{
	/* ������ */
	if (CheckCurrentModel() == FAILED)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[8]);
		
		return DISABLE;
	}
	
	/* �������״̬ */
	if (GetLinkStatus() == LINK_UNLINK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[0]);
		
		return DISABLE;
	}
	
	/* ������ */
	if (g_ForceCalibration.sumSampleNum == 0)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pForceCalibrationWarning[1]);
		
		return DISABLE;
	}
	
	return ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ClearTestResultArea
 * Description    : ��������������
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
 * Description    : ���¼��ر궨�������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReloadForceCalibrationArea( void )
{
	ClearTestResultArea(COLOR_BACK);
	
	/* ������ʼ�� */
	ForceCalibationInit();
	
	/* �������� */
	ForceCalibationConfig();
	
	/* ��ȡ���� */
	ForceCalibrationReadParameter();
	
	/* ��GUI��� */
	GUI_ForceCalibrationDrawCalibrationTable();
	
	/* ���� */
	Traverse_ForceCalibration();
}

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationExecuteStartBody
 * Description    : У׼��ʼ
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
 * Description    : У׼����
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
 * Description    : д����ֵ(��Ŵ�1��ʼ)
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
 * Description    : д��λ��(��Ŵ�1��ʼ)
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
 * Description    : д�����(��Ŵ�1��ʼ)
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
 * Description    : �����ֵ(��Ŵ�1��ʼ)
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
 * Description    : д����ֵ(��Ŵ�1��ʼ)
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
 * Description    : �����ֵ(��Ŵ�1��ʼ)
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
 * Description    : д�����(��Ŵ�1��ʼ)
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
 * Description    : ������(��Ŵ�1��ʼ)
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
 * Description    : ��ȡ��ֵУ׼�������
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
 * Description    : ���ѭ����
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
		
		/* ����ֵ */
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
		
		/* �������� */
		code = GetSammpleCode(g_ForceCalibration.curChannel);
		g_CalibrationBody.code[g_CalibrationBody.curCompletePieceSerial-1] = code;
		ForceCalibrationWriteCode(GetCompletePieceSerialInCurrentPageSerial(),code);
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_CODE);
		
		/* ������� */
		standardForce = g_CalibrationBody.checkValue[g_CalibrationBody.curCompletePieceSerial-1];
		deviation = ForceCalibrationGetDeviation(standardForce,curValue);
		g_CalibrationBody.deviation[g_CalibrationBody.curCompletePieceSerial-1] = deviation;
		ForceCalibrationWriteDeviation(GetCompletePieceSerialInCurrentPageSerial(),deviation);
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_ERROR);
		
		/* ����ҳβ */
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
					SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"У׼��������ж�أ�");
				}
				else if (GetPageName() == FORCE_VERIFICATION_PAGE)
				{
					SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"�춨��������ж�أ�");
				}
			#endif
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationCancelPointProcess
 * Description    : ������ѭ����
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
		
		/* ������ֵ */	
		g_CalibrationBody.realValue[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearForce(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_FORCE);
		
		/* �������� */
		g_CalibrationBody.code[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearCode(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_CODE);
		
		/* ������� */
		g_CalibrationBody.deviation[g_CalibrationBody.curCompletePieceSerial-1] = 0;
		ForceCalibrationClearDeviation(GetCompletePieceSerialInCurrentPageSerial());
		Show_ForceCalibrationTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_ERROR);
		
		g_CalibrationBody.curCompletePieceSerial--;
		
		/* ����ҳ�� */
		if (g_CalibrationBody.curCompletePieceSerial)
		{
			if (GetForceCalibrationPageNum() != g_ForceCalibration.curPage)
			{
				/* �����Կ�����һҳ��ҳβ */
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
 * Description    : ���ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationTakePointCoreCycle( void )
{	
	/* ��㴦�� */
	ForceCalibrationTakePointProcess();
	
	/* ����һ�㴦�� */
	ForceCalibrationCancelPointProcess();
}	

/*------------------------------------------------------------
 * Function Name  : ForceCalibrationExecuteCoreCycle
 * Description    : ִ�к���ѭ����
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
 * Description    : �뿪ҳ���
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
 * Description    : ���ƺ���ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ForceCalibrationCtrlCoreCycle( void )
{
	while (!bsp_CheckTimer(MAIN_CYCLE_TIMING));
	
	/* ��λ���·�����(��������) */
	PCM_CmdSendCycle();
	
	/* У׼ִ�� */
	ForceCalibrationExecuteCoreCycle();
	
	/* ϵͳ������ */
	ForceCalibrationCheckWarn();
	
	/* �ѻ���� */
	CheckOfflineCycle();
	
	/* ���ö�̬���� */
	SetForceCalibrationDynamicContentTask();
	
	/* ˢ�¶�̬���� */
	RefreshForceCalibrationDynamicContent();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
