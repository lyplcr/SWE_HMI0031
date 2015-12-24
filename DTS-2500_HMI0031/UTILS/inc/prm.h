/****************************************Copyright (c)**************************************************
**                                 �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: PRM.H
** ����޸�����: 2014/5/5 18:05:38
** ��        ��: ������д����
** ��	     ��: V1.1
** ��  ��  о Ƭ:STM32F103   
** IDE:MDK4.12
**********************************************************************************************************/
#ifndef __PRM_H
#define	__PRM_H	
 
 		   
#define		PRM_MEM_SIZE			(sizeof(SYS_PRM_TypeDef))				//�����洢���ֽ���	
#define		PRV_MEM_SIZE			(sizeof(SYS_PRV_TypeDef))				//��Ȩ�洢���ֽ���	
#define		PRM_MEM_SIZE_2CH		(400+2*sizeof(PRM_SMPL_TypeDef))		//����+2��ͨ���Ĳ���(���ڿ��ۿ�ѹ��ֻ��2��ͨ������)

#pragma pack(1)

//��Ȩ�����йصĺ���  
typedef enum
{
	PGA_64X=0x6464,				//64���Ŵ���	
	PGA_32X=0x3232,				//32���Ŵ���
}GAIN_TypeDef;  				//ADC��������  				 
typedef enum
{
	INSTRCT_DIRE_P=0x5050, 		//ָ���Ϳ�����������ͬ	
	INSTRCT_DIRE_N=0x4E4E, 		//ָ���Ϳ����������෴
}INSTRCT_DIRE_TypeDef; 			//ָ�������

#pragma pack()

uint8_t *get_prm(void);										//����ϵͳ����ָ��
uint8_t *get_prv(void);										//������Ȩ����ָ��
uint32_t devc_ip_get(void);									//��ȡ�豸IP����
uint8_t *devc_mac_get(void);								//��ȡ�豸mac��ַָ��

uint8_t devc_token_get(void);								//��ȡ�豸��ʶ,��ʾ���ֻ���
void devc_token_set(uint8_t token);							//��������ʶ
 
 
//���Ʋ�����ȡ����
uint32_t ctrl_max_get(void);								//��ȡ������������󿪶ȣ�
uint16_t ctrl_max_cnt_get(void);							//��ȡ��������������������
uint16_t ctrl_max_one_get(void);							//��ȡ������������
uint16_t ctrl_min_one_get(void);							//��ȡ������С������
uint32_t ctrl_open_get(void);								//��ȡ���������� 
uint8_t ctrl_exc_dir_get(void);								//��ȡ����ʱ���������з������ϻ�������
 
//��������
#define	PROTECT_SMPL_KY			0x0001						//ͨ��0���ֵ����
#define	PROTECT_SMPL_KZ			0x0002						//ͨ��1���ֵ����
#define	PROTECT_SMPL_2			0x0004						//ͨ��2���ֵ����
#define	PROTECT_SMPL_3			0x0008						//ͨ��3���ֵ����
#define	PROTECT_SMPL_4			0x0010						//ͨ��4���ֵ����
#define	PROTECT_SMPL_5			0x0020						//ͨ��5���ֵ����
#define	PROTECT_LIMIT			0x0040						//��λ����
#define	PROTECT_CRASH_KY		0x0080						//��ѹԤ��ײ��ȫ����
#define	PROTECT_CRASH_KZ		0x0100						//����Ԥ��ײ��ȫ����
#define	PROTECT_UNEXP			0x0200						//��Ԥ�ڱ���
#define	PROTECT_SPEED_KY		0x0400						//��ѹ���ٱ���
#define	PROTECT_SPEED_KZ		0x0800						//���۹��ٱ���  
#define	PROTECT_DEFAULT			(PROTECT_SMPL_KY|PROTECT_SMPL_KZ|PROTECT_CRASH_KY|PROTECT_CRASH_KZ|PROTECT_SPEED_KY|PROTECT_SPEED_KZ)


uint16_t spcl_protect_get(void);							//��ȡ����λ��־
//EDC-2300��EHC-2300���������  
uint32_t spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch);		//��ȡж����ʱʱ��
uint32_t spcl_valve_init_get(SMPL_NAME_TypeDef2 ch);		//��ȡָ��ͨ���ķ��ڳ�ʼλ��
uint32_t spcl_valve_back_get(SMPL_NAME_TypeDef2 ch);	 	//��ȡָ��ͨ���ķ��ڻ��˿���
uint32_t spcl_valve_middle_get(SMPL_NAME_TypeDef2 ch);		//��ȡָ��ͨ���ķ�����λ
uint32_t spcl_quick_up_get(SMPL_NAME_TypeDef2 ch);	 		//��ȡָ��ͨ���Ŀ��Ͽ���
uint32_t spcl_quick_down_get(SMPL_NAME_TypeDef2 ch); 		//��ȡָ��ͨ���Ŀ��¿���
uint32_t spcl_slow_up_get(SMPL_NAME_TypeDef2 ch);  			//��ȡָ��ͨ�������Ͽ���
uint32_t spcl_slow_down_get(SMPL_NAME_TypeDef2 ch); 	 	//��ȡָ��ͨ�������¿���  
  
//���ڴ����������ĺ���  
float smpl_ctrl_entry_get(uint8_t ch);	 					//��ȡָ��ͨ���Ŀ������	
float smpl_ctrl_full_p_get(uint8_t ch); 		  			//��ȡָ��ͨ��������������	 
float smpl_ctrl_full_n_get(uint8_t ch);   					//��ȡָ��ͨ���ķ���������
float smpl_cof_ahead_get(uint8_t ch);						//��ȡָ��ͨ������ǰ����ϵ�� 
float smpl_cof_spd_get(uint8_t ch);							//��ȡָ��ͨ�����ٶȱ���ϵ��
float smpl_cof_break_get(uint8_t ch);						//��ȡָ��ͨ�����ƶ�����ϵ��
float smpl_ctrl_crash_get(uint8_t ch);						//��ȡָ��ͨ����Ԥ��ײ��ȫֵ
float smpl_ctrl_overspeed_get(uint8_t ch); 					//���ش��������Ʋ������ٱ���ֵ  
float smpl_pid_node_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ����pid ָ����Ľڵ��ֵ
uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ����pid ָ�����kpֵ
uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ����pid ָ�����kiֵ
uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ����pid ָ�����kdֵ
uint8_t smpl_pid_segs_get(uint8_t ch);						//��ȡָ��ͨ����pid����
uint8_t smpl_tab_num_get(uint8_t ch);						//��ȡָ��ͨ���ı궨������
float smpl_tab_value_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ���ı궨�� ָ�����ֵ
int32_t smpl_tab_code_get(uint8_t ch,uint8_t num);			//��ȡָ��ͨ���ı궨�� ָ�������
int32_t smpl_tab_zero_get(uint8_t ch);						//��ȡָ��ͨ�������ֵ   
float smpl_cal(int32_t adc_code,uint8_t chn);				//���㴫����ֵ  
  
//�������ݺͻָ�
void prm_read(void);										//��flash����ȡϵͳ����
void prv_read(void);										//��flash����ȡ��Ȩ����
void prm_save(void);										//����ϵͳ������flash
void prv_save(void);										//������Ȩ������flash
FRESULT prm_backup(uint8_t type); 							//ϵͳ��������
FRESULT prv_backup(uint8_t type);							//��Ȩ��������
FRESULT prm_recover(uint8_t type); 							//ϵͳ�����ָ�,����������Ϣ���� 
FRESULT prv_recover(uint8_t type);							//��Ȩ�����ָ�  
void prm_write(uint8_t *dat,uint16_t addr,uint16_t len);	//д�����ڴ������
void prv_write(uint8_t *dat,uint16_t addr,uint16_t len);	//д��Ȩ�����ڴ������ 
//���溯�����ݺͻָ���SD�����������ļ�����ʽ�洢
FRESULT PrmBackupWithDate(void);
FRESULT PrvBackupWithDate(void);
FRESULT PrmRecoverWithDate( const char *pFile_name );
FRESULT PrvRecoverWithDate( const char *pFile_name );

 
void devc_ip_set(uint32_t ip);										//�����豸IP����,ֻ�ı��ڴ�Ĳ��������ı�flash������
void devc_mac_set(uint8_t *mac);									//�����豸mac��ַָ��
void ctrl_max_set(uint32_t arg);									//������������ 
void ctrl_max_cnt_set(uint16_t arg);								//�������������������� 
void ctrl_max_one_set(uint32_t arg);								//���õ����������� 
void ctrl_min_one_set(uint16_t arg);								//���õ�����С������ 
void ctrl_open_set(uint32_t arg);									//���ÿ��������� 
void ctrl_exc_dir_set(uint8_t arg);									//���ÿ��������� 
void spcl_protect_set(uint16_t arg);								//���ÿ��������� 
void spcl_unload_delay_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ж����ʱʱ��
void spcl_valve_init_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ָ��ͨ���ķ��ڳ�ʼλ�� 
void spcl_valve_back_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ָ��ͨ���ķ��ڻ��˿���	
void spcl_valve_middle_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ָ��ͨ���ķ�����λ
void spcl_quick_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);			//����ָ��ͨ���Ŀ��Ͽ���
void spcl_quick_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ָ��ͨ���Ŀ��¿���
void spcl_slow_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);			//����ָ��ͨ�������Ͽ���
void spcl_slow_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//����ָ��ͨ���Ŀ��¿���
void smpl_ctrl_entry_set(uint8_t ch,float entry);					//����ָ��ͨ���Ŀ������	
void smpl_ctrl_full_p_set(uint8_t ch,float full_p);					//����ָ��ͨ��������������
void smpl_ctrl_full_n_set(uint8_t ch,float full_n);					//����ָ��ͨ���ķ���������
void smpl_cof_ahead_set(uint8_t ch,float ahead);					//���ñ�����ǰϵ��
void smpl_cof_spd_set(uint8_t ch,float keep_spd);					//����ϵ������
void smpl_cof_break_set(uint8_t ch,float keep_brk);					//�����ƶ�ϵ��
void smpl_ctrl_crash_set(uint8_t ch,float crash);					//����ָ��ͨ����Ԥ��ײ��ȫֵ
void smpl_ctrl_overspeed_set(uint8_t ch,float speed);				//���ô��������Ʋ������ٱ���ֵ
void smpl_pid_segs_set(uint8_t ch,uint8_t segs);					//����ָ��ͨ����pid����
void smpl_pid_node_set(uint8_t ch,uint8_t num,float node);			//����ָ��ͨ����pid ָ����Ľڵ�ֵ
void smpl_pid_kp_set(uint8_t ch,uint8_t num,uint16_t kp);			//����ָ��ͨ����pid ָ�����kpֵ
void smpl_pid_ki_set(uint8_t ch,uint8_t num,uint16_t ki);			//����ָ��ͨ����pid ָ�����kiֵ
void smpl_pid_kd_set(uint8_t ch,uint8_t num,uint16_t kd);			//����ָ��ͨ����pid ָ�����kdֵ
void smpl_tab_value_set(uint8_t ch,uint8_t num,float value);		//����ָ��ͨ���ı궨�� ָ�����ֵ
void smpl_tab_code_set(uint8_t ch,uint8_t num,int32_t code);		//����ָ��ͨ���ı궨�� ָ�������
void smpl_tab_zero_set(uint8_t ch,int32_t code);					//����ָ��ͨ�������ֵ
void smpl_tab_num_set(uint8_t ch,uint8_t table);					//����ָ��ͨ���ı궨������
//void prv_ins_dir_set(INSTRCT_DIRE_TypeDef dire);					//����ָ���
void prv_gain_set(uint8_t ch,uint16_t gain);						//����ָ��ͨ��������
void prv_ele_gear_set(uint32_t eg);									//���õ��ӳ���
void prv_devc_key_set(uint32_t key);								//�����豸Կ��

 
uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len);				//��ȡ�豸������ʼ��ַ�ͳ���
uint8_t *prm_ctrl_info(uint32_t *addr,uint16_t *len);				//��ȡ���Ʋ�����ʼ��ַ�ͳ���
uint8_t *prm_spcl_info(uint32_t *addr,uint16_t *len);				//��ȡ���������ʼ��ַ�ͳ���
uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len);				//��ȡ�ڲ�������ʼ��ַ�ͳ���
uint8_t *prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch);	//��ȡ������������ʼ��ַ�ͳ���(������������PID�������궨��)




#endif








