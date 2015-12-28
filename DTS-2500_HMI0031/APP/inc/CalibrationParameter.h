/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALIBRATION_PARAMETER_H
#define __CALIBRATION_PARAMETER_H

/* Includes ------------------------------------------------------------------*/
#define MAX_CALIBRATION_SEGS			10

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t calibrationSegments;
	int32_t calibrationValue[MAX_CALIBRATION_SEGS];		
}CALIBRATION_POINT_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadCalibrationParameterPage( void );
CALIBRATION_POINT_TypeDef *GetCalibrationPoint( void );


#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
