/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   Network connection configuration
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "netconf.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "stm32f4x7_eth.h"
#include "prm.h"

  

/* Private variables ---------------------------------------------------------*/
struct netif netif;
__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;
__IO uint32_t LinkTimer = 0;
static BoolStatus EthLinkStatus = NO;

#define Link_TMR_INTERVAL       250

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;
//static uint32_t IPaddress = 0;
#endif 


/* Private function prototypes -----------------------------------------------*/
extern void client_init(void);
extern void server_init(void);

/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : GetEthLinkStatus
 * Description    : 获取以太网连接状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus GetEthLinkStatus( void )
{
	EthLinkStatus = (ETH_ReadPHYRegister(LAN8720_PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status) ? YES : NO;
	
	return EthLinkStatus;
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void bsp_InitEthernet(void)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	uint8_t pucIPAddress[4];
	uint8_t macaddress[6];
	uint32_t ip = 0;
	
	/* MAC层芯片初始化 	*/
	bsp_InitLan8720();
	
	/* Initializes the dynamic memory heap defined by MEM_SIZE.*/
	mem_init();

	/* Initializes the memory pools defined by MEMP_NUM_x.*/
	memp_init();

	#if LWIP_DHCP
	  ipaddr.addr = 0;
	  netmask.addr = 0;
	  gw.addr = 0; 
	#else   
		prm_read();
		ip = devc_ip_get();

		if (ip == 0xffffffff)
		{
			ip = 0xC0A800EA;	//192.168.0.234
		}

		pucIPAddress[0] = (ip >> 24);
		pucIPAddress[1] = (ip >> 16);
		pucIPAddress[2] = (ip >> 8);
		pucIPAddress[3] = ip;

		IP4_ADDR(&ipaddr, pucIPAddress[0], pucIPAddress[1], pucIPAddress[2], pucIPAddress[3]);
		IP4_ADDR(&netmask, 255, 255, 255, 0);
		IP4_ADDR(&gw, pucIPAddress[0], pucIPAddress[1], pucIPAddress[2], 1);
	#endif

  //获取MAC
	GetMACAdress(macaddress);
	
	if ( (macaddress[0] == 0xff) && (macaddress[1] == 0xff) && (macaddress[2] == 0xff) \
			&& (macaddress[3] == 0xff)  && (macaddress[4] == 0xff)  && (macaddress[5] == 0xff) )
	{
		macaddress[0] = 0x00;
		macaddress[0] = 0x80;
		macaddress[0] = 0xE1;
		macaddress[0] = 0x2F;
		macaddress[0] = 0x02;
		macaddress[0] = 0x75;
	}
	Set_MAC_Address(macaddress);

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/  
  netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface.*/
  netif_set_default(&netif);

#if LWIP_DHCP
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.*/
  dhcp_start(&netif);
#endif

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&netif);

  client_init();

}


/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
void LwIP_Pkt_Handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
  ethernetif_input(&netif);
}

/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwIP_Periodic_Handle(__IO uint32_t localtime)
{
   /* Link periodic process every 250 ms */
  if (localtime - LinkTimer >= Link_TMR_INTERVAL)
  {
    LinkTimer =  localtime;
    Link_tmr();
  }

  /* TCP periodic process every 250 ms */
  if (localtime - TCPTimer >= TCP_TMR_INTERVAL)
  {
    TCPTimer =  localtime;
    tcp_tmr(); 
  }
  /* ARP periodic process every 5s */
  if (localtime - ARPTimer >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  localtime;
    etharp_tmr();
  }

#if LWIP_DHCP
  /* Fine DHCP periodic process every 500ms */
  if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  localtime;
    dhcp_fine_tmr();
  }
  /* DHCP Coarse periodic process every 60s */
  if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
  {
    DHCPcoarseTimer =  localtime;
    dhcp_coarse_tmr();
  }

#endif

} 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
