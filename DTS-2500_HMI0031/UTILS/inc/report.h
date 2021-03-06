/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: report.h
** 最后修改日期: 2014/5/6 15:35:54
** 描        述: 报告有关的函数
** 版	     本: V1.1
** 主  控  芯 片:STM32F103 	   
** IDE:MDK4.12
**********************************************************************************************************/
#ifndef __REPORT_H
#define	__REPORT_H	
    

#pragma pack(1)

#define SUPPORT_TEST_NUM				12							//支持的试验个数

/* 试验类型选择 */
typedef enum 
{
	NONE_TEST = 0,	//[0]错误的种类
	KYSNJS,			//[1]水泥胶砂抗压
	KYJZSJ,			//[2]建筑砂浆抗压	
	KYHNT,			//[3]普通混凝土抗压
	KZHNT,			//[4]普通混凝土抗折
	KYQQZ,			//[5]砌墙砖抗压
	KYZJDH,			//[6]逐级定荷
	KYTY,			//[7]通用抗压
	KZSNJS,			//[8]水泥胶砂抗折
	KZYJSNJ,		//[9]压浆水泥浆抗折
	KZTY,			//[10]通用抗折
	KLJSSW,			//[11]金属室温拉伸
}TEST_TYPE_TypeDef;


//试验信息结构体 
typedef struct
{
	char fname[17];
	tTime time;
}TEST_INFO_TypeDef;	 	
 	
 	
//最近10个实验的报告记录
typedef struct
{
	uint8_t index;			//0-9
	uint8_t type[10];
	char fname[10][17];
	tTime time[10];
}REPORT_LOG_TypeDef; 	 
		
//试验参数 	
typedef struct
{
	char test_standard[17];				//试验标准
	char sample_type[17];				//试件品种
	char strength_grade[17];			//强度等级
	uint16_t sample_age;				//试件龄期 
	char sample_spec[17];				//试件规格
	float sample_span;					//试件跨距
	uint8_t sample_num;					//试验试件块数
	uint8_t sample_serial;				//试验当前块数
	uint8_t test_is_complete;			//一组试验已完成(0-未完成，1--完成)
	float  force[20];					//试验力值
	float  strength[20];				//试验强度
	float  force_valid[3];				//试验有效力值
	float  strength_valid[3];			//试验有效强度
	uint8_t result_valid;				//试验结果有效性 （0-结果无效，1-有效强度，2-平均强度）
	uint8_t test_process;				//试验是否完成 (0-不需抗压，1-抗压完成，2-抗压未完成）
	uint8_t collect_load[20];			//是否在集中载荷内，1表示是 0表示否
	float length;						//长度
	float width;						//宽度
	float high;							//高度
	float correct_cof;					//修正系数
	float area;							//面积
	char sample_shape[11];				//试块形状
	
	/* 万能机参数 */
	float maxForce;						//最大力
	float maxStrength;					//最大强度
	float upYieldForce;					//上屈服力
	float downYieldForce;				//下屈服力
	float upYieldStrength;				//上屈服强度
	float downYieldStrength;			//下屈服强度
	float maxForceSumExtend;			//最大力总延伸
	float maxForceSumElongation;		//最大力总伸长率
}REPORT_TypeDef;
    
#pragma pack()     

FRESULT get_free_space(float *space);												//获取SD卡剩余空间，单位是Mb    								
FRESULT report_save(uint8_t type,const char *file,const REPORT_TypeDef *report);    //保存结果,type：试验类型 file:文件名 report:实验报告
FRESULT report_read(uint8_t type,const char *file,REPORT_TypeDef *report);			//读取结果,type：试验类型 file:文件名 report:实验报告   
FRESULT report_delete(uint8_t type,const char *file);								//删除指定试验种类指定文件名的文件，如果type=0xFF，删除所有试验种类中该试验名的文件
FRESULT report_lately_10(REPORT_LOG_TypeDef *rlog);									//读取最近10个实验的信息
FRESULT report_exist(uint8_t type,const char *file);								//查询指定试验种类指定文件名的文件是否存在

//functionName:FRESULT report_search_subsn(uint8_t type,char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
//description:通过试验编号查找结果,在指定试验类型中试验编号只要包含“subsn”中的字符，都搜索出来
//type:试验种类
//subsn:搜索文件名中包含“subsn”字符串
//offset 偏移量，一次只能搜索10个文件名，搜索后面的试验，需要使用此变量，提过前面的文件
//test:最多返回偏移后的10组试验信息,test定时的时候必须是11个数组，最后一个数组文件名全部为0表示结束。
//fnumb:返回查找到的文件数量 
FRESULT report_search_sn(uint8_t type,const char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb);
//根据时间段查询试验
//type:试验种类
//date_start：起始日期  date_end：结束日期
//offset 偏移量
//test:最多返回偏移后的10组试验信息
//fnumb:返回查找到的文件数量 
FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb); 
  
FRESULT report_save_usb(uint8_t type,const char *file,REPORT_TypeDef *report);
FRESULT report_save_usb_set_time (uint8_t type,const char *file);

#endif








