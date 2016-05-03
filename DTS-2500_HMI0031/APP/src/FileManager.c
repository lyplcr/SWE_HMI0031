/**
  ******************************************************************************
  * @file    FileManager.c
  * @author  SY
  * @version V1.0.0
  * @date    2016-2-22 10:54:11
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   �ļ�����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define FILE_MANAGER_MENU_CNT			7

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_CHINESE_FONT = 0,
	OBJECT_ENGLISH_FONT,
	OBJECT_IMAGE,
	OBJECT_SYSTEM_FILE,
	OBJECT_TEST_REPORT,
	OBJECT_TEST_CURVE,
	OBJECT_ALL_FILE,
}OBJECT_SYSTEM_SET_TypeDef;

typedef enum
{
	INDEX_CHINESE_FONT = 0,
	INDEX_ENGLISH_FONT,
	INDEX_IMAGE,
	INDEX_SYSTEM_FILE,
	INDEX_TEST_REPORT,
	INDEX_TEST_CURVE,
	INDEX_ALL_FILE,
}INDEX_SYSTEM_SET_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[FILE_MANAGER_MENU_CNT];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[FILE_MANAGER_MENU_CNT];
	uint8_t sumCnt;	
	uint8_t rowNum;
	const char *pParameterNameArray[FILE_MANAGER_MENU_CNT];
	const char * pTitle;
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	LEAVE_PAGE_TypeDef leavePage;						
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
}FILE_MANAGER_TypeDef;
/* Private constants ---------------------------------------------------------*/
const char * const g_fileManagerNamePtr[] =
{
	"1�������ֿ�",		//0
	"2��Ӣ���ֿ�",		//1
	"3��ϵͳͼƬ",		//2
	"4��ϵͳ�ļ�",		//3
	"5�����鱨��",		//4
	"6����������",		//5
	"7��ȫ���ļ�",		//6
};

const char * const g_fileManagerWarningPtr[] =
{
	"δʶ������ֵ��",		//0
	"���ڵ���...",			//1
	"����ɹ���",			//2
	"����ʧ�ܣ�",			//3
	"���ڵ���...",			//4
	"�����ɹ���",			//5
	"����ʧ�ܣ�",			//6
	"�����U�̣�",			//7
	"����ɾ��...",			//8
	"ɾ���ɹ���",			//9
	"ɾ��ʧ�ܣ�",			//10
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FILE_MANAGER_TypeDef g_fileManager;

/* Private function prototypes -----------------------------------------------*/
static void FileManagerInit( void );
static void FileManagerConfig( void );
static void GUI_FileManager( void );
static void FileManagerShortcutCycleTask( void );
static void FileManagerMoveIndexProcess( void );
static void FileManagerMoveCursorProcess( void );
static void FileManagerShortcutCheckCycle( void );
static void FileManagerKeyProcess( void );
static void FileManagerLeavePageCheckCycle( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadFileManagerPage
 * Description    : �ļ�����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadFileManagerPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	FileManagerInit();
	
	/* �������� */
	FileManagerConfig();
	
	/* ��GUI��� */
	GUI_FileManager();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_fileManager.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* �ƶ�����ֵ */
		FileManagerMoveIndexProcess();
		
		/* �ƶ���� */
		FileManagerMoveCursorProcess();
		
		/* ��ݲ˵� */
		FileManagerShortcutCycleTask();
		
		/* �������� */
		FileManagerKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		FileManagerLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerInit( void )
{
	g_fileManager.recordIndex = 0xff;
	
	g_fileManager.isIndexMove = NO;
	g_fileManager.refreshShortcut = ENABLE;
	g_fileManager.leavePage.flagLeavePage = RESET;
	g_fileManager.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : FileManagerConfig
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerConfig( void )
{
	/* ���� */
	g_fileManager.pTitle = "�ļ�����";
	
	/* �Կ���� */
	g_fileManager.sumCnt = FILE_MANAGER_MENU_CNT;
	
	/* ���� */
	g_fileManager.rowNum = 5;
	
	/* ����ֵ */		
	g_fileManager.indexArray[INDEX_CHINESE_FONT] 	= OBJECT_CHINESE_FONT;	
	g_fileManager.indexArray[INDEX_ENGLISH_FONT] 	= OBJECT_ENGLISH_FONT;		
	g_fileManager.indexArray[INDEX_IMAGE] 			= OBJECT_IMAGE;		
	g_fileManager.indexArray[INDEX_SYSTEM_FILE] 	= OBJECT_SYSTEM_FILE;			
	g_fileManager.indexArray[INDEX_TEST_REPORT] 	= OBJECT_TEST_REPORT;
	g_fileManager.indexArray[INDEX_TEST_CURVE] 		= OBJECT_TEST_CURVE;
	g_fileManager.indexArray[INDEX_ALL_FILE] 		= OBJECT_ALL_FILE;
	
	/* ���� */
	g_fileManager.pParameterNameArray[INDEX_CHINESE_FONT]	= g_fileManagerNamePtr[0];	
	g_fileManager.pParameterNameArray[INDEX_ENGLISH_FONT] 	= g_fileManagerNamePtr[1];		
	g_fileManager.pParameterNameArray[INDEX_IMAGE] 			= g_fileManagerNamePtr[2];		
	g_fileManager.pParameterNameArray[INDEX_SYSTEM_FILE] 	= g_fileManagerNamePtr[3];			
	g_fileManager.pParameterNameArray[INDEX_TEST_REPORT] 	= g_fileManagerNamePtr[4];
	g_fileManager.pParameterNameArray[INDEX_TEST_CURVE] 	= g_fileManagerNamePtr[5];
	g_fileManager.pParameterNameArray[INDEX_ALL_FILE] 		= g_fileManagerNamePtr[6];
}

/*------------------------------------------------------------
 * Function Name  : ConfigFileManagerRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigFileManagerRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 60;
	const uint16_t INIT_START_Y = 81;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_fileManager.sumCnt; ++i)
	{
		g_fileManager.oneLevelMenu[i].x = startX;
		g_fileManager.oneLevelMenu[i].y = startY;
		g_fileManager.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_fileManager.oneLevelMenu[i].backColor = COLOR_POINT;
		g_fileManager.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_fileManager.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_fileManager.oneLevelMenu[i].lenth = 320;
		g_fileManager.oneLevelMenu[i].width = 36;
		g_fileManager.oneLevelMenu[i].fontSize = 32;
		g_fileManager.oneLevelMenu[i].rowDistance = 37;
		g_fileManager.oneLevelMenu[i].columnDistance = 0;
		
		if (i == g_fileManager.rowNum - 1)
		{
			startX += g_fileManager.oneLevelMenu[i].lenth + 40;
			startY = INIT_START_Y;
		}
		else
		{
			startY += g_fileManager.oneLevelMenu[i].rowDistance + g_fileManager.oneLevelMenu[i].width;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_FileManagerDrawOneRectangleFrame
 * Description    : һ�н���GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_FileManagerDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_fileManager.oneLevelMenu[index].x;
	const uint16_t y = g_fileManager.oneLevelMenu[index].y;
	const uint16_t lenth = g_fileManager.oneLevelMenu[index].lenth;
	const uint16_t width = g_fileManager.oneLevelMenu[index].width;
	const uint16_t pointColor = g_fileManager.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_fileManager.oneLevelMenu[index].backColor;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr32At(x+16,y+1,pointColor,backColor,g_fileManager.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_FileManagerRectangleFrame
 * Description    : ������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_FileManagerRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_fileManager.sumCnt; ++i)
	{
		GUI_FileManagerDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_FileManager
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_FileManager( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_fileManager.pTitle);
	
	ConfigFileManagerRectangleFrameCoordinate();
	
	GUI_FileManagerRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : FileManagerMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_fileManager.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_fileManager.rowNum;
	indexObj.colNum = 2;
	indexObj.sumNum = g_fileManager.sumCnt;
	indexObj.pNowIndex = &g_fileManager.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_fileManager.nowIndex != g_fileManager.recordIndex)
	{		
		g_fileManager.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerMoveCursorProcess( void )
{
	if (g_fileManager.isIndexMove == YES)
	{		
		if (g_fileManager.recordIndex != 0xff)
		{
			g_fileManager.oneLevelMenu[g_fileManager.recordIndex].pointColor = g_fileManager.oneLevelMenu[g_fileManager.recordIndex].recordPointColor;
			g_fileManager.oneLevelMenu[g_fileManager.recordIndex].backColor = g_fileManager.oneLevelMenu[g_fileManager.recordIndex].recordBackColor;
			
			GUI_FileManagerDrawOneRectangleFrame(g_fileManager.recordIndex);
			
			g_fileManager.oneLevelMenu[g_fileManager.nowIndex].pointColor = COLOR_POINT;
			g_fileManager.oneLevelMenu[g_fileManager.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_FileManagerDrawOneRectangleFrame(g_fileManager.nowIndex);
		}
		else
		{
			g_fileManager.oneLevelMenu[g_fileManager.nowIndex].pointColor = COLOR_POINT;
			g_fileManager.oneLevelMenu[g_fileManager.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_FileManagerDrawOneRectangleFrame(g_fileManager.nowIndex);
		}
		
		g_fileManager.recordIndex = g_fileManager.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_fileManager.refreshShortcut == ENABLE)
	{
		g_fileManager.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F1 | SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[0] = pTwoLevelMenu[15];
		pShortCut->pContent[2] = pTwoLevelMenu[87];
		pShortCut->pContent[3] = pTwoLevelMenu[78];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerShortcutCheckCycle
 * Description    : ��ݼ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue>0) && (keyValue<=g_fileManager.sumCnt))
	{	
		if (keyValue)
		{
			g_fileManager.nowIndex = keyValue - 1;
		}
		else
		{
			g_fileManager.nowIndex = g_fileManager.sumCnt - 1;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ShowFileNameCallBack
 * Description    : ��ʾ�ļ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ShowFileNameCallBack( const char *FolderNamePtr, const char *FileNamePtr )
{
	POP_WINDOWS_TypeDef *popWindowsPtr = GetPopWindowsAddr();
	uint16_t x = popWindowsPtr->x + 10;
	uint16_t y = popWindowsPtr->y + 70;
	
	GUI_DispStr24At(x,y,popWindowsPtr->pointColor,popWindowsPtr->backColor,"�ļ��У�");
	GUI_DispStr24At(x,y+30,popWindowsPtr->pointColor,popWindowsPtr->backColor,"�ļ�����");
	
	if (FolderNamePtr != NULL)
	{	
		uint16_t offsetX = 4 * popWindowsPtr->fontSize;
		
		lcd_fill(x+offsetX,y,popWindowsPtr->lenth-20-offsetX,popWindowsPtr->fontSize,popWindowsPtr->backColor);		
		GUI_DispStr24At(x+offsetX,y,popWindowsPtr->pointColor,popWindowsPtr->backColor,FolderNamePtr);
	}
	
	if (FileNamePtr != NULL)
	{
		uint16_t offsetX = 4 * popWindowsPtr->fontSize;
		
		y += 30;
		lcd_fill(x+offsetX,y,popWindowsPtr->lenth-20-offsetX,popWindowsPtr->fontSize,popWindowsPtr->backColor);		
		GUI_DispStr24At(x+offsetX,y,popWindowsPtr->pointColor,popWindowsPtr->backColor,FileNamePtr);
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerKeyProcess( void )
{
	ErrorStatus status;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F1:
				g_fileManager.leavePage.flagLeavePage = SET;
				g_fileManager.leavePage.flagSaveData = RESET;
			
				if (Get_USB_Status() == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[7]);
					
					return;
				}
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"ɾ���ļ�",1,&g_fileManagerWarningPtr[8]);		
				PopWindowsProcessCycle();
				
				switch ( g_fileManager.indexArray[g_fileManager.nowIndex] )
				{
					case OBJECT_CHINESE_FONT:
						status = DeleteFolderFromDevices("0:/hz",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ENGLISH_FONT:
						status = DeleteFolderFromDevices("0:/asc",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_IMAGE:
						status = DeleteFolderFromDevices("0:/image",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_SYSTEM_FILE:
						status = DeleteFolderFromDevices("0:/sys",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_REPORT:
						status = DeleteFolderFromDevices("0:/test",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_CURVE:
						status = DeleteFolderFromDevices("0:/curve",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ALL_FILE:
						status = DeleteFolderFromDevices("0:",GUI_ShowFileNameCallBack);
						break;
					default:
						SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[0]);
						return;
				}
				
				if (status == SUCCESS)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[9]);	
				}
				else
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[10]);
				}
				break;
			case KEY_F3:
				g_fileManager.leavePage.flagLeavePage = SET;
				g_fileManager.leavePage.flagSaveData = RESET;
			
				if (Get_USB_Status() == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[7]);
					
					return;
				}
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"�����ļ�",1,&g_fileManagerWarningPtr[1]);		
				PopWindowsProcessCycle();
				
				switch ( g_fileManager.indexArray[g_fileManager.nowIndex] )
				{
					case OBJECT_CHINESE_FONT:
						status = CopyFolderFromDevices("1:/hz","0:/hz",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ENGLISH_FONT:
						status = CopyFolderFromDevices("1:/asc","0:/asc",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_IMAGE:
						status = CopyFolderFromDevices("1:/image","0:/image",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_SYSTEM_FILE:
						status = CopyFolderFromDevices("1:/sys","0:/sys",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_REPORT:
						status = CopyFolderFromDevices("1:/test","0:/test",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_CURVE:
						status = CopyFolderFromDevices("1:/curve","0:/curve",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ALL_FILE:
						status = CopyFolderFromDevices("1:","0:",GUI_ShowFileNameCallBack);
						break;
					default:
						SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[0]);
						return;
				}
				
				if (status == SUCCESS)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[2]);	
				}
				else
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[3]);
				}
				break;
			case KEY_F4:
				g_fileManager.leavePage.flagLeavePage = SET;
				g_fileManager.leavePage.flagSaveData = RESET;
			
				if (Get_USB_Status() == ERROR)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[7]);
					
					return;
				}
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"�����ļ�",1,&g_fileManagerWarningPtr[4]);		
				PopWindowsProcessCycle();
				
				switch ( g_fileManager.indexArray[g_fileManager.nowIndex] )
				{
					case OBJECT_CHINESE_FONT:
						status = CopyFolderFromDevices("0:/hz","1:/hz",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ENGLISH_FONT:
						status = CopyFolderFromDevices("0:/asc","1:/asc",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_IMAGE:
						status = CopyFolderFromDevices("0:/image","1:/image",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_SYSTEM_FILE:
						status = CopyFolderFromDevices("0:/sys","1:/sys",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_REPORT:
						status = CopyFolderFromDevices("0:/test","1:/test",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_TEST_CURVE:
						status = CopyFolderFromDevices("0:/curve","1:/curve",GUI_ShowFileNameCallBack);
						break;
					case OBJECT_ALL_FILE:
						status = CopyFolderFromDevices("0:","1:",GUI_ShowFileNameCallBack);
						break;
					default:
						SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[0]);
						return;
				}
				
				if (status == SUCCESS)
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[5]);	
				}
				else
				{
					SetPopWindowsInfomation(POP_PCM_CUE,1,&g_fileManagerWarningPtr[6]);
				}
				break;
			case KEY_ESC:
				g_fileManager.leavePage.flagLeavePage = SET;
				g_fileManager.leavePage.flagSaveData = RESET;
				SetPage(ADVANCE_PARAMETER_SET_PAGE);
				break;
		}
	}
	
	FileManagerShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : FileManagerLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerLeavePageCheckCycle( void )
{
	if (g_fileManager.leavePage.flagLeavePage == SET)
	{
		if (g_fileManager.leavePage.flagSaveData == SET)
		{
			
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
