/**
  ******************************************************************************
  * @file    SystemPassword.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-21 16:23:27
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ϵͳ��������ҳ
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SystemPassword.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				3
#define MAX_PARAMETER_PUTIN_BIT			8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_INITIAL_PASSWORD = 0,
	OBJECT_NEW_PASSWORD,
	OBJECT_CONFIRM_NEW_PASSWORD,
}OBJECT_SYSTEM_PASSWORD_TypeDef;

typedef enum
{
	INDEX_INITIAL_PASSWORD = 0,
	INDEX_NEW_PASSWORD,
	INDEX_CONFIRM_NEW_PASSWORD,
}INDEX_SYSTEM_PASSWORD_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_PARAMETER_CNT][MAX_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_PARAMETER_CNT];
	const char *pParameterNameArray[MAX_PARAMETER_CNT];
	uint8_t indexArray[MAX_PARAMETER_CNT];
	uint8_t curParameterNum;						//��������
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	uint8_t putinNum;								//�����ַ�����
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}SYSTEM_PASSWORD_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSystemPasswordName[] =
{
	"������ԭ���룺",
	"�����������룺",
	"��ȷ�������룺",
};

const char * const pSystemPasswordWarn[] = 
{
	"ԭ������ϵͳ���벻һ�£�",		//0
	"ȷ�������������벻һ�£�",		//1
	"�޸�ϵͳ����ɹ���",			//2
	"�����벻��Ϊ�գ�",				//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SYSTEM_PASSWORD_TypeDef g_systemPassword;

/* Private function prototypes -----------------------------------------------*/
static void SystemPasswordInit( void );
static void SystemPasswordParameterConfig( void );
static void SystemPasswordReadParameter( void );
static void GUI_SystemPassword( void );
static void Traverse_SystemPassword( void );
static void SystemPasswordPutinProcess( void );
static void SystemPasswordShortcutCycleTask( void );
static void SystemPasswordMoveIndexProcess( void );
static void SystemPasswordMoveCursorProcess( void );
static void SystemPasswordKeyProcess( void );
static void SystemPasswordLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadSystemPasswordPage
 * Description    : ϵͳ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemPasswordPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	SystemPasswordInit();
	
	/* �������� */
	SystemPasswordParameterConfig();
	
	/* ��ȡ���� */
	SystemPasswordReadParameter();
	
	/* ��GUI��� */
	GUI_SystemPassword();
	
	/* ���� */
	Traverse_SystemPassword();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_systemPassword.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		SystemPasswordPutinProcess();
		
		/* �ƶ�����ֵ */
		SystemPasswordMoveIndexProcess();
		
		/* �ƶ���� */
		SystemPasswordMoveCursorProcess();
		
		/* ��ݲ˵� */
		SystemPasswordShortcutCycleTask();
		
		/* �������� */
		SystemPasswordKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		SystemPasswordLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordInit( void )
{
	g_systemPassword.isIndexMove = NO;	
	g_systemPassword.recordIndex = 0xff;
	g_systemPassword.refreshShortcut = ENABLE;
	g_systemPassword.leavePage.flagLeavePage = RESET;
	g_systemPassword.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordParameterConfig( void )
{
	/* ���� */
	g_systemPassword.curParameterNum = MAX_PARAMETER_CNT;
	
	/* ����ֵ */
	g_systemPassword.indexArray[INDEX_INITIAL_PASSWORD] 		= OBJECT_INITIAL_PASSWORD;
	g_systemPassword.indexArray[INDEX_NEW_PASSWORD] 			= OBJECT_NEW_PASSWORD;
	g_systemPassword.indexArray[INDEX_CONFIRM_NEW_PASSWORD] 	= OBJECT_CONFIRM_NEW_PASSWORD;
	
	/* �������� */
	g_systemPassword.pParameterNameArray[INDEX_INITIAL_PASSWORD] 		= pSystemPasswordName[0];
	g_systemPassword.pParameterNameArray[INDEX_NEW_PASSWORD] 			= pSystemPasswordName[1];
	g_systemPassword.pParameterNameArray[INDEX_CONFIRM_NEW_PASSWORD] 	= pSystemPasswordName[2];
	
	/* ���� */
	g_systemPassword.pTitle = "ϵͳ��������";
	
	/* ���ݱ������� */
	g_systemPassword.oneLevelMenu[INDEX_INITIAL_PASSWORD].saveType 		= TYPE_INT;
	g_systemPassword.oneLevelMenu[INDEX_NEW_PASSWORD].saveType 			= TYPE_INT;
	g_systemPassword.oneLevelMenu[INDEX_CONFIRM_NEW_PASSWORD].saveType	= TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : GetSoftActiveIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetSystemPasswordIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_systemPassword.curParameterNum; ++i)
	{
		if (g_systemPassword.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : SoftActiveReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetSystemPasswordIndex(OBJECT_INITIAL_PASSWORD);
	if (index != 0xff)
	{
		g_systemPassword.parameterData[index][0] = NULL;
	}
	
	index = GetSystemPasswordIndex(OBJECT_NEW_PASSWORD);
	if (index != 0xff)
	{
		g_systemPassword.parameterData[index][0] = NULL;
	}
	
	index = GetSystemPasswordIndex(OBJECT_CONFIRM_NEW_PASSWORD);
	if (index != 0xff)
	{
		g_systemPassword.parameterData[index][0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordWriteParameter
 * Description    : д����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordWriteParameter( void )
{
	uint8_t index = 0;
	
	index = GetSystemPasswordIndex(OBJECT_NEW_PASSWORD);
	if (index != 0xff)
	{
		strcpy(pHmi->paseword_system,g_systemPassword.parameterData[index]);
		
		pcm_save();
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigSystemPasswordParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSystemPasswordParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 147;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_systemPassword.curParameterNum; ++i)
	{
		g_systemPassword.oneLevelMenu[i].x = startX;
		g_systemPassword.oneLevelMenu[i].y = startY;
		g_systemPassword.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_systemPassword.oneLevelMenu[i].backColor = COLOR_BACK;
		g_systemPassword.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_systemPassword.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_systemPassword.oneLevelMenu[i].lenth = 102;
		g_systemPassword.oneLevelMenu[i].width = 30;
		g_systemPassword.oneLevelMenu[i].fontSize = 24;
		g_systemPassword.oneLevelMenu[i].rowDistance = 24;
		g_systemPassword.oneLevelMenu[i].columnDistance = 0;
		g_systemPassword.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_systemPassword.oneLevelMenu[i].width + g_systemPassword.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemPasswordDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemPasswordDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_systemPassword.oneLevelMenu[index].x;
	rectangle.y = g_systemPassword.oneLevelMenu[index].y;
	rectangle.lenth = g_systemPassword.oneLevelMenu[index].lenth;
	rectangle.width = g_systemPassword.oneLevelMenu[index].width;
	rectangle.lineWidth = g_systemPassword.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_systemPassword.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemPasswordRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemPasswordRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemPassword.curParameterNum; ++i)
	{
		GUI_SystemPasswordDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemPasswordDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemPasswordDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_systemPassword.oneLevelMenu[index].x - 7 * 24;
	const uint16_t y = g_systemPassword.oneLevelMenu[index].y + g_systemPassword.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemPassword.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemPassword.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemPassword.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemPasswordOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemPasswordOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_systemPassword.curParameterNum; ++i)
	{
		GUI_SystemPasswordDrawOneRowOneLevelMenu(i);
	}
}
	
/*------------------------------------------------------------
 * Function Name  : GUI_SystemPassword
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemPassword( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_systemPassword.pTitle);
	
	ConfigSystemPasswordParameterRectangleFrameCoordinate();
	
	GUI_SystemPasswordRectangleFrame();
	
	GUI_SystemPasswordOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemPasswordOneRowOneLevelMenuContent
 * Description    : ��ʾһ�в���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemPasswordOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_systemPassword.oneLevelMenu[index].x + g_systemPassword.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_systemPassword.oneLevelMenu[index].y + g_systemPassword.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemPassword.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemPassword.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_systemPassword.oneLevelMenu[index].lenth - 2 * g_systemPassword.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_systemPassword.oneLevelMenu[index].width - 2 * g_systemPassword.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemPassword.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Traverse_SystemPassword
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_SystemPassword( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemPassword.curParameterNum; ++i)
	{
		Show_SystemPasswordOneRowOneLevelMenuContent(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordPutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_systemPassword.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_systemPassword.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_systemPassword.oneLevelMenu[index].x + g_systemPassword.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_systemPassword.oneLevelMenu[index].y + g_systemPassword.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_systemPassword.oneLevelMenu[index].lenth - 2 * g_systemPassword.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_systemPassword.oneLevelMenu[index].width - 2 * g_systemPassword.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_systemPassword.putinNum;
	pPutin->SaveType = g_systemPassword.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_systemPassword.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_systemPassword.refreshShortcut == ENABLE)
	{
		g_systemPassword.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[81];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordMoveIndexProcess( void )
{	
	INDEX_MANAGE_TypeDef indexObj;
	
	g_systemPassword.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_systemPassword.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_systemPassword.curParameterNum;
	indexObj.pNowIndex = &g_systemPassword.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_systemPassword.nowIndex != g_systemPassword.recordIndex)
	{		
		g_systemPassword.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordMoveCursorProcess( void )
{
	if (g_systemPassword.isIndexMove == YES)
	{		
		if (g_systemPassword.recordIndex != 0xff)
		{
			g_systemPassword.oneLevelMenu[g_systemPassword.recordIndex].pointColor = g_systemPassword.oneLevelMenu[g_systemPassword.recordIndex].recordPointColor;
			g_systemPassword.oneLevelMenu[g_systemPassword.recordIndex].backColor = g_systemPassword.oneLevelMenu[g_systemPassword.recordIndex].recordBackColor;
			
			Show_SystemPasswordOneRowOneLevelMenuContent(g_systemPassword.recordIndex);		
			
			g_systemPassword.oneLevelMenu[g_systemPassword.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemPassword.oneLevelMenu[g_systemPassword.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemPasswordOneRowOneLevelMenuContent(g_systemPassword.nowIndex);	
		}
		else
		{
			g_systemPassword.oneLevelMenu[g_systemPassword.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemPassword.oneLevelMenu[g_systemPassword.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemPasswordOneRowOneLevelMenuContent(g_systemPassword.nowIndex);
		}
		
		g_systemPassword.recordIndex = g_systemPassword.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordIndexUpdate
 * Description    : ����ֵ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordIndexUpdate( void )
{
	g_systemPassword.nowIndex++;
	g_systemPassword.nowIndex %= g_systemPassword.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordUpdateStatus( void )
{
	SystemPasswordIndexUpdate();
	
	g_systemPassword.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordCheckDataCycle
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemPasswordCheckDataCycle( void )
{
	uint8_t index = 0;
	uint8_t indexTemp = 0;
	
	index = GetSystemPasswordIndex(OBJECT_INITIAL_PASSWORD);
	if (index != 0xff)
	{
		if ( (TestManagerPassword(g_systemPassword.parameterData[index])==FAILED) &&\
			(TestUserPassword(g_systemPassword.parameterData[index])==FAILED) )
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemPasswordWarn[0]);	
					
			return FAILED;
		}
	}
	
	index = GetSystemPasswordIndex(INDEX_NEW_PASSWORD);
	if (index != 0xff)
	{
		indexTemp = index;
	}
	
	index = GetSystemPasswordIndex(INDEX_CONFIRM_NEW_PASSWORD);
	if (index != 0xff)
	{
		if ( strcmp(g_systemPassword.parameterData[indexTemp],g_systemPassword.parameterData[index]) )
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemPasswordWarn[1]);	
					
			return FAILED;
		}
		
		if ( strcmp(g_systemPassword.parameterData[indexTemp],"") == 0 )
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemPasswordWarn[3]);	
					
			return FAILED;
		}
	}
	
	SetPopWindowsInfomation(POP_PCM_CUE,1,&pSystemPasswordWarn[2]);	
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordKeyProcess( void )
{
	uint8_t index = g_systemPassword.nowIndex;
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
						switch ( g_systemPassword.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_systemPassword.putinNum,*GetPutinIntDataAddr(),g_systemPassword.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_systemPassword.putinNum,g_systemPassword.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_systemPassword.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_systemPassword.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				
				SystemPasswordUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(SYSTEM_SET);
						g_systemPassword.leavePage.flagLeavePage = SET;
						g_systemPassword.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						SystemPasswordUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
				
			case KEY_F3:
				if (PASSED == SystemPasswordCheckDataCycle() )
				{
					g_systemPassword.leavePage.flagSaveData = SET;
				}
				else
				{
					g_systemPassword.leavePage.flagSaveData = RESET;
				}
				g_systemPassword.leavePage.flagLeavePage = SET;				
				break;
			
			case KEY_F4:
				SetPage(SYSTEM_SET);
				g_systemPassword.leavePage.flagLeavePage = SET;
				g_systemPassword.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemPasswordLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemPasswordLeavePageCheckCycle( void )
{
	if (g_systemPassword.leavePage.flagLeavePage == SET)
	{
		if (g_systemPassword.leavePage.flagSaveData == SET)
		{
			SystemPasswordWriteParameter();		
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
