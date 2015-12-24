/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	Copyright (C), 2013-2014, ���������� www.armfly.com
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

