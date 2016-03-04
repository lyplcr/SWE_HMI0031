/**
  ******************************************************************************
  * @file    bsp_lcd.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   LCD�ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
  
#include "bsp.h"
#include "font_asc.h"


#define KByte(x)				(x * (1 << 10))

/* �ֿ� */		
#define FONT_BASE_ADDR			( KByte(100) )
#define FONT_PASSWORD_ADDR		( FONT_BASE_ADDR - KByte(4) )

/* ����24��-���� */
#define SONG24_FONT_HZ_ADDR		(FONT_BASE_ADDR + 0)
#define SD_FONT_HZ_PATH			"0:hz/"

/* ����24��-Ӣ�ģ���������24�����ĺ��棬4K�ֽڶ��룩 */
#define SONG24_FONT_ENG_ADDR	(FONT_BASE_ADDR + KByte(592))
#define SD_FONT_ENG_PATH		"0:asc/"


#define		LCD_WR_Data(x)		*(__IO uint16_t *) (LCD_DATA_ADD)=(x)
#define		Bank1_LCD_D			LCD_DATA_ADD   


#define	FONT_ENGLISH 			0
#define	FONT_CHINESE			1

static FATFS fs;            // Work area (file system object) for logical drive 

/**********************************************************************
functionName:void lcd_hard_reset(void)
description: LCDӲ����λ
**********************************************************************/  
void lcd_hard_reset(void)
{
	GPIO_ResetBits  (GPIOB,GPIO_Pin_0); 		//��λRA8875
	bsp_DelayMS(10);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);			//RESET=1 
	bsp_DelayMS(10);
}
 

/**********************************************************************
functionName:void ssd1963_gpio_init(void)
description: LCD���ų�ʼ��
**********************************************************************/  
void lcd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��FSMCʱ�� */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	/* ʹ�� GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);

	/* ���� PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	 PD.10(D15), PD.14(D0), PD.15(D1) Ϊ����������� */

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);  
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ���� PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) Ϊ����������� */

	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FSMC);
	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FSMC);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* ���� PD.11(A16 (RS))  Ϊ����������� */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ���� PD7 (LCD/CS)) Ϊ����������� */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	//RST����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//WAIT����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

 
/**********************************************************************
functionName:void FSMC_LCD_Init(void)
description:FSMC��ʼ��
**********************************************************************/  
void FSMC_LCD_Init(void)
{
	FSMC_NORSRAMInitTypeDef  init;
	FSMC_NORSRAMTimingInitTypeDef  timingWrite;
	FSMC_NORSRAMTimingInitTypeDef  timingRead;

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 4 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	/* ����ͷDMA��㣬������ 4 0 5 2 0 0 */
	timingWrite.FSMC_AddressSetupTime = 1;		//4
	timingWrite.FSMC_AddressHoldTime = 0;
	timingWrite.FSMC_DataSetupTime = 1;		//6
	timingWrite.FSMC_BusTurnAroundDuration = 1;
	timingWrite.FSMC_CLKDivision = 0;
	timingWrite.FSMC_DataLatency = 0;
	timingWrite.FSMC_AccessMode = FSMC_AccessMode_A;

	timingRead.FSMC_AddressSetupTime = 2;		//4
	timingRead.FSMC_AddressHoldTime = 0;
	timingRead.FSMC_DataSetupTime = 4;		//8
	timingRead.FSMC_BusTurnAroundDuration = 1;
	timingRead.FSMC_CLKDivision = 0;
	timingRead.FSMC_DataLatency = 0;
	timingRead.FSMC_AccessMode = FSMC_AccessMode_A;


	/*
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable
	*/
	init.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	/* ע��ɿ��������Ա */
	init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

	init.FSMC_ReadWriteTimingStruct = &timingRead;
	init.FSMC_WriteTimingStruct = &timingWrite;

	FSMC_NORSRAMInit(&init);

	/* - BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

 
/**********************************************************************
functionName:void bsp_InitLCD(void)
description:lcd��ʼ��
**********************************************************************/  
void bsp_InitLCD(void)
{
	lcd_gpio_init();
	lcd_hard_reset(); 
	FSMC_LCD_Init(); 
	Init_RA8875();	   //RA8875��ʼ��
}
   
/**********************************************************************
functionName:void lcd_clear(uint16_t color)
description: LCD��������
**********************************************************************/ 
void lcd_clear(uint16_t color)
{ 
	Active_Window(0,799,0,479);
	Geometric_Coordinate(0,799,0,479); 		//�趨��������
	Text_Foreground_Color(color);			//������ɫ
	Draw_square_fill();
	Chk_Busy(); 
}    
    
/**********************************************************************
functionName:void lcd_fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
description: ��ָ�����������ָ����ɫ
**********************************************************************/  
void lcd_fill(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color)
{ 
	Active_Window(0,799,0,479);
	Geometric_Coordinate(sx,sx+w-1,sy,sy+h-1);
	Text_Foreground_Color(color);  
    Draw_square_fill();
    Chk_Busy();	   
} 

 

/**********************************************************************
functionName:void lcd_set_backlight(uint8_t duty)
description: ����LCD�ı��� ����ֵ0-0xA0
**********************************************************************/  
void lcd_set_backlight(uint8_t duty)
{
// 	PWM1_enable();
//	PWM1_fnuction_sel();
//	PWM1_clock_ratio(0x01);		//PWM��Ƶ��=SYS_CLK/256/��Ƶ��
	PWM1_duty_cycle(duty); 
}

   
 
/**********************************************************************
functionName:void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color)
description: ����
ֻҪ�趨�ڴ�д��������Ϳ����ˣ����в��ԡ�
**********************************************************************/  
void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color)
{ 
	//Active_Window(x,x+1,y,y+1);
	XY_Coordinate(x,y);
	WriteCommand(0x02); 
	WriteData(color); 
}  
 
 
 
/**********************************************************************
functionName:void lcd_draw_line(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2)
description: ����
**********************************************************************/  
//void lcd_draw_line(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
//{
//	uint16_t x, y, t;
//	int32_t temp;  
//	Active_Window(0,799,0,479);
//	if(x1==x2)
//	{ 
//		if(y1>y2) 
//		{
//			t=y1;
//			y1=y2;
//			y2=t;  
//		} 
//		lcd_fill(x1,y1,(x2-x1+1),(y2-y1+1),color);
//	}	 		
//	else 
//	{
//		if(x1>x2)		//�������㣬ʹ��x1ʼ��<x2
//		{
//			t=y1;
//			y1=y2;
//			y2=t;
//			t=x1;
//			x1=x2;
//			x2=t; 
//		} 
//		for(x=x1;x<=x2;x++)			//��x��Ϊ��׼ 
//		{
//			temp=(int32_t)y2-(int32_t)y1;
//			temp=temp*(x-x1);
//			temp/=((int32_t)x2-(int32_t)x1);
//			y=y1+temp; 
//			lcd_draw_point(x,y,color); 
//		}
//	}	 
//} 

void lcd_draw_line( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )
{
	uint16_t i; 
	int16_t xerr=0,yerr=0,delta_x,delta_y,distance; 
	int16_t incx,incy,uRow,uCol; 

	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(delta_x > 0)
	{
		incx = 1; //���õ������� 
	}
	else if(delta_x == 0)
	{
		incx = 0;//��ֱ�� 
	}
	else 
	{
		incx = -1;
		delta_x = -delta_x;
	}
	
	if(delta_y > 0)
	{
		incy = 1; 
	}
	else if(delta_y == 0)
	{
		incy = 0;//ˮƽ�� 
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x > delta_y)
	{	
		distance = delta_x; //ѡȡ�������������� 
	}
	else
	{
		distance = delta_y; 
	}
	
	for(i=0; i<=distance+1; ++i)	//������� 
	{  
		lcd_draw_point(uRow,uCol,color); 
		xerr += delta_x ; 
		yerr += delta_y ; 
		
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}
}
  

/**********************************************************************
functionName:void lcd_draw_dashed(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2,uint16_t space,uint16_t color)
description: ������
x,x+fill-1,x+fill+space-1,x+fill+space,s+length-1
**********************************************************************/  
void lcd_draw_dashed(tDashedObject dashed_line)
{    
	uint16_t sx,ex,exl; 
	uint16_t sy,ey,eyl; 
	sx=dashed_line.x;
	ex=sx+dashed_line.fill-1; 
	exl=dashed_line.x+dashed_line.length-1;  
	sy=dashed_line.y;
	ey=sy+dashed_line.fill-1; 
	eyl=dashed_line.y+dashed_line.length-1; 
	if(!(dashed_line.direct))		//����Ǻ���
	{ 
		 while(ex<=exl)
		 {
		 	lcd_fill(sx,dashed_line.y,dashed_line.fill,dashed_line.width,dashed_line.color); 
		 	sx+=dashed_line.space+dashed_line.fill;
		 	ex=sx+dashed_line.fill-1; 
		 }
		 if(sx<=exl)	 	
		 {
		 	dashed_line.fill=exl-sx+1;
		 	lcd_fill(sx,dashed_line.y,dashed_line.fill,dashed_line.width,dashed_line.color); 
		 }	 
	}	
	else							//���������
	{ 
		while(ey<=eyl)
		 {
		 	lcd_fill(dashed_line.x,sy,dashed_line.width,dashed_line.fill,dashed_line.color); 
		 	sy+=dashed_line.space+dashed_line.fill;
		 	ey=sy+dashed_line.fill-1; 
		 }
		 if(sy<=eyl)	 	
		 {
		 	dashed_line.fill=eyl-sy+1;
		 	lcd_fill(dashed_line.x,sy,dashed_line.width,dashed_line.fill,dashed_line.color); 
		 }	
	}	
}
  

/**********************************************************************
functionName:void lcd_draw_dashed_frame(tDashedFrameObject dashed_frame)
description: �����߿�
**********************************************************************/ 
void lcd_draw_dashed_frame(tDashedFrameObject dashed_frame)
{
	uint8_t i;
	tDashedObject dashed_line; 
	dashed_line.x=dashed_frame.x;
	dashed_line.y=dashed_frame.y;
	dashed_line.width=dashed_frame.width;
	dashed_line.fill=dashed_frame.fill;
	dashed_line.space=dashed_frame.space;
	
	dashed_line.length=dashed_frame.veri_space*dashed_frame.veri_num;  
	dashed_line.direct=0;
	dashed_line.color=dashed_frame.color_row;
	
	lcd_draw_dashed(dashed_line);
	for(i=0;i<dashed_frame.hori_num;i++)
	{
		dashed_line.y+=dashed_frame.hori_space;
		lcd_draw_dashed(dashed_line);
	} 
	dashed_line.y=dashed_frame.y;  
	dashed_line.length=dashed_frame.hori_space*dashed_frame.hori_num; 
	dashed_line.direct=1; 
	dashed_line.color=dashed_frame.color_col;
	lcd_draw_dashed(dashed_line);
	for(i=0;i<dashed_frame.veri_num;i++)
	{
		dashed_line.x+=dashed_frame.veri_space;
		lcd_draw_dashed(dashed_line);
	} 
}  
  
  
/**********************************************************************
functionName:void lcd_draw_circle(uint16_t  x0,uint16_t  y0,uint16_t  r,uint16_t  color)
description: ��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
**********************************************************************/  
void lcd_draw_circle(uint16_t  x0,uint16_t y0,uint16_t r,uint16_t color)
{
	int32_t a,b;
	int32_t di;
	Active_Window(0,799,0,479);
	a=0;
	b=r;
	di=3-2*r;             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		lcd_draw_point(x0-b,y0-a,color);             //3           
		lcd_draw_point(x0+b,y0-a,color);             //0           
		lcd_draw_point(x0-a,y0+b,color);             //1       
		lcd_draw_point(x0-b,y0-a,color);             //7           
		lcd_draw_point(x0-a,y0-b,color);             //2             
		lcd_draw_point(x0+b,y0+a,color);             //4               
		lcd_draw_point(x0+a,y0-b,color);             //5
		lcd_draw_point(x0+a,y0+b,color);             //6 
		lcd_draw_point(x0-b,y0+a,color);             
		a++;
		/***ʹ��Bresenham�㷨��Բ**/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd_draw_point(x0+a,y0+b,color);
	}
}

/**********************************************************************
functionName:void lcd_draw_rect(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color)
description: �����ο�
**********************************************************************/  
void lcd_draw_rect(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color)
{
	lcd_draw_line(sx,sy,sx+w-1,sy,color); 
	lcd_draw_line(sx,sy,sx,sy+h-1,color);
	lcd_draw_line(sx+w-1,sy,sx+w-1,sy+h-1,color);
	lcd_draw_line(sx,sy+h-1,sx+w-1,sy+h-1,color);			
}


#define		FILE_BUFF_SIZE	2000  
static uint16_t file_buff[FILE_BUFF_SIZE];



FRESULT fresult; 
FIL file_obj; 
FIL english_obj;
FIL chinese_obj;

typedef struct
{
   uint8_t scan;
   uint8_t gray;
   uint16_t w;
   uint16_t h;
   uint8_t is565;
   uint8_t rgb;
}IMAGE_HEAD_TypeDef; 

//#define LCD_COMM_ADD	    ((uint32_t)0x60020000)		 
//#define LCD_DATA_ADD		((uint32_t)0x60000000)	


/**********************************************************************
functionName:FRESULT lcd_show_image(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,char *path)
description: ��ʾͼƬ
�������XY ��� �߶� �ļ�·��
**********************************************************************/   
FRESULT lcd_show_image(uint16_t sx,uint16_t sy,char *path)
{
	uint32_t n;    
	uint16_t *pix; 
	uint16_t *pix_end;
	uint32_t br;  
	IMAGE_HEAD_TypeDef image_head; 	   
	 
	fresult=f_open(&file_obj,path,FA_READ);
	if(fresult!=FR_OK)
	return fresult; 
	f_read(&file_obj,&image_head,sizeof(IMAGE_HEAD_TypeDef),&br); 
	 
	Active_Window(sx,sx+image_head.w-1,sy,sy+image_head.h-1);
	XY_Coordinate(sx,sy);  
	WriteCommand(0x02);		//д���ڴ�
	 
	n=image_head.w;
	n*=image_head.h;
	n<<=1;
	n+=sizeof(IMAGE_HEAD_TypeDef); 
	if(n!=(file_obj.fsize))
	return FR_INVALID_OBJECT; 	
	do{
		fresult = f_read(&file_obj,file_buff,sizeof(file_buff),&br); 
		pix_end=&file_buff[br>>1];
		for(pix=file_buff;pix<pix_end;pix++) 
		{
			*(__IO uint16_t *)(Bank1_LCD_D)=*pix; 
		}   
	}while(br==(sizeof(file_buff)));
	fresult=f_close(&file_obj); 
	return FR_OK;
} 

 

///**********************************************************************
//functionName:void lcd_color_change(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t orig_color,uint16_t new_color)
//description: �������һ����ɫ��������һ����ɫ
//**********************************************************************/     
//void lcd_color_change(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t orig_color,uint16_t new_color) 
//{
//	uint16_t ex,j;
//	ex=w+sx-1; 
//	while(h--)
//	{ 
//		Active_Window(sx,ex,sy,sy);
//		Memory_read_Coordinate(sx,sy);
//		//XY_Coordinate(sx,sy);  
//		WriteCommand(0x02);		//д���ڴ�
//		for(j=0;j<w;j++)
//		{
//			file_buff[j]=*(__IO uint16_t *)(Bank1_LCD_D);
//			//file_buff[j]=ReadData();
//			if(file_buff[j]==orig_color)
//			file_buff[j]=new_color;	
//		} 
//		Active_Window(sx,ex,sy,sy);
//		XY_Coordinate(sx,sy);   
//		WriteCommand(0x02);		//д���ڴ�
//		for(j=0;j<w;j++)
//		{
//			*(__IO uint16_t *) (Bank1_LCD_D)=file_buff[j];
//		} 
//		sy++; 
//	} 
//} 

    
/**********************************************************************
functionName:void lcd_asc16_A(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c,uint8_t *buff)
description: ��ʾ16X8�����ͼ��
**********************************************************************/ 
void lcd_draw16x8(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;    
	 
	Active_Window(x,x+7,y,y+15);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		} 
	}   
}
 
/**********************************************************************
functionName:void lcd_draw16x16(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
description: ��ʾ16X16�����ͼ��
**********************************************************************/ 
void lcd_draw16x16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;     
	Active_Window(x,x+15,y,y+15);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<32;i+=2)
	{ 
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j)) 
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else 
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+1]&(0x80>>j)) 
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else 
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		} 
	}
} 
 
 
/**********************************************************************
functionName:FRESULT lcd_font16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font)
description: ��ʾ16�������Ӣ�Ļ����
**********************************************************************/
FRESULT lcd_font16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font)
{
	uint8_t qh=0,wh=0;
	uint32_t offset; 
	char path[20];
	uint32_t br; 
	uint8_t font_buff[34];
	const char *sc = s;
	uint8_t open[2] = {0};	
	fresult = FR_OK;
			
	while(*sc) 
	{
		if(*sc<=127)
		{
			if ( !open[FONT_ENGLISH] )
			{					
				usprintf(path,"asc/%s",font);
				fresult=f_open(&english_obj,path,FA_READ);
				if(fresult!=FR_OK)
				break; 
				
				open[FONT_ENGLISH] = 1;
			}
				
			offset=(uint16_t)(*sc)<<4;
			f_lseek(&english_obj,offset); 
			f_read(&english_obj,font_buff,16,&br); 
			lcd_draw16x8(x,y,fc,bc,font_buff);
			sc++;
			x+=8;
		}	
		else
		{
			if ( !open[FONT_CHINESE] )
			{								
				usprintf(path,"hz/%s",font);
				fresult=f_open(&chinese_obj,path,FA_READ);
				if(fresult!=FR_OK)
				break;
				
				open[FONT_CHINESE] = 1;
			}
			
			qh=sc[0]-0xa0;
			wh=sc[1]-0xa0;
			offset=(94*(qh-1)+wh-1)<<5;		//�õ�ƫ�Ƶ�ַ
			f_lseek(&chinese_obj,offset); 
			f_read(&chinese_obj,font_buff,32,&br);
			lcd_draw16x16(x,y,fc,bc,font_buff);
			sc+=2;
			x+=16; 
		}	
	}
	
	if ( open[FONT_ENGLISH] )
	{
		f_close(&english_obj);
	}
	if ( open[FONT_CHINESE] )
	{
		f_close(&chinese_obj);
	}
	
	return fresult;
}


/**********************************************************************
functionName:void lcd_draw24x12(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c,uint8_t *buff)
description: ��ʾ24x12�����ͼ��
**********************************************************************/ 
void lcd_draw24x12(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;    
	Active_Window(x,x+11,y,y+23);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<48;i+=2)
	{
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}
		for(j=0;j<4;j++)
		{
			if(buff[i+1]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;	
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}
	}  
}



/**********************************************************************
functionName:void lcd_draw24x24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
description: ��ʾ24x12�����ͼ��
**********************************************************************/ 
void lcd_draw24x24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;    
	Active_Window(x,x+23,y,y+23);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	

	
	for(i=0;i<72;i+=3)
	{ 
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+1]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+2]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
	}  
}

#if 0
	/**********************************************************************
	functionName:FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font)
	description: ��ʾ24�������Ӣ�Ļ����
	**********************************************************************/
	FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font)
	{
		uint8_t qh=0,wh=0;
		uint32_t offset; 
		char path[20];
		uint32_t br; 
		uint8_t font_buff[73];
		const char *sc = s;
		uint8_t open[2] = {0};	
		fresult = FR_OK;
		
		while(*sc) 
		{
			if(*sc<=127)
			{
				if ( !open[FONT_ENGLISH] )
				{					
					usprintf(path,"asc/%s",font);
					fresult=f_open(&english_obj,path,FA_READ);
					if(fresult!=FR_OK)
					break; 
					
					open[FONT_ENGLISH] = 1;
				}
					
				offset=(uint16_t)(*sc)*48;
				f_lseek(&english_obj,offset); 
				f_read(&english_obj,font_buff,48,&br); 
				lcd_draw24x12(x,y,fc,bc,font_buff);
				sc++;
				x+=12;
			}	
			else
			{
				if ( !open[FONT_CHINESE] )
				{								
					usprintf(path,"hz/%s",font);
					fresult=f_open(&chinese_obj,path,FA_READ);
					if(fresult!=FR_OK)
					break;
					
					open[FONT_CHINESE] = 1;
				}
				
				qh=sc[0]-0xa0;
				wh=sc[1]-0xa0;
				offset=(94*(qh-1)+wh-1)*72;		//�õ�ƫ�Ƶ�ַ
				f_lseek(&chinese_obj,offset); 
				f_read(&chinese_obj,font_buff,72,&br);
				lcd_draw24x24(x,y,fc,bc,font_buff);
				sc+=2;
				x+=24; 
			}	
		}
		
		if ( open[FONT_ENGLISH] )
		{
			f_close(&english_obj);
		}
		if ( open[FONT_CHINESE] )
		{
			f_close(&chinese_obj);
		}
		
		return fresult;
	}
#endif
	
/**********************************************************************
functionName:FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font)
description: ��ʾ24�������Ӣ�Ļ����
**********************************************************************/
FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font)
{
	uint8_t qh=0,wh=0;
	uint32_t offset; 
	uint8_t font_buff[73];
	const char *sc = s;
	fresult = FR_OK;
	
	while(*sc) 
	{
		if(*sc<=127)
		{				
			offset=(uint16_t)(*sc)*48;
			
			sf_ReadBuffer(font_buff,SONG24_FONT_ENG_ADDR+offset,48); 
			
			lcd_draw24x12(x,y,fc,bc,font_buff);
			sc++;
			x+=12;
		}	
		else
		{			
			qh=sc[0]-0xa0;
			wh=sc[1]-0xa0;
			offset=(94*(qh-1)+wh-1)*72;		//�õ�ƫ�Ƶ�ַ
			
			sf_ReadBuffer(font_buff,SONG24_FONT_HZ_ADDR+offset,72);
			lcd_draw24x24(x,y,fc,bc,font_buff);
			sc+=2;
			x+=24; 
		}	
	}
	
	return fresult;
}	

/**********************************************************************
functionName:void lcd_draw32x16(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c,uint8_t *buff)
description: ��ʾ32x16�����ͼ��
**********************************************************************/ 
void lcd_draw32x16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;   
	Active_Window(x,x+15,y,y+31);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<64;i+=2)
	{
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			LCD_WR_Data(fc);
			//*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			//*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
			LCD_WR_Data(bc);
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+1]&(0x80>>j))
			LCD_WR_Data(fc);
			//*(__IO uint16_t *) (Bank1_LCD_D)=fc;	
			else
			LCD_WR_Data(bc);	
			//*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}  
	}    
}


/**********************************************************************
functionName:void lcd_draw32x32(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c,uint8_t *buff)
description: ��ʾ32x32�����ͼ��
**********************************************************************/ 
void lcd_draw32x32(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t *buff)
{
	uint8_t i,j;   
	  
	Active_Window(x,x+31,y,y+31);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
		  
	for(i=0;i<128;i+=4)
	{ 
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+1]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+2]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+3]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;
		}
	}    
}



/**********************************************************************
functionName:void lcd_ascin20(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
description: ��ʾ20X10�����ASCII�ַ�
**********************************************************************/ 
void lcd_ascin20(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
{
	unsigned char i,j;
	if(c>32)
	c-=32; 
	else 
	c=0;
	
	Active_Window(x,x+9,y,y+19);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<40;i+=2)
	{
		for(j=0;j<8;j++)
		{
			if(asc20[c][i]&(0x80>>j))
			LCD_WR_Data(fc);
			else
			LCD_WR_Data(bc);		
		}
		for(j=0;j<2;j++)
		{
			if(asc20[c][i+1]&(0x80>>j))
			LCD_WR_Data(fc);
			else
			LCD_WR_Data(bc);		
		} 
	} 
} 
 
 
/**********************************************************************
functionName:void lcd_ascin24s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
description: ��ʾ24X12�����ASCII�ַ���
**********************************************************************/ 
void lcd_ascin20s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
{
	while(*s) 
	{
		lcd_ascin20(x,y,fc,bc,*s);
		s++;
		x+=10;
	}
}



/**********************************************************************
functionName:void lcd_draw20x20(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c,uint8_t *buff)
description: ��ʾ20x20�����ͼ��
**********************************************************************/ 
void lcd_draw20x20(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, const char *buff)
{
	uint8_t i,j;    
	Active_Window(x,x+19,y,y+19);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<60;i+=3)
	{
		for(j=0;j<8;j++)
		{
			if(buff[i]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}
		for(j=0;j<8;j++)
		{
			if(buff[i+1]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}
		for(j=0;j<4;j++)
		{
			if(buff[i+2]&(0x80>>j))
			*(__IO uint16_t *) (Bank1_LCD_D)=fc;	
			else
			*(__IO uint16_t *) (Bank1_LCD_D)=bc;		
		}
	}  
}


/**********************************************************************
functionName:FRESULT lcd_font20(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font)
description: ��ʾ20�������Ӣ�Ļ����
**********************************************************************/
FRESULT lcd_font20(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font)
{
	uint8_t qh=0,wh=0;
	uint32_t offset; 
	uint32_t br; 
	char path[20];
	char font_buff[130];
	const char *sc = s;
	uint8_t open = 0;	
	fresult = FR_OK;
	
	while(*sc) 
	{
		if(*sc < 0x7f)
		{			
			lcd_ascin20(x,y,fc,bc,*sc);
			x += 10;
			sc += 1;
		}	
		else
		{				
			if ( !open )
			{								
				usprintf(path,"hz/%s",font);
				fresult=f_open(&file_obj,path,FA_READ);
				if(fresult!=FR_OK)
				break;
				
				open = 1;
			}
			
			qh=sc[0];
			wh=sc[1];
			if(wh<0x7f)wh-=0x40;
			else wh-=0x41;
			qh-=0x81; 
			offset=((unsigned long)190*qh+wh)*60;//�õ��ֿ��е��ֽ�ƫ����	
			f_lseek(&file_obj,offset); 
			f_read(&file_obj,font_buff,60,&br);		//60(�ֽ�) = 24(������) * 20(������) / 8
			lcd_draw20x20(x,y,fc,bc,font_buff);
			sc+=2;
			x+=20; 
		}	
	}
	
	if ( open )
	{
		f_close(&file_obj);
	}

	return fresult;
} 

/**********************************************************************
functionName:FRESULT lcd_font32(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,char *s,char *font)
description: ��ʾ32�������Ӣ�Ļ����
**********************************************************************/
FRESULT lcd_font32(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font)
{
	uint8_t qh=0,wh=0;
	uint32_t offset; 
	char path[20];
	uint32_t br; 
	uint8_t font_buff[130];
	const char *sc = s;
	uint8_t open[2] = {0};	
	fresult = FR_OK;
	
	while(*sc) 
	{
		if(*sc<=127)
		{
			if ( !open[FONT_ENGLISH] )
			{					
				usprintf(path,"asc/%s",font);
				fresult=f_open(&english_obj,path,FA_READ);
				if(fresult!=FR_OK)
				break; 
				
				open[FONT_ENGLISH] = 1;
			}
			
			offset=(uint16_t)(*sc)*64;
			f_lseek(&english_obj,offset); 
			f_read(&english_obj,font_buff,64,&br); 
			lcd_draw32x16(x,y,fc,bc,font_buff);
			sc++;
			x+=16;
		}	
		else
		{
			if ( !open[FONT_CHINESE] )
			{								
				usprintf(path,"hz/%s",font);
				fresult=f_open(&chinese_obj,path,FA_READ);
				if(fresult!=FR_OK)
				break;
				
				open[FONT_CHINESE] = 1;
			}
				
			qh=sc[0]-0xa0;
			wh=sc[1]-0xa0;
			offset=(94*(qh-1)+wh-1)*128;		//�õ�ƫ�Ƶ�ַ
			f_lseek(&chinese_obj,offset); 
			f_read(&chinese_obj,font_buff,128,&br);
			lcd_draw32x32(x,y,fc,bc,font_buff);
			sc+=2;
			x+=32; 
		}	
	}
	
	if ( open[FONT_ENGLISH] )
	{
		f_close(&english_obj);
	}
	if ( open[FONT_CHINESE] )
	{
		f_close(&chinese_obj);
	}
	
	return fresult;
} 

 
 
/**********************************************************************
functionName:void LCD_HL_DrawHLine(uint16_t x0,uint16_t y,uint16_t x1,uint16_t color)
description:�����x0 y �յ���x1 y�ĺ���
**********************************************************************/  
void LCD_HL_DrawHLine(uint16_t x0,uint16_t y,uint16_t x1,uint16_t color)
{ 
	Geometric_Coordinate(x0,x1,y,y); 
	Text_Foreground_Color(color);     
	Draw_square_fill();
	Chk_Busy();	 
}
 

/**********************************************************************
functionName:void lcd_fill_circle(int32_t x0, int32_t y0, int32_t r,uint16_t color) 
description:���Բ��������x0 y0 �뾶��r��Բ
**********************************************************************/  
void lcd_fill_circle(uint16_t x0, uint16_t y0, uint16_t r,uint16_t color) 
{
	int32_t i;
	int32_t imax = ((int32_t)((int32_t)r*707))/1000+1;
	int32_t sqmax = (int32_t)r*(int32_t)r+(int32_t)r/2;
	int32_t x=r;
	Active_Window(0,799,0,479);
	LCD_HL_DrawHLine(x0-r,y0,x0+r,color);
	for (i=1; i<= imax; i++) {
	if ((i*i+x*x) >sqmax) {
	  /* draw lines from outside */
	  if (x>imax) {
	    LCD_HL_DrawHLine (x0-i+1,y0+x, x0+i-1,color);
	    LCD_HL_DrawHLine (x0-i+1,y0-x, x0+i-1,color);
	  }
	  x--;
	}
	/* draw lines from inside (center) */
	LCD_HL_DrawHLine(x0-x,y0+i, x0+x,color);
	LCD_HL_DrawHLine(x0-x,y0-i, x0+x,color);
	}
}

/**********************************************************************
functionName:void lcd_fill_ellipse(uint16_t x0, uint16_t y0, uint16_t rx, uint16_t ry,uint16_t color)
description:���Բ��������x0 y0 �뾶��rx ry����Բ
**********************************************************************/  
void lcd_fill_ellipse(uint16_t x0, uint16_t y0, uint16_t rx, uint16_t ry,uint16_t color)
{
	int32_t OutConst, Sum, SumY;
	int32_t x,y;
	uint32_t _rx = rx;
	uint32_t _ry = ry;
	Active_Window(0,799,0,479);
	OutConst = _rx*_rx*_ry*_ry  /* Constant as explaint above */
	        +(_rx*_rx*_ry>>1); /* To compensate for rounding */
	x = rx;
	for (y=0; y<=ry; y++) {
	SumY =((int32_t)(rx*rx))*((int32_t)(y*y)); /* Does not change in loop */
	while (Sum = SumY + ((int32_t)(ry*ry))*((int32_t)(x*x)),
	       (x>0) && (Sum>OutConst))
	{
	  x--;
	}
	LCD_HL_DrawHLine(x0-x, y0+y, x0+x,color);
	if (y)
	LCD_HL_DrawHLine(x0-x, y0-y, x0+x,color);
	}
}  


/**********************************************************************
functionName:void lcd_ascin16(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
description: ��ʾ16X8�����ASCII�ַ�
**********************************************************************/ 
void lcd_ascin16(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
{
	uint8_t i,j;
	if(c>32)
	c-=32; 
	else 
	c=0; 
	Active_Window(x,x+7,y,y+15);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if(asc16[c][i]&(0x80>>j))
			*(__IO uint16_t *) (LCD_DATA_ADD)=fc; 
			else
			*(__IO uint16_t *) (LCD_DATA_ADD)=bc; 		
		} 
	}  
}




/**********************************************************************
functionName:void lcd_ascin16s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
description: ��ʾ16X8�����ASCII�ַ���
**********************************************************************/ 
void lcd_ascin16s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
{
	while(*s) 
	{
		lcd_ascin16(x,y,fc,bc,*s);
		s++;
		x+=8;
	}
} 

/**********************************************************************
functionName:void lcd_ascin24(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
description: ��ʾ24X12�����ASCII�ַ�
**********************************************************************/ 
void lcd_ascin24(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char c)
{
	unsigned char i,j;
	if(c>32)
	c-=32; 
	else 
	c=0;
	
	Active_Window(x,x+11,y,y+23);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	for(i=0;i<48;i+=2)
	{
		for(j=0;j<8;j++)
		{
			if(asc24[c][i]&(0x80>>j))
			LCD_WR_Data(fc);
			else
			LCD_WR_Data(bc);		
		}
		for(j=0;j<4;j++)
		{
			if(asc24[c][i+1]&(0x80>>j))
			LCD_WR_Data(fc);
			else
			LCD_WR_Data(bc);		
		} 
	} 
} 
 
 
/**********************************************************************
functionName:void lcd_ascin24s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
description: ��ʾ24X12�����ASCII�ַ���
**********************************************************************/ 
void lcd_ascin24s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s)
{
	while(*s) 
	{
		lcd_ascin24(x,y,fc,bc,*s);
		s++;
		x+=12;
	}
}


/**********************************************************************
functionName:void lcd_mem_err(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc)
description: ��ʾ���ڴ濨�������֣���С�̶�Ϊ32X32
**********************************************************************/
void lcd_mem_err(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc)
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		lcd_draw32x32(x,y,fc,bc,(uint8_t *)mem_err_info[i]);
		x+=32;
	}
}

/**********************************************************************
functionName:void lcd_num48_char(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char c) 
description: ��ʾ48X24��������ֺ�kN
**********************************************************************/ 
void lcd_num48_char(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char c) 
{
	unsigned char i,j;
	if(c=='k')
	c=10;
	else if(c=='N')
	c=11;
	else if(c=='.')	
	c=12;  
	else if(c==' ')	
	c=13; 
	else if(c=='-')	
	c=14; 
	if((c>=0x30)&&(c<=0x39))		//ASCII �����ֶ�֧��
	c-=0x30;	  
	

	Active_Window(x,x+23,y,y+47);
	XY_Coordinate(x,y);  
	WriteCommand(0x02);		//д���ڴ�	
	
	for(i=0;i<144;i+=3)
	{
		for(j=0;j<8;j++)
		{
			if(num48[c][i]&(0x80>>j))
			LCD_WR_Data(fc);
			else
			LCD_WR_Data(bc);		
		}
		for(j=0;j<8;j++)
		{
			if(num48[c][i+1]&(0x80>>j))
			LCD_WR_Data(fc);	
			else
			LCD_WR_Data(bc);		
		}
		for(j=0;j<8;j++)
		{
			if(num48[c][i+2]&(0x80>>j))
			LCD_WR_Data(fc);	
			else
			LCD_WR_Data(bc);		
		}
	}	 	
} 

/**********************************************************************
functionName:void lcd_num48(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char *s) 
description:��ʾ48X24��������ֺ�kN
**********************************************************************/ 
void lcd_num48(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char *s) 
{
	while(*s) 
	{
		lcd_num48_char(x,y,fc,bc,*s);
		s++;
		x+=24;
	} 
}



#define	DISP_BUFF   10
#define	DISP_CHN 	6
char disp_buff[DISP_CHN][DISP_BUFF];
 

/**********************************************************************
functionName:void disp_syn(uint8_t chn)
description:��ʾͬ��
**********************************************************************/  
void disp_syn(uint8_t chn)
{
	uint8_t i;
	if(chn>=DISP_CHN)
	chn=0;	
	for(i=0;i<DISP_BUFF;i++)
	disp_buff[chn][i]=' ';
}


/**********************************************************************
functionName: 
description:��ʾ������ֵ
**********************************************************************/ 
void disp_value(float val,uint8_t chn,uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t int_len,uint8_t dec_len)
{
	char str[DISP_BUFF]; 
	uint8_t i=0;
	float2str(val,str,int_len,dec_len); 	
	while(str[i])
	{
		if(str[i]!=disp_buff[chn][i])
		{
			lcd_num48_char(x,y,fc,bc,str[i]);	
			disp_buff[chn][i]=str[i];	
		}
		x+=24;
		i++;	
		if(i>=DISP_BUFF)
		break;	 	
	} 
	//memcpy(disp_buff[chn],str,DISP_BUFF); 
}
 
/**********************************************************************
functionName:uint16_t rgb888_2_565(uint32_t rgb888)
description:��RGB888����ɫ��ʽת��ΪRGB565��ʽ
**********************************************************************/ 
uint16_t rgb888_2_565(uint32_t rgb888)
{
	uint16_t rgb565; 
	rgb565=((rgb888&0x00F80000)>>8)|((rgb888&0x0000FC00)>>5)|((rgb888&0x000000F8)>>3);
	return rgb565; 
}  


/**********************************************************************
functionName:void bsp_InitFont( void )
description:������س�ʼ��
**********************************************************************/
void bsp_InitFont( void )
{		
	fresult = f_mount(&fs,"0:/",1);	/* ע���豸 */
	
	if (fresult != FR_OK)
	{
		lcd_mem_err(400-16*5,240-16,RED,BLACK);
		
		while (1);
	}
}


/**********************************************************************
functionName:void lcd_time_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc)
description:ʱ����ʾ
**********************************************************************/ 
void lcd_time_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc)
{
	tTime t;
	char c[20];
 	t=get_time();
	usprintf(c,"%02d:%02d:%02d",t.ucHour,t.ucMin,t.ucSec);
	lcd_font16(x,y,fc,bc,c,"song16.zk");
}

/**********************************************************************
functionName:void lcd_time_diplay(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc)
description:������ʾ
**********************************************************************/ 
void lcd_date_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc)
{
	tTime t;
	char c[20];
 	t=get_time();
	usprintf(c,"%4d-%02d-%02d",t.usYear,t.ucMon,t.ucMday);
	lcd_font16(x,y,fc,bc,c,"song16.zk");
} 

/*------------------------------------------------------------
 * Function Name  : UpdateFlashFont
 * Description    : ����FLASH�ֿ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT UpdateFlashFont( const char *const pFont )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br = 0; 
	char pathFile[30];
	const uint16_t ONCE_READ_SIZE = 1024;
	uint8_t fileBuff[ONCE_READ_SIZE];
	uint32_t writeAddr = 0;
	uint32_t totalReadSize = 0;
	uint32_t totalWriteSize = 0;
	const char *pFontPath = NULL;
	const char *pFontName = NULL;
	
	if (strcmp("hz_song24",pFont) == 0)
	{
		writeAddr = SONG24_FONT_HZ_ADDR;
		pFontPath = SD_FONT_HZ_PATH;
		pFontName = "song24.zk";
	}
	else if (strcmp("eng_song24",pFont) == 0)
	{
		writeAddr = SONG24_FONT_ENG_ADDR;
		pFontPath = SD_FONT_ENG_PATH;
		pFontName = "song24.zk";
	}
	else
	{
		return FR_NO_FILE;
	}
	
	usprintf(pathFile,"%s%s",pFontPath,pFontName);   	
	fresult = f_open(&file_obj,pathFile,FA_OPEN_EXISTING|FA_READ); 	
	if (fresult != FR_OK)
	{
		f_close(&file_obj);
		
		return fresult;
	}
	
	totalReadSize = file_obj.fsize;
	
	do{
		fresult = f_read(&file_obj,fileBuff,ONCE_READ_SIZE,&br);  
		if (fresult != FR_OK)
		{
			break;
		}
		
		if (sf_WriteBuffer(fileBuff,writeAddr,br) == 0)
		{
			fresult = FR_DISK_ERR;
			break;
		}
		
		totalWriteSize += br;
		writeAddr += br;		
	} while (br != 0);
	
	f_close(&file_obj);
	
	if (fresult != FR_OK)
	{
		return fresult;
	}
	
	if (totalReadSize != totalWriteSize)
	{
		return FR_DISK_ERR;
	}
	
	return FR_OK;
}

/*------------------------------------------------------------
 * Function Name  : FlashFontIsExist
 * Description    : ���FLASH�ֿ��Ƿ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus FlashFontIsExist( void )
{
	uint32_t readFontPassword = 0;
	
	sf_ReadBuffer((uint8_t *)&readFontPassword,FONT_PASSWORD_ADDR,4);
	
	if (readFontPassword == FLASH_FONT_PASSWORD)
	{
		return YES;
	}
	else
	{
		return NO;
	}
}

/*------------------------------------------------------------
 * Function Name  : WriteFlashFontPassword
 * Description    :	д��FLASH�ֿ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus WriteFlashFontPassword( void )
{
	uint32_t writeFontPassword = FLASH_FONT_PASSWORD;
	uint32_t readFontPassword = 0;
	
	if ( sf_WriteBuffer((uint8_t *)&writeFontPassword,FONT_PASSWORD_ADDR,4) == 1)
	{
		sf_ReadBuffer((uint8_t *)&readFontPassword,FONT_PASSWORD_ADDR,4);
		if (readFontPassword == FLASH_FONT_PASSWORD)
		{
			return SUCCESS;
		}
	}
	
	return ERROR;
}

/*------------------------------------------------------------
 * Function Name  : SyncDispAllChannel
 * Description    : ͬ��������ʾͨ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SyncDispAllChannel( void )
{
	disp_syn(DISP_CHN_FH);
	disp_syn(DISP_CHN_WY);
	disp_syn(DISP_CHN_BX);
	disp_syn(DISP_CHN_STRENGTH);
	disp_syn(DISP_CHN_PEAK);
	disp_syn(DISP_CHN_SPEED);
}

/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/  
