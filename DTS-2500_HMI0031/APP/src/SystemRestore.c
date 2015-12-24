/**
  ******************************************************************************
  * @file    SystemRestore.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-24 08:50:03
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ϵͳ��ԭ
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SystemRestore.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_NUM				4
#define	MAX_PUTIN_BIT					8

#define INDEX_USER_NAME					0

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	TEST_USER = 0,
	MANAGE_USER,
}USER_NAME_TypeDef;

typedef enum
{
	INDEX_SYSTEM_PASSWORD = 1,
	INDEX_SYSTEM_TIME_USER,
}INDEX_USER_RESTORE_TypeDef;

typedef enum
{
	INDEX_RESTORE_DATE = 1,
	INDEX_MANAGE_PASSWORD,
	INDEX_SYSTEM_TIME_MANAGE,
}INDEX_MANAGE_RESTORE_TypeDef;

typedef enum
{
	OBJECT_USER_NAME = 0,
	OBJECT_SYSTEM_PASSWORD,
	OBJECT_MANAGE_PASSWORD,
	OBJECT_RESTORE_DATE,
	OBJECT_SYSTEM_DATE,
}OBJECT_RESTORE_NAME_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_NUM][MAX_PUTIN_BIT+1];	
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_NUM];
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_PARAMETER_NUM];
	const char *pParameterNameArray[MAX_PARAMETER_NUM];
	uint8_t indexArray[MAX_PARAMETER_NUM];
	uint8_t curParameterNum;						//��������
	uint8_t allowMoveNum;							//�����ƶ�����
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	FunctionalState enableMoveIndex;				//ʹ���ƶ�����ֵ
	FunctionalState enablePutin;					//ʹ������
	FunctionalState enableArrow;					//ʹ�ܼ�ͷ
	uint8_t putinNum;								//�����ַ�����
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ	
	FunctionalState encryption;						//����
}SYSTEM_RESTORE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSystemRestoreUserName[] = 
{
	"����Ա",
	"����Ա",	
};

const char * const pSystemRestoreParameterName[] = 
{
	"�û�����",
	"����Ա���룺",
	"ϵͳ���ڣ�",
	"��ԭ���ڣ�",
	"����Ա���룺",
};

const char * const pSystemRestoreCue[] = 
{
	"��ԭ���ڱ�������8λ��",		//0
	"����Ա�����������",		//1
	"����Ա�����������",		//2
	"�����U�̣�",				//3
	"���ѳɹ���ԭϵͳ��",		//4
	"����ȡ����ԭϵͳ��",		//5
	"��ԭϵͳʧ�ܣ�",			//6
	"ϵͳ��������������",		//7
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SYSTEM_RESTORE_TypeDef g_systemRestore;

/* Private function prototypes -----------------------------------------------*/
static void SystemRestoreInit( void );
static void SystemRestoreConfig( void );
static void SystemRestoreReadParameter( void );
static void GUI_SystemRestore( void );
static void Traverse_SystemRestore( void );
static void SystemRestorePutinProcess( void );
static void SystemRestoreMoveIndexProcess( void );
static void SystemRestoreStatusProcess( void );
static void SystemRestoreMoveCursorProcess( void );
static void SystemRestoreShortcutCycleTask( void );
static void SystemRestoreKeyProcess( void );
static void SystemRestoreLeavePageCheckCycle( void );
static USER_NAME_TypeDef GetSystemRestoreUserName( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadSystemRestorePage
 * Description    : ϵͳ��ԭҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemRestorePage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	SystemRestoreInit();
	
	/* �������� */
	SystemRestoreConfig();
	
	/* ��ȡ���� */
	SystemRestoreReadParameter();
	
	/* ��GUI��� */
	GUI_SystemRestore();
	
	/* ���� */
	Traverse_SystemRestore();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_systemRestore.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		SystemRestorePutinProcess();
		
		/* �ƶ�����ֵ */
		SystemRestoreMoveIndexProcess();
		
		/* ״̬���� */
		SystemRestoreStatusProcess();
		
		/* �ƶ���� */
		SystemRestoreMoveCursorProcess();
		
		/* ��ݲ˵� */
		SystemRestoreShortcutCycleTask();
		
		/* �������� */
		SystemRestoreKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		SystemRestoreLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreInit( void )
{	
	g_systemRestore.isIndexMove = NO;		
	g_systemRestore.enableMoveIndex = ENABLE;
	g_systemRestore.enablePutin = DISABLE;
	g_systemRestore.enableArrow = DISABLE;
	g_systemRestore.recordIndex = 0xff;
	g_systemRestore.refreshShortcut = ENABLE;
	g_systemRestore.leavePage.flagLeavePage = RESET;
	g_systemRestore.leavePage.flagSaveData = RESET;
}


/*------------------------------------------------------------
 * Function Name  : GetSystemRestoreIndex
 * Description    : ��ȡ��ʵ������
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetSystemRestoreIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_systemRestore.curParameterNum; ++i)
	{
		if (g_systemRestore.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreConfig
 * Description    : ������Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreConfig( void )
{
	uint8_t index = 0;
	
	index = GetSystemRestoreIndex(OBJECT_USER_NAME);
	if (index == 0xff)
	{
		index = INDEX_USER_NAME;
		
		g_systemRestore.twoLevelMenu[index].index = TEST_USER;
	}
	
	if (g_systemRestore.twoLevelMenu[index].index == TEST_USER)
	{
		/* �Կ���� */
		g_systemRestore.curParameterNum = 3;
		g_systemRestore.allowMoveNum = 2;
		
		/* ����ֵ */
		g_systemRestore.indexArray[INDEX_USER_NAME] 		= OBJECT_USER_NAME;		
		g_systemRestore.indexArray[INDEX_SYSTEM_PASSWORD] 	= OBJECT_SYSTEM_PASSWORD;		
		g_systemRestore.indexArray[INDEX_SYSTEM_TIME_USER] 	= OBJECT_SYSTEM_DATE;		
		
		/* �������� */
		g_systemRestore.pParameterNameArray[INDEX_USER_NAME] 		= pSystemRestoreParameterName[0];
		g_systemRestore.pParameterNameArray[INDEX_SYSTEM_PASSWORD] 	= pSystemRestoreParameterName[1];
		g_systemRestore.pParameterNameArray[INDEX_SYSTEM_TIME_USER] = pSystemRestoreParameterName[2];
	}
	else
	{
		/* ���� */
		g_systemRestore.curParameterNum = 4;
		g_systemRestore.allowMoveNum = 3;
		
		/* ����ֵ */
		g_systemRestore.indexArray[INDEX_USER_NAME] 			= OBJECT_USER_NAME;		
		g_systemRestore.indexArray[INDEX_RESTORE_DATE] 			= OBJECT_RESTORE_DATE;		
		g_systemRestore.indexArray[INDEX_MANAGE_PASSWORD] 		= OBJECT_MANAGE_PASSWORD;
		g_systemRestore.indexArray[INDEX_SYSTEM_TIME_MANAGE] 	= OBJECT_SYSTEM_DATE;
		
		/* �������� */
		g_systemRestore.pParameterNameArray[INDEX_USER_NAME] 			= pSystemRestoreParameterName[0];
		g_systemRestore.pParameterNameArray[INDEX_RESTORE_DATE] 		= pSystemRestoreParameterName[3];
		g_systemRestore.pParameterNameArray[INDEX_MANAGE_PASSWORD] 		= pSystemRestoreParameterName[4];
		g_systemRestore.pParameterNameArray[INDEX_SYSTEM_TIME_MANAGE] 	= pSystemRestoreParameterName[2];
	}
	
	/* ���� */
	g_systemRestore.pTitle = "ϵͳ��ԭ";
	
	/* �����˵��������� */
	g_systemRestore.twoLevelMenu[INDEX_USER_NAME].parameterCnt = 2;	
	
	/* �����˵����� */
	g_systemRestore.twoLevelMenu[INDEX_USER_NAME].parameterType = NONE_USE_USER_DEFINE;
	if (g_systemRestore.twoLevelMenu[index].index == TEST_USER)
	{
		g_systemRestore.twoLevelMenu[INDEX_SYSTEM_PASSWORD].parameterType = IMMEDIATELY_PUTIN_NONE;
	}
	else
	{
		g_systemRestore.twoLevelMenu[INDEX_MANAGE_PASSWORD].parameterType = IMMEDIATELY_PUTIN_NONE;
	}
	
	/* �����˵������� */	
	g_systemRestore.twoLevelMenu[INDEX_USER_NAME].pParameterNameArray = pSystemRestoreUserName;
	
	/* ���ݱ������� */
	if (g_systemRestore.twoLevelMenu[index].index == TEST_USER)
	{
		g_systemRestore.oneLevelMenu[INDEX_SYSTEM_PASSWORD].saveType 	= TYPE_INT;
	}
	else
	{
		g_systemRestore.oneLevelMenu[INDEX_MANAGE_PASSWORD].saveType 	= TYPE_INT;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetSystemRestoreIndex(OBJECT_USER_NAME);
	if (index != 0xff)
	{	
		strcpy(g_systemRestore.parameterData[index],g_systemRestore.twoLevelMenu[index].pParameterNameArray[g_systemRestore.twoLevelMenu[index].index]);
	}
	
	if (g_systemRestore.twoLevelMenu[index].index == TEST_USER)
	{
		index = GetSystemRestoreIndex(OBJECT_SYSTEM_PASSWORD);
		if (index != 0xff)
		{
			g_systemRestore.parameterData[index][0] = NULL;
		}
	}
	else
	{
		index = GetSystemRestoreIndex(OBJECT_RESTORE_DATE);
		if (index != 0xff)
		{
			g_systemRestore.parameterData[index][0] = NULL;
		}
		
		index = GetSystemRestoreIndex(OBJECT_MANAGE_PASSWORD);
		if (index != 0xff)
		{
			g_systemRestore.parameterData[index][0] = NULL;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigSystemRestoreRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSystemRestoreRectangleFrameCoordinate( void )
{
	uint16_t startX = 230;
	uint16_t startY = 120;
	uint8_t i;
	
	for (i=0; i<g_systemRestore.curParameterNum; ++i)
	{
		g_systemRestore.oneLevelMenu[i].x = startX;
		g_systemRestore.oneLevelMenu[i].y = startY;
		g_systemRestore.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_systemRestore.oneLevelMenu[i].backColor = COLOR_BACK;
		g_systemRestore.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_systemRestore.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_systemRestore.oneLevelMenu[i].lenth = 102;
		g_systemRestore.oneLevelMenu[i].width = 30;
		g_systemRestore.oneLevelMenu[i].fontSize = 24;
		g_systemRestore.oneLevelMenu[i].rowDistance = 36;
		g_systemRestore.oneLevelMenu[i].columnDistance = 0;
		g_systemRestore.oneLevelMenu[i].lineWidth = 2;
		
		g_systemRestore.twoLevelMenu[i].x = g_systemRestore.oneLevelMenu[i].x + g_systemRestore.oneLevelMenu[i].lenth + 36;
		g_systemRestore.twoLevelMenu[i].y = g_systemRestore.oneLevelMenu[i].y;
		g_systemRestore.twoLevelMenu[i].pointColor = g_systemRestore.oneLevelMenu[i].pointColor;
		g_systemRestore.twoLevelMenu[i].backColor = g_systemRestore.oneLevelMenu[i].backColor;
		g_systemRestore.twoLevelMenu[i].lenth = 102;
		g_systemRestore.twoLevelMenu[i].fontSize = 24;
		g_systemRestore.twoLevelMenu[i].rowDistance = 2;
		g_systemRestore.twoLevelMenu[i].columnDistance = 0;
		g_systemRestore.twoLevelMenu[i].lineWidth = 2;				
		g_systemRestore.twoLevelMenu[i].width = g_systemRestore.twoLevelMenu[i].parameterCnt * \
												(g_systemRestore.twoLevelMenu[i].fontSize + g_systemRestore.twoLevelMenu[i].rowDistance) + \
												g_systemRestore.twoLevelMenu[i].rowDistance + 2 * g_systemRestore.twoLevelMenu[i].lineWidth;
		g_systemRestore.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_systemRestore.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_systemRestore.oneLevelMenu[i].width + g_systemRestore.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestoreDrawOneRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestoreDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_systemRestore.oneLevelMenu[index].x;
	rectangle.y = g_systemRestore.oneLevelMenu[index].y;
	rectangle.lenth = g_systemRestore.oneLevelMenu[index].lenth;
	rectangle.width = g_systemRestore.oneLevelMenu[index].width;
	rectangle.lineWidth = g_systemRestore.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_systemRestore.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestoreRectangleFrame
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestoreRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemRestore.allowMoveNum; ++i)
	{
		GUI_SystemRestoreDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestoreDrawOneRowOneLevelMenu
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestoreDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_systemRestore.oneLevelMenu[index].x - 6 * 24;
	const uint16_t y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemRestore.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemRestore.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemRestore.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestoreOneLevelMenu
 * Description    : ����һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestoreOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemRestore.curParameterNum; ++i)
	{
		GUI_SystemRestoreDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestoreTimeCue
 * Description    : ʱ����ʾ��Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestoreTimeCue( void )
{	
	uint8_t index = 0;
	uint16_t x,y,pointColor,backColor;
	
	index = GetSystemRestoreIndex(OBJECT_SYSTEM_DATE);
	
	if (index != 0xff)
	{
		x = g_systemRestore.oneLevelMenu[index].x + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
		y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
		pointColor = g_systemRestore.oneLevelMenu[index].pointColor;
		backColor = g_systemRestore.oneLevelMenu[index].backColor;
		
		GUI_ShowSystemDate(x,y,pointColor,backColor);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemRestore
 * Description    : ������������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemRestore( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_systemRestore.pTitle);
	
	ConfigSystemRestoreRectangleFrameCoordinate();
	
	GUI_SystemRestoreRectangleFrame();
	
	GUI_SystemRestoreOneLevelMenu();
	
	GUI_SystemRestoreTimeCue();
}	

/*------------------------------------------------------------
 * Function Name  : Show_SystemRestoreOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemRestoreOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_systemRestore.oneLevelMenu[index].x + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemRestore.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemRestore.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_systemRestore.oneLevelMenu[index].lenth - 2 * g_systemRestore.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_systemRestore.oneLevelMenu[index].width - 2 * g_systemRestore.oneLevelMenu[index].lineWidth - 2;
	char showBuff[MAX_PUTIN_BIT+1];
	
	lcd_fill(x,y,lenth,width,backColor);
	
	if (GetSystemRestoreUserName() == TEST_USER)
	{
		if (index == GetSystemRestoreIndex(OBJECT_SYSTEM_PASSWORD))
		{
			StrFillSpecifyChar(g_systemRestore.parameterData[index],showBuff,'*');
			GUI_DispStr24At(x,y,pointColor,backColor,showBuff);
			
			return;
		}
	}
	else
	{
		if (index == GetSystemRestoreIndex(OBJECT_MANAGE_PASSWORD))
		{
			StrFillSpecifyChar(g_systemRestore.parameterData[index],showBuff,'*');
			GUI_DispStr24At(x,y,pointColor,backColor,showBuff);
			
			return;
		}
	}
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemRestore.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemRestoreOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemRestoreOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemRestore.allowMoveNum; ++i)
	{
		Show_SystemRestoreOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ReportSearch
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_SystemRestore( void )
{
	Show_SystemRestoreOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : SystemRestorePutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestorePutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_systemRestore.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_systemRestore.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_systemRestore.enablePutin;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_systemRestore.oneLevelMenu[index].x + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PUTIN_BIT;
	pPutin->FillBit = MAX_PUTIN_BIT;
	pPutin->putinFrameLenth = g_systemRestore.oneLevelMenu[index].lenth - 2 * g_systemRestore.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_systemRestore.oneLevelMenu[index].width - 2 * g_systemRestore.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_systemRestore.putinNum;
	pPutin->SaveType = g_systemRestore.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_systemRestore.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = g_systemRestore.encryption;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_systemRestore.isIndexMove = NO;
	
	indexObj.enableMoveIndex = g_systemRestore.enableMoveIndex;
	indexObj.rowNum = g_systemRestore.allowMoveNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_systemRestore.allowMoveNum;
	indexObj.pNowIndex = &g_systemRestore.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_systemRestore.nowIndex != g_systemRestore.recordIndex)
	{		
		g_systemRestore.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreStatusProcess
 * Description    : ״̬����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreStatusProcess( void )
{
	uint8_t index = g_systemRestore.nowIndex;
	
	g_systemRestore.nowIndex %= g_systemRestore.allowMoveNum;
	
	switch ( g_systemRestore.twoLevelMenu[index].parameterType )
	{
		case IMMEDIATELY_PUTIN_NONE:
		case IMMEDIATELY_PUTIN_SHIFT:
			g_systemRestore.enablePutin = ENABLE;
			break;
		
		default:
			g_systemRestore.enablePutin = DISABLE;
			break;
	}
	
	switch ( g_systemRestore.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_systemRestore.enableArrow = ENABLE;
			break;
		
		default:
			g_systemRestore.enableArrow = DISABLE;
			break;
	}
	
	/* ���� */
	g_systemRestore.encryption = ENABLE;
	if (GetSystemRestoreUserName() == MANAGE_USER)
	{
		if (GetSystemRestoreIndex(OBJECT_RESTORE_DATE) == g_systemRestore.nowIndex)
		{
			g_systemRestore.encryption = DISABLE;
		}
	}
	
}

/*------------------------------------------------------------
 * Function Name  : Clear_SystemRestoreOneRowOneLevelMenuArrow
 * Description    : ���һ�м�ͷ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_SystemRestoreOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_systemRestore.oneLevelMenu[index].x + g_systemRestore.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_systemRestore.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemRestoreOneRowOneLevelMenuArrow
 * Description    : ��ʾһ�м�ͷ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemRestoreOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_systemRestore.oneLevelMenu[index].x + g_systemRestore.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_systemRestore.oneLevelMenu[index].y + g_systemRestore.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreMoveCursorProcess( void )
{
	if (g_systemRestore.isIndexMove == YES)
	{		
		if (g_systemRestore.recordIndex != 0xff)
		{
			g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].pointColor = g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].recordPointColor;
			g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].backColor = g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].recordBackColor;
			
			Show_SystemRestoreOneRowOneLevelMenuContent(g_systemRestore.recordIndex);
			Clear_SystemRestoreOneRowOneLevelMenuArrow(g_systemRestore.recordIndex);
			
			g_systemRestore.oneLevelMenu[g_systemRestore.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemRestore.oneLevelMenu[g_systemRestore.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemRestoreOneRowOneLevelMenuContent(g_systemRestore.nowIndex);		
			if (g_systemRestore.enableArrow == ENABLE)
			{
				Show_SystemRestoreOneRowOneLevelMenuArrow(g_systemRestore.nowIndex);
			}
		}
		else
		{
			g_systemRestore.oneLevelMenu[g_systemRestore.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemRestore.oneLevelMenu[g_systemRestore.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemRestoreOneRowOneLevelMenuContent(g_systemRestore.nowIndex);
			if (g_systemRestore.enableArrow == ENABLE)
			{
				Show_SystemRestoreOneRowOneLevelMenuArrow(g_systemRestore.nowIndex);
			}
		}
		
		g_systemRestore.recordIndex = g_systemRestore.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_systemRestore.refreshShortcut == ENABLE)
	{
		g_systemRestore.refreshShortcut = DISABLE;
		
		if (GetSystemRestoreUserName() == MANAGE_USER)
		{
			pShortCut->status = SHOW_F3 | SHOW_F4;
		}
		else
		{
			pShortCut->status = SHOW_F2 | SHOW_F3 | SHOW_F4;
		}
		
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[1] = pTwoLevelMenu[83];
		pShortCut->pContent[2] = pTwoLevelMenu[44];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreIndexUpdate
 * Description    : ����ֵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreIndexUpdate( void )
{
	g_systemRestore.nowIndex++;
	g_systemRestore.nowIndex %= g_systemRestore.allowMoveNum;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreUpdateStatus( void )
{
	SystemRestoreIndexUpdate();
	
	g_systemRestore.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : GetSystemRestoreUserName
 * Description    : ��ȡ�û���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static USER_NAME_TypeDef GetSystemRestoreUserName( void )
{
	uint8_t index = 0;
	
	index = GetSystemRestoreIndex(OBJECT_USER_NAME);
	
	return (USER_NAME_TypeDef)g_systemRestore.twoLevelMenu[index].index;
}
	
/*------------------------------------------------------------
 * Function Name  : SystemRestoreCheckSystemPasswordProcess
 * Description    : ���ϵͳ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemRestoreCheckSystemPasswordProcess( void )
{
	uint8_t index = 0;
	
	index = GetSystemRestoreIndex(OBJECT_SYSTEM_PASSWORD);
	if (index != 0xff)
	{
		if ((TestManagerPassword(g_systemRestore.parameterData[index])==PASSED) ||\
			(TestUserPassword(g_systemRestore.parameterData[index])==PASSED))
		{
			return PASSED;
		}
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreCheckManagerPasswordProcess
 * Description    : ������Ա����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemRestoreCheckManagerPasswordProcess( void )
{
	uint8_t index = 0;
	
	index = GetSystemRestoreIndex(OBJECT_MANAGE_PASSWORD);
	if (index != 0xff)
	{
		if (TestManagerPassword(g_systemRestore.parameterData[index]) == PASSED)
		{
			return PASSED;
		}
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreFromUSBProcess
 * Description    : ��U�̻�ԭ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemRestoreFromUSBProcess( void )
{
	FRESULT result[3];
	
	if (Get_USB_Status() == ERROR)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[3]);	
					
		return FAILED;
	}
	
	if (GetSystemRestoreUserName() == TEST_USER)
	{
		result[0] = pcm_recover(BACKUP_USB);
		result[1] = prm_recover(BACKUP_USB);
		result[2] = prv_recover(BACKUP_USB);
	}
	
	if ((result[0]==FR_OK) && (result[1]==FR_OK) && (result[2]==FR_OK))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[4]);	
	}
	else
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[6]);

		return FAILED;
	}
	
	pcm_save();
	prm_save();
	prv_save();
	
	pcm_read();										
	prm_read();								
	prv_read();							
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreFromSDProcess
 * Description    : ��SD����ԭ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemRestoreFromSDProcess( void )
{
	FRESULT result[3];
	uint8_t index = 0;
	
	if (GetSystemRestoreUserName() == TEST_USER)
	{
		result[0] = pcm_recover(BACKUP_SD);
		result[1] = prm_recover(BACKUP_SD);
		result[2] = prv_recover(BACKUP_SD);
	}
	else
	{
		index = GetSystemRestoreIndex(OBJECT_RESTORE_DATE);
		if (index != 0xff)
		{
			result[0] = PcmRecoverWithDate(g_systemRestore.parameterData[index]);
			result[1] = PrmRecoverWithDate(g_systemRestore.parameterData[index]);
			result[2] = PrvRecoverWithDate(g_systemRestore.parameterData[index]);
		}	
	}
	
	if ((result[0]==FR_OK) && (result[1]==FR_OK) && (result[2]==FR_OK))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[4]);	
	}
	else
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[6]);

		return FAILED;
	}
	
	pcm_save();
	prm_save();
	prv_save();
	
	pcm_read();										
	prm_read();								
	prv_read();	
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreKeyProcess( void )
{
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	uint8_t index = g_systemRestore.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_systemRestore.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_systemRestore.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* �����ݲ˵� */
					GUI_ClearShortcutMenu();
					
					/* ��ԭһ���˵� */
					g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].pointColor = g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].recordPointColor;
					g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].backColor = g_systemRestore.oneLevelMenu[g_systemRestore.recordIndex].recordBackColor;
					
					Show_SystemRestoreOneRowOneLevelMenuContent(g_systemRestore.recordIndex);
					
					/* ���ö����˵� */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_systemRestore.twoLevelMenu[index].x;
					pMenu->y = g_systemRestore.twoLevelMenu[index].y;
					pMenu->nowIndex = g_systemRestore.twoLevelMenu[index].index;
					pMenu->maxUpY = g_systemRestore.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_systemRestore.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_systemRestore.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_systemRestore.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_systemRestore.twoLevelMenu[index].lenth;
					pMenu->width = g_systemRestore.twoLevelMenu[index].width;
					pMenu->pointColor = g_systemRestore.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_systemRestore.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_systemRestore.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_systemRestore.twoLevelMenu[index].pointColor;
					pMenu->nums = g_systemRestore.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_systemRestore.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_systemRestore.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{		
						g_systemRestore.twoLevelMenu[index].index = pMenu->nowIndex;
						
						strcpy(g_systemRestore.parameterData[index],pMenu->pParameterNameArray[pMenu->nowIndex]);
						
						if (index == GetSystemRestoreIndex(OBJECT_USER_NAME))
						{
							g_systemRestore.leavePage.flagLeavePage = SET;
							g_systemRestore.leavePage.flagSaveData = RESET;
						}
					}
					
					SystemRestoreUpdateStatus();				
				}
				break;
			
			case KEY_ENTER:
				if ( (g_systemRestore.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_NONE) || \
				     (g_systemRestore.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_SHIFT) )
				{				
					putinStatus = GetPutinStatus();
					
					switch ( putinStatus )
					{
						case STATUS_DISABLE_PUTIN:
							return;
						
						case STATUS_EDIT_COMP:	
							switch ( g_systemRestore.oneLevelMenu[index].saveType )
							{
								case TYPE_INT:
									if (GetSystemRestoreUserName() == MANAGE_USER)
									{
										if (index == GetSystemRestoreIndex(OBJECT_RESTORE_DATE))
										{
											if (g_systemRestore.putinNum != 8)
											{
												SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[0]);
												g_systemRestore.leavePage.flagLeavePage = SET;
												g_systemRestore.leavePage.flagSaveData = RESET;
												
												return;
											}
										}
									}									
									numtochar(g_systemRestore.putinNum,*GetPutinIntDataAddr(),g_systemRestore.parameterData[index]);																											
									break;
								default:
									break;
							}
							break;						
						
						default:
							break;
					}
					
					if (GetSystemRestoreUserName() == TEST_USER)
					{
						if (index == GetSystemRestoreIndex(OBJECT_SYSTEM_PASSWORD))
						{
							if (SystemRestoreCheckSystemPasswordProcess() == FAILED)
							{
								SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[1]);
								g_systemRestore.leavePage.flagLeavePage = SET;
								g_systemRestore.leavePage.flagSaveData = RESET;
								
								return;
							}
						}
					}
					else
					{
						if (index == GetSystemRestoreIndex(OBJECT_MANAGE_PASSWORD))
						{
							if (SystemRestoreCheckManagerPasswordProcess() == FAILED)
							{
								SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[2]);
								g_systemRestore.leavePage.flagLeavePage = SET;
								g_systemRestore.leavePage.flagSaveData = RESET;
								
								return;
							}
						}
					}
					g_systemRestore.recordIndex	= 0xff;
					g_systemRestore.refreshShortcut = ENABLE;
				}
				break;
			
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(ADVANCE_PARAMETER_SET_PAGE);
						g_systemRestore.leavePage.flagLeavePage = SET;
						g_systemRestore.leavePage.flagSaveData = RESET;
						break;
					case STATUS_CANCEL_PUTIN:
						g_systemRestore.recordIndex	= 0xff;
						g_systemRestore.refreshShortcut = ENABLE;		
						break;
					
					default:
						break;
				}			
				break;
				
			case KEY_F2:
				if (GetSystemRestoreUserName() == TEST_USER)
				{
					if (SystemRestoreCheckSystemPasswordProcess() == FAILED)
					{
						SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[1]);
						g_systemRestore.leavePage.flagLeavePage = SET;
						g_systemRestore.leavePage.flagSaveData = RESET;
						
						return;
					}	
					
					if (SystemRestoreFromUSBProcess() == PASSED)
					{
						g_systemRestore.leavePage.flagSaveData = SET;
					}
					else
					{
						g_systemRestore.leavePage.flagSaveData = RESET;
					}
					g_systemRestore.leavePage.flagLeavePage = SET;					
				}
				break;
			
			case KEY_F3:
				if (GetSystemRestoreUserName() == TEST_USER)
				{
					if (SystemRestoreCheckSystemPasswordProcess() == FAILED)
					{
						SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[1]);
						g_systemRestore.leavePage.flagLeavePage = SET;
						g_systemRestore.leavePage.flagSaveData = RESET;
						
						return;
					}
				}
				else
				{
					if (SystemRestoreCheckManagerPasswordProcess() == FAILED)
					{
						SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[2]);
						g_systemRestore.leavePage.flagLeavePage = SET;
						g_systemRestore.leavePage.flagSaveData = RESET;
						
						return;
					}
				}
			
				if (SystemRestoreFromSDProcess() == PASSED)
				{
					g_systemRestore.leavePage.flagSaveData = SET;
				}
				else
				{
					g_systemRestore.leavePage.flagSaveData = RESET;
				}
				g_systemRestore.leavePage.flagLeavePage = SET;				
				break;
				
			case KEY_F4:
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				g_systemRestore.leavePage.flagLeavePage = SET;
				g_systemRestore.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemRestoreLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemRestoreLeavePageCheckCycle( void )
{
	if (g_systemRestore.leavePage.flagLeavePage == SET)
	{
		if (g_systemRestore.leavePage.flagSaveData == SET)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemRestoreCue[7]);
			
			PopWindowsProcessCycle();
			
			SystemSoftwareReset();
		}
	}
}

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
