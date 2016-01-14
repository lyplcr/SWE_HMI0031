/**
  ******************************************************************************
  * @file    gui.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   GUI������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "gui.h"
#include "active.h"


/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	WINDOWS_DIR_UP = 0,
	WINDOWS_DIR_DOWN,
	WINDOWS_DIR_MIDDLE,
}POP_WINDOWS_DIRECTION_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pWarn_windows[] = 
{
	"����λ������",						//0
	"����λ������",						//1
	"Ԥ��ײ������",						//2
	"��ѹͨ�����ٱ�����",				//3
	"����ͨ�����ٱ�����",				//4
	"����ָ��ִ�С�",					//5
	"�Ƿ�������",						//6
	"CRCУ��ʧ�ܡ�",						//7
	"��ѹͨ�������������",				//8
	"����ͨ�������������",				//9
	"δ֪����",						//10
	"�������еȴ�ʱ�䡣",				//11
	"�����������",					//12
	"��ѹͨ�������������",				//13
	"����ͨ�������������",				//14
	"��ѹͨ�����롣",					//15
	"����ͨ�����롣",					//16
	"������λͬʱ���¡�",				//17
	"��ѹͨ�����������������",			//18
	"����ͨ�����������������",			//19
	"δ֪���档",						//20
	"��ǰ�����ѻ�״̬��",				//21
	"���͡���ȡ��������ʧ�ܡ�",			//22
	"�͸���λ������",					//23
	"��ͣ��ť���¡�",					//24
	"�ŷ�����������档",				//25
};	// ������ʾ��(11������)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static TWO_LEVEL_MENU_TypeDef g_twoLevelMenu;
static SHORTCUT_TypeDef g_shortcutMenu;
static POP_WINDOWS_TypeDef g_popWindows;
static SELECT_WINDOWS_TypeDef g_selectWindows;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : GUI_DispStr16At
 * Description    : ��ʾ16������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr16At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font16( x0, y0,fc,bc,s,"song16.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DispStr20At
 * Description    : ��ʾ20������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr20At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font20( x0, y0,fc,bc,s,"song20.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DispStr24At
 * Description    : ��ʾ24������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr24At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font24( x0, y0,fc,bc,s,"song24.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DispStr32At
 * Description    : ��ʾ32������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr32At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font32( x0, y0,fc,bc,s,"song32.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DispStr24Ht
 * Description    : ��ʾ24�ź���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr24Ht(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font24( x0, y0,fc,bc,s,"heiti24.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DispStr32Ht
 * Description    : ��ʾ32�ź���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DispStr32Ht(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s)
{
	lcd_font32( x0, y0,fc,bc,s,"heiti32.zk");
}

/*------------------------------------------------------------
 * Function Name  : GUI_DrawRectangleFrame
 * Description    : �����ο�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawRectangleFrame( RECTANGLE_FRAME_TypeDef const *pRectangle )
{
	uint8_t i;
	RECTANGLE_FRAME_TypeDef rect = *pRectangle;
	
	for (i=0; i<rect.lineWidth; ++i)
	{
		lcd_draw_rect(rect.x,rect.y,rect.lenth,rect.width,rect.lineColor);
		
		rect.x++;
		rect.y++;
		rect.lenth -= 2;
		rect.width -= 2;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_DrawRectangleFrameWithTitle
 * Description    : ��������ľ��ο�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawRectangleFrameWithTitle( RECTANGLE_FRAME_WITH_TITLE_TypeDef const *pRectangle )
{
	uint8_t i;
	uint16_t fontLenth = 0;
	RECTANGLE_FRAME_WITH_TITLE_TypeDef rect = *pRectangle;
	
	/* ������� */
	lcd_fill(rect.x,rect.y,rect.lenth,rect.titleWidth+rect.contentWidth-rect.lineWidth,rect.contentBackColor);
	
	/* �������� */
	for (i=0; i<rect.lineWidth; ++i)
	{
		lcd_draw_rect(rect.x,rect.y,rect.lenth,rect.titleWidth,rect.lineColor);	
		
		rect.x++;
		rect.y++;
		rect.lenth -= 2;
		rect.titleWidth -= 2;
	}
	
	/* �������� */
	rect.x = pRectangle->x;
	rect.y = pRectangle->y;
	rect.lenth = pRectangle->lenth;
	rect.titleWidth = pRectangle->titleWidth;
	rect.y += rect.titleWidth - rect.lineWidth;
	
	for (i=0; i<rect.lineWidth; ++i)
	{
		lcd_draw_rect(rect.x,rect.y,rect.lenth,rect.contentWidth,rect.lineColor);	
		
		rect.x++;
		rect.y++;
		rect.lenth -= 2;
		rect.titleWidth -= 2;
	}
	
	/* ������ */
	rect.x = pRectangle->x + rect.lineWidth;
	rect.y = pRectangle->y + rect.lineWidth;
	rect.lenth = pRectangle->lenth;
	rect.titleWidth = pRectangle->titleWidth;
	
	lcd_fill(rect.x,rect.y,rect.lenth-2*rect.lineWidth,rect.titleWidth-2*rect.lineWidth,rect.titleBackColor);
	
	fontLenth = strlen(rect.pTitleContent) * (rect.titleFontSize >> 1);
	rect.x += (rect.lenth - fontLenth) >> 1;
	rect.y += ((rect.titleWidth - rect.titleFontSize - 2 * rect.lineWidth) >> 1);
	
	switch (rect.titleFontSize)
	{
		case 24:
			GUI_DispStr24Ht(rect.x,rect.y,rect.titlePointColor,rect.titleBackColor,rect.pTitleContent);
			break;
		
		case 32:
			GUI_DispStr32Ht(rect.x,rect.y,rect.titlePointColor,rect.titleBackColor,rect.pTitleContent);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterDrawTitleRectangleFrame
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawGeneralTitleRectangleFrame( const char * const pTitle )
{
	RECTANGLE_FRAME_WITH_TITLE_TypeDef rect;
	
	rect.x = 30;
	rect.y = 2;
	rect.lenth = 740;
	rect.titleWidth = 42;
	rect.contentWidth = 404;
	rect.lineWidth = 4;
	rect.lineColor = WHITE;
	rect.titlePointColor = WHITE;
	rect.titleBackColor = DARKBLUE;
	rect.pTitleContent = pTitle;
	rect.titleFontSize = 32;
	rect.contentBackColor = BLACK;
	
	GUI_DrawRectangleFrameWithTitle(&rect);
}

/*------------------------------------------------------------
 * Function Name  : TwoLevelMenuInit
 * Description    : �����˵���ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TwoLevelMenuInit( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	pMenu->recordIndex = 0xff;
	if (pMenu->nowIndex >= pMenu->nums)	/* ����ֵԽ�紦�� */
	{
		pMenu->nowIndex = 0;
	}
	
	if (pMenu->nums > MAX_TWO_LEVEL_MENU_SEGS)
	{
		pMenu->nums = MAX_TWO_LEVEL_MENU_SEGS;
	}
	
	pMenu->recordPointColor = pMenu->pointColor;
	pMenu->recordBackColor = pMenu->backColor;
	
	pMenu->isLeavePage = NO;
}

/*------------------------------------------------------------
 * Function Name  : JudgeWindowsDirection
 * Description    : �жϴ��ڷ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static POP_WINDOWS_DIRECTION_TypeDef JudgeWindowsDirection( TWO_LEVEL_MENU_TypeDef *pMenu )
{	
	if (pMenu->y + pMenu->nums * (pMenu->fontSize + pMenu->rowSpacing) < pMenu->maxDownY)		//δ�����±���
	{
		return WINDOWS_DIR_DOWN;
	}
	else if (pMenu->y - pMenu->nums * (pMenu->fontSize + pMenu->rowSpacing) > pMenu->maxUpY)	//δ�����ϱ���
	{
		return WINDOWS_DIR_UP;
	}
	
	return WINDOWS_DIR_MIDDLE;
}

/*------------------------------------------------------------
 * Function Name  : RecordTwoLevelMenuPosition
 * Description    : ��¼�����˵�λ����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void RecordTwoLevelMenuPosition( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	uint8_t i = 0;
	uint16_t x = pMenu->x + pMenu->lineWidth + 1;
	uint16_t y = pMenu->y + pMenu->lineWidth + 2;
	
	for (i=0; i<pMenu->nums; ++i)
	{
		pMenu->pos[i].x = x;
		pMenu->pos[i].y = y;
		
		y += pMenu->fontSize + pMenu->lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ShowOneRowTwoLevelMenu
 * Description    : ��ʾ�����˵�һ��GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ShowOneRowTwoLevelMenu( TWO_LEVEL_MENU_TypeDef *pMenu, uint8_t index )
{
	uint16_t lenth = pMenu->lenth - pMenu->lineWidth * 2 - 2;
	
	lcd_fill(pMenu->pos[index].x,pMenu->pos[index].y,lenth,pMenu->fontSize,pMenu->backColor);
	
	if (pMenu->fontSize == 16)
	{
		GUI_DispStr16At(pMenu->pos[index].x,pMenu->pos[index].y,pMenu->pointColor,pMenu->backColor,pMenu->pParameterNameArray[index]);
	}
	else if (pMenu->fontSize == 24)
	{
		GUI_DispStr24At(pMenu->pos[index].x,pMenu->pos[index].y,pMenu->pointColor,pMenu->backColor,pMenu->pParameterNameArray[index]);
	}
}
	

/*------------------------------------------------------------
 * Function Name  : GUI_DrawTwoLevelMenu
 * Description    : �������˵�GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_DrawTwoLevelMenu( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	uint8_t i = 0;
	uint16_t x = pMenu->x;
	uint16_t y = pMenu->y;
	RECTANGLE_FRAME_TypeDef rect;
	
	rect.x = x;
	rect.y = y;
	rect.lenth = pMenu->lenth;
	rect.width = pMenu->width;
	rect.lineWidth = pMenu->lineWidth;
	rect.lineColor = pMenu->lineColor;
	
	GUI_DrawRectangleFrame(&rect);
	
	for (i=0; i<pMenu->nums; ++i)
	{
		GUI_ShowOneRowTwoLevelMenu(pMenu,i);
	}
}

/*------------------------------------------------------------
 * Function Name  : LoadTwoLevelMenuPage
 * Description    : ��������˵�ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TwoLevelMenu( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	POP_WINDOWS_DIRECTION_TypeDef windowsDir;
	
	windowsDir = JudgeWindowsDirection(pMenu);	 
	
	switch (windowsDir)
	{
		case WINDOWS_DIR_UP:		
			pMenu->y -= (pMenu->fontSize + pMenu->rowSpacing) * (pMenu->nums - 1);	
			break;
		case WINDOWS_DIR_DOWN:	
			break;
		case WINDOWS_DIR_MIDDLE:			
			pMenu->y = pMenu->maxUpY;
			break;
	}	
	
	RecordTwoLevelMenuPosition(pMenu);
	
	GUI_DrawTwoLevelMenu(pMenu);
}

/*------------------------------------------------------------
 * Function Name  : TestParameterMoveIndexProcess
 * Description    : ��������ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MoveIndexProcess( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	pMenu->isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = pMenu->nums;
	indexObj.colNum = 1;
	indexObj.sumNum = pMenu->nums;
	indexObj.pNowIndex = &pMenu->nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (pMenu->nowIndex != pMenu->recordIndex)
	{		
		pMenu->isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : MoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MoveCursorProcess( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	if (pMenu->isIndexMove == YES)
	{		
		if (pMenu->recordIndex != 0xff)
		{		
			pMenu->pointColor = pMenu->recordPointColor;
			pMenu->backColor = pMenu->recordBackColor;		
			GUI_ShowOneRowTwoLevelMenu(pMenu,pMenu->recordIndex);
			
			pMenu->pointColor = COLOR_SELECT_POINT;
			pMenu->backColor = COLOR_SELECT_BACK;			
			GUI_ShowOneRowTwoLevelMenu(pMenu,pMenu->nowIndex);
		}
		else
		{
			pMenu->pointColor = COLOR_SELECT_POINT;
			pMenu->backColor = COLOR_SELECT_BACK;			
			GUI_ShowOneRowTwoLevelMenu(pMenu,pMenu->nowIndex);			
		}
		
		pMenu->recordIndex = pMenu->nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TwoLevelMenuKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TwoLevelMenuKeyProcess( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:		
				pMenu->isLeavePage = YES;
				pMenu->isSelect = YES;
				break;
			case KEY_ESC:
				pMenu->isLeavePage = YES;
				pMenu->isSelect = NO;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ClearTwoLevelMenuPage
 * Description    : ����ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ClearTwoLevelMenuPage( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	lcd_fill(pMenu->x,pMenu->y,pMenu->lenth,pMenu->width,pMenu->recordBackColor);
}	

/*------------------------------------------------------------
 * Function Name  : LoadTwoLevelMenuPage
 * Description    : ��������˵�ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTwoLevelMenuPage( TWO_LEVEL_MENU_TypeDef *pMenu )
{
	TwoLevelMenuInit(pMenu);
	
	GUI_TwoLevelMenu(pMenu);
	
	while (1)
	{
		PutinProcessCycle();
		
		MoveIndexProcess(pMenu);
		
		MoveCursorProcess(pMenu);
		
		TwoLevelMenuKeyProcess(pMenu);
		
		if (pMenu->isLeavePage == YES)
		{
			ClearTwoLevelMenuPage(pMenu);
			
			return;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTwoLevelMenuAddr
 * Description    : ��ȡ�����˵���ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TWO_LEVEL_MENU_TypeDef *GetTwoLevelMenuAddr( void )
{
	return &g_twoLevelMenu;
}

/*------------------------------------------------------------
 * Function Name  : ShortcutMenuTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ShortcutMenuTask( SHORTCUT_TypeDef *pMenu )
{
	const uint8_t FUNCTION_KEY_LENTH = 120;
	const uint8_t FUNCTION_KEY_WIDTH = 26;
	const uint8_t FUNCTION_KEY_DISTANCE = (LCD_LENTH_X - FUNCTION_KEY_LENTH * 4) / (SHORTCUT_CNT + 1);
	const uint16_t startX = FUNCTION_KEY_DISTANCE;
	const uint16_t startY = LCD_WIDTH_Y - 30;
	uint8_t i;
	uint16_t x = startX;
	uint16_t y = startY;
	uint8_t bitStatus = SHOW_F1;
	
	for (i=0; i<SHORTCUT_CNT; ++i)
	{
		if (pMenu->status & bitStatus)
		{
			lcd_fill(x,y,FUNCTION_KEY_LENTH,FUNCTION_KEY_WIDTH,pMenu->backColor);
			GUI_DispStr24At(x,y,pMenu->pointColor,pMenu->backColor,pMenu->pContent[i]);
		}
		else
		{
			lcd_fill(x,y,FUNCTION_KEY_LENTH,FUNCTION_KEY_WIDTH,COLOR_SHORTCUT_USELESS);
		}
		
		bitStatus <<= 1;
		x += FUNCTION_KEY_LENTH + FUNCTION_KEY_DISTANCE;
	}
}

/*------------------------------------------------------------
 * Function Name  : InitShortCutMenu
 * Description    : ��ʼ����ݲ˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitShortCutMenu( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	pShortCut->status = NULL;
	pShortCut->pointColor = COLOR_SHORTCUT_POINT;
	pShortCut->backColor = COLOR_SHORTCUT_BACK;
	pShortCut->clearBackColor = BLACK;
}

/*------------------------------------------------------------
 * Function Name  : ClearShortCutMenuArea
 * Description    : �����ݲ˵�������ʾ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ClearShortCutMenuArea( uint16_t backColor )
{
	lcd_fill(0,LCD_WIDTH_Y - 30,LCD_LENTH_X,30,backColor);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ClearShortcutMenu
 * Description    : ������ݲ˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_ClearShortcutMenu( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	ClearShortCutMenuArea(pShortCut->clearBackColor);
}

/*------------------------------------------------------------
 * Function Name  : GetShortcutMenuAddr
 * Description    : ��ȡ��ݲ˵���ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SHORTCUT_TypeDef *GetShortcutMenuAddr( void )
{
	return &g_shortcutMenu;
}

/*------------------------------------------------------------
 * Function Name  : GUI_PopWindows
 * Description    : ���ص���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PopWindows( POP_WINDOWS_TypeDef *pWindows )
{
	RECTANGLE_FRAME_WITH_TITLE_TypeDef rect;
	
	rect.x = pWindows->x;
	rect.y = pWindows->y;
	rect.lenth = pWindows->lenth;
	rect.titleWidth = pWindows->titleWidth;
	rect.contentWidth = pWindows->contentWidth;
	rect.lineWidth = pWindows->lineWidth;
	rect.lineColor = pWindows->WindowsColor;
	rect.titlePointColor = pWindows->titlePointColor;
	rect.titleBackColor = pWindows->titleBackColor;
	rect.pTitleContent = pWindows->pTitleContent;
	rect.titleFontSize = pWindows->fontSize;
	rect.contentBackColor = pWindows->backColor;
	
	GUI_DrawRectangleFrameWithTitle(&rect);
}	

/*------------------------------------------------------------
 * Function Name  : LoadPopWindowsContent
 * Description    : ���ص�������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadPopWindowsContent( POP_WINDOWS_TypeDef *pWindows )
{
	uint8_t i;
	uint16_t x = pWindows->x + pWindows->lineWidth + 3;
	uint16_t y = pWindows->y + pWindows->titleWidth + 3;
	const char * const *p = pWindows->pCueContent;
	char codeBuff[15];
	const uint8_t MAX_ROW_CNT = 3;
	
	for(i=0; i<pWindows->cueRowNum; ++i)
	{
		GUI_DispStr24At(x,y,pWindows->pointColor,pWindows->backColor,*p++);
		y += pWindows->fontSize + pWindows->rowSpacing;
	}
	
	if (pWindows->popType == POP_PRM_WARNING)
	{
		y = pWindows->y + pWindows->titleWidth + 3;
		y += MAX_ROW_CNT * (pWindows->fontSize + pWindows->rowSpacing);
		
		usprintf(codeBuff,"[ %02X H ]",pWindows->warningCode);
		GUI_DispStr24At(x,y,pWindows->pointColor,pWindows->backColor,"������룺");
		
		GUI_DispStr24At(x+5*24,y,pWindows->pointColor,pWindows->backColor,codeBuff);	
	}
}

/*------------------------------------------------------------
 * Function Name  : LoadPopWindowsShortCut
 * Description    : ���ؿ�ݲ˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadPopWindowsShortCut( POP_WINDOWS_TypeDef *pWindows )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
		
	if ((pWindows->popType==POP_PCM_CUE) || (pWindows->popType==POP_PRM_WARNING))
	{
		pShortCut->status = SHOW_F1;
		pShortCut->pointColor = WHITE;
		pShortCut->backColor = DARKBLUE;
		pShortCut->pContent[0] = pTwoLevelMenu[11];
		
		ShortcutMenuTask(pShortCut);
	}
	
	if (pWindows->popType == POP_PCM_ASKING)
	{
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = WHITE;
		pShortCut->backColor = DARKBLUE;
		pShortCut->pContent[2] = pWindows->pAskContent[0];
		pShortCut->pContent[3] = pWindows->pAskContent[1];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SetPopWindowsContent
 * Description    : ���õ�������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetPopWindowsContent( uint8_t cueRowNum, const char * const *pCueContent )
{
	g_popWindows.cueRowNum = cueRowNum;
	g_popWindows.pCueContent = pCueContent;
}

/*------------------------------------------------------------
 * Function Name  : GetPopWindowsAddr
 * Description    : ��ȡ������ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
POP_WINDOWS_TypeDef *GetPopWindowsAddr( void )
{
	return &g_popWindows;
}

/*------------------------------------------------------------
 * Function Name  : PopWindowsWithoutContent
 * Description    : ������ʽ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PopWindowsWithoutOperate( void )
{
	GUI_ClearShortcutMenu();
	
	GUI_PopWindows(&g_popWindows);
	
	LoadPopWindowsContent(&g_popWindows);
}

/*------------------------------------------------------------
 * Function Name  : PopWindowsProcessCueInfomation
 * Description    : ������ʾ��Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PopWindowsProcessCueInfomation( void )
{
	WARN_HANDLER_TypeDef status = INIT_WARN;
	
	while (1)
	{
		PutinProcessCycle();
		
		switch (status)
		{
			case INIT_WARN:
				GUI_ClearShortcutMenu();
				LoadPopWindowsShortCut(&g_popWindows);
				
				g_popWindows.pTitleContent = "��ʾ��Ϣ";	
				GUI_PopWindows(&g_popWindows);
				LoadPopWindowsContent(&g_popWindows);
				status = CLEAR_WARN;
				break;
			case CLEAR_WARN:
				if (IsPressKey() == YES)
				{
					if (GetKeyVal() == KEY_F1)
					{
						status = WAIT_WARN_CLEAR;
					}
				}
				break;
			case WAIT_WARN_CLEAR:
				GUI_ClearShortcutMenu();
				return;	
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : PopWindowsProcessAskInfomation
 * Description    : ����ѯ����Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PopWindowsProcessAskInfomation( void )
{
	SELECT_STATUS_TypeDef status = INIT_SELECT;
	
	while (1)
	{
		PutinProcessCycle();
		
		switch (status)
		{
			case INIT_SELECT:
				GUI_ClearShortcutMenu();
				LoadPopWindowsShortCut(&g_popWindows);
				
				g_popWindows.pTitleContent = "ѯ����Ϣ";	
				GUI_PopWindows(&g_popWindows);
				LoadPopWindowsContent(&g_popWindows);
				status = WAIT_SELECT;
				break;
			case WAIT_SELECT:
				if (IsPressKey() == YES)
				{
					switch ( GetKeyVal() )
					{
						case KEY_F3:
						case KEY_ENTER:
							g_popWindows.selectResult = YES;
							status = SELECT_END;
							break;
						case KEY_F4:
						case KEY_ESC:
							g_popWindows.selectResult = NO;
							status = SELECT_END;
							break;
					}
				}
				break;
			case SELECT_END:
				GUI_ClearShortcutMenu();
				return;	
			
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetPopWindowsSelectResult
 * Description    : ��ȡ����ѡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus GetPopWindowsSelectResult( void )
{
	return g_popWindows.selectResult;
}

/*------------------------------------------------------------
 * Function Name  : GetErrCueInfomation
 * Description    : ��ȡ��λ��������Ϣ��ʾ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static const char * const * GetErrCueInfomation( uint32_t err ) 
{
	switch ( err )
	{
		case STATUS_ERROR_UPLIMIT:			//����λ
			return &pWarn_windows[0];

		case STATUS_ERROR_DWLIMIT:			//����λ
			return &pWarn_windows[1];

		case STATUS_ERROR_CRASH:			//Ԥ��ײ����
			return &pWarn_windows[2];

		case STATUS_ERROR_OVERSPEED_KY:		//��ѹͨ�����ٱ���
			return &pWarn_windows[3];

		case STATUS_ERROR_OVERSPEED_KZ:		//����ͨ�����ٱ���
			return &pWarn_windows[4];

		case STATUS_ERROR_LIMIT:			//����ָ��ִ��
			return &pWarn_windows[5];

		case STATUS_ERROR_IVLD_PARA:		//�Ƿ�����
			return &pWarn_windows[6];

	   case STATUS_ERROR_CRC:				//CRCУ���
			return &pWarn_windows[7];

		case STATUS_ERROR_OVER_ENTRY_KY:	//��ѹͨ�����������
			return &pWarn_windows[8];

		case STATUS_ERROR_OVER_ENTRY_KZ:	//����ͨ�����������
			return &pWarn_windows[9];

		case STATUS_ERROR_STEP:				//�����������
			return &pWarn_windows[12]; 

		case STATUS_ERROR_PROT_P_FH:		//��ѹͨ�����������
			return &pWarn_windows[13];

		case STATUS_ERROR_PROT_P_WY:		//����ͨ�����������
			return &pWarn_windows[14];

		case STATUS_ERROR_FULL_SCALE_KY:	//��ѹͨ������
			return &pWarn_windows[15];

		case STATUS_ERROR_FULL_SCALE_KZ:	//����ͨ������
			return &pWarn_windows[16];

		case STATUS_ERROR_SWLIMIT:			//����ʱ��λͬʱ����
			return &pWarn_windows[17];
		
		case STATUS_ERROR_PROT_N_FH:		//��ѹͨ������������󱣻�ֵ 
			return &pWarn_windows[18];
		
		case STATUS_ERROR_PROT_N_WY:		//����ͨ������������󱣻�ֵ
			return &pWarn_windows[19];
		
		case STATUS_ERROR_LIMIT_POS:		//�͸���λ
			return &pWarn_windows[23];
		
		case STATUS_EMERGENCY_STOP:			//��ͣ��ť
			return &pWarn_windows[24];
		
		case STATUS_SERVO_DRIVE_WARN:		//�ŷ��������
			return &pWarn_windows[25];

		default:
			return &pWarn_windows[20];
	}
}

/*------------------------------------------------------------
 * Function Name  : CmdRxContentProcess
 * Description    : ����������ݴ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CMD_STATUS_TypeDef CmdRxContentProcess( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	
	cmdStatus = PCM_CmdSendCycle();
	
	switch (cmdStatus)
	{
		case CMD_IDLE:
			break;
		case CMD_BUSY:
			SetPopWindowsContent(1,&pWarn_windows[21]);						
			break;
		case CMD_UNLINK:
			SetPopWindowsContent(1,&pWarn_windows[22]);						
			break;
		default:
			break;
	}
	
	return cmdStatus;
}

/*------------------------------------------------------------
 * Function Name  : PopWindowsProcessPrmWarningInfomation
 * Description    : ������λ��������Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PopWindowsProcessPrmWarningInfomation( void )
{
	WARN_HANDLER_TypeDef status = INIT_WARN;
	CMD_STATUS_TypeDef cmdStatus;
	PRM_REPLY_TypeDef *pPrmReply = NULL;
	
	while (1)
	{
		PutinProcessCycle();
		
		switch (status)
		{
			case INIT_WARN:
				GUI_ClearShortcutMenu();
				LoadPopWindowsShortCut(&g_popWindows);
				
				g_popWindows.pTitleContent = "������Ϣ";	
				GUI_PopWindows(&g_popWindows);
				
				status = GET_WARN;
				break;
			case GET_WARN:
				cmd_get_error_pkg();
				
				cmdStatus = CmdRxContentProcess();
				
				switch ( cmdStatus )
				{
					case CMD_IDLE:
					case CMD_BUSY:
					case CMD_UNLINK:					
						status = WATCH_WARN;
						break;
					default:
						break;
				}
				break;
			case WATCH_WARN:
				if ( cmdStatus == CMD_IDLE )
				{
					pPrmReply = GetPrmReplyAddr();
					
					g_popWindows.warningCode = pPrmReply->err;
					SetPopWindowsContent(1,GetErrCueInfomation(g_popWindows.warningCode));
				}
				LoadPopWindowsContent(&g_popWindows);
				status = CLEAR_WARN;
				break;
			case CLEAR_WARN:
				if (IsPressKey() == YES)
				{
					if (GetKeyVal() == KEY_F1)
					{
						if ( cmdStatus == CMD_IDLE )
						{
							cmd_clear_error_pkg();
							
							cmdStatus = CmdRxContentProcess();
								
							switch ( cmdStatus )
							{
								case CMD_IDLE:
								case CMD_BUSY:
								case CMD_UNLINK:					
									status = WAIT_WARN_CLEAR;
									break;
								default:
									break;
							}
						}
						
						status = WAIT_WARN_CLEAR;
					}
				}
				break;
			case WAIT_WARN_CLEAR:
				GUI_ClearShortcutMenu();				
				return;		
		}
	}
}


/*------------------------------------------------------------
 * Function Name  : PopWindowsInit
 * Description    : ������ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PopWindowsInit( void )
{
	g_popWindows.popType = POP_NULL;
	g_popWindows.x = POS_WINDOWS_START_X;
	g_popWindows.y = POS_WINDOWS_START_Y;
	g_popWindows.lenth = 370;
	g_popWindows.contentWidth = 118;
	g_popWindows.titleWidth = 32;
	g_popWindows.pointColor = WHITE;
	g_popWindows.backColor =  BLACK;
	g_popWindows.WindowsColor = WHITE;
	g_popWindows.titlePointColor = WHITE;
	g_popWindows.titleBackColor = DARKBLUE;
	g_popWindows.lineWidth = 3;
	g_popWindows.rowSpacing = 2;
	g_popWindows.warningCode = 0;
	g_popWindows.cueRowNum = 0;
	g_popWindows.fontSize = 24;
	g_popWindows.pTitleContent = NULL;
	g_popWindows.pCueContent = NULL;
}	

/*------------------------------------------------------------
 * Function Name  : SetPopWindowsWithoutContent
 * Description    : �����޲���ʽ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPopWindowsWithoutOperate( POP_WINDOWS_TYPE_TypeDef popType, const char *pTitle, uint8_t cueRowNum, const char * const *pCueContent )
{
	g_popWindows.popType = popType;
	
	g_popWindows.pTitleContent = pTitle;
	
	SetPopWindowsContent(cueRowNum,pCueContent);
}

/*------------------------------------------------------------
 * Function Name  : SetPopWindowsInfomation
 * Description    : ����ͨ�õ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetPopWindowsInfomation( POP_WINDOWS_TYPE_TypeDef popType, uint8_t cueRowNum, const char * const *pCueContent )
{
	g_popWindows.popType = popType;
	SetPopWindowsContent(cueRowNum,pCueContent);
}

/*------------------------------------------------------------
 * Function Name  : SetSelectPopWindowsInfomation
 * Description    : ����ѡ�񵯴�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetSelectPopWindowsInfomation( POP_WINDOWS_TYPE_TypeDef popType, uint8_t cueRowNum, const char * const *pCueContent, const char * const *pSel )
{
	g_popWindows.popType = popType;
	g_popWindows.pAskContent = pSel;
	SetPopWindowsContent(cueRowNum,pCueContent);
}

/*------------------------------------------------------------
 * Function Name  : ClearExceptionFlag
 * Description    : ����쳣��־
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ClearExceptionFlag( void )
{
	g_popWindows.popType = POP_NULL;
}

/*------------------------------------------------------------
 * Function Name  : GetExceptionFlag
 * Description    : ��ȡ�쳣��־
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
POP_WINDOWS_TYPE_TypeDef GetExceptionFlag( void )
{
	return g_popWindows.popType;
}

/*------------------------------------------------------------
 * Function Name  : ClearPrmWarningShieldBit
 * Description    : �����λ����������λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ClearPrmWarningShieldBit( void )
{
	g_popWindows.exceptionShieldCode = STATUS_OK;
}	

/*------------------------------------------------------------
 * Function Name  : CheckPrmWarning
 * Description    : �����λ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus CheckPrmWarning( void )
{
	if (SET == GetPrmWarnStatus() )
	{		
		g_popWindows.popType = POP_PRM_WARNING;
			
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : PopWindowsProcessCycle
 * Description    : �����������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PopWindowsProcessCycle( void )
{	
	switch ( GetExceptionFlag() )
	{
		case POP_NULL:		
			break;
		
		case POP_PCM_CUE:
			PopWindowsProcessCueInfomation();
			break;
		
		case POP_PCM_ASKING:
			PopWindowsProcessAskInfomation();
			break;
		
		case POP_PRM_WARNING:
			PopWindowsProcessPrmWarningInfomation();
			break;
		
		case POP_PCM_CUE_WITHOUT_OPERATE:
			PopWindowsWithoutOperate();
			break;
	}
	
	ClearExceptionFlag();
}

/*------------------------------------------------------------
 * Function Name  : SetBackLightClose
 * Description    : ���ñ���Ч���ر�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetBackLightEffectClose( uint16_t color ) 
{
//	uint16_t i;
//	
//	for (i=0; i<=0x80; ++i)
//	{
//		bsp_DelayUS(100);
//		lcd_set_backlight(0xff-i);
//	}
//	
//	lcd_set_backlight(0x80);
	
	lcd_clear(color);
}

/*------------------------------------------------------------
 * Function Name  : SetBackLightEffectOpen
 * Description    : ���ñ���Ч����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetBackLightEffectOpen( void ) 
{
//	uint16_t i;
//	
//	for (i=0x80; i<=0xff; ++i)
//	{
//		bsp_DelayUS(100);
//		lcd_set_backlight(i);
//	}
	
//	lcd_set_backlight(0xff);
}

/*------------------------------------------------------------
 * Function Name  : SelectWindowsInit
 * Description    : ѡ�񴰳�ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SelectWindowsInit( void )
{
	g_selectWindows.x = POS_WINDOWS_START_X;
	g_selectWindows.y = POS_WINDOWS_START_Y;
	g_selectWindows.lenth = 370;
	g_selectWindows.contentWidth = 171;
	g_selectWindows.titleWidth = 32;
	g_selectWindows.pointColor = BLACK;
	g_selectWindows.backColor =  WHITE;
	g_selectWindows.recordPointColor = BLACK;
	g_selectWindows.recordBackColor = WHITE;
	g_selectWindows.windowsPointColor = WHITE;
	g_selectWindows.windowsBackColor = BLACK;
	g_selectWindows.titlePointColor = WHITE;
	g_selectWindows.titleBackColor = DARKBLUE;
	g_selectWindows.lineWidth = 3;
	g_selectWindows.rowSpacing = 24;
	g_selectWindows.fontSize = 24;
	g_selectWindows.pTitleContent = NULL;
	g_selectWindows.pSelectContent = NULL;
}

/*------------------------------------------------------------
 * Function Name  : Init_SelectWindows
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Init_SelectWindows( SELECT_WINDOWS_TypeDef *pSelect )
{
	pSelect->nowIndex = 0;
	pSelect->recordIndex = 0xff;
	pSelect->isIndexMove = NO;
	pSelect->refreshShortcut = ENABLE;
	pSelect->leavePage.flagLeavePage = RESET;
	pSelect->leavePage.flagSaveData = RESET;
	pSelect->pointColor = pSelect->recordPointColor;
	pSelect->backColor = pSelect->recordBackColor;
}	

/*------------------------------------------------------------
 * Function Name  : GUI_SelectWindows
 * Description    : GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SelectWindows( SELECT_WINDOWS_TypeDef *pSelect )
{
	RECTANGLE_FRAME_WITH_TITLE_TypeDef rect;
	
	GUI_ClearShortcutMenu();
	
	rect.x = pSelect->x;
	rect.y = pSelect->y;
	rect.lenth = pSelect->lenth;
	rect.titleWidth = pSelect->titleWidth;
	rect.contentWidth = pSelect->contentWidth;
	rect.lineWidth = pSelect->lineWidth;
	rect.lineColor = pSelect->windowsPointColor;
	rect.titlePointColor = pSelect->titlePointColor;
	rect.titleBackColor = pSelect->titleBackColor;
	rect.pTitleContent = pSelect->pTitleContent;
	rect.titleFontSize = pSelect->fontSize;
	rect.contentBackColor = pSelect->windowsBackColor;
	
	GUI_DrawRectangleFrameWithTitle(&rect);
}

/*------------------------------------------------------------
 * Function Name  : ConfigReportSearchRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigSelectWindowsCoordinate( SELECT_WINDOWS_TypeDef *pSelect )
{
	uint8_t i;
	uint16_t x = pSelect->x;
	uint16_t y = pSelect->y;
	uint16_t selLen = strlen(*(pSelect->pSelectContent));
	
	x += (pSelect->lenth - (pSelect->fontSize >> 1) * selLen) >> 1;
	
	y += pSelect->titleWidth + 24;
	
	for (i=0; i<pSelect->selectNum; ++i)
	{
		pSelect->pos[i].x = x;
		pSelect->pos[i].y = y;
		
		y += pSelect->fontSize + pSelect->rowSpacing;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_SelectWindowsDrawOneRowOneLevelMenu
 * Description    : һ�в�������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SelectWindowsDrawOneRowOneLevelMenu( SELECT_WINDOWS_TypeDef *pSelect, uint8_t index )
{
	const uint16_t x = pSelect->pos[index].x;
	const uint16_t y = pSelect->pos[index].y;
	const uint16_t pointColor = pSelect->pointColor;
	const uint16_t backColor = pSelect->backColor;
	const uint16_t lenth = strlen(pSelect->pSelectContent[index]) * (pSelect->fontSize >> 1);
	const uint16_t width = pSelect->fontSize;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,pSelect->pSelectContent[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_SelectWindowsOneLevelMenu
 * Description    : ����һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_SelectWindowsOneLevelMenu( SELECT_WINDOWS_TypeDef *pSelect )
{
	uint8_t i;
	
	for (i=0; i<pSelect->selectNum; ++i)
	{
		GUI_SelectWindowsDrawOneRowOneLevelMenu(pSelect,i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : LoadSelectWindows
 * Description    : ����ѡ�񴰿�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SELECT_WINDOWS_TypeDef *GetSelectWindowsAddr( void )
{
	return &g_selectWindows;
}

/*------------------------------------------------------------
 * Function Name  : SelectWindowsMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SelectWindowsMoveIndexProcess( SELECT_WINDOWS_TypeDef *pSelect )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	pSelect->isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = pSelect->selectNum;
	indexObj.colNum = 1;
	indexObj.sumNum = pSelect->selectNum;
	indexObj.pNowIndex = &pSelect->nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (pSelect->nowIndex != pSelect->recordIndex)
	{		
		pSelect->isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : SelectWindowsMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SelectWindowsMoveCursorProcess( SELECT_WINDOWS_TypeDef *pSelect )
{
	if (pSelect->isIndexMove == YES)
	{		
		if (pSelect->recordIndex != 0xff)
		{
			pSelect->pointColor = pSelect->recordPointColor;
			pSelect->backColor = pSelect->recordBackColor;		
			GUI_SelectWindowsDrawOneRowOneLevelMenu(pSelect, pSelect->recordIndex);
			
			pSelect->pointColor = pSelect->recordBackColor;
			pSelect->backColor = COLOR_SELECT_BACK;
			GUI_SelectWindowsDrawOneRowOneLevelMenu(pSelect, pSelect->nowIndex);
		}
		else
		{
			pSelect->pointColor = pSelect->recordBackColor;
			pSelect->backColor = COLOR_SELECT_BACK;
			GUI_SelectWindowsDrawOneRowOneLevelMenu(pSelect, pSelect->nowIndex);
		}
		
		pSelect->recordIndex = pSelect->nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : SelectWindowsShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SelectWindowsShortcutCycleTask( SELECT_WINDOWS_TypeDef *pSelect )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (pSelect->refreshShortcut == ENABLE)
	{
		pSelect->refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[79];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : LoadSelectWindows
 * Description    : ����ѡ�񴰿�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSelectWindows( SELECT_WINDOWS_TypeDef *pSelect )
{
	Init_SelectWindows(pSelect);
	
	ConfigSelectWindowsCoordinate(pSelect);
	
	GUI_SelectWindows(pSelect);
	
	GUI_SelectWindowsOneLevelMenu(pSelect);
	
	while (pSelect->leavePage.flagLeavePage == RESET)
	{
		PutinProcessCycle();
		
		SelectWindowsMoveIndexProcess(pSelect);
		
		SelectWindowsMoveCursorProcess(pSelect);
		
		SelectWindowsShortcutCycleTask(pSelect);
		
		if (IsPressKey() == YES)
		{
			switch ( GetKeyVal() )
			{
				case KEY_F3:
				case KEY_ENTER:
					pSelect->selectResult = pSelect->nowIndex;
					pSelect->leavePage.flagLeavePage = SET;
					break;
				
				case KEY_F4:
				case KEY_ESC: 
					pSelect->selectResult = 0xff;
					pSelect->leavePage.flagLeavePage = SET;
					break;
			}
		}
		
		if (pSelect->leavePage.flagLeavePage == SET)
		{
			GUI_ClearShortcutMenu();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ShowSystemDate
 * Description    : ��ʾϵͳ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_ShowSystemDate( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc )
{
	char time_buff[12];
	tTime t;
	
	time_cycle();
	t = get_time();	 	
	usprintf(time_buff,"%04d-%02d-%02d",t.usYear,t.ucMon,t.ucMday);
	
	GUI_DispStr24At(x,y,pc,bc,time_buff);	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ShowActiveDate
 * Description    : �Լ�������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_ShowActiveDate( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc )
{
	tTime t;
	uint32_t day;
	char buff[12];
	ErrorStatus errStatus;

	errStatus = dvc_active_date_get(&day,&t);		//��ȡ����ʱ��
	
	if (errStatus == SUCCESS)
	{
		usprintf(buff,"%04d-%02d-%02d",t.usYear,t.ucMon,t.ucMday);
	}
	else
	{
		strcpy(buff,"****-**-**");
	}

	GUI_DispStr24At(x,y,pc,bc,buff);		
}

/*------------------------------------------------------------
 * Function Name  : GUI_DrawTitleFrame
 * Description    : �����б���ı߿�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawTitleFrame( TITLE_FRAME_TypeDef *pFrame )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint16_t x = 0;
	uint16_t y;
	
	/* �����ο� */
	rectangle.x = pFrame->x;
	rectangle.y = pFrame->y;
	rectangle.lenth = pFrame->lenth;
	rectangle.width = pFrame->width;
	rectangle.lineWidth = pFrame->lineWidth;
	rectangle.lineColor = pFrame->linePointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	/* �����ⲿ�ֱ��� */
	lcd_fill(pFrame->x+pFrame->titleOffsetDistance,pFrame->y,pFrame->titleLenth,pFrame->lineWidth,pFrame->lineBackColor);
	
	x = pFrame->x + pFrame->titleOffsetDistance + ( ( pFrame->titleLenth - strlen(pFrame->pTitle) * (pFrame->fontSize >> 1) ) >> 1);
	y = pFrame->y +  (pFrame->lineWidth >> 1) - (pFrame->fontSize >> 1);
	
	switch ( pFrame->fontSize )
	{
		case 16:
			GUI_DispStr16At(x,y,pFrame->titlePointColor,pFrame->titleBackPoint,pFrame->pTitle);
			break;
		default:
			GUI_DispStr24At(x,y,pFrame->titlePointColor,pFrame->titleBackPoint,pFrame->pTitle);
			break;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

