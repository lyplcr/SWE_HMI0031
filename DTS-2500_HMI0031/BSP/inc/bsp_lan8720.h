#ifndef __BSP_LAN8720_H
#define __BSP_LAN8720_H


			
#define LAN8720_PHY_ADDRESS  	0x00				//LAN8720 PHY–æ∆¨µÿ÷∑. 

#define LAN8720_CLR     GPIO_ResetBits(GPIOA,GPIO_Pin_0);
#define LAN8720_SET     GPIO_SetBits(GPIOA,GPIO_Pin_0);
 

uint8_t bsp_InitLan8720(void);
void ETHERNET_NVICConfiguration(void);
uint8_t ETH_MACDMA_Config(void);

#endif 

