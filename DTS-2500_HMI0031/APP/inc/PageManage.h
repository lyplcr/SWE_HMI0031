/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGE_MANAGE_H
#define __PAGE_MANAGE_H

/* Includes ------------------------------------------------------------------*/



/* Exported define -----------------------------------------------------------*/
#define MAX_PAGE_NUM					40

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	SYS_SETUP = 0,					//系统开始 	 	[0]
	MAIN_PAGE,						//主页			[1]
	TEST_PARAMETER_PAGE,			//试验参数		[2]
	TEST_REPORT_PAGE,				//试验报告		[3]
	SEARCH_REPORT_PAGE,				//搜索报告		[4]
	DETAIL_REPORT_PAGE,				//详细报告		[5]
	FORCE_CALIBRATION_PAGE,			//力值校准		[6]
	CALIBRATION_PARAMETER,			//校准参数		[7]
	SYSTEM_SET,						//系统设置		[8]
	CONTROL_PARAMETER,				//控制参数		[9]
	CALIBRATION_TABLE,				//标定表			[10]
	EXTEND_PARAMETER_PAGE,			//扩展参数		[11]
	DATE_TIME_PAGE,					//日期时间		[12]
	SOFT_ACTIVE_PAGE,				//软件激活		[13]
	ONLINE_PARAMETER_PAGE,			//联机参数		[14]
	SYSTEM_PASSWORD_PAGE,			//系统密码		[15]
	SYSTEM_INFOMATION_PAGE,			//系统信息		[16]
	SYSTEM_BACKUP_PAGE,				//系统备份		[17]
	SYSTEM_RESTORE_PAGE,			//系统还原		[18]
	UNIT_SELECT_PAGE,				//单位选择		[19]
	MODEL_SELECT_PAGE,				//机型选择		[20]
	USER_LOGIN_PAGE,				//用户登录		[21]
	HARD_TEST_PAGE,					//硬件测试		[22]
	TEST_AFTER_DISPOSE_PAGE,		//试验后处理		[23]
	MANAGE_MODE_PAGE,				//管理员模式		[24]
	OTHER_PARAMETER_PAGE,			//其他参数		[25]
	LINK_PAGE,						//联机页面		[26]
	POS_ADJUST_PAGE,				//位置调整		[27]
	CHANGE_REPORT,					//修改报告		[28]
	TEST_SELECT,					//试验选择		[29]
	FORCE_VERIFICATION_PAGE,		//力值检定		[30]
	ADVANCE_PARAMETER_SET_PAGE,		//高级参数设置	[31]
	CHANNEL_SELECT_PAGE,			//通道选择		[32]
	TEST_TYPE_SELECT_PAGE,			//试验类型选择	[33]
	CURVE_SHOW_PAGE,				//曲线显示		[34]
	FILE_MANAGER_PAGE,				//文件管理		[35]
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
