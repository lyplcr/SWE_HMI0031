/**
  ******************************************************************************
  * @file    SearchReport.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-14 09:20:39
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   报告搜索
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SearchReport.h"
#include "ModelSelect.h"


/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK
#define MAX_PARAMETER_NUM				4
#define MAX_PARAMETER_BIT				16			//支持参数位数
#define	DATE_SEARCH_ALLOW_PUTIN_BIT		8
#define MAX_REPORT_NAME_BIT				16			//试验报告名称位数


#define	INDEX_FIND_OBJECT 				0
#define	INDEX_FIND_MODE					1
#define	INDEX_START_DATE				2
#define	INDEX_END_DATE					3
#define	INDEX_SPECIMEN_SERIAL			2

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	FIND_OBJECT = 0,
	FIND_MODE,
	START_DATE,
	END_DATE,
	SPECIMEN_SERIAL,
}SEARCH_REPORT_NAME_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_NUM+1][MAX_PARAMETER_BIT+1];	//最后一个用于存储开始日期的备份。
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_NUM];
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_PARAMETER_NUM];
	const char *pParameterNameArray[MAX_PARAMETER_NUM];
	TEST_TYPE_TypeDef testType;
	uint8_t indexArray[MAX_PARAMETER_NUM];
	uint8_t testIndexArray[SUPPORT_TEST_NUM];		//试验索引值
	uint8_t dataIndexArray[SUPPORT_TEST_NUM];		//数据索引值
	uint8_t curParameterNum;						//参数个数
	BoolStatus isIndexMove;							//索引值移动
	FunctionalState enableMoveIndex;				//使能移动索引值
	FunctionalState enablePutin;					//使能输入
	FunctionalState enableArrow;					//使能箭头
	STATUS_SHIFT_TypeDef enableShift;				//使能切换输入法
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
	MODEL_TYPE_TypeDef modelType;					//机型类型
	BoolStatus isAlreadyLoad;					
	BoolStatus isSearch;
}REPORT_SEARCH_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pReportSearchNameKY[] = 
{
	"所有试验类型",
	"水泥胶砂抗压",
	"建筑砂浆抗压",
	"混凝土抗压",
	"混凝土抗折",
	"砌墙砖抗压",
	"通用抗压",		
};

const char * const pReportSearchNameKZ[] = 
{
	"所有试验类型",
	"水泥胶砂抗折",	
	"压浆水泥浆抗折",
	"通用抗折",		
};

const char * const pReportSearchNameKZKY[] = 
{
	"全部试验",
	"水泥胶砂抗压",
	"建筑砂浆抗压",
	"混凝土抗压",
	"混凝土抗折",
	"砌墙砖抗压",
	"通用抗压",
	"水泥胶砂抗折",
	"压浆水泥浆抗折",
	"通用抗折",		
};

const char * const pReportSearchParameterName[] = 
{
	"查找对象：",
	"查找方式：",
	"开始日期：",
	"结束日期：",
	"试件编号：",
};

const char * const pFindModeName[] =
{
	"日期查询        ",
	"编号查询        ",
};

const char * const pSearchReportCue[] = 
{
	"日期必须输入8位，请参考下方",
	"日期输入格式。"
};

const char * const pReportSearchTitle = "报告搜索";

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static REPORT_SEARCH_TypeDef g_reportSearch;
static TEST_SEARCH_RESULT_TypeDef g_searchResult;


/* Private function prototypes -----------------------------------------------*/
static void ReportSearchInit( void );
static void ReportSearchConfig( void );
static void ReportSearchReadParameter( void );
static void GUI_ReportSearch( void );
static void Traverse_ReportSearch( void );
static void ReportSearchPutinProcess( void );
static void ReportSearchMoveIndexProcess( void );
static void ReportSearchStatusProcess( void );
static void ReportSearchMoveCursorProcess( void );
static void ReportSearchShortcutCycleTask( void );
static void ReportSearchKeyProcess( void );
static void ReportSearchLeavePageCheckCycle( void );



/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadReportSearchPage
 * Description    : 报告搜索页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadReportSearchPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	ReportSearchInit();
	
	/* 参数配置 */
	ReportSearchConfig();
	
	/* 获取参数 */
	ReportSearchReadParameter();
	
	/* 画GUI框架 */
	GUI_ReportSearch();
	
	/* 遍历 */
	Traverse_ReportSearch();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_reportSearch.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		ReportSearchPutinProcess();
		
		/* 移动索引值 */
		ReportSearchMoveIndexProcess();
		
		/* 状态处理 */
		ReportSearchStatusProcess();
		
		/* 移动光标 */
		ReportSearchMoveCursorProcess();
		
		/* 快捷菜单 */
		ReportSearchShortcutCycleTask();
		
		/* 按键处理 */
		ReportSearchKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ReportSearchLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchInit
 * Description    : 报告搜索初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchInit( void )
{
	g_reportSearch.modelType = GetModelType();
	g_reportSearch.testType = (TEST_TYPE_TypeDef)pHmi->test_standard_index;
		
	g_reportSearch.isIndexMove = NO;		
	g_reportSearch.enableMoveIndex = ENABLE;
	g_reportSearch.enablePutin = DISABLE;
	g_reportSearch.enableArrow = DISABLE;
	g_reportSearch.enableShift = DISABLE_SHIFT;
	g_reportSearch.recordIndex = 0xff;
	g_reportSearch.refreshShortcut = ENABLE;
	g_reportSearch.leavePage.flagLeavePage = RESET;
	g_reportSearch.leavePage.flagSaveData = RESET;
	g_reportSearch.isSearch = NO;
}

/*------------------------------------------------------------
 * Function Name  : GetReportSearchTestTypeIndex
 * Description    : 获取试验类型索引值
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetReportSearchTestTypeIndex( uint8_t type )
{
	uint8_t i;
	uint8_t testIndex = 0xff;	//错误的值
	
	for (i=0; i<g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].parameterCnt; ++i)
	{
		if (g_reportSearch.testIndexArray[i] == type)
		{
			testIndex = i;
			break;
		}
	}
	
	return testIndex;
}

/*------------------------------------------------------------
 * Function Name  : GetReportSearchIndex
 * Description    : 获取真实的索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetReportSearchIndex( uint8_t index )
{
	uint8_t i;
	uint8_t testIndex = 0xff;	//错误的值
	
	for (i=0; i<g_reportSearch.curParameterNum; ++i)
	{
		if (g_reportSearch.indexArray[i] == index)
		{
			testIndex = i;
			break;
		}
	}
	
	return testIndex;
}

/*------------------------------------------------------------
 * Function Name  : GetReportSearchDataAddr
 * Description    : 获取数据存储区索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetReportSearchDataRealIndex( uint8_t index )
{
	return g_reportSearch.dataIndexArray[index];
}

/*------------------------------------------------------------
 * Function Name  : GetReportSearchDataAddr
 * Description    : 获取数据地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static char *GetReportSearchDataAddr( uint8_t realIndex )
{
	uint8_t dataIndex = 0;
	
	dataIndex = GetReportSearchDataRealIndex(realIndex);
	
	return g_reportSearch.parameterData[dataIndex];
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchConfig
 * Description    : 配置信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchConfig( void )
{
	uint8_t index = GetReportSearchIndex(FIND_MODE);
	
	if (index == 0xff)	//第一次进入页面
	{
		index = INDEX_FIND_MODE;
		g_reportSearch.twoLevelMenu[index].index = DATE_SEARCH;	//默认值
	}
	
	/* 标题 */
	g_reportSearch.pTitle = pReportSearchTitle;
	
	if (g_reportSearch.twoLevelMenu[index].index == DATE_SEARCH)
	{
		/* 试块个数 */
		g_reportSearch.curParameterNum = 4;
		
		/* 索引值 */
		g_reportSearch.indexArray[INDEX_FIND_OBJECT] 	= FIND_OBJECT;		
		g_reportSearch.indexArray[INDEX_FIND_MODE] 		= FIND_MODE;		
		g_reportSearch.indexArray[INDEX_START_DATE] 	= START_DATE;	
		g_reportSearch.indexArray[INDEX_END_DATE] 		= END_DATE;	
		
		g_reportSearch.dataIndexArray[INDEX_FIND_OBJECT] 	= FIND_OBJECT;		
		g_reportSearch.dataIndexArray[INDEX_FIND_MODE] 		= FIND_MODE;		
		g_reportSearch.dataIndexArray[INDEX_START_DATE] 	= START_DATE;	
		g_reportSearch.dataIndexArray[INDEX_END_DATE] 		= END_DATE;
		
		/* 参数名称 */
		g_reportSearch.pParameterNameArray[INDEX_FIND_OBJECT] 	= pReportSearchParameterName[0];
		g_reportSearch.pParameterNameArray[INDEX_FIND_MODE] 	= pReportSearchParameterName[1];
		g_reportSearch.pParameterNameArray[INDEX_START_DATE] 	= pReportSearchParameterName[2];
		g_reportSearch.pParameterNameArray[INDEX_END_DATE] 		= pReportSearchParameterName[3];
	}
	else
	{
		/* 个数 */
		g_reportSearch.curParameterNum = 3;
		
		/* 索引值 */
		g_reportSearch.indexArray[INDEX_FIND_OBJECT] 		= FIND_OBJECT;		
		g_reportSearch.indexArray[INDEX_FIND_MODE] 			= FIND_MODE;		
		g_reportSearch.indexArray[INDEX_SPECIMEN_SERIAL] 	= SPECIMEN_SERIAL;		

		g_reportSearch.dataIndexArray[INDEX_FIND_OBJECT] 		= FIND_OBJECT;		
		g_reportSearch.dataIndexArray[INDEX_FIND_MODE] 			= FIND_MODE;		
		g_reportSearch.dataIndexArray[INDEX_SPECIMEN_SERIAL] 	= SPECIMEN_SERIAL;	
		
		/* 参数名称 */
		g_reportSearch.pParameterNameArray[INDEX_FIND_OBJECT] 		= pReportSearchParameterName[0];
		g_reportSearch.pParameterNameArray[INDEX_FIND_MODE] 		= pReportSearchParameterName[1];
		g_reportSearch.pParameterNameArray[INDEX_SPECIMEN_SERIAL] 	= pReportSearchParameterName[4];
	}
	
	/* 二级菜单试验类型索引值 */
	switch ( g_reportSearch.modelType )
	{
		case MODEL_KY:
			g_reportSearch.testIndexArray[0] 	= 0xff;
			g_reportSearch.testIndexArray[1] 	= KYSNJS;	
			g_reportSearch.testIndexArray[2] 	= KYJZSJ;		
			g_reportSearch.testIndexArray[3] 	= KYHNT;		
			g_reportSearch.testIndexArray[4] 	= KZHNT;			
			g_reportSearch.testIndexArray[5] 	= KYQQZ;
			g_reportSearch.testIndexArray[6] 	= KYTY;		
			break;
		case MODEL_KZ:
			g_reportSearch.testIndexArray[0] 	= 0xff;
			g_reportSearch.testIndexArray[1] 	= KZSNJS;	
			g_reportSearch.testIndexArray[2] 	= KZYJSNJ;	
			g_reportSearch.testIndexArray[3] 	= KZTY;
			break;
		case MODEL_KZKY:
			g_reportSearch.testIndexArray[0] 	= 0xff;
			g_reportSearch.testIndexArray[1] 	= KYSNJS;	
			g_reportSearch.testIndexArray[2] 	= KYJZSJ;		
			g_reportSearch.testIndexArray[3] 	= KYHNT;		
			g_reportSearch.testIndexArray[4] 	= KZHNT;			
			g_reportSearch.testIndexArray[5] 	= KYQQZ;
			g_reportSearch.testIndexArray[6] 	= KYTY;
			g_reportSearch.testIndexArray[7] 	= KZSNJS;	
			g_reportSearch.testIndexArray[8] 	= KZYJSNJ;	
			g_reportSearch.testIndexArray[9] 	= KZTY;
			break;		
	}
	
	/* 二级菜单参数个数 */
	switch ( g_reportSearch.modelType )
	{
		case MODEL_KY:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].parameterCnt = 7;
			break;
		case MODEL_KZ:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].parameterCnt = 4;
			break;
		case MODEL_KZKY:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].parameterCnt = 10;
			break;		
	}	
	g_reportSearch.twoLevelMenu[INDEX_FIND_MODE].parameterCnt			= 2;
	g_reportSearch.twoLevelMenu[INDEX_START_DATE].parameterCnt 			= 0;
	g_reportSearch.twoLevelMenu[INDEX_END_DATE].parameterCnt 			= 0;
	g_reportSearch.twoLevelMenu[INDEX_SPECIMEN_SERIAL].parameterCnt 	= 0;
	
	/* 二级菜单类型 */
	g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].parameterType 		= NONE_USE_USER_DEFINE;
	g_reportSearch.twoLevelMenu[INDEX_FIND_MODE].parameterType 			= NONE_USE_USER_DEFINE;
	g_reportSearch.twoLevelMenu[INDEX_START_DATE].parameterType 		= IMMEDIATELY_PUTIN_NONE;
	g_reportSearch.twoLevelMenu[INDEX_END_DATE].parameterType 			= IMMEDIATELY_PUTIN_NONE;
	g_reportSearch.twoLevelMenu[INDEX_SPECIMEN_SERIAL].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
	
	/* 二级菜单参数名 */
	switch ( g_reportSearch.modelType )
	{
		case MODEL_KY:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].pParameterNameArray = pReportSearchNameKY;
			break;
		case MODEL_KZ:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].pParameterNameArray = pReportSearchNameKZ;
			break;
		case MODEL_KZKY:
			g_reportSearch.twoLevelMenu[INDEX_FIND_OBJECT].pParameterNameArray = pReportSearchNameKZKY;
			break;		
	}	
	g_reportSearch.twoLevelMenu[INDEX_FIND_MODE].pParameterNameArray 		= pFindModeName;
	g_reportSearch.twoLevelMenu[INDEX_START_DATE].pParameterNameArray 		= NULL;
	g_reportSearch.twoLevelMenu[INDEX_END_DATE].pParameterNameArray 		= NULL;
	g_reportSearch.twoLevelMenu[INDEX_SPECIMEN_SERIAL].pParameterNameArray 	= NULL;
	
	/* 数据保存类型 */
	g_reportSearch.oneLevelMenu[INDEX_FIND_OBJECT].saveType 		= TYPE_CHAR;
	g_reportSearch.oneLevelMenu[INDEX_FIND_MODE].saveType 			= TYPE_CHAR;
	
	index = GetReportSearchIndex(FIND_MODE);
	if (g_reportSearch.twoLevelMenu[index].index == DATE_SEARCH)
	{
		g_reportSearch.oneLevelMenu[INDEX_START_DATE].saveType 			= TYPE_INT;
		g_reportSearch.oneLevelMenu[INDEX_END_DATE].saveType 			= TYPE_INT;
	}
	else
	{
		g_reportSearch.oneLevelMenu[INDEX_SPECIMEN_SERIAL].saveType 	= TYPE_CHAR;
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchReadParameter( void )
{
	uint8_t index = 0;
	uint8_t testIndex = 0;
	char time_buff[12];
	tTime t;
	
	if (g_reportSearch.isAlreadyLoad == NO)
	{
		index = GetReportSearchIndex(FIND_OBJECT);
		if (index != 0xff)
		{
			testIndex = GetReportSearchTestTypeIndex(g_reportSearch.testType);
			if (testIndex == 0xff)
			{
				g_reportSearch.twoLevelMenu[index].index = 0;
			}
			else
			{
				g_reportSearch.twoLevelMenu[index].index = testIndex;			
			}
			
			strcpy(GetReportSearchDataAddr(index),g_reportSearch.twoLevelMenu[index].pParameterNameArray[g_reportSearch.twoLevelMenu[index].index]);
		}
		
		index = GetReportSearchIndex(FIND_MODE);
		if (index != 0xff)
		{
			//g_reportSearch.twoLevelMenu[index].index = 0;	//配置信息已经配置过了
			strcpy(GetReportSearchDataAddr(index),g_reportSearch.twoLevelMenu[index].pParameterNameArray[g_reportSearch.twoLevelMenu[index].index]);
		}
		
		if (GetSearchType() == DATE_SEARCH)
		{
			time_cycle();
			t = get_time();	 	//获取时间
			usprintf(time_buff,"%04d%02d%02d",t.usYear,t.ucMon,t.ucMday);
			
			index = GetReportSearchIndex(START_DATE);
			if (index != 0xff)
			{
				g_reportSearch.twoLevelMenu[index].index = 0;
				strcpy(GetReportSearchDataAddr(index),time_buff);
			}
			
			index = GetReportSearchIndex(END_DATE);
			if (index != 0xff)
			{
				g_reportSearch.twoLevelMenu[index].index = 0;
				strcpy(GetReportSearchDataAddr(index),time_buff);
			}
		}
		
		g_reportSearch.isAlreadyLoad = YES;
	}

	if (GetSearchType() == SERIAL_SEARCH)
	{
		index = GetReportSearchIndex(SPECIMEN_SERIAL);
		if (index != 0xff)
		{
			g_reportSearch.twoLevelMenu[index].index = 0;
			strcpy(GetReportSearchDataAddr(index),"*");
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigReportSearchRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigReportSearchRectangleFrameCoordinate( void )
{
	uint16_t startY = 74;
	uint8_t i;
	
	for (i=0; i<g_reportSearch.curParameterNum; ++i)
	{
		g_reportSearch.oneLevelMenu[i].x = 180;
		g_reportSearch.oneLevelMenu[i].y = startY;
		g_reportSearch.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_reportSearch.oneLevelMenu[i].backColor = COLOR_BACK;
		g_reportSearch.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_reportSearch.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_reportSearch.oneLevelMenu[i].lenth = 198;
		g_reportSearch.oneLevelMenu[i].width = 30;
		g_reportSearch.oneLevelMenu[i].fontSize = 24;
		g_reportSearch.oneLevelMenu[i].rowDistance = 36;
		g_reportSearch.oneLevelMenu[i].columnDistance = 0;
		g_reportSearch.oneLevelMenu[i].lineWidth = 2;
		
		g_reportSearch.twoLevelMenu[i].x = g_reportSearch.oneLevelMenu[i].x + g_reportSearch.oneLevelMenu[i].lenth + 36;
		g_reportSearch.twoLevelMenu[i].y = g_reportSearch.oneLevelMenu[i].y;
		g_reportSearch.twoLevelMenu[i].pointColor = g_reportSearch.oneLevelMenu[i].pointColor;
		g_reportSearch.twoLevelMenu[i].backColor = g_reportSearch.oneLevelMenu[i].backColor;
		g_reportSearch.twoLevelMenu[i].lenth = 200;
		g_reportSearch.twoLevelMenu[i].fontSize = 24;
		g_reportSearch.twoLevelMenu[i].rowDistance = 2;
		g_reportSearch.twoLevelMenu[i].columnDistance = 0;
		g_reportSearch.twoLevelMenu[i].lineWidth = 2;				
		g_reportSearch.twoLevelMenu[i].width = g_reportSearch.twoLevelMenu[i].parameterCnt * \
												(g_reportSearch.twoLevelMenu[i].fontSize + g_reportSearch.twoLevelMenu[i].rowDistance) + \
												g_reportSearch.twoLevelMenu[i].rowDistance + 2 * g_reportSearch.twoLevelMenu[i].lineWidth;
		g_reportSearch.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_reportSearch.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_reportSearch.oneLevelMenu[i].width + g_reportSearch.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearchDrawOneRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearchDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_reportSearch.oneLevelMenu[index].x;
	rectangle.y = g_reportSearch.oneLevelMenu[index].y;
	rectangle.lenth = g_reportSearch.oneLevelMenu[index].lenth;
	rectangle.width = g_reportSearch.oneLevelMenu[index].width;
	rectangle.lineWidth = g_reportSearch.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_reportSearch.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearchRectangleFrame
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearchRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_reportSearch.curParameterNum; ++i)
	{
		GUI_ReportSearchDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearchDrawOneRowOneLevelMenu
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearchDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_reportSearch.oneLevelMenu[index].x - 5 * 24;
	const uint16_t y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_reportSearch.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_reportSearch.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_reportSearch.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearchOneLevelMenu
 * Description    : 界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearchOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_reportSearch.curParameterNum; ++i)
	{
		GUI_ReportSearchDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearchCue
 * Description    : 界面一级菜单提示内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearchCue( void )
{
	uint8_t index = GetReportSearchIndex(END_DATE);
	const uint16_t x = g_reportSearch.oneLevelMenu[index].x - 3 * 24;
	const uint16_t y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 50;
	const uint16_t pointColor = COLOR_FORBIT_EDIT_POINT;
	const uint16_t backColor = g_reportSearch.oneLevelMenu[index].backColor;
	
	GUI_DispStr16At(x,y,pointColor,backColor,"日期格式：20151010");
}

/*------------------------------------------------------------
 * Function Name  : GUI_ReportSearch
 * Description    : 报告搜索界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ReportSearch( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_reportSearch.pTitle);
	
	ConfigReportSearchRectangleFrameCoordinate();
	
	GUI_ReportSearchRectangleFrame();
	
	GUI_ReportSearchOneLevelMenu();
	
	if (GetSearchType() == DATE_SEARCH)
	{
		GUI_ReportSearchCue();
	}
}	

/*------------------------------------------------------------
 * Function Name  : Show_ReportSearchOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ReportSearchOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_reportSearch.oneLevelMenu[index].x + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_reportSearch.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_reportSearch.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_reportSearch.oneLevelMenu[index].lenth - 2 * g_reportSearch.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_reportSearch.oneLevelMenu[index].width - 2 * g_reportSearch.oneLevelMenu[index].lineWidth - 2;
	char timeBuff[12];
	
	lcd_fill(x,y,lenth,width,backColor);
	
	if ( (GetSearchType() == DATE_SEARCH) && ( (GetReportSearchIndex(START_DATE)==index) || (GetReportSearchIndex(END_DATE)==index) ) )
	{		
		ChangeDateFormate(GetReportSearchDataAddr(index),timeBuff,'-');
		GUI_DispStr24At(x,y,pointColor,backColor,timeBuff);
	}
	else
	{
		GUI_DispStr24At(x,y,pointColor,backColor,GetReportSearchDataAddr(index));
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_ReportSearchOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ReportSearchOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_reportSearch.curParameterNum; ++i)
	{
		Show_ReportSearchOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ReportSearch
 * Description    : 遍历报告搜索
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ReportSearch( void )
{
	Show_ReportSearchOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : GetSearchType
 * Description    : 获取搜索类型
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FIND_TYPE_INDEX_TypeDef GetSearchType( void )
{
	uint8_t index = GetReportSearchIndex(FIND_MODE);
	
	if (g_reportSearch.twoLevelMenu[index].index == DATE_SEARCH)
	{
		return DATE_SEARCH;
	}
	
	return SERIAL_SEARCH;
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_reportSearch.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_reportSearch.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_reportSearch.enablePutin;
	pPutin->NewShift = g_reportSearch.enableShift;
	pPutin->x = g_reportSearch.oneLevelMenu[index].x + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	if(GetSearchType() == DATE_SEARCH)
	{
		pPutin->AllowPutinBit = DATE_SEARCH_ALLOW_PUTIN_BIT;
		pPutin->FillBit = DATE_SEARCH_ALLOW_PUTIN_BIT;
	}
	else
	{
		pPutin->AllowPutinBit = MAX_REPORT_NAME_BIT;
		pPutin->FillBit = MAX_REPORT_NAME_BIT;
	}
	pPutin->putinFrameLenth = g_reportSearch.oneLevelMenu[index].lenth - 2 * g_reportSearch.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_reportSearch.oneLevelMenu[index].width - 2 * g_reportSearch.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_reportSearch.putinNum;
	pPutin->SaveType = g_reportSearch.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_reportSearch.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchMoveIndexProcess
 * Description    : 移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_reportSearch.isIndexMove = NO;
	
	indexObj.enableMoveIndex = g_reportSearch.enableMoveIndex;
	indexObj.rowNum = g_reportSearch.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_reportSearch.curParameterNum;
	indexObj.pNowIndex = &g_reportSearch.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_reportSearch.nowIndex != g_reportSearch.recordIndex)
	{		
		g_reportSearch.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchStatusProcess( void )
{
	uint8_t index = g_reportSearch.nowIndex;
	
	g_reportSearch.nowIndex %= g_reportSearch.curParameterNum;
	
	switch ( g_reportSearch.twoLevelMenu[index].parameterType )
	{
		case IMMEDIATELY_PUTIN_NONE:
		case IMMEDIATELY_PUTIN_SHIFT:
			g_reportSearch.enablePutin = ENABLE;
			break;
		
		default:
			g_reportSearch.enablePutin = DISABLE;
			break;
	}
	
	if (g_reportSearch.oneLevelMenu[index].saveType == TYPE_CHAR)
	{
		g_reportSearch.enableShift = ENABLE_SHIFT;
	}
	else
	{
		g_reportSearch.enableShift = DISABLE_SHIFT;
	}
	
	switch ( g_reportSearch.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_reportSearch.enableArrow = ENABLE;
			break;
		
		default:
			g_reportSearch.enableArrow = DISABLE;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : Clear_ReportSearchOneRowOneLevelMenuArrow
 * Description    : 清除一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_ReportSearchOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_reportSearch.oneLevelMenu[index].x + g_reportSearch.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_reportSearch.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Show_ReportSearchOneRowOneLevelMenuArrow
 * Description    : 显示一行箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ReportSearchOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_reportSearch.oneLevelMenu[index].x + g_reportSearch.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_reportSearch.oneLevelMenu[index].y + g_reportSearch.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchMoveCursorProcess( void )
{
	if (g_reportSearch.isIndexMove == YES)
	{		
		if (g_reportSearch.recordIndex != 0xff)
		{
			g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].pointColor = g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].recordPointColor;
			g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].backColor = g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].recordBackColor;
			
			Show_ReportSearchOneRowOneLevelMenuContent(g_reportSearch.recordIndex);
			Clear_ReportSearchOneRowOneLevelMenuArrow(g_reportSearch.recordIndex);
			
			g_reportSearch.oneLevelMenu[g_reportSearch.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_reportSearch.oneLevelMenu[g_reportSearch.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ReportSearchOneRowOneLevelMenuContent(g_reportSearch.nowIndex);		
			if (g_reportSearch.enableArrow == ENABLE)
			{
				Show_ReportSearchOneRowOneLevelMenuArrow(g_reportSearch.nowIndex);
			}
		}
		else
		{
			g_reportSearch.oneLevelMenu[g_reportSearch.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_reportSearch.oneLevelMenu[g_reportSearch.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ReportSearchOneRowOneLevelMenuContent(g_reportSearch.nowIndex);
			if (g_reportSearch.enableArrow == ENABLE)
			{
				Show_ReportSearchOneRowOneLevelMenuArrow(g_reportSearch.nowIndex);
			}
		}
		
		g_reportSearch.recordIndex = g_reportSearch.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_reportSearch.refreshShortcut == ENABLE)
	{
		g_reportSearch.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[3] = pTwoLevelMenu[63];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchIndexUpdate( void )
{
	g_reportSearch.nowIndex++;
	g_reportSearch.nowIndex %= g_reportSearch.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchUpdateStatus( void )
{
	ReportSearchIndexUpdate();
	
	g_reportSearch.refreshShortcut = ENABLE;
}
	

/*------------------------------------------------------------
 * Function Name  : TestParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchKeyProcess( void )
{
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	uint8_t index = g_reportSearch.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_reportSearch.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_reportSearch.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* 处理快捷菜单 */
					GUI_ClearShortcutMenu();
					
					/* 还原一级菜单 */
					g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].pointColor = \
						g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].recordPointColor;
					
					g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].backColor = \
						g_reportSearch.oneLevelMenu[g_reportSearch.recordIndex].recordBackColor;
					
					Show_ReportSearchOneRowOneLevelMenuContent(g_reportSearch.recordIndex);
					
					/* 配置二级菜单 */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_reportSearch.twoLevelMenu[index].x;
					pMenu->y = g_reportSearch.twoLevelMenu[index].y;
					pMenu->nowIndex = g_reportSearch.twoLevelMenu[index].index;
					pMenu->maxUpY = g_reportSearch.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_reportSearch.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_reportSearch.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_reportSearch.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_reportSearch.twoLevelMenu[index].lenth;
					pMenu->width = g_reportSearch.twoLevelMenu[index].width;
					pMenu->pointColor = g_reportSearch.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_reportSearch.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_reportSearch.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_reportSearch.twoLevelMenu[index].pointColor;
					pMenu->nums = g_reportSearch.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_reportSearch.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_reportSearch.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{		
						g_reportSearch.twoLevelMenu[index].index = pMenu->nowIndex;
						
						strcpy(GetReportSearchDataAddr(index),pMenu->pParameterNameArray[pMenu->nowIndex]);
						
						if (index == GetReportSearchIndex(FIND_MODE))
						{
							g_reportSearch.leavePage.flagLeavePage = SET;
							g_reportSearch.leavePage.flagSaveData = RESET;
						}
					}
					
					ReportSearchUpdateStatus();				
				}
				break;
				
			case KEY_F4:
				SetPage(TEST_REPORT_PAGE);
				g_reportSearch.leavePage.flagLeavePage = SET;
				g_reportSearch.leavePage.flagSaveData = SET;
				break;
			
			case KEY_ENTER:
				if ( (g_reportSearch.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_NONE) || \
				     (g_reportSearch.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_SHIFT) )
				{				
					putinStatus = GetPutinStatus();
					
					switch ( putinStatus )
					{
						case STATUS_DISABLE_PUTIN:
							return;
						
						case STATUS_EDIT_COMP:	
							switch ( g_reportSearch.oneLevelMenu[index].saveType )
							{
								case TYPE_INT:
									if (g_reportSearch.putinNum != 8)
									{
										SetPopWindowsInfomation(POP_PCM_CUE,2,pSearchReportCue);
										g_reportSearch.leavePage.flagLeavePage = SET;
										g_reportSearch.leavePage.flagSaveData = RESET;
										
										return;
									}
									numtochar(g_reportSearch.putinNum,*GetPutinIntDataAddr(),GetReportSearchDataAddr(index));																											

									if (g_reportSearch.nowIndex == GetReportSearchIndex(START_DATE))
									{
										g_reportSearch.nowIndex = GetReportSearchIndex(END_DATE);
									}
									else if (g_reportSearch.nowIndex == GetReportSearchIndex(END_DATE))
									{
										g_reportSearch.nowIndex = GetReportSearchIndex(START_DATE);
									}
									break;
								case TYPE_CHAR:
									strcpy(GetReportSearchDataAddr(index),GetPutinCharDataAddr());
									if (g_reportSearch.nowIndex == GetReportSearchIndex(SPECIMEN_SERIAL))
									{
										g_reportSearch.recordIndex = 0xff;
									}
									break;
								default:
									break;
							}
							break;						
						
						default:
							break;
					}

					g_reportSearch.refreshShortcut = ENABLE;
				}
				break;
			
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(TEST_REPORT_PAGE);
						g_reportSearch.leavePage.flagLeavePage = SET;
						g_reportSearch.leavePage.flagSaveData = RESET;
						break;
					case STATUS_CANCEL_PUTIN:
						ReportSearchUpdateStatus();		
						break;
					
					default:
						break;
				}			
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ReportSearchLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ReportSearchLeavePageCheckCycle( void )
{
	if (g_reportSearch.leavePage.flagLeavePage == SET)
	{
		if (g_reportSearch.leavePage.flagSaveData == SET)
		{
			g_reportSearch.isSearch = YES;
			
			StartOneReportSearch();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestSearchResultInit
 * Description    : 试验搜索结果初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestSearchResultInit( void )
{	
	g_searchResult.sumNum = 0;
	g_searchResult.sumPage = 0;	
}

/*------------------------------------------------------------
 * Function Name  : GetTestDataFromSD
 * Description    : 获取试验数据从SD卡(搜索所有试验时，没有根据机型配置，将搜索SD卡中所有类型的试验)
 * Input          : SearchAll：全部重新搜索，searchType：搜索类型，offset：起始点偏移量，cnt：搜索到个数
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GetTestDataFromSD( FunctionalState SearchAll, FIND_TYPE_INDEX_TypeDef searchType, uint32_t offset )
{
	tTime start,end;
	uint8_t test_type = 0;
	uint8_t testIndex = 0,j = 0;
	uint32_t sum = 0,search_cnt = 0,offset_temp = 0,temp_pos = 0;
	TEST_INFO_TypeDef temp_info[SEARCH_ONE_PAGE_REPORT_NUM+1];
	char file_name[MAX_REPORT_NAME_BIT+1];
	uint8_t indexTemp = 0;
	
	indexTemp = GetReportSearchIndex(FIND_OBJECT);
	test_type = g_reportSearch.testIndexArray[g_reportSearch.twoLevelMenu[indexTemp].index];
	
	switch ( searchType )
	{
		case DATE_SEARCH:
			indexTemp = GetReportSearchIndex(START_DATE);
			CharConvTimeFormate(GetReportSearchDataAddr(indexTemp),&start);
		
			indexTemp = GetReportSearchIndex(END_DATE);
			CharConvTimeFormate(GetReportSearchDataAddr(indexTemp),&end);
		
			if ( test_type != 0xff )	//某种试验类型
			{
				report_search_date(test_type,start,end,offset,g_searchResult.testReportData,&g_searchResult.sumNum);
				for (j=0; j<SEARCH_ONE_PAGE_REPORT_NUM; ++j)
				{
					g_searchResult.recordTestType[j] = test_type;	
				}
			}
			else				//所有试验类型
			{
				if (SearchAll == ENABLE)	//所有结果
				{
					for (testIndex=KYSNJS; testIndex<=KZTY; ++testIndex)
					{
						report_search_date(testIndex,start,end,0,temp_info,&search_cnt);
						g_searchResult.searchNum[testIndex] = search_cnt;
						sum += search_cnt;
					}
					
					g_searchResult.sumNum = sum;
				}
				else				//指定位置处开始的结果
				{
					//定位偏移量在试验中的位置
					for (testIndex=KYSNJS; testIndex<=KZTY; ++testIndex)
					{
						if (offset_temp + g_searchResult.searchNum[testIndex] > offset)
						{
							temp_pos = offset - offset_temp;
							
							while (testIndex <= KZTY)
							{
								if ( g_searchResult.searchNum[testIndex] )
								{
									report_search_date(testIndex,start,end,temp_pos,temp_info,&search_cnt);
									
									search_cnt -= temp_pos;	//当前索引位置之后的个数
									
									if (sum + search_cnt >= SEARCH_ONE_PAGE_REPORT_NUM)	//搜索完毕
									{
										memcpy(&g_searchResult.testReportData[sum],temp_info,(SEARCH_ONE_PAGE_REPORT_NUM-sum)*sizeof(TEST_INFO_TypeDef));
										
										for (j=sum; j<SEARCH_ONE_PAGE_REPORT_NUM; ++j)
										{
											g_searchResult.recordTestType[j] = testIndex;
										}
										
										return;
									}
									else
									{
										memcpy(&g_searchResult.testReportData[sum],temp_info,search_cnt*sizeof(TEST_INFO_TypeDef));
										for (j=sum; j<sum+search_cnt; ++j)
										{
											g_searchResult.recordTestType[j] = testIndex;
										}
									}
									
									sum += search_cnt;
								}
								
								temp_pos = 0;
								testIndex++;
							}
						}
						else
						{
							offset_temp += g_searchResult.searchNum[testIndex];					
						}
					}
				}
			}	
			break;
		
		case SERIAL_SEARCH:
//			LowercaseConvCapital(SearchReport.data[SEARCH_SERIAL].buff,file_name);
			indexTemp = GetReportSearchIndex(SPECIMEN_SERIAL);
			strcpy(file_name,GetReportSearchDataAddr(indexTemp));
		
			if ( strcmp(file_name,"*") == 0 )
			{
				file_name[0] = NULL;	//搜索全部文件
			}

			if ( test_type != 0xff )	//某种试验类型
			{				
				report_search_sn(test_type,file_name,offset,g_searchResult.testReportData,&g_searchResult.sumNum);
				
				for (j=0; j<SEARCH_ONE_PAGE_REPORT_NUM; ++j)
				{
					g_searchResult.recordTestType[j] = test_type;
				}
			}
			else				//所有试验类型
			{
				if (SearchAll == ENABLE)	//所有结果
				{
					for (testIndex=KYSNJS; testIndex<=KZTY; ++testIndex)
					{
						report_search_sn(testIndex,file_name,0,temp_info,&search_cnt);
						g_searchResult.searchNum[testIndex] = search_cnt;
						sum += search_cnt;
					}
					
					g_searchResult.sumNum = sum;
				}
				else				//指定位置处开始的结果
				{
					//定位偏移量在试验中的位置
					for (testIndex=KYSNJS; testIndex<=KZTY; ++testIndex)
					{
						if (offset_temp + g_searchResult.searchNum[testIndex] > offset)
						{
							temp_pos = offset - offset_temp;
							
							while (testIndex <= KZTY)
							{
								if ( g_searchResult.searchNum[testIndex] )
								{
									report_search_sn(testIndex,file_name,temp_pos,temp_info,&search_cnt);
									
									search_cnt -= temp_pos;	//当前索引位置之后的个数
									
									if (sum + search_cnt >= SEARCH_ONE_PAGE_REPORT_NUM)	//搜索完毕
									{
										memcpy(&g_searchResult.testReportData[sum],temp_info,(SEARCH_ONE_PAGE_REPORT_NUM-sum)*sizeof(TEST_INFO_TypeDef));
										
										for (j=sum; j<SEARCH_ONE_PAGE_REPORT_NUM; ++j)
										{
											g_searchResult.recordTestType[j] = testIndex;
										}
										
										return;
									}
									else
									{
										memcpy(&g_searchResult.testReportData[sum],temp_info,search_cnt*sizeof(TEST_INFO_TypeDef));
										
										for (j=sum; j<sum+search_cnt; ++j)
										{
											g_searchResult.recordTestType[j] = testIndex;
										}
									}
									
									sum += search_cnt;
								}
								
								temp_pos = 0;
								testIndex++;
							}
						}
						else
						{
							offset_temp += g_searchResult.searchNum[testIndex];					
						}
					}
				}
			}
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : StartOneReportSearch
 * Description    : 启动一次报告搜索
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void StartOneReportSearch( void )
{
	TestSearchResultInit();
	
	GetTestDataFromSD( ENABLE, GetSearchType(),0);
	
	if ( g_searchResult.sumNum )
	{
		g_searchResult.sumPage = (g_searchResult.sumNum - 1) / SEARCH_ONE_PAGE_REPORT_NUM + 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetSearchReportResultAddr
 * Description    : 获取报告搜索结果地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TEST_SEARCH_RESULT_TypeDef *GetSearchReportResultAddr( void )
{
	return &g_searchResult;
}

/*------------------------------------------------------------
 * Function Name  : isAgainSearchReport
 * Description    : 是否重新搜索报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus isAgainSearchReport( void )
{
	return g_reportSearch.isSearch;
}

/*------------------------------------------------------------
 * Function Name  : ResetAgainSearchReport
 * Description    : 复位重新搜索报告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ResetAgainSearchReport( void )
{
	g_reportSearch.isSearch = NO;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

