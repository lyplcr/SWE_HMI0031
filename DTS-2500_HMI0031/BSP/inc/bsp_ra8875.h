
#ifndef __BSP_RA8875_H__
#define __BSP_RA8875_H__

// ra8875 d0-d15 -------- STM32F103VCT6 D0-D15
// ra8875 #d/c   -------- STM32F103VCT6 A16
// ra8875 #cs    -------- STM32F103VCT6 NE1
// ra8875 #wr    -------- STM32F103VCT6 WR
// ra8875 #rd    -------- STM32F103VCT6 RD

 
	
	
#define LCD_COMM_ADD	    ((uint32_t)0x60020000)		 
#define LCD_DATA_ADD		((uint32_t)0x60000000)	
 
 	
#define DISP_HOR_RESOLUTION		800
#define DISP_VER_RESOLUTION		480
#define DISP_HOR_PULSE_WIDTH	1
#define DISP_HOR_BACK_PORCH		210
#define DISP_HOR_FRONT_PORCH	45
#define DISP_VER_PULSE_WIDTH	1
#define DISP_VER_BACK_PORCH		34	
#define DISP_VER_FRONT_PORCH	10

#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)

#define	cSetD0		 0x01
#define	cSetD1		 0x02
#define	cSetD2		 0x04
#define	cSetD3		 0x08
#define	cSetD4		 0x10
#define	cSetD5		 0x20
#define	cSetD6		 0x40
#define	cSetD7		 0x80
                         
#define	cClrD0		 0xfe
#define	cClrD1		 0xfd
#define	cClrD2		 0xfb
#define	cClrD3		 0xf7
#define	cClrD4		 0xef
#define	cClrD5		 0xdf
#define	cClrD6		 0xbf
#define	cClrD7		 0x7f

#define cClrD1D0        0xfc
#define cClrD1SetD0	 	0x01
#define cSetD1ClrD0	 	0x02
#define cSetD1D0	 	0x03
#define cClrD3D2        0xf3
#define cClrD3SetD2     0x04
#define cSetD3ClrD2     0x08
#define cSetD3D2	 	0x0c

#define cClrD2D1D0	 		0xf8
#define cClrD2D1SetD0	 	0x01
#define cClrD2SetD1ClrD0 	0x02
#define cClrD2SetD1D0	 	0x03
#define cSetD2ClrD1D0	 	0x04
#define cSetD2ClrD1SetD0 	0x05

#define cClrD6D5D4	 		0x8f
#define cClrD6D5SetD4    	0x10
#define cClrD6SetD5ClrD4 	0x20
#define cClrD6SetD5D4	 	0x30
#define cSetD6ClrD5D4	 	0x40
#define cSetD6ClrD5SetD4 	0x50
#define cSetD6D5ClrD4	 	0x60
#define cSetD6D5D4	 		0x70
	 

//PWM1…Ë÷√
void PWM1_duty_cycle(uint8_t setx) ;
void PWM1_fnuction_sel(void);
void PWM1_enable(void);
void PWM1_clock_ratio(uint8_t setx);	 
	 
	 
void Active_Window(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB);
void Clear_Active_Window(void);
void Memory_Clear(void);
void XY_Coordinate(uint16_t X,uint16_t Y);
void Memory_read_Coordinate(uint16_t X,uint16_t Y);

void Geometric_Coordinate(uint16_t XL,uint16_t XR ,uint16_t YT ,uint16_t YB);
void Text_Foreground_Color(uint16_t color);
void Draw_square_fill(void);
void Chk_Busy(void);

void HDIR_SEG0_SEGn(void);
void Software_Reset(void);
void Display_ON(void); 

void Init_RA8875(void);
void LcdClear(uint16_t color);
void LCD_test(void); 
 

void WriteCommand(uint16_t index);
void WriteData(uint16_t val);
uint16_t ReadData(void);
uint16_t ReadCOMM(void);	
void RA8875_WAITSTATUS(void);

void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY);

#endif



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


