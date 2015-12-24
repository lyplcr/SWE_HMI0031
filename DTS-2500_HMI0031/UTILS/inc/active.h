/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: active.h
** ����޸�����: 2014/5/5 22:41:21
** ��        ��: ��������
** ��	     ��: V1.0
** ��  ��  о Ƭ:STM32F103   ����Ƶ��: 
** IDE:  MDK4.12
**********************************************************************************************************/
#ifndef __ACTIVE_H
#define	__ACTIVE_H

  
 	
TestStatus dvc_test_active(void);									//�ж��Ƿ����ü���														 
ErrorStatus dvc_active_date_get(uint32_t *date,tTime *time);		//���ؼ��������յ�ֵ,����BCD�룬��������д��str�ַ�����ȥ
ErrorStatus dvc_product_id_get(uint32_t *id,char *str);				//���ز�Ʒ��ţ�����Ʒ���str�ַ�����ȥ
ErrorStatus dvc_preuse_date_get(uint32_t *date,tTime *time);		//������һ��ʹ�������յ�ֵ,����BCD�룬��������д��str�ַ�����ȥ
ErrorStatus dvc_product_id_set(char *str);							//���ò�Ʒ���
void dvc_preuse_date_set(tTime time);								//������һ��ʹ�õ����� 
void dvc_active_set(TestStatus pass);								//�����Ƿ����ü���
void dvc_active_date_set(tTime time);  								//����ʱ������
void dvc_limit_date_set(uint16_t date);  							//������������
  
typedef enum 
{
	ACTIVE_OK= 0,				//��������
	ACTIVE_ID_ERR,				//��Ʒ��Ŵ��� 
	ACTIVE_PASS_ERR,			//���󼤻�����
	ACTIVE_PASS_EXPIRE			//�������
	//ACTIVE_DATE_ERR,			//���ڴ���
}ACTIVE_STATUS_TypeDef; 
//�����Ʒ��now_date,Ϊ��ǰʱ�䣬passΪ���������ַ�����"234345"
ACTIVE_STATUS_TypeDef active_product(tTime now_date,const char *pass);  

typedef enum   
{
	EXPIRE_NONE=0,				//û������
	EXPIRE_DATE_ERR,			//�����ǰ����<�ϴ�ʹ������
	EXPIRE_NEAR,				//��ӽ�����
	EXPIRE_ACTIVE_DATE_ERR,		//�������ڴ���
	EXPIRE_ACTIVE_DAY_ERR,		//������������
	EXPIRE,						//ʱ���ѵ� 
}EXPIRE_STATUS_TypeDef;   
//�ж��Ƿ��Ʒ����,now_dateΪ��ǰ����,dayΪ���ػ�ʣ�����쵽��
EXPIRE_STATUS_TypeDef expire_judge(tTime now_date,int32_t *day);	 


#endif




	

	

	












