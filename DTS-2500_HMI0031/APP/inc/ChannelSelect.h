/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CHANNEL_SELECT_H
#define __CHANNEL_SELECT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PrestrainLoadChannelSelectPage( void );
void SetChannelSelectSourcePage( PAGE_NAME_TypeDef sourcePage );
void SetChannelSelectTargetPage( PAGE_NAME_TypeDef targetPage );
SMPL_NAME_TypeDef GetChannelSelectChannel( void );
void SetChannelSelectNextPage( PAGE_NAME_TypeDef nextPage );
PAGE_NAME_TypeDef GetChannelSelectNextPage( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
