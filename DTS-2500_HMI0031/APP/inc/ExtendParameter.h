/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXTEND_PARAMETER_H
#define __EXTEND_PARAMETER_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	OBVIOUS_YIELD = 0,		//明显屈服模式
	SIGMA0_2,				//σ0.2模式
	NO_YIELD,				//无屈服
}YIELD_JUDGE_MODE_TypeDef;

typedef enum
{
	NO_LASTIC_MODULUS = 0,	//不计算弹性模量
	COMPUTE_LASTIC_MODULUS,	//计算弹性模量
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
