/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_TYPE_SELECT_H
#define __TEST_TYPE_SELECT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadTestTypeSelectPage( void );
void SetTestTypeSelectSourcePage( PAGE_NAME_TypeDef sourcePage );
void SetTestTypeSelectTargetPage( PAGE_NAME_TypeDef targetPage );
TEST_ATTRIBUTE_TypeDef GetSelectTestAttribute( void );
TEST_ATTRIBUTE_TypeDef GetTestAttribute( uint8_t testIndex );


#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
