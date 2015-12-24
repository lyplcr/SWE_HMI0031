/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	�޸ļ�¼ :
*		�汾��   ����        ����     ˵��
*		V1.0    2013-12-12  armfly   ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

static void RTC_Config(void);


/* 20��RTC�������ݼĴ��� */
#define RTC_BKP_DR_NUMBER              20  

/* ѡ��ʱ��Դ */
/* #define RTC_CLOCK_SOURCE_LSE */       
#define RTC_CLOCK_SOURCE_LSI     

/* RTC �������ݼĴ��� */
uint32_t aRTC_BKP_DR[RTC_BKP_DR_NUMBER] =
{
	RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2, RTC_BKP_DR3, 
	RTC_BKP_DR4, RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7,
	RTC_BKP_DR8, RTC_BKP_DR9, RTC_BKP_DR10, RTC_BKP_DR11,
	RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14, RTC_BKP_DR15,
	RTC_BKP_DR16, RTC_BKP_DR17, RTC_BKP_DR18, RTC_BKP_DR19
};

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitRTC
*	����˵��: ��ʼ��RTC
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{
	RTC_Config();
}
	
/*
*********************************************************************************************************
*	�� �� ��: bsp_DeInitRTC
*	����˵��: �ָ�Ĭ��RTC
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void bsp_DeInitRTC(void)
{
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(DISABLE);
}
	
/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: ����RTC�����ܱ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void RTC_Config(void)
{
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* ��λ������ */
	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);
	 
    /* ѡ��RTCʱ��Դ LSI����LSE */	
	#if defined (RTC_CLOCK_SOURCE_LSI) 
		RCC_LSICmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	#elif defined (RTC_CLOCK_SOURCE_LSE) 
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	#else
		#error Please select the RTC Clock source inside the main.c file
	#endif
}

/*
*********************************************************************************************************
*	�� �� ��: WriteToRTC_BKP_DR
*	����˵��: д���ݵ�RTC�ı������ݼĴ���
*	��    �Σ���FirstRTCBackupData д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void WriteToRTC_BKP_DR(uint8_t RTCBackupIndex, uint32_t RTCBackupData)
{
	if (RTCBackupIndex < RTC_BKP_DR_NUMBER)
	{
		/* д���ݱ������ݼĴ��� */
		RTC_WriteBackupRegister(aRTC_BKP_DR[RTCBackupIndex], RTCBackupData);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ReadRTC_BKP_DR
*	����˵��: ��ȡ����������
*	��    �Σ�RTCBackupIndex �Ĵ�������ֵ
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint32_t ReadRTC_BKP_DR(uint8_t RTCBackupIndex)
{
	if (RTCBackupIndex < RTC_BKP_DR_NUMBER)
	{
		return RTC_ReadBackupRegister(aRTC_BKP_DR[RTCBackupIndex]);
	}
	
	return 0xFFFFFFFF;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
