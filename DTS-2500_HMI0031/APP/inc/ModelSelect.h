/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODEL_SELECT_H
#define __MODEL_SELECT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	HYDRAULIC_PRESS = 0x00,		//液压式	
	ELECTRONIC 		= 0x01,		//电子式
}MODEL_ATTRIBUTE_TypeDef;

typedef enum
{
	MODEL_KY = 0,		//抗压机
	MODEL_KZ,			//抗折机
	MODEL_KZKY,			//抗折抗压一体机
	MODEL_UNIVERSAL,	//万能机
}MODEL_TYPE_TypeDef;

typedef enum
{
	MODEL_KY_DZ = 0,		//电子抗压机
	MODEL_KZ_DZ,			//电子抗折机
	MODEL_KZKY_DZ,			//电子式抗折抗压一体机
	MODEL_KY_YY,			//液压抗压机
	MODEL_KZKY_YY,			//液压式抗折抗压一体机
}MODEL_DETAIL_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
MODEL_TYPE_TypeDef GetModelType( void );
void LoadModelSelectPage( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
