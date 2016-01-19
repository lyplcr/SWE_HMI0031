/**
  ******************************************************************************
  * @file    TestParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-10-25
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   试验参数文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "TestParameter.h"
#include "report.h"
#include "ModelSelect.h"
#include "TestTypeSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK

#define MAX_TEST_PARAMETER_NUM				15			//最大支持试验参数个数
#define MAX_TEST_PARAMETER_PUTIN_BIT		16			//支持最大输入位数
#define MAX_SPECIMEN_NUMS					20			//支持最大试块个数

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_TEST_TYPE = 0,		/* 试验类型 */
	OBJECT_SPECIMEN_SERIAL,		/* 试件编号 */
	OBJECT_SPECIMEN_VARIETY,	/* 试件品种 */
	OBJECT_STRENGTH_GRADE,		/* 强度等级 */
	OBJECT_SPECIMEN_AGE,		/* 试件龄期 */
	OBJECT_SPECIMEN_FORMAT,		/* 试件规格 */
	OBJECT_SPECIMEN_LENTH,		/* 试件长度 */
	OBJECT_SPECIMEN_WIDTH,		/* 试件宽度 */
	OBJECT_SPECIMEN_HIGH,		/* 试件高度 */
	OBJECT_SPECIMEN_SPAN,		/* 试件跨距 */
	OBJECT_CORRECTION_FACTOR,	/* 修正系数 */
	OBJECT_LOAD_SPEED,			/* 加载速度 */
	OBJECT_SPECIMEN_NUMS,		/* 试件块数 */
	OBJECT_LOAD_MODE,			/* 加载方式 */
	OBJECT_DELAY_TIMING,		/* 延时时间 */
	OBJECT_AREA,				/* 面积 	*/
	OBJECT_SHAPE,				/* 形状 	*/
	OBJECT_ROUND_DIAMETER,		/* 圆形直径 	*/
	OBJECT_EXTERNSOMETER_GAUGE,	/* 引伸计标距 */
	OBJECT_PARALLEL_LENTH,		/* 平行长度 */
	OBJECT_ORIGINAL_GAUGE,		/* 原始标距 */
	OBJECT_PIPE_THICKNESS,		/* 管段厚度 */
	OBJECT_PIPE_OUTER_DIAMETER,	/* 管段外直径 */
}ALL_PATAMETER_NAME_TypeDef;

typedef enum				//水泥胶砂抗折
{
	TEST_SERIAL_KZSNJS = 0,	//试验编号
	TEST_TYPE_KZSNJS,		//试验品种
	STRANG_GRADE_KZSNJS,	//强度等级
	SAMPLE_AGE_KZSNJS,		//试件龄期
	ZFX_LEN_KZSNJS,			//边长
	ZFX_SPAN_KZSNJS,		//跨距
	SAMPLE_NUM_KZSNJS,		//试件块数
}KZSNJS_PARA_NAME_TypeDef;	

typedef enum				 //压浆水泥浆抗折
{
	TEST_SERIAL_YJSNJ = 0,	//试验编号
	TEST_TYPE_YJSNJ,		//试验品种
	STRANG_GRADE_YJSNJ,		//强度等级
	SAMPLE_AGE_YJSNJ,		//试件龄期
	ZFX_LEN_YJSNJ,			//边长
	ZFX_SPAN_YJSNJ,			//跨距
	SAMPLE_NUM_YJSNJ,		//试件块数
}KZYJSNJ_PARA_NAME_TypeDef; 

typedef enum				 //通用抗折
{
	TEST_SERIAL_KZTY = 0,	//试验编号
	STRANG_GRADE_KZTY,		//强度等级
	SAMPLE_AGE_KZTY,		//试件龄期
	SAMPLE_WIDTH_KZTY,		//试件宽度
	SAMPLE_HIGH_KZTY,		//试件高度
	ZFX_SPAN_KZTY,			//跨距
	SAMPLE_NUM_KZTY,		//试件块数
}KZTY_PARA_NAME_TypeDef; 

typedef enum				 //水泥胶砂抗压
{
	TEST_SERIAL_KYSNJS = 0,	//试验编号
	TEST_TYPE_KYSNJS,		//试验品种
	STRANG_GRADE_KYSNJS,	//强度等级
	SAMPLE_AGE_KYSNJS,		//试件龄期
	SPEC_KYSNJS,			//试件规格
	SAMPLE_NUM_KYSNJS,		//试件块数
}KYSNJS_PARA_NAME_TypeDef;

typedef enum				 //建筑砂浆抗压
{
	TEST_SERIAL_KYJZSJ = 0,	//试验编号
	TEST_TYPE_KYJZSJ,		//试验品种
	SAMPLE_AGE_KYJZSJ,		//试件龄期
	SPEC_KYJZSJ,			//试件规格
	CORRECT_COF_KYJZSJ,		//修正系数
	SAMPLE_NUM_KYJZSJ,		//试件块数
}KYJZSJ_PARA_NAME_TypeDef;

typedef enum				//普通混凝土抗折
{
	TEST_SERIAL_KZHNT = 0,	//试验编号
	STRANG_GRADE_KZHNT,		//强度等级
	SAMPLE_AGE_KZHNT,		//试件龄期
	SPEC_KZHNT,				//试件规格
	CORRECT_COF_KZHNT,		//修正系数
	SAMPLE_NUM_KZHNT,		//试件块数		
}KZHNT_PARA_NAME_TypeDef;

typedef enum				//普通混凝土抗压
{
	TEST_SERIAL_KYHNT = 0,	//试验编号
	STRANG_GRADE_KYHNT,		//强度等级
	SAMPLE_AGE_KYHNT,		//试件龄期
	SPEC_KYHNT,				//试件规格
	CORRECT_COF_KYHNT,		//修正系数
	SAMPLE_NUM_KYHNT,		//试件块数
}KYHNT_PARA_NAME_TypeDef;

typedef enum				 //砌墙砖抗压
{
	TEST_SERIAL_KYQQZ = 0,	//试验编号
	TEST_TYPE_KYQQZ,		//试件品种
	LENGTH_KYQQZ,			//长度
	WIDTH_KYQQZ,			//宽度
	HIGH_KYQQZ,				//高度
	SAMPLE_NUM_KYQQZ,		//试件块数
}KYQQZ_PARA_NAME_TypeDef;

union KYTY_PARA_NAME_TypeDef		//通用抗压
{
	enum 				 
	{
		TEST_SERIAL_RECTANGLE_KYTY = 0,	//试验编号
		SAMPLE_SHAPE_RECTANGLE_KYTY,	//形状
		SAMPLE_LENTH_RECTANGLE_KYTY,	//长度
		SAMPLE_WIDTH_RECTANGLE_KYTY,	//宽度
		CORRECT_COF_RECTANGLE_KYTY,		//修正系数
		SAMPLE_NUM_RECTANGLE_KYTY,		//试件块数
	}KYTY_SHAPE_RECTANGLE;
	enum				 
	{
		TEST_SERIAL_ROUND_KYTY = 0,	//试验编号
		SAMPLE_SHAPE_ROUND_KYTY,	//形状
		SAMPLE_DIAMETER_ROUND_KYTY,	//直径
		CORRECT_COF_ROUND_KYTY,		//修正系数
		SAMPLE_NUM_ROUND_KYTY,		//试件块数
	}KYTY_SHAPE_ROUND;
	enum				 
	{
		TEST_SERIAL_IRREGULAR_KYTY = 0,	//试验编号
		SAMPLE_SHAPE_IRREGULAR_KYTY,	//形状
		SAMPLE_AREA_IRREGULAR_KYTY,		//面积
		CORRECT_COF_IRREGULAR_KYTY,		//修正系数
		SAMPLE_NUM_IRREGULAR_KYTY,		//试件块数
	}KYTY_SHAPE_RIRREGULAR;
};

union KLJSSW_PARA_NAME_TypeDef		//金属室温拉伸
{
	enum 				 
	{
		TEST_SERIAL_RECTANGLE_KLJSSW = 0,			//试验编号
		SAMPLE_SHAPE_RECTANGLE_KLJSSW,				//试件形状
		SAMPLE_THICKNESS_RECTANGLE_KLJSSW,			//矩形厚度
		SAMPLE_WIDTH_RECTANGLE_KLJSSW,				//矩形宽度
		ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW,		//原始截面积
		EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW,		//引伸计标距
		ORIGINAL_GAUGE_RECTANGLE_KLJSSW,			//原始标距
		PARALLEL_LENTH_RECTANGLE_KLJSSW,			//平行长度
		SAMPLE_NUM_RECTANGLE_KLJSSW,				//试件根数
	}KYTY_SHAPE_RECTANGLE;
	enum				 
	{
		TEST_SERIAL_ROUND_KLJSSW = 0,			//试验编号
		SAMPLE_SHAPE_ROUND_KLJSSW,				//试件形状
		SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW,		//圆形直径
		ORIGINAL_CROSS_AREA_ROUND_KLJSSW,		//原始截面积
		EXTERNSOMETER_GAUGE_ROUND_KLJSSW,		//引伸计标距
		ORIGINAL_GAUGE_ROUND_KLJSSW,			//原始标距
		PARALLEL_LENTH_ROUND_KLJSSW,			//平行长度
		SAMPLE_NUM_ROUND_KLJSSW,				//试件根数
	}KLJSSW_SHAPE_ROUND;
	enum				 
	{
		TEST_SERIAL_TUBE_KLJSSW = 0,			//试验编号
		SAMPLE_SHAPE_TUBE_KLJSSW,				//试件形状
		SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW,		//管段厚度
		SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW,	//管段外径
		ORIGINAL_CROSS_AREA_TUBE_KLJSSW,		//原始截面积
		EXTERNSOMETER_GAUGE_TUBE_KLJSSW,		//引伸计标距
		ORIGINAL_GAUGE_TUBE_KLJSSW,				//原始标距
		PARALLEL_LENTH_TUBE_KLJSSW,				//平行长度
		SAMPLE_NUM_TUBE_KLJSSW,					//试件根数
	}KYTY_SHAPE_TUBE;
	enum				 
	{
		TEST_SERIAL_RIRREGULAR_KLJSSW = 0,				//试验编号
		SAMPLE_SHAPE_RIRREGULAR_KLJSSW,					//试件形状
		ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW,			//原始截面积
		EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW,			//引伸计标距
		ORIGINAL_GAUGE_RIRREGULAR_KLJSSW,				//原始标距
		PARALLEL_LENTH_RIRREGULAR_KLJSSW,				//平行长度
		SAMPLE_NUM_RIRREGULAR_KLJSSW,					//试件根数
	}KYTY_SHAPE_RIRREGULAR;
};

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MAX_TEST_PARAMETER_NUM][MAX_TEST_PARAMETER_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_TEST_PARAMETER_NUM];
	TWO_LEVEL_MENU_TYPE_TypeDef twoLevelMenu[MAX_TEST_PARAMETER_NUM];
	const char *pParameterNameArray[MAX_TEST_PARAMETER_NUM];
	const char *pParameterUnitArray[MAX_TEST_PARAMETER_NUM];
	const char *pTestStandard;
	TEST_TYPE_TypeDef testType;
	uint8_t indexArray[MAX_TEST_PARAMETER_NUM];
	uint8_t curParameterNum;						//参数个数
	SMPL_NAME_TypeDef showChannel;					//显示通道
	SMPL_NAME_TypeDef tureChannel;					//真实通道
	BoolStatus isIndexMove;							//索引值移动
	FunctionalState enableMoveIndex;				//使能移动索引值
	FunctionalState enablePutin;					//使能输入
	FunctionalState enableArrow;					//使能箭头
	STATUS_SHIFT_TypeDef enableShift;				//使能切换输入法
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	FunctionalState refreshShortcut;				//刷新快捷菜单
	LEAVE_PAGE_TypeDef leavePage;					//离开页
}TEST_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* 标题名称 */
const char * const pTestParameterTitleName[] = 
{
	"参数设置 | 水泥胶砂抗压",		//0
	"参数设置 | 建筑砂浆抗压",		//1
	"参数设置 | 混凝土抗压",			//2
	"参数设置 | 混凝土抗折",			//3
	"参数设置 | 砌墙砖抗压",			//4
	"参数设置 | 逐级定荷",			//5
	"参数设置 | 通用抗压",			//6
	"参数设置 | 水泥胶砂抗折",		//7
	"参数设置 | 压浆水泥浆抗折",		//8
	"参数设置 | 通用抗折",			//9	
	"参数设置 | 金属室温拉伸",		//10
};

/* 试验标准 */
const char * const pTestStandard[] = 
{
	"GB/T 17671-1999",	  	//水泥胶砂抗压/水泥胶砂抗折/压浆水泥浆抗折
	"GB/T 50081-2002",	  	//普通混凝土抗压/普通混凝土抗折
	"JGJ/T 70-2009",	  	//建筑砂浆抗压
	"GB/T 2542-2012",	  	//砌墙砖抗压
	"GB/T 228-2002",		//金属室温拉伸
};

/* 水泥胶砂抗压参数名称显示 */
const char * const pValue_KYSNJS[] = 
{
	"试验类型：",
	"试件编号：",
	"试件品种：",
	"强度等级：",
	"试件龄期：",
	"试件规格：",
	"加载速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};						
						
/* 水泥胶砂抗折参数名称显示 */
const char * const pValue_KZSNJS[] = 
{
	"试验类型：",
	"试件编号：",
	"试件品种：",
	"强度等级：",
	"试件龄期：",
	"边长：",
	"跨距：",
	"加载速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};

/* 压浆水泥浆抗折参数名称显示 */
const char * const pValue_KZYJSNJ[] = 
{
	"试验类型：",
	"试件编号：",
	"试件品种：",
	"强度等级：",
	"试件龄期：",
	"边长：",
	"跨距：",
	"加载速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};	
						
/* 普通混凝土抗压参数名称显示 */
const char * const pValue_KYHNT[] = 
{
  	"试验类型：",
	"试件编号：",
	"强度等级：",
	"试件龄期：",
	"试件规格：",
	"修正系数：",
	"应力速度：",
	"载荷速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};	

/* 普通混凝土抗折参数名称显示 */
const char * const pValue_KZHNT[] = 
{
   	"试验类型：",
	"试件编号：",
	"强度等级：",
	"试件龄期：",
	"试件规格：",
	"修正系数：",
	"应力速度：",
	"载荷速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};

/* 砌墙砖抗压参数名称显示 */
const char * const pValue_KYQQZ[] = 
{
	"试验类型：",
	"试件编号：",
	"试件品种：",
	"长度：",
	"宽度：",
	"高度：",
	"加载速度：",
	"试件块数：",
	"加载方式：",
	"延时时间：",
};

/* 逐级定荷抗压参数名称显示 */
const char * const pValue_KYZJDH[] = 
{
	"试验类型：",
	"加载速度：",
	"定荷级数：",
};
						
/* 建筑砂浆抗压参数名称显示 */
const char * const pValue_KYJZSJ[] = 
{
  	"试验类型：",
	"试件编号：",
	"试件品种：",
	"试件龄期：",
	"试件规格：",
	"修正系数：",
	"加载速度：",
	"试件块数：",
	"加载方式：",	
	"延时时间：",
};
						
/* 通用抗压参数名称显示 */
const char * const pValue_KYTY[] = 
{
	"试验类型：",
	"试件编号：",
	"试件龄期：",
	"试件形状：",
	"试件面积：",
	"加载速度：",
	"试件块数：",
	"加载方式：",
	"延时时间：",
	"修正系数：",
	"长度：",
	"宽度：",
	"圆形直径：",
};
						
/* 通用抗折参数名称显示 */
const char * const pValue_KZTY[] = 
{
  "试验类型：",
  "试件编号：",
  "强度等级：",
  "试件龄期：",
  "试件宽度：",
  "试件高度：",
  "试件跨距：",
  "加载速度：",
  "试件块数：",
  "加载方式：",
  "延时时间：",
};						
																																							
/* 金属室温抗拉参数名称显示 */
const char * const pValue_KLJSSW[] = 
{
	"试验类型：",		//0
	"试件编号：",		//1
	"圆形直径：",		//2
	"原始截面积：",		//3
	"引伸计标距：",		//4
	"试件根数：",		//5
	"平行长度：",		//6
	"原始标距：",		//7
	"矩形厚度：",		//8
	"矩形宽度：",		//9
	"管段厚度：",		//10
	"管段外径：",		//11
	"试件形状：",		//12
};


/* 水泥胶砂抗折参数-----------------------------------------------------------*/
//试样品种
const char * const pSample_type[] = 
{					
	"自定义",
	"P.I",
	"P.II",
	"P.O",
	"P.S.A",
	"P.S.B",
	"P.P",
	"P.F",
	"P.C",
	"M",		
};
			
//强度等级
const char * const pStrength_grade[] = 
{
	"自定义",
	"12.5",
	"22.5",
	"32.5",
	"32.5R",
	"42.5",
	"42.5R",
	"52.5",
	"52.5R",
	"62.5",
	"62.5R",
};																			

//试件龄期	
const char * const pSample_age[] = 
{
	"自定义",
	"1",
	"2",
	"3",
	"7",
	"28",
};												

//边长
const char * const pZfx_length[] = 
{
	"自定义",
	"30",
	"40",
	"50",
	"60",
};										
					
//跨距					
const char * const pZfx_span[] = 
{
	"自定义",
	"80",
	"100",
	"120",
	"140",
	"160",
};									
		
//加载速度						
const char * const pSpeed_load[] = 
{
	"自定义",
	"50",
	"100",
};									
					
//试件块数						
const char * const pSample_num[] = 
{
	"自定义",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
};									
					
//是否连续加载						
const char * const pIs_continue_load[] = 
{
	"单独试验",
	"连续试验",
};									
					
//延时时间					
const char * const pDelay_time[] = 
{
	"自定义",
	"8",
	"12",
	"16",
	"20",
	"24",
	"28",
	"32",
};


/* 压浆水泥浆抗折参数---------------------------------------------------------*/
//试件品种同 -> pSample_type
//强度等级
const char * const pStrength_grade_KZYJSNJ[] = 
{
	"自定义",
	"52.5",
	"52.5R",
	"62.5",
	"62.5R",
};

//试件龄期同 -> pSample_age
//试件边长同 -> pZfx_length
//试件跨距同 -> pZfx_span
//加载速度同 -> pSpeed_load
//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time

	
/* 水泥胶砂抗压参数-----------------------------------------------------------*/
//试件品种同 -> pSample_type
//试件等级同 -> pStrength_grade
//试件龄期同 -> pSample_age

//试件规格
const char * const pSpecimen_spec_KYSNJS[] = 
{
	"自定义",
	"40*40",
	"50*50",
};

//加载速度(kN)
const char * const pSpeed_load_KYSNJS[] = 
{
	"自定义",
	"0.5",
	"0.7",
	"1.0",
	"1.1",
	"1.3",
	"1.5",
	"2.4",
	"5.0",
};

//加载速度(N)
const char * const pSpeed_N_load_KYSNJS[] = 
{
	"自定义",
	"500",
	"700",
	"1000",
	"1100",
	"1300",
	"1500",
	"2400",
	"5000",
};


//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time


/* 建筑砂浆抗压参数-----------------------------------------------------------*/
//试样品种
const char * const pSample_type_KYJZSJ[] = 
{					
	"M3",
	"M5",
	"M7.5",
	"M10",
	"M12.5",
	"M15",
	"M20",
	"M25",
	"M30",
	"M40",
};

//试件龄期同 -> pSample_age

//试件规格
const char * const pSpecimen_spec_KYJZSJ[] = 
{
	"自定义",
	"70.7*70.7*70.7",
};

//修正系数
const char * const pCorrect_cof_KYJZSJ[] = 
{
	"1.00",
	"1.35",
};

//加载速度同 -> pSpeed_load_KYSNJS
//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time

/* 普通混凝土抗压参数---------------------------------------------------------*/
//强度等级同 -> pStrength_grade_KZHNT
//试件龄期同 -> pSample_age
//试件规格
const char * const pSpecimen_spec_KYHNT[] = 
{
	"100*100*100",
	"150*150*150",
	"200*200*200",
	"Φ100*200",
	"Φ150*300",
	"Φ200*400",
};

//修正系数
const char * const pCorrect_coef_KYHNT[] = 
{
	"0.95",
	"1",
	"1.05",
};

//应力速度
const char * const pStress_speed_KYHNT[]=
{
	"自定义",
	"0.3",
	"0.4",
	"0.5",
	"0.6",
	"0.7",
	"0.8",
	"0.9",
	"1.0",
};

//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time


/* 普通混凝土抗折参数---------------------------------------------------------*/
//强度等级
const char * const pStrength_grade_KZHNT[] = 
{
	"C10",
	"C15",
	"C20",
	"C25",
	"C30",
	"C35",
	"C40",
	"C45",
	"C50",
	"C55",
	"C60",
	"C65",
	"C70",
	"C75",
	"C80",
};

//试件龄期同 -> pSample_age

//试件规格
const char * const pSpecimen_spec[] = 
{
	"150*150*600",		  //高度+宽度+长度
	"150*150*550",
	"100*100*400",
};

//修正系数
const char * const pCorrect_coef[] = 
{
	"0.85",
	"1",
};

//应力速度
const char * const pStress_speed_KZHNT[]=
{
	"自定义",
	"0.02",
	"0.03",
	"0.04",
	"0.05",
	"0.06",
	"0.07",
	"0.08",
	"0.09",
	"0.1",
};

//载荷速度根据应力速度和试验规格计算


//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time


/* 砌墙砖抗压参数-------------------------------------------------------------*/
//试件品种
const char * const pSample_type_KYQQZ[] = 
{
	"烧结普通砖",
	"烧结多孔砖",
	"烧结空心砖",
	"空心砌砖",
	"蒸压灰砂砖",
	"煤粉灰砖",
	"炉渣砖",
	"碳化砖",
};
    
//长度
const char * const pLength_KYQQZ[] = 
{
	"90",
	"115",
	"140",
	"175",
	"180",
	"190",
	"240",
	"290",
	"390",
};

//宽度
const char * const pWidth_KYQQZ[] = 
{
	"90",
	"115",
	"140",
	"175",
	"180",
	"190",
	"240",
	"290",
	"390",
};

//高度
const char * const pHigh_KYQQZ[] = 
{
	"90",
	"115",
	"140",
	"175",
	"180",
	"190",
	"240",
	"290",
	"390",
};

//加载速度(kN)
const char * const pSpeed_load_KYQQZ[] = 
{
	"自定义",
	"2.0",
	"3.0",
	"4.0",
	"5.0",
	"6.0",
};

//加载速度(N)
const char * const pSpeed_N_load_KYQQZ[] = 
{
	"自定义",
	"2000",
	"3000",
	"4000",
	"5000",
	"6000",
};

//试件块数
const char * const pSample_num_KYQQZ[] = 
{
	"自定义",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
};

//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time

/* 逐级定荷抗压参数---------------------------------------------------------------*/
//加载速度同 -> pSpeed_load_KYSNJS
const char * const pMenu_SerialKYZJDH[] =
{
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
};



/* 通用抗压参数---------------------------------------------------------------*/
//试件龄期同 -> pSample_age

//试件形状
const char * const pSpecimen_sharp[] = 
{
	"长方形",
	"圆形",
	"不规则",
};

//修正系数
const char * const pSpecimen_cof[] = 
{
	"自定义",
	"1.00",
};

//加载速度同 -> pSpeed_load_KYSNJS
//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time


/* 通用抗折参数---------------------------------------------------------------*/
//强度等级
const char * const pStrength_grade_KZTY[] = 
{
	"自定义",
	"50",
	"100",	
};

//试件龄期同 -> pSample_age
//试件宽度同 -> pZfx_length
//试件高度同 -> pZfx_length
//试件跨距同 -> pZfx_span
//加载速度同 -> pSpeed_load
//试件块数同 -> pSample_num
//是否连续加载同 -> pIs_continue_load
//延时时间同 -> pDelay_time

/* 金属室温拉伸参数-------------------------------------------------------------*/
//圆形直径
const char * const pRoundDiameter_KLJSSW[] = 
{
	"自定义",
	"3",
	"5",
	"6",
	"8",
	"10",
	"15",
	"20",
	"25",
};

//试件形状
const char * const pSpecimenSharp_KLJSSW[] = 
{
	"长方形",
	"圆形",
	"管形",
	"不规则",
};

//试件块数同 -> pSample_num

/* 警告信息 */
const char * const pSampleNumRangeCue[] = 
{
	"试块个数输入范围：(1 ~ 20)",		//0
	"延时时间设置范围：(3 ~ 300)",		//1
	"定荷级数输入范围：(1 ~ 6)",			//2
	"圆形直径不能为0！",					//3
	"引伸计标距不能为0！",				//4
	"平行长度不能为0！",					//5
	"原始标距不能为0！",					//6
	"试件长度不能为0！",					//7
	"试件宽度不能为0！",					//8
	"修正系数不能为0！",					//9
	"试件面积不能为0！",					//10
	"试件高度不能为0！",					//11
	"试件跨距不能为0！",					//12
	"管壁厚度不能为0！",					//13
	"管段外径不能为0！",					//14
};

const char * const pSerialPutinCue[] = 
{
	"试件编号不支持以下字符：* ",	//0
	"输入的编号已存在,已自动生成",	//1
	"新的编号。",					//2
	"输入的编号已存在,请重新输入。",	//3
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_TEST_PARAMETER",zidata = "RAM_TEST_PARAMETER"
	static TEST_PARAMETER_TypeDef g_testParameter;
#pragma arm section


/* Private function prototypes -----------------------------------------------*/
static void TestParameterInit( void );
static void TestParameterConfig( void );
static uint8_t GetTestParameterIndex( uint8_t index );
static void TestParameterReadParameter( void );
static void GUI_TestParameter( void );
static void GUI_TestParameterRectangleFrame( void );
static void Traverse_TestParameter( void );
static void TestParameterIndexCrossTheBorderProcess( void );
static void TestParameterOneLevelMenuContentProcess( void );
static void TestParameterMoveIndexProcess( void );
static void TestParameterMoveCursorProcess( void );
static void TestParameterStatusProcess( void );
static void TestParameterPutinProcess( void );
static void TestParameterKeyProcess( void );
static void TestParameterShortcutCycleTask( void );
static void TestParameterPopWindowsProcess( void );
static void TestParameterLeavePageCheckCycle( void );
static void TestParameterExchangeSampleShape( uint8_t nowIndex );


/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadTestParameterPage
 * Description    : 试验参数页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestParameterPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	TestParameterInit();
	
	/* 参数配置 */
	TestParameterConfig();
	
	/* 获取参数 */
	TestParameterReadParameter();
	
	/* 索引值溢出处理 */
	TestParameterIndexCrossTheBorderProcess();
	
	/* 处理一级菜单内容 */
	TestParameterOneLevelMenuContentProcess();
	
	/* 画GUI框架 */
	GUI_TestParameter();
	
	/* 遍历 */
	Traverse_TestParameter();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_testParameter.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		TestParameterPutinProcess();
		
		/* 移动索引值 */
		TestParameterMoveIndexProcess();
		
		/* 状态处理 */
		TestParameterStatusProcess();
		
		/* 移动光标 */
		TestParameterMoveCursorProcess();	
		
		/* 快捷菜单 */
		TestParameterShortcutCycleTask();

		/* 按键处理 */
		TestParameterKeyProcess();
		
		/* 弹窗处理 */
		TestParameterPopWindowsProcess();
		
		/* 离开页 */
		TestParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterInit
 * Description    : 试验参数初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterInit( void )
{
	MODEL_TYPE_TypeDef modelType;
	
	GetTestContent();
	
	g_testParameter.testType = (TEST_TYPE_TypeDef)pHmi->test_standard_index;
	
	if (pHmi->test_standard_index == KZTY)
	{
		pHmi->test_standard_index = KZSNJS;
		
		GetTestContent();
		
		pcm_save();
	}
	
	if (g_testParameter.testType == NONE_TEST)
	{
		modelType = GetModelType();
		
		switch ( modelType )
		{
			case MODEL_KY:
				g_testParameter.testType = KYSNJS;
				break;
			case MODEL_KZ:
				g_testParameter.testType = KZSNJS;
				break;
			case MODEL_KZKY:
				g_testParameter.testType = KYSNJS;
				break;	
			case MODEL_UNIVERSAL:
				g_testParameter.testType = KYSNJS;
				break;
		}
	}
	
	g_testParameter.isIndexMove = NO;		
	g_testParameter.enableMoveIndex = ENABLE;
	g_testParameter.enablePutin = DISABLE;
	g_testParameter.enableArrow = DISABLE;
	g_testParameter.enableShift = DISABLE_SHIFT;
	g_testParameter.recordIndex = 0xff;
	g_testParameter.refreshShortcut = ENABLE;
	g_testParameter.leavePage.flagLeavePage = RESET;
	g_testParameter.leavePage.flagSaveData = RESET;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterConfig
 * Description    : 试验参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterConfig( void )
{
	switch ( g_testParameter.testType )
	{
		case NONE_TEST:
			
			break;
		case KYSNJS:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 6;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KYSNJS] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[TEST_TYPE_KYSNJS] 	= OBJECT_SPECIMEN_VARIETY;		/* 试件品种 */
			g_testParameter.indexArray[STRANG_GRADE_KYSNJS] = OBJECT_STRENGTH_GRADE;		/* 强度等级 */
			g_testParameter.indexArray[SAMPLE_AGE_KYSNJS] 	= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[SPEC_KYSNJS] 		= OBJECT_SPECIMEN_FORMAT;		/* 试件规格 */
			g_testParameter.indexArray[SAMPLE_NUM_KYSNJS] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYSNJS] 	= pValue_KYSNJS[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYSNJS] 		= pValue_KYSNJS[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KYSNJS] 	= pValue_KYSNJS[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYSNJS] 		= pValue_KYSNJS[4];
			g_testParameter.pParameterNameArray[SPEC_KYSNJS] 			= pValue_KYSNJS[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYSNJS] 		= pValue_KYSNJS[7];	
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYSNJS] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYSNJS] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KYSNJS]	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYSNJS] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYSNJS] 			= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYSNJS] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[0];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].parameterCnt		= 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].parameterCnt 	= 11;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].parameterCnt 			= 3;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].parameterType = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].parameterType 		= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].pParameterNameArray 	= pStrength_grade;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].pParameterNameArray 			= pSpecimen_spec_KYSNJS;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].pParameterNameArray 	= pSample_num;
			
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYSNJS].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYSNJS].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYSNJS].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYSNJS].pointBit 			= 0;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYSNJS].pointBit 	= 0;
			break;
		case KYJZSJ:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[2];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 6;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KYJZSJ] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[TEST_TYPE_KYJZSJ] 	= OBJECT_SPECIMEN_VARIETY;		/* 试件品种 */
			g_testParameter.indexArray[SAMPLE_AGE_KYJZSJ] 	= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[SPEC_KYJZSJ] 		= OBJECT_SPECIMEN_FORMAT;		/* 试件规格 */
			g_testParameter.indexArray[CORRECT_COF_KYJZSJ] 	= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
			g_testParameter.indexArray[SAMPLE_NUM_KYJZSJ] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYJZSJ] 	= pValue_KYJZSJ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYJZSJ] 		= pValue_KYJZSJ[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYJZSJ] 		= pValue_KYJZSJ[3];
			g_testParameter.pParameterNameArray[SPEC_KYJZSJ] 			= pValue_KYJZSJ[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KYJZSJ] 	= pValue_KYJZSJ[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYJZSJ] 		= pValue_KYJZSJ[7];	
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYJZSJ] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYJZSJ] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYJZSJ]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYJZSJ] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KYJZSJ] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYJZSJ] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[1];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].parameterCnt		= 10;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].parameterCnt 			= 2;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].parameterCnt 	= 2;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].parameterType 		= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].pParameterNameArray 		= pSample_type_KYJZSJ;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].pParameterNameArray 			= pSpecimen_spec_KYJZSJ;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].pParameterNameArray 	= pCorrect_cof_KYJZSJ;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].pParameterNameArray 	= pSample_num;
			
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYJZSJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYJZSJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYJZSJ].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYJZSJ].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYJZSJ].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYJZSJ].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYJZSJ].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYJZSJ].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYJZSJ].pointBit 	= 0;
			break;
		case KYHNT:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[1];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 6;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KYHNT] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[STRANG_GRADE_KYHNT] 	= OBJECT_STRENGTH_GRADE;		/* 强度等级 */
			g_testParameter.indexArray[SAMPLE_AGE_KYHNT] 	= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[SPEC_KYHNT] 			= OBJECT_SPECIMEN_FORMAT;		/* 试件规格 */
			g_testParameter.indexArray[CORRECT_COF_KYHNT] 	= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
			g_testParameter.indexArray[SAMPLE_NUM_KYHNT] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYHNT] 		= pValue_KYHNT[1];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KYHNT] 	= pValue_KYHNT[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYHNT] 		= pValue_KYHNT[3];
			g_testParameter.pParameterNameArray[SPEC_KYHNT] 			= pValue_KYHNT[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KYHNT] 		= pValue_KYHNT[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYHNT] 		= pValue_KYHNT[8];	
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KYHNT] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYHNT]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYHNT] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KYHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYHNT] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[2];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].parameterCnt	= 15;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].parameterCnt 			= 6;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].parameterCnt 	= 3;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].pParameterNameArray 	= pStrength_grade_KZHNT;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].pParameterNameArray 			= pSpecimen_spec_KYHNT;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].pParameterNameArray 	= pCorrect_coef_KYHNT;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].pParameterNameArray 		= pSample_num;
			
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYHNT].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYHNT].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYHNT].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYHNT].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYHNT].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYHNT].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYHNT].pointBit 	= 0;
			break;
		case KZHNT:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[1];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 6;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KZHNT] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[STRANG_GRADE_KZHNT] 	= OBJECT_STRENGTH_GRADE;		/* 强度等级 */
			g_testParameter.indexArray[SAMPLE_AGE_KZHNT] 	= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[SPEC_KZHNT] 			= OBJECT_SPECIMEN_FORMAT;		/* 试件规格 */
			g_testParameter.indexArray[CORRECT_COF_KZHNT] 	= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
			g_testParameter.indexArray[SAMPLE_NUM_KZHNT] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KZHNT] 		= pValue_KZHNT[1];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KZHNT] 	= pValue_KZHNT[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KZHNT] 		= pValue_KZHNT[3];
			g_testParameter.pParameterNameArray[SPEC_KZHNT] 			= pValue_KZHNT[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KZHNT] 		= pValue_KZHNT[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KZHNT] 		= pValue_KZHNT[8];	
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KZHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KZHNT] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KZHNT]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KZHNT] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KZHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KZHNT] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[3];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].parameterCnt	= 15;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].parameterCnt 			= 3;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].parameterCnt 	= 2;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].pParameterNameArray 	= pStrength_grade_KZHNT;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].pParameterNameArray 			= pSpecimen_spec;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].pParameterNameArray 	= pCorrect_coef;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].pParameterNameArray 		= pSample_num;
			
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZHNT].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KZHNT].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KZHNT].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KZHNT].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KZHNT].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KZHNT].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KZHNT].pointBit 	= 0;
			break;
		case KYQQZ:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[3];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 6;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KYQQZ] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[TEST_TYPE_KYQQZ] 	= OBJECT_SPECIMEN_VARIETY;		/* 试件品种 */
			g_testParameter.indexArray[LENGTH_KYQQZ] 		= OBJECT_SPECIMEN_LENTH;		/* 试件长度 */
			g_testParameter.indexArray[WIDTH_KYQQZ] 		= OBJECT_SPECIMEN_WIDTH;		/* 试件宽度 */
			g_testParameter.indexArray[HIGH_KYQQZ] 			= OBJECT_SPECIMEN_HIGH;			/* 试件高度 */
			g_testParameter.indexArray[SAMPLE_NUM_KYQQZ] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYQQZ] 	= pValue_KYQQZ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYQQZ] 	= pValue_KYQQZ[2];
			g_testParameter.pParameterNameArray[LENGTH_KYQQZ] 		= pValue_KYQQZ[3];
			g_testParameter.pParameterNameArray[WIDTH_KYQQZ] 		= pValue_KYQQZ[4];
			g_testParameter.pParameterNameArray[HIGH_KYQQZ] 		= pValue_KYQQZ[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYQQZ] 	= pValue_KYQQZ[7];	
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYQQZ] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYQQZ] 	= "NULL";
			g_testParameter.pParameterUnitArray[LENGTH_KYQQZ]		= pUnitType[4];
			g_testParameter.pParameterUnitArray[WIDTH_KYQQZ] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[HIGH_KYQQZ] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYQQZ] 	= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[4];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].parameterCnt		= 8;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].parameterCnt 		= 9;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].parameterCnt 			= 9;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].parameterCnt 			= 9;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].parameterCnt 	= 11;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].parameterType 		= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].parameterType 		= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].pParameterNameArray 		= pSample_type_KYQQZ;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].pParameterNameArray 			= pLength_KYQQZ;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].pParameterNameArray 			= pWidth_KYQQZ;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].pParameterNameArray 			= pHigh_KYQQZ;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].pParameterNameArray 		= pSample_num_KYQQZ;
			
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYQQZ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYQQZ].saveType 		= TYPE_CHAR;
			g_testParameter.oneLevelMenu[LENGTH_KYQQZ].saveType 		= TYPE_INT;
			g_testParameter.oneLevelMenu[WIDTH_KYQQZ].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[HIGH_KYQQZ].saveType 			= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYQQZ].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYQQZ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYQQZ].pointBit 		= 0;
			g_testParameter.oneLevelMenu[LENGTH_KYQQZ].pointBit 		= 0;
			g_testParameter.oneLevelMenu[WIDTH_KYQQZ].pointBit 			= 0;
			g_testParameter.oneLevelMenu[HIGH_KYQQZ].pointBit 			= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYQQZ].pointBit 	= 0;
			break;
		case KYZJDH:
			
			break;
		case KYTY:
		{
			TYKY_TEST_SHAPE_TypeDef sampleShape;
			
			if (pTest->sample_type_index > TYKY_SHAPE_IRREGULAR)
			{
				pTest->sample_type_index = TYKY_SHAPE_RECTANGLE;
			}
			
			sampleShape = (TYKY_TEST_SHAPE_TypeDef)pTest->sample_shape_index;
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[6];
			
			/* 试验标准 */
			g_testParameter.pTestStandard = "------";
		
			switch ( sampleShape )
			{
				case TYKY_SHAPE_RECTANGLE:
					/* 试块个数 */
					g_testParameter.curParameterNum = 6;
					
					/* 索引值 */
					g_testParameter.indexArray[TEST_SERIAL_RECTANGLE_KYTY] 	= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
					g_testParameter.indexArray[SAMPLE_SHAPE_RECTANGLE_KYTY] = OBJECT_SHAPE;					/* 试件形状 */
					g_testParameter.indexArray[SAMPLE_LENTH_RECTANGLE_KYTY] = OBJECT_SPECIMEN_LENTH;		/* 长度	    */
					g_testParameter.indexArray[SAMPLE_WIDTH_RECTANGLE_KYTY] = OBJECT_SPECIMEN_WIDTH;		/* 宽度 	*/
					g_testParameter.indexArray[CORRECT_COF_RECTANGLE_KYTY] 	= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
					g_testParameter.indexArray[SAMPLE_NUM_RECTANGLE_KYTY] 	= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */			/* 延时时间 */
				
					/* 参数名称 */
					g_testParameter.pParameterNameArray[TEST_SERIAL_RECTANGLE_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RECTANGLE_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_LENTH_RECTANGLE_KYTY] 	= pValue_KYTY[10];
					g_testParameter.pParameterNameArray[SAMPLE_WIDTH_RECTANGLE_KYTY] 	= pValue_KYTY[11];
					g_testParameter.pParameterNameArray[CORRECT_COF_RECTANGLE_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_RECTANGLE_KYTY] 		= pValue_KYTY[6];		
				
					/* 单位 */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_LENTH_RECTANGLE_KYTY]	= pUnitType[4];
					g_testParameter.pParameterUnitArray[SAMPLE_WIDTH_RECTANGLE_KYTY] 	= pUnitType[4];
					g_testParameter.pParameterUnitArray[CORRECT_COF_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_RECTANGLE_KYTY] 		= "NULL";
					
					/* 二级菜单参数个数 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].parameterCnt 		= 7;
					
					/* 二级菜单类型 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* 二级菜单参数名 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].pParameterNameArray 	= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].pParameterNameArray 	= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].pParameterNameArray 	= pSample_num;
					
					/* 数据保存类型 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].saveType 		= TYPE_INT;	

					/* 小数点位数 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[CORRECT_COF_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].pointBit 		= 0;
					break;
				case TYKY_SHAPE_ROUND:
					/* 试块个数 */
					g_testParameter.curParameterNum = 5;
					
					/* 索引值 */
					g_testParameter.indexArray[TEST_SERIAL_ROUND_KYTY] 		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
					g_testParameter.indexArray[SAMPLE_SHAPE_ROUND_KYTY] 	= OBJECT_SHAPE;					/* 试件形状 */
					g_testParameter.indexArray[SAMPLE_DIAMETER_ROUND_KYTY] 	= OBJECT_ROUND_DIAMETER;				/* 直径	    */
					g_testParameter.indexArray[CORRECT_COF_ROUND_KYTY] 		= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
					g_testParameter.indexArray[SAMPLE_NUM_ROUND_KYTY] 		= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */			/* 延时时间 */
				
					/* 参数名称 */
					g_testParameter.pParameterNameArray[TEST_SERIAL_ROUND_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_ROUND_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_DIAMETER_ROUND_KYTY] = pValue_KYTY[12];
					g_testParameter.pParameterNameArray[CORRECT_COF_ROUND_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_ROUND_KYTY] 		= pValue_KYTY[6];		
				
					/* 单位 */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_DIAMETER_ROUND_KYTY]	= pUnitType[4];
					g_testParameter.pParameterUnitArray[CORRECT_COF_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_ROUND_KYTY] 		= "NULL";
					
					/* 二级菜单参数个数 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].parameterCnt 	= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].parameterCnt 		= 7;
					
					/* 二级菜单类型 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* 二级菜单参数名 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].pParameterNameArray 		= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].pParameterNameArray 		= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].pParameterNameArray 		= pSample_num;
					
					/* 数据保存类型 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].saveType 	= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_ROUND_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KYTY].saveType 		= TYPE_INT;	

					/* 小数点位数 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].pointBit 	= 2;
					g_testParameter.oneLevelMenu[CORRECT_COF_ROUND_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KYTY].pointBit 		= 0;
					break;
				case TYKY_SHAPE_IRREGULAR:
					/* 试块个数 */
					g_testParameter.curParameterNum = 5;
					
					/* 索引值 */
					g_testParameter.indexArray[TEST_SERIAL_IRREGULAR_KYTY] 		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
					g_testParameter.indexArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= OBJECT_SHAPE;					/* 试件形状 */
					g_testParameter.indexArray[SAMPLE_AREA_IRREGULAR_KYTY] 		= OBJECT_AREA;					/* 面积	    */
					g_testParameter.indexArray[CORRECT_COF_IRREGULAR_KYTY] 		= OBJECT_CORRECTION_FACTOR;		/* 修正系数 */
					g_testParameter.indexArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= OBJECT_SPECIMEN_NUMS;			/* 试件块数 */			/* 延时时间 */
				
					/* 参数名称 */
					g_testParameter.pParameterNameArray[TEST_SERIAL_IRREGULAR_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_AREA_IRREGULAR_KYTY] 	= pValue_KYTY[4];
					g_testParameter.pParameterNameArray[CORRECT_COF_IRREGULAR_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= pValue_KYTY[6];	
				
					/* 单位 */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_AREA_IRREGULAR_KYTY]		= pUnitType[5];
					g_testParameter.pParameterUnitArray[CORRECT_COF_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= "NULL";
					
					/* 二级菜单参数个数 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].parameterCnt 		= 7;
					
					/* 二级菜单类型 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].parameterType 		= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* 二级菜单参数名 */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].pParameterNameArray 		= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].pParameterNameArray 		= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].pParameterNameArray 		= pSample_num;
					
					/* 数据保存类型 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_IRREGULAR_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].saveType 		= TYPE_INT;	

					/* 小数点位数 */
					g_testParameter.oneLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[CORRECT_COF_IRREGULAR_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].pointBit 		= 0;				
					break;				
			}		
			break;
		}
		case KZSNJS:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 7;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_KZSNJS] 	    = OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[TEST_TYPE_KZSNJS] 	    = OBJECT_SPECIMEN_VARIETY;		/* 试件品种 */
			g_testParameter.indexArray[STRANG_GRADE_KZSNJS] 	= OBJECT_STRENGTH_GRADE;		/* 强度等级 */
			g_testParameter.indexArray[SAMPLE_AGE_KZSNJS] 		= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[ZFX_LEN_KZSNJS] 	        = OBJECT_SPECIMEN_LENTH;		/* 试件边长 */
			g_testParameter.indexArray[ZFX_SPAN_KZSNJS] 	    = OBJECT_SPECIMEN_SPAN;			/* 试件跨距 */
            g_testParameter.indexArray[SAMPLE_NUM_KZSNJS] 	    = OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KZSNJS] 	= pValue_KZSNJS[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KZSNJS] 	    = pValue_KZSNJS[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KZSNJS] 	= pValue_KZSNJS[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KZSNJS] 		= pValue_KZSNJS[4];
			g_testParameter.pParameterNameArray[ZFX_LEN_KZSNJS] 		= pValue_KZSNJS[5];
			g_testParameter.pParameterNameArray[ZFX_SPAN_KZSNJS] 		= pValue_KZSNJS[6];
            g_testParameter.pParameterNameArray[SAMPLE_NUM_KZSNJS] 		= pValue_KZSNJS[8];
		
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KZSNJS] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KZSNJS] 	    = "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KZSNJS]	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KZSNJS] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[ZFX_LEN_KZSNJS] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[ZFX_SPAN_KZSNJS] 		= pUnitType[4];
            g_testParameter.pParameterUnitArray[SAMPLE_NUM_KZSNJS] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[7];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].parameterCnt	    = 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].parameterCnt 	= 11;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].parameterCnt 	    = 5;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].parameterCnt 	    = 6;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].parameterType = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].parameterType 	    = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].parameterType 	= USE_USER_DEFINE;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].pParameterNameArray 	= pStrength_grade;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].pParameterNameArray 		= pZfx_length;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].pParameterNameArray 		= pZfx_span;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].pParameterNameArray 	= pSample_num;
            
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZSNJS].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[ZFX_LEN_KZSNJS].saveType 		= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[ZFX_SPAN_KZSNJS].saveType 		= TYPE_FLOAT;	
            g_testParameter.oneLevelMenu[SAMPLE_NUM_KZSNJS].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[ZFX_LEN_KZSNJS].pointBit 		= 2;
			g_testParameter.oneLevelMenu[ZFX_SPAN_KZSNJS].pointBit 		= 2;
            g_testParameter.oneLevelMenu[SAMPLE_NUM_KZSNJS].pointBit 	= 0;
			break;
		case KZYJSNJ:
			/* 试验标准 */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* 试块个数 */
			g_testParameter.curParameterNum = 7;
			
			/* 索引值 */
			g_testParameter.indexArray[TEST_SERIAL_YJSNJ] 	    = OBJECT_SPECIMEN_SERIAL;		/* 试件编号 */	
			g_testParameter.indexArray[TEST_TYPE_YJSNJ] 	    = OBJECT_SPECIMEN_VARIETY;		/* 试件品种 */
			g_testParameter.indexArray[STRANG_GRADE_YJSNJ] 		= OBJECT_STRENGTH_GRADE;		/* 强度等级 */
			g_testParameter.indexArray[SAMPLE_AGE_YJSNJ] 		= OBJECT_SPECIMEN_AGE;			/* 试件龄期 */
			g_testParameter.indexArray[ZFX_LEN_YJSNJ] 	        = OBJECT_SPECIMEN_LENTH;		/* 试件边长 */
			g_testParameter.indexArray[ZFX_SPAN_YJSNJ] 	    	= OBJECT_SPECIMEN_SPAN;			/* 试件跨距 */
            g_testParameter.indexArray[SAMPLE_NUM_YJSNJ] 	    = OBJECT_SPECIMEN_NUMS;			/* 试件块数 */
			
			/* 参数名称 */
			g_testParameter.pParameterNameArray[TEST_SERIAL_YJSNJ] 		= pValue_KZYJSNJ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_YJSNJ] 	    = pValue_KZYJSNJ[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_YJSNJ] 	= pValue_KZYJSNJ[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_YJSNJ] 		= pValue_KZYJSNJ[4];
			g_testParameter.pParameterNameArray[ZFX_LEN_YJSNJ] 			= pValue_KZYJSNJ[5];
			g_testParameter.pParameterNameArray[ZFX_SPAN_YJSNJ] 		= pValue_KZYJSNJ[6];
            g_testParameter.pParameterNameArray[SAMPLE_NUM_YJSNJ] 		= pValue_KZYJSNJ[8];
			
			/* 单位 */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_YJSNJ] 		= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_YJSNJ] 	    = "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_YJSNJ]		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_YJSNJ] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[ZFX_LEN_YJSNJ] 			= pUnitType[4];
			g_testParameter.pParameterUnitArray[ZFX_SPAN_YJSNJ] 		= pUnitType[4];
            g_testParameter.pParameterUnitArray[SAMPLE_NUM_YJSNJ] 		= "NULL";
			
			/* 标题 */
			g_testParameter.pTitle = pTestParameterTitleName[8];
			
			/* 二级菜单参数个数 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].parameterCnt	    = 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].parameterCnt 	= 5;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].parameterCnt 	    = 5;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].parameterCnt 	    = 6;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].parameterCnt 	= 7;
			
			/* 二级菜单类型 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].parameterType 	    = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].parameterType 		= USE_USER_DEFINE;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].parameterType 	= USE_USER_DEFINE;
			
			/* 二级菜单参数名 */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].pParameterNameArray 	= pStrength_grade_KZYJSNJ;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].pParameterNameArray 		= pZfx_length;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].pParameterNameArray 		= pZfx_span;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].pParameterNameArray 		= pSample_num;
            
			/* 数据保存类型 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_YJSNJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_YJSNJ].saveType 		= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_YJSNJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_YJSNJ].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[ZFX_LEN_YJSNJ].saveType 		= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[ZFX_SPAN_YJSNJ].saveType 		= TYPE_FLOAT;	
            g_testParameter.oneLevelMenu[SAMPLE_NUM_YJSNJ].saveType 	= TYPE_INT;	

			/* 小数点位数 */
			g_testParameter.oneLevelMenu[TEST_SERIAL_YJSNJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_YJSNJ].pointBit 		= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_YJSNJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_YJSNJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[ZFX_LEN_YJSNJ].pointBit 		= 2;
			g_testParameter.oneLevelMenu[ZFX_SPAN_YJSNJ].pointBit 		= 2;
            g_testParameter.oneLevelMenu[SAMPLE_NUM_YJSNJ].pointBit 	= 0;
			break;
		case KZTY:
			
			break;
		case KLJSSW:
			{
				JSSWKL_TEST_SHAPE_TypeDef sampleShape;
				
				if (pTest->sample_type_index > JSSWKL_SHAPE_IRREGULAR)
				{
					pTest->sample_type_index = JSSWKL_SHAPE_RECTANGLE;
				}
				
				sampleShape = (JSSWKL_TEST_SHAPE_TypeDef)pTest->sample_shape_index;
				
				/* 标题 */
				g_testParameter.pTitle = pTestParameterTitleName[10];
				
				/* 试验标准 */
				g_testParameter.pTestStandard = pTestStandard[4];
			
				switch ( sampleShape )
				{
					case JSSWKL_SHAPE_RECTANGLE:
						/* 试块个数 */
						g_testParameter.curParameterNum = 9;
						
						/* 索引值 */
						g_testParameter.indexArray[TEST_SERIAL_RECTANGLE_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 				= OBJECT_SHAPE;					/* 形状 	 */
						g_testParameter.indexArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW] 			= OBJECT_SPECIMEN_LENTH;		/* 试件长度  */
						g_testParameter.indexArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 				= OBJECT_SPECIMEN_WIDTH;		/* 试件宽度 	 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 		= OBJECT_AREA;					/* 原始截面积 */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* 引伸计标距 */
						g_testParameter.indexArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* 原始标距 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* 平行长度 	 */
						g_testParameter.indexArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* 试件根数 	 */
					
						/* 参数名称 */
						g_testParameter.pParameterNameArray[TEST_SERIAL_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW] 		= pValue_KLJSSW[8];
						g_testParameter.pParameterNameArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[9];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 	= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]	= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 		= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	    = pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	       	= pValue_KLJSSW[5];
						
						/* 单位 */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_RECTANGLE_KLJSSW] 			= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 			= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW]		= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 	= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]	= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 		= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	    = pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	       	= "NULL";
						
						/* 二级菜单参数个数 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].parameterCnt			= 4;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].parameterCnt 		= 0;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].parameterCnt 	    	= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].parameterCnt  	   	= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].parameterCnt  	       	= 7;
						
						/* 二级菜单类型 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].parameterType 			= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].parameterType 			= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].parameterType 			= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].parameterType 	= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].parameterType	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].parameterType 	    = IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].parameterType  	   	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].parameterType  	       	= USE_USER_DEFINE;
						
						/* 二级菜单参数名 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].pParameterNameArray 		= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].pParameterNameArray	= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].pParameterNameArray  	   	= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].pParameterNameArray  	       	= pSample_num;
						
						/* 数据保存类型 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].saveType 			= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].saveType 			= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].saveType 		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].saveType 	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].saveType		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].saveType  	   	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].saveType  	       	= TYPE_INT;

						/* 小数点位数 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].pointBit 			= 0;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].pointBit 			= 0;
						g_testParameter.oneLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].pointBit 		= 2;
						g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].pointBit 	= 2;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].pointBit		= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].pointBit  	   	= 2;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].pointBit  	       	= 0;
						break;
					case JSSWKL_SHAPE_ROUND:
						/* 试块个数 */
						g_testParameter.curParameterNum = 8;
						
						/* 索引值 */
						g_testParameter.indexArray[TEST_SERIAL_ROUND_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_ROUND_KLJSSW] 				= OBJECT_SHAPE;					/* 形状 	 */
						g_testParameter.indexArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= OBJECT_ROUND_DIAMETER;		/* 圆形直径 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 		= OBJECT_AREA;					/* 原始截面积 */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* 引伸计标距 */
						g_testParameter.indexArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* 原始标距 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_ROUND_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* 平行长度 	 */
						g_testParameter.indexArray[SAMPLE_NUM_ROUND_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* 试件根数 	 */
					
						/* 参数名称 */
						g_testParameter.pParameterNameArray[TEST_SERIAL_ROUND_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_ROUND_KLJSSW] 					= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= pValue_KLJSSW[2];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_ROUND_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_ROUND_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* 单位 */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_ROUND_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_ROUND_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_ROUND_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_ROUND_KLJSSW] 	       			= "NULL";
						
						/* 二级菜单参数个数 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].parameterCnt 	    			= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].parameterCnt  	       			= 7;
						
						/* 二级菜单类型 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].parameterType 		= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].parameterType		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].parameterType 	    	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* 二级菜单参数名 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].pParameterNameArray 					= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].pParameterNameArray 				= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].pParameterNameArray  	   			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].pParameterNameArray  	       			= pSample_num;
						
						/* 数据保存类型 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].saveType 					= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].saveType				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].saveType 					= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].saveType  	       			= TYPE_INT;

						/* 小数点位数 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KLJSSW].pointBit 					= 0;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].pointBit 					= 0;
						g_testParameter.oneLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].pointBit				= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].pointBit 					= 2;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].pointBit  	   			= 2;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].pointBit  	   				= 0;
						break;
					case JSSWKL_SHAPE_TUBE:
						/* 试块个数 */
						g_testParameter.curParameterNum = 9;
						
						/* 索引值 */
						g_testParameter.indexArray[TEST_SERIAL_TUBE_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_TUBE_KLJSSW] 				= OBJECT_SHAPE;					/* 形状 	 */
						g_testParameter.indexArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW] 		= OBJECT_PIPE_THICKNESS;		/* 管段厚度  */
						g_testParameter.indexArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= OBJECT_PIPE_OUTER_DIAMETER;	/* 管段外直径 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 		= OBJECT_AREA;					/* 原始截面积 */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= OBJECT_EXTERNSOMETER_GAUGE;	/* 引伸计标距 */
						g_testParameter.indexArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 	        	= OBJECT_ORIGINAL_GAUGE;		/* 原始标距 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_TUBE_KLJSSW] 	        	= OBJECT_PARALLEL_LENTH;		/* 平行长度 	 */
						g_testParameter.indexArray[SAMPLE_NUM_TUBE_KLJSSW] 	        		= OBJECT_SPECIMEN_NUMS;			/* 试件根数 	 */
					
						/* 参数名称 */
						g_testParameter.pParameterNameArray[TEST_SERIAL_TUBE_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_TUBE_KLJSSW] 					= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW] 			= pValue_KLJSSW[10];
						g_testParameter.pParameterNameArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= pValue_KLJSSW[11];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_TUBE_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_TUBE_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* 单位 */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_TUBE_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_TUBE_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_TUBE_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_TUBE_KLJSSW] 	       			= "NULL";
						
						/* 二级菜单参数个数 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].parameterCnt 		= 0;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].parameterCnt 	    		= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].parameterCnt  	       			= 7;
						
						/* 二级菜单类型 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].parameterType 	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].parameterType 		= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].parameterType			= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].parameterType 	    		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* 二级菜单参数名 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].pParameterNameArray 					= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].pParameterNameArray 					= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].pParameterNameArray  	   			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].pParameterNameArray  	       			= pSample_num;
						
						/* 数据保存类型 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_TUBE_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].saveType 				= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].saveType 		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].saveType 	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].saveType			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].saveType 				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].saveType  	       			= TYPE_INT;

						/* 小数点位数 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_TUBE_KLJSSW].pointBit 					= 0;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].pointBit 				= 0;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].pointBit 		= 2;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].pointBit 	= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].pointBit			= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].pointBit 				= 2;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].pointBit  	   			= 2;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].pointBit  	       			= 0;
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						/* 试块个数 */
						g_testParameter.curParameterNum = 7;
						
						/* 索引值 */
						g_testParameter.indexArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* 试件编号  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= OBJECT_SHAPE;					/* 形状 	 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 		= OBJECT_AREA;					/* 原始截面积 */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* 引伸计标距 */
						g_testParameter.indexArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* 原始标距 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* 平行长度 	 */
						g_testParameter.indexArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* 试件根数 	 */
					
						/* 参数名称 */
						g_testParameter.pParameterNameArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* 单位 */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= "NULL";
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	       			= "NULL";
						
						/* 二级菜单参数个数 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].parameterCnt 	    		= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].parameterCnt  	       			= 7;
						
						/* 二级菜单类型 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].parameterType		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].parameterType 	    	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* 二级菜单参数名 */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].pParameterNameArray 				= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].pParameterNameArray  	   		= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].pParameterNameArray  	       		= pSample_num;
						
						/* 数据保存类型 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].saveType 					= TYPE_INT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].saveType			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].saveType 				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].saveType  	       			= TYPE_INT;

						/* 小数点位数 */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].pointBit 					= 0;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].pointBit 					= 0;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].pointBit 			= 2;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].pointBit			= 2;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].pointBit 				= 2;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].pointBit  	   			= 2;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].pointBit  	   				= 0;
						break;
				}
				break;
			}
	}
}


/*------------------------------------------------------------
 * Function Name  : GetTestParameterIndex
 * Description    : 获取试验参数索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetTestParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t testIndex = 0xff;	//错误的值
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		if (g_testParameter.indexArray[i] == handle)
		{
			testIndex = i;
			break;
		}
	}
	
	return testIndex;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterReadParameter( void )
{
	uint8_t index = 0;
	TEST_TypeDef * const pCurTest = pTest;
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_SERIAL);
	if (index != 0xff)
	{
		strcpy(g_testParameter.parameterData[index],pCurTest->test_serial);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_VARIETY);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->sample_type_index;
		strcpy(g_testParameter.parameterData[index],pCurTest->custom_sample_type);
	}
	
	index = GetTestParameterIndex(OBJECT_STRENGTH_GRADE);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->strength_grade_index;
		strcpy(g_testParameter.parameterData[index],pCurTest->custom_strength_grade);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_AGE);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->sample_age_index;
		numtochar(MAX_TEST_PARAMETER_PUTIN_BIT,pCurTest->sample_age,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_FORMAT);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->sample_spec_index;
		strcpy(g_testParameter.parameterData[index],pCurTest->custom_sample_spec);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_NUMS);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->sample_num_index;
		numtochar(MAX_TEST_PARAMETER_PUTIN_BIT,pCurTest->sample_num,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_LENTH);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->zfx_length_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->zfx_length,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_WIDTH);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->zfx_width_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->zfx_width,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_HIGH);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->zfx_higth_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->zfx_higth,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_SPAN);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->zfx_span_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->zfx_span,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_CORRECTION_FACTOR);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->correct_cof_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->correct_cof,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SHAPE);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->sample_shape_index;
	}
	
	index = GetTestParameterIndex(OBJECT_AREA);
	if (index != 0xff)
	{
		switch ( g_testParameter.testType )
		{
			case KYTY:
				switch (pCurTest->sample_shape_index)
				{
					case TYKY_SHAPE_RECTANGLE:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->gz_area,g_testParameter.parameterData[index]);
						break;
					case TYKY_SHAPE_ROUND:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->gz_area,g_testParameter.parameterData[index]);
						break;
					case TYKY_SHAPE_IRREGULAR:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->bgz_area,g_testParameter.parameterData[index]);
						break;
					default:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->bgz_area,g_testParameter.parameterData[index]);
						break;
				}
				break;
			case KLJSSW:
				switch (pCurTest->sample_shape_index)
				{
					case JSSWKL_SHAPE_RECTANGLE:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->gz_area,g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_ROUND:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->gz_area,g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_TUBE:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->gz_area,g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->bgz_area,g_testParameter.parameterData[index]);
						break;
					default:
						floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->bgz_area,g_testParameter.parameterData[index]);
						break;
				}
				break;
			default:
				break;		
		}	
	}
	
	index = GetTestParameterIndex(OBJECT_ROUND_DIAMETER);
	if (index != 0xff)
	{
		g_testParameter.twoLevelMenu[index].index = pCurTest->yx_diameter_index;
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->yx_diameter,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_EXTERNSOMETER_GAUGE);
	if (index != 0xff)
	{
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->extensometerGauge,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PARALLEL_LENTH);
	if (index != 0xff)
	{
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->parallelLenth,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_ORIGINAL_GAUGE);
	if (index != 0xff)
	{
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->originalGauge,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PIPE_THICKNESS);
	if (index != 0xff)
	{
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->pipeThickness,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PIPE_OUTER_DIAMETER);
	if (index != 0xff)
	{
		floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,pCurTest->pipeOuterDiameter,g_testParameter.parameterData[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterWriteParameter( void )
{
	uint8_t index = 0;
	TEST_TypeDef * const pCurTest = pTest;
	
	strcpy(pCurTest->test_standard,g_testParameter.pTestStandard);
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_SERIAL);
	if (index != 0xff)
	{
		strcpy(pCurTest->test_serial,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_VARIETY);
	if (index != 0xff)
	{
		pCurTest->sample_type_index = g_testParameter.twoLevelMenu[index].index;
		strcpy(pCurTest->custom_sample_type,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_STRENGTH_GRADE);
	if (index != 0xff)
	{
		pCurTest->strength_grade_index = g_testParameter.twoLevelMenu[index].index;
		strcpy(pCurTest->custom_strength_grade,g_testParameter.parameterData[index]);
	}

	index = GetTestParameterIndex(OBJECT_SPECIMEN_AGE);
	if (index != 0xff)
	{
		pCurTest->sample_age_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->sample_age = (uint8_t)ustrtoul(g_testParameter.parameterData[index],0,10);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_FORMAT);
	if (index != 0xff)
	{
		pCurTest->sample_spec_index = g_testParameter.twoLevelMenu[index].index;
		strcpy(pCurTest->custom_sample_spec,g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_ROUND_DIAMETER);
	if (index != 0xff)
	{
		pCurTest->yx_diameter_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->yx_diameter = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_NUMS);
	if (index != 0xff)
	{
		pCurTest->sample_num_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->sample_num = (uint8_t)ustrtoul(g_testParameter.parameterData[index],0,10);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_LENTH);
	if (index != 0xff)
	{
		pCurTest->zfx_length_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->zfx_length = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_WIDTH);
	if (index != 0xff)
	{
		pCurTest->zfx_width_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->zfx_width = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_HIGH);
	if (index != 0xff)
	{
		pCurTest->zfx_higth_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->zfx_higth = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SPECIMEN_SPAN);
	if (index != 0xff)
	{
		pCurTest->zfx_span_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->zfx_span = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_CORRECTION_FACTOR);
	if (index != 0xff)
	{
		pCurTest->correct_cof_index = g_testParameter.twoLevelMenu[index].index;
		pCurTest->correct_cof = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_SHAPE);
	if (index != 0xff)
	{
		pCurTest->sample_shape_index = g_testParameter.twoLevelMenu[index].index;
	}
	
	index = GetTestParameterIndex(OBJECT_AREA);
	if (index != 0xff)
	{
		switch ( g_testParameter.testType )
		{
			case KYTY:	
				switch ( pCurTest->sample_shape_index )
				{
					case TYKY_SHAPE_RECTANGLE:
					case TYKY_SHAPE_ROUND:
						break;
					case TYKY_SHAPE_IRREGULAR:
						pCurTest->bgz_area = str2float(g_testParameter.parameterData[index]);
						break;
				}
				break;
			case KLJSSW:
				switch ( pCurTest->sample_shape_index )
				{
					case JSSWKL_SHAPE_RECTANGLE:
						pCurTest->gz_area = str2float(g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_ROUND:
						pCurTest->gz_area = str2float(g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_TUBE:
						pCurTest->gz_area = str2float(g_testParameter.parameterData[index]);
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						pCurTest->bgz_area = str2float(g_testParameter.parameterData[index]);
						break;
				}			
				break;
			default:
				break;
		}
	}
	else
	{
		switch ( g_testParameter.testType )
		{
			case KYTY:	
				switch ( pCurTest->sample_shape_index )
				{
					case TYKY_SHAPE_RECTANGLE:
						pCurTest->gz_area = pCurTest->zfx_length * pCurTest->zfx_width;
						break;					
					case TYKY_SHAPE_ROUND:
						pCurTest->gz_area = GetCircularArea(pCurTest->yx_diameter);
						break;					
					case TYKY_SHAPE_IRREGULAR:
						break;
				}
				break;
			case KLJSSW:			
				break;
			default:
				break;
		}
	}
	
	index = GetTestParameterIndex(OBJECT_EXTERNSOMETER_GAUGE);
	if (index != 0xff)
	{
		pCurTest->extensometerGauge = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PARALLEL_LENTH);
	if (index != 0xff)
	{
		pCurTest->parallelLenth = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_ORIGINAL_GAUGE);
	if (index != 0xff)
	{
		pCurTest->originalGauge = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PIPE_THICKNESS);
	if (index != 0xff)
	{
		pCurTest->pipeThickness = str2float(g_testParameter.parameterData[index]);
	}
	
	index = GetTestParameterIndex(OBJECT_PIPE_OUTER_DIAMETER);
	if (index != 0xff)
	{
		pCurTest->pipeOuterDiameter = str2float(g_testParameter.parameterData[index]);
	}
	
	pcm_save();
}

/*------------------------------------------------------------
 * Function Name  : TestParameterOneLevelMenuContentProcess
 * Description    : 一级菜单内容处理(必须放在索引值越界处理之后)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterOneLevelMenuContentProcess( void )
{
	uint8_t i;
	const char *pParaName = NULL;
	uint8_t index = 0;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		index = g_testParameter.twoLevelMenu[i].index;	
		
		if (g_testParameter.twoLevelMenu[i].parameterType == NONE_USE_USER_DEFINE)
		{					
			pParaName = g_testParameter.twoLevelMenu[i].pParameterNameArray[index];
			
			strcpy(g_testParameter.parameterData[i],pParaName);
		}
		else if (g_testParameter.twoLevelMenu[i].parameterType == USE_USER_DEFINE) 
		{
			if (index)	//非“自定义”的其他内容
			{
				pParaName = g_testParameter.twoLevelMenu[i].pParameterNameArray[index];
			
				strcpy(g_testParameter.parameterData[i],pParaName);
			}
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigTestParameterRectangleFrameCoordinate
 * Description    : 配置试验参数界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigTestParameterRectangleFrameCoordinate( uint16_t startX, uint16_t startY, uint8_t rowDistance )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		g_testParameter.oneLevelMenu[i].x = startX;
		g_testParameter.oneLevelMenu[i].y = startY;
		g_testParameter.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_testParameter.oneLevelMenu[i].backColor = COLOR_BACK;
		g_testParameter.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_testParameter.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_testParameter.oneLevelMenu[i].lenth = 264;
		g_testParameter.oneLevelMenu[i].width = 30;
		g_testParameter.oneLevelMenu[i].fontSize = 24;
		g_testParameter.oneLevelMenu[i].rowDistance = rowDistance;
		g_testParameter.oneLevelMenu[i].columnDistance = 0;
		g_testParameter.oneLevelMenu[i].lineWidth = 2;
		
		g_testParameter.twoLevelMenu[i].x = g_testParameter.oneLevelMenu[i].x + g_testParameter.oneLevelMenu[i].lenth + 36;
		g_testParameter.twoLevelMenu[i].y = g_testParameter.oneLevelMenu[i].y;
		g_testParameter.twoLevelMenu[i].pointColor = g_testParameter.oneLevelMenu[i].pointColor;
		g_testParameter.twoLevelMenu[i].backColor = g_testParameter.oneLevelMenu[i].backColor;
		g_testParameter.twoLevelMenu[i].lenth = 200;
		g_testParameter.twoLevelMenu[i].fontSize = 24;
		g_testParameter.twoLevelMenu[i].rowDistance = 2;
		g_testParameter.twoLevelMenu[i].columnDistance = 0;
		g_testParameter.twoLevelMenu[i].lineWidth = 2;				
		g_testParameter.twoLevelMenu[i].width = g_testParameter.twoLevelMenu[i].parameterCnt * \
												(g_testParameter.twoLevelMenu[i].fontSize + g_testParameter.twoLevelMenu[i].rowDistance) + \
												g_testParameter.twoLevelMenu[i].rowDistance + 2 * g_testParameter.twoLevelMenu[i].lineWidth;
		g_testParameter.twoLevelMenu[i].maxUpY = MAX_TWO_MENU_HIGH_POS;
		g_testParameter.twoLevelMenu[i].maxDownY = MIN_TWO_MENU_HIGH_POS;
		
		startY += g_testParameter.oneLevelMenu[i].width + g_testParameter.oneLevelMenu[i].rowDistance - \
				  g_testParameter.oneLevelMenu[i].lineWidth;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterDrawOneRectangleFrame
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestParameterDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_testParameter.oneLevelMenu[index].x;
	rectangle.y = g_testParameter.oneLevelMenu[index].y;
	rectangle.lenth = g_testParameter.oneLevelMenu[index].lenth;
	rectangle.width = g_testParameter.oneLevelMenu[index].width;
	rectangle.lineWidth = g_testParameter.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_testParameter.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterRectangleFrame
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		GUI_TestParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterDrawOneRowOneLevelMenu
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestParameterDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x - 6 * 24;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + \
					   g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_testParameter.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameterOneLevelMenu
 * Description    : 试验参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestParameterOneLevelMenu( void )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		GUI_TestParameterDrawOneRowOneLevelMenu(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_TestParameter
 * Description    : 试验参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_TestParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_testParameter.pTitle);
	
	ConfigTestParameterRectangleFrameCoordinate(204,74,10);
	
	GUI_TestParameterRectangleFrame();
	
	GUI_TestParameterOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneRowOneLevelMenu
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x + \
					   g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + \
					   g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testParameter.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_testParameter.oneLevelMenu[index].lenth - \
						   2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_testParameter.oneLevelMenu[index].width - \
						   2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_testParameter.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneRowOneLevelMenuCue
 * Description    : 显示一级菜单提示内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneRowOneLevelMenuCue( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x + \
					   g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + \
					   g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_PUTIN_FONT_POINT; 
	const uint16_t backColor = COLOR_PUTIN_FONT_BACK;
	const uint16_t lenth = g_testParameter.oneLevelMenu[index].lenth - \
						   2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_testParameter.oneLevelMenu[index].width - \
						   2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	GUI_DispStr16At(x,y+4,pointColor,backColor,PUTIN_RECANGLE_CUE);
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		Show_TestParameterOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneRowOneLevelMenuUnit
 * Description    : 显示一行试验参数单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneRowOneLevelMenuUnit( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x + g_testParameter.oneLevelMenu[index].lenth - \
					   g_testParameter.oneLevelMenu[index].lineWidth - 5 * 12;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_testParameter.oneLevelMenu[index].backColor;
	
	if ( strcmp(g_testParameter.pParameterUnitArray[index],"NULL") )
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_testParameter.pParameterUnitArray[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneLevelMenuUnit
 * Description    : 显示一级菜单单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneLevelMenuUnit( void )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		Show_TestParameterOneRowOneLevelMenuUnit(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneRowOneLevelMenuArrow
 * Description    : 显示一行试验参数箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_TestParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x + g_testParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = COLOR_SELECT_BACK;//g_testParameter.oneLevelMenu[index].pointColor;
	const uint16_t backColor = COLOR_BACK;
	
	GUI_DispStr24At(x,y,pointColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Clear_TestParameterOneRowOneLevelMenuArrow
 * Description    : 清除一行试验参数箭头
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Clear_TestParameterOneRowOneLevelMenuArrow( uint8_t index )
{
	const uint16_t x = g_testParameter.oneLevelMenu[index].x + g_testParameter.oneLevelMenu[index].lenth + 12;
	const uint16_t y = g_testParameter.oneLevelMenu[index].y + g_testParameter.oneLevelMenu[index].lineWidth + 1;
	const uint16_t backColor = g_testParameter.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,backColor,backColor,">");
}

/*------------------------------------------------------------
 * Function Name  : Traverse_TestParameter
 * Description    : 遍历试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_TestParameter( void )
{
	Show_TestParameterOneLevelMenuContent();
	
	Show_TestParameterOneLevelMenuUnit();
}

/*------------------------------------------------------------
 * Function Name  : TestParameterIndexCrossTheBorderProcess
 * Description    : 试验参数索引值越界处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterIndexCrossTheBorderProcess( void )
{
	uint8_t i;
	
	for (i=0; i<g_testParameter.curParameterNum; ++i)
	{
		if (g_testParameter.twoLevelMenu[i].index >= g_testParameter.twoLevelMenu[i].parameterCnt)
		{
			g_testParameter.twoLevelMenu[i].index = 0;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterMoveIndexProcess
 * Description    : 试验参数移动索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_testParameter.isIndexMove = NO;
	
	indexObj.enableMoveIndex = g_testParameter.enableMoveIndex;
	indexObj.rowNum = g_testParameter.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_testParameter.curParameterNum;
	indexObj.pNowIndex = &g_testParameter.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_testParameter.nowIndex != g_testParameter.recordIndex)
	{		
		g_testParameter.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterRestoreShowOneMenu
 * Description    : 还原显示一个菜单项
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterRestoreShowOneMenu( uint8_t index )
{
	g_testParameter.oneLevelMenu[index].pointColor = g_testParameter.oneLevelMenu[index].recordPointColor;
	g_testParameter.oneLevelMenu[index].backColor = g_testParameter.oneLevelMenu[index].recordBackColor;
	
	Show_TestParameterOneRowOneLevelMenuContent(index);
	Show_TestParameterOneRowOneLevelMenuUnit(index);		
	Clear_TestParameterOneRowOneLevelMenuArrow(index);
}

/*------------------------------------------------------------
 * Function Name  : TestParameterShowOneMenuWithCursor
 * Description    : 显示一个带光标的菜单项
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterShowOneMenuWithCursor( uint8_t index )
{
	g_testParameter.oneLevelMenu[index].pointColor = COLOR_SELECT_POINT;
	g_testParameter.oneLevelMenu[index].backColor = COLOR_SELECT_BACK;
	
	Show_TestParameterOneRowOneLevelMenuContent(index);
	Show_TestParameterOneRowOneLevelMenuUnit(index);	
	if (g_testParameter.enableArrow == ENABLE)
	{
		Show_TestParameterOneRowOneLevelMenuArrow(index);
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterMoveCursorProcess( void )
{
	if (g_testParameter.isIndexMove == YES)
	{		
		if (g_testParameter.recordIndex != 0xff)
		{	
			TestParameterRestoreShowOneMenu(g_testParameter.recordIndex);
			
			TestParameterShowOneMenuWithCursor(g_testParameter.nowIndex);
		}
		else
		{
			TestParameterShowOneMenuWithCursor(g_testParameter.nowIndex);
		}
		
		g_testParameter.recordIndex = g_testParameter.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterStatusProcess
 * Description    : 状态处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterStatusProcess( void )
{
	uint8_t index = g_testParameter.nowIndex;
	
	g_testParameter.nowIndex %= g_testParameter.curParameterNum;
	
	switch ( g_testParameter.twoLevelMenu[index].parameterType )
	{
		case IMMEDIATELY_PUTIN_NONE:
		case IMMEDIATELY_PUTIN_SHIFT:
			g_testParameter.enablePutin = ENABLE;
			break;
		
		default:
			g_testParameter.enablePutin = DISABLE;
			break;
	}
	
	if (g_testParameter.oneLevelMenu[index].saveType == TYPE_CHAR)
	{
		g_testParameter.enableShift = ENABLE_SHIFT;
	}
	else
	{
		g_testParameter.enableShift = DISABLE_SHIFT;
	}
	
	switch ( g_testParameter.twoLevelMenu[index].parameterType )
	{
		case NONE_USE_USER_DEFINE:
		case USE_USER_DEFINE:
			g_testParameter.enableArrow = ENABLE;
			break;
		
		default:
			g_testParameter.enableArrow = DISABLE;
			break;
	}
	
}

/*------------------------------------------------------------
 * Function Name  : TestParameterPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_testParameter.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_testParameter.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = g_testParameter.enablePutin;
	pPutin->NewShift = g_testParameter.enableShift;
	pPutin->x = g_testParameter.oneLevelMenu[index].x + g_testParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_testParameter.oneLevelMenu[index].y + g_testParameter.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_TEST_PARAMETER_PUTIN_BIT;
	pPutin->FillBit = MAX_TEST_PARAMETER_PUTIN_BIT;
	pPutin->putinFrameLenth = g_testParameter.oneLevelMenu[index].lenth - 2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_testParameter.oneLevelMenu[index].width - 2 * g_testParameter.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_testParameter.putinNum;
	pPutin->SaveType = g_testParameter.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_testParameter.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : TestParameterPullData
 * Description    : 压入数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterPullData( void )
{
	uint8_t index = g_testParameter.nowIndex;
	
	switch ( g_testParameter.oneLevelMenu[index].saveType )
	{
		case TYPE_INT:
			numtochar(g_testParameter.putinNum,*GetPutinIntDataAddr(),g_testParameter.parameterData[index]);
			break;
		case TYPE_FLOAT:
			floattochar(g_testParameter.putinNum,g_testParameter.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_testParameter.parameterData[index]);
			break;
		case TYPE_CHAR:
			if ( strcmp(GetPutinCharDataAddr(),"") )
			{
				strcpy(g_testParameter.parameterData[index],GetPutinCharDataAddr());
			}
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterCheckPutinData
 * Description    : 检测进输入数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus TestParameterCheckPutinData( uint8_t nowIndex )
{
	uint8_t checkIndex = 0;
	
	checkIndex = GetTestParameterIndex(OBJECT_SPECIMEN_NUMS);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			int32_t putinNum = *GetPutinIntDataAddr();
			
			if ((putinNum==0) || (putinNum>MAX_SPECIMEN_NUMS))
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[0]);
				
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_ROUND_DIAMETER);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[3]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_EXTERNSOMETER_GAUGE);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[4]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_PARALLEL_LENTH);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[5]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_ORIGINAL_GAUGE);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[6]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_SPECIMEN_LENTH);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[7]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_SPECIMEN_WIDTH);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[8]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_CORRECTION_FACTOR);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[9]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_AREA);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[10]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_SPECIMEN_HIGH);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[11]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_SPECIMEN_SPAN);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[12]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_PIPE_THICKNESS);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[13]);
			
				return FAILED;
			}
		}
	}
	
	checkIndex = GetTestParameterIndex(OBJECT_PIPE_OUTER_DIAMETER);
	if (checkIndex != 0xff)
	{
		if (nowIndex == checkIndex)
		{
			float tempf = *GetPutinFloatDataAddr();
				
			if (fabs(tempf) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSampleNumRangeCue[14]);
			
				return FAILED;
			}
		}
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterTwoLevelMenuKeyProcess
 * Description    : 二级菜单按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterTwoLevelMenuKeyProcess( void )
{
	STATUS_PUTIN_TypeDef status;
	uint8_t index = g_testParameter.nowIndex;
	
	g_testParameter.enablePutin = ENABLE;
	
	Show_TestParameterOneRowOneLevelMenuCue(index);
		
	while (1)
	{
		TestParameterPutinProcess();
		
		status = GetPutinStatus();
		
		switch ( status )
		{
			case STATUS_EDIT_COMP:				
				if (TestParameterCheckPutinData(index) == FAILED)
				{
					g_testParameter.leavePage.flagLeavePage = SET;
					g_testParameter.leavePage.flagSaveData = RESET;
					
					return;
				}
				
				TestParameterPullData();
				return;
				
			case STATUS_CANCEL_PUTIN:
				return;
			
			case STATUS_DISABLE_PUTIN:
				if (IsPressKey() == YES)
				{
					switch ( GetKeyVal() )
					{
						case KEY_ENTER:
						case KEY_ESC:
							return;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterIndexUpdate
 * Description    : 索引值更新
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterIndexUpdate( void )
{
	g_testParameter.nowIndex++;
	g_testParameter.nowIndex %= g_testParameter.curParameterNum;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterUpdateStatus( void )
{
	TestParameterIndexUpdate();
	
	g_testParameter.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterShortcutCycleTask
 * Description    : 快捷菜单任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_testParameter.refreshShortcut == ENABLE)
	{
		g_testParameter.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F1 | SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[0] = pTwoLevelMenu[0];
		pShortCut->pContent[2] = pTwoLevelMenu[37];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : SerialAddOneCycle
 * Description    : 加1循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SerialAddOneCycle( char *p, uint8_t *pCur_bit )
{
	if ( ((p[*pCur_bit]>='0') && (p[*pCur_bit]<'9')) || ((p[*pCur_bit]>='A') && (p[*pCur_bit]<'Z')) || ((p[*pCur_bit]>='a') && (p[*pCur_bit]<'z')) )
	{
		p[*pCur_bit]++;	
	}
	else if (p[*pCur_bit] == '9')
	{
		p[*pCur_bit] = '0';	
		(*pCur_bit)++;
	}
	else if (p[*pCur_bit] == 'Z')
	{
		p[*pCur_bit] = 'A';	
		(*pCur_bit)++;
	}
	else if (p[*pCur_bit] == 'z')
	{
		p[*pCur_bit] = 'a';	
		(*pCur_bit)++;
	}
	else		//遇到类似'-'跳过该字符
	{
		(*pCur_bit)++;			
	}	
}

/*------------------------------------------------------------
 * Function Name  : TestParameterSerialAddOne
 * Description    : 试验参数编号加1循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static SERIAL_ADD_ONE_STA TestParameterSerialAddOne( TEST_TYPE_TypeDef type, char *pSerial, uint8_t *pCnt )
{
	uint8_t cur_bit = 0, last_bit = 0,bit_num = 1;
	char buff[MAX_TEST_PARAMETER_PUTIN_BIT+1] = {0};	  //暂存待处理的编号
	const char EXTEND_CHARACTER = '0';	//扩展一位的填充字符

	strcpy(buff,pSerial);
	
	while ( FR_OK == report_exist(type,buff) )	//编号存在同名
	{ 	
		cur_bit = 0;
		last_bit = 0;
		
		StrCutBackReplace(buff);	//交换原有数据次序			
		
		while (last_bit <= cur_bit)
		{
			SerialAddOneCycle(buff,&cur_bit);				
			if (buff[cur_bit] == '\0')	 
			{
				buff[cur_bit] = EXTEND_CHARACTER;	
			}
			last_bit++;		 
		}
				
		StrCutBackReplace(buff);	//还原原有数据次序
	}
	
	bit_num = strlen(buff);
	
	if (bit_num > MAX_TEST_PARAMETER_PUTIN_BIT)		//越界			
	{
		return SERIAL_REACH_MAX_VALUE;
	}

	strcpy(pSerial,buff);

	*pCnt = bit_num;	
	
	return CHANGE_SERIAL_OK;		
}

/*------------------------------------------------------------
 * Function Name  : TestParameterCheckSerialRepeat
 * Description    : 编号重复性检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SERIAL_ADD_ONE_STA TestParameterCheckSerialRepeat( TEST_TYPE_TypeDef type, char *pStr, uint8_t *pNum )
{
	SERIAL_ADD_ONE_STA status;
	REPORT_TypeDef report;
	
	if(SET == FindStrIncludeIllegalChar(pStr,'*') ) 			//文件系统不支持该字符	
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pSerialPutinCue[0]);
		
		return SERIAL_ILLEGAL;
	}
		
	if ( FR_OK == report_exist(type,pStr) )
	{
		report_read(type,pStr,&report);
		
		status = TestParameterSerialAddOne(type,pStr,pNum);
		
		switch ( status )
		{
			case CHANGE_SERIAL_OK:
				SetPopWindowsInfomation(POP_PCM_CUE,2,&pSerialPutinCue[1]);
			
				return CHANGE_SERIAL_OK;
			
			case SERIAL_REACH_MAX_VALUE:
				SetPopWindowsInfomation(POP_PCM_CUE,1,&pSerialPutinCue[3]);
			
				return SERIAL_REACH_MAX_VALUE;
			default:
				break;
		}
	}
	
	return SERIAL_NO_CHG;
}

/*------------------------------------------------------------
 * Function Name  : TestParameterAccordRoundDiameterGetCutAreaProcess
 * Description    : 根据圆形直径获取截面积
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterAccordRoundDiameterGetCutAreaProcess( void )
{
	float dismeter = 0;
	uint8_t index = 0;
	float area = 0;
	
	if (g_testParameter.testType == KLJSSW)
	{
		index = GetTestParameterIndex(OBJECT_ROUND_DIAMETER);
		
		if (g_testParameter.nowIndex == index)
		{
			dismeter = str2float(g_testParameter.parameterData[index]);
			
			area = PI * (dismeter / 2) * (dismeter / 2);
			
			index = GetTestParameterIndex(OBJECT_AREA);
			floattochar(MAX_TEST_PARAMETER_PUTIN_BIT,g_testParameter.oneLevelMenu[index].pointBit,area,g_testParameter.parameterData[index]);
			
			TestParameterRestoreShowOneMenu(index);
		}
	}
}	

/*------------------------------------------------------------
 * Function Name  : TestParameterKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterKeyProcess( void )
{
	uint8_t index = g_testParameter.nowIndex;
	TWO_LEVEL_MENU_TypeDef *pMenu = NULL;
	STATUS_PUTIN_TypeDef putinStatus;
	SERIAL_ADD_ONE_STA serialStatus = SERIAL_NO_CHG;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_RIGHT:
				if ( (g_testParameter.twoLevelMenu[index].parameterType == NONE_USE_USER_DEFINE) || \
				     (g_testParameter.twoLevelMenu[index].parameterType == USE_USER_DEFINE) )
				{
					/* 处理快捷菜单 */
					GUI_ClearShortcutMenu();
					
					/* 还原一级菜单 */
					g_testParameter.oneLevelMenu[g_testParameter.recordIndex].pointColor = \
						g_testParameter.oneLevelMenu[g_testParameter.recordIndex].recordPointColor;
					g_testParameter.oneLevelMenu[g_testParameter.recordIndex].backColor = \
						g_testParameter.oneLevelMenu[g_testParameter.recordIndex].recordBackColor;
					
					Show_TestParameterOneRowOneLevelMenuContent(g_testParameter.recordIndex);
					Show_TestParameterOneRowOneLevelMenuUnit(g_testParameter.recordIndex);	
					
					/* 配置二级菜单 */
					pMenu = GetTwoLevelMenuAddr();
					
					pMenu->x = g_testParameter.twoLevelMenu[index].x;
					pMenu->y = g_testParameter.twoLevelMenu[index].y;
					pMenu->nowIndex = g_testParameter.twoLevelMenu[index].index;
					pMenu->maxUpY = g_testParameter.twoLevelMenu[index].maxUpY;
					pMenu->maxDownY = g_testParameter.twoLevelMenu[index].maxDownY;
					pMenu->rowSpacing = g_testParameter.twoLevelMenu[index].rowDistance;
					pMenu->lineWidth = g_testParameter.twoLevelMenu[index].lineWidth;
					pMenu->lenth = g_testParameter.twoLevelMenu[index].lenth;
					pMenu->width = g_testParameter.twoLevelMenu[index].width;
					pMenu->pointColor = g_testParameter.twoLevelMenu[index].pointColor;
					pMenu->backColor = g_testParameter.twoLevelMenu[index].backColor;
					pMenu->recordBackColor = g_testParameter.twoLevelMenu[index].backColor;
					pMenu->lineColor = g_testParameter.twoLevelMenu[index].pointColor;
					pMenu->nums = g_testParameter.twoLevelMenu[index].parameterCnt;
					pMenu->pParameterNameArray = g_testParameter.twoLevelMenu[index].pParameterNameArray;
					pMenu->fontSize = g_testParameter.twoLevelMenu[index].fontSize;
					
					LoadTwoLevelMenuPage(pMenu);
					
					if (pMenu->isSelect == YES)
					{												
						g_testParameter.twoLevelMenu[index].index = pMenu->nowIndex;
						
						if ((g_testParameter.twoLevelMenu[index].parameterType==USE_USER_DEFINE) && (pMenu->nowIndex==0))	//第一项必须默认为自定义选项
						{													
							TestParameterTwoLevelMenuKeyProcess();
						}
						else
						{
							strcpy(g_testParameter.parameterData[index],pMenu->pParameterNameArray[pMenu->nowIndex]);
						}
						
						TestParameterAccordRoundDiameterGetCutAreaProcess();	

						TestParameterExchangeSampleShape(index);
					}
					
					TestParameterUpdateStatus();				
				}
				break;
				
			case KEY_ENTER:
				if ( (g_testParameter.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_NONE) || \
				     (g_testParameter.twoLevelMenu[index].parameterType == IMMEDIATELY_PUTIN_SHIFT) )
				{				
					putinStatus = GetPutinStatus();
					
					switch ( putinStatus )
					{
						case STATUS_DISABLE_PUTIN:
							return;
						
						case STATUS_EDIT_COMP:
							if (GetTestParameterIndex(OBJECT_SPECIMEN_SERIAL) == index)
							{
								serialStatus = TestParameterCheckSerialRepeat(g_testParameter.testType,GetPutinCharDataAddr(),&g_testParameter.putinNum);
								switch ( serialStatus )
								{
									case SERIAL_NO_CHG:
										break;
									case CHANGE_SERIAL_OK:
										g_testParameter.leavePage.flagLeavePage = SET;
										g_testParameter.leavePage.flagSaveData = SET;
										break;
									case SERIAL_ILLEGAL:
									case SERIAL_REACH_MAX_VALUE:
										g_testParameter.leavePage.flagLeavePage = SET;
										g_testParameter.leavePage.flagSaveData = RESET;
										break;
								}
							}
							
							if (TestParameterCheckPutinData(index) == FAILED)
							{
								g_testParameter.leavePage.flagLeavePage = SET;
								g_testParameter.leavePage.flagSaveData = RESET;
								
								return;
							}
							
							TestParameterPullData();		
							break;						
						
						default:
							break;
					}	

					TestParameterUpdateStatus();
				}
				break;
			
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						SetPage(MAIN_PAGE);
						g_testParameter.leavePage.flagLeavePage = SET;
						g_testParameter.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						TestParameterUpdateStatus();
						break;
					
					default:						
						break;
				}
				break;
				
			case KEY_F1:
				SetTestTypeSelectSourcePage(TEST_PARAMETER_PAGE);
				SetTestTypeSelectTargetPage(TEST_SELECT);
				SetPage(TEST_TYPE_SELECT_PAGE);
				
				g_testParameter.leavePage.flagLeavePage = SET;
				g_testParameter.leavePage.flagSaveData = RESET;	
				break;
				
			case KEY_F3:
				SetPage(MAIN_PAGE);
				g_testParameter.leavePage.flagLeavePage = SET;
				g_testParameter.leavePage.flagSaveData = SET;
				break;
			
			case KEY_F4:
				SetPage(MAIN_PAGE);
				g_testParameter.leavePage.flagLeavePage = SET;
				g_testParameter.leavePage.flagSaveData = RESET;
				break;
		}
	}
}


/*------------------------------------------------------------
 * Function Name  : TestParameterPopWindowsProcess
 * Description    : 弹窗处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterPopWindowsProcess( void )
{	
	PopWindowsProcessCycle();	
}

/*------------------------------------------------------------
 * Function Name  : TestParameterLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterLeavePageCheckCycle( void )
{
	if (g_testParameter.leavePage.flagLeavePage == SET)
	{
		if (g_testParameter.leavePage.flagSaveData == SET)
		{
			TestParameterWriteParameter();
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MainPageTestInfomation
 * Description    : 试验信息GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MainPageTestInfomation( void )
{	
	ConfigTestParameterRectangleFrameCoordinate(520,150,0);
	
	GUI_TestParameterRectangleFrame();
	
	GUI_TestParameterOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : LoadMainPageGetTestInfomationPage
 * Description    : 加载获取试验信息
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadMainPageGetTestInfomationPage( void )
{
//	uint8_t indexTarget,indexSource;
	
	/* 参数初始化 */
	TestParameterInit();
	
	/* 参数配置 */
	TestParameterConfig();
	
	/* 获取参数 */
	TestParameterReadParameter();
	
	/* 索引值溢出处理 */
	TestParameterIndexCrossTheBorderProcess();
	
	/* 处理一级菜单内容 */
	TestParameterOneLevelMenuContentProcess();
	
	/* 画GUI框架 */
	GUI_MainPageTestInfomation();
	
	/* 遍历 */
	Traverse_TestParameter();
	
//	for (indexTarget=0; indexTarget<pTestInfo->testInfomationNum; ++indexTarget)
//	{
//		for (indexSource=0; indexSource<g_testParameter.curParameterNum; ++indexSource)
//		{
//			if (pTestInfo->testHandle[indexTarget] == g_testParameter.indexArray[indexSource])
//			{
//				strcpy(pTestInfo->pParameterContent[indexTarget],g_testParameter.parameterData[indexSource]);
//				
//				continue;
//			}
//		}
//	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterExchangeSampleShape
 * Description    : 切换试件形状
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void TestParameterExchangeSampleShape( uint8_t nowIndex )
{
	uint8_t index = 0;
	
	switch ( g_testParameter.testType )
	{
		case KYTY:
		case KLJSSW:	
			break;
		default:
			return;
	}
	
	index = GetTestParameterIndex(OBJECT_SHAPE);
	if (index != 0xff)
	{
		if (index != nowIndex)
		{
			return;
		}
	}
	else
	{
		return;
	}
	
	index = GetTestParameterIndex(OBJECT_SHAPE);
	if (index != 0xff)
	{
		pTest->sample_shape_index = g_testParameter.twoLevelMenu[index].index;
	}
	
	g_testParameter.leavePage.flagLeavePage = SET;
	g_testParameter.leavePage.flagSaveData = RESET;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
