/****************************************Copyright (c)**************************************************
**                                 �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: prm_struct.h
** ����޸�����: 2011-1-24 15:07:25
** ��        ��: 
** ��	     ��: V1.0
** ��  ��  о Ƭ:STM32F103 	  ����Ƶ��:8MHZ 
** IDE:MDK3.50
**********************************************************************************************************/
#ifndef __PRM_STRUCT_H
#define	__PRM_STRUCT_H

 
#pragma pack(1)

//�豸����
typedef struct
{
	uint8_t  device_addr;				//�豸��ַ
	uint32_t device_baud;				//������ 
	uint8_t  send_freq;					//����Ƶ��
	uint8_t  devc_sn[16];				//�豸��ţ�ͨ��������û������
	uint8_t  devc_token;				//�豸��ʶ,��ʾ���ֻ���
	uint32_t device_ip_addr;			//�豸IP��ַ
	uint8_t  mac_addr[6];				//�豸mac��ַ
}PRM_DEVC_TypeDef;


  
//���Ʋ���
typedef struct 
{
	uint32_t ctrl_max;					//��������
	uint16_t ctrl_max_cnt;				//�����������������������������������������������ֵ�������������������ֵ=0��û�д�������ܡ�	
	uint16_t ctrl_max_one;				//������������
	uint16_t ctrl_min_one;				//������С������
	uint32_t ctrl_open;					//����������
	uint32_t ctrl_back;					//������
	uint8_t  exc_dir;					//����ʱ���������з������ϻ�������		 
}
PRM_CTRL_TypeDef;

 
//��������ֵ
typedef struct 
{
	uint32_t fast_speed;
	uint32_t slow_speed;
}
PRM_SPCL_SPEED_TypeDef;


#ifdef	EDC_3000	
//�������[EDC-3000]
typedef struct 			
{
	PRM_SPCL_SPEED_TypeDef hand_speed[2]; 
	uint16_t protect_bit;				//Ĭ�ϱ���ʹ��
}
PRM_SPCL_TypeDef;
#else
//�������[����EHC-2300,EDC-2300]
typedef struct			
{
	uint16_t protect_bit;				//Ĭ�ϱ���ʹ��
	uint8_t  rsv1[20-sizeof(uint16_t)];	
	uint32_t ky_unload_delay;
	uint32_t ky_valve_init;
	uint32_t ky_valve_back;
	uint32_t ky_valve_middle;
	uint32_t ky_valve_qup;			//����
	uint32_t ky_valve_qdown;		//����
	uint32_t ky_valve_sup;			//����
	uint32_t ky_valve_sdown;		//����
	uint8_t  rsv2[40-sizeof(uint32_t)*8];	
	uint32_t kz_unload_delay;
	uint32_t kz_valve_init;
	uint32_t kz_valve_back;
	uint32_t kz_valve_middle;
	uint32_t kz_valve_qup;	  		//����
	uint32_t kz_valve_qdown;	  	//����
	uint32_t kz_valve_sup;	  		//����
	uint32_t kz_valve_sdown;	  	//����
}
PRM_SPCL_TypeDef;
#endif
  
 
typedef struct
{
	uint16_t limit_pass;		//�Ƿ�����ʹ��0x1111��ʾ����ʹ��
	uint16_t use_time_max;		//���ʹ�ô����������д���
	uint16_t time_use;			//��ǰʹ�ô��� 
	uint16_t active_year;		//������ 0x2011
	uint16_t active_date;		//�������� 0x0624
	uint16_t product_idh;		//��Ʒ��Ÿ�4λ  BCD��
	uint16_t product_idl;		//��Ʒ��ŵ�4λ	 BCD��	
	uint16_t expire_day;		//��������
	uint16_t pre_use_year;		//��һ��ʹ�����
	uint16_t pre_use_date;		//��һ��ʹ������
}DEVICE_INFO_TypeDef;

    
//�ڲ�����
typedef struct 	 
{
	uint32_t dvc_pwd;	 									
	uint8_t  dvc_info[32];								
}
PRM_INSD_TypeDef;

 
//***************************************************** 
//���ڿ��Ʋ��ֵĴ���������
typedef struct  
{
	uint8_t  smpl_type[2];					//����			2
	float smpl_entry;						//�������		4
	float smpl_full_p;						//����������		4
	float smpl_full_n;						//����������		4
	float cof_keep_ahead;					//������ǰϵ��	4
	float cof_keep_spd;						//�����ٶ�ϵ��	4
	float cof_keep_brk;						//�����ƶ�ϵ��	4
	float crash_threshold;					//Ԥ��ײ��ȫֵ	4 
	float overspeed;						//����������ֵ	4
}PRM_SMPL_CTRL_TypeDef;
		
//PID�ṹ��
typedef  struct 	 	
{
	float pid_node;				//PID�ڵ�
	uint16_t pid_kp;			//KPֵ
	uint16_t pid_ki;			//KIֵ
	uint16_t pid_kd;			//KDֵ
}SMPL_PID_TypeDef;

//����pid���ֵĴ���������
typedef struct 			 
{
	uint8_t  smpl_pid_segs;						//PID�ֶ���
	SMPL_PID_TypeDef pid_str[5];				//PID����
}PRM_SMPL_PID_TypeDef;
		 
//�����ṹ��
typedef struct 	 
{
	float value;						//�궨ֵ
	int32_t code;						//�궨��
}
TABLE_PT_TypeDef;
		
//���ڱ��겿�ֵĴ���������
typedef struct 						 
{
	int32_t smpl_zero;										//���	
	uint8_t  table_num;										//�궨������
	TABLE_PT_TypeDef table_pt[16];							//�궨��
}PRM_SMPL_TAB_TypeDef;
	
//*****************������ͨ���Ĵ�ṹ��,һ����250�ֽ�һ��ͨ��
typedef struct 		 
{
	PRM_SMPL_CTRL_TypeDef prm_smpl_ctrl;
	uint8_t  rsv_1[50-sizeof(PRM_SMPL_CTRL_TypeDef)];				//��50�ֽڵĿ��Ʋ��ֵĴ���������
	
	PRM_SMPL_PID_TypeDef  prm_smpl_pid;
	uint8_t  rsv_2[60-sizeof(PRM_SMPL_PID_TypeDef)];		 
	
	PRM_SMPL_TAB_TypeDef prm_smpl_tab;
	uint8_t  rsv_3[140-sizeof(PRM_SMPL_TAB_TypeDef)];	
}PRM_SMPL_TypeDef;


//ϵͳ����[1������]		
typedef struct 
{
	PRM_DEVC_TypeDef prm_devc;						//�豸����
	uint8_t  rsv_1[100-sizeof(PRM_DEVC_TypeDef)];
	PRM_CTRL_TypeDef prm_ctrl;						//���Ʋ���
	uint8_t  rsv_2[100-sizeof(PRM_CTRL_TypeDef)];
	PRM_SPCL_TypeDef prm_spcl;						//�������
	uint8_t  rsv_3[100-sizeof(PRM_SPCL_TypeDef)]; 
	PRM_INSD_TypeDef prm_insd;						//�ڲ�����	
	uint8_t  rsv_4[100-sizeof(PRM_INSD_TypeDef)];
	PRM_SMPL_TypeDef  prm_smpl[SMPL_NUM];			//ͨ������				
}SYS_PRM_TypeDef;   /* ��1900�ֽ� */
  
  
  
//��Щ����Ҫͨ�����Թ����޸ģ�Ҫ������Ȩ���ʲ����޸�,��Ȩ����
typedef struct
{
	uint16_t	instrct_dire;				//ָ���ָ��Ϳ������Ĺ�ϵ
	uint16_t 	sensor_valid;				//���ô�����
	uint16_t 	sensor_used;				//���ô�����
	uint16_t  	gain[6];	 				//����	
	uint32_t 	electronic_gear;			//���ӳ���ֵ�������תһȦ����������� 
	uint32_t  	devc_key;					//�豸Կ��
	uint8_t 	rsv[100-26];
}SYS_PRV_TypeDef;	/* ��100�ֽ� */
  
  
#pragma pack()  

  
#endif
