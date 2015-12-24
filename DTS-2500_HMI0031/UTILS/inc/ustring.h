/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: ustring.h
** ����޸�����: 2014/4/25 18:07:05
** ��        ��: 
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE:  �ַ���������
**********************************************************************************************************/
#ifndef __USTRING_H
#define	__USTRING_H

#include "stm32f4xx.h"
 
ErrorStatus bcd2val_8(uint8_t bcd,uint8_t *val);
uint8_t val2bcd_8(uint8_t val);
uint16_t val2bcd_16(uint16_t val);
ErrorStatus bcd2val_16(uint16_t bcd,uint16_t *val);
uint32_t val2bcd_32(uint32_t val);
ErrorStatus bcd2val_32(uint32_t bcd,uint32_t *val);
ErrorStatus bcd2str_16(uint16_t bcd,char *str); 
ErrorStatus bcd2str_32(uint32_t bcd,char *str);
void hex2str_32(uint32_t hex,char *str);
void hex2str_16(uint16_t hex,char *str);
void hex2str_8(uint8_t hex,char *str);
ErrorStatus str2bcd_32(uint32_t *bcd,char *str);
uint16_t float2str(float f,char *str,uint8_t int_len,uint8_t dec_len);
float str2float(char *str);
TestStatus  buffcmp(uint8_t *buff1,uint8_t *buff2,uint16_t len);
 
 
 

#endif




	

	

	












