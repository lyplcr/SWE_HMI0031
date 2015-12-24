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
	uint32_t sumNum;			//�������Ľ������
	uint32_t sumPage;			//��ҳ��
	uint32_t searchNum[SUPPORT_TEST_NUM];			//���ÿ�������������Ľ������
	uint8_t recordTestType[SEARCH_ONE_PAGE_REPORT_NUM];	//��¼��ʾʱ����������
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
