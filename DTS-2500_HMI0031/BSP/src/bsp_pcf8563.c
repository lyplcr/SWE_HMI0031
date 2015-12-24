/**
  ******************************************************************************
  * @file    bsp_pcf8563.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   时钟驱动文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
  
#include "bsp.h"

 
//PB6------SCL
//PB7------SDA
 
#define		PIN_I2C_SCL		GPIO_Pin_6
#define		PIN_I2C_SDA 	GPIO_Pin_7
 
  
#define		STU_I2C_SDA				(GPIO_ReadInputDataBit(GPIOB,PIN_I2C_SDA))	 
#define		SET_SCL					GPIO_SetBits(GPIOB,PIN_I2C_SCL) 
#define		CLR_SCL				 	GPIO_ResetBits(GPIOB,PIN_I2C_SCL) 
#define		SET_SDA					GPIO_SetBits(GPIOB,PIN_I2C_SDA) 
#define		CLR_SDA					GPIO_ResetBits(GPIOB,PIN_I2C_SDA) 
#define		I2C_DELAY 				1



#define		PCF8563_ADD_WR					0xA2	
#define		PCF8563_ADD_RD					0xA3  
#define		REG_8563_SEC					0x02
#define		REG_8563_MIN					0x03
#define		REG_8563_HOR					0x04
#define		REG_8563_DATE					0x05
#define		REG_8563_WEEK					0x06
#define		REG_8563_MON					0x07
#define		REG_8563_YEAR					0x08
#define		REG_8563_SAV1					0x09
#define		REG_8563_SAV2					0x0A


// uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
//                                       This parameter can be any value of @ref GPIO_pins_define */
//
//  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
//                                       This parameter can be a value of @ref GPIOMode_TypeDef */
//
//  GPIOSpeed_TypeDef GPIO_Speed;   /*!< Specifies the speed for the selected pins.
//                                       This parameter can be a value of @ref GPIOSpeed_TypeDef */
//
//  GPIOOType_TypeDef GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
//                                       This parameter can be a value of @ref GPIOOType_TypeDef */
//
//  GPIOPuPd_TypeDef GPIO_PuPd;

/**********************************************************************
functionName:void i2c_mastar(void)
description:i2c是主机
**********************************************************************/
void i2c_mastar(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = PIN_I2C_SDA;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //口线速度50MHZ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		 //开漏输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**********************************************************************
functionName:void i2c_slave(void)
description:I2C从机设置
**********************************************************************/
void i2c_slave(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = PIN_I2C_SDA;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;		 //浮空输入 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

 /**********************************************************************
functionName:void i2c_init(void)
description:初始化I2C
**********************************************************************/
void i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = PIN_I2C_SCL|PIN_I2C_SDA;				  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			 //口线速度50MHZ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	i2c_mastar();
}


 /**********************************************************************
functionName:void i2_start(void)
description:I2C开始标志
**********************************************************************/
void i2_start(void)
{
	SET_SDA;
	SET_SCL;
	bsp_DelayUS(I2C_DELAY);
	CLR_SDA;
	bsp_DelayUS(I2C_DELAY);
	CLR_SCL;
}

/**********************************************************************
functionName:void i2_stop(void)
description:I2C停止标志
**********************************************************************/
void i2_stop(void)
{
	CLR_SCL;
	CLR_SDA;
	bsp_DelayUS(I2C_DELAY);
	SET_SCL;
	bsp_DelayUS(I2C_DELAY);
	SET_SDA; 
}

/**********************************************************************
functionName:void i2c_sendbyte(uint8_t temp)
description:I2C发送字节
**********************************************************************/
void i2c_sendbyte(uint8_t temp)
{
	uint8_t i=8;	
	while (i--)
	{
		CLR_SCL; 
		bsp_DelayUS(I2C_DELAY);
		if(temp&0x80)
	  	{
	       	SET_SDA;
		}
		else
		{
			CLR_SDA;
		} 
		temp<<=1;
		bsp_DelayUS(I2C_DELAY);
		SET_SCL;
		bsp_DelayUS(I2C_DELAY);
	}
	CLR_SCL;
 }


/**********************************************************************
functionName:uint8_t i2c_recv_byte(void)
description:I2C接受字节
**********************************************************************/
uint8_t i2c_recv_byte(void)
{
	uint8_t i=8;
	uint8_t ddata=0; 
	
	i2c_slave();	 
	while (i--)
	{
		ddata<<=1;
		CLR_SCL;
		bsp_DelayUS(I2C_DELAY);
		SET_SCL;
		bsp_DelayUS(I2C_DELAY);
		if(STU_I2C_SDA)
		ddata|=0x01;
	}
	CLR_SCL;
	i2c_mastar(); 
	return ddata;
}

/**********************************************************************
functionName:uint8_t i2c_wait_ack(void) 
description:I2C等待应答
**********************************************************************/
uint8_t i2c_wait_ack(void) 
{
	uint16_t errtime=2555;//因故障接收方无ACK，超时值为255。 
	i2c_slave();	
	bsp_DelayUS(I2C_DELAY);
	SET_SCL;
	bsp_DelayUS(I2C_DELAY);
	while(STU_I2C_SDA)
	{
		errtime--;
		if (!errtime)
		{
			i2_stop();
			return 0x00;
		}
	}
	CLR_SCL;
	i2c_mastar();
	return 0xff;
}

/**********************************************************************
functionName:void i2c_send_ack(void)
description:发送应答
**********************************************************************/
void i2c_send_ack(void)
 {
	CLR_SDA;
	bsp_DelayUS(I2C_DELAY);
	SET_SCL;
	bsp_DelayUS(I2C_DELAY);
	CLR_SCL;
 }

/**********************************************************************
functionName:  void i2c_send_notack(void)
description:发送非应答
**********************************************************************/
void i2c_send_notack(void) 
{
	SET_SDA;
	bsp_DelayUS(I2C_DELAY);
	SET_SCL;
	bsp_DelayUS(I2C_DELAY);
	CLR_SCL;
}
 

 
/**********************************************************************
functionName:void pcf8563_get(uint8_t start,uint8_t num,uint8_t *buf)
description:获取数据
**********************************************************************/
void pcf8563_get(uint8_t start,uint8_t num,uint8_t *buf) 
{
	uint8_t i; 
	i2_start();
	i2c_sendbyte(0xA2);
	i2c_wait_ack();
	i2c_sendbyte(start);
	i2c_wait_ack(); 
	i2_start();
	i2c_sendbyte(0xA3);
	i2c_wait_ack(); 
	for (i=0;i<num;i++)
	{
		buf[i]=i2c_recv_byte();
		if (i!=(num-1)) i2c_send_ack();
	}
	i2c_send_notack(); 
	i2_stop();
}

/**********************************************************************
functionName:void pcf8563_set(uint8_t start,uint8_t num,uint8_t *buf) 
description:设置数据
**********************************************************************/
void pcf8563_set(uint8_t start,uint8_t num,uint8_t *buf) 
{
	 uint8_t i;
	 i2_start();
	 i2c_sendbyte(0xA2);
	 i2c_wait_ack();
	 i2c_sendbyte(start);
	 i2c_wait_ack();
	 for(i=0;i<num;i++)
	 {
	 	i2c_sendbyte(buf[i]);
	 	i2c_wait_ack();	
	 }
	 i2_stop();
}

/**********************************************************************
functionName:void bsp_InitPcf8563(void)
description:初始化
**********************************************************************/
void bsp_InitPcf8563(void)
{
	uint8_t t[4];
	i2c_init();
	t[0]=0;
	t[1]=0;
	pcf8563_set(0,2,t);
	t[0]=0x80;
	t[1]=0x80;
	t[2]=0x80;
	t[3]=0x80;
	pcf8563_set(0x09,4,t);
}

/**********************************************************************
functionName:void time_get(tTime *time)
description:更新时分秒
**********************************************************************/
void time_get(tTime *time)
{
	uint8_t t[3];
	uint8_t temp;
	pcf8563_get(0x02,3,t); 
	temp=t[0]&0x7F;
	time->ucSec=temp&0x0F;
	time->ucSec+=(temp>>4)*10;
	temp=t[1]&0x7F;
	time->ucMin=temp&0x0F;
	time->ucMin+=(temp>>4)*10;
	temp=t[2]&0x3F;
	time->ucHour=temp&0x0F;
	time->ucHour+=(temp>>4)*10;
}
 
/**********************************************************************
functionName:void date_get(tTime *time)
description:获取年月日星期
**********************************************************************/
void date_get(tTime *time)
{
	uint8_t t[4];
	uint8_t temp;
	pcf8563_get(0x05,4,t); 
	temp=t[0]&0x3F;
	time->ucMday=temp&0x0F;
	time->ucMday+=(temp>>4)*10; 
	time->ucWday=t[1]&0x07;	
	temp=t[2]&0x1F;
	time->ucMon=temp&0x0F;
	time->ucMon+=(temp>>4)*10;

	time->usYear=t[3]&0x0F;
	time->usYear+=(t[3]>>4)*10;
	time->usYear+=2000;
	
}
 
/**********************************************************************
functionName:void pcf8563_reg_set(tTime *time)
description:设置时分秒，年月日星期
**********************************************************************/
void pcf8563_time_set(tTime *time)
{
	uint8_t t[7];
	uint8_t sw,gw;
	uint16_t year;
	if(time->ucSec>=60)
	time->ucSec=0;	
	sw=time->ucSec/10;
	gw=time->ucSec%10;
	t[0]=(sw<<4)+gw;
	
	if(time->ucMin>=60)
	time->ucMin=0;
	sw=time->ucMin/10;
	gw=time->ucMin%10;
	t[1]=(sw<<4)+gw;
 
	if(time->ucHour>=24)
	time->ucHour=0;	 
	sw=time->ucHour/10;
	gw=time->ucHour%10;
	t[2]=(sw<<4)+gw;
	
	if(time->ucMday>=32)
	time->ucMday=0;	 
	sw=time->ucMday/10;
	gw=time->ucMday%10;
	t[3]=(sw<<4)+gw;
	
	if(time->ucWday>=7)
	time->ucWday=0;	  
	t[4]=time->ucWday; 
	
	if(time->ucMon>=13)
	time->ucMon=0;	 
	sw=time->ucMon/10;
	gw=time->ucMon%10;
	t[5]=(sw<<4)+gw;
	
	if(time->usYear<2000)
	time->usYear=2012;	
	if(time->usYear>2100)
	time->usYear=2012;	 
	year=(time->usYear)%100; 
	sw=year/10;
	gw=year%10;
	t[6]=(sw<<4)+gw;
	pcf8563_set(0x02,7,t); 
}





  

/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE************************************/  
