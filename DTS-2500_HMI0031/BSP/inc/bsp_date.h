/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: date.h
** ����޸�����: 2014/4/25 18:07:05
** ��        ��: 
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE:  ���ڴ�����
**********************************************************************************************************/
#ifndef __BSP_DATE_H
#define	__BSP_DATE_H
 

 
  
uint32_t days_in_year(tTime day);								//����ĳһ������һ���еĵڼ���
int32_t days_interval(tTime d1,tTime d2);						//�����������ڵ�������
void fstime2tTime(tTime *t,uint16_t fdate,uint16_t ftime);		//���ļ�ϵͳ��ʱ��ת����tTime���͵�ʱ�� 
void tTime2fstime(tTime t1,uint32_t *t2);						//tTime��ʽ��ʱ��ת�����ļ�ϵͳ��ʽ��ʱ�� 

tTime get_time(void);			//��ȡȫ��ʱ��
void time_cycle(void);  		//��Ҫ��ϵִ�д˺��������ܻ�ȡʱ��
void time_set(tTime t);			//ʱ������

#endif




	

	

	












