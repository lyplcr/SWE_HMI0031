/**
  ******************************************************************************
  * @file    SystemSet.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-19 14:00:04
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   系统设置
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SystemSet.h"
#include "ChannelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define SYSTEM_SET_MENU_CNT				10

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_CONTROL_PARAMETER_SET = 0,
	OBJECT_TIME_DATE_SET,
	OBJECT_SOFT_ACTIVE,
	OBJECT_ONLINE_PARAMETER_SET,
	OBJECT_SYSTEM_PASSWORD_SET,
	OBJECT_FORCE_CALIBRATION,
	OBJECT_FORCE_VERIFICATION,
	OBJECT_TEST_AFTER_DISPOSE,
	OBJECT_SENIOR_PARAMETER_SET,
	OBJECT_HARD_TEST,
}OBJECT_SYSTEM_SET_TypeDef;

typedef enum
{
	INDEX_CONTROL_PARAMETER_SET = 0,
	INDEX_TIME_DATE_SET,
	INDEX_SOFT_ACTIVE,
	INDEX_ONLINE_PARAMETER_SET,
	INDEX_SYSTEM_PASSWORD_SET,
	INDEX_FORCE_CALIBRATION,
	INDEX_FORCE_VERIFICATION,
	INDEX_TEST_AFTER_DISPOSE,
	INDEX_SENIOR_PARAMETER_SET,
	INDEX_HARD_TEST,
}INDEX_SYSTEM_SET_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[SYSTEM_SET_MENU_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[SYSTEM_SET_MENU_CNT];
	uint8_t menuCnt;	
	uint8_t rowNum;
	const char *pParameterNameArray[SYSTEM_SET_MENU_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//索引值移动
	LEAVE_PAGE_TypeDef leavePage;						
}SYSTEM_SET_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSystemSetParameterName[] =
{
	"1、控制参数设置",
	"2、时间日期设置",
	"3、软件激活设置",
	"4、联机参数设置",
	"5、系统密码设置",
	"6、力值校准",
	"7、力值检定",
	"8、试验后处理",
	"9、高级参数",
	"0、硬件测试",
	"6、负荷校准",
	"7、负荷检定",
	"6、位移校准",
	"7、位移检定",
	"6、变形校准",
	"7、变形检定",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SYSTEM_SET_TypeDef g_systemSet;

/* Private function prototypes -----------------------------------------------*/
static void SystemSetInit( void );
static void SystemSetConfig( void );
static void GUI_SystemSet( void );
static void SystemSetMoveIndexProcess( void );
static void SystemSetMoveCursorProcess( void );
static void SystemSetKeyProcess( void );
static void SystemSetLeavePageCheckCycle( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadSystemSetPage
 * Description    : 系统设置页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemSetPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	SystemSetInit();
	
	/* 参数配置 */
	SystemSetConfig();
	
	/* 画GUI框架 */
	GUI_SystemSet();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_systemSet.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		SystemSetMoveIndexProcess();
		
		/* 移动光标 */
		SystemSetMoveCursorProcess();
		
		/* 按键处理 */
		SystemSetKeyProcess();
		
		/* 离开页 */
		SystemSetLeavePageCheckCycle();
	}
	
}

/*------------------------------------------------------------
 * Function Name  : SystemSetInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetInit( void )
{
	g_systemSet.recordIndex = 0xff;
	
	g_systemSet.isIndexMove = NO;
	
	g_systemSet.leavePage.flagLeavePage = RESET;
	g_systemSet.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : SystemSetConfig
 * Description    : 配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetConfig( void )
{
	/* 试块个数 */
	g_systemSet.menuCnt = SYSTEM_SET_MENU_CNT;
	
	/* 行数 */
	g_systemSet.rowNum = 5;
	
	/* 索引值 */		
	g_systemSet.indexArray[INDEX_CONTROL_PARAMETER_SET] = OBJECT_CONTROL_PARAMETER_SET;	
	g_systemSet.indexArray[INDEX_TIME_DATE_SET] 		= OBJECT_TIME_DATE_SET;		
	g_systemSet.indexArray[INDEX_SOFT_ACTIVE] 			= OBJECT_SOFT_ACTIVE;		
	g_systemSet.indexArray[INDEX_ONLINE_PARAMETER_SET] 	= OBJECT_ONLINE_PARAMETER_SET;			
	g_systemSet.indexArray[INDEX_SYSTEM_PASSWORD_SET] 	= OBJECT_SYSTEM_PASSWORD_SET;
	g_systemSet.indexArray[INDEX_FORCE_CALIBRATION] 	= OBJECT_FORCE_CALIBRATION;		
	g_systemSet.indexArray[INDEX_FORCE_VERIFICATION] 	= OBJECT_FORCE_VERIFICATION;	
	g_systemSet.indexArray[INDEX_TEST_AFTER_DISPOSE] 	= OBJECT_TEST_AFTER_DISPOSE;	
	g_systemSet.indexArray[INDEX_SENIOR_PARAMETER_SET]  = OBJECT_SENIOR_PARAMETER_SET;
	g_systemSet.indexArray[INDEX_HARD_TEST] 			= OBJECT_HARD_TEST;
	
	/* 名称 */
	g_systemSet.pParameterNameArray[INDEX_CONTROL_PARAMETER_SET]= pSystemSetParameterName[0];	
	g_systemSet.pParameterNameArray[INDEX_TIME_DATE_SET] 		= pSystemSetParameterName[1];		
	g_systemSet.pParameterNameArray[INDEX_SOFT_ACTIVE] 			= pSystemSetParameterName[2];		
	g_systemSet.pParameterNameArray[INDEX_ONLINE_PARAMETER_SET] = pSystemSetParameterName[3];			
	g_systemSet.pParameterNameArray[INDEX_SYSTEM_PASSWORD_SET] 	= pSystemSetParameterName[4];
	g_systemSet.pParameterNameArray[INDEX_FORCE_CALIBRATION] 	= pSystemSetParameterName[5];		
	g_systemSet.pParameterNameArray[INDEX_FORCE_VERIFICATION] 	= pSystemSetParameterName[6];	
	g_systemSet.pParameterNameArray[INDEX_TEST_AFTER_DISPOSE] 	= pSystemSetParameterName[7];	
	g_systemSet.pParameterNameArray[INDEX_SENIOR_PARAMETER_SET] = pSystemSetParameterName[8];
	g_systemSet.pParameterNameArray[INDEX_HARD_TEST] 			= pSystemSetParameterName[9];
	
	/* 标题 */
	g_systemSet.pTitle = "系统设置";
}

/*------------------------------------------------------------
 * Function Name  : ConfigSystemSetRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSystemSetRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 60;
	const uint16_t INIT_START_Y = 81;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_systemSet.menuCnt; ++i)
	{
		g_systemSet.oneLevelMenu[i].x = startX;
		g_systemSet.oneLevelMenu[i].y = startY;
		g_systemSet.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_systemSet.oneLevelMenu[i].backColor = COLOR_POINT;
		g_systemSet.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_systemSet.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_systemSet.oneLevelMenu[i].lenth = 320;
		g_systemSet.oneLevelMenu[i].width = 36;
		g_systemSet.oneLevelMenu[i].fontSize = 32;
		g_systemSet.oneLevelMenu[i].rowDistance = 37;
		g_systemSet.oneLevelMenu[i].columnDistance = 0;
		
		if (i == g_systemSet.rowNum - 1)
		{
			startX += g_systemSet.oneLevelMenu[i].lenth + 40;
			startY = INIT_START_Y;
		}
		else
		{
			startY += g_systemSet.oneLevelMenu[i].rowDistance + g_systemSet.oneLevelMenu[i].width;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemSetDrawOneRectangleFrame
 * Description    : 一行界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemSetDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_systemSet.oneLevelMenu[index].x;
	const uint16_t y = g_systemSet.oneLevelMenu[index].y;
	const uint16_t lenth = g_systemSet.oneLevelMenu[index].lenth;
	const uint16_t width = g_systemSet.oneLevelMenu[index].width;
	const uint16_t pointColor = g_systemSet.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemSet.oneLevelMenu[index].backColor;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr32At(x+16,y+1,pointColor,backColor,g_systemSet.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemSetRectangleFrame
 * Description    : 画界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemSetRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemSet.menuCnt; ++i)
	{
		GUI_SystemSetDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemSet
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemSet( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_systemSet.pTitle);
	
	ConfigSystemSetRectangleFrameCoordinate();
	
	GUI_SystemSetRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : SystemSetMoveIndexProcess
 * Description    : 移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_systemSet.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_systemSet.rowNum;
	indexObj.colNum = 2;
	indexObj.sumNum = g_systemSet.menuCnt;
	indexObj.pNowIndex = &g_systemSet.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_systemSet.nowIndex != g_systemSet.recordIndex)
	{		
		g_systemSet.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemSetMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetMoveCursorProcess( void )
{
	if (g_systemSet.isIndexMove == YES)
	{		
		if (g_systemSet.recordIndex != 0xff)
		{
			g_systemSet.oneLevelMenu[g_systemSet.recordIndex].pointColor = g_systemSet.oneLevelMenu[g_systemSet.recordIndex].recordPointColor;
			g_systemSet.oneLevelMenu[g_systemSet.recordIndex].backColor = g_systemSet.oneLevelMenu[g_systemSet.recordIndex].recordBackColor;
			
			GUI_SystemSetDrawOneRectangleFrame(g_systemSet.recordIndex);
			
			g_systemSet.oneLevelMenu[g_systemSet.nowIndex].pointColor = COLOR_POINT;
			g_systemSet.oneLevelMenu[g_systemSet.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_SystemSetDrawOneRectangleFrame(g_systemSet.nowIndex);
		}
		else
		{
			g_systemSet.oneLevelMenu[g_systemSet.nowIndex].pointColor = COLOR_POINT;
			g_systemSet.oneLevelMenu[g_systemSet.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_SystemSetDrawOneRectangleFrame(g_systemSet.nowIndex);
		}
		
		g_systemSet.recordIndex = g_systemSet.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemSetShortcutCheckCycle
 * Description    : 快捷键检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if (keyValue <= g_systemSet.menuCnt)
	{
		g_systemSet.leavePage.flagLeavePage = SET;
		g_systemSet.leavePage.flagSaveData = SET;
		
		if (keyValue)
		{
			g_systemSet.nowIndex = keyValue - 1;
		}
		else
		{
			g_systemSet.nowIndex = g_systemSet.menuCnt - 1;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemSetKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				g_systemSet.leavePage.flagLeavePage = SET;
				g_systemSet.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_systemSet.leavePage.flagLeavePage = SET;
				g_systemSet.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	SystemSetShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : SystemSetLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemSetLeavePageCheckCycle( void )
{
	if (g_systemSet.leavePage.flagLeavePage == SET)
	{
		if (g_systemSet.leavePage.flagSaveData == SET)
		{
			switch ( g_systemSet.indexArray[g_systemSet.nowIndex] )
			{
				case OBJECT_CONTROL_PARAMETER_SET:
					SetPage(CONTROL_PARAMETER);
					break;
				case OBJECT_TIME_DATE_SET:
					SetPage(DATE_TIME_PAGE);
					break;
				case OBJECT_SOFT_ACTIVE:
					SetPage(SOFT_ACTIVE_PAGE);
					break;
				case OBJECT_ONLINE_PARAMETER_SET:
					SetPage(ONLINE_PARAMETER_PAGE);
					break;
				case OBJECT_SYSTEM_PASSWORD_SET:
					SetPage(SYSTEM_PASSWORD_PAGE);
					break;
				case OBJECT_FORCE_CALIBRATION:
					SetChannelSelectNextPage(FORCE_CALIBRATION_PAGE);
					SetPage(CALIBRATION_PARAMETER);
					break;
				case OBJECT_FORCE_VERIFICATION:
					SetChannelSelectNextPage(FORCE_VERIFICATION_PAGE);
					SetPage(CALIBRATION_PARAMETER);
					break;
				case OBJECT_TEST_AFTER_DISPOSE:
					SetPage(TEST_AFTER_DISPOSE_PAGE);
					break;
				case OBJECT_SENIOR_PARAMETER_SET:
					SetPage(ADVANCE_PARAMETER_SET_PAGE);
					break;
				case OBJECT_HARD_TEST:
					SetPage(HARD_TEST_PAGE);
					break;
			}
		}
		else
		{
			LssuedParameterProcess();
			
			SetPage(MAIN_PAGE);
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
