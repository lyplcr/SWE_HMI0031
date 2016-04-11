/**
  ******************************************************************************
  * @file    DetailReport.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-17 15:12:24
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��ϸ���鱨��
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "DetailReport.h"
#include "TestReport.h"
#include "TestTypeSelect.h"
#include "ExtendParameter.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_FIELD_NUM					15
#define MAX_REPORT_NAME_BIT				16
#define MAX_ONE_PAGE_SHOW_NUM			6	//һҳ�����ʾ�Կ����
#define MAX_OTHER_DATA_NUM				4
#define MAX_OTHER_DATA_BIT				20
#define AVAIL_STRENGTH_DOT_BIT			1

#define DETAIL_REPORT_FRAM_START_X		56
#define DETAIL_REPORT_FRAM_START_Y		114

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_TEST_TYPE = 0,		/* �������� */
	OBJECT_SPECIMEN_SERIAL,		/* �Լ���� */
	OBJECT_SPECIMEN_VARIETY,	/* �Լ�Ʒ�� */
	OBJECT_STRENGTH_GRADE,		/* ǿ�ȵȼ� */
	OBJECT_SPECIMEN_AGE,		/* �Լ����� */
	OBJECT_SPECIMEN_FORMAT,		/* �Լ���� */
	OBJECT_SPECIMEN_LENTH,		/* �Լ����� */
	OBJECT_SPECIMEN_WIDTH,		/* �Լ���� */
	OBJECT_SPECIMEN_HIGH,		/* �Լ��߶� */
	OBJECT_SPECIMEN_SPAN,		/* �Լ���� */
	OBJECT_CORRECTION_FACTOR,	/* ����ϵ�� */
	OBJECT_LOAD_SPEED,			/* �����ٶ� */
	OBJECT_SPECIMEN_NUMS,		/* �Լ����� */	
	OBJECT_SERIAL,				/* ��� 	*/
	OBJECT_FORCE,				/* ��ֵ 	*/
	OBJECT_PRESSURE,			/* ѹǿ 	*/
	OBJECT_SAMPLE_SHAPE,		/* ��״ 	*/
	OBJECT_SAMPLE_AREA,			/* ��� 	*/
	OBJECT_ROUND_DIAMETER,		/* Բ��ֱ�� 	*/
	OBJECT_KL_STRENGTH,			/* ����ǿ�� */
	OBJECT_UP_YIELD_STRENGTH,	/* ������ǿ�� */
	OBJECT_DOWN_YIELD_STRENGTH,	/* ������ǿ�� */
	OBJECT_MAX_FORCE_TOTAL_ELONGATION,	/* ��������쳤�� */
	OBJECT_PIPE_THICKNESS,		/* �ܶκ�� */
	OBJECT_PIPE_OUTER_DIAMETER,	/* �ܶ���ֱ�� */
	OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH,/* �Ǳ�������ǿ�� */
	OBJECT_MAX_FORCE,			/* ����� */
	OBJECT_ELASTIC_MODULUS,		/* ����ģ�� */
}DETAIL_REPORT_PATAMETER_NAME_TypeDef;

typedef enum
{
	INDEX_SNJSKY_TEST_SERIAL = 0,
	INDEX_SNJSKY_TEST_FORMAT,
	INDEX_SNJSKY_TEST_VARIETY,
	INDEX_SNJSKY_TEST_GRADE,
	INDEX_SNJSKY_TEST_AGE,
	INDEX_SNJSKY_TEST_FORCE,
	INDEX_SNJSKY_TEST_PRESSURE,
}INDEX_SNJSKY_TypeDef;

typedef enum
{
	INDEX_JZSJKY_TEST_SERIAL = 0,
	INDEX_JZSJKY_TEST_FORMAT,
	INDEX_JZSJKY_TEST_VARIETY,
	INDEX_JZSJKY_TEST_CORRECTION,
	INDEX_JZSJKY_TEST_AGE,
	INDEX_JZSJKY_TEST_FORCE,
	INDEX_JZSJKY_TEST_PRESSURE,
}INDEX_JZSJKY_TypeDef;

typedef enum
{
	INDEX_HNTKY_TEST_SERIAL = 0,
	INDEX_HNTKY_TEST_FORMAT,
	INDEX_HNTKY_TEST_CORRECTION,
	INDEX_HNTKY_TEST_STRENGTH,
	INDEX_HNTKY_TEST_AGE,
	INDEX_HNTKY_TEST_FORCE,
	INDEX_HNTKY_TEST_PRESSURE,
}INDEX_HNTKY_TypeDef;

typedef enum
{
	INDEX_HNTKZ_TEST_SERIAL = 0,
	INDEX_HNTKZ_TEST_FORMAT,
	INDEX_HNTKZ_TEST_CORRECTION,
	INDEX_HNTKZ_TEST_STRENGTH,
	INDEX_HNTKZ_TEST_AGE,
	INDEX_HNTKZ_TEST_FORCE,
	INDEX_HNTKZ_TEST_PRESSURE,
}INDEX_HNTKZ_TypeDef;

typedef enum
{
	INDEX_QQZKY_TEST_SERIAL = 0,
	INDEX_QQZKY_TEST_VARIETY,
	INDEX_QQZKY_TEST_LENTH,
	INDEX_QQZKY_TEST_WIDTH,
	INDEX_QQZKY_TEST_HIGH,
	INDEX_QQZKY_TEST_FORCE,
	INDEX_QQZKY_TEST_PRESSURE,
}INDEX_QQZKY_TypeDef;

union INDEX_TYKY_TypeDef
{
	enum KYTY_SAMPLE_RECTANGLE
	{
		INDEX_TYKY_RECTANGLE_TEST_SERIAL = 0,
		INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE,
		INDEX_TYKY_RECTANGLE_LENTH,
		INDEX_TYKY_RECTANGLE_WIDTH,
		INDEX_TYKY_RECTANGLE_AREA,
		INDEX_TYKY_RECTANGLE_TEST_CORRECTION,
		INDEX_TYKY_RECTANGLE_TEST_FORCE,
		INDEX_TYKY_RECTANGLE_TEST_PRESSURE,
	}KYTY_SHAPE_RECTANGLE;
	enum KYTY_SAMPLE_ROUND
	{
		INDEX_TYKY_ROUND_TEST_SERIAL = 0,
		INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE,
		INDEX_TYKY_ROUND_DIAMETER,
		INDEX_TYKY_ROUND_AREA,
		INDEX_TYKY_ROUND_TEST_CORRECTION,
		INDEX_TYKY_ROUND_TEST_FORCE,
		INDEX_TYKY_ROUND_TEST_PRESSURE,
	}KYTY_SHAPE_ROUND;
	enum KYTY_SAMPLE_IRREGULAR
	{
		INDEX_TYKY_IRREGULAR_TEST_SERIAL = 0,
		INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE,
		INDEX_TYKY_IRREGULAR_AREA,
		INDEX_TYKY_IRREGULAR_TEST_CORRECTION,
		INDEX_TYKY_IRREGULAR_TEST_FORCE,
		INDEX_TYKY_IRREGULAR_TEST_PRESSURE,
	}KYTY_SHAPE_RIRREGULAR;
};

typedef enum
{
	INDEX_SNJSKZ_TEST_SERIAL = 0,
	INDEX_SNJSKZ_TEST_LENTH,
	INDEX_SNJSKZ_TEST_SPAN,
	INDEX_SNJSKZ_TEST_STRENGTH,
	INDEX_SNJSKZ_TEST_AGE,
	INDEX_SNJSKZ_TEST_FORCE,
	INDEX_SNJSKZ_TEST_PRESSURE,
}INDEX_SNJSKZ_TypeDef;

typedef enum
{
	INDEX_YJSNJKZ_TEST_SERIAL = 0,
	INDEX_YJSNJKZ_TEST_LENTH,
	INDEX_YJSNJKZ_TEST_SPAN,
	INDEX_YJSNJKZ_TEST_STRENGTH,
	INDEX_YJSNJKZ_TEST_AGE,
	INDEX_YJSNJKZ_TEST_FORCE,
	INDEX_YJSNJKZ_TEST_PRESSURE,
}INDEX_YJSNJKZ_TypeDef;

typedef struct
{
	char parameterData[MAX_FIELD_NUM][MAX_REPORT_NAME_BIT+1];
}DETAIL_REPORT_FIELD_DATA_TypeDef;

typedef struct
{
	char serial[MAX_OTHER_DATA_BIT+1];
	char time[MAX_OTHER_DATA_BIT+1];
	char standard[MAX_OTHER_DATA_BIT+1];
	char availStrength[MAX_OTHER_DATA_BIT+1];
}DETAIL_REPORT_OTHER_DATA_TypeDef;

typedef enum
{
	SHOW_SINGLE = 0,
	SHOW_ALL,
}DETAIL_REPORT_SHOW_TYPE_TypeDef;

typedef struct
{
	POSITION_TypeDef posResultData;
	POSITION_TypeDef posSerial;
	POSITION_TypeDef posTime;
	POSITION_TypeDef posStandard;
	POSITION_TypeDef posAvailStrength;
}DETAIL_REPORT_OTHER_POS_TypeDef;

typedef struct
{
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_ONE_PAGE_SHOW_NUM][MAX_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTitle[MAX_FIELD_NUM];
	DETAIL_REPORT_FIELD_DATA_TypeDef fieldData[MAX_ONE_PAGE_SHOW_NUM];
	DETAIL_REPORT_OTHER_DATA_TypeDef otherData;
	DETAIL_REPORT_OTHER_POS_TypeDef otherField;
	const char *pParameterNameArray[MAX_FIELD_NUM];
	ALIGN_TYPE_TypeDef align[MAX_FIELD_NUM];		//���뷽ʽ
	uint8_t indexArray[MAX_FIELD_NUM];				//�������
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ	
	uint8_t curPageSampleNum;						//��ǰҳ�Կ����
	uint8_t sumSampleNum;							//ȫ���Կ����
	uint8_t curPage;								//��ǰҳ��
	uint8_t sumPage;								//��ҳ��
	uint8_t fieldNum;								//�ֶθ���
	DETAIL_REPORT_SHOW_TYPE_TypeDef oneFieldShowType[MAX_FIELD_NUM];	//һ���ֶ���ʾ����
	TEST_TYPE_TypeDef testType;						//��������
	FH_UINT_TypeDef fhChannelUnit;					//����ͨ����λ
	WY_UINT_TypeDef	wyChannelUnit;					//λ��ͨ����λ
	BX_UINT_TypeDef	bxChannelUnit;					//����ͨ����λ
	BoolStatus isPageTurning;						//��ҳ�¼�
	TEST_ATTRIBUTE_TypeDef testAttribute;			//��������
}DETAIL_REPORT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pDetailTestReportTitleName[] = 
{
	"���鱨�� | ˮ�ེɰ��ѹ",		//0
	"���鱨�� | ����ɰ����ѹ",		//1
	"���鱨�� | ��������ѹ",			//2
	"���鱨�� | ����������",			//3
	"���鱨�� | ��ǽש��ѹ",			//4
	"���鱨�� | �𼶶���",			//5
	"���鱨�� | ͨ�ÿ�ѹ",			//6
	"���鱨�� | ˮ�ེɰ����",		//7
	"���鱨�� | ѹ��ˮ�ཬ����",		//8
	"���鱨�� | ͨ�ÿ���",			//9	
	"���鱨�� | ������������",		//10
};

const char * const pDetailReportFieldName[] = 
{
	"���",					//0
	"Ʒ��",					//1
	"�Լ����(mm2)",			//2
	"�Լ����(mm3)",			//3
	"ǿ��",					//4
	"�߳�",					//5
	"���",					//6
	"����",					//7
	"���",					//8
	"�߶�",					//9
	"����",					//10
	"��ֵ(N)",				//11
	"��ֵ(kN)",				//12
	"ѹǿ(MPa)",			//13
	"ϵ��",					//14
	"�Լ����",				//15
	"���(mm2)",			//16
	"��״",					//17
	"Բ��ֱ��",				//18
	"���",					//19
	"���",					//20
	"���",					//21
	"�⾶",					//22
	"����ǿ��",				//23
	"������",				//24
	"������",				//25
	"���쳤��",				//26
	"�Ǳ�������ǿ��",		//27
	"�����",				//28
	"����ģ��",				//29
	"����",					//30
};	

extern const char * const pSpecimen_sharp[];
extern const char * const pSpecimenSharp_KLJSSW[];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_DETAIL_REPORT",zidata = "RAM_DETAIL_REPORT"
	static DETAIL_REPORT_TypeDef g_detailReport;
#pragma arm section

extern REPORT_TypeDef g_readReport;

/* Private function prototypes -----------------------------------------------*/
static void DetailReportInit( void );
static void DetailReportConfig( void );
static void DetailReportReadParameter( void );
static void GUI_DetailReport( void );
static void Traverse_DetailReport( void );
static void DetailReportShortcutCycleTask( void );
static void DetailReportKeyProcess( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadDetailReportPage
 * Description    : ��ϸ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadDetailReportPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* ������ʼ�� */
	DetailReportInit();
	
	/* �������� */
	DetailReportConfig();
	
	/* ��ȡ���� */
	DetailReportReadParameter();
	
	/* ��GUI��� */
	GUI_DetailReport();
	
	/* ���� */
	Traverse_DetailReport();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_detailReport.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ��ݲ˵� */
		DetailReportShortcutCycleTask();
		
		/* �������� */
		DetailReportKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : DetailReportInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportInit( void )
{
	g_detailReport.isIndexMove = NO;
	g_detailReport.refreshShortcut = ENABLE;
	g_detailReport.leavePage.flagLeavePage = RESET;
	g_detailReport.leavePage.flagSaveData = RESET;
	g_detailReport.curPageSampleNum = 0;
	g_detailReport.sumSampleNum = 0;
	
	g_detailReport.fhChannelUnit = GetFH_SmplUnit();
	g_detailReport.wyChannelUnit = GetWY_SmplUnit();
	g_detailReport.bxChannelUnit = GetBX_SmplUnit();
	
	if (isShowDetailReport() == YES)
	{
		ResetShowDetailReport();
		
		g_detailReport.curPage = 1;
	}
	if (g_detailReport.isPageTurning == YES)
	{
		g_detailReport.isPageTurning = NO;
		g_detailReport.curPage = 1;
	}
	
	g_detailReport.sumPage = 1;
	
	g_detailReport.testType = (TEST_TYPE_TypeDef)GetSelectReportTestType();
	
	g_detailReport.testAttribute = GetTestAttribute(g_detailReport.testType);
	
	if (GetCurPageTestReportNum() == 0)
	{
		SetPage(TEST_REPORT_PAGE);
		g_detailReport.leavePage.flagLeavePage = SET;
		g_detailReport.leavePage.flagSaveData = RESET;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetDetailReportFieldIndex
 * Description    : ��ȡ�ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetDetailReportFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_detailReport.fieldNum; ++i)
	{
		if (g_detailReport.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : DetailReportConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportConfig( void )
{
	uint8_t i;
	
	switch ( g_detailReport.testType )
	{
		case NONE_TEST:
			
			break;
		case KYSNJS:
			/* �ֶ�����ֵ */
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_SERIAL] 	= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_FORMAT] 	= OBJECT_SPECIMEN_FORMAT;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_VARIETY] 	= OBJECT_SPECIMEN_VARIETY;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_GRADE] 		= OBJECT_STRENGTH_GRADE;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_SNJSKY_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_SERIAL] 	= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_FORMAT] 	= pDetailReportFieldName[2];
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_VARIETY] 	= pDetailReportFieldName[1];
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_GRADE] 	= pDetailReportFieldName[4];
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_AGE] 		= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_SNJSKY_TEST_PRESSURE] 	= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[0];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_SERIAL] 	= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_FORMAT] 	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_VARIETY]	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_GRADE] 	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_AGE] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_FORCE] 	= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_SNJSKY_TEST_PRESSURE] = SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_SERIAL].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_FORMAT].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_VARIETY].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_GRADE].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_AGE].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_FORCE].pointBit 	= 2;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKY_TEST_PRESSURE].pointBit = 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_SNJSKY_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_SNJSKY_TEST_FORMAT] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKY_TEST_VARIETY] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKY_TEST_GRADE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKY_TEST_AGE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKY_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKY_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KYJZSJ:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_SERIAL] 	= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_FORMAT] 	= OBJECT_SPECIMEN_FORMAT;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_VARIETY] 	= OBJECT_SPECIMEN_VARIETY;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_CORRECTION] = OBJECT_CORRECTION_FACTOR;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_JZSJKY_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_SERIAL] 		= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_FORMAT] 		= pDetailReportFieldName[3];
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_VARIETY] 		= pDetailReportFieldName[1];
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_CORRECTION] 	= pDetailReportFieldName[14];
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_AGE] 			= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_JZSJKY_TEST_PRESSURE] 		= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[1];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_FORMAT] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_VARIETY]		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_CORRECTION] 	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_AGE] 			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_JZSJKY_TEST_PRESSURE]	 	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_FORMAT].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_VARIETY].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_CORRECTION].pointBit 	= 2;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_AGE].pointBit	 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_JZSJKY_TEST_PRESSURE].pointBit		= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_JZSJKY_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_JZSJKY_TEST_FORMAT] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_JZSJKY_TEST_VARIETY] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_JZSJKY_TEST_CORRECTION] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_JZSJKY_TEST_AGE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_JZSJKY_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_JZSJKY_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KYHNT:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_HNTKY_TEST_SERIAL] 		= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_FORMAT] 		= OBJECT_SPECIMEN_FORMAT;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_CORRECTION] 	= OBJECT_CORRECTION_FACTOR;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_STRENGTH] 	= OBJECT_STRENGTH_GRADE;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_HNTKY_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_SERIAL] 		= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_FORMAT] 		= pDetailReportFieldName[15];
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_CORRECTION] 	= pDetailReportFieldName[14];
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_STRENGTH] 		= pDetailReportFieldName[4];
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_AGE] 			= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_HNTKY_TEST_PRESSURE] 		= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[2];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_FORMAT] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_CORRECTION]	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_STRENGTH] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_AGE] 			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_HNTKY_TEST_PRESSURE]	 	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_FORMAT].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_CORRECTION].pointBit 	= 2;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_STRENGTH].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_AGE].pointBit	 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKY_TEST_PRESSURE].pointBit		= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_HNTKY_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_HNTKY_TEST_FORMAT] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKY_TEST_CORRECTION] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKY_TEST_STRENGTH] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKY_TEST_AGE] 			= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKY_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKY_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KZHNT:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_SERIAL] 		= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_FORMAT] 		= OBJECT_SPECIMEN_FORMAT;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_CORRECTION] 	= OBJECT_CORRECTION_FACTOR;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_STRENGTH] 	= OBJECT_STRENGTH_GRADE;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_HNTKZ_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_SERIAL] 		= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_FORMAT] 		= pDetailReportFieldName[15];
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_CORRECTION] 	= pDetailReportFieldName[14];
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_STRENGTH] 		= pDetailReportFieldName[4];
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_AGE] 			= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_HNTKZ_TEST_PRESSURE] 		= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[3];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_FORMAT] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_CORRECTION]	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_STRENGTH] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_AGE] 			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_HNTKZ_TEST_PRESSURE]	 	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_FORMAT].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_CORRECTION].pointBit 	= 2;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_STRENGTH].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_AGE].pointBit	 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_HNTKZ_TEST_PRESSURE].pointBit		= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_HNTKZ_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_HNTKZ_TEST_FORMAT] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKZ_TEST_CORRECTION] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKZ_TEST_STRENGTH] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKZ_TEST_AGE] 			= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKZ_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_HNTKZ_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KYQQZ:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_QQZKY_TEST_SERIAL] 		= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_VARIETY] 	= OBJECT_SPECIMEN_VARIETY;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_LENTH] 		= OBJECT_SPECIMEN_LENTH;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_WIDTH] 		= OBJECT_SPECIMEN_WIDTH;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_HIGH] 		= OBJECT_SPECIMEN_HIGH;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_QQZKY_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_SERIAL] 	= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_VARIETY] 	= pDetailReportFieldName[1];
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_LENTH] 		= pDetailReportFieldName[7];
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_WIDTH] 		= pDetailReportFieldName[8];
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_HIGH] 		= pDetailReportFieldName[9];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_QQZKY_TEST_PRESSURE] 	= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[4];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_VARIETY] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_LENTH]			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_WIDTH] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_HIGH] 			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_QQZKY_TEST_PRESSURE]	 	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_VARIETY].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_LENTH].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_WIDTH].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_HIGH].pointBit	 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_QQZKY_TEST_PRESSURE].pointBit		= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_QQZKY_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_QQZKY_TEST_VARIETY] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_QQZKY_TEST_LENTH] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_QQZKY_TEST_WIDTH] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_QQZKY_TEST_HIGH] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_QQZKY_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_QQZKY_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KYZJDH:			
			break;
		case KYTY:
			report_read(g_detailReport.testType,GetSelectReportFileNameAddr(),&g_readReport);
			
			switch (g_readReport.sample_shape_index)
			{
				case TYKY_SHAPE_RECTANGLE:
					/* ����ֵ */
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_TEST_SERIAL] 		= OBJECT_SERIAL;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE] 	= OBJECT_SAMPLE_SHAPE;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_LENTH] 				= OBJECT_SPECIMEN_LENTH;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_WIDTH] 				= OBJECT_SPECIMEN_WIDTH;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_AREA] 				= OBJECT_SAMPLE_AREA;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_TEST_CORRECTION] 	= OBJECT_CORRECTION_FACTOR;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_TEST_FORCE] 			= OBJECT_FORCE;
					g_detailReport.indexArray[INDEX_TYKY_RECTANGLE_TEST_PRESSURE] 		= OBJECT_PRESSURE;
					
					/* �ֶ��� */
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_SERIAL] 			= pDetailReportFieldName[0];
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE] 		= pDetailReportFieldName[17];
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_LENTH] 					= pDetailReportFieldName[7];
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_WIDTH] 					= pDetailReportFieldName[8];
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_AREA]					= pDetailReportFieldName[16];
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_CORRECTION] 		= pDetailReportFieldName[14];
					if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_FORCE] 		= pDetailReportFieldName[12];
					}
					else
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_FORCE] 		= pDetailReportFieldName[11];
					}
					g_detailReport.pParameterNameArray[INDEX_TYKY_RECTANGLE_TEST_PRESSURE] 			= pDetailReportFieldName[13];
					
					/* ���� */
					g_detailReport.pTitle = pDetailTestReportTitleName[6];
					
					/* �ֶθ��� */
					g_detailReport.fieldNum = 8;
					
					/* һ���ֶ���ʾ���� */
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_TEST_SERIAL] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE] = SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_LENTH]				= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_WIDTH] 			= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_AREA]				= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_TEST_CORRECTION] 	= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_TEST_FORCE] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_RECTANGLE_TEST_PRESSURE]	 	= SHOW_ALL;
					
					/* С����λ�� */
					for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
					{
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_TEST_SERIAL].pointBit 		= 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE].pointBit = 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_LENTH].pointBit 			= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_WIDTH].pointBit 			= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_AREA].pointBit 				= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_TEST_CORRECTION].pointBit	= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_TEST_FORCE].pointBit 		= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_RECTANGLE_TEST_PRESSURE].pointBit		= 1;
					}
					
					/* ���ݶ��� */
					g_detailReport.align[INDEX_TYKY_RECTANGLE_TEST_SERIAL] 			= ALIGN_MIDDLE;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_TEST_SAMPLE_SHAPE] 	= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_LENTH] 				= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_WIDTH] 				= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_AREA]					= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_TEST_CORRECTION] 		= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_TEST_FORCE] 			= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_RECTANGLE_TEST_PRESSURE] 		= ALIGN_LEFT;
					break;
				case TYKY_SHAPE_ROUND:
					/* ����ֵ */
					g_detailReport.indexArray[INDEX_TYKY_ROUND_TEST_SERIAL] 		= OBJECT_SERIAL;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE] 	= OBJECT_SAMPLE_SHAPE;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_DIAMETER] 			= OBJECT_ROUND_DIAMETER;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_AREA] 				= OBJECT_SAMPLE_AREA;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_TEST_CORRECTION] 	= OBJECT_CORRECTION_FACTOR;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_TEST_FORCE] 			= OBJECT_FORCE;
					g_detailReport.indexArray[INDEX_TYKY_ROUND_TEST_PRESSURE] 		= OBJECT_PRESSURE;
					
					/* �ֶ��� */
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_SERIAL] 			= pDetailReportFieldName[0];
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE] 		= pDetailReportFieldName[17];
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_DIAMETER] 				= pDetailReportFieldName[18];
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_AREA]					= pDetailReportFieldName[16];
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_CORRECTION] 		= pDetailReportFieldName[14];
					if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_FORCE] 		= pDetailReportFieldName[12];
					}
					else
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_FORCE] 		= pDetailReportFieldName[11];
					}
					g_detailReport.pParameterNameArray[INDEX_TYKY_ROUND_TEST_PRESSURE] 			= pDetailReportFieldName[13];
					
					/* ���� */
					g_detailReport.pTitle = pDetailTestReportTitleName[6];
					
					/* �ֶθ��� */
					g_detailReport.fieldNum = 7;
					
					/* һ���ֶ���ʾ���� */
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_TEST_SERIAL] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE] = SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_DIAMETER]			= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_AREA]				= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_TEST_CORRECTION] 	= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_TEST_FORCE] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_ROUND_TEST_PRESSURE]	 	= SHOW_ALL;
					
					/* С����λ�� */
					for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
					{
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_TEST_SERIAL].pointBit 		= 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE].pointBit = 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_DIAMETER].pointBit 			= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_AREA].pointBit 				= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_TEST_CORRECTION].pointBit	= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_TEST_FORCE].pointBit 		= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_ROUND_TEST_PRESSURE].pointBit		= 1;
					}
					
					/* ���ݶ��� */
					g_detailReport.align[INDEX_TYKY_ROUND_TEST_SERIAL] 			= ALIGN_MIDDLE;
					g_detailReport.align[INDEX_TYKY_ROUND_TEST_SAMPLE_SHAPE] 	= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_ROUND_DIAMETER] 			= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_ROUND_AREA]					= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_ROUND_TEST_CORRECTION] 		= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_ROUND_TEST_FORCE] 			= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_ROUND_TEST_PRESSURE] 		= ALIGN_LEFT;
					break;
				case TYKY_SHAPE_IRREGULAR:
					/* ����ֵ */
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_TEST_SERIAL] 		= OBJECT_SERIAL;
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE] 	= OBJECT_SAMPLE_SHAPE;
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_AREA] 				= OBJECT_SAMPLE_AREA;
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_TEST_CORRECTION] 	= OBJECT_CORRECTION_FACTOR;
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_TEST_FORCE] 			= OBJECT_FORCE;
					g_detailReport.indexArray[INDEX_TYKY_IRREGULAR_TEST_PRESSURE] 		= OBJECT_PRESSURE;
					
					/* �ֶ��� */
					g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_SERIAL] 			= pDetailReportFieldName[0];
					g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE] 		= pDetailReportFieldName[17];
					g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_AREA] 					= pDetailReportFieldName[16];
					g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_CORRECTION] 		= pDetailReportFieldName[14];
					if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_FORCE] 		= pDetailReportFieldName[12];
					}
					else
					{
						g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_FORCE] 		= pDetailReportFieldName[11];
					}
					g_detailReport.pParameterNameArray[INDEX_TYKY_IRREGULAR_TEST_PRESSURE] 			= pDetailReportFieldName[13];
					
					/* ���� */
					g_detailReport.pTitle = pDetailTestReportTitleName[6];
					
					/* �ֶθ��� */
					g_detailReport.fieldNum = 6;
					
					/* һ���ֶ���ʾ���� */
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_TEST_SERIAL] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE] = SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_AREA]				= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_TEST_CORRECTION] 	= SHOW_SINGLE;
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_TEST_FORCE] 		= SHOW_ALL;
					g_detailReport.oneFieldShowType[INDEX_TYKY_IRREGULAR_TEST_PRESSURE]	 	= SHOW_ALL;
					
					/* С����λ�� */
					for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
					{
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_TEST_SERIAL].pointBit 		= 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE].pointBit = 0;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_AREA].pointBit 				= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_TEST_CORRECTION].pointBit	= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_TEST_FORCE].pointBit 		= 2;
						g_detailReport.oneLevelMenu[i][INDEX_TYKY_IRREGULAR_TEST_PRESSURE].pointBit		= 1;
					}
					
					/* ���ݶ��� */
					g_detailReport.align[INDEX_TYKY_IRREGULAR_TEST_SERIAL] 			= ALIGN_MIDDLE;
					g_detailReport.align[INDEX_TYKY_IRREGULAR_TEST_SAMPLE_SHAPE] 	= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_IRREGULAR_AREA] 				= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_IRREGULAR_TEST_CORRECTION] 		= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_IRREGULAR_TEST_FORCE] 			= ALIGN_LEFT;
					g_detailReport.align[INDEX_TYKY_IRREGULAR_TEST_PRESSURE] 		= ALIGN_LEFT;
					break;
			}			
			break;
		case KZSNJS:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_SERIAL] 	= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_LENTH] 		= OBJECT_SPECIMEN_LENTH;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_SPAN] 		= OBJECT_SPECIMEN_SPAN;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_STRENGTH] 	= OBJECT_STRENGTH_GRADE;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_FORCE] 		= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_SNJSKZ_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_SERIAL] 		= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_LENTH] 		= pDetailReportFieldName[5];
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_SPAN] 			= pDetailReportFieldName[6];
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_STRENGTH] 		= pDetailReportFieldName[4];
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_AGE] 			= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_SNJSKZ_TEST_PRESSURE] 		= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[7];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_LENTH] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_SPAN]			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_STRENGTH] 	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_AGE] 			= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_SNJSKZ_TEST_PRESSURE]	 	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_LENTH].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_SPAN].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_STRENGTH].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_AGE].pointBit	 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_SNJSKZ_TEST_PRESSURE].pointBit		= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_SNJSKZ_TEST_SERIAL] 		= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_SNJSKZ_TEST_LENTH] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKZ_TEST_SPAN] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKZ_TEST_STRENGTH] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKZ_TEST_AGE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKZ_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_SNJSKZ_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KZYJSNJ:
			/* ����ֵ */
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_SERIAL] 	= OBJECT_SERIAL;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_LENTH] 	= OBJECT_SPECIMEN_LENTH;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_SPAN] 		= OBJECT_SPECIMEN_SPAN;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_STRENGTH] 	= OBJECT_STRENGTH_GRADE;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_AGE] 		= OBJECT_SPECIMEN_AGE;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_FORCE] 	= OBJECT_FORCE;
			g_detailReport.indexArray[INDEX_YJSNJKZ_TEST_PRESSURE] 	= OBJECT_PRESSURE;
			
			/* �ֶ��� */
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_SERIAL] 		= pDetailReportFieldName[0];
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_LENTH] 		= pDetailReportFieldName[5];
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_SPAN] 		= pDetailReportFieldName[6];
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_STRENGTH] 	= pDetailReportFieldName[4];
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_AGE] 			= pDetailReportFieldName[10];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_FORCE] 	= pDetailReportFieldName[12];
			}
			else
			{
				g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_FORCE] 	= pDetailReportFieldName[11];
			}
			g_detailReport.pParameterNameArray[INDEX_YJSNJKZ_TEST_PRESSURE] 	= pDetailReportFieldName[13];
			
			/* ���� */
			g_detailReport.pTitle = pDetailTestReportTitleName[8];
			
			/* �ֶθ��� */
			g_detailReport.fieldNum = 7;
			
			/* һ���ֶ���ʾ���� */
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_SERIAL] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_LENTH] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_SPAN]		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_STRENGTH] 	= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_AGE] 		= SHOW_SINGLE;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_FORCE] 		= SHOW_ALL;
			g_detailReport.oneFieldShowType[INDEX_YJSNJKZ_TEST_PRESSURE]	= SHOW_ALL;
			
			/* С����λ�� */
			for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
			{
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_SERIAL].pointBit 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_LENTH].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_SPAN].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_STRENGTH].pointBit 	= 0;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_AGE].pointBit	 		= 0;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_FORCE].pointBit 		= 2;
				g_detailReport.oneLevelMenu[i][INDEX_YJSNJKZ_TEST_PRESSURE].pointBit	= 1;
			}
			
			/* ���ݶ��� */
			g_detailReport.align[INDEX_YJSNJKZ_TEST_SERIAL] 	= ALIGN_MIDDLE;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_LENTH] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_SPAN] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_STRENGTH] 	= ALIGN_LEFT;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_AGE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_FORCE] 		= ALIGN_LEFT;
			g_detailReport.align[INDEX_YJSNJKZ_TEST_PRESSURE] 	= ALIGN_LEFT;
			break;
		case KZTY:
			
			break;
		case KLJSSW:
			report_read(g_detailReport.testType,GetSelectReportFileNameAddr(),&g_readReport);
			
			{
				uint8_t indexNum = 0;
				
				/* ���� */
				g_detailReport.pTitle = pDetailTestReportTitleName[10];
				
				/* ����ֵ */
				g_detailReport.indexArray[indexNum++] = OBJECT_SERIAL;
				g_detailReport.indexArray[indexNum++] = OBJECT_SAMPLE_SHAPE;		
				switch (g_readReport.sample_shape_index)
				{
					case JSSWKL_SHAPE_RECTANGLE:
						g_detailReport.indexArray[indexNum++] = OBJECT_SPECIMEN_LENTH;
						g_detailReport.indexArray[indexNum++] = OBJECT_SPECIMEN_WIDTH;
						break;
					case JSSWKL_SHAPE_ROUND:
						g_detailReport.indexArray[indexNum++] = OBJECT_ROUND_DIAMETER;
						break;
					case JSSWKL_SHAPE_TUBE:
						g_detailReport.indexArray[indexNum++] = OBJECT_PIPE_THICKNESS;
						g_detailReport.indexArray[indexNum++] = OBJECT_PIPE_OUTER_DIAMETER;
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						g_detailReport.indexArray[indexNum++] = OBJECT_SAMPLE_AREA;
						break;
					default:
						break;
				}
				g_detailReport.indexArray[indexNum++] = OBJECT_KL_STRENGTH;				
				switch (g_readReport.yieldJudgeMode)
				{
					case OBVIOUS_YIELD:
						g_detailReport.indexArray[indexNum++] = OBJECT_UP_YIELD_STRENGTH;	
						g_detailReport.indexArray[indexNum++] = OBJECT_DOWN_YIELD_STRENGTH;	
						break;
					case SIGMA0_2:
						g_detailReport.indexArray[indexNum++] = OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH;
						break;
					case NO_YIELD:
						g_detailReport.indexArray[indexNum++] = OBJECT_MAX_FORCE;	
						break;
					default:
						break;
				}
				if (g_readReport.computeElasticModulus)
				{
					g_detailReport.indexArray[indexNum++] = OBJECT_ELASTIC_MODULUS;	
				}
				/* �ֶθ��� */
				g_detailReport.fieldNum = indexNum;
				
				/* �ֶ��� */
				{
					uint8_t i;
					const char *pName = NULL;
					
					for (i=0; i<g_detailReport.fieldNum; ++i)
					{
						switch (g_detailReport.indexArray[i])
						{
							case OBJECT_SERIAL:
								pName = pDetailReportFieldName[0];
								break;
							case OBJECT_SAMPLE_SHAPE:
								pName = pDetailReportFieldName[17];
								break;
							case OBJECT_SPECIMEN_LENTH:
								pName = pDetailReportFieldName[19];
								break;
							case OBJECT_SPECIMEN_WIDTH:
								pName = pDetailReportFieldName[20];
								break;
							case OBJECT_ROUND_DIAMETER:
								pName = pDetailReportFieldName[18];
								break;
							case OBJECT_PIPE_THICKNESS:
								pName = pDetailReportFieldName[21];
								break;
							case OBJECT_PIPE_OUTER_DIAMETER:
								pName = pDetailReportFieldName[22];
								break;
							case OBJECT_SAMPLE_AREA:
								pName = pDetailReportFieldName[16];
								break;
							case OBJECT_KL_STRENGTH:
								pName = pDetailReportFieldName[23];
								break;
							case OBJECT_UP_YIELD_STRENGTH:
								pName = pDetailReportFieldName[24];
								break;
							case OBJECT_DOWN_YIELD_STRENGTH:
								pName = pDetailReportFieldName[25];
								break;
							case OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH:
								pName = pDetailReportFieldName[27];
								break;
							case OBJECT_MAX_FORCE:
								pName = pDetailReportFieldName[28];
								break;
							case OBJECT_ELASTIC_MODULUS:
								pName = pDetailReportFieldName[29];
								break;
							default:
								pName = pDetailReportFieldName[30];
								break;
						}					
						g_detailReport.pParameterNameArray[i] = pName;
					}
				}
				
				/* �ֶ���ʾ���� */
				{
					uint8_t i;
					
					for (i=0; i<g_detailReport.fieldNum; ++i)
					{
						g_detailReport.oneFieldShowType[i] = SHOW_ALL;
					}
				}
				
				/* С����λ�� */
				{
					uint8_t i,j;
					uint8_t dotNum = 0;
					
					for (i=0; i<MAX_ONE_PAGE_SHOW_NUM; ++i)
					{
						for (j=0; j<g_detailReport.fieldNum; ++j)
						{
							switch (g_detailReport.indexArray[j])
							{
								case OBJECT_SERIAL:
									dotNum = 0;
									break;
								case OBJECT_SAMPLE_SHAPE:
									dotNum = 0;
									break;
								case OBJECT_SPECIMEN_LENTH:
									dotNum = 2;
									break;
								case OBJECT_SPECIMEN_WIDTH:
									dotNum = 2;
									break;
								case OBJECT_ROUND_DIAMETER:
									dotNum = 2;
									break;
								case OBJECT_PIPE_THICKNESS:
									dotNum = 2;
									break;
								case OBJECT_PIPE_OUTER_DIAMETER:
									dotNum = 2;
									break;
								case OBJECT_SAMPLE_AREA:
									dotNum = 2;
									break;
								case OBJECT_KL_STRENGTH:
									dotNum = 1;
									break;
								case OBJECT_UP_YIELD_STRENGTH:
									dotNum = 1;
									break;
								case OBJECT_DOWN_YIELD_STRENGTH:
									dotNum = 1;
									break;
								case OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH:
									dotNum = 1;
									break;
								case OBJECT_MAX_FORCE:
									dotNum = 2;
									break;
								case OBJECT_ELASTIC_MODULUS:
									dotNum = 1;
									break;
								default:
									dotNum = 0;
									break;
							}					
							g_detailReport.oneLevelMenu[i][j].pointBit = dotNum;
						}
					}
				}
				
				/* ���ݶ��� */
				{
					uint8_t i;
					ALIGN_TYPE_TypeDef type;
					
					for (i=0; i<g_detailReport.fieldNum; ++i)
					{
						if (g_detailReport.indexArray[i] == OBJECT_SERIAL)
						{
							type = ALIGN_MIDDLE;
						}
						else
						{
							type = ALIGN_LEFT;
						}
						
						g_detailReport.align[i] = type;
					}
				}
			}			
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : DetailReportReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportReadParameter( void )
{
	uint8_t index = 0;
	uint8_t remain = 0;
	uint8_t i;
	uint8_t baseIndex = 0;
	float force = 0;
	
	if (FR_OK != report_read(g_detailReport.testType,GetSelectReportFileNameAddr(),&g_readReport) )
	{
		return;
	}
	
	if ( g_readReport.sample_serial == 0 )
	{
		return;
	}
	
	g_detailReport.sumSampleNum = g_readReport.sample_serial;
	
	remain = g_detailReport.sumSampleNum % MAX_ONE_PAGE_SHOW_NUM;
	if (remain)
	{
		g_detailReport.sumPage = g_detailReport.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM + 1;
	}
	else
	{
		g_detailReport.sumPage = g_detailReport.sumSampleNum / MAX_ONE_PAGE_SHOW_NUM;
	}	
	
	if ((g_detailReport.curPage==0) || (g_detailReport.curPage>g_detailReport.sumPage))
	{
		g_detailReport.curPage = 1;
	}
	
	if (g_detailReport.curPage == g_detailReport.sumPage)
	{
		if (remain)
		{
			g_detailReport.curPageSampleNum = remain;
		}
		else
		{
			g_detailReport.curPageSampleNum = MAX_ONE_PAGE_SHOW_NUM;
		}
	}
	else
	{
		g_detailReport.curPageSampleNum = MAX_ONE_PAGE_SHOW_NUM;
	}
	
	/* ��ȡ�ļ��� */
	strcpy(g_detailReport.otherData.serial,GetSelectReportFileNameAddr());
	
	/* ��ȡʱ��*/
	tTimeConvTimeFormate(ENABLE,GetTestReportTime(),g_detailReport.otherData.time);
	
	/* ��ȡ��׼ */
	strcpy(g_detailReport.otherData.standard,g_readReport.test_standard);
	
	/* ��ȡ��Чǿ�� */
	if (g_readReport.result_valid)
	{
		floattochar(MAX_OTHER_DATA_BIT,AVAIL_STRENGTH_DOT_BIT,g_readReport.strength_valid[0],g_detailReport.otherData.availStrength);
	}
	else
	{
		strcpy(g_detailReport.otherData.availStrength,"��Ч");
	}
	
	/* ��ȡ��ϸ���� */
	baseIndex = (g_detailReport.curPage - 1) * MAX_ONE_PAGE_SHOW_NUM;
	
	index = GetDetailReportFieldIndex(OBJECT_SERIAL);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			numtochar(MAX_REPORT_NAME_BIT,baseIndex+i+1,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_VARIETY);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			strcpy(g_detailReport.fieldData[i].parameterData[index],g_readReport.sample_type);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_STRENGTH_GRADE);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			strcpy(g_detailReport.fieldData[i].parameterData[index],g_readReport.strength_grade);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_AGE);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			numtochar(MAX_REPORT_NAME_BIT,g_readReport.sample_age,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_FORMAT);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			strcpy(g_detailReport.fieldData[i].parameterData[index],g_readReport.sample_spec);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_LENTH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.length,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_WIDTH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.width,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_HIGH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.high,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SPECIMEN_SPAN);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.sample_span,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_CORRECTION_FACTOR);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.correct_cof,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_FORCE);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			force = g_readReport.force[baseIndex+i];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				force /= 1000;
			}
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,force,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_PRESSURE);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.strength[baseIndex+i],g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SAMPLE_SHAPE);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{
			switch ( g_detailReport.testType )
			{
				case KYTY:
					switch (g_readReport.sample_shape_index)
					{
						case TYKY_SHAPE_RECTANGLE:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimen_sharp[0]);
							break;
						case TYKY_SHAPE_ROUND:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimen_sharp[1]);
							break;
						case TYKY_SHAPE_IRREGULAR:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimen_sharp[2]);
							break;
					}
					break;
				case KLJSSW:
					switch (g_readReport.sample_shape_index)
					{
						case JSSWKL_SHAPE_RECTANGLE:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimenSharp_KLJSSW[0]);
							break;
						case JSSWKL_SHAPE_ROUND:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimenSharp_KLJSSW[1]);
							break;
						case JSSWKL_SHAPE_TUBE:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimenSharp_KLJSSW[2]);
							break;
						case JSSWKL_SHAPE_IRREGULAR:
							strcpy(g_detailReport.fieldData[i].parameterData[index],pSpecimenSharp_KLJSSW[3]);
							break;
					}
					break;
				default:
					break;
			}
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_SAMPLE_AREA);
	if (index != 0xff)
	{
		float area = 0;
		
		switch ( g_detailReport.testType )
		{
			case KYTY:
				switch (g_readReport.sample_shape_index)
				{
					case TYKY_SHAPE_RECTANGLE:
					case TYKY_SHAPE_ROUND:	
						area = g_readReport.gz_area;
						break;
					case TYKY_SHAPE_IRREGULAR:
						area = g_readReport.bgz_area;
						break;
				}
				break;
			case KLJSSW:
				switch (g_readReport.sample_shape_index)
				{
					case JSSWKL_SHAPE_RECTANGLE:
						area = g_readReport.gz_area;
						break;
					case JSSWKL_SHAPE_ROUND:
						area = g_readReport.gz_area;
						break;
					case JSSWKL_SHAPE_TUBE:
						area = g_readReport.gz_area;
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						area = g_readReport.bgz_area;
						break;
				}
				break;
			default:
				break;
		}
		
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{			
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,area,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_ROUND_DIAMETER);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{			
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.yx_diameter,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_KL_STRENGTH);
	if (index != 0xff)
	{		
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{	
			{
				float tempf = g_readReport.maxStrength[baseIndex+i];
				
				floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,tempf,g_detailReport.fieldData[i].parameterData[index]);
			}
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_UP_YIELD_STRENGTH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			{
				float tempf = g_readReport.upYieldStrength[baseIndex+i];
			
				floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,tempf,g_detailReport.fieldData[i].parameterData[index]);
			}
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_DOWN_YIELD_STRENGTH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			{
				float tempf = g_readReport.downYieldStrength[baseIndex+i];
			
				floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,tempf,g_detailReport.fieldData[i].parameterData[index]);
			}
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_MAX_FORCE_TOTAL_ELONGATION);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			{
				float tempf = g_readReport.maxForceSumElongation[baseIndex+i];
			
				floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,tempf,g_detailReport.fieldData[i].parameterData[index]);
			}
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_PIPE_THICKNESS);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.pipeThickness,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_PIPE_OUTER_DIAMETER);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,g_readReport.pipeOuterDiameter,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,\
				g_readReport.nonProportionalExtensionStrength[baseIndex+i],g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_MAX_FORCE);
	if (index != 0xff)
	{
		float force = 0;
		
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{	
			force = g_readReport.maxForce[baseIndex+i];
			if (g_detailReport.fhChannelUnit == FH_UNIT_kN)
			{
				force /= 1000;
			}			
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,\
				force,g_detailReport.fieldData[i].parameterData[index]);
		}
	}
	
	index = GetDetailReportFieldIndex(OBJECT_ELASTIC_MODULUS);
	if (index != 0xff)
	{
		for (i=0; i<g_detailReport.curPageSampleNum; ++i)
		{					
			floattochar(MAX_REPORT_NAME_BIT,g_detailReport.oneLevelMenu[i][index].pointBit,\
				g_readReport.elasticModulus[baseIndex+i],g_detailReport.fieldData[i].parameterData[index]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigDetailReportOnwRowRectangleFrameCoordinate
 * Description    : ���ý���һ���ֶ�GUI����(���ռ700����)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigDetailReportOneFieldRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_detailReport.indexArray[fieldIndex];
	
	/* ͨ������ */
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].x = x;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].y = y;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].pointColor = COLOR_POINT;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].backColor = COLOR_BACK;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].recordBackColor = COLOR_BACK;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].width = 28;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].fontSize = 24;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].rowDistance = 0;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].columnDistance = 0;
	g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 52;
			break;
		case OBJECT_SPECIMEN_FORMAT:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 196;
			break;
		case OBJECT_SAMPLE_SHAPE:
			switch (g_detailReport.testType)
			{
				case KYTY:
					switch (g_readReport.sample_shape_index)
					{
						case TYKY_SHAPE_RECTANGLE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 88;
							break;
						case TYKY_SHAPE_ROUND:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
							break;
						case TYKY_SHAPE_IRREGULAR:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 192;
							break;
					}
					break;					
				case KLJSSW:
					switch (g_readReport.sample_shape_index)
					{
						case JSSWKL_SHAPE_RECTANGLE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 88;
							break;
						case JSSWKL_SHAPE_ROUND:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 88;
							break;
						case JSSWKL_SHAPE_TUBE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 88;
							break;
						case JSSWKL_SHAPE_IRREGULAR:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 88;
							break;
					}
					break;	
				default:
					break;
			}
			break; 
		case OBJECT_ROUND_DIAMETER:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 133;
			break;
		case OBJECT_KL_STRENGTH:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 101;
			break;
		case OBJECT_UP_YIELD_STRENGTH:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 101;
			break;
		case OBJECT_DOWN_YIELD_STRENGTH:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 101;
			break;
		case OBJECT_MAX_FORCE_TOTAL_ELONGATION:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 114;
			break;
		case OBJECT_PIPE_THICKNESS:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			break;
		case OBJECT_PIPE_OUTER_DIAMETER:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			break;
		case OBJECT_SAMPLE_AREA:
			switch (g_detailReport.testType)
			{
				case KYTY:
					switch (g_readReport.sample_shape_index)
					{
						case TYKY_SHAPE_RECTANGLE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 121;
							break;
						case TYKY_SHAPE_ROUND:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 133;
							break;
						case TYKY_SHAPE_IRREGULAR:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 150;
							break;
					}
					break;					
				case KLJSSW:
					switch (g_readReport.sample_shape_index)
					{
						case JSSWKL_SHAPE_RECTANGLE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 150;
							break;
						case JSSWKL_SHAPE_ROUND:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 150;
							break;
						case JSSWKL_SHAPE_TUBE:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 150;
							break;
						case JSSWKL_SHAPE_IRREGULAR:
							g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 150;
							break;
					}
					break;	
				default:
					break;
			}
			break;
		case OBJECT_SPECIMEN_VARIETY:
			if (g_detailReport.testType == KYQQZ)
			{
				g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 196;
			}
			else
			{
				g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			}
			break;
		case OBJECT_SPECIMEN_LENTH:
			if ((g_detailReport.testType == KZSNJS) || (g_detailReport.testType == KZYJSNJ))
			{
				g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 196;
			}
			else
			{
				g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			}
			break;
		case OBJECT_STRENGTH_GRADE:
		case OBJECT_SPECIMEN_AGE:		
		case OBJECT_SPECIMEN_WIDTH:
		case OBJECT_SPECIMEN_HIGH:
		case OBJECT_SPECIMEN_SPAN:
		case OBJECT_CORRECTION_FACTOR:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 76;
			break;
		
		case OBJECT_FORCE:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 100;
			break;
		
		case OBJECT_PRESSURE:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 124;
			break;
		case OBJECT_NON_PROPORTIONAL_EXTENSION_STRENGTH:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 172;
			break;
		case OBJECT_MAX_FORCE:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 100;
			break; 
		case OBJECT_ELASTIC_MODULUS:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 100;
			break;
		default:
			g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth = 0;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigDetailReportOnwRowRectangleFrameCoordinate
 * Description    : ���ý���һ��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigDetailReportOnwRowRectangleFrameCoordinate( uint8_t rowIndex, uint16_t startX, uint16_t startY )	
{
	uint8_t fieldIndex = 0;
	uint16_t x = startX;
	uint16_t y = startY;
	
	for (fieldIndex=0; fieldIndex<g_detailReport.fieldNum; ++fieldIndex)
	{
		ConfigDetailReportOneFieldRectangleFrameCoordinate(rowIndex,fieldIndex,x,y);
		
		x += g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lenth - g_detailReport.oneLevelMenu[rowIndex][fieldIndex].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigDetailReportOtherFieldRectangleFrameCoordinate
 * Description    : ���������ֶ�GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigDetailReportOtherFieldRectangleFrameCoordinate( uint16_t startX, uint16_t startY )
{
	uint16_t x = startX;
	uint16_t y = startY;
	uint8_t index = 0;
	
	x += g_detailReport.oneLevelMenu[0][0].lineWidth;
	y += g_detailReport.oneLevelMenu[0][0].lineWidth;
	g_detailReport.otherField.posResultData.x = x;
	g_detailReport.otherField.posResultData.y = y;
	
	y -= 48;
	g_detailReport.otherField.posSerial.x = x;
	g_detailReport.otherField.posSerial.y = y;
	
	x += 336;
	g_detailReport.otherField.posTime.x = x;
	g_detailReport.otherField.posTime.y = y;
	
	x = startX;
	y = startY;
	
	if (g_detailReport.curPageSampleNum)
	{		
		index = g_detailReport.curPageSampleNum - 1;
		
		x += g_detailReport.oneLevelMenu[index][0].lineWidth;
		y = g_detailReport.oneLevelMenu[index][0].y + g_detailReport.oneLevelMenu[index][0].width +\
			 g_detailReport.oneLevelMenu[index][0].lineWidth;	
	}
	else
	{
		x += g_detailReport.oneLevelMenuTitle[0].lineWidth;
		y = g_detailReport.oneLevelMenuTitle[0].y + g_detailReport.oneLevelMenuTitle[0].width + \
			g_detailReport.oneLevelMenuTitle[0].lineWidth;
	}
	
	g_detailReport.otherField.posStandard.x = x;
	g_detailReport.otherField.posStandard.y = y;
	
	x += 384;
	g_detailReport.otherField.posAvailStrength.x = x;
	g_detailReport.otherField.posAvailStrength.y = y;
}

/*------------------------------------------------------------
 * Function Name  : ConfigDetailReportRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigDetailReportRectangleFrameCoordinate( void )
{	
	uint16_t x = DETAIL_REPORT_FRAM_START_X;
	uint16_t y = DETAIL_REPORT_FRAM_START_Y;
	uint8_t i;
	
	y += 30;
	ConfigDetailReportOnwRowRectangleFrameCoordinate(0,x,y);
	
	memcpy(g_detailReport.oneLevelMenuTitle,g_detailReport.oneLevelMenu,MAX_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
	
	y += (g_detailReport.oneLevelMenuTitle[0].width - g_detailReport.oneLevelMenuTitle[0].lineWidth);
	
	for (i=0; i<g_detailReport.curPageSampleNum; ++i)
	{
		ConfigDetailReportOnwRowRectangleFrameCoordinate(i,x,y);
		y += g_detailReport.oneLevelMenu[i][0].width - g_detailReport.oneLevelMenu[i][0].lineWidth;
	}
	
	ConfigDetailReportOtherFieldRectangleFrameCoordinate(DETAIL_REPORT_FRAM_START_X,DETAIL_REPORT_FRAM_START_Y);
}	

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportDrawOneRowTitleRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DetailReportDrawOneRowTitleRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_detailReport.fieldNum; ++fieldIndex)
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
 * Function Name  : GUI_DetailReportRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DetailReportRectangleFrame( void )
{
	uint8_t i;
	
	GUI_DetailReportDrawOneRowTitleRectangleFrame(g_detailReport.oneLevelMenuTitle);
	
	for (i=0; i<g_detailReport.curPageSampleNum; ++i)
	{
		GUI_DetailReportDrawOneRowTitleRectangleFrame(g_detailReport.oneLevelMenu[i]);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportOtherField
 * Description    : ���������ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DetailReportOtherField( void )
{
	uint16_t x = DETAIL_REPORT_FRAM_START_X;
	uint16_t y = DETAIL_REPORT_FRAM_START_Y;
	uint16_t pointColor = COLOR_POINT;
	uint16_t backColor = COLOR_BACK;
	uint16_t lenth = g_detailReport.oneLevelMenuTitle[0].lineWidth;
	uint16_t width = 30 + 2 * g_detailReport.oneLevelMenuTitle[0].width + g_detailReport.oneLevelMenuTitle[0].lineWidth;
	uint8_t i;
	
	for (i=0; i<g_detailReport.fieldNum; ++i)
	{
		lenth += g_detailReport.oneLevelMenuTitle[i].lenth - g_detailReport.oneLevelMenuTitle[i].lineWidth;
	}
	for (i=0; i<g_detailReport.curPageSampleNum; ++i)
	{
		width += g_detailReport.oneLevelMenuTitle[i].width - g_detailReport.oneLevelMenuTitle[i].lineWidth;
	}
	lcd_draw_rect(x,y,lenth,width,pointColor);
	lcd_draw_rect(x+1,y+1,lenth-2,width-2,pointColor);
	
	x = g_detailReport.otherField.posResultData.x;
	y = g_detailReport.otherField.posResultData.y;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"������ݣ�");
	
	x = g_detailReport.otherField.posSerial.x;
	y = g_detailReport.otherField.posSerial.y;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"�Լ���ţ�");
	
	x = g_detailReport.otherField.posTime.x;
	y = g_detailReport.otherField.posTime.y;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"�������ڣ�");
	
	x = g_detailReport.otherField.posStandard.x;
	y = g_detailReport.otherField.posStandard.y;
	
	GUI_DispStr24At(x,y,pointColor,backColor,"����׼��");
	
	x = g_detailReport.otherField.posAvailStrength.x;
	y = g_detailReport.otherField.posAvailStrength.y;
	
	switch ( g_detailReport.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			GUI_DispStr24At(x,y,pointColor,backColor,"��Чǿ��(MPa)��");	
			break;
		case STRETCH_TEST:				
			break;
		case INVALID_TEST:
			break;
		default:
			break;
	}
	
}	

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReportField
 * Description    : �����ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DetailReportField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint8_t fieldLen = 0;
	uint16_t lenth = 0;
	
	for (fieldIndex=0; fieldIndex<g_detailReport.fieldNum; ++fieldIndex)
	{
		fieldLen = strlen(g_detailReport.pParameterNameArray[fieldIndex]) * \
				   (g_detailReport.oneLevelMenuTitle[fieldIndex].fontSize>>1);
		lenth = g_detailReport.oneLevelMenuTitle[fieldIndex].lenth - \
				2 * g_detailReport.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		x = g_detailReport.oneLevelMenuTitle[fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_detailReport.oneLevelMenuTitle[fieldIndex].lineWidth;
		y = g_detailReport.oneLevelMenuTitle[fieldIndex].y + \
			g_detailReport.oneLevelMenuTitle[fieldIndex].lineWidth;
		
		pointColor = g_detailReport.oneLevelMenuTitle[fieldIndex].pointColor;
		backColor = g_detailReport.oneLevelMenuTitle[fieldIndex].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.pParameterNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : DetailReportShowCurPageInfomation
 * Description    : ��ʾ��ǰҳ��Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportShowCurPageInfomation( void )
{
	char buff[10];
	uint32_t pageNum = 0;
	const uint16_t back_color = COLOR_BACK;
	const uint16_t point_color = MIDDLEBLUE;
	
	if ( g_detailReport.curPageSampleNum )
	{
		GUI_DispStr24At(PAGE_DI_POS_X,PAGE_DI_POS_Y,COLOR_POINT,back_color,"��      ҳ;");		
		lcd_fill(PAGE_DI_POS_X+36,PAGE_DI_POS_Y,48,24,back_color);
		
		pageNum = g_detailReport.curPage;
		if (pageNum < 10000)
		{
			usprintf(buff,"%04d",pageNum);
		}
		else if (pageNum < 100000)
		{
			usprintf(buff,"%05d",pageNum);
		}
		else
		{
			strcpy(buff,"-----");
		}
		
		GUI_DispStr24At(PAGE_DI_POS_X+36,PAGE_DI_POS_Y,point_color,back_color,buff);
		
		GUI_DispStr24At(PAGE_GONG_POS_X,PAGE_GONG_POS_Y,COLOR_POINT,back_color,"��      ҳ");
		lcd_fill(PAGE_GONG_POS_X+36,PAGE_GONG_POS_Y,48,24,back_color);
		
		pageNum = g_detailReport.sumPage;
		if (pageNum < 10000)
		{
			usprintf(buff,"%04d",pageNum);
		}
		else if (pageNum < 100000)
		{
			usprintf(buff,"%05d",pageNum);
		}
		else
		{
			strcpy(buff,"-----");
		}
		
		GUI_DispStr24At(PAGE_GONG_POS_X+36,PAGE_GONG_POS_Y,point_color,back_color,buff);
		
		if (g_detailReport.sumPage > 1)
		{
			lcd_show_image(PAGE_GONG_POS_X+300,PAGE_GONG_POS_Y-3,PIC_DIR_UP_DOWN);
		}
	}
	else
	{
		GUI_DispStr24At(PAGE_DI_POS_X,PAGE_DI_POS_Y,COLOR_POINT,back_color,"δ�ҵ����������ı��棡");	
	}
}

/*------------------------------------------------------------
 * Function Name  : DetailReportShowCircleMarkInfomation
 * Description    : ��ʾԲ�α����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportShowCircleMarkInfomation( void )
{
	DRAW_CIRCLE_MARK_TypeDef circleMark;
	const uint16_t RADIUS = 3;		/* �뾶 */
	const uint16_t DISTANCE = 12;	/* Բ��Բ֮��ľ��� */
	uint32_t curNum = GetCurPageCurTestReportIndex() + 1;
	uint32_t sumNum = GetCurPageTestReportNum();
	uint32_t lenth = 0;
	
	/* Բ�α�����򳤶� */
	lenth = (2 * RADIUS + DISTANCE) *  sumNum - DISTANCE;
	
	circleMark.x = ((LCD_LENTH_X - lenth) >> 1);
	circleMark.y = PAGE_DI_POS_Y + 45;
	circleMark.distance = DISTANCE;
	circleMark.curSerial = curNum;
	circleMark.sumSerial = sumNum;
	circleMark.pointColor = CL_BLUE3;
	circleMark.backColor = COLOR_BACK;
	circleMark.radius = RADIUS;
	
	DrawCircleMark(&circleMark);
}

/*------------------------------------------------------------
 * Function Name  : GUI_DetailReport
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DetailReport( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_detailReport.pTitle);
	
	ConfigDetailReportRectangleFrameCoordinate();
	
	GUI_DetailReportRectangleFrame();
	
	GUI_DetailReportOtherField();
	
	GUI_DetailReportField();
	
	DetailReportShowCurPageInfomation();
	
	DetailReportShowCircleMarkInfomation();
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOneFieldContent
 * Description    : ��ʾһ���ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportOneFieldContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_detailReport.oneLevelMenu[indexRow][indexField].x + g_detailReport.oneLevelMenu[indexRow][indexField].lineWidth;
	uint16_t y = g_detailReport.oneLevelMenu[indexRow][indexField].y + g_detailReport.oneLevelMenu[indexRow][indexField].lineWidth;
	uint16_t pointColor = g_detailReport.oneLevelMenu[indexRow][indexField].pointColor;
	uint16_t backColor = g_detailReport.oneLevelMenu[indexRow][indexField].backColor;
	uint16_t fontLenth = strlen(g_detailReport.fieldData[indexRow].parameterData[indexField]) * \
						(g_detailReport.oneLevelMenu[indexRow][indexField].fontSize>>1);
	uint16_t rectLenth = g_detailReport.oneLevelMenu[indexRow][indexField].lenth - \
						(2 * g_detailReport.oneLevelMenu[indexRow][indexField].lineWidth);
	
	if (g_detailReport.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.fieldData[indexRow].parameterData[indexField]);
}	

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportOneRowOneLevelMenuContent( uint8_t indexRow )
{
	uint8_t i;
	
	for (i=0; i<g_detailReport.fieldNum; ++i)
	{
		Show_DetailReportOneFieldContent(indexRow,i);
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_detailReport.curPageSampleNum; ++i)
	{
		Show_DetailReportOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportSerialFieldContent
 * Description    : ��ʾ����ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportSerialFieldContent( void )
{
	uint16_t x = g_detailReport.otherField.posSerial.x + 5 * 24;
	uint16_t y = g_detailReport.otherField.posSerial.y;
	uint16_t pointColor = COLOR_POINT;
	uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.otherData.serial);
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportTimeFieldContent
 * Description    : ��ʾʱ���ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportTimeFieldContent( void )
{
	uint16_t x = g_detailReport.otherField.posTime.x + 5 * 24;
	uint16_t y = g_detailReport.otherField.posTime.y;
	uint16_t pointColor = COLOR_POINT;
	uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.otherData.time);
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportStandardFieldContent
 * Description    : ��ʾ�Լ���׼�ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportStandardFieldContent( void )
{
	uint16_t x = g_detailReport.otherField.posStandard.x + 5 * 24;
	uint16_t y = g_detailReport.otherField.posStandard.y;
	uint16_t pointColor = COLOR_POINT;
	uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.otherData.standard);
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportAvailStrengthFieldContent
 * Description    : ��ʾ�Լ���Чǿ���ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportAvailStrengthFieldContent( void )
{
	uint16_t x = g_detailReport.otherField.posAvailStrength.x + 8 * 24;
	uint16_t y = g_detailReport.otherField.posAvailStrength.y;
	uint16_t pointColor = COLOR_POINT;
	uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_detailReport.otherData.availStrength);
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOtherFieldContent
 * Description    : ��ʾ�����ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_DetailReportOtherFieldContent( void )
{
	Show_DetailReportSerialFieldContent();
	Show_DetailReportTimeFieldContent();
	Show_DetailReportStandardFieldContent();
	switch ( g_detailReport.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			Show_DetailReportAvailStrengthFieldContent();
			break;
		case STRETCH_TEST:				
			break;
		case INVALID_TEST:
			break;
		default:
			break;
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_DetailReport
 * Description    : ������ϸ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_DetailReport( void )
{
	Show_DetailReportOtherFieldContent();
	
	Show_DetailReportOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : DetailReportShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_detailReport.refreshShortcut == ENABLE)
	{
		g_detailReport.refreshShortcut = DISABLE;
		
		pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
		
		pShortCut->pContent[0] = pTwoLevelMenu[15];
		pShortCut->pContent[1] = pTwoLevelMenu[86];
		pShortCut->pContent[2] = pTwoLevelMenu[50];
		pShortCut->pContent[3] = pTwoLevelMenu[66];
		
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : DetailReportDeleteReport
 * Description    : ɾ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportDeleteReport( void )
{
	TestReportDeleteReport();
	
	g_detailReport.isPageTurning = YES;
	
	g_detailReport.leavePage.flagLeavePage = SET;
	g_detailReport.leavePage.flagSaveData = RESET;
}
#if 0
/*------------------------------------------------------------
 * Function Name  : DetailReportPrintReport
 * Description    : ��ӡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportPrintReport( void )
{
	ErrorStatus errStatus;
	
	if ( g_detailReport.sumSampleNum == 0)
	{
		return;
	}
	
	errStatus = TestReportPrintCursorRowReport();
	
	if (errStatus == ERROR)
	{
		g_detailReport.leavePage.flagLeavePage = SET;
		g_detailReport.leavePage.flagSaveData = RESET;
	}
	
	g_detailReport.refreshShortcut = ENABLE;
}
#endif

/*------------------------------------------------------------
 * Function Name  : DetailReportKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DetailReportKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F1:
				DetailReportDeleteReport();
				break;
			case KEY_F2:
			case KEY_ENTER:
//			case KEY_EXPORT:
//				TestReportExportReport();
				if ( g_detailReport.curPageSampleNum )
				{
					SetPage(CURVE_SHOW_PAGE);
					g_detailReport.leavePage.flagLeavePage = SET;
					g_detailReport.leavePage.flagSaveData = RESET;
				}
				break;	
//			case KEY_PRINT:
//				DetailReportPrintReport();
//				break;
			case KEY_UP:
				if (g_detailReport.sumPage > 1)
				{
					if (g_detailReport.curPage)
					{
						g_detailReport.curPage--;
					}
					
					if (!g_detailReport.curPage)
					{
						g_detailReport.curPage = g_detailReport.sumPage;
					}
				
					g_detailReport.leavePage.flagLeavePage = SET;
					g_detailReport.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_DOWN:
				if (g_detailReport.sumPage > 1)
				{
					g_detailReport.curPage++;
					if (g_detailReport.curPage > g_detailReport.sumPage)
					{
						g_detailReport.curPage = 1;
					}

					g_detailReport.leavePage.flagLeavePage = SET;
					g_detailReport.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_F3:
			case KEY_LEFT:
				TestReportIndexDecrease();
				g_detailReport.isPageTurning = YES;
				g_detailReport.leavePage.flagLeavePage = SET;
				g_detailReport.leavePage.flagSaveData = RESET;
				break;
			case KEY_F4:
			case KEY_RIGHT:
				TestReportIndexIncrease();
				g_detailReport.isPageTurning = YES;
				g_detailReport.leavePage.flagLeavePage = SET;
				g_detailReport.leavePage.flagSaveData = RESET;
				break;			
			case KEY_ESC:
				SetPage(TEST_REPORT_PAGE);
				g_detailReport.leavePage.flagLeavePage = SET;
				g_detailReport.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
