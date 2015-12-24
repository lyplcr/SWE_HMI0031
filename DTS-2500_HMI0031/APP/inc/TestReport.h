/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_REPORT_H
#define __TEST_REPORT_H

/* Includes ------------------------------------------------------------------*/


/* Exported define -----------------------------------------------------------*/
//第几页位置
#define PAGE_DI_POS_X					59	
#define PAGE_DI_POS_Y					380

//共几页位置
#define PAGE_GONG_POS_X					227
#define PAGE_GONG_POS_Y					380

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadTestReportPage( void );
uint8_t GetSelectReportTestType( void );
char *GetSelectReportFileNameAddr( void );
BoolStatus isShowDetailReport( void );
void ResetShowDetailReport( void );
tTime GetTestReportTime( void );
void TestReportIndexIncrease( void );
void TestReportIndexDecrease( void );
void TestReportDeleteReport( void );
uint8_t GetCurPageCurTestReportIndex( void );
uint8_t GetCurPageTestReportNum( void );
ErrorStatus TestReportPrintCursorRowReport( void );
void TestReportExportReport( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
