/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: protocol.h
** ����޸�����: 2014/5/5 10:59:03
** ��        ��: ��Э���йصĺ���
** ��	     ��: V1.0
** ��  ��  о Ƭ:  ����Ƶ��: 
** IDE:  MDK 4.12
**********************************************************************************************************/
#ifndef __PROTOCOL_H
#define	__PROTOCOL_H
 


/********************************************************************************

				�� �� ͨ Ѷ �� ʽ
��Ϣ��ʽ
MESSAGE_START
NODE
INDEX		//����
MSG_SIZE 
TOKEN 
BODY 
CHECKL
CHECKH
END
********************************************************************************/
#define		ST_START					0x50	//��Ϣ��ʼ
#define 	ST_GET_ADDR					0x51	//��������2
#define 	ST_GET_INDEX				0x52	//��������1
#define 	ST_MSG_SIZE					0x53	//��Ϣ�ߴ�
#define 	ST_GET_TOKEN				0x54	//�Ǻź˶�
#define 	ST_GET_DATA					0x55	//������
#define		ST_GET_CHECK_L				0x56	//��У��L
#define		ST_GET_CHECK_H				0x57	//��У��H
#define		ST_GET_END					0x58	//�����ֽ�  

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

//UART���Ͱ�β
typedef struct
{
	uint16_t check;
	uint8_t end;
}UART_TAIL_Typdef;    
  
typedef enum
{
	COM_ADD_UDP=0,				//UDPͨѶ��ַΪ0				
	COM_ADD_UART=1,				//���ڵĵ�ַΪ1
}COM_ADD_TypeDef;   
   
  
typedef enum
{
	INDEX_H_ASW=0x00,			//���������������ҪӦ��ĳ�������
	INDEX_HD_NASW, 				//������ӻ���������Ĳ���Ӧ��ĳ�������
	INDEX_D_ASW, 				//�豸�����,����Ӧ���豸�ĳ�������
	INDEX_IVLD=0xFF,			//�Ƿ�����
}INDEX_TypeDef;	  

//UDP���Ͱ�ͷ 
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

//UDP���Ͱ�β
typedef struct 
{
	uint16_t check;
	uint8_t end;
}UDP_TAIL_Typdef;


#pragma pack()

uint8_t next_index(INDEX_TypeDef index_type,COM_ADD_TypeDef add);		//��ȡ��һ��index,����ѡ��UDP���Ǵ��ڵ�
INDEX_TypeDef index_type_get(uint8_t index); 							//��ȡָ����index������
uint8_t index_get(INDEX_TypeDef index_type,COM_ADD_TypeDef add);		//��ȡ��ǰ��index	



#endif




	

	

	












