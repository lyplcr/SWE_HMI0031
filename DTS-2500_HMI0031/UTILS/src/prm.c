/**
  ******************************************************************************
  * @file    prm.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ��λ������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */ 
#include "bsp.h"
#include "prm_struct.h"
#include "prm.h"





//#define		USE_PRM_SD_SAVE
// #define		USE_PRM_INSIDE_FLASH_SAVE
 #define		USE_PRM_OUTSIDE_FLASH_SAVE

#define		PRV_DATA_BASE			(10<<10)

static __ALIGN_RAM uint8_t prm_ram[PRM_MEM_SIZE] = {0};	
static __ALIGN_RAM uint8_t prv_ram[PRV_MEM_SIZE] = {0};
					
SYS_PRM_TypeDef *const prm_str=(SYS_PRM_TypeDef *)(void *)prm_ram; 						
SYS_PRV_TypeDef *const prv_str=(SYS_PRV_TypeDef *)(void *)prv_ram;
 
FRESULT prm_save_sd(void);
FRESULT prm_read_sd(void);
FRESULT prv_save_sd(void);
FRESULT prv_read_sd(void);

/**********************************************************************
functionName:uint8_t *get_prm(void)
description:����ϵͳ����ָ��
**********************************************************************/ 
uint8_t *get_prm(void)
{
	return prm_ram;
}

/**********************************************************************
functionName:uint8_t *get_prv(void)
description:������Ȩ����ָ��
**********************************************************************/ 
uint8_t *get_prv(void)
{
	return prv_ram;
}

/**********************************************************************
functionName:uint32_t devc_ip_get(void)
description: ��ȡ�豸IP����
**********************************************************************/ 
uint32_t devc_ip_get(void)
{
	uint32_t temp32u;
	uint32_t *pu32;
	uint8_t ip[4];
	temp32u=prm_str->prm_devc.device_ip_addr;	
	ip[0]=temp32u>>24;
	ip[1]=(temp32u>>16)&0xFF;
	ip[2]=(temp32u>>8)&0xFF;
	ip[3]=temp32u&0xFF;
	pu32=(uint32_t *)ip;
	return(*pu32);
}

/**********************************************************************
functionName:uint8_t devc_token_get(void)
description: ��ȡ�豸��ʶ,��ʾ���ֻ���
**********************************************************************/ 
uint8_t devc_token_get(void)
{
	uint8_t token;
	token=prm_str->prm_devc.devc_token;
	return token;
}

/**********************************************************************
functionName:void devc_token_set(uint8_t token);		
description:��������ʶ
**********************************************************************/ 
void devc_token_set(uint8_t token)						 
{
	prm_str->prm_devc.devc_token=token;
}

/**********************************************************************
functionName:uint8_t *devc_mac_get(void)
description:��ȡ�豸mac��ַָ��
**********************************************************************/
uint8_t *devc_mac_get(void)
{
	uint8_t *p_mac;
	uint32_t *mac;
	p_mac=prm_str->prm_devc.mac_addr;
	mac=(uint32_t *)p_mac;
	if(*mac==0xFFFFFFFF)
	*mac=0x12345678; 
	return(p_mac);
}

 

/**********************************************************************
functionName: PRM_SMPL_TAB_TypeDef  *get_smpl_tab(uint8_t chn)
description:����ָ��ͨ���Ĵ����������й��ڱ�������Ľṹָ��
**********************************************************************/
PRM_SMPL_TAB_TypeDef  *get_smpl_tab(uint8_t chn)
{
	PRM_SMPL_TAB_TypeDef *smpl_tab;
	smpl_tab=&(prm_str->prm_smpl[chn].prm_smpl_tab);
	return smpl_tab;
}
  
  
/**********************************************************************
functionName: uint32_t ctrl_max_get(void)
description: ��ȡ��������
**********************************************************************/
uint32_t ctrl_max_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max);
}

/**********************************************************************
functionName: uint16_t ctrl_max_cnt_get(void)
description: ��ȡ����������������
**********************************************************************/
uint16_t ctrl_max_cnt_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max_cnt);
}

/**********************************************************************
functionName: uint16_t ctrl_max_one_get(void)
description: ��ȡ������������
**********************************************************************/
uint16_t ctrl_max_one_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max_one);
}

/**********************************************************************
functionName: uint16_t ctrl_min_one_get(void)
description: ��ȡ������С������
**********************************************************************/
uint16_t ctrl_min_one_get(void)
{
	return(prm_str->prm_ctrl.ctrl_min_one);
}

/**********************************************************************
functionName: uint32_t ctrl_open_get(void)
description: ��ȡ����������
**********************************************************************/
uint32_t ctrl_open_get(void)
{
	return(prm_str->prm_ctrl.ctrl_open);
}
 

/**********************************************************************
functionName: uint8_t ctrl_exc_dir_get(void)
description: ��ȡ����ʱ���������з������ϻ������� 
**********************************************************************/
uint8_t ctrl_exc_dir_get(void)
{
	return(prm_str->prm_ctrl.exc_dir);
} 
 
       
/////**********************************************************************
////functionName:void backup_cycle(void)
////description: ���ݵĲ�������
////**********************************************************************/	
////void backup_cycle(void)
////{
////	float tempf;
////	struct ntf_process_str *ntf_process; 
////	uint8_t index;
////	uint8_t awr_buf[10];	
////	uint8_t backup_progress;						//��ǰ���ݽ���		 
////	
////	if(b_backup&&(backup_password==BACKUP_PASS))
////	{ 
////		if((backup_position+BLOCK_SIZE)<=PRM_MEM_SIZE)
////		{
////			memory_write((sram+backup_position),backup_position,BLOCK_SIZE); 
////			backup_position+=BLOCK_SIZE;		//ÿ��д�룬λ�ü���  
////		}
////		else
////		{
////			memory_write((sram+backup_position),backup_position,(PRM_MEM_SIZE-backup_position));  
////			backup_position=PRM_MEM_SIZE;
////			b_backup=0;
////			backup_password=0;					//д�����һ������	 
////		}	
////		
////		tempf=rint(backup_position*100.0f/PRM_MEM_SIZE);
////		backup_progress=tempf;
////		if(b_backup)		//���ͽ���
////		{
////			index=index_cycle(INDEX_ACT_NASW_NOR);
////			 
////			ntf_process=(struct ntf_process_str *)(void *)awr_buf;
////			ntf_process->fun_code=NTF_PROCESSING; 
////			ntf_process->cmd=CMD_BACKUP_PRM;
////			ntf_process->process=backup_progress;  
////			udp_send_pkg(UDP_ADDR,index,sizeof(struct ntf_process_str),awr_buf);	  
////		}	
////		else				//�������
////		{
////			index=index_cycle(INDEX_PAS_ASW_NOR);
////			ntf_process=(struct ntf_process_str *)(void *)awr_buf;
////			ntf_process->fun_code=NTF_FINISHED;
////			ntf_process->cmd=CMD_BACKUP_PRM;
////			udp_send_pkg(UDP_ADDR,index,sizeof(struct ntf_process_str)-1,awr_buf);
////		}	
////	}  		
////} 
////
/////**********************************************************************
////functionName:void backup_set(void)
////description: �������� 
////**********************************************************************/	
////void backup_set(void)
////{
////	b_backup=1;
////	backup_password=BACKUP_PASS;
////	backup_position=0;
////} 
   
 
/**********************************************************************
functionName:uint16_t para_spcl_protect_get(void)
description:��ȡ����λ��־
**********************************************************************/ 
uint16_t spcl_protect_get(void)
{
	uint16_t protect;
	protect=prm_str->prm_spcl.protect_bit;
	return protect;
}
 
/**********************************************************************
functionName:uint32_t prm_spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch)
description:��ȡж����ʱʱ��
**********************************************************************/  
uint32_t spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_unload_delay); 
	else
	return (prm_str->prm_spcl.kz_unload_delay); 
} 


/**********************************************************************
functionName:uint32_t prm_spcl_valve_init_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���ķ��ڳ�ʼλ�� 
**********************************************************************/ 
uint32_t spcl_valve_init_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_init); 
	else
	return (prm_str->prm_spcl.kz_valve_init); 
}

/**********************************************************************
functionName:uint32_t prm_spcl_valve_back_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���ķ��ڻ��˿���
**********************************************************************/ 
uint32_t spcl_valve_back_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_back); 
	else
	return (prm_str->prm_spcl.kz_valve_back); 
}
 

/**********************************************************************
functionName:uint32_t spcl_valve_middle_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���ķ�����λ
**********************************************************************/ 
uint32_t spcl_valve_middle_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_middle); 
	else
	return (prm_str->prm_spcl.kz_valve_middle); 
}
 
/**********************************************************************
functionName:uint32_t para_spcl_quick_up_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���Ŀ��Ͽ���
**********************************************************************/ 
uint32_t spcl_quick_up_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_qup); 
	else
	return (prm_str->prm_spcl.kz_valve_qup);
}

/**********************************************************************
functionName:uint32_t prm_spcl_quick_down_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���Ŀ��¿���
**********************************************************************/ 
uint32_t spcl_quick_down_get(SMPL_NAME_TypeDef2 ch)
{
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_qdown); 
	else
	return (prm_str->prm_spcl.kz_valve_qdown); 
}

/**********************************************************************
functionName:uint32_t para_spcl_slow_up_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ�������Ͽ���
**********************************************************************/ 
uint32_t spcl_slow_up_get(SMPL_NAME_TypeDef2 ch)
{ 
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_sup); 
	else
	return (prm_str->prm_spcl.kz_valve_sup); 
}

/**********************************************************************
functionName:uint32_t para_spcl_slow_down_get(SMPL_NAME_TypeDef2 ch)
description:��ȡָ��ͨ���Ŀ��Ͽ���
**********************************************************************/ 
uint32_t spcl_slow_down_get(SMPL_NAME_TypeDef2 ch)
{
	if(ch==SMPL_KY_NUM)
	return (prm_str->prm_spcl.ky_valve_sdown); 
	else
	return (prm_str->prm_spcl.kz_valve_sdown); 
}

/**********************************************************************
functionName:float smpl_ctrl_entry_get(uint8_t ch)
description:��ȡָ��ͨ���Ŀ������
**********************************************************************/
float smpl_ctrl_entry_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_entry);
}

/**********************************************************************
functionName:float smpl_ctrl_full_p(uint8_t ch)
description:��ȡָ��ͨ��������������
**********************************************************************/
float smpl_ctrl_full_p_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_p);
}

/**********************************************************************
functionName:float smpl_ctrl_full_n(uint8_t ch)
description:��ȡָ��ͨ���ķ���������
**********************************************************************/
float smpl_ctrl_full_n_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_n);
}

/**********************************************************************
functionName:float cof_ahead_get(uint8_t chn)
description:��ȡ������ǰϵ��
**********************************************************************/
float smpl_cof_ahead_get(uint8_t ch)
{
 return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_ahead);
}

/**********************************************************************
functionName:float cof_spd_get(float error,uint8_t ch)
description:����ϵ������
**********************************************************************/
float smpl_cof_spd_get(uint8_t ch)
{	 
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_spd);
}

/**********************************************************************
functionName:float cof_break_get(uint8_t ch)
description:��ȡ�ƶ�ϵ��
**********************************************************************/
float smpl_cof_break_get(uint8_t ch)
{ 
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_brk);
}

/**********************************************************************
functionName:float smpl_ctrl_crash_get(uint8_t ch)
description:��ȡָ��ͨ����Ԥ��ײ��ȫֵ
**********************************************************************/
float smpl_ctrl_crash_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.crash_threshold);
}

/**********************************************************************
functionName:float smpl_ctrl_overspeed_get(uint8_t ch)
description:���ش��������Ʋ������ٱ���ֵ
**********************************************************************/  
float smpl_ctrl_overspeed_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.overspeed);
}


/**********************************************************************
functionName:uint8_t smpl_pid_segs_get(uint8_t ch)
description:��ȡָ��ͨ����pid����
**********************************************************************/    
uint8_t smpl_pid_segs_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.smpl_pid_segs);
} 


/**********************************************************************
functionName:float smpl_pid_node_get(uint8_t ch,uint8_t num)
description: ��ȡָ��ͨ����pid ָ����Ľڵ��ֵ
**********************************************************************/ 
float smpl_pid_node_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_node);
} 


/**********************************************************************
functionName:uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num)
description:��ȡָ��ͨ����pid ָ�����kpֵ
**********************************************************************/ 
uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kp);
}

/**********************************************************************
functionName:uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
description:��ȡָ��ͨ����pid ָ�����kiֵ
**********************************************************************/ 
uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_ki);
}
 
/**********************************************************************
functionName:uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
description:��ȡָ��ͨ����pid ָ�����kdֵ
**********************************************************************/ 
uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kd);
} 


/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: ��ȡָ��ͨ���ı궨�� ָ�����ֵ
**********************************************************************/
float smpl_tab_value_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].value);
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: ��ȡָ��ͨ���ı궨�� ָ�������
**********************************************************************/
int32_t smpl_tab_code_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].code);
}

/**********************************************************************
functionName:int32_t smpl_tab_zero_get(uint8_t ch)
description: ��ȡָ��ͨ�������ֵ
**********************************************************************/
int32_t smpl_tab_zero_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.smpl_zero);
}  

/**********************************************************************
functionName:uint8_t smpl_tab_num_get(uint8_t ch)
description: ��ȡָ��ͨ���ı궨������
**********************************************************************/
uint8_t smpl_tab_num_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_num);
}   

/**********************************************************************
functionName:float smpl_cal(int32_t adc_code,uint8_t chn)
description: ���㴫����ʵ��ֵ
**********************************************************************/	
float smpl_cal(int32_t adc_code,uint8_t chn)
{
	PRM_SMPL_TAB_TypeDef *prm_smpl_tab;	//�������궨�����
	int32_t x1,x2;
	float y1,y2;
	uint8_t i;
	float tempf;
	prm_smpl_tab=get_smpl_tab(chn);
	//���ֻ��һ���궨��
	if(prm_smpl_tab->table_num==1)		
	{
		x1=0;
		y1=0;
		x2=prm_smpl_tab->table_pt[0].code;
		y2=prm_smpl_tab->table_pt[0].value;
	}	
	else
	{
		if(prm_smpl_tab->table_pt[0].code<prm_smpl_tab->table_pt[1].code)
		{
			for(i=1;i<(prm_smpl_tab->table_num-1);i++)
			{
				 if(adc_code<(prm_smpl_tab->table_pt[i].code))
				 break;	
			}
		}	
		else
		{
			for(i=1;i<(prm_smpl_tab->table_num-1);i++)
			{
				 if(adc_code>(prm_smpl_tab->table_pt[i].code))
				 break;	
			}	
		}		
		x1=prm_smpl_tab->table_pt[i-1].code;
		y1=prm_smpl_tab->table_pt[i-1].value;
		x2=prm_smpl_tab->table_pt[i].code;
		y2=prm_smpl_tab->table_pt[i].value;
	}	
	tempf=(y2-y1)/(x2-x1)*(adc_code-x1)+y1;
	return(tempf);
}
 
   
/**********************************************************************
functionName:INSTRCT_DIRE_TypeDef prv_ins_dir_get(void)
description:��ȡָ���
**********************************************************************/
INSTRCT_DIRE_TypeDef prv_ins_dir_get(void)
{
	return((INSTRCT_DIRE_TypeDef)(prv_str->instrct_dire));
} 

/**********************************************************************
functionName:uint16_t prvt_gain_get(uint8_t chn)
description:��ȡָ��ͨ��������
**********************************************************************/
uint16_t prv_gain_get(uint8_t chn)
{
	return(prv_str->gain[chn]);
}

/**********************************************************************
functionName:uint32_t prv_ele_gear_get(void)
description:��ȡ���ӳ���
**********************************************************************/
uint32_t prv_ele_gear_get(void)
{
	return(prv_str->electronic_gear);
}

/**********************************************************************
functionName:uint32_t prv_devc_key_get(void)
description:��ȡ�豸Կ��
**********************************************************************/
uint32_t prv_devc_key_get(void)
{
	return(prv_str->devc_key);
}

  

 
/**********************************************************************
functionName:void devc_ip_get(void)
description: ��ȡ�豸IP����,ֻ�ı��ڴ�Ĳ��������ı�flash������
**********************************************************************/ 
void devc_ip_set(uint32_t ip)
{
	uint8_t *p1,*p2;
	uint32_t temp32u;
	p1=(uint8_t *)&ip;
	p2=(uint8_t *)&temp32u;
	p2[0]=p1[3];
	p2[1]=p1[2];
	p2[2]=p1[1];
	p2[3]=p1[0]; 
	prm_str->prm_devc.device_ip_addr=temp32u;	  
}


/**********************************************************************
functionName:void prm_devc_mac_set(uint8_t *mac)
description:�����豸mac��ַָ��
**********************************************************************/
void devc_mac_set(uint8_t *mac)
{ 	
	uint8_t i;
	for(i=0;i<6;i++)
	prm_str->prm_devc.mac_addr[i]=mac[i];
}


/**********************************************************************
functionName: void ctrl_max_set(uint32_t dgr)
description: ������������,���ز�����ַ
**********************************************************************/
void ctrl_max_set(uint32_t arg)
{ 	
	prm_str->prm_ctrl.ctrl_max=arg; 
} 


/**********************************************************************
functionName: void ctrl_max_cnt_get(void)
description: ��ȡ����������������,���ز�����ַ
**********************************************************************/
void ctrl_max_cnt_set(uint16_t arg)
{ 
	prm_str->prm_ctrl.ctrl_max_cnt=arg; 
} 


/**********************************************************************
functionName:void ctrl_max_one_set(uint32_t arg)
description: ���õ�����������,���ز�����ַ
**********************************************************************/
void ctrl_max_one_set(uint32_t arg)
{ 
	prm_str->prm_ctrl.ctrl_max_one=arg; 
}


/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: ���õ�����С������,���ز�����ַ
**********************************************************************/
void ctrl_min_one_set(uint16_t arg)
{ 
	prm_str->prm_ctrl.ctrl_min_one=arg; 
}	

/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: ���ÿ���������,���ز�����ַ
**********************************************************************/
void ctrl_open_set(uint32_t arg)
{ 
	prm_str->prm_ctrl.ctrl_open=arg; 
}	


/**********************************************************************
functionName:void ctrl_exc_dir_set(uint8_t arg)
description: ���ÿ���������,���ز�����ַ
**********************************************************************/
void ctrl_exc_dir_set(uint8_t arg)
{ 
	prm_str->prm_ctrl.exc_dir=arg; 
}

/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: ���ÿ���������,���ز�����ַ
**********************************************************************/
void spcl_protect_set(uint16_t arg)
{ 
	prm_str->prm_spcl.protect_bit=arg; 
}

/**********************************************************************
functionName:uint16_t para_spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch)
description:����ж����ʱʱ��
**********************************************************************/  
void spcl_unload_delay_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_unload_delay=arg; 
	else
	prm_str->prm_spcl.kz_unload_delay=arg; 
} 


/**********************************************************************
functionName:void prm_spcl_valve_init_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
description:����ָ��ͨ���ķ��ڳ�ʼλ�� 
**********************************************************************/ 
void spcl_valve_init_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_init=arg; 
	else
	prm_str->prm_spcl.kz_valve_init=arg; 
}


/**********************************************************************
functionName:uint32_t prm_spcl_valve_back_get(SMPL_NAME_TypeDef2 ch)
description:����ָ��ͨ���ķ��ڻ��˿���
**********************************************************************/ 
void spcl_valve_back_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_back=arg; 
	else
	prm_str->prm_spcl.kz_valve_back=arg; 
}


/**********************************************************************
functionName:void prm_spcl_valve_middle_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
description:����ָ��ͨ���ķ�����λ
**********************************************************************/ 
void spcl_valve_middle_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_middle=arg;  
	else
	prm_str->prm_spcl.kz_valve_middle=arg; 
}

/**********************************************************************
functionName:void prm_spcl_quick_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
description:����ָ��ͨ���Ŀ��Ͽ���
**********************************************************************/ 
void spcl_quick_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_qup=arg;  
	else
	prm_str->prm_spcl.kz_valve_qup=arg;
}

/**********************************************************************
functionName:uint32_t prm_spcl_quick_down_get(SMPL_NAME_TypeDef2 ch)
description:����ָ��ͨ���Ŀ��¿���
**********************************************************************/ 
void spcl_quick_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_qdown=arg; 
	else
	prm_str->prm_spcl.kz_valve_qdown=arg; 
}


/**********************************************************************
functionName:uint32_t para_spcl_slow_up_get(SMPL_NAME_TypeDef2 ch)
description:����ָ��ͨ�������Ͽ���
**********************************************************************/ 
void spcl_slow_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{ 
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_sup=arg; 
	else
	prm_str->prm_spcl.kz_valve_sup=arg; 
}


/**********************************************************************
functionName:uint32_t para_spcl_slow_down_get(SMPL_NAME_TypeDef2 ch)
description:����ָ��ͨ���Ŀ��Ͽ���
**********************************************************************/ 
void spcl_slow_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg)
{
	if(ch==SMPL_KY_NUM)
	prm_str->prm_spcl.ky_valve_sdown=arg; 
	else
	prm_str->prm_spcl.kz_valve_sdown=arg; 
}


/**********************************************************************
functionName:float smpl_ctrl_entry_get(uint8_t ch)
description:����ָ��ͨ���Ŀ������
**********************************************************************/
void smpl_ctrl_entry_set(uint8_t ch,float entry)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_entry=entry;
}

/**********************************************************************
functionName:float smpl_ctrl_full_p(uint8_t ch)
description:����ָ��ͨ��������������
**********************************************************************/
void smpl_ctrl_full_p_set(uint8_t ch,float full_p)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_p=full_p;
}

/**********************************************************************
functionName:float smpl_ctrl_full_n(uint8_t ch)
description:����ָ��ͨ���ķ���������
**********************************************************************/
void smpl_ctrl_full_n_set(uint8_t ch,float full_n)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_n=full_n;
}


/**********************************************************************
functionName:float cof_ahead_get(uint8_t chn)
description:���ñ�����ǰϵ��
**********************************************************************/
void smpl_cof_ahead_set(uint8_t ch,float ahead)
{
 	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_ahead=ahead;
}

/**********************************************************************
functionName:float cof_spd_get(float error,uint8_t ch)
description:����ϵ������
**********************************************************************/
void smpl_cof_spd_set(uint8_t ch,float keep_spd)
{	 
	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_spd=keep_spd;
}

/**********************************************************************
functionName:float cof_break_get(uint8_t ch)
description:�����ƶ�ϵ��
**********************************************************************/
void smpl_cof_break_set(uint8_t ch,float keep_brk)
{ 
	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_brk=keep_brk;
}

/**********************************************************************
functionName:float smpl_ctrl_crash_get(uint8_t ch)
description:����ָ��ͨ����Ԥ��ײ��ȫֵ
**********************************************************************/
void smpl_ctrl_crash_set(uint8_t ch,float crash)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.crash_threshold=crash;
}

/**********************************************************************
functionName:float smpl_ctrl_overspeed_get(uint8_t ch)
description:���ô��������Ʋ������ٱ���ֵ
**********************************************************************/  
void smpl_ctrl_overspeed_set(uint8_t ch,float speed)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.overspeed=speed;
}

/**********************************************************************
functionName:uint8_t smpl_pid_segs_get(uint8_t ch)
description:����ָ��ͨ����pid����
**********************************************************************/    
void smpl_pid_segs_set(uint8_t ch,uint8_t segs)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.smpl_pid_segs=segs;
}

/**********************************************************************
functionName:uint16_t smpl_pid_node_set(uint8_t ch,uint8_t num)
description:����ָ��ͨ����pid ָ�����kpֵ
**********************************************************************/ 
void smpl_pid_node_set(uint8_t ch,uint8_t num,float node)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_node=node;
}

/**********************************************************************
functionName:uint16_t smpl_pid_kp_set(uint8_t ch,uint8_t num)
description:����ָ��ͨ����pid ָ�����kpֵ
**********************************************************************/ 
void smpl_pid_kp_set(uint8_t ch,uint8_t num,uint16_t kp)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kp=kp;
}


/**********************************************************************
functionName:uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
description:����ָ��ͨ����pid ָ�����kiֵ
**********************************************************************/ 
void smpl_pid_ki_set(uint8_t ch,uint8_t num,uint16_t ki)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_ki=ki;
}

/**********************************************************************
functionName:uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
description:����ָ��ͨ����pid ָ�����kdֵ
**********************************************************************/ 
void smpl_pid_kd_set(uint8_t ch,uint8_t num,uint16_t kd)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kd=kd;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: ����ָ��ͨ���ı궨�� ָ�����ֵ
**********************************************************************/
void smpl_tab_value_set(uint8_t ch,uint8_t num,float value)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].value=value;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: ����ָ��ͨ���ı궨�� ָ�������
**********************************************************************/
void smpl_tab_code_set(uint8_t ch,uint8_t num,int32_t code)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].code=code;
}

/**********************************************************************
functionName:int32_t smpl_tab_zero_get(uint8_t ch)
description: ����ָ��ͨ�������ֵ
**********************************************************************/
void smpl_tab_zero_set(uint8_t ch,int32_t code)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.smpl_zero=code;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_num_get(uint8_t ch)
description: ����ָ��ͨ���ı궨������
**********************************************************************/
void smpl_tab_num_set(uint8_t ch,uint8_t table)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_num=table;
} 

/**********************************************************************
functionName:INSTRCT_DIRE_TypeDef prv_ins_dir_get(void)
description:����ָ���
**********************************************************************/
void prv_ins_dir_set(INSTRCT_DIRE_TypeDef dire)
{
	prv_str->instrct_dire=dire;
} 

/**********************************************************************
functionName:uint16_t prvt_gain_get(uint8_t ch)
description:����ָ��ͨ��������
**********************************************************************/
void prv_gain_set(uint8_t ch,uint16_t gain)
{
	prv_str->gain[ch]=gain;
}

/**********************************************************************
functionName:uint32_t prv_ele_gear_get(void)
description:���õ��ӳ���
**********************************************************************/
void prv_ele_gear_set(uint32_t eg)
{
 	prv_str->electronic_gear=eg;
}

/**********************************************************************
functionName:uint32_t prv_devc_key_get(void)
description:�����豸Կ��
**********************************************************************/
void prv_devc_key_set(uint32_t key)
{
	prv_str->devc_key=key;
}

/**********************************************************************
functionName:uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len)
description:��ȡ�豸������ʼ���ƺͳ���
**********************************************************************/  
uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len)
{
	*addr=0;
	*len=sizeof(PRM_DEVC_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_devc));
}  

/**********************************************************************
functionName:void prm_ctrl_info(uint32_t *addr,uint16_t *len)
description:��ȡ���Ʋ�����ʼ���ƺͳ���
**********************************************************************/ 
uint8_t *prm_ctrl_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_ctrl)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_CTRL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_ctrl));
}


/**********************************************************************
functionName:void prm_spcl_info(uint32_t *addr,uint16_t *len)
description:��ȡ���������ʼ���ƺͳ���
**********************************************************************/ 
uint8_t *prm_spcl_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_spcl)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_SPCL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_spcl));
}


/**********************************************************************
functionName:uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len)
description:��ȡ�ڲ�������ʼ���ƺͳ���
**********************************************************************/ 
uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_insd)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_INSD_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_insd));
}

/**********************************************************************
functionName:void prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch)
description:��ȡָ��ͨ��������ʼ���ƺͳ���
**********************************************************************/ 
uint8_t *prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch)
{
	*addr=(uint32_t)&(prm_str->prm_smpl[ch])-(uint32_t)prm_ram;	
	*len=sizeof(PRM_SMPL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_smpl[ch]));
}

/**********************************************************************
functionName:void prm_read(void)
description:��flash����ȡϵͳ���� 
**********************************************************************/
void prm_read(void)
{   
	#if 	defined	USE_PRM_SD_SAVE
		prm_read_sd();
	#elif 	defined USE_PRM_INSIDE_FLASH_SAVE
		eep_read(prm_ram,0,PRM_MEM_SIZE); 
	#elif 	defined USE_PRM_OUTSIDE_FLASH_SAVE
		sf_ReadBuffer(prm_ram,0,PRM_MEM_SIZE); 
	#endif
	
//	eep_read(prm_ram,0,PRM_MEM_SIZE); 
//	prm_read_sd();
//	sf_ReadBuffer(prm_ram,0,PRM_MEM_SIZE); 
}

/**********************************************************************
functionName:void prv_read(void)
description:��flash����ȡ��Ȩ����
**********************************************************************/
void prv_read(void)
{  
	#if 	defined	USE_PRM_SD_SAVE
		prv_read_sd();
	#elif 	defined USE_PRM_INSIDE_FLASH_SAVE
		eep_read(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE); 
	#elif 	defined USE_PRM_OUTSIDE_FLASH_SAVE
		sf_ReadBuffer(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE);
	#endif
	
//	eep_read(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE); 
//	prv_read_sd();
//	sf_ReadBuffer(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE); 
}



/**********************************************************************
functionName:void prm_save(void)
description:����ϵͳ������flash
PRM_MEM_SIZE		//ϵͳ�����ֽ���
**********************************************************************/ 
void prm_save(void)
{
	#if 	defined	USE_PRM_SD_SAVE
		prm_save_sd();
	#elif 	defined USE_PRM_INSIDE_FLASH_SAVE
		eep_write(prm_ram,0,PRM_MEM_SIZE);
	#elif 	defined USE_PRM_OUTSIDE_FLASH_SAVE
		sf_WriteBuffer(prm_ram,0,PRM_MEM_SIZE);
	#endif
	
//	eep_write(prm_ram,0,PRM_MEM_SIZE);
//	prm_save_sd();
// 	sf_WriteBuffer(prm_ram,0,PRM_MEM_SIZE);
}

/**********************************************************************
functionName:void prv_save(void)
description:������Ȩ������flash
**********************************************************************/ 
void prv_save(void)
{
	#if 	defined	USE_PRM_SD_SAVE
		prv_save_sd();
	#elif 	defined USE_PRM_INSIDE_FLASH_SAVE
		eep_write(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE);
	#elif 	defined USE_PRM_OUTSIDE_FLASH_SAVE
		sf_WriteBuffer(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE);
	#endif
	
//	eep_write(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE);
//	prv_save_sd();
// 	sf_WriteBuffer(prv_ram,PRV_DATA_BASE,PRV_MEM_SIZE);
}

/**********************************************************************
functionName:void prm_write(uint8_t *dat,uint16_t addr,uint16_t len)
description:д�����ڴ������
**********************************************************************/ 
void prm_write(uint8_t *dat,uint16_t addr,uint16_t len)
{
	uint8_t *prm;
	uint16_t j;
	prm=prm_ram;
	prm+=addr;
	if((addr+len)>sizeof(SYS_PRM_TypeDef))
	len=sizeof(SYS_PRM_TypeDef)-addr;	
	for(j=0;j<len;j++)
	prm[j]=dat[j]; 
}


/**********************************************************************
functionName:void prv_write(uint8_t *dat,uint16_t addr,uint16_t len)
description:д��Ȩ�����ڴ������ 
**********************************************************************/ 
void prv_write(uint8_t *dat,uint16_t addr,uint16_t len)
{
	uint8_t *prv;
	uint16_t j;
	prv=prv_ram;
	prv+=addr;
	if((addr+len)>sizeof(SYS_PRV_TypeDef))
	len=sizeof(SYS_PRV_TypeDef)-addr;	
	for(j=0;j<len;j++)
	prv[j]=dat[j]; 
}



#define		SAVE_SD_BUF_SIZE		64
/**********************************************************************
functionName: FRESULT prm_save_sd(void) 
description:��λ���������浽SD��
**********************************************************************/ 
FRESULT prm_save_sd(void) 								
{
	uint8_t *dat = NULL;
	uint32_t prm_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	
	fresult=f_open(&file_obj,"para/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);

	if(fresult==FR_NO_PATH)
	{
		f_mkdir("para"); 
		fresult=f_open(&file_obj,"para/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);
		if(fresult!=FR_OK)
		return fresult; 	
	}
	else if(fresult!=FR_OK)	 
	return fresult; 
	
	dat=prm_ram;
	prm_len=PRM_MEM_SIZE;
	
	while(prm_len)
	{
		if(prm_len>SAVE_SD_BUF_SIZE)
		{
			f_write(&file_obj,dat,SAVE_SD_BUF_SIZE,&br);  
			dat+=SAVE_SD_BUF_SIZE;	
			prm_len-=SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_write(&file_obj,dat,prm_len,&br);  
			prm_len=0;
		}
	}  
	f_close(&file_obj);

	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prm_read_sd(void)	
description:��λ��������SD��ȡ����
**********************************************************************/ 
FRESULT prm_read_sd(void)							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat;
	uint32_t prm_len=0; 
	
	fresult=f_open(&file_obj,"para/prm.bin",FA_OPEN_EXISTING|FA_READ);
	if(fresult!=FR_OK)
	return fresult; 
	 
	dat=prm_ram;
	prm_len=PRM_MEM_SIZE; 
	while(prm_len)
	{
		if(prm_len>SAVE_SD_BUF_SIZE)
		{
			f_read(&file_obj,dat,SAVE_SD_BUF_SIZE,&br);
			dat+=SAVE_SD_BUF_SIZE;
			prm_len-=SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_read(&file_obj,dat,prm_len,&br);
			prm_len=0;
		}	
	}  
	f_close(&file_obj);

	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prv_save_sd(void) 
description:��Ȩ�������浽SD��
**********************************************************************/ 
FRESULT prv_save_sd(void) 								
{
	uint8_t *dat = NULL;
	uint32_t prv_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	 
	fresult=f_open(&file_obj,"para/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
	if(fresult==FR_NO_PATH)
	{
		f_mkdir("para"); 
		fresult=f_open(&file_obj,"para/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
		if(fresult!=FR_OK)
		return fresult; 	
	}
	else if(fresult!=FR_OK)	 
	return fresult; 
	 
	dat=prv_ram;
	prv_len=PRV_MEM_SIZE;
	while(prv_len)
	{
		if(prv_len>SAVE_SD_BUF_SIZE)
		{
			f_write (&file_obj,dat,SAVE_SD_BUF_SIZE,&br);  
			dat+=SAVE_SD_BUF_SIZE;	
			prv_len-=SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_write (&file_obj,dat,prv_len,&br);  
			prv_len=0;
		}	
	}  
	f_close(&file_obj);
	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prv_read_sd(void)	
description:��Ȩ������SD��ȡ����
**********************************************************************/ 
FRESULT prv_read_sd(void)							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat = NULL;
	uint32_t prv_len=0; 
	
	fresult=f_open(&file_obj,"para/prv.bin",FA_OPEN_EXISTING|FA_READ);
	if(fresult!=FR_OK)
	return fresult; 
	 
	dat=prv_ram;
	prv_len=PRV_MEM_SIZE; 
	while(prv_len)
	{
		if(prv_len>SAVE_SD_BUF_SIZE)
		{
			f_read(&file_obj,dat,SAVE_SD_BUF_SIZE,&br);
			dat+=SAVE_SD_BUF_SIZE;
			prv_len-=SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_read(&file_obj,dat,prv_len,&br);
			prv_len=0;
		}	
	}  
	f_close(&file_obj);

	return FR_OK;
}


#define		BACKUP_BUF_SIZE		64
/**********************************************************************
functionName: FRESULT prm_backup(void) 
description:ϵͳ�������ݵ�SD��
**********************************************************************/ 
FRESULT prm_backup( uint8_t type ) 								
{
	uint8_t *dat = NULL;
	uint32_t prm_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	 
	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("0:/sys"); 
				fresult=f_open(&file_obj,"0:/sys/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 	
			}
			else if(fresult!=FR_OK)
			{		
				return fresult; 
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("1:/sys"); 
				fresult=f_open(&file_obj,"1:/sys/prm.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 	
			}
			else if(fresult!=FR_OK)
			{		
				return fresult; 
			}
			break;
	}
	 
	dat=prm_ram;
	prm_len=PRM_MEM_SIZE;
	while(prm_len)
	{
		if(prm_len>BACKUP_BUF_SIZE)
		{
			f_write (&file_obj,dat,BACKUP_BUF_SIZE,&br);  
			dat+=BACKUP_BUF_SIZE;	
			prm_len-=BACKUP_BUF_SIZE;
		}	
		else
		{
			f_write (&file_obj,dat,prm_len,&br);  
			prm_len=0;
		}	
	}  
	f_close(&file_obj);
	return FR_OK;
}



/**********************************************************************
functionName: 
description:��Ȩ�������ݵ�SD��
**********************************************************************/ 
FRESULT prv_backup(uint8_t type)
{
	uint8_t *dat = NULL;
	uint32_t prm_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br; 

	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("0:/sys"); 
				fresult=f_open(&file_obj,"0:/sys/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 
			}	 
			else if(fresult!=FR_OK)
			{
				return fresult;  
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("1:/sys"); 
				fresult=f_open(&file_obj,"1:/sys/prv.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 
			}	 
			else if(fresult!=FR_OK)
			{
				return fresult;  
			}
			break;
	}
	
	dat=prv_ram;
	prm_len=PRV_MEM_SIZE; 
	f_write (&file_obj,dat,prm_len,&br);   
	f_close(&file_obj);
	return FR_OK;
}	
  

/**********************************************************************
functionName: FRESULT prm_recover(void)	
description:ϵͳ������SD���ָ�,ע�Ᵽ��������Ϣ 
**********************************************************************/ 
FRESULT prm_recover(uint8_t type)							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat = NULL;
	uint32_t prm_len=0; 
	uint8_t info[32];
	
	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/prm.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult; 
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/prm.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult; 
			}
			break;
	}
	 
	memcpy(info,prm_str->prm_insd.dvc_info,32); 		//��ȡ������Ϣ����
	dat=prm_ram;
	prm_len=PRM_MEM_SIZE; 
	while(prm_len)
	{
		if(prm_len>BACKUP_BUF_SIZE)
		{
			f_read(&file_obj,dat,BACKUP_BUF_SIZE,&br);
			dat+=BACKUP_BUF_SIZE;
			prm_len-=BACKUP_BUF_SIZE;
		}	
		else
		{
			f_read(&file_obj,dat,prm_len,&br);
			prm_len=0;
		}	
	}  
	f_close(&file_obj);
	memcpy(prm_str->prm_insd.dvc_info,info,32); 		//����������Ϣ
	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prv_recover(void)
description:��Ȩ������SD���ָ� ��ע����Ҫ������ǰ������
**********************************************************************/ 
FRESULT prv_recover(uint8_t type)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat = NULL;
	
	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/prv.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult;    
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/prv.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult;    
			}
			break;
	}
	
	dat=prv_ram; 
	f_read(&file_obj,dat,PRV_MEM_SIZE,&br); 
	f_close(&file_obj);
	return FR_OK;
}
 
 


/**********************************************************************
functionName: FRESULT PrmBackupWithDate(void) 
description:ϵͳ�������ݵ�SD�������ļ�����ʽ�洢��
**********************************************************************/ 
FRESULT PrmBackupWithDate(void) 								
{
	FRESULT fresult;
	DIR dir_object;
	FIL file_obj;  
	uint32_t br;  	 
	tTime t;
	char file_name[30];
	
	fresult = f_opendir(&dir_object,"sys"); 
	
	switch ( fresult )
	{
		case FR_NO_PATH:
			fresult = f_mkdir("sys");
			if (FR_OK != fresult)
			{
				return fresult;
			}
			break;
		
		case FR_OK:
			break;
		
		default:
			return fresult;
	}
	
	time_cycle();
	t = get_time();
	
	usprintf(file_name,"sys/%04d%02d%02d",t.usYear, t.ucMon, t.ucMday);		
	fresult = f_mkdir(file_name);
	
	usprintf(file_name,"sys/%04d%02d%02d/prm.bin",t.usYear, t.ucMon, t.ucMday);
	fresult = f_open(&file_obj,file_name,FA_CREATE_ALWAYS|FA_WRITE);
		
	if(FR_OK != fresult)	
	{
		return fresult;
	}
	   
	f_write(&file_obj,prm_ram,PRM_MEM_SIZE,&br); 
	f_close(&file_obj);
	
	return FR_OK;
}


/**********************************************************************
functionName: FRESULT PrmBackupWithDate(void) 
description:ϵͳ��Ȩ�������ݵ�SD�������ļ�����ʽ�洢��
**********************************************************************/ 
FRESULT PrvBackupWithDate(void) 
{
	FRESULT fresult;
	DIR dir_object;
	FIL file_obj;  
	uint32_t br; 
	tTime t;
	char file_name[30];
	
	fresult = f_opendir(&dir_object,"sys"); 
	
	switch ( fresult )
	{
		case FR_NO_PATH:
			fresult = f_mkdir("sys");
			if (FR_OK != fresult)
			{
				return fresult;
			}
			break;
		
		case FR_OK:
			break;
		
		default:
			return fresult;
	}
	
	time_cycle();
	t = get_time();
	
	usprintf(file_name,"sys/%04d%02d%02d",t.usYear, t.ucMon, t.ucMday);		
	fresult = f_mkdir(file_name);
	
	usprintf(file_name,"sys/%04d%02d%02d/prv.bin",t.usYear, t.ucMon, t.ucMday);
	fresult = f_open(&file_obj,file_name,FA_CREATE_ALWAYS|FA_WRITE);
		
	if(FR_OK != fresult)	
	{
		return fresult;
	}
 
	f_write(&file_obj,prv_ram,PRV_MEM_SIZE,&br);   
	f_close(&file_obj);
	
	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prm_recover(void)	
description:ϵͳ������SD���ָ�,ע�Ᵽ��������Ϣ 
**********************************************************************/ 
FRESULT PrmRecoverWithDate( const char *pFile_name )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t info[32];
	char file_name[30];
	uint32_t name_buff = 0;
	
	name_buff = ustrtoul(pFile_name,0,10);
	
	usprintf(file_name,"sys/%08d/prm.bin",name_buff);
	
	fresult = f_open(&file_obj,file_name,FA_OPEN_EXISTING|FA_READ);
	if (FR_OK != fresult)
	{
		return fresult;
	}
	 
	memcpy(info,prm_str->prm_insd.dvc_info,32); 		//��ȡ������Ϣ����
	
	f_read(&file_obj,prm_ram,PRM_MEM_SIZE,&br);
	
	f_close(&file_obj);
	memcpy(prm_str->prm_insd.dvc_info,info,32); 		//����������Ϣ
	
	return FR_OK;
}


/**********************************************************************
functionName: FRESULT prv_recover(void)
description:��Ȩ������SD���ָ� ��ע����Ҫ������ǰ������
**********************************************************************/ 
FRESULT PrvRecoverWithDate( const char *pFile_name )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char file_name[30];
	uint32_t name_buff = 0;
	
	name_buff = ustrtoul(pFile_name,0,10);
	
	usprintf(file_name,"sys/%08d/prv.bin",name_buff);
	
	fresult = f_open(&file_obj,file_name,FA_OPEN_EXISTING|FA_READ);
	if (FR_OK != fresult)
	{
		return fresult;
	}

	f_read(&file_obj,prv_ram,PRV_MEM_SIZE,&br); 
	f_close(&file_obj);
	
	return FR_OK;
} 
  

  
/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/ 
