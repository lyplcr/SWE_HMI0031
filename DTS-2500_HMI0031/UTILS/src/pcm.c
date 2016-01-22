/**
  ******************************************************************************
  * @file    pcm.c
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
#include "pcm.h" 


//#define		USE_PCM_SD_SAVE
// #define		USE_PCM_INSIDE_FLASH_SAVE
 #define		USE_PCM_OUTSIDE_FLASH_SAVE

#define		PCM_DATA_BASE			(4<<10)

static __ALIGN_RAM uint8_t pcm_ram[PCM_MEM_SIZE] = {0};				//��λ������  


PCM_TypeDef *const pcm_str=(PCM_TypeDef *)(void *)pcm_ram; 



FRESULT pcm_save_sd(void);
FRESULT pcm_read_sd(void);

  
/**********************************************************************
functionName:void pcm_write(uint8_t *dat,uint16_t addr,uint16_t len)
description:д�����ڴ������
**********************************************************************/ 
void pcm_write(uint8_t *dat,uint16_t addr,uint16_t len)
{
	uint8_t *pcm;
	uint16_t j;
	pcm=pcm_ram;
	pcm+=addr;
	if((addr+len)>sizeof(PCM_TypeDef))
	len=sizeof(PCM_TypeDef)-addr;	
	for(j=0;j<len;j++)
	pcm[j]=dat[j]; 
}


/**********************************************************************
functionName:void pcm_read(void)
description:��flash����ȡ��λ������
**********************************************************************/
void pcm_read(void)
{  
	#if 	defined	USE_PCM_SD_SAVE
		pcm_read_sd();
	#elif 	defined USE_PCM_INSIDE_FLASH_SAVE
		eep_read(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE);
	#elif 	defined USE_PCM_OUTSIDE_FLASH_SAVE
		sf_ReadBuffer(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE); 
	#endif		
	
//	eep_read(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE); 
//	pcm_read_sd();	
//	sf_ReadBuffer(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE); 
}


/**********************************************************************
functionName:void pcm_save(void)
description:����ϵͳ������flash����������ִ��eep_erase�� 
**********************************************************************/ 
void pcm_save(void)
{
	#if 	defined	USE_PCM_SD_SAVE
		pcm_save_sd();
	#elif 	defined USE_PCM_INSIDE_FLASH_SAVE
		eep_write(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE);
	#elif 	defined USE_PCM_OUTSIDE_FLASH_SAVE
		sf_WriteBuffer(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE);
	#endif
	
//	eep_write(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE);	
//	pcm_save_sd();
//	sf_WriteBuffer(pcm_ram,PCM_DATA_BASE,PCM_MEM_SIZE);
}

#define		SAVE_SD_BUF_SIZE		64
/**********************************************************************
functionName: FRESULT pcm_save_sd(void) 
description:��λ���������浽SD��
**********************************************************************/ 
FRESULT pcm_save_sd(void) 								
{
	uint8_t *dat = NULL;
	uint32_t pcm_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	
	fresult=f_open(&file_obj,"para/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);

	if(fresult==FR_NO_PATH)
	{
		f_mkdir("para"); 
		fresult=f_open(&file_obj,"para/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);
		if(fresult!=FR_OK)
		return fresult; 	
	}
	else if(fresult!=FR_OK)	 
	return fresult; 
	
	dat=pcm_ram;
	pcm_len=PCM_MEM_SIZE;
	
	while(pcm_len)
	{
		if(pcm_len>SAVE_SD_BUF_SIZE)
		{
			f_write(&file_obj,dat,SAVE_SD_BUF_SIZE,&br);  
			dat+=SAVE_SD_BUF_SIZE;	
			pcm_len-=SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_write(&file_obj,dat,pcm_len,&br);  
			pcm_len=0;
		}
	}  
	f_close(&file_obj);

	return FR_OK;
}

/**********************************************************************
functionName: FRESULT pcm_read_sd(void)	
description:��λ��������SD��ȡ����
**********************************************************************/ 
FRESULT pcm_read_sd(void)							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat;
	uint32_t pcm_len=0; 
 
	fresult=f_open(&file_obj,"para/pcm.bin",FA_OPEN_EXISTING|FA_READ);
	if(fresult!=FR_OK)
	return fresult; 
	  
	dat = pcm_ram;
	pcm_len = PCM_MEM_SIZE; 
	
	while( pcm_len )
	{
		if (pcm_len > SAVE_SD_BUF_SIZE)
		{
			f_read(&file_obj,dat,SAVE_SD_BUF_SIZE,&br);
			dat += SAVE_SD_BUF_SIZE;
			pcm_len -= SAVE_SD_BUF_SIZE;
		}	
		else
		{
			f_read(&file_obj,dat,pcm_len,&br);
			pcm_len = 0;
		}	
	}  
	f_close(&file_obj); 
	
	return FR_OK;
}


#define		BACKUP_BUF_SIZE		64
/**********************************************************************
functionName: FRESULT pcm_backup(void) 
description:��λ���������ݵ�SD��
**********************************************************************/ 
FRESULT pcm_backup( uint8_t type ) 								
{
	uint8_t *dat = NULL;
	uint32_t pcm_len=0;
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	
	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("0:/sys"); 
				fresult=f_open(&file_obj,"0:/sys/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 	
			}
			else if(fresult!=FR_OK)
			{				
				return fresult;
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);
			if(fresult==FR_NO_PATH)
			{
				f_mkdir("1:/sys"); 
				fresult=f_open(&file_obj,"1:/sys/pcm.bin",FA_CREATE_ALWAYS|FA_WRITE);
				if(fresult!=FR_OK)
				return fresult; 	
			}
			else if(fresult!=FR_OK)
			{				
				return fresult;
			}
			break;
	} 
	 
	dat=pcm_ram;
	pcm_len=PCM_MEM_SIZE;
	while(pcm_len)
	{
		if(pcm_len>BACKUP_BUF_SIZE)
		{
			f_write(&file_obj,dat,BACKUP_BUF_SIZE,&br);  
			dat+=BACKUP_BUF_SIZE;	
			pcm_len-=BACKUP_BUF_SIZE;
		}	
		else
		{
			f_write(&file_obj,dat,pcm_len,&br);  
			pcm_len=0;
		}	
	}  
	f_close(&file_obj);
	return FR_OK;
}


/**********************************************************************
functionName: FRESULT pcm_recover(void)	
description:��λ��������SD���ָ�,����������Ϣ
char 		paseword_system[11];						//ϵͳ����  
**********************************************************************/ 
FRESULT pcm_recover( uint8_t type )							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	uint8_t *dat = NULL;
	uint32_t pcm_len=0; 
	char pass[11]; 
 
	switch ( type )
	{
		case BACKUP_SD:
			fresult=f_open(&file_obj,"0:/sys/pcm.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult; 
			}
			break;
		
		case BACKUP_USB:
			fresult=f_open(&file_obj,"1:/sys/pcm.bin",FA_OPEN_EXISTING|FA_READ);
			if(fresult!=FR_OK)
			{
				return fresult; 
			}
			break;
	}
	
	memcpy(pass,pcm_str->pcm_hmi.paseword_system,11);    	//��¼��ǰ����
	  
	dat = pcm_ram;
	pcm_len = PCM_MEM_SIZE; 
	
	while( pcm_len )
	{
		if (pcm_len > BACKUP_BUF_SIZE)
		{
			f_read(&file_obj,dat,BACKUP_BUF_SIZE,&br);
			dat += BACKUP_BUF_SIZE;
			pcm_len -= BACKUP_BUF_SIZE;
		}	
		else
		{
			f_read(&file_obj,dat,pcm_len,&br);
			pcm_len = 0;
		}	
	}  
	f_close(&file_obj); 
	
	memcpy(pcm_str->pcm_hmi.paseword_system,pass,11); 		//�ָ���ǰ���� 
	
	return FR_OK;
}



/**********************************************************************
functionName:HMI_TypeDef *pcm_hmi_get(void)
description:��ȡ�˻��������������ṹ��
**********************************************************************/ 
HMI_TypeDef *pcm_hmi_get(void)
{
	return(&(pcm_str->pcm_hmi));
}

/**********************************************************************
functionName:PCM_TypeDef *get_pcm(void)
description:��ȡ��λ��������ʼ��ַ
**********************************************************************/ 
uint8_t *get_pcm(void)
{
	return pcm_ram;
}


/**********************************************************************
functionName:TEST_TypeDef *pcm_test_get(uint8_t index)
description: ��ȡ������ز���
**********************************************************************/ 
TEST_TypeDef *pcm_test_get(uint8_t index)
{
	return(&(pcm_str->pcm_test[index]));
}


/**********************************************************************
functionName:STEP_TypeDef *pcm_step_get(uint8_t index)
description: ��ȡ����༭����
**********************************************************************/
STEP_TypeDef *pcm_step_get(uint8_t index)
{
	return(&(pcm_str->pcm_step[index]));
}

/**********************************************************************
functionName:MEMORY_TypeDef *pcm_memory_get(void)
description: ��ȡ�������
**********************************************************************/
MEMORY_TypeDef *pcm_memory_get(void)
{
	return(&(pcm_str->memory_func));
}
 



/**********************************************************************
functionName: FRESULT pcm_backup(void) 
description:��λ���������ݵ�SD��(�Ե�ǰ����Ϊ�ļ������б���)
**********************************************************************/ 
FRESULT PcmBackupWithDate( void ) 								
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

	usprintf(file_name,"sys/%04d%02d%02d/pcm.bin",t.usYear, t.ucMon, t.ucMday);
	fresult = f_open(&file_obj,file_name,FA_CREATE_ALWAYS|FA_WRITE);

	if(FR_OK != fresult)	
	{
		return fresult;
	} 

	f_write(&file_obj,pcm_ram,PCM_MEM_SIZE,&br);

	f_close(&file_obj);
	
// 	usprintf(file_name,"%04d%02d%02d",t.usYear, t.ucMon, t.ucMday);
// 	
// 	fresult = f_open(&file_obj,"sys/log.bin",FA_CREATE_NEW|FA_READ); 	//���ڲ����Ƿ����log.bin�ļ�
// 	if (FR_OK == fresult)	//SD��û�ж�Ӧ���ļ�
// 	{
// 		sys_log.index = 0;
// 		sys_log.num = 1;
// 				
// 		strcpy(sys_log.f_name[0],file_name);
// 	     
// 		for (i=1;i<10;i++)
// 		{
// 			sys_log.f_name[i][0] = '\0'; 
// 		}
// 	}
// 	else
// 	{
// 		fresult = f_open(&file_obj,"sys/log.bin",FA_OPEN_ALWAYS|FA_READ); 		//��
// 	
// 		if(fresult == FR_OK)	//��
// 		{
// 			f_read(&file_obj,&sys_log,sizeof(SYS_BACKUP_LOG_TypeDef),&br);

// 			sys_log.num++;
// 			sys_log.index++;		
// 			if (sys_log.index >= 10)
// 			{
// 				sys_log.index = 0;
// 			}
// 			
// 			strcpy(sys_log.f_name[sys_log.index],file_name);
// 		}
// 	}		
// 	
// 	f_close(&file_obj);	
// 	
// 	fresult = f_open(&file_obj,"sys/log.bin",FA_CREATE_ALWAYS|FA_WRITE);			//д
// 	
// 	f_write(&file_obj,&sys_log,sizeof(SYS_BACKUP_LOG_TypeDef),&br);  
// 	  
// 	f_close(&file_obj);
	
	return FR_OK;
}

//��ȡ���ݲ�����־��Ϣ
ErrorStatus GetBackupLog( SYS_BACKUP_LOG_TypeDef *log )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;

	fresult = f_open(&file_obj,"sys/log.bin",FA_OPEN_ALWAYS|FA_READ); 		//��
	
	if (FR_OK != fresult)
	{
		return ERROR;
	}
	
	f_read(&file_obj,log,sizeof(SYS_BACKUP_LOG_TypeDef),&br);
	
	f_close(&file_obj);
	
	return SUCCESS;
}




/**********************************************************************
functionName: FRESULT PcmRecoverWithDate(void)	
description:��λ��������SD���ָ�,����������Ϣ(���ļ�����ʽ)
char 		paseword_system[11];						//ϵͳ����  
**********************************************************************/ 
FRESULT PcmRecoverWithDate( const char *pFile_name )							
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char pass[11]; 
	char file_name[30];
	uint32_t name_buff = 0;
	
	name_buff = ustrtoul(pFile_name,0,10);
	
	usprintf(file_name,"sys/%08d/pcm.bin",name_buff);
	
	fresult = f_open(&file_obj,file_name,FA_OPEN_EXISTING|FA_READ);
	if (FR_OK != fresult)
	{
		return fresult;
	}	
	
	memcpy(pass,pcm_str->pcm_hmi.paseword_system,11);    	//��¼��ǰ����
	  
	f_read(&file_obj,pcm_ram,PCM_MEM_SIZE,&br);
	f_close(&file_obj); 
	
	memcpy(pcm_str->pcm_hmi.paseword_system,pass,11); 		//�ָ���ǰ���� 
	
	return FR_OK;
}


 /**********************************************************************
functionName: FRESULT CheckBackUpFileExist( const char *pFile_name )
description: ���������ļ��Ƿ����  YES�����ڣ� NO��������  
**********************************************************************/ 
BoolStatus CheckBackUpFileExist( const char *pFile_name )
{
	char file_name[30];
	DIR dir_object;
	uint32_t name_buff = 0;
	
	name_buff = ustrtoul(pFile_name,0,10);
	
	usprintf(file_name,"sys/%08d",name_buff);
	
	if (FR_OK == f_opendir(&dir_object,file_name))
	{
		return YES;
	}
	
	return NO;
}


/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/ 
