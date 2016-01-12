/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_PARAMETER_H
#define __CONTROL_PARAMETER_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	ATTENUATION_RATE = 0,			//˥����
	WITH_MAX_FORCE_DIFFERENCE,		//���������ֵ
	ADJOIN_TWO_POINT_DIFFERENCE,	//���������ֵ
}JUDGE_BREAK_TYPE_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadControlParameterPage( void );
JUDGE_BREAK_TYPE_TypeDef GetJudgeBreakType( SMPL_NAME_TypeDef channel );
uint8_t GetTargetAttenuationRate( SMPL_NAME_TypeDef channel );
float GetTargetWithMaxForceDifference( SMPL_NAME_TypeDef channel );
float GetTargetBreakDownPoint( SMPL_NAME_TypeDef channel );
float GetTargetAdjoinTwoPointDiff( SMPL_NAME_TypeDef channel );
float GetTargetBreakStartValue( SMPL_NAME_TypeDef channel );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
