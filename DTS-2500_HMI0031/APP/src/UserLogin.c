/**
  ******************************************************************************
  * @file    UserLogin.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-21 14:07:49
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   �û���¼����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "UserLogin.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				1
#define MAX_PARAMETER_PUTIN_BIT			8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_PASSWORD = 0,
}OBJECT_TIME_DATE_TypeDef;

typedef enum
{
	INDEX_PASSWORD = 0,
}INDEX_TIME_DATE_TypeDef;

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
	PASSWORD_TYPE_TypeDef passwordType;				//��������
	TestStatus userStatus;							//��¼�û�����״̬
	TestStatus managerStatus;						//��¼����Ա����״̬
	PAGE_NAME_TypeDef sourcePage;					//Դҳ
	PAGE_NAME_TypeDef targetPage;					//Ŀ��ҳ	
	BoolStatus verifyOK;							//��֤�ɹ�
}USER_LOGIN_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pPasswordName[] =
{
	"����Ա���룺",	
	"����Ա���룺",
};

const char * const pPasswordWarn[] = 
{
	"����Ա�����������",		//0
	"����Ա�����������",		//1
};


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static USER_LOGIN_TypeDef g_userLogin;

/* Private function prototypes -----------------------------------------------*/
static void LoadPasswordPage( void );
static void PasswordInit( void );
static void PasswordParameterConfig( void );
static void PasswordReadParameter( void );
static void GUI_Password( void );
static void Traverse_Password( void );
static void PasswordPutinProcess( void );
//static void PasswordShortcutCycleTask( void );
static void PasswordMoveIndexProcess( void );
static void PasswordMoveCursorProcess( void );
static void PasswordUpdateStatus( void );
static void PasswordKeyProcess( void );
static void PasswordLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : PrestrainLoadPasswordPage
 * Description    : Ԥ��������ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PrestrainLoadPasswordPage( void )
{
	if (g_userLogin.passwordType == MANAGER_PASSWORD)
	{
		if (g_userLogin.managerStatus == PASSED)
		{
			SetPage(g_userLogin.targetPage);
			
			return;
		}
	}
	else
	{
		if (g_userLogin.userStatus == PASSED)
		{
			SetPage(g_userLogin.targetPage);
			
			return;
		}
	}
	
	LoadPasswordPage();
}

/*------------------------------------------------------------
 * Function Name  : LoadPasswordPage
 * Description    : ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadPasswordPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	PasswordInit();
	
	/* �������� */
	PasswordParameterConfig();
	
	/* ��ȡ���� */
	PasswordReadParameter();
	
	/* ��GUI��� */
	GUI_Password();
	
	/* ���� */
	Traverse_Password();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_userLogin.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PasswordPutinProcess();
		
		/* �ƶ�����ֵ */
		PasswordMoveIndexProcess();
		
		/* �ƶ���� */
		PasswordMoveCursorProcess();
		
		/* ��ݲ˵� */
//		PasswordShortcutCycleTask();
		
		/* �������� */
		PasswordKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		PasswordLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SetPasswordType
 * Description    : ���õ�¼��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPasswordType( PASSWORD_TYPE_TypeDef passwordType )
{
	g_userLogin.passwordType = passwordType;
}

/*------------------------------------------------------------
 * Function Name  : SetPasswordPutinSourcePage
 * Description    : ��������������Դҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPasswordPutinSourcePage( PAGE_NAME_TypeDef sourcePage )
{
	g_userLogin.sourcePage = sourcePage;
}

/*------------------------------------------------------------
 * Function Name  : SetPasswordPutinTargetPage
 * Description    : ������������Ŀ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPasswordPutinTargetPage( PAGE_NAME_TypeDef targetPage )
{
	g_userLogin.targetPage = targetPage;
}

/*------------------------------------------------------------
 * Function Name  : PasswordInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordInit( void )
{
	g_userLogin.isIndexMove = NO;		
	g_userLogin.recordIndex = 0xff;
	g_userLogin.refreshShortcut = ENABLE;
	g_userLogin.leavePage.flagLeavePage = RESET;
	g_userLogin.leavePage.flagSaveData = RESET;
	g_userLogin.verifyOK = NO;
}

/*------------------------------------------------------------
 * Function Name  : PasswordParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordParameterConfig( void )
{
	/* �Կ���� */
	g_userLogin.curParameterNum = MAX_PARAMETER_CNT;
	
	/* ����ֵ */
	g_userLogin.indexArray[INDEX_PASSWORD] 		= OBJECT_PASSWORD;
	
	/* �������� */
	if (g_userLogin.passwordType == MANAGER_PASSWORD)
	{
		g_userLogin.pParameterNameArray[INDEX_PASSWORD] 	= pPasswordName[1];
	}
	else
	{
		g_userLogin.pParameterNameArray[INDEX_PASSWORD] 	= pPasswordName[0];
	}

	/* ���� */
	g_userLogin.pTitle = "�û���¼";
	
	/* ���ݱ������� */
	g_userLogin.oneLevelMenu[INDEX_PASSWORD].saveType 		= TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : GetPasswordIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetPasswordIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_userLogin.curParameterNum; ++i)
	{
		if (g_userLogin.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : PasswordReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetPasswordIndex(OBJECT_PASSWORD);
	
	if (index != 0xff)
	{
		g_userLogin.parameterData[index][0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigPasswordRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigPasswordRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 229;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_userLogin.curParameterNum; ++i)
	{
		g_userLogin.oneLevelMenu[i].x = startX;
		g_userLogin.oneLevelMenu[i].y = startY;
		g_userLogin.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_userLogin.oneLevelMenu[i].backColor = COLOR_BACK;
		g_userLogin.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_userLogin.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_userLogin.oneLevelMenu[i].lenth = 102;
		g_userLogin.oneLevelMenu[i].width = 30;
		g_userLogin.oneLevelMenu[i].fontSize = 24;
		g_userLogin.oneLevelMenu[i].rowDistance = 48;
		g_userLogin.oneLevelMenu[i].columnDistance = 48;
		g_userLogin.oneLevelMenu[i].lineWidth = 2;
		
		startX += g_userLogin.oneLevelMenu[i].lenth + g_userLogin.oneLevelMenu[i].columnDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_PasswordDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PasswordDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_userLogin.oneLevelMenu[index].x;
	rectangle.y = g_userLogin.oneLevelMenu[index].y;
	rectangle.lenth = g_userLogin.oneLevelMenu[index].lenth;
	rectangle.width = g_userLogin.oneLevelMenu[index].width;
	rectangle.lineWidth = g_userLogin.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_userLogin.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_PasswordRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PasswordRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_userLogin.curParameterNum; ++i)
	{
		GUI_PasswordDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_PasswordDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PasswordDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_userLogin.oneLevelMenu[index].x - 144;
	const uint16_t y = g_userLogin.oneLevelMenu[index].y + 3;
	const uint16_t pointColor = g_userLogin.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_userLogin.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_userLogin.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_PasswordOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PasswordOneLevelMenu( void )
{	
	uint8_t index = 0;
	
	index = GetPasswordIndex(OBJECT_PASSWORD);
	if (index != 0xff)
	{
		GUI_PasswordDrawOneRowOneLevelMenu(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_Password
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_Password( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_userLogin.pTitle);
	
	ConfigPasswordRectangleFrameCoordinate();
	
	GUI_PasswordRectangleFrame();
	
	GUI_PasswordOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_PasswordDataOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_PasswordDataOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_userLogin.oneLevelMenu[index].x + g_userLogin.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_userLogin.oneLevelMenu[index].y + g_userLogin.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_userLogin.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_userLogin.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_userLogin.oneLevelMenu[index].lenth - 2 * g_userLogin.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_userLogin.oneLevelMenu[index].width - 2 * g_userLogin.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_userLogin.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_PasswordDataOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_PasswordDataOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_userLogin.curParameterNum; ++i)
	{
		Show_PasswordDataOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_Password
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_Password( void )
{
	Show_PasswordDataOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : PasswordPutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_userLogin.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_userLogin.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_userLogin.oneLevelMenu[index].x + g_userLogin.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_userLogin.oneLevelMenu[index].y + g_userLogin.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_userLogin.oneLevelMenu[index].lenth - 2 * g_userLogin.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_userLogin.oneLevelMenu[index].width - 2 * g_userLogin.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_userLogin.putinNum;
	pPutin->SaveType = g_userLogin.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_userLogin.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = ENABLE;
	
	KeyPutinChars(pPutin);
}

///*------------------------------------------------------------
// * Function Name  : PasswordShortcutCycleTask
// * Description    : ��ݲ˵�����
// * Input          : None
// * Output         : None
// * Return         : None
// *------------------------------------------------------------*/
//static void PasswordShortcutCycleTask( void )
//{
//	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
//	
//	if (g_userLogin.refreshShortcut == ENABLE)
//	{
//		g_userLogin.refreshShortcut = DISABLE;
//			
//		pShortCut->status = SHOW_F3 | SHOW_F4;
//		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
//		pShortCut->backColor = COLOR_SHORTCUT_BACK;
//		pShortCut->pContent[2] = pTwoLevelMenu[80];
//		pShortCut->pContent[3] = pTwoLevelMenu[58];
//		
//		ShortcutMenuTask(pShortCut);
//	}
//}

/*------------------------------------------------------------
 * Function Name  : PasswordMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordMoveIndexProcess( void )
{	
	g_userLogin.isIndexMove = NO;
	
	if (g_userLogin.nowIndex != g_userLogin.recordIndex)
	{		
		g_userLogin.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : PasswordMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordMoveCursorProcess( void )
{
	if (g_userLogin.isIndexMove == YES)
	{		
		if (g_userLogin.recordIndex != 0xff)
		{
			g_userLogin.oneLevelMenu[g_userLogin.recordIndex].pointColor = g_userLogin.oneLevelMenu[g_userLogin.recordIndex].recordPointColor;
			g_userLogin.oneLevelMenu[g_userLogin.recordIndex].backColor = g_userLogin.oneLevelMenu[g_userLogin.recordIndex].recordBackColor;
			
			Show_PasswordDataOneRowOneLevelMenuContent(g_userLogin.recordIndex);		
			
			g_userLogin.oneLevelMenu[g_userLogin.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_userLogin.oneLevelMenu[g_userLogin.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_PasswordDataOneRowOneLevelMenuContent(g_userLogin.nowIndex);	
		}
		else
		{
			g_userLogin.oneLevelMenu[g_userLogin.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_userLogin.oneLevelMenu[g_userLogin.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_PasswordDataOneRowOneLevelMenuContent(g_userLogin.nowIndex);
		}
		
		g_userLogin.recordIndex = g_userLogin.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : PasswordUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordUpdateStatus( void )
{
	g_userLogin.recordIndex = 0xff;
	
	g_userLogin.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : GetPasswordPutinStatus
 * Description    : ��ȡ��������״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus GetPasswordPutinStatus( void )
{
	return g_userLogin.verifyOK;
}

/*------------------------------------------------------------
 * Function Name  : PasswordProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus PasswordProcess( const char *password )
{
	switch ( g_userLogin.passwordType )
	{
		case USER_PASSWORD:
		case USER_OR_MANAGER_PASSWORD:
			if ((TestManagerPassword(password)==PASSED)	|| (TestUserPassword(password)==PASSED))
			{
				g_userLogin.userStatus = PASSED;
				
				return PASSED;
			}
			
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pPasswordWarn[0]);
			return PASSED;
		
		case MANAGER_PASSWORD:
			if (TestManagerPassword(password) == PASSED)
			{
				g_userLogin.managerStatus = PASSED;
				
				return PASSED;
			}
			
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pPasswordWarn[1]);
			break;
		
		default:
			break;
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : PasswordKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordKeyProcess( void )
{
	uint8_t index = g_userLogin.nowIndex;
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
						switch ( g_userLogin.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_userLogin.putinNum,*GetPutinIntDataAddr(),g_userLogin.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_userLogin.putinNum,g_userLogin.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_userLogin.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_userLogin.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				
				index = GetPasswordIndex(OBJECT_PASSWORD);
				if (index != 0xff)
				{
					PasswordProcess(g_userLogin.parameterData[index]);
					
					g_userLogin.leavePage.flagLeavePage = SET;
					g_userLogin.leavePage.flagSaveData = SET;	
				}
				
				PasswordUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(g_userLogin.sourcePage);
						g_userLogin.leavePage.flagLeavePage = SET;
						g_userLogin.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						PasswordUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : PasswordLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PasswordLeavePageCheckCycle( void )
{
	if (g_userLogin.leavePage.flagLeavePage == SET)
	{
		if (g_userLogin.leavePage.flagSaveData == SET)
		{
			g_userLogin.verifyOK = YES;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
