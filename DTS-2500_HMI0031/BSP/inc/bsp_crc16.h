/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_CRC16_H
#define __BSP_CRC16_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t GetModBus_CRC16(uint8_t *puchMsg, uint16_t usDataLen);
uint16_t get_crc16(uint16_t crc_init_val,uint8_t *p_data, uint16_t len);

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
