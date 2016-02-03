/**
  ******************************************************************************
  * @file    init.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   初始化
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "initial.h"
#include "PageManage.h"
#include "UserLogin.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK

/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
const char * const pInitialPageCue[] = 
{
	"是否格式化系统参数存储区?",		//0
	"格式化后,所有的参数将被擦除!",	//1
	"擦除系统参数成功！",			//2
	"与下位机通讯失败！",			//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration( void );
static void NVIC_Configuration( void );
static void InitGeneralTask( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : InitSystem
 * Description    : 初始化系统
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitSystem( void )
{
	NVIC_Configuration();		/* 配置优先级分组    */
	GPIO_Configuration();		/* 配置GPIO 		*/	
	
	BSP_Init();					/* 底层驱动初始化 	*/
	
	InitGeneralTask();			/* 初始化通用任务 	*/
	
	#ifndef VERSION_RELEASE
		#warning "Version : Demo"
	
		printf("Demo！\r\n");
	#endif
}

/*------------------------------------------------------------
 * Function Name  : InitGeneralTask
 * Description    : 初始化通用任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitGeneralTask( void )
{	
	/* 初始化状态 */
	SetCoreStatus(STATUS_CORE_MANULE);
	
	/* 初始化输入法皮肤 */
	InitPutinShow();
	
	/* 初始化通讯灯 */
	InitCommucationLamp();
	
	/* 初始化快捷菜单 */
	InitShortCutMenu();
	
	/* 初始化弹窗 */
	PopWindowsInit();
	
	/* 初始化选择窗 */
	SelectWindowsInit();
	
	/* 初始化页面 */
	InitPage();
	
	/* 关闭全部小灯 */
	SetLampVal(0x00000000);

	bsp_StartAutoTimer(MAIN_CYCLE_TIMING,CTRL_PERIOD);	/* 开始定时，控制主循环保持50HZ */
}

/*------------------------------------------------------------
 * Function Name  : EraseSystemParameterAskHandler
 * Description    : 擦除系统参数询问
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FunctionalState EraseSystemParameterAskHandler( void )
{
	SetSelectPopWindowsInfomation(POP_PCM_ASKING,2,&pInitialPageCue[0],pSelectMenuCue);
	
	PopWindowsProcessCycle();
	
	if ( GetPopWindowsSelectResult() == YES)
	{
		return ENABLE;
	}
	
	return DISABLE;
}

/*------------------------------------------------------------
 * Function Name  : FormateSystemParameterExecuteBody
 * Description    : 格式化系统参数执行体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FormateSystemParameterExecuteBody( void )
{
	memset(get_pcm(),0x00,PCM_MEM_SIZE);				
	memset(get_prm(),0x00,PRM_MEM_SIZE);		
	memset(get_prv(),0x00,PRV_MEM_SIZE);
	
	pHmi->test_standard_index = KYSNJS;	//设置默认试验
	
	pcm_recover(BACKUP_SD);
	prm_recover(BACKUP_SD);
	prv_recover(BACKUP_SD);
	
	pcm_save();
	prm_save();
	prv_save();
	
	pcm_read();												
	prm_read();												
	prv_read();	

	lcd_clear(COLOR_BACK);
}

/*------------------------------------------------------------
 * Function Name  : CheckSystemParameterIsNull
 * Description    : 检测系统参数是否为空
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus CheckSystemParameterIsNull( void )
{
	TestStatus result[3];
	
	result[0] = CompareStrBuffIsEqualWithChar(get_pcm(),0xff,PCM_MEM_SIZE);
	result[1] = CompareStrBuffIsEqualWithChar(get_prm(),0xff,PRM_MEM_SIZE);
	result[2] = CompareStrBuffIsEqualWithChar(get_prv(),0xff,PRV_MEM_SIZE);
	
	if ((result[0]==PASSED) && (result[1]==PASSED) && (result[2]==PASSED))
	{
		return YES;
	}
	
	return NO;
}	

/*------------------------------------------------------------
 * Function Name  : UpdateFlashFontBody
 * Description    : 更新FLASH字库
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus UpdateFlashFontBody( void )
{
	FRESULT result;
	
	result = UpdateFlashFont("hz_song24");
	if (result != FR_OK)
	{
		return ERROR;
	}
	
	result = UpdateFlashFont("eng_song24");
	if (result != FR_OK)
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : UpdateFlashFontTask
 * Description    : 更新FLASH字库任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UpdateFlashFontTask( void )
{
	uint16_t x = 336;
	uint16_t y = 232;

	if (FlashFontIsExist() == YES)
	{
		return;
	}
	
	GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"正在写入字库，请稍后...");
	
	if (UpdateFlashFontBody() == SUCCESS)
	{
		lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);			
		GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"字库更新成功！");
		bsp_DelayMS(1000);
		
		if (WriteFlashFontPassword() == SUCCESS)
		{
			lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);			
			GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"写入字体钥匙成功！");
			bsp_DelayMS(1000);
			
			return;
		}
	}

	lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);
	GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"字库更新失败！");
	
	while (1);		
}

/*------------------------------------------------------------
 * Function Name  : FormateSystemParameter
 * Description    : 格式化系统参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FormateSystemParameter( void )
{			
	if (CheckSystemParameterIsNull() == YES)
	{
		#if 0
			/* 询问是否擦除系统参数 */
			if (EraseSystemParameterAskHandler() == DISABLE)
			{
				return;
			}
		#endif
		
		FormateSystemParameterExecuteBody();
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pInitialPageCue[2]);		
		PopWindowsProcessCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : AutoBackUpSystem
 * Description    : 自动备份系统(以时间为文件名存储在SD卡，如果今日已经备份，将不再备份)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void AutoBackUpSystem( void )
{
	char time_buff[12];
	tTime t;
	static uint16_t x = 336;
	static uint16_t y = 232;
	
	time_cycle();
	t = get_time();	 	//获取时间
	usprintf(time_buff,"%04d%02d%02d",t.usYear,t.ucMon,t.ucMday);
	
	//今日未进行备份
	if ( NO == CheckBackUpFileExist(time_buff))
	{	
		GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"正在自动备份...");
		
		PcmBackupWithDate();
		PrmBackupWithDate();
		PrvBackupWithDate();
		
		GUI_DispStr16At(x,y,COLOR_BACK,COLOR_BACK,"                ");
	}
}

/*------------------------------------------------------------
 * Function Name  : PowerOnLink
 * Description    : 上电后联机
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PowerOnLink( void )
{
	PROGRESS_BAR_TypeDef progressBar;
	uint16_t fontLenth = 0;
	LINK_STATUS_TypeDef linkStatus;

	progressBar.lenth = 104;
	progressBar.width = 20;
	progressBar.upperPointColor = PROCESS_UP_COLOR;
	progressBar.lowerPointColor = PROCESS_DN_COLOR;
	progressBar.lineColor = COLOR_POINT;
	progressBar.backColor = COLOR_BACK;
	progressBar.lineWidth = 2;
	progressBar.curProgress = 0;
	progressBar.totalProgress = 100;
	progressBar.pTitle = "联机进度：";
	progressBar.fontSize = 16;
	progressBar.fontColor = COLOR_POINT;
	fontLenth = strlen(progressBar.pTitle) * (progressBar.fontSize >> 1);
	progressBar.x = ((LCD_LENTH_X - progressBar.lenth - fontLenth) >> 1) + fontLenth;
	progressBar.y = ((LCD_WIDTH_Y - progressBar.width) >> 1) + 100;
	
	GUI_ProgressBar(&progressBar);
	
	link_init();
	
	bsp_StartTimer(GENERAL_TIMEOUT_TIMING,10000);
	
	while (linkStatus != LINK_IDLE)
	{
		linkStatus = link_cycle();
		
		progressBar.curProgress = GetLinkProcess();
		DrawProcessBody(&progressBar);
		
		switch ( linkStatus )
		{
			case LINK_IDLE:				
				break;
			case LINK_UNLINK:
				if (bsp_CheckTimer(GENERAL_TIMEOUT_TIMING))
				{	
					lcd_clear(COLOR_BACK);
					SetPopWindowsInfomation(POP_PCM_CUE,1,&pInitialPageCue[3]);
					PopWindowsProcessCycle();
					
					return;
				}
				link_init();
				break;
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : HandlerDefaultPassword
 * Description    : 处理默认密码
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void HandlerDefaultPassword( void )
{
	if ( strcmp(pHmi->paseword_system,"") == 0)
	{
		strcpy(pHmi->paseword_system,DEFAULT_PASSWORD);
		
		pcm_save();
	}
}	

/*------------------------------------------------------------
 * Function Name  : LoadSystemSetupPage
 * Description    : 系统开始页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemSetupPage( void )
{		
	/* 读取FLASH */
	pcm_read();								
	prm_read();							
	prv_read();	
	
	/* 更新字库 */
	UpdateFlashFontTask();
	
	/* 擦除系统参数 */
	FormateSystemParameter();
	
	/* 获取试验内容 */
	GetTestContent();
	
	/* 处理机型与试验匹配关系 */
	ProcessMachineMatchTestType();
	
	/* 自动备份 */
	AutoBackUpSystem();
	
	/* 上电后联机 */
	PowerOnLink();
	
	/* 关闭油泵 */
	SendClosePumpCmd();
	
	/* 处理默认密码 */
	HandlerDefaultPassword();
	
	/* 跳转到主页 */	
	SetPage(MAIN_PAGE);	
	
}

 /*------------------------------------------------------------
 * Function Name  : GPIO_Configuration
 * Description    : GPIO配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GPIO_Configuration( void )
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
						   RCC_AHB1Periph_GPIOC	| RCC_AHB1Periph_GPIOD | \
						   RCC_AHB1Periph_GPIOE ,ENABLE);
}

/*------------------------------------------------------------
 * Function Name  : NVIC_Configuration
 * Description    : 中断优先级分组管理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void NVIC_Configuration( void )
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,USER_PROGRAM_START_ADDR-0x08000000);
	
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 开总中断，一定要在重定位中断向量表之后开启 */
	SEI();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
