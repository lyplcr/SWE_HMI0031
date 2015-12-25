/**
  ******************************************************************************
  * @file    TestReport.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-15 09:49:52
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   试验报告
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TestReport.h"
#include "SearchReport.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_REPORT_NAME_BIT				16			//试验报告名称位数
#define MAX_FIELD_NUM					4			//最大字段个数
#define SHOW_ONE_PAGE_REPORT_NUM		10


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	EXPORT_SUCCESS = 0,
	EXPORT_ERROR,
	EXPORT_CANCEL,
}EXPORT_STATUS_TypeDef;

typedef enum
{
	INDEX_TEST_SERIAL = 0,
	INDEX_TEST_TYPE,
	INDEX_TEST_TIME,
	INDEX_TEST_PRINT,
}REPORT_INDEX_NAME_TypeDef;

typedef enum
{
	OBJECT_TEST_SERIAL = 0,
	OBJECT_TEST_TYPE,
	OBJECT_TEST_TIME,
	OBJECT_TEST_PRINT,
}TEST_REPORT_OBJECT_NAME_TypeDef;

typedef struct
{
	ONE_LEVEL_MENU_TYPE_TypeDef Serial;
	ONE_LEVEL_MENU_TYPE_TypeDef Type;
	ONE_LEVEL_MENU_TYPE_TypeDef Time;
	ONE_LEVEL_MENU_TYPE_TypeDef Print;
	char parameterData[MAX_FIELD_NUM][MAX_REPORT_NAME_BIT+1];
}TEST_REPORT_COLUMN_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	TEST_REPORT_COLUMN_TypeDef oneLevelMenu[SHOW_ONE_PAGE_REPORT_NUM];
	TEST_REPORT_COLUMN_TypeDef oneLevelMenuTitle;
	const char *pParameterNameArray[MAX_FIELD_NUM];
	uint8_t indexArray[MAX_FIELD_NUM];
	BoolStatus isIndexMove;							//索引值移动
	FunctionalState enableMoveIndex;				//使能移动索引值
	const char *pTitle;								//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页	
	const TEST_SEARCH_RESULT_TypeDef *pSearchResult;
	uint32_t curReportNum;							//当前页结果数
	uint32_t curPage;								//当前页
	BoolStatus showDetailReport;					//显示详细报告
}TEST_REPORT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pTestReportField[] = 
{
	"试件编号",
	"试验类型",
	"时    间",
	"打 印",
};

const char * const pTestPrintName[] = 
{
	"按1打印",
	"按2打印",
	"按3打印",
	"按4打印",
	"按5打印",
	"按6打印",
	"按7打印",
	"按8打印",
	"按9打印",
	"按0打印",
};

const char * const pReportSearchName[] = 
{
	"全部试验",
	"水泥胶砂抗压",
	"建筑砂浆抗压",
	"混凝土抗压",
	"混凝土抗折",
	"砌墙砖抗压",
	"逐级定荷",
	"通用抗压",
	"水泥胶砂抗折",
	"压浆水泥浆抗折",
	"通用抗折",		
};

const char * const pReportDeleteContent[] = 
{
	"删除当前报告",
	"删除整页报告",
	"删除所有报告",
};

const char * const pReportExportContent[] = 
{
	"导出当前报告",
	"导出整页报告",
	"导出所有报告",
};

const char * const pReportExportCue[] = 
{
	"导出报告失败！",					//0
	"导出进度",							//1
	"正在导出当前页：",					//2
	"正在导出第      页，共      页",	//3
	"导出报告成功！",					//4
	"取消导出报告！",					//5
};

const char * const pReportDeleteCue[] = 
{
	"删除报告后不可恢复，确认删除？",		//0
	"删除报告失败！",					//1
	"删除进度",							//2
	"正在删除当前页：",					//3
	"正在删除第      页，共      页",	//4
};

const char * const pTestReportWarningCue[] = 
{
	"索引值溢出！",						//0
	"请插入U盘！",						//1
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_TEST_REPORT",zidata = "RAM_TEST_REPORT"
	static TEST_REPORT_TypeDef g_testReport;
#pragma arm section


/* Private function prototypes -----------------------------------------------*/
static void TestReportInit( void );
static void TestReportConfig( void );
static void TestReportReadParameter( void );
static void GUI_TestReport( void );
static void Traverse_TestReport( void );
static void TestReportKeyProcess( void );
static void TestReportMoveIndexProcess( void );
static void TestReportMoveCursorProcess( void );
static void TestReportShortcutCycleTask( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadTestReportPage
 * Description    : 试验报告页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestReportPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	TestReportInit();
	
	/* 参数配置 */
	TestReportConfig();
	
	/* 获取参数 */
	TestReportReadParameter();
	
	/* 画GUI框架 */
	GUI_TestReport();
	
	/* 遍历 */
	Traverse_TestReport();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_testReport.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		TestReportMoveIndexProcess();
		
		/* 移动光标 */
		TestReportMoveCursorProcess();
		
		/* 快捷菜单 */
		TestReportShortcutCycleTask();
		
		/* 按键处理 */
		TestReportKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportInit( void )
{
	g_testReport.recordIndex = 0xff;
	g_testReport.isIndexMove = NO;	
	g_testReport.refreshShortcut = ENABLE;
	g_testReport.leavePage.flagLeavePage = RESET;
	g_testReport.leavePage.flagSaveData = RESET;
	g_testReport.enableMoveIndex = DISABLE;
	g_testReport.showDetailReport = NO;
	
	if ( isAgainSearchReport() == YES)
	{
		ResetAgainSearchReport();
		
		g_testReport.nowIndex = 0;
		g_testReport.curPage = 1;
	}
	
	g_testReport.curReportNum = 0;	
}

/*------------------------------------------------------------
 * Function Name  : TestReportConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportConfig( void )
{
	/* 索引值 */
	g_testReport.indexArray[INDEX_TEST_SERIAL] 	= OBJECT_TEST_SERIAL;
	g_testReport.indexArray[INDEX_TEST_TYPE] 	= OBJECT_TEST_TYPE;
	g_testReport.indexArray[INDEX_TEST_TIME] 	= OBJECT_TEST_TIME;
	g_testReport.indexArray[INDEX_TEST_PRINT] 	= OBJECT_TEST_PRINT;
	
	/* 字段名 */
	g_testReport.pParameterNameArray[INDEX_TEST_SERIAL] = pTestReportField[0];
	g_testReport.pParameterNameArray[INDEX_TEST_TYPE] 	= pTestReportField[1];
	g_testReport.pParameterNameArray[INDEX_TEST_TIME] 	= pTestReportField[2];
	g_testReport.pParameterNameArray[INDEX_TEST_PRINT] 	= pTestReportField[3];
	
	/* 标题 */
	g_testReport.pTitle = "试验报告";
	
	/* 报告搜索结果 */
	g_testReport.pSearchResult = GetSearchReportResultAddr();	
}

/*------------------------------------------------------------
 * Function Name  : GetTestReportIndex
 * Description    : 获取真实的索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetTestReportIndex( uint8_t index )
{
	uint8_t i;
	uint8_t testIndex = 0xff;	//错误的值
	
	for (i=0; i<MAX_FIELD_NUM; ++i)
	{
		if (g_testReport.indexArray[i] == index)
		{
			testIndex = i;
			break;
		}
	}
	
	return testIndex;
}

/*------------------------------------------------------------
 * Function Name  : TestReportReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportReadParameter( void )
{
	uint32_t numOffset = 0;
	uint8_t i,index = 0;
	uint8_t testType = 0;
	
	/* 从SD重新获取数据 */
	if ( g_testReport.curPage )
	{
		numOffset = (g_testReport.curPage - 1) * SHOW_ONE_PAGE_REPORT_NUM;
		
		GetTestDataFromSD(DISABLE, GetSearchType(), numOffset);
		
		//只读取上一次搜索到的报告，如果不进入搜索界面 “报告总数”保持不变
		g_testReport.curReportNum = g_testReport.pSearchResult->sumNum - (g_testReport.curPage - 1) * SHOW_ONE_PAGE_REPORT_NUM;
		if (g_testReport.curReportNum > SHOW_ONE_PAGE_REPORT_NUM)
		{
			g_testReport.curReportNum = SHOW_ONE_PAGE_REPORT_NUM;
		}	
	}
	
	if ( g_testReport.curPage )
	{
		for (i=0; i<g_testReport.curReportNum; ++i)
		{
			index = GetTestReportIndex(OBJECT_TEST_SERIAL);
			if (index != 0xff)
			{
				strcpy(g_testReport.oneLevelMenu[i].parameterData[index],g_testReport.pSearchResult->testReportData[i].fname);
			}
			
			index = GetTestReportIndex(OBJECT_TEST_TYPE);
			if (index != 0xff)
			{
				testType = g_testReport.pSearchResult->recordTestType[i];
				strcpy(g_testReport.oneLevelMenu[i].parameterData[index],pReportSearchName[testType]);
			}
			
			index = GetTestReportIndex(OBJECT_TEST_TIME);
			if (index != 0xff)
			{
				tTimeConvTimeFormate(DISABLE,g_testReport.pSearchResult->testReportData[i].time,g_testReport.oneLevelMenu[i].parameterData[index]);
			}
			
			index = GetTestReportIndex(OBJECT_TEST_PRINT);
			if (index != 0xff)
			{
				strcpy(g_testReport.oneLevelMenu[i].parameterData[index],pTestPrintName[i]);		
			}
		}
	}
	
	if (g_testReport.nowIndex >= g_testReport.curReportNum)
	{
		g_testReport.nowIndex = 0;
	}
	
	if ( g_testReport.curReportNum )
	{
		g_testReport.enableMoveIndex = ENABLE;
	}	
}

/*------------------------------------------------------------
 * Function Name  : ConfigTestReportOnwRowRectangleFrameCoordinate
 * Description    : 配置界面一行GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestReportOnwRowRectangleFrameCoordinate( uint8_t index, uint16_t x, uint16_t y )
{	
	g_testReport.oneLevelMenu[index].Serial.x = x;
	g_testReport.oneLevelMenu[index].Serial.y = y;
	g_testReport.oneLevelMenu[index].Serial.pointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Serial.backColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Serial.recordPointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Serial.recordBackColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Serial.lenth = 196;
	g_testReport.oneLevelMenu[index].Serial.width = 28;
	g_testReport.oneLevelMenu[index].Serial.fontSize = 24;
	g_testReport.oneLevelMenu[index].Serial.rowDistance = 0;
	g_testReport.oneLevelMenu[index].Serial.columnDistance = 0;
	g_testReport.oneLevelMenu[index].Serial.lineWidth = 2;
	
	x += g_testReport.oneLevelMenu[index].Serial.lenth - g_testReport.oneLevelMenu[index].Serial.lineWidth;
	g_testReport.oneLevelMenu[index].Type.x = x;
	g_testReport.oneLevelMenu[index].Type.y = y;
	g_testReport.oneLevelMenu[index].Type.pointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Type.backColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Type.recordPointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Type.recordBackColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Type.lenth = 196;
	g_testReport.oneLevelMenu[index].Type.width = 28;
	g_testReport.oneLevelMenu[index].Type.fontSize = 24;
	g_testReport.oneLevelMenu[index].Type.rowDistance = 0;
	g_testReport.oneLevelMenu[index].Type.columnDistance = 0;
	g_testReport.oneLevelMenu[index].Type.lineWidth = 2;
	
	x += g_testReport.oneLevelMenu[index].Type.lenth - g_testReport.oneLevelMenu[index].Type.lineWidth;
	g_testReport.oneLevelMenu[index].Time.x = x;
	g_testReport.oneLevelMenu[index].Time.y = y;
	g_testReport.oneLevelMenu[index].Time.pointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Time.backColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Time.recordPointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Time.recordBackColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Time.lenth = 196;
	g_testReport.oneLevelMenu[index].Time.width = 28;
	g_testReport.oneLevelMenu[index].Time.fontSize = 24;
	g_testReport.oneLevelMenu[index].Time.rowDistance = 0;
	g_testReport.oneLevelMenu[index].Time.columnDistance = 0;
	g_testReport.oneLevelMenu[index].Time.lineWidth = 2;
	
	x += g_testReport.oneLevelMenu[index].Time.lenth - g_testReport.oneLevelMenu[index].Time.lineWidth;
	g_testReport.oneLevelMenu[index].Print.x = x;
	g_testReport.oneLevelMenu[index].Print.y = y;
	g_testReport.oneLevelMenu[index].Print.pointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Print.backColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Print.recordPointColor = COLOR_POINT;
	g_testReport.oneLevelMenu[index].Print.recordBackColor = COLOR_BACK;
	g_testReport.oneLevelMenu[index].Print.lenth = 100;
	g_testReport.oneLevelMenu[index].Print.width = 28;
	g_testReport.oneLevelMenu[index].Print.fontSize = 24;
	g_testReport.oneLevelMenu[index].Print.rowDistance = 0;
	g_testReport.oneLevelMenu[index].Print.columnDistance = 0;
	g_testReport.oneLevelMenu[index].Print.lineWidth = 2;	
}	


/*------------------------------------------------------------
 * Function Name  : ConfigTestReportRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestReportRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 59;
	const uint16_t START_Y = 74;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	ConfigTestReportOnwRowRectangleFrameCoordinate(0,startX,startY);
	
	g_testReport.oneLevelMenuTitle = g_testReport.oneLevelMenu[0];
		
	startY += g_testReport.oneLevelMenu[0].Serial.width - g_testReport.oneLevelMenu[0].Serial.lineWidth;
	
	for (i=0; i<SHOW_ONE_PAGE_REPORT_NUM; ++i)
	{		
		ConfigTestReportOnwRowRectangleFrameCoordinate(i,startX,startY);	
		
		startY += g_testReport.oneLevelMenu[i].Serial.width - g_testReport.oneLevelMenu[i].Serial.lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestReportDrawOneRowTitleRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestReportDrawOneRowTitleRectangleFrame( void )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_testReport.oneLevelMenuTitle.Serial.x;
	rectangle.y = g_testReport.oneLevelMenuTitle.Serial.y;
	rectangle.lenth = g_testReport.oneLevelMenuTitle.Serial.lenth;
	rectangle.width = g_testReport.oneLevelMenuTitle.Serial.width;
	rectangle.lineWidth = g_testReport.oneLevelMenuTitle.Serial.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenuTitle.Serial.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenuTitle.Type.x;
	rectangle.y = g_testReport.oneLevelMenuTitle.Type.y;
	rectangle.lenth = g_testReport.oneLevelMenuTitle.Type.lenth;
	rectangle.width = g_testReport.oneLevelMenuTitle.Type.width;
	rectangle.lineWidth = g_testReport.oneLevelMenuTitle.Type.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenuTitle.Type.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenuTitle.Time.x;
	rectangle.y = g_testReport.oneLevelMenuTitle.Time.y;
	rectangle.lenth = g_testReport.oneLevelMenuTitle.Time.lenth;
	rectangle.width = g_testReport.oneLevelMenuTitle.Time.width;
	rectangle.lineWidth = g_testReport.oneLevelMenuTitle.Time.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenuTitle.Time.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenuTitle.Print.x;
	rectangle.y = g_testReport.oneLevelMenuTitle.Print.y;
	rectangle.lenth = g_testReport.oneLevelMenuTitle.Print.lenth;
	rectangle.width = g_testReport.oneLevelMenuTitle.Print.width;
	rectangle.lineWidth = g_testReport.oneLevelMenuTitle.Print.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenuTitle.Print.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestReportDrawOneRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestReportDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_testReport.oneLevelMenu[index].Serial.x;
	rectangle.y = g_testReport.oneLevelMenu[index].Serial.y;
	rectangle.lenth = g_testReport.oneLevelMenu[index].Serial.lenth;
	rectangle.width = g_testReport.oneLevelMenu[index].Serial.width;
	rectangle.lineWidth = g_testReport.oneLevelMenu[index].Serial.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenu[index].Serial.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenu[index].Type.x;
	rectangle.y = g_testReport.oneLevelMenu[index].Type.y;
	rectangle.lenth = g_testReport.oneLevelMenu[index].Type.lenth;
	rectangle.width = g_testReport.oneLevelMenu[index].Type.width;
	rectangle.lineWidth = g_testReport.oneLevelMenu[index].Type.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenu[index].Type.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenu[index].Time.x;
	rectangle.y = g_testReport.oneLevelMenu[index].Time.y;
	rectangle.lenth = g_testReport.oneLevelMenu[index].Time.lenth;
	rectangle.width = g_testReport.oneLevelMenu[index].Time.width;
	rectangle.lineWidth = g_testReport.oneLevelMenu[index].Time.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenu[index].Time.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	rectangle.x = g_testReport.oneLevelMenu[index].Print.x;
	rectangle.y = g_testReport.oneLevelMenu[index].Print.y;
	rectangle.lenth = g_testReport.oneLevelMenu[index].Print.lenth;
	rectangle.width = g_testReport.oneLevelMenu[index].Print.width;
	rectangle.lineWidth = g_testReport.oneLevelMenu[index].Print.lineWidth;
	rectangle.lineColor = g_testReport.oneLevelMenu[index].Print.pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestReportRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestReportRectangleFrame( void )
{
	uint8_t i;
	
	GUI_TestReportDrawOneRowTitleRectangleFrame();
	
	for (i=0; i<SHOW_ONE_PAGE_REPORT_NUM; ++i)
	{
		GUI_TestReportDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestReportField
 * Description    : 界面字段名称
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestReportField( void )
{
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t pointColor = 0;
	uint16_t backColor = 0;
	uint8_t index = 0;
	
	x = g_testReport.oneLevelMenuTitle.Serial.x + g_testReport.oneLevelMenuTitle.Serial.lineWidth + 48;
	y = g_testReport.oneLevelMenuTitle.Serial.y + g_testReport.oneLevelMenuTitle.Serial.lineWidth;
	pointColor = g_testReport.oneLevelMenuTitle.Serial.pointColor;
	backColor = g_testReport.oneLevelMenuTitle.Serial.backColor;
	
	index = GetTestReportIndex(OBJECT_TEST_SERIAL);
	if (index != 0xff)
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.pParameterNameArray[index]);
	}
	
	x = g_testReport.oneLevelMenuTitle.Type.x + g_testReport.oneLevelMenuTitle.Type.lineWidth + 48;
	y = g_testReport.oneLevelMenuTitle.Type.y + g_testReport.oneLevelMenuTitle.Type.lineWidth;
	pointColor = g_testReport.oneLevelMenuTitle.Type.pointColor;
	backColor = g_testReport.oneLevelMenuTitle.Type.backColor;
	
	index = GetTestReportIndex(OBJECT_TEST_TYPE);
	if (index != 0xff)
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.pParameterNameArray[index]);
	}
	
	x = g_testReport.oneLevelMenuTitle.Time.x + g_testReport.oneLevelMenuTitle.Time.lineWidth + 48;
	y = g_testReport.oneLevelMenuTitle.Time.y + g_testReport.oneLevelMenuTitle.Time.lineWidth;
	pointColor = g_testReport.oneLevelMenuTitle.Time.pointColor;
	backColor = g_testReport.oneLevelMenuTitle.Time.backColor;
	
	index = GetTestReportIndex(OBJECT_TEST_TIME);
	if (index != 0xff)
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.pParameterNameArray[index]);
	}
	
	x = g_testReport.oneLevelMenuTitle.Print.x + g_testReport.oneLevelMenuTitle.Print.lineWidth + 18;
	y = g_testReport.oneLevelMenuTitle.Print.y + g_testReport.oneLevelMenuTitle.Print.lineWidth;
	pointColor = g_testReport.oneLevelMenuTitle.Print.pointColor;
	backColor = g_testReport.oneLevelMenuTitle.Print.backColor;
	
	index = GetTestReportIndex(OBJECT_TEST_PRINT);
	if (index != 0xff)
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.pParameterNameArray[index]);
	}
}	

/*------------------------------------------------------------
 * Function Name  : TestReportShowCurPageInfomation
 * Description    : 试验报告显示当前页信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportShowCurPageInfomation( void )
{
	char buff[10];
	uint32_t pageNum = 0;
	const uint16_t back_color = COLOR_BACK;
	const uint16_t point_color = MIDDLEBLUE;
	
	if ( g_testReport.curReportNum )
	{
		GUI_DispStr24At(PAGE_DI_POS_X,PAGE_DI_POS_Y,COLOR_POINT,back_color,"第      页;");		
		lcd_fill(PAGE_DI_POS_X+36,PAGE_DI_POS_Y,48,24,back_color);
		
		pageNum = g_testReport.curPage;
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
		
		GUI_DispStr24At(PAGE_GONG_POS_X,PAGE_GONG_POS_Y,COLOR_POINT,back_color,"共      页");
		lcd_fill(PAGE_GONG_POS_X+36,PAGE_GONG_POS_Y,48,24,back_color);
		
		pageNum = g_testReport.pSearchResult->sumPage;
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
		
		if (g_testReport.pSearchResult->sumPage > 1)
		{
			lcd_show_image(PAGE_GONG_POS_X+300,PAGE_GONG_POS_Y-3,PIC_DIR_LEFT_RIGHT);
		}
	}
	else
	{
		GUI_DispStr24At(PAGE_DI_POS_X,PAGE_DI_POS_Y,COLOR_POINT,back_color,"未找到符合条件的报告！");	
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestReport
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestReport( void )
{	
	GUI_DrawGeneralTitleRectangleFrame(g_testReport.pTitle);
	
	ConfigTestReportRectangleFrameCoordinate();
	
	GUI_TestReportRectangleFrame();
	
	GUI_TestReportField();
	
	TestReportShowCurPageInfomation();
}	

/*------------------------------------------------------------
 * Function Name  : Show_TestReportOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestReportOneRowOneLevelMenuContent( uint8_t index )
{
	uint16_t x = 0,y = 0,pointColor = 0,backColor = 0,lenth = 0,width = 0;
	uint8_t indexBuff = 0;
	
	x = g_testReport.oneLevelMenu[index].Serial.x + g_testReport.oneLevelMenu[index].Serial.lineWidth;
	y = g_testReport.oneLevelMenu[index].Serial.y + g_testReport.oneLevelMenu[index].Serial.lineWidth;
	lenth = g_testReport.oneLevelMenu[index].Serial.lenth - 2 * g_testReport.oneLevelMenu[index].Serial.lineWidth;
	width = g_testReport.oneLevelMenu[index].Serial.width - 2 * g_testReport.oneLevelMenu[index].Serial.lineWidth;
	pointColor = g_testReport.oneLevelMenu[index].Serial.pointColor;
	backColor = g_testReport.oneLevelMenu[index].Serial.backColor;
	
	indexBuff = GetTestReportIndex(OBJECT_TEST_SERIAL);
	if (index != 0xff)	
	{
		lcd_fill(x,y,lenth,width,backColor);
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.oneLevelMenu[index].parameterData[indexBuff]);
	}
	
	x = g_testReport.oneLevelMenu[index].Type.x + g_testReport.oneLevelMenu[index].Type.lineWidth;
	y = g_testReport.oneLevelMenu[index].Type.y + g_testReport.oneLevelMenu[index].Type.lineWidth;
	lenth = g_testReport.oneLevelMenu[index].Type.lenth - 2 * g_testReport.oneLevelMenu[index].Type.lineWidth;
	width = g_testReport.oneLevelMenu[index].Type.width - 2 * g_testReport.oneLevelMenu[index].Type.lineWidth;
	pointColor = g_testReport.oneLevelMenu[index].Type.pointColor;
	backColor = g_testReport.oneLevelMenu[index].Type.backColor;

	indexBuff = GetTestReportIndex(OBJECT_TEST_TYPE);
	if (index != 0xff)	
	{
		lcd_fill(x,y,lenth,width,backColor);
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.oneLevelMenu[index].parameterData[indexBuff]);
	}
	
	x = g_testReport.oneLevelMenu[index].Time.x + g_testReport.oneLevelMenu[index].Time.lineWidth;
	y = g_testReport.oneLevelMenu[index].Time.y + g_testReport.oneLevelMenu[index].Time.lineWidth;
	lenth = g_testReport.oneLevelMenu[index].Time.lenth - 2 * g_testReport.oneLevelMenu[index].Time.lineWidth;
	width = g_testReport.oneLevelMenu[index].Time.width - 2 * g_testReport.oneLevelMenu[index].Time.lineWidth;
	pointColor = g_testReport.oneLevelMenu[index].Time.pointColor;
	backColor = g_testReport.oneLevelMenu[index].Time.backColor;
	
	indexBuff = GetTestReportIndex(OBJECT_TEST_TIME);
	if (index != 0xff)	
	{
		lcd_fill(x,y,lenth,width,backColor);
		GUI_DispStr24At(x,y,pointColor,backColor,g_testReport.oneLevelMenu[index].parameterData[indexBuff]);
	}
	
	x = g_testReport.oneLevelMenu[index].Print.x + g_testReport.oneLevelMenu[index].Print.lineWidth;
	y = g_testReport.oneLevelMenu[index].Print.y + g_testReport.oneLevelMenu[index].Print.lineWidth;
	lenth = g_testReport.oneLevelMenu[index].Print.lenth - 2 * g_testReport.oneLevelMenu[index].Print.lineWidth;
	width = g_testReport.oneLevelMenu[index].Print.width - 2 * g_testReport.oneLevelMenu[index].Print.lineWidth;
	pointColor = g_testReport.oneLevelMenu[index].Print.pointColor;
	backColor = g_testReport.oneLevelMenu[index].Print.backColor;
	
	indexBuff = GetTestReportIndex(OBJECT_TEST_PRINT);
	if (index != 0xff)	
	{
		lcd_fill(x,y,lenth,width,backColor);
		GUI_DispStr24At(x+6,y,pointColor,backColor,g_testReport.oneLevelMenu[index].parameterData[indexBuff]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_TestReportOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestReportOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_testReport.curReportNum; ++i)
	{
		Show_TestReportOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TestReport
 * Description    : 遍历试验报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TestReport( void )
{
	Show_TestReportOneLevelMenuContent();
}	

/*------------------------------------------------------------
 * Function Name  : TestReportMoveIndexProcess
 * Description    : 移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_testReport.isIndexMove = NO;
	
	if ( !g_testReport.curReportNum )
	{
		return;
	}
	
	indexObj.enableMoveIndex = g_testReport.enableMoveIndex;
	indexObj.rowNum = g_testReport.curReportNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_testReport.curReportNum;
	indexObj.pNowIndex = &g_testReport.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_testReport.nowIndex != g_testReport.recordIndex)
	{		
		g_testReport.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportMoveCursorProcess( void )
{
	if (g_testReport.isIndexMove == YES)
	{		
		if (g_testReport.recordIndex != 0xff)
		{
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Serial.pointColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Serial.recordPointColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Serial.backColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Serial.recordBackColor;	
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Type.pointColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Type.recordPointColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Type.backColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Type.recordBackColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Time.pointColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Time.recordPointColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Time.backColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Time.recordBackColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Print.pointColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Print.recordPointColor;
			g_testReport.oneLevelMenu[g_testReport.recordIndex].Print.backColor = g_testReport.oneLevelMenu[g_testReport.recordIndex].Print.recordBackColor;
			Show_TestReportOneRowOneLevelMenuContent(g_testReport.recordIndex);
			
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Serial.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Serial.backColor = COLOR_SELECT_BACK;	
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Type.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Type.backColor = COLOR_SELECT_BACK;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Time.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Time.backColor = COLOR_SELECT_BACK;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Print.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Print.backColor = COLOR_SELECT_BACK;
			Show_TestReportOneRowOneLevelMenuContent(g_testReport.nowIndex);		
		}
		else
		{
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Serial.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Serial.backColor = COLOR_SELECT_BACK;	
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Type.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Type.backColor = COLOR_SELECT_BACK;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Time.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Time.backColor = COLOR_SELECT_BACK;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Print.pointColor = COLOR_SELECT_POINT;
			g_testReport.oneLevelMenu[g_testReport.nowIndex].Print.backColor = COLOR_SELECT_BACK;
			Show_TestReportOneRowOneLevelMenuContent(g_testReport.nowIndex);
		}
		
		g_testReport.recordIndex = g_testReport.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_testReport.refreshShortcut == ENABLE)
	{
		g_testReport.refreshShortcut = DISABLE;
		
		if (g_testReport.curReportNum)
		{
			pShortCut->status = SHOW_F1 | SHOW_F2 | SHOW_F3 | SHOW_F4;
		
			pShortCut->pContent[0] = pTwoLevelMenu[15];
			pShortCut->pContent[1] = pTwoLevelMenu[78];
			pShortCut->pContent[2] = pTwoLevelMenu[34];
			pShortCut->pContent[3] = pTwoLevelMenu[63];
		}
		else
		{
			pShortCut->status = SHOW_F4;
		
			pShortCut->pContent[3] = pTwoLevelMenu[63];
		}
		
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : LoadTestReportDeleteSelectPage
 * Description    : 试验报告删除选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t LoadTestReportDeleteSelectPage( void )
{
	SELECT_WINDOWS_TypeDef *pSelect = GetSelectWindowsAddr();
	POP_WINDOWS_TypeDef *pPopWindows = GetPopWindowsAddr();
	POSITION_TypeDef pos; 
	
	pSelect->pTitleContent = "报告删除";
	pSelect->selectNum = 3;
	pSelect->pSelectContent = pReportDeleteContent;
	
	LoadSelectWindows(pSelect);
	
	if (pSelect->selectResult != 0xff)
	{
		SetSelectPopWindowsInfomation(POP_PCM_ASKING,1,pReportDeleteCue,pSelectMenuCue);
		
		/* 弹窗向下角偏移24 */
		pos.x = pPopWindows->x;
		pos.y = pPopWindows->y;
		pPopWindows->x += 24;
		pPopWindows->y += 24;
		
		PopWindowsProcessCycle();
		
		pPopWindows->x = pos.x;
		pPopWindows->y = pos.y;
		
		if ( GetPopWindowsSelectResult() == YES)
		{
			return pSelect->selectResult;
		}
	}
	
	return 0xff;
}

/*------------------------------------------------------------
 * Function Name  : DeleteAllReportShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DeleteAllReportShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	pShortCut->status = SHOW_F4;	
	pShortCut->pContent[3] = pTwoLevelMenu[58];
	
	pShortCut->pointColor = COLOR_SHORTCUT_POINT;
	pShortCut->backColor = COLOR_SHORTCUT_BACK;
		
	ShortcutMenuTask(pShortCut);
}

/*------------------------------------------------------------
 * Function Name  : ExportCurrentReport
 * Description    : 导出当前报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static EXPORT_STATUS_TypeDef ExportCurrentReport( uint8_t reportIndex )
{
	uint8_t testType = 0;
	REPORT_TypeDef readReport;
	FRESULT result;
	
	if (reportIndex >= SHOW_ONE_PAGE_REPORT_NUM)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pTestReportWarningCue[0]);
		
		return EXPORT_ERROR;
	}
	
	testType = g_testReport.pSearchResult->recordTestType[reportIndex];
	
	result = report_read(testType,g_testReport.pSearchResult->testReportData[reportIndex].fname,&readReport);
	if (result != FR_OK)
	{
		return EXPORT_ERROR;
	}
	
	result = report_save_usb(testType,g_testReport.pSearchResult->testReportData[reportIndex].fname,&readReport);
	if (result != FR_OK)
	{
		return EXPORT_ERROR;
	}
	
	result = report_save_usb_set_time(testType,g_testReport.pSearchResult->testReportData[reportIndex].fname);	
	if (result != FR_OK)
	{
		return EXPORT_ERROR;
	}
	
	return EXPORT_SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : ExportPageReport
 * Description    : 导出整页报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static EXPORT_STATUS_TypeDef ExportPageReport( uint32_t exportPage, uint32_t sumPage )
{
	uint8_t index = 0;
	uint8_t indexBuff = GetTestReportIndex(OBJECT_TEST_SERIAL);
	const uint16_t x = POS_WINDOWS_START_X;
	const uint16_t y = POS_WINDOWS_START_Y;
	POP_WINDOWS_TypeDef *popWindows = GetPopWindowsAddr();
	POSITION_TypeDef numPos,sumPos;
	char showBuff[10];
	POSITION_TypeDef pos;
	POP_WINDOWS_TypeDef *pPopWindows = GetPopWindowsAddr();
	PROGRESS_BAR_TypeDef progressBar;
	
	if (exportPage >= 10000)
	{
		return EXPORT_CANCEL;
	}
	
	SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,pReportExportCue[1],1,&pReportExportCue[3]);
		
	/* 弹窗向下角偏移48 */
	pos.x = pPopWindows->x;
	pos.y = pPopWindows->y;
	pPopWindows->x += 48;
	pPopWindows->y += 48;
	
	PopWindowsProcessCycle();
	
	pPopWindows->x = pos.x;
	pPopWindows->y = pos.y;
	
	numPos.x = popWindows->x + popWindows->lineWidth + 3 + 11 * (popWindows->fontSize>>1) + 48;
	numPos.y = popWindows->y + popWindows->titleWidth + 3 + 48;
	
	sumPos.x = numPos.x + 6 * popWindows->fontSize;
	sumPos.y = numPos.y;
	
	usprintf(showBuff,"%4d",exportPage);
	GUI_DispStr24At(numPos.x,numPos.y,MIDDLEBLUE,popWindows->backColor,showBuff);
	
	usprintf(showBuff,"%4d",sumPage);
	GUI_DispStr24At(sumPos.x,sumPos.y,MIDDLEBLUE,popWindows->backColor,showBuff);
	
	progressBar.x = x + 135 + 48;
	progressBar.y = y + 83 + 48;
	progressBar.lenth = 104;
	progressBar.width = 24;
	progressBar.upperPointColor = PROCESS_UP_COLOR;
	progressBar.lowerPointColor = PROCESS_DN_COLOR;
	progressBar.lineColor = COLOR_POINT;
	progressBar.backColor = COLOR_BACK;
	progressBar.lineWidth = 2;
	progressBar.curProgress = 0;
	progressBar.totalProgress = g_testReport.curReportNum;
	progressBar.pTitle = "";
	progressBar.fontSize = 16;
	progressBar.fontColor = COLOR_POINT;
	
	GUI_ProgressBar(&progressBar);
	
	for (index=0; index<g_testReport.curReportNum; ++index)
	{
		if (ExportCurrentReport(index) == EXPORT_SUCCESS)
		{
			progressBar.curProgress = index + 1;
			DrawProcessBody(&progressBar);
		}
		else
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportExportCue[0]);
		
			PopWindowsProcessCycle();
	
			return EXPORT_ERROR;
		}
	}
			
	return EXPORT_SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : ExportAllReport
 * Description    : 导出所有报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static EXPORT_STATUS_TypeDef ExportAllReport( void )
{		
	uint32_t sumPage = g_testReport.pSearchResult->sumPage;
	EXPORT_STATUS_TypeDef exportStatus;
	
	g_testReport.curPage = 1;
	
	while (1)
	{	
		DeleteAllReportShortcutCycleTask();
		
		if (GetKey() == KEY_F4)
		{
			return EXPORT_CANCEL;
		}
				
		TestReportReadParameter();
		
		exportStatus = ExportPageReport(g_testReport.curPage,sumPage);
		
		if (exportStatus == EXPORT_SUCCESS)
		{
			g_testReport.curPage++;
			if (g_testReport.curPage > sumPage)
			{
				return exportStatus;
			}
		}
		else
		{
			return exportStatus;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : DeleteCurrentReport
 * Description    : 删除当前报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus DeleteCurrentReport( void )
{
	uint8_t index = g_testReport.nowIndex;
	uint8_t indexBuff = GetTestReportIndex(OBJECT_TEST_SERIAL);
	
	if (FR_OK == report_delete( g_testReport.pSearchResult->recordTestType[index],g_testReport.oneLevelMenu[index].parameterData[indexBuff] ) )
	{	
		return SUCCESS;
	}
	
	return ERROR;
}

/*------------------------------------------------------------
 * Function Name  : DeletePageReport
 * Description    : 删除整页报告
 * Input          : deletePage：删除页码，0：表示当前页
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus DeletePageReport( uint32_t deletePage, uint32_t sumPage )
{
	uint8_t index = 0;
	uint8_t indexBuff = GetTestReportIndex(OBJECT_TEST_SERIAL);
	const uint16_t x = POS_WINDOWS_START_X;
	const uint16_t y = POS_WINDOWS_START_Y;
	POP_WINDOWS_TypeDef *popWindows = GetPopWindowsAddr();
	POSITION_TypeDef numPos,sumPos;
	char showBuff[10];
	POSITION_TypeDef pos;
	POP_WINDOWS_TypeDef *pPopWindows = GetPopWindowsAddr();
	PROGRESS_BAR_TypeDef progressBar;
	
	if (deletePage >= 10000)
	{
		return ERROR;
	}
	
	if (deletePage == 0)
	{
		SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,pReportDeleteCue[2],1,&pReportDeleteCue[3]);
	}
	else
	{
		SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,pReportDeleteCue[2],1,&pReportDeleteCue[4]);
	}
		
	/* 弹窗向下角偏移48 */
	pos.x = pPopWindows->x;
	pos.y = pPopWindows->y;
	pPopWindows->x += 48;
	pPopWindows->y += 48;
	
	PopWindowsProcessCycle();
	
	pPopWindows->x = pos.x;
	pPopWindows->y = pos.y;
	
	if (deletePage)
	{
		numPos.x = popWindows->x + popWindows->lineWidth + 3 + 11 * (popWindows->fontSize>>1) + 48;
		numPos.y = popWindows->y + popWindows->titleWidth + 3 + 48;
		
		sumPos.x = numPos.x + 6 * popWindows->fontSize;
		sumPos.y = numPos.y;
		
		usprintf(showBuff,"%4d",deletePage);
		GUI_DispStr24At(numPos.x,numPos.y,MIDDLEBLUE,popWindows->backColor,showBuff);
		
		usprintf(showBuff,"%4d",sumPage);
		GUI_DispStr24At(sumPos.x,sumPos.y,MIDDLEBLUE,popWindows->backColor,showBuff);
	}
	
	progressBar.x = x + 135 + 48;
	progressBar.y = y + 83 + 48;
	progressBar.lenth = 104;
	progressBar.width = 24;
	progressBar.upperPointColor = PROCESS_UP_COLOR;
	progressBar.lowerPointColor = PROCESS_DN_COLOR;
	progressBar.lineColor = COLOR_POINT;
	progressBar.backColor = COLOR_BACK;
	progressBar.lineWidth = 2;
	progressBar.curProgress = 0;
	progressBar.totalProgress = g_testReport.curReportNum;
	progressBar.pTitle = "";
	progressBar.fontSize = 16;
	progressBar.fontColor = COLOR_POINT;
	
	GUI_ProgressBar(&progressBar);
	
	for (index=0; index<g_testReport.curReportNum; ++index)
	{
		if (FR_OK == report_delete( g_testReport.pSearchResult->recordTestType[index],g_testReport.oneLevelMenu[index].parameterData[indexBuff] ) )
		{
			progressBar.curProgress = index + 1;
			DrawProcessBody(&progressBar);
		}
		else
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportDeleteCue[1]);
		
			PopWindowsProcessCycle();
	
			return ERROR;
		}
	}
			
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : DeleteAllReport
 * Description    : 删除所有报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus DeleteAllReport( void )
{		
	uint32_t curPage = 1;
	uint32_t sumPage = g_testReport.pSearchResult->sumPage;
	
	g_testReport.curPage = 1;	//始终删除第一页
	
	while (1)
	{		
		DeleteAllReportShortcutCycleTask();
		
		if (GetKey() == KEY_F4)
		{
			return SUCCESS;
		}
		
		StartOneReportSearch();
		
		TestReportReadParameter();
		
		if ((g_testReport.curReportNum!=0) && (curPage<=sumPage))
		{
			if (ERROR == DeletePageReport(curPage,sumPage) )
			{
				return ERROR;
			}
			curPage++;
		}
		else
		{				
			return SUCCESS;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportDelete
 * Description    : 试验报告删除
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus TestReportDelete( void )
{
	ErrorStatus ErrSta = ERROR;
	uint8_t selectResult = 0;
	
	selectResult = LoadTestReportDeleteSelectPage();
	
	if (selectResult == 0xff)
	{	
		return NO;
	}

	switch ( selectResult )
	{
		case 0:
			ErrSta = DeleteCurrentReport();
			break;
		case 1:
			ErrSta = DeletePageReport(0,g_testReport.pSearchResult->sumPage);
			break;
		case 2:
			ErrSta = DeleteAllReport();
			break;
		default:
			break;
	}
	
	if (ErrSta == ERROR)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportDeleteCue[1]);
		
		PopWindowsProcessCycle();
		
		return NO;
	}
	
	return YES;
}

/*------------------------------------------------------------
 * Function Name  : TestReportDeleteReport
 * Description    : 删除报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void TestReportDeleteReport( void )
{
	if ( g_testReport.curReportNum )
	{			
		TestReportDelete();
		
		StartOneReportSearch();
		
		TestReportReadParameter();
		
		if ( g_testReport.pSearchResult->sumPage )
		{
			if (g_testReport.curPage > g_testReport.pSearchResult->sumPage)
			{
				g_testReport.curPage = 1;
			}
		}
		
		g_testReport.leavePage.flagLeavePage = SET;
		g_testReport.leavePage.flagSaveData = RESET;
	}
}

/*------------------------------------------------------------
 * Function Name  : LoadTestReportExportSelectPage
 * Description    : 试验报告导出选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t LoadTestReportExportSelectPage( void )
{
	SELECT_WINDOWS_TypeDef *pSelect = GetSelectWindowsAddr();
	POP_WINDOWS_TypeDef *pPopWindows = GetPopWindowsAddr();
	
	pSelect->pTitleContent = "报告导出";
	pSelect->selectNum = 3;
	pSelect->pSelectContent = pReportExportContent;
	
	LoadSelectWindows(pSelect);
	
	return pSelect->selectResult;
}

/*------------------------------------------------------------
 * Function Name  : TestReportExport
 * Description    : 试验报告导出
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static EXPORT_STATUS_TypeDef TestReportExport( void )
{
	EXPORT_STATUS_TypeDef ExportStatus;
	uint8_t selectResult = 0;
	
	selectResult = LoadTestReportExportSelectPage();
	
	if (selectResult == 0xff)
	{	
		return EXPORT_CANCEL;
	}

	switch ( selectResult )
	{
		case 0:
			ExportStatus = ExportCurrentReport(g_testReport.nowIndex);
			break;
		case 1:
			ExportStatus = ExportPageReport(g_testReport.curPage,g_testReport.pSearchResult->sumPage);
			break;
		case 2:
			ExportStatus = ExportAllReport();
			g_testReport.curPage = 1;
			break;
		default:
			break;
	}
	
	switch ( ExportStatus )
	{
		case EXPORT_SUCCESS:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportExportCue[4]);		
			break;
		case EXPORT_ERROR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportExportCue[0]);	
			break;
		case EXPORT_CANCEL:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pReportExportCue[5]);	
			break;
	}
	
	PopWindowsProcessCycle();
	
	return ExportStatus;
}

/*------------------------------------------------------------
 * Function Name  : TestReportExportReport
 * Description    : 导出报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void TestReportExportReport( void )
{
	if ( g_testReport.curReportNum )
	{		
		g_testReport.leavePage.flagLeavePage = SET;
		g_testReport.leavePage.flagSaveData = RESET;
		
		if (Get_USB_Status() == ERROR)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pTestReportWarningCue[1]);							
			
			return;
		}
		
		TestReportExport();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportPrintOneReport
 * Description    : 打印一个报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus TestReportPrintOneReport( uint8_t reportIndex )
{
	uint8_t testType = 0;
	REPORT_TypeDef readReport;
	ErrorStatus errStatus;
	
	if (reportIndex >= SHOW_ONE_PAGE_REPORT_NUM)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pTestReportWarningCue[0]);
		
		return ERROR;
	}
	
	testType = g_testReport.pSearchResult->recordTestType[reportIndex];
	
	report_read(testType,g_testReport.pSearchResult->testReportData[reportIndex].fname,&readReport);
	
	errStatus = PrintTestReport(SMPL_FH_NUM,(TEST_TYPE_TypeDef)testType,&readReport,\
				&g_testReport.pSearchResult->testReportData[reportIndex]);
	
	return errStatus;
}

/*------------------------------------------------------------
 * Function Name  : TestReportPrintCursorRowReport
 * Description    : 打印光标所在行报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus TestReportPrintCursorRowReport( void )
{
	ErrorStatus errStatus;
	
	if ( g_testReport.curReportNum == 0)
	{
		return ERROR;
	}
	
	ShowPrintingShortcutMenu();
	
	errStatus = TestReportPrintOneReport(g_testReport.nowIndex);
		
	if (errStatus == ERROR)
	{			
		g_testReport.leavePage.flagLeavePage = SET;
		g_testReport.leavePage.flagSaveData = RESET;
	}
	
	g_testReport.refreshShortcut = ENABLE;
	
	return errStatus;
}	

/*------------------------------------------------------------
 * Function Name  : TestReportPrintAllPageReport
 * Description    : 打印整页报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportPrintAllPageReport( void )
{
	uint8_t reportIndex = 0;
	ErrorStatus errStatus;
	
	if ( g_testReport.curReportNum == 0)
	{
		return;
	}
	
	ShowPrintingShortcutMenu();
	
	for (reportIndex=0; reportIndex<g_testReport.curReportNum; ++reportIndex)
	{
		errStatus = TestReportPrintOneReport(reportIndex);
		
		if (errStatus == ERROR)
		{			
			g_testReport.leavePage.flagLeavePage = SET;
			g_testReport.leavePage.flagSaveData = RESET;
			
			break;
		}
	}
	
	g_testReport.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : GetSelectReportTestType
 * Description    : 获取选中报告的试验类型
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetSelectReportTestType( void )
{
	return g_testReport.pSearchResult->recordTestType[g_testReport.nowIndex];
}

/*------------------------------------------------------------
 * Function Name  : GetSelectReportFileNameAddr
 * Description    : 获取选中报告的文件名
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
char *GetSelectReportFileNameAddr( void )
{
	return (char *)g_testReport.pSearchResult->testReportData[g_testReport.nowIndex].fname;
}

/*------------------------------------------------------------
 * Function Name  : isShowDetailReport
 * Description    : 显示详细报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus isShowDetailReport( void )
{
	return g_testReport.showDetailReport;
}

/*------------------------------------------------------------
 * Function Name  : ResetShowDetailReport
 * Description    : 复位详细报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ResetShowDetailReport( void )
{
	g_testReport.showDetailReport = NO;
}

/*------------------------------------------------------------
 * Function Name  : GetTestReportTime
 * Description    : 获取试验报告时间
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
tTime GetTestReportTime( void )
{
	return g_testReport.pSearchResult->testReportData[g_testReport.nowIndex].time;
}

/*------------------------------------------------------------
 * Function Name  : TestReportIndexIncrease
 * Description    : 索引值递增
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void TestReportIndexIncrease( void )
{
	if ( g_testReport.curReportNum )
	{
		g_testReport.nowIndex++;
		g_testReport.nowIndex %= g_testReport.curReportNum;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportIndexDecrease
 * Description    : 索引值递减
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void TestReportIndexDecrease( void )
{
	if ( g_testReport.curReportNum )
	{
		if ( g_testReport.nowIndex )
		{
			g_testReport.nowIndex--;
		}
		else
		{
			g_testReport.nowIndex = g_testReport.curReportNum - 1;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetCurPageCurTestReportIndex
 * Description    : 获取当前页当前试验报告索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetCurPageCurTestReportIndex( void )
{
	return g_testReport.nowIndex;
}

/*------------------------------------------------------------
 * Function Name  : GetCurPageTestReportNum
 * Description    : 获取试验报告个数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetCurPageTestReportNum( void )
{
	return g_testReport.curReportNum;
}

/*------------------------------------------------------------
 * Function Name  : TestReportPressNumPrintReport
 * Description    : 按数字键打印报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportPressNumPrintReport( void )
{
	uint8_t keyValue = 0;
	BoolStatus isIncludeZero = NO;
	ErrorStatus errStatus;
	
	if (g_testReport.curReportNum == 0)
	{
		return;
	}
	
	if (g_testReport.curReportNum == SHOW_ONE_PAGE_REPORT_NUM)
	{
		isIncludeZero = YES;
	}
	
	keyValue = GetPressNum();
	
	if (keyValue <= g_testReport.curReportNum)
	{
		ShowPrintingShortcutMenu();
		
		if (keyValue)
		{
			errStatus = TestReportPrintOneReport(keyValue-1);	
		}
		else if ((isIncludeZero==YES) && (keyValue==0))
		{
			errStatus = TestReportPrintOneReport(keyValue);	
		}
		
		if (errStatus == ERROR)
		{			
			g_testReport.leavePage.flagLeavePage = SET;
			g_testReport.leavePage.flagSaveData = RESET;
		}
		
		g_testReport.refreshShortcut = ENABLE;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestReportKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestReportKeyProcess( void )
{
	TestReportPressNumPrintReport();
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F1:
				TestReportDeleteReport();
				break;
			
			case KEY_F2:
			case KEY_EXPORT:
				TestReportExportReport();
				break;

			case KEY_F3:
				TestReportPrintAllPageReport();
				break;
			
			case KEY_F4:
				SetPage(SEARCH_REPORT_PAGE);
				g_testReport.leavePage.flagLeavePage = SET;
				g_testReport.leavePage.flagSaveData = SET;
				break;
			
			case KEY_PRINT:
				TestReportPrintCursorRowReport();
				break;
			
			case KEY_LEFT:
				if (g_testReport.pSearchResult->sumPage > 1)
				{
					if (g_testReport.curPage)
					{
						g_testReport.curPage--;
					}
					
					if (!g_testReport.curPage)
					{
						g_testReport.curPage = g_testReport.pSearchResult->sumPage;
					}
					
					g_testReport.nowIndex = 0;
					
					g_testReport.leavePage.flagLeavePage = SET;
					g_testReport.leavePage.flagSaveData = RESET;
				}
				break;
			case KEY_RIGHT:
				if (g_testReport.pSearchResult->sumPage > 1)
				{
					g_testReport.curPage++;
					if (g_testReport.curPage > g_testReport.pSearchResult->sumPage)
					{
						g_testReport.curPage = 1;
					}
					
					g_testReport.nowIndex = 0;
					
					g_testReport.leavePage.flagLeavePage = SET;
					g_testReport.leavePage.flagSaveData = RESET;
				}
				break;
				
			case KEY_ENTER:
				SetPage(DETAIL_REPORT_PAGE);
				g_testReport.showDetailReport = YES;
				g_testReport.leavePage.flagLeavePage = SET;
				g_testReport.leavePage.flagSaveData = RESET;
				break;
			
			case KEY_ESC:
				SetPage(MAIN_PAGE);
				g_testReport.leavePage.flagLeavePage = SET;
				g_testReport.leavePage.flagSaveData = RESET;
				break;
		}
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

