/**
  ******************************************************************************
  * @file    OnlineParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-21 11:24:25
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ������������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "OnlineParameter.h"
#include "netconf.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				4
#define MAX_PARAMETER_PUTIN_BIT			3

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_IP1 = 0,
	OBJECT_IP2,
	OBJECT_IP3,
	OBJECT_IP4,
}OBJECT_ONLINE_PARAMETER_TypeDef;

typedef enum
{
	INDEX_IP1 = 0,
	INDEX_IP2,
	INDEX_IP3,
	INDEX_IP4,
}INDEX_ONLINE_PARAMETER_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	const char *pCutOffRuleNameArray[MAX_PARAMETER_CNT];	//�ָ���
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//��������
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	uint8_t putinNum;								//�����ַ�����
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}ONLINE_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pOnlineParameterName[] =
{
	"IP��ַ��",
};

const char * const pOnlineParameterCutOffRuleName[] =
{
	".",
	".",
	".",
	".",
};

const char * const pOnlineParameterErrorCue[] =
{
	"IP��ַ��һ��ܳ���255��",		//0
	"IP��ַ����������������Ч��",
	"�Ƿ�������",
};

const char * const pOnlineParameterSelectMenuCue[] = 
{
	"  ����  ",
	"  ȡ��  ",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ONLINE_PARAMETER_TypeDef g_onlineParameter;

/* Private function prototypes -----------------------------------------------*/
static void OnlineParameterInit( void );
static void OnlineParameterParameterConfig( void );
static void OnlineParameterReadParameter( void );
static void GUI_OnlineParameter( void );
static void Traverse_OnlineParameter( void );
static void OnlineParameterPutinProcess( void );
static void OnlineParameterMoveIndexProcess( void );
static void OnlineParameterMoveCursorProcess( void );
static void OnlineParameterShortcutCycleTask( void );
static void OnlineParameterKeyProcess( void );
static void OnlineParameterLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadOnlineParameterPage
 * Description    : ��������ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadOnlineParameterPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	OnlineParameterInit();
	
	/* �������� */
	OnlineParameterParameterConfig();
	
	/* ��ȡ���� */
	OnlineParameterReadParameter();
	
	/* ��GUI��� */
	GUI_OnlineParameter();
	
	/* ���� */
	Traverse_OnlineParameter();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_onlineParameter.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		OnlineParameterPutinProcess();
		
		/* �ƶ�����ֵ */
		OnlineParameterMoveIndexProcess();
		
		/* �ƶ���� */
		OnlineParameterMoveCursorProcess();
		
		/* ��ݲ˵� */
		OnlineParameterShortcutCycleTask();
		
		/* �������� */
		OnlineParameterKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		OnlineParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterInit( void )
{
	g_onlineParameter.isIndexMove = NO;		
	g_onlineParameter.recordIndex = 0xff;
	g_onlineParameter.refreshShortcut = ENABLE;
	g_onlineParameter.leavePage.flagLeavePage = RESET;
	g_onlineParameter.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterParameterConfig( void )
{
	/* �Կ���� */
	g_onlineParameter.curParameterNum = MAX_PARAMETER_CNT;
	
	/* ����ֵ */
	g_onlineParameter.indexArray[INDEX_IP1] 		= OBJECT_IP1;
	g_onlineParameter.indexArray[INDEX_IP2] 		= OBJECT_IP2;
	g_onlineParameter.indexArray[INDEX_IP3] 		= OBJECT_IP3;
	g_onlineParameter.indexArray[INDEX_IP4] 		= OBJECT_IP4;
	
	/* �������� */
	g_onlineParameter.pParameterNameArray[INDEX_IP1] 	= pOnlineParameterName[0];
	g_onlineParameter.pParameterNameArray[INDEX_IP2] 	= pOnlineParameterName[0];
	g_onlineParameter.pParameterNameArray[INDEX_IP3] 	= pOnlineParameterName[0];
	g_onlineParameter.pParameterNameArray[INDEX_IP4] 	= pOnlineParameterName[0];
	
	/* �ָ�������*/
	g_onlineParameter.pCutOffRuleNameArray[INDEX_IP1] 	= pOnlineParameterCutOffRuleName[0];
	g_onlineParameter.pCutOffRuleNameArray[INDEX_IP2] 	= pOnlineParameterCutOffRuleName[1];
	g_onlineParameter.pCutOffRuleNameArray[INDEX_IP3] 	= pOnlineParameterCutOffRuleName[2];
	g_onlineParameter.pCutOffRuleNameArray[INDEX_IP4] 	= pOnlineParameterCutOffRuleName[3];
	
	/* ���� */
	g_onlineParameter.pTitle = "������������";
	
	/* ���ݱ������� */
	g_onlineParameter.oneLevelMenu[INDEX_IP1].saveType 		= TYPE_INT;
	g_onlineParameter.oneLevelMenu[INDEX_IP2].saveType 		= TYPE_INT;
	g_onlineParameter.oneLevelMenu[INDEX_IP3].saveType 		= TYPE_INT;
	g_onlineParameter.oneLevelMenu[INDEX_IP4].saveType 		= TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : GetOnlineParameterIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetOnlineParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_onlineParameter.curParameterNum; ++i)
	{
		if (g_onlineParameter.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterReadParameter( void )
{
	uint32_t IP = 0;
	uint8_t index = 0;
	
	IP = devc_ip_get();
	
	index = GetOnlineParameterIndex(OBJECT_IP4);
	if (index != 0xff)
	{
		usprintf(g_onlineParameter.parameterData[index],"%03d",(uint8_t)IP);	
	}
	IP >>= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP3);
	if (index != 0xff)
	{
		usprintf(g_onlineParameter.parameterData[index],"%03d",(uint8_t)IP);	
	}
	IP >>= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP2);
	if (index != 0xff)
	{
		usprintf(g_onlineParameter.parameterData[index],"%03d",(uint8_t)IP);	
	}
	IP >>= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP1);
	if (index != 0xff)
	{
		usprintf(g_onlineParameter.parameterData[index],"%03d",(uint8_t)IP);	
	}
	IP >>= 8;
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterWriteParameter
 * Description    : д����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterWriteParameter( void )
{
	uint32_t IP = 0;
	uint8_t index = 0;
	
	index = GetOnlineParameterIndex(OBJECT_IP1);
	if (index != 0xff)
	{
		IP |= (uint8_t)ustrtoul(g_onlineParameter.parameterData[index],0,10); 		
	}
	IP <<= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP2);
	if (index != 0xff)
	{
		IP |= (uint8_t)ustrtoul(g_onlineParameter.parameterData[index],0,10); 		
	}
	IP <<= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP3);
	if (index != 0xff)
	{
		IP |= (uint8_t)ustrtoul(g_onlineParameter.parameterData[index],0,10); 		
	}
	IP <<= 8;
	
	index = GetOnlineParameterIndex(OBJECT_IP4);
	if (index != 0xff)
	{
		IP |= (uint8_t)ustrtoul(g_onlineParameter.parameterData[index],0,10); 		
	}
	
	devc_ip_set(IP);
	
	prm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigOnlineParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigOnlineParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 244;
	const uint16_t START_Y = 229;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_onlineParameter.curParameterNum; ++i)
	{
		g_onlineParameter.oneLevelMenu[i].x = startX;
		g_onlineParameter.oneLevelMenu[i].y = startY;
		g_onlineParameter.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_onlineParameter.oneLevelMenu[i].backColor = COLOR_BACK;
		g_onlineParameter.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_onlineParameter.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_onlineParameter.oneLevelMenu[i].lenth = 42;
		g_onlineParameter.oneLevelMenu[i].width = 30;
		g_onlineParameter.oneLevelMenu[i].fontSize = 24;
		g_onlineParameter.oneLevelMenu[i].rowDistance = 48;
		g_onlineParameter.oneLevelMenu[i].columnDistance = 48;
		g_onlineParameter.oneLevelMenu[i].lineWidth = 2;
		
		startX += g_onlineParameter.oneLevelMenu[i].lenth + g_onlineParameter.oneLevelMenu[i].columnDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_OnlineParameterDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_onlineParameter.oneLevelMenu[index].x;
	rectangle.y = g_onlineParameter.oneLevelMenu[index].y;
	rectangle.lenth = g_onlineParameter.oneLevelMenu[index].lenth;
	rectangle.width = g_onlineParameter.oneLevelMenu[index].width;
	rectangle.lineWidth = g_onlineParameter.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_onlineParameter.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_OnlineParameterRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_onlineParameter.curParameterNum; ++i)
	{
		GUI_OnlineParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_OnlineParameterDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_onlineParameter.oneLevelMenu[index].x - 96;
	const uint16_t y = g_onlineParameter.oneLevelMenu[index].y + 3;
	const uint16_t pointColor = g_onlineParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_onlineParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_onlineParameter.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_OnlineParameterOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterOneLevelMenu( void )
{	
	uint8_t index = 0;
	
	index = GetOnlineParameterIndex(OBJECT_IP1);
	if (index != 0xff)
	{
		GUI_OnlineParameterDrawOneRowOneLevelMenu(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateCutOffCuleSingleOneLevelMenu
 * Description    : �����ָ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterCutOffRuleSingleOneLevelMenu( uint8_t index )
{
	uint16_t tempLen = (g_onlineParameter.oneLevelMenu[index].columnDistance - (g_onlineParameter.oneLevelMenu[index].fontSize>>1)) >> 1;
	const uint16_t lenth = g_onlineParameter.oneLevelMenu[index].lenth;
	const uint16_t x = g_onlineParameter.oneLevelMenu[index].x + lenth + tempLen;
	const uint16_t y = g_onlineParameter.oneLevelMenu[index].y + g_onlineParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_onlineParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_onlineParameter.oneLevelMenu[index].backColor;
		
	GUI_DispStr24At(x,y,pointColor,backColor,g_onlineParameter.pCutOffRuleNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDateCutOffCuleOneLevelMenu
 * Description    : ��������һ���˵��ָ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameterCutOffRuleOneLevelMenu( void )
{	
	uint8_t i;

	for (i=0; i<g_onlineParameter.curParameterNum - 1; ++i)
	{		
		GUI_OnlineParameterCutOffRuleSingleOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_OnlineParameter
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_OnlineParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_onlineParameter.pTitle);
	
	ConfigOnlineParameterRectangleFrameCoordinate();
	
	GUI_OnlineParameterRectangleFrame();
	
	GUI_OnlineParameterOneLevelMenu();
	
	GUI_OnlineParameterCutOffRuleOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_OnlineParameterDataOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_OnlineParameterDataOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_onlineParameter.oneLevelMenu[index].x + g_onlineParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_onlineParameter.oneLevelMenu[index].y + g_onlineParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_onlineParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_onlineParameter.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_onlineParameter.oneLevelMenu[index].lenth - 2 * g_onlineParameter.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_onlineParameter.oneLevelMenu[index].width - 2 * g_onlineParameter.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_onlineParameter.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_OnlineParameterDataOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_OnlineParameterDataOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_onlineParameter.curParameterNum; ++i)
	{
		Show_OnlineParameterDataOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_OnlineParameter
 * Description    : ����ʱ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_OnlineParameter( void )
{
	Show_OnlineParameterDataOneLevelMenuContent();
}	

/*------------------------------------------------------------
 * Function Name  : OnlineParameterPutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_onlineParameter.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_onlineParameter.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_onlineParameter.oneLevelMenu[index].x + g_onlineParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_onlineParameter.oneLevelMenu[index].y + g_onlineParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_onlineParameter.oneLevelMenu[index].lenth - 2 * g_onlineParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_onlineParameter.oneLevelMenu[index].width - 2 * g_onlineParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_onlineParameter.putinNum;
	pPutin->SaveType = g_onlineParameter.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_onlineParameter.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_onlineParameter.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = 1;
	indexObj.colNum = g_onlineParameter.curParameterNum;
	indexObj.sumNum = g_onlineParameter.curParameterNum;
	indexObj.pNowIndex = &g_onlineParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_onlineParameter.nowIndex != g_onlineParameter.recordIndex)
	{		
		g_onlineParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterMoveCursorProcess( void )
{
	if (g_onlineParameter.isIndexMove == YES)
	{		
		if (g_onlineParameter.recordIndex != 0xff)
		{
			g_onlineParameter.oneLevelMenu[g_onlineParameter.recordIndex].pointColor = g_onlineParameter.oneLevelMenu[g_onlineParameter.recordIndex].recordPointColor;
			g_onlineParameter.oneLevelMenu[g_onlineParameter.recordIndex].backColor = g_onlineParameter.oneLevelMenu[g_onlineParameter.recordIndex].recordBackColor;
			
			Show_OnlineParameterDataOneRowOneLevelMenuContent(g_onlineParameter.recordIndex);		
			
			g_onlineParameter.oneLevelMenu[g_onlineParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_onlineParameter.oneLevelMenu[g_onlineParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_OnlineParameterDataOneRowOneLevelMenuContent(g_onlineParameter.nowIndex);	
		}
		else
		{
			g_onlineParameter.oneLevelMenu[g_onlineParameter.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_onlineParameter.oneLevelMenu[g_onlineParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_OnlineParameterDataOneRowOneLevelMenuContent(g_onlineParameter.nowIndex);
		}
		
		g_onlineParameter.recordIndex = g_onlineParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_onlineParameter.refreshShortcut == ENABLE)
	{
		g_onlineParameter.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterIndexUpdate
 * Description    : ����ֵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterIndexUpdate( void )
{
	g_onlineParameter.nowIndex++;
	g_onlineParameter.nowIndex %= g_onlineParameter.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterUpdateStatus( void )
{
	OnlineParameterIndexUpdate();
	
	g_onlineParameter.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterCheckDataCycle
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus OnlineParameterCheckDataCycle( void )
{
	uint8_t i = 0;
	uint32_t temp = 0;
	
	for (i=0; i<g_onlineParameter.curParameterNum; ++i)
	{
		temp = ustrtoul(g_onlineParameter.parameterData[i],0,10);
		
		if (temp > 0xff)
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pOnlineParameterErrorCue[0]);
						
			return FAILED;
		}
	}
	
	return PASSED;
}	

/*------------------------------------------------------------
 * Function Name  : OnlineParameterKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterKeyProcess( void )
{
	uint8_t index = g_onlineParameter.nowIndex;
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
						switch ( g_onlineParameter.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								usprintf(g_onlineParameter.parameterData[index],"%03d",*GetPutinIntDataAddr());
								break;
							case TYPE_FLOAT:
								floattochar(g_onlineParameter.putinNum,g_onlineParameter.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_onlineParameter.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_onlineParameter.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				if (OnlineParameterCheckDataCycle() == FAILED)
				{
					g_onlineParameter.leavePage.flagLeavePage = SET;
					g_onlineParameter.leavePage.flagSaveData = RESET;
					break;
				}
				
				OnlineParameterUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_onlineParameter.leavePage.flagLeavePage = SET;
						g_onlineParameter.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						OnlineParameterUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
				
			case KEY_F3:
				SetPage(SYSTEM_SET);
				g_onlineParameter.leavePage.flagLeavePage = SET;
				g_onlineParameter.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_onlineParameter.leavePage.flagLeavePage = SET;
				g_onlineParameter.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : OnlineParameterLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void OnlineParameterLeavePageCheckCycle( void )
{
	if (g_onlineParameter.leavePage.flagLeavePage == SET)
	{
		if (g_onlineParameter.leavePage.flagSaveData == SET)
		{
			if (OnlineParameterCheckDataCycle() == FAILED)
			{
				SetPage(ONLINE_PARAMETER_PAGE);
				PopWindowsProcessCycle();
				
				return;
			}
				
			OnlineParameterWriteParameter();
			
			/* ѡ���Ƿ����� */
			SetSelectPopWindowsInfomation(POP_PCM_ASKING,2,&pOnlineParameterErrorCue[1],pOnlineParameterSelectMenuCue);
			PopWindowsProcessCycle();
			if ( GetPopWindowsSelectResult() == YES)
			{
				SystemSoftwareReset();
			}
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
