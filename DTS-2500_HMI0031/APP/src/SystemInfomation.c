/**
  ******************************************************************************
  * @file    SystemInfomation.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-24 15:53:57
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   系统信息
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SystemInfomation.h"
#include "ModelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				8
#define MAX_PARAMETER_BIT				16

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_SYSTEM_MODEL = 0,
	OBJECT_OBJECT_ADAPTATION_MODEL,
	OBJECT_DEVICE_SERIAL,
	OBJECT_UPPER_MONITOR_SOFT_VERSION,
	OBJECT_UPPER_MONITOR_PCB_VERSION,
	OBJECT_LOWER_MONITOR_SOFT_VERSION,
	OBJECT_LOWER_MONITOR_PCB_VERSION,
	OBJECT_SD_CAPACITY,
}OBJECT_SYSTEM_INFOMATION_TypeDef;

typedef enum
{
	INDEX_SYSTEM_MODEL = 0,
	INDEX_OBJECT_ADAPTATION_MODEL,
	INDEX_DEVICE_SERIAL,
	INDEX_UPPER_MONITOR_SOFT_VERSION,
	INDEX_UPPER_MONITOR_PCB_VERSION,
	INDEX_LOWER_MONITOR_SOFT_VERSION,
	INDEX_LOWER_MONITOR_PCB_VERSION,
	INDEX_SD_CAPACITY,
}INDEX_SYSTEM_INFOMATION_TypeDef;

typedef struct
{
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//参数个数
	const char * pTitle;							//标题
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}SYSTEM_INFOMATION_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSystemInfomationName[] =
{
	"系统型号：",
	"适配机型：",
	"设备序列号：",
	"上位机固件版本：",
	"上位机PCB版本：",
	"下位机固件版本：",
	"下位机PCB版本：",
	"SD卡剩余容量(MB)：",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SYSTEM_INFOMATION_TypeDef g_systemInfomation;

/* Private function prototypes -----------------------------------------------*/
static void SystemInfomationInit( void );
static void SystemInfomationConfig( void );
static void GUI_SystemInfomation( void );
static void SystemInfomationReadParameter( void );
static void Traverse_SystemInfomation( void );
static void SystemInfomationKeyProcess( void );
static void SystemInfomationLeavePageCheckCycle( void );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadSystemInfomationPage
 * Description    : 系统信息页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemInfomationPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	SystemInfomationInit();
	
	/* 参数配置 */
	SystemInfomationConfig();
	
	/* 获取参数 */
	SystemInfomationReadParameter();
	
	/* 画GUI框架 */
	GUI_SystemInfomation();
	
	/* 遍历 */
	Traverse_SystemInfomation();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_systemInfomation.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 按键处理 */
		SystemInfomationKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		SystemInfomationLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemInfomationInit
 * Description    : 控制参数初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemInfomationInit( void )
{	
	g_systemInfomation.leavePage.flagLeavePage = RESET;
	g_systemInfomation.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : GetSystemInfomationIndex
 * Description    : 获取控制参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetSystemInfomationIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
	for (i=0; i<g_systemInfomation.curParameterNum; ++i)
	{
		if (g_systemInfomation.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : SystemInfomationConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemInfomationConfig( void )
{
	/* 试块个数 */
	g_systemInfomation.curParameterNum = MAX_PARAMETER_CNT;
	
	/* 索引值 */
	g_systemInfomation.indexArray[INDEX_SYSTEM_MODEL] 					= OBJECT_SYSTEM_MODEL;
	g_systemInfomation.indexArray[INDEX_OBJECT_ADAPTATION_MODEL] 		= OBJECT_OBJECT_ADAPTATION_MODEL;
	g_systemInfomation.indexArray[INDEX_DEVICE_SERIAL] 					= OBJECT_DEVICE_SERIAL;
	g_systemInfomation.indexArray[INDEX_UPPER_MONITOR_SOFT_VERSION] 	= OBJECT_UPPER_MONITOR_SOFT_VERSION;
	g_systemInfomation.indexArray[INDEX_UPPER_MONITOR_PCB_VERSION] 		= OBJECT_UPPER_MONITOR_PCB_VERSION;
	g_systemInfomation.indexArray[INDEX_LOWER_MONITOR_SOFT_VERSION] 	= OBJECT_LOWER_MONITOR_SOFT_VERSION;
	g_systemInfomation.indexArray[INDEX_LOWER_MONITOR_PCB_VERSION] 		= OBJECT_LOWER_MONITOR_PCB_VERSION;
	g_systemInfomation.indexArray[INDEX_SD_CAPACITY] 					= OBJECT_SD_CAPACITY;
	
	/* 参数名称 */
	g_systemInfomation.pParameterNameArray[INDEX_SYSTEM_MODEL] 					= pSystemInfomationName[0];
	g_systemInfomation.pParameterNameArray[INDEX_OBJECT_ADAPTATION_MODEL] 		= pSystemInfomationName[1];
	g_systemInfomation.pParameterNameArray[INDEX_DEVICE_SERIAL] 				= pSystemInfomationName[2];
	g_systemInfomation.pParameterNameArray[INDEX_UPPER_MONITOR_SOFT_VERSION] 	= pSystemInfomationName[3];
	g_systemInfomation.pParameterNameArray[INDEX_UPPER_MONITOR_PCB_VERSION] 	= pSystemInfomationName[4];
	g_systemInfomation.pParameterNameArray[INDEX_LOWER_MONITOR_SOFT_VERSION] 	= pSystemInfomationName[5];
	g_systemInfomation.pParameterNameArray[INDEX_LOWER_MONITOR_PCB_VERSION] 	= pSystemInfomationName[6];
	g_systemInfomation.pParameterNameArray[INDEX_SD_CAPACITY] 					= pSystemInfomationName[7];
	
	/* 标题 */
	g_systemInfomation.pTitle = "系统信息";
}

/*------------------------------------------------------------
 * Function Name  : ConfigSystemInfomationParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSystemInfomationParameterRectangleFrameCoordinate( void )
{
	uint16_t startX = 330;
	uint16_t startY = 74;
	uint8_t i;
	
	for (i=0; i<g_systemInfomation.curParameterNum; ++i)
	{
		g_systemInfomation.oneLevelMenu[i].x = startX;
		g_systemInfomation.oneLevelMenu[i].y = startY;
		g_systemInfomation.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_systemInfomation.oneLevelMenu[i].backColor = COLOR_BACK;
		g_systemInfomation.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_systemInfomation.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_systemInfomation.oneLevelMenu[i].lenth = 198;
		g_systemInfomation.oneLevelMenu[i].width = 30;
		g_systemInfomation.oneLevelMenu[i].fontSize = 24;
		g_systemInfomation.oneLevelMenu[i].rowDistance = 10;
		g_systemInfomation.oneLevelMenu[i].columnDistance = 0;
		g_systemInfomation.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_systemInfomation.oneLevelMenu[i].width + g_systemInfomation.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemInfomationDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemInfomationDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_systemInfomation.oneLevelMenu[index].x;
	rectangle.y = g_systemInfomation.oneLevelMenu[index].y;
	rectangle.lenth = g_systemInfomation.oneLevelMenu[index].lenth;
	rectangle.width = g_systemInfomation.oneLevelMenu[index].width;
	rectangle.lineWidth = g_systemInfomation.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_systemInfomation.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemInfomationRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemInfomationRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemInfomation.curParameterNum; ++i)
	{
		GUI_SystemInfomationDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemInfomationDrawOneRowOneLevelMenu
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemInfomationDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_systemInfomation.oneLevelMenu[index].x - 9 * 24;
	const uint16_t y = g_systemInfomation.oneLevelMenu[index].y + g_systemInfomation.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemInfomation.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemInfomation.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemInfomation.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemInfomationOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemInfomationOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemInfomation.curParameterNum; ++i)
	{
		GUI_SystemInfomationDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemInfomation
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemInfomation( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_systemInfomation.pTitle);
	
	ConfigSystemInfomationParameterRectangleFrameCoordinate();
	
	GUI_SystemInfomationRectangleFrame();
	
	GUI_SystemInfomationOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : SystemInfomationReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemInfomationReadParameter( void )
{
	uint8_t index = 0;
	uint8_t Model = 0;
	uint32_t ID = 0;
	char ID_buff[9] = {0};
	float SD = 0;
	char SD_buff[10];	
	CMD_STATUS_TypeDef cmdStatus;
	PRM_REPLY_TypeDef *pPRM = GetPrmReplyAddr();
	uint8_t replyStatus;
	
	cmd_get_device_version_pkg();
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	index = GetSystemInfomationIndex(OBJECT_SYSTEM_MODEL);
	if (index != 0xff)
	{
		strcpy(g_systemInfomation.parameterData[index],SYSTEM_MODEL);
	}
	
	index = GetSystemInfomationIndex(OBJECT_OBJECT_ADAPTATION_MODEL);
	if (index != 0xff)
	{
		Model = devc_token_get();
	
		switch ( Model )
		{
			case MODEL_KY_DZ:
				strcpy(g_systemInfomation.parameterData[index],"电子式抗压机");
				break;
			case MODEL_KZ_DZ:
				strcpy(g_systemInfomation.parameterData[index],"电动抗折机");
				break;
			case MODEL_KZKY_DZ:
				strcpy(g_systemInfomation.parameterData[index],"电子式抗折抗压机");
				break;
			case MODEL_KY_YY:
				strcpy(g_systemInfomation.parameterData[index],"液压式抗压机");
				break;
			case MODEL_KZKY_YY:
				strcpy(g_systemInfomation.parameterData[index],"液压式抗折抗压机");
				break;	
			default:
				strcpy(g_systemInfomation.parameterData[index],"未知机型");
				break;
		}
	}
	
	index = GetSystemInfomationIndex(OBJECT_DEVICE_SERIAL);
	if (index != 0xff)
	{
		ID = GetDeviceID();	
		hex2str_32(ID,ID_buff);
		
		strcpy(g_systemInfomation.parameterData[index],ID_buff);
	}
	
	index = GetSystemInfomationIndex(OBJECT_UPPER_MONITOR_SOFT_VERSION);
	if (index != 0xff)
	{
		strcpy(g_systemInfomation.parameterData[index],SOFT_VERSION);
	}
	
	index = GetSystemInfomationIndex(OBJECT_UPPER_MONITOR_PCB_VERSION);
	if (index != 0xff)
	{
		strcpy(g_systemInfomation.parameterData[index],PCB_VERSION);
	}
	
	index = GetSystemInfomationIndex(OBJECT_LOWER_MONITOR_SOFT_VERSION);
	if (index != 0xff)
	{
		if ((cmdStatus==CMD_IDLE) && (replyStatus==STATUS_OK))
		{
			hex2str_16(pPRM->fw_ver,g_systemInfomation.parameterData[index]);		
		}
		else
		{
			strcpy(g_systemInfomation.parameterData[index],"----");
		}
	}
	
	index = GetSystemInfomationIndex(OBJECT_LOWER_MONITOR_PCB_VERSION);
	if (index != 0xff)
	{
		if (cmdStatus == CMD_IDLE)
		{
			strcpy(g_systemInfomation.parameterData[index],pPRM->pcb_info);		
		}
		else
		{
			strcpy(g_systemInfomation.parameterData[index],"----");
		}
	}
	
	index = GetSystemInfomationIndex(OBJECT_SD_CAPACITY);
	if (index != 0xff)
	{
		get_free_space(&SD);
		floattochar(MAX_PARAMETER_BIT,1,SD,SD_buff);
		strcpy(g_systemInfomation.parameterData[index],SD_buff);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemInfomationOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemInfomationOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_systemInfomation.oneLevelMenu[index].x + g_systemInfomation.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_systemInfomation.oneLevelMenu[index].y + g_systemInfomation.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemInfomation.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemInfomation.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_systemInfomation.oneLevelMenu[index].lenth - 2 * g_systemInfomation.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_systemInfomation.oneLevelMenu[index].width - 2 * g_systemInfomation.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemInfomation.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemInfomationOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemInfomationOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemInfomation.curParameterNum; ++i)
	{
		Show_SystemInfomationOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_SystemInfomation
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_SystemInfomation( void )
{
	Show_SystemInfomationOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : SystemInfomationKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemInfomationKeyProcess( void )
{	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ESC:
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				g_systemInfomation.leavePage.flagLeavePage = SET;
				g_systemInfomation.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemInfomationLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemInfomationLeavePageCheckCycle( void )
{
	if (g_systemInfomation.leavePage.flagLeavePage == SET)
	{
		if (g_systemInfomation.leavePage.flagSaveData == SET)
		{
			;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
