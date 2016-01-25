/**
  ******************************************************************************
  * @file    HardTest.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-25 08:33:03
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   硬件测试
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "HardTest.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

/* 通道区域 */
#define MAX_CHANNEL_ROW_CNT				2
#define MAX_CHANNEL_COLUMN_NUM			3

/* 限位区域 */
#define MAX_LIMIT_ROW_CNT				3

#define MAX_PARAMETER_BIT				8



/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	ABSOLUTE_CODE = 0,
	OPPOSITE_CODE,
}CODE_TYPE_TypeDef;

typedef enum
{
	INDEX_ABSOLUTE_CODE = 0,
	INDEX_OPPOSITE_CODE,
}INDEX_CODE_TYPE_TypeDef;

typedef enum
{
	SEND_CMD_ENCHANGE = 0,
	SEND_CMD_ABSOLUTE_WAIT,
	SEND_CMD_ABSOLUTE,
	SEND_CMD_RELATIVE_WAIT,
	SEND_CMD_RELATIVE,
}STATUS_CMD_ENCHANGE_TypeDef;

typedef enum
{
	OBJECT_FH_CHANNEL = 0,
	OBJECT_WY_CHANNEL,
	OBJECT_BX_CHANNEL,
}OBJECT_CHANNEL_BOX_TypeDef;

typedef enum
{
	INDEX_FH_CHANNEL = 0,
	INDEX_WY_CHANNEL,
	INDEX_BX_CHANNEL,
}INDEX_CHANNEL_BOX_TypeDef;

typedef enum
{
	OBJECT_UP_LIMIT = 0,
	OBJECT_DOWN_LIMIT,
	OBJECT_OIL_LIMIT,
}OBJECT_LIMIT_TypeDef;

typedef enum
{
	INDEX_UP_LIMIT = 0,
	INDEX_DOWN_LIMIT,
	INDEX_OIL_LIMIT,
}INDEX_LIMIT_TypeDef;

typedef struct
{
	char data[MAX_PARAMETER_BIT+1];
}CHANNEL_DATA_TypeDef;

typedef struct
{
	/* 通道区 */
	CHANNEL_DATA_TypeDef parameterDataChannel[MAX_CHANNEL_ROW_CNT][MAX_CHANNEL_COLUMN_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuChannel[MAX_CHANNEL_ROW_CNT][MAX_CHANNEL_COLUMN_NUM];
	const char *pParameterNameArrayChannel[MAX_CHANNEL_COLUMN_NUM];
	const char *pParameterNameArrayRowChannel[MAX_CHANNEL_ROW_CNT];
	uint8_t indexArrayChannel[MAX_CHANNEL_COLUMN_NUM];
	uint8_t channelRowNum;				
	uint8_t channelColumnNum;
	
	/* 限位区 */
	char parameterDataLimit[MAX_LIMIT_ROW_CNT][MAX_PARAMETER_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuLimit[MAX_LIMIT_ROW_CNT];
	const char *pParameterNameArrayLimit[MAX_LIMIT_ROW_CNT];
	uint8_t indexArrayLimit[MAX_LIMIT_ROW_CNT];
	uint8_t limitRowNum;				
	uint8_t limitColumnNum;
	
	const char * pTitle;							//标题
	LEAVE_PAGE_TypeDef leavePage;					//离开页
	STATUS_CMD_ENCHANGE_TypeDef statusCmd;
}HARD_TEST_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pHardTestName[] =
{
	"负荷通道",		//0
	"位移通道",		//1
	"变形通道",		//2
	"绝对码",		//3
	"相对码",		//4
	"上限位：",		//5
	"下限位：",		//6
	"油缸限位：",	//7
	"油泵：",		//8
	"急停按钮：",	//9
};

const char * const pHardTestCue[] = 
{
	"发送实时数据命令失败！",		//0
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static HARD_TEST_TypeDef g_hardTest;

/* Private function prototypes -----------------------------------------------*/
static void HardTestInit( void );
static void HardTestConfig( void );
static void GUI_HardTest( void );
static void HardTestReadParameter( void );
static void Traverse_HardTest( void );
static void HardTestKeyProcess( void );
static void HardTestLeavePageCheckCycle( void );
static void DataInteractionWithPRM( void );
static void SendTareCmdInteractionTask( SMPL_NAME_TypeDef channel );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadHardTestPage
 * Description    : 硬件测试页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadHardTestPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	HardTestInit();
	
	/* 参数配置 */
	HardTestConfig();
	
	/* 获取参数 */
	HardTestReadParameter();
	
	/* 画GUI框架 */
	GUI_HardTest();
	
	/* 遍历 */
	Traverse_HardTest();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_hardTest.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 数据交互 */
		DataInteractionWithPRM();
		
		/* 按键处理 */
		HardTestKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		HardTestLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : HardTestInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestInit( void )
{	
	g_hardTest.leavePage.flagLeavePage = RESET;
	g_hardTest.leavePage.flagSaveData = RESET;
	
	g_hardTest.statusCmd = SEND_CMD_ENCHANGE;
}

/*------------------------------------------------------------
 * Function Name  : GetHardTestChannelIndex
 * Description    : 获取通道参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetHardTestChannelIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_hardTest.channelColumnNum; ++i)
	{
		if (g_hardTest.indexArrayChannel[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : GetHardTestLimitIndex
 * Description    : 获取限位参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetHardTestLimitIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		if (g_hardTest.indexArrayLimit[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : HardTestConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestConfig( void )
{
	/* 标题 */
	g_hardTest.pTitle = "硬件测试";
	
	/* 试块个数 */
	g_hardTest.channelRowNum = MAX_CHANNEL_ROW_CNT;
	g_hardTest.channelColumnNum = MAX_CHANNEL_COLUMN_NUM;
	
	/* 索引值 */
	g_hardTest.indexArrayChannel[INDEX_FH_CHANNEL] 		= OBJECT_FH_CHANNEL;
	g_hardTest.indexArrayChannel[INDEX_WY_CHANNEL] 		= OBJECT_WY_CHANNEL;
	g_hardTest.indexArrayChannel[INDEX_BX_CHANNEL] 		= OBJECT_BX_CHANNEL;
	
	/* 参数名称 */
	g_hardTest.pParameterNameArrayChannel[INDEX_FH_CHANNEL] 	= pHardTestName[0];
	g_hardTest.pParameterNameArrayChannel[INDEX_WY_CHANNEL] 	= pHardTestName[1];
	g_hardTest.pParameterNameArrayChannel[INDEX_BX_CHANNEL] 	= pHardTestName[2];
	
	g_hardTest.pParameterNameArrayRowChannel[INDEX_ABSOLUTE_CODE] = pHardTestName[3];
	g_hardTest.pParameterNameArrayRowChannel[INDEX_OPPOSITE_CODE] = pHardTestName[4];
	
	/* 试块个数 */
	g_hardTest.limitRowNum = MAX_LIMIT_ROW_CNT;
	g_hardTest.limitColumnNum = 1;
	
	/* 索引值 */
	g_hardTest.indexArrayLimit[INDEX_UP_LIMIT] 		= OBJECT_UP_LIMIT;
	g_hardTest.indexArrayLimit[INDEX_DOWN_LIMIT] 	= OBJECT_DOWN_LIMIT;
	g_hardTest.indexArrayLimit[INDEX_OIL_LIMIT] 	= OBJECT_OIL_LIMIT;
	
	/* 参数名称 */
	g_hardTest.pParameterNameArrayLimit[INDEX_UP_LIMIT] 		= pHardTestName[5];
	g_hardTest.pParameterNameArrayLimit[INDEX_DOWN_LIMIT] 		= pHardTestName[6];
	g_hardTest.pParameterNameArrayLimit[INDEX_OIL_LIMIT] 		= pHardTestName[7];
}

/*------------------------------------------------------------
 * Function Name  : ConfigHardTestChannelRectangleFrameCoordinate
 * Description    : 配置界面通道GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigHardTestChannelRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 300;
	const uint16_t INIT_START_Y = 120;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i,j;
	
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		for (j=0; j<g_hardTest.channelColumnNum; ++j)
		{
			g_hardTest.oneLevelMenuChannel[i][j].x = startX;
			g_hardTest.oneLevelMenuChannel[i][j].y = startY;
			g_hardTest.oneLevelMenuChannel[i][j].pointColor = COLOR_POINT;
			g_hardTest.oneLevelMenuChannel[i][j].backColor = COLOR_BACK;
			g_hardTest.oneLevelMenuChannel[i][j].recordPointColor = COLOR_POINT;
			g_hardTest.oneLevelMenuChannel[i][j].recordBackColor = COLOR_BACK;
			g_hardTest.oneLevelMenuChannel[i][j].lenth = 102;
			g_hardTest.oneLevelMenuChannel[i][j].width = 30;
			g_hardTest.oneLevelMenuChannel[i][j].fontSize = 24;
			g_hardTest.oneLevelMenuChannel[i][j].rowDistance = 0;
			g_hardTest.oneLevelMenuChannel[i][j].columnDistance = 0;
			g_hardTest.oneLevelMenuChannel[i][j].lineWidth = 2;
			
			startX += g_hardTest.oneLevelMenuChannel[i][j].lenth + \
						g_hardTest.oneLevelMenuChannel[i][j].columnDistance - \
						g_hardTest.oneLevelMenuChannel[i][j].lineWidth;
		}
		
		startX = INIT_START_X;
		startY += g_hardTest.oneLevelMenuChannel[0][0].width + \
					g_hardTest.oneLevelMenuChannel[0][0].rowDistance - \
					g_hardTest.oneLevelMenuChannel[0][0].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawOneChannelRectangleFrame
 * Description    : 画一个通道GUI矩形框
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawOneChannelRectangleFrame( uint8_t rowIndex, uint8_t columnIndex )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].x;
	rectangle.y = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].y;
	rectangle.lenth = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lenth;
	rectangle.width = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].width;
	rectangle.lineWidth = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lineWidth;
	rectangle.lineColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestChannelRectangleFrame
 * Description    : 通道界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestChannelRectangleFrame( void )
{
	uint8_t i,j;
	
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		for (j=0; j<g_hardTest.channelColumnNum; ++j)
		{
			GUI_HardTestDrawOneChannelRectangleFrame(i,j);
		}
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawChannelUpOneRowOneLevelMenu
 * Description    : 通道区域上方一级菜单标题
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawChannelUpOneRowOneLevelMenu( uint8_t rowIndex, uint8_t columnIndex )
{
	const uint16_t x = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].x;
	const uint16_t y = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].y - 40;
	const uint16_t pointColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArrayChannel[columnIndex]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawChannelLeftOneRowOneLevelMenu
 * Description    : 通道区域左方一级菜单标题
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawChannelLeftOneRowOneLevelMenu( uint8_t rowIndex, uint8_t columnIndex )
{
	const uint16_t x = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].x - \
						3 * g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].fontSize - 20;
	const uint16_t y = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].y + \
						g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lineWidth;
	const uint16_t pointColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArrayRowChannel[rowIndex]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestChannelOneLevelMenu
 * Description    : 通道界面一级菜单标题
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestChannelOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.channelColumnNum; ++i)
	{
		GUI_HardTestDrawChannelUpOneRowOneLevelMenu(0,i);
	}	
	
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		GUI_HardTestDrawChannelLeftOneRowOneLevelMenu(i,0);
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigHardTestLimitParameterRectangleFrameCoordinate
 * Description    : 配置限位界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigHardTestLimitParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 350;
	const uint16_t START_Y = 230;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		g_hardTest.oneLevelMenuLimit[i].x = startX;
		g_hardTest.oneLevelMenuLimit[i].y = startY;
		g_hardTest.oneLevelMenuLimit[i].pointColor = COLOR_POINT;
		g_hardTest.oneLevelMenuLimit[i].backColor = COLOR_BACK;
		g_hardTest.oneLevelMenuLimit[i].recordPointColor = COLOR_POINT;
		g_hardTest.oneLevelMenuLimit[i].recordBackColor = COLOR_BACK;
		g_hardTest.oneLevelMenuLimit[i].lenth = 102;
		g_hardTest.oneLevelMenuLimit[i].width = 30;
		g_hardTest.oneLevelMenuLimit[i].fontSize = 24;
		g_hardTest.oneLevelMenuLimit[i].rowDistance = 24;
		g_hardTest.oneLevelMenuLimit[i].columnDistance = 0;
		g_hardTest.oneLevelMenuLimit[i].lineWidth = 2;
		
		startY += g_hardTest.oneLevelMenuLimit[i].width + g_hardTest.oneLevelMenuLimit[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawOneLimitRectangleFrame
 * Description    : 画一个限位GUI矩形框
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawOneLimitRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_hardTest.oneLevelMenuLimit[index].x;
	rectangle.y = g_hardTest.oneLevelMenuLimit[index].y;
	rectangle.lenth = g_hardTest.oneLevelMenuLimit[index].lenth;
	rectangle.width = g_hardTest.oneLevelMenuLimit[index].width;
	rectangle.lineWidth = g_hardTest.oneLevelMenuLimit[index].lineWidth;
	rectangle.lineColor = g_hardTest.oneLevelMenuLimit[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestLimitRectangleFrame
 * Description    : 限位界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestLimitRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		GUI_HardTestDrawOneLimitRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawLimitOneRowOneLevelMenu
 * Description    : 限位区域一级菜单标题
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawLimitOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_hardTest.oneLevelMenuLimit[index].x - \
						5 * g_hardTest.oneLevelMenuLimit[index].fontSize;
	const uint16_t y = g_hardTest.oneLevelMenuLimit[index].y + \
						g_hardTest.oneLevelMenuLimit[index].lineWidth;
	const uint16_t pointColor = g_hardTest.oneLevelMenuLimit[index].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuLimit[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArrayLimit[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestChannelOneLevelMenu
 * Description    : 通道界面一级菜单标题
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestLimitOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		GUI_HardTestDrawLimitOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestLimitFrame
 * Description    : 硬件测试边框GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_HardTestLimitFrame( void )
{
	TITLE_FRAME_TypeDef frame;
	
	frame.x = g_hardTest.oneLevelMenuLimit[0].x - \
				5 * g_hardTest.oneLevelMenuLimit[0].fontSize - 20;
	frame.y = g_hardTest.oneLevelMenuLimit[0].y - 30;
	frame.lenth = g_hardTest.oneLevelMenuLimit[0].lenth + \
					5 * g_hardTest.oneLevelMenuLimit[0].fontSize + 40;
	frame.width = (g_hardTest.oneLevelMenuLimit[0].width + 15) * \
					g_hardTest.limitRowNum + 50;
	frame.linePointColor = CL_GREY2;
	frame.lineBackColor = COLOR_BACK;
	frame.lineWidth = 2;
	frame.titleOffsetDistance = 10;
	frame.titleLenth = 72;
	frame.pTitle = "限位";
	frame.titlePointColor = CL_GREY2;
	frame.titleBackPoint = COLOR_BACK;
	frame.fontSize = 24;
	
	GUI_DrawTitleFrame(&frame);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTest
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTest( void )
{
	/* 标题 */
	GUI_DrawGeneralTitleRectangleFrame(g_hardTest.pTitle);
	
	/* 配置通道区域坐标 */
	ConfigHardTestChannelRectangleFrameCoordinate();
	
	/* 通道区域矩形框 */
	GUI_HardTestChannelRectangleFrame();
	
	/* 通道区域菜单 */
	GUI_HardTestChannelOneLevelMenu();
	
	
	/* 配置限位区域坐标 */
	ConfigHardTestLimitParameterRectangleFrameCoordinate();
	
	/* 限位区域外边框 */
	GUI_HardTestLimitFrame();
	
	/* 限位区域矩形框 */
	GUI_HardTestLimitRectangleFrame();
	
	/* 限位区域菜单 */
	GUI_HardTestLimitOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : HardTestReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestReadParameter( void )
{
	uint8_t i,j;
	
	/* 通道区域初始化 */
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		for (j=0; j<g_hardTest.channelColumnNum; ++j)
		{
			strcpy(g_hardTest.parameterDataChannel[i][j].data,"--------");
		}
	}
	
	/* 限位区域初始化 */
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		strcpy(g_hardTest.parameterDataLimit[i],"--------");
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestChannelOneRowOneLevelMenuContent
 * Description    : 显示一行通道区域参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestChannelOneRowOneLevelMenuContent( uint8_t rowIndex, uint8_t columnIndex )
{
	const uint16_t x = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].x + \
						g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lineWidth + 1;
	const uint16_t y = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].y + \
						g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lineWidth + 1;
	const uint16_t pointColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.parameterDataChannel[rowIndex][columnIndex].data);
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestChannelOneLevelMenuContent
 * Description    : 显示通道区域一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestChannelOneLevelMenuContent( void )
{
	uint8_t i,j;
	
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		for (j=0; j<g_hardTest.channelColumnNum; ++j)
		{
			Show_HardTestChannelOneRowOneLevelMenuContent(i,j);
		}
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestLimitOneRowOneLevelMenuContent
 * Description    : 显示一行限位区域参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestLimitOneRowOneLevelMenuContent( uint8_t rowIndex )
{
	const uint16_t x = g_hardTest.oneLevelMenuLimit[rowIndex].x + \
						g_hardTest.oneLevelMenuLimit[rowIndex].lineWidth + 1;
	const uint16_t y = g_hardTest.oneLevelMenuLimit[rowIndex].y + \
						g_hardTest.oneLevelMenuLimit[rowIndex].lineWidth + 1;
	const uint16_t pointColor = g_hardTest.oneLevelMenuLimit[rowIndex].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuLimit[rowIndex].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.parameterDataLimit[rowIndex]);
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestLimitOneLevelMenuContent
 * Description    : 显示限位区域一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestLimitOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		Show_HardTestLimitOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_HardTest
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_HardTest( void )
{
	Show_HardTestChannelOneLevelMenuContent();
	
	Show_HardTestLimitOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : HardTestKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestKeyProcess( void )
{	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_FORCE_TARE:
				SendTareCmdInteractionTask(SMPL_FH_NUM);
				break;
			
			case KEY_DISPLACE_TARE:
				SendTareCmdInteractionTask(SMPL_WY_NUM);
				break;
			
			case KEY_DEFORMATE_TARE:
				SendTareCmdInteractionTask(SMPL_BX_NUM);
				break;
			
			case KEY_ESC:
				SetPage(SYSTEM_SET);
				g_hardTest.leavePage.flagLeavePage = SET;
				g_hardTest.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : HardTestLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestLeavePageCheckCycle( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	
	if (g_hardTest.leavePage.flagLeavePage == SET)
	{
		cmd_send_start_pkg(SEND_RCODE);
		
		cmdStatus = PCM_CmdSendCycle();
		
		if (cmdStatus != CMD_IDLE)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pHardTestCue[0]);
			
			PopWindowsProcessCycle();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SetHardTestChnLimit
 * Description    : 设置硬件测试限位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetHardTestChnLimit( uint8_t limit, FunctionalState NewFunc )
{
	uint8_t index = 0;
	
	switch ( limit )
	{
		case OBJECT_UP_LIMIT:
			index = GetHardTestLimitIndex(OBJECT_UP_LIMIT);
			if (index != 0xff)
			{
				if (NewFunc == ENABLE)
				{
					strcpy(g_hardTest.parameterDataLimit[index],"上限位  ");
				}
				else
				{
					strcpy(g_hardTest.parameterDataLimit[index],"--------");
				}
			}
			break;
		
		case OBJECT_DOWN_LIMIT:
			index = GetHardTestLimitIndex(OBJECT_DOWN_LIMIT);
			if (index != 0xff)
			{
				if (NewFunc == ENABLE)
				{
					strcpy(g_hardTest.parameterDataLimit[index],"下限位  ");
				}
				else
				{
					strcpy(g_hardTest.parameterDataLimit[index],"--------");
				}
			}
			break;
			
		case OBJECT_OIL_LIMIT:
			index = GetHardTestLimitIndex(OBJECT_OIL_LIMIT);
			if (index != 0xff)
			{
				if (NewFunc == ENABLE)
				{
					strcpy(g_hardTest.parameterDataLimit[index],"油缸限位");
				}
				else
				{
					strcpy(g_hardTest.parameterDataLimit[index],"--------");
				}
			}
			break;
	}
	
}

/*------------------------------------------------------------
 * Function Name  : GetInputDataFromPrm
 * Description    : 获取输入信号
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GetInputDataFromPrm( void )
{
	SAMPLE_C_TypeDef *pSample = GetSampleAddr();
	uint8_t stInput = pSample->input;
	uint8_t maskBit = 0x01;
	
	while ( maskBit )
	{
		if (stInput & (1 << BIT_UP_LIMIT))
		{
			SetHardTestChnLimit(OBJECT_UP_LIMIT,ENABLE);
		}
		else
		{
			SetHardTestChnLimit(OBJECT_UP_LIMIT,DISABLE);
		}
		
		if (stInput & (1 << BIT_DOWN_LIMIT))
		{
			SetHardTestChnLimit(OBJECT_DOWN_LIMIT,ENABLE);
		}
		else
		{
			SetHardTestChnLimit(OBJECT_DOWN_LIMIT,DISABLE);
		}
		
		if (stInput & (1 << BIT_OIL_LIMIT))
		{
			SetHardTestChnLimit(OBJECT_OIL_LIMIT,ENABLE);
		}
		else
		{
			SetHardTestChnLimit(OBJECT_OIL_LIMIT,DISABLE);
		}
		
		maskBit <<= 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetCodeFromPrm
 * Description    : 从下位机获取码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GetCodeFromPrm( CODE_TYPE_TypeDef type )
{
	uint8_t index = 0;
	char convBuff[MAX_PARAMETER_BIT+1];
	
	index = GetHardTestChannelIndex(OBJECT_FH_CHANNEL);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_BIT,GetSammpleCode(SMPL_FH_NUM),convBuff);
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[type][index].data,convBuff);
	}
	
	index = GetHardTestChannelIndex(OBJECT_WY_CHANNEL);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_BIT,GetSammpleCode(SMPL_WY_NUM),convBuff);
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[type][index].data,convBuff);
	}
	
	index = GetHardTestChannelIndex(OBJECT_BX_CHANNEL);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_BIT,GetSammpleCode(SMPL_BX_NUM),convBuff);
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[type][index].data,convBuff);
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshHardTest
 * Description    : 刷新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void RefreshHardTest( CODE_TYPE_TypeDef type )
{	
	uint8_t index = 0;
	
	index = GetHardTestChannelIndex(OBJECT_FH_CHANNEL);
	if (index != 0xff)
	{
		Show_HardTestChannelOneRowOneLevelMenuContent(type,index);
	}
	
	index = GetHardTestChannelIndex(OBJECT_WY_CHANNEL);
	if (index != 0xff)
	{
		Show_HardTestChannelOneRowOneLevelMenuContent(type,index);
	}
	
	index = GetHardTestChannelIndex(OBJECT_BX_CHANNEL);
	if (index != 0xff)
	{
		Show_HardTestChannelOneRowOneLevelMenuContent(type,index);
	}
	
	index = GetHardTestLimitIndex(OBJECT_UP_LIMIT);
	if (index != 0xff)
	{
		Show_HardTestLimitOneRowOneLevelMenuContent(index);
	}
	
	index = GetHardTestLimitIndex(OBJECT_DOWN_LIMIT);
	if (index != 0xff)
	{
		Show_HardTestLimitOneRowOneLevelMenuContent(index);
	}
	
	index = GetHardTestLimitIndex(OBJECT_OIL_LIMIT);
	if (index != 0xff)
	{
		Show_HardTestLimitOneRowOneLevelMenuContent(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : DataInteractionWithPRM
 * Description    : 与下位机数据交互
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DataInteractionWithPRM( void )
{
	static uint8_t sendType = 0;
	
	CMD_STATUS_TypeDef cmdStatus;
	
	cmdStatus = PCM_CmdSendCycle();
	
	switch ( g_hardTest.statusCmd )
	{
		case SEND_CMD_ENCHANGE:
			sendType ^= 0x01;
			if (sendType)
			{				
				ECHO(DEBUG_HARD_TEST,"发送绝对码\r\n");
				
				cmd_send_start_pkg(SEND_ACODE);
				g_hardTest.statusCmd = SEND_CMD_ABSOLUTE_WAIT;
			}
			else
			{				
				ECHO(DEBUG_HARD_TEST,"发送相对码\r\n");
				
				cmd_send_start_pkg(SEND_RCODE);	
				g_hardTest.statusCmd = SEND_CMD_RELATIVE_WAIT;
			}			
			break;
			
		case SEND_CMD_ABSOLUTE_WAIT:
			if (cmdStatus != CMD_IDLE)
			{
				return;
			}
			bsp_StartTimer(HARD_TEST_WAIT_TIMING,100);
			g_hardTest.statusCmd = SEND_CMD_ABSOLUTE;
			break;
			
		case SEND_CMD_ABSOLUTE:
			if ( bsp_CheckTimer(HARD_TEST_WAIT_TIMING) )
			{			
				ECHO(DEBUG_HARD_TEST,"显示绝对码\r\n");
				
				GetInputDataFromPrm();
				GetCodeFromPrm(ABSOLUTE_CODE);
				
				RefreshHardTest(ABSOLUTE_CODE);
				g_hardTest.statusCmd = SEND_CMD_ENCHANGE;
			}
			break;
		
		case SEND_CMD_RELATIVE_WAIT:
			if (cmdStatus != CMD_IDLE)
			{
				return;
			}
			bsp_StartTimer(HARD_TEST_WAIT_TIMING,100);
			g_hardTest.statusCmd = SEND_CMD_RELATIVE;
			break;
		
		case SEND_CMD_RELATIVE:
			if ( bsp_CheckTimer(HARD_TEST_WAIT_TIMING) )
			{			
				ECHO(DEBUG_HARD_TEST,"显示相对码\r\n");
				
				GetInputDataFromPrm();
				GetCodeFromPrm(OPPOSITE_CODE);
				
				RefreshHardTest(OPPOSITE_CODE);
				g_hardTest.statusCmd = SEND_CMD_ENCHANGE;
			}
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : SendTareCmdInteractionTask
 * Description    : 清零命令交互
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SendTareCmdInteractionTask( SMPL_NAME_TypeDef channel )
{
	SendChannelTareCmd(channel);
	
	g_hardTest.statusCmd = SEND_CMD_ENCHANGE;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
