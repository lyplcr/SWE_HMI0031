/**
  ******************************************************************************
  * @file    init.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ��ʼ��
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
	"�Ƿ��ʽ��ϵͳ�����洢��?",		//0
	"��ʽ����,���еĲ�����������!",	//1
	"����ϵͳ�����ɹ���",			//2
	"����λ��ͨѶʧ�ܣ�",			//3
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
 * Description    : ��ʼ��ϵͳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitSystem( void )
{
	NVIC_Configuration();		/* �������ȼ�����    */
	GPIO_Configuration();		/* ����GPIO 		*/	
	
	BSP_Init();					/* �ײ�������ʼ�� 	*/
	
	InitGeneralTask();			/* ��ʼ��ͨ������ 	*/
	
	#ifndef VERSION_RELEASE
		#warning "Version : Demo"
	
		printf("Demo��\r\n");
	#endif
}

/*------------------------------------------------------------
 * Function Name  : InitGeneralTask
 * Description    : ��ʼ��ͨ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitGeneralTask( void )
{	
	/* ��ʼ��״̬ */
	SetCoreStatus(STATUS_CORE_MANULE);
	
	/* ��ʼ�����뷨Ƥ�� */
	InitPutinShow();
	
	/* ��ʼ��ͨѶ�� */
	InitCommucationLamp();
	
	/* ��ʼ����ݲ˵� */
	InitShortCutMenu();
	
	/* ��ʼ������ */
	PopWindowsInit();
	
	/* ��ʼ��ѡ�� */
	SelectWindowsInit();
	
	/* ��ʼ��ҳ�� */
	InitPage();
	
	/* �ر�ȫ��С�� */
	SetLampVal(0x00000000);

	bsp_StartAutoTimer(MAIN_CYCLE_TIMING,CTRL_PERIOD);	/* ��ʼ��ʱ��������ѭ������50HZ */
}

/*------------------------------------------------------------
 * Function Name  : EraseSystemParameterAskHandler
 * Description    : ����ϵͳ����ѯ��
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
 * Description    : ��ʽ��ϵͳ����ִ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FormateSystemParameterExecuteBody( void )
{
	memset(get_pcm(),0x00,PCM_MEM_SIZE);				
	memset(get_prm(),0x00,PRM_MEM_SIZE);		
	memset(get_prv(),0x00,PRV_MEM_SIZE);
	
	pHmi->test_standard_index = KYSNJS;	//����Ĭ������
	
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
 * Description    : ���ϵͳ�����Ƿ�Ϊ��
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
 * Description    : ����FLASH�ֿ�
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
 * Description    : ����FLASH�ֿ�����
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
	
	GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"����д���ֿ⣬���Ժ�...");
	
	if (UpdateFlashFontBody() == SUCCESS)
	{
		lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);			
		GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"�ֿ���³ɹ���");
		bsp_DelayMS(1000);
		
		if (WriteFlashFontPassword() == SUCCESS)
		{
			lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);			
			GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"д������Կ�׳ɹ���");
			bsp_DelayMS(1000);
			
			return;
		}
	}

	lcd_fill(0,y,LCD_LENTH_X,16,COLOR_BACK);
	GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"�ֿ����ʧ�ܣ�");
	
	while (1);		
}

/*------------------------------------------------------------
 * Function Name  : FormateSystemParameter
 * Description    : ��ʽ��ϵͳ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FormateSystemParameter( void )
{			
	if (CheckSystemParameterIsNull() == YES)
	{
		#if 0
			/* ѯ���Ƿ����ϵͳ���� */
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
 * Description    : �Զ�����ϵͳ(��ʱ��Ϊ�ļ����洢��SD������������Ѿ����ݣ������ٱ���)
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
	t = get_time();	 	//��ȡʱ��
	usprintf(time_buff,"%04d%02d%02d",t.usYear,t.ucMon,t.ucMday);
	
	//����δ���б���
	if ( NO == CheckBackUpFileExist(time_buff))
	{	
		GUI_DispStr16At(x,y,COLOR_POINT,COLOR_BACK,"�����Զ�����...");
		
		PcmBackupWithDate();
		PrmBackupWithDate();
		PrvBackupWithDate();
		
		GUI_DispStr16At(x,y,COLOR_BACK,COLOR_BACK,"                ");
	}
}

/*------------------------------------------------------------
 * Function Name  : PowerOnLink
 * Description    : �ϵ������
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
	progressBar.pTitle = "�������ȣ�";
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
 * Description    : ����Ĭ������
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
 * Description    : ϵͳ��ʼҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadSystemSetupPage( void )
{		
	/* ��ȡFLASH */
	pcm_read();								
	prm_read();							
	prv_read();	
	
	/* �����ֿ� */
	UpdateFlashFontTask();
	
	/* ����ϵͳ���� */
	FormateSystemParameter();
	
	/* ��ȡ�������� */
	GetTestContent();
	
	/* �������������ƥ���ϵ */
	ProcessMachineMatchTestType();
	
	/* �Զ����� */
	AutoBackUpSystem();
	
	/* �ϵ������ */
	PowerOnLink();
	
	/* �ر��ͱ� */
	SendClosePumpCmd();
	
	/* ����Ĭ������ */
	HandlerDefaultPassword();
	
	/* ��ת����ҳ */	
	SetPage(MAIN_PAGE);	
	
}

 /*------------------------------------------------------------
 * Function Name  : GPIO_Configuration
 * Description    : GPIO����
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
 * Description    : �ж����ȼ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void NVIC_Configuration( void )
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,USER_PROGRAM_START_ADDR-0x08000000);
	
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* �����жϣ�һ��Ҫ���ض�λ�ж�������֮���� */
	SEI();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
