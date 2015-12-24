/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: bsp_lcd.h
** ����޸�����: 2014/4/24 8:45:57
** ��        ��: lcd��������
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE:  
**********************************************************************************************************/
#ifndef __BSP_LCD_H
#define	__BSP_LCD_H
 
#define	LCD_LENTH_X		800
#define	LCD_WIDTH_Y		480

/*
	LCD ��ɫ���룬CL_��Color�ļ�д
	16Bit�ɸ�λ����λ�� RRRR RGGG GGGB BBBB

	�����RGB �꽫24λ��RGBֵת��Ϊ16λ��ʽ��
	����windows�Ļ��ʳ��򣬵���༭��ɫ��ѡ���Զ�����ɫ�����Ի�õ�RGBֵ��

	�Ƽ�ʹ������ȡɫ���������㿴���Ľ�����ɫ��
*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ��8λR,G,Bת��Ϊ 16λRGB565��ʽ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)
enum
{
	CL_WHITE        = RGB(255,255,255),	/* ��ɫ */
	CL_BLACK        = RGB(  0,  0,  0),	/* ��ɫ */
	CL_RED          = RGB(255,	0,  0),	/* ��ɫ */
	CL_GREEN        = RGB(  0,255,  0),	/* ��ɫ */
	CL_BLUE         = RGB(  0,	0,255),	/* ��ɫ */
	CL_YELLOW       = RGB(255,255,  0),	/* ��ɫ */

	CL_GREY			= RGB( 98, 98, 98), 	/* ���ɫ */
	CL_GREY1		= RGB( 150, 150, 150), 	/* ǳ��ɫ */
	CL_GREY2		= RGB( 180, 180, 180), 	/* ǳ��ɫ */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ��ǳ��ɫ */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ��ǳ��ɫ */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS ��ť�����ɫ */

	CL_MAGENTA      = 0xF81F,	/* ����ɫ�����ɫ */
	CL_CYAN         = 0x7FFF,	/* ����ɫ����ɫ */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ����ɫ */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ����ɫ */
	CL_BLUE3        = RGB(  68, 68, 255),		/* ǳ��ɫ1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* ǳ��ɫ1 */

	/* UI ���� Windows�ؼ�����ɫ */
	CL_BTN_FACE		= RGB(236, 233, 216),	/* ��ť������ɫ(��) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* �����������ɫ */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* �������Ӱ����ɫ */


	CL_MASK			= 0x9999	/* ��ɫ���룬�������ֱ���͸�� */
};
 
//LCD��ɫ����
#define WHITE         	  0xFFFF				//��ɫ
#define BLACK         	  0x0000	  			//��ɫ
#define BLUE         	  0x001F  				//��ɫ
#define BRED              0XF81F
#define GRED 			  0XFFE0
#define GBLUE			  0X07FF
#define ORANGE			  0XBD20				//�ٻ�ɫ
#define RED           	  0xF800				//��ɫ
#define CRIMSON			  0xD8A7				//���ɺ�
#define MAGENTA       	  0xF81F				//�Ϻ�ɫ��
#define JOHNNY			  0X895C				//������ɫ
#define INDIGOO			  0X4810				//����/����ɫ
#define FUCHSIA_RED		  0x618A				//�Ϻ�ɫ
#define GREEN         	  0x07E0				//��ɫ
#define DARK_BLUE		  0x0174				//����ɫ
#define CYAN          	  0x7FFF				//��ɫ
#define INDIGO			  0x45D4				//����
#define YELLOW        	  0xFFE0				//��ɫ
#define BROWN 			  0XBC40 				//��ɫ
#define BRRED 			  0XFC07 				//�غ�ɫ
#define GRAY  			  0X8430 				//��ɫ
#define DARKBLUE      	  0X01CF				//����ɫ
#define LIGHTBLUE      	  0X7D7C				//ǳ��ɫ
#define MIDDLEBLUE		  0X047A				//����ɫ
#define GRAYBLUE       	  0X5458 				//����ɫ 
#define LIGHTGREEN     	  0X841F 				//ǳ��ɫ
#define LGRAY 			  0XC618 				//ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE         0XA651 				//ǳ����ɫ(�м����ɫ)
#define LBBLUE            0X2B12 				//ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#define CREAM_YELLOW	  0XFFDE				//�׻�ɫ
#define ELEPH_BLACK		  0X2924				//������
#define CHOCO			  0xD343   				//�ɿ���ɫ
#define COLD_ASH		  0X8450				//���
#define DEEP_SKY		  0X05FF				//����ɫ
#define INDIAN_RED		  0XCAEB				//ӡ�Ⱥ�
#define FOREST_GREEN	  0X2444				//ɭ����
#define ORANGE_RED		  0XFAE0				//�ٺ�ɫ
#define DarkSlateGray	  0X2A69				//ī��ɫ
#define DARK_BLACK		  0X3186				//����ɫ
#define LIGHT_BLACK		  0X632C				//����ɫ
#define WIN_CAPTION_UPC	  0X7D7C				//����CAPTION�ϲ������ɫ
#define WIN_CAPTION_DOWNC 0X5458				//����CAPTION�²������ɫ
#define	EASY_GREEN		  0X3287				//ǳ��ɫ
#define EASY_GRAY		  0X3186				//����ɫ(���)
#define WHITE1 			  0XD69A				//�Ұ�ɫ
#define GRAY_GREEN		  0X2726				//����

#define LITTLE_BLUE		  0X3D3B				//ǳ��
#define LIGHT_BLUE		  0X5E3C				//����
#define LIGHT_GRAY		  0xEF5D				//ǳ��(235 235 235) 
#define LIGHT_GRAY1		  0XCE5D				//ǳ��(200 200 232)
#define LIGHT_GRAY2		  0XA514				//ǳ��(160 160 160)
#define LIGHT_GRAY3		  0X6B4D				//ǳ��(105 105 105)
#define LIGHT_GRAY4		  0XDEFB				//ǳ��(220 220 220)
#define LIGHT_GREEN		  0XCFFF				//����ɫ
#define LIGHT_BROWN		  0X2160				//ǳ��
#define LIGHTLITTLE_BLUE  0XDF7E				//������
#define MIDDLE_BULU		  0X1CBC				//����
#define LITTLE_WHITE	  0XEF7D				//ǳ��
#define LITTLE_WHITE1	  0XE71C				//ǳ��1

#define GREEN_LITTLE1	  0x01E5				//ǳ��
#define BROWN_LITTLE1	  0x31E0				//ǳ��

#define APPLLE_GRAY		  0XC617				//��
#define APPLE_RECT_GRAY1  0XEF3B				
#define APPLE_RECT_GRAY2  0XD698

#define COLOR_BROWN		  0XB485				//��ɫ

#define PROCESS_UP_COLOR  0XCFF9				//�������ϰ벿��
#define PROCESS_DN_COLOR  0X0664				//�������°벿��

#define LITTLE_BLUE_SKY  0X1CFC					//�����(0x189ce0)24 156 224
#define FRESH_GREEN		 0XA667					//����

//���ߵĽṹ��
typedef struct
{
  uint16_t x;							//���x����
  uint16_t y;							//���y����
  uint16_t width;						//�ߵĿ��
  uint16_t fill;						//��䳤��  
  uint16_t space;						//�����ĳ���
  uint16_t length;						//�ߵ��ܳ�
  uint8_t direct;						//�ߵķ��� 0��ʾ���ߣ�1��ʾ����		
  uint16_t color; 						//��ɫ		
}tDashedObject;

//���߿�ṹ��
typedef struct
{
  uint16_t x;								//���x����
  uint16_t y;								//���y����
  uint16_t hori_num;						//������
  uint16_t hori_space;						//�������	
  uint16_t veri_num;						//������
  uint16_t veri_space;						//������
  uint16_t width;							//�ߵĿ��
  uint16_t fill;							//��䳤��  
  uint16_t space;							//�����ĳ���	
  uint16_t color_row; 						//������ɫ
  uint16_t color_col;						//������ɫ		
}tDashedFrameObject; 

/*LCD��صĺ���*/ 
void lcd_fill(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color);		  	  	//������
void lcd_fill_ellipse(uint16_t x0, uint16_t y0, uint16_t rx, uint16_t ry,uint16_t color); 	//����Բ
void lcd_fill_circle(uint16_t x0, uint16_t y0, uint16_t r,uint16_t color);			 	  	//���Բ
void lcd_draw_circle(uint16_t  x0,uint16_t y0,uint16_t r,uint16_t color);			 		//��Բ
void lcd_draw_line(uint16_t x1, uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color); 		//����
void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color);							 		//����
void lcd_draw_rect(uint16_t sx,uint16_t sy,uint16_t w,uint16_t h,uint16_t color);	 		//������

FRESULT lcd_font16(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font); 		//��ʾ16������
FRESULT lcd_font20(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font);		//��ʾ20������
FRESULT lcd_font24(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font);		//��ʾ24������
FRESULT lcd_font32(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,const char *const s,const char *const font);		//��ʾ32������

void lcd_ascin16s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s); 	   				//��ʾ16��ASCII��
void lcd_ascin24s(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc,char *s); 					//��ʾ24��ASCII��

void lcd_num48(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char *s);						//��ʾ48���ַ�
void lcd_num48_char(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc, char c);					//��ʾ48��.

 

void bsp_InitLCD(void);
void lcd_clear(uint16_t color);
void lcd_set_backlight(uint8_t duty);														//����LCD�ı���  ����ֵ0-0xA0
FRESULT lcd_show_image(uint16_t sx,uint16_t sy,char *path); 							    //��ʾͼƬ
void lcd_draw_dashed(tDashedObject dashed_line);											//������
void lcd_draw_dashed_frame(tDashedFrameObject dashed_frame);								//�����߿�
void lcd_time_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc);					    //��ʾʱ��
void lcd_date_display(uint16_t x, uint16_t y,uint16_t fc,uint16_t bc); 					    //��ʾ����
void lcd_mem_err(uint16_t x,uint16_t y,uint16_t fc,uint16_t bc);						    //��ʾ���ڴ濨�������֣���С�̶�Ϊ32X32,����ȡSD��������ִ�д˺�������ʾ�û�
uint16_t rgb888_2_565(uint32_t rgb888);														//��ɫ��ʽת��  
//��ɫת���� 888 -> 565 ,ɾ��ԭʼ���ݵ�3λ-��2λ-��3λ 
void bsp_InitFont( void );
 
 
//��ʾͨ��  
#define		DISP_CHN_FH			0		//����
#define		DISP_CHN_WY			1		//λ��
#define		DISP_CHN_BX			2  		//����

#define		DISP_CHN_SPEED		1		//�ٶ�
#define 	DISP_CHN_STRENGTH	2		//ǿ��
#define 	DISP_CHN_PEAK		3		//��ֵ

void disp_syn(uint8_t chn);			//��ʾͬ��,���л�����ʾֵ�Ľ���ʱ����Ҫ����ˢ����ʾ���壬��Ȼ��ʾ����ߵ����ֻ���ֲ����µ�����
void disp_value(float val,uint8_t chn,uint16_t x,uint16_t y,uint16_t fc,uint16_t bc,uint8_t int_len,uint8_t dec_len);  	//��ʾ������ɼ����ĸ���/λ��/�������ݣ�������ʾnλ��С�������ʾnλ

#endif




	

	

	












