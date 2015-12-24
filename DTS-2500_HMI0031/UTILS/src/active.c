/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: active.c
** 最后修改日期: 2014/5/5 22:40:37
** 描        述: 软件激活函数 
** 版	     本: V1.0
** 主  控  芯 片:STM32F103 		晶振频率: 
** IDE:MDK 4.12
**********************************************************************************************************/
#include "bsp.h"
#include "active.h"
#include "prm_struct.h"
#include "prm.h"


/**********************************************************************
functionName:uint16_t dvc_limit_passs_get(void)
description:判断是否永久激活	
**********************************************************************/ 
TestStatus dvc_test_active(void)
{ 
	SYS_PRM_TypeDef *prm;
	uint16_t temp;
	DEVICE_INFO_TypeDef *dvc_info; 
	prm=(SYS_PRM_TypeDef *)get_prm(); 
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	temp=dvc_info->limit_pass; 
	if(temp==0x1111)
	return PASSED;
	else
	return FAILED;	
}
 
 
/**********************************************************************
functionName:ErrorStatus dvc_active_date_get(uint32_t *date,tTime *time)
description:返回激活年月日的值,不是BCD码
**********************************************************************/  
ErrorStatus dvc_active_date_get(uint32_t *date,tTime *time)
{
	SYS_PRM_TypeDef *prm;
	uint32_t temp32u;
	uint16_t temp16u;
	ErrorStatus status=SUCCESS;	 
	DEVICE_INFO_TypeDef *dvc_info;
	
	prm=(SYS_PRM_TypeDef *)get_prm();  
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	temp16u=dvc_info->active_year; 
	temp32u=temp16u;
	temp32u<<=16;
	
	temp16u=dvc_info->active_date; 
	temp32u+=temp16u;					//获得日期32位BCD码：0x20110624
	 
	status=bcd2val_32(temp32u,date);
 	if(status==ERROR)
 	return (ERROR); 	
 	    
	bcd2val_16(dvc_info->active_year,(uint16_t *)&(time->usYear));	
	bcd2val_8((temp16u>>8),&(time->ucMon));
	bcd2val_8((temp16u&0x00FF),&(time->ucMday));
	return (SUCCESS); 
}
  
   
  
/**********************************************************************
functionName:ErrorStatus dvc_product_id_get(uint32_t *date,char *str)
description:返回产品编号，将产品编号str字符串中去
**********************************************************************/    
ErrorStatus dvc_product_id_get(uint32_t *id,char *str)
{
	SYS_PRM_TypeDef *prm;
	uint32_t temp32u;
	uint16_t temp16u;
	ErrorStatus status;	 
	DEVICE_INFO_TypeDef *dvc_info;
	
	prm=(SYS_PRM_TypeDef *)get_prm();  
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	
	temp16u=dvc_info->product_idh; 
	temp32u=temp16u;
	temp32u<<=16;
	
	temp16u=dvc_info->product_idl; 
	temp32u+=temp16u;					//获得编号32位BCD码：0x20110624
	 
	//*id=bcd2val_32(temp32u); 
	status=bcd2val_32(temp32u,id);
	if(status==ERROR)
	return ERROR;
	status=bcd2str_32(temp32u,str);		 
	str[8]=0;
	return (status);  
}

  
/**********************************************************************
functionName:ErrorStatus dvc_preuse_date_get(uint32_t *date,char *str)
description:返回上一次使用年月日的值,不是BCD码，将年月日写入str字符串中去
**********************************************************************/     
ErrorStatus dvc_preuse_date_get(uint32_t *date,tTime *time)
{
	SYS_PRM_TypeDef *prm;
	uint32_t temp32u;
	uint16_t temp16u;
	ErrorStatus status;	 
	DEVICE_INFO_TypeDef *dvc_info;
	
	prm=(SYS_PRM_TypeDef *)get_prm();  
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	temp16u=dvc_info->pre_use_year; 
	temp32u=temp16u;
	temp32u<<=16;
	temp16u=dvc_info->pre_use_date; 
	temp32u+=temp16u;					//获得日期32位BCD码：0x20110624
	status=bcd2val_32(temp32u,date);
	if(status==ERROR)
 	return (ERROR); 
	bcd2val_16(dvc_info->pre_use_year,(uint16_t *)&(time->usYear));	
	bcd2val_8((temp16u>>8),&(time->ucMon));
	bcd2val_8((temp16u&0x00FF),&(time->ucMday));
	return (SUCCESS); 
}
   
   
/**********************************************************************
functionName:uint16_t dvc_expire_day_get(void)
description:返回限制时间
**********************************************************************/      
uint16_t dvc_expire_day_get(void)
{
	SYS_PRM_TypeDef *prm;
	DEVICE_INFO_TypeDef *dvc_info;
	
	prm=(SYS_PRM_TypeDef *)get_prm(); 
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	return (dvc_info->expire_day);
}    
   
   
/**********************************************************************
functionName:void dvc_active_set(TestStatus pass)
description:设置是否永久激活
**********************************************************************/ 
void dvc_active_set(TestStatus pass)
{  
	SYS_PRM_TypeDef *prm;
	DEVICE_INFO_TypeDef *dvc_info;
	prm=(SYS_PRM_TypeDef *)get_prm(); 
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info; 
	if(pass==PASSED)
	dvc_info->limit_pass=0x1111;
	else
	dvc_info->limit_pass=0;		 
}


/**********************************************************************
functionName:void dvc_expire_day_set(uint16_t date)
description:限制天数设置
**********************************************************************/ 
void dvc_expire_day_set(uint16_t date)
{
	SYS_PRM_TypeDef *prm;
	DEVICE_INFO_TypeDef *dvc_info;
	prm=(SYS_PRM_TypeDef *)get_prm(); 
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info; 
	dvc_info->use_time_max=date*100;
	dvc_info->time_use=0;
	dvc_info->expire_day=date;
}
 

/**********************************************************************
functionName:void dvc_active_date_set(tTime time)
description: 激活时间设置
**********************************************************************/ 
void dvc_active_date_set(tTime time)
{
	SYS_PRM_TypeDef *prm;
	uint16_t temp16u;
	DEVICE_INFO_TypeDef *dvc_info; 
	prm=(SYS_PRM_TypeDef *)get_prm(); 
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	temp16u = time.ucMon;
	temp16u*=100;
	temp16u+=time.ucMday; 
	dvc_info->active_year=val2bcd_16(time.usYear);
	dvc_info->active_date=val2bcd_16(temp16u); 
}


/**********************************************************************
functionName:ErrorStatus dvc_product_id_set(char *str)
description:设置产品编号
**********************************************************************/  
ErrorStatus dvc_product_id_set(char *str)
{
	SYS_PRM_TypeDef *prm;
	DEVICE_INFO_TypeDef *dvc_info; 
	uint32_t id;		//产品编号
	ErrorStatus status;
	prm=(SYS_PRM_TypeDef *)get_prm();
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info;
	
	status=str2bcd_32(&id,str);  
	if(status==ERROR) 
	return 	ERROR;
	else
	{
		dvc_info->product_idh=id>>16;
		dvc_info->product_idl=id&0x0000ffff; 
		prm_save();
	}	
	return SUCCESS;	 
}


/**********************************************************************
functionName:void dvc_preuse_date_set(tTime time)
description: 设置上一次使用的日期
**********************************************************************/     
void dvc_preuse_date_set(tTime time)
{
	SYS_PRM_TypeDef *prm;
	DEVICE_INFO_TypeDef *dvc_info;  
	uint16_t temp16u;
	prm=(SYS_PRM_TypeDef *)get_prm();
	dvc_info=(DEVICE_INFO_TypeDef *)prm->prm_insd.dvc_info; 
	temp16u=time.ucMon;
	temp16u*=100;
	temp16u+=time.ucMday;  
	dvc_info->pre_use_year=val2bcd_16(time.usYear);
	dvc_info->pre_use_date=val2bcd_16(temp16u); 
}




/**********************************************************************
functionName:ACTIVE_STATUS_TypeDef active_product(tTime time,const char *pass)
description: 激活产品，time=当前时间，pass=激活密码字符串
**********************************************************************/  
ACTIVE_STATUS_TypeDef active_product(tTime now_date,const char *pass)
{
	ErrorStatus status;
	char str[9];
	uint32_t temp32u;
	uint32_t pass_val;
	uint32_t pass7[7];  
	uint32_t active_date;
	uint32_t date;
	uint32_t id; 
	uint8_t same_day=0;
	tTime tempt;

	status=dvc_product_id_get(&id,str);
	if(status==ERROR)
	return 	ACTIVE_ID_ERR; 
	id=id/10000+(id%10000)*10000;
	
	date=now_date.ucMon;
	date*=100;
	date+=now_date.ucMday;
	date*=10000ul;
	date+=now_date.usYear; 
	temp32u=date+id;
	id=isqrt(temp32u);
	if(id*id!=temp32u)
	id+=1;				  
	pass7[0]=id;
	pass7[0]*=111ul;		//7天 
	pass7[1]=id*122ul;		//30天
	pass7[2]=id*133ul;		//60天
	pass7[3]=id*144ul;		//永久
	pass7[4]=id*155ul;		//90天
	pass7[5]=id*156ul;		//180天
	pass7[6]=id*157ul;		//365天
	 
	dvc_active_date_get(&active_date,&tempt); 
	active_date=active_date/10000+(active_date%10000)*10000;
	if(active_date==date) 
	same_day=1;	
	active_date=dvc_expire_day_get();	
	pass_val=ustrtoul(pass,0,10); 		//输入的激活密码
	 
	if(pass_val==pass7[0])	
	{
		if(same_day&&(active_date==7))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_set(FAILED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(7);
	}
	else if(pass_val==pass7[1])	
	{
		if(same_day&&(active_date==30))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_set(FAILED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(30);
	}
	else if(pass_val==pass7[2])	
	{
		if(same_day&&(active_date==60))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_set(FAILED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(60);
	}
	else if(pass_val==pass7[3])	
	{
		dvc_active_set(PASSED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(60);
	}
	else if(pass_val==pass7[4])	
	{
		if(same_day&&(active_date==90))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_active_set(FAILED);
		dvc_expire_day_set(90);
	}
	else if(pass_val==pass7[5])	
	{
		if(same_day&&(active_date==180))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_set(FAILED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(180);
	}
	else if(pass_val==pass7[6])	
	{
		if(same_day&&(active_date==365))
		return 	ACTIVE_PASS_EXPIRE;
		dvc_active_set(FAILED);
		dvc_active_date_set(now_date);
		dvc_preuse_date_set(now_date);
		dvc_expire_day_set(365);
	}
	else
	{
		return ACTIVE_PASS_ERR;
	} 
	prm_save();
	return ACTIVE_OK;
}  
  
const uint16_t experience_day[6]={7,30,60,90,180,365};   
  
/**********************************************************************
functionName:EXPIRE_STATUS_TypeDef expire_judge(uint16_t *day)
description: 判断是否产品到期,day为返回还剩多少天到期
**********************************************************************/    
EXPIRE_STATUS_TypeDef expire_judge(tTime now_date,int32_t *day)
{
 	ErrorStatus err;
	uint32_t temp32u; 	
 	tTime preuse_date,active_date;
 	int interval;
 	uint8_t i;
 	uint16_t exday;
 	uint8_t flag=0;	  
	*day=0;

 	if(dvc_test_active()==PASSED)
 	return 	EXPIRE_NONE;
 	
 	err=dvc_preuse_date_get(&temp32u,&preuse_date); 
 	  
 	interval=days_interval(now_date,preuse_date);
 	if(interval>0)
 	return 	EXPIRE_DATE_ERR;	 
 	dvc_preuse_date_set(now_date);
 	prm_save();
 	
 	
 	err=dvc_active_date_get(&temp32u,&active_date);	
 	if(err==ERROR)
 	return 	EXPIRE_ACTIVE_DATE_ERR;
 	
 	exday=dvc_expire_day_get();
 	for(i=0;i<6;i++)
 	{
 		if(exday==experience_day[i])
 		{
 			flag=1;
 			break;
 		}	
 	}
 	if(!flag)
 	return 	EXPIRE_ACTIVE_DAY_ERR;	
 	
 	interval=days_interval(active_date,now_date); 
 	*day=(int32_t)exday-interval;  
 	if(*day<0)
 	return 	EXPIRE;
 	else if(*day<7)	
	return EXPIRE_NEAR;	 
 	else
 	return  EXPIRE_NONE;	 
} 

  


/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/  
