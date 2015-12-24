/**
  ******************************************************************************
  * @file    SystemBackup.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-23 15:34:38
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ϵͳ����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "SystemBackup.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_PARAMETER_CNT				2
#define MAX_PARAMETER_PUTIN_BIT			8

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_SYSTEM_PASSWORD = 0,
	OBJECT_SYSTEM_TIME,
}OBJECT_SYSTEM_BACKUP_TypeDef;

typedef enum
{
	INDEX_SYSTEM_PASSWORD = 0,
	INDEX_SYSTEM_TIME,
}INDEX_SYSTEM_BACKUP_TypeDef;

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
}SYSTEM_BACKUP_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pSystemBackupName[] =
{
	"ϵͳ���룺",
	"ϵͳʱ�䣺",
};

const char * const SystemBackupWarn[] = 
{
	"ϵͳ�����������",		//0
	"ȷ�ϱ���ϵͳ��",			//1
	"���ѳɹ�����ϵͳ��",		//2
	"����ȡ������ϵͳ��",		//3
	"����ϵͳʧ�ܣ�",			//4
	"�����U�̣�",				//5
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SYSTEM_BACKUP_TypeDef g_systemBackup;

/* Private function prototypes -----------------------------------------------*/
static void SystemBackupInit( void );
static void SystemBackupParameterConfig( void );
static void SystemBackupReadParameter( void );
static void GUI_SystemBackup( void );
static void Traverse_SystemBackup( void );
static void SystemBackupPutinProcess( void );
static void SystemBackupShortcutCycleTask( void );
static void SystemBackupMoveIndexProcess( void );
static void SystemBackupMoveCursorProcess( void );
static void SystemBackupKeyProcess( void );
static void SystemBackupLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadSystemBackupPage
 * Description    : ϵͳ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemBackupPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	SystemBackupInit();
	
	/* �������� */
	SystemBackupParameterConfig();
	
	/* ��ȡ���� */
	SystemBackupReadParameter();
	
	/* ��GUI��� */
	GUI_SystemBackup();
	
	/* ���� */
	Traverse_SystemBackup();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_systemBackup.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		SystemBackupPutinProcess();
		
		/* �ƶ�����ֵ */
		SystemBackupMoveIndexProcess();
		
		/* �ƶ���� */
		SystemBackupMoveCursorProcess();
		
		/* ��ݲ˵� */
		SystemBackupShortcutCycleTask();
		
		/* �������� */
		SystemBackupKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		SystemBackupLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupInit( void )
{
	g_systemBackup.isIndexMove = NO;	
	g_systemBackup.nowIndex = 0;
	g_systemBackup.recordIndex = 0xff;
	g_systemBackup.refreshShortcut = ENABLE;
	g_systemBackup.leavePage.flagLeavePage = RESET;
	g_systemBackup.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupParameterConfig( void )
{
	/* ���� */
	g_systemBackup.curParameterNum = 1;
	
	/* ����ֵ */
	g_systemBackup.indexArray[INDEX_SYSTEM_PASSWORD] 		= OBJECT_SYSTEM_PASSWORD;
	g_systemBackup.indexArray[INDEX_SYSTEM_TIME] 			= OBJECT_SYSTEM_TIME;
	
	/* �������� */
	g_systemBackup.pParameterNameArray[INDEX_SYSTEM_PASSWORD] 	= pSystemBackupName[0];
	g_systemBackup.pParameterNameArray[INDEX_SYSTEM_TIME] 		= pSystemBackupName[1];
	
	/* ���� */
	g_systemBackup.pTitle = "ϵͳ����";
	
	/* ���ݱ������� */
	g_systemBackup.oneLevelMenu[INDEX_SYSTEM_PASSWORD].saveType = TYPE_INT;
	g_systemBackup.oneLevelMenu[INDEX_SYSTEM_TIME].saveType 	= TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : GetSystemBackupIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetSystemBackupIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		if (g_systemBackup.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupReadParameter( void )
{
	uint8_t index = 0;

	index = GetSystemBackupIndex(OBJECT_SYSTEM_PASSWORD);
	if (index != 0xff)
	{
		g_systemBackup.parameterData[index][0] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigSystemBackupParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSystemBackupParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 150;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		g_systemBackup.oneLevelMenu[i].x = startX;
		g_systemBackup.oneLevelMenu[i].y = startY;
		g_systemBackup.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_systemBackup.oneLevelMenu[i].backColor = COLOR_BACK;
		g_systemBackup.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_systemBackup.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_systemBackup.oneLevelMenu[i].lenth = 102;
		g_systemBackup.oneLevelMenu[i].width = 30;
		g_systemBackup.oneLevelMenu[i].fontSize = 24;
		g_systemBackup.oneLevelMenu[i].rowDistance = 24;
		g_systemBackup.oneLevelMenu[i].columnDistance = 0;
		g_systemBackup.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_systemBackup.oneLevelMenu[i].width + g_systemBackup.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackupDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackupDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_systemBackup.oneLevelMenu[index].x;
	rectangle.y = g_systemBackup.oneLevelMenu[index].y;
	rectangle.lenth = g_systemBackup.oneLevelMenu[index].lenth;
	rectangle.width = g_systemBackup.oneLevelMenu[index].width;
	rectangle.lineWidth = g_systemBackup.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_systemBackup.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackupRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackupRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_systemBackup.curParameterNum; ++i)
	{
		GUI_SystemBackupDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackupDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackupDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_systemBackup.oneLevelMenu[index].x - 5 * 24;
	const uint16_t y = g_systemBackup.oneLevelMenu[index].y + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemBackup.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemBackup.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_systemBackup.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackupOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackupOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<MAX_PARAMETER_CNT; ++i)
	{
		GUI_SystemBackupDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackupTimeCue
 * Description    : ʱ����ʾ��Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackupTimeCue( void )
{	
	uint8_t index = 0;
	uint16_t x,y,pointColor,backColor;
	
	index = GetSystemBackupIndex(OBJECT_SYSTEM_TIME);
	if (index != 0xff)
	{
		x = g_systemBackup.oneLevelMenu[index].x + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
		y = g_systemBackup.oneLevelMenu[index].y + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
		pointColor = g_systemBackup.oneLevelMenu[index].pointColor;
		backColor = g_systemBackup.oneLevelMenu[index].backColor;
		
		GUI_ShowSystemDate(x,y,pointColor,backColor);
	}	
}
	
/*------------------------------------------------------------
 * Function Name  : GUI_SystemBackup
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SystemBackup( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_systemBackup.pTitle);
	
	ConfigSystemBackupParameterRectangleFrameCoordinate();
	
	GUI_SystemBackupRectangleFrame();
	
	GUI_SystemBackupOneLevelMenu();
	
	GUI_SystemBackupTimeCue();	
}

/*------------------------------------------------------------
 * Function Name  : Show_SystemBackupOneRowOneLevelMenuContent
 * Description    : ��ʾһ�в���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_SystemBackupOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_systemBackup.oneLevelMenu[index].x + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_systemBackup.oneLevelMenu[index].y + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_systemBackup.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_systemBackup.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_systemBackup.oneLevelMenu[index].lenth - 2 * g_systemBackup.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_systemBackup.oneLevelMenu[index].width - 2 * g_systemBackup.oneLevelMenu[index].lineWidth - 2;
	char showBuff[MAX_PARAMETER_PUTIN_BIT+1];

	lcd_fill(x,y,lenth,width,backColor);
	
	StrFillSpecifyChar(g_systemBackup.parameterData[index],showBuff,'*');
	
	GUI_DispStr24At(x,y,pointColor,backColor,showBuff);
}

/*------------------------------------------------------------
 * Function Name  : Traverse_SystemBackup
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_SystemBackup( void )
{
	uint8_t index = 0;
	
	index = GetSystemBackupIndex(OBJECT_SYSTEM_PASSWORD);
	if (index != 0xff)
	{
		Show_SystemBackupOneRowOneLevelMenuContent(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupPutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_systemBackup.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_systemBackup.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_systemBackup.oneLevelMenu[index].x + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_systemBackup.oneLevelMenu[index].y + g_systemBackup.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_systemBackup.oneLevelMenu[index].lenth - 2 * g_systemBackup.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_systemBackup.oneLevelMenu[index].width - 2 * g_systemBackup.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_systemBackup.putinNum;
	pPutin->SaveType = g_systemBackup.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_systemBackup.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = ENABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_systemBackup.refreshShortcut == ENABLE)
	{
		g_systemBackup.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F2 | SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[1] = pTwoLevelMenu[82];
		pShortCut->pContent[2] = pTwoLevelMenu[43];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupMoveIndexProcess( void )
{	
	g_systemBackup.isIndexMove = NO;
	
	if (g_systemBackup.nowIndex != g_systemBackup.recordIndex)
	{		
		g_systemBackup.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupMoveCursorProcess( void )
{
	if (g_systemBackup.isIndexMove == YES)
	{		
		if (g_systemBackup.recordIndex != 0xff)
		{
			g_systemBackup.oneLevelMenu[g_systemBackup.recordIndex].pointColor = g_systemBackup.oneLevelMenu[g_systemBackup.recordIndex].recordPointColor;
			g_systemBackup.oneLevelMenu[g_systemBackup.recordIndex].backColor = g_systemBackup.oneLevelMenu[g_systemBackup.recordIndex].recordBackColor;
			
			Show_SystemBackupOneRowOneLevelMenuContent(g_systemBackup.recordIndex);		
			
			g_systemBackup.oneLevelMenu[g_systemBackup.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemBackup.oneLevelMenu[g_systemBackup.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemBackupOneRowOneLevelMenuContent(g_systemBackup.nowIndex);	
		}
		else
		{
			g_systemBackup.oneLevelMenu[g_systemBackup.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_systemBackup.oneLevelMenu[g_systemBackup.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_SystemBackupOneRowOneLevelMenuContent(g_systemBackup.nowIndex);
		}
		
		g_systemBackup.recordIndex = g_systemBackup.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupUpdateStatus( void )
{
	g_systemBackup.recordIndex = 0xff;
	
	g_systemBackup.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupUSBProcess
 * Description    : ������U��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemBackupUSBProcess( void )
{
	FRESULT result[3];
	
	if (Get_USB_Status() == ERROR)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[5]);	
					
		return FAILED;
	}
	
	pcm_save();
	prm_save();
	prv_save();
	
	result[0] = pcm_backup(BACKUP_USB);
	result[1] = prm_backup(BACKUP_USB);
	result[2] = prv_backup(BACKUP_USB);
	
	if ((result[0]==FR_OK) && (result[1]==FR_OK) && (result[2]==FR_OK))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[2]);	
	}
	else
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[4]);

		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupSDProcess
 * Description    : ������SD
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemBackupSDProcess( void )
{
	FRESULT result[3];
	
	pcm_save();
	prm_save();
	prv_save();
	
	result[0] = pcm_backup(BACKUP_SD);
	result[1] = prm_backup(BACKUP_SD);
	result[2] = prv_backup(BACKUP_SD);
	
	if ((result[0]==FR_OK) && (result[1]==FR_OK) && (result[2]==FR_OK))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[2]);	
	}
	else
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[4]);

		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupCheckSystemPasswordProcess
 * Description    : ���ϵͳ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SystemBackupCheckSystemPasswordProcess( void )
{
	uint8_t index = 0;
	
	index = GetSystemBackupIndex(OBJECT_SYSTEM_PASSWORD);
	if (index != 0xff)
	{
		if ((TestManagerPassword(g_systemBackup.parameterData[index])==PASSED) ||\
			(TestUserPassword(g_systemBackup.parameterData[index])==PASSED))
		{
			return PASSED;
		}
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupKeyProcess( void )
{
	uint8_t index = g_systemBackup.nowIndex;
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
						switch ( g_systemBackup.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_systemBackup.putinNum,*GetPutinIntDataAddr(),g_systemBackup.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_systemBackup.putinNum,g_systemBackup.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_systemBackup.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_systemBackup.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				
				if (SystemBackupCheckSystemPasswordProcess() == FAILED)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[0]);	
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = RESET;
					break;
				}
				
				SystemBackupUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(ADVANCE_PARAMETER_SET_PAGE);
						g_systemBackup.leavePage.flagLeavePage = SET;
						g_systemBackup.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						SystemBackupUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
			
			case KEY_F2:				
				if (SystemBackupCheckSystemPasswordProcess() == FAILED)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[0]);	
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = RESET;
					break;
				}
				
				if (SystemBackupUSBProcess() == PASSED)
				{
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = SET;
				}
				else
				{
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = RESET;
				}
				break;
				
			case KEY_F3:
				if (SystemBackupCheckSystemPasswordProcess() == FAILED)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&SystemBackupWarn[0]);
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = RESET;
					break;
				}
				
				if (SystemBackupSDProcess() == PASSED)
				{
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = SET;
				}
				else
				{
					g_systemBackup.leavePage.flagLeavePage = SET;
					g_systemBackup.leavePage.flagSaveData = RESET;
				}
				break;
			
			case KEY_F4:
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				g_systemBackup.leavePage.flagLeavePage = SET;
				g_systemBackup.leavePage.flagSaveData = RESET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemBackupLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SystemBackupLeavePageCheckCycle( void )
{
	if (g_systemBackup.leavePage.flagLeavePage == SET)
	{
		if (g_systemBackup.leavePage.flagSaveData == SET)
		{
			return;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
