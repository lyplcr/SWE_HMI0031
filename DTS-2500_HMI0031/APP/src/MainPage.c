/**
  ******************************************************************************
  * @file    MainPage.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-28 15:06:55
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "MainPage.h"
#include "TestParameter.h"
#include "UserLogin.h"
#include "LinkMode.h"
#include "netconf.h"
#include "ControlParameter.h"
#include "SoftActive.h"
#include "TestAfterDispose.h"
#include "ChannelSelect.h"


/* Private define ------------------------------------------------------------*/
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK
	
#define MAX_TEST_INFOMATION_NUM				5		//���������Ϣ��
#define MAX_TEST_INFOMATION_OBJECT_NUM		20		//���������Ϣ�������

#define MAX_RECORD_TEST_RESULT_NUM			20		//��¼����Կ����
#define MAX_TEST_RESULT_TABLE_FIELD_NUM		3		//����ֶθ���
#define MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM	6		//һҳ�Կ����

#define TEST_INFOMATION_FAILED_MAX_CHAR_NUM	16
#define TEST_RESULT_SERIAL_MAX_CHAR_NUM		4
#define TEST_RESULT_FORCE_MAX_CHAR_NUM		8	
#define TEST_RESULT_STRENGTH_MAX_CHAR_NUM	6	//0->����floattocharת���󣬱��-0.0�������ڴ���Ҫ��һЩ
#define TEST_RESULT_AVAIL_SERIAL_MAX_CHAR_NUM	8

#define SHOW_FORCE_DOT_BIT					2		//��ֵС��λ
#define SHOW_STRENGTH_DOT_BIT				1		//ǿ����Чλ

/* ʾֵ�� */
#define INDICATE_WINDOWS_NUMS				4		//ʾֵ������
#define	INDICATE_WINDOWS_ROW_NUMS			2		//ʾֵ������
#define INDICATE_WINDOWS_FORCE_CHAR_NUM		8		
#define INDICATE_WINDOWS_SPEED_CHAR_NUM		6
#define INDICATE_WINDOWS_STRENGTH_CHAR_NUM	5
#define INDICATE_WINDOWS_PEAK_CHAR_NUM		8

/* ״̬�� */
#define STATUS_BAR_NUMS						5			

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	REFRESH_FORCE = 0,
	REFRESH_SPEED,
	REFRESH_STRENGTH,
	REFRESH_PEAK,
	REFRESH_COMMUNICATION_STATUS,
	REFRESH_TEST_STATUS,
	REFRESH_CODE,
	REFRESH_TIME,
	REFRESH_PERIPHERAL_STATUS,
}REFRESH_TERM_TypeDef;

typedef enum
{
	OBJECT_SERIAL = 0,	
	OBJECT_FORCE,		
	OBJECT_STRENGTH,	
}OBJECT_TEST_RESULT_TITLE_NAME_TypeDef;

typedef enum
{
	INDEX_SERIAL = 0,	
	INDEX_FORCE,		
	INDEX_STRENGTH,		
}INDEX_TEST_RESULT_TITLE_NAME_TypeDef;

typedef enum
{
	OBJECT_COMM_STATUS = 0,
	OBJECT_TEST_STATUS,
	OBJECT_CODE,
	OBJECT_TIME,
	OBJECT_PERIPHERAL_STATUS,
}OBJECT_STATUS_BAR_NAME_TypeDef;

typedef enum
{
	INDEX_COMM_STATUS = 0,
	INDEX_TEST_STATUS,
	INDEX_CODE,
	INDEX_TIME,
	INDEX_STATUS,
}INDEX_STATUS_BAR_NAME_TypeDef;

typedef enum
{
	OBJECT_WINDOWS_FORCE = 0,	
	OBJECT_WINDOWS_SPEED,		
	OBJECT_WINDOWS_STRENGTH,	
	OBJECT_WINDOWS_PEAK,
}OBJECT_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	INDEX_WINDOWS_FORCE = 0,	
	INDEX_WINDOWS_SPEED,		
	INDEX_WINDOWS_STRENGTH,	
	INDEX_WINDOWS_PEAK,
}INDEX_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	STATUS_PEAK_IDLE = 0,
	STATUS_PEAK_SHOW,
	STATUS_PEAK_KEEP,
}PEAK_STATUS_TypeDef;

typedef struct
{
	char serial[TEST_RESULT_SERIAL_MAX_CHAR_NUM+1];
	char force[TEST_RESULT_FORCE_MAX_CHAR_NUM+1];
	char strength[TEST_RESULT_STRENGTH_MAX_CHAR_NUM+1];
}TEST_RESULT_FIELD_DATA_TypeDef;

typedef struct
{
	char availValue[TEST_RESULT_AVAIL_SERIAL_MAX_CHAR_NUM+1];
	char availForce[TEST_RESULT_FORCE_MAX_CHAR_NUM+1];
	char availStrength[TEST_RESULT_STRENGTH_MAX_CHAR_NUM+1];
}TEST_RESULT_FIELD_AVAIL_DATA_TypeDef;

typedef struct
{
	char force[INDICATE_WINDOWS_FORCE_CHAR_NUM+1];
	char speed[INDICATE_WINDOWS_SPEED_CHAR_NUM+1];
	char strength[INDICATE_WINDOWS_STRENGTH_CHAR_NUM+1];
	char peak[INDICATE_WINDOWS_PEAK_CHAR_NUM+1];
}INDICATE_WINDOWS_FIELD_DATA_TypeDef;

typedef struct
{
//	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTestInfomation[MAX_TEST_INFOMATION_NUM];
//	uint8_t indexTestInfomationArray[MAX_TEST_INFOMATION_NUM];				//������Ϣ�������
//	char testInfomationData[MAX_TEST_INFOMATION_NUM][TEST_INFOMATION_FAILED_MAX_CHAR_NUM+1];
//	const char *pTestInfomationNameArray[MAX_TEST_INFOMATION_NUM];
//	const char *pTestInfomationUnitArray[MAX_TEST_INFOMATION_NUM];
	
	/* ʾֵ�� */
	uint8_t indexIndicateWindowsArray[INDICATE_WINDOWS_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuIndicateWindows[INDICATE_WINDOWS_ROW_NUMS][INDICATE_WINDOWS_NUMS];
	const char *pIndicateWindowsTitleNameArray[INDICATE_WINDOWS_NUMS];
	uint8_t indicateWindowNum;						//ʾֵ���ֶθ���
	INDICATE_WINDOWS_FIELD_DATA_TypeDef indicateWindowsData[INDICATE_WINDOWS_NUMS];
	
	/* ��� */
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableTitle[MAX_TEST_RESULT_TABLE_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableContent[MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM][MAX_TEST_RESULT_TABLE_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableAvailValue[MAX_TEST_RESULT_TABLE_FIELD_NUM];	
	uint8_t indexTestResultTableArray[MAX_TEST_RESULT_TABLE_FIELD_NUM];		//�������������	
	TEST_RESULT_FIELD_DATA_TypeDef testResultData[MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM];
	TEST_RESULT_FIELD_AVAIL_DATA_TypeDef testResultAvailData;
	const char *pTestResultTitleNameArray[MAX_TEST_RESULT_TABLE_FIELD_NUM];
	uint8_t tableFieldNum;							//����ֶθ���
	
	/* ״̬�� */
	uint8_t indexStatusBarArray[STATUS_BAR_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuStatusBar[STATUS_BAR_NUMS];
	const char *pStatusBarTitleNameArray[STATUS_BAR_NUMS];
	uint8_t statusBarNum;							//״̬������
	
	const char * pTestTitle;							//����
	ALIGN_TYPE_TypeDef align[MAX_TEST_RESULT_TABLE_FIELD_NUM];		//���뷽ʽ
	uint8_t testInfomationNum;						//������Ϣ����	
	uint8_t curPageSampleNum;						//��ǰҳ�Կ����
	uint8_t sumSampleNum;							//ȫ���Կ����
	uint8_t curPage;								//��ǰҳ��
	uint8_t sumPage;								//��ҳ��
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	TEST_TYPE_TypeDef testType;						//��������
	SMPL_NAME_TypeDef2 showChannel;					//��ʾͨ��
	SMPL_NAME_TypeDef2 tureChannel;					//��ʵͨ��
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}MAIN_PAGE_TypeDef;

typedef struct
{
	TEST_STATUS_TypeDef status;						//����״̬
	FlagStatus flagOnePieceSampleComplete;			//һ��������ɱ�־
	FlagStatus flagOneGroupSampleComplete;			//һ��������ɱ�־
	FlagStatus flagOneGroupTestResultAvail;			//һ����������Ч��־
	FlagStatus flagCancelLastPiece;					//������һ��
	FlagStatus flagHandEndOneGroup;					//����һ��
	FlagStatus flagAutoUpdateSerial;				//�Զ������Լ����
	FlagStatus flagStartJudgeBreak;					//��ʼ����
	FlagStatus startTest;							//��ʼ��־
	FlagStatus endTest;								//������־
	uint8_t curCompletePieceSerial;					//��ǰ��������Կ���
	FlagStatus flagTestAfterDispose;				//��������־
	float breakPeak;								//���Ʒ�ֵ
	float breakStrength;							//���ǿ��
	PEAK_STATUS_TypeDef peakStatus;					//��ֵ״̬
	BoolStatus isWriteToSD;							//�Ƿ�д��SD��
	BoolStatus isExecuteEndGroup;					//�Ƿ�ִ�н���һ��
	BoolStatus isAutoPrintReport;					//�Ƿ��Զ���ӡ����
}TEST_BODY_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTestInfomationName[] = 
{
	"�Լ���ţ�",		//0
	"�Լ�Ʒ�֣�",		//1
	"ǿ�ȵȼ���",		//2
	"�Լ����ڣ�",		//3
	"�Լ����",		//4
	"�����ٶȣ�",		//5
	"�Լ�������",		//6
	"���ط�ʽ��",		//7
	"��ʱʱ�䣺",		//8
	"�Լ����ȣ�",		//9
	"�Լ���ȣ�",		//10
	"�Լ��߶ȣ�",		//11
	"�߳���",			//12
	"��ࣺ",			//13
	"����ϵ����",		//14
};

const char * const pMainPageTestName[] = 
{
	"�������������",//0
	"ˮ�ེɰ��ѹ",	//1
	"����ɰ����ѹ",	//2
	"��������ѹ",	//3
	"����������",	//4
	"��ǽש��ѹ",	//5
	"�𼶶���",		//6
	"ͨ�ÿ�ѹ",		//7
	"ˮ�ེɰ����",	//8
	"ѹ��ˮ�ཬ����",//9
	"ͨ�ÿ���",		//10
};

const char * const pMainPageTestResultTitleName[] = 
{
	"���",			//0
	"��ֵ(kN)",		//1
	"��ֵ(N)",		//2
	"ǿ��(MPa)",	//3
};

const char * const pMainPageIndecateWindowsTitleName[] = 
{
	"��ֵ(kN)",		//0
	"��ֵ(N)",		//1
	"�ٶ�(kN/s)",	//2
	"�ٶ�(N/s)",	//3
	"ǿ��(MPa)",	//4
	"��ֵ(kN)",		//5
	"��ֵ(N)",		//6
};

const char * const pStatusBarTitleName[] = 
{
	"ͨѶ״̬��",	//0
	"����״̬��",	//1
	"��ֵ��",		//2
	"ʱ�䣺",		//3
	"",				//4
};

const char * const pMainPageWarning[] = 
{
	"��ǰ�����ѻ�״̬��",			//0
	"�Կ��������Ϊ0��",				//1
	"���鱣��ʧ�ܣ�",				//2
	"������ֵ��������ʧ�ܣ�",		//3
	"һ������δ��ɣ����ܽ���",		//4
	"����������档",				//5
	"һ��������ɣ�����ִ�г���",		//6
	"������",					 	//7
	"����ɾ��ʧ�ܣ�",				//8
	"�۶����Ƿ�λ�ڼ����غ��ڣ�",		//9
	"δ���ֿ��Դ�ӡ�����鱨�棡",		//10
	"���浼��ʧ�ܣ�",				//11
	"�����ȡʧ�ܣ�",				//12
	"�����U�̣�",					//13
};	

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_MAIN_PAGE",zidata = "RAM_MAIN_PAGE"
	static MAIN_PAGE_TypeDef g_mainPage;
	static TEST_BODY_TypeDef g_testBody;
#pragma arm section

static REPORT_TypeDef g_readReport;

/* Private function prototypes -----------------------------------------------*/
static void MainPageKeyProcess( void );
static void MainPageInit( void );
static void MainPageConfig( void );
static void MainPageReadParameter( void );
static void GUI_MainPage( void );
static void MainPageCtrlCoreCycle( void );
static void GUI_MainPageDrawCoordinate( uint32_t maxForce, uint32_t maxTime );
static void Traverse_TestResultTable( void );
static void MainPageClearCursor( uint8_t nowSampleSerial );
static void MainPageLoadCursor( uint8_t nowSampleSerial );
static void ReloadTestResultArea( void );
static void MainPageLeavePageCheckCycle( void );
static void MainPageShortcutCycleTask( void );
static void InitMainPageCoordinateDrawLine( void );
static TestStatus MainPageCheckSerialRepeat( void );
static void SetTestStatus( TEST_STATUS_TypeDef testStatus );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadMainPage
 * Description    : ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadMainPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	MainPageInit();
	
	/* �������� */
	MainPageConfig();
	
	/* ��ȡ���� */
	MainPageReadParameter();
	
	/* ��GUI��� */
	GUI_MainPage();
	
	/* ���� */
	Traverse_TestResultTable();

	/* ����Ļ */
	SetBackLightEffectOpen();

	while (g_mainPage.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ��ݲ˵� */
		MainPageShortcutCycleTask();
		
		/* �������� */
		MainPageKeyProcess();
		
		/* ���ƺ��� */
		MainPageCtrlCoreCycle();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		MainPageLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageInit( void )
{
	disp_syn(DISP_CHN_FH);
	disp_syn(DISP_CHN_SPEED);
	disp_syn(DISP_CHN_STRENGTH);
	disp_syn(DISP_CHN_PEAK);
	
	InitInterfaceElement();
	
	g_mainPage.isIndexMove = NO;
	g_mainPage.refreshShortcut = ENABLE;
	g_mainPage.leavePage.flagLeavePage = RESET;
	g_mainPage.leavePage.flagSaveData = RESET;
	
	g_mainPage.curPageSampleNum = 0;
	g_mainPage.sumSampleNum = 0;
	g_mainPage.testInfomationNum = 0;	
	g_mainPage.sumPage = 0;
	
	g_mainPage.testType = (TEST_TYPE_TypeDef)pHmi->test_standard_index;
	
	if ( (UNIT_kN==GetFH_SmplUnit() ) && (SMPL_KY_NUM==GetCurChannel()) )
	{
		g_mainPage.showChannel = SMPL_KY_NUM;	
	}
	else
	{
		g_mainPage.showChannel = SMPL_KZ_NUM;	
	}
	
	g_mainPage.tureChannel = GetCurChannel();
	
	SetTestStatus(TEST_IDLE);
	g_testBody.peakStatus = STATUS_PEAK_IDLE;
	g_testBody.flagOnePieceSampleComplete = RESET;
	g_testBody.flagOneGroupSampleComplete = RESET;
	g_testBody.flagOneGroupTestResultAvail = RESET;
	g_testBody.flagCancelLastPiece = RESET;
	g_testBody.flagHandEndOneGroup = RESET;
	g_testBody.startTest = RESET;
	g_testBody.endTest = RESET;
	g_testBody.curCompletePieceSerial = 0;
	g_testBody.isWriteToSD = NO;
	g_testBody.isExecuteEndGroup = NO;
	g_testBody.isAutoPrintReport = NO;
}

/*------------------------------------------------------------
 * Function Name  : SetTestStatus
 * Description    : ��������״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetTestStatus( TEST_STATUS_TypeDef testStatus )
{
	g_testBody.status = testStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetTestStatus
 * Description    : ��ȡ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TEST_STATUS_TypeDef GetTestStatus( void )
{
	return g_testBody.status;
}


/*------------------------------------------------------------
 * Function Name  : GetMainPageTestResultFieldIndex
 * Description    : ��ȡ�������ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetMainPageTestResultFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_mainPage.tableFieldNum; ++i)
	{
		if (g_mainPage.indexTestResultTableArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetMainPageIndicateWindowsFieldIndex
 * Description    : ��ȡʾֵ���ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetMainPageIndicateWindowsFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_mainPage.indicateWindowNum; ++i)
	{
		if (g_mainPage.indexIndicateWindowsArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetMainPageStatusBarFieldIndex
 * Description    : ��ȡ״̬���ֶε�����
 * Input          : handle�����
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetMainPageStatusBarFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_mainPage.statusBarNum; ++i)
	{
		if (g_mainPage.indexStatusBarArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : MainPageConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageConfig( void )
{
	/* ʾֵ������ */
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_FORCE] 		= OBJECT_WINDOWS_FORCE;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_SPEED] 		= OBJECT_WINDOWS_SPEED;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_STRENGTH] 	= OBJECT_WINDOWS_STRENGTH;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_PEAK] 		= OBJECT_WINDOWS_PEAK;
	
	/* �������ֶ�����ֵ */
	g_mainPage.indexTestResultTableArray[INDEX_SERIAL] = OBJECT_SERIAL;
	g_mainPage.indexTestResultTableArray[INDEX_FORCE] = OBJECT_FORCE;
	g_mainPage.indexTestResultTableArray[INDEX_STRENGTH] = OBJECT_STRENGTH;
	
	/* ״̬������ */
	g_mainPage.indexStatusBarArray[INDEX_COMM_STATUS] 	= OBJECT_COMM_STATUS;
	g_mainPage.indexStatusBarArray[INDEX_TEST_STATUS] 	= OBJECT_TEST_STATUS;
	g_mainPage.indexStatusBarArray[INDEX_CODE] 			= OBJECT_CODE;
	g_mainPage.indexStatusBarArray[INDEX_TIME] 			= OBJECT_TIME;
	g_mainPage.indexStatusBarArray[INDEX_STATUS] 		= OBJECT_PERIPHERAL_STATUS;
	
	/* ʾֵ�������� */
	g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_STRENGTH] 	= pMainPageIndecateWindowsTitleName[4];
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pMainPageIndecateWindowsTitleName[0];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pMainPageIndecateWindowsTitleName[2];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_PEAK] 	= pMainPageIndecateWindowsTitleName[5];
	}
	else
	{
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pMainPageIndecateWindowsTitleName[1];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pMainPageIndecateWindowsTitleName[3];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_PEAK] 	= pMainPageIndecateWindowsTitleName[6];
	}
	
	/* ������������ */
	g_mainPage.pTestResultTitleNameArray[INDEX_SERIAL] 		= pMainPageTestResultTitleName[0];
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		g_mainPage.pTestResultTitleNameArray[INDEX_FORCE] 	= pMainPageTestResultTitleName[1];
	}
	else
	{
		g_mainPage.pTestResultTitleNameArray[INDEX_FORCE] 	= pMainPageTestResultTitleName[2];
	}
	g_mainPage.pTestResultTitleNameArray[INDEX_STRENGTH] 	= pMainPageTestResultTitleName[3];
	
	/* ״̬������ */
	g_mainPage.pStatusBarTitleNameArray[INDEX_COMM_STATUS] 	= pStatusBarTitleName[0];
	g_mainPage.pStatusBarTitleNameArray[INDEX_TEST_STATUS] 	= pStatusBarTitleName[1];
	g_mainPage.pStatusBarTitleNameArray[INDEX_CODE] 		= pStatusBarTitleName[2];
	g_mainPage.pStatusBarTitleNameArray[INDEX_TIME] 		= pStatusBarTitleName[3];
	g_mainPage.pStatusBarTitleNameArray[INDEX_STATUS] 		= pStatusBarTitleName[4];
	
	/* ���� */
	g_mainPage.tableFieldNum = MAX_TEST_RESULT_TABLE_FIELD_NUM;
	g_mainPage.indicateWindowNum = INDICATE_WINDOWS_NUMS;
	g_mainPage.statusBarNum = STATUS_BAR_NUMS;
	
	/* ���ݶ��� */
	g_mainPage.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_mainPage.align[INDEX_FORCE] 		= ALIGN_LEFT;
	g_mainPage.align[INDEX_STRENGTH] 	= ALIGN_LEFT;
	
	switch ( g_mainPage.testType )
	{
		case NONE_TEST:
			g_mainPage.pTestTitle = pMainPageTestName[0];
			break;
		case KYSNJS:
			g_mainPage.pTestTitle = pMainPageTestName[1];
			break;
		case KYJZSJ:
			g_mainPage.pTestTitle = pMainPageTestName[2];
			break;
		case KYHNT:
			g_mainPage.pTestTitle = pMainPageTestName[3];
			break;
		case KZHNT:
			g_mainPage.pTestTitle = pMainPageTestName[4];
			break;
		case KYQQZ:
			g_mainPage.pTestTitle = pMainPageTestName[5];
			break;
		case KYZJDH:
			g_mainPage.pTestTitle = pMainPageTestName[6];
			break;
		case KYTY:
			g_mainPage.pTestTitle = pMainPageTestName[7];
			break;
		case KZSNJS:
			g_mainPage.pTestTitle = pMainPageTestName[8];
			break;
		case KZYJSNJ:
			g_mainPage.pTestTitle = pMainPageTestName[9];
			break;
		case KZTY:
			g_mainPage.pTestTitle = pMainPageTestName[10];
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : FromTestParameterCopyToReport
 * Description    : �������������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FromTestParameterCopyToReport( REPORT_TypeDef *pReport )
{
	TEST_TypeDef * const pCurTest = pTest;
	
	strcpy(pReport->test_standard,pCurTest->test_standard);
	strcpy(pReport->sample_type,pCurTest->custom_sample_type);
	strcpy(pReport->strength_grade,pCurTest->custom_strength_grade);
	pReport->sample_age = pCurTest->sample_age;
	strcpy(pReport->sample_spec,pCurTest->custom_sample_spec);
	pReport->sample_span = pCurTest->zfx_span;
	pReport->sample_num = pCurTest->sample_num;
	pReport->sample_serial = 0;
	pReport->test_is_complete = 0;
	pReport->force_valid[0] = 0;
	pReport->strength_valid[0] = 0;
	pReport->result_valid = 0;
	pReport->test_process = 0;
	pReport->length = pCurTest->zfx_length;
	pReport->width = pCurTest->zfx_width;
	pReport->high = pCurTest->zfx_higth;
	pReport->correct_cof = pCurTest->correct_cof;
	pReport->area = pCurTest->gz_area;
	strcpy(pReport->sample_shape,pCurTest->sample_shape);
}	

/*------------------------------------------------------------
 * Function Name  : MainPageWriteSerial
 * Description    : д����(��Ŵ�1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteSerial( uint8_t index, uint8_t serial )
{	
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{		
		numtochar(TEST_RESULT_SERIAL_MAX_CHAR_NUM,serial,g_mainPage.testResultData[index-1].serial);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteForce
 * Description    : д����ֵ(��Ŵ�1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteForce( uint8_t index, float force )
{
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{
		if (g_mainPage.showChannel == SMPL_KY_NUM)
		{
			force /= 1000;
		}
		
		if (fabs(force) > 9999999)
		{
			strcpy(g_mainPage.testResultData[index-1].force,"--------");
			
			return;
		}
		floattochar(TEST_RESULT_FORCE_MAX_CHAR_NUM,SHOW_FORCE_DOT_BIT,force,g_mainPage.testResultData[index-1].force);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageClearForce
 * Description    : �����ֵ(��Ŵ�1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageClearForce( uint8_t index )
{
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{
		g_mainPage.testResultData[index-1].force[0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteStrength
 * Description    : д��ǿ��(��Ŵ�1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteStrength( uint8_t index, float strength )
{
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{		
		if (fabs(strength) > 999)
		{
			strcpy(g_mainPage.testResultData[index-1].strength,"---");
			
			return;
		}
		floattochar(TEST_RESULT_STRENGTH_MAX_CHAR_NUM,SHOW_STRENGTH_DOT_BIT,strength,g_mainPage.testResultData[index-1].strength);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteStrength
 * Description    : д��ǿ��(��Ŵ�1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageClearStrength( uint8_t index )
{
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{		
		g_mainPage.testResultData[index-1].strength[0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteAvailValue
 * Description    : д����Чֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteAvailValue( const char *pValue )
{	
	strcpy(g_mainPage.testResultAvailData.availValue,pValue);
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteAvailForce
 * Description    : д����Ч��ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteAvailForce( float force )
{
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		force /= 1000;
	}
	
	if (force > 9999999)
	{
		strcpy(g_mainPage.testResultAvailData.availForce,"--------");
		
		return;
	}
	
	if (force < MIN_FLOAT_PRECISION_DIFF_VALUE)
	{
		strcpy(g_mainPage.testResultAvailData.availForce,"��Ч");
	}
	else
	{
		floattochar(TEST_RESULT_FORCE_MAX_CHAR_NUM,SHOW_FORCE_DOT_BIT,force,g_mainPage.testResultAvailData.availForce);	
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteAvailStrength
 * Description    : д����Чǿ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteAvailStrength( float strength )
{
	if (strength > 999)
	{
		strcpy(g_mainPage.testResultAvailData.availStrength,"---");
		
		return;
	}
	
	if(g_readReport.result_valid)
	{
		floattochar(TEST_RESULT_STRENGTH_MAX_CHAR_NUM,SHOW_STRENGTH_DOT_BIT,strength,g_mainPage.testResultAvailData.availStrength);
	}
	else
	{
		strcpy(g_mainPage.testResultAvailData.availStrength,"��Ч");
	}
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
	
	serial = g_testBody.curCompletePieceSerial % MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
	if (serial == 0)
	{
		if (g_testBody.curCompletePieceSerial)
		{
			serial = g_mainPage.curPageSampleNum;
		}
		else
		{
			serial = 0;
		}
	}
	
	return serial;
}

/*------------------------------------------------------------
 * Function Name  : GetCompletePiecePageNum
 * Description    : ��ȡ�����Կ�ҳ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetCompletePiecePageNum( void )
{
	uint8_t serial = 0;
	uint8_t remain = 0;
	
	if (g_mainPage.sumPage == 0)
	{
		return 0;
	}
	
	if (g_testBody.curCompletePieceSerial == 0)
	{
		return 1;
	}
	
	serial = g_testBody.curCompletePieceSerial / MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
	
	remain = g_testBody.curCompletePieceSerial % MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
	if (remain)
	{
		serial += 1;
	}
	
	return serial;
}

/*------------------------------------------------------------
 * Function Name  : IsCurrentSampleReachToPageTail
 * Description    : �жϵ�ǰ�Կ��Ƿ��ѵ���ҳβ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus IsCurrentSampleReachToPageTail( void )
{
	if (g_mainPage.curPageSampleNum == 0)
	{
		return NO;
	}
	
	if (GetCompletePieceSerialInCurrentPageSerial() == g_mainPage.curPageSampleNum)
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : MainPageAutoUpdateSerial
 * Description    : �Զ����±��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageAutoUpdateSerial( void )
{
	if (g_testBody.flagAutoUpdateSerial == SET)
	{
		g_testBody.flagAutoUpdateSerial = RESET;
		
		if (MainPageCheckSerialRepeat() == FAILED)
		{
			/* ����Ļ */
			SetBackLightEffectOpen();
			
			PopWindowsProcessCycle();
			
			g_mainPage.leavePage.flagLeavePage = SET;
			g_mainPage.leavePage.flagSaveData = RESET;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageReadParameter( void )
{
	uint8_t i;
	FRESULT result;
	uint8_t remain = 0;
	uint8_t baseIndex = 0;
	uint8_t index = 0;
	
//	testInfo.testInfomationNum = g_mainPage.testInfomationNum;
//	if (testInfo.testInfomationNum > MAX_TEST_INFOMATION_PARAMETER_NUM)
//	{
//		testInfo.testInfomationNum = MAX_TEST_INFOMATION_PARAMETER_NUM;
//	}
//	
//	for (i=0; i<testInfo.testInfomationNum; ++i)
//	{
//		testInfo.testHandle[i] = g_mainPage.indexTestInfomationArray[i];
//		testInfo.pParameterContent[i] = g_mainPage.testInfomationData[i];
//	}
	
	result = report_read(g_mainPage.testType,pTest->test_serial,&g_readReport);
	if (result != FR_OK)	//δ���ֱ���
	{
		memset(&g_readReport,0x00,sizeof(REPORT_TypeDef));
		
		FromTestParameterCopyToReport(&g_readReport);
	}
	
	if ((g_readReport.sample_num==0) || (g_readReport.sample_num>MAX_RECORD_TEST_RESULT_NUM))
	{
		g_mainPage.curPage = 0;
		
		return;
	}
	
	/* �Կ��ܸ��� */
	g_mainPage.sumSampleNum = g_readReport.sample_num;
	
	/* ��ҳ�� */
	remain = g_mainPage.sumSampleNum % MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;	
	if (remain)
	{
		g_mainPage.sumPage = g_mainPage.sumSampleNum / MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM + 1;
	}
	else
	{
		g_mainPage.sumPage = g_mainPage.sumSampleNum / MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
	}
	
	/* ��ǰҳ*/
	if (g_mainPage.curPage > g_mainPage.sumPage)
	{
		g_mainPage.curPage = 0;
	}
	
	/* ��ǰҳ�Կ���� */
	if (g_mainPage.curPage)
	{
		if (g_mainPage.curPage == g_mainPage.sumPage)
		{
			if (remain)
			{
				g_mainPage.curPageSampleNum = remain;
			}
			else
			{
				g_mainPage.curPageSampleNum = MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
			}
		}
		else
		{
			g_mainPage.curPageSampleNum = MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
		}
	}
	else
	{
		g_mainPage.curPageSampleNum = 0;
	}
	
	/* ��־λ */
	if (g_readReport.test_is_complete)
	{
		g_testBody.flagOneGroupSampleComplete = SET;
	}
	else
	{
		g_testBody.flagOneGroupSampleComplete = RESET;
	}
	
	if (g_readReport.result_valid)
	{
		g_testBody.flagOneGroupTestResultAvail = SET;
	}
	else
	{
		g_testBody.flagOneGroupTestResultAvail = RESET;
	}
	
	g_testBody.curCompletePieceSerial = g_readReport.sample_serial;
	
	/* �����������Ϣ */
	baseIndex = (g_mainPage.curPage - 1) * MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
	
	index = GetMainPageTestResultFieldIndex(OBJECT_SERIAL);
	if (index != 0xff)
	{
		for (i=1; i<=g_mainPage.curPageSampleNum; ++i)
		{
			MainPageWriteSerial(i,baseIndex+i);
		}
	}
	
	index = GetMainPageTestResultFieldIndex(OBJECT_FORCE);
	if (index != 0xff)
	{
		for (i=1; i<=g_mainPage.curPageSampleNum; ++i)
		{
			MainPageWriteForce(i,g_readReport.force[baseIndex+i-1]);
		}
	}
	
	index = GetMainPageTestResultFieldIndex(OBJECT_STRENGTH);
	if (index != 0xff)
	{
		for (i=1; i<=g_mainPage.curPageSampleNum; ++i)
		{
			MainPageWriteStrength(i,g_readReport.strength[baseIndex+i-1]);
		}
	}
	
	index = GetMainPageTestResultFieldIndex(OBJECT_SERIAL);
	if (index != 0xff)
	{
		MainPageWriteAvailValue("��Чֵ");
	}
	
	index = GetMainPageTestResultFieldIndex(OBJECT_FORCE);
	if (index != 0xff)
	{
		MainPageWriteAvailForce(g_readReport.force_valid[0]);
	}
	
	index = GetMainPageTestResultFieldIndex(OBJECT_STRENGTH);
	if (index != 0xff)
	{
		MainPageWriteAvailStrength(g_readReport.strength_valid[0]);
	}
	
	/* �Զ����±�� */
	MainPageAutoUpdateSerial();
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate
 * Description    : ����ʾֵ��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_mainPage.indexIndicateWindowsArray[fieldIndex];
	
	g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x = x;
	g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y = y;
	if (!rowIndex)
	{
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor = DARKBLUE;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordBackColor = DARKBLUE;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width = 30;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize = 24;
	}
	else
	{
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor = LGRAY;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].recordBackColor = LGRAY;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width = 60;
		g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize = 48;
	}
	g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].rowDistance = 0;
	g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].columnDistance = 0;
	g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_WINDOWS_FORCE:
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 238;
			break;
		case OBJECT_WINDOWS_SPEED:
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 180;
			break;
		case OBJECT_WINDOWS_STRENGTH:
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 158;		
			break;
		case OBJECT_WINDOWS_PEAK:
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 230;		
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageIndicateWindowsRectangleFrameCoordinate
 * Description    : ����ʾֵ��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageIndicateWindowsRectangleFrameCoordinate( void )
{
	uint8_t fieldIndex;
	const uint16_t START_X = 0;
	const uint16_t START_Y = 2;
	uint16_t x = START_X;
	uint16_t y = START_Y;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.indicateWindowNum; ++fieldIndex)
	{
		y = START_Y;
		
		ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate(0,fieldIndex,x,y);
		
		y += g_mainPage.oneLevelMenuIndicateWindows[0][fieldIndex].width - g_mainPage.oneLevelMenuIndicateWindows[0][fieldIndex].lineWidth;
		
		ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate(1,fieldIndex,x,y);
		
		x += g_mainPage.oneLevelMenuIndicateWindows[0][fieldIndex].lenth - g_mainPage.oneLevelMenuIndicateWindows[0][fieldIndex].lineWidth; 
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageTableTestResultOneFeildRectangleFrameCoordinate
 * Description    : ����������GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageTableTestResultOneFeildRectangleFrameCoordinate( uint8_t rowIndex, uint8_t fieldIndex, uint16_t x, uint16_t y )	
{
	uint8_t handle = g_mainPage.indexTestResultTableArray[fieldIndex];
	
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].x = x;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].y = y;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].pointColor = COLOR_POINT;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].backColor = COLOR_BACK;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].recordPointColor = COLOR_POINT;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].recordBackColor = COLOR_BACK;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].width = 30;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].fontSize = 24;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].rowDistance = 0;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].columnDistance = 0;
	g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lenth = 102;
			break;
		case OBJECT_FORCE:
			g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lenth = 120;
			break;
		case OBJECT_STRENGTH:
			g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lenth = 120;		
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageTableTestResultOneRectangleFrameCoordinate
 * Description    : ����������GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageTableTestResultOneRowRectangleFrameCoordinate( uint8_t rowIndex, uint16_t startX, uint16_t startY )
{
	uint8_t fieldIndex = 0;
	uint16_t x = startX;
	uint16_t y = startY;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.tableFieldNum; ++fieldIndex)
	{
		ConfigMainPageTableTestResultOneFeildRectangleFrameCoordinate(rowIndex,fieldIndex,x,y);
		
		x += g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lenth - g_mainPage.oneLevelMenuTableContent[rowIndex][fieldIndex].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageTableTestResultOneRectangleFrameCoordinate
 * Description    : ����������GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageTableTestResultRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 409;
	const uint16_t START_Y = 150; 
	uint16_t x = START_X;
	uint16_t y = START_Y;
	uint8_t rowIndex = 0;
	uint8_t fieldIndex = 0;
	
	ConfigMainPageTableTestResultOneRowRectangleFrameCoordinate(0,x,y);
	
	memcpy(g_mainPage.oneLevelMenuTableTitle,g_mainPage.oneLevelMenuTableContent,MAX_TEST_RESULT_TABLE_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
	
	if (g_mainPage.curPageSampleNum)
	{
		y += (g_mainPage.oneLevelMenuTableTitle[0].width - g_mainPage.oneLevelMenuTableTitle[0].lineWidth);
		
		for (rowIndex=0; rowIndex<g_mainPage.curPageSampleNum; ++rowIndex)
		{
			ConfigMainPageTableTestResultOneRowRectangleFrameCoordinate(rowIndex,x,y);
			y += g_mainPage.oneLevelMenuTableContent[rowIndex][0].width - g_mainPage.oneLevelMenuTableContent[rowIndex][0].lineWidth;
		}	
		
		memcpy(g_mainPage.oneLevelMenuTableAvailValue,g_mainPage.oneLevelMenuTableContent[g_mainPage.curPageSampleNum-1],MAX_TEST_RESULT_TABLE_FIELD_NUM * sizeof(ONE_LEVEL_MENU_TYPE_TypeDef));
		for (fieldIndex=0; fieldIndex<g_mainPage.tableFieldNum; ++fieldIndex)
		{
			g_mainPage.oneLevelMenuTableAvailValue[fieldIndex].y += \
			g_mainPage.oneLevelMenuTableContent[g_mainPage.curPageSampleNum-1][0].width - \
			g_mainPage.oneLevelMenuTableContent[g_mainPage.curPageSampleNum-1][0].lineWidth;
		}
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawOneIndicateWindowsRectangleFrame
 * Description    : ��1��ʾֵ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawOneIndicateWindowsRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.indicateWindowNum; ++fieldIndex)
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
 * Function Name  : GUI_MainPageDrawIndicateWindowsRectangleFrame
 * Description    : ��ʾֵ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawIndicateWindowsRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<INDICATE_WINDOWS_ROW_NUMS; ++i)
	{
		GUI_MainPageDrawOneIndicateWindowsRectangleFrame(g_mainPage.oneLevelMenuIndicateWindows[i]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawOneRowTitleRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawOneRowRectangleFrame( ONE_LEVEL_MENU_TYPE_TypeDef *pMenu )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.tableFieldNum; ++fieldIndex)
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
 * Function Name  : GUI_MainPageRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageRectangleFrame( void )
{
	uint8_t i;
	
	GUI_MainPageDrawOneRowRectangleFrame(g_mainPage.oneLevelMenuTableTitle);
	
	for (i=0; i<g_mainPage.curPageSampleNum; ++i)
	{
		GUI_MainPageDrawOneRowRectangleFrame(g_mainPage.oneLevelMenuTableContent[i]);
	}
	
	GUI_MainPageDrawOneRowRectangleFrame(g_mainPage.oneLevelMenuTableAvailValue);
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageIndicateWindowOneField
 * Description    : ���˵��ֶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageIndicateWindowOneField( uint8_t rowIndex )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint16_t lenth = 0;
	uint16_t width = 0;
	uint8_t fieldLen = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.indicateWindowNum; ++fieldIndex)
	{		
		x = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x + \
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		y = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y + \
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		
		fieldLen = strlen(g_mainPage.pIndicateWindowsTitleNameArray[fieldIndex]) * \
				   (g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].fontSize>>1);
		lenth = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth - \
				2 * g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		width = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].width - \
				2 * g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		
		pointColor = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].pointColor;
		backColor = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].backColor;
		
		lcd_fill(x,y,lenth,width,backColor);
		
		x = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth;
		y = g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].y + \
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lineWidth + 1;
		
		if (!rowIndex)
		{
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.pIndicateWindowsTitleNameArray[fieldIndex]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageIndicateWindowField
 * Description    : ʾֵ���˵��ֶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageIndicateWindowField( void )
{
	uint8_t rowIndex = 0;
	
	for (rowIndex=0; rowIndex<INDICATE_WINDOWS_ROW_NUMS; ++rowIndex)
	{		
		GUI_MainPageIndicateWindowOneField(rowIndex);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageTableTitleField
 * Description    : ���˵��ֶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageTableTitleField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint8_t fieldLen = 0;
	uint16_t lenth = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.tableFieldNum; ++fieldIndex)
	{
		fieldLen = strlen(g_mainPage.pTestResultTitleNameArray[fieldIndex]) * \
				   (g_mainPage.oneLevelMenuTableTitle[fieldIndex].fontSize>>1);
		lenth = g_mainPage.oneLevelMenuTableTitle[fieldIndex].lenth - \
				2 * g_mainPage.oneLevelMenuTableTitle[fieldIndex].lineWidth;
		
		x = g_mainPage.oneLevelMenuTableTitle[fieldIndex].x + ((lenth - fieldLen) >> 1) + \
			g_mainPage.oneLevelMenuTableTitle[fieldIndex].lineWidth;
		y = g_mainPage.oneLevelMenuTableTitle[fieldIndex].y + \
			g_mainPage.oneLevelMenuTableTitle[fieldIndex].lineWidth;
		
		pointColor = g_mainPage.oneLevelMenuTableTitle[fieldIndex].pointColor;
		backColor = g_mainPage.oneLevelMenuTableTitle[fieldIndex].backColor;
		
		GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.pTestResultTitleNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageShowFrame
 * Description    : ��ʾ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageShowFrame( void )
{
	const uint16_t START_Y = 90;
	const uint16_t LENTH = 2;
	const uint16_t WIDTH = 337;
	
	lcd_fill(0,START_Y,LENTH,WIDTH,COLOR_POINT);
	lcd_fill(360,START_Y,LENTH,WIDTH,COLOR_POINT);
	lcd_fill(LCD_LENTH_X-2,START_Y,LENTH,WIDTH,COLOR_POINT);
}	

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageShowFrame
 * Description    : ��ʾ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageShowTestTitle( void )
{
	uint16_t x,y,fontLenth;
	const uint8_t fontSize = 32;
	const uint16_t lenth = 439;
	
	fontLenth = strlen(g_mainPage.pTestTitle) * (fontSize >> 1);
	x = 360 + ( (lenth - fontLenth) >> 1);
	y = 100;
	
	GUI_DispStr32At(x,y,ORANGE,COLOR_BACK,g_mainPage.pTestTitle);
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageStatusBarRectangleFrameCoordinate
 * Description    : ����״̬��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageStatusBarOneRectangleFrameCoordinate( uint8_t fieldIndex, uint16_t x, uint16_t y )
{
	uint8_t handle = g_mainPage.indexStatusBarArray[fieldIndex];
	
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].x = x;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].y = y;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].pointColor = COLOR_POINT;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].backColor = LIGHT_GRAY2;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].recordPointColor = COLOR_POINT;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].recordBackColor = LIGHT_GRAY2;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].width = 22;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].fontSize = 16;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].rowDistance = 0;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].columnDistance = 0;
	g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth = 2;
	
	switch ( handle )
	{
		case OBJECT_COMM_STATUS:
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth = 150;
			break;
		case OBJECT_TEST_STATUS:
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth = 150;
			break;
		case OBJECT_CODE:
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth = 150;		
			break;
		case OBJECT_TIME:
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth = 254;	
			break;
		case OBJECT_PERIPHERAL_STATUS:
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth = 104;		
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageStatusBarRectangleFrameCoordinate
 * Description    : ����״̬��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageStatusBarRectangleFrameCoordinate( void )
{
	uint8_t fieldIndex;
	const uint16_t START_X = 0;
	const uint16_t START_Y = LCD_WIDTH_Y - 54;
	uint16_t x = START_X;
	uint16_t y = START_Y;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.statusBarNum; ++fieldIndex)
	{
		ConfigMainPageStatusBarOneRectangleFrameCoordinate(fieldIndex,x,y);
		
		x += g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth - g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth; 
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawStatusBarRectangleFrame
 * Description    : ��״̬������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawStatusBarRectangleFrame( void )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.statusBarNum; ++fieldIndex)
	{
		rectangle.x = g_mainPage.oneLevelMenuStatusBar[fieldIndex].x;
		rectangle.y = g_mainPage.oneLevelMenuStatusBar[fieldIndex].y;
		rectangle.lenth = g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth;
		rectangle.width = g_mainPage.oneLevelMenuStatusBar[fieldIndex].width;
		rectangle.lineWidth = g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		rectangle.lineColor = g_mainPage.oneLevelMenuStatusBar[fieldIndex].pointColor;
		
		GUI_DrawRectangleFrame(&rectangle);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageStatusBarField
 * Description    : ���˵��ֶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageStatusBarField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t fieldIndex = 0;
	uint16_t lenth = 0;
	uint16_t width = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.statusBarNum; ++fieldIndex)
	{		
		x = g_mainPage.oneLevelMenuStatusBar[fieldIndex].x + \
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		y = g_mainPage.oneLevelMenuStatusBar[fieldIndex].y + \
			g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		
		lenth = g_mainPage.oneLevelMenuStatusBar[fieldIndex].lenth - \
				2 * g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		width = g_mainPage.oneLevelMenuStatusBar[fieldIndex].width - \
				2 * g_mainPage.oneLevelMenuStatusBar[fieldIndex].lineWidth;
		
		pointColor = COLOR_BACK;//g_mainPage.oneLevelMenuStatusBar[fieldIndex].pointColor;
		backColor = g_mainPage.oneLevelMenuStatusBar[fieldIndex].backColor;
		
		lcd_fill(x,y,lenth,width,backColor);
		
		GUI_DispStr16At(x+2,y+1,pointColor,backColor,g_mainPage.pStatusBarTitleNameArray[fieldIndex]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageLoadPageInfomation
 * Description    : ����ҳ����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageLoadPageInfomation( void )
{
	lcd_show_image(551,390,PIC_DIR_LEFT_RIGHT);
}	

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawTetResultArea
 * Description    : ��ʵ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawTetResultArea( void )
{
	if (g_mainPage.curPage)
	{
		/* ��������� */
		ConfigMainPageTableTestResultRectangleFrameCoordinate();	
		GUI_MainPageRectangleFrame();		
		GUI_MainPageTableTitleField();	
	}
	else
	{
		/* ������Ϣ�� */
		LoadMainPageGetTestInfomationPage();
	}
	
	GUI_MainPageLoadPageInfomation();
}

/*------------------------------------------------------------
 * Function Name  : LoadDefaultCoordinate
 * Description    : ����Ĭ������ϵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadDefaultCoordinate( void )
{
	uint32_t maxForce = 0,maxTime = 0;
	
	maxForce = smpl_ctrl_full_p_get(g_mainPage.tureChannel);
	
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		maxForce /= 1000;
	}
	
	maxForce >>= 1;
	
	maxTime = 20;
	
	GUI_MainPageDrawCoordinate(maxForce,maxTime);
	
	InitMainPageCoordinateDrawLine();
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPage
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPage( void )
{	
	/* ʾֵ�� */
	ConfigMainPageIndicateWindowsRectangleFrameCoordinate();
	GUI_MainPageDrawIndicateWindowsRectangleFrame();
	GUI_MainPageIndicateWindowField();
	
	/* ����ϵ */
	LoadDefaultCoordinate();
	
	/* ���������� */
	GUI_MainPageDrawTetResultArea();
	
	/* �м���� */
	GUI_MainPageShowFrame();
	
	/* ���� */
	GUI_MainPageShowTestTitle();
	
	/* ״̬�� */
	ConfigMainPageStatusBarRectangleFrameCoordinate();
	GUI_MainPageDrawStatusBarRectangleFrame();
	GUI_MainPageStatusBarField();
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPage
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCheckLinkPCCycle( void )
{
	BoolStatus linkStatus;
	int32_t remainDay = 0;
	EXPIRE_STATUS_TypeDef activeStatus;
	
	if (GetLinkStatus() != LINK_IDLE)
	{
		return;
	}
	
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	linkStatus = CheckLinkStatus();
	
	if (linkStatus == YES)
	{
		activeStatus = JudgeActiveStatus(&remainDay);	
		switch ( activeStatus )
		{
			case EXPIRE_NONE:
			case EXPIRE_NEAR:
			case EXPIRE_DATE_ERR:			
				break;
			
			default:
				ActiveStatusHandler(activeStatus);
				
				g_mainPage.leavePage.flagLeavePage = SET;
				g_mainPage.leavePage.flagSaveData = RESET;
				
				return;
		}
		
		SetPage(LINK_PAGE);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageOneIndicateWindowsContent
 * Description    : ��ʾһ��ʾֵ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageOneIndicateWindowsContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].x + \
				 g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint16_t y = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].y + \
				 g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint16_t pointColor = FUCHSIA_RED;//g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].pointColor;
	uint16_t backColor = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].backColor;
//	uint16_t lenth = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lenth - \
//					2 * g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
//	uint16_t width = g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].width - \
//					2 * g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].lineWidth;
	uint8_t handle = g_mainPage.indexIndicateWindowsArray[indexField];
	
//	lcd_fill(x,y,lenth,width,backColor);
	y += 6;
	
	switch ( handle )
	{
		case OBJECT_WINDOWS_FORCE:
			RefreshDynamicForce(x+43,y,pointColor,backColor,GetInterfaceElementForce());		
			break;
		case OBJECT_WINDOWS_SPEED:
			RefreshDynamicSpeed(x+24,y,pointColor,backColor,GetInterfaceElementSpeed());
			break;
		case OBJECT_WINDOWS_STRENGTH:
			RefreshDynamicStrength(x+12,y,pointColor,backColor,GetInterfaceElementStrength());
			break;
		case OBJECT_WINDOWS_PEAK:
			RefreshDynamicPeak(x+43,y,pointColor,backColor,GetInterfaceElementPeak());
			break;
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_MainPageOneStatusBarContent
 * Description    : ��ʾһ��״̬������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageOneStatusBarContent( uint8_t indexField )
{
	uint16_t x = g_mainPage.oneLevelMenuStatusBar[indexField].x + \
				 g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
	uint16_t y = g_mainPage.oneLevelMenuStatusBar[indexField].y + \
				 g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
	uint16_t pointColor = FUCHSIA_RED;//g_mainPage.oneLevelMenuIndicateWindows[indexRow][indexField].pointColor;
	uint16_t backColor = g_mainPage.oneLevelMenuStatusBar[indexField].backColor;
//	uint16_t lenth = g_mainPage.oneLevelMenuStatusBar[indexField].lenth - \
//					2 * g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
//	uint16_t width = g_mainPage.oneLevelMenuStatusBar[indexField].width - \
//					2 * g_mainPage.oneLevelMenuStatusBar[indexField].lineWidth;
	uint8_t handle = g_mainPage.indexStatusBarArray[indexField];
	uint8_t fontSize = g_mainPage.oneLevelMenuStatusBar[indexField].fontSize;
	
//	lcd_fill(x,y,lenth,width,backColor);
	
	x += strlen(g_mainPage.pStatusBarTitleNameArray[indexField]) * (fontSize>>1) + 10;
	y += 1;
	
	switch ( handle )
	{
		case OBJECT_COMM_STATUS:	
			RefreshDynamicCommunicationStatus(x,y,pointColor,backColor,GetInterfaceLinkStatus());
			break;
		case OBJECT_TEST_STATUS:
			RefreshDynamicTestStatus(x,y,pointColor,backColor,GetInterfaceTestStatus());
			break;
		case OBJECT_CODE:
			RefreshDynamicCode(x,y,pointColor,backColor,GetInterfaceElementCode());		
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
 * Function Name  : RefreshDynamicContent
 * Description    : ˢ�¶�̬����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void RefreshDynamicContent( void )
{
	static uint8_t s_refreshIndex = REFRESH_FORCE;
	const uint8_t endIndex = REFRESH_PERIPHERAL_STATUS;
	uint8_t indexField = 0;
	
	s_refreshIndex++;
	s_refreshIndex %= endIndex + 1;
	
	switch ( s_refreshIndex )
	{
		case REFRESH_FORCE:
			indexField = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_FORCE);
			Show_MainPageOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_SPEED:
			indexField = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_SPEED);
			Show_MainPageOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_STRENGTH:
			indexField = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_STRENGTH);
			Show_MainPageOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_PEAK:
			indexField = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_PEAK);
			Show_MainPageOneIndicateWindowsContent(1,indexField);
			break;
		case REFRESH_COMMUNICATION_STATUS:
			indexField = GetMainPageStatusBarFieldIndex(OBJECT_COMM_STATUS);
			Show_MainPageOneStatusBarContent(indexField);
			break;
		case REFRESH_TEST_STATUS:
			indexField = GetMainPageStatusBarFieldIndex(OBJECT_TEST_STATUS);
			Show_MainPageOneStatusBarContent(indexField);
			break;
		case REFRESH_CODE:
			indexField = GetMainPageStatusBarFieldIndex(OBJECT_CODE);
			Show_MainPageOneStatusBarContent(indexField);
			break;
		case REFRESH_TIME:
			indexField = GetMainPageStatusBarFieldIndex(OBJECT_TIME);
			Show_MainPageOneStatusBarContent(indexField);
			break;
		case REFRESH_PERIPHERAL_STATUS:
			indexField = GetMainPageStatusBarFieldIndex(OBJECT_PERIPHERAL_STATUS);
			Show_MainPageOneStatusBarContent(indexField);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ClearMainPageCoordinate
 * Description    : �������ϵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ClearMainPageCoordinate( uint16_t color )
{
	lcd_fill(4,110,350,290,color);
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawCoordinate
 * Description    : ������ϵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawCoordinate( uint32_t maxForce, uint32_t maxTime )
{
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	pCoordinate->x = 75;
	pCoordinate->y = 120;
	pCoordinate->rowSpace = 50;
	pCoordinate->columnSpace = 50;
	pCoordinate->xLenth = 250;
	pCoordinate->yLenth = 250;
	pCoordinate->fillFieldLenth = 5;
	pCoordinate->emptyFieldLenth = 5;
	pCoordinate->lineWidth = 1;
	pCoordinate->rowFieldNum = 5;
	pCoordinate->columnFieldNum = 5;
	pCoordinate->mainBackColor = COLOR_BACK;
	pCoordinate->windowsBackColor = COLOR_BACK;
	pCoordinate->rowLineColor = FOREST_GREEN;
	pCoordinate->columnLineColor = FOREST_GREEN;
	pCoordinate->fontPointColor = ORANGE;
	pCoordinate->fontBackColor = COLOR_BACK;
	pCoordinate->xLinePointColor = FRESH_GREEN;
	pCoordinate->yLinePointColor = FRESH_GREEN;
	
	pCoordinate->maxForce = maxForce;
	pCoordinate->maxTime = maxTime;
	pCoordinate->pXUnit = "(S)";
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		pCoordinate->pYUnit = "(kN)";
	}
	else
	{
		pCoordinate->pYUnit = "(N)";
	}
	ClearMainPageCoordinate(COLOR_BACK);
	
	GUI_DrawCoordinate(pCoordinate);
}

/*------------------------------------------------------------
 * Function Name  : Show_DetailReportOneFieldContent
 * Description    : ��ʾһ���ֶ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageTestResultOneFieldTableContent( uint8_t indexRow, uint8_t indexField )
{
	uint16_t x = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].x +\
				 g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lineWidth;
	uint16_t y = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].y + \
				 g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lineWidth;
	uint16_t lenth = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lenth -\
					 2 * g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lineWidth;
	uint16_t width = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].width -\
					 2 * g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lineWidth;
	uint16_t pointColor = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].pointColor;
	uint16_t backColor = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].backColor;
	uint16_t fontLenth = 0;
	uint16_t rectLenth = g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lenth - \
						(2 * g_mainPage.oneLevelMenuTableContent[indexRow][indexField].lineWidth);
	uint16_t handle = g_mainPage.indexTestResultTableArray[indexField];
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			fontLenth = strlen(g_mainPage.testResultData[indexRow].serial) * \
						(g_mainPage.oneLevelMenuTableContent[indexRow][indexField].fontSize>>1);
			break;
		case OBJECT_FORCE:
			fontLenth = strlen(g_mainPage.testResultData[indexRow].force) * \
						(g_mainPage.oneLevelMenuTableContent[indexRow][indexField].fontSize>>1);
			break;
		case OBJECT_STRENGTH:
			fontLenth = strlen(g_mainPage.testResultData[indexRow].strength) * \
						(g_mainPage.oneLevelMenuTableContent[indexRow][indexField].fontSize>>1);
			break;
	}
	
	lcd_fill(x,y,lenth,width,backColor);	
	
	if (g_mainPage.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultData[indexRow].serial);
			break;
		case OBJECT_FORCE:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultData[indexRow].force);
			break;
		case OBJECT_STRENGTH:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultData[indexRow].strength);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageTestResultTable
 * Description    : ��ʾ���������(rowNum����1��ʼ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageTestResultTable( uint8_t rowNum, uint8_t handle )
{
	uint8_t indexField = GetMainPageTestResultFieldIndex(handle);
	
	if (indexField != 0xff)
	{
		if ((rowNum>=1) && (rowNum<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
		{
			Show_MainPageTestResultOneFieldTableContent(rowNum-1,indexField);
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : Traverse_MainPageTestResultTableContent
 * Description    : �����������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_MainPageTestResultTableContent( void )
{
	uint8_t indexRow = 0;
	uint8_t sampleNum = 0;
	uint8_t pageNum = 0;
	
	for (indexRow=1; indexRow<=g_mainPage.curPageSampleNum; ++indexRow)
	{
		Show_MainPageTestResultTable(indexRow,OBJECT_SERIAL);
	}
	
	MainPageClearCursor(g_testBody.curCompletePieceSerial);
	MainPageLoadCursor(g_testBody.curCompletePieceSerial+1);
		
	pageNum = GetCompletePiecePageNum();
	if (g_mainPage.curPage < pageNum)
	{
		sampleNum = g_mainPage.curPageSampleNum;
	}
	else if (g_mainPage.curPage == pageNum)
	{
		sampleNum = GetCompletePieceSerialInCurrentPageSerial();
	}
	else
	{
		return;
	}
	
	for (indexRow=1; indexRow<=sampleNum; ++indexRow)
	{		
		Show_MainPageTestResultTable(indexRow,OBJECT_FORCE);
		Show_MainPageTestResultTable(indexRow,OBJECT_STRENGTH);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageTestResultAvailValueOneFieldTableContent
 * Description    : ��ʾ��Чֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageTestResultAvailValueOneFieldTableContent( uint8_t indexField )
{
	uint16_t x = g_mainPage.oneLevelMenuTableAvailValue[indexField].x +\
				 g_mainPage.oneLevelMenuTableAvailValue[indexField].lineWidth;
	uint16_t y = g_mainPage.oneLevelMenuTableAvailValue[indexField].y + \
				 g_mainPage.oneLevelMenuTableAvailValue[indexField].lineWidth;
	uint16_t lenth = g_mainPage.oneLevelMenuTableAvailValue[indexField].lenth -\
					 2 * g_mainPage.oneLevelMenuTableAvailValue[indexField].lineWidth;
	uint16_t width = g_mainPage.oneLevelMenuTableAvailValue[indexField].width -\
					 2 * g_mainPage.oneLevelMenuTableAvailValue[indexField].lineWidth;
	uint16_t pointColor = g_mainPage.oneLevelMenuTableAvailValue[indexField].pointColor;
	uint16_t backColor = g_mainPage.oneLevelMenuTableAvailValue[indexField].backColor;
	uint16_t fontLenth = 0;
	uint16_t rectLenth = g_mainPage.oneLevelMenuTableAvailValue[indexField].lenth - \
						(2 * g_mainPage.oneLevelMenuTableAvailValue[indexField].lineWidth);
	uint16_t handle = g_mainPage.indexTestResultTableArray[indexField];
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			fontLenth = strlen(g_mainPage.testResultAvailData.availValue) * \
						(g_mainPage.oneLevelMenuTableAvailValue[indexField].fontSize>>1);
			break;
		case OBJECT_FORCE:
			fontLenth = strlen(g_mainPage.testResultAvailData.availForce) * \
						(g_mainPage.oneLevelMenuTableAvailValue[indexField].fontSize>>1);
			break;
		case OBJECT_STRENGTH:
			fontLenth = strlen(g_mainPage.testResultAvailData.availStrength) * \
						(g_mainPage.oneLevelMenuTableAvailValue[indexField].fontSize>>1);
			break;
	}
	
	lcd_fill(x,y,lenth,width,backColor);	
	
	if (g_mainPage.align[indexField] == ALIGN_MIDDLE)
	{
		x += ((rectLenth - fontLenth) >> 1);
	}
	
	switch ( handle )
	{
		case OBJECT_SERIAL:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultAvailData.availValue);
			break;
		case OBJECT_FORCE:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultAvailData.availForce);
			break;
		case OBJECT_STRENGTH:
			GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.testResultAvailData.availStrength);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageTestResultTableAvailValue
 * Description    : ��ʾ�����������Чֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageTestResultTableAvailValue( uint8_t handle )
{
	uint8_t indexField = GetMainPageTestResultFieldIndex(handle);
	
	if (indexField != 0xff)
	{
		Show_MainPageTestResultAvailValueOneFieldTableContent(indexField);
	}
}

/*------------------------------------------------------------
 * Function Name  : Traverse_MainPageTestResultTableAvailValue
 * Description    : ���������������Чֵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_MainPageTestResultTableAvailValue( void )
{	
	Show_MainPageTestResultTableAvailValue(OBJECT_SERIAL);
	
	if (g_testBody.flagOneGroupSampleComplete == SET)
	{
		Show_MainPageTestResultTableAvailValue(OBJECT_FORCE);
		Show_MainPageTestResultTableAvailValue(OBJECT_STRENGTH);	
	}
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TestResultTable
 * Description    : �������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TestResultTable( void )
{	
	if (g_mainPage.curPage)
	{				
		Traverse_MainPageTestResultTableContent();
		
		Traverse_MainPageTestResultTableAvailValue();
	}
}

/*------------------------------------------------------------
 * Function Name  : IsCompleteSampleSerialAtThisPage
 * Description    : ��������Կ�ı���Ƿ��ڵ�ǰҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus IsCompleteSampleSerialAtThisPage( uint8_t nowSampleSerial, uint8_t *posIndex )
{
	uint8_t index;
	uint8_t sampleSerial = 0;
	
	for (index=0; index<g_mainPage.curPageSampleNum; ++index)
	{
		sampleSerial = (uint8_t)ustrtoul(g_mainPage.testResultData[index].serial,0,10);
		
		if (sampleSerial == nowSampleSerial)
		{
			*posIndex = index;
			
			return YES;
		}
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : MainPageClearCursor
 * Description    : ������
 * Input          : nowSampleSerial����ǰ�����Կ����
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageClearCursor( uint8_t nowSampleSerial )
{
	uint8_t serialIndex = 0;
	BoolStatus isFindSampleSerial = NO;
	uint8_t indexField = 0;
	uint8_t sampleSerial = 0;
	
	if (g_mainPage.curPage == 0)
	{
		return;
	}
	
	/* �����һ����� */
	isFindSampleSerial = IsCompleteSampleSerialAtThisPage(nowSampleSerial,&serialIndex);
	
	if (isFindSampleSerial == YES)
	{
		indexField = GetMainPageTestResultFieldIndex(OBJECT_SERIAL);
		if (indexField == 0xff)
		{
			return;
		}
	
		g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].pointColor = \
			g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].recordPointColor;
		g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].backColor = \
			g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].recordBackColor;
		
		sampleSerial = serialIndex + 1;
		Show_MainPageTestResultTable(sampleSerial,OBJECT_SERIAL);
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageLoadNextCursor
 * Description    : �ƶ���굽��һ��δ���������
 * Input          : nowSampleSerial����ǰ�����Կ����
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageLoadCursor( uint8_t nowSampleSerial )
{
	uint8_t serialIndex = 0;
	BoolStatus isFindSampleSerial = NO;
	uint8_t indexField = 0;
	uint8_t sampleSerial = 0;
	
	if (g_mainPage.curPage == 0)
	{
		return;
	}
	
	/* ������һ����� */
	isFindSampleSerial = IsCompleteSampleSerialAtThisPage(nowSampleSerial,&serialIndex);
		
	if (isFindSampleSerial == YES)
	{
		indexField = GetMainPageTestResultFieldIndex(OBJECT_SERIAL);
		if (indexField == 0xff)
		{
			return;
		}
	
		g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].pointColor = COLOR_SELECT_POINT;
		g_mainPage.oneLevelMenuTableContent[serialIndex][indexField].backColor = COLOR_SELECT_BACK;
		
		sampleSerial = serialIndex + 1;
		Show_MainPageTestResultTable(sampleSerial,OBJECT_SERIAL);
	}
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
	lcd_fill(380,140,410,240,color);
}

/*------------------------------------------------------------
 * Function Name  : ReloadTestResultArea
 * Description    : ���¼�������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReloadTestResultArea( void )
{
	ClearTestResultArea(COLOR_BACK);
	
	/* ������ʼ�� */
	MainPageInit();
	
	/* ��ȡ���� */
	MainPageReadParameter();
	
	/* ���������� */
	GUI_MainPageDrawTetResultArea();
	
	/* ���� */
	Traverse_TestResultTable();
}

/*------------------------------------------------------------
 * Function Name  : MainPageLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageLeavePageCheckCycle( void )
{
	if (g_mainPage.leavePage.flagLeavePage == SET)
	{
		if (g_mainPage.leavePage.flagSaveData == SET)
		{
			;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_mainPage.refreshShortcut == ENABLE)
	{
		g_mainPage.refreshShortcut = DISABLE;
			
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
		switch ( GetTestStatus() )
		{
			case TEST_LOAD:
				pShortCut->status = NULL;
				break;
			case TEST_IDLE:
				pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
					
				if (g_testBody.flagTestAfterDispose == SET)
				{
					pShortCut->pContent[0] = pTwoLevelMenu[24];
					pShortCut->pContent[1] = pTwoLevelMenu[36];
					pShortCut->pContent[2] = pTwoLevelMenu[64];
					pShortCut->pContent[3] = pTwoLevelMenu[74];
				}
				else
				{
					pShortCut->pContent[0] = pTwoLevelMenu[1];
					pShortCut->pContent[1] = pTwoLevelMenu[27];
					pShortCut->pContent[2] = pTwoLevelMenu[59];
					pShortCut->pContent[3] = pTwoLevelMenu[77];
				}
				break;
			default:
				pShortCut->status = NULL;
				break;
		}
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : PageTurningLeft
 * Description    : ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTurningPageLeft( void )
{
	if (g_mainPage.sumPage == 0)
	{
		return;
	}
	
	if (g_mainPage.curPage)
	{
		g_mainPage.curPage--;
	}
	else
	{
		g_mainPage.curPage = g_mainPage.sumPage;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageTurningPageRight
 * Description    : ���ҷ�ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTurningPageRight( void )
{
	if (g_mainPage.sumPage == 0)
	{
		return;
	}
	
	g_mainPage.curPage++;
	
	g_mainPage.curPage %= (g_mainPage.sumPage + 1);
}

/*------------------------------------------------------------
 * Function Name  : FindMatchSerialInfomation
 * Description    : ��ȡ����ƥ����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus FindMatchSerialInfomation( TEST_INFO_TypeDef *test_info, uint8_t test_type )
{
	uint32_t i = 0,index = 0,AllNum = 0;	
	const uint8_t MAX_NUM_ONCE = 10;
	TEST_INFO_TypeDef TestInfoBuff[11];
	uint32_t Cnt = 0;
	
	report_search_sn(test_type,pTest->test_serial,0,TestInfoBuff,&AllNum);
	
	if ( AllNum == 0)
	{
		return ERROR;
	}
	
	Cnt = AllNum;
	
	while ( Cnt )
	{
		if ( Cnt >= MAX_NUM_ONCE )
		{
			Cnt -= MAX_NUM_ONCE;
		}
		else
		{
			Cnt = 0;
		}
		
		report_search_sn(test_type,pTest->test_serial,index,TestInfoBuff,&AllNum);
		
		for (i=0; i<MAX_NUM_ONCE; ++i)
		{					
			if ( strcmp(TestInfoBuff[i].fname,pTest->test_serial) == 0)
			{
				*test_info = TestInfoBuff[i];
				
				return SUCCESS;
			}
		}
		
		index += MAX_NUM_ONCE;
	}
	
	test_info = NULL;
	
	return ERROR;
}

/*------------------------------------------------------------
 * Function Name  : MainPagePrintReport
 * Description    : ��ӡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPagePrintReport( void )
{
	ErrorStatus errStatus;
	REPORT_TypeDef readReport;
	TEST_INFO_TypeDef testInfo;
	FRESULT result;
	
	if (g_testBody.curCompletePieceSerial == 0)
	{
		return;
	}
	
	ShowPrintingShortcutMenu();
	
	if (SUCCESS == FindMatchSerialInfomation(&testInfo,g_mainPage.testType) )
	{
		result = report_read(g_mainPage.testType,pTest->test_serial,&readReport);
		
		if (result == FR_OK)
		{
			errStatus = PrintTestReport(g_mainPage.showChannel,g_mainPage.testType,&readReport,&testInfo);
		}
		else
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[12]);
			
			errStatus = ERROR;
		}
	}
	else
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[10]);
		
		errStatus = ERROR;
	}
	
	if (errStatus == ERROR)
	{
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
	}
			
	g_mainPage.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : MainPageExportReport
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExportReport( void )
{
	REPORT_TypeDef readReport;
	FRESULT result;
	
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	if (g_testBody.curCompletePieceSerial == 0)
	{
		return;
	}
	
	if (Get_USB_Status() == ERROR)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[13]);							
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
	
	ShowExportingShortcutMenu();
	
	GUI_DrawTestSaveProgressBar(LIGHT_GRAY2);
	
	result = report_read(g_mainPage.testType,pTest->test_serial,&readReport);
	if (result != FR_OK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[10]);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
	
	result = report_save_usb(g_mainPage.testType,pTest->test_serial,&readReport);
	if (result != FR_OK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[11]);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
		
	result = report_save_usb_set_time(g_mainPage.testType,pTest->test_serial);	
	if (result != FR_OK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[11]);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
	
	g_mainPage.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : MainPagePressPrintKey
 * Description    : ���´�ӡ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPagePressPrintKey( void )
{
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	MainPagePrintReport();
}

/*------------------------------------------------------------
 * Function Name  : MainPagePressKEY_F1
 * Description    : ����F1
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPagePressKEY_F1( void )
{
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	if (g_testBody.flagTestAfterDispose == SET)
	{
		MainPagePrintReport();
	}
	else
	{
		if ((g_testBody.flagOneGroupSampleComplete==RESET) && (g_testBody.curCompletePieceSerial))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,2,&pMainPageWarning[4]);
				
			g_mainPage.leavePage.flagLeavePage = SET;
			g_mainPage.leavePage.flagSaveData = RESET;
			
			return;
		}
	
		SetPage(TEST_PARAMETER_PAGE);
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		g_testBody.flagAutoUpdateSerial = SET;
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageCancelLastPiece
 * Description    : ������һ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCancelLastPiece( void )
{
	if (g_testBody.flagOneGroupSampleComplete == SET)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,2,&pMainPageWarning[6]);
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
	
	if (g_testBody.curCompletePieceSerial == 0)
	{	
		return;
	}
	
	if (GetCompletePiecePageNum() != g_mainPage.curPage)
	{
		return;
	}
	
	g_testBody.flagCancelLastPiece = SET;
	
	SetTestStatus(TEST_SAVE);
}

/*------------------------------------------------------------
 * Function Name  : MainPagePressKEY_F2
 * Description    : ����F2
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPagePressKEY_F2( void )
{
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	if (g_testBody.flagTestAfterDispose == SET)
	{
		MainPageCancelLastPiece();
	}
	else
	{
		SetPage(TEST_REPORT_PAGE);
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageEndOneGroup
 * Description    : ����һ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageEndOneGroup( void )
{
	if (g_testBody.flagOneGroupSampleComplete == SET)
	{	
		return;
	}
	
	if (g_testBody.curCompletePieceSerial == 0)
	{	
		return;
	}
	
	g_testBody.flagHandEndOneGroup = SET;
	
	SetTestStatus(TEST_SAVE);
}

/*------------------------------------------------------------
 * Function Name  : MainPagePressKEY_F3
 * Description    : ����F3
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPagePressKEY_F3( void )
{
	if (GetTestStatus() != TEST_IDLE)
	{
		return;
	}
	
	if (g_testBody.flagTestAfterDispose == SET)
	{
		MainPageEndOneGroup();
	}
	else
	{
		/* �����û���¼ */
		SetPasswordType(USER_OR_MANAGER_PASSWORD);
		SetPasswordPutinSourcePage(MAIN_PAGE);
		SetPasswordPutinTargetPage(CHANNEL_SELECT_PAGE);
		SetPage(USER_LOGIN_PAGE);
		
		/* ����ͨ��ѡ�� */
		SetChannelSelectSourcePage(MAIN_PAGE);
		SetChannelSelectTargetPage(SYSTEM_SET);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		InitLssuedParameter();
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F1:
				MainPagePressKEY_F1();
				break;
			case KEY_F2:
				MainPagePressKEY_F2();
				break;
			case KEY_F3:
				MainPagePressKEY_F3();
				break;
			case KEY_F4:
				if (GetTestStatus() != TEST_IDLE)
				{
					return;
				}
			
				g_testBody.flagTestAfterDispose = (g_testBody.flagTestAfterDispose == SET) \
												  ? RESET : SET;
				g_mainPage.refreshShortcut = ENABLE;
				break;
			case KEY_LEFT:
				if (GetTestStatus() != TEST_IDLE)
				{
					return;
				}
				MainPageTurningPageLeft();
				ReloadTestResultArea();
				break;
			case KEY_RIGHT:
				if (GetTestStatus() != TEST_IDLE)
				{
					return;
				}
				MainPageTurningPageRight();
				ReloadTestResultArea();
				break;
			case KEY_FORCE_TARE:		
				if (SendChannelTareCmd(g_mainPage.tureChannel) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[3]);
			
					g_mainPage.leavePage.flagLeavePage = SET;
					g_mainPage.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_PRINT:
				MainPagePressPrintKey();
				break;
			case KEY_EXPORT:
				MainPageExportReport();
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentForce
 * Description    : ���ö�̬������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentForce( void )
{
	float force = 0;
	
	force = get_smpl_force(g_mainPage.tureChannel);
	
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		force /= 1000;
	}

	SetInterfaceElementForce(force);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentSpeed
 * Description    : ���ö�̬�����ٶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentSpeed( void )
{
	float speed = 0;
	
	speed = get_smpl_spd(g_mainPage.tureChannel);
	
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		speed /= 1000;
	}

	SetInterfaceElementSpeed(speed);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentStrength
 * Description    : ���ö�̬����ǿ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentStrength( void )
{
	float strength = 0;
	float force = 0;
	
	if (GetTestStatus() != TEST_IDLE)
	{
		force = get_smpl_force(g_mainPage.tureChannel);
	
		strength = FromForceGetStrength(g_mainPage.testType,&g_readReport,force);
	}
	
	if (GetTestStatus() == TEST_LOAD)
	{	
		g_testBody.breakStrength = FromForceGetStrength(g_mainPage.testType,&g_readReport,g_testBody.breakPeak);
	}

	SetInterfaceElementStrength(strength);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentPeak
 * Description    : ���ö�̬���ݷ�ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentPeak( void )
{
	static uint32_t s_peakDelayTime = 0;
	float peak = 0;
	
	switch ( g_testBody.peakStatus )
	{
		case STATUS_PEAK_IDLE:
			g_testBody.breakPeak = 0;
			if (GetTestStatus() == TEST_LOAD)
			{
				g_testBody.peakStatus = STATUS_PEAK_SHOW;
			}
			break;
		case STATUS_PEAK_SHOW:
			if (GetTestStatus() == TEST_LOAD)
			{
				g_testBody.breakPeak = GetPeakValue(g_mainPage.tureChannel);
			}
		
			if (GetTestStatus() == TEST_IDLE)
			{
				s_peakDelayTime = GetCurveStayTime() * CTRL_FREQ;
				g_testBody.peakStatus = STATUS_PEAK_KEEP;
			}
			break;
		case STATUS_PEAK_KEEP:
			if (GetTestStatus() == TEST_LOAD)
			{
				g_testBody.peakStatus = STATUS_PEAK_IDLE;
			}
			
			if (s_peakDelayTime)
			{
				s_peakDelayTime--;
			}
			else
			{
				g_testBody.peakStatus = STATUS_PEAK_IDLE;
				
				LoadDefaultCoordinate();
			}
			break;
	}
	
	peak = g_testBody.breakPeak;
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		peak /= 1000;
	}
	
	SetInterfaceElementPeak(peak);
}	

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentTestStatus
 * Description    : ���ö�̬��������״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentTestStatus( void )
{
	SetInterfaceTestStatus(GetTestStatus());
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentTask
 * Description    : ���ö�̬����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentTask( void )
{
	SetDynamicContentForce();
	SetDynamicContentSpeed();
	SetDynamicContentPeak();
	SetDynamicContentStrength();
	SetDynamicContentLinkStatus();
	SetDynamicContentTestStatus();
	SetDynamicContentCode(g_mainPage.tureChannel);
	SetDynamicContentUSBStatus();
	SetDynamicContentNETStatus();
}

/*------------------------------------------------------------
 * Function Name  : MainPageCheckSerialRepeat
 * Description    : ������ظ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus MainPageCheckSerialRepeat( void )
{
	char str_buff[TEST_INFOMATION_FAILED_MAX_CHAR_NUM+1] = {0};
	uint8_t len = 0;
	SERIAL_ADD_ONE_STA SerialStatus;
	
	if (g_testBody.flagOneGroupSampleComplete == RESET)
	{
		return PASSED;
	}
	
	strcpy(str_buff,pTest->test_serial);
	len = strlen(str_buff);
	
	SerialStatus = TestParameterCheckSerialRepeat(g_mainPage.testType,str_buff,&len);
	switch ( SerialStatus )
	{
		case CHANGE_SERIAL_OK:
			ClearExceptionFlag();	//����������
			strcpy(pTest->test_serial,str_buff);
			pcm_save();
		
			g_mainPage.curPage = 1;
			ReloadTestResultArea();
		
			#ifdef DEBUG_TEST_LOAD
				printf("�Զ���ż�1��\r\n");
			#endif
			return PASSED;
		
		case SERIAL_NO_CHG:
			return PASSED;
		
		default: 
			return FAILED;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageAllowRunTest
 * Description    : ������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FunctionalState MainPageAllowRunTest( void )
{
	int32_t remainDay = 0;
	EXPIRE_STATUS_TypeDef activeStatus;
	
	/* �������״̬ */
	if (GetLinkStatus() == LINK_UNLINK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[0]);
		
		return DISABLE;
	}
	
	/* ����Կ���� */
	if (g_mainPage.sumSampleNum == 0)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[1]);
		
		return DISABLE;
	}
	
	/* ����Լ���� */
	if (MainPageCheckSerialRepeat() == FAILED)
	{
		return DISABLE;
	}
	
	/* ��⼤��״̬ */
	activeStatus = JudgeActiveStatus(&remainDay);	
	
	switch ( activeStatus )
	{
		case EXPIRE_NONE:
		case EXPIRE_NEAR:
		case EXPIRE_DATE_ERR:			
			break;
		
		default:
			ActiveStatusHandler(activeStatus);
			
			return DISABLE;
	}
	
	
	#ifdef DEBUG_TEST_LOAD
		printf("�������飡\r\n");
	#endif
	
	return ENABLE;
}	

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteTestStartBody
 * Description    : ���鿪ʼ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteTestStartBody( void )
{
	uint8_t curPage = 0;
	
	/* ��ʼ������ */
	InitJudgeBreakPoint();
	
	/* ��ת��������ҳ */
	curPage = GetCompletePiecePageNum();
	if(curPage != g_mainPage.curPage)
	{
		g_mainPage.curPage = curPage;
		
		ReloadTestResultArea();
		
		#ifdef DEBUG_TEST_LOAD
			printf("��ת�������Կ�ҳ�棡\r\n");
		#endif
	}
	
	if (IsCurrentSampleReachToPageTail() == YES)
	{
		g_mainPage.curPage++;
		
		ReloadTestResultArea();
		
		#ifdef DEBUG_TEST_LOAD
			printf("��ǰҳ�Կ������꣡\r\n");
		#endif
	}
	else
	{
		MainPageClearCursor(g_testBody.curCompletePieceSerial);
		MainPageLoadCursor(g_testBody.curCompletePieceSerial+1);
	}	
	
	g_testBody.startTest = SET;
	g_testBody.endTest = RESET;
	g_testBody.flagOnePieceSampleComplete = RESET;
	g_testBody.flagOneGroupSampleComplete = RESET;
	g_testBody.flagStartJudgeBreak = RESET;
	
	g_testBody.breakPeak = 0;
	g_testBody.breakStrength = 0;
	
	g_mainPage.refreshShortcut = ENABLE;
	
	SetTestStatus(TEST_LOAD);
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteTestEndBody
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteTestEndBody( void )
{
	g_testBody.startTest = RESET;
	g_testBody.endTest = SET;
	
	if (g_testBody.flagOneGroupSampleComplete == SET)
	{
		g_testBody.flagTestAfterDispose = RESET;
	}
	else
	{
		g_testBody.flagTestAfterDispose = SET;
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageJudgeBreakCoreCycle
 * Description    : ����ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageJudgeBreakCoreCycle( void )
{
	uint8_t nowAttenuationRate = 0;
	uint8_t targetAttenuationRate = 0;
	float nowWithMaxForceDiff = 0;
	float targetWithMaxForceDiff = 0;
	float nowAdjoinTwoPointDiff = 0;
	float targetAdjoinTwoPointDiff = 0;
	uint16_t nowDownPoint = 0;
	uint16_t targetDownPoint = 0;
	BoolStatus isBreak = NO;
	JUDGE_BREAK_TYPE_TypeDef breakType;
	float breakStartValue = 0;
	float curForce = 0;
	
	breakStartValue = GetTargetBreakStartValue(g_mainPage.tureChannel);
	curForce = get_smpl_force(g_mainPage.tureChannel);
	
	if (curForce > breakStartValue)
	{
		if (g_testBody.flagStartJudgeBreak == RESET)
		{
			g_testBody.flagStartJudgeBreak = SET;
			
			#ifdef DEBUG_TEST_LOAD
				printf("����������ֵ��\r\n");
			#endif
		}
	}
	
	if (g_testBody.flagStartJudgeBreak == RESET)
	{
		return;
	}
	
	breakType = GetJudgeBreakType(g_mainPage.tureChannel);
	
	switch ( breakType )
	{
		case ATTENUATION_RATE:	
			nowAttenuationRate = GetAttenuationRate(g_mainPage.tureChannel);
			targetAttenuationRate = GetTargetAttenuationRate(g_mainPage.tureChannel);
			
			if (nowAttenuationRate > targetAttenuationRate)
			{
				#ifdef DEBUG_TEST_LOAD
					printf("˥�������ƣ�\r\n");
				#endif
				
				isBreak = YES;
			}
			break;
		case WITH_MAX_FORCE_DIFFERENCE:
			nowWithMaxForceDiff = GetWithMaxForceDifference(g_mainPage.tureChannel);
			targetWithMaxForceDiff = GetTargetWithMaxForceDifference(g_mainPage.tureChannel);
			nowDownPoint = GetBreakDownPoint(g_mainPage.tureChannel);
			targetDownPoint = GetTargetBreakDownPoint(g_mainPage.tureChannel);
		
			if ((nowWithMaxForceDiff>targetWithMaxForceDiff) && (nowDownPoint>targetDownPoint))
			{
				#ifdef DEBUG_TEST_LOAD
					printf("���������ֵ���ƣ�\r\n");
				#endif
				
				isBreak = YES;
			}
			break;
		case ADJOIN_TWO_POINT_DIFFERENCE:
			nowAdjoinTwoPointDiff = GetAdjoinTwoPointDifference(g_mainPage.tureChannel);
			targetAdjoinTwoPointDiff = GetTargetAdjoinTwoPointDiff(g_mainPage.tureChannel);
			nowDownPoint = GetBreakDownPoint(g_mainPage.tureChannel);
			targetDownPoint = GetTargetBreakDownPoint(g_mainPage.tureChannel);
		
			if ((nowAdjoinTwoPointDiff>targetAdjoinTwoPointDiff) && (nowDownPoint>targetDownPoint))
			{
				#ifdef DEBUG_TEST_LOAD
					printf("���������ֵ���ƣ�\r\n");
				#endif
				
				isBreak = YES;
			}
			break;
		default:
			break;
	}
	
	if (isBreak == YES)
	{
		g_testBody.startTest = RESET;
		g_testBody.flagOnePieceSampleComplete = SET;
		
		g_testBody.curCompletePieceSerial++;
		
		if (g_testBody.curCompletePieceSerial >= g_mainPage.sumSampleNum)
		{
			g_testBody.flagOneGroupSampleComplete = SET;
		}
		
		SetTestStatus(TEST_BREAK);
		
		#ifdef ENABLE_BEEP
			BEEP_RING_ONE();
		#endif
	}
}	

/*------------------------------------------------------------
 * Function Name  : JudgeForceAvail
 * Description    : �ж���ֵ�Ƿ���Ч
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus JudgeForceAvail( TEST_TYPE_TypeDef type, uint8_t num, float *pAvail_force, uint8_t *match )
{
	uint8_t i,cnt = 0,invalid_cnt = 0;
	float avg = 0.0f;
	float buff_force = 0.0f;
	const uint8_t CNT_KYHNT_STANDARD = 3;
	float force[MAX_RECORD_TEST_RESULT_NUM] = {0};
	float force_buff[MAX_RECORD_TEST_RESULT_NUM] = {0};
	uint8_t illegal_index = 0,match_cnt = 0;
	
	if ( !num )
	{
		*pAvail_force = 0.0f;
		
		return FAILED;
	}
	
	memcpy(force,g_readReport.force,MAX_RECORD_TEST_RESULT_NUM*sizeof(float));
	
	switch ( type )
	{
		case KYHNT:
		case KYJZSJ:
			if ( num != CNT_KYHNT_STANDARD )
			{
				avg = GetAverage(force,num);
				break;
			}
						
			if ( g_readReport.result_valid == 1 )
			{					
				invalid_cnt = FindArrayValNotZeroCount(num,match);
			
				switch ( invalid_cnt )
				{
					case 1:
						memcpy(force_buff,force,sizeof(float)*CNT_KYHNT_STANDARD);
						
						SortSmallToLarge(CNT_KYHNT_STANDARD,force_buff);
						avg = force_buff[1];	//�м�ֵ��Ϊ��Чֵ
						break;
					
					default:
						avg = GetAverage(force,CNT_KYHNT_STANDARD);
						break;
				}
				
			}
			else
			{
				avg = GetAverage(force,CNT_KYHNT_STANDARD);
			}					
			break;
			
		case KZHNT:
			if ( num != CNT_KYHNT_STANDARD )
			{
				avg = GetAverage(force,num);
				break;
			}
			
			match_cnt = GetArrayEqualZeroIndex(CNT_KYHNT_STANDARD,g_readReport.collect_load,&illegal_index);	//�۶����Ƿ��ڼ����غ��ڣ�
			
			switch ( match_cnt )
			{
				case CNT_KYHNT_STANDARD:
					if ( g_readReport.result_valid == 1 )
					{													
						invalid_cnt = FindArrayValNotZeroCount(num,match);
					
						switch ( invalid_cnt )
						{
							case 1:
								memcpy(force_buff,force,sizeof(float)*CNT_KYHNT_STANDARD);
								
								SortSmallToLarge(CNT_KYHNT_STANDARD,force_buff);
								avg = force_buff[1];	//�м�ֵ��Ϊ��Чֵ
								break;
							
							default:
								avg = GetAverage(force,CNT_KYHNT_STANDARD);
								break;
						}				
					}
					else
					{
						avg = GetAverage(force,num);
					}
					break;
					
				case 2:
					if ( g_readReport.result_valid == 1 )
					{
						for (i=0; i<num; ++i)
						{
							if ( !(match[i]) )
							{
								buff_force += g_readReport.force[i];
								cnt++;
							}
						}
						
						if ( cnt )
						{
							avg = buff_force / cnt;
						}
					}
					else
					{
						avg = GetAverage(force,num);
					}
					break;
				
				default:
					avg = GetAverage(force,num);
					break;
			}		
			break;
		
		default:	
			if ( g_readReport.result_valid == 1 )
			{
				for (i=0; i<num; ++i)
				{
					if ( !(match[i]) )
					{
						buff_force += force[i];
						cnt++;
					}
				}
				
				if ( cnt )
				{
					avg = buff_force / cnt;
				}
			}
			else
			{
				avg = GetAverage(force,num);
			}
			break;
	}	
	
    *pAvail_force = avg;
	
	return PASSED;	
}

/*------------------------------------------------------------
 * Function Name  : JudgeStrengthAvail
 * Description    : �ж�ǿ���Ƿ���Ч
 * Input          : num���Լ�������match_sample:�����������Կ���
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus JudgeStrengthAvail( TEST_TYPE_TypeDef type, uint8_t num,float *pAvail_press, uint8_t *match_sample )
{
	uint8_t i,no_match_cnt = 0,illegal_index = 0,match_cnt = 0;
	float avg = 0.0f,value = 0.0f,s_val = 0.0f;
	float press_buff[MAX_RECORD_TEST_RESULT_NUM];
	const uint8_t CNT_KYSNJS_STANDARD = 6;
	const uint8_t CNT_KZSNJS_STANDARD = 3;
	const uint8_t CNT_KYHNT_STANDARD = 3;
	uint8_t no_match[MAX_RECORD_TEST_RESULT_NUM] = {0};	//Ĭ��ȫ�����Ϊ��Ч
	uint8_t no_match_buff[MAX_RECORD_TEST_RESULT_NUM] = {0};

	if ( !num )
	{
		*pAvail_press = 0.0f;
		
		return FAILED;
	}
	
	memcpy(press_buff,g_readReport.strength,num*sizeof(float));
	avg = GetAverage(press_buff,num);

	switch ( type )
	{
		case KYSNJS:
			if (CNT_KYSNJS_STANDARD != num)	
			{
				break;
			}
			
			FindNotMatch_10_To_90_Condition(num,press_buff,no_match);
			
			no_match_cnt = FindArrayValNotZeroCount(num,no_match);
			
			switch ( no_match_cnt )
			{
				case 0:
					break;
				
				case 1:				
					illegal_index = FindArrayElementIsOneIndex(num,no_match);
				
					DeleteArrayWhichOnePositionElement(press_buff,num,illegal_index);
					
					num--;
				
					avg = GetAverage(press_buff,num);
					
					FindNotMatch_10_To_90_Condition(num,press_buff,no_match_buff);
					
					no_match_cnt = FindArrayValNotZeroCount(num,no_match_buff);
										
					if ( no_match_cnt )
					{
						*pAvail_press = 0.0f;
						
						for (i=0; i<CNT_KYSNJS_STANDARD; ++i)
						{		
							if ( no_match_buff[i] )		//�쳣����
							{
								if ( SUCCESS == ContrastFloatDataEqual(CNT_KYSNJS_STANDARD,g_readReport.strength,&illegal_index,press_buff[i]) )	//���������ǿ��ֵpress_buff��ԭʼǿ��ֵpPress��һ�ȶԣ��ҳ�����ԭʼ���ݶ�Ӧ��λ��
								{
									no_match[illegal_index] = 1;
								}
							}
						}					
						memcpy(match_sample,no_match,sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );
						
						return FAILED;
					}
					else
					{
						;
					}
					break;
				
				default:
					*pAvail_press = 0.0f;
					memcpy(match_sample, no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );
				
					return FAILED;
			}
			break;

		case KZSNJS:
		case KZYJSNJ:
			if (CNT_KZSNJS_STANDARD != num)	
			{
				break;
			}
			
			FindNotMatch_10_To_90_Condition(num,press_buff,no_match);
			
			no_match_cnt = FindArrayValNotZeroCount(num,no_match);
			
			switch ( no_match_cnt )
			{
				case 0:
					break;
			
				case 1:
					for (i=0; i<CNT_KZSNJS_STANDARD; ++i)
					{
						if ( no_match[i] )
						{
							DeleteArrayWhichOnePositionElement(press_buff,num,i);
							avg = GetAverage(press_buff,num-1);
							break;
						}
					}
					break;
					
				case 2:
					for (i=0; i<CNT_KZSNJS_STANDARD; ++i)
					{
						if ( !(no_match[i]) )
						{
							avg = press_buff[i];
						}
					}	
					break;	
				
				case CNT_KZSNJS_STANDARD:
					*pAvail_press = 0.0f;
					memcpy(match_sample,no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );
				
					return FAILED;
					
				default:															
					break;
			}
			break;
						
		case KYHNT:
		case KYJZSJ:
			if (CNT_KYHNT_STANDARD != num)
			{
				break;
			}
			
			avg = FindNotMatch_Per15_Condition(CNT_KYHNT_STANDARD,press_buff,no_match);
			
			no_match_cnt = FindArrayValNotZeroCount(CNT_KYHNT_STANDARD,no_match);
			
			if ( CNT_KYHNT_STANDARD == no_match_cnt )	
			{
				*pAvail_press = 0.0f;
				memcpy(match_sample,no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );
				
				return FAILED;
			}		
			break;
			
		case KZHNT:
			if (CNT_KYHNT_STANDARD != num)
			{
				break;
			}
			
			match_cnt = GetArrayEqualZeroIndex(CNT_KYHNT_STANDARD,g_readReport.collect_load,&illegal_index);	//�۶����Ƿ��ڼ����غ��ڣ�
				
			switch ( match_cnt )
			{
				case 3:
					avg = FindNotMatch_Per15_Condition(CNT_KYHNT_STANDARD,press_buff,no_match);
			
					no_match_cnt = FindArrayValNotZeroCount(CNT_KYHNT_STANDARD,no_match);
					
					if ( CNT_KYHNT_STANDARD == no_match_cnt )	
					{
						*pAvail_press = 0.0f;
						memcpy(match_sample,no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );
						
						return FAILED;
					}
					break;
				
				case 2:					
					DeleteArrayWhichOnePositionElement(press_buff,CNT_KYHNT_STANDARD,illegal_index);
					no_match[illegal_index] = 1;
				
					value = fabs(press_buff[0]-press_buff[1]);
					
					s_val = GetFloatSmallValue(press_buff[0],press_buff[1]);
					
					if (value <= s_val * 0.15f)
					{
						avg = GetAverage(press_buff,2);
					}
					else
					{
						*pAvail_press = 0.0f;
						
						for (i=0; i<CNT_KYHNT_STANDARD; ++i)
						{
							match_sample[i] = 1;
						}
			
						return FAILED;
					}
					break;
					
				default:
					*pAvail_press = 0.0f;
				
					for (i=0; i<CNT_KYHNT_STANDARD; ++i)
					{
						if ( g_readReport.collect_load[i] )
						{
							match_sample[i] = 0;
						}
						else
						{
							match_sample[i] = 1;
						}
					}
			
					return FAILED;	
			}
			break;
			
		case KYQQZ:	
			break;
		
		case KYZJDH:
			*pAvail_press = 0.0f;
			break;
		
		case KYTY:
			*pAvail_press = 0.0f;
			break;
		
		case KZTY:	
			*pAvail_press = 0.0f;
			
			return FAILED;
		
		default:
			*pAvail_press = 0.0f;
		
			return FAILED;
		
	}
	
	*pAvail_press = avg;
	memcpy(match_sample,no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM );

	return PASSED;
}


/*------------------------------------------------------------
 * Function Name  : MainPageGetAvailForce
 * Description    : ��ȡ��Ч��ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageGetAvailForce( void )
{
	uint8_t no_match[MAX_RECORD_TEST_RESULT_NUM] = {0};
	float availForce = 0;
	
	JudgeForceAvail(g_mainPage.testType,g_testBody.curCompletePieceSerial,&availForce,no_match);
	
	g_readReport.force_valid[0] = availForce;
	
	#ifdef DEBUG_TEST_LOAD
		printf("��ȡ��Ч��ֵ��\r\n");
	#endif
}	

/*------------------------------------------------------------
 * Function Name  : MainPageGetAvailStrength
 * Description    : ��ȡ��Чǿ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageGetAvailStrength( void )
{
	TestStatus result = FAILED;
	uint8_t no_match[MAX_RECORD_TEST_RESULT_NUM] = {0};
	float availPress = 0;
	
	result = JudgeStrengthAvail(g_mainPage.testType,g_testBody.curCompletePieceSerial,&availPress,no_match);
	
	g_readReport.strength_valid[0] = availPress;
	
	if (PASSED == result)
	{
		g_readReport.result_valid = 1;
	}
	else
	{
		g_readReport.result_valid = 0;
	}
	
	#ifdef DEBUG_TEST_LOAD
		printf("��ȡ��Чǿ�ȣ�\r\n");
	#endif
}

/*------------------------------------------------------------
 * Function Name  : MainPageAskSampleFractureSurfaceProcess
 * Description    : ѯ���۶�����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageAskSampleFractureSurfaceProcess( void )
{
	if (g_mainPage.testType != KZHNT)
	{
		return;
	}
	
	if (g_testBody.flagOnePieceSampleComplete == RESET)
	{
		return;
	}
	
	#ifdef DEBUG_TEST_LOAD
		printf("ѯ���۶�����Ϣ��\r\n");
	#endif
	
	SetSelectPopWindowsInfomation(POP_PCM_ASKING,1,&pMainPageWarning[9],pSelectMenuConfirmCue);
	
	PopWindowsProcessCycle();
	
	if ( GetPopWindowsSelectResult() == YES)
	{
		g_readReport.collect_load[g_testBody.curCompletePieceSerial-1] = 1;
	}
	else
	{
		g_readReport.collect_load[g_testBody.curCompletePieceSerial-1] = 0;
	}
	
	g_mainPage.leavePage.flagLeavePage = SET;
	g_mainPage.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : MainPageAutoPrintReportProcess
 * Description    : �Զ���ӡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageAutoPrintReportProcess( void )
{
	if (g_testBody.isAutoPrintReport == NO)
	{
		return;
	}
	
	if (g_testBody.flagOneGroupSampleComplete == RESET)
	{
		return;
	}
	
	if (iSAutoPrintReport() == YES)
	{
		MainPagePrintReport();
		
		#ifdef DEBUG_TEST_LOAD
			printf("�Զ���ӡ���棡\r\n");
		#endif
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteCancelLastPiece
 * Description    : ִ�г�����һ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteCancelLastPiece( void )
{
	if (g_testBody.flagCancelLastPiece == SET)
	{
		g_testBody.flagCancelLastPiece = RESET;
		
		#ifdef DEBUG_TEST_LOAD
			printf("������һ�飡\r\n");
		#endif
		
		MainPageClearForce(GetCompletePieceSerialInCurrentPageSerial());
		MainPageClearStrength(GetCompletePieceSerialInCurrentPageSerial());
		
		if (g_mainPage.curPage)
		{
			Show_MainPageTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_FORCE);
			Show_MainPageTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_STRENGTH);
		}
		
		if (g_testBody.curCompletePieceSerial)
		{
			g_readReport.force[g_testBody.curCompletePieceSerial-1] = 0;
			g_readReport.strength[g_testBody.curCompletePieceSerial-1] = 0;
		}
		
		MainPageClearCursor(g_testBody.curCompletePieceSerial);
		MainPageClearCursor(g_testBody.curCompletePieceSerial+1);

		if (g_testBody.curCompletePieceSerial)
		{
			g_testBody.curCompletePieceSerial--;
		}
		g_readReport.sample_serial = g_testBody.curCompletePieceSerial;
		
		MainPageLoadCursor(g_testBody.curCompletePieceSerial+1);
		
		if (g_readReport.sample_serial)
		{
			g_testBody.isWriteToSD = YES;
		}
		else 
		{
			if (FR_OK == report_delete(g_mainPage.testType,pTest->test_serial) )
			{
				GUI_DrawTestSaveProgressBar(LIGHT_GRAY2);
			}
			else
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[8]);
				
				g_mainPage.leavePage.flagLeavePage = SET;
				g_mainPage.leavePage.flagSaveData = RESET;
			}
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteHandEndGroup
 * Description    : ִ�н���һ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteHandEndGroup( void )
{
	if (g_testBody.flagHandEndOneGroup == SET)
	{
		g_testBody.flagHandEndOneGroup = RESET;
		
		g_testBody.flagOneGroupSampleComplete = SET;
		
		g_testBody.isExecuteEndGroup = YES;
		
		#ifdef DEBUG_TEST_LOAD
			printf("ִ���ֶ�����һ�飡\r\n");
		#endif
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteBreakProcess
 * Description    : ִ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteBreakProcess( void )
{
	if (g_testBody.flagOnePieceSampleComplete == SET)
	{
		g_testBody.flagOnePieceSampleComplete = RESET;
		
		#ifdef DEBUG_TEST_LOAD
			printf("һ���Կ������\r\n");
		#endif
		
		g_testBody.isExecuteEndGroup = YES;
		
		MainPageWriteForce(GetCompletePieceSerialInCurrentPageSerial(),g_testBody.breakPeak);
		MainPageWriteStrength(GetCompletePieceSerialInCurrentPageSerial(),g_testBody.breakStrength);
		
		if (g_mainPage.curPage)
		{
			Show_MainPageTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_FORCE);
			Show_MainPageTestResultTable(GetCompletePieceSerialInCurrentPageSerial(),OBJECT_STRENGTH);
		}
		
		if (g_testBody.curCompletePieceSerial)
		{
			g_readReport.force[g_testBody.curCompletePieceSerial-1] = g_testBody.breakPeak;
			g_readReport.strength[g_testBody.curCompletePieceSerial-1] = g_testBody.breakStrength;
		}
		
		g_readReport.sample_serial = g_testBody.curCompletePieceSerial;

		g_testBody.isWriteToSD = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteEndGroupProcess
 * Description    : ִ�н���һ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteEndOneGroupProcess( void )
{
	if ((g_testBody.isExecuteEndGroup==YES) && (g_testBody.flagOneGroupSampleComplete==SET))
	{			
		#ifdef DEBUG_TEST_LOAD
			printf("һ������������\r\n");
		#endif
		
		g_readReport.test_is_complete = 1;
		
		/* �����Ȼ�ȡ��Чǿ�ȣ��ٻ�ȡ��Ч��ֵ */
		MainPageGetAvailStrength();
		MainPageGetAvailForce();
		
		MainPageWriteAvailForce(g_readReport.force_valid[0]);
		MainPageWriteAvailStrength(g_readReport.strength_valid[0]);
		
		if (g_mainPage.curPage)
		{
			Show_MainPageTestResultTableAvailValue(OBJECT_FORCE);
			Show_MainPageTestResultTableAvailValue(OBJECT_STRENGTH);
		}
		
		g_testBody.isWriteToSD = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteWriteSDProcess
 * Description    : ִ��д��SD
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteWriteSDProcess( void )
{
	if (g_testBody.isWriteToSD == YES)
	{
		#ifdef DEBUG_TEST_LOAD
			printf("���汨�浽SD����\r\n");
		#endif
		
		if (FR_OK == report_save(g_mainPage.testType,pTest->test_serial,&g_readReport) )
		{
			GUI_DrawTestSaveProgressBar(LIGHT_GRAY2);	
			
			g_testBody.isAutoPrintReport = YES;
		}
		else
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[2]);
			
			g_mainPage.leavePage.flagLeavePage = SET;
			g_mainPage.leavePage.flagSaveData = RESET;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageTestAfterDisposeExecuteCycle
 * Description    : �������ִ��ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTestAfterDisposeExecuteCycle( void )
{
	g_testBody.isWriteToSD = NO;
	g_testBody.isExecuteEndGroup = NO;
	g_testBody.isAutoPrintReport = NO;
	
	/* ѯ���۶�����Ϣ */
	MainPageAskSampleFractureSurfaceProcess();
	
	/* ������һ�� */
	MainPageExecuteCancelLastPiece();
	
	/* �ֶ�����һ�� */
	MainPageExecuteHandEndGroup();
	
	/* һ���������� */
	MainPageExecuteBreakProcess();
	
	/* һ���������� */
	MainPageExecuteEndOneGroupProcess();
	
	/* д��SD�� */
	MainPageExecuteWriteSDProcess();
	
	/* �Զ���ӡ���� */
	MainPageAutoPrintReportProcess();
}	

/*------------------------------------------------------------
 * Function Name  : MainPageTestExecuteCoreCycle
 * Description    : ����ִ�к���ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTestExecuteCoreCycle( void )
{
	float force = get_smpl_force(g_mainPage.tureChannel);
	float startLoadForce = smpl_ctrl_entry_get(g_mainPage.tureChannel);
	
	switch ( GetTestStatus() )
	{
		case TEST_IDLE:
			if (force > startLoadForce)
			{
				if (MainPageAllowRunTest() == ENABLE)
				{
					MainPageExecuteTestStartBody();
					#ifdef DEBUG_TEST_LOAD
						printf("������أ�\r\n");
					#endif
				}
				else
				{
					g_mainPage.leavePage.flagLeavePage = SET;
					g_mainPage.leavePage.flagSaveData = RESET;
				}
			}
			break;
		case TEST_LOAD:
			if (force < startLoadForce)
			{
				g_mainPage.refreshShortcut = ENABLE;
				
				SetTestStatus(TEST_IDLE);
				
				#ifdef DEBUG_TEST_LOAD
					printf("����ֹͣ��\r\n");
				#endif
			}
			
			MainPageJudgeBreakCoreCycle();			
			break;
		case TEST_BREAK:
			SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"���������飡");
		
			SetTestStatus(TEST_UNLOAD);
		
			#ifdef DEBUG_TEST_LOAD
				printf("������ʾ��\r\n");
			#endif
			break;
		case TEST_UNLOAD:
			if (force < startLoadForce)
			{
				GUI_ClearShortcutMenu();
				
				MainPageExecuteTestEndBody();
				SetTestStatus(TEST_SAVE);
				
				#ifdef DEBUG_TEST_LOAD
					printf("�������\r\n");
				#endif
			}
			break;
		case TEST_SAVE:
			MainPageTestAfterDisposeExecuteCycle();
			SetTestStatus(TEST_IDLE);

			g_mainPage.refreshShortcut = ENABLE;

			#ifdef DEBUG_TEST_LOAD
				printf("���������\r\n");
			#endif
			break;
		
		default:
			SetTestStatus(TEST_IDLE);	
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageCheckWarn
 * Description    : ��⾯��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCheckWarn( void )
{	
	switch ( GetTestStatus() )
	{
		case TEST_IDLE:
			if (CheckPrmWarning() == YES)
			{
				g_mainPage.leavePage.flagLeavePage = SET;
				g_mainPage.leavePage.flagSaveData = RESET;
			}
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : InitMainPageCoordinateDrawLine
 * Description    : ��ʼ������ϵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitMainPageCoordinateDrawLine( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	uint32_t maxForce = 0;
	
	maxForce = smpl_ctrl_full_p_get(g_mainPage.tureChannel);
	
	pDrawLine->status = STATUS_DRAW_LINE_IDLE;
	pDrawLine->start = RESET;
	pDrawLine->enableRedraw = DISABLE;
	pDrawLine->originX = pCoordinate->x;
	pDrawLine->originY = pCoordinate->y + pCoordinate->yLenth;
	pDrawLine->lenthX = pCoordinate->xLenth;
	pDrawLine->lenthY = pCoordinate->yLenth;
	pDrawLine->maxForce = (maxForce >> 1);
	pDrawLine->maxTime = pCoordinate->maxTime * 1000;
	pDrawLine->nowTimePoint = 0;
	pDrawLine->lineColor = RED;
	if (g_mainPage.showChannel == SMPL_KY_NUM)
	{
		pDrawLine->forceScalingCoefficient = 0.001;
	}
	else
	{
		pDrawLine->forceScalingCoefficient = 1;
	}
	pDrawLine->pDrawCoordinate = GUI_MainPageDrawCoordinate;
				
	InitCoordinateDrawLine(pDrawLine);
}

/*------------------------------------------------------------
 * Function Name  : MainPageCoordinateDrawLineJudgeCondition
 * Description    : ����ϵ�����ж�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCoordinateDrawLineJudgeCondition( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	float force = 0;
	float curveShowStartForce = 0; 
	
	force = get_smpl_force(g_mainPage.tureChannel);
	
	curveShowStartForce = GetCurveShowStartValue(g_mainPage.tureChannel);
	
	switch ( pDrawLine->status )
	{
		case STATUS_DRAW_LINE_IDLE:
			if ((GetTestStatus()==TEST_LOAD) && (force>curveShowStartForce))
			{
				InitMainPageCoordinateDrawLine();
				ReloadCoordinate(pDrawLine);
			
				pDrawLine->start = SET;
				pDrawLine->status = STATUS_DRAW_LINE_LOAD;					
			}
			break;
		case STATUS_DRAW_LINE_LOAD:		
			if (GetTestStatus() == TEST_IDLE)
			{
				pDrawLine->start = RESET;
				pDrawLine->status = STATUS_DRAW_LINE_IDLE;
			}
			break;
			
		default:
			pDrawLine->status = STATUS_DRAW_LINE_IDLE;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageCoordinateDrawLineRedrawJudgeCondition
 * Description    : ����ϵ���»����ж�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCoordinateDrawLineRedrawJudgeCondition( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	float force = 0;
	uint32_t maxForce = 0;
	float checkForce = 0;
	uint32_t checkTime = 0;
	uint32_t curTime = 0;
	const uint8_t ONCE_LOAD_SECOND = 20;
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	
	force = get_smpl_force(g_mainPage.tureChannel);
	maxForce = smpl_ctrl_full_p_get(g_mainPage.tureChannel);
	
	checkForce = pDrawLine->maxForce * ((float)(pCoordinate->rowFieldNum - 1) / pCoordinate->rowFieldNum);
	checkTime = pDrawLine->maxTime * ((float)(pCoordinate->columnFieldNum - 1) / pCoordinate->columnFieldNum);
	
	if (force > checkForce)
	{
		pDrawLine->maxForce += maxForce / 10;
		pDrawLine->enableRedraw = ENABLE;
	}
	
	curTime = pDrawLine->nowTimePoint * RECORD_COORDINATE_PERIOD;
	
	if (curTime > checkTime)
	{
		pDrawLine->maxTime += ONCE_LOAD_SECOND * 1000;
		pDrawLine->enableRedraw = ENABLE;
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageCoordinateDrawLineBodyCycle
 * Description    : ������ϵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCoordinateDrawLineBodyCycle( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	float force = 0;
	static uint8_t s_ctrlCoordinateDrawLineCount = 0;
	const uint8_t DRAW_COORDINATE_COUNT = 5;		//100MS

	/* �����ж����� */
	MainPageCoordinateDrawLineJudgeCondition(pDrawLine);
	
	if (pDrawLine->start == RESET)
	{
		return;
	}
	
	/* �������ڣ�100ms */
	s_ctrlCoordinateDrawLineCount++;
	if (s_ctrlCoordinateDrawLineCount < DRAW_COORDINATE_COUNT)
	{
		return;
	}	
	s_ctrlCoordinateDrawLineCount = 0;
	
	/* ��¼��ֵ��Ϣ */
	pDrawLine->nowTimePoint++;
	if (IsCoordinateRecordPointOverflow(pDrawLine) == YES)
	{
		pDrawLine->nowTimePoint = DECORD_COORDINATE_FORCE_NUM - 1;
		
		return;
	}
	
	force = get_smpl_force(g_mainPage.tureChannel);
	
	pDrawLine->force[pDrawLine->nowTimePoint] = force;
	
	#ifdef DEBUG_COORDINATE_DRAW_LINE
		printf("time ��%d\r\n",pDrawLine->nowTimePoint);
		printf("force��%f\r\n",force);
	#endif
	
	/* ���»����ж����� */
	MainPageCoordinateDrawLineRedrawJudgeCondition(pDrawLine);
	
	/* ����ϵ���� */
	CoordinateDrawLineBodyCycle(pDrawLine);
}

/*------------------------------------------------------------
 * Function Name  : MainPageJudgeBreakCycle
 * Description    : ����ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageJudgeBreakCycle( void )
{
	JudgeBreakCalculateCycle(g_mainPage.tureChannel);
}

/*------------------------------------------------------------
 * Function Name  : MainPageCtrlCoreCycle
 * Description    : ���ƺ���ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCtrlCoreCycle( void )
{
	/* ��������50HZ */
	while (!bsp_CheckTimer(MAIN_CYCLE_TIMING));
	
	/* ��λ���·�����(��������) */
	PCM_CmdSendCycle();
	
	/* ����ѭ���� */
	MainPageJudgeBreakCycle();
	
	/* ������ϵ */
	MainPageCoordinateDrawLineBodyCycle();
	
	/* ���ö�̬���� */
	SetDynamicContentTask();
	
	/* ˢ�¶�̬���� */
	RefreshDynamicContent();
	
	/* ����ִ�� */
	MainPageTestExecuteCoreCycle();
	
	/* ϵͳ������ */
	MainPageCheckWarn();
	
	/* �ѻ���� */
	CheckOfflineCycle();
	
	/* ������� */
	MainPageCheckLinkPCCycle();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
