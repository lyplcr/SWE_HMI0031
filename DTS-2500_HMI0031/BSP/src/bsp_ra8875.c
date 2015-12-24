/**
  ******************************************************************************
  * @file    bsp_ra8875.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   RA8875�����ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
  
#include "bsp.h"

//#include "p16x16.h"
//#include "p16x16_2.h"
//#include "p16x16_3.h"
//#include "pic_80x80.h"
//#include "rabbit.h"
//#include "touch.h"
 

//uint16_t  HDP=799;
//uint16_t  HT=1055;
//uint16_t  HPS=219;
//uint16_t  LPS=3;
//uint8_t 	HPW=8;
//
//uint16_t  VDP=479;
//uint16_t  VT=524;
//uint16_t  VPS=34;
//uint16_t  FPS=3;
//uint8_t   VPW=3;  
 


//uint8_t RAiO_DS1[] = "������(RaiO Tech)��88�~8��8��O�Ѥ@�s����O�ʹq�l�θ�T�q�l���~��h�~�]�p�B��P�B�޲z���M�~�H�~�ҳХߡA�D�n�q��IC�]�p�ξP��A�}�ӱ��Ȥ�e���]�p������޹q�l���q�A�����M�ΰꤺ�~�C" ;
//uint8_t RAiO_DS2[] = "RA8875 TFT LCD����A�䴩�r����ܡB2Dø�ϥ[�t�BDMA�ϸ�h���B����Touch panel ����A��TFT LCD�����²��A���ΰ�����CPU�]�i�H�F���}�n����ܮĪG�C" ;
//uint8_t RAiO_DS3[] = "������ʾ֧Ԯ�Ϻ���ͨ�ֿ�IC���5�֣�����16*16 24*24 32*32���壬֧ԮBIG5 GB2312 GB12345 UNICODE ASCII UNIJIS JIS0208 LATIN �����" ; 			 


//������ʱ
// void DelayMs(uint32_t nMs)
// {
// 	uint32_t i;
// 	for(; nMs !=0; nMs--)
// 	{
// 		i = 10301;
// 		while(i--);
// 	}
// }

//����ʾ��ַͼ�����ݣ����ݶ�ȡ
uint16_t ReadData(void)
{   
	uint16_t a=0; 
	a=*(__IO uint16_t *) (LCD_DATA_ADD); //L
	RA8875_WAITSTATUS();
	return(a);

}
 //��RA8875�������ݡ�״̬��ȡ
uint16_t ReadCOMM(void)
{
	uint16_t a=0; 
	a=*(__IO uint16_t *) (LCD_COMM_ADD); //L 
	RA8875_WAITSTATUS();
	 return(a); 
}

 
/**********************************************************************
functionName:void WriteCommand(uint16_t index)
description:д�Ĵ�����ַ����/д�Ĵ�����ַ������ָ��д��
**********************************************************************/
void WriteCommand(uint16_t index)
{ 
	  *(__IO uint16_t *) (LCD_COMM_ADD)= index;
	  RA8875_WAITSTATUS(); 
}
 

//д16λ���ݺ���������д��
void WriteData(uint16_t val)
{   
	  *(__IO uint16_t *) (LCD_DATA_ADD)= val;
//	  RA8875_WAITSTATUS(); 
}
 

 //��ʾ����
void LCD_test(void)
{
	uint16_t  temp=0; 
	uint32_t n; 
	LcdClear(0xf800);  
	n=0; 
	XY_Coordinate(0,0); 
	WriteCommand(0x02);//MRWC
	temp=0x07e0;
	while(n<384000l)
	{ 
	  	WriteData(temp); 
		n++; 
	}
	
	
	n=0; 
	XY_Coordinate(0,0); 
	WriteCommand(0x02);//MRWC
	temp=0xCCe0;
	while(n<384000l)
	{ 
	  	WriteData(temp); 
		n++; 
	}
	
	
	n=0; 
	XY_Coordinate(0,0); 
	WriteCommand(0x02);//MRWC
	temp=0x00CC;
	while(n<384000l)
	{ 
	  	WriteData(temp); 
		n++; 
	}
	
	 
	for(temp=0;temp<30;temp++)
	bsp_DelayMS(10);
	
	LcdClear(0xF800); 	
}


/**************************************************************************************
�������ܣ��������LCD��Ļ
���룺    Ŀ����ɫ
�����	  ��
2014/6/9 11:07:26:chengs �޸�˳��ע�͵�Text_Background_Color
**************************************************************************************/ 
void LcdClear(uint16_t color)
{
 //  Text_Background_Color(color);			//������ɫ
   Geometric_Coordinate(0,799,0,479); 		//�趨��������
   Text_Foreground_Color(color);			//������ɫ
   Draw_square_fill();
   Chk_Busy(); 
}

 

/**************************************************************************************
�������ܣ���ʼ��RA8875
���룺    ��0x20
�����	  �� 78m 80 
ע�⣺    �� 
**************************************************************************************/
void Init_RA8875(void)
{		
	uint16_t i;
	
	WriteCommand(0x88);	//SYS_CLK = FIN*(PLLDIVN[4:0] +1)/((PLLDIVM+1)*(2^PLLDIVK[2:0]))
	WriteData(0x09);	//PLLDIVN[4:0] = 5 FIN = 25MHz  PLLDIVM =0    
	bsp_DelayMS(1);
	
	WriteCommand(0x89);
	WriteData(0x01);	//PLLDIVK[2:0] =2
	bsp_DelayMS(10);
	
	//����ϵͳƵ��Ϊ37.5MHz	
	Software_Reset();		//��Ƶʱ�����ʹ���߱������һ�����Reset�����PLLƵ�ʸı�ĳ���
	
	//ѡ��16λMCU�ӿڣ�16-bpp TFT�ӿ�
	WriteCommand(0x10);  //SYSR   uint8_t[4:3]=00 256 color  uint8_t[2:1]=  00 8uint8_t MPU interface
	WriteData(0x0F);   // if 8uint8_t MCU interface   and 65k color display----- 0x0c
	bsp_DelayMS(1);				
	
  	 
    //AT070TN92 setting   falling edgeȡ����pclk=sysclk
	WriteCommand(0x04);  //PCLK inverse
	WriteData(0x82);
	bsp_DelayMS(1);
	
	//TFTҺ��������������
	//Horizontal set
	WriteCommand(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                      
	WriteData(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8                      
	WriteCommand(0x15);//Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)   
	WriteData(0x00);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]               
	WriteCommand(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                         
	WriteData(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8                
	WriteCommand(0x17); //HSTR//HSYNC Start Position[4:0]                                      
	WriteData(0x03);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                            
	WriteCommand(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                     
	WriteData(0x0B);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8    
	bsp_DelayMS(1);
	//Vertical set                             
	WriteCommand(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                            
	WriteData(0xdf);//Vertical pixels = VDHR + 1                                           
	WriteCommand(0x1a); //VDHR1 //Vertical Display Height Bit [8]                              
	WriteData(0x01);//Vertical pixels = VDHR + 1                                           
	WriteCommand(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                        
	WriteData(0x1f);//Vertical Non-Display area = (VNDR + 1)                               
	WriteCommand(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                          
	WriteData(0x00);//Vertical Non-Display area = (VNDR + 1)                               
	WriteCommand(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                    
	WriteData(0x16);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
	WriteCommand(0x1e); //VSTR1 //VSYNC Start Position[8]                                      
	WriteData(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
	WriteCommand(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                        
	WriteData(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)                                 
	bsp_DelayMS(1);
	 
  
	//Active window  set 
	//setting active window X ���������趨X
	WriteCommand(0x30); //Horizontal Start Point 0 of Active Window (HSAW0)
	WriteData(0x00); //Horizontal Start Point of Active Window [7:0]
	WriteCommand(0x31); //Horizontal Start Point 1 of Active Window (HSAW1)	   
	WriteData(0x00); //Horizontal Start Point of Active Window [9:8]
	WriteCommand(0x34); //Horizontal End Point 0 of Active Window (HEAW0)
	WriteData(0x1F); //Horizontal End Point of Active Window [7:0]
	WriteCommand(0x35); //Horizontal End Point 1 of Active Window (HEAW1)	   
	WriteData(0x03); //Horizontal End Point of Active Window [9:8]
	bsp_DelayMS(1);
	
	 
	Active_Window(0,799,0,479);	//�趨���ֱ���Ϊ800*480 pixel 
	HDIR_SEG0_SEGn();			//����Ļ������ʾ
	Clear_Active_Window();		
	Memory_Clear();
	
	//��LCD 
	Display_ON();
	  
	//���ñ���
	PWM1_enable();
	PWM1_fnuction_sel();
	PWM1_clock_ratio(0x01);		//PWM��Ƶ��=SYS_CLK/256/��Ƶ��
	
	for (i=0; i<=0xff; ++i)
	{
		bsp_DelayMS(1);
		PWM1_duty_cycle(i);
	}
	
	//[40h]����ϵͳĬ��ֵ�趨ϵͳĬ��
	//1����ͼģʽ 
	//2���ڴ�д��ʱ���Ϊ����ʾ
	//3���α겻��˸
	//4����->�� ��->��
	//5�����ڴ�д��ʱ���λ���Զ���һ
	//6�����ڴ��ȡʱ���λ���Զ���һ
	  
}


////����ǰ��ɫ
//void Text_color(uint16_t color)//0xF800 == RED
//{
//	WriteCommand(0x63);	//��R
//	WriteData((color&0xf800)>>11);//5  R
//	 
//	WriteCommand(0x64);
//	WriteData((color&0x07e0)>>5);//6  G
//	
//	WriteCommand(0x65);
//	WriteData((color&0x001f));//5    B
//	
//	RA8875_WAITSTATUS();
//}


//RA8875�ȴ�����
void RA8875_WAITSTATUS(void)
{
//	while(!(GPIOB->IDR & GPIO_Pin_7 ));
	 while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6));  
}


//�ȴ���ȡlcd��״̬
uint8_t LCD_StatusRead(void)
{
	uint16_t Data;
	Data = ReadCOMM();
	return Data;
}
				
//------------------------//
//�ȴ�STATUS �ڴ��ȡ��д��æµ
void Chk_Busy(void)
{
	uint8_t temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
}

//�ȴ�BTEæµ״̬
void Chk_Busy_BTE(void)
{
	uint8_t temp; 
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);
}

//�ȴ�����FLASH/ROMæµ
void Chk_Busy_DIR_Access(void)
{
	uint8_t  temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x01)==0x01);
}

//REG[BFh]  �ȴ�DMAæµ
void Chk_DMA_Busy(void)
{
	uint8_t temp; 	
	do
	{
		WriteCommand(0xBF);
		temp = ReadData();
	}while((temp&0x01)==0x01);
}


//REG[01h]	��LCD 
void Display_ON(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x80);
	RA8875_WAITSTATUS();
}

//�ر�LCD
void Display_OFF(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}

//�˳�����ģʽ�����ǹر�LCD
void Normal_Mode_exitSleep(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}

//����˯��ģʽ
void Sleep_Mode(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x02);
}

//�������RA8875 
void Software_Reset(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x01);
	WriteData(0x00);
	bsp_DelayMS(1);// 
}				

			   
//-------------------------------//
//REG[04h] for test  �½����ز���
void PCLK_inversion(void)
{
	uint8_t temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp |= cSetD7;
	WriteData(temp);
}

//�����ز���
void PCLK_non_inversion(void)
{
	uint8_t temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp &= cClrD7;
	WriteData(temp);
}

//����PCLKƵ������
void PCLK_width(uint8_t setx) //uint8_t[1:0]
{
	uint8_t temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp &=0x80;
	temp |= setx;
	WriteData(temp);
}

////-------------------------------//
////REG[05h] ѡ�񴮿�FLASH 0
//void Serial_ROM_select0(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD7;
//	WriteData(temp);
//}
//
////ѡ�񴮿�FLASH1
//void Serial_ROM_select1(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD7;
//	WriteData(temp);
//}
//
////��λ��������Ϊ0
//void Serial_ROM_Address_set_24bit(void)
//{   uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD6;
//	WriteData(temp);
//}
//
//void Serial_ROM_Address_set_32bit(void)
//{   uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD6;
//	WriteData(temp);
//}
//
//
//void Serial_ROM_Address_set_24uint8_t(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD6;
//	WriteData(temp);
//}
//
//void Serial_ROM_Address_set_32uint8_t(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD6;
//	WriteData(temp);
//}
//
//void Select_Serial_Waveform_mode0(void)
//{	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD5;
//	WriteData(temp);
//}
//
//void Select_Serial_Waveform_mode3(void)
//{	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD5;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Read_Cycle_4bus(void)
//{   uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xE7;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Read_Cycle_5bus(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xE7;
//	temp |=cSetD3;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Read_Cycle_6bus(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xE7;
//	temp |=cSetD4;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Font_mode(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD2;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_DMA_mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD2;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Signal_mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xFC;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Dual_mode0(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xFC;
//	temp |=cSetD1;
//	WriteData(temp);
//}
//
//void SERIAL_ROM_Dual_mode1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=0xFC;
//	temp |=cSetD1;
//	temp |=cSetD0;
//	WriteData(temp);
//}
////���ô���FLASH Ƶ��
//void SROM_CLK_DIV(uint8_t CLK_DIV)
//{
//	WriteCommand(0x06);
//	WriteData(CLK_DIV);
//}


//-------------------------------//
//REG[10h] ����8bppTFT�ӿ�
void Color_256(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
} 

//����16bpp TFT�ӿ�
void Color_65K(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteCommand(0x10);//SYSR
	WriteData(temp);
}

//����8λMCU�ӿ�
void MPU_8uint8_t_Interface(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD1D0 ;
	WriteData(temp);
}


//����16λMCU�ӿ�
void MPU_16uint8_t_Interface(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp |= cSetD1ClrD0 ;
	WriteData(temp);
} 								 

 
//
////REG[12h]  ��������ڵ�ֵ
//uint8_t GPI_data(void)
//{
//	uint8_t temp;
//	WriteCommand(0x12);//SYSR
//	temp = ReadData();
//	return temp;
//}
//
////��ʱ����ڵ�ֵ
//void GPO_data(uint8_t setx)
//{
//	WriteCommand(0x13); 
//	WriteData(setx);
//}

							
//---------------------------------//
//REG[20h]  ���õ�ͼ��
void One_Layer(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}	
		
//����˫ͼ��				 
void Two_Layers(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

//����ˮƽɨ�跽��SEG0->SEGn
void HDIR_SEG0_SEGn(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD3 ;
	WriteData(temp);
}

//����ˮƽɨ�跽��SEGn->SEG0
void HDIR_SEGn_SEG0(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD3 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

//���ô�ֱɨ�跽��COM0->COMn
void VDIR_COM0_COMn(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD2 ;
	WriteData(temp);
}

//���ô�ֱɨ�跽��COMn->COM0
void VDIR_COMn_COM0(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD2 ;
	WriteData(temp);
}


////----------------------------------//
////REG[21h] 
////ѡ��CGrom����
//void CGROM_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD7 ;
//	WriteData(temp);
//}

////ѡ��CGRAM����
//void CGRAM_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);
//}

//// ѡ���ڲ�CGROM
//void Internal_CGROM(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD5 ;
//	WriteData(temp);
//}
//
//// ѡ���ⲿCGROM
//void External_CGROM(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp |= cSetD5 ;
//	WriteData(temp);
//}
//
//
//void ISO8859_1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	WriteData(temp);
//}
//void ISO8859_2(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cClrD1SetD0 ;
//	WriteData(temp);
//}
//void ISO8859_3(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cSetD1ClrD0 ;
//	WriteData(temp);
//}
//void ISO8859_4(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cSetD1D0 ;
//	WriteData(temp);
//}
								 
////--------------------------------//
////REG[22h]
//// ���ֶ��빦�ܹرա�
//void No_FullAlignment(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD7 ;
//	WriteData(temp);
//}
//
//// ���ֶ��빦�ܿ���
//void FullAlignment(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);
//}
//
////���־߱���ɫģʽ
//void Font_with_BackgroundColor(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD6 ;
//	WriteData(temp);
//}

//// ���ֱ���ͨ͸ģʽ���ޱ���ɫ
// void Font_with_BackgroundTransparency(void)
// {
// 	uint8_t temp;
// 	WriteCommand(0x22);//FNCR1
// //	temp = ReadData();
// 	temp = cSetD6 ;
// 	WriteData(temp);
// 	RA8875_WAITSTATUS();
// }
//
//
////���ֲ���ת 
//void NoRotate_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD4 ;
//	WriteData(temp);
//}
//
////������ת90�� 
//void Rotate90_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp |= cSetD4 ;
//	WriteData(temp);
//}
//
////ˮƽ���ַŴ�1X
//void Horizontal_FontEnlarge_x1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD3D2 ;
//	WriteData(temp);
//}
//
////ˮƽ���ַŴ�2X
//void Horizontal_FontEnlarge_x2(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD3D2 ;
//	temp |= cClrD3SetD2 ;
//	WriteData(temp);
//}
//
////ˮƽ���ַŴ�3X
//void Horizontal_FontEnlarge_x3(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD3D2 ;
//	temp |= cSetD3ClrD2 ;
//	WriteData(temp);
//}
//
////ˮƽ���ַŴ�4X
//void Horizontal_FontEnlarge_x4(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD3D2 ;
//	temp |= cSetD3D2 ;
//	WriteData(temp);
//}
//
////��ֱ���ַŴ�X1
//void Vertical_FontEnlarge_x1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	WriteData(temp);
//}
//
////��ֱ���ַŴ�X2
//void Vertical_FontEnlarge_x2(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cClrD1SetD0 ;
//	WriteData(temp);
//}
//
////��ֱ���ַŴ�X3
//void Vertical_FontEnlarge_x3(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cSetD1ClrD0 ;
//	WriteData(temp);
//}
//
////��ֱ���ַŴ�X4
//void Vertical_FontEnlarge_x4(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	temp |= cSetD1D0 ;
//	WriteData(temp);
//}
//
////-------------------------------------------//
////REG[23h]	��������λ�� CGRAM No							 
//void CGRAM_Select_Number(uint8_t setx)
//{ 
//	WriteCommand(0x23);//CGSR
//	WriteData(setx);
//}	
								
//-------------------------------------------//
//REG[24h]REG[25h]REG[26h]REG[27h]ˮƽ��ʾ��ƫ��
void Scroll_Offset(uint16_t X,uint16_t Y)
{
	WriteCommand(0x24);//HOFS0
	WriteData(X);
	//RA8875_WAITSTATUS();
	WriteCommand(0x25);//HOFS1	   
	WriteData(X>>8);
	//RA8875_WAITSTATUS();
	WriteCommand(0x26);//VOFS0
	WriteData(Y);  
	//RA8875_WAITSTATUS();
	WriteCommand(0x27);//VOFS1	   
	WriteData(Y>>8); 
	RA8875_WAITSTATUS();
}	 
//
//
////-------------------------------------------//
////REG[29h] �����о��趨  
//void Line_distance(uint8_t setx)
//{ 
//	WriteCommand(0x29);//LDS
//	WriteData(setx);
//}	  


////-------------------------------------------//
////REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]����д��ʱ��ˮƽ���λ��
//void Font_Coordinate(uint16_t X,uint16_t Y)
//{
// 
//	WriteCommand(0x2A);//HOFS0
//	WriteData(X);
//
//	WriteCommand(0x2B);//HOFS1	   
//	WriteData(X>>8);
//	
//	WriteCommand(0x2C);//VOFS0
//	WriteData(Y);
//	
//	WriteCommand(0x2D);//VOFS1	   
//	WriteData(Y>>8); 
//}
//
//
////-------------------------------------------//
////REG[2Eh]���ִ�С�趨 
//void Font_size_16x16_8x16(void) 
//{
// 	uint8_t temp;
//	WriteCommand(0x2E);//FNCR1
//	temp = ReadData();
//	temp &= 0x3F;
//	WriteData(temp);
//}
//
////���ִ�С�趨 
//void Font_size_24x24_12x24(void) 
//{
// 	uint8_t temp;
//	WriteCommand(0x2E);//FNCR1
//	temp = ReadData();
//	temp &= 0x3F;
//	temp |= 0x40; 
//	WriteData(temp);
//}
//
////���ִ�С�趨 
//void Font_size_32x32_16x32(void) 
//{
// 	uint8_t temp;
//	WriteCommand(0x2E);//FNCR1
//	temp = ReadData();
//	temp &= 0x3f;
//	temp |= 0x80; 
//	WriteData(temp);
//}
//
////�ַ�ˮƽ����趨
//void Font_spacing_set(uint8_t setx) //uint8_t[5:0]
//{
//	uint8_t temp,temp1;
//	temp1=setx&0x3F;
//   	WriteCommand(0x2E);//FNCR1
//	temp = ReadData();
//	temp &= 0xC0;
//	temp |= temp1; 
//	WriteData(temp);
//}
//
////-------------------------------------------//
////REG[2Fh]  Serial Font ROM Setting
//void GT_serial_ROM_select_GT21L16T1W(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0x1F;
//	WriteData(temp);
//}
//
//void GT_serial_ROM_select_GT23L16U2W(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0x1F;
//	temp |= 0x20;
//	WriteData(temp);
//}
//
//void GT_serial_ROM_select_GT23L24T3Y(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0x1F;
//	temp |= 0x40;
//	WriteData(temp);
//}
//
//void GT_serial_ROM_select_GT23L24M1Z(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0x1F;
//	temp |= 0x60;
//	WriteData(temp);
//}
//
//void GT_serial_ROM_select_GT23L32S4W(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0x1F;
//	temp |= 0x80;
//	WriteData(temp);
//}
//
//void Font_code_GB2312(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	WriteData(temp);
//}
//
//void Font_code_GB12345(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x04;
//	WriteData(temp);
//}
//
//void Font_code_BIG5(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x08;
//	WriteData(temp);
//}
//
//void Font_code_UNICODE(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x0C;
//	WriteData(temp);
//}
//
//void Font_code_ASCII(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x10;
//	WriteData(temp);
//}
//
//void Font_code_UNIJIS(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x14;
//	WriteData(temp);
//}
//
//void Font_code_JIS0208(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x18;
//	WriteData(temp);
//}
//
//void Font_code_LATIN(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xE3;
//	temp |= 0x1C;
//	WriteData(temp);
//}
//
//void Font_Standard(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xFC;
//	WriteData(temp);
//}

//void Font_Arial(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xFC;
//	temp |= 0x01;
//	WriteData(temp);
//}
//
//void Font_Roman(void)
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= 0xFC;
//	temp |= 0x02;
//	WriteData(temp);
//}
//
//void Font_Bold(void) //for ASCII
//{
//	uint8_t temp;
//	WriteCommand(0x2F);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0;
//	temp |= cSetD1D0;
//	WriteData(temp);
//}

//-------------------------------------------//
//REG[30h]~REG[37h]  �������� 
void Active_Window(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{
	//setting active window X
	
	WriteCommand(0x30);//HSAW0
	WriteData(XL); 
	WriteCommand(0x31);//HSAW1	   
	WriteData(XL>>8);
	
	WriteCommand(0x34);//HEAW0
	WriteData(XR);
	WriteCommand(0x35);//HEAW1	   
	WriteData(XR>>8);
	
	//setting active window Y
	
	WriteCommand(0x32);//VSAW0
	WriteData(YT);  
	WriteCommand(0x33);//VSAW1	   
	WriteData(YT>>8);
	
	WriteCommand(0x36);//VEAW0
	WriteData(YB); 
	WriteCommand(0x37);//VEAW1	   
	WriteData(YB>>8);
	RA8875_WAITSTATUS();
}


//--------------------------------------------//
//REG[38h]~REG[3Fh]����������
void Scroll_Window(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{
	WriteCommand(0x38);//HSSW0
	WriteData(XL);
	WriteCommand(0x39);//HSSW1	   
	WriteData(XL>>8);
	
	WriteCommand(0x3c);//HESW0
	WriteData(XR);  
	WriteCommand(0x3d);//HESW1	   
	WriteData(XR>>8);   
	
	WriteCommand(0x3a);//VSSW0
	WriteData(YT);
	WriteCommand(0x3b);//VSSW1	   
	WriteData(YT>>8);
	
	WriteCommand(0x3e);//VESW0
	WriteData(YB);  
	WriteCommand(0x3f);//VESW1	   
	WriteData(YB>>8);
}  


//--------------------------------------------//
//REG[40h]  ��ͼģʽ
void Graphic_Mode(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD7 ;
	temp &= 0x00 ;
	WriteData(temp);
}


// ���ڴ�д��ʱ���λ�ò����Զ���
void Graphic_Mode_Memery_Cursor_NoAdd(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD7 ;
	temp &= 0x00 ;
	temp |= 0x02 ;
	WriteData(temp);
}

//
//// ����ģʽ
//void Text_Mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);	 
//} 

//// �趨����/�ڴ�д����Ϊ����ʾ��
//void Text_Cursor_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	//temp &= cClrD6 ;
//	temp &= 0x8F ;
//	WriteData(temp);
//}

//// �趨����/�ڴ�д����Ϊ����ʾ�� �α���˸
//void Text_Cursor_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	//temp |= cSetD6 ;//0x40
//	temp |= 0x60;
//	WriteData(temp);
//} 

//// �α겻��˸
//void Text_Cursor_Blink_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD5 ;
//	//temp &= 0x8F ;
//	WriteData(temp);
//}

//// �α���˸
//void Text_Cursor_Blink_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD5 ;//0x20
//	WriteData(temp);
//}

//��ͼģʽʱ���ڴ�д�뷽��  ��->�ң�Ȼ����->��
void Memory_Write_LeftRight_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
}


//��ͼģʽʱ���ڴ�д�뷽�� 
void Memory_Write_RightLeft_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

//��ͼģʽʱ���ڴ�д�뷽��
void Memory_Write_DownTop_LeftRight(void)
{
	uint8_t temp;	
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	WriteData(temp);
}

//��ͼģʽʱ���ڴ�д�뷽��
void Memory_Write_TopDown_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
} 

//// ���ڴ�д��ʱ���λ���Զ���һ
//void MemoryWrite_Cursor_autoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD1 ;
//	WriteData(temp);
//}
//
//// ���ڴ�д��ʱ���λ�ò����Զ���һ��
//void MemoryWrite_Cursor_NoautoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD1 ;
//	WriteData(temp);
//}
//
//// ���ڴ��ȡʱ���λ���Զ���һ
//void MemoryRead_Cursor_autoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD0 ;
//	WriteData(temp);
//}
//
//// ���ڴ��ȡʱ���λ�ò����Զ���һ��
//void MemoryRead_Cursor_NoautoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD0 ;
//	WriteData(temp);
//}  										 

//--------------------------------------------//
//REG[41h] ͼ�ι��ر�
void No_Graphic_Cursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

// ͼ�ι�꿪��
void Graphic_Cursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp |= cSetD7 ;
	temp |= 0xF0 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set1(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set2(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set3(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set4(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set5(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set6(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set7(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	WriteData(temp);
}

//ͼ�ι���ѡ��
void Graphic_Cursor_Set8(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	WriteData(temp);
}

//д��Ŀ�ĵ�ѡ�� ͼ�� 1~2��  
void Write_To_Bank1and2(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

//д��Ŀ�ĵ�ѡ�� CGRAM��
void Write_To_CGRAM(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

//д��Ŀ�ĵ�ѡ��  ͼ�ι��
void Write_To_GraphicCursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
}

// д��Ŀ�ĵ�ѡ��  Pattern
void Write_To_Pattern(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	WriteData(temp);
}

//д��ͼ�� 1				   
void Write_To_Bank1(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD0 ;
	WriteData(temp);
}

//д��ͼ�� 2
void Write_To_Bank2(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp |= cSetD0 ;
	WriteData(temp);  
}	
							
////--------------------------------------------//
////--------------------------------------------//
////REG[44h]������˸ʱ���趨 
//void Text_Blink_Time(uint8_t setx)
//{	
//	WriteCommand(0x44);//CURS
//	WriteData(setx);
//}

//--------------------------------------------//
//REG[45h]  ��ͼģʽʱ���ڴ��ȡ���� 
void Memory_read_LeftRight_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp &= cClrD1D0;
	WriteData(temp);
}

//��ͼģʽʱ���ڴ��ȡ���� 
void Memory_read_RightLeft_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cClrD1SetD0;
	WriteData(temp);
}

//��ͼģʽʱ���ڴ��ȡ���� 
void Memory_read_TopDown_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1ClrD0;
	WriteData(temp);
}

//��ͼģʽʱ���ڴ��ȡ���� 
void Memory_read_DownTop_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1D0;
	WriteData(temp);
}

//--------------------------------------------//
//REG[46h]~REG[49h]�ڴ�д����ˮƽλ��[
void XY_Coordinate(uint16_t X,uint16_t Y)
{ 
	WriteCommand(0x46);
	WriteData(X);
	WriteCommand(0x47);
	WriteData(X>>8); 
	WriteCommand(0x48);
	WriteData(Y);
	WriteCommand(0x49);
	WriteData(Y>>8);
}

//--------------------------------------------//
//REG[4Ah]~REG[4Dh]�ڴ��ȡ���ˮƽλ��
void Memory_read_Coordinate(uint16_t X,uint16_t Y)
{
	WriteCommand(0x4a);
	WriteData(X);   
	WriteCommand(0x4b);	   
	WriteData(X>>8);
	
	WriteCommand(0x4c);
	WriteData(Y);
	WriteCommand(0x4d);	   
	WriteData(Y>>8);
} 
						  
////--------------------------------------------//
////REG[4Eh]����д����ˮƽ��С�趨
//void Text_Cursor_Horizontal_Size(uint8_t setx)//uint8_t[3:0]
//{
//	WriteCommand(0x4E);//CURS	   
//	WriteData(setx);
//} 
//								
////REG[4Fh] ����д���괹ֱ��С�趨 [
//void Text_Cursor_Vertical_Size(uint8_t setx)//uint8_t[3:0]
//{
//	WriteCommand(0x4F);//CURS	   
//	WriteData(setx);
//} 	
//	
////--------------------------------------------//
////REG[50h] 
//void no_BTE_write(void)
//{
//	uint8_t temp;
//	WriteCommand(0x50);//BECR0
//	temp = ReadData();
//	temp &= cClrD7 ;
//	WriteData(temp);  
//}
//
//void BTE_enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x50);//BECR0
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);  
//}
//
//void BTE_contiguous_Data(void)
//{
//	uint8_t temp;
//	WriteCommand(0x50);//BECR0
//	temp = ReadData();
//	temp |= cSetD6 ;
//	WriteData(temp);  
//}
//
//void BTE_rectangular_Data(void)
//{
//	uint8_t temp;												   
//	WriteCommand(0x50);//BECR0
//	temp = ReadData();
//	temp &= cClrD6 ;
//	WriteData(temp);  
//}									  
//
////---------------------------------------------//
////REG[51h] 
//void BTE_ROP_Code(uint8_t setx)
//{	 
//	WriteCommand(0x51);//BECR1	   
//	WriteData(setx); 
//}									   

//---------------------------------------------//
//REG[52h] 
void Layer1_Visible(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	WriteData(temp);  
}

void Layer2_Visible(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x01;
	WriteData(temp);  
}	
						
void Transparent_Mode(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x03;
	WriteData(temp);  
}

void Lighten_Overlay_Mode(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x02;
	WriteData(temp);  
}	
						
void Boolean_OR(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x04;
	WriteData(temp);
}

void Boolean_AND(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x05;
	WriteData(temp);  
}  	 
	
  
void Floating_window_mode(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x06;
	WriteData(temp);  
} 

//void Floating_Window_transparent_with_BGTR_enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x52);//LTPR0
//	temp = ReadData();
//	temp|=cSetD5;
//	WriteData(temp);  
//}
//
//void Floating_Window_transparent_with_BGTR_disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x52);//LTPR0
//	temp = ReadData();
//	temp&=cClrD5;
//	WriteData(temp);  
//}


void Layer1_2_scroll(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	WriteData(temp);  
}  	 

void Layer1_scroll(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0x40;
	WriteData(temp);  
} 

void Layer2_scroll(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0x80;
	WriteData(temp);  
} 

void Buffer_scroll(void)
{
	uint8_t temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0xC0;
	WriteData(temp);  
} 

//---------------------------------------------//
//REG[53h] 
void layer2_1_transparency(uint8_t setx)
{  
	WriteCommand(0x53);//LTPR1
	WriteData(setx); 
}

//---------------------------------------------//
//REG[54h]~ [5Fh]
void BTE_Source_Destination	(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{
	uint8_t temp,temp1;
	
	temp=XL;   
	WriteCommand(0x54);//HSBE0
	WriteData(temp);
	temp=XL>>8;   
	WriteCommand(0x55);//HSBE1	   
	WriteData(temp);

	temp=XR;   
	WriteCommand(0x58);//HDBE0
	WriteData(temp);
	temp=XR>>8;   
	WriteCommand(0x59);//HDBE1	   
	WriteData(temp); 
    
	temp=YT;   
	WriteCommand(0x56);//VSBE0
	WriteData(temp);
	temp=YT>>8;   
	WriteCommand(0x57);//VSBE1
	temp1 = ReadData();
	temp1 &= 0x80;
	temp=temp|temp1; 
	WriteCommand(0x57);//VSBE1  
	WriteData(temp);


	temp=YB;   
	WriteCommand(0x5a);//VDBE0
	WriteData(temp);
	temp=YB>>8;   
	WriteCommand(0x5b);//VDBE1
	temp1 = ReadData();
	temp1 &= 0x80;
	temp=temp|temp1;	
	WriteCommand(0x5b);//VDBE1   
	WriteData(temp);
}							

void Source_Layer1(void)
{
	uint8_t temp;	
	WriteCommand(0x57);//VSBE1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

void Source_Layer2(void)
{
	uint8_t temp;	
	WriteCommand(0x57);//VSBE1
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

void Destination_Layer1(void)
{
	uint8_t temp;	
	WriteCommand(0x5b);//VDBE1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

void Destination_Layer2(void)
{
	uint8_t temp;	
	WriteCommand(0x5b);//VDBE1
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}
		  
////---------------------------------------------//
////---------------------------------------------//
//void BTE_Size_setting(uint16_t X,uint16_t Y)
//{	 
//	WriteCommand(0x5c);//BEWR0
//	WriteData(X); 
//	WriteCommand(0x5d);//BEWR1	   
//	WriteData(X>>8);
//  
//	WriteCommand(0x5e);//BEHR0
//	WriteData(Y);   
//	WriteCommand(0x5f);//BEHR1	   
//	WriteData(Y>>8);
//}									 

//---------------------------------------------//
//REG[60h]~REG[67h]����ɫ 
void Background_color(uint16_t color)
{
	WriteCommand(0x60);
	WriteData((color&0x001f));
		
	WriteCommand(0x61);
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x62);
	WriteData((color&0xf800)>>11);
}

////--------------------------------------------//
////����ɫ
//void Text_Background_Color(uint16_t color)
//{
//	WriteCommand(0x60);//BGCR0
//	WriteData((color&0x001f));
//
//	WriteCommand(0x61);//BGCR0
//	WriteData((color&0x07e0)>>5);
//	
//	WriteCommand(0x62);//BGCR0
//	WriteData((color&0xf800)>>11);
//	
//	RA8875_WAITSTATUS();
//} 
//

//ǰ��ɫ 
void Text_Foreground_Color(uint16_t color)
{	    
	WriteCommand(0x65);//BGCR0
	WriteData((color&0x001f));
	
	WriteCommand(0x64);//BGCR0
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x63);//BGCR0
	WriteData((color&0xf800)>>11);
	RA8875_WAITSTATUS();

/*
	WriteCommand(0x63);//BGCR0
	WriteData((color&0x001f));
	
	WriteCommand(0x64);//BGCR0
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x65);//BGCR0
	WriteData((color&0xf800)>>11);
	RA8875_WAITSTATUS();
	*/ 
}


//void BTE_Background_color(uint16_t color )
//{
//	WriteCommand(0x60);//BGCR0
//	//WriteData(setB);
//	 WriteData((color&0x001f));
//	WriteCommand(0x61);//BGCR0
//	//WriteData(setG);
//	 WriteData((color&0x07e0)>>5);
//	WriteCommand(0x62);//BGCR0
//	//WriteData(setR);
//	 WriteData((color&0xf800)>>11);
//	RA8875_WAITSTATUS();
//}

//
//void BTE_Foreground_color(uint16_t color)
//{
//	WriteCommand(0x63);//BGCR0
////	WriteData(setB);
//	 WriteData((color&0x001f));
//	WriteCommand(0x64);//BGCR0
////	WriteData(setG);
//	WriteData((color&0x07e0)>>5);
//	WriteCommand(0x65);//BGCR0
////	WriteData(setR);
//	 WriteData((color&0xf800)>>11);
//	RA8875_WAITSTATUS();
//}


////======================================
////for test
//void BTE_Background_red(uint8_t setx)
//{	  
//	WriteCommand(0x62);//BGCR0
//	WriteData(setx);
//}	
//						
//void BTE_Background_green(uint8_t setx)
//{	  
//	WriteCommand(0x61);//BGCR1
//	WriteData(setx);
//}							  
//
//void BTE_Background_blue(uint8_t setx)
//{	 
//	WriteCommand(0x60);//BGCR2
//	WriteData(setx);
//} 								
//
//void BTE_Foreground_red(uint8_t setx)
//{	
//	WriteCommand(0x65);//FGCR0
//	WriteData(setx);
//}								 
//
//void BTE_Foreground_green(uint8_t setx)
//{	  
//	WriteCommand(0x64);//FGCR1
//	WriteData(setx);
//}							 
//
//void BTE_Foreground_blue(uint8_t setx)
//{  
//	WriteCommand(0x63);//FGCR2
//	WriteData(setx);
//} 	
//
////======================================
//	
//void Pattern_Set_8x8(void)
//{
//	uint8_t temp;
//	
//	WriteCommand(0x66);//PTNO
//	temp = ReadData();
//	temp &= cClrD7;
//	WriteData(temp);
//}	
//
//void Pattern_Set_16x16(void)
//{
//	uint8_t  temp;
//
//	WriteCommand(0x66);//PTNO
//	temp = ReadData();
//	temp |= cSetD7; 
//	WriteData(temp);
//}								
//
//
//void Pattern_Set_number(uint8_t setx)
//{
//	uint8_t temp;
//	
//	WriteCommand(0x66);//PTNO
//	temp = ReadData();
//	temp &= 0x80;    
//	WriteData(temp|setx);
//} 	
//
////ͨ͸ģʽ�µı���ɫ֮��ɫ����							
//void BackgroundColor_TransparentMode(uint8_t setR,uint8_t setG,uint8_t setB)
//{
//	WriteCommand(0x67);//BGTR_R
//	WriteData(setR);
//
//	WriteCommand(0x68);//BGTR_G
//	WriteData(setG);
//
//	WriteCommand(0x69);//BGTR_B
//	WriteData(setB);
//}	  

////REG[70h]~REG[74h]
////==============================================================================
////Subroutine:	Enable_TP 
////==============================================================================
//void Enable_TP(void)
//{
//	uint8_t temp;	  
//	WriteCommand(0x70);//TPCR0	
//	temp = ReadData();
//	temp |= cSetD7;   
//	WriteData(temp);
//	RA8875_WAITSTATUS();
//}
////==============================================================================
////Subroutine:	Disable_TP 
////==============================================================================
//void Disable_TP(void)
//{
//	uint8_t temp;
//	WriteCommand(0x70);//TPCR0
//	temp = ReadData();
//	temp &= cClrD7;   
//	WriteData(temp);
//}
//
////==============================================================================
////Subroutine:	Enable_Touch_WakeUp
////==============================================================================
//void Enable_Touch_WakeUp(void)
//{
//	uint8_t temp;
//	WriteCommand(0x70);//TPCR0	
//	temp = ReadData();
//	temp |= cSetD3;   //cSetb3 = 00001000
//	WriteData(temp);
//}
////==============================================================================
////Subroutine:	Disable_Touch_WakeUp
////==============================================================================
//void Disable_Touch_WakeUp(void)
//{
//	uint8_t temp;
//	WriteCommand(0x70);//TPCR0	
//	temp = ReadData();
//	temp &= cClrD3;
//	WriteData(temp);
//}
//
////==============================================================================
////Subroutine:	TP manual or auto  mode enable
////==============================================================================
//void TP_manual_mode(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x71);//TPCR1	
//	temp = ReadData();
//	temp |= cSetD6;
//	WriteData(temp);
//}
//void TP_auto_mode(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x71);//TPCR1	
//	temp = ReadData();
//	temp &= cClrD6;//temp |= 0x3F;
//	temp |= 0x3F;
//	WriteData(temp);
//	RA8875_WAITSTATUS();
//}
////==============================================================================
////Subroutine:	TP_IDLE_mode
////==============================================================================
//void TP_IDLE_mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x71);//TPCR1
//	temp=ReadData(); 
//	temp &= 0xf0;
//	WriteData(temp);
//}
////==============================================================================
////Subroutine:	TP_event_mode
////==============================================================================
//void TP_event_mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x71);//TPCR1
//	temp=ReadData(); 
//	temp &= 0xf0;
//	temp |= 0x01;
//	WriteData(temp);
//	//  WriteData(0xc1);
//}
////==============================================================================
////Subroutine:	TP_latch_X
////==============================================================================
//void TP_latch_X(void)
//{
//	uint8_t temp;
//	WriteCommand(0x71);//TPCR1
//	temp=ReadData(); 
//	temp &= 0xc0;
//	temp |= 0x02;
//	WriteData(temp);
// }
// //==============================================================================
////Subroutine:	TP_latch_Y
////==============================================================================
//void TP_latch_Y(void)
//{
//	uint8_t temp;
//	WriteCommand(0x71);//TPCR1
//	temp=ReadData(); 
//	temp &= 0xc0;
//	temp |= 0x03;
//	WriteData(temp);
//}
////==============================================================================
////Subroutine:	Detect_TP_X_Y coordinate 
////==============================================================================
//uint8_t ADC_X(void)
//{
//	
//	uint8_t temp;
//	WriteCommand(0x72);//TPXH	 X_coordinate high byte
//	RA8875_WAITSTATUS();
//	temp=ReadData();
//	RA8875_WAITSTATUS();
//	return temp;
//}
//
//uint8_t ADC_Y(void)
//{
//	
//	uint8_t temp;
//	WriteCommand(0x73);//TPYH	  Y_coordinate high byte
//	RA8875_WAITSTATUS();
//	temp=ReadData();
//	RA8875_WAITSTATUS();
//	return temp;
//}
//
//uint8_t ADC_XY(void)
//{
//	
//	uint8_t temp;
// 	WriteCommand(0x74);//TPXYL	  uint8_t[3:2] Y_coordinate low byte  uint8_t[1:0] X_coordinate low byte 
//	temp=ReadData();
//	RA8875_WAITSTATUS();
//	return temp;
//}   



//REG[80h]~REG[83h]
//ͼ�ι��ˮƽλ��
void Graphic_Cursor_Coordinate(uint16_t X,uint16_t Y)
{
	WriteCommand(0x80);
	WriteData(X);  
	WriteCommand(0x81);	   
	WriteData(X>>8);
  
	WriteCommand(0x82);
	WriteData(Y);  
	WriteCommand(0x83);	   
	WriteData(Y>>8);
}

//---------------------------------//
//REG[84h]
//256ɫͼ�ι����ɫ 0�趨 
void Graphic_Cursor_Color_0(uint8_t setx)
{	  
	WriteCommand(0x84);//GCC0
	WriteData(setx); 
}  									  

//256ɫͼ�ι����ɫ 1�趨 
void Graphic_Cursor_Color_1(uint8_t setx)
{	 
	WriteCommand(0x85);//GCC1
	WriteData(setx); 
}  	
 	 						  
//---------------------------------------------//
//REG[8A]������ȵ��� (PWM1)  ����
void PWM1_enable(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
 }

//������ȵ��� (PWM1) �ر�
void PWM1_disable(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

// ��PWM�رջ���˯��ģʽʱ��PWM1���Ϊ��Low�� ״̬�� 
void PWM1_disable_level_low(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

// ��PWM�رջ���˯��ģʽʱ��PWM1���Ϊ��High�� ״̬
void PWM1_disable_level_high(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}


//PWM1����
 void PWM1_fnuction_sel(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);
	RA8875_WAITSTATUS();  
 }

// PWM1 �̶����һƵ��Ϊ�ⲿ��������Clock (Fin) Ƶ�� 
//1 /16 ��Clock��
 void PWM1_system_clk_out(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

//PWM1 ��·��Ƶ����Դѡ�� 
 void PWM1_clock_ratio(uint8_t setx) //uint8_t0~3 
{
	uint8_t temp,temp1;
	temp1= setx&0x0f;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= 0xf0;
	temp |= temp1 ;
	WriteData(temp);
	RA8875_WAITSTATUS(); 
 }

/* ʹ��PWM1, ����ռ�ձȵ��� */

/* 	REG[8Ah] PWM1 Control Register (P1CR)

	Bit7 ������ȵ��� (PWM1) �趨
		0 : �رգ���״̬�£�PWM1 ���׼λ���մ˻�����Bit6 ������
		1 : ������

	Bit6 PWM1 �ر�ʱ��׼λ
		0 : ��PWM �رջ���˯��ģʽʱ��PWM1 ���Ϊ��Low�� ״̬��
		1 : ��PWM �رջ���˯��ģʽʱ��PWM1 ���Ϊ��High�� ״̬��

	Bit5 ����

	Bit4 PWM1 ����ѡ��
		0 : PWM1 ���ܡ�
		1 : PWM1 �̶����һƵ��Ϊ�ⲿ��������Clock (Fin) Ƶ�� 1 /16 ��Clock

	Bit3-0  PWM1 ��·��Ƶ����Դѡ��PWM_CLK	������PWM���Ƶ�ʡ�
		0000b : SYS_CLK / 1   , 1000b : SYS_CLK / 256
		0001b : SYS_CLK / 2   , 1001b : SYS_CLK / 512
		0010b : SYS_CLK / 4   , 1010b : SYS_CLK / 1024
		0011b : SYS_CLK / 8   , 1011b : SYS_CLK / 2048
		0100b : SYS_CLK / 16  , 1100b : SYS_CLK / 4096
		0101b : SYS_CLK / 32  , 1101b : SYS_CLK / 8192
		0110b : SYS_CLK / 64  , 1110b : SYS_CLK / 16384
		0111b : SYS_CLK / 128 , 1111b : SYS_CLK / 32768

		��SYS_CLK�� ����ϵͳƵ�ʣ� ����SYS_CLK Ϊ20MHz�� ��Bit[3:0] =0001b ʱ��PWM1 Ƶ����ԴΪ10MHz��
		ʵ�������PWMƵ�ʻ���Ҫ���� 256��֧��8λ�ķֱ��ʡ�

		��������4.3��(480*272)ģ�飬SYS_CLK =  68.75MHz
		��������7.0��(800*480)ģ�飬SYS_CLK =  81.25MHz

		Ϊ�˱�����Ƶ������PWMƵ�ʿ���ѡ��
		��1�� ��Ƶ100Hz
		��2�� ���� 20KHz

		���磬Bit3-0Ϊ0011bʱ SYS_CLK / 8��
			4.3�� �����PWMƵ��Ϊ (68.75MHz / 8) / 256 = 33.56KHz
			7�� �����PWMƵ��Ϊ (81.25MHz / 8) / 256 = 39.67KHz
*/
//PWM �� Duty�趨 
 void PWM1_duty_cycle(uint8_t setx)		 //uint8_t0~7
{
	WriteCommand(0x8a);
	WriteData((1<<7) | 8);		//256��Ƶ
	
	WriteCommand(0x8b);//PTNO
	WriteData(setx);
	RA8875_WAITSTATUS();
}

//������ȵ��� (PWM2) ����
void PWM2_enable(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);  
 }


//������ȵ��� (PWM2)  �ر�
void PWM2_disable(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

// ��PWM�رջ���˯��ģʽʱ��PWM2���Ϊ��Low�� ״̬
void PWM2_disable_level_low(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

// ��PWM�رջ���˯��ģʽʱ��PWM2���Ϊ��High�� ״̬
void PWM2_disable_level_high(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}


//PWM2 ���� 
 void PWM2_fnuction_sel(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);  
 }

// PWM2 ���һ��ͬ��ϵͳƵ���ź�Ƶ�ʵ�Clock�� 
//PWM2 = SYS_CLK / 16 
  void PWM2_system_clk_out(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

//PWM2��·��Ƶ����Դѡ�� 
  void PWM2_clock_ratio(uint8_t setx) //uint8_t0~3 
{
	uint8_t temp,temp1;
	temp1= setx&0x0f;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= 0xf0;
	temp |= temp1 ;
	WriteData(temp);  
 }
 
 //PWM�� Duty�趨 
 void PWM2_duty_cycle(uint8_t setx) //uint8_t0~7
{  
    WriteCommand(0x8d);//PTNO
	WriteData(setx);
} 	


//---------------------------------------------//
//REG[8Eh]�ڴ��������������ֹͣ
void Stop_Memory_Clear(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS(); 
}		

// �ڴ����������ʼ	 
void Memory_Clear(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp); 
	RA8875_WAITSTATUS();
}

// �ڴ������ΧΪ��ʾ���ڣ���ο�REG[14h],[19h], [1Ah]��
void Clear_Full_Window(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}


//�ڴ������ΧΪ�������ڣ���ο� REG[30h~37h] ���趨
void Clear_Active_Window(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

									
//---------------------------------------------//
//REG[90h]  ��ֱ��
void Draw_line(void)
{ 
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x80);
}


// �����Σ������
void Draw_square(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);
	WriteData(0x90);
}

//�����Σ����
void Draw_square_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);   
	WriteData(0xb0);   //1011 0000
}

//��Բ�Σ������
void Draw_circle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x40);
}

//��Բ�Σ����
void Draw_circle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x60);
}

// �������Σ������
void Draw_Triangle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x81);
}

// �������Σ����
void Draw_Triangle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x01);
	WriteData(0xA1);
}


//
//---------------------------------------------//
//REG[90h]~/EG[9Dh]  	  	//д����ʼ����XY								
void Geometric_Coordinate(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB)
{	  
	WriteCommand(0x91);//Hs
	WriteData(XL);   
	WriteCommand(0x92);	   
	WriteData(XL>>8);
	
	WriteCommand(0x95);//Vs
	WriteData(XR);  
	WriteCommand(0x96);	   
	WriteData(XR>>8);
	
	WriteCommand(0x93);
	WriteData(YT);  
	WriteCommand(0x94);	   
	WriteData(YT>>8);
	
	WriteCommand(0x97);
	WriteData(YB);   
	WriteCommand(0x98);	   
	WriteData(YB>>8);
	RA8875_WAITSTATUS();
}

//�趨Բ�εĲ���
void Circle_Coordinate_Radius(uint16_t X,uint16_t Y,uint16_t R)
{ 
	WriteCommand(0x99);
	WriteData(X);  
	WriteCommand(0x9a);	   
	WriteData(X>>8);  
	
	WriteCommand(0x9b);
	WriteData(Y); 
	WriteCommand(0x9c);	   
	WriteData(Y>>8);
	
	WriteCommand(0x9d);
	WriteData(R);
	RA8875_WAITSTATUS();
} 

//---------------------------------------------//
//REG[A0h] ����Բ
void Draw_Ellipse(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x00);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}

//����Բ����
void Draw_Ellipse_Curve(void)
{
	uint8_t temp;

	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp &=0x1F;
	temp |= 0x90;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

// ������Բ����
void Draw_Ellipse_Curve_Fill(void)
{
	uint8_t temp;
	
	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp &=0x1F;
	temp |= 0xD0;
	WriteData(temp);
	RA8875_WAITSTATUS();
}


//������Բ
void Draw_Ellipse_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x40);
	WriteData(0xC0);
}

// ��Բ�Ƿ���, ������
void Draw_Circle_Square(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x20);
	WriteData(0xA0);
}

// ��Բ�Ƿ���, ����
void Draw_Circle_Square_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x60);
	WriteData(0xE0);
}


//ѡ����Բcurve fill��ĳһ��
void Draw_Ellipse_Curve_part(uint8_t setx)
{
	uint8_t temp,temp1;
	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp1 = setx&0x03;
	temp &=0xfc;
	temp |=temp1;
	WriteData(temp);   
}

//����Բ/Բ�Ƿ��β����趨
void Ellipse_Coordinate_setting(uint16_t X,uint16_t Y,uint16_t ELL_A,uint16_t ELL_B)
{

	WriteCommand(0xA5);
	WriteData(X);
	WriteCommand(0xA6);	   
	WriteData(X>>8);  
	   
	WriteCommand(0xA7);
	WriteData(Y); 
	WriteCommand(0xA8);	   
	WriteData(Y>>8);
	
	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);
	
	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8);
} 


//�趨ԲȦ�����Ρ�??
void Circle_Square_Coordinate_setting(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB ,uint16_t ELL_A,uint16_t ELL_B)
{	    
  
	WriteCommand(0x91);
	WriteData(XL);  
	WriteCommand(0x92);	   
	WriteData(XL>>8);
	
	WriteCommand(0x95);
	WriteData(XR);   
	WriteCommand(0x96);	   
	WriteData(XR>>8);
	     
	WriteCommand(0x93);
	WriteData(YT);  
	WriteCommand(0x94);	   
	WriteData(YT>>8);
	
	WriteCommand(0x97);
	WriteData(YB);  
	WriteCommand(0x98);	   
	WriteData(YB>>8);
	
	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);
	
	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8); 
}

//������������������
void Draw_Triangle_3point_Coordinate(uint16_t P1_X,uint16_t P1_Y,uint16_t P2_X,uint16_t P2_Y,uint16_t P3_X,uint16_t P3_Y)
{
	WriteCommand(0x91);
	WriteData(P1_X);
	WriteCommand(0x92);	   
	WriteData(P1_X>>8);
	
	WriteCommand(0x93);
	WriteData(P1_Y);
	WriteCommand(0x94);	   
	WriteData(P1_Y>>8);
	
	WriteCommand(0x95);
	WriteData(P2_X);
	WriteCommand(0x96);	   
	WriteData(P2_X>>8);
	
	WriteCommand(0x97);
	WriteData(P2_Y);
	WriteCommand(0x98);	   
	WriteData(P2_Y>>8);
	
	WriteCommand(0xA9);
	WriteData(P3_X);
	WriteCommand(0xAA);	   
	WriteData(P3_X>>8);
	
	WriteCommand(0xAB);
	WriteData(P3_Y);
	WriteCommand(0xAC);	   
	WriteData(P3_Y>>8);   
}

////---------------------------------------------//
////REG[B0]~REG[B3]DMA��ʼ��ַ����
//void DMA_Start_address_setting(uint32_t set_address)
//{ 
//	WriteCommand(0xB0);
//	WriteData(set_address);
//	
//	WriteCommand(0xB1);
//	WriteData(set_address>>8);
//	
//	WriteCommand(0xB2);
//	WriteData(set_address>>16);
//	
//	WriteCommand(0xB3);
//	WriteData(set_address>>24);
//}
//
//
////DMA�����������ã�������ģʽ�£�
////input pic data quantity  for example  pic size = 800*480 = 384000
//void DMA_Continuous_mode_size_setting(uint32_t set_size)
//{ 
//	WriteCommand(0xB4);
//	WriteData(set_size);
//	
//	WriteCommand(0xB6);
//	WriteData(set_size>>8);
//	
//	WriteCommand(0xB8);
//	WriteData(set_size>>16);
//}

////��ģʽ�£� ����������
//void DMA_block_mode_size_setting(uint16_t BWR,uint16_t BHR,uint16_t SPWR)
//{
//	WriteCommand(0xB4);
//	WriteData(BWR);
//	WriteCommand(0xB5);
//	WriteData(BWR>>8);
//	
//	WriteCommand(0xB6);
//	WriteData(BHR);
//	WriteCommand(0xB7);
//	WriteData(BHR>>8);
//	
//	WriteCommand(0xB8);
//	WriteData(SPWR);
//	WriteCommand(0xB9);
//	WriteData(SPWR>>8);  
//}

////---------------------------------------------//
////REG[BFh] ��������ģʽ
//void DMA_Continuous_mode(void)
//{ 
//	WriteCommand(0xBF);
//	WriteData(0x00);
//}
//
//
////���ÿ�ģʽ
//void DMA_Block_mode(void)
//{ 
//	WriteCommand(0xBF);
//	WriteData(0x02);
//}
//
//
////
//void DMA_Start_enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xBF);
//	temp = ReadData();
//	temp |= 0x01;
//	WriteData(temp);
//}
//
////---------------------------------------------//
////REG[C0h] ����ɨ��ʹ��
//void Key_Scan_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xC0);
//	temp = ReadData();
//	temp |= 0x80;
//	WriteData(temp);  
//}
//
////����ɨ������
//void Key_Scan_setting(uint8_t setx)//uint8_t[2:0]
//{
//	uint8_t temp;
//	WriteCommand(0xC0);
//	temp = ReadData();
//	temp &= 0xf0;
//	temp|= (setx&0x07);
//	WriteData(temp);  
//} 
//
//
//void Long_Key_Scan_enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xC0);
//	temp = ReadData();
//	temp |= 0x40;
//	WriteData(temp);
//}
//
//void Key_Scan_Wakeup_Function_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xC1);	
//	temp = ReadData();
//	temp |= 0x80;	// 
//	WriteData(temp);
//}
//
//void Long_Key_Timing_Adjustment(uint8_t setx)
//{
//	uint8_t temp,temp1;
//	temp = setx & 0x1c;
//	WriteCommand(0xC1);
//	temp1 = ReadData();
//	temp1|=temp;
//	WriteData(temp1);
//}

////---------------------------------------------//
////REG[D0]
//void Floating_window_start_point(uint16_t X,uint16_t Y)
//{ 
//	WriteCommand(0xD0);
//	WriteData(X);
//	WriteCommand(0xD1);
//	WriteData(X>>8);
//	
//	WriteCommand(0xD2);
//	WriteData(Y);
//	WriteCommand(0xD3);
//	WriteData(Y>>8);  
//}


//void Floating_window_size(uint16_t width_X,uint16_t high_Y)
//{
//	WriteCommand(0xD4);
//	WriteData(width_X);
//	WriteCommand(0xD5);
//	WriteData(width_X>>8);
//	
//	WriteCommand(0xD6);
//	WriteData(high_Y);
//	WriteCommand(0xD7);
//	WriteData(high_Y>>8);
//}
//
//void Floating_window_display_point(uint16_t X,uint16_t Y)
//{
//	WriteCommand(0xD8);
//	WriteData(X);
//	WriteCommand(0xD9);
//	WriteData(X>>8);
//	
//	WriteCommand(0xDA);
//	WriteData(Y);
//	WriteCommand(0xDB);
//	WriteData(Y>>8);
//}

//
////REG[E0h] 
//void Serial_ROM_direct_access_mode_enable(void)
//{
//	WriteCommand(0xE0);
//	WriteData(0x01);
//}
//
//void Serial_ROM_direct_access_mode_disable(void)
//{
//	WriteCommand(0xE0);
//	WriteData(0x00);
//}
//
//void Direct_access_mode_Address_32uint8_ts(uint32_t set_address)
//{
//	uint8_t temp;  
//	temp = set_address>>24;
//	WriteCommand(0xE1);
//	WriteData(temp);
//	temp = set_address>>16;
//	WriteData(temp);
//	temp = set_address>>8;
//	WriteData(temp);
//	temp = set_address;
//	WriteData(temp);
//}
//
//void Direct_access_mode_Address_24uint8_ts(uint32_t set_address)
//{
//	uint8_t temp=0;
//	
//	temp = set_address>>16;
//	WriteCommand(0xE1);
//	WriteData(temp);  
//	temp = set_address>>8;
//	WriteCommand(0xE1);
//	WriteData(temp);   
//	temp = set_address;
//	WriteCommand(0xE1);
//	WriteData(temp);
//}
//
//uint8_t Direct_access_mode_Read_Data_buffer(void)
//{
//	uint8_t Serial_Data;
//	
//	WriteCommand(0xE2);
//	Serial_Data=ReadData(); 
//	return Serial_Data;
//}

////---------------------------------------------//
//
//
//
////---------------------------------------------//
////REG[F0h] 
//
//void KEYSCAN_Interrupt_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);	
//	temp = ReadData();
//	temp |= 0x10;	// 
//	WriteData(temp); 
//}
//
//void KEYSCAN_Interrupt_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);	
//	temp = ReadData();
//	temp &= 0x0F;	// 
//	WriteData(temp); 
//}
//
//
//void DMA_Interrupt_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);	
//	temp = ReadData();
//	temp |= 0x08;	// 
//	WriteData(temp); 
//}
//
//void DMA_Interrupt_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);	
//	temp = ReadData();
//	temp &= 0x17;	// 
//	WriteData(temp); 
//}
//
//void Touch_Panel_Interrupt_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp = ReadData();
//	temp |= 0x04;
//	WriteData(temp);
//	RA8875_WAITSTATUS();
//}
//
//void Touch_Panel_Interrupt_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp = ReadData(); 
//	temp &= 0x1B;
//	WriteData(temp);
//}
//
//void BTE_Interrupt_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp=ReadData(); 
//	temp |= 0x02;
//	WriteData(temp);
//}
//
//void BTE_Interrupt_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp = ReadData(); 
//	temp &= 0x1D;
//	WriteData(temp);
//}
//
//void Font_BTE_read_write_Interrupt_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp = ReadData(); 
//	temp |= 0x01;
//	WriteData(temp);
//}
//
//void Font_BTE_read_write_Interrupt_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF0);//INTC
//	temp = ReadData(); 
//	temp &= 0x1E;
//	WriteData(temp);
//}

////---------------------------------------------//
////REG[F1h] 
//void Clear_keyscan_interrupt(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC
//	temp = ReadData(); 
//	temp |=cSetD4 ;
//	WriteData(temp);
//}
//
//uint8_t Keyscan_Status(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC	
//	temp = ReadData();
//	if ((temp&0x10)==0x10)
//	return 1;
//	else 
//	return 0;
//}
//
//void Clear_DMA_interrupt(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC
//	temp = ReadData(); 
//	temp |=cSetD3 ;
//	WriteData(temp);
//}
//
//uint8_t DMA_Status(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC	
//	temp = ReadData();
//	if ((temp&0x08)==0x08)
//	return 1;
//	else 
//	return 0;
//}
//
//void clear_TP_interrupt(void)
//{
////	RA8875_WAITSTATUS();
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC
//	temp=ReadData(); 
//	temp |= cSetD2;
//	WriteData(temp);
//	RA8875_WAITSTATUS();
// }


////=============================================
//
////��ط����������޸ģ�Ҫ��֤��ȷ��
//
////=============================================
//uint8_t Touch_Status(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC	
//	temp = ReadData();
//	
////	temp |=cSetD4 ;
////	WriteData(temp);
////	RA8875_WAITSTATUS();
//
//
//	if((temp&0x04) == 0x04)
//		return 1;
//	else
//		return 0;
//
//}
//
//void Clear_BTE_interrupt(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC
//	temp=ReadData(); 
//	temp |= cSetD1;
//	WriteData(temp);
//}
//
//uint8_t BTE_Status(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC	
//	temp = ReadData();
//	if ((temp&0x02)==0x02)
//		return 1;
//	else 
//		return 0;
//}
//
//void Clear_Font_BTE_read_write_Interrupt(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC
//	temp=ReadData(); 
//	temp |= cSetD0;
//	WriteData(temp);
//}
//
//uint8_t Font_BTE_read_write_Status(void)
//{
//	uint8_t temp;
//	WriteCommand(0xF1);//INTC	
//	temp = ReadData();
//	if ((temp&0x01)==0x01)
//		return 1;
//	else 
//		return 0;
//}
//
//

//void Chk_Wait(void)
//{
	
// while(MCU_WAIT==0);	   
//}


//void Show_String(uint8_t *str,uint8_t n)
//{
//	Text_Mode();
//	WriteCommand(0x02);
//	while(*str != '\0')
//	{
//		WriteData(*str);
//		++str;
//		Chk_Busy();
//		//Chk_Wait();
//		DelayMs(1);
//	}
//	DelayMs(n);
//}

//
////��ʾһ����ĸ
//void Show_WD(char *str,uint8_t n)
//{
//	RA8875_WAITSTATUS();
//	Text_Mode();
//	WriteCommand(0x02);
//	//while(*str != '\0')
//	//{
//		WriteData(*str);
//		//++str;
//		//Chk_Busy();
//		//Chk_Wait();
//		//DelayMs(1);
//	//}
//	DelayMs(n);
//}

//void LCD_Reset(void)
//{
//	GPIOA->BRR  = GPIO_Pin_3; //RA8875 RESET pin
//	DelayMs(110);
//	GPIOA->BSRR  = GPIO_Pin_3;
//	DelayMs(110);
//}
//
//void Waitus(uint16_t time)
//{
//	TIM4->CNT = 0;			//����������
//	while(TIM4->CNT < time);	//�ȴ�
//}

//uint16_t Get_X_ADC_Value_10bit(void)
//{
//	
//	uint16_t temp,temp1;
//	
//	temp=0;
//	temp|=ADC_X();
//	temp1=(temp<<2);
//	temp= temp1|((ADC_XY()&0x03));
//	RA8875_WAITSTATUS();
//	return temp;
//}

//
//uint16_t Get_Y_ADC_Value_10bit(void)
//{
//	
//	uint16_t temp,temp1;
//	
//	temp=0;
//	temp|=ADC_Y();
//	temp1=(temp<<2);
//	temp=temp1|((ADC_XY()>>2)&0x03);
//	RA8875_WAITSTATUS();
//	return temp;
//}
//void delaysometime(void)
//{
//	uint16_t i;
//	for(i=0;i<65530;i++);
//}


//void RA_Button_create(uint16_t x0,uint16_t y0,uint8_t xsize,uint8_t ysize,uint16_t fx,uint16_t fy,char *str,uint8_t state,uint8_t type)
//{
//  if(type==0)// type=0 2d button  type = 1 3d button
//  {
//    Graphic_Mode();
//    Text_Foreground_Color(BLACK);
//   
//    Geometric_Coordinate(x0,x0+xsize-1,y0,y0+ysize-1);
//    Draw_square();
//    Chk_Busy();
//    if(state) //if = 0 normal  if = 1 click
//    Text_Foreground_Color(WHITE);
//    else
//    Text_Foreground_Color(0x0430); 
//
//    Geometric_Coordinate(x0+1,x0+xsize-2,y0+1,y0+ysize-2);
//    Draw_square_fill();
//    Chk_Busy(); 
//    Text_Foreground_Color(BLACK); //set font color
//  
//    if(state)   //if = 0 normal  if = 1 click
//    Text_Background_Color(WHITE);
//    else
//    Text_Background_Color(0x0430);
//  }
//  else 
//  { //3D button setting 
//    Graphic_Mode();
//    Text_Foreground_Color(BLACK);
//    Geometric_Coordinate(x0,x0+xsize-1,y0,y0+ysize-1);
//    Draw_square();
//    Chk_Busy();
//
//    Text_Foreground_Color(WHITE);
//    Geometric_Coordinate(x0+1,x0+xsize-2,y0+1,y0+1);
//	Draw_line();
//    Chk_Busy();
//    Text_Foreground_Color(WHITE);
//    Geometric_Coordinate(x0+1,x0+1,y0+1,y0+ysize-2);
//	Draw_line();
//    Chk_Busy();
//
//	Text_Foreground_Color(0x3fef);
//    Geometric_Coordinate(x0+xsize-2,x0+xsize-2,y0+2,y0+ysize-2);
//	Draw_line();
//    Chk_Busy();
//    Text_Foreground_Color(0x3fef);
//    Geometric_Coordinate(x0+2,x0+xsize-2,y0+ysize-2,y0+ysize-2);
//	Draw_line();
//    Chk_Busy();
//
//    if(state) //if = 0 normal  if = 1 click
//     Text_Foreground_Color(0xf800);
//    else
//     Text_Foreground_Color(0x0430);
//
//    Geometric_Coordinate(x0+2,x0+xsize-3,y0+2,y0+ysize-3);
//    Draw_square_fill();
//    Chk_Busy(); 
//
//
//    Text_Foreground_Color(BLACK); //set font color 
//    if(state)   //if = 0 normal  if = 1 click
//    Text_Background_Color(WHITE);
//    else
//    Text_Background_Color(0x0430);
//
//   }
//
//   // Font_Coordinate(fx,fy);//set font start point
//   	lcd_text24(fx,fy,0xffff,0xffff,str);
// 
//  
//}







////======================================================
//void show_flash_pic(uint16_t x,uint16_t y,uint16_t lenth,uint16_t high,uint8_t number)
//{
//
//   SROM_CLK_DIV(2);    
//   Serial_ROM_select0(); 
//   //MX25F1605
//   Select_Serial_Waveform_mode3();  
//   //SST25VF016B
//
//   SERIAL_ROM_Read_Cycle_4bus();
//   SERIAL_ROM_DMA_mode();
//   
//   SERIAL_ROM_Signal_mode();
// 
//
//   DMA_Block_mode();
//
//   XY_Coordinate(x,y);
//   //if(number==0)
//   //DMA_Start_address_setting(0);
//   //else if(number==1)
//   Active_Window(x,x+lenth-1,y,y+high-1);
//   if(number!=14)
//   DMA_Start_address_setting(24200*number);
//   else
//	DMA_Start_address_setting(24200*12+141120+768000);
//   DMA_block_mode_size_setting(lenth,high,lenth);
//   DMA_Start_enable();
//   Chk_DMA_Busy();
//
//   Active_Window(0,799,0,479);
//
//
//
//}
////=====================================================

// 
//
////=====================================================
//void BTE_write_data_with_ROP(uint8_t setx)
//{ unsigned long i;
//    uint16_t temp;
//
//     BTE_ROP_Code(setx);
//     BTE_enable();
//    // WriteCommand(0x02);//MRWC
//	 WriteCommand(0x02);
//	 for(i=0;i<12800;i=i+2)
//	 {
//	 // WriteData(gImage_pic_80x80[i*2]);
//	  //WriteData(gImage_pic_80x80[i*2+1]);
//	    temp=(uint16_t)( gImage_pic_80x80[i]<<8)+gImage_pic_80x80[i+1];
//	   
//	   WriteData(temp);
//	  Chk_Busy();
//	 }
//}
////=====================================================
  

////=====================================================
//void show_pic(void)
//{  
//	unsigned long i;
//	uint16_t temp;
//	Graphic_Mode(); 
//	WriteCommand(0x02);
//	for(i=0;i<12800;i=i+2)
//	{
//		//WriteData(gImage_pic_80x80[i*2]);
//		//WriteData(gImage_pic_80x80[i*2+1]);
//		temp=(uint16_t)( gImage_pic_80x80[i]<<8)+gImage_pic_80x80[i+1];	   
//		WriteData(temp);
//		Chk_Busy();
//	}
//}


////=====================================================
//
////
//
////=====================================================
//void Show_button_BTE_function(void)
//{
//  uint16_t i,temp;
//  uint8_t h,k;
//  uint32_t j;
//  
//
//  //write pattern 16*16 to internal pattern ram
//   Pattern_Set_16x16();	   
//   Write_To_Pattern();        
//   Pattern_Set_number(0);
//   Graphic_Mode();
//   WriteCommand(0x02);                
//  // WriteData
//   for(i=0;i<512;i=i+2)//
//        {
//       //  WriteData(gImage_p16x16[i]);
//          temp=(uint16_t)( gImage_p16x16[i]<<8)+gImage_p16x16[i+1];
//	   
//	      WriteData(temp);
//		// gImage_rabbit
//		 Chk_Busy();
//        } 
//
//   Pattern_Set_number(1);
//   Graphic_Mode();
//   WriteCommand(0x02);                
//   for(i=0;i<512;i=i+2)//
//        {
//        // WriteData(gImage_p16x16_2[i]);
//          temp=(uint16_t)( gImage_p16x16_2[i]<<8)+gImage_p16x16_2[i+1];
//	   
//	      WriteData(temp);
//		 Chk_Busy();
//        } 
//   Pattern_Set_number(2);
//   Graphic_Mode();
//   WriteCommand(0x02);                
//   for(i=0;i<512;i=i+2)//
//        {
//         //WriteData(gImage_p16x16_3[i]);
//          temp=(uint16_t)( gImage_p16x16_3[i]<<8)+gImage_p16x16_3[i+1];
//	   
//	      WriteData(temp);
//		 Chk_Busy();
//        } 
// 
//     Write_To_Bank1and2();
//	 Write_To_Bank1();
//
//
//
//    
//	Text_Foreground_Color(WHITE);
//    Text_Background_Color(GRAY);
//   
//   
//	lcd_text24(10,10,0x0000,0xffff,"solid fill");
//
//    Font_Coordinate(10,120);   //�]�w��ܦ�m	
//  
//	lcd_text24(10,120,0x0000,0xffff,"pattern fill");
//
//   
//     Font_Coordinate(10,200);   //�]�w��ܦ�m	
//   
//	 lcd_text24(10,200,0x0000,0xffff,"write with ROP");
//
//    	
//	 lcd_text24(350,200,0x0000,0xffff,"color expansion");
//	 Graphic_Mode();
//
//
//    i=0;
//	k=0;
//   do{
//	 
//	
//
//	 if(k>2)
//	 k=0;
//
//
//	  
//
//	 
//
//
//	  Active_Window(0,639,0,479);
//     // Font_Coordinate(10,320);   //�]�w��ܦ�m	
//	 switch(i)
//	   {
//	    case 0 : 
//             //  Show_String("ROP=0      black",0);	
//			   lcd_text24(10,320,0x0000,0xffff,"ROP=0      black");
//			  
//			   break;
//		case 1 :
//		      // Show_String("ROP=1      ~(S+D)",0);
//			   lcd_text24(10,320,0x0000,0xffff,"ROP=1      ~(S+D)");
//			    break;
//		case 2 :
//		      // Show_String("ROP=2      ~S & D",0);
//			   	lcd_text24(10,320,0x0000,0xffff,"ROP=2      ~S & D");
//			    break;
//		case 3 :
//		      // Show_String("ROP=3      ~S",0);
//			   	lcd_text24(10,320,0x0000,0xffff,"ROP=3      ~S");
//			    break;
//		case 4 :
//		       //Show_String("ROP=4      S & ~D",0);
//			   		lcd_text24(10,320,0x0000,0xffff,"ROP=4      S & ~D");
//			    break;
//		case 5 :
//		      // Show_String("ROP=5      ~D",0);
//			   	   lcd_text24(10,320,0x0000,0xffff,"ROP=5      ~D");
//			    break;
//		case 6 :
//		      // Show_String("ROP=6      S XOR D",0);
//			      lcd_text24(10,320,0x0000,0xffff,"ROP=6      S XOR D");
//				break;
//		case 7 :
//		      // Show_String("ROP=7      S & D",0);
//			   	  lcd_text24(10,320,0x0000,0xffff,"ROP=7      S & D");
//			    break;
//		case 8 :
//		      // Show_String("ROP=8      S & D",0);
//			   //Show_String("",0);
//			   	  lcd_text24(10,320,0x0000,0xffff,"ROP=8      S & D");
//			    break;
//		case 9 :
//		      // Show_String("ROP=9      ~(S XOR D)",0);
//				lcd_text24(10,320,0x0000,0xffff,"ROP=9      ~(S XOR D)");
//			    break;
//		case 10 :
//		      // Show_String("ROP=10      D",0);
//			  	lcd_text24(10,320,0x0000,0xffff,"ROP=10      D");
//			    break;					
//		case 11 :
//		      // Show_String("ROP=11      ~S+D",0);
//			   	lcd_text24(10,320,0x0000,0xffff,"ROP=11      ~S+D");
//			    break;				
//		case 12 :
//		      // Show_String("ROP=12      S",0);
//			     lcd_text24(10,320,0x0000,0xffff,"ROP=12      S");
//				break;
//		case 13 :
//		      // Show_String("ROP=13      S+~D",0);
//			  	 lcd_text24(10,320,0x0000,0xffff,"ROP=13      S+~D");
//			    break;	
//		case 14 :
//		      // Show_String("ROP=14      S+D",0);
//			   	lcd_text24(10,320,0x0000,0xffff,"ROP=14      S+D");
//			    break;
//		case 15 :
//		     //  Show_String("ROP=15      white",0);
//			    lcd_text24(10,320,0x0000,0xffff,"ROP=15      white");
//				break;	
//				
//		default : break;
//
//		}
//	
//	
//	Graphic_Mode();																	  
//	 
//	 	 Active_Window(10+i,89+i,230,309);
//	     XY_Coordinate(10+i,230);
//         show_pic();
//		 Graphic_Mode();	
//	  	 BTE_Source_Destination(650,10+i,230,230);
//	  	 BTE_Size_setting(1,80);
//		 BTE_ROP_Code(0xc2);
//		 BTE_enable();
//		 Chk_Busy_BTE();
//		 if(i==0)
//		 {
//		   Graphic_Mode();	
//	  	   BTE_Source_Destination(650,89,230,230);
//	  	   BTE_Size_setting(16,80);
//		   BTE_ROP_Code(0xc2);
//		   BTE_enable();
//		   Chk_Busy_BTE();
//
//
//
//		 }
//		  
//		   
//	   Active_Window(400-i,519-i,230,329);
//	   XY_Coordinate(400-i,230);
//	    Graphic_Mode();
//
//	  WriteCommand(0x02);//MRWC
//      for(j=0;j<24000;j=j+2)
//      {
//      //   
//		temp=(uint16_t)( gImage_rabbit[j]<<8)+gImage_rabbit[j+1];
//	   
//	   WriteData(temp);
//	   	   
//	   Chk_Busy();	 
//	  }
//
//	   Graphic_Mode();	
//	  	 BTE_Source_Destination(650,400-i,230,230);
//	  	 BTE_Size_setting(1,100);
//		 BTE_ROP_Code(0xc2);
//		 BTE_enable();
//		 Chk_Busy_BTE();
//
//		 	 if(i==0)
//		 {
//		   Graphic_Mode();	
//	  	   BTE_Source_Destination(650,384,230,230);
//	  	   BTE_Size_setting(16,100);
//		   BTE_ROP_Code(0xc2);
//		   BTE_enable();
//		   Chk_Busy_BTE();
//
//
//
//		 }
//
//	   h=0;
//
//	    i++;
//	   if(i>15)
//	   i=0;	 	   
//	
//	 DelayMs(50);
//	
//	}while(1);
//	   
//	   Active_Window(0,799,0,479);
//
//}
////==================================================


//==================================================
//void Show_button_Font_function(void)
//{
// //uint16_t i;
// //uint8_t h;
//
//
//   //*******set Font rom*******
//	CGROM_Font();
//	//CGRAM_Font();
//
//	//Internal_CGROM();
//    External_CGROM();
//
//
//   // Font_size_16x16_8x16(); 
//    Font_size_24x24_12x24();
//   // Font_size_32x32_16x32();
//    
//    //GT_serial_ROM_select_GT21L16T1W();
//	//GT_serial_ROM_select_GT23L16U2W();
//	  GT_serial_ROM_select_GT23L24T3Y();
//	//GT_serial_ROM_select_GT23L24M1Z();
//	//GT_serial_ROM_select_GT23L32S4W();
//
//    //Font_code_GB2312();
//    //Font_code_GB12345();
//    Font_code_BIG5();
//    //Font_code_UNICODE();
//    //Font_code_ASCII();
//    //Font_code_UNIJIS();
//    //Font_code_JIS0208();
//     //Font_code_LATIN();
//
//
//    //Font_Standard();
//    Font_Arial();
//    //Font_Roman();
//    //Font_Bold();
//     
//    Serial_ROM_select1();
//    //Serial_ROM_select1();
//	Serial_ROM_Address_set_24bit();
//    //Serial_ROM_Address_set_32bit();
//    //Select_Serial_Waveform_mode0();
//    Select_Serial_Waveform_mode3();
//
//    //SERIAL_ROM_Read_Cycle_4bus();
//    SERIAL_ROM_Read_Cycle_5bus();
//    //SERIAL_ROM_Read_Cycle_6bus();
//
//	SERIAL_ROM_Font_mode();
//    //SERIAL_ROM_DMA_mode();
//
//    SERIAL_ROM_Signal_mode();
//    //SERIAL_ROM_Dual_mode0();
//    //SERIAL_ROM_Dual_mode1();
//
//	//*************************
//    SROM_CLK_DIV(0x02);	//�վ�r�wclock
//
//
// Font_code_BIG5();
// Text_Foreground_Color(0x00);
// Font_Coordinate(10,10);			 
// Show_String("�c�餤��:",0);
// Text_Foreground_Color(0x1f);
// Font_Coordinate(10,40);
// Show_String(RAiO_DS2,0);
//
//  Text_Foreground_Color(0x1f);
// Font_Coordinate(10,140);
//   Show_String(RAiO_DS1,0);
// Font_code_GB2312();
// Font_Coordinate(10,240);
// Text_Foreground_Color(0x00);
// Show_String("��������:",0);
// Text_Foreground_Color(0x00);
// Font_Coordinate(10,270);
// Show_String(RAiO_DS3,0);
//
// Graphic_Mode();
//
//  Active_Window(0,799,0,479);
//
//}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  :������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	/* չ�� 	RA8875_SetCursor(_usX, _usY); ���� */
	WriteCommand(0x46); WriteData(_usX);
	WriteCommand(0x47); WriteData(_usX >> 8);
	WriteCommand(0x48); WriteData(_usY);
	WriteCommand(0x49); WriteData(_usY >> 8);

	WriteCommand(0x02); 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	WriteData(_usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  :������ɫ
*	�� �� ֵ: RGB��ɫֵ
*********************************************************************************************************
*/
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;
	
	WriteCommand(0x40);				/* ����Ϊ��ͼģʽ����ȡ��겻�Զ���1 */
	WriteData((1 << 0));

	Memory_read_Coordinate(_usX, _usY);	/* ���ö�ȡ���λ�� */

	WriteCommand(0x02);
	usRGB = ReadData();	/* ��1�ζ�ȡ���ݶ��� */
	usRGB = ReadData();
	usRGB = ReadData();

	return usRGB;
}













/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

