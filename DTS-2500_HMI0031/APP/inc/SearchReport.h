/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SEARCH_REPORT_H
#define __SEARCH_REPORT_H

/* Includes ------------------------------------------------------------------*/
#define SEARCH_ONE_PAGE_REPORT_NUM					10

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	DATE_SEARCH = 0,
	SERIAL_SEARCH,
}FIND_TYPE_INDEX_TypeDef;

typedef struct
{
	uint32_t sumNum;			//搜索出的结果总数
	uint32_t sumPage;			//总页数
	uint32_t searchNum[SUPPORT_TEST_NUM];			//标记每种试验搜索到的结果总数
	uint8_t recordTestType[SEARCH_ONE_PAGE_REPORT_NUM];	//记录显示时的试验类型
	TEST_INFO_TypeDef testReportData[SEARCH_ONE_PAGE_REPORT_NUM+1];
}TEST_SEARCH_RESULT_TypeDef;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadReportSearchPage( void );
FIND_TYPE_INDEX_TypeDef GetSearchType( void );
void StartOneReportSearch( void );
TEST_SEARCH_RESULT_TypeDef *GetSearchReportResultAddr( void );
BoolStatus isAgainSearchReport( void );
void ResetAgainSearchReport( void );
void GetTestDataFromSD( FunctionalState SearchAll, FIND_TYPE_INDEX_TypeDef searchType, uint32_t offset );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
