/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: date.h
** 最后修改日期: 2014/4/25 18:07:05
** 描        述: 
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE:  日期处理函数
**********************************************************************************************************/
#ifndef __BSP_DATE_H
#define	__BSP_DATE_H
 

 
  
uint32_t days_in_year(tTime day);								//计算某一日期是一年中的第几天
int32_t days_interval(tTime d1,tTime d2);						//计算两个日期的天数差
void fstime2tTime(tTime *t,uint16_t fdate,uint16_t ftime);		//把文件系统的时间转换成tTime类型的时间 
void tTime2fstime(tTime t1,uint32_t *t2);						//tTime格式的时间转换到文件系统格式的时间 

tTime get_time(void);			//获取全局时间
void time_cycle(void);  		//需要联系执行此函数，才能获取时间
void time_set(tTime t);			//时间设置

#endif




	

	

	












