/*
*********************************************************************************************************
*
*	ģ������ : ���ݹ۲�������(DWT)ģ��
*	�ļ����� : bsp_dwt.c
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_DWT_H
#define __BSP_DWT_H

#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
	
void bsp_InitDWT(void);

void MeasureFunctionRunTimeStart( void );
void MeasureFunctionRunTimeStop( void );
uint32_t GetFunctionRunTimeUS( void );
void PrintFunctionRunTimeUS( void );
void PrintFunctionRunColokCnt( void );


#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/