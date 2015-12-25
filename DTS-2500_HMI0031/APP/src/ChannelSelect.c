/**
  ******************************************************************************
  * @file    ChannelSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-22 09:37:14
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   通道选择
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "ChannelSelect.h"
#include "ModelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define CHANNEL_SELECT_MENU_CNT			3

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	INDEX_FH_CNANNEL = 0,
	INDEX_WY_CNANNEL,
	INDEX_BX_CNANNEL,
}INDEX_CHANNEL_SELECT_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[CHANNEL_SELECT_MENU_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[CHANNEL_SELECT_MENU_CNT];
	uint8_t menuCnt;	
	const char *pParameterNameArray[CHANNEL_SELECT_MENU_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//索引值移动
	LEAVE_PAGE_TypeDef leavePage;			
	PAGE_NAME_TypeDef sourcePage;					//源页
	PAGE_NAME_TypeDef targetPage;					//目标页
	PAGE_NAME_TypeDef nextPage;						//下一页
	SMPL_NAME_TypeDef selChannel;
}CHANNEL_SELECT_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pChannelSelectParameterName[] =
{
	"负荷通道",
	"位移通道",
	"变形通道",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static CHANNEL_SELECT_TypeDef g_channelSelect;

/* Private function prototypes -----------------------------------------------*/
static void LoadChannelSelectPage( void );
static void ChannelSelectInit( void );
static void ChannelSelectConfig( void );
static void GUI_ChannelSelect( void );
static void ChannelSelectMoveIndexProcess( void );
static void ChannelSelectMoveCursorProcess( void );
static void ChannelSelectKeyProcess( void );
static void ChannelSelectLeavePageCheckCycle( void );
static void ChannelSelectReadParameter( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : PrestrainLoadChannelSelectPage
 * Description    : 预加载通道选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PrestrainLoadChannelSelectPage( void )
{
	MODEL_TYPE_TypeDef modelType;
	
	modelType = GetModelType();
		
	switch ( modelType )
	{
		case MODEL_KY:
			g_channelSelect.selChannel = SMPL_FH_NUM;
			SetPage(g_channelSelect.targetPage);
			
			return;
		case MODEL_KZ:
			g_channelSelect.selChannel = SMPL_FH_NUM;
			SetPage(g_channelSelect.targetPage);
			
			return;
		case MODEL_KZKY:
			LoadChannelSelectPage();
			break;	
		
		case MODEL_UNIVERSAL:
			LoadChannelSelectPage();
			break;
	}
}	

/*------------------------------------------------------------
 * Function Name  : LoadChannelSelectPage
 * Description    : 通道选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadChannelSelectPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	ChannelSelectInit();
	
	/* 参数配置 */
	ChannelSelectConfig();
	
	/* 获取参数 */
	ChannelSelectReadParameter();
	
	/* 画GUI框架 */
	GUI_ChannelSelect();
	
	/* 遍历 */
	ChannelSelectMoveIndexProcess();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_channelSelect.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		ChannelSelectMoveIndexProcess();
		
		/* 移动光标 */
		ChannelSelectMoveCursorProcess();
		
		/* 按键处理 */
		ChannelSelectKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ChannelSelectLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SetChannelSelectSourcePage
 * Description    : 设置通道选择来源页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetChannelSelectSourcePage( PAGE_NAME_TypeDef sourcePage )
{
	g_channelSelect.sourcePage = sourcePage;
}

/*------------------------------------------------------------
 * Function Name  : SetChannelSelectTargetPage
 * Description    : 设置通道选择目标页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetChannelSelectTargetPage( PAGE_NAME_TypeDef targetPage )
{
	g_channelSelect.targetPage = targetPage;
}

/*------------------------------------------------------------
 * Function Name  : SetChannelSelectNextPage
 * Description    : 设置通道选择下一页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetChannelSelectNextPage( PAGE_NAME_TypeDef nextPage )
{
	g_channelSelect.nextPage = nextPage;
}

/*------------------------------------------------------------
 * Function Name  : GetChannelSelectNextPage
 * Description    : 获取通道选择下一页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
PAGE_NAME_TypeDef GetChannelSelectNextPage( void )
{
	return g_channelSelect.nextPage;
}

/*------------------------------------------------------------
 * Function Name  : GetChannelSelectChannel
 * Description    : 获取通道选择通道
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SMPL_NAME_TypeDef GetChannelSelectChannel( void )
{
	return g_channelSelect.selChannel;
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectInit( void )
{
	g_channelSelect.recordIndex = 0xff;
	
	g_channelSelect.isIndexMove = NO;
	
	g_channelSelect.leavePage.flagLeavePage = RESET;
	g_channelSelect.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectConfig
 * Description    : 配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectConfig( void )
{
	/* 标题 */
	g_channelSelect.pTitle = "通道选择";
	
	/* 试块个数 */
	g_channelSelect.menuCnt = CHANNEL_SELECT_MENU_CNT;
	
	/* 索引值 */		
	g_channelSelect.indexArray[INDEX_FH_CNANNEL] = SMPL_FH_NUM;	
	g_channelSelect.indexArray[INDEX_WY_CNANNEL] = SMPL_WY_NUM;	
	g_channelSelect.indexArray[INDEX_BX_CNANNEL] = SMPL_BX_NUM;
	
	/* 名称 */
	g_channelSelect.pParameterNameArray[INDEX_FH_CNANNEL]	= pChannelSelectParameterName[0];	
	g_channelSelect.pParameterNameArray[INDEX_WY_CNANNEL] 	= pChannelSelectParameterName[1];	
	g_channelSelect.pParameterNameArray[INDEX_BX_CNANNEL] 	= pChannelSelectParameterName[2];
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectIndex
 * Description    : 获取通道选择索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetChannelSelectIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_channelSelect.menuCnt; ++i)
	{
		if (g_channelSelect.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectReadParameter( void )
{	
	uint8_t index = 0;
	
	index = GetChannelSelectIndex(g_channelSelect.selChannel);
	if (index != 0xff)
	{
		g_channelSelect.nowIndex = index;
	}
}	

/*------------------------------------------------------------
 * Function Name  : ConfigChannelSelectRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigChannelSelectRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 240;
	const uint16_t INIT_START_Y = 105;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_channelSelect.menuCnt; ++i)
	{
		g_channelSelect.oneLevelMenu[i].x = startX;
		g_channelSelect.oneLevelMenu[i].y = startY;
		g_channelSelect.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_channelSelect.oneLevelMenu[i].backColor = COLOR_POINT;
		g_channelSelect.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_channelSelect.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_channelSelect.oneLevelMenu[i].lenth = 320;
		g_channelSelect.oneLevelMenu[i].width = 64;
		g_channelSelect.oneLevelMenu[i].fontSize = 32;
		g_channelSelect.oneLevelMenu[i].rowDistance = 40;
		g_channelSelect.oneLevelMenu[i].columnDistance = 0;
		
		startY += g_channelSelect.oneLevelMenu[i].rowDistance + g_channelSelect.oneLevelMenu[i].width;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ChannelSelectDrawOneRectangleFrame
 * Description    : 一行界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ChannelSelectDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_channelSelect.oneLevelMenu[index].x;
	const uint16_t y = g_channelSelect.oneLevelMenu[index].y;
	const uint16_t lenth = g_channelSelect.oneLevelMenu[index].lenth;
	const uint16_t width = g_channelSelect.oneLevelMenu[index].width;
	const uint16_t pointColor = g_channelSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_channelSelect.oneLevelMenu[index].backColor;
	uint16_t fontLenth = strlen(g_channelSelect.pParameterNameArray[index]);
	uint8_t fontSize = g_channelSelect.oneLevelMenu[index].fontSize;
	uint16_t xOffset = 0;
	uint16_t yOffset = 0;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	xOffset = (lenth - fontLenth * (fontSize >> 1)) >> 1;
	yOffset = (width - fontSize) >> 1;
	
	GUI_DispStr32At(x+xOffset,y+yOffset,pointColor,backColor,g_channelSelect.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ChannelSelectRectangleFrame
 * Description    : 画界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ChannelSelectRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_channelSelect.menuCnt; ++i)
	{
		GUI_ChannelSelectDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ChannelSelect
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ChannelSelect( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_channelSelect.pTitle);
	
	ConfigChannelSelectRectangleFrameCoordinate();
	
	GUI_ChannelSelectRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectMoveIndexProcess
 * Description    : 移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_channelSelect.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_channelSelect.menuCnt;
	indexObj.colNum = 1;
	indexObj.sumNum = indexObj.rowNum * indexObj.colNum;
	indexObj.pNowIndex = &g_channelSelect.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_channelSelect.nowIndex != g_channelSelect.recordIndex)
	{		
		g_channelSelect.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectMoveCursorProcess( void )
{
	if (g_channelSelect.isIndexMove == YES)
	{		
		if (g_channelSelect.recordIndex != 0xff)
		{
			g_channelSelect.oneLevelMenu[g_channelSelect.recordIndex].pointColor = g_channelSelect.oneLevelMenu[g_channelSelect.recordIndex].recordPointColor;
			g_channelSelect.oneLevelMenu[g_channelSelect.recordIndex].backColor = g_channelSelect.oneLevelMenu[g_channelSelect.recordIndex].recordBackColor;
			
			GUI_ChannelSelectDrawOneRectangleFrame(g_channelSelect.recordIndex);
			
			g_channelSelect.oneLevelMenu[g_channelSelect.nowIndex].pointColor = COLOR_POINT;
			g_channelSelect.oneLevelMenu[g_channelSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_ChannelSelectDrawOneRectangleFrame(g_channelSelect.nowIndex);
		}
		else
		{
			g_channelSelect.oneLevelMenu[g_channelSelect.nowIndex].pointColor = COLOR_POINT;
			g_channelSelect.oneLevelMenu[g_channelSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_ChannelSelectDrawOneRectangleFrame(g_channelSelect.nowIndex);
		}
		
		g_channelSelect.recordIndex = g_channelSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectShortcutCheckCycle
 * Description    : 快捷键检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue > 0) && (keyValue <= g_channelSelect.menuCnt))
	{
		g_channelSelect.leavePage.flagLeavePage = SET;
		g_channelSelect.leavePage.flagSaveData = SET;
		
		g_channelSelect.nowIndex = keyValue - 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				g_channelSelect.leavePage.flagLeavePage = SET;
				g_channelSelect.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_channelSelect.leavePage.flagLeavePage = SET;
				g_channelSelect.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	ChannelSelectShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : ChannelSelectLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ChannelSelectLeavePageCheckCycle( void )
{
	if (g_channelSelect.leavePage.flagLeavePage == SET)
	{
		if (g_channelSelect.leavePage.flagSaveData == SET)
		{
			switch ( g_channelSelect.indexArray[g_channelSelect.nowIndex] )
			{
				case SMPL_FH_NUM:
					g_channelSelect.selChannel = SMPL_FH_NUM;
					SetPage(g_channelSelect.targetPage);
					break;
				case SMPL_WY_NUM:
					g_channelSelect.selChannel = SMPL_WY_NUM;
					SetPage(g_channelSelect.targetPage);
					break;
				case SMPL_BX_NUM:
					g_channelSelect.selChannel = SMPL_BX_NUM;
					SetPage(g_channelSelect.targetPage);
					break;
			}
		}
		else
		{
			SetPage(g_channelSelect.sourcePage);
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
