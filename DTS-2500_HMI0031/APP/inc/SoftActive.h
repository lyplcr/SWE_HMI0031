/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOFT_ACTIVE_H
#define __SOFT_ACTIVE_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadSoftActivePage( void );
EXPIRE_STATUS_TypeDef JudgeActiveStatus( int32_t *day );
void ActiveStatusHandler( EXPIRE_STATUS_TypeDef status );


#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
