/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: report.c
** ����޸�����: 2014/5/6 15:34:42
** ��        ��: �θ߱���⺯��
** ��	     ��: V1.1
** ��  ��  о Ƭ:STM32F103 ����Ƶ��:8.00MHz
** IDE:MDK4.12 
1���������飬����Ѿ��д��ļ��������ļ����ڣ�����д���ļ�
2������ָ���ļ������ļ������ҷ����ļ��洢ʱ��
3�������ļ��洢ʱ����ļ�
4��������ͨ�������������

**********************************************************************************************************/
#include "bsp.h"
#include "report.h"
  
    

#define 	PATH_BUF_SIZE   50 	  
  
  
typedef struct
{
    uint8_t test_type;
    char *path;
}
TEST_PATH_TypeDef; 

//�����ŵ�·��
TEST_PATH_TypeDef test_path[] =
{
    {NONE_TEST, "0:/"				},
    {KYSNJS,	"0:test/KYSNJS"		},
    {KYJZSJ, 	"0:test/KYJZSJ"		},
    {KYHNT, 	"0:test/KYHNT"		},
    {KZHNT, 	"0:test/KZHNT"		}, 
    {KYQQZ, 	"0:test/KYQQZ"		},
	{KYZJDH, 	"0:test/KYZJDH"		},
    {KYTY, 		"0:test/KYTY"		},
    {KZSNJS, 	"0:test/KZSNJS"		}, 
    {KZYJSNJ, 	"0:test/KZYJSNJ"	}, 
    {KZTY, 		"0:test/KZTY"		},
	{KLJSSW,	"0:test/KLJSSW"		},
};

TEST_PATH_TypeDef test_path_usb[] =
{
    {NONE_TEST, "1:/"				},
    {KYSNJS,  	"1:/test/KYSNJS"	},
    {KYJZSJ,  	"1:/test/KYJZSJ"	},
    {KYHNT,		"1:/test/KYHNT"		},
    {KZHNT,   	"1:/test/KZHNT"		}, 
    {KYQQZ,    	"1:/test/KYQQZ"		},
	{KYZJDH,  	"1:/test/KYZJDH"	},
    {KYTY,      "1:/test/KYTY"		},
    {KZSNJS,  	"1:/test/KZSNJS"	}, 
    {KZYJSNJ,	"1:/test/KZYJSNJ"	}, 
    {KZTY, 		"1:/test/KZTY"		},
	{KLJSSW, 	"1:/test/KLJSSW"	},
};


static char Lfname[(_MAX_LFN * 2) + 1] = {0};

uint8_t GetDotNum( const char *p );

/**********************************************************************
functionName:float get_free_space(void)
description:��ȡSD��ʣ��ռ䣬��λ��Mb
**********************************************************************/ 
FRESULT get_free_space(float *space)
{
	FRESULT fresult;
	unsigned long free_space=0; 
	FATFS *fs;
	fresult = f_getfree("/", &free_space, &fs);
	if(fresult != FR_OK)
  {
      *space=0;
      return(fresult);
  } 
	*space=free_space * fs->csize /2048.0f;		//֮ǰ�õ�ʱ0.04�汾��FATFSϵͳ��0.07E�汾���ļ�ϵͳ��һ�� 
	return FR_OK; 
}

 
/**********************************************************************
functionName:void report_get_path(uint8_t type,char *path)
description:��ȡָ������ľ���·�� ��type���������� *pathҪ�õ���·��
**********************************************************************/ 
void report_get_path(uint8_t type,char *path)
{
	strcpy(path,test_path[type].path);
}



/**********************************************************************
functionName:FRESULT report_save(char *type,char *file,REPORT_TypeDef *report)
description:������,type���������� file:�ļ��� report:ʵ�鱨��
typedef struct
{
	uint8_t index;		 
	uint8_t type[10];
	char fname[10][9];
	tTime time[10];
}
REPORT_LOG_TypeDef; 
**********************************************************************/ 
FRESULT report_save(uint8_t type,const char *file,const REPORT_TypeDef *report)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char path[PATH_BUF_SIZE];  	
//	REPORT_LOG_TypeDef xlog;
//	uint8_t i=0;
	  
	usprintf(path,"%s/%s.bin",test_path[type].path,file);  
//	fresult=f_open(&file_obj,path,FA_CREATE_NEW|FA_WRITE); 
	fresult=f_open(&file_obj,path,FA_CREATE_ALWAYS|FA_WRITE); 	//By.SY 
	if(fresult==FR_NO_PATH)
	{	 
		fresult=f_mkdir("/test"); 
		fresult=f_mkdir(test_path[type].path); 	  
		fresult=f_open(&file_obj,path,FA_CREATE_NEW|FA_WRITE);  
		if(fresult!=FR_OK)
		return fresult; 
	}
	else if(fresult!=FR_OK)
	return 	fresult;
	fresult=f_write(&file_obj,report,sizeof(REPORT_TypeDef),&br);   
	f_close(&file_obj);
	
// 	fresult=f_open(&file_obj,"test/log.bin",FA_CREATE_NEW|FA_READ); 
// 	
// 	if (FR_OK == fresult)	//SD��û�ж�Ӧ���ļ�	By:SY
// 	{
// 		xlog.index=0;
// 		xlog.type[0]=type;
// 		strcpy(xlog.fname[0],file);
// 		//date_get(&(xlog.time[0]));	       
// 		xlog.time[0]=get_time();
// 		for(i=1;i<10;i++)
// 		xlog.fname[i][0]=0; 
// 	}
// 	else
// 	{
// 		fresult=f_open(&file_obj,"test/log.bin",FA_OPEN_ALWAYS|FA_READ); 
// 		if(fresult==FR_OK)
// 		{
// 			f_read(&file_obj,&xlog,sizeof(REPORT_LOG_TypeDef),&br); 
// 			xlog.index++;
// 			if(xlog.index>=10)
// 			xlog.index=0; 	  
// 			xlog.type[xlog.index]=type;
// 			strcpy(xlog.fname[xlog.index],file);
// 			 
// 			//date_get(&(xlog.time[xlog.index]));  
// 			//time_get(&(xlog.time[xlog.index]));
// 			xlog.time[xlog.index]=get_time();
// 		}
// 	}
// 		
// 	f_close(&file_obj);		
// 	fresult=f_open(&file_obj,"test/log.bin",FA_CREATE_ALWAYS|FA_WRITE);
// 	f_write(&file_obj,&xlog,sizeof(REPORT_LOG_TypeDef),&br);  
// 	f_close(&file_obj);	 
	return fresult; 
}
 
 
 
/**********************************************************************
functionName:FRESULT report_lately_10(REPORT_LOG_TypeDef rlog)
description: ��ȡ���10��ʵ�����Ϣ
**********************************************************************/  
FRESULT report_lately_10(REPORT_LOG_TypeDef *rlog)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;   
	fresult=f_open(&file_obj,"test/log.bin",FA_OPEN_ALWAYS|FA_READ); 
	if(fresult!=FR_OK)
	return fresult; 
	fresult=f_read(&file_obj,rlog,sizeof(REPORT_LOG_TypeDef),&br); 	
	if(fresult!=FR_OK)
	return fresult; 
	f_close(&file_obj);	 
	return FR_OK; 
}
 
 
/**********************************************************************
functionName:FRESULT report_read(uint8_t type,char *file,REPORT_TypeDef *report)
description:��ȡ���,type���������� file:�ļ��� report:ʵ�鱨��
**********************************************************************/ 
FRESULT report_read(uint8_t type,const char *file,REPORT_TypeDef *report)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br; 
	char path[PATH_BUF_SIZE];
	
	if(type>=SUPPORT_TEST_NUM)
	return FR_INVALID_OBJECT;			//chengs120717
		
	usprintf(path,"%s/%s.bin",test_path[type].path,file); 
	fresult=f_open(&file_obj,path,FA_OPEN_EXISTING|FA_READ); 
	if(fresult!=FR_OK)
	return fresult;  
	f_read(&file_obj,report,sizeof(REPORT_TypeDef),&br); 
	f_close(&file_obj);	 
	return FR_OK;  
}

 

/**********************************************************************
functionName:FRESULT report_delete(uint8_t type,char *file)
description:ɾ��ָ����������ָ���ļ������ļ������type=0xFF��ɾ��ȫ���ļ�
**********************************************************************/ 
FRESULT report_delete(uint8_t type,const char *file)
{
	FRESULT fresult=FR_OK;
//	FIL file_obj;  
//	uint32_t br; 
	char path[PATH_BUF_SIZE];   
	uint8_t i=0; 
//	uint8_t j=0; 
//	uint8_t f=0;
// 	REPORT_LOG_TypeDef xlog;
// 	report_lately_10(&xlog);
	if(type==0xFF)
	{
		for(i=KYSNJS;i<=SUPPORT_TEST_NUM-1;i++)
		{
			usprintf(path,"%s/%s.bin",test_path[i].path,file);
			fresult=f_unlink(path); 
// 			if((fresult==FR_OK)||(fresult==FR_NO_FILE)||(fresult==FR_NO_PATH))
// 			{
// 				for(j=0;j<10;j++)
// 				{ 
// 					if((!strcmp(file,xlog.fname[j]))&&(i==xlog.type[j]))
// 					{
// 						xlog.fname[j][0]=0;
// 						if(!j)
// 						xlog.index=9;
// 						else
// 						xlog.index=j-1;	
// 						f=1;
// 					}
// 				} 
// 			}	
		} 
		fresult=FR_OK;
	}	
	else if(type<=SUPPORT_TEST_NUM-1)
	{
		usprintf(path,"%s/%s.bin",test_path[type].path,file);
		fresult=f_unlink(path);
// 		if((fresult==FR_OK)||(fresult==FR_NO_FILE)||(fresult==FR_NO_PATH))
// 		{
// 			for(j=0;j<10;j++)
// 			{
// 				if((!strcmp(file,xlog.fname[j]))&&(type==xlog.type[j]))
// 				{
// 					xlog.fname[j][0]=0; 
// 					if(!j)
// 					xlog.index=9;
// 					else
// 					xlog.index=j-1;
// 					f=1;
// 				}
// 			}	 
// 		}
	}	
// 	if(f)
// 	{
// 		fresult=f_open(&file_obj,"test/log.bin",FA_CREATE_ALWAYS|FA_WRITE);
// 		f_write(&file_obj,&xlog,sizeof(REPORT_LOG_TypeDef),&br);  
// 		f_close(&file_obj);	  
// 	}	 
	return fresult;  
}
 
/**********************************************************************
functionName:FRESULT report_exist(uint8_t type,char *file)
description:��ѯָ����������ָ���ļ������ļ��Ƿ����
**********************************************************************/  
FRESULT report_exist(uint8_t type,const char *file)
{
	FRESULT fresult=FR_OK;
	FIL file_obj;  
	char path[PATH_BUF_SIZE];   
	usprintf(path,"%s/%s.bin",test_path[type].path,file); 
	fresult=f_open(&file_obj,path,FA_OPEN_EXISTING|FA_READ); 
	f_close(&file_obj);	 
	return fresult;
}  

 
/**********************************************************************
functionName:FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
description: ����ʱ��β�ѯ����
type:��������
date_start����ʼ����  date_end����������
offset ƫ����
test:��෵��ƫ�ƺ��10��������Ϣ
fnumb:���ز��ҵ����ļ����� 
**********************************************************************/ 
FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
{
	DIR dir_object;  
	FILINFO file_info;
	FRESULT fresult; 
	uint8_t i=0;  
	uint8_t j=0;  	  
	uint32_t start=0;
	uint32_t end=0;		//��ʼ�ͽ���ʱ��
	uint32_t temp=0;
	uint8_t long_file_name = 0;	//���ļ�����־
	uint8_t dot_num = 0;
	
	#if _USE_LFN								//BY.SY
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
     #endif
	
	tTime2fstime(date_start,&start);
	tTime2fstime(date_end,&end);
	*fnumb=0;

	if((type>SUPPORT_TEST_NUM-1)||(!type))
	return 	FR_NO_FILE;
		 
	fresult = f_opendir(&dir_object,test_path[type].path); 	
	if(fresult != FR_OK)
	return fresult; 
	
	for(;;)
	{
		fresult = f_readdir(&dir_object, &file_info);
		if(fresult != FR_OK)
		return fresult; 
		
		if(!file_info.fname[0])		 
		break;	 
		
		long_file_name = (file_info.lfname[0] == 0) ? 0 : 1;	//�ж��Ƿ��ǳ��ļ���
		
		if(file_info.fattrib&AM_ARC)		//�Ǹ��ļ�
		{
			temp=file_info.fdate;
			temp<<=16;
			//temp|=file_info.ftime;
			if((temp>=start)&&(temp<=end))
			{
				*fnumb+=1;
				if(offset)
			 	{ 
			 		offset--;
			 	}	
			 	else if(j<10)
			 	{		 						
					if ( !long_file_name )
					{
						dot_num = GetDotNum(file_info.fname);
						for(i=0;i<8;i++)
						{ 
							if (file_info.fname[i] == '.')
							{
								if (dot_num == 1)
								{
									break;
								}
								else
								{
									dot_num--;
								}
							}
							
							test->fname[i]=file_info.fname[i];
						} 
						test->fname[i]=0;
					}
					else
					{
						dot_num = GetDotNum(file_info.lfname);
						
						for(i=0;i<16;i++)
						{ 
							if (file_info.lfname[i] == '.')
							{
								if (dot_num == 1)
								{
									break;
								}
								else
								{
									dot_num--;
								}
							}
							
							test->fname[i]=file_info.lfname[i];
						} 
						test->fname[i]=0;
					}
					
			 		fstime2tTime((tTime *)&(test->time),file_info.fdate,file_info.ftime);
			 		test++;
			 		j++;  
			 	}	 
			}	
		}	
	}
	for(i=0;i<16;i++)
	test->fname[i]=0; 
	return FR_OK;   
}  
   
    

/**********************************************************************
functionName:FRESULT report_search_subsn(uint8_t type,char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
description:ͨ�������Ų��ҽ��,��ָ������������������ֻҪ������subsn���е��ַ�������������
type:��������
subsn:�����ļ����а�����subsn���ַ���
offset ƫ������һ��ֻ������10���ļ�����������������飬��Ҫʹ�ô˱��������ǰ����ļ�
test:��෵��ƫ�ƺ��10��������Ϣ,test��ʱ��ʱ�������11�����飬���һ�������ļ���ȫ��Ϊ0��ʾ������
fnumb:���ز��ҵ����ļ����� 
**********************************************************************/ 
FRESULT report_search_sn(uint8_t type,const char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
{
	DIR dir_object;  
	FILINFO file_info; 
	FRESULT fresult; 
	uint8_t i=0;  
	uint8_t j=0; 
	uint8_t long_file_name = 0;	//���ļ�����־
	uint8_t dot_num = 0;
	
	#if _USE_LFN
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
    #endif
	
	*fnumb=0;
	if((type>SUPPORT_TEST_NUM-1)||(!type))
	return 	FR_NO_FILE;
	else
	{
		fresult = f_opendir(&dir_object,test_path[type].path);
		if(fresult != FR_OK)
		return fresult; 
		for(;;)
		{
			fresult = f_readdir(&dir_object, &file_info);
			if(fresult != FR_OK)
			return fresult; 
			if(!file_info.fname[0])				//����ļ����ǿյģ������ļ� 
			break;	
			
			long_file_name = (file_info.lfname[0] == 0) ? 0 : 1;	//�ж��Ƿ��ǳ��ļ���
						
			if(file_info.fattrib&AM_ARC)		//�����һ���ļ�
			{ 
				if ( !long_file_name )
				{					
					if(ustrstr(file_info.fname,subsn)!=NULL)
					{
						*fnumb+=1;
						if(offset)
					 	{ 
					 		offset--;
					 	}	
					 	else if(j<10)
					 	{
							dot_num = GetDotNum(file_info.fname);
							
					 		for(i=0;i<8;i++)
					 		{ 
					 			if (file_info.fname[i] == '.')
								{
									if (dot_num == 1)
									{
										break;
									}
									else
									{
										dot_num--;
									}
								}
								
								test->fname[i]=file_info.fname[i];
					 		} 
					 		test->fname[i]=0;		
					 		fstime2tTime((tTime *)&(test->time),file_info.fdate,file_info.ftime);
					 		test++;
					 		j++; 
					 	} 
					}
				}
				else
				{
					if(ustrstr(file_info.lfname,subsn)!=NULL)
					{
						*fnumb+=1;
						if(offset)
					 	{ 
					 		offset--;
					 	}	
					 	else if(j<10)
					 	{
					 		dot_num = GetDotNum(file_info.lfname);
							for(i=0;i<16;i++)
					 		{ 
					 			if (file_info.lfname[i] == '.')
								{
									if (dot_num == 1)
									{
										break;
									}
									else
									{
										dot_num--;
									}
								}
								
								test->fname[i]=file_info.lfname[i];
					 		} 
					 		test->fname[i]=0;		
					 		fstime2tTime((tTime *)&(test->time),file_info.fdate,file_info.ftime);
					 		test++;
					 		j++; 
					 	} 
					}
				}					
			}	 
		}
		for(i=0;i<16;i++)
		test->fname[i]=0;	
	} 
	return FR_OK;  
}

/*------------------------------------------------------------
 * Function Name  : GetDotNum
 * Description    : ��ȡ�ַ�����С�������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetDotNum( const char *p )
{
	uint8_t i = 0;
	uint8_t num = 0;
	
	while ( p[i] != NULL)
	{
		if (p[i] == '.')
		{
			num++;
		}
		i++;
	}
	
	return num;
}


/**********************************************************************
functionName:FRESULT report_save(char *type,char *file,REPORT_TypeDef *report)
description:������,type���������� file:�ļ��� report:ʵ�鱨��
**********************************************************************/ 
FRESULT report_save_usb(uint8_t type,const char *file,REPORT_TypeDef *report)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char path[PATH_BUF_SIZE];  	
	  
	usprintf(path,"%s/%s.bin",test_path_usb[type].path,file);  
	fresult=f_open(&file_obj,path,FA_CREATE_ALWAYS|FA_WRITE); 
	if(fresult==FR_NO_PATH)
	{	 
		fresult=f_mkdir("1:/test"); 
		fresult=f_mkdir(test_path_usb[type].path); 	  
		fresult=f_open(&file_obj,path,FA_CREATE_NEW|FA_WRITE);  
		if(fresult!=FR_OK)
		return fresult; 
	}
	else if(fresult!=FR_OK)
	return 	fresult;
	
	fresult=f_write(&file_obj,report,sizeof(REPORT_TypeDef),&br);   
	f_close(&file_obj);

	return fresult; 
}


/**********************************************************************
functionName:FRESULT report_save_usb_set_time (uint8_t type, char *file)
description:���õ���������ļ�ʱ��
**********************************************************************/ 
FRESULT report_save_usb_set_time (uint8_t type, const char *file)
{
	char path_sd[PATH_BUF_SIZE]; 
	char path_usb[PATH_BUF_SIZE]; 
	FILINFO file_info;
	FRESULT result;
	
	usprintf(path_sd,"%s/%s.bin",test_path[type].path,file);	
	result = f_stat(path_sd,&file_info);
	if (result != FR_OK)
	{
		return result;
	}
	
	usprintf(path_usb,"%s/%s.bin",test_path_usb[type].path,file);	
	result = f_utime(path_usb,&file_info);
	
	return result;
}


/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE************************************/ 
