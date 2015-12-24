/****************************************Copyright (c)**************************************************
**                               	杭州鑫高科技有限公司
**                                   
**                                 
** 文   件   名: report.c
** 最后修改日期: 2014/5/6 15:34:42
** 描        述: 鑫高报告库函数
** 版	     本: V1.1
** 主  控  芯 片:STM32F103 晶振频率:8.00MHz
** IDE:MDK4.12 
1、保存试验，如果已经有此文件，返回文件存在，否者写入文件
2、搜索指定文件名的文件，并且返回文件存储时间
3、搜索文件存储时间的文件
4、新增加通过编号搜索报告

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

//报告存放的路径
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
description:获取SD卡剩余空间，单位是Mb
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
	*space=free_space * fs->csize /2048.0f;		//之前用的时0.04版本的FATFS系统，0.07E版本的文件系统不一样 
	return FR_OK; 
}

 
/**********************************************************************
functionName:void report_get_path(uint8_t type,char *path)
description:获取指定试验的绝对路径 ，type：试验种类 *path要得到的路径
**********************************************************************/ 
void report_get_path(uint8_t type,char *path)
{
	strcpy(path,test_path[type].path);
}



/**********************************************************************
functionName:FRESULT report_save(char *type,char *file,REPORT_TypeDef *report)
description:保存结果,type：试验类型 file:文件名 report:实验报告
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
// 	if (FR_OK == fresult)	//SD卡没有对应的文件	By:SY
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
description: 读取最近10个实验的信息
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
description:读取结果,type：试验类型 file:文件名 report:实验报告
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
description:删除指定试验种类指定文件名的文件，如果type=0xFF，删除全部文件
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
description:查询指定试验种类指定文件名的文件是否存在
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
description: 根据时间段查询试验
type:试验种类
date_start：起始日期  date_end：结束日期
offset 偏移量
test:最多返回偏移后的10组试验信息
fnumb:返回查找到的文件数量 
**********************************************************************/ 
FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
{
	DIR dir_object;  
	FILINFO file_info;
	FRESULT fresult; 
	uint8_t i=0;  
	uint8_t j=0;  	  
	uint32_t start=0;
	uint32_t end=0;		//起始和结束时间
	uint32_t temp=0;
	uint8_t long_file_name = 0;	//长文件名标志
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
		
		long_file_name = (file_info.lfname[0] == 0) ? 0 : 1;	//判断是否是长文件名
		
		if(file_info.fattrib&AM_ARC)		//是个文件
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
description:通过试验编号查找结果,在指定试验类型中试验编号只要包含“subsn”中的字符，都搜索出来
type:试验种类
subsn:搜索文件名中包含“subsn”字符串
offset 偏移量，一次只能搜索10个文件名，搜索后面的试验，需要使用此变量，提过前面的文件
test:最多返回偏移后的10组试验信息,test定时的时候必须是11个数组，最后一个数组文件名全部为0表示结束。
fnumb:返回查找到的文件数量 
**********************************************************************/ 
FRESULT report_search_sn(uint8_t type,const char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
{
	DIR dir_object;  
	FILINFO file_info; 
	FRESULT fresult; 
	uint8_t i=0;  
	uint8_t j=0; 
	uint8_t long_file_name = 0;	//长文件名标志
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
			if(!file_info.fname[0])				//如果文件名是空的，跳过文件 
			break;	
			
			long_file_name = (file_info.lfname[0] == 0) ? 0 : 1;	//判断是否是长文件名
						
			if(file_info.fattrib&AM_ARC)		//如果是一个文件
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
 * Description    : 获取字符串中小数点个数
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
description:保存结果,type：试验类型 file:文件名 report:实验报告
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
description:设置导出报告的文件时间
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
