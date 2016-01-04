/**
  ******************************************************************************
  * @file    HardTest.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-25 08:33:03
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   Ӳ������
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

/* ͨ������ */
#define MAX_CHANNEL_ROW_CNT				2
#define MAX_CHANNEL_COLUMN_NUM			3

/* ��λ���� */
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
	/* ͨ���� */
	CHANNEL_DATA_TypeDef parameterDataChannel[MAX_CHANNEL_ROW_CNT][MAX_CHANNEL_COLUMN_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuChannel[MAX_CHANNEL_ROW_CNT][MAX_CHANNEL_COLUMN_NUM];
	const char *pParameterNameArrayChannel[MAX_CHANNEL_COLUMN_NUM];
	uint8_t indexArrayChannel[MAX_CHANNEL_COLUMN_NUM];
	uint8_t channelRowNum;				
	uint8_t channelColumnNum;
	
	/* ��λ�� */
	char parameterDataLimit[MAX_LIMIT_ROW_CNT][MAX_PARAMETER_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenuLimit[MAX_LIMIT_ROW_CNT];
	const char *pParameterNameArrayLimit[MAX_LIMIT_ROW_CNT];
	uint8_t indexArrayLimit[MAX_LIMIT_ROW_CNT];
	uint8_t limitRowNum;				
	uint8_t limitColumnNum;
	
	const char * pTitle;							//����
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
	STATUS_CMD_ENCHANGE_TypeDef statusCmd;
}HARD_TEST_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pHardTestName[] =
{
	"����ͨ��",		//0
	"λ��ͨ��",		//1
	"����ͨ��",		//2
	"�����룺",		//3
	"����룺",		//4
	"����λ��",		//5
	"����λ��",		//6
	"�͸���λ��",	//7
	"�ͱã�",		//8
	"��ͣ��ť��",	//9
};

const char * const pHardTestCue[] = 
{
	"����ʵʱ��������ʧ�ܣ�",		//0
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
 * Description    : Ӳ������ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadHardTestPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	HardTestInit();
	
	/* �������� */
	HardTestConfig();
	
	/* ��ȡ���� */
	HardTestReadParameter();
	
	/* ��GUI��� */
	GUI_HardTest();
	
	/* ���� */
	Traverse_HardTest();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_hardTest.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ���ݽ��� */
		DataInteractionWithPRM();
		
		/* �������� */
		HardTestKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		HardTestLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : HardTestInit
 * Description    : ��ʼ��
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
 * Description    : ��ȡͨ����������
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetHardTestChannelIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��ȡ��λ��������
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetHardTestLimitIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
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
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestConfig( void )
{
	/* ���� */
	g_hardTest.pTitle = "Ӳ������";
	
	/* �Կ���� */
	g_hardTest.channelRowNum = MAX_CHANNEL_ROW_CNT;
	g_hardTest.channelColumnNum = MAX_CHANNEL_COLUMN_NUM;
	
	/* ����ֵ */
	g_hardTest.indexArrayChannel[INDEX_FH_CHANNEL] 		= OBJECT_FH_CHANNEL;
	g_hardTest.indexArrayChannel[INDEX_WY_CHANNEL] 		= OBJECT_WY_CHANNEL;
	g_hardTest.indexArrayChannel[INDEX_BX_CHANNEL] 		= OBJECT_BX_CHANNEL;
	
	/* �������� */
	g_hardTest.pParameterNameArrayChannel[INDEX_FH_CHANNEL] 	= pHardTestName[0];
	g_hardTest.pParameterNameArrayChannel[INDEX_WY_CHANNEL] 	= pHardTestName[1];
	g_hardTest.pParameterNameArrayChannel[INDEX_BX_CHANNEL] 	= pHardTestName[2];
	
	/* �Կ���� */
	g_hardTest.channelRowNum = MAX_LIMIT_ROW_CNT;
	g_hardTest.channelColumnNum = 1;
	
	/* ����ֵ */
	g_hardTest.indexArrayChannel[INDEX_UP_LIMIT] 		= OBJECT_UP_LIMIT;
	g_hardTest.indexArrayChannel[INDEX_DOWN_LIMIT] 		= OBJECT_DOWN_LIMIT;
	g_hardTest.indexArrayChannel[INDEX_OIL_LIMIT] 		= OBJECT_OIL_LIMIT;
	
	/* �������� */
	g_hardTest.pParameterNameArrayChannel[INDEX_UP_LIMIT] 		= pHardTestName[5];
	g_hardTest.pParameterNameArrayChannel[INDEX_DOWN_LIMIT] 	= pHardTestName[6];
	g_hardTest.pParameterNameArrayChannel[INDEX_OIL_LIMIT] 		= pHardTestName[7];
}

/*------------------------------------------------------------
 * Function Name  : ConfigHardTestChannelRectangleFrameCoordinate
 * Description    : ���ý���ͨ��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigHardTestChannelRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 238;
	const uint16_t INIT_START_Y = 175;
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
		startY = g_hardTest.oneLevelMenuChannel[i][j].width + \
					g_hardTest.oneLevelMenuChannel[i][j].rowDistance - \
					g_hardTest.oneLevelMenuChannel[i][j].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestDrawOneChannelRectangleFrame
 * Description    : ��һ��ͨ��GUI���ο�
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
 * Description    : ͨ������GUI
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
 * Description    : ͨ�������Ϸ�һ���˵�����
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
 * Description    : ͨ��������һ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawChannelLeftOneRowOneLevelMenu( uint8_t rowIndex, uint8_t columnIndex )
{
	const uint16_t x = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].x - \
						4 * g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].fontSize;
	const uint16_t y = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].y + \
						g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].lineWidth;
	const uint16_t pointColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuChannel[rowIndex][columnIndex].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArrayChannel[columnIndex]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestChannelOneLevelMenu
 * Description    : ͨ������һ���˵�����
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
 * Description    : ������λ����GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigHardTestLimitParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 400;
	const uint16_t START_Y = 175;
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
 * Description    : ��һ����λGUI���ο�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawOneLimitRectangleFrame( index )
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
 * Description    : ��λ����GUI
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
 * Description    : ��λ����һ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTestDrawLimitOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_hardTest.oneLevelMenuLimit[index].x;
	const uint16_t y = g_hardTest.oneLevelMenuLimit[index].y - 40;
	const uint16_t pointColor = g_hardTest.oneLevelMenuLimit[index].pointColor;
	const uint16_t backColor = g_hardTest.oneLevelMenuLimit[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_hardTest.pParameterNameArrayLimit[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_HardTestChannelOneLevelMenu
 * Description    : ͨ������һ���˵�����
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
 * Function Name  : GUI_HardTest
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_HardTest( void )
{
	/* ���� */
	GUI_DrawGeneralTitleRectangleFrame(g_hardTest.pTitle);
	
	/* ����ͨ���������� */
	ConfigHardTestChannelRectangleFrameCoordinate();
	
	/* ͨ��������ο� */
	GUI_HardTestChannelRectangleFrame();
	
	/* ͨ������˵� */
	GUI_HardTestChannelOneLevelMenu();
	
	
	/* ������λ�������� */
	ConfigHardTestLimitParameterRectangleFrameCoordinate();
	
	/* ��λ������ο� */
	GUI_HardTestLimitRectangleFrame();
	
	/* ��λ����˵� */
	GUI_HardTestLimitOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : HardTestReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HardTestReadParameter( void )
{
	uint8_t i,j;
	
	/* ͨ�������ʼ�� */
	for (i=0; i<g_hardTest.channelRowNum; ++i)
	{
		for (j=0; j<g_hardTest.channelColumnNum; ++j)
		{
			strcpy(g_hardTest.parameterDataChannel[i][j].data,"--------");
		}
	}
	
	/* ��λ�����ʼ�� */
	for (i=0; i<g_hardTest.limitRowNum; ++i)
	{
		strcpy(g_hardTest.parameterDataLimit[i],"--------");
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_HardTestChannelOneRowOneLevelMenuContent
 * Description    : ��ʾһ��ͨ���������
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
 * Description    : ��ʾͨ������һ���˵�����
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
 * Description    : ��ʾһ����λ�������
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
 * Description    : ��ʾ��λ����һ���˵�����
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
 * Description    : ����
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
 * Description    : ��������
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
 * Description    : �뿪ҳ���
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
 * Description    : ����Ӳ��������λ
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
					strcpy(g_hardTest.parameterDataLimit[index],"����λ  ");
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
					strcpy(g_hardTest.parameterDataLimit[index],"����λ  ");
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
					strcpy(g_hardTest.parameterDataLimit[index],"�͸���λ");
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
 * Description    : ��ȡ�����ź�
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
		if (maskBit & BIT_UP_LIMIT)
		{
			SetHardTestChnLimit(OBJECT_UP_LIMIT,ENABLE);
		}
		else
		{
			SetHardTestChnLimit(OBJECT_UP_LIMIT,DISABLE);
		}
		
		if (maskBit & BIT_DOWN_LIMIT)
		{
			SetHardTestChnLimit(OBJECT_DOWN_LIMIT,ENABLE);
		}
		else
		{
			SetHardTestChnLimit(OBJECT_DOWN_LIMIT,DISABLE);
		}
		
		if (maskBit & BIT_OIL_LIMIT)
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
 * Description    : ����λ����ȡ��ֵ
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
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[index][type].data,convBuff);
	}
	
	index = GetHardTestChannelIndex(OBJECT_WY_CHANNEL);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_BIT,GetSammpleCode(SMPL_WY_NUM),convBuff);
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[index][type].data,convBuff);
	}
	
	index = GetHardTestChannelIndex(OBJECT_BX_CHANNEL);
	if (index != 0xff)
	{
		numtochar(MAX_PARAMETER_BIT,GetSammpleCode(SMPL_BX_NUM),convBuff);
		CharToChar(MAX_PARAMETER_BIT,g_hardTest.parameterDataChannel[index][type].data,convBuff);
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshHardTestChannelArea
 * Description    : ˢ��ͨ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void RefreshHardTestChannelArea( CODE_TYPE_TypeDef type )
{	
	uint8_t index = 0;
	
	switch ( type )
	{
		case ABSOLUTE_CODE:
			index = GetHardTestChannelIndex(OBJECT_FH_CHANNEL);
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
 * Description    : ����λ�����ݽ���
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
					printf("���;�����\r\n");
				#endif
				cmd_send_start_pkg(SEND_ACODE);
				g_hardTest.statusCmd = SEND_CMD_ABSOLUTE_WAIT;
			}
			else
			{
				#ifdef DEBUG_HARD_TEST
					printf("���������\r\n");
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
					printf("��ʾ������\r\n");
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
					printf("��ʾ�����\r\n");
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
 * Description    : ���������
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
