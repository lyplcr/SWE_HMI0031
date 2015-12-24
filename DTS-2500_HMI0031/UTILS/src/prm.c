/**
  ******************************************************************************
  * @file    prm.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   下位机参数
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

uint8_t prm_ram[PRM_MEM_SIZE] = {0};	
uint8_t prv_ram[PRV_MEM_SIZE] = {0};
					
SYS_PRM_TypeDef *const prm_str=(SYS_PRM_TypeDef *)(void *)prm_ram; 						
SYS_PRV_TypeDef *const prv_str=(SYS_PRV_TypeDef *)(void *)prv_ram;
 
FRESULT prm_save_sd(void);
FRESULT prm_read_sd(void);
FRESULT prv_save_sd(void);
FRESULT prv_read_sd(void);

/**********************************************************************
functionName:uint8_t *get_prm(void)
description:返回系统参数指针
**********************************************************************/ 
uint8_t *get_prm(void)
{
	return prm_ram;
}

/**********************************************************************
functionName:uint8_t *get_prv(void)
description:返回特权参数指针
**********************************************************************/ 
uint8_t *get_prv(void)
{
	return prv_ram;
}

/**********************************************************************
functionName:uint32_t devc_ip_get(void)
description: 获取设备IP参数
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
description: 获取设备标识,表示哪种机型
**********************************************************************/ 
uint8_t devc_token_get(void)
{
	uint8_t token;
	token=prm_str->prm_devc.devc_token;
	return token;
}

/**********************************************************************
functionName:void devc_token_set(uint8_t token);		
description:设置设别标识
**********************************************************************/ 
void devc_token_set(uint8_t token)						 
{
	prm_str->prm_devc.devc_token=token;
}

/**********************************************************************
functionName:uint8_t *devc_mac_get(void)
description:获取设备mac地址指针
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
description:返回指定通道的传感器参数中关于表定标参数的结构指针
**********************************************************************/
PRM_SMPL_TAB_TypeDef  *get_smpl_tab(uint8_t chn)
{
	PRM_SMPL_TAB_TypeDef *smpl_tab;
	smpl_tab=&(prm_str->prm_smpl[chn].prm_smpl_tab);
	return smpl_tab;
}
  
  
/**********************************************************************
functionName: uint32_t ctrl_max_get(void)
description: 获取最大控制量
**********************************************************************/
uint32_t ctrl_max_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max);
}

/**********************************************************************
functionName: uint16_t ctrl_max_cnt_get(void)
description: 获取允许最大控制量次数
**********************************************************************/
uint16_t ctrl_max_cnt_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max_cnt);
}

/**********************************************************************
functionName: uint16_t ctrl_max_one_get(void)
description: 获取单次最大控制量
**********************************************************************/
uint16_t ctrl_max_one_get(void)
{
	return(prm_str->prm_ctrl.ctrl_max_one);
}

/**********************************************************************
functionName: uint16_t ctrl_min_one_get(void)
description: 获取单次最小控制量
**********************************************************************/
uint16_t ctrl_min_one_get(void)
{
	return(prm_str->prm_ctrl.ctrl_min_one);
}

/**********************************************************************
functionName: uint32_t ctrl_open_get(void)
description: 获取开环控制量
**********************************************************************/
uint32_t ctrl_open_get(void)
{
	return(prm_str->prm_ctrl.ctrl_open);
}
 

/**********************************************************************
functionName: uint8_t ctrl_exc_dir_get(void)
description: 获取进程时横梁的运行方向，向上或者向下 
**********************************************************************/
uint8_t ctrl_exc_dir_get(void)
{
	return(prm_str->prm_ctrl.exc_dir);
} 
 
       
/////**********************************************************************
////functionName:void backup_cycle(void)
////description: 备份的操作函数
////**********************************************************************/	
////void backup_cycle(void)
////{
////	float tempf;
////	struct ntf_process_str *ntf_process; 
////	uint8_t index;
////	uint8_t awr_buf[10];	
////	uint8_t backup_progress;						//当前备份进度		 
////	
////	if(b_backup&&(backup_password==BACKUP_PASS))
////	{ 
////		if((backup_position+BLOCK_SIZE)<=PRM_MEM_SIZE)
////		{
////			memory_write((sram+backup_position),backup_position,BLOCK_SIZE); 
////			backup_position+=BLOCK_SIZE;		//每次写入，位置记忆  
////		}
////		else
////		{
////			memory_write((sram+backup_position),backup_position,(PRM_MEM_SIZE-backup_position));  
////			backup_position=PRM_MEM_SIZE;
////			b_backup=0;
////			backup_password=0;					//写完最后一块内容	 
////		}	
////		
////		tempf=rint(backup_position*100.0f/PRM_MEM_SIZE);
////		backup_progress=tempf;
////		if(b_backup)		//发送进度
////		{
////			index=index_cycle(INDEX_ACT_NASW_NOR);
////			 
////			ntf_process=(struct ntf_process_str *)(void *)awr_buf;
////			ntf_process->fun_code=NTF_PROCESSING; 
////			ntf_process->cmd=CMD_BACKUP_PRM;
////			ntf_process->process=backup_progress;  
////			udp_send_pkg(UDP_ADDR,index,sizeof(struct ntf_process_str),awr_buf);	  
////		}	
////		else				//发送完成
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
////description: 备份设置 
////**********************************************************************/	
////void backup_set(void)
////{
////	b_backup=1;
////	backup_password=BACKUP_PASS;
////	backup_position=0;
////} 
   
 
/**********************************************************************
functionName:uint16_t para_spcl_protect_get(void)
description:获取保护位标志
**********************************************************************/ 
uint16_t spcl_protect_get(void)
{
	uint16_t protect;
	protect=prm_str->prm_spcl.protect_bit;
	return protect;
}
 
/**********************************************************************
functionName:uint32_t prm_spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch)
description:获取卸载延时时间
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
description:获取指定通道的阀口初始位置 
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
description:获取指定通道的阀口回退开度
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
description:获取指定通道的阀口中位
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
description:获取指定通道的快上开度
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
description:获取指定通道的快下开度
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
description:获取指定通道的慢上开度
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
description:获取指定通道的快上开度
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
description:获取指定通道的控制入口
**********************************************************************/
float smpl_ctrl_entry_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_entry);
}

/**********************************************************************
functionName:float smpl_ctrl_full_p(uint8_t ch)
description:获取指定通道的正向满量程
**********************************************************************/
float smpl_ctrl_full_p_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_p);
}

/**********************************************************************
functionName:float smpl_ctrl_full_n(uint8_t ch)
description:获取指定通道的反向满量程
**********************************************************************/
float smpl_ctrl_full_n_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_n);
}

/**********************************************************************
functionName:float cof_ahead_get(uint8_t chn)
description:获取保持提前系数
**********************************************************************/
float smpl_cof_ahead_get(uint8_t ch)
{
 return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_ahead);
}

/**********************************************************************
functionName:float cof_spd_get(float error,uint8_t ch)
description:保持系数计算
**********************************************************************/
float smpl_cof_spd_get(uint8_t ch)
{	 
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_spd);
}

/**********************************************************************
functionName:float cof_break_get(uint8_t ch)
description:获取制动系数
**********************************************************************/
float smpl_cof_break_get(uint8_t ch)
{ 
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_brk);
}

/**********************************************************************
functionName:float smpl_ctrl_crash_get(uint8_t ch)
description:获取指定通道的预碰撞安全值
**********************************************************************/
float smpl_ctrl_crash_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.crash_threshold);
}

/**********************************************************************
functionName:float smpl_ctrl_overspeed_get(uint8_t ch)
description:返回传感器控制参数过速保护值
**********************************************************************/  
float smpl_ctrl_overspeed_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_ctrl.overspeed);
}


/**********************************************************************
functionName:uint8_t smpl_pid_segs_get(uint8_t ch)
description:获取指定通道的pid段数
**********************************************************************/    
uint8_t smpl_pid_segs_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.smpl_pid_segs);
} 


/**********************************************************************
functionName:float smpl_pid_node_get(uint8_t ch,uint8_t num)
description: 获取指定通道的pid 指定点的节点的值
**********************************************************************/ 
float smpl_pid_node_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_node);
} 


/**********************************************************************
functionName:uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num)
description:获取指定通道的pid 指定点的kp值
**********************************************************************/ 
uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kp);
}

/**********************************************************************
functionName:uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
description:获取指定通道的pid 指定点的ki值
**********************************************************************/ 
uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_ki);
}
 
/**********************************************************************
functionName:uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
description:获取指定通道的pid 指定点的kd值
**********************************************************************/ 
uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kd);
} 


/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: 获取指定通道的标定表 指定点的值
**********************************************************************/
float smpl_tab_value_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].value);
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: 获取指定通道的标定表 指定点的码
**********************************************************************/
int32_t smpl_tab_code_get(uint8_t ch,uint8_t num)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].code);
}

/**********************************************************************
functionName:int32_t smpl_tab_zero_get(uint8_t ch)
description: 获取指定通道的零点值
**********************************************************************/
int32_t smpl_tab_zero_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.smpl_zero);
}  

/**********************************************************************
functionName:uint8_t smpl_tab_num_get(uint8_t ch)
description: 获取指定通道的标定表项数
**********************************************************************/
uint8_t smpl_tab_num_get(uint8_t ch)
{
	return(prm_str->prm_smpl[ch].prm_smpl_tab.table_num);
}   

/**********************************************************************
functionName:float smpl_cal(int32_t adc_code,uint8_t chn)
description: 计算传感器实际值
**********************************************************************/	
float smpl_cal(int32_t adc_code,uint8_t chn)
{
	PRM_SMPL_TAB_TypeDef *prm_smpl_tab;	//传感器标定表参数
	int32_t x1,x2;
	float y1,y2;
	uint8_t i;
	float tempf;
	prm_smpl_tab=get_smpl_tab(chn);
	//如果只有一个标定点
	if(prm_smpl_tab->table_num==1)		
	{
		x1=0;
		y1=0;
		x2=prm_smpl_tab->table_pt[0].code;;
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
description:获取指令方向
**********************************************************************/
INSTRCT_DIRE_TypeDef prv_ins_dir_get(void)
{
	return((INSTRCT_DIRE_TypeDef)(prv_str->instrct_dire));
} 

/**********************************************************************
functionName:uint16_t prvt_gain_get(uint8_t chn)
description:获取指定通道的增益
**********************************************************************/
uint16_t prv_gain_get(uint8_t chn)
{
	return(prv_str->gain[chn]);
}

/**********************************************************************
functionName:uint32_t prv_ele_gear_get(void)
description:获取电子齿轮
**********************************************************************/
uint32_t prv_ele_gear_get(void)
{
	return(prv_str->electronic_gear);
}

/**********************************************************************
functionName:uint32_t prv_devc_key_get(void)
description:获取设备钥匙
**********************************************************************/
uint32_t prv_devc_key_get(void)
{
	return(prv_str->devc_key);
}

  

 
/**********************************************************************
functionName:void devc_ip_get(void)
description: 获取设备IP参数,只改变内存的参数，不改变flash参数。
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
description:设置设备mac地址指针
**********************************************************************/
void devc_mac_set(uint8_t *mac)
{ 	
	uint8_t i;
	for(i=0;i<6;i++)
	prm_str->prm_devc.mac_addr[i]=mac[i];
}


/**********************************************************************
functionName: void ctrl_max_set(uint32_t dgr)
description: 设置最大控制量,返回参数地址
**********************************************************************/
void ctrl_max_set(uint32_t arg)
{ 	
	prm_str->prm_ctrl.ctrl_max=arg; 
} 


/**********************************************************************
functionName: void ctrl_max_cnt_get(void)
description: 获取允许最大控制量次数,返回参数地址
**********************************************************************/
void ctrl_max_cnt_set(uint16_t arg)
{ 
	prm_str->prm_ctrl.ctrl_max_cnt=arg; 
} 


/**********************************************************************
functionName:void ctrl_max_one_set(uint32_t arg)
description: 设置单次最大控制量,返回参数地址
**********************************************************************/
void ctrl_max_one_set(uint32_t arg)
{ 
	prm_str->prm_ctrl.ctrl_max_one=arg; 
}


/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: 设置单次最小控制量,返回参数地址
**********************************************************************/
void ctrl_min_one_set(uint16_t arg)
{ 
	prm_str->prm_ctrl.ctrl_min_one=arg; 
}	

/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: 设置开环控制量,返回参数地址
**********************************************************************/
void ctrl_open_set(uint32_t arg)
{ 
	prm_str->prm_ctrl.ctrl_open=arg; 
}	


/**********************************************************************
functionName:void ctrl_exc_dir_set(uint8_t arg)
description: 设置开环控制量,返回参数地址
**********************************************************************/
void ctrl_exc_dir_set(uint8_t arg)
{ 
	prm_str->prm_ctrl.exc_dir=arg; 
}

/**********************************************************************
functionName: void ctrl_min_one_set(uint16_t arg)
description: 设置开环控制量,返回参数地址
**********************************************************************/
void spcl_protect_set(uint16_t arg)
{ 
	prm_str->prm_spcl.protect_bit=arg; 
}

/**********************************************************************
functionName:uint16_t para_spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch)
description:设置卸载延时时间
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
description:设置指定通道的阀口初始位置 
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
description:设置指定通道的阀口回退开度
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
description:设置指定通道的阀口中位
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
description:设置指定通道的快上开度
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
description:设置指定通道的快下开度
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
description:设置指定通道的慢上开度
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
description:设置指定通道的快上开度
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
description:设置指定通道的控制入口
**********************************************************************/
void smpl_ctrl_entry_set(uint8_t ch,float entry)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_entry=entry;
}

/**********************************************************************
functionName:float smpl_ctrl_full_p(uint8_t ch)
description:设置指定通道的正向满量程
**********************************************************************/
void smpl_ctrl_full_p_set(uint8_t ch,float full_p)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_p=full_p;
}

/**********************************************************************
functionName:float smpl_ctrl_full_n(uint8_t ch)
description:设置指定通道的反向满量程
**********************************************************************/
void smpl_ctrl_full_n_set(uint8_t ch,float full_n)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.smpl_full_n=full_n;
}


/**********************************************************************
functionName:float cof_ahead_get(uint8_t chn)
description:设置保持提前系数
**********************************************************************/
void smpl_cof_ahead_set(uint8_t ch,float ahead)
{
 	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_ahead=ahead;
}

/**********************************************************************
functionName:float cof_spd_get(float error,uint8_t ch)
description:保持系数计算
**********************************************************************/
void smpl_cof_spd_set(uint8_t ch,float keep_spd)
{	 
	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_spd=keep_spd;
}

/**********************************************************************
functionName:float cof_break_get(uint8_t ch)
description:设置制动系数
**********************************************************************/
void smpl_cof_break_set(uint8_t ch,float keep_brk)
{ 
	prm_str->prm_smpl[ch].prm_smpl_ctrl.cof_keep_brk=keep_brk;
}

/**********************************************************************
functionName:float smpl_ctrl_crash_get(uint8_t ch)
description:设置指定通道的预碰撞安全值
**********************************************************************/
void smpl_ctrl_crash_set(uint8_t ch,float crash)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.crash_threshold=crash;
}

/**********************************************************************
functionName:float smpl_ctrl_overspeed_get(uint8_t ch)
description:设置传感器控制参数过速保护值
**********************************************************************/  
void smpl_ctrl_overspeed_set(uint8_t ch,float speed)
{
	prm_str->prm_smpl[ch].prm_smpl_ctrl.overspeed=speed;
}

/**********************************************************************
functionName:uint8_t smpl_pid_segs_get(uint8_t ch)
description:设置指定通道的pid段数
**********************************************************************/    
void smpl_pid_segs_set(uint8_t ch,uint8_t segs)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.smpl_pid_segs=segs;
}

/**********************************************************************
functionName:uint16_t smpl_pid_node_set(uint8_t ch,uint8_t num)
description:设置指定通道的pid 指定点的kp值
**********************************************************************/ 
void smpl_pid_node_set(uint8_t ch,uint8_t num,float node)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_node=node;
}

/**********************************************************************
functionName:uint16_t smpl_pid_kp_set(uint8_t ch,uint8_t num)
description:设置指定通道的pid 指定点的kp值
**********************************************************************/ 
void smpl_pid_kp_set(uint8_t ch,uint8_t num,uint16_t kp)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kp=kp;
}


/**********************************************************************
functionName:uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num)
description:设置指定通道的pid 指定点的ki值
**********************************************************************/ 
void smpl_pid_ki_set(uint8_t ch,uint8_t num,uint16_t ki)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_ki=ki;
}

/**********************************************************************
functionName:uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num)
description:设置指定通道的pid 指定点的kd值
**********************************************************************/ 
void smpl_pid_kd_set(uint8_t ch,uint8_t num,uint16_t kd)
{
	prm_str->prm_smpl[ch].prm_smpl_pid.pid_str[num].pid_kd=kd;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: 设置指定通道的标定表 指定点的值
**********************************************************************/
void smpl_tab_value_set(uint8_t ch,uint8_t num,float value)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].value=value;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_value_get(uint8_t ch,uint8_t num)
description: 设置指定通道的标定表 指定点的码
**********************************************************************/
void smpl_tab_code_set(uint8_t ch,uint8_t num,int32_t code)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_pt[num].code=code;
}

/**********************************************************************
functionName:int32_t smpl_tab_zero_get(uint8_t ch)
description: 设置指定通道的零点值
**********************************************************************/
void smpl_tab_zero_set(uint8_t ch,int32_t code)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.smpl_zero=code;
} 

/**********************************************************************
functionName:uint8_t smpl_tab_num_get(uint8_t ch)
description: 设置指定通道的标定表项数
**********************************************************************/
void smpl_tab_num_set(uint8_t ch,uint8_t table)
{
	prm_str->prm_smpl[ch].prm_smpl_tab.table_num=table;
} 

/**********************************************************************
functionName:INSTRCT_DIRE_TypeDef prv_ins_dir_get(void)
description:设置指令方向
**********************************************************************/
void prv_ins_dir_set(INSTRCT_DIRE_TypeDef dire)
{
	prv_str->instrct_dire=dire;
} 

/**********************************************************************
functionName:uint16_t prvt_gain_get(uint8_t ch)
description:设置指定通道的增益
**********************************************************************/
void prv_gain_set(uint8_t ch,uint16_t gain)
{
	prv_str->gain[ch]=gain;
}

/**********************************************************************
functionName:uint32_t prv_ele_gear_get(void)
description:设置电子齿轮
**********************************************************************/
void prv_ele_gear_set(uint32_t eg)
{
 	prv_str->electronic_gear=eg;
}

/**********************************************************************
functionName:uint32_t prv_devc_key_get(void)
description:设置设备钥匙
**********************************************************************/
void prv_devc_key_set(uint32_t key)
{
	prv_str->devc_key=key;
}

/**********************************************************************
functionName:uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len)
description:获取设备参数启始抵制和长度
**********************************************************************/  
uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len)
{
	*addr=0;
	*len=sizeof(PRM_DEVC_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_devc));
}  

/**********************************************************************
functionName:void prm_ctrl_info(uint32_t *addr,uint16_t *len)
description:获取控制参数开始抵制和长度
**********************************************************************/ 
uint8_t *prm_ctrl_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_ctrl)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_CTRL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_ctrl));
}


/**********************************************************************
functionName:void prm_spcl_info(uint32_t *addr,uint16_t *len)
description:获取特殊参数开始抵制和长度
**********************************************************************/ 
uint8_t *prm_spcl_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_spcl)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_SPCL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_spcl));
}


/**********************************************************************
functionName:uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len)
description:获取内部参数开始抵制和长度
**********************************************************************/ 
uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len)
{
	*addr=(uint32_t)&(prm_str->prm_insd)-(uint32_t)prm_ram;	
	*len=sizeof(PRM_INSD_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_insd));
}

/**********************************************************************
functionName:void prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch)
description:获取指定通道参数开始抵制和长度
**********************************************************************/ 
uint8_t *prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch)
{
	*addr=(uint32_t)&(prm_str->prm_smpl[ch])-(uint32_t)prm_ram;	
	*len=sizeof(PRM_SMPL_TypeDef);
	return ((uint8_t *)(void *)&(prm_str->prm_smpl[ch]));
}

/**********************************************************************
functionName:void prm_read(void)
description:从flash区读取系统参数 
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
description:从flash区读取特权参数
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
description:保存系统参数到flash
PRM_MEM_SIZE		//系统参数字节数
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
description:保存特权参数到flash
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
description:写参数内存的数据
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
description:写特权参数内存的数据 
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
description:下位机参数保存到SD卡
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
description:下位机参数从SD读取参数
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
description:特权参数保存到SD卡
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
description:特权参数从SD读取参数
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
description:系统参数备份到SD卡
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
description:特权参数备份到SD卡
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
description:系统参数从SD卡恢复,注意保留加密信息 
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
	 
	memcpy(info,prm_str->prm_insd.dvc_info,32); 		//读取加密信息内容
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
	memcpy(prm_str->prm_insd.dvc_info,info,32); 		//保留加密信息
	return FR_OK;
}

/**********************************************************************
functionName: FRESULT prv_recover(void)
description:特权参数从SD卡恢复 ，注意需要保留当前的密码
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
description:系统参数备份到SD卡（以文件夹形式存储）
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
description:系统特权参数备份到SD卡（以文件夹形式存储）
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
description:系统参数从SD卡恢复,注意保留加密信息 
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
	 
	memcpy(info,prm_str->prm_insd.dvc_info,32); 		//读取加密信息内容
	
	f_read(&file_obj,prm_ram,PRM_MEM_SIZE,&br);
	
	f_close(&file_obj);
	memcpy(prm_str->prm_insd.dvc_info,info,32); 		//保留加密信息
	
	return FR_OK;
}


/**********************************************************************
functionName: FRESULT prv_recover(void)
description:特权参数从SD卡恢复 ，注意需要保留当前的密码
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
