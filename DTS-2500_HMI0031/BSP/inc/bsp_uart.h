/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: bsp_uart.h
** 最后修改日期: 2015/6/23
** 描        述: 
** 版	     本: V1.1
** 主  控  芯 片:  晶振频率: 
** IDE: 
**********************************************************************************************************/
#ifndef __BSP_UART_H
#define	__BSP_UART_H



//配置哪些串口需要使用，使用的配置为1  
#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	0
#define	UART3_FIFO_EN	0
#define	UART4_FIFO_EN	0
#define	UART5_FIFO_EN	0
#define	UART6_FIFO_EN	1 

//#define	UART3_USE_RS485_EN	1	//串口3配置为RS485
 
/* 定义端口号 */
typedef enum
{
	COM1 = 0,		/* USART1  PA9, PA10 或  PB6, PB7 */
	COM2 = 1,		/* USART2, PA2, PA3或 PD5,PD6	  */
	COM3 = 2,		/* USART3, PB10, PB11 			  */
	COM4 = 3,		/* UART4, PC10, PC11 			  */
	COM5 = 4,		/* UART5, PC12, PD2 			  */
	COM6 = 5		/* USART6, PC6, PC7 STM32F107无此串口*/
}COM_PORT_E;

 
/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			38400L
	#define UART1_TX_BUF_SIZE	1*300
	#define UART1_RX_BUF_SIZE	1*300
#endif
 
#if UART2_FIFO_EN == 1
	#define UART2_BAUD			115200L
	#define UART2_TX_BUF_SIZE	1*1024
	#define UART2_RX_BUF_SIZE	1*1024
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			115200L		
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200L
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200L
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			19200L
	#define UART6_TX_BUF_SIZE	1*500
	#define UART6_RX_BUF_SIZE	1*100
#endif
  
//用户实际使用只需要这三个函数
void bsp_InitCOM(void);														//BSP串口初始化
uint8_t ComSend(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);		//COM口发送函数 
uint8_t ComGet(COM_PORT_E _ucPort, uint8_t *_pByte);						//COM口接收函数
 

#endif




	

	

	












