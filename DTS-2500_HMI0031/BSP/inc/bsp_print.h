/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: print.h
** ����޸�����: 2012-8-18
** ��        ��: 
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE: 
**********************************************************************************************************/
#ifndef __BSP_PRINT_H
#define	__BSP_PRINT_H

 
void bsp_InitPrint(void);					//��ʼ����ӡ��
void print(const char *dat);  				//��ӡ����
void print_chinese(void);					//���ù���һ�������ֿ⺺�ִ�ӡ��ʽ 
void print_direct(uint8_t dire);			//�������Ϊ0��ѡ�����ӡ,������������ӡ
void print_english(void);					//��ӡ�����Ӻ��ִ�ӡ��ʽ�л��������ַ���ӡ��ʽ
void print_enter(void); 					//��ӡ���������������ݶ�������ӡ������ǰ��һ�У�Ч����س����CR��һ����
void print_run_paper(uint8_t n);			//��ӡ����ǰ����n����
void print_set_invert(uint8_t en);			//����/��ֹ���״�ӡ  
void print_set_left(uint8_t n);				//��������
void print_set_right(uint8_t n);			//��������
void print_set_underline(uint8_t en);		//�����»��� 1���»��� 0û���»���
void print_set_vertical_space(uint8_t n);	//����n���м��
void print_space_hori(uint8_t n);			//��ӡn���ո�
void print_space_veri(uint8_t n);			//��ӡn������
void print_zoom_all(uint8_t n);				//��������Ŵ�  
void print_zoom_hori(uint8_t n);			//����Ŵ�
void print_zoom_veri(uint8_t n);			//����Ŵ�
 
 
#endif




	

	

	












