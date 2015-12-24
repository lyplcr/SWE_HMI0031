/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: protocol.h
** 最后修改日期: 2014/5/5 10:59:03
** 描        述: 和协议有关的函数
** 版	     本: V1.0
** 主  控  芯 片:  晶振频率: 
** IDE:  MDK 4.12
**********************************************************************************************************/
#ifndef __PROTOCOL_H
#define	__PROTOCOL_H
 


/********************************************************************************

				串 口 通 讯 格 式
信息格式
MESSAGE_START
NODE
INDEX		//索引
MSG_SIZE 
TOKEN 
BODY 
CHECKL
CHECKH
END
********************************************************************************/
#define		ST_START					0x50	//消息开始
#define 	ST_GET_ADDR					0x51	//附加数据2
#define 	ST_GET_INDEX				0x52	//附加数据1
#define 	ST_MSG_SIZE					0x53	//消息尺寸
#define 	ST_GET_TOKEN				0x54	//记号核对
#define 	ST_GET_DATA					0x55	//数据体
#define		ST_GET_CHECK_L				0x56	//和校验L
#define		ST_GET_CHECK_H				0x57	//和校验H
#define		ST_GET_END					0x58	//结束字节  

#pragma pack(1)

typedef struct
{
	uint8_t start;
	uint8_t addr;
	uint8_t index;
	uint8_t size;
	uint8_t token;
	uint8_t data;
}UART_HEAD_Typdef; 

//UART发送包尾
typedef struct
{
	uint16_t check;
	uint8_t end;
}UART_TAIL_Typdef;    
  
typedef enum
{
	COM_ADD_UDP=0,				//UDP通讯地址为0				
	COM_ADD_UART=1,				//串口的地址为1
}COM_ADD_TypeDef;   
   
  
typedef enum
{
	INDEX_H_ASW=0x00,			//主机主动发起的需要应答的常规数据
	INDEX_HD_NASW, 				//主机或从机主动发起的不需应答的常规数据
	INDEX_D_ASW, 				//设备发起的,主机应答设备的常规数据
	INDEX_IVLD=0xFF,			//非法索引
}INDEX_TypeDef;	  

//UDP发送包头 
typedef struct 
{
	uint8_t start;
	uint8_t addr;
	uint8_t index;
	uint8_t rsv1;
	uint16_t size;
	uint8_t rsv2;
	uint8_t token;
	uint8_t data;
}UDP_HEAD_Typdef; 

//UDP发送包尾
typedef struct 
{
	uint16_t check;
	uint8_t end;
}UDP_TAIL_Typdef;


#pragma pack()

uint8_t next_index(INDEX_TypeDef index_type,COM_ADD_TypeDef add);		//获取下一个index,可以选择UDP还是串口的
INDEX_TypeDef index_type_get(uint8_t index); 							//获取指定的index的属性
uint8_t index_get(INDEX_TypeDef index_type,COM_ADD_TypeDef add);		//获取当前的index	



#endif




	

	

	












