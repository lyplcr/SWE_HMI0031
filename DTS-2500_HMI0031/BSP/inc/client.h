/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLIENT_H
#define __CLIENT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UDP_SendStr(void *str_p, uint16_t len);
void * GetEthernetTxDataAddr( void );
void * GetEthernetRxDataAddr( void );
uint16_t UDP_GetDataLen( void );
BoolStatus UDP_IsFindUntreatedData( void );
void UDP_SetUntreatedDataStatus( BoolStatus NewStatus );
extern void SetBindingPort( void );
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
