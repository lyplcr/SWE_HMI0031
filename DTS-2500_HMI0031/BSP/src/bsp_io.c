/**
  ******************************************************************************
  * @file    bsp_io.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   IO�����ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/* Private define ------------------------------------------------------------*/
/* 74HC165����IO */
#define		HC165_GPIO_CLK		(RCC_AHB1Periph_GPIOE)

#define		HC165_PIN_CLK		GPIO_Pin_0
#define		HC165_PORT_CLK		GPIOE

#define		HC165_PIN_LOCK		GPIO_Pin_1
#define		HC165_PORT_LOCK		GPIOE

#define		HC165_PIN_DAT		GPIO_Pin_2
#define		HC165_PORT_DAT		GPIOE

#define		SET_HC165_CLK()		GPIO_SetBits(HC165_PORT_CLK,HC165_PIN_CLK)
#define		SET_HC165_LOCK() 	GPIO_SetBits(HC165_PORT_LOCK,HC165_PIN_LOCK)
 
#define		CLR_HC165_CLK()		GPIO_ResetBits(HC165_PORT_CLK,HC165_PIN_CLK)	
#define		CLR_HC165_LOCK() 	GPIO_ResetBits(HC165_PORT_LOCK,HC165_PIN_LOCK)

#define		RD_HC165_DAT()   	GPIO_ReadInputDataBit(HC165_PORT_DAT,HC165_PIN_DAT)

/* 74HC595����IO */
//#define		USE_HC595_EN_CONFIG_PORT		//ʹ��оƬʹ����������

#define		EXT_HC595_GPIO_CLK		(RCC_AHB1Periph_GPIOE)

#define		EXT_PORT_HC595_RCK		GPIOE
#define		EXT_PIN_HC595_RCK		GPIO_Pin_4

#define		EXT_PORT_HC595_SCK		GPIOE
#define		EXT_PIN_HC595_SCK		GPIO_Pin_0

#define		EXT_PORT_HC595_SDA		GPIOE
#define		EXT_PIN_HC595_SDA		GPIO_Pin_3

#define		EXT_PORT_HC595_SE		GPIOB
#define		EXT_PIN_HC595_SE		GPIO_Pin_9

#define		SET_EXT_HC595_RCK()		GPIO_SetBits(EXT_PORT_HC595_RCK,EXT_PIN_HC595_RCK)
#define		SET_EXT_HC595_SCK() 	GPIO_SetBits(EXT_PORT_HC595_SCK,EXT_PIN_HC595_SCK)
#define		SET_EXT_HC595_SDA() 	GPIO_SetBits(EXT_PORT_HC595_SDA,EXT_PIN_HC595_SDA)	
#define		SET_EXT_HC595_SE() 		GPIO_SetBits(EXT_PORT_HC595_SE,EXT_PIN_HC595_SE)
 
#define		CLR_EXT_HC595_RCK()		GPIO_ResetBits(EXT_PORT_HC595_RCK,EXT_PIN_HC595_RCK)
#define		CLR_EXT_HC595_SCK() 	GPIO_ResetBits(EXT_PORT_HC595_SCK,EXT_PIN_HC595_SCK)
#define		CLR_EXT_HC595_SDA() 	GPIO_ResetBits(EXT_PORT_HC595_SDA,EXT_PIN_HC595_SDA)	
#define		CLR_EXT_HC595_SE() 		GPIO_ResetBits(EXT_PORT_HC595_SE,EXT_PIN_HC595_SE)


/* LED */
#define		RCC_DO_LED_COM		(RCC_AHB1Periph_GPIOB)
#define		PORT_DO_LED_COM 	GPIOB
#define		PIN_DO_LED_COM		GPIO_Pin_5

#define 	PUMP_START_BIT		0x00000040
#define 	PUMP_STOP_BIT		0x00000080

/* ������ */
#define		RCC_DO_BEEP			(RCC_AHB1Periph_GPIOB)
#define		PORT_DO_BEEP  		GPIOB
#define		PIN_DO_BEEP			GPIO_Pin_8

/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t g_lampData = 0;
static uint32_t g_OutportData = 0;

static uint32_t g_lampExecute = 0;

typedef enum
{ 
	BEEP_STATUS_IDLE=0,
	BEEP_STATUS_BEEP,	 
	BEEP_STATUS_MUTE 
}BEEP_STATUS_TypeDef; 
 
__IO static BEEP_STATUS_TypeDef beep_status = BEEP_STATUS_IDLE; 
__IO static uint16_t beep_time = 0;
__IO static uint16_t beep_cnt = 0;
__IO static uint16_t mute_comp = 0;
__IO static uint16_t beep_comp = 0;

/* Private function prototypes -----------------------------------------------*/
static void InitInport(void);
static void InitOutport(void);
static void InitLamp(void);
static void Init_74HC165( void );
static void InitBeep( void );
static void BeepOn( void );
static void BeepOff( void );
static void InitExt_74HC595( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : bsp_InitIO
 * Description    : ��ʼ��IO
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/     
void bsp_InitIO(void)
{
	/* ����IO��ʼ�� */ 
	InitInport();
	
	/* ���IO��ʼ�� */
	InitOutport();
	
	/* LED��ʼ�� */
	InitLamp();
	
	SetLampVal(0xFFFFFFFF);
}  

/*------------------------------------------------------------
 * Function Name  : InitInport
 * Description    : ��ʼ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitInport(void)
{	
	Init_74HC165();	
}

/*------------------------------------------------------------
 * Function Name  : InitOutport
 * Description    : ��ʼ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitOutport(void)
{
	InitBeep();
	InitExt_74HC595();
}

/*------------------------------------------------------------
 * Function Name  : InitExt_74HC595
 * Description    : ��ʼ����չоƬ74HC595
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitExt_74HC595( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(EXT_HC595_GPIO_CLK, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = EXT_PIN_HC595_RCK;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(EXT_PORT_HC595_RCK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EXT_PIN_HC595_SCK;	
	GPIO_Init(EXT_PORT_HC595_SCK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EXT_PIN_HC595_SDA;	
	GPIO_Init(EXT_PORT_HC595_SDA, &GPIO_InitStructure);
	
	#ifdef USE_HC595_EN_CONFIG_PORT
		GPIO_InitStructure.GPIO_Pin = EXT_PIN_HC595_SE;
		GPIO_Init(EXT_PORT_HC595_SE, &GPIO_InitStructure);
	#endif
	
 	CLR_EXT_HC595_RCK();
 	CLR_EXT_HC595_SCK();
 	CLR_EXT_HC595_SDA();
	
	#ifdef USE_HC595_EN_CONFIG_PORT
		CLR_EXT_HC595_SE();
	#endif
}

/*------------------------------------------------------------
 * Function Name  : InitLamp
 * Description    : ��ʼ��LED
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitLamp(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_DO_LED_COM, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = PIN_DO_LED_COM;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(PORT_DO_LED_COM, &GPIO_InitStructure);
	
	GPIO_SetBits(PORT_DO_LED_COM,PIN_DO_LED_COM);
}

/*------------------------------------------------------------
 * Function Name  : Init_74HC165
 * Description    : ��ʼ��оƬ74HC165
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Init_74HC165( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(HC165_GPIO_CLK, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = HC165_PIN_CLK;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(HC165_PORT_CLK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_PIN_LOCK;				  
	GPIO_Init(HC165_PORT_LOCK, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_PIN_DAT;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(HC165_PORT_DAT, &GPIO_InitStructure);
	
	CLR_HC165_CLK();
	SET_HC165_LOCK();
}
   
/*------------------------------------------------------------
 * Function Name  : GetKey
 * Description    : ��ȡ��ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/   
uint32_t GetKey( void )
{
	uint8_t i;
	uint32_t key = 0;
	
	CLR_HC165_LOCK();
	bsp_DelayUS(1);
	SET_HC165_LOCK(); 
	
	for (i=0; i<32; i++)
	{		  			
		key <<= 1;
		if( RD_HC165_DAT() )
		{
			key |= 0x01;
		} 
		
		SET_HC165_CLK();
		CLR_HC165_CLK();	
	}
	
	key =~ key;
	 
	return key;
}

/*------------------------------------------------------------
 * Function Name  : SetLamp
 * Description    : ����LED
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/   
void SetLampVal( uint32_t led )
{
	uint8_t i;
	
	for (i=0; i<32; i++)
	{		  	
		if (led & 0x80000000)
		{
			SET_EXT_HC595_SDA();
		}
		else
		{
			CLR_EXT_HC595_SDA();
		}
		
		SET_EXT_HC595_SCK();
		CLR_EXT_HC595_SCK();
		
		led<<=1;		
	}
	
	SET_EXT_HC595_RCK();	
	CLR_EXT_HC595_RCK();
}

/*------------------------------------------------------------
 * Function Name  : ClrLampBit
 * Description    : ���λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void ClrLampBit(uint32_t bit)
{
	g_lampData &= (~bit);
} 

/*------------------------------------------------------------
 * Function Name  : SetLampBit
 * Description    : ����λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void SetLampBit(uint32_t bit)
{
	g_lampData |= bit;
}

/*------------------------------------------------------------
 * Function Name  : RevLampBit
 * Description    : ȡ��λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void RevLampBit(uint32_t bit)
{
	g_lampData ^= bit;
}

/*------------------------------------------------------------
 * Function Name  : SetLamp
 * Description    : ����С��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void SetLamp(uint32_t status)
{
	g_lampData = status;
}

/*------------------------------------------------------------
 * Function Name  : GetLamp
 * Description    : ��ȡС��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
uint32_t GetLamp( void )
{
	return g_lampData;
}

/*------------------------------------------------------------
 * Function Name  : LampTask
 * Description    : С������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LampTask( void )
{
	if (g_lampData & LAMP_BOARD_COM)
	{
		GPIO_ResetBits(PORT_DO_LED_COM,PIN_DO_LED_COM);
	}
	else
	{
		GPIO_SetBits(PORT_DO_LED_COM,PIN_DO_LED_COM);
	}
	
	if (g_lampData & LAMP_PUMP_START)
	{
		SetLampVal( SET_BIT(g_lampExecute,PUMP_START_BIT) );		
	}
	else
	{
		SetLampVal( CLEAR_BIT(g_lampExecute,PUMP_START_BIT) );
	}
	
	if (g_lampData & LAMP_PUMP_STOP)
	{
		SetLampVal( SET_BIT(g_lampExecute,PUMP_STOP_BIT) );		
	}
	else
	{
		SetLampVal( CLEAR_BIT(g_lampExecute,PUMP_STOP_BIT) );
	}
}

/*------------------------------------------------------------
 * Function Name  : ClrOutportBit
 * Description    : ���λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void ClrOutportBit(uint32_t bit)
{
	g_OutportData &= (~bit);
} 

/*------------------------------------------------------------
 * Function Name  : SetLampBit
 * Description    : ����λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void SetOutportBit(uint32_t bit)
{
	g_OutportData |= bit;
}

/*------------------------------------------------------------
 * Function Name  : RevOutportBit
 * Description    : ȡ��λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void RevOutportBit(uint32_t bit)
{
	g_OutportData ^= bit;
}

/*------------------------------------------------------------
 * Function Name  : SetOutport
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
void SetOutport(uint32_t status)
{
	g_OutportData = status;
}

/*------------------------------------------------------------
 * Function Name  : GetOutport
 * Description    : ��ȡ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
uint32_t GetOutport( void )
{
	return g_OutportData;
}

/*------------------------------------------------------------
 * Function Name  : OutportTask
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void OutportTask( void )
{
	if (g_OutportData & DO_BEEP)
	{
		GPIO_SetBits(PORT_DO_BEEP,PIN_DO_BEEP);
	}
	else
	{
		GPIO_ResetBits(PORT_DO_BEEP,PIN_DO_BEEP);
	}
}

/*------------------------------------------------------------
 * Function Name  : InitOutport
 * Description    : ��ʼ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitBeep( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_DO_BEEP, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = PIN_DO_BEEP;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(PORT_DO_BEEP, &GPIO_InitStructure);
	
	GPIO_ResetBits(PORT_DO_BEEP,PIN_DO_BEEP);
	
	beep_cnt=0;
	beep_time=0;
	mute_comp=0;
	beep_comp=0;
	beep_status=BEEP_STATUS_IDLE;
}

/*------------------------------------------------------------
 * Function Name  : SetBeep
 * Description    : ���÷��������	��λ��10ms
 * Input          : beep_on������ʱ�䣬mute���ر�ʱ�䣬time������
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetBeep(uint16_t beep_on, uint16_t mute, uint16_t time)
{
	if (beep_status != BEEP_STATUS_IDLE)
	{
		return;
	}
	
	if(time)
	{  
		BeepOn();
		
		beep_cnt=0;
		beep_time=time;
		beep_comp=beep_on;
		mute_comp=beep_on+mute; 
		beep_status=BEEP_STATUS_BEEP; 
 	}
}

/*------------------------------------------------------------
 * Function Name  : BeepOn
 * Description    : �򿪷�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void BeepOn( void )
{
	SetOutportBit(DO_BEEP);
	
	OutportTask();
}

/*------------------------------------------------------------
 * Function Name  : BeepOff
 * Description    : �رշ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void BeepOff( void )
{
	ClrOutportBit(DO_BEEP);
	
	OutportTask();
}

/*------------------------------------------------------------
 * Function Name  : BeepISR
 * Description    : �������жϷ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void BeepISR( void )
{
	switch(beep_status)
	{
		case BEEP_STATUS_BEEP:
		{
			beep_cnt++;
			if(beep_cnt>=beep_comp)
			{
				BeepOff();
				beep_status=BEEP_STATUS_MUTE; 
			}	 
			break;
		}	
		case BEEP_STATUS_MUTE:
		{
			beep_cnt++;
			if(beep_cnt>=mute_comp)
			{
				beep_cnt=0;
				if(beep_time>1)
				{
					beep_time--; 	
					beep_status=BEEP_STATUS_BEEP;
					BeepOn();
				}	
				else
				{
					beep_status=BEEP_STATUS_IDLE; 
				}	
			}	 
			break;
		}	
		case BEEP_STATUS_IDLE:
		{ 
			break;
		}
		default:
		{
			BeepOff();
			beep_status=BEEP_STATUS_IDLE; 
			break;
		}	
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
