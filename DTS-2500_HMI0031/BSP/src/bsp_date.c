/**
  ******************************************************************************
  * @file    bsp_date.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ʱ�������ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
  
#include "bsp.h"



  
//�ж��Ƿ�������
#define leap(y) ((y)%4==0 && (y)%100!=0 || (y)%400==0)
 
 
//����:1�µ�12��ÿ���µ����� 31,29,31,30,31,30,31,31,30,31,30,31
//������:1�µ�12��ÿ���µ����� 31,28,31,30,31,30,31,31,30,31,30,31
//mon����Ƿ����굱ǰ��֮ǰ�������� 
const uint16_t  mon[2][13]= 
{
	{0,0,31,59,90,120,151,181,212,243,273,304,334},
	{0,0,31,60,91,121,152,182,213,244,274,305,335}
};
			  
/**********************************************************************
functionName:uint32 days_in_year(struct d day)
description:���������������һ���еĵڼ���
**********************************************************************/ 
uint32_t days_in_year(tTime day)
{
	uint32_t sum=0;
	sum=mon[leap(day.usYear)][day.ucMon]+day.ucMday;
	return sum;
} 
 
/**********************************************************************
functionName:int days_interval(DATE_TypeDef d1,DATE_TypeDef d2)
description:����������������֮���������� 
**********************************************************************/  
int32_t days_interval(tTime d1,tTime d2)
{
	tTime temp_day;
	int32_t sum;
	int32_t i;
	uint8_t flag=0; 
	if(d1.usYear>d2.usYear)
	{
		temp_day=d2;
		d2=d1;
		d1=temp_day;
		flag=1;
	}	
	sum=days_in_year(d2)-days_in_year(d1);				//����ͬһ���в��; 
	for(i=d1.usYear;i<d2.usYear;i++)
	sum+=leap(i)?366:365;	
	if(flag)
	sum=-sum;	
	return sum;
} 


/**********************************************************************
functionName:void fstime2tTime(tTime *t1,uint32 t2)
description: ���ļ�ϵͳ��ʱ��ת����tTime���͵�ʱ�� 
**********************************************************************/  
void fstime2tTime(tTime *t,uint16_t fdate,uint16_t ftime)
{
	t->usYear=(fdate>>9)+1980;
	t->ucMon=(fdate>>5)&0x0F;
	t->ucMday=fdate&0x1F;
	t->ucHour=ftime>>11;
	t->ucMin=(ftime>>5)&0x3F;
	t->ucSec =(ftime&0x1F)<<1;
}

/**********************************************************************
functionName:void tTime2fstime(tTime t1,uint32_t *t2)
description: tTime��ʽ��ʱ��ת�����ļ�ϵͳ��ʽ��ʱ��
**********************************************************************/    
void tTime2fstime(tTime t1,uint32_t *t2)
{
	*t2=((t1.usYear-1980) << 25)   	 	// Year = 2007
            | (t1.ucMon << 21)            	// Month = June
            | (t1.ucMday << 16)           	// Day = 5
            ;
} 

//void tTime2fstime(tTime t1,uint32_t *t2)
//{
//	*t2=((t1.usYear-1980) << 25)   	 			// Year = 2007
//            | (t1.ucMon << 21)            	// Month = June
//            | (t1.ucMday << 16)           	// Day = 5
//            | (t1.ucHour << 11)           	// Hour = 11
//            | (t1.ucMin << 5)            		// Min = 38
//            | (t1.ucSec >> 1)             	// Sec = 0
//            ;
//}


tTime	time_global;		//ȫ��ʱ�� 


/**********************************************************************
functionName:tTime get_time(void)
description:ͨ���˺�����ȡȫ��ʱ��
**********************************************************************/
tTime get_time(void)
{
	return time_global;
}
 
 
/**********************************************************************
functionName:void time_cycle(void)
description:��Ҫ�������еĺ�����ͨ���˺�����ʱ�����ʱ��ȫ�ֱ���
**********************************************************************/ 
void time_cycle(void)
{
	time_get(&time_global);
	date_get(&time_global);
}



/**********************************************************************
functionName:void time_set(tTime t)
description:ʱ����������
**********************************************************************/ 
void time_set(tTime t)
{
	pcf8563_time_set(&t);	
}
  
  
 
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/
