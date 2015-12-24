/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGE_MANAGE_H
#define __PAGE_MANAGE_H

/* Includes ------------------------------------------------------------------*/



/* Exported define -----------------------------------------------------------*/
#define MAX_PAGE_NUM					40

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	SYS_SETUP = 0,					//ϵͳ��ʼ 	 	[0]
	MAIN_PAGE,						//��ҳ			[1]
	TEST_PARAMETER_PAGE,			//�������ҳ		[2]
	TEST_REPORT_PAGE,				//���鱨��ҳ		[3]
	SEARCH_REPORT_PAGE,				//��������ҳ		[4]
	DETAIL_REPORT_PAGE,				//��ϸ����ҳ		[5]
	FORCE_CALIBRATION_PAGE,			//��ֵУ׼ҳ		[6]
	CALIBRATION_PARAMETER,			//У׼����		[7]
	SYSTEM_SET,						//ϵͳ����ҳ		[8]
	CONTROL_PARAMETER,				//���Ʋ���		[9]
	CALIBRATION_TABLE,				//�궨��ҳ		[10]
	MORE_PARAMETER,					//�������ҳ		[11]
	DATE_TIME_PAGE,					//����ʱ��		[12]
	SOFT_ACTIVE_PAGE,				//�������		[13]
	ONLINE_PARAMETER_PAGE,			//��������		[14]
	SYSTEM_PASSWORD_PAGE,			//ϵͳ����		[15]
	SYSTEM_INFOMATION_PAGE,			//ϵͳ��Ϣ		[16]
	SYSTEM_BACKUP_PAGE,				//ϵͳ����		[17]
	SYSTEM_RESTORE_PAGE,			//ϵͳ��ԭ		[18]
	UNIT_SELECT_PAGE,				//��λѡ��		[19]
	MODEL_SELECT_PAGE,				//����ѡ��		[20]
	USER_LOGIN_PAGE,				//�û���¼		[21]
	HARD_TEST_PAGE,					//Ӳ������		[22]
	TEST_AFTER_DISPOSE_PAGE,		//�������		[23]
	MANAGE_MODE_PAGE,				//����Աģʽҳ	[24]
	OTHER_PARAMETER_PAGE,			//��������		[25]
	LINK_PAGE,						//����ҳ��		[26]
	POS_ADJUST_PAGE,				//λ�õ���		[27]
	CHANGE_REPORT,					//�޸ı���		[28]
	TEST_SELECT,					//����ѡ��		[29]
	FORCE_VERIFICATION_PAGE,		//��ֵ�춨		[30]
	ADVANCE_PARAMETER_SET_PAGE,		//�߼���������	[31]
	CHANNEL_SELECT_PAGE,			//ͨ��ѡ��		[32]
	TEST_TYPE_SELECT_PAGE,			//��������ѡ��	[33]
}PAGE_NAME_TypeDef;



/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void InitPage( void );
void SetPage( PAGE_NAME_TypeDef setPage );
PAGE_NAME_TypeDef GetPageName( void );
pFunction GetPage( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
