/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_TYPE_SELECT_H
#define __TEST_TYPE_SELECT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	COMPRESSION_TEST = 0,	/* —πÀı ‘—È */
	BENDING_TEST,			/* Õ‰«˙ ‘—È */
	STRETCH_TEST,			/* ¿≠…Ï ‘—È */
}TEST_TYPE_NAME_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadTestTypeSelectPage( void );
void SetTestTypeSelectSourcePage( PAGE_NAME_TypeDef sourcePage );
void SetTestTypeSelectTargetPage( PAGE_NAME_TypeDef targetPage );
TEST_TYPE_NAME_TypeDef GetSelectTestType( void );


#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
