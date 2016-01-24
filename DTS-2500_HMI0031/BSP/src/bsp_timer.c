/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.c
*	��    �� : V1.3
*	˵    �� : ����systick��ʱ����Ϊϵͳ�δ�ʱ����ȱʡ��ʱ����Ϊ10ms��
*
*				ʵ���˶�������ʱ����������ʹ��(����10ms)�� ����ͨ���޸� TMR_COUNT ������ʱ������
*				ʵ����ms�����ӳٺ���������10ms�� ��us���ӳٺ���
*				ʵ����ϵͳ����ʱ�亯����10ms��λ��
*				�����޸��԰��������� www.armfly.com
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*		V1.1    2013-06-21 armfly  ����us���ӳٺ��� bsp_DelayUS
*   	V1.2	2015/6/23  chengs  ��ʱ����Ϊ10mS,���������ֲ��STM32F1xx��STM32F4XXоƬ������US����ʱ����	
*		V1.3	2015-9-17  sy	   g_iRunTime��Ϊ uint32_t
*		V1.4	2015-9-22  sy	   ʹ��NVIC_SetPriority();�޸ĵδ�ʱ�����ȼ�Ϊ������ȼ���
*	Copyright (C), 2004-2015, �����θ߿Ƽ� www.hzxingao.com
*
*********************************************************************************************************
*/
#include "bsp.h"


#define		Systick_Int_Enable()			(SysTick->CTRL)|=(1<<SysTick_CTRL_TICKINT_Pos)
#define		Systick_Int_Disable()			(SysTick->CTRL)&=~(1<<SysTick_CTRL_TICKINT_Pos)


//SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT)
   

/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_tTmr[TMR_COUNT];


/*
	ȫ������ʱ�䣬��λ10ms
	����Ա�ʾ 248.5�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*/
__IO uint32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
static void InitBeepTime(uint16_t Period,uint16_t Prescale);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTimer
*	����˵��: ����systick�жϣ�����ʼ�������ʱ������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}

	/*
		����systic�ж�����Ϊ10ms��������systick�жϡ�
    SystemFrequency �ǹ̼��ж����ϵͳ�ں�ʱ�ӣ�����STM32F103,һ��Ϊ 72MHz
    ���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    �������ǵĶ�ʱ����ȡ1mS,1000Hz
    */
	SysTick_Config(SystemCoreClock / 1000);
	
	NVIC_SetPriority(SysTick_IRQn, 15);
	
	/*
		����TIM3�ж�����Ϊ1ms��������TIM3��ʱ�жϡ�
		APB1Ƶ��42MHz�����Զ�ʱ��Ƶ��84MHz��Ԥ��Ƶ84,TIM3��ʱʱ��Ϊ1MHz���Զ���װֵΪ999��TIM3�Ķ�ʱ����Ϊ1ms�� 
		���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    */
	InitBeepTime(999,83);
}



/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��1ms����1��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void bsp_RunPer1ms(void);
extern void bsp_RunPer10ms(void);

void SysTick_ISR(void)
{
	uint8_t i;

	/* ÿ��1mS����1�� �������� bsp_DelayMS�� */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}
	/* ÿ��10ms���������ʱ���ļ��������м�һ���� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}

	/* ȫ������ʱ��ÿ1ms��1 */
	g_iRunTime++;
	if (g_iRunTime == 0xFFFFFFFF)	/* ��������� uint32_t ���ͣ������Ϊ 0xFFFFFFFF */
	{
		g_iRunTime = 0;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SoftTimerDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��:  _tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

 

/*
*********************************************************************************************************
*	�� �� ��: bsp_Delay10MS
*	����˵��: 10ms���ӳ٣��ӳپ���Ϊ����10ms
*	��    ��:  n : �ӳٳ��ȣ���λ10 ms�� n Ӧ����2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Delay10MS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	Systick_Int_Disable();  			/* ���ж� */

	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;

	Systick_Int_Enable();  				/* ���ж� */

	while (1)
	{

		/*
			�ȴ��ӳ�ʱ�䵽
			ע�⣺��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�������� s_ucTimeOutFlag ������������Ϊ volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_DelayUS
*    ����˵��: us���ӳ١� ������systick��ʱ����������ܵ��ô˺�����
*    ��    ��:  n : �ӳٳ��ȣ���λ1 us
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK��һ���ݼ��ļ����� */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: 1ms���ӳ٣��ӳپ���Ϊ����1ms
*	��    ��:  n : �ӳٳ��ȣ���λ1ms�� n Ӧ����2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	while(n)
	{
		bsp_DelayUS(1000);
		n--;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	Systick_Int_Disable();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	Systick_Int_Enable();  				/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartAutoTimer
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);

		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	Systick_Int_Disable();  		/* ���ж� */

	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	Systick_Int_Enable();  			/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StopTimer
*	����˵��: ֹͣһ����ʱ��
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);

		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	Systick_Int_Disable();  	/* ���ж� */

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	Systick_Int_Enable();  		/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CheckTimer
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_GetRunTime
*	����˵��: ��ȡCPU����ʱ�䣬��λ1ms������Ա�ʾ 49�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
*	��    ��:  ��
*	�� �� ֵ: CPU����ʱ�䣬��λ1ms
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	uint32_t runtime;

	Systick_Int_Disable();  	/* ���ж� */

	runtime = g_iRunTime;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	Systick_Int_Enable();  		/* ���ж� */

	return runtime;
}
 
static SOFT_TMR_US	TmrUs[TMR_COUNT_US];

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimeUS
*	����˵��: US����ʱ��ʼ
*	��    ��:  ��ʱ��ID,0~3
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
		TmrUs[_id].startRunTime=bsp_GetRunTime();
		TmrUs[_id].startTick=SysTick->VAL;  
}
  
  
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetTimeUS
*	����˵��: ��ȷ��US��ʱ����������Լ�ʱ2147��,����1uS��
*	��    ��: ��ʱ��ID,0~3
*	�� �� ֵ: ������ִ�� bsp_StartTimerUS��ʼ��������ִ�������е�ʱ�䣬��λUs
*********************************************************************************************************
*/ 
uint32_t bsp_GetTimeUS(uint8_t _id)
{
	uint32_t Us;
	uint32_t	endRunTime;
	uint32_t load=0;
	uint32_t endTick;
	endTick=SysTick->VAL;
	endRunTime=bsp_GetRunTime();
	load=SysTick->LOAD;  
	Us=((endRunTime-TmrUs[_id].startRunTime)*(load+1)+TmrUs[_id].startTick-endTick)/(SystemCoreClock / 1000000L); 	 
	return Us;
}

 

/*
*********************************************************************************************************
*	�� �� ��: SysTick_Handler
*	����˵��: ϵͳ��શ�ʱ���жϷ�����������ļ��������˸ú�����
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
	SysTick_ISR();
}

/*------------------------------------------------------------
 * Function Name  : InitBeepTime
 * Description    : ��ʼ����������ʱ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitBeepTime(uint16_t Period,uint16_t Prescale)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  			//ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = Period; 					//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=Prescale;  				//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//��ʼ��TIM3
		
	TIM_ClearFlag(TIM3,TIM_IT_Update);								//������±�־
	TIM_SetCounter(TIM3,0);																					//���ö�ʱ��ֵ
	 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//����ʱ��3�����ж�																						//ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; 		//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 			//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM3,ENABLE); 
}

/*------------------------------------------------------------
 * Function Name  : TIM3_IRQHandler
 * Description    : ��ʱ���жϷ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	__IO static uint8_t s_count = 0;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 

		if (++s_count >= 10)		
		{
			s_count = 0;
				
			BeepISR();
		}
	}	  
} 



/***************************** �����θ߿Ƽ� www.hzxingao.com (END OF FILE) *********************************/
