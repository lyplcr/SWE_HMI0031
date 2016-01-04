/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: PCM.H
** 最后修改日期: 2014-6-22 15:29:02
** 描        述: 参数读写函数
** 版	     本: V1.1
** 主  控  芯 片:STM32F103VE 	   
** IDE: MDK4.12
**********************************************************************************************************/
#ifndef __PCM_H
#define	__PCM_H	

/* Includes------------------------------------------------------------------*/


/* 宏定义---------------------------------------------------------------------*/
#define		TEST_NUM		15   		//试验种类数量
#define		STEP_NUM		10   		//可编辑步骤数

/* 数据类型声明---------------------------------------------------------------*/
#pragma pack(1)

typedef struct
{
	float 		start_force[CTRL_CHN];						//起始力值						   
	uint16_t	run_wait_time[CTRL_CHN];					//运行等待时间
	float		break_judge_value[CTRL_CHN];				//破碎起判力值
	uint8_t  	break_point[CTRL_CHN];						//判断下降点数
	uint8_t  	break_condition[CTRL_CHN];					//破碎判断条件
	float   	break_max_value[CTRL_CHN];					//与最大力差值
	float   	break_border_value[CTRL_CHN];				//相邻两点差值
	uint8_t  	test_standard_index;						//试验标准索引	 
 	uint8_t 	load_mode[CTRL_CHN];						//加载方式
 	uint8_t  	calibration_speed_fast_index[CTRL_CHN];		//快速校准索引
 	uint8_t 	calibration_speed_slow_index[CTRL_CHN];		//慢速校准索引
 	float 		calibration_speed_fast[CTRL_CHN];			//检定快速加载速度
 	float   	calibration_speed_slow[CTRL_CHN];			//检定慢速加载速度
 	uint8_t 	calibration_load_mode[CTRL_CHN];			//检定加载方式
 	uint8_t  	print_mode;									//打印报告方式	
 	uint8_t		is_ask_endgroup;							//询问“结束一组”
 	uint8_t		is_ask_repeal;								//询问“撤销上一块”	 
 	uint8_t  	shortcut_delay_index;						//快捷菜单停留时间索引
 	uint8_t  	shortcut_delay;								//快捷菜单停留时间
 	uint8_t  	lcd_light_use;								//使用时屏幕亮度索引
 	uint8_t  	lcd_light_idle;								//屏保时屏幕亮度索引
 	uint8_t  	lcd_idle_time_index;						//进入屏幕保护时间索引
 	uint8_t  	lcd_idle_time;								//进入屏幕保护时间
 	uint16_t	manual_fast_open[CTRL_CHN];					//手动控制快速开度
 	uint16_t	manual_fast_step[CTRL_CHN];					//手动控制快速步进
 	uint16_t	manual_slow_open[CTRL_CHN];					//手动控制慢速开度
 	uint16_t	manual_slow_step[CTRL_CHN];					//手动控制慢速步进
 	uint8_t  	step_num;									//当前控制步骤数量
 	char 		paseword_system[11];						//系统密码
	uint8_t		factory_debug;								//厂家调试模式开关
	uint8_t		fhUnit;										//负荷单位
	uint8_t		redo_test;									//重做试验
	uint8_t		check_flash;								//检测FLASH是否未被初始化;声明该变量一定要在该结构体的末尾
	uint8_t 	attenuationRate[CTRL_CHN];					//衰减率
	uint8_t 	curveStayTime;								//曲线停留时间
	uint8_t 	wyUnit;										//位移单位
	uint8_t 	bxUnit;										//变形单位
}HMI_TypeDef; 	/* 111Byte */

//记忆功能    
typedef struct
{
	uint8_t a;
	uint8_t b;	
}MEMORY_TypeDef; 

 //系统参数备份日志结构体
typedef struct
{
	uint8_t index;		//0~9
	uint32_t num;		//备份参数个数
	char f_name[10][9];	//10个用户备份文件夹名	
}SYS_BACKUP_LOG_TypeDef;
 	
		
//试验参数 	
typedef struct
{
	char 		test_standard[17];				//执行标准
	char 		test_serial[17];				//试件编号
	uint8_t 	sample_type_index;				//试件品种索引
	char 		custom_sample_type[17];			//自定义试件品种	
	uint8_t 	strength_grade_index;			//强度等级索引
	char 		custom_strength_grade[17];		//自定义强度等级
	uint8_t 	sample_age_index;				//试件龄期索引
	uint16_t	sample_age;						//自定义试件龄期
	uint8_t 	sample_spec_index;				//试件规格索引
	char 		custom_sample_spec[17];			//自定义试件规格
	uint8_t 	speed_unit_index;				//速度单位索引
	uint8_t 	speed_load_index;				//加载速度索引
	float 		speed_load;						//加载速度
	uint8_t 	sample_num_index;				//试件块数索引
	uint8_t 	sample_num;						//试件块数	
	uint8_t 	is_continue_load;				//是否连续加载	
	uint8_t 	delay_time_index;				//延时时间索引
	uint16_t 	delay_time;						//延时时间
	uint8_t 	correct_cof_index;				//修正系数索引	 
	float 		correct_cof;					//修正系数	
	uint8_t 	sample_shape_index;				//试件形状索引 
	uint8_t 	zfx_length_index;				//试件长度索引
	float 		zfx_length;						//试件长度
	uint8_t 	zfx_width_index;				//试件宽度索引
	float 		zfx_width;						//试件宽度
	uint8_t		zfx_higth_index;				//试件高度索引	
	float 		zfx_higth;						//试件高度 
	uint8_t 	zfx_span_index;					//试件跨距索引
	float 		zfx_span;						//试件跨距  
	uint8_t 	yx_length_index;				//圆形试件长度索引
	float 		yx_length;						//圆形试件长度 
	uint8_t 	yx_higth_index;					//圆形试件高度索引	
	float 		yx_higth;						//圆形试件高度 
	uint8_t 	yx_span_index;					//圆形试件跨距索引
	float 		yx_span;						//圆形试件跨距 
	uint8_t 	bgz_area_index;					//不规则试件面积索引
	float 		bgz_area;						//不规则试件面积
	uint8_t		train_speed_index;				//应力速度索引
	float		train_speed;					//应力速度
	float		gz_area;						//规则试件的面积	
	char		sample_shape[11];				//试块形状
	uint8_t 	yx_diameter_index;				//圆形直径索引值
	uint8_t 	yx_diameter;					//圆形直径
	float		original_cut_area;				//原始截面积
	uint16_t 	extensometer_gauge;				//引伸计标距
	uint8_t 	rsv[17];
}TEST_TypeDef; 	/* 194Byte */	
    
        
//上位机参数结构体 		    
typedef struct
{ 
	HMI_TypeDef pcm_hmi;						//人机界面的杂项参数
	uint8_t rev1[200-sizeof(HMI_TypeDef)];		//保留参数
	
	TEST_TypeDef pcm_test[TEST_NUM];			//试验参数
	uint8_t rev2[3000-(TEST_NUM*sizeof(TEST_TypeDef))];	//保留参数
	
	STEP_TypeDef pcm_step[STEP_NUM];			//可编辑的步骤数量
	uint8_t rev3[500-(STEP_NUM*sizeof(STEP_TypeDef))];		//保留参数
	
	MEMORY_TypeDef memory_func;					//记忆功能参数
	uint8_t rev4[100-sizeof(MEMORY_TypeDef)];	//保留参数
}PCM_TypeDef;  		/* 共3800Byte */

#define		PCM_MEM_SIZE		sizeof(PCM_TypeDef)

#pragma pack()

/* 函数声明-------------------------------------------------------------------*/
void pcm_read(void);							//从flash区读取上位机参数,启动时执行该函数，将flash参数加载到内存中
void pcm_save(void);							//保存系统参数到flas,将所有的上位机参数保存到flash
FRESULT pcm_backup( uint8_t type ) ;			//上位机参数备份
FRESULT pcm_recover(uint8_t type);				//上位机参数恢复
uint8_t *get_pcm(void);							//上位机参数起始地址
HMI_TypeDef *pcm_hmi_get(void);					//获取人机界面的杂项参数
TEST_TypeDef *pcm_test_get(uint8_t index);		//获取试验相关参数
STEP_TypeDef *pcm_step_get(uint8_t index);		//获取步骤编辑参数
//下面函数备份和恢复在SD卡中以日期文件夹形式存储
FRESULT PcmBackupWithDate( void );				
FRESULT PcmRecoverWithDate( const char *pFile_name );
ErrorStatus GetBackupLog( SYS_BACKUP_LOG_TypeDef *log );
BoolStatus CheckBackUpFileExist( const char *pFile_name );	//检测备份文件是否存在



/* 全局变量声明---------------------------------------------------------------*/   
  

#endif

