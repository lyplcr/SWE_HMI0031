/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: print.h
** 最后修改日期: 2012-8-18
** 描        述: 
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE: 
**********************************************************************************************************/
#ifndef __BSP_PRINT_H
#define	__BSP_PRINT_H

 
void bsp_InitPrint(void);					//初始化打印机
void print(const char *dat);  				//打印文字
void print_chinese(void);					//设置国标一、二级字库汉字打印方式 
void print_direct(uint8_t dire);			//如果参数为0，选择反向打印,否则按照正常打印
void print_english(void);					//打印机将从汉字打印方式切换到西文字符打印方式
void print_enter(void); 					//打印缓冲区中所有数据都将被打印而且向前走一行，效果与回车命令（CR）一样。
void print_run_paper(uint8_t n);			//打印机向前进给n点行
void print_set_invert(uint8_t en);			//允许/禁止反白打印  
void print_set_left(uint8_t n);				//设置左限
void print_set_right(uint8_t n);			//设置右限
void print_set_underline(uint8_t en);		//设置下划线 1有下划线 0没有下划线
void print_set_vertical_space(uint8_t n);	//设置n点行间距
void print_space_hori(uint8_t n);			//打印n个空格
void print_space_veri(uint8_t n);			//打印n个空行
void print_zoom_all(uint8_t n);				//横向纵向放大  
void print_zoom_hori(uint8_t n);			//横向放大
void print_zoom_veri(uint8_t n);			//纵向放大
 
 
#endif




	

	

	












