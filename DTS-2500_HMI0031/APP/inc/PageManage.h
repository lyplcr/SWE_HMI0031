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
	TEST_PARAMETER_PAGE,			//�������		[2]
	TEST_REPORT_PAGE,				//���鱨��		[3]
	SEARCH_REPORT_PAGE,				//��������		[4]
	DETAIL_REPORT_PAGE,				//��ϸ����		[5]
	FORCE_CALIBRATION_PAGE,			//��ֵУ׼		[6]
	CALIBRATION_PARAMETER,			//У׼����		[7]
	SYSTEM_SET,						//ϵͳ����		[8]
	CONTROL_PARAMETER,				//���Ʋ���		[9]
	CALIBRATION_TABLE,				//�궨��			[10]
	EXTEND_PARAMETER_PAGE,			//��չ����		[11]
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
	MANAGE_MODE_PAGE,				//����Աģʽ		[24]
	OTHER_PARAMETER_PAGE,			//��������		[25]
	LINK_PAGE,						//����ҳ��		[26]
	POS_ADJUST_PAGE,				//λ�õ���		[27]
	CHANGE_REPORT,					//�޸ı���		[28]
	TEST_SELECT,					//����ѡ��		[29]
	FORCE_VERIFICATION_PAGE,		//��ֵ�춨		[30]
	ADVANCE_PARAMETER_SET_PAGE,		//�߼���������	[31]
	CHANNEL_SELECT_PAGE,			//ͨ��ѡ��		[32]
	TEST_TYPE_SELECT_PAGE,			//��������ѡ��	[33]
	CURVE_SHOW_PAGE,				//������ʾ		[34]
	FILE_MANAGER_PAGE,				//�ļ�����		[35]
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
