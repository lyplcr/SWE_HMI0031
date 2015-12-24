/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	修改记录 :
*		版本号   日期        作者     说明
*		V1.0    2013-12-12  armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

static void RTC_Config(void);


/* 20个RTC备份数据寄存器 */
#define RTC_BKP_DR_NUMBER              20  

/* 选择时钟源 */
/* #define RTC_CLOCK_SOURCE_LSE */       
#define RTC_CLOCK_SOURCE_LSI     

/* RTC 备份数据寄存器 */
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
*	函 数 名: bsp_InitRTC
*	功能说明: 初始化RTC
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{
	RTC_Config();
}
	
/*
*********************************************************************************************************
*	函 数 名: bsp_DeInitRTC
*	功能说明: 恢复默认RTC
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void bsp_DeInitRTC(void)
{
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(DISABLE);
}
	
/*
*********************************************************************************************************
*	函 数 名: RTC_Config
*	功能说明: 配置RTC用于跑表
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void RTC_Config(void)
{
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* 复位备份域 */
	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);
	 
    /* 选择RTC时钟源 LSI或者LSE */	
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
*	函 数 名: WriteToRTC_BKP_DR
*	功能说明: 写数据到RTC的备份数据寄存器
*	形    参：无FirstRTCBackupData 写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void WriteToRTC_BKP_DR(uint8_t RTCBackupIndex, uint32_t RTCBackupData)
{
	if (RTCBackupIndex < RTC_BKP_DR_NUMBER)
	{
		/* 写数据备份数据寄存器 */
		RTC_WriteBackupRegister(aRTC_BKP_DR[RTCBackupIndex], RTCBackupData);
	}
}

/*
*********************************************************************************************************
*	函 数 名: ReadRTC_BKP_DR
*	功能说明: 读取备份域数据
*	形    参：RTCBackupIndex 寄存器索引值
*	返 回 值: 
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
