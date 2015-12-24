/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: bsp_print.c
** 最后修改日期: 2015-12-5 21:51:35  
** 描        述: 串口
** 版	     本: V1.0
** 主 控  芯 片: STM32F407VET6		晶振频率: 25MHz
** IDE		  :	MDK 4.70 
**********************************************************************************************************/
#include "bsp.h"

/*------------------------------------------------------------
 * Function Name  : UartPrintChar
 * Description    : 打印一个字符
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
description: 打印缓冲区中所有数据都将被打印而且向前走一行，效果与回车命
令（CR）一样。
**********************************************************************/
void print_enter(void)
{
	UartPrintChar(0x0A);	 
}  
 
/**********************************************************************
functionName:void print_run_paper(uint8_t n)
description: 打印机向前进给n点行
**********************************************************************/
void print_run_paper(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x4A);	//'J'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_vertical_space(uint8_t n)
description:设置n点行间距
**********************************************************************/
void print_set_vertical_space(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x31);	//'1'
	UartPrintChar(n);
}

 

/**********************************************************************
functionName:void print_space_hori(uint8_t n)
description:打印n个空格
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
description:打印n个空行
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
description:设置左限
**********************************************************************/
void print_set_left(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x6C);	//'l'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_set_right(uint8_t n)
description:设置右限
**********************************************************************/
void print_set_right(uint8_t n)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x51);	//'Q'
	UartPrintChar(n);
}

/**********************************************************************
functionName:void print_zoom_hori(uint8_t n)
description:横向放大
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
description:纵向放大 
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
description:横向纵向放大  
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
description:设置下划线 1有下划线 0没有下划线
**********************************************************************/
void print_set_underline(uint8_t en)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x2D);	//'_'
	UartPrintChar(en);
}
 
/**********************************************************************
functionName:void print_set_invert(uint8_t en)
description:允许/禁止反白打印  
**********************************************************************/
void print_set_invert(uint8_t en)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x69);	//'i'
	UartPrintChar(en);
}
 
/**********************************************************************
functionName:void bsp_InitPrint(void)
description:初始化打印机
**********************************************************************/
void bsp_InitPrint(void)
{
//	uart6_init(19200);
	UartPrintChar(0x1B);		//ESC
	UartPrintChar(0x40);		//'@'
	UartPrintChar(0x1B);		//ESC
	UartPrintChar(0x63);		//'c'
	UartPrintChar(1);			//反向打印方式禁止		
}

/**********************************************************************
functionName:void print_use_chinese(void)
description:设置国标一、二级字库汉字打印方式 
**********************************************************************/
void print_chinese(void)
{
	UartPrintChar(0x1C);	//FS
	UartPrintChar(0x26);	//'&'
}

/**********************************************************************
functionName:void print_use_english(void)
description:打印机将从汉字打印方式切换到西文字符打印方式
**********************************************************************/
void print_english(void)
{
	UartPrintChar(0x1C);	//FS
	UartPrintChar(0x2E);	//'.'
}

/**********************************************************************
functionName:void print_direct(uint8_t dire)
description:如果参数为0，选择反向打印,否则按照正常打印
**********************************************************************/
void print_direct(uint8_t dire)
{
	UartPrintChar(0x1B);	//ESC
	UartPrintChar(0x63);	//'c'
	UartPrintChar(dire);	
}

/**********************************************************************
functionName:void print(char *dat)
description:打印文字
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
