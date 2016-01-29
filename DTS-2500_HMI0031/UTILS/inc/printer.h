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
ErrorStatus PrintCalibrationTable( SMPL_NAME_TypeDef tureChannel );
ErrorStatus PrintTestReport( TEST_TYPE_TypeDef test_type, TEST_ATTRIBUTE_TypeDef testAttribute, \
				const REPORT_TypeDef *report, const TEST_INFO_TypeDef *test_info );
ErrorStatus PrintForceCalibrationResultTable( CALIBRATION_RSULT_PRINT_TypeDef *pCalibrationResult );							 

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
