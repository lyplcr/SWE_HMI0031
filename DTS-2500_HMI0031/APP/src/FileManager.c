/**
  ******************************************************************************
  * @file    FileManager.c
  * @author  SY
  * @version V1.0.0
  * @date    2016-2-22 10:54:11
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   文件管理
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
	BoolStatus isIndexMove;							//索引值移动
	LEAVE_PAGE_TypeDef leavePage;						
	FunctionalState refreshShortcut;				//刷新快捷菜单
}FILE_MANAGER_TypeDef;
/* Private constants ---------------------------------------------------------*/
const char * const g_fileManagerNamePtr[] =
{
	"1、中文字库",		//0
	"2、英文字库",		//1
	"3、系统图片",		//2
	"4、系统文件",		//3
	"5、试验报告",		//4
	"6、试验曲线",		//5
	"7、全部文件",		//6
};

const char * const g_fileManagerWarningPtr[] =
{
	"未识别索引值！",		//0
	"正在导入...",			//1
	"导入成功！",			//2
	"导入失败！",			//3
	"正在导出...",			//4
	"导出成功！",			//5
	"导出失败！",			//6
	"请插入U盘！",			//7
	"正在删除...",			//8
	"删除成功！",			//9
	"删除失败！",			//10
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
 * Description    : 文件管理页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadFileManagerPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	FileManagerInit();
	
	/* 参数配置 */
	FileManagerConfig();
	
	/* 画GUI框架 */
	GUI_FileManager();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_fileManager.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		FileManagerMoveIndexProcess();
		
		/* 移动光标 */
		FileManagerMoveCursorProcess();
		
		/* 快捷菜单 */
		FileManagerShortcutCycleTask();
		
		/* 按键处理 */
		FileManagerKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		FileManagerLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : FileManagerInit
 * Description    : 初始化
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
 * Description    : 配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FileManagerConfig( void )
{
	/* 标题 */
	g_fileManager.pTitle = "文件管理";
	
	/* 试块个数 */
	g_fileManager.sumCnt = FILE_MANAGER_MENU_CNT;
	
	/* 行数 */
	g_fileManager.rowNum = 5;
	
	/* 索引值 */		
	g_fileManager.indexArray[INDEX_CHINESE_FONT] 	= OBJECT_CHINESE_FONT;	
	g_fileManager.indexArray[INDEX_ENGLISH_FONT] 	= OBJECT_ENGLISH_FONT;		
	g_fileManager.indexArray[INDEX_IMAGE] 			= OBJECT_IMAGE;		
	g_fileManager.indexArray[INDEX_SYSTEM_FILE] 	= OBJECT_SYSTEM_FILE;			
	g_fileManager.indexArray[INDEX_TEST_REPORT] 	= OBJECT_TEST_REPORT;
	g_fileManager.indexArray[INDEX_TEST_CURVE] 		= OBJECT_TEST_CURVE;
	g_fileManager.indexArray[INDEX_ALL_FILE] 		= OBJECT_ALL_FILE;
	
	/* 名称 */
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
 * Description    : 配置界面GUI坐标
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
 * Description    : 一行界面GUI
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
 * Description    : 画界面GUI
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
 * Description    : 界面GUI
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
 * Description    : 移动索引值
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
 * Description    : 移动光标处理
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
 * Description    : 快捷菜单任务
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
 * Description    : 快捷键检测
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
 * Description    : 显示文件名
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ShowFileNameCallBack( const char *FolderNamePtr, const char *FileNamePtr )
{
	POP_WINDOWS_TypeDef *popWindowsPtr = GetPopWindowsAddr();
	uint16_t x = popWindowsPtr->x + 10;
	uint16_t y = popWindowsPtr->y + 70;
	
	GUI_DispStr24At(x,y,popWindowsPtr->pointColor,popWindowsPtr->backColor,"文件夹：");
	GUI_DispStr24At(x,y+30,popWindowsPtr->pointColor,popWindowsPtr->backColor,"文件名：");
	
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
 * Description    : 按键处理
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
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"删除文件",1,&g_fileManagerWarningPtr[8]);		
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
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"拷贝文件",1,&g_fileManagerWarningPtr[1]);		
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
			
				SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"拷贝文件",1,&g_fileManagerWarningPtr[4]);		
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
 * Description    : 离开页检测
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
