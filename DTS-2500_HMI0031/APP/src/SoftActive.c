/**
  ******************************************************************************
  * @file    SoftActive.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-20 16:03:12
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   软件激活模块
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SoftActive.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				4
#define MAX_PARAMETER_PUTIN_BIT			8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_FACTORY_SERIAL = 0,
	OBJECT_ACTIVE_PASSWORD,
	OBJECT_CURRENT_DATE,
	OBJECT_ACTIVE_DATE,
}OBJECT_SOFT_ACTIVE_TypeDef;

typedef enum
{
	INDEX_FACTORY_SERIAL = 0,
	INDEX_ACTIVE_PASSWORD,
	INDEX_CURRENT_DATE,
	INDEX_ACTIVE_DATE,
}INDEX_SOFT_ACTIVE_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	BoolStatus isIndexMove;							//索引值移动
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}SOFT_ACTIVE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSoftActiveName[] =
{
	"产品编号：",
	"激活密码：",
	"当前日期：",
	"激活日期：",
};

const char * const ActiveStatusWarn[] = 
{
	"设备激活时间到期!",			//0
	"设备激活时间快到期!",		//1
	"当前日期小于上次使用日期!",	//2				
	"激活日期错误！",			//3
	"激活天数错误！",			//4
	"产品编号错误！",			//5
	"产品激活成功！",			//6
	"激活密码错误！",			//7
	"激活密码过期！",			//8
	"产品编号设置错误！",		//9
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SOFT_ACTIVE_TypeDef g_softActive;

/* Private function prototypes -----------------------------------------------*/
static void SoftActiveInit( void );
static void SoftActiveParameterConfig( void );
static void SoftActiveReadParameter( void );
static void GUI_SoftActive( void );
static void Traverse_SoftActive( void );
static void SoftActivePutinProcess( void );
static void SoftActiveShortcutCycleTask( void );
static void SoftActiveKeyProcess( void );
static void SoftActiveLeavePageCheckCycle( void );
static void SoftActiveMoveCursorProcess( void );
static void SoftActiveMoveIndexProcess( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadTimeDatePage
 * Description    : 时间日期页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSoftActivePage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	SoftActiveInit();
	
	/* 参数配置 */
	SoftActiveParameterConfig();
	
	/* 获取参数 */
	SoftActiveReadParameter();
	
	/* 画GUI框架 */
	GUI_SoftActive();
	
	/* 遍历 */
	Traverse_SoftActive();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_softActive.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		SoftActivePutinProcess();
		
		/* 移动索引值 */
		SoftActiveMoveIndexProcess();	
		
		/* 快捷菜单 */
		SoftActiveShortcutCycleTask();
		
		/* 移动光标 */
		SoftActiveMoveCursorProcess();
		
		/* 按键处理 */
		SoftActiveKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		SoftActiveLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveInit
 * Description    : 软件激活初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveInit( void )
{
	g_softActive.isIndexMove = NO;	
	g_softActive.recordIndex = 0xff;
	g_softActive.refreshShortcut = ENABLE;
	g_softActive.leavePage.flagLeavePage = RESET;
	g_softActive.leavePage.flagSaveData = RESET;
	
	/* 检测激活状态 */
	{
		int32_t remainDay = 0;
		EXPIRE_STATUS_TypeDef activeStatus;
		
		activeStatus = JudgeActiveStatus(&remainDay);
		
		switch ( activeStatus )
		{
			case EXPIRE_NONE:
				ECHO(DEBUG_ACTIVE,"已激活！\r\n");
				break;
			case EXPIRE_DATE_ERR:	
				ECHO(DEBUG_ACTIVE,"时间设置错误！\r\n");
				break; 
			case EXPIRE_NEAR:
				ECHO(DEBUG_ACTIVE,"激活将要到期！\r\n");
				break;
			case EXPIRE_ACTIVE_DATE_ERR:	
				ECHO(DEBUG_ACTIVE,"激活日期错误！\r\n");
				break; 
			case EXPIRE_ACTIVE_DAY_ERR:		
				ECHO(DEBUG_ACTIVE,"激活时间错误！\r\n");
				break; 
			case EXPIRE:
				ECHO(DEBUG_ACTIVE,"激活已到期！\r\n");
				break; 
			default:
				ECHO(DEBUG_ACTIVE,"未知状态！\r\n");
				break;
		}
		
		ECHO(DEBUG_ACTIVE,"激活剩余天数：%d\r\n",remainDay);
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveParameterConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveParameterConfig( void )
{
	/* 个数 */
	g_softActive.curParameterNum = 2;
	
	/* 索引值 */
	g_softActive.indexArray[INDEX_FACTORY_SERIAL] 		= OBJECT_FACTORY_SERIAL;
	g_softActive.indexArray[INDEX_ACTIVE_PASSWORD] 		= OBJECT_ACTIVE_PASSWORD;
	g_softActive.indexArray[INDEX_CURRENT_DATE] 		= OBJECT_CURRENT_DATE;
	g_softActive.indexArray[INDEX_ACTIVE_DATE] 			= OBJECT_ACTIVE_DATE;
	
	/* 参数名称 */
	g_softActive.pParameterNameArray[INDEX_FACTORY_SERIAL] 		= pSoftActiveName[0];
	g_softActive.pParameterNameArray[INDEX_ACTIVE_PASSWORD] 	= pSoftActiveName[1];
	g_softActive.pParameterNameArray[INDEX_CURRENT_DATE] 		= pSoftActiveName[2];
	g_softActive.pParameterNameArray[INDEX_ACTIVE_DATE] 		= pSoftActiveName[3];
	
	/* 标题 */
	g_softActive.pTitle = "软件激活设置";
	
	/* 数据保存类型 */
	g_softActive.oneLevelMenu[INDEX_FACTORY_SERIAL].saveType 		= TYPE_CHAR;
	g_softActive.oneLevelMenu[INDEX_ACTIVE_PASSWORD].saveType 		= TYPE_CHAR;
	g_softActive.oneLevelMenu[INDEX_CURRENT_DATE].saveType			= TYPE_INT;
	g_softActive.oneLevelMenu[INDEX_ACTIVE_DATE].saveType 			= TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : GetSoftActiveIndex
 * Description    : 获取索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetSoftActiveIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		if (g_softActive.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveReadParameter( void )
{
	uint32_t ID = 0;
	char ID_buff[9] = {0};
	uint8_t index = 0;
	
	index = GetSoftActiveIndex(OBJECT_FACTORY_SERIAL);
	if (index != 0xff)
	{
		ErrorStatus errStatus = dvc_product_id_get(&ID,ID_buff);
		
		if (errStatus == SUCCESS)
		{
			strcpy(g_softActive.parameterData[index],ID_buff);
		}
		else
		{
			strcpy(g_softActive.parameterData[index],"--------");
		}
	}
	
	index = GetSoftActiveIndex(OBJECT_ACTIVE_PASSWORD);
	if (index != 0xff)
	{
		g_softActive.parameterData[index][0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigSoftActiveParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSoftActiveParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 120;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		g_softActive.oneLevelMenu[i].x = startX;
		g_softActive.oneLevelMenu[i].y = startY;
		g_softActive.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_softActive.oneLevelMenu[i].backColor = COLOR_BACK;
		g_softActive.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_softActive.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_softActive.oneLevelMenu[i].lenth = 102;
		g_softActive.oneLevelMenu[i].width = 30;
		g_softActive.oneLevelMenu[i].fontSize = 24;
		g_softActive.oneLevelMenu[i].rowDistance = 24;
		g_softActive.oneLevelMenu[i].columnDistance = 0;
		g_softActive.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_softActive.oneLevelMenu[i].width + g_softActive.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_softActive.oneLevelMenu[index].x;
	rectangle.y = g_softActive.oneLevelMenu[index].y;
	rectangle.lenth = g_softActive.oneLevelMenu[index].lenth;
	rectangle.width = g_softActive.oneLevelMenu[index].width;
	rectangle.lineWidth = g_softActive.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_softActive.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_softActive.curParameterNum; ++i)
	{
		GUI_SoftActiveDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveDrawOneRowOneLevelMenu
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_softActive.oneLevelMenu[index].x - 5 * 24;
	const uint16_t y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_softActive.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_softActive.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_softActive.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		GUI_SoftActiveDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveTimeCue
 * Description    : 时间提示信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveTimeCue( void )
{	
	uint8_t index = 0;
	uint16_t x,y,pointColor,backColor;
	
	index = GetSoftActiveIndex(OBJECT_CURRENT_DATE);
	if (index != 0xff)
	{
		x = g_softActive.oneLevelMenu[index].x + g_softActive.oneLevelMenu[index].lineWidth + 1;
		y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
		pointColor = g_softActive.oneLevelMenu[index].pointColor;
		backColor = g_softActive.oneLevelMenu[index].backColor;
		
		GUI_ShowSystemDate(x,y,pointColor,backColor);
	}	
	
	index = GetSoftActiveIndex(OBJECT_ACTIVE_DATE);
	if (index != 0xff)
	{
		x = g_softActive.oneLevelMenu[index].x + g_softActive.oneLevelMenu[index].lineWidth + 1;
		y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
		pointColor = g_softActive.oneLevelMenu[index].pointColor;
		backColor = g_softActive.oneLevelMenu[index].backColor;
		
		GUI_ShowActiveDate(x,y,pointColor,backColor);
	}
}
#if 0
/*------------------------------------------------------------
 * Function Name  : GUI_SoftActiveProductSerialCue
 * Description    : 产品编号提示信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActiveProductSerialCue( void )
{
	uint16_t x,y,pointColor,backColor;
	uint8_t index = 0;
	
	index = GetSoftActiveIndex(OBJECT_FACTORY_SERIAL);
	if (index != 0xff)
	{
		x = g_softActive.oneLevelMenu[index].x + g_softActive.oneLevelMenu[index].lineWidth + 1;
		y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
		pointColor = COLOR_FORBIT_EDIT_POINT;//g_softActive.oneLevelMenu[index].pointColor;
		backColor = g_softActive.oneLevelMenu[index].backColor;
		GUI_DispStr24At(x,y,pointColor,backColor,g_softActive.parameterData[index]);
	}	
}
#endif	
/*------------------------------------------------------------
 * Function Name  : GUI_SoftActive
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SoftActive( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_softActive.pTitle);
	
	ConfigSoftActiveParameterRectangleFrameCoordinate();
	
	GUI_SoftActiveRectangleFrame();
	
	GUI_SoftActiveOneLevelMenu();
	
	#if 0
		GUI_SoftActiveProductSerialCue();
	#endif
	
	GUI_SoftActiveTimeCue();	
}

/*------------------------------------------------------------
 * Function Name  : Show_SoftActiveOneRowOneLevelMenuContent
 * Description    : 显示一行参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SoftActiveOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_softActive.oneLevelMenu[index].x + g_softActive.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_softActive.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_softActive.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_softActive.oneLevelMenu[index].lenth - 2 * g_softActive.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_softActive.oneLevelMenu[index].width - 2 * g_softActive.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_softActive.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Traverse_SoftActive
 * Description    : 遍历软件激活
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_SoftActive( void )
{
	uint8_t index = 0;
	
	index = GetSoftActiveIndex(OBJECT_FACTORY_SERIAL);
	if (index != 0xff)
	{
		Show_SoftActiveOneRowOneLevelMenuContent(index);
	}
	
	index = GetSoftActiveIndex(OBJECT_ACTIVE_PASSWORD);
	if (index != 0xff)
	{
		Show_SoftActiveOneRowOneLevelMenuContent(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActivePutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActivePutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_softActive.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_softActive.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_softActive.oneLevelMenu[index].x + g_softActive.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_softActive.oneLevelMenu[index].y + g_softActive.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_softActive.oneLevelMenu[index].lenth - 2 * g_softActive.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_softActive.oneLevelMenu[index].width - 2 * g_softActive.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_softActive.putinNum;
	pPutin->SaveType = g_softActive.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_softActive.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_softActive.refreshShortcut == ENABLE)
	{
		g_softActive.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[40];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveMoveIndexProcess
 * Description    : 参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveMoveIndexProcess( void )
{		
	INDEX_MANAGE_TypeDef indexObj;
	
	g_softActive.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_softActive.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_softActive.curParameterNum;
	indexObj.pNowIndex = &g_softActive.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_softActive.nowIndex != g_softActive.recordIndex)
	{		
		g_softActive.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveMoveCursorProcess( void )
{
	if (g_softActive.isIndexMove == YES)
	{		
		if (g_softActive.recordIndex != 0xff)
		{
			g_softActive.oneLevelMenu[g_softActive.recordIndex].pointColor = g_softActive.oneLevelMenu[g_softActive.recordIndex].recordPointColor;
			g_softActive.oneLevelMenu[g_softActive.recordIndex].backColor = g_softActive.oneLevelMenu[g_softActive.recordIndex].recordBackColor;
			
			Show_SoftActiveOneRowOneLevelMenuContent(g_softActive.recordIndex);		
			
			g_softActive.oneLevelMenu[g_softActive.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_softActive.oneLevelMenu[g_softActive.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SoftActiveOneRowOneLevelMenuContent(g_softActive.nowIndex);	
		}
		else
		{
			g_softActive.oneLevelMenu[g_softActive.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_softActive.oneLevelMenu[g_softActive.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SoftActiveOneRowOneLevelMenuContent(g_softActive.nowIndex);
		}
		
		g_softActive.recordIndex = g_softActive.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveUpdateIndex( void )
{
	g_softActive.nowIndex++;
	
	g_softActive.nowIndex %= g_softActive.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveUpdateStatus( void )
{
	SoftActiveUpdateIndex();
	
	g_softActive.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveActiveProcess
 * Description    : 激活处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SoftActiveActiveProcess( const char *password )
{
	tTime t;
	ACTIVE_STATUS_TypeDef activeStatus = ACTIVE_PASS_EXPIRE;
	
	time_cycle();
	t = get_time();	 
	
	activeStatus = active_product(t,password);
	
	switch ( activeStatus )
	{
		case ACTIVE_OK:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[6]);
			return PASSED;
		
		case ACTIVE_ID_ERR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[5]);
			break;
		
		case ACTIVE_PASS_ERR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[7]);
			break;
		
		case ACTIVE_PASS_EXPIRE:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[8]);
			break;
		
		default:
			break;
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveKeyProcess( void )
{
	uint8_t index = g_softActive.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
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
						switch ( g_softActive.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_softActive.putinNum,*GetPutinIntDataAddr(),g_softActive.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_softActive.putinNum,g_softActive.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_softActive.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_softActive.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}
						
						{
							index = GetSoftActiveIndex(OBJECT_FACTORY_SERIAL);
							if (index != 0xff)
							{
								ErrorStatus errStatus = dvc_product_id_set(g_softActive.parameterData[index]);
								
								if (errStatus == ERROR)
								{
									SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[9]);
									
									g_softActive.leavePage.flagLeavePage = SET;
									g_softActive.leavePage.flagSaveData = RESET;
								}
							}
						}
						break;						
					
					default:
						break;
				}
				
				SoftActiveUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_softActive.leavePage.flagLeavePage = SET;
						g_softActive.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						SoftActiveUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
				
			case KEY_F3:
				index = GetSoftActiveIndex(OBJECT_ACTIVE_PASSWORD);
				if (index != 0xff)
				{							
					SoftActiveActiveProcess(g_softActive.parameterData[index]);
					g_softActive.leavePage.flagLeavePage = SET;
					g_softActive.leavePage.flagSaveData = SET;	
				}
				break;
			
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_softActive.leavePage.flagLeavePage = SET;
				g_softActive.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SoftActiveLeavePageCheckCycle( void )
{
	if (g_softActive.leavePage.flagLeavePage == SET)
	{
		if (g_softActive.leavePage.flagSaveData == SET)
		{
			return;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : JudgeActiveStatus
 * Description    : 判断激活状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
EXPIRE_STATUS_TypeDef JudgeActiveStatus( int32_t *day )
{
	tTime t = {0};
	int32_t actDay = 0;
	EXPIRE_STATUS_TypeDef status = EXPIRE;

	time_cycle();		  //判断激活状态
	t = get_time();	
	
	status = expire_judge(t,&actDay);	

	*day = actDay;
	
	return status;
}

/*------------------------------------------------------------
 * Function Name  : ActiveStatusHandler
 * Description    : 激活状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ActiveStatusHandler( EXPIRE_STATUS_TypeDef status )
{
	switch ( status )
	{
		case EXPIRE:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[0]);
			break;
		
		case EXPIRE_NEAR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[1]);
			break;
		
		case EXPIRE_DATE_ERR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[2]);
			break;
		
		case EXPIRE_ACTIVE_DATE_ERR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[3]);
			break;
		
		case EXPIRE_ACTIVE_DAY_ERR:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&ActiveStatusWarn[4]);
			break;
		
		default:
			break;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
