/**
  ******************************************************************************
  * @file    MainPage.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-28 15:06:55
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   主界面
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
#include "TestTypeSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK
	
#define MAX_TEST_INFOMATION_NUM				5		//最大试验信息项
#define MAX_TEST_INFOMATION_OBJECT_NUM		20		//最大试验信息对象个数

#define MAX_RECORD_TEST_RESULT_NUM			20		//记录最大试块个数
#define MAX_TEST_RESULT_TABLE_FIELD_NUM		3		//表格字段个数
#define MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM	6		//一页试块个数

#define TEST_INFOMATION_FAILED_MAX_CHAR_NUM	16
#define TEST_RESULT_SERIAL_MAX_CHAR_NUM		4
#define TEST_RESULT_FORCE_MAX_CHAR_NUM		8	
#define TEST_RESULT_STRENGTH_MAX_CHAR_NUM	6	//0->经过floattochar转换后，变成-0.0，所以内存需要大一些
#define TEST_RESULT_AVAIL_SERIAL_MAX_CHAR_NUM	8
#define KL_TEST_MAX_CHAR_BIT				8	//抗拉试验支持最大字符个数

#define SHOW_FORCE_DOT_BIT					2		//力值小数位
#define SHOW_STRENGTH_DOT_BIT				1		//强度有效位

/* 示值窗 */
#define INDICATE_WINDOWS_NUMS				4		//示值窗个数
#define	INDICATE_WINDOWS_ROW_NUMS			2		//示值窗行数
#define INDICATE_WINDOWS_FORCE_CHAR_NUM		8		
#define INDICATE_WINDOWS_SPEED_CHAR_NUM		6
#define INDICATE_WINDOWS_STRENGTH_CHAR_NUM	5
#define INDICATE_WINDOWS_PEAK_CHAR_NUM		8

/* 状态栏 */
#define STATUS_BAR_NUMS						5		

/* 抗拉试验结果 */
#define	KL_TEST_MAX_RESULT_ROW_NUM			9

/* 抗拉试验进度 */
#define MAX_KL_TEST_PROCESS_NUM				6		//最大进度个数

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	REFRESH_FORCE = 0,
	REFRESH_SPEED,
	REFRESH_STRENGTH,
//	REFRESH_PEAK,
	REFRESH_DISPLACEMENT,
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
	OBJECT_WINDOWS_DISPLACEMENT,
}OBJECT_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	INDEX_WINDOWS_FORCE = 0,	
	INDEX_WINDOWS_SPEED,		
	INDEX_WINDOWS_STRENGTH,	
	INDEX_WINDOWS_DISPLACEMENT,
}INDEX_INDICATE_WINDOWS_NAME_TypeDef;

typedef enum
{
	OBJECT_KL_SAMPLE_SERIAL = 0,
	OBJECT_KL_MAX_FORCE,
	OBJECT_KL_STRENGTH,
	OBJECT_KL_UP_YIELD,
	OBJECT_KL_DOWN_YIELD,
	OBJECT_KL_UP_YIELD_STRENGTH,
	OBJECT_KL_DOWN_YIELD_STRENGTH,
	OBJECT_KL_MAX_FORCE_ALL_EXTEND,
	OBJECT_KL_TOTAL_ELONGATION,
}OBJECT_KL_TEST_RESULT_TypeDef;

typedef enum
{
	INDEX_KL_SAMPLE_SERIAL = 0,
	INDEX_KL_MAX_FORCE,
	INDEX_KL_STRENGTH,
	INDEX_KL_UP_YIELD,
	INDEX_KL_DOWN_YIELD,
	INDEX_KL_UP_YIELD_STRENGTH,
	INDEX_KL_DOWN_YIELD_STRENGTH,
	INDEX_KL_MAX_FORCE_ALL_EXTEND,
	INDEX_KL_TOTAL_ELONGATION,
}INDEX_KL_TEST_RESULT_TypeDef;

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
	/* 示值窗 */
	uint8_t indexIndicateWindowsArray[INDICATE_WINDOWS_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuIndicateWindows[INDICATE_WINDOWS_ROW_NUMS][INDICATE_WINDOWS_NUMS];
	const char *pIndicateWindowsTitleNameArray[INDICATE_WINDOWS_NUMS];
	uint8_t indicateWindowNum;						//示值窗字段个数
	INDICATE_WINDOWS_FIELD_DATA_TypeDef indicateWindowsData[INDICATE_WINDOWS_NUMS];
	
	/* 表格 */
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableTitle[MAX_TEST_RESULT_TABLE_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableContent[MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM][MAX_TEST_RESULT_TABLE_FIELD_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuTableAvailValue[MAX_TEST_RESULT_TABLE_FIELD_NUM];	
	uint8_t indexTestResultTableArray[MAX_TEST_RESULT_TABLE_FIELD_NUM];		//试验结果句柄索引	
	TEST_RESULT_FIELD_DATA_TypeDef testResultData[MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM];
	TEST_RESULT_FIELD_AVAIL_DATA_TypeDef testResultAvailData;
	const char *pTestResultTitleNameArray[MAX_TEST_RESULT_TABLE_FIELD_NUM];
	uint8_t tableFieldNum;							//表格字段个数
	
	/* 抗拉试验结果 */
	uint8_t klFieldNum;
	uint8_t indexKLTestResultArray[KL_TEST_MAX_RESULT_ROW_NUM]; 
	char kLTestResultData[KL_TEST_MAX_RESULT_ROW_NUM][KL_TEST_MAX_CHAR_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuKLTestResult[KL_TEST_MAX_RESULT_ROW_NUM];
	const char *pKLTestResultParameterNameArray[KL_TEST_MAX_RESULT_ROW_NUM];
	const char *pKLTestResultParameterUnitArray[KL_TEST_MAX_RESULT_ROW_NUM];
	
	/* 状态栏 */
	uint8_t indexStatusBarArray[STATUS_BAR_NUMS];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuStatusBar[STATUS_BAR_NUMS];
	const char *pStatusBarTitleNameArray[STATUS_BAR_NUMS];
	uint8_t statusBarNum;							//状态栏个数
	
	const char * pTestTitle;							//标题
	ALIGN_TYPE_TypeDef align[MAX_TEST_RESULT_TABLE_FIELD_NUM];		//对齐方式
	uint8_t testInfomationNum;						//试验信息个数	
	uint8_t curPageSampleNum;						//当前页试块个数
	uint8_t sumSampleNum;							//全部试块个数
	uint8_t curPage;								//当前页数
	uint8_t sumPage;								//总页数
	BoolStatus isIndexMove;							//索引值移动
	TEST_TYPE_TypeDef testType;						//试验类型
	TEST_ATTRIBUTE_TypeDef testAttribute;			//试验属性
	FH_UINT_TypeDef fhChannelUnit;					//负荷通道单位
	WY_UINT_TypeDef	wyChannelUnit;					//位移通道单位
	BX_UINT_TypeDef	bxChannelUnit;					//变形通道单位
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
	
	REPORT_TypeDef *pReadReport;
}MAIN_PAGE_TypeDef;

typedef struct
{
	TEST_STATUS_TypeDef status;						//试验状态
	FlagStatus flagOnePieceSampleComplete;			//一块试样完成标志
	FlagStatus flagOneGroupSampleComplete;			//一组试样完成标志
	FlagStatus flagOneGroupTestResultAvail;			//一组试验结果有效标志
	FlagStatus flagCancelLastPiece;					//撤销上一块
	FlagStatus flagHandEndOneGroup;					//结束一组
	FlagStatus flagAutoUpdateSerial;				//自动更新试件编号
	FlagStatus flagStartJudgeBreak;					//开始判破
	FlagStatus startTest;							//开始标志
	FlagStatus endTest;								//结束标志
	uint8_t curCompletePieceSerial;					//当前已做完的试块编号
	FlagStatus flagTestAfterDispose;				//试验后处理标志
	float breakPeak;								//判破峰值
	float breakStrength;							//最大强度
	PEAK_STATUS_TypeDef peakStatus;					//峰值状态
	BoolStatus isWriteToSD;							//是否写入SD卡
	BoolStatus isExecuteEndGroup;					//是否执行结束一组
	BoolStatus isAutoPrintReport;					//是否自动打印报告
}TEST_BODY_TypeDef;

/* 抗拉试验进度 */
typedef enum
{
	STATUS_KL_PROGRESS_IDLE = 0,				/* 空闲状态 */
	STATUS_KL_PROGRESS_FORMAL_LOAD,				/* 正式加载 */
	STATUS_KL_PROGRESS_ELASTIC_DEFORMATION,		/* 弹性变形 */
	STATUS_KL_PROGRESS_YIELD,					/* 屈服阶段 */
	STATUS_KL_PROGRESS_PLASTIC_DEFORMATION,		/* 塑性变形 */
	STATUS_KL_PROGRESS_END,						/* 结束 	*/
}STATUS_KL_TEST_PROGRESS_TypeDef;

typedef struct _LIST_KL_TEST_PROGRESS_TypeDef
{
	uint16_t x;
	uint16_t y;
	uint16_t pointColor;
	uint16_t backColor;
	uint8_t fontSize;
	const char *name;	
	STATUS_KL_TEST_PROGRESS_TypeDef status;
	FunctionalState enShowSeparator;
	struct list_head list;
}LIST_KL_TEST_PROGRESS_TypeDef;

typedef struct
{
	LIST_KL_TEST_PROGRESS_TypeDef progressList[MAX_KL_TEST_PROCESS_NUM];
	struct list_head head;
	struct list_head *pHead;
	const char *pSeparator;	
}KL_TEST_PROGRESS_TypeDef;

typedef struct
{
	float maxForce;						//最大力
	float maxStrength;					//最大强度
	float upYieldForce;					//上屈服力
	float downYieldForce;				//下屈服力
	float upYieldStrength;				//上屈服强度
	float downYieldStrength;			//下屈服强度
	float maxForceSumExtend;			//最大力总延伸
	float maxForceSumElongation;		//最大力总伸长率
	
	/* 计算最大力总延伸 */
	float originalDisplacemen;			//原始位移
	float originalDeform;				//原始变形
	
	/* 计算屈服点、最大力 */
	uint32_t upYieldForceIndex;
	uint32_t maxForceIndex;
}KL_TEST_BODY_TypeDef;


/* Private constants ---------------------------------------------------------*/
const char * const pTestInfomationName[] = 
{
	"试件编号：",		//0
	"试件品种：",		//1
	"强度等级：",		//2
	"试件龄期：",		//3
	"试件规格：",		//4
	"加载速度：",		//5
	"试件块数：",		//6
	"加载方式：",		//7
	"延时时间：",		//8
	"试件长度：",		//9
	"试件宽度：",		//10
	"试件高度：",		//11
	"边长：",			//12
	"跨距：",			//13
	"修正系数：",		//14
};

const char * const pMainPageTestName[] = 
{
	"错误的试验类型",//0
	"水泥胶砂抗压",	//1
	"建筑砂浆抗压",	//2
	"混凝土抗压",	//3
	"混凝土抗折",	//4
	"砌墙砖抗压",	//5
	"逐级定荷",		//6
	"通用抗压",		//7
	"水泥胶砂抗折",	//8
	"压浆水泥浆抗折",//9
	"通用抗折",		//10
	"金属室温拉伸",	//11
};

const char * const pMainPageTestResultTitleName[] = 
{
	"序号",			//0
	"力值(kN)",		//1
	"力值(N)",		//2
	"强度(MPa)",	//3
};

const char * const pMainPageIndecateWindowsTitleName[] = 
{
	"力值(kN)",		//0
	"力值(N)",		//1
	"速度(kN/s)",	//2
	"速度(N/s)",	//3
	"强度(MPa)",	//4
	"峰值(kN)",		//5
	"峰值(N)",		//6
	"位移(mm)",		//7
	"位移(cm)",		//8
	"位移(dm)",		//9
	"位移(m)",		//10
	"变形(mm)",		//11
	"变形(cm)",		//12
	"变形(dm)",		//13
	"变形(m)",		//14
};

const char * const pStatusBarTitleName[] = 
{
	"通讯状态：",	//0
	"试验状态：",	//1
	"码值：",		//2
	"时间：",		//3
	"",				//4
};

const char * const pMainPageWarning[] = 
{
	"当前处于脱机状态！",			//0
	"试块个数不能为0！",				//1
	"试验保存失败！",				//2
	"发送负荷通道清零命令失败！",		//3
	"一组试验未完成，不能进入",		//4
	"试验参数界面。",				//5
	"一组试验完成，不能执行撤销",		//6
	"操作！",					 	//7
	"报告删除失败！",				//8
	"折断面是否位于集中载荷内？",		//9
	"未发现可以打印的试验报告！",		//10
	"报告导出失败！",				//11
	"报告读取失败！",				//12
	"请插入U盘！",					//13
	"发送位移通道清零命令失败！",		//14
	"发送变形通道清零命令失败！",		//15
	"系统不支持当前配置的机型！",		//16
	"请按“<- ->”键，将页面切换至",	//17
	"已做试样的最后一页！",
	"坐标点数据保存失败！",			//19
};	

const char * const pKLTestResultName[] = 
{
	"试件序号：",					//0
	"最大力[Fm]：",					//1
	"抗拉强度[Rm]：",				//2
	"上屈服[FeH]：",					//3
	"下屈服[FeL]：",					//4
	"上屈服强度[ReH]：",				//5
	"下屈服强度[ReL]：",				//6
	"最大力总延伸[ΔLm]：",			//7
	"最大力总伸长率[Agt]：",			//8
};

const char * const pKLTestProgressName[] = 
{
	"正式加载阶段",		//0
	"弹性变形阶段",		//1
	"屈服阶段阶段",		//2
	"塑性变形阶段",		//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_MAIN_PAGE",zidata = "RAM_MAIN_PAGE"
	static MAIN_PAGE_TypeDef g_mainPage;
	static TEST_BODY_TypeDef g_testBody;
	#if 0
		static KL_TEST_PROGRESS_TypeDef g_klTestProgress;
	#endif
	static KL_TEST_BODY_TypeDef	g_klTestBody;
#pragma arm section

extern REPORT_TypeDef g_readReport;

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

#if 0
	static void InitKL_TestProgress( void );
	static void SetKL_TestProgress( STATUS_KL_TEST_PROGRESS_TypeDef status );
#endif

static void InitKL_TestBody( KL_TEST_BODY_TypeDef *pKlTest );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadMainPage
 * Description    : 主页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadMainPage( void )
{	
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	MainPageInit();
	
	/* 参数配置 */
	MainPageConfig();
	
	/* 获取参数 */
	MainPageReadParameter();
	
	/* 画GUI框架 */
	GUI_MainPage();
	
	/* 遍历 */
	Traverse_TestResultTable();

	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_mainPage.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 快捷菜单 */
		MainPageShortcutCycleTask();
		
		/* 按键处理 */
		MainPageKeyProcess();
		
		/* 控制核心 */
		MainPageCtrlCoreCycle();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		MainPageLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageInit( void )
{
	disp_syn(DISP_CHN_FH);
	disp_syn(DISP_CHN_WY);
	disp_syn(DISP_CHN_BX);
	disp_syn(DISP_CHN_STRENGTH);
	disp_syn(DISP_CHN_PEAK);
	disp_syn(DISP_CHN_SPEED);
	
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
	g_mainPage.testAttribute = GetTestAttribute(pHmi->test_standard_index);
	
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
	
	g_mainPage.fhChannelUnit = GetFH_SmplUnit();
	g_mainPage.wyChannelUnit = GetWY_SmplUnit();
	g_mainPage.bxChannelUnit = GetBX_SmplUnit();
}

/*------------------------------------------------------------
 * Function Name  : SetTestStatus
 * Description    : 设置试验状态
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
 * Description    : 获取试验状态
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
 * Description    : 获取试验结果字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetMainPageTestResultFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
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
 * Description    : 获取示值窗字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetMainPageIndicateWindowsFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
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
 * Description    : 获取状态栏字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetMainPageStatusBarFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
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
 * Function Name  : GetMainPageKLTestResultFieldIndex
 * Description    : 获取抗拉试验结果字段的索引
 * Input          : handle：句柄
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetMainPageKLTestResultFieldIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		if (g_mainPage.indexKLTestResultArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : MainPageConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageConfig( void )
{	
	/* 示值窗索引 */
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_FORCE] 			= OBJECT_WINDOWS_FORCE;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_SPEED] 			= OBJECT_WINDOWS_SPEED;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_STRENGTH] 		= OBJECT_WINDOWS_STRENGTH;
	g_mainPage.indexIndicateWindowsArray[INDEX_WINDOWS_DISPLACEMENT] 	= OBJECT_WINDOWS_DISPLACEMENT;
	
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			/* 试验结果字段索引值 */
			g_mainPage.indexTestResultTableArray[INDEX_SERIAL] 		= OBJECT_SERIAL;
			g_mainPage.indexTestResultTableArray[INDEX_FORCE] 		= OBJECT_FORCE;
			g_mainPage.indexTestResultTableArray[INDEX_STRENGTH] 	= OBJECT_STRENGTH;
			break;
		case STRETCH_TEST:	
			/* 索引值 */
			g_mainPage.indexKLTestResultArray[INDEX_KL_SAMPLE_SERIAL] 			= OBJECT_KL_SAMPLE_SERIAL;		
			g_mainPage.indexKLTestResultArray[INDEX_KL_MAX_FORCE] 				= OBJECT_KL_MAX_FORCE;		
			g_mainPage.indexKLTestResultArray[INDEX_KL_STRENGTH] 				= OBJECT_KL_STRENGTH;		
			g_mainPage.indexKLTestResultArray[INDEX_KL_UP_YIELD] 				= OBJECT_KL_UP_YIELD;			
			g_mainPage.indexKLTestResultArray[INDEX_KL_DOWN_YIELD] 				= OBJECT_KL_DOWN_YIELD;	
			g_mainPage.indexKLTestResultArray[INDEX_KL_UP_YIELD_STRENGTH] 		= OBJECT_KL_UP_YIELD_STRENGTH;	
			g_mainPage.indexKLTestResultArray[INDEX_KL_DOWN_YIELD_STRENGTH] 	= OBJECT_KL_DOWN_YIELD_STRENGTH;	
			g_mainPage.indexKLTestResultArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= OBJECT_KL_MAX_FORCE_ALL_EXTEND;	
			g_mainPage.indexKLTestResultArray[INDEX_KL_TOTAL_ELONGATION] 		= OBJECT_KL_TOTAL_ELONGATION;	
			break;
	}
	
	/* 状态栏索引 */
	g_mainPage.indexStatusBarArray[INDEX_COMM_STATUS] 	= OBJECT_COMM_STATUS;
	g_mainPage.indexStatusBarArray[INDEX_TEST_STATUS] 	= OBJECT_TEST_STATUS;
	g_mainPage.indexStatusBarArray[INDEX_CODE] 			= OBJECT_CODE;
	g_mainPage.indexStatusBarArray[INDEX_TIME] 			= OBJECT_TIME;
	g_mainPage.indexStatusBarArray[INDEX_STATUS] 		= OBJECT_PERIPHERAL_STATUS;
	
	/* 示值窗标题名 */
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pMainPageIndecateWindowsTitleName[0];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pMainPageIndecateWindowsTitleName[2];
	}
	else
	{
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_FORCE] 	= pMainPageIndecateWindowsTitleName[1];
		g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_SPEED] 	= pMainPageIndecateWindowsTitleName[3];
	}	
	g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_STRENGTH] 	= pMainPageIndecateWindowsTitleName[4];
	
	if (GetDisplacementOrDeformShow() == SHOW_DISPLACEMENT)
	{
		switch ( g_mainPage.wyChannelUnit )
		{
			case WY_UNIT_MM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[7];
				break;
			case WY_UNIT_CM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[8];
				break;
			case WY_UNIT_DM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[9];
				break;
			case WY_UNIT_M:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[10];
				break;
		}
	}
	else
	{
		switch ( g_mainPage.bxChannelUnit )
		{
			case BX_UNIT_MM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[11];
				break;
			case BX_UNIT_CM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[12];
				break;
			case BX_UNIT_DM:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[13];
				break;
			case BX_UNIT_M:
				g_mainPage.pIndicateWindowsTitleNameArray[INDEX_WINDOWS_DISPLACEMENT] = pMainPageIndecateWindowsTitleName[14];
				break;
		}
	}
	
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			/* 试验结果标题名 */
			g_mainPage.pTestResultTitleNameArray[INDEX_SERIAL] 		= pMainPageTestResultTitleName[0];
			if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
			{
				g_mainPage.pTestResultTitleNameArray[INDEX_FORCE] 	= pMainPageTestResultTitleName[1];
			}
			else
			{
				g_mainPage.pTestResultTitleNameArray[INDEX_FORCE] 	= pMainPageTestResultTitleName[2];
			}
			g_mainPage.pTestResultTitleNameArray[INDEX_STRENGTH] 	= pMainPageTestResultTitleName[3];
			break;
		case STRETCH_TEST:	
			/* 参数名称 */
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_SAMPLE_SERIAL] 			= pKLTestResultName[0];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_MAX_FORCE] 				= pKLTestResultName[1];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_STRENGTH] 				= pKLTestResultName[2];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_UP_YIELD] 				= pKLTestResultName[3];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_DOWN_YIELD] 			= pKLTestResultName[4];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_UP_YIELD_STRENGTH] 		= pKLTestResultName[5];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_DOWN_YIELD_STRENGTH] 	= pKLTestResultName[6];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= pKLTestResultName[7];
			g_mainPage.pKLTestResultParameterNameArray[INDEX_KL_TOTAL_ELONGATION] 		= pKLTestResultName[8];
			break;
	}
	
	/* 状态栏标题 */
	g_mainPage.pStatusBarTitleNameArray[INDEX_COMM_STATUS] 	= pStatusBarTitleName[0];
	g_mainPage.pStatusBarTitleNameArray[INDEX_TEST_STATUS] 	= pStatusBarTitleName[1];
	g_mainPage.pStatusBarTitleNameArray[INDEX_CODE] 		= pStatusBarTitleName[2];
	g_mainPage.pStatusBarTitleNameArray[INDEX_TIME] 		= pStatusBarTitleName[3];
	g_mainPage.pStatusBarTitleNameArray[INDEX_STATUS] 		= pStatusBarTitleName[4];
	
	/* 个数 */
	g_mainPage.tableFieldNum = MAX_TEST_RESULT_TABLE_FIELD_NUM;
	g_mainPage.indicateWindowNum = INDICATE_WINDOWS_NUMS;
	g_mainPage.statusBarNum = STATUS_BAR_NUMS;
	g_mainPage.klFieldNum = KL_TEST_MAX_RESULT_ROW_NUM;
	
	/* 数据对齐 */
	g_mainPage.align[INDEX_SERIAL] 		= ALIGN_MIDDLE;
	g_mainPage.align[INDEX_FORCE] 		= ALIGN_LEFT;
	g_mainPage.align[INDEX_STRENGTH] 	= ALIGN_LEFT;
	
	/* 单位 */
	g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_SAMPLE_SERIAL] 			= "NULL";
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE] 				= pUnitType[1];
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_UP_YIELD] 				= pUnitType[1];
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_DOWN_YIELD] 			= pUnitType[1];
	}
	else
	{
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE] 				= pUnitType[0];
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_UP_YIELD] 				= pUnitType[0];
		g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_DOWN_YIELD] 			= pUnitType[0];
	}
	g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_STRENGTH]				= pUnitType[15];
	g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_UP_YIELD_STRENGTH] 		= pUnitType[15];
	g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_DOWN_YIELD_STRENGTH] 	= pUnitType[15];
	switch ( g_mainPage.bxChannelUnit )
	{
		case BX_UNIT_MM:
			g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= pUnitType[4];
			break;
		case BX_UNIT_CM:
			g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= pUnitType[12];
			break;
		case BX_UNIT_DM:
			g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= pUnitType[13];
			break;
		case BX_UNIT_M:
			g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_MAX_FORCE_ALL_EXTEND] 	= pUnitType[14];
			break;
	}
	g_mainPage.pKLTestResultParameterUnitArray[INDEX_KL_TOTAL_ELONGATION] 		= pUnitType[10];
	
	/* 小数点位数 */
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_SAMPLE_SERIAL].pointBit 		= 0;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_MAX_FORCE].pointBit 			= 2;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_STRENGTH].pointBit 			= 1;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_UP_YIELD].pointBit 			= 2;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_DOWN_YIELD].pointBit 			= 2;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_UP_YIELD_STRENGTH].pointBit 	= 1;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_DOWN_YIELD_STRENGTH].pointBit 	= 1;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_MAX_FORCE_ALL_EXTEND].pointBit = 2;
	g_mainPage.oneLevelMenuKLTestResult[INDEX_KL_TOTAL_ELONGATION].pointBit 	= 1;
	
	/* 试验名称 */
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
		case KLJSSW:
			g_mainPage.pTestTitle = pMainPageTestName[11];
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : FromTestParameterCopyToReport
 * Description    : 从试验参数拷贝到报告
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
	pReport->gz_area = pCurTest->gz_area;
	pReport->bgz_area = pCurTest->bgz_area;	
	pReport->sample_shape_index = pCurTest->sample_shape_index;
	pReport->yx_diameter = pCurTest->yx_diameter;
	pReport->parallelLenth = pCurTest->parallelLenth;
	pReport->extensometerGauge = pCurTest->extensometerGauge;
	pReport->originalGauge = pCurTest->originalGauge;
	pReport->pipeThickness = pCurTest->pipeThickness;
	pReport->pipeOuterDiameter = pCurTest->pipeOuterDiameter;
	
	{
		uint8_t i;
		
		for (i=0; i<MAX_RECORD_TEST_RESULT_NUM; ++i)
		{
			pReport->maxForce[i] = 0;
			pReport->maxStrength[i] = 0;
			pReport->upYieldForce[i] = 0;
			pReport->downYieldForce[i] = 0;
			pReport->upYieldStrength[i] = 0;
			pReport->downYieldStrength[i] = 0;
			pReport->maxForceSumExtend[i] = 0;
			pReport->maxForceSumElongation[i] = 0;
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageWriteSerial
 * Description    : 写入编号(编号从1开始)
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
 * Description    : 写入力值(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteForce( uint8_t index, float force )
{
	if ((index>=1) && (index<=MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM))
	{
		if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
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
 * Description    : 清除力值(编号从1开始)
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
 * Description    : 写入强度(编号从1开始)
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
 * Function Name  : MainPageClearStrength
 * Description    : 清除强度(编号从1开始)
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
 * Description    : 写入有效值
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
 * Description    : 写入有效力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteAvailForce( float force )
{
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
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
		strcpy(g_mainPage.testResultAvailData.availForce,"无效");
	}
	else
	{
		floattochar(TEST_RESULT_FORCE_MAX_CHAR_NUM,SHOW_FORCE_DOT_BIT,force,g_mainPage.testResultAvailData.availForce);	
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteAvailStrength
 * Description    : 写入有效强度
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
		strcpy(g_mainPage.testResultAvailData.availStrength,"无效");
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageClearKLTestData
 * Description    : 清除抗拉试验数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageClearKLTestData( uint8_t handle )
{
	uint8_t index = GetMainPageKLTestResultFieldIndex(handle);
	
	if (index != 0xff)
	{
		g_mainPage.kLTestResultData[index][0] = NULL;
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestSerial
 * Description    : 写入抗拉试验编号(编号从1开始)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestSerial( uint8_t serial )
{
	uint8_t index = 0;
	
	if ((serial>=1) && (serial<=MAX_RECORD_TEST_RESULT_NUM))
	{		
		index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_SAMPLE_SERIAL);
		
		if (index != 0xff)
		{
			numtochar(KL_TEST_MAX_CHAR_BIT,serial,g_mainPage.kLTestResultData[index]);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestMaxForce
 * Description    : 写入抗拉试验最大力
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestMaxForce( float maxForce )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
		
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_MAX_FORCE);
	
	if (index != 0xff)
	{
		if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
		{
			maxForce /= 1000;
		}
	
		if (maxForce > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,maxForce,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestStrength
 * Description    : 写入抗拉试验强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestMaxStrength( float maxStrength )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_STRENGTH);
	
	if (index != 0xff)
	{
		if (maxStrength > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,maxStrength,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestUpYieldForce
 * Description    : 写入上屈服力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestUpYieldForce( float upYieldForce )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_UP_YIELD);
	
	if (index != 0xff)
	{
		if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
		{
			upYieldForce /= 1000;
		}
	
		if (upYieldForce > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,upYieldForce,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestDownYieldForce
 * Description    : 写入下屈服力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestDownYieldForce( float downYieldForce )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_DOWN_YIELD);
	
	if (index != 0xff)
	{
		if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
		{
			downYieldForce /= 1000;
		}
	
		if (downYieldForce > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,downYieldForce,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestUpYieldStrength
 * Description    : 写入上屈服强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestUpYieldStrength( float upYieldStrength )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_UP_YIELD_STRENGTH);
	
	if (index != 0xff)
	{	
		if (upYieldStrength > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,upYieldStrength,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestDownYieldStrength
 * Description    : 写入下屈服强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestDownYieldStrength( float downYieldStrength )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_DOWN_YIELD_STRENGTH);
	
	if (index != 0xff)
	{	
		if (downYieldStrength > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,downYieldStrength,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestMaxForceAllExtend
 * Description    : 写入最大力总延伸
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestMaxForceAllExtend( float maxForceAllExtend )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_MAX_FORCE_ALL_EXTEND);
	
	if (index != 0xff)
	{
		switch ( g_mainPage.bxChannelUnit )
		{
			case BX_UNIT_MM:
				
				break;
			case BX_UNIT_CM:
				maxForceAllExtend /= 10;
				break;
			case BX_UNIT_DM:
				maxForceAllExtend /= 100;
				break;
			case BX_UNIT_M:
				maxForceAllExtend /= 1000;
				break;
		}
		
		if (maxForceAllExtend > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,maxForceAllExtend,g_mainPage.kLTestResultData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLTestMaxForceTotalElongation
 * Description    : 写入最大力总伸长率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLTestMaxForceTotalElongation( float maxForceTotalElongation )
{
	uint8_t index = 0;
	uint8_t dotNum = 0;
			
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_TOTAL_ELONGATION);
	
	if (index != 0xff)
	{
		if (maxForceTotalElongation > 9999999)
		{
			strcpy(g_mainPage.kLTestResultData[index],"--------");
			
			return;
		}
	
		dotNum = g_mainPage.oneLevelMenuKLTestResult[index].pointBit;
		
		floattochar(KL_TEST_MAX_CHAR_BIT,dotNum,maxForceTotalElongation,g_mainPage.kLTestResultData[index]);
	}
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
 * Description    : 获取已做试块页号
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
 * Function Name  : GetKL_TestCompletePiecePageNum
 * Description    : 获取抗拉试验已做试块页号
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetKL_TestCompletePiecePageNum( void )
{
	if (g_mainPage.sumPage == 0)
	{
		return 0;
	}
	
	return g_testBody.curCompletePieceSerial;
}

/*------------------------------------------------------------
 * Function Name  : IsCurrentSampleReachToPageTail
 * Description    : 判断当前试块是否已到达页尾
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
 * Description    : 自动更新编号
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
			PopWindowsProcessCycle();
			
			g_mainPage.leavePage.flagLeavePage = SET;
			g_mainPage.leavePage.flagSaveData = RESET;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKZKYParameter
 * Description    : 写入抗折抗压参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKZKYParameter( void )
{
	uint8_t baseIndex = 0;
	uint8_t index = 0;
	uint8_t i;
	
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
		MainPageWriteAvailValue("有效值");
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
}	

/*------------------------------------------------------------
 * Function Name  : MainPageWriteKLParameter
 * Description    : 写入抗拉参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageWriteKLParameter( void )
{
	if (g_mainPage.curPage == 0)
	{
		return;
	}
	
	MainPageWriteKLTestSerial(g_mainPage.curPage);
	MainPageWriteKLTestMaxForce(g_readReport.maxForce[g_mainPage.curPage-1]);
	MainPageWriteKLTestMaxStrength(g_readReport.maxStrength[g_mainPage.curPage-1]);
	MainPageWriteKLTestUpYieldForce(g_readReport.upYieldForce[g_mainPage.curPage-1]);
	MainPageWriteKLTestDownYieldForce(g_readReport.downYieldForce[g_mainPage.curPage-1]);
	MainPageWriteKLTestUpYieldStrength(g_readReport.upYieldStrength[g_mainPage.curPage-1]);
	MainPageWriteKLTestDownYieldStrength(g_readReport.downYieldStrength[g_mainPage.curPage-1]);
	MainPageWriteKLTestMaxForceAllExtend(g_readReport.maxForceSumExtend[g_mainPage.curPage-1]);
	MainPageWriteKLTestMaxForceTotalElongation(g_readReport.maxForceSumElongation[g_mainPage.curPage-1]);
}

/*------------------------------------------------------------
 * Function Name  : MainPageReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageReadParameter( void )
{
	FRESULT result;
	uint8_t remain = 0;
	
	result = report_read(g_mainPage.testType,pTest->test_serial,&g_readReport);
	if (result != FR_OK)	//未发现报告
	{
		memset(&g_readReport,0x00,sizeof(REPORT_TypeDef));
		
		FromTestParameterCopyToReport(&g_readReport);
	}
	
	if ((g_readReport.sample_num==0) || (g_readReport.sample_num>MAX_RECORD_TEST_RESULT_NUM))
	{
		g_mainPage.curPage = 0;
		
		return;
	}
	
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			/* 试块总个数 */
			g_mainPage.sumSampleNum = g_readReport.sample_num;
			
			/* 总页数 */
			remain = g_mainPage.sumSampleNum % MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;	
			if (remain)
			{
				g_mainPage.sumPage = g_mainPage.sumSampleNum / MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM + 1;
			}
			else
			{
				g_mainPage.sumPage = g_mainPage.sumSampleNum / MAX_TEST_RESULT_TABLE_ONE_PAGE_NUM;
			}
			
			/* 当前页*/
			if (g_mainPage.curPage > g_mainPage.sumPage)
			{
				g_mainPage.curPage = 0;
			}
			
			/* 当前页试块个数 */
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
			break;
		case STRETCH_TEST:	
			/* 试块总个数 */
			g_mainPage.sumSampleNum = g_readReport.sample_num;
			
			/* 总页数 */
			g_mainPage.sumPage = g_readReport.sample_num;
			
			/* 当前页*/
			if (g_mainPage.curPage > g_mainPage.sumPage)
			{
				g_mainPage.curPage = 0;
			}
			
			/* 当前页试块个数 */
			if (g_mainPage.curPage)
			{
				g_mainPage.curPageSampleNum = 1;
			}
			else
			{
				g_mainPage.curPageSampleNum = 0;
			}
			break;
	}	
	g_testBody.curCompletePieceSerial = g_readReport.sample_serial;
	
	/* 标志位 */
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
	
	/* 试验结果表格信息 */
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			MainPageWriteKZKYParameter();
			break;
		case STRETCH_TEST:	
			MainPageWriteKLParameter();
			break;
	}
	
	/* 自动更新编号 */
	MainPageAutoUpdateSerial();
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageIndicateWindowsOneRectangleFrameCoordinate
 * Description    : 配置示值窗GUI坐标
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
		case OBJECT_WINDOWS_DISPLACEMENT:
			g_mainPage.oneLevelMenuIndicateWindows[rowIndex][fieldIndex].lenth = 230;	
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageIndicateWindowsRectangleFrameCoordinate
 * Description    : 配置示值窗GUI坐标
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
 * Description    : 配置试验结果GUI坐标
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
 * Description    : 配置试验结果GUI坐标
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
 * Description    : 配置试验结果GUI坐标
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
 * Description    : 画1个示值窗
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
 * Description    : 画示值窗
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
 * Description    : 界面GUI
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
 * Description    : 界面GUI
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
 * Function Name  : GUI_MainPageIndicateWindowOneRowField
 * Description    : 表格菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageIndicateWindowOneField( uint8_t rowIndex, uint8_t fieldIndex )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint16_t lenth = 0;
	uint16_t width = 0;
	uint8_t fieldLen = 0;
	
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


/*------------------------------------------------------------
 * Function Name  : GUI_MainPageIndicateWindowOneRowField
 * Description    : 表格菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageIndicateWindowOneRowField( uint8_t rowIndex )
{
	uint8_t fieldIndex = 0;
	
	for (fieldIndex=0; fieldIndex<g_mainPage.indicateWindowNum; ++fieldIndex)
	{		
		GUI_MainPageIndicateWindowOneField(rowIndex,fieldIndex);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageIndicateWindowField
 * Description    : 示值窗菜单字段
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageIndicateWindowField( void )
{
	uint8_t rowIndex = 0;
	
	for (rowIndex=0; rowIndex<INDICATE_WINDOWS_ROW_NUMS; ++rowIndex)
	{		
		GUI_MainPageIndicateWindowOneRowField(rowIndex);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageTableTitleField
 * Description    : 表格菜单字段
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
 * Description    : 显示框线
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
 * Description    : 显示框线
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
 * Description    : 配置状态栏GUI坐标
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
 * Description    : 配置状态栏GUI坐标
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
 * Description    : 画状态栏框线
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
 * Description    : 表格菜单字段
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
 * Description    : 加载页面信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageLoadPageInfomation( void )
{
	lcd_show_image(551,390,PIC_DIR_LEFT_RIGHT);
}	

/*------------------------------------------------------------
 * Function Name  : LoadDefaultCoordinate
 * Description    : 加载默认坐标系
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadDefaultCoordinate( void )
{
	uint32_t maxForce = 0,maxTime = 0;
	
	maxForce = smpl_ctrl_full_p_get(SMPL_FH_NUM);
	
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		maxForce /= 1000;
	}
	
	maxForce >>= 1;
	
	maxTime = 20;
	
	GUI_MainPageDrawCoordinate(maxForce,maxTime);
	
	InitMainPageCoordinateDrawLine();
}

/*------------------------------------------------------------
 * Function Name  : ConfigMainPageKLTestRectangleFrameCoordinate
 * Description    : 配置抗拉试验结果界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMainPageKLTestRectangleFrameCoordinate( uint16_t startX, uint16_t startY, uint8_t rowDistance )
{
	uint8_t i;
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		g_mainPage.oneLevelMenuKLTestResult[i].x = startX;
		g_mainPage.oneLevelMenuKLTestResult[i].y = startY;
		g_mainPage.oneLevelMenuKLTestResult[i].pointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuKLTestResult[i].backColor = COLOR_BACK;
		g_mainPage.oneLevelMenuKLTestResult[i].recordPointColor = COLOR_POINT;
		g_mainPage.oneLevelMenuKLTestResult[i].recordBackColor = COLOR_BACK;
		g_mainPage.oneLevelMenuKLTestResult[i].lenth = 156;
		g_mainPage.oneLevelMenuKLTestResult[i].width = 30;
		g_mainPage.oneLevelMenuKLTestResult[i].fontSize = 24;
		g_mainPage.oneLevelMenuKLTestResult[i].rowDistance = rowDistance;
		g_mainPage.oneLevelMenuKLTestResult[i].columnDistance = 0;
		g_mainPage.oneLevelMenuKLTestResult[i].lineWidth = 2;
		
		startY += g_mainPage.oneLevelMenuKLTestResult[i].width + g_mainPage.oneLevelMenuKLTestResult[i].rowDistance - \
				  g_mainPage.oneLevelMenuKLTestResult[i].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageKLTestDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageKLTestDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_mainPage.oneLevelMenuKLTestResult[index].x;
	rectangle.y = g_mainPage.oneLevelMenuKLTestResult[index].y;
	rectangle.lenth = g_mainPage.oneLevelMenuKLTestResult[index].lenth;
	rectangle.width = g_mainPage.oneLevelMenuKLTestResult[index].width;
	rectangle.lineWidth = g_mainPage.oneLevelMenuKLTestResult[index].lineWidth;
	rectangle.lineColor = g_mainPage.oneLevelMenuKLTestResult[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageKLTestRectangleFrame
 * Description    : 抗拉试验界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageKLTestRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		GUI_MainPageKLTestDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageKLTestDrawOneRowOneLevelMenu
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageKLTestDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_mainPage.oneLevelMenuKLTestResult[index].x - 21 * (g_mainPage.oneLevelMenuKLTestResult[index].fontSize >> 1);
	const uint16_t y = g_mainPage.oneLevelMenuKLTestResult[index].y + \
					   g_mainPage.oneLevelMenuKLTestResult[index].lineWidth + 1;
	const uint16_t pointColor = g_mainPage.oneLevelMenuKLTestResult[index].pointColor;
	const uint16_t backColor = g_mainPage.oneLevelMenuKLTestResult[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.pKLTestResultParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterOneLevelMenu
 * Description    : 试验参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageKLTestOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		GUI_MainPageKLTestDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageKLTestOneRowOneLevelMenuUnit
 * Description    : 显示一行试验参数单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageKLTestOneRowOneLevelMenuUnit( uint8_t index )
{
	const uint16_t x = g_mainPage.oneLevelMenuKLTestResult[index].x + g_mainPage.oneLevelMenuKLTestResult[index].lenth - \
					   g_mainPage.oneLevelMenuKLTestResult[index].lineWidth - 5 * 12;
	const uint16_t y = g_mainPage.oneLevelMenuKLTestResult[index].y + g_mainPage.oneLevelMenuKLTestResult[index].lineWidth + 1;
	const uint16_t pointColor = g_mainPage.oneLevelMenuKLTestResult[index].pointColor;
	const uint16_t backColor = g_mainPage.oneLevelMenuKLTestResult[index].backColor;
	
	if ( strcmp(g_mainPage.pKLTestResultParameterUnitArray[index],"NULL") )
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.pKLTestResultParameterUnitArray[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_MainPageKLTestOneLevelMenuUnit
 * Description    : 显示一级菜单单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageKLTestOneLevelMenuUnit( void )
{
	uint8_t i;
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		Show_MainPageKLTestOneRowOneLevelMenuUnit(i);
	}
}	

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageDrawTetResultArea
 * Description    : 画实验结果区域
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageDrawTetResultArea( void )
{
	if (g_mainPage.curPage)
	{
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
				/* 试验结果表格 */
				ConfigMainPageTableTestResultRectangleFrameCoordinate();	
				GUI_MainPageRectangleFrame();		
				GUI_MainPageTableTitleField();
				break;
			case STRETCH_TEST:	
				ConfigMainPageKLTestRectangleFrameCoordinate(630,150,0);
				GUI_MainPageKLTestRectangleFrame();
				GUI_MainPageKLTestOneLevelMenu();
				break;
		}		
	}
	else
	{
		/* 试验信息区 */
		LoadMainPageGetTestInfomationPage();
	}
	
	if (g_mainPage.curPage)
	{
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
				GUI_MainPageLoadPageInfomation();
				break;
			case STRETCH_TEST:	
				break;
		}		
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPage
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPage( void )
{	
	/* 示值窗 */
	ConfigMainPageIndicateWindowsRectangleFrameCoordinate();
	GUI_MainPageDrawIndicateWindowsRectangleFrame();
	GUI_MainPageIndicateWindowField();
	
	/* 坐标系 */
	LoadDefaultCoordinate();
	
	/* 试验结果区域 */
	GUI_MainPageDrawTetResultArea();
	
	/* 中间框线 */
	GUI_MainPageShowFrame();
	
	/* 标题 */
	GUI_MainPageShowTestTitle();
	
	/* 状态栏 */
	ConfigMainPageStatusBarRectangleFrameCoordinate();
	GUI_MainPageDrawStatusBarRectangleFrame();
	GUI_MainPageStatusBarField();
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPage
 * Description    : 界面GUI
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
 * Description    : 显示一个示值窗内容
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
			RefreshDynamicSpeed(x+12,y,pointColor,backColor,GetInterfaceElementFHSpeed());
			break;
		case OBJECT_WINDOWS_STRENGTH:
			RefreshDynamicStrength(x+12,y,pointColor,backColor,GetInterfaceElementStrength());
			break;
		case OBJECT_WINDOWS_PEAK:
			RefreshDynamicPeak(x+43,y,pointColor,backColor,GetInterfaceElementPeak());
			break;
		case OBJECT_WINDOWS_DISPLACEMENT:
			if (GetDisplacementOrDeformShow() == SHOW_DISPLACEMENT)
			{
				RefreshDynamicDisplacement(x+20,y,pointColor,backColor,GetInterfaceElementDisPlacement());
			}
			else
			{
				RefreshDynamicDeform(x+20,y,pointColor,backColor,GetInterfaceElementDeform());
			}
			break;
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_MainPageOneStatusBarContent
 * Description    : 显示一个状态栏内容
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
 * Description    : 刷新动态内容
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
//		case REFRESH_PEAK:
		case REFRESH_DISPLACEMENT:
			indexField = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_DISPLACEMENT);
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
 * Description    : 清除坐标系区域
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
 * Description    : 画坐标系
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
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
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
 * Description    : 显示一个字段内容
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
 * Description    : 显示试验结果表格(rowNum：从1开始)
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
 * Description    : 遍历试验结果表格内容
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
 * Description    : 显示有效值
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
 * Description    : 显示试验结果表格有效值
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
 * Description    : 遍历试验结果表格有效值内容
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
 * Function Name  : Show_MainPageKLTestResultOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageKLTestResultOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_mainPage.oneLevelMenuKLTestResult[index].x + \
					   g_mainPage.oneLevelMenuKLTestResult[index].lineWidth + 1;
	const uint16_t y = g_mainPage.oneLevelMenuKLTestResult[index].y + \
					   g_mainPage.oneLevelMenuKLTestResult[index].lineWidth + 1;
	const uint16_t pointColor = g_mainPage.oneLevelMenuKLTestResult[index].pointColor;
	const uint16_t backColor = g_mainPage.oneLevelMenuKLTestResult[index].backColor;
	const uint16_t lenth = g_mainPage.oneLevelMenuKLTestResult[index].lenth - \
						   2 * g_mainPage.oneLevelMenuKLTestResult[index].lineWidth - 2;
	const uint16_t width = g_mainPage.oneLevelMenuKLTestResult[index].width - \
						   2 * g_mainPage.oneLevelMenuKLTestResult[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_mainPage.kLTestResultData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MainPageKLTestResultOneLevelMenuContent( void )
{
	uint8_t i;
	uint8_t index = 0;
	
	index = GetMainPageKLTestResultFieldIndex(OBJECT_KL_SAMPLE_SERIAL);
	if (index != 0xff)
	{
		Show_MainPageKLTestResultOneRowOneLevelMenuContent(index);
	}
	
	if (g_mainPage.curPage > g_testBody.curCompletePieceSerial)
	{
		return;
	}
	
	for (i=0; i<g_mainPage.klFieldNum; ++i)
	{
		Show_MainPageKLTestResultOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TestResultTable
 * Description    : 遍历试验结果表格
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TestResultTable( void )
{	
	if (g_mainPage.curPage)
	{		
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
				Traverse_MainPageTestResultTableContent();
		
				Traverse_MainPageTestResultTableAvailValue();
				break;
			case STRETCH_TEST:
				Show_MainPageKLTestResultOneLevelMenuContent();
				
				Show_MainPageKLTestOneLevelMenuUnit();
				break;
			}
	}
}

/*------------------------------------------------------------
 * Function Name  : IsCompleteSampleSerialAtThisPage
 * Description    : 已做完的试块的编号是否在当前页
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
 * Description    : 清除光标
 * Input          : nowSampleSerial：当前已做试块个数
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
	
	/* 清除上一个光标 */
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
 * Description    : 移动光标到下一个未处理的区域
 * Input          : nowSampleSerial：当前已做试块个数
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
	
	/* 加载下一个光标 */
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
 * Description    : 擦除试验结果区域
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ClearTestResultArea( uint16_t color )
{
	lcd_fill(364,140,430,285,color);
}

/*------------------------------------------------------------
 * Function Name  : ReloadTestResultArea
 * Description    : 重新加载试验结果区域
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReloadTestResultArea( void )
{
	ClearTestResultArea(COLOR_BACK);
	
	/* 参数初始化 */
	MainPageInit();
	
	/* 获取参数 */
	MainPageReadParameter();
	
	/* 试验结果区域 */
	GUI_MainPageDrawTetResultArea();
	
	/* 遍历 */
	Traverse_TestResultTable();
}

/*------------------------------------------------------------
 * Function Name  : MainPageLeavePageCheckCycle
 * Description    : 离开页检测
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
 * Description    : 快捷菜单任务
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
 * Description    : 向左翻页
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
 * Description    : 向右翻页
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
 * Description    : 获取报告匹配信息
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
 * Description    : 打印报告
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
			errStatus = PrintTestReport(SMPL_FH_NUM,g_mainPage.testType,&readReport,&testInfo);
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
#if 0
/*------------------------------------------------------------
 * Function Name  : MainPageExportReport
 * Description    : 导出报告
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
	
	result = report_save(USB,g_mainPage.testType,pTest->test_serial,&readReport);
	if (result != FR_OK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[11]);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		return;
	}
		
	result = SetReportSaveTime(MMC,USB,g_mainPage.testType,pTest->test_serial);	
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
 * Description    : 按下打印键
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

#endif

/*------------------------------------------------------------
 * Function Name  : MainPagePressKEY_F1
 * Description    : 按下F1
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
 * Description    : 撤销上一块
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
	
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			if (GetCompletePiecePageNum() != g_mainPage.curPage)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,2,&pMainPageWarning[17]);
				g_mainPage.leavePage.flagLeavePage = SET;
				g_mainPage.leavePage.flagSaveData = RESET;
				
				return;
			}
			break;
		case STRETCH_TEST:	
			if (GetKL_TestCompletePiecePageNum() != g_mainPage.curPage)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,2,&pMainPageWarning[17]);
				g_mainPage.leavePage.flagLeavePage = SET;
				g_mainPage.leavePage.flagSaveData = RESET;
				
				return;
			}			
			break;
	}
	
	g_testBody.flagCancelLastPiece = SET;
	
	SetTestStatus(TEST_SAVE);
}

/*------------------------------------------------------------
 * Function Name  : MainPagePressKEY_F2
 * Description    : 按下F2
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
 * Description    : 结束一组
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
 * Description    : 按下F3
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
		/* 设置用户登录 */
		SetPasswordType(USER_OR_MANAGER_PASSWORD);
		SetPasswordPutinSourcePage(MAIN_PAGE);
		SetPasswordPutinTargetPage(CHANNEL_SELECT_PAGE);
		SetPage(USER_LOGIN_PAGE);
		
		/* 设置通道选择 */
		SetChannelSelectSourcePage(MAIN_PAGE);
		SetChannelSelectTargetPage(SYSTEM_SET);
		
		g_mainPage.leavePage.flagLeavePage = SET;
		g_mainPage.leavePage.flagSaveData = RESET;
		
		InitLssuedParameter();
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageKeyProcess
 * Description    : 按键处理
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
				if (SendChannelTareCmd(SMPL_FH_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[3]);
			
					g_mainPage.leavePage.flagLeavePage = SET;
					g_mainPage.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_DISPLACE_TARE:
				if (SendChannelTareCmd(SMPL_WY_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[14]);
			
					g_mainPage.leavePage.flagLeavePage = SET;
					g_mainPage.leavePage.flagSaveData = RESET;
				}
				else
				{
					g_klTestBody.originalDisplacemen = 0.0f;
					g_klTestBody.originalDeform = 0.0f;
				}
				break;
			case KEY_DEFORMATE_TARE:
				if (SendChannelTareCmd(SMPL_BX_NUM) == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[15]);
			
					g_mainPage.leavePage.flagLeavePage = SET;
					g_mainPage.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_DISPLACE_DEFORMATE:
				{
					uint8_t index = 0;
					
					if (GetDisplacementOrDeformShow() == SHOW_DISPLACEMENT)
					{
						SetDisplacementOrDeformShow(SHOW_DEFORM);
					}
					else
					{
						g_klTestBody.originalDisplacemen = get_smpl_value(SMPL_WY_NUM);
						g_klTestBody.originalDeform = get_smpl_value(SMPL_BX_NUM);
						
						SetDisplacementOrDeformShow(SHOW_DISPLACEMENT);
					}
					MainPageConfig();
					index = GetMainPageIndicateWindowsFieldIndex(OBJECT_WINDOWS_DISPLACEMENT);
					if (index != 0xff)
					{
						/* 擦除显示 */
						GUI_MainPageIndicateWindowOneField(0,index);
						GUI_MainPageIndicateWindowOneField(1,index);
						
						/* 清除显示缓存 */
						disp_syn(DISP_CHN_WY);
						disp_syn(DISP_CHN_BX);
					}		
				}
				break;
//			case KEY_PRINT:
//				MainPagePressPrintKey();
//				break;
//			case KEY_EXPORT:
//				MainPageExportReport();
//				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentStrength
 * Description    : 设置动态内容强度
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
		force = get_smpl_value(FH_UNIT_kN);
	
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
 * Description    : 设置动态内容峰值
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
				g_testBody.breakPeak = GetPeakValue(SMPL_FH_NUM);
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
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		peak /= 1000;
	}
	
	SetInterfaceElementPeak(peak);
}	

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentTestStatus
 * Description    : 设置动态内容试验状态
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
 * Description    : 设置动态内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetDynamicContentTask( void )
{
	SetDynamicContentForce(g_mainPage.fhChannelUnit);
	SetDynamicContentFHSpeed(g_mainPage.fhChannelUnit);
	SetDynamicContentPeak();
	SetDynamicContentStrength();
	if (GetDisplacementOrDeformShow() == SHOW_DISPLACEMENT)
	{
		SetDynamicContentDispalcement(g_mainPage.wyChannelUnit);
	}
	else
	{
		SetDynamicContentDeform(g_mainPage.bxChannelUnit);
	}
	
	SetDynamicContentLinkStatus();
	SetDynamicContentTestStatus();
	SetDynamicContentCode(SMPL_FH_NUM);
	SetDynamicContentUSBStatus();
	SetDynamicContentNETStatus();
}

/*------------------------------------------------------------
 * Function Name  : MainPageCheckSerialRepeat
 * Description    : 检测编号重复
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
			ClearExceptionFlag();	//不进行提醒
			strcpy(pTest->test_serial,str_buff);
			pcm_save();
		
			g_mainPage.curPage = 1;
			ReloadTestResultArea();
		
			ECHO(DEBUG_TEST_LOAD,"自动编号加1！\r\n");

			return PASSED;
		
		case SERIAL_NO_CHG:
			return PASSED;
		
		default: 
			return FAILED;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageAllowRunTest
 * Description    : 允许运行试验
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FunctionalState MainPageAllowRunTest( void )
{
	int32_t remainDay = 0;
	EXPIRE_STATUS_TypeDef activeStatus;
	
	/* 检测机型 */
	if (CheckCurrentModel() == FAILED)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[16]);
		
		return DISABLE;
	}
	
	/* 检测联机状态 */
	if (GetLinkStatus() == LINK_UNLINK)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[0]);
		
		return DISABLE;
	}
	
	/* 检测试块个数 */
	if (g_mainPage.sumSampleNum == 0)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[1]);
		
		return DISABLE;
	}
	
	/* 检测试件编号 */
	if (MainPageCheckSerialRepeat() == FAILED)
	{
		return DISABLE;
	}
	
	/* 检测激活状态 */
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
	
	ECHO(DEBUG_TEST_LOAD,"允许试验！\r\n");
	
	return ENABLE;
}	

/*------------------------------------------------------------
 * Function Name  : KZKY_TestJumpTestResultPage
 * Description    : 抗折抗压试验跳转到试验结果页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void KZKY_TestJumpTestResultPage( void )
{
	uint8_t curPage = GetCompletePiecePageNum();
	
	if(curPage != g_mainPage.curPage)
	{
		g_mainPage.curPage = curPage;
		
		ReloadTestResultArea();
		
		ECHO(DEBUG_TEST_LOAD,"跳转到已做试块页面！\r\n");
	}
	
	if (IsCurrentSampleReachToPageTail() == YES)
	{
		g_mainPage.curPage++;
		
		ReloadTestResultArea();
		
		ECHO(DEBUG_TEST_LOAD,"当前页试块已做完！\r\n");
	}
	else
	{
		MainPageClearCursor(g_testBody.curCompletePieceSerial);
		MainPageLoadCursor(g_testBody.curCompletePieceSerial+1);
	}
}

/*------------------------------------------------------------
 * Function Name  : KL_TestJumpTestResultPage
 * Description    : 抗拉试验跳转到试验结果页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void KL_TestJumpTestResultPage( void )
{
	uint8_t curPage = GetKL_TestCompletePiecePageNum();
	uint8_t targetPage = 0;
	
	targetPage = (curPage + 1) % (g_mainPage.sumPage + 1);
	
	if (g_mainPage.curPage != targetPage)
	{
		g_mainPage.curPage = targetPage;
		
		ReloadTestResultArea();
		
		ECHO(DEBUG_TEST_LOAD,"跳转到已做试块页面！\r\n");
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteTestStartBody
 * Description    : 试验开始
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteTestStartBody( void )
{
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			/* 初始化判破 */
			InitJudgeBreakPoint();
		
			/* 跳转到试验结果页 */
			KZKY_TestJumpTestResultPage();
			
			g_mainPage.refreshShortcut = ENABLE;
			break;
		case STRETCH_TEST:	
			/* 初始化判破 */
			InitJudgeBreakPoint();
			
			/* 初始化抗拉试验 */
			InitKL_TestBody(&g_klTestBody);
			
			#if 0
				InitKL_TestProgress();
				SetKL_TestProgress(STATUS_KL_PROGRESS_IDLE);
				SetKL_TestProgress(STATUS_KL_PROGRESS_ELASTIC_DEFORMATION);
			#endif
		
			/* 跳转到试验结果页 */
			KL_TestJumpTestResultPage();
			
			/* 此时菜单栏正在显示进度，不能刷新 */
			g_mainPage.refreshShortcut = DISABLE;
			break;
	}

	LoadDefaultCoordinate();
	
	g_testBody.startTest = SET;
	g_testBody.endTest = RESET;
	
	g_testBody.flagOnePieceSampleComplete = RESET;
	g_testBody.flagOneGroupSampleComplete = RESET;
	g_testBody.flagStartJudgeBreak = RESET;
	
	g_testBody.breakPeak = 0;
	g_testBody.breakStrength = 0;
	
	SetTestStatus(TEST_LOAD);
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteTestEndBody
 * Description    : 试验结束
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
 * Description    : 判破循环体
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
	
	breakStartValue = GetTargetBreakStartValue(SMPL_FH_NUM);
	curForce = get_smpl_value(SMPL_FH_NUM);
	
	if (curForce > breakStartValue)
	{
		if (g_testBody.flagStartJudgeBreak == RESET)
		{
			g_testBody.flagStartJudgeBreak = SET;
			
			ECHO(DEBUG_TEST_LOAD,"到达起判力值！\r\n");
		}
	}
	
	if (g_testBody.flagStartJudgeBreak == RESET)
	{
		return;
	}
	
	breakType = GetJudgeBreakType(SMPL_FH_NUM);
	
	switch ( breakType )
	{
		case ATTENUATION_RATE:	
			nowAttenuationRate = GetAttenuationRate(SMPL_FH_NUM);
			targetAttenuationRate = GetTargetAttenuationRate(SMPL_FH_NUM);
			
			if (nowAttenuationRate > targetAttenuationRate)
			{
				ECHO(DEBUG_TEST_LOAD,"衰减率判破！\r\n");
				
				isBreak = YES;
			}
			break;
		case WITH_MAX_FORCE_DIFFERENCE:
			nowWithMaxForceDiff = GetWithMaxForceDifference(SMPL_FH_NUM);
			targetWithMaxForceDiff = GetTargetWithMaxForceDifference(SMPL_FH_NUM);
			nowDownPoint = GetBreakDownPoint(SMPL_FH_NUM);
			targetDownPoint = GetTargetBreakDownPoint(SMPL_FH_NUM);
		
			if ((nowWithMaxForceDiff>targetWithMaxForceDiff) && (nowDownPoint>targetDownPoint))
			{
				ECHO(DEBUG_TEST_LOAD,"与最大力差值判破！\r\n");
				
				isBreak = YES;
			}
			break;
		case ADJOIN_TWO_POINT_DIFFERENCE:
			nowAdjoinTwoPointDiff = GetAdjoinTwoPointDifference(SMPL_FH_NUM);
			targetAdjoinTwoPointDiff = GetTargetAdjoinTwoPointDiff(SMPL_FH_NUM);
			nowDownPoint = GetBreakDownPoint(SMPL_FH_NUM);
			targetDownPoint = GetTargetBreakDownPoint(SMPL_FH_NUM);
		
			if ((nowAdjoinTwoPointDiff>targetAdjoinTwoPointDiff) && (nowDownPoint>targetDownPoint))
			{
				ECHO(DEBUG_TEST_LOAD,"相邻两点差值判破！\r\n");
				
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
 * Function Name  : AccordDispalcementGetDeformIncrement
 * Description    : 获取变形增量
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static float AccordDispalcementGetDeformIncrement( float dispalcement )
{
	float originalGauge = GetOriginalGauge();
	float extensometerGauge = GetExtensometerGauge();
	float deformIncrement = 0;
	const float COF = 1.0f;
	
	if (fabs(originalGauge) < MIN_FLOAT_PRECISION_DIFF_VALUE)
	{
		originalGauge = 1;
	}
	
	deformIncrement = COF * (extensometerGauge / originalGauge) * (dispalcement - g_klTestBody.originalDisplacemen);
	
	ECHO_ASSERT(fabs(originalGauge)>MIN_FLOAT_PRECISION_DIFF_VALUE,"原始标距除零错误！\r\n");
	
	return deformIncrement;
}

/*------------------------------------------------------------
 * Function Name  : MainPageGetDeform
 * Description    : 获取变形量
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static float MainPageGetDeform( void )
{
	float deform = 0;
	
	if (g_mainPage.testAttribute != STRETCH_TEST)
	{
		return 0;
	}
	
	if (GetDisplacementOrDeformShow() == SHOW_DISPLACEMENT)
	{
		deform = g_klTestBody.originalDeform + AccordDispalcementGetDeformIncrement( get_smpl_value(SMPL_WY_NUM) );
	}
	else
	{
		deform = get_smpl_value(SMPL_BX_NUM);
	}
	
	ECHO(DEBUG_DEFORM_SHOW,"deform： %f \r\n",deform);
	
	return deform;
}

/*------------------------------------------------------------
 * Function Name  : KL_TestCheckPeakCycle
 * Description    : 抗拉试验检测峰值循环
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void KL_TestCheckPeakCycle( void )
{
	static float s_recordPeak = 0;

	float peak = GetPeakValue(SMPL_FH_NUM);		
	float deform = MainPageGetDeform();
	
	if ( fabs(peak-s_recordPeak) > MIN_FLOAT_PRECISION_DIFF_VALUE)
	{
		g_klTestBody.maxForceIndex = GetDrawLineNowTimePoint();		
		g_klTestBody.maxForce = GetDrawLineSomeTimePointForce( g_klTestBody.maxForceIndex );
		
		g_klTestBody.maxForceSumExtend = deform;
		
		s_recordPeak = peak;
	}
}

/*------------------------------------------------------------
 * Function Name  : KL_TestLoadCoreCycle
 * Description    : 抗拉试验加载循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void KL_TestLoadCoreCycle( void )
{
	float force = get_smpl_value(SMPL_FH_NUM);
	float peak = GetPeakValue(SMPL_FH_NUM);
	
	/* 首次力值下降点 */
	if (force < peak)
	{
		g_klTestBody.upYieldForceIndex = GetDrawLineNowTimePoint();
		g_klTestBody.upYieldForce = GetDrawLineSomeTimePointForce( g_klTestBody.upYieldForceIndex );	
		g_klTestBody.upYieldStrength = FromForceGetStrength(g_mainPage.testType,&g_readReport,g_klTestBody.upYieldForce);
		
		SetTestStatus(TEST_DEFORM);
		
		ECHO(DEBUG_TEST_LOAD,"到达上屈服点！\r\n");
		ECHO(DEBUG_TEST_LOAD,"上屈服力值：%f, 上屈服强度：%f\r\n",g_klTestBody.upYieldForce,g_klTestBody.upYieldStrength);
	}
}
#if 0
	/*------------------------------------------------------------
	 * Function Name  : KL_TestYieldCoreCycle
	 * Description    : 抗拉试验屈服段循环体
	 * Input          : None
	 * Output         : None
	 * Return         : None
	 *------------------------------------------------------------*/
	static void KL_TestYieldCoreCycle( void )
	{
		float force = get_smpl_value(SMPL_FH_NUM);
		float startLoadForce = smpl_ctrl_entry_get(SMPL_FH_NUM);
		
		/* 记录屈服阶段最低力值点 */
		if (force < g_klTestBody.downYieldForce)
		{
			g_klTestBody.downYieldForce = force;
		}
		
		/* 离开屈服段 */
		if (force > g_klTestBody.upYieldForce)
		{
			g_klTestBody.downYieldStrength = FromForceGetStrength(g_mainPage.testType,&g_readReport,g_klTestBody.downYieldForce);
			
			SetTestStatus(TEST_DEFORM);
			
			ECHO(DEBUG_TEST_LOAD,"离开屈服段！\r\n");
			ECHO(DEBUG_TEST_LOAD,"下屈服力值：%f, 下屈服强度：%f\r\n",g_klTestBody.downYieldForce,g_klTestBody.downYieldStrength);		
		}
		
		if (force < startLoadForce)
		{
			g_mainPage.refreshShortcut = ENABLE;
			
			SetTestStatus(TEST_IDLE);
			
			ECHO(DEBUG_TEST_LOAD,"试验停止！\r\n");
		}
	}
#endif

/*------------------------------------------------------------
 * Function Name  : KL_TestDeformCoreCycle
 * Description    : 抗拉试验塑性变形段循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void KL_TestDeformCoreCycle( void )
{
	float force = get_smpl_value(SMPL_FH_NUM);
	float startLoadForce = smpl_ctrl_entry_get(SMPL_FH_NUM);
	
	if (force < startLoadForce)
	{
		g_klTestBody.maxStrength = FromForceGetStrength(g_mainPage.testType,&g_readReport,g_klTestBody.maxForce);
		
		g_testBody.flagOnePieceSampleComplete = SET;
		
		g_testBody.curCompletePieceSerial++;
		
		if (g_testBody.curCompletePieceSerial >= g_mainPage.sumSampleNum)
		{
			g_testBody.flagOneGroupSampleComplete = SET;
		}
		
		MainPageExecuteTestEndBody();
		
		SetTestStatus(TEST_SAVE);
		
		ECHO(DEBUG_TEST_LOAD,"试验后处理！\r\n");
		ECHO(DEBUG_TEST_LOAD,"最大力：%f, 抗拉强度：%f\r\n",g_klTestBody.maxForce,g_klTestBody.maxStrength);
	}
}

/*------------------------------------------------------------
 * Function Name  : JudgeForceAvail
 * Description    : 判断力值是否有效
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
						{
							float tempf = 0;
							
							SortBubble((void *)force_buff,CNT_KYHNT_STANDARD,&tempf,compFloatData);
						}
						avg = force_buff[1];	//中间值作为有效值
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
			
			match_cnt = GetArrayEqualZeroIndex(CNT_KYHNT_STANDARD,g_readReport.collect_load,&illegal_index);	//折断面是否在集中载荷内？
			
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
								{
									float tempf = 0;
									
									SortBubble((void *)force_buff,CNT_KYHNT_STANDARD,&tempf,compFloatData);
								}
								avg = force_buff[1];	//中间值作为有效值
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
 * Description    : 判断强度是否有效
 * Input          : num：试件块数，match_sample:符合条件的试块标记
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
	uint8_t no_match[MAX_RECORD_TEST_RESULT_NUM] = {0};	//默认全部标记为有效
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
							if ( no_match_buff[i] )		//异常数据
							{
								if ( SUCCESS == ContrastFloatDataEqual(CNT_KYSNJS_STANDARD,g_readReport.strength,&illegal_index,press_buff[i]) )	//将有问题的强度值press_buff与原始强度值pPress逐一比对，找出其在原始数据对应的位置
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
			
			match_cnt = GetArrayEqualZeroIndex(CNT_KYHNT_STANDARD,g_readReport.collect_load,&illegal_index);	//折断面是否在集中载荷内？
				
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
	memcpy(match_sample,no_match, sizeof(uint8_t)*MAX_RECORD_TEST_RESULT_NUM);

	return PASSED;
}


/*------------------------------------------------------------
 * Function Name  : MainPageGetAvailForce
 * Description    : 获取有效力值
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
	
	ECHO(DEBUG_TEST_LOAD,"获取有效力值！\r\n");
}	

/*------------------------------------------------------------
 * Function Name  : MainPageGetAvailStrength
 * Description    : 获取有效强度
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

	ECHO(DEBUG_TEST_LOAD,"获取有效强度！\r\n");
}

/*------------------------------------------------------------
 * Function Name  : MainPageAskSampleFractureSurfaceProcess
 * Description    : 询问折断面信息
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
	
	ECHO(DEBUG_TEST_LOAD,"询问折断面信息！\r\n");
	
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
 * Description    : 自动打印报告
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
		
		ECHO(DEBUG_TEST_LOAD,"自动打印报告！\r\n");
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteCancelLastPiece
 * Description    : 执行撤销上一块
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteCancelLastPiece( void )
{
	if (g_testBody.flagCancelLastPiece == SET)
	{
		g_testBody.flagCancelLastPiece = RESET;
		
		ECHO(DEBUG_TEST_LOAD,"撤销上一块！\r\n");
		
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
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
				break;
			case STRETCH_TEST:	
				MainPageClearKLTestData(OBJECT_KL_MAX_FORCE);
				MainPageClearKLTestData(OBJECT_KL_STRENGTH);
				MainPageClearKLTestData(OBJECT_KL_UP_YIELD);
				MainPageClearKLTestData(OBJECT_KL_DOWN_YIELD);
				MainPageClearKLTestData(OBJECT_KL_UP_YIELD_STRENGTH);
				MainPageClearKLTestData(OBJECT_KL_DOWN_YIELD_STRENGTH);
				MainPageClearKLTestData(OBJECT_KL_MAX_FORCE_ALL_EXTEND);
				MainPageClearKLTestData(OBJECT_KL_TOTAL_ELONGATION);
			
				if ((g_mainPage.curPage!=0) && (g_testBody.curCompletePieceSerial!=0)) 
				{
					Show_MainPageKLTestResultOneLevelMenuContent();
					
					Show_MainPageKLTestOneLevelMenuUnit();
					
					g_readReport.maxForce[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.maxStrength[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.upYieldForce[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.downYieldForce[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.upYieldStrength[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.downYieldStrength[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.maxForceSumExtend[g_testBody.curCompletePieceSerial-1] = 0;
					g_readReport.maxForceSumElongation[g_testBody.curCompletePieceSerial-1] = 0;
					
					if (g_testBody.curCompletePieceSerial)
					{
						g_testBody.curCompletePieceSerial--;
					}
					g_readReport.sample_serial = g_testBody.curCompletePieceSerial;
				}
				break;
		}

		if (g_readReport.sample_serial)
		{
			g_testBody.isWriteToSD = YES;
		}
		else 
		{
			FRESULT result;
			
			result = report_delete(g_mainPage.testType,pTest->test_serial);
			
			if (result == FR_OK)
			{
				result = DeleteCoordinateFolder(MMC,g_mainPage.testType,pTest->test_serial);
			}
			
			if (result == FR_OK)
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
 * Description    : 执行结束一组
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
		
		ECHO(DEBUG_TEST_LOAD,"执行手动结束一组！\r\n");
	}
}

/*------------------------------------------------------------
 * Function Name  : GetMaxForceSumExtend
 * Description    : 获取最大力总延伸
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static float GetMaxForceSumExtend( void )
{
	return g_klTestBody.maxForceSumExtend;
}

/*------------------------------------------------------------
 * Function Name  : GetMaxForceSumElongation
 * Description    : 获取最大力总延伸率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static float GetMaxForceSumElongation( void )
{
	float maxForceSumExtend = GetMaxForceSumExtend();
	float maxForceSumElongation = 0;
	float parallelLenth = GetParallelLenth();
	
	if (fabs(parallelLenth) < MIN_FLOAT_PRECISION_DIFF_VALUE)
	{
		parallelLenth = 1;
	}
	
	maxForceSumElongation = maxForceSumExtend / parallelLenth * 100;
	
	ECHO_ASSERT(fabs(parallelLenth)>MIN_FLOAT_PRECISION_DIFF_VALUE,"平行长度除零错误！\r\n");
	
	return maxForceSumElongation;
}

/*------------------------------------------------------------
 * Function Name  : GetDownYieldForce
 * Description    : 获取下屈服力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static float GetDownYieldForce( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	
	if (g_klTestBody.upYieldForceIndex >= g_klTestBody.maxForceIndex)
	{
		float downYieldForce = 0;
		
		return downYieldForce;
	}
	
	{
		float tempf = 0;
		COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
		uint32_t num = (g_klTestBody.maxForceIndex - g_klTestBody.upYieldForceIndex) + 1;		
		
//		SetTime(0);
		
		SortBubble((void *)&(pDrawLine->force[g_klTestBody.upYieldForceIndex]),\
			num,&tempf,compFloatData);
		
//		GetTime(0);
		
		return pDrawLine->force[g_klTestBody.upYieldForceIndex];
	}
}

/*------------------------------------------------------------
 * Function Name  : PrintfUpYieldToMaxForceData
 * Description    : 打印上屈服到最大力段数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PrintfUpYieldToMaxForceData( void )
{
	{
		uint32_t i;
			
		ECHO(DEBUG_TEST_LOAD,"------------\r\n");
		
		if (g_klTestBody.upYieldForceIndex > g_klTestBody.maxForceIndex)
		{
			ECHO(DEBUG_TEST_LOAD,"上屈服点就是最大力！\r\n");
			
			return;
		}
		
		for (i=g_klTestBody.upYieldForceIndex; i<=g_klTestBody.maxForceIndex; ++i)
		{
			ECHO(DEBUG_TEST_LOAD,"%f\r\n",GetDrawLineSomeTimePointForce(i));
			bsp_DelayMS(100);
		}
	}
	
	{
		float tempf = 0;
		COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
		uint32_t diff = g_klTestBody.maxForceIndex - g_klTestBody.upYieldForceIndex + 1;
		uint32_t i;
		
		ECHO(DEBUG_TEST_LOAD,"------------\r\n");
		
		SortBubble((void *)&(pDrawLine->force[g_klTestBody.upYieldForceIndex]),diff,&tempf,compFloatData);
		for (i=g_klTestBody.upYieldForceIndex; i<=g_klTestBody.maxForceIndex; ++i)
		{
			ECHO(DEBUG_TEST_LOAD,"%f\r\n",GetDrawLineSomeTimePointForce(i));
			bsp_DelayMS(100);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageSaveCoordinateCurveToSD
 * Description    : 保存曲线信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageSaveCoordinateCurveToSD( void )
{	
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	COORDINATE_POINT_TypeDef *pCoordinatePoint = GetCoordinatePointAddr();
	
	pCoordinatePoint->xType = 0;
	pCoordinatePoint->yType = 0;
	pCoordinatePoint->xUint = 0;
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		pCoordinatePoint->yUint = 0;
	}
	else
	{
		pCoordinatePoint->yUint = 1;
	}
	pCoordinatePoint->maxValueX = pDrawLine->maxTime;
	pCoordinatePoint->maxValueY = pDrawLine->maxForce;
	pCoordinatePoint->systemMaxForce = smpl_ctrl_full_p_get(g_mainPage.fhChannelUnit);
	pCoordinatePoint->recordPointFreq = RECORD_COORDINATE_FREQ;
	pCoordinatePoint->nowUsePointNum = pDrawLine->nowTimePoint;
	pCoordinatePoint->maxPointNum = DECORD_COORDINATE_FORCE_NUM;
	pCoordinatePoint->xScalingCoefficient = pDrawLine->timeScalingCoefficient;
	pCoordinatePoint->yScalingCoefficient = pDrawLine->forceScalingCoefficient;
	memcpy(pCoordinatePoint->force,pDrawLine->force,sizeof(float) * DECORD_COORDINATE_FORCE_NUM);
	
	{
		FRESULT result = SaveCoordinatePoint(MMC,g_mainPage.testType,g_testBody.curCompletePieceSerial,\
							pTest->test_serial,pCoordinatePoint);
		if (result != FR_OK)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pMainPageWarning[19]);
			
			ECHO(DEBUG_TEST_LOAD,"保存曲线失败！\r\n");
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteEndOnePieceProcess
 * Description    : 结束一块
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteEndOnePieceProcess( void )
{
	if (g_testBody.flagOnePieceSampleComplete == SET)
	{
		g_testBody.flagOnePieceSampleComplete = RESET;
		
		ECHO(DEBUG_TEST_LOAD,"一块试块结束！\r\n");	
		ECHO(DEBUG_TEST_LOAD,"保存曲线...\r\n");

//		SetTime(0);
		MainPageSaveCoordinateCurveToSD();
//		GetTime(0);
		
		g_testBody.isExecuteEndGroup = YES;
		
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
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
				break;
			case STRETCH_TEST:	
				if (g_testBody.curCompletePieceSerial)
				{
					//PrintfUpYieldToMaxForceData();
					
					/* 获取下屈服点数据 */
					g_klTestBody.downYieldForce = GetDownYieldForce();
					g_klTestBody.downYieldStrength = FromForceGetStrength(g_mainPage.testType,&g_readReport,g_klTestBody.downYieldForce);
					
					g_klTestBody.maxForceSumElongation = GetMaxForceSumElongation();
				
					g_readReport.maxForce[g_testBody.curCompletePieceSerial-1] = g_klTestBody.maxForce;
					g_readReport.maxStrength[g_testBody.curCompletePieceSerial-1] = g_klTestBody.maxStrength;
					g_readReport.upYieldForce[g_testBody.curCompletePieceSerial-1] = g_klTestBody.upYieldForce;
					g_readReport.downYieldForce[g_testBody.curCompletePieceSerial-1] = g_klTestBody.downYieldForce;
					g_readReport.upYieldStrength[g_testBody.curCompletePieceSerial-1] = g_klTestBody.upYieldStrength;
					g_readReport.downYieldStrength[g_testBody.curCompletePieceSerial-1] = g_klTestBody.downYieldStrength;
					g_readReport.maxForceSumExtend[g_testBody.curCompletePieceSerial-1] = g_klTestBody.maxForceSumExtend;
					g_readReport.maxForceSumElongation[g_testBody.curCompletePieceSerial-1] = g_klTestBody.maxForceSumElongation;
					
					ECHO(DEBUG_TEST_LOAD,"下屈服力值：%f, 下屈服强度：%f\r\n",g_klTestBody.downYieldForce,g_klTestBody.downYieldStrength);
					ECHO(DEBUG_TEST_LOAD,"最大力总延伸：%f, 最大力总伸长率：%f\r\n",g_klTestBody.maxForceSumExtend,g_klTestBody.maxForceSumElongation);
				}
				
				if (g_mainPage.curPage)
				{
					MainPageWriteKLTestMaxForce(g_klTestBody.maxForce);
					MainPageWriteKLTestMaxStrength(g_klTestBody.maxStrength);
					MainPageWriteKLTestUpYieldForce(g_klTestBody.upYieldForce);
					MainPageWriteKLTestDownYieldForce(g_klTestBody.downYieldForce);
					MainPageWriteKLTestUpYieldStrength(g_klTestBody.upYieldStrength);
					MainPageWriteKLTestDownYieldStrength(g_klTestBody.downYieldStrength);
					MainPageWriteKLTestMaxForceAllExtend(g_klTestBody.maxForceSumExtend);
					MainPageWriteKLTestMaxForceTotalElongation(g_klTestBody.maxForceSumElongation);
					
					Show_MainPageKLTestResultOneLevelMenuContent();
					
					Show_MainPageKLTestOneLevelMenuUnit();
				}
				break;
		}
		
		g_readReport.sample_serial = g_testBody.curCompletePieceSerial;

		g_testBody.isWriteToSD = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteEndGroupProcess
 * Description    : 执行结束一组
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteEndOneGroupProcess( void )
{
	if ((g_testBody.isExecuteEndGroup==YES) && (g_testBody.flagOneGroupSampleComplete==SET))
	{			
		ECHO(DEBUG_TEST_LOAD,"一组试样结束！\r\n");
		
		g_readReport.test_is_complete = 1;
		
		switch ( g_mainPage.testAttribute )
		{
			case COMPRESSION_TEST:
			case BENDING_TEST:
				/* 必须先获取有效强度，再获取有效力值 */
				MainPageGetAvailStrength();
				MainPageGetAvailForce();
				
				MainPageWriteAvailForce(g_readReport.force_valid[0]);
				MainPageWriteAvailStrength(g_readReport.strength_valid[0]);
				
				if (g_mainPage.curPage)
				{
					Show_MainPageTestResultTableAvailValue(OBJECT_FORCE);
					Show_MainPageTestResultTableAvailValue(OBJECT_STRENGTH);
				}
				break;
			case STRETCH_TEST:	
				
				break;
		}
		
		g_testBody.isWriteToSD = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageExecuteWriteSDProcess
 * Description    : 执行写入SD
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageExecuteWriteSDProcess( void )
{
	if (g_testBody.isWriteToSD == YES)
	{
		ECHO(DEBUG_TEST_LOAD,"保存报告到SD卡！\r\n");
		
		if (FR_OK == report_save(MMC,g_mainPage.testType,pTest->test_serial,&g_readReport) )
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
 * Description    : 试验后处理执行循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTestAfterDisposeExecuteCycle( void )
{
	g_testBody.isWriteToSD = NO;
	g_testBody.isExecuteEndGroup = NO;
	g_testBody.isAutoPrintReport = NO;
	
	/* 询问折断面信息 */
	MainPageAskSampleFractureSurfaceProcess();
	
	/* 撤销上一块 */
	MainPageExecuteCancelLastPiece();
	
	/* 手动结束一组 */
	MainPageExecuteHandEndGroup();
	
	/* 一块试样结束 */
	MainPageExecuteEndOnePieceProcess();
	
	/* 一组试样结束 */
	MainPageExecuteEndOneGroupProcess();
	
	/* 写入SD卡 */
	MainPageExecuteWriteSDProcess();
	
	/* 自动打印报告 */
	MainPageAutoPrintReportProcess();
}			

/*------------------------------------------------------------
 * Function Name  : MainPageTestExecuteCoreCycle
 * Description    : 试验执行核心循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageTestExecuteCoreCycle( void )
{
	float force = get_smpl_value(SMPL_FH_NUM);
	float startLoadForce = smpl_ctrl_entry_get(SMPL_FH_NUM);
	
	switch ( GetTestStatus() )
	{
		case TEST_IDLE:
			if (force > startLoadForce)
			{
				if (MainPageAllowRunTest() == ENABLE)
				{
					MainPageExecuteTestStartBody();
					
					ECHO(DEBUG_TEST_LOAD,"试验加载！\r\n");
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
				
				ECHO(DEBUG_TEST_LOAD,"试验停止！\r\n");
			}
			
			switch ( g_mainPage.testAttribute )
			{
				case COMPRESSION_TEST:
				case BENDING_TEST:
					MainPageJudgeBreakCoreCycle();		
					break;
				case STRETCH_TEST:	
					KL_TestLoadCoreCycle();
					break;
			}			
			break;
		case TEST_YIELD:
			#if 0
				KL_TestYieldCoreCycle();
			#endif
			break;
		case TEST_DEFORM:
			KL_TestDeformCoreCycle();
			break;
		case TEST_BREAK:
			SetShortCutMenuCue(COLOR_POINT,DARKBLUE,"试样已破碎！");
		
			SetTestStatus(TEST_UNLOAD);
		
			ECHO(DEBUG_TEST_LOAD,"判破提示！\r\n");
			break;
		case TEST_UNLOAD:
			if (force < startLoadForce)
			{
				GUI_ClearShortcutMenu();
				
				MainPageExecuteTestEndBody();
				
				SetTestStatus(TEST_SAVE);
				
				ECHO(DEBUG_TEST_LOAD,"试验后处理！\r\n");
			}
			break;
		case TEST_SAVE:
			MainPageTestAfterDisposeExecuteCycle();
		
			SetTestStatus(TEST_IDLE);

			g_mainPage.refreshShortcut = ENABLE;

			ECHO(DEBUG_TEST_LOAD,"试验结束！\r\n");
			break;
		
		default:
			SetTestStatus(TEST_IDLE);	
			break;
	}
	
	switch ( g_mainPage.testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:	
			break;
		case STRETCH_TEST:	
			KL_TestCheckPeakCycle();
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : MainPageCheckWarn
 * Description    : 检测警告
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
 * Description    : 初始化坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitMainPageCoordinateDrawLine( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	COORDINATE_TypeDef *pCoordinate = GetCoordinateDataAddr();
	uint32_t maxForce = 0;
	
	maxForce = smpl_ctrl_full_p_get(SMPL_FH_NUM);
	
	pDrawLine->baseIndex = 0;
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
	pDrawLine->timeScalingCoefficient = 0.001;
	if (g_mainPage.fhChannelUnit == FH_UNIT_kN)
	{
		pDrawLine->forceScalingCoefficient = 0.001;
	}
	else
	{
		pDrawLine->forceScalingCoefficient = 1;
	}
	pDrawLine->recordPointFreq = RECORD_COORDINATE_FREQ;
	pDrawLine->pDrawCoordinate = GUI_MainPageDrawCoordinate;
		
	memset(pDrawLine->force,0x00,sizeof(float)*DECORD_COORDINATE_FORCE_NUM);	
}

/*------------------------------------------------------------
 * Function Name  : MainPageCoordinateDrawLineJudgeCondition
 * Description    : 坐标系画线判断条件
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCoordinateDrawLineJudgeCondition( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
//	float force = 0;
//	float curveShowStartForce = 0; 
//	
//	curveShowStartForce = GetCurveShowStartValue(SMPL_FH_NUM);
//	force = get_smpl_value(SMPL_FH_NUM);
	
	switch ( pDrawLine->status )
	{
		case STATUS_DRAW_LINE_IDLE:
			if (GetTestStatus() == TEST_LOAD)
			{			
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
 * Description    : 坐标系重新画线判断条件
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
	
	force = get_smpl_value(SMPL_FH_NUM);
	maxForce = smpl_ctrl_full_p_get(SMPL_FH_NUM);
	
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
 * Description    : 画坐标系
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCoordinateDrawLineBodyCycle( void )
{
	COORDINATE_DRAW_LINE_TypeDef *pDrawLine = GetDrawLineAddr();
	float force = 0;

	/* 画线判断条件 */
	MainPageCoordinateDrawLineJudgeCondition(pDrawLine);
	
	if (pDrawLine->start == RESET)
	{
		return;
	}
	
	/* 画线周期 */
	pDrawLine->baseIndex++;
	if (pDrawLine->baseIndex < (uint16_t)(CTRL_FREQ/RECORD_COORDINATE_FREQ))
	{
		return;
	}	
	pDrawLine->baseIndex = 0;
//	GetTime(0);
//	SetTime(0);
	/* 记录力值信息 */
	pDrawLine->nowTimePoint++;
	if (IsCoordinateRecordPointOverflow(pDrawLine) == YES)
	{
		pDrawLine->nowTimePoint = DECORD_COORDINATE_FORCE_NUM - 1;
		
		return;
	}
	
	force = get_smpl_value(SMPL_FH_NUM);
	
	pDrawLine->force[pDrawLine->nowTimePoint] = force;
	
	ECHO(DEBUG_COORDINATE_DRAW_LINE,"time ：%d\r\n",pDrawLine->nowTimePoint);
	ECHO(DEBUG_COORDINATE_DRAW_LINE,"force：%f\r\n",force);
	
	/* 重新画线判断条件 */
	MainPageCoordinateDrawLineRedrawJudgeCondition(pDrawLine);
	
	/* 坐标系画线 */
	CoordinateDrawLineBodyCycle(pDrawLine);
}

/*------------------------------------------------------------
 * Function Name  : MainPageSamplePointCycle
 * Description    : 采样点循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageSamplePointCycle( void )
{
	JudgeBreakCalculateCycle(SMPL_FH_NUM);
}

#if 0
/*------------------------------------------------------------
 * Function Name  : InitKL_TestProgress
 * Description    : 初始化抗拉试验进度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitKL_TestProgress( void )
{
	INIT_LIST_HEAD(&g_klTestProgress.head);
	
	g_klTestProgress.pHead = NULL;
	g_klTestProgress.pSeparator = " -> ";
}

///*------------------------------------------------------------
// * Function Name  : FindKL_TestProgressStatusElement
// * Description    : 查找与状态匹配的元素
// * Input          : None
// * Output         : None
// * Return         : None
// *------------------------------------------------------------*/
//static void *FindKL_TestProgressStatusElement( STATUS_KL_TEST_PROGRESS_TypeDef status )
//{
//	LIST_KL_TEST_PROGRESS_TypeDef *pElement = NULL;
//	
//	/* 遍历所有元素 */
//	list_for_each_entry_reverse(pElement,&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list)
//	{
//		if (pElement->status == status)
//		{
//			return pElement;
//		}
//	}
//	
//	return NULL;
//}

/*------------------------------------------------------------
 * Function Name  : InitKL_TestProgress
 * Description    : 初始化抗拉试验进度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetKL_TestProgress( STATUS_KL_TEST_PROGRESS_TypeDef status )
{
	const uint16_t START_X = 10;
	const uint16_t START_Y = LCD_WIDTH_Y - 27;	
	
	switch ( status )
	{
		case STATUS_KL_PROGRESS_IDLE:
			GUI_ClearShortcutMenu();
			
			g_klTestProgress.progressList[status].x = START_X;
			g_klTestProgress.progressList[status].y = START_Y;
			g_klTestProgress.progressList[status].pointColor = COLOR_POINT;
			g_klTestProgress.progressList[status].backColor = DARKBLUE;
			g_klTestProgress.progressList[status].fontSize = 24;
			g_klTestProgress.progressList[status].name = "";
			g_klTestProgress.progressList[status].status = STATUS_KL_PROGRESS_IDLE;
			g_klTestProgress.progressList[status].enShowSeparator = DISABLE;
			
			list_add(&g_klTestProgress.progressList[status].list,&g_klTestProgress.head);	
			break;
		case STATUS_KL_PROGRESS_FORMAL_LOAD:
			{
				LIST_KL_TEST_PROGRESS_TypeDef * const pElement = \
					list_first_entry(&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list);				
				if (pElement == NULL)
				{
					return;
				}
				
				/* 恢复上一个元素背景色 */
				pElement->pointColor = COLOR_POINT;
				pElement->backColor = COLOR_BACK;	
				
				g_klTestProgress.progressList[status].x = pElement->x + \
						( strlen(pElement->name) * (pElement->fontSize >> 1) );
				g_klTestProgress.progressList[status].y = pElement->y;
				g_klTestProgress.progressList[status].pointColor = COLOR_POINT;
				g_klTestProgress.progressList[status].backColor = DARKBLUE;
				g_klTestProgress.progressList[status].fontSize = 24;
				g_klTestProgress.progressList[status].name = pKLTestProgressName[0];
				g_klTestProgress.progressList[status].status = STATUS_KL_PROGRESS_FORMAL_LOAD;
				g_klTestProgress.progressList[status].enShowSeparator = ENABLE;
				
				list_add(&g_klTestProgress.progressList[status].list,&g_klTestProgress.head);
			}
			break;
		case STATUS_KL_PROGRESS_ELASTIC_DEFORMATION:
			{
				LIST_KL_TEST_PROGRESS_TypeDef * const pElement = \
					list_first_entry(&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list);				
				if (pElement == NULL)
				{
					return;
				}
				
				/* 恢复上一个元素背景色 */
				pElement->pointColor = COLOR_POINT;
				pElement->backColor = COLOR_BACK;	
				
				g_klTestProgress.progressList[status].x = pElement->x + \
						( strlen(pElement->name) * (pElement->fontSize >> 1) ) +\
						( strlen(g_klTestProgress.pSeparator) * (pElement->fontSize >> 1) );
				g_klTestProgress.progressList[status].y = pElement->y;
				g_klTestProgress.progressList[status].pointColor = COLOR_POINT;
				g_klTestProgress.progressList[status].backColor = DARKBLUE;
				g_klTestProgress.progressList[status].fontSize = 24;
				g_klTestProgress.progressList[status].name = pKLTestProgressName[1];
				g_klTestProgress.progressList[status].status = STATUS_KL_PROGRESS_ELASTIC_DEFORMATION;
				g_klTestProgress.progressList[status].enShowSeparator = ENABLE;
				
				list_add(&g_klTestProgress.progressList[status].list,&g_klTestProgress.head);
			}
			break;
		case STATUS_KL_PROGRESS_YIELD:	
			{
				LIST_KL_TEST_PROGRESS_TypeDef * const pElement = \
					list_first_entry(&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list);				
				if (pElement == NULL)
				{
					return;
				}
				
				/* 恢复上一个元素背景色 */
				pElement->pointColor = COLOR_POINT;
				pElement->backColor = COLOR_BACK;	
				
				g_klTestProgress.progressList[status].x = pElement->x + \
						( strlen(pElement->name) * (pElement->fontSize >> 1) ) +\
						( strlen(g_klTestProgress.pSeparator) * (pElement->fontSize >> 1) );
				g_klTestProgress.progressList[status].y = pElement->y;
				g_klTestProgress.progressList[status].pointColor = COLOR_POINT;
				g_klTestProgress.progressList[status].backColor = DARKBLUE;
				g_klTestProgress.progressList[status].fontSize = 24;
				g_klTestProgress.progressList[status].name = pKLTestProgressName[2];
				g_klTestProgress.progressList[status].status = STATUS_KL_PROGRESS_YIELD;
				g_klTestProgress.progressList[status].enShowSeparator = ENABLE;
				
				list_add(&g_klTestProgress.progressList[status].list,&g_klTestProgress.head);
			}
			break;
		case STATUS_KL_PROGRESS_PLASTIC_DEFORMATION:
			{
				LIST_KL_TEST_PROGRESS_TypeDef * const pElement = \
					list_first_entry(&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list);				
				if (pElement == NULL)
				{
					return;
				}
				
				/* 恢复上一个元素背景色 */
				pElement->pointColor = COLOR_POINT;
				pElement->backColor = COLOR_BACK;	
				
				g_klTestProgress.progressList[status].x = pElement->x + \
						( strlen(pElement->name) * (pElement->fontSize >> 1) ) +\
						( strlen(g_klTestProgress.pSeparator) * (pElement->fontSize >> 1) );
				g_klTestProgress.progressList[status].y = pElement->y;
				g_klTestProgress.progressList[status].pointColor = COLOR_POINT;
				g_klTestProgress.progressList[status].backColor = DARKBLUE;
				g_klTestProgress.progressList[status].fontSize = 24;
				g_klTestProgress.progressList[status].name = pKLTestProgressName[3];
				g_klTestProgress.progressList[status].status = STATUS_KL_PROGRESS_PLASTIC_DEFORMATION;
				g_klTestProgress.progressList[status].enShowSeparator = DISABLE;
				
				list_add(&g_klTestProgress.progressList[status].list,&g_klTestProgress.head);
			}
			break;			
		case STATUS_KL_PROGRESS_END:
			GUI_ClearShortcutMenu();
			
			return;

		default:
			break;
	}
	
	/* 遍历所有元素 */
	{
		LIST_KL_TEST_PROGRESS_TypeDef *pElement = NULL;
		
		list_for_each_entry_reverse(pElement,&g_klTestProgress.head,LIST_KL_TEST_PROGRESS_TypeDef,list)
		{			
			GUI_DispStr24At(pElement->x,pElement->y,pElement->pointColor,pElement->backColor,pElement->name);
			
			if (pElement->enShowSeparator == ENABLE)
			{
				uint16_t x = pElement->x + strlen(pElement->name) * (pElement->fontSize >> 1) ;
				
				GUI_DispStr24At(x,pElement->y,COLOR_POINT,COLOR_BACK,g_klTestProgress.pSeparator);
			}
		}
	}
}
#endif

/*------------------------------------------------------------
 * Function Name  : InitKL_TestBody
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitKL_TestBody( KL_TEST_BODY_TypeDef *pKlTest )
{
	pKlTest->maxForce = 0;
	pKlTest->maxStrength = 0;
	pKlTest->upYieldForce = 0;
	pKlTest->downYieldForce = 0;
	pKlTest->upYieldStrength = 0;
	pKlTest->downYieldStrength = 0;
	pKlTest->maxForceSumExtend = 0;
	pKlTest->maxForceSumElongation = 0;
	
	pKlTest->originalDisplacemen = get_smpl_value(SMPL_WY_NUM);
	pKlTest->originalDeform = get_smpl_value(SMPL_BX_NUM);
	
	pKlTest->upYieldForceIndex = 0;
	pKlTest->maxForceIndex = 0;
}

/*------------------------------------------------------------
 * Function Name  : MainPageCtrlCoreCycle
 * Description    : 控制核心循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MainPageCtrlCoreCycle( void )
{
	/* 控制周期50HZ */
	while (!bsp_CheckTimer(MAIN_CYCLE_TIMING));
	
	/* 上位机下发命令(包含采样) */
	PCM_CmdSendCycle();
	
	/* 采样点循环体 */
	MainPageSamplePointCycle();
	
	/* 画坐标系 */
	MainPageCoordinateDrawLineBodyCycle();
	
	/* 设置动态内容 */
	SetDynamicContentTask();
	
	/* 刷新动态内容 */
	RefreshDynamicContent();
	
	/* 试验执行 */
	MainPageTestExecuteCoreCycle();
	
	/* 系统警告检测 */
	MainPageCheckWarn();
	
	/* 脱机检测 */
	CheckOfflineCycle();
	
	/* 联机检测 */
	MainPageCheckLinkPCCycle();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
