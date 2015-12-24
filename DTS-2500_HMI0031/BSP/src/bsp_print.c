/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: bsp_print.c
** ����޸�����: 2015-12-5 21:51:35  
** ��        ��: ����
** ��	     ��: V1.0
** �� ��  о Ƭ: STM32F407VET6		����Ƶ��: 25MHz
** IDE		  :	MDK 4.70 
**********************************************************************************************************/
#include "bsp.h"

/*------------------------------------------------------------
 * Function Name  : UartPrintChar
 * Description    : ��ӡһ���ַ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UartPrintChar( const char ch )
{
	ComSend(COM6,(uint8_t *)&ch,1);
}

/**********************************************************************
functionName:void print_enter(void)
description: ��ӡ���������������ݶ�������ӡ������ǰ��һ�У�Ч����س���
�CR��һ����
**********************************************************************/
void print_enter(void)
{
	UartPrintChar(0x0A);	 
}  
 
/**********************************************************************
functionName:void print_run_paper(uint8_t n)
description: ��ӡ����ǰ����n����
**********************************************************************/
void print_run_paper(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x4A);	//'J'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_vertical_space(uint8_t n)
description:����n���м��
**********************************************************************/
void print_set_vertical_space(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x31);	//'1'
	UartPrintChar(n);
}

 

/**********************************************************************
functionName:void print_space_hori(uint8_t n)
description:��ӡn���ո�
**********************************************************************/
void print_space_hori(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x66);	//'f'
	UartPrintChar(0);
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_space_veri(uint8_t n)
description:��ӡn������
**********************************************************************/
void print_space_veri(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x66);	//'f'
	UartPrintChar(1);
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_left(uint8_t n)
description:��������
**********************************************************************/
void print_set_left(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x6C);	//'l'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_right(uint8_t n)
description:��������
**********************************************************************/
void print_set_right(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x51);	//'Q'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_zoom_hori(uint8_t n)
description:����Ŵ�
**********************************************************************/
void print_zoom_hori(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x55);	//'U'
	UartPrintChar(n);
}


/**********************************************************************
functionName:void print_zoom_veri(uint8_t n)
description:����Ŵ� 
**********************************************************************/
void print_zoom_veri(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x56);	//'V'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_zoom_all(uint8_t n)
description:��������Ŵ�  
**********************************************************************/
void print_zoom_all(uint8_t n)
{
	if(!n)
	n=2;
	if(n>8)
	n=2;		
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x57);	//'W'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_underline(uint8_t en)
description:�����»��� 1���»��� 0û���»���
**********************************************************************/
void print_set_underline(uint8_t en)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x2D);	//'_'
	UartPrintChar(en);
}
 
/**********************************************************************
functionName:void print_set_invert(uint8_t en)
description:����/��ֹ���״�ӡ  
**********************************************************************/
void print_set_invert(uint8_t en)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x69);	//'i'
	UartPrintChar(en);
}
 
/**********************************************************************
functionName:void bsp_InitPrint(void)
description:��ʼ����ӡ��
**********************************************************************/
void bsp_InitPrint(void)
{
//	uart6_init(19200);
	UartPrintChar(0x1B);		//ESC
	UartPrintChar(0x40);		//'@'
	UartPrintChar(0x1B);		//ESC
	UartPrintChar(0x63);		//'c'
	UartPrintChar(1);			//�����ӡ��ʽ��ֹ		
}

/**********************************************************************
functionName:void print_use_chinese(void)
description:���ù���һ�������ֿ⺺�ִ�ӡ��ʽ 
**********************************************************************/
void print_chinese(void)
{
	UartPrintChar(0x1C);	//FS
	UartPrintChar(0x26);	//'&'
}

/**********************************************************************
functionName:void print_use_english(void)
description:��ӡ�����Ӻ��ִ�ӡ��ʽ�л��������ַ���ӡ��ʽ
**********************************************************************/
void print_english(void)
{
	UartPrintChar(0x1C);	//FS
	UartPrintChar(0x2E);	//'.'
}

/**********************************************************************
functionName:void print_direct(uint8_t dire)
description:�������Ϊ0��ѡ�����ӡ,������������ӡ
**********************************************************************/
void print_direct(uint8_t dire)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x63);	//'c'
	UartPrintChar(dire);	
}

/**********************************************************************
functionName:void print(char *dat)
description:��ӡ����
**********************************************************************/ 
void print(const char *dat)
{
	const char *d;
	
	d = dat;
	
	while(*d)
	{
		UartPrintChar((uint8_t)*d);
		d++;
	}
} 



 


  
/******************* (C) COPYRIGHT 2012 XinGao Tech 			 *****END OF FILE****/  
