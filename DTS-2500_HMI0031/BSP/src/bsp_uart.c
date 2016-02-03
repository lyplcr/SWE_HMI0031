/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: bsp_uart.c
** 最后修改日期: 2015/6/24  
** 描        述: 串口bsp驱动程序
** 版	     本: V2.0
** 主  控  芯 片:STM32F407VE
** IDE:MDK4.5 
2015/6/24:
1、驱动程序改编自安富莱电子串口驱动，支持多串口驱动
2、没有把所有的串口驱动写全，因为没法测试，后续只需要修改UartHardInit、UartNVICConfig两个函数即可，需要增加新的串口UARTx_FIFO_EN定义为1即初始化该串口


**********************************************************************************************************/
#include "bsp.h"
 
/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	__IO uint8_t *pTxBuf;			/* 发送缓冲区 */
	__IO uint8_t *pRxBuf;			/* 接收缓冲区 */
	__IO uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	__IO uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;		/* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;			/* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;		/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;		/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;			/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;		/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(void);	/* 串口收到数据的回调函数指针 */
}UART_T;  
 

/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

/* 485总线宏定义 */
#if UART3_USE_RS485_EN == 1
	#define RS485_DIR_GPIO_CLK              RCC_APB2Periph_GPIOB
	#define	RS485_DIR_GPIO_PORT				GPIOB
	#define RS485_DIR_GPIO_PIN				GPIO_Pin_12
	
	#define RS485_TX_EN()					GPIO_SetBits(RS485_DIR_GPIO_PORT,RS485_DIR_GPIO_PIN)
	#define RS485_RX_EN()					GPIO_ResetBits(RS485_DIR_GPIO_PORT,RS485_DIR_GPIO_PIN)
	
	void USART3_SendBeforCallBack( void );
	void USART3_SendOverCallBack( void );
	
#endif

UART_T *ComToUart(COM_PORT_E _ucPort);

/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart1.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart1.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart1.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart1.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart1.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 串口设备 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* 发送缓冲区指针 */
	g_tUart2.pRxBuf = g_RxBuf2;					/* 接收缓冲区指针 */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart2.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart2.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart2.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart2.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart2.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart2.usTxCount = 0;						/* 待发送的数据个数 */	
	g_tUart2.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart2.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart2.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 串口设备 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* 发送缓冲区指针 */
	g_tUart3.pRxBuf = g_RxBuf3;					/* 接收缓冲区指针 */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart3.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart3.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart3.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart3.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart3.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart3.usTxCount = 0;						/* 待发送的数据个数 */	
	g_tUart3.SendBefor = USART3_SendBeforCallBack;/* 发送数据前的回调函数 */
	g_tUart3.SendOver = USART3_SendOverCallBack;/* 发送完毕后的回调函数 */
	g_tUart3.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;						/* STM32 串口设备 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* 发送缓冲区指针 */
	g_tUart4.pRxBuf = g_RxBuf4;					/* 接收缓冲区指针 */
	g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart4.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart4.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart4.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart4.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart4.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart4.usTxCount = 0;						/* 待发送的数据个数 */	
	g_tUart4.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart4.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart4.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;						/* STM32 串口设备 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* 发送缓冲区指针 */
	g_tUart5.pRxBuf = g_RxBuf5;					/* 接收缓冲区指针 */
	g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart5.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart5.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart5.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart5.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart5.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart5.usTxCount = 0;						/* 待发送的数据个数 */	
	g_tUart5.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart5.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart5.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART6_FIFO_EN == 1
	g_tUart6.uart = USART6;						/* STM32 串口设备 */
	g_tUart6.pTxBuf = g_TxBuf6;					/* 发送缓冲区指针 */
	g_tUart6.pRxBuf = g_RxBuf6;					/* 接收缓冲区指针 */
	g_tUart6.usTxBufSize = UART6_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart6.usRxBufSize = UART6_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart6.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart6.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart6.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart6.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart6.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart6.usTxCount = 0;						/* 待发送的数据个数 */	
	g_tUart6.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart6.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart6.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif
}


/*
*********************************************************************************************************
*	函 数 名: UartHardInit
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartHardInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#if UART1_FIFO_EN == 1		/* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/

	USART_DeInit(USART1);
	
	/* 第1步： 配置GPIO */
	#if 1	/* TX = PA9   RX = PA10 */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		
		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
		/* 配置引脚复用映射 */
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 
		
		/* 配置IO */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure); 

	#else	/* TX = PB6   RX = PB7  */
		/* 打开 GPIO 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		 
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);		//选用USART1的Remap引脚

		/* Configure USART1 Tx (PB.06) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		/* Configure USART1 Rx (PB.07) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */ 
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */ 
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
	{
		;
	}
	USART_ClearFlag(USART1, USART_FLAG_TC);
#endif

#if UART6_FIFO_EN == 1		/* 串口1 TX = PC6   RX = PC7 或 */

	USART_DeInit(USART6);
	
	/* 第1步： 配置GPIO */
	#if 1	/* 串口1 TX = PC6   RX = PC7 */
		/* 打开 GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		
		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
		/* 配置引脚复用映射 */
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); 
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); 
		
		/* 配置IO */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure); 

	#else	/* TX = PB6   RX = PB7  */
		/* 打开 GPIO 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		 
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);		//选用USART1的Remap引脚

		/* Configure USART1 Tx (PB.06) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		/* Configure USART1 Rx (PB.07) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART6_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	  
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */ 
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART6, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART6, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */ 
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET)
	{
		;
	}
	USART_ClearFlag(USART6, USART_FLAG_TC);
#endif	
	
}
  
/*
*********************************************************************************************************
*	函 数 名: UartNVICConfig
*	功能说明: 配置串口硬件中断.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartNVICConfig(void)  
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	#if UART1_FIFO_EN == 1		 
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
	
	#if UART2_FIFO_EN == 1		 
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
	
	#if UART3_FIFO_EN == 1		 
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
	 
	#if UART4_FIFO_EN == 1		 
	/* 使能串口4中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif 
	
	#if UART6_FIFO_EN == 1		 
	/* 使能串口6中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif 
} 
  		
/*
*********************************************************************************************************
*	函 数 名: bsp_COMInit
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  	无
*	返 回 值: 	无
*********************************************************************************************************
*/		
void bsp_InitCOM(void)
{
	UartVarInit();
	UartHardInit();
	UartNVICConfig();
}				

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#if UART4_FIFO_EN == 1
			return &g_tUart4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#if UART5_FIFO_EN == 1
			return &g_tUart5;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM6)
	{
		#if UART6_FIFO_EN == 1
			return &g_tUart6;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}  

 
///*
//*********************************************************************************************************
//*	函 数 名: UartIsTxBuffFull
//*	功能说明: 判断发送缓存是否已满
//*	形    参:  	_pUart : 串口设备
//*	返 回 值: 	1表示发送缓存已满，0表示发送缓存未满
//*********************************************************************************************************
//*/	
//uint8_t UartIsTxBuffFull(UART_T *_pUart)
//{ 
//	uint16_t ucCount; 
//	USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);
//	ucCount=_pUart->usTxCount;
//	USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
//	
//	if(ucCount==_pUart->usTxBufSize)
//	return 1;
//	else
//	return 0;		
//}
//
//
///*
//*********************************************************************************************************
//*	函 数 名: UartIsRxBuffFull
//*	功能说明: 判断接收缓存是否已满
//*	形    参:  	_pUart : 串口设备
//*	返 回 值: 	1表示接收缓存已满，0表示接收缓存未满
//*********************************************************************************************************
//*/	
//uint8_t UartIsRxBuffFull(UART_T *_pUart)
//{
//	uint16_t ucCount; 
//	USART_ITConfig(_pUart->uart, USART_IT_RXNE, DISABLE);
//	ucCount=_pUart->usRxCount;
//	USART_ITConfig(_pUart->uart, USART_IT_RXNE, ENABLE);	
//	if(ucCount>=_pUart->usRxBufSize)
//	{
//		return 1;
//	}	  
//	else
//	return 0;	
//}

/*
*********************************************************************************************************
*	函 数 名: UartPutChar
*	功能说明: 通过Uart口发送一个字节数据，数据先写入缓存，然后再决定是否发送
*	形    参:  	COM_PORT_E : COM号，_ucDat：发送的数据。
*	返 回 值: 	1表示发送缓存已满，0表示发送缓存未满
*********************************************************************************************************
*/
static uint8_t UartPutChar(UART_T *_pUart, uint8_t _ucDat)
{
	USART_ITConfig(_pUart->uart, USART_IT_TC,DISABLE);				//禁止串口发送结束中断
	//发送缓冲满，后面发送的数据忽略掉
	if(_pUart->usTxCount==_pUart->usTxBufSize)
	{
		USART_ITConfig(_pUart->uart, USART_IT_TC,ENABLE);			//打开串口发送结束中断  
		return 1;
	}	
	
	_pUart->pTxBuf[_pUart->usTxWrite]=_ucDat;		//向发送缓存中写数据
	_pUart->usTxWrite++;
	if(_pUart->usTxWrite>=_pUart->usTxBufSize)
	_pUart->usTxWrite=0;	
	  
	_pUart->usTxCount++; 
	if(_pUart->usTxCount==1)
	{
		if(!(_pUart->usTxWrite))
		{
			_pUart->usTxRead=_pUart->usTxBufSize-1;
		}	
		else
		_pUart->usTxRead=_pUart->usTxWrite-1;
		
		USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);		   
		_pUart->usTxRead=_pUart->usTxWrite;
	}   
	
	USART_ITConfig(_pUart->uart, USART_IT_TC,ENABLE);			//打开串口发送结束中断   
	return 0;
}

 


/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{  
	USART_ITConfig(_pUart->uart, USART_IT_RXNE, DISABLE);  
	if(_pUart->usRxCount)		//表示接收缓冲中有数据
	{ 
		_pUart->usRxCount--;
		
		*_pByte=_pUart->pRxBuf[_pUart->usRxRead];
		 
		_pUart->usRxRead++;
		if(_pUart->usRxRead>=_pUart->usRxBufSize)
		_pUart->usRxRead=0;	
		  
		USART_ITConfig(_pUart->uart, USART_IT_RXNE, ENABLE);
		return 1;
	}	
	else
	{ 
		USART_ITConfig(_pUart->uart, USART_IT_RXNE, ENABLE);
		return 0;
	} 
}

///*
//*********************************************************************************************************
//*	函 数 名: ComClearRxFifo
//*	功能说明: 清零串口接收缓冲区
//*	形    参: _ucPort: 端口号(COM1 - COM6)
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void ComClearRxFifo(COM_PORT_E _ucPort)
//{
//	UART_T *pUart; 
//	pUart = ComToUart(_ucPort);
//	if (pUart == 0)
//	{
//		return;
//	} 
//	pUart->usRxWrite = 0;
//	pUart->usRxRead = 0;
//	pUart->usRxCount = 0;
//}
//
//
///*
//*********************************************************************************************************
//*	函 数 名: ComClearTxFifo
//*	功能说明: 清零串口发送缓冲区
//*	形    参: _ucPort: 端口号(COM1 - COM6)
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void ComClearTxFifo(COM_PORT_E _ucPort)
//{
//	UART_T *pUart;
//	pUart = ComToUart(_ucPort);
//	if (pUart == 0)
//	{
//		return;
//	} 
//	pUart->usTxWrite = 0;
//	pUart->usTxRead = 0;
//	pUart->usTxCount = 0;
//}


/*
*********************************************************************************************************
*	函 数 名: ComSend
*	功能说明: 向COM口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参:  	COM_PORT_E : COM号，_ucaBuf：发送的数据指针，_usLen发送数据长度
*	返 回 值: 	1表示发送缓存已满，0表示发送缓存未满,返回0xFF表示不存在的串口
*********************************************************************************************************
*/	
uint8_t ComSend(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;
	uint8_t _ucFull;
	uint16_t i;
	
	pUart=ComToUart(_ucPort);
	if (pUart == 0)
	{		
		return 0xFF;
	}

	for(i=0;i<_usLen;i++)
	{
		_ucFull=UartPutChar(pUart,_ucaBuf[i]);
		if(_ucFull)
		{
			return 1;
		}
	} 
		
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t ComGet(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;
	uint8_t Exist;
	pUart=ComToUart(_ucPort);
	Exist=UartGetChar(pUart,_pByte);
	return Exist;
}


/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{	
	if(USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)	//发送接收中断
	{
		//在缓冲已满的状态下，忽略新收到的数据
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
			/* 从串口接收数据寄存器读取数据存放到接收FIFO */
			_pUart->pRxBuf[_pUart->usRxWrite]=USART_ReceiveData(_pUart->uart);
			_pUart->usRxWrite++;
			if(_pUart->usRxWrite>=_pUart->usRxBufSize)
			_pUart->usRxWrite=0; 
		} 
		else
		{
			USART_ReceiveData(_pUart->uart);
		}	
	}	 
	if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)	//发送完成中断
	{
		_pUart->usTxCount--;
		if(_pUart->usTxCount)		//如果还有发送的数据
		{
			USART_SendData(_pUart->uart,_pUart->pTxBuf[_pUart->usTxRead]);     
			_pUart->usTxRead++;
			if(_pUart->usTxRead>=_pUart->usTxBufSize)
			_pUart->usTxRead=0; 	
		}	
		else
		{
			USART_ClearFlag(_pUart->uart,USART_FLAG_TC);			 //清除发送完成标志
		}
	} 
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}
#endif

#if UART4_FIFO_EN == 1
void UART4_IRQHandler(void)
{
	UartIRQ(&g_tUart4);
}
#endif

#if UART5_FIFO_EN == 1
void UART5_IRQHandler(void)
{
	UartIRQ(&g_tUart5);
}
#endif

#if UART6_FIFO_EN == 1
void USART6_IRQHandler(void)
{
	UartIRQ(&g_tUart6);
}
#endif

/*------------------------------------------------------------
 * Function Name  : USART3_SendBeforCallBack
 * Description    : 串口发送前回调函数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USART3_SendBeforCallBack( void )
{
	#ifdef	UART3_USE_RS485_EN
		RS485_TX_EN();
	#endif
}

/*------------------------------------------------------------
 * Function Name  : USART3_SendOverCallBack
 * Description    : 串口发送完成回调函数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USART3_SendOverCallBack( void )
{
	#ifdef	UART3_USE_RS485_EN
		RS485_RX_EN();
	#endif
}

/*------------------------------------------------------------
 * Function Name  : GetReceiveBuffUnreadPercentage
 * Description    : 获取接收缓存未读取数据百分比
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetReceiveBuffUnreadPercentage( COM_PORT_E _ucPort )
{
	UART_T *pUart = ComToUart(_ucPort);
	
	return (1.0 * pUart->usRxCount / pUart->usRxBufSize) * 100;
}

/*------------------------------------------------------------
 * Function Name  : GetSendBuffUnwritePercentage
 * Description    : 获取发送缓存未写入数据百分比
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetSendBuffUnwritePercentage( COM_PORT_E _ucPort )
{
	UART_T *pUart = ComToUart(_ucPort);
	
	return (1.0 * pUart->usTxCount / pUart->usTxBufSize) * 100;
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
		ComSend(COM6,(uint8_t *)&ch,1);
		return ch;
	#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
		/* 写一个字节到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		/* 等待发送结束 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
			
		}
		return ch;
	#endif
}

    
  
/******************* (C) COPYRIGHT 2015 XinGao Tech ********END OF FILE***************************/  
