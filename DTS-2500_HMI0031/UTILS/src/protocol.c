/**
  ******************************************************************************
  * @file    protocol.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ͨѶЭ����غ���
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
#include "bsp.h"
#include "protocol.h"

 
 
uint8_t index_h_asw[2]={0x7F,0x7F};  
uint8_t index_hd_nasw[2]={0xBF,0xBF};
uint8_t index_d_asw[2]={0xFF,0xFF};				//�ӻ�����������ҪӦ��� 
 
 
/**********************************************************************
functionName:uint8_t next_index(INDEX_TypeDef index_type,INDEX_Channal_TypeDef chn)
description:��ȡ��һ��index
INDEX_H_ASW=0x00,			//���������������ҪӦ��ĳ�������
INDEX_HD_NASW, 				//������ӻ���������Ĳ���Ӧ��ĳ�������
INDEX_D_ASW, 				//�豸����ģ�����Ӧ���豸�ĳ�������
INDEX_IVLD=0xFF,			//�Ƿ�����
**********************************************************************/
uint8_t next_index(INDEX_TypeDef index_type,COM_ADD_TypeDef add)
{
	
	if(index_type==INDEX_H_ASW)							//0x20-0x7F  �����������ҪӦ��
	{
		index_h_asw[add]++;
		if(index_h_asw[add]>0x7F)
		index_h_asw[add]=0x20;
		return index_h_asw[add];
	}
	else if(index_type==INDEX_HD_NASW)					//0x80-0xBF   ���������豸��������Ĳ���ҪӦ���
	{
		index_hd_nasw[add]++;
		if(index_hd_nasw[add]>0xBF)
		index_hd_nasw[add]=0x80;
		return index_hd_nasw[add];	
	}
	else if(index_type==INDEX_D_ASW)					//0xC0-0xFF	 �豸�������ҪӦ���
	{
		index_d_asw[add]++;
		if(!index_d_asw[add])
		index_d_asw[add]=0xC0;
		return index_d_asw[add];
	}	
	else
	return 0x00;			
}  
  
/**********************************************************************
functionName:uint8_t index_get(INDEX_TypeDef index_type,COM_ADD_TypeDef add)
description:��ȡ��ǰ��index 
**********************************************************************/
uint8_t index_get(INDEX_TypeDef index_type,COM_ADD_TypeDef add)
{
	if(index_type==INDEX_H_ASW)	
	return 	index_h_asw[add];
	else if(index_type==INDEX_HD_NASW)		
	return 	index_hd_nasw[add];
	else
	return index_d_asw[add];
}
   
  
/**********************************************************************
functionName:INDEX_TypeDef index_type_get(uint8_t index)
description:ʶ���INDEX����
**********************************************************************/ 
INDEX_TypeDef index_type_get(uint8_t index)
{
	INDEX_TypeDef index_type;  
	if((index>=0x20)&&(index<=0x7F))
	index_type=INDEX_H_ASW;
	else if((index>=0x80)&&(index<=0xBF))
	index_type=INDEX_HD_NASW;
	else if((index>=0xC0)&&(index<=0xFF)) 
	index_type=INDEX_D_ASW;
	else
  	index_type=INDEX_IVLD;
	return(index_type);
}    



  
   
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/  
