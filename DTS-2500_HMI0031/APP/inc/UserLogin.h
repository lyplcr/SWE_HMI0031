/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_LOGIN_H
#define __USER_LOGIN_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	USER_PASSWORD = 0,
	MANAGER_PASSWORD,
	USER_OR_MANAGER_PASSWORD,
}PASSWORD_TYPE_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PrestrainLoadPasswordPage( void );
void SetPasswordType( PASSWORD_TYPE_TypeDef passwordType );
void SetPasswordPutinSourcePage( PAGE_NAME_TypeDef sourcePage );
void SetPasswordPutinTargetPage( PAGE_NAME_TypeDef targetPage );
BoolStatus GetPasswordPutinStatus( void );
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
