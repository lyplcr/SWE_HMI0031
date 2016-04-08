/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXTEND_PARAMETER_H
#define __EXTEND_PARAMETER_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	OBVIOUS_YIELD = 0,		//��������ģʽ
	SIGMA0_2,				//��0.2ģʽ
	NO_YIELD,				//������
}YIELD_JUDGE_MODE_TypeDef;

typedef enum
{
	NO_LASTIC_MODULUS = 0,	//�����㵯��ģ��
	COMPUTE_LASTIC_MODULUS,	//���㵯��ģ��
}COMPUTE_LASTIC_MODULUS_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadExtendParameterPage( void );
YIELD_JUDGE_MODE_TypeDef GetYieldJudgeMode( void );
COMPUTE_LASTIC_MODULUS_TypeDef GetComputeLasticModulusMode( void );
uint32_t GetYieldDisturbThreshold( void );
float GetElasticModulusStartStrength( void );
float GetElasticModulusEndStrength( void );	

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
