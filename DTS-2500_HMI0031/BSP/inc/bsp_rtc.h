/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H

void bsp_InitRTC(void);
void bsp_DeInitRTC(void);
void WriteToRTC_BKP_DR(uint8_t RTCBackupIndex, uint32_t RTCBackupData);
uint32_t ReadRTC_BKP_DR(uint8_t RTCBackupIndex);


#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

