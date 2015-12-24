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

#define MAX_PARAMETER_CNT				6
#define MAX_PARAMETER_BIT				8

#define	NO_LIMIT_TOUCH					0x0000			//无限位触发
#define	UP_LIMIT_TOUCH					0x0001			//上限位触发
#define	DOWN_LIMIT_TOUCH				0x0002			//下限位触发
#define	UP_AND_DOWN_TOUCH				0x0003			//上下限位同时触发

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	ABSOLUTE_CODE = 0,
	OPPOSITE_CODE,
}CODE_TYPE_TypeDef;

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
	UP_LIMIT = 0,
	DOWN_LIMIT,
}HARD_TEST_LIMIT_NAME_TypeDef;

typedef enum
{
	OBJECT_KY_ABSOLUTE_CODE = 0,
	OBJECT_KY_RELATIVE_CODE,
	OBJECT_UP_LIMIT,
	OBJECT_KZ_ABSOLUTE_CODE,
	OBJECT_KZ_RELATIVE_CODE,
	OBJECT_DOWN_LIMIT,
}OBJECT_HARD_TEST_TypeDef;

typedef enum
{
	INDEX_KY_ABSOLUTE_CODE = 0,
	INDEX_KY_RELATIVE_CODE,
	INDEX_UP_LIMIT,
	INDEX_KZ_ABSOLUTE_CODE,
	INDEX_KZ_RELATIVE_CODE,
	INDEX_DOWN_LIMIT,
}INDEX_HARD_TEST_TypeDef;

typedef struct
{
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	uint8_t rowNum;
	const char * pTitle;							//标题
	LEAVE_PAGE_TypeDef leavePage;					//离开页
	STATUS_CMD_ENCHANGE_TypeDef statusCmd;
}HARD_TEST_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pHardTestName[] =
{
	"抗压绝对码：",
	"抗压相对码：",
	"上限位：",
	"抗折绝对码：",
	"抗折相对码：",
	"下限位：",
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
static void SendTareCmdInteractionTask( void );

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
 * Function Name  : GetHardTestIndex
 * Description    : 获取参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetHardTestIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		if (g_hardTest.indexArray[i] == handle)
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
	/* 试块个数 */
	g_hardTest.curParameterNum = MAX_PARAMETER_CNT;
	g_hardTest.rowNum = 3;
	
	/* 索引值 */
	g_hardTest.indexArray[INDEX_KY_ABSOLUTE_CODE] 		= OBJECT_KY_ABSOLUTE_CODE;
	g_hardTest.indexArray[INDEX_KY_RELATIVE_CODE] 		= OBJECT_KY_RELATIVE_CODE;
	g_hardTest.indexArray[INDEX_UP_LIMIT] 				= OBJECT_UP_LIMIT;
	g_hardTest.indexArray[INDEX_KZ_ABSOLUTE_CODE] 		= OBJECT_KZ_ABSOLUTE_CODE;
	g_hardTest.indexArray[INDEX_KZ_RELATIVE_CODE] 		= OBJECT_KZ_RELATIVE_CODE;
	g_hardTest.indexArray[INDEX_DOWN_LIMIT] 			= OBJECT_DOWN_LIMIT;
	
	/* 参数名称 */
	g_hardTest.pParameterNameArray[INDEX_KY_ABSOLUTE_CODE] 	= pHardTestName[0];
	g_hardTest.pParameterNameArray[INDEX_KY_RELATIVE_CODE] 	= pHardTestName[1];
	g_hardTest.pParameterNameArray[INDEX_UP_LIMIT] 			= pHardTestName[2];
	g_hardTest.pParameterNameArray[INDEX_KZ_ABSOLUTE_CODE] 	= pHardTestName[3];
	g_hardTest.pParameterNameArray[INDEX_KZ_RELATIVE_CODE] 	= pHardTestName[4];
	g_hardTest.pParameterNameArray[INDEX_DOWN_LIMIT] 		= pHardTestName[5];
	
	/* 标题 */
	g_hardTest.pTitle = "硬件测试";
}

/*------------------------------------------------------------
 * Function Name  : ConfigHardTestParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigHardTestParameterRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 238;
	const uint16_t INIT_START_Y = 175;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		g_hardTest.oneLevelMenu[i].x = startX;
		g_hardTest.oneLevelMenu[i].y = startY;
		g_hardTest.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_hardTest.oneLevelMenu[i].backColor = COLOR_BACK;
		g_hardTest.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_hardTest.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_hardTest.oneLevelMenu[i].lenth = 102;
		g_hardTest.oneLevelMenu[i].width = 30;
		g_hardTest.oneLevelMenu[i].fontSize = 24;
		g_hardTest.oneLevelMenu[i].rowDistance = 24;
		g_hardTest.oneLevelMenu[i].columnDistance = 0;
		g_hardTest.oneLevelMenu[i].lineWidth = 2;
		
		if (i == g_hardTest.rowNum - 1)
		{
			startX += 360;
			startY = INIT_START_Y;
		}
		else
		{
			startY += g_hardTest.oneLevelMenu[i].rowDistance + g_hardTest.oneLevelMenu[i].width;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_hardTest.oneLevelMenu[index].x;
	rectangle.y = g_hardTest.oneLevelMenu[index].y;
	rectangle.lenth = g_hardTest.oneLevelMenu[index].lenth;
	rectangle.width = g_hardTest.oneLevelMenu[index].width;
	rectangle.lineWidth = g_hardTest.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_hardTest.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		GUI_HardTestDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawOneRowOneLevelMenu
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_hardTest.oneLevelMenu[index].x - 6 * g_hardTest.oneLevelMenu[index].fontSize;
	const uint16_t y = g_hardTest.oneLevelMenu[index].y + g_hardTest.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_hardTest.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_hardTest.oneLevelMenu[index].lenth - 2 * g_hardTest.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_hardTest.oneLevelMenu[index].width - 2 * g_hardTest.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		GUI_HardTestDrawOneRowOneLevelMenu(i);
	}	
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
	GUI_DrawGeneralTitleRectangleFrame(g_hardTest.pTitle);
	
	ConfigHardTestParameterRectangleFrameCoordinate();
	
	GUI_HardTestRectangleFrame();
	
	GUI_HardTestOneLevelMenu();
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
	uint8_t i;

	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		strcpy(g_hardTest.parameterData[i],"--------");
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_hardTest.oneLevelMenu[index].x + g_hardTest.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_hardTest.oneLevelMenu[index].y + g_hardTest.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_hardTest.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_HardTestOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_hardTest.curParameterNum; ++i)
	{
		Show_HardTestOneRowOneLevelMenuContent(i);
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
	Show_HardTestOneLevelMenuContent();
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
				SendTareCmdInteractionTask();
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
		case UP_LIMIT:
			index = GetHardTestIndex(OBJECT_UP_LIMIT);
			if (NewFunc == ENABLE)
			{
				strcpy(g_hardTest.parameterData[index],"上限位  ");
			}
			else
			{
				strcpy(g_hardTest.parameterData[index],"--------");
			}
			break;
		
		case DOWN_LIMIT:
			index = GetHardTestIndex(OBJECT_DOWN_LIMIT);
			if (NewFunc == ENABLE)
			{
				strcpy(g_hardTest.parameterData[index],"下限位  ");
			}
			else
			{
				strcpy(g_hardTest.parameterData[index],"--------");
			}
			break;
	}
	
}

/*------------------------------------------------------------
 * Function Name  : GetInputDataFromPrm
 * Description    : 从下位机获取输入信号
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GetInputDataFromPrm( void )
{
	SAMPLE_C_TypeDef *pSample = GetSampleAddr();
	
	switch ( pSample->input )
	{
		case NO_LIMIT_TOUCH:
			SetHardTestChnLimit(UP_LIMIT,DISABLE);
			SetHardTestChnLimit(DOWN_LIMIT,DISABLE);
			break;
		
		case UP_LIMIT_TOUCH:
			SetHardTestChnLimit(UP_LIMIT,ENABLE);
			SetHardTestChnLimit(DOWN_LIMIT,DISABLE);
			break;
		
		case DOWN_LIMIT_TOUCH:
			SetHardTestChnLimit(UP_LIMIT,DISABLE);
			SetHardTestChnLimit(DOWN_LIMIT,ENABLE);
			break;
		
		case UP_AND_DOWN_TOUCH:
			SetHardTestChnLimit(UP_LIMIT,ENABLE);
			SetHardTestChnLimit(DOWN_LIMIT,ENABLE);
			break;
		
		default:			
			break;
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
	SAMPLE_C_TypeDef *pSample = GetSampleAddr();
	uint8_t index = 0;
	char convBuff[MAX_PARAMETER_BIT+1];
	
	switch ( type )
	{
		case ABSOLUTE_CODE:
			index = GetHardTestIndex(OBJECT_KY_ABSOLUTE_CODE);
			if (index != 0xff)
			{
				numtochar(MAX_PARAMETER_BIT,pSample->smpl[SMPL_KY_NUM],convBuff);
				CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterData[index],convBuff);
			}
			
			index = GetHardTestIndex(OBJECT_KZ_ABSOLUTE_CODE);
			if (index != 0xff)
			{
				numtochar(MAX_PARAMETER_BIT,pSample->smpl[SMPL_KZ_NUM],convBuff);
				CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterData[index],convBuff);
			}
			break;
		
		case OPPOSITE_CODE:
			index = GetHardTestIndex(OBJECT_KY_RELATIVE_CODE);
			if (index != 0xff)
			{
				numtochar(MAX_PARAMETER_BIT,pSample->smpl[SMPL_KY_NUM],convBuff);
				CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterData[index],convBuff);
			}
			
			index = GetHardTestIndex(OBJECT_KZ_RELATIVE_CODE);
			if (index != 0xff)
			{
				numtochar(MAX_PARAMETER_BIT,pSample->smpl[SMPL_KZ_NUM],convBuff);
				CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterData[index],convBuff);
			}
			break;
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
	
	switch ( type )
	{
		case ABSOLUTE_CODE:
			index = GetHardTestIndex(OBJECT_KY_ABSOLUTE_CODE);
			if (index != 0xff)
			{
				Show_HardTestOneRowOneLevelMenuContent(index);
			}
			
			index = GetHardTestIndex(OBJECT_KZ_ABSOLUTE_CODE);
			if (index != 0xff)
			{
				Show_HardTestOneRowOneLevelMenuContent(index);
			}
			break;
		
		case OPPOSITE_CODE:
			index = GetHardTestIndex(OBJECT_KY_RELATIVE_CODE);
			if (index != 0xff)
			{
				Show_HardTestOneRowOneLevelMenuContent(index);
			}
			
			index = GetHardTestIndex(OBJECT_KZ_RELATIVE_CODE);
			if (index != 0xff)
			{
				Show_HardTestOneRowOneLevelMenuContent(index);
			}
			break;
	}
	
	index = GetHardTestIndex(OBJECT_UP_LIMIT);
	if (index != 0xff)
	{
		Show_HardTestOneRowOneLevelMenuContent(index);
	}
	
	index = GetHardTestIndex(OBJECT_DOWN_LIMIT);
	if (index != 0xff)
	{
		Show_HardTestOneRowOneLevelMenuContent(index);
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
				#ifdef DEBUG_HARD_TEST
					printf("发送绝对码\r\n");
				#endif
				cmd_send_start_pkg(SEND_ACODE);
				g_hardTest.statusCmd = SEND_CMD_ABSOLUTE_WAIT;
			}
			else
			{
				#ifdef DEBUG_HARD_TEST
					printf("发送相对码\r\n");
				#endif
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
				#ifdef DEBUG_HARD_TEST
					printf("显示绝对码\r\n");
				#endif
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
				#ifdef DEBUG_HARD_TEST
					printf("显示相对码\r\n");
				#endif
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
static void SendTareCmdInteractionTask( void )
{
	SendChannelTareCmd(SMPL_KY_NUM);
	SendChannelTareCmd(SMPL_KZ_NUM);
	
	g_hardTest.statusCmd = SEND_CMD_ENCHANGE;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
