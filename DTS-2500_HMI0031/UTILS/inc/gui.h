/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

/* Includes ------------------------------------------------------------------*/
#define MAX_TWO_LEVEL_MENU_SEGS			20			//二级菜单最大菜单个数

#define SHORTCUT_CNT					4			//快捷菜单个数
#define COLOR_SHORTCUT_POINT			WHITE		//功能键的前景色
#define COLOR_SHORTCUT_BACK				DARKBLUE	//功能键的背景色 
#define	COLOR_SHORTCUT_USELESS			BLACK		//未使用的按键背景色

//DTS界面颜色宏定义
#define	COLOR_SELECT_POINT				BLACK		//选中功能前景色
#define	COLOR_SELECT_BACK				ORANGE_RED	//选中功能背景色(光标)	
#define COLOR_FORBIT_EDIT_POINT			COLD_ASH	//禁止编辑字体前景色

#define MAX_SELECT_WINDOWS_SEGS			5			//最大选择弹窗个数

#define POS_WINDOWS_START_X				215			//弹窗起始X
#define POS_WINDOWS_START_Y				120			//弹窗起始Y


/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	FlagStatus flagLeavePage;		
	FlagStatus flagSaveData;
}LEAVE_PAGE_TypeDef;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t lenth;
	uint16_t width;
	uint8_t lineWidth;
	uint16_t lineColor;
}RECTANGLE_FRAME_TypeDef;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t lenth;
	uint16_t titleWidth;
	uint16_t contentWidth;
	uint8_t lineWidth;
	uint16_t lineColor;
	uint16_t titlePointColor;
	uint16_t titleBackColor;
	const char *pTitleContent;
	uint8_t titleFontSize;
	uint16_t contentBackColor;
}RECTANGLE_FRAME_WITH_TITLE_TypeDef;

typedef struct
{
	uint16_t x;
	uint16_t y;
}POSITION_TypeDef;

typedef enum
{
	ALIGN_LEFT = 0,
	ALIGN_MIDDLE,
	ALIGN_RIGHT,
}ALIGN_TYPE_TypeDef;

typedef struct
{
	uint16_t x;			/* 一级菜单坐标 */
	uint16_t y;
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint16_t maxUpY;
	uint16_t maxDownY;
	uint8_t rowSpacing;		//行间距
	uint8_t lineWidth;
	POSITION_TypeDef pos[MAX_TWO_LEVEL_MENU_SEGS];
	uint16_t lenth;
	uint16_t width;
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t recordPointColor;
	uint16_t recordBackColor;
	uint16_t lineColor;
	uint8_t nums;
	const char * const *pParameterNameArray;
	uint8_t fontSize;	
	BoolStatus isSelect;
	BoolStatus isIndexMove;
	BoolStatus isLeavePage;
}TWO_LEVEL_MENU_TypeDef;



/* 快捷菜单 */
#define SHOW_F1		0x01
#define SHOW_F2		0x02
#define SHOW_F3		0x04
#define SHOW_F4		0x08

typedef struct
{
	uint8_t status;	
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t clearBackColor;
	const char *pContent[SHORTCUT_CNT];
}SHORTCUT_TypeDef;

typedef enum
{
	POP_NULL = 0,			//不弹窗
	POP_PCM_CUE,			//上位机提示信息
	POP_PCM_ASKING,			//上位机询问信息
	POP_PRM_WARNING,		//下位机警告信息
	POP_PCM_CUE_WITHOUT_OPERATE,//只有一个GUI界面，无操作
}POP_WINDOWS_TYPE_TypeDef;

typedef enum
{
	INIT_WARN = 0,
	GET_WARN,		
	WATCH_WARN,
	CLEAR_WARN,
	WAIT_WARN_CLEAR,
}WARN_HANDLER_TypeDef;

typedef enum
{
	INIT_SELECT = 0,
	WAIT_SELECT,
	SELECT_END,
}SELECT_STATUS_TypeDef;

typedef struct
{
	POP_WINDOWS_TYPE_TypeDef popType;	
	uint16_t x;
	uint16_t y;
	uint16_t lenth;
	uint16_t titleWidth;
	uint16_t contentWidth;
	uint16_t pointColor;		//内容前景色
	uint16_t backColor;			//内容背景色
	uint16_t WindowsColor;		//窗体颜色
	uint16_t titlePointColor;
	uint16_t titleBackColor;
	uint8_t lineWidth;
	uint8_t rowSpacing;			//行间距
	uint32_t warningCode;
	uint8_t fontSize;
	const char * const *pAskContent;
	const char *pTitleContent;
	uint8_t cueRowNum;	
	uint8_t exceptionShieldCode;	//异常屏蔽码
	const char * const *pCueContent;
	BoolStatus selectResult;	//选择结果
}POP_WINDOWS_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint16_t x;
	uint16_t y;
	uint16_t lenth;
	uint16_t titleWidth;
	uint16_t contentWidth;
	uint16_t pointColor;		//内容前景色
	uint16_t backColor;			//内容背景色
	uint16_t recordPointColor;
	uint16_t recordBackColor;
	uint16_t windowsPointColor;	
	uint16_t windowsBackColor;	
	uint16_t titlePointColor;
	uint16_t titleBackColor;
	uint8_t lineWidth;
	uint8_t rowSpacing;			//行间距
	uint8_t fontSize;
	const char *pTitleContent;
	uint8_t selectNum;			//选择项个数
	const char * const *pSelectContent;
	POSITION_TypeDef pos[MAX_SELECT_WINDOWS_SEGS];
	uint8_t selectResult;		//选择结果
	BoolStatus isIndexMove;					//索引值移动
	FunctionalState refreshShortcut;		//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;			//离开页
}SELECT_WINDOWS_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GUI_DispStr16At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *s);
void GUI_DispStr20At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *s);
void GUI_DispStr24At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *s);
void GUI_DispStr32At(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *s);
void GUI_DispStr24Ht(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s);
void GUI_DispStr32Ht(uint16_t x0,uint16_t y0,uint16_t fc,uint16_t bc, const char *const s);
void GUI_DrawRectangleFrame( RECTANGLE_FRAME_TypeDef const *pRectangle );
void GUI_DrawRectangleFrameWithTitle( RECTANGLE_FRAME_WITH_TITLE_TypeDef const *pRectangle );
void GUI_DrawGeneralTitleRectangleFrame( const char * const pTitle );
void LoadTwoLevelMenuPage( TWO_LEVEL_MENU_TypeDef *pMenu );
TWO_LEVEL_MENU_TypeDef *GetTwoLevelMenuAddr( void );
void ShortcutMenuTask( SHORTCUT_TypeDef *pMenu );
SHORTCUT_TypeDef *GetShortcutMenuAddr( void );
void ClearShortCutMenuArea( uint16_t backColor );
void GUI_ClearShortcutMenu( void );
void PopWindowsInit( void );
void PopWindowsProcessCycle( void );
void SetPopWindowsWithoutOperate( POP_WINDOWS_TYPE_TypeDef popType, const char *pTitle, uint8_t cueRowNum, const char * const *pCueContent );
void SetPopWindowsInfomation( POP_WINDOWS_TYPE_TypeDef popType, uint8_t cueRowNum, const char * const *pCueContent );
void SetSelectPopWindowsInfomation( POP_WINDOWS_TYPE_TypeDef popType, uint8_t cueRowNum, const char * const *pCueContent, const char * const *pSel );
CMD_STATUS_TypeDef CmdRxContentProcess( void );
void ClearExceptionFlag( void );
POP_WINDOWS_TYPE_TypeDef GetExceptionFlag( void );
BoolStatus CheckPrmWarning( void );
void ClearPrmWarningShieldBit( void );
void SetBackLightEffectOpen( void ) ;
void SetBackLightEffectClose( uint16_t color );
void LoadSelectWindows( SELECT_WINDOWS_TypeDef *pSelect );
SELECT_WINDOWS_TypeDef *GetSelectWindowsAddr( void );
void SelectWindowsInit( void );
BoolStatus GetPopWindowsSelectResult( void );
POP_WINDOWS_TypeDef *GetPopWindowsAddr( void );
void GUI_ShowSystemDate( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc );
void GUI_ShowActiveDate( uint16_t x, uint16_t y, uint16_t pc, uint16_t bc );
void InitShortCutMenu( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
