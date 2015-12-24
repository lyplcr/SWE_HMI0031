/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODEL_SELECT_H
#define __MODEL_SELECT_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	HYDRAULIC_PRESS = 0x00,		//Һѹʽ	
	ELECTRONIC 		= 0x01,		//����ʽ
}MODEL_ATTRIBUTE_TypeDef;

typedef enum
{
	MODEL_KY = 0,		//��ѹ��
	MODEL_KZ,			//���ۻ�
	MODEL_KZKY,			//���ۿ�ѹһ���
	MODEL_UNIVERSAL,	//���ܻ�
}MODEL_TYPE_TypeDef;

typedef enum
{
	MODEL_KY_DZ = 0,		//���ӿ�ѹ��
	MODEL_KZ_DZ,			//���ӿ��ۻ�
	MODEL_KZKY_DZ,			//����ʽ���ۿ�ѹһ���
	MODEL_KY_YY,			//Һѹ��ѹ��
	MODEL_KZKY_YY,			//Һѹʽ���ۿ�ѹһ���
}MODEL_DETAIL_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
MODEL_TYPE_TypeDef GetModelType( void );
void LoadModelSelectPage( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
