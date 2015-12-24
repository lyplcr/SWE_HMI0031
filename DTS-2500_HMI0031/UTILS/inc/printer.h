/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PRINTER_H
#define __PRINTER_H

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char * const pPrintErrorCue[];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ErrorStatus PrintCalibrationTable( SMPL_NAME_TypeDef2 tureChannel, SMPL_NAME_TypeDef2 showChannel );
ErrorStatus PrintTestReport( SMPL_NAME_TypeDef2 showChannel, TEST_TYPE_TypeDef test_type, \
							 const REPORT_TypeDef *report, const TEST_INFO_TypeDef *test_info );
ErrorStatus PrintForceCalibrationResultTable( CALIBRATION_RSULT_PRINT_TypeDef *pCalibrationResult );							 

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
