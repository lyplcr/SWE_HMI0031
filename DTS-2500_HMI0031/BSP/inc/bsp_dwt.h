/*
*********************************************************************************************************
*
*	模块名称 : 数据观察点与跟踪(DWT)模块
*	文件名称 : bsp_dwt.c
*	版    本 : V1.0
*	说    明 : 头文件
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
