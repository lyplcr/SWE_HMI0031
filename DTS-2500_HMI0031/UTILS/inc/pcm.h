/****************************************Copyright (c)**************************************************
**                                 �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: PCM.H
** ����޸�����: 2014-6-22 15:29:02
** ��        ��: ������д����
** ��	     ��: V1.1
** ��  ��  о Ƭ:STM32F103VE 	   
** IDE: MDK4.12
**********************************************************************************************************/
#ifndef __PCM_H
#define	__PCM_H	

/* Includes------------------------------------------------------------------*/


/* �궨��---------------------------------------------------------------------*/
#define		TEST_NUM		15   		//������������
#define		STEP_NUM		10   		//�ɱ༭������

/* ������������---------------------------------------------------------------*/
#pragma pack(1)

typedef struct
{
	float 		start_force[CTRL_CHN];						//��ʼ��ֵ						   
	uint16_t	run_wait_time[CTRL_CHN];					//���еȴ�ʱ��
	float		break_judge_value[CTRL_CHN];				//����������ֵ
	uint8_t  	break_point[CTRL_CHN];						//�ж��½�����
	uint8_t  	break_condition[CTRL_CHN];					//�����ж�����
	float   	break_max_value[CTRL_CHN];					//���������ֵ
	float   	break_border_value[CTRL_CHN];				//���������ֵ
	uint8_t  	test_standard_index;						//�����׼����	 
 	uint8_t 	load_mode[CTRL_CHN];						//���ط�ʽ
 	uint8_t  	calibration_speed_fast_index[CTRL_CHN];		//����У׼����
 	uint8_t 	calibration_speed_slow_index[CTRL_CHN];		//����У׼����
 	float 		calibration_speed_fast[CTRL_CHN];			//�춨���ټ����ٶ�
 	float   	calibration_speed_slow[CTRL_CHN];			//�춨���ټ����ٶ�
 	uint8_t 	calibration_load_mode[CTRL_CHN];			//�춨���ط�ʽ
 	uint8_t  	print_mode;									//��ӡ���淽ʽ	
 	uint8_t		is_ask_endgroup;							//ѯ�ʡ�����һ�顱
 	uint8_t		is_ask_repeal;								//ѯ�ʡ�������һ�顱	 
 	uint8_t  	shortcut_delay_index;						//��ݲ˵�ͣ��ʱ������
 	uint8_t  	shortcut_delay;								//��ݲ˵�ͣ��ʱ��
 	uint8_t  	lcd_light_use;								//ʹ��ʱ��Ļ��������
 	uint8_t  	lcd_light_idle;								//����ʱ��Ļ��������
 	uint8_t  	lcd_idle_time_index;						//������Ļ����ʱ������
 	uint8_t  	lcd_idle_time;								//������Ļ����ʱ��
 	uint16_t	manual_fast_open[CTRL_CHN];					//�ֶ����ƿ��ٿ���
 	uint16_t	manual_fast_step[CTRL_CHN];					//�ֶ����ƿ��ٲ���
 	uint16_t	manual_slow_open[CTRL_CHN];					//�ֶ��������ٿ���
 	uint16_t	manual_slow_step[CTRL_CHN];					//�ֶ��������ٲ���
 	uint8_t  	step_num;									//��ǰ���Ʋ�������
 	char 		paseword_system[11];						//ϵͳ����
	uint8_t		factory_debug;								//���ҵ���ģʽ����
	uint8_t		fhUnit;										//���ɵ�λ
	uint8_t		redo_test;									//��������
	uint8_t		check_flash;								//���FLASH�Ƿ�δ����ʼ��;�����ñ���һ��Ҫ�ڸýṹ���ĩβ
	uint8_t 	attenuationRate[CTRL_CHN];					//˥����
	uint8_t 	curveStayTime;								//����ͣ��ʱ��
	uint8_t 	wyUnit;										//λ�Ƶ�λ
	uint8_t 	bxUnit;										//���ε�λ
}HMI_TypeDef; 	/* 111Byte */

//���书��    
typedef struct
{
	uint8_t a;
	uint8_t b;	
}MEMORY_TypeDef; 

 //ϵͳ����������־�ṹ��
typedef struct
{
	uint8_t index;		//0~9
	uint32_t num;		//���ݲ�������
	char f_name[10][9];	//10���û������ļ�����	
}SYS_BACKUP_LOG_TypeDef;
 	
		
//������� 	
typedef struct
{
	char 		test_standard[17];				//ִ�б�׼
	char 		test_serial[17];				//�Լ����
	uint8_t 	sample_type_index;				//�Լ�Ʒ������
	char 		custom_sample_type[17];			//�Զ����Լ�Ʒ��	
	uint8_t 	strength_grade_index;			//ǿ�ȵȼ�����
	char 		custom_strength_grade[17];		//�Զ���ǿ�ȵȼ�
	uint8_t 	sample_age_index;				//�Լ���������
	uint16_t	sample_age;						//�Զ����Լ�����
	uint8_t 	sample_spec_index;				//�Լ��������
	char 		custom_sample_spec[17];			//�Զ����Լ����
	uint8_t 	speed_unit_index;				//�ٶȵ�λ����
	uint8_t 	speed_load_index;				//�����ٶ�����
	float 		speed_load;						//�����ٶ�
	uint8_t 	sample_num_index;				//�Լ���������
	uint8_t 	sample_num;						//�Լ�����	
	uint8_t 	is_continue_load;				//�Ƿ���������	
	uint8_t 	delay_time_index;				//��ʱʱ������
	uint16_t 	delay_time;						//��ʱʱ��
	uint8_t 	correct_cof_index;				//����ϵ������	 
	float 		correct_cof;					//����ϵ��	
	uint8_t 	sample_shape_index;				//�Լ���״���� 
	uint8_t 	zfx_length_index;				//�Լ���������
	float 		zfx_length;						//�Լ�����
	uint8_t 	zfx_width_index;				//�Լ��������
	float 		zfx_width;						//�Լ����
	uint8_t		zfx_higth_index;				//�Լ��߶�����	
	float 		zfx_higth;						//�Լ��߶� 
	uint8_t 	zfx_span_index;					//�Լ��������
	float 		zfx_span;						//�Լ����  
	uint8_t 	yx_length_index;				//Բ���Լ���������
	float 		yx_length;						//Բ���Լ����� 
	uint8_t 	yx_higth_index;					//Բ���Լ��߶�����	
	float 		yx_higth;						//Բ���Լ��߶� 
	uint8_t 	yx_span_index;					//Բ���Լ��������
	float 		yx_span;						//Բ���Լ���� 
	uint8_t 	bgz_area_index;					//�������Լ��������
	float 		bgz_area;						//�������Լ����
	uint8_t		train_speed_index;				//Ӧ���ٶ�����
	float		train_speed;					//Ӧ���ٶ�
	float		gz_area;						//�����Լ������	
	char		sample_shape[11];				//�Կ���״
	uint8_t 	yx_diameter_index;				//Բ��ֱ������ֵ
	uint8_t 	yx_diameter;					//Բ��ֱ��
	float		original_cut_area;				//ԭʼ�����
	uint16_t 	extensometer_gauge;				//����Ʊ��
	uint8_t 	rsv[17];
}TEST_TypeDef; 	/* 194Byte */	
    
        
//��λ�������ṹ�� 		    
typedef struct
{ 
	HMI_TypeDef pcm_hmi;						//�˻�������������
	uint8_t rev1[200-sizeof(HMI_TypeDef)];		//��������
	
	TEST_TypeDef pcm_test[TEST_NUM];			//�������
	uint8_t rev2[3000-(TEST_NUM*sizeof(TEST_TypeDef))];	//��������
	
	STEP_TypeDef pcm_step[STEP_NUM];			//�ɱ༭�Ĳ�������
	uint8_t rev3[500-(STEP_NUM*sizeof(STEP_TypeDef))];		//��������
	
	MEMORY_TypeDef memory_func;					//���书�ܲ���
	uint8_t rev4[100-sizeof(MEMORY_TypeDef)];	//��������
}PCM_TypeDef;  		/* ��3800Byte */

#define		PCM_MEM_SIZE		sizeof(PCM_TypeDef)

#pragma pack()

/* ��������-------------------------------------------------------------------*/
void pcm_read(void);							//��flash����ȡ��λ������,����ʱִ�иú�������flash�������ص��ڴ���
void pcm_save(void);							//����ϵͳ������flas,�����е���λ���������浽flash
FRESULT pcm_backup( uint8_t type ) ;			//��λ����������
FRESULT pcm_recover(uint8_t type);				//��λ�������ָ�
uint8_t *get_pcm(void);							//��λ��������ʼ��ַ
HMI_TypeDef *pcm_hmi_get(void);					//��ȡ�˻�������������
TEST_TypeDef *pcm_test_get(uint8_t index);		//��ȡ������ز���
STEP_TypeDef *pcm_step_get(uint8_t index);		//��ȡ����༭����
//���溯�����ݺͻָ���SD�����������ļ�����ʽ�洢
FRESULT PcmBackupWithDate( void );				
FRESULT PcmRecoverWithDate( const char *pFile_name );
ErrorStatus GetBackupLog( SYS_BACKUP_LOG_TypeDef *log );
BoolStatus CheckBackUpFileExist( const char *pFile_name );	//��ⱸ���ļ��Ƿ����



/* ȫ�ֱ�������---------------------------------------------------------------*/   
  

#endif

