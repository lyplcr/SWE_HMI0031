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
TEST_PATH_TypeDef testPathSD[] =
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

/* ������USB��·�� */
TEST_PATH_TypeDef testPathUSB[] =
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

/* ������SD���������·�� */
TEST_PATH_TypeDef g_coordinatePointPath_SD[] =
{
    {NONE_TEST, "0:/"				},
    {KYSNJS,	"0:curve/KYSNJS"	},
    {KYJZSJ, 	"0:curve/KYJZSJ"	},
    {KYHNT, 	"0:curve/KYHNT"		},
    {KZHNT, 	"0:curve/KZHNT"		}, 
    {KYQQZ, 	"0:curve/KYQQZ"		},
	{KYZJDH, 	"0:curve/KYZJDH"	},
    {KYTY, 		"0:curve/KYTY"		},
    {KZSNJS, 	"0:curve/KZSNJS"	}, 
    {KZYJSNJ, 	"0:curve/KZYJSNJ"	}, 
    {KZTY, 		"0:curve/KZTY"		},
	{KLJSSW,	"0:curve/KLJSSW"	},
};

/* ������SD���������·�� */
TEST_PATH_TypeDef g_coordinatePointPath_USB[] =
{
    {NONE_TEST, "1:/"				},
    {KYSNJS,	"1:curve/KYSNJS"	},
    {KYJZSJ, 	"1:curve/KYJZSJ"	},
    {KYHNT, 	"1:curve/KYHNT"		},
    {KZHNT, 	"1:curve/KZHNT"		}, 
    {KYQQZ, 	"1:curve/KYQQZ"		},
	{KYZJDH, 	"1:curve/KYZJDH"	},
    {KYTY, 		"1:curve/KYTY"		},
    {KZSNJS, 	"1:curve/KZSNJS"	}, 
    {KZYJSNJ, 	"1:curve/KZYJSNJ"	}, 
    {KZTY, 		"1:curve/KZTY"		},
	{KLJSSW,	"1:curve/KLJSSW"	},
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
	strcpy(path,testPathSD[type].path);
}

/**********************************************************************
functionName:FRESULT report_save(char *type,char *file,REPORT_TypeDef *report)
description:������,type���������� file:�ļ��� report:ʵ�鱨��
**********************************************************************/ 
FRESULT report_save(uint8_t pdrv, uint8_t type, const char * const file, const REPORT_TypeDef *report)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char path[PATH_BUF_SIZE];  	
	
	switch ( pdrv )
	{
		case MMC:
			usprintf(path,"%s/%s.bin",testPathSD[type].path,file); 
			break;
		case USB:
			usprintf(path,"%s/%s.bin",testPathUSB[type].path,file);
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	
	fresult = f_open(&file_obj,path,FA_CREATE_ALWAYS|FA_WRITE); 
	
	if (fresult == FR_NO_PATH)
	{	 
		switch ( pdrv )
		{
			case MMC:
				fresult = f_mkdir("0:/test"); 
				fresult = f_mkdir(testPathSD[type].path); 
				break;
			case USB:
				fresult = f_mkdir("1:/test"); 
				fresult = f_mkdir(testPathUSB[type].path);
				break;
			default:
				return FR_INVALID_DRIVE;
		}		 	  
		fresult = f_open(&file_obj,path,FA_CREATE_NEW|FA_WRITE);  
		
		if (fresult != FR_OK)
		{
			return fresult; 
		}
	}
	else if(fresult != FR_OK)
	{
		return fresult;
	}
	
	fresult = f_write(&file_obj,report,sizeof(REPORT_TypeDef),&br);   
	
	f_close(&file_obj);

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
FRESULT report_read(uint8_t type,const char * const file,REPORT_TypeDef *report)
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br; 
	char path[PATH_BUF_SIZE];
	
	if(type>=SUPPORT_TEST_NUM)
	return FR_INVALID_OBJECT;			//chengs120717
		
	usprintf(path,"%s/%s.bin",testPathSD[type].path,file); 
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
FRESULT report_delete(uint8_t type,const char * const file)
{
	FRESULT fresult=FR_OK;
	char path[PATH_BUF_SIZE];   
	uint8_t i=0; 

	if(type==0xFF)
	{
		for(i=KYSNJS;i<=SUPPORT_TEST_NUM-1;i++)
		{
			usprintf(path,"%s/%s.bin",testPathSD[i].path,file);
			fresult=f_unlink(path); 	
		} 
		fresult=FR_OK;
	}	
	else if(type<=SUPPORT_TEST_NUM-1)
	{
		usprintf(path,"%s/%s.bin",testPathSD[type].path,file);
		fresult=f_unlink(path);
	}	
 
	return fresult;  
}
 
/**********************************************************************
functionName:FRESULT report_exist(uint8_t type,char *file)
description:��ѯָ����������ָ���ļ������ļ��Ƿ����
**********************************************************************/  
FRESULT report_exist(uint8_t type,const char * const file)
{
	FRESULT fresult=FR_OK;
	FIL file_obj;  
	char path[PATH_BUF_SIZE];   
	usprintf(path,"%s/%s.bin",testPathSD[type].path,file); 
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
		 
	fresult = f_opendir(&dir_object,testPathSD[type].path); 	
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
FRESULT report_search_sn(uint8_t type,const char * const subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
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
		fresult = f_opendir(&dir_object,testPathSD[type].path);
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

/*------------------------------------------------------------
 * Function Name  : SetReportSaveTime
 * Description    : ���ñ��汣��ʱ��
 * Input          : sourcePdrv��Դ�豸�ţ�targetPdrv��Ŀ���豸�ţ�testType���������ͣ�file���ļ���
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT SetReportSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, \
			const char * const file)
{
	char path_sd[PATH_BUF_SIZE]; 
	char path_usb[PATH_BUF_SIZE]; 
	FILINFO file_info;
	FRESULT result;
	
	if (testType >= SUPPORT_TEST_NUM)
	{
		return FR_INVALID_NAME;
	}
	
	#if _USE_LFN
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
    #endif
	
	switch ( sourcePdrv )
	{
		case MMC:
			usprintf(path_sd,"%s/%s.bin",testPathSD[testType].path,file);
			result = f_stat(path_sd,&file_info);
			break;
		case USB:
			usprintf(path_usb,"%s/%s.bin",testPathUSB[testType].path,file);	
			result = f_stat(path_usb,&file_info);
			break;
		default:
			return FR_INVALID_DRIVE;
	}

	if (result != FR_OK)
	{
		return result;
	}
	
	switch ( targetPdrv )
	{
		case MMC:
			usprintf(path_sd,"%s/%s.bin",testPathSD[testType].path,file); 
			result = f_utime(path_sd,&file_info);
			break;
		case USB:
			usprintf(path_usb,"%s/%s.bin",testPathUSB[testType].path,file);	
			result = f_utime(path_usb,&file_info);
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	
	return result;
}

/*------------------------------------------------------------
 * Function Name  : SetCurveSaveTime
 * Description    : �������߱���ʱ��
 * Input          : sourcePdrv��Դ�豸�ţ�targetPdrv��Ŀ���豸�ţ�testType���������ͣ�file���ļ���
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT SetCurveSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial)
{
	FILINFO file_info;
	FRESULT fresult;
	char pathFile[PATH_BUF_SIZE];  
	const uint8_t MAX_TEST_SERIAL_LEN = 16;		//һ��������ų���
	const uint8_t MAX_SAMPLE_NUM = 20;			//����Կ����
	const uint8_t MAX_CURVE_SERIAL_LEN = 20;	
	char fileSerialBuff[MAX_CURVE_SERIAL_LEN];	
	char tempBuff[5];
	
	if ( (strlen(pSerial) > MAX_TEST_SERIAL_LEN) 	||\
		(sampleNum > MAX_SAMPLE_NUM) 				||\
		(testType >= SUPPORT_TEST_NUM)	)
	{
		return FR_INVALID_NAME;
	}
	
	#if _USE_LFN
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
    #endif
	
	usprintf(tempBuff,"_%02d",sampleNum);	
	strcpy(fileSerialBuff,pSerial);
	strcat(fileSerialBuff,tempBuff);
	
	switch ( sourcePdrv )
	{
		case MMC:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_SD[testType].path,pSerial,fileSerialBuff);   
			fresult = f_stat(pathFile,&file_info);
			break;
		case USB:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_USB[testType].path,pSerial,fileSerialBuff);  
			fresult = f_stat(pathFile,&file_info);
			break;
		default:
			return FR_INVALID_DRIVE;
	}

	if (fresult != FR_OK)
	{
		return fresult;
	}
	
	switch ( targetPdrv )
	{
		case MMC:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_SD[testType].path,pSerial,fileSerialBuff);   
			fresult = f_utime(pathFile,&file_info);
			break;
		case USB:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_USB[testType].path,pSerial,fileSerialBuff);
			fresult = f_utime(pathFile,&file_info);
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	
	return fresult;
}

/*------------------------------------------------------------
 * Function Name  : SaveCoordinatePointToSD
 * Description    : �����������Ϣ��SD��
 * Input          : pdrv���豸�ţ�testType���������ͣ�sampleNum���Կ���ţ�pSerial���Լ���ţ�pCoordinate���������Ϣ
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT SaveCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, const COORDINATE_POINT_TypeDef * const pCoordinate )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char pathFile[PATH_BUF_SIZE];  
	char pathSerial[PATH_BUF_SIZE];
	const uint8_t MAX_TEST_SERIAL_LEN = 16;		//һ��������ų���
	const uint8_t MAX_SAMPLE_NUM = 20;			//����Կ����
	const uint8_t MAX_CURVE_SERIAL_LEN = 20;	
	char fileSerialBuff[MAX_CURVE_SERIAL_LEN];	
	char tempBuff[5];
	
	if ( (strlen(pSerial) > MAX_TEST_SERIAL_LEN) 	||\
		(sampleNum > MAX_SAMPLE_NUM) 				||\
		(testType >= SUPPORT_TEST_NUM)	)
	{
		return FR_INVALID_NAME;
	}
	
	usprintf(tempBuff,"_%02d",sampleNum);	
	strcpy(fileSerialBuff,pSerial);
	strcat(fileSerialBuff,tempBuff);
	
	switch ( pdrv )
	{
		case MMC:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_SD[testType].path,pSerial,fileSerialBuff);   
			break;
		case USB:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_USB[testType].path,pSerial,fileSerialBuff);   
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	
	fresult = f_open(&file_obj,pathFile,FA_CREATE_ALWAYS|FA_WRITE); 
	
	if (fresult == FR_NO_PATH)
	{	
		switch ( pdrv )
		{
			case MMC:
				fresult = f_mkdir("0:/curve"); 
				fresult = f_mkdir(g_coordinatePointPath_SD[testType].path); 	
				usprintf(pathSerial,"%s/%s",g_coordinatePointPath_SD[testType].path,pSerial);  
				break;
			case USB:
				fresult = f_mkdir("1:/curve"); 
				fresult = f_mkdir(g_coordinatePointPath_USB[testType].path); 	
				usprintf(pathSerial,"%s/%s",g_coordinatePointPath_USB[testType].path,pSerial); 
				break;
			default:
				f_close(&file_obj);
			
				return FR_INVALID_DRIVE;
		}
		 	
		fresult = f_mkdir(pathSerial); 	
		
		fresult = f_open(&file_obj,pathFile,FA_CREATE_NEW|FA_WRITE);  		
	}
	
	if (fresult != FR_OK)
	{
		f_close(&file_obj);
		
		return fresult; 
	}
	
	fresult = f_write(&file_obj,pCoordinate,sizeof(COORDINATE_POINT_TypeDef),&br);   
	
	f_close(&file_obj);

	return fresult; 
}

/*------------------------------------------------------------
 * Function Name  : ReadCoordinatePointFromSD
 * Description    : ��SD����ȡ�������Ϣ
 * Input          : pdrv���豸�ţ�testType���������ͣ�sampleNum���Կ���ţ�pSerial���Լ���ţ�pCoordinate���������Ϣ
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT ReadCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, COORDINATE_POINT_TypeDef * const pCoordinate )
{
	FRESULT fresult;
	FIL file_obj;  
	uint32_t br;  
	char pathFile[PATH_BUF_SIZE];  
	const uint8_t MAX_TEST_SERIAL_LEN = 16;		//һ��������ų���
	const uint8_t MAX_SAMPLE_NUM = 20;			//����Կ����
	const uint8_t MAX_CURVE_SERIAL_LEN = 20;	
	char fileSerialBuff[MAX_CURVE_SERIAL_LEN];	
	char tempBuff[5];
	
	if ( (strlen(pSerial) > MAX_TEST_SERIAL_LEN) 	||\
		(sampleNum > MAX_SAMPLE_NUM) 				||\
		(testType >= SUPPORT_TEST_NUM)	)
	{
		return FR_INVALID_NAME;
	}
	
	usprintf(tempBuff,"_%02d",sampleNum);	
	strcpy(fileSerialBuff,pSerial);
	strcat(fileSerialBuff,tempBuff);
	
	switch ( pdrv )
	{
		case MMC:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_SD[testType].path,pSerial,fileSerialBuff);  
			break;
		case USB:
			usprintf(pathFile,"%s/%s/%s.bin",g_coordinatePointPath_USB[testType].path,pSerial,fileSerialBuff); 
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	 
	fresult = f_open(&file_obj,pathFile,FA_OPEN_EXISTING|FA_READ); 
	
	if (fresult != FR_OK)
	{	 
		f_close(&file_obj);
		
		return fresult;	
	}
	
	fresult = f_read(&file_obj,pCoordinate,sizeof(COORDINATE_POINT_TypeDef),&br);   
	
	f_close(&file_obj);

	return fresult; 
}

/*------------------------------------------------------------
 * Function Name  : DeleteCoordinateFolder
 * Description    : ɾ��������ļ��У����ļ��а������ļ���
 * Input          : pdrv���豸�ţ�testType���������ͣ�pSerial���Լ����
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FRESULT DeleteCoordinateFolder( uint8_t pdrv, uint8_t testType, const char * const pSerial )
{
	DIR dir_object;  
	FILINFO file_info;
	FRESULT fresult;
	char pathFile[PATH_BUF_SIZE];  
	const uint8_t MAX_TEST_SERIAL_LEN = 16;		//һ��������ų���
	FlagStatus flagLongFileName = RESET;
	
	#if _USE_LFN								
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
     #endif
	
	if ( (strlen(pSerial) > MAX_TEST_SERIAL_LEN) 	||\
		(testType >= SUPPORT_TEST_NUM)	)
	{
		return FR_INVALID_NAME;
	}
	
	switch ( pdrv )
	{
		case MMC:
			usprintf(pathFile,"%s/%s",g_coordinatePointPath_SD[testType].path,pSerial);  
			break;
		case USB:
			usprintf(pathFile,"%s/%s",g_coordinatePointPath_USB[testType].path,pSerial);
			break;
		default:
			return FR_INVALID_DRIVE;
	}
	
	fresult = f_opendir(&dir_object,pathFile); 	
	if (fresult != FR_OK)
	{
		return fresult; 
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object, &file_info);
		
		if (fresult != FR_OK)
		{
			return fresult; 
		}
		
		/* �ļ����ڲ������ļ� */
		if ( !file_info.fname[0] )	
		{			
			break;	
		}
		
		flagLongFileName = (file_info.lfname[0] == 0) ? RESET : SET;	//�ж��Ƿ��ǳ��ļ���
		
		if (file_info.fattrib & AM_ARC)		//�Ǹ��ļ�
		{
			if (flagLongFileName == SET)
			{
				usprintf(pathFile,"%s/%s/%s",g_coordinatePointPath_SD[testType].path,pSerial,file_info.lfname);
				fresult = f_unlink(pathFile); 
			}
			else
			{
				usprintf(pathFile,"%s/%s/%s",g_coordinatePointPath_SD[testType].path,pSerial,file_info.fname);
				fresult = f_unlink(pathFile); 
			}
			
			if (fresult != FR_OK)
			{
				return fresult; 
			}
		}
	}
	
	/* ����ɾ���ļ��󣬲���ɾ���ļ��� */
	usprintf(pathFile,"%s/%s",g_coordinatePointPath_SD[testType].path,pSerial);
	
	fresult = f_unlink(pathFile); 	
	
	return fresult; 
}

/*------------------------------------------------------------
 * Function Name  : CreatFolder
 * Description    : �����ļ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FRESULT CreatFolder( const char * const pPath )
{
	uint32_t i = 0;
	uint32_t pathIndex = 0;
	char pathBuff[100];
	FRESULT fresult;
	
	while (pPath[i] != NULL)
	{	
		if (pPath[i] == '/')
		{
			pathBuff[pathIndex] = NULL;
			
			fresult = f_mkdir(pathBuff); 
			if ((fresult!=FR_OK) && (fresult!=FR_EXIST))
			{									
				break;
			}
			
			pathBuff[pathIndex++] = '/';
		}
		else
		{
			pathBuff[pathIndex++] = pPath[i];
		}
		
		i++;
	}
	
	return fresult;
}

/*------------------------------------------------------------
 * Function Name  : CopyFileTime
 * Description    : �����ļ�ʱ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static FRESULT CopyFileTime( const char * const pSourcePath, const char * const pTargetPath )
{
	FILINFO file_info;
	FRESULT fresult;
	
	#if _USE_LFN
         file_info.lfname = Lfname;
         file_info.lfsize = sizeof(Lfname);
    #endif
	
	fresult = f_stat(pSourcePath,&file_info);
	if (fresult != FR_OK)
	{									
		return fresult;
	}
	
	fresult = f_utime(pTargetPath,&file_info);
	
	return fresult;
}

/*------------------------------------------------------------
 * Function Name  : CopyFileFromDevices
 * Description    : ���豸�����ļ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus CopyFileFromDevices( const char * const pSourcePath, const char * const pTargetPath )
{
	FRESULT fresult;
	FIL sourceFileObj; 
	FIL targetFileObj; 
	uint32_t br; 
	const uint16_t ONCE_READ_SIZE = 512;
	uint8_t fileBuff[ONCE_READ_SIZE];
	uint32_t totalReadSize = 0;
	uint32_t totalWriteSize = 0;
	
	fresult = f_open(&sourceFileObj,pSourcePath,FA_OPEN_EXISTING|FA_READ);
	if (fresult != FR_OK)
	{
		f_close(&sourceFileObj);
		
		return ERROR;
	}
	
	fresult = f_open(&targetFileObj,pTargetPath,FA_CREATE_ALWAYS|FA_WRITE);
	if (fresult == FR_NO_PATH)
	{
		fresult = CreatFolder(pTargetPath);		
		fresult = f_open(&targetFileObj,pTargetPath,FA_CREATE_ALWAYS|FA_WRITE);
		if (fresult != FR_OK)
		{
			f_close(&sourceFileObj);
			f_close(&targetFileObj);
		
			return ERROR;
		}	
	}
	else if (fresult != FR_OK)
	{
		f_close(&sourceFileObj);
		f_close(&targetFileObj);
	
		return ERROR;
	}

	totalReadSize = sourceFileObj.fsize;
	
	do{
		uint32_t realReadSize = 0;
		
		fresult = f_read(&sourceFileObj,fileBuff,ONCE_READ_SIZE,&br); 
		if (fresult != FR_OK)
		{
			break;
		}
		realReadSize = br;
		
		fresult = f_write(&targetFileObj,fileBuff,realReadSize,&br);
		if (fresult != FR_OK)
		{
			break;
		}		
		totalWriteSize += br;	
		
	} while (br != 0);
	
	f_close(&sourceFileObj);
	f_close(&targetFileObj);
	
	if (fresult != FR_OK)
	{
		return ERROR;
	}
	
	if (totalReadSize != totalWriteSize)
	{
		return ERROR;
	}
	
	fresult = CopyFileTime(pSourcePath,pTargetPath);
	if (fresult != FR_OK)
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : CopyFileBody
 * Description    : ���豸�����ļ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus CopyFileBody( const char * const pSourceFolderPath, const char * const pTargetFolderPath,\
				const char * const pSourceFileName )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char sourcePathName[FILE_PATH_MAX_LEN];
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pSourceFolderPath) + strlen(pSourceFileName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	pathLen = strlen(pTargetFolderPath) + strlen(pSourceFileName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(sourcePathName,pSourceFolderPath);
	strcat(sourcePathName,"/");
	strcat(sourcePathName,pSourceFileName);
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pSourceFileName);

	if (ERROR == CopyFileFromDevices(sourcePathName,targetPathName) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : CopyFolderBody
 * Description    : ���豸�����ļ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus CopyFolderBody( const char * const pSourceFolderPath, const char * const pTargetFolderPath,\
				const char * const pSourceFolderName, void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char sourcePathName[FILE_PATH_MAX_LEN];
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pSourceFolderPath) + strlen(pSourceFolderName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	pathLen = strlen(pTargetFolderPath) + strlen(pSourceFolderName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(sourcePathName,pSourceFolderPath);
	strcat(sourcePathName,"/");
	strcat(sourcePathName,pSourceFolderName);
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pSourceFolderName);

	if (ERROR == CopyFolderFromDevices(sourcePathName,targetPathName,GUI_CallBack) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : CopyFolderFromDevices
 * Description    : ���豸�����ļ��У�·��������'/'��β��
 * Input          : pSourceFolderPath��Դ�ļ���·����pTargetFolderPath��Ŀ���ļ���·����GUI_CallBack���ص�����
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus CopyFolderFromDevices( const char * const pSourceFolderPath, const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	DIR dir_object;  
	FILINFO sourceFileInfo; 
	FRESULT fresult;
	FlagStatus flagLongFileName = RESET;
	
	#if _USE_LFN								
		sourceFileInfo.lfname = Lfname;
		sourceFileInfo.lfsize = sizeof(Lfname);
    #endif
	
	fresult = f_opendir(&dir_object,pSourceFolderPath); 	
	if (fresult != FR_OK)
	{
		return ERROR; 
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object,&sourceFileInfo);		
		if (fresult != FR_OK)
		{
			return ERROR; 
		}
		
		/* �ļ������ļ���ȡ��� */
		if ( !sourceFileInfo.fname[0] )	
		{			
			break;	
		}
		
		flagLongFileName = (sourceFileInfo.lfname[0] == 0) ? RESET : SET;	//�ж��Ƿ��ǳ��ļ���
		
		{
			const char *pName = NULL;
			
			if (flagLongFileName == SET)
			{
				pName = sourceFileInfo.lfname;
			}
			else
			{
				pName = sourceFileInfo.fname;
			}			
			
			if (sourceFileInfo.fattrib & AM_ARC)		//�Ǹ��ļ�
			{		
				/* �ص�������ִ���û����� */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(NULL,pName);	
				}
				
				if ( CopyFileBody(pSourceFolderPath,pTargetFolderPath,pName) == ERROR)
				{
					return ERROR;
				}
			}
			else if (sourceFileInfo.fattrib & AM_DIR)		//�Ǹ��ļ���
			{		
				/* �ص�������ִ���û����� */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(pName,NULL);
				}
				
				if ( CopyFolderBody(pSourceFolderPath,pTargetFolderPath,pName,GUI_CallBack) == ERROR)
				{
					return ERROR;
				}
			}
		}
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : DeleteFileFromDevices
 * Description    : ���豸ɾ���ļ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus DeleteFileFromDevices( const char * const pTargetPath )
{
	FRESULT fresult = f_unlink(pTargetPath); 	
	
	if (fresult == FR_OK)
	{
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

/*------------------------------------------------------------
 * Function Name  : DeleteFileBody
 * Description    : ���豸ɾ���ļ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus DeleteFileBody( const char * const pTargetFolderPath,\
						const char * const pTargetFileName )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pTargetFolderPath) + strlen(pTargetFileName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pTargetFileName);

	if (ERROR == DeleteFileFromDevices(targetPathName) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : DeleteFolderBody
 * Description    : ���豸ɾ���ļ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus DeleteFolderBody( const char * const pTargetFolderPath,\
				const char * const pTargetFolderName, void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	const uint8_t FILE_PATH_MAX_LEN = 100;
	char targetPathName[FILE_PATH_MAX_LEN];
	uint16_t pathLen = 0;
	
	pathLen = strlen(pTargetFolderPath) + strlen(pTargetFolderName) + 1;	//+1���м���һ��'/'����
	if (pathLen >= FILE_PATH_MAX_LEN)
	{
		return ERROR; 
	}
	
	strcpy(targetPathName,pTargetFolderPath);
	strcat(targetPathName,"/");
	strcat(targetPathName,pTargetFolderName);

	if (ERROR == DeleteFolderFromDevices(targetPathName,GUI_CallBack) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : DeleteFolderFromDevices
 * Description    : ���豸ɾ���ļ��У�·��������'/'��β��
 * Input          : pSourceFolderPath��Դ�ļ���·����pTargetFolderPath��Ŀ���ļ���·����GUI_CallBack���ص�����
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus DeleteFolderFromDevices( const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) )
{
	DIR dir_object;  
	FILINFO targetFileInfo; 
	FRESULT fresult;
	FlagStatus flagLongFileName = RESET;
	
	#if _USE_LFN								
		targetFileInfo.lfname = Lfname;
		targetFileInfo.lfsize = sizeof(Lfname);
    #endif
	
	fresult = f_opendir(&dir_object,pTargetFolderPath); 	
	if (fresult != FR_OK)
	{
		return ERROR; 
	}
	
	for (;;)
	{
		fresult = f_readdir(&dir_object,&targetFileInfo);		
		if (fresult != FR_OK)
		{
			return ERROR; 
		}
		
		/* �ļ������ļ���ȡ��� */
		if ( !targetFileInfo.fname[0] )	
		{	
			/* 0��ʾ��Ŀ¼������ɾ�� */
			if (dir_object.sclust != 0)
			{
				if (DeleteFileFromDevices(pTargetFolderPath) == ERROR)
				{
					return ERROR;
				}
			}					
			break;	
		}
		
		flagLongFileName = (targetFileInfo.lfname[0] == 0) ? RESET : SET;	//�ж��Ƿ��ǳ��ļ���
		
		{
			const char *pName = NULL;
			
			if (flagLongFileName == SET)
			{
				pName = targetFileInfo.lfname;
			}
			else
			{
				pName = targetFileInfo.fname;
			}
			
			if (targetFileInfo.fattrib & AM_ARC)		//�Ǹ��ļ�
			{			
				/* �ص�������ִ���û����� */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(NULL,pName);
				}
				
				if ( DeleteFileBody(pTargetFolderPath,pName) == ERROR)
				{
					return ERROR;
				}
			}
			else if (targetFileInfo.fattrib & AM_DIR)		//�Ǹ��ļ���
			{	
				/* �ص�������ִ���û����� */
				if (GUI_CallBack != NULL)
				{
					GUI_CallBack(pName,NULL);
				}
				
				if ( DeleteFolderBody(pTargetFolderPath,pName,GUI_CallBack) == ERROR)
				{
					return ERROR;
				}
			}
		}
	}
	
	return SUCCESS;
}

/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE************************************/ 
