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
	float gz_area;						//规则面积
	float bgz_area;						//不规则面积
	uint8_t sample_shape_index;			//试块形状索引
	float	yx_diameter;				//圆形直径
	
	/* 万能机参数 */
	float maxForce[20];					//最大力
	float maxStrength[20];				//最大强度
	float upYieldForce[20];				//上屈服力
	float downYieldForce[20];			//下屈服力
	float upYieldStrength[20];			//上屈服强度
	float downYieldStrength[20];		//下屈服强度
	float maxForceSumExtend[20];		//最大力总延伸
	float maxForceSumElongation[20];	//最大力总伸长率
	float parallelLenth;				//平行长度
	float extensometerGauge;			//引伸计标距
	float originalGauge;				//原始标距
	float pipeThickness;				//管段厚度
	float pipeOuterDiameter;			//管段外径
}REPORT_TypeDef;

#define RECORD_COORDINATE_PERIOD		100					//100ms记录一个点
#define RECORD_COORDINATE_FREQ			(1000/RECORD_COORDINATE_PERIOD)	
#define RECORD_COORDINATE_TIME_SECOND	(5 * 60)			//5分钟	
#define DECORD_COORDINATE_FORCE_NUM		(RECORD_COORDINATE_TIME_SECOND * RECORD_COORDINATE_FREQ) 

typedef struct
{
	uint8_t xType;						//X轴类型
	uint8_t yType;						//Y轴类型
	uint8_t xUint;						//X轴单位（0：s，1：ms）
	uint8_t yUint;						//Y轴单位（0：kN，1：N）	
	float xMaxValue;					//X轴最大值
	float yMaxValue;					//Y轴最大值
	float systemMaxForce;				//系统最大力值
	uint16_t recordPointFreq;			//记录每个点频率
	uint32_t nowUsePointNum;			//已使用的点数
	uint32_t maxPointNum;				//支持的最大点数
	float xScalingCoefficient;			//缩放系数（将X轴当前值按照缩放系数修饰）
	float yScalingCoefficient;			//缩放系数（将Y轴当前值按照缩放系数修饰）	
	float force[DECORD_COORDINATE_FORCE_NUM];
	float deform[DECORD_COORDINATE_FORCE_NUM];
}COORDINATE_POINT_TypeDef;
    
#pragma pack()     

FRESULT get_free_space(float *space);												//获取SD卡剩余空间，单位是Mb    								
FRESULT report_save(uint8_t pdrv, uint8_t type, const char * const file, const REPORT_TypeDef *report);    //保存结果,type：试验类型 file:文件名 report:实验报告
FRESULT report_read(uint8_t type,const char * const file,REPORT_TypeDef *report);			//读取结果,type：试验类型 file:文件名 report:实验报告   
FRESULT report_delete(uint8_t type,const char * const file);						//删除指定试验种类指定文件名的文件，如果type=0xFF，删除所有试验种类中该试验名的文件
FRESULT report_lately_10(REPORT_LOG_TypeDef *rlog);									//读取最近10个实验的信息
FRESULT report_exist(uint8_t type,const char * const file);								//查询指定试验种类指定文件名的文件是否存在

//functionName:FRESULT report_search_subsn(uint8_t type,char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
//description:通过试验编号查找结果,在指定试验类型中试验编号只要包含“subsn”中的字符，都搜索出来
//type:试验种类
//subsn:搜索文件名中包含“subsn”字符串
//offset 偏移量，一次只能搜索10个文件名，搜索后面的试验，需要使用此变量，提过前面的文件
//test:最多返回偏移后的10组试验信息,test定时的时候必须是11个数组，最后一个数组文件名全部为0表示结束。
//fnumb:返回查找到的文件数量 
FRESULT report_search_sn(uint8_t type,const char * const subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb);
//根据时间段查询试验
//type:试验种类
//date_start：起始日期  date_end：结束日期
//offset 偏移量
//test:最多返回偏移后的10组试验信息
//fnumb:返回查找到的文件数量 
FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb); 
  
FRESULT SetReportSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, \
			const char * const file);
FRESULT SetCurveSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial);

FRESULT SaveCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, const COORDINATE_POINT_TypeDef * const pCoordinate );
FRESULT ReadCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, COORDINATE_POINT_TypeDef * const pCoordinate );			
FRESULT DeleteCoordinateFolder( uint8_t pdrv, uint8_t testType, const char * const pSerial );	

ErrorStatus CopyFileFromDevices( const char * const pSourcePath, const char * const pTargetPath );
ErrorStatus CopyFolderFromDevices( const char * const pSourceFolderPath, const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );
ErrorStatus DeleteFileFromDevices( const char * const pTargetPath );
ErrorStatus DeleteFolderFromDevices( const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );

#endif








