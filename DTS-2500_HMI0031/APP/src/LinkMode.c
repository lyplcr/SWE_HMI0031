/**
  ******************************************************************************
  * @file    LinkMode.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-27 16:18:46
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   ����ģʽ
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "LinkMode.h"
#include "netconf.h"
#include "client.h"
#include "cmd.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define USART_BUFF_SIZE					512

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}LINK_MODE_TypeDef;

/* Private constants ---------------------------------------------------------*/
const char * const pLinkModeCue[] = 
{
	"�ɹ�����Խ������ӣ���[�˳�]",	//0
	"�뿪����ģʽ��",				//1
	"��̫�����ӶϿ����������ߣ�",	//2
	"�ɹ�������̫����",				//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static LINK_MODE_TypeDef g_linkMode;
static uint8_t usart_tx_package[USART_BUFF_SIZE];
static uint8_t usart_rx_package[USART_BUFF_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void LinkModeInit( void );
static void LinkModeConfig( void );
static void GUI_LinkMode( void );
static void LinkModeShortcutCycleTask( void );
static void LinkModeKeyProcess( void );
static void LinkModeLeavePageCheckCycle( void );
static void LinkHandlerConvCore( void );

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : LoadLinkModePage
 * Description    : ����ģʽҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadLinkModePage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	LinkModeInit();
	
	/* �������� */
	LinkModeConfig();
	
	/* ��GUI��� */
	GUI_LinkMode();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_linkMode.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* ��ݲ˵� */
		LinkModeShortcutCycleTask();
		
		/* �������� */
		LinkModeKeyProcess();
		
		/* ת�����ĳ��� */
		LinkHandlerConvCore();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		LinkModeLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SystemSetInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkModeInit( void )
{	
	g_linkMode.leavePage.flagLeavePage = RESET;
	g_linkMode.leavePage.flagSaveData = RESET;	
	g_linkMode.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : LinkParameterConfig
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkModeConfig( void )
{
	/* ���� */
	g_linkMode.pTitle = "����ģʽ";
}

/*------------------------------------------------------------
 * Function Name  : GUI_LinkModeCue
 * Description    : ����GUI��ʾ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_LinkModeCue( void )
{
	SetPopWindowsWithoutOperate(POP_PCM_CUE_WITHOUT_OPERATE,"����״̬",2,&pLinkModeCue[0]);
	
	PopWindowsProcessCycle();
}

/*------------------------------------------------------------
 * Function Name  : GUI_LinkParameter
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_LinkMode( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_linkMode.pTitle);
	
	GUI_LinkModeCue();
}

/*------------------------------------------------------------
 * Function Name  : LinkModeShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkModeShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_linkMode.refreshShortcut == ENABLE)
	{
		g_linkMode.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[3] = pTwoLevelMenu[67];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : LinkModeKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkModeKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_F4:
				g_linkMode.leavePage.flagLeavePage = SET;
				g_linkMode.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_linkMode.leavePage.flagLeavePage = SET;
				g_linkMode.leavePage.flagSaveData = SET;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : LinkModeLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkModeLeavePageCheckCycle( void )
{
	if (g_linkMode.leavePage.flagLeavePage == SET)
	{
		if (g_linkMode.leavePage.flagSaveData == SET)
		{
			SystemSoftwareReset();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : UDP_Check
 * Description    : UDP���ݰ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus UDP_Check( UDP_HEAD_Typdef *pHead )
{
	uint16_t crc_cal = 0;
	UDP_TAIL_Typdef *UdpTail = NULL;
	
	crc_cal = get_crc16(0,(uint8_t *)pHead,sizeof(UDP_HEAD_Typdef) - 1 + pHead->size);
	
	UdpTail = (UDP_TAIL_Typdef *)((uint8_t *)pHead + sizeof(UDP_HEAD_Typdef) - 1 \
									+ pHead->size);
	
	if (UdpTail->check == crc_cal)
	{
		return PASSED;
	}
	
	return FAILED;
}
	
/*------------------------------------------------------------
 * Function Name  : UDP_GetCmd
 * Description    : ��ȡUDP����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint16_t UDP_GetCmd( UDP_HEAD_Typdef *p )
{
	uint16_t cmd = 0;
	uint8_t *dat = &p->data;
	
	cmd = ((dat[1] << 8) | dat[0]);
	
	return cmd;
}

/*------------------------------------------------------------
 * Function Name  : CheckLinkStatus
 * Description    : �������״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus CheckLinkStatus( void )
{
	UDP_HEAD_Typdef *pHead = NULL;
	uint16_t cmd = 0;
	
	if (UDP_IsFindUntreatedData() == YES)
	{
		UDP_SetUntreatedDataStatus(NO);
		
		pHead = GetEthernetRxDataAddr();
		
		if (UDP_Check(pHead) == FAILED)
		{
			#ifdef DEBUG_LINK_MODE
				printf("UDP->UART��CRCУ��ʧ�ܣ�\r\n");
			#endif
			return NO;
		}
		
		cmd = UDP_GetCmd(pHead);
		
		if ((cmd == CMD_SIGN_ON) || (cmd == CMD_DEVICE_VERSION))
		{
			#ifdef DEBUG_LINK_MODE
				printf("UDP->UART��������֤�ɹ���\r\n");
			#endif
			
			SetBindingPort();
			
			return YES;
		}
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : UDPConvUSARTFormate
 * Description    : UDP->USART
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UDPConvUSARTFormate( void *pHead )
{
	UDP_HEAD_Typdef *pUdpHead 	 = (UDP_HEAD_Typdef *)pHead;
	UART_HEAD_Typdef *pUsartHead = (UART_HEAD_Typdef *)usart_tx_package;
	UDP_TAIL_Typdef *UdpTail 	 = NULL;
	UART_TAIL_Typdef *UsartTail  = NULL;
	
	pUsartHead->start = pUdpHead->start;
	pUsartHead->addr  = pUdpHead->addr;
	pUsartHead->index = pUdpHead->index;
	pUsartHead->size  = pUdpHead->size;
	pUsartHead->token = pUdpHead->token;
	memcpy(&pUsartHead->data,&pUdpHead->data,pUdpHead->size);
	
	UsartTail  	= (UART_TAIL_Typdef *)((uint8_t *)pUsartHead + sizeof(UART_HEAD_Typdef) - 1\
									+pUsartHead->size);
	UdpTail 	= (UDP_TAIL_Typdef *)((uint8_t *)pUdpHead + sizeof(UDP_HEAD_Typdef) - 1 \
									+ pUdpHead->size);
	
	UsartTail->check = get_crc16(0,(uint8_t *)pUsartHead,sizeof(UART_HEAD_Typdef) - 1\
								 + pUsartHead->size);
	UsartTail->end = UdpTail->end;
}

/*------------------------------------------------------------
 * Function Name  : GetUdpPackageSize
 * Description    : ��ȡUDP���ݰ���С
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint16_t GetUdpPackageSize( UDP_HEAD_Typdef *pHead )
{
	return (sizeof(UDP_HEAD_Typdef) - 1 + pHead->size + sizeof(UDP_TAIL_Typdef));
}

/*------------------------------------------------------------
 * Function Name  : GetUSARTPackageSize
 * Description    : ��ȡUSART���ݰ���С
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetUSARTPackageSize( UART_HEAD_Typdef *pHead )
{
	return (sizeof(UART_HEAD_Typdef) - 1 + pHead->size + sizeof(UART_TAIL_Typdef));
}			

/*------------------------------------------------------------
 * Function Name  : USART_SendPackage
 * Description    : ���ڷ������ݰ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void USART_SendPackage( void )
{
	UART_HEAD_Typdef *pHead = (UART_HEAD_Typdef *)usart_tx_package;
	
	ComSend(COM1,(uint8_t *)pHead,GetUSARTPackageSize(pHead));
}

/*------------------------------------------------------------
 * Function Name  : USARTConvUDPFormate
 * Description    : USART->UDP
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void USARTConvUDPFormate( void *pHead )
{
	UDP_HEAD_Typdef *pUdpHead 	 = (UDP_HEAD_Typdef *)pHead;
	UART_HEAD_Typdef *pUsartHead = (UART_HEAD_Typdef *)usart_rx_package;
	UDP_TAIL_Typdef *UdpTail 	 = NULL;
	UART_TAIL_Typdef *UsartTail  = NULL;
	
	pUdpHead->start   = pUsartHead->start;
	pUdpHead->addr	  = 0x00;//pUsartHead->addr;
	pUdpHead->index	  = pUsartHead->index;
	pUdpHead->rsv1	  = 0x00;
	pUdpHead->size	  = pUsartHead->size;
	pUdpHead->rsv2	  = 0x00;
	pUdpHead->token	  = pUsartHead->token;
	
	memcpy(&pUdpHead->data,&pUsartHead->data,pUsartHead->size);
	
	UsartTail  	= (UART_TAIL_Typdef *)((uint8_t *)pUsartHead + sizeof(UART_HEAD_Typdef) - 1\
									+pUsartHead->size);
	UdpTail 	= (UDP_TAIL_Typdef *)((uint8_t *)pUdpHead + sizeof(UDP_HEAD_Typdef) - 1 \
									+ pUdpHead->size);
	
	UdpTail->check = get_crc16(0,(uint8_t *)pUdpHead,sizeof(UDP_HEAD_Typdef) - 1\
								 + pUdpHead->size);
	UdpTail->end = UsartTail->end;
}

/*------------------------------------------------------------
 * Function Name  : UDP_GetPackageCycle
 * Description    : ��ȡUDP���ݰ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void UDP_GetPackageCycle( void )
{
	uint8_t *pHead = NULL;
	uint16_t pkg_len = 0;
	uint16_t sum = 0;
	
	if (UDP_IsFindUntreatedData() == YES)
	{		
		pHead = GetEthernetRxDataAddr();
		
		while (1)
		{
			if (UDP_Check((UDP_HEAD_Typdef *)pHead) == FAILED)
			{
				#ifdef DEBUG_LINK_MODE
					printf("UDP->UART��CRCУ��ʧ�ܣ�\r\n");
				#endif
				break;
			}
			
			if (CMD_SIGN_OFF == UDP_GetCmd((UDP_HEAD_Typdef *)pHead))
			{
				g_linkMode.leavePage.flagLeavePage = SET;
				g_linkMode.leavePage.flagSaveData = SET;
				#ifdef DEBUG_LINK_MODE
					printf("UDP->UART���˳�����ģʽ��\r\n");
				#endif
				break;
			}
			
			UDPConvUSARTFormate(pHead);
			USART_SendPackage();
			
			#ifdef DEBUG_LINK_MODE
				printf("UDP->UART�����ʹ��ڰ���\r\n");
			#endif
			
			pkg_len = GetUdpPackageSize((UDP_HEAD_Typdef *)pHead);
			
			sum += pkg_len;
			if (sum == UDP_GetDataLen())
			{	
				#ifdef DEBUG_LINK_MODE
					printf("UDP->UART��������ϣ�\r\n");
				#endif
				break;
			}
			pHead += pkg_len;
		}
		
		UDP_SetUntreatedDataStatus(NO);
	}
}


/*------------------------------------------------------------
 * Function Name  : USART_GetPackageCycle
 * Description    : ��ȡUSART���ݰ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void USART_GetPackageCycle( void )
{
	uint8_t *pHead = NULL;
	uint16_t msg_size = 0;
	uint8_t *start_addr = NULL;
	uint16_t rxCmd = 0;
	
	while ( Uart_GetPackageCycle() == YES )
	{
		pHead = GetEthernetTxDataAddr();
		
		rxCmd = uart_get_cmd();
		
		/* 
			��ȡ��λ���ɶ�д���λ��
			�����ڼ�ͨ����ť�����ͱ���ͣ��
		*/
		if ( (rxCmd==RTM_SAMPLE_VALUE) || (rxCmd==RTM_SAMPLE_CODE) || (rxCmd==RTM_SAMPLE_RCODE) )	
		{	
			sample_cycle();
		}

			
		if (ERROR == uart_check() )	
		{
			#ifdef DEBUG_LINK_MODE 
				printf("UDP<-UART��CRCУ��ʧ�ܣ�\r\n");
			#endif
			continue;
		}
		
		start_addr = uart_get_frame_addr();
		msg_size = GetUSARTPackageSize((UART_HEAD_Typdef *)start_addr);
		
		memcpy(usart_rx_package,start_addr,msg_size);
		
 		USARTConvUDPFormate(pHead);
 		
 		UDP_SendStr(pHead,GetUdpPackageSize((UDP_HEAD_Typdef *)pHead));
		#ifdef DEBUG_LINK_MODE
			printf("UDP<-UART������UDP����\r\n");
		#endif
	}
}

/*------------------------------------------------------------
 * Function Name  : NetworkDisConnectProcess
 * Description    : ����δ���Ӵ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void NetworkDisConnectProcess( void )
{
	static BoolStatus isConnect = YES;
	
	switch ( isConnect )
	{
		case NO:
			if (GetEthLinkStatus() == YES)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pLinkModeCue[3]);
				isConnect = YES;
			}	
			else
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pLinkModeCue[2]);
			}
			g_linkMode.leavePage.flagLeavePage = SET;
			g_linkMode.leavePage.flagSaveData = RESET;	
			break;
		case YES:
			if (GetEthLinkStatus() == NO)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pLinkModeCue[2]);
				g_linkMode.leavePage.flagLeavePage = SET;
				g_linkMode.leavePage.flagSaveData = RESET;
				isConnect = NO;
			}
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : LinkHandlerConvCore
 * Description    : �������Ĵ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkHandlerConvCore( void )
{		
	/* ���� -> ���� */
	UDP_GetPackageCycle();
	
	/* ���� -> ���� */
	USART_GetPackageCycle();
	
	/* ���߼�� */
	NetworkDisConnectProcess();		
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
