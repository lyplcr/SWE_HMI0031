/**
  ******************************************************************************
  * @file    AdvanceParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-22 09:23:33
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   �߼�����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "AdvanceParameter.h"
#include "UserLogin.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define ADVANCE_PARAMETER_MENU_CNT		6

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_MODEL_SELECT = 0,
	OBJECT_UNIT_SELECT,
	OBJECT_BACKUP_FACTORY,
	OBJECT_RESTORE_FACTORY,
	OBJECT_SYSTEM_INFOMATION,
	OBJECT_FIRMWARE_UPDATE,
}OBJECT_ADVANCE_PARAMETER_TypeDef;

typedef enum
{
	INDEX_MODEL_SELECT = 0,
	INDEX_UNIT_SELECT,
	INDEX_BACKUP_FACTORY,
	INDEX_RESTORE_FACTORY,
	INDEX_SYSTEM_INFOMATION,
	INDEX_FIRMWARE_UPDATE,
}INDEX_ADVANCE_PARAMETER_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[ADVANCE_PARAMETER_MENU_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[ADVANCE_PARAMETER_MENU_CNT];
	uint8_t menuCnt;	
	uint8_t rowNum;
	const char *pParameterNameArray[ADVANCE_PARAMETER_MENU_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	LEAVE_PAGE_TypeDef leavePage;						
}ADVANCE_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pAdvanceParameterName[] =
{
	"1������ѡ��",
	"2����λѡ��",
	"3��ϵͳ����",
	"4��ϵͳ��ԭ",
	"5��ϵͳ��Ϣ",
	"6���̼�����",
};

const char * const pFirmwareUpdateWarn[] =
{
	"���� [�̼�����] ����ʧ�ܣ�",
};	

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADVANCE_PARAMETER_TypeDef g_advanceParameter;

/* Private function prototypes -----------------------------------------------*/
static void AdvanceParameterInit( void );
static void AdvanceParameterConfig( void );
static void GUI_AdvanceParameter( void );
static void AdvanceParameterMoveIndexProcess( void );
static void AdvanceParameterMoveCursorProcess( void );
static void AdvanceParameterKeyProcess( void );
static void AdvanceParameterLeavePageCheckCycle( void );
static void LoadFirmwareUpdatePage( void );


/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadAdvanceParameterPage
 * Description    : �߼�����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadAdvanceParameterPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	AdvanceParameterInit();
	
	/* �������� */
	AdvanceParameterConfig();
	
	/* ��GUI��� */
	GUI_AdvanceParameter();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_advanceParameter.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* �ƶ�����ֵ */
		AdvanceParameterMoveIndexProcess();
		
		/* �ƶ���� */
		AdvanceParameterMoveCursorProcess();
		
		/* �������� */
		AdvanceParameterKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		AdvanceParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterInit( void )
{
	g_advanceParameter.recordIndex = 0xff;
	
	g_advanceParameter.isIndexMove = NO;
	
	g_advanceParameter.leavePage.flagLeavePage = RESET;
	g_advanceParameter.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterConfig
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterConfig( void )
{
	/* �Կ���� */
	g_advanceParameter.menuCnt = ADVANCE_PARAMETER_MENU_CNT;
	
	/* ���� */
	g_advanceParameter.rowNum = 5;
	
	/* ����ֵ */		
	g_advanceParameter.indexArray[INDEX_MODEL_SELECT]	 	= OBJECT_MODEL_SELECT;	
	g_advanceParameter.indexArray[INDEX_UNIT_SELECT] 		= OBJECT_UNIT_SELECT;		
	g_advanceParameter.indexArray[INDEX_BACKUP_FACTORY] 	= OBJECT_BACKUP_FACTORY;		
	g_advanceParameter.indexArray[INDEX_RESTORE_FACTORY] 	= OBJECT_RESTORE_FACTORY;			
	g_advanceParameter.indexArray[INDEX_SYSTEM_INFOMATION] 	= OBJECT_SYSTEM_INFOMATION;
	g_advanceParameter.indexArray[INDEX_FIRMWARE_UPDATE] 	= OBJECT_FIRMWARE_UPDATE;
	
	/* ���� */
	g_advanceParameter.pParameterNameArray[INDEX_MODEL_SELECT]			= pAdvanceParameterName[0];	
	g_advanceParameter.pParameterNameArray[INDEX_UNIT_SELECT] 			= pAdvanceParameterName[1];		
	g_advanceParameter.pParameterNameArray[INDEX_BACKUP_FACTORY] 		= pAdvanceParameterName[2];		
	g_advanceParameter.pParameterNameArray[INDEX_RESTORE_FACTORY] 		= pAdvanceParameterName[3];			
	g_advanceParameter.pParameterNameArray[INDEX_SYSTEM_INFOMATION] 	= pAdvanceParameterName[4];
	g_advanceParameter.pParameterNameArray[INDEX_FIRMWARE_UPDATE] 		= pAdvanceParameterName[5];
	
	/* ���� */
	g_advanceParameter.pTitle = "�߼�����";
}

/*------------------------------------------------------------
 * Function Name  : ConfigAdvanceParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigAdvanceParameterRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 60;
	const uint16_t INIT_START_Y = 81;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_advanceParameter.menuCnt; ++i)
	{
		g_advanceParameter.oneLevelMenu[i].x = startX;
		g_advanceParameter.oneLevelMenu[i].y = startY;
		g_advanceParameter.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_advanceParameter.oneLevelMenu[i].backColor = COLOR_POINT;
		g_advanceParameter.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_advanceParameter.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_advanceParameter.oneLevelMenu[i].lenth = 320;
		g_advanceParameter.oneLevelMenu[i].width = 36;
		g_advanceParameter.oneLevelMenu[i].fontSize = 32;
		g_advanceParameter.oneLevelMenu[i].rowDistance = 37;
		g_advanceParameter.oneLevelMenu[i].columnDistance = 0;
		
		if (i == g_advanceParameter.rowNum - 1)
		{
			startX += g_advanceParameter.oneLevelMenu[i].lenth + 40;
			startY = INIT_START_Y;
		}
		else
		{
			startY += g_advanceParameter.oneLevelMenu[i].rowDistance + g_advanceParameter.oneLevelMenu[i].width;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_AdvanceParameterDrawOneRectangleFrame
 * Description    : һ�н���GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_AdvanceParameterDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_advanceParameter.oneLevelMenu[index].x;
	const uint16_t y = g_advanceParameter.oneLevelMenu[index].y;
	const uint16_t lenth = g_advanceParameter.oneLevelMenu[index].lenth;
	const uint16_t width = g_advanceParameter.oneLevelMenu[index].width;
	const uint16_t pointColor = g_advanceParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_advanceParameter.oneLevelMenu[index].backColor;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr32At(x+16,y+1,pointColor,backColor,g_advanceParameter.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_AdvanceParameterRectangleFrame
 * Description    : ������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_AdvanceParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_advanceParameter.menuCnt; ++i)
	{
		GUI_AdvanceParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_AdvanceParameter
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_AdvanceParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_advanceParameter.pTitle);
	
	ConfigAdvanceParameterRectangleFrameCoordinate();
	
	GUI_AdvanceParameterRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_advanceParameter.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_advanceParameter.rowNum;
	indexObj.colNum = 2;
	indexObj.sumNum = g_advanceParameter.menuCnt;
	indexObj.pNowIndex = &g_advanceParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_advanceParameter.nowIndex != g_advanceParameter.recordIndex)
	{		
		g_advanceParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterMoveCursorProcess( void )
{
	if (g_advanceParameter.isIndexMove == YES)
	{		
		if (g_advanceParameter.recordIndex != 0xff)
		{
			g_advanceParameter.oneLevelMenu[g_advanceParameter.recordIndex].pointColor = g_advanceParameter.oneLevelMenu[g_advanceParameter.recordIndex].recordPointColor;
			g_advanceParameter.oneLevelMenu[g_advanceParameter.recordIndex].backColor = g_advanceParameter.oneLevelMenu[g_advanceParameter.recordIndex].recordBackColor;
			
			GUI_AdvanceParameterDrawOneRectangleFrame(g_advanceParameter.recordIndex);
			
			g_advanceParameter.oneLevelMenu[g_advanceParameter.nowIndex].pointColor = COLOR_POINT;
			g_advanceParameter.oneLevelMenu[g_advanceParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_AdvanceParameterDrawOneRectangleFrame(g_advanceParameter.nowIndex);
		}
		else
		{
			g_advanceParameter.oneLevelMenu[g_advanceParameter.nowIndex].pointColor = COLOR_POINT;
			g_advanceParameter.oneLevelMenu[g_advanceParameter.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_AdvanceParameterDrawOneRectangleFrame(g_advanceParameter.nowIndex);
		}
		
		g_advanceParameter.recordIndex = g_advanceParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterShortcutCheckCycle
 * Description    : ��ݼ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue>0) && (keyValue<=g_advanceParameter.menuCnt))
	{
		g_advanceParameter.leavePage.flagLeavePage = SET;
		g_advanceParameter.leavePage.flagSaveData = SET;
		
		g_advanceParameter.nowIndex = keyValue - 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				g_advanceParameter.leavePage.flagLeavePage = SET;
				g_advanceParameter.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_advanceParameter.leavePage.flagLeavePage = SET;
				g_advanceParameter.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	AdvanceParameterShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : AdvanceParameterLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AdvanceParameterLeavePageCheckCycle( void )
{
	if (g_advanceParameter.leavePage.flagLeavePage == SET)
	{
		if (g_advanceParameter.leavePage.flagSaveData == SET)
		{
			switch ( g_advanceParameter.indexArray[g_advanceParameter.nowIndex] )
			{
				case OBJECT_MODEL_SELECT:
					SetPasswordType(MANAGER_PASSWORD);
					SetPasswordPutinSourcePage(ADVANCE_PARAMETER_SET_PAGE);
					SetPasswordPutinTargetPage(MODEL_SELECT_PAGE);
					SetPage(USER_LOGIN_PAGE);
					break;
				case OBJECT_UNIT_SELECT:
					SetPasswordType(MANAGER_PASSWORD);
					SetPasswordPutinSourcePage(ADVANCE_PARAMETER_SET_PAGE);
					SetPasswordPutinTargetPage(UNIT_SELECT_PAGE);
					SetPage(USER_LOGIN_PAGE);
					break;
				case OBJECT_BACKUP_FACTORY:
					SetPage(SYSTEM_BACKUP_PAGE);
					break;
				case OBJECT_RESTORE_FACTORY:
					SetPage(SYSTEM_RESTORE_PAGE);
					break;
				case OBJECT_SYSTEM_INFOMATION:
					SetPage(SYSTEM_INFOMATION_PAGE);
					break;
				case OBJECT_FIRMWARE_UPDATE:
					LoadFirmwareUpdatePage();
					break;
			}
		}
		else
		{
			SetPage(SYSTEM_SET);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SetAutoUpdatePassword
 * Description    : �����Զ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus SetAutoUpdatePassword( void )
{
	TestStatus status = FAILED;
	uint32_t RTC_BKP_DATA = 0;
	
	bsp_InitRTC();
	
	WriteToRTC_BKP_DR(0,PRIVATE_PASSWORD);
	
	RTC_BKP_DATA = ReadRTC_BKP_DR(0);
	
	if (RTC_BKP_DATA == PRIVATE_PASSWORD)
	{
		status = PASSED;
	}
	
	bsp_DeInitRTC();
	
	RTC_BKP_DATA = ReadRTC_BKP_DR(0);
	
	return status;
}

/*------------------------------------------------------------
 * Function Name  : LoadFirmwareUpdatePage
 * Description    : �����Զ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadFirmwareUpdatePage( void )
{	
	if (SetAutoUpdatePassword() == FAILED)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pFirmwareUpdateWarn[0]);
		
		PopWindowsProcessCycle();
		
		g_advanceParameter.leavePage.flagLeavePage = SET;
		g_advanceParameter.leavePage.flagSaveData = RESET;
		
		return;
	}
	
	SystemSoftwareReset();
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
