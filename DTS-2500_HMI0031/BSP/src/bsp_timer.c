/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.c
*	版    本 : V1.3
*	说    明 : 配置systick定时器作为系统滴答定时器。缺省定时周期为10ms。
*
*				实现了多个软件定时器供主程序使用(精度10ms)， 可以通过修改 TMR_COUNT 增减定时器个数
*				实现了ms级别延迟函数（精度10ms） 和us级延迟函数
*				实现了系统运行时间函数（10ms单位）
*				程序修改自安富莱电子 www.armfly.com
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*		V1.1    2013-06-21 armfly  增加us级延迟函数 bsp_DelayUS
*   	V1.2	2015/6/23  chengs  定时器改为10mS,程序可以移植到STM32F1xx、STM32F4XX芯片，增加US级计时函数	
*		V1.3	2015-9-17  sy	   g_iRunTime改为 uint32_t
*		V1.4	2015-9-22  sy	   使用NVIC_SetPriority();修改滴答定时器优先级为最低优先级。
*	Copyright (C), 2004-2015, 杭州鑫高科技 www.hzxingao.com
*
*********************************************************************************************************
*/
#include "bsp.h"


#define		Systick_Int_Enable()			(SysTick->CTRL)|=(1<<SysTick_CTRL_TICKINT_Pos)
#define		Systick_Int_Disable()			(SysTick->CTRL)&=~(1<<SysTick_CTRL_TICKINT_Pos)


//SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT)
   

/* 这2个全局变量转用于 bsp_DelayMS() 函数 */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_tTmr[TMR_COUNT];


/*
	全局运行时间，单位10ms
	最长可以表示 248.5天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*/
__IO uint32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
static void InitBeepTime(uint16_t Period,uint16_t Prescale);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitTimer
*	功能说明: 配置systick中断，并初始化软件定时器变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}

	/*
		配置systic中断周期为10ms，并启动systick中断。
    SystemFrequency 是固件中定义的系统内核时钟，对于STM32F103,一般为 72MHz
    对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms
    这里我们的定时周期取1mS,1000Hz
    */
	SysTick_Config(SystemCoreClock / 1000);
	
	NVIC_SetPriority(SysTick_IRQn, 15);
	
	/*
		配置TIM3中断周期为1ms，并启动TIM3定时中断。
		APB1频率42MHz，所以定时器频率84MHz，预分频84,TIM3定时时钟为1MHz，自动重装值为999，TIM3的定时周期为1ms。 
		对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms
    */
	InitBeepTime(999,83);
}



/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void bsp_RunPer1ms(void);
extern void bsp_RunPer10ms(void);

void SysTick_ISR(void)
{
	uint8_t i;

	/* 每隔1mS进来1次 （仅用于 bsp_DelayMS） */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}
	/* 每隔10ms，对软件定时器的计数器进行减一操作 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}

	/* 全局运行时间每1ms增1 */
	g_iRunTime++;
	if (g_iRunTime == 0xFFFFFFFF)	/* 这个变量是 uint32_t 类型，最大数为 0xFFFFFFFF */
	{
		g_iRunTime = 0;
	}

}

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

 

/*
*********************************************************************************************************
*	函 数 名: bsp_Delay10MS
*	功能说明: 10ms级延迟，延迟精度为正负10ms
*	形    参:  n : 延迟长度，单位10 ms。 n 应大于2
*	返 回 值: 无
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

	Systick_Int_Disable();  			/* 关中断 */

	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;

	Systick_Int_Enable();  				/* 开中断 */

	while (1)
	{

		/*
			等待延迟时间到
			注意：编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误，因此 s_ucTimeOutFlag 变量必须申明为 volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*    函 数 名: bsp_DelayUS
*    功能说明: us级延迟。 必须在systick定时器启动后才能调用此函数。
*    形    参:  n : 延迟长度，单位1 us
*    返 回 值: 无
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
    ticks = n * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK是一个递减的计数器 */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: 1ms级延迟，延迟精度为正负1ms
*	形    参:  n : 延迟长度，单位1ms。 n 应大于2
*	返 回 值: 无
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
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	Systick_Int_Disable();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	Systick_Int_Enable();  				/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartAutoTimer
*	功能说明: 启动一个自动定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);

		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	Systick_Int_Disable();  		/* 关中断 */

	s_tTmr[_id].Count = _period;			/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	Systick_Int_Enable();  			/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StopTimer
*	功能说明: 停止一个定时器
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		ECHO(DEBUG_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);

		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	Systick_Int_Disable();  	/* 关中断 */

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	Systick_Int_Enable();  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
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
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间，单位1ms。最长可以表示 49天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*	形    参:  无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	uint32_t runtime;

	Systick_Int_Disable();  	/* 关中断 */

	runtime = g_iRunTime;	/* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	Systick_Int_Enable();  		/* 开中断 */

	return runtime;
}
 
static SOFT_TMR_US	TmrUs[TMR_COUNT_US];

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimeUS
*	功能说明: US级计时开始
*	形    参:  计时器ID,0~3
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
		TmrUs[_id].startRunTime=bsp_GetRunTime();
		TmrUs[_id].startTick=SysTick->VAL;  
}
  
  
/*
*********************************************************************************************************
*	函 数 名: bsp_GetTimeUS
*	功能说明: 精确到US计时函数，最长可以计时2147秒,精度1uS。
*	形    参: 计时器ID,0~3
*	返 回 值: 返回自执行 bsp_StartTimerUS开始到本函数执行所运行的时间，单位Us
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
*	函 数 名: SysTick_Handler
*	功能说明: 系统嘀嗒定时器中断服务程序。启动文件中引用了该函数。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
	SysTick_ISR();
}

/*------------------------------------------------------------
 * Function Name  : InitBeepTime
 * Description    : 初始化蜂鸣器定时器
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitBeepTime(uint16_t Period,uint16_t Prescale)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  			//使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = Period; 					//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=Prescale;  				//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//初始化TIM3
		
	TIM_ClearFlag(TIM3,TIM_IT_Update);								//清除更新标志
	TIM_SetCounter(TIM3,0);																					//设置定时器值
	 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//允许定时器3更新中断																						//使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; 		//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 			//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM3,ENABLE); 
}

/*------------------------------------------------------------
 * Function Name  : TIM3_IRQHandler
 * Description    : 定时器中断服务函数
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



/***************************** 杭州鑫高科技 www.hzxingao.com (END OF FILE) *********************************/
