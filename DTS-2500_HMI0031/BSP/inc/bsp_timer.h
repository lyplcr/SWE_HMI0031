/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.1
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H


/*
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT		10			/* �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) */
#define	TMR_COUNT_US	4			//�����ʱ�������ڼ����¼���ʱ����ʱ��ID��Χ0-3				

 

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;

 
/* uS����ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint32_t startRunTime;	/* ������ */
	volatile uint32_t startTick;	/* ������Ԥװֵ */
}SOFT_TMR_US;

typedef enum
{
	MAIN_CYCLE_TIMING = 0,		//0
	CMD_MUTUAL_TIMING,			//1
	LINK_MUTUAL_TIMING,			//2
	KEY_TIMING,					//3
	PUTIN_SHOW_TIMING,			//4
	PUTIN_WAIT_CHAR_TIMING,		//5
	HARD_TEST_WAIT_TIMING,		//6
	SEND_CMD_TIMEOUT,			//7
	GENERAL_TIMEOUT_TIMING,		//8
}USE_TIM_ID_TypeDef;


/* �ṩ������C�ļ����õĺ��� */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_DelayUS(uint32_t n);
void bsp_Delay10MS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
uint32_t bsp_GetRunTime(void);

void bsp_StartTimeUS(uint8_t _id);
uint32_t bsp_GetTimeUS(uint8_t _id);

void TimerStart( uint8_t id );
uint32_t TimerEnd( uint8_t id );
void TimerPrintf( uint8_t id );



#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
