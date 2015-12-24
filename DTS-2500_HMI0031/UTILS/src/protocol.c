/**
  ******************************************************************************
  * @file    protocol.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   通讯协议相关函数
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
uint8_t index_d_asw[2]={0xFF,0xFF};				//从机主动发起，需要应答的 
 
 
/**********************************************************************
functionName:uint8_t next_index(INDEX_TypeDef index_type,INDEX_Channal_TypeDef chn)
description:获取下一个index
INDEX_H_ASW=0x00,			//主机主动发起的需要应答的常规数据
INDEX_HD_NASW, 				//主机或从机主动发起的不需应答的常规数据
INDEX_D_ASW, 				//设备发起的，主机应答设备的常规数据
INDEX_IVLD=0xFF,			//非法索引
**********************************************************************/
uint8_t next_index(INDEX_TypeDef index_type,COM_ADD_TypeDef add)
{
	
	if(index_type==INDEX_H_ASW)							//0x20-0x7F  主机发起的需要应答
	{
		index_h_asw[add]++;
		if(index_h_asw[add]>0x7F)
		index_h_asw[add]=0x20;
		return index_h_asw[add];
	}
	else if(index_type==INDEX_HD_NASW)					//0x80-0xBF   主机或者设备主动发起的不需要应答的
	{
		index_hd_nasw[add]++;
		if(index_hd_nasw[add]>0xBF)
		index_hd_nasw[add]=0x80;
		return index_hd_nasw[add];	
	}
	else if(index_type==INDEX_D_ASW)					//0xC0-0xFF	 设备发起的需要应答的
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
description:获取当前的index 
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
description:识别的INDEX类型
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
