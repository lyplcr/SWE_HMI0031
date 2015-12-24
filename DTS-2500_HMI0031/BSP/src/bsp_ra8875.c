/**
  ******************************************************************************
  * @file    bsp_ra8875.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   RA8875驱动文件
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
 


//uint8_t RAiO_DS1[] = "风м(RaiO Tech)888る8ら琌パ竤癸禣┦筿の戈癟筿玻珇ㄣ砞璸︽綪恨瞶盡穨┮承ミ璶眖ㄆIC砞璸の綪扳┯钡め〆Λ砞璸ぇ蔼м筿そカ初筂の瓣ず" ;
//uint8_t RAiO_DS2[] = "RA8875 TFT LCD北竟や穿陪ボ2D酶瓜硉DMA瓜戈穐簿ずTouch panel 北竟ㄏTFT LCD北虏虫ぃノ蔼顶CPU笷Θ▆陪ボ狦" ;
//uint8_t RAiO_DS3[] = "字型显示支援上海集通字库IC多达5种，包含16*16 24*24 32*32字体，支援BIG5 GB2312 GB12345 UNICODE ASCII UNIJIS JIS0208 LATIN 字码表。" ; 			 


//毫秒延时
// void DelayMs(uint32_t nMs)
// {
// 	uint32_t i;
// 	for(; nMs !=0; nMs--)
// 	{
// 		i = 10301;
// 		while(i--);
// 	}
// }

//读显示地址图像数据，数据读取
uint16_t ReadData(void)
{   
	uint16_t a=0; 
	a=*(__IO uint16_t *) (LCD_DATA_ADD); //L
	RA8875_WAITSTATUS();
	return(a);

}
 //读RA8875返回数据、状态读取
uint16_t ReadCOMM(void)
{
	uint16_t a=0; 
	a=*(__IO uint16_t *) (LCD_COMM_ADD); //L 
	RA8875_WAITSTATUS();
	 return(a); 
}

 
/**********************************************************************
functionName:void WriteCommand(uint16_t index)
description:写寄存器地址函数/写寄存器地址函数、指令写入
**********************************************************************/
void WriteCommand(uint16_t index)
{ 
	  *(__IO uint16_t *) (LCD_COMM_ADD)= index;
	  RA8875_WAITSTATUS(); 
}
 

//写16位数据函数、数据写入
void WriteData(uint16_t val)
{   
	  *(__IO uint16_t *) (LCD_DATA_ADD)= val;
//	  RA8875_WAITSTATUS(); 
}
 

 //演示程序
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
函数功能：清除整个LCD屏幕
输入：    目标颜色
输出：	  无
2014/6/9 11:07:26:chengs 修改顺序及注释掉Text_Background_Color
**************************************************************************************/ 
void LcdClear(uint16_t color)
{
 //  Text_Background_Color(color);			//设置颜色
   Geometric_Coordinate(0,799,0,479); 		//设定矩形坐标
   Text_Foreground_Color(color);			//设置颜色
   Draw_square_fill();
   Chk_Busy(); 
}

 

/**************************************************************************************
函数功能：初始化RA8875
输入：    无0x20
输出：	  无 78m 80 
注意：    无 
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
	
	//这样系统频率为37.5MHz	
	Software_Reset();		//锁频时间过后，使用者必须产生一个软件Reset已完成PLL频率改变的程序
	
	//选择16位MCU接口，16-bpp TFT接口
	WriteCommand(0x10);  //SYSR   uint8_t[4:3]=00 256 color  uint8_t[2:1]=  00 8uint8_t MPU interface
	WriteData(0x0F);   // if 8uint8_t MCU interface   and 65k color display----- 0x0c
	bsp_DelayMS(1);				
	
  	 
    //AT070TN92 setting   falling edge取样，pclk=sysclk
	WriteCommand(0x04);  //PCLK inverse
	WriteData(0x82);
	bsp_DelayMS(1);
	
	//TFT液晶屏参数的设置
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
	//setting active window X 工作窗口设定X
	WriteCommand(0x30); //Horizontal Start Point 0 of Active Window (HSAW0)
	WriteData(0x00); //Horizontal Start Point of Active Window [7:0]
	WriteCommand(0x31); //Horizontal Start Point 1 of Active Window (HSAW1)	   
	WriteData(0x00); //Horizontal Start Point of Active Window [9:8]
	WriteCommand(0x34); //Horizontal End Point 0 of Active Window (HEAW0)
	WriteData(0x1F); //Horizontal End Point of Active Window [7:0]
	WriteCommand(0x35); //Horizontal End Point 1 of Active Window (HEAW1)	   
	WriteData(0x03); //Horizontal End Point of Active Window [9:8]
	bsp_DelayMS(1);
	
	 
	Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel 
	HDIR_SEG0_SEGn();			//让屏幕正常显示
	Clear_Active_Window();		
	Memory_Clear();
	
	//打开LCD 
	Display_ON();
	  
	//设置背光
	PWM1_enable();
	PWM1_fnuction_sel();
	PWM1_clock_ratio(0x01);		//PWM的频率=SYS_CLK/256/分频数
	
	for (i=0; i<=0xff; ++i)
	{
		bsp_DelayMS(1);
		PWM1_duty_cycle(i);
	}
	
	//[40h]按照系统默认值设定系统默认
	//1、绘图模式 
	//2、内存写入时光标为不显示
	//3、游标不闪烁
	//4、左->右 上->下
	//5、当内存写入时光标位置自动加一
	//6、当内存读取时光标位置自动加一
	  
}


////设置前景色
//void Text_color(uint16_t color)//0xF800 == RED
//{
//	WriteCommand(0x63);	//放R
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


//RA8875等待空闲
void RA8875_WAITSTATUS(void)
{
//	while(!(GPIOB->IDR & GPIO_Pin_7 ));
	 while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6));  
}


//等待读取lcd的状态
uint8_t LCD_StatusRead(void)
{
	uint16_t Data;
	Data = ReadCOMM();
	return Data;
}
				
//------------------------//
//等待STATUS 内存读取、写入忙碌
void Chk_Busy(void)
{
	uint8_t temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
}

//等待BTE忙碌状态
void Chk_Busy_BTE(void)
{
	uint8_t temp; 
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);
}

//等待串口FLASH/ROM忙碌
void Chk_Busy_DIR_Access(void)
{
	uint8_t  temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x01)==0x01);
}

//REG[BFh]  等待DMA忙碌
void Chk_DMA_Busy(void)
{
	uint8_t temp; 	
	do
	{
		WriteCommand(0xBF);
		temp = ReadData();
	}while((temp&0x01)==0x01);
}


//REG[01h]	打开LCD 
void Display_ON(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x80);
	RA8875_WAITSTATUS();
}

//关闭LCD
void Display_OFF(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}

//退出随眠模式，但是关闭LCD
void Normal_Mode_exitSleep(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}

//进入睡眠模式
void Sleep_Mode(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x02);
}

//软件重启RA8875 
void Software_Reset(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x01);
	WriteData(0x00);
	bsp_DelayMS(1);// 
}				

			   
//-------------------------------//
//REG[04h] for test  下降沿沿采样
void PCLK_inversion(void)
{
	uint8_t temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp |= cSetD7;
	WriteData(temp);
}

//上升沿采样
void PCLK_non_inversion(void)
{
	uint8_t temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp &= cClrD7;
	WriteData(temp);
}

//设置PCLK频率周期
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
////REG[05h] 选择串口FLASH 0
//void Serial_ROM_select0(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp &=cClrD7;
//	WriteData(temp);
//}
//
////选择串口FLASH1
//void Serial_ROM_select1(void)
//{
//	uint8_t temp;
//  	WriteCommand(0x05);//PWRR
//	temp = ReadData();
//	temp |=cSetD7;
//	WriteData(temp);
//}
//
////此位必须设置为0
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
////设置串口FLASH 频率
//void SROM_CLK_DIV(uint8_t CLK_DIV)
//{
//	WriteCommand(0x06);
//	WriteData(CLK_DIV);
//}


//-------------------------------//
//REG[10h] 设置8bppTFT接口
void Color_256(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
} 

//设置16bpp TFT接口
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

//设置8位MCU接口
void MPU_8uint8_t_Interface(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD1D0 ;
	WriteData(temp);
}


//设置16位MCU接口
void MPU_16uint8_t_Interface(void)
{
	uint8_t temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp |= cSetD1ClrD0 ;
	WriteData(temp);
} 								 

 
//
////REG[12h]  返回输入口的值
//uint8_t GPI_data(void)
//{
//	uint8_t temp;
//	WriteCommand(0x12);//SYSR
//	temp = ReadData();
//	return temp;
//}
//
////这时输出口的值
//void GPO_data(uint8_t setx)
//{
//	WriteCommand(0x13); 
//	WriteData(setx);
//}

							
//---------------------------------//
//REG[20h]  设置单图层
void One_Layer(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}	
		
//设置双图层				 
void Two_Layers(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

//设置水平扫描方向SEG0->SEGn
void HDIR_SEG0_SEGn(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD3 ;
	WriteData(temp);
}

//设置水平扫描方向SEGn->SEG0
void HDIR_SEGn_SEG0(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD3 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

//设置垂直扫描方向COM0->COMn
void VDIR_COM0_COMn(void)
{
	uint8_t temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD2 ;
	WriteData(temp);
}

//设置垂直扫描方向COMn->COM0
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
////选择CGrom文字
//void CGROM_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD7 ;
//	WriteData(temp);
//}

////选择CGRAM文字
//void CGRAM_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);
//}

//// 选择内部CGROM
//void Internal_CGROM(void)
//{
//	uint8_t temp;
//	WriteCommand(0x21);//FNCR0
//	temp = ReadData();
//	temp &= cClrD5 ;
//	WriteData(temp);
//}
//
//// 选择外部CGROM
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
//// 文字对齐功能关闭。
//void No_FullAlignment(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD7 ;
//	WriteData(temp);
//}
//
//// 文字对齐功能开启
//void FullAlignment(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);
//}
//
////文字具背景色模式
//void Font_with_BackgroundColor(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD6 ;
//	WriteData(temp);
//}

//// 文字背景通透模式，无背景色
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
////文字不旋转 
//void NoRotate_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD4 ;
//	WriteData(temp);
//}
//
////文字旋转90度 
//void Rotate90_Font(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp |= cSetD4 ;
//	WriteData(temp);
//}
//
////水平文字放大1X
//void Horizontal_FontEnlarge_x1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD3D2 ;
//	WriteData(temp);
//}
//
////水平文字放大2X
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
////水平文字放大3X
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
////水平文字放大4X
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
////垂直文字放大X1
//void Vertical_FontEnlarge_x1(void)
//{
//	uint8_t temp;
//	WriteCommand(0x22);//FNCR1
//	temp = ReadData();
//	temp &= cClrD1D0 ;
//	WriteData(temp);
//}
//
////垂直文字放大X2
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
////垂直文字放大X3
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
////垂直文字放大X4
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
////REG[23h]	自造字型位置 CGRAM No							 
//void CGRAM_Select_Number(uint8_t setx)
//{ 
//	WriteCommand(0x23);//CGSR
//	WriteData(setx);
//}	
								
//-------------------------------------------//
//REG[24h]REG[25h]REG[26h]REG[27h]水平显示卷动偏移
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
////REG[29h] 文字行距设定  
//void Line_distance(uint8_t setx)
//{ 
//	WriteCommand(0x29);//LDS
//	WriteData(setx);
//}	  


////-------------------------------------------//
////REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]文字写入时的水平光标位置
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
////REG[2Eh]文字大小设定 
//void Font_size_16x16_8x16(void) 
//{
// 	uint8_t temp;
//	WriteCommand(0x2E);//FNCR1
//	temp = ReadData();
//	temp &= 0x3F;
//	WriteData(temp);
//}
//
////文字大小设定 
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
////文字大小设定 
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
////字符水平间距设定
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
//REG[30h]~REG[37h]  工作窗口 
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
//REG[38h]~REG[3Fh]卷动窗口设置
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
//REG[40h]  绘图模式
void Graphic_Mode(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD7 ;
	temp &= 0x00 ;
	WriteData(temp);
}


// 当内存写入时光标位置不会自动加
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
//// 文字模式
//void Text_Mode(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD7 ;
//	WriteData(temp);	 
//} 

//// 设定文字/内存写入光标为不显示。
//void Text_Cursor_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	//temp &= cClrD6 ;
//	temp &= 0x8F ;
//	WriteData(temp);
//}

//// 设定文字/内存写入光标为不显示。 游标闪烁
//void Text_Cursor_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	//temp |= cSetD6 ;//0x40
//	temp |= 0x60;
//	WriteData(temp);
//} 

//// 游标不闪烁
//void Text_Cursor_Blink_Disable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD5 ;
//	//temp &= 0x8F ;
//	WriteData(temp);
//}

//// 游标闪烁
//void Text_Cursor_Blink_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD5 ;//0x20
//	WriteData(temp);
//}

//绘图模式时的内存写入方向  左->右，然后上->下
void Memory_Write_LeftRight_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
}


//绘图模式时的内存写入方向 
void Memory_Write_RightLeft_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

//绘图模式时的内存写入方向
void Memory_Write_DownTop_LeftRight(void)
{
	uint8_t temp;	
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	WriteData(temp);
}

//绘图模式时的内存写入方向
void Memory_Write_TopDown_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
} 

//// 当内存写入时光标位置自动加一
//void MemoryWrite_Cursor_autoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD1 ;
//	WriteData(temp);
//}
//
//// 当内存写入时光标位置不会自动加一。
//void MemoryWrite_Cursor_NoautoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD1 ;
//	WriteData(temp);
//}
//
//// 当内存读取时光标位置自动加一
//void MemoryRead_Cursor_autoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp &= cClrD0 ;
//	WriteData(temp);
//}
//
//// 当内存读取时光标位置不会自动加一。
//void MemoryRead_Cursor_NoautoIncrease(void)
//{
//	uint8_t temp;
//	WriteCommand(0x40);//MWCR0
//	temp = ReadData();
//	temp |= cSetD0 ;
//	WriteData(temp);
//}  										 

//--------------------------------------------//
//REG[41h] 图形光标关闭
void No_Graphic_Cursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

// 图形光标开启
void Graphic_Cursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp |= cSetD7 ;
	temp |= 0xF0 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set1(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set2(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set3(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set4(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set5(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set6(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set7(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	WriteData(temp);
}

//图形光标的选择
void Graphic_Cursor_Set8(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	WriteData(temp);
}

//写入目的地选择 图层 1~2。  
void Write_To_Bank1and2(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

//写入目的地选择 CGRAM。
void Write_To_CGRAM(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

//写入目的地选择  图形光标
void Write_To_GraphicCursor(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
}

// 写入目的地选择  Pattern
void Write_To_Pattern(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	WriteData(temp);
}

//写入图层 1				   
void Write_To_Bank1(void)
{
	uint8_t temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD0 ;
	WriteData(temp);
}

//写入图层 2
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
////REG[44h]文字闪烁时间设定 
//void Text_Blink_Time(uint8_t setx)
//{	
//	WriteCommand(0x44);//CURS
//	WriteData(setx);
//}

//--------------------------------------------//
//REG[45h]  绘图模式时的内存读取方向 
void Memory_read_LeftRight_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp &= cClrD1D0;
	WriteData(temp);
}

//绘图模式时的内存读取方向 
void Memory_read_RightLeft_TopDown(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cClrD1SetD0;
	WriteData(temp);
}

//绘图模式时的内存读取方向 
void Memory_read_TopDown_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1ClrD0;
	WriteData(temp);
}

//绘图模式时的内存读取方向 
void Memory_read_DownTop_LeftRight(void)
{
	uint8_t temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1D0;
	WriteData(temp);
}

//--------------------------------------------//
//REG[46h]~REG[49h]内存写入光标水平位置[
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
//REG[4Ah]~REG[4Dh]内存读取光标水平位置
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
////REG[4Eh]文字写入光标水平大小设定
//void Text_Cursor_Horizontal_Size(uint8_t setx)//uint8_t[3:0]
//{
//	WriteCommand(0x4E);//CURS	   
//	WriteData(setx);
//} 
//								
////REG[4Fh] 文字写入光标垂直大小设定 [
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
//REG[60h]~REG[67h]背景色 
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
////背景色
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

//前景色 
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
////通透模式下的背景色之红色部分							
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
//图形光标水平位置
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
//256色图形光标颜色 0设定 
void Graphic_Cursor_Color_0(uint8_t setx)
{	  
	WriteCommand(0x84);//GCC0
	WriteData(setx); 
}  									  

//256色图形光标颜色 1设定 
void Graphic_Cursor_Color_1(uint8_t setx)
{	 
	WriteCommand(0x85);//GCC1
	WriteData(setx); 
}  	
 	 						  
//---------------------------------------------//
//REG[8A]脉波宽度调变 (PWM1)  开启
void PWM1_enable(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
 }

//脉波宽度调变 (PWM1) 关闭
void PWM1_disable(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

// 当PWM关闭或于睡眠模式时，PWM1输出为”Low” 状态。 
void PWM1_disable_level_low(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

// 当PWM关闭或于睡眠模式时，PWM1输出为”High” 状态
void PWM1_disable_level_high(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}


//PWM1功能
 void PWM1_fnuction_sel(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);
	RA8875_WAITSTATUS();  
 }

// PWM1 固定输出一频率为外部晶体振荡器Clock (Fin) 频率 
//1 /16 的Clock。
 void PWM1_system_clk_out(void)
{
	uint8_t temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

//PWM1 电路的频率来源选择 
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

/* 使能PWM1, 进行占空比调节 */

/* 	REG[8Ah] PWM1 Control Register (P1CR)

	Bit7 脉波宽度调变 (PWM1) 设定
		0 : 关闭，此状态下，PWM1 输出准位依照此缓存器Bit6 决定。
		1 : 开启。

	Bit6 PWM1 关闭时的准位
		0 : 当PWM 关闭或于睡眠模式时，PWM1 输出为”Low” 状态。
		1 : 当PWM 关闭或于睡眠模式时，PWM1 输出为”High” 状态。

	Bit5 保留

	Bit4 PWM1 功能选择
		0 : PWM1 功能。
		1 : PWM1 固定输出一频率为外部晶体振荡器Clock (Fin) 频率 1 /16 的Clock

	Bit3-0  PWM1 电路的频率来源选择PWM_CLK	【不是PWM输出频率】
		0000b : SYS_CLK / 1   , 1000b : SYS_CLK / 256
		0001b : SYS_CLK / 2   , 1001b : SYS_CLK / 512
		0010b : SYS_CLK / 4   , 1010b : SYS_CLK / 1024
		0011b : SYS_CLK / 8   , 1011b : SYS_CLK / 2048
		0100b : SYS_CLK / 16  , 1100b : SYS_CLK / 4096
		0101b : SYS_CLK / 32  , 1101b : SYS_CLK / 8192
		0110b : SYS_CLK / 64  , 1110b : SYS_CLK / 16384
		0111b : SYS_CLK / 128 , 1111b : SYS_CLK / 32768

		“SYS_CLK” 代表系统频率， 例如SYS_CLK 为20MHz， 当Bit[3:0] =0001b 时，PWM1 频率来源为10MHz。
		实际输出的PWM频率还需要除以 256，支持8位的分辨率。

		安富莱的4.3寸(480*272)模块，SYS_CLK =  68.75MHz
		安富莱的7.0寸(800*480)模块，SYS_CLK =  81.25MHz

		为了避免音频噪声，PWM频率可以选择
		（1） 低频100Hz
		（2） 高于 20KHz

		比如，Bit3-0为0011b时 SYS_CLK / 8，
			4.3寸 输出的PWM频率为 (68.75MHz / 8) / 256 = 33.56KHz
			7寸 输出的PWM频率为 (81.25MHz / 8) / 256 = 39.67KHz
*/
//PWM 的 Duty设定 
 void PWM1_duty_cycle(uint8_t setx)		 //uint8_t0~7
{
	WriteCommand(0x8a);
	WriteData((1<<7) | 8);		//256分频
	
	WriteCommand(0x8b);//PTNO
	WriteData(setx);
	RA8875_WAITSTATUS();
}

//脉波宽度调变 (PWM2) 开启
void PWM2_enable(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);  
 }


//脉波宽度调变 (PWM2)  关闭
void PWM2_disable(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

// 当PWM关闭或于睡眠模式时，PWM2输出为”Low” 状态
void PWM2_disable_level_low(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

// 当PWM关闭或于睡眠模式时，PWM2输出为”High” 状态
void PWM2_disable_level_high(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}


//PWM2 功能 
 void PWM2_fnuction_sel(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);  
 }

// PWM2 输出一相同于系统频率信号频率的Clock。 
//PWM2 = SYS_CLK / 16 
  void PWM2_system_clk_out(void)
{
	uint8_t temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

//PWM2电路的频率来源选择 
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
 
 //PWM的 Duty设定 
 void PWM2_duty_cycle(uint8_t setx) //uint8_t0~7
{  
    WriteCommand(0x8d);//PTNO
	WriteData(setx);
} 	


//---------------------------------------------//
//REG[8Eh]内存清除动作结束或停止
void Stop_Memory_Clear(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS(); 
}		

// 内存清除动作开始	 
void Memory_Clear(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp); 
	RA8875_WAITSTATUS();
}

// 内存清除范围为显示窗口，请参考REG[14h],[19h], [1Ah]。
void Clear_Full_Window(void)
{
	uint8_t temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}


//内存清除范围为工作窗口，请参考 REG[30h~37h] 的设定
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
//REG[90h]  画直线
void Draw_line(void)
{ 
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x80);
}


// 画矩形，不填充
void Draw_square(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);
	WriteData(0x90);
}

//画矩形，填充
void Draw_square_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);   
	WriteData(0xb0);   //1011 0000
}

//画圆形，不填充
void Draw_circle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x40);
}

//画圆形，填充
void Draw_circle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x60);
}

// 画三角形，不填充
void Draw_Triangle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x81);
}

// 画三角形，填充
void Draw_Triangle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x01);
	WriteData(0xA1);
}


//
//---------------------------------------------//
//REG[90h]~/EG[9Dh]  	  	//写入起始坐标XY								
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

//设定圆形的参数
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
//REG[A0h] 画椭圆
void Draw_Ellipse(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x00);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}

//画椭圆曲线
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

// 填满椭圆曲线
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


//填满椭圆
void Draw_Ellipse_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x40);
	WriteData(0xC0);
}

// 画圆角方形, 不填满
void Draw_Circle_Square(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x20);
	WriteData(0xA0);
}

// 画圆角方形, 填满
void Draw_Circle_Square_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x60);
	WriteData(0xE0);
}


//选择椭圆curve fill的某一块
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

//画椭圆/圆角方形参数设定
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


//设定圆圈、矩形、??
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

//画三点三点坐标输入
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
////REG[B0]~REG[B3]DMA起始地址设置
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
////DMA传输数量设置（在连续模式下）
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

////块模式下， 区块宽度设置
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
////REG[BFh] 设置连续模式
//void DMA_Continuous_mode(void)
//{ 
//	WriteCommand(0xBF);
//	WriteData(0x00);
//}
//
//
////设置块模式
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
////REG[C0h] 按键扫描使能
//void Key_Scan_Enable(void)
//{
//	uint8_t temp;
//	WriteCommand(0xC0);
//	temp = ReadData();
//	temp |= 0x80;
//	WriteData(temp);  
//}
//
////按键扫描设置
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
////这地方代码做过修改，要验证正确性
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
////显示一个字母
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
//	TIM4->CNT = 0;			//计数器清零
//	while(TIM4->CNT < time);	//等待
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
//    Font_Coordinate(10,120);   //砞﹚陪ボ竚	
//  
//	lcd_text24(10,120,0x0000,0xffff,"pattern fill");
//
//   
//     Font_Coordinate(10,200);   //砞﹚陪ボ竚	
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
//     // Font_Coordinate(10,320);   //砞﹚陪ボ竚	
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
//    SROM_CLK_DIV(0x02);	//秸俱畐clock
//
//
// Font_code_BIG5();
// Text_Foreground_Color(0x00);
// Font_Coordinate(10,10);			 
// Show_String("羉砰いゅ:",0);
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
// Show_String("简体中文:",0);
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
*	函 数 名: RA8875_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  :像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	/* 展开 	RA8875_SetCursor(_usX, _usY); 函数 */
	WriteCommand(0x46); WriteData(_usX);
	WriteCommand(0x47); WriteData(_usX >> 8);
	WriteCommand(0x48); WriteData(_usY);
	WriteCommand(0x49); WriteData(_usY >> 8);

	WriteCommand(0x02); 		/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */
	WriteData(_usColor);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  :像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;
	
	WriteCommand(0x40);				/* 设置为绘图模式，读取光标不自动加1 */
	WriteData((1 << 0));

	Memory_read_Coordinate(_usX, _usY);	/* 设置读取光标位置 */

	WriteCommand(0x02);
	usRGB = ReadData();	/* 第1次读取数据丢弃 */
	usRGB = ReadData();
	usRGB = ReadData();

	return usRGB;
}













/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

