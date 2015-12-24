/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_PARAMETER_H
#define __CONTROL_PARAMETER_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	ATTENUATION_RATE = 0,			//衰减率
	WITH_MAX_FORCE_DIFFERENCE,		//与最大力差值
	ADJOIN_TWO_POINT_DIFFERENCE,	//相邻两点差值
}JUDGE_BREAK_TYPE_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadControlParameterPage( void );
SMPL_NAME_TypeDef2 GetControlParameterShowChannel( void );
SMPL_NAME_TypeDef2 GetControlParameterTureChannel( void );
JUDGE_BREAK_TYPE_TypeDef GetJudgeBreakType( SMPL_NAME_TypeDef2 channel );
uint8_t GetTargetAttenuationRate( SMPL_NAME_TypeDef2 channel );
float GetTargetWithMaxForceDifference( SMPL_NAME_TypeDef2 channel );
float GetTargetBreakDownPoint( SMPL_NAME_TypeDef2 channel );
float GetTargetAdjoinTwoPointDiff( SMPL_NAME_TypeDef2 channel );
float GetTargetBreakStartValue( SMPL_NAME_TypeDef2 channel );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
