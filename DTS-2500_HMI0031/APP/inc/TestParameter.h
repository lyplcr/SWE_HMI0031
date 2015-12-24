/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TESTPARAMETER_H
#define __TESTPARAMETER_H

/* Includes ------------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MAX_TEST_INFOMATION_PARAMETER_NUM 		10

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	SERIAL_NO_CHG = 0,		//��Ų���Ҫ�ı�
	CHANGE_SERIAL_OK,		//�ı��ųɹ�		
	SERIAL_ILLEGAL,			//���Ϸ��ַ�							
	SERIAL_REACH_MAX_VALUE,	//���������ﵽ���ֵ
}SERIAL_ADD_ONE_STA;

typedef struct
{
	uint8_t testInfomationNum;
	uint8_t testHandle[MAX_TEST_INFOMATION_PARAMETER_NUM];
	char *pParameterContent[MAX_TEST_INFOMATION_PARAMETER_NUM];
}TEST_INFOMATION_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoadTestParameterPage( void );
SERIAL_ADD_ONE_STA TestParameterCheckSerialRepeat( TEST_TYPE_TypeDef type, char *pStr, uint8_t *pNum );
void LoadMainPageGetTestInfomationPage( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
