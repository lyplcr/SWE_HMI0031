/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: active.h
** 最后修改日期: 2014/5/5 22:41:21
** 描        述: 软件激活函数
** 版	     本: V1.0
** 主  控  芯 片:STM32F103   晶振频率: 
** IDE:  MDK4.12
**********************************************************************************************************/
#ifndef __ACTIVE_H
#define	__ACTIVE_H

  
 	
TestStatus dvc_test_active(void);									//判断是否永久激活														 
ErrorStatus dvc_active_date_get(uint32_t *date,tTime *time);		//返回激活年月日的值,不是BCD码，将年月日写入str字符串中去
ErrorStatus dvc_product_id_get(uint32_t *id,char *str);				//返回产品编号，将产品编号str字符串中去
ErrorStatus dvc_preuse_date_get(uint32_t *date,tTime *time);		//返回上一次使用年月日的值,不是BCD码，将年月日写入str字符串中去
ErrorStatus dvc_product_id_set(char *str);							//设置产品编号
void dvc_preuse_date_set(tTime time);								//设置上一次使用的日期 
void dvc_active_set(TestStatus pass);								//设置是否永久激活
void dvc_active_date_set(tTime time);  								//激活时间设置
void dvc_limit_date_set(uint16_t date);  							//限制天数设置
  
typedef enum 
{
	ACTIVE_OK= 0,				//正常激活
	ACTIVE_ID_ERR,				//产品编号错误 
	ACTIVE_PASS_ERR,			//错误激活密码
	ACTIVE_PASS_EXPIRE			//密码过期
	//ACTIVE_DATE_ERR,			//日期错误
}ACTIVE_STATUS_TypeDef; 
//激活产品，now_date,为当前时间，pass为激活密码字符串如"234345"
ACTIVE_STATUS_TypeDef active_product(tTime now_date,const char *pass);  

typedef enum   
{
	EXPIRE_NONE=0,				//没有限制
	EXPIRE_DATE_ERR,			//如果当前日期<上次使用日期
	EXPIRE_NEAR,				//快接近到期
	EXPIRE_ACTIVE_DATE_ERR,		//激活日期错误
	EXPIRE_ACTIVE_DAY_ERR,		//激活天数错误
	EXPIRE,						//时间已到 
}EXPIRE_STATUS_TypeDef;   
//判断是否产品到期,now_date为当前日期,day为返回还剩多少天到期
EXPIRE_STATUS_TypeDef expire_judge(tTime now_date,int32_t *day);	 


#endif




	

	

	












