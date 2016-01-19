/**
  ******************************************************************************
  * @file    TestParameter.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-10-25
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ��������ļ�
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

#define MAX_TEST_PARAMETER_NUM				15			//���֧�������������
#define MAX_TEST_PARAMETER_PUTIN_BIT		16			//֧���������λ��
#define MAX_SPECIMEN_NUMS					20			//֧������Կ����

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	OBJECT_TEST_TYPE = 0,		/* �������� */
	OBJECT_SPECIMEN_SERIAL,		/* �Լ���� */
	OBJECT_SPECIMEN_VARIETY,	/* �Լ�Ʒ�� */
	OBJECT_STRENGTH_GRADE,		/* ǿ�ȵȼ� */
	OBJECT_SPECIMEN_AGE,		/* �Լ����� */
	OBJECT_SPECIMEN_FORMAT,		/* �Լ���� */
	OBJECT_SPECIMEN_LENTH,		/* �Լ����� */
	OBJECT_SPECIMEN_WIDTH,		/* �Լ���� */
	OBJECT_SPECIMEN_HIGH,		/* �Լ��߶� */
	OBJECT_SPECIMEN_SPAN,		/* �Լ���� */
	OBJECT_CORRECTION_FACTOR,	/* ����ϵ�� */
	OBJECT_LOAD_SPEED,			/* �����ٶ� */
	OBJECT_SPECIMEN_NUMS,		/* �Լ����� */
	OBJECT_LOAD_MODE,			/* ���ط�ʽ */
	OBJECT_DELAY_TIMING,		/* ��ʱʱ�� */
	OBJECT_AREA,				/* ��� 	*/
	OBJECT_SHAPE,				/* ��״ 	*/
	OBJECT_ROUND_DIAMETER,		/* Բ��ֱ�� 	*/
	OBJECT_EXTERNSOMETER_GAUGE,	/* ����Ʊ�� */
	OBJECT_PARALLEL_LENTH,		/* ƽ�г��� */
	OBJECT_ORIGINAL_GAUGE,		/* ԭʼ��� */
	OBJECT_PIPE_THICKNESS,		/* �ܶκ�� */
	OBJECT_PIPE_OUTER_DIAMETER,	/* �ܶ���ֱ�� */
}ALL_PATAMETER_NAME_TypeDef;

typedef enum				//ˮ�ེɰ����
{
	TEST_SERIAL_KZSNJS = 0,	//������
	TEST_TYPE_KZSNJS,		//����Ʒ��
	STRANG_GRADE_KZSNJS,	//ǿ�ȵȼ�
	SAMPLE_AGE_KZSNJS,		//�Լ�����
	ZFX_LEN_KZSNJS,			//�߳�
	ZFX_SPAN_KZSNJS,		//���
	SAMPLE_NUM_KZSNJS,		//�Լ�����
}KZSNJS_PARA_NAME_TypeDef;	

typedef enum				 //ѹ��ˮ�ཬ����
{
	TEST_SERIAL_YJSNJ = 0,	//������
	TEST_TYPE_YJSNJ,		//����Ʒ��
	STRANG_GRADE_YJSNJ,		//ǿ�ȵȼ�
	SAMPLE_AGE_YJSNJ,		//�Լ�����
	ZFX_LEN_YJSNJ,			//�߳�
	ZFX_SPAN_YJSNJ,			//���
	SAMPLE_NUM_YJSNJ,		//�Լ�����
}KZYJSNJ_PARA_NAME_TypeDef; 

typedef enum				 //ͨ�ÿ���
{
	TEST_SERIAL_KZTY = 0,	//������
	STRANG_GRADE_KZTY,		//ǿ�ȵȼ�
	SAMPLE_AGE_KZTY,		//�Լ�����
	SAMPLE_WIDTH_KZTY,		//�Լ����
	SAMPLE_HIGH_KZTY,		//�Լ��߶�
	ZFX_SPAN_KZTY,			//���
	SAMPLE_NUM_KZTY,		//�Լ�����
}KZTY_PARA_NAME_TypeDef; 

typedef enum				 //ˮ�ེɰ��ѹ
{
	TEST_SERIAL_KYSNJS = 0,	//������
	TEST_TYPE_KYSNJS,		//����Ʒ��
	STRANG_GRADE_KYSNJS,	//ǿ�ȵȼ�
	SAMPLE_AGE_KYSNJS,		//�Լ�����
	SPEC_KYSNJS,			//�Լ����
	SAMPLE_NUM_KYSNJS,		//�Լ�����
}KYSNJS_PARA_NAME_TypeDef;

typedef enum				 //����ɰ����ѹ
{
	TEST_SERIAL_KYJZSJ = 0,	//������
	TEST_TYPE_KYJZSJ,		//����Ʒ��
	SAMPLE_AGE_KYJZSJ,		//�Լ�����
	SPEC_KYJZSJ,			//�Լ����
	CORRECT_COF_KYJZSJ,		//����ϵ��
	SAMPLE_NUM_KYJZSJ,		//�Լ�����
}KYJZSJ_PARA_NAME_TypeDef;

typedef enum				//��ͨ����������
{
	TEST_SERIAL_KZHNT = 0,	//������
	STRANG_GRADE_KZHNT,		//ǿ�ȵȼ�
	SAMPLE_AGE_KZHNT,		//�Լ�����
	SPEC_KZHNT,				//�Լ����
	CORRECT_COF_KZHNT,		//����ϵ��
	SAMPLE_NUM_KZHNT,		//�Լ�����		
}KZHNT_PARA_NAME_TypeDef;

typedef enum				//��ͨ��������ѹ
{
	TEST_SERIAL_KYHNT = 0,	//������
	STRANG_GRADE_KYHNT,		//ǿ�ȵȼ�
	SAMPLE_AGE_KYHNT,		//�Լ�����
	SPEC_KYHNT,				//�Լ����
	CORRECT_COF_KYHNT,		//����ϵ��
	SAMPLE_NUM_KYHNT,		//�Լ�����
}KYHNT_PARA_NAME_TypeDef;

typedef enum				 //��ǽש��ѹ
{
	TEST_SERIAL_KYQQZ = 0,	//������
	TEST_TYPE_KYQQZ,		//�Լ�Ʒ��
	LENGTH_KYQQZ,			//����
	WIDTH_KYQQZ,			//���
	HIGH_KYQQZ,				//�߶�
	SAMPLE_NUM_KYQQZ,		//�Լ�����
}KYQQZ_PARA_NAME_TypeDef;

union KYTY_PARA_NAME_TypeDef		//ͨ�ÿ�ѹ
{
	enum 				 
	{
		TEST_SERIAL_RECTANGLE_KYTY = 0,	//������
		SAMPLE_SHAPE_RECTANGLE_KYTY,	//��״
		SAMPLE_LENTH_RECTANGLE_KYTY,	//����
		SAMPLE_WIDTH_RECTANGLE_KYTY,	//���
		CORRECT_COF_RECTANGLE_KYTY,		//����ϵ��
		SAMPLE_NUM_RECTANGLE_KYTY,		//�Լ�����
	}KYTY_SHAPE_RECTANGLE;
	enum				 
	{
		TEST_SERIAL_ROUND_KYTY = 0,	//������
		SAMPLE_SHAPE_ROUND_KYTY,	//��״
		SAMPLE_DIAMETER_ROUND_KYTY,	//ֱ��
		CORRECT_COF_ROUND_KYTY,		//����ϵ��
		SAMPLE_NUM_ROUND_KYTY,		//�Լ�����
	}KYTY_SHAPE_ROUND;
	enum				 
	{
		TEST_SERIAL_IRREGULAR_KYTY = 0,	//������
		SAMPLE_SHAPE_IRREGULAR_KYTY,	//��״
		SAMPLE_AREA_IRREGULAR_KYTY,		//���
		CORRECT_COF_IRREGULAR_KYTY,		//����ϵ��
		SAMPLE_NUM_IRREGULAR_KYTY,		//�Լ�����
	}KYTY_SHAPE_RIRREGULAR;
};

union KLJSSW_PARA_NAME_TypeDef		//������������
{
	enum 				 
	{
		TEST_SERIAL_RECTANGLE_KLJSSW = 0,			//������
		SAMPLE_SHAPE_RECTANGLE_KLJSSW,				//�Լ���״
		SAMPLE_THICKNESS_RECTANGLE_KLJSSW,			//���κ��
		SAMPLE_WIDTH_RECTANGLE_KLJSSW,				//���ο��
		ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW,		//ԭʼ�����
		EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW,		//����Ʊ��
		ORIGINAL_GAUGE_RECTANGLE_KLJSSW,			//ԭʼ���
		PARALLEL_LENTH_RECTANGLE_KLJSSW,			//ƽ�г���
		SAMPLE_NUM_RECTANGLE_KLJSSW,				//�Լ�����
	}KYTY_SHAPE_RECTANGLE;
	enum				 
	{
		TEST_SERIAL_ROUND_KLJSSW = 0,			//������
		SAMPLE_SHAPE_ROUND_KLJSSW,				//�Լ���״
		SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW,		//Բ��ֱ��
		ORIGINAL_CROSS_AREA_ROUND_KLJSSW,		//ԭʼ�����
		EXTERNSOMETER_GAUGE_ROUND_KLJSSW,		//����Ʊ��
		ORIGINAL_GAUGE_ROUND_KLJSSW,			//ԭʼ���
		PARALLEL_LENTH_ROUND_KLJSSW,			//ƽ�г���
		SAMPLE_NUM_ROUND_KLJSSW,				//�Լ�����
	}KLJSSW_SHAPE_ROUND;
	enum				 
	{
		TEST_SERIAL_TUBE_KLJSSW = 0,			//������
		SAMPLE_SHAPE_TUBE_KLJSSW,				//�Լ���״
		SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW,		//�ܶκ��
		SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW,	//�ܶ��⾶
		ORIGINAL_CROSS_AREA_TUBE_KLJSSW,		//ԭʼ�����
		EXTERNSOMETER_GAUGE_TUBE_KLJSSW,		//����Ʊ��
		ORIGINAL_GAUGE_TUBE_KLJSSW,				//ԭʼ���
		PARALLEL_LENTH_TUBE_KLJSSW,				//ƽ�г���
		SAMPLE_NUM_TUBE_KLJSSW,					//�Լ�����
	}KYTY_SHAPE_TUBE;
	enum				 
	{
		TEST_SERIAL_RIRREGULAR_KLJSSW = 0,				//������
		SAMPLE_SHAPE_RIRREGULAR_KLJSSW,					//�Լ���״
		ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW,			//ԭʼ�����
		EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW,			//����Ʊ��
		ORIGINAL_GAUGE_RIRREGULAR_KLJSSW,				//ԭʼ���
		PARALLEL_LENTH_RIRREGULAR_KLJSSW,				//ƽ�г���
		SAMPLE_NUM_RIRREGULAR_KLJSSW,					//�Լ�����
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
	uint8_t curParameterNum;						//��������
	SMPL_NAME_TypeDef showChannel;					//��ʾͨ��
	SMPL_NAME_TypeDef tureChannel;					//��ʵͨ��
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	FunctionalState enableMoveIndex;				//ʹ���ƶ�����ֵ
	FunctionalState enablePutin;					//ʹ������
	FunctionalState enableArrow;					//ʹ�ܼ�ͷ
	STATUS_SHIFT_TypeDef enableShift;				//ʹ���л����뷨
	uint8_t putinNum;								//�����ַ�����
	const char * pTitle;							//����
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
}TEST_PARAMETER_TypeDef;

/* Private constants ---------------------------------------------------------*/
/* �������� */
const char * const pTestParameterTitleName[] = 
{
	"�������� | ˮ�ེɰ��ѹ",		//0
	"�������� | ����ɰ����ѹ",		//1
	"�������� | ��������ѹ",			//2
	"�������� | ����������",			//3
	"�������� | ��ǽש��ѹ",			//4
	"�������� | �𼶶���",			//5
	"�������� | ͨ�ÿ�ѹ",			//6
	"�������� | ˮ�ེɰ����",		//7
	"�������� | ѹ��ˮ�ཬ����",		//8
	"�������� | ͨ�ÿ���",			//9	
	"�������� | ������������",		//10
};

/* �����׼ */
const char * const pTestStandard[] = 
{
	"GB/T 17671-1999",	  	//ˮ�ེɰ��ѹ/ˮ�ེɰ����/ѹ��ˮ�ཬ����
	"GB/T 50081-2002",	  	//��ͨ��������ѹ/��ͨ����������
	"JGJ/T 70-2009",	  	//����ɰ����ѹ
	"GB/T 2542-2012",	  	//��ǽש��ѹ
	"GB/T 228-2002",		//������������
};

/* ˮ�ེɰ��ѹ����������ʾ */
const char * const pValue_KYSNJS[] = 
{
	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ�Ʒ�֣�",
	"ǿ�ȵȼ���",
	"�Լ����ڣ�",
	"�Լ����",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};						
						
/* ˮ�ེɰ���۲���������ʾ */
const char * const pValue_KZSNJS[] = 
{
	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ�Ʒ�֣�",
	"ǿ�ȵȼ���",
	"�Լ����ڣ�",
	"�߳���",
	"��ࣺ",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};

/* ѹ��ˮ�ཬ���۲���������ʾ */
const char * const pValue_KZYJSNJ[] = 
{
	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ�Ʒ�֣�",
	"ǿ�ȵȼ���",
	"�Լ����ڣ�",
	"�߳���",
	"��ࣺ",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};	
						
/* ��ͨ��������ѹ����������ʾ */
const char * const pValue_KYHNT[] = 
{
  	"�������ͣ�",
	"�Լ���ţ�",
	"ǿ�ȵȼ���",
	"�Լ����ڣ�",
	"�Լ����",
	"����ϵ����",
	"Ӧ���ٶȣ�",
	"�غ��ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};	

/* ��ͨ���������۲���������ʾ */
const char * const pValue_KZHNT[] = 
{
   	"�������ͣ�",
	"�Լ���ţ�",
	"ǿ�ȵȼ���",
	"�Լ����ڣ�",
	"�Լ����",
	"����ϵ����",
	"Ӧ���ٶȣ�",
	"�غ��ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};

/* ��ǽש��ѹ����������ʾ */
const char * const pValue_KYQQZ[] = 
{
	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ�Ʒ�֣�",
	"���ȣ�",
	"��ȣ�",
	"�߶ȣ�",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",
	"��ʱʱ�䣺",
};

/* �𼶶��ɿ�ѹ����������ʾ */
const char * const pValue_KYZJDH[] = 
{
	"�������ͣ�",
	"�����ٶȣ�",
	"���ɼ�����",
};
						
/* ����ɰ����ѹ����������ʾ */
const char * const pValue_KYJZSJ[] = 
{
  	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ�Ʒ�֣�",
	"�Լ����ڣ�",
	"�Լ����",
	"����ϵ����",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",	
	"��ʱʱ�䣺",
};
						
/* ͨ�ÿ�ѹ����������ʾ */
const char * const pValue_KYTY[] = 
{
	"�������ͣ�",
	"�Լ���ţ�",
	"�Լ����ڣ�",
	"�Լ���״��",
	"�Լ������",
	"�����ٶȣ�",
	"�Լ�������",
	"���ط�ʽ��",
	"��ʱʱ�䣺",
	"����ϵ����",
	"���ȣ�",
	"��ȣ�",
	"Բ��ֱ����",
};
						
/* ͨ�ÿ��۲���������ʾ */
const char * const pValue_KZTY[] = 
{
  "�������ͣ�",
  "�Լ���ţ�",
  "ǿ�ȵȼ���",
  "�Լ����ڣ�",
  "�Լ���ȣ�",
  "�Լ��߶ȣ�",
  "�Լ���ࣺ",
  "�����ٶȣ�",
  "�Լ�������",
  "���ط�ʽ��",
  "��ʱʱ�䣺",
};						
																																							
/* �������¿�������������ʾ */
const char * const pValue_KLJSSW[] = 
{
	"�������ͣ�",		//0
	"�Լ���ţ�",		//1
	"Բ��ֱ����",		//2
	"ԭʼ�������",		//3
	"����Ʊ�ࣺ",		//4
	"�Լ�������",		//5
	"ƽ�г��ȣ�",		//6
	"ԭʼ��ࣺ",		//7
	"���κ�ȣ�",		//8
	"���ο�ȣ�",		//9
	"�ܶκ�ȣ�",		//10
	"�ܶ��⾶��",		//11
	"�Լ���״��",		//12
};


/* ˮ�ེɰ���۲���-----------------------------------------------------------*/
//����Ʒ��
const char * const pSample_type[] = 
{					
	"�Զ���",
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
			
//ǿ�ȵȼ�
const char * const pStrength_grade[] = 
{
	"�Զ���",
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

//�Լ�����	
const char * const pSample_age[] = 
{
	"�Զ���",
	"1",
	"2",
	"3",
	"7",
	"28",
};												

//�߳�
const char * const pZfx_length[] = 
{
	"�Զ���",
	"30",
	"40",
	"50",
	"60",
};										
					
//���					
const char * const pZfx_span[] = 
{
	"�Զ���",
	"80",
	"100",
	"120",
	"140",
	"160",
};									
		
//�����ٶ�						
const char * const pSpeed_load[] = 
{
	"�Զ���",
	"50",
	"100",
};									
					
//�Լ�����						
const char * const pSample_num[] = 
{
	"�Զ���",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
};									
					
//�Ƿ���������						
const char * const pIs_continue_load[] = 
{
	"��������",
	"��������",
};									
					
//��ʱʱ��					
const char * const pDelay_time[] = 
{
	"�Զ���",
	"8",
	"12",
	"16",
	"20",
	"24",
	"28",
	"32",
};


/* ѹ��ˮ�ཬ���۲���---------------------------------------------------------*/
//�Լ�Ʒ��ͬ -> pSample_type
//ǿ�ȵȼ�
const char * const pStrength_grade_KZYJSNJ[] = 
{
	"�Զ���",
	"52.5",
	"52.5R",
	"62.5",
	"62.5R",
};

//�Լ�����ͬ -> pSample_age
//�Լ��߳�ͬ -> pZfx_length
//�Լ����ͬ -> pZfx_span
//�����ٶ�ͬ -> pSpeed_load
//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time

	
/* ˮ�ེɰ��ѹ����-----------------------------------------------------------*/
//�Լ�Ʒ��ͬ -> pSample_type
//�Լ��ȼ�ͬ -> pStrength_grade
//�Լ�����ͬ -> pSample_age

//�Լ����
const char * const pSpecimen_spec_KYSNJS[] = 
{
	"�Զ���",
	"40*40",
	"50*50",
};

//�����ٶ�(kN)
const char * const pSpeed_load_KYSNJS[] = 
{
	"�Զ���",
	"0.5",
	"0.7",
	"1.0",
	"1.1",
	"1.3",
	"1.5",
	"2.4",
	"5.0",
};

//�����ٶ�(N)
const char * const pSpeed_N_load_KYSNJS[] = 
{
	"�Զ���",
	"500",
	"700",
	"1000",
	"1100",
	"1300",
	"1500",
	"2400",
	"5000",
};


//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time


/* ����ɰ����ѹ����-----------------------------------------------------------*/
//����Ʒ��
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

//�Լ�����ͬ -> pSample_age

//�Լ����
const char * const pSpecimen_spec_KYJZSJ[] = 
{
	"�Զ���",
	"70.7*70.7*70.7",
};

//����ϵ��
const char * const pCorrect_cof_KYJZSJ[] = 
{
	"1.00",
	"1.35",
};

//�����ٶ�ͬ -> pSpeed_load_KYSNJS
//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time

/* ��ͨ��������ѹ����---------------------------------------------------------*/
//ǿ�ȵȼ�ͬ -> pStrength_grade_KZHNT
//�Լ�����ͬ -> pSample_age
//�Լ����
const char * const pSpecimen_spec_KYHNT[] = 
{
	"100*100*100",
	"150*150*150",
	"200*200*200",
	"��100*200",
	"��150*300",
	"��200*400",
};

//����ϵ��
const char * const pCorrect_coef_KYHNT[] = 
{
	"0.95",
	"1",
	"1.05",
};

//Ӧ���ٶ�
const char * const pStress_speed_KYHNT[]=
{
	"�Զ���",
	"0.3",
	"0.4",
	"0.5",
	"0.6",
	"0.7",
	"0.8",
	"0.9",
	"1.0",
};

//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time


/* ��ͨ���������۲���---------------------------------------------------------*/
//ǿ�ȵȼ�
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

//�Լ�����ͬ -> pSample_age

//�Լ����
const char * const pSpecimen_spec[] = 
{
	"150*150*600",		  //�߶�+���+����
	"150*150*550",
	"100*100*400",
};

//����ϵ��
const char * const pCorrect_coef[] = 
{
	"0.85",
	"1",
};

//Ӧ���ٶ�
const char * const pStress_speed_KZHNT[]=
{
	"�Զ���",
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

//�غ��ٶȸ���Ӧ���ٶȺ����������


//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time


/* ��ǽש��ѹ����-------------------------------------------------------------*/
//�Լ�Ʒ��
const char * const pSample_type_KYQQZ[] = 
{
	"�ս���ͨש",
	"�ս���ש",
	"�ս����ש",
	"������ש",
	"��ѹ��ɰש",
	"ú�ۻ�ש",
	"¯��ש",
	"̼��ש",
};
    
//����
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

//���
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

//�߶�
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

//�����ٶ�(kN)
const char * const pSpeed_load_KYQQZ[] = 
{
	"�Զ���",
	"2.0",
	"3.0",
	"4.0",
	"5.0",
	"6.0",
};

//�����ٶ�(N)
const char * const pSpeed_N_load_KYQQZ[] = 
{
	"�Զ���",
	"2000",
	"3000",
	"4000",
	"5000",
	"6000",
};

//�Լ�����
const char * const pSample_num_KYQQZ[] = 
{
	"�Զ���",
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

//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time

/* �𼶶��ɿ�ѹ����---------------------------------------------------------------*/
//�����ٶ�ͬ -> pSpeed_load_KYSNJS
const char * const pMenu_SerialKYZJDH[] =
{
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
};



/* ͨ�ÿ�ѹ����---------------------------------------------------------------*/
//�Լ�����ͬ -> pSample_age

//�Լ���״
const char * const pSpecimen_sharp[] = 
{
	"������",
	"Բ��",
	"������",
};

//����ϵ��
const char * const pSpecimen_cof[] = 
{
	"�Զ���",
	"1.00",
};

//�����ٶ�ͬ -> pSpeed_load_KYSNJS
//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time


/* ͨ�ÿ��۲���---------------------------------------------------------------*/
//ǿ�ȵȼ�
const char * const pStrength_grade_KZTY[] = 
{
	"�Զ���",
	"50",
	"100",	
};

//�Լ�����ͬ -> pSample_age
//�Լ����ͬ -> pZfx_length
//�Լ��߶�ͬ -> pZfx_length
//�Լ����ͬ -> pZfx_span
//�����ٶ�ͬ -> pSpeed_load
//�Լ�����ͬ -> pSample_num
//�Ƿ���������ͬ -> pIs_continue_load
//��ʱʱ��ͬ -> pDelay_time

/* ���������������-------------------------------------------------------------*/
//Բ��ֱ��
const char * const pRoundDiameter_KLJSSW[] = 
{
	"�Զ���",
	"3",
	"5",
	"6",
	"8",
	"10",
	"15",
	"20",
	"25",
};

//�Լ���״
const char * const pSpecimenSharp_KLJSSW[] = 
{
	"������",
	"Բ��",
	"����",
	"������",
};

//�Լ�����ͬ -> pSample_num

/* ������Ϣ */
const char * const pSampleNumRangeCue[] = 
{
	"�Կ�������뷶Χ��(1 ~ 20)",		//0
	"��ʱʱ�����÷�Χ��(3 ~ 300)",		//1
	"���ɼ������뷶Χ��(1 ~ 6)",			//2
	"Բ��ֱ������Ϊ0��",					//3
	"����Ʊ�಻��Ϊ0��",				//4
	"ƽ�г��Ȳ���Ϊ0��",					//5
	"ԭʼ��಻��Ϊ0��",					//6
	"�Լ����Ȳ���Ϊ0��",					//7
	"�Լ���Ȳ���Ϊ0��",					//8
	"����ϵ������Ϊ0��",					//9
	"�Լ��������Ϊ0��",					//10
	"�Լ��߶Ȳ���Ϊ0��",					//11
	"�Լ���಻��Ϊ0��",					//12
	"�ܱں�Ȳ���Ϊ0��",					//13
	"�ܶ��⾶����Ϊ0��",					//14
};

const char * const pSerialPutinCue[] = 
{
	"�Լ���Ų�֧�������ַ���* ",	//0
	"����ı���Ѵ���,���Զ�����",	//1
	"�µı�š�",					//2
	"����ı���Ѵ���,���������롣",	//3
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
 * Description    : �������ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadTestParameterPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* ������ʼ�� */
	TestParameterInit();
	
	/* �������� */
	TestParameterConfig();
	
	/* ��ȡ���� */
	TestParameterReadParameter();
	
	/* ����ֵ������� */
	TestParameterIndexCrossTheBorderProcess();
	
	/* ����һ���˵����� */
	TestParameterOneLevelMenuContentProcess();
	
	/* ��GUI��� */
	GUI_TestParameter();
	
	/* ���� */
	Traverse_TestParameter();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_testParameter.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		TestParameterPutinProcess();
		
		/* �ƶ�����ֵ */
		TestParameterMoveIndexProcess();
		
		/* ״̬���� */
		TestParameterStatusProcess();
		
		/* �ƶ���� */
		TestParameterMoveCursorProcess();	
		
		/* ��ݲ˵� */
		TestParameterShortcutCycleTask();

		/* �������� */
		TestParameterKeyProcess();
		
		/* �������� */
		TestParameterPopWindowsProcess();
		
		/* �뿪ҳ */
		TestParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : TestParameterInit
 * Description    : ���������ʼ��
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
 * Description    : �����������
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
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 6;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KYSNJS] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[TEST_TYPE_KYSNJS] 	= OBJECT_SPECIMEN_VARIETY;		/* �Լ�Ʒ�� */
			g_testParameter.indexArray[STRANG_GRADE_KYSNJS] = OBJECT_STRENGTH_GRADE;		/* ǿ�ȵȼ� */
			g_testParameter.indexArray[SAMPLE_AGE_KYSNJS] 	= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[SPEC_KYSNJS] 		= OBJECT_SPECIMEN_FORMAT;		/* �Լ���� */
			g_testParameter.indexArray[SAMPLE_NUM_KYSNJS] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYSNJS] 	= pValue_KYSNJS[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYSNJS] 		= pValue_KYSNJS[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KYSNJS] 	= pValue_KYSNJS[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYSNJS] 		= pValue_KYSNJS[4];
			g_testParameter.pParameterNameArray[SPEC_KYSNJS] 			= pValue_KYSNJS[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYSNJS] 		= pValue_KYSNJS[7];	
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYSNJS] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYSNJS] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KYSNJS]	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYSNJS] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYSNJS] 			= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYSNJS] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[0];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].parameterCnt		= 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].parameterCnt 	= 11;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].parameterCnt 			= 3;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].parameterType = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].parameterType 		= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYSNJS].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYSNJS].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYSNJS].pParameterNameArray 	= pStrength_grade;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYSNJS].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYSNJS].pParameterNameArray 			= pSpecimen_spec_KYSNJS;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYSNJS].pParameterNameArray 	= pSample_num;
			
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYSNJS].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYSNJS].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYSNJS].saveType 	= TYPE_INT;	

			/* С����λ�� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYSNJS].pointBit 			= 0;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYSNJS].pointBit 	= 0;
			break;
		case KYJZSJ:
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[2];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 6;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KYJZSJ] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[TEST_TYPE_KYJZSJ] 	= OBJECT_SPECIMEN_VARIETY;		/* �Լ�Ʒ�� */
			g_testParameter.indexArray[SAMPLE_AGE_KYJZSJ] 	= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[SPEC_KYJZSJ] 		= OBJECT_SPECIMEN_FORMAT;		/* �Լ���� */
			g_testParameter.indexArray[CORRECT_COF_KYJZSJ] 	= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
			g_testParameter.indexArray[SAMPLE_NUM_KYJZSJ] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYJZSJ] 	= pValue_KYJZSJ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYJZSJ] 		= pValue_KYJZSJ[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYJZSJ] 		= pValue_KYJZSJ[3];
			g_testParameter.pParameterNameArray[SPEC_KYJZSJ] 			= pValue_KYJZSJ[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KYJZSJ] 	= pValue_KYJZSJ[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYJZSJ] 		= pValue_KYJZSJ[7];	
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYJZSJ] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYJZSJ] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYJZSJ]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYJZSJ] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KYJZSJ] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYJZSJ] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[1];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].parameterCnt		= 10;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].parameterCnt 			= 2;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].parameterCnt 	= 2;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].parameterType 		= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYJZSJ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYJZSJ].pParameterNameArray 		= pSample_type_KYJZSJ;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYJZSJ].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYJZSJ].pParameterNameArray 			= pSpecimen_spec_KYJZSJ;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYJZSJ].pParameterNameArray 	= pCorrect_cof_KYJZSJ;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYJZSJ].pParameterNameArray 	= pSample_num;
			
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYJZSJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYJZSJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYJZSJ].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYJZSJ].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYJZSJ].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYJZSJ].saveType 	= TYPE_INT;	

			/* С����λ�� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYJZSJ].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYJZSJ].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYJZSJ].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYJZSJ].pointBit 	= 0;
			break;
		case KYHNT:
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[1];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 6;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KYHNT] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[STRANG_GRADE_KYHNT] 	= OBJECT_STRENGTH_GRADE;		/* ǿ�ȵȼ� */
			g_testParameter.indexArray[SAMPLE_AGE_KYHNT] 	= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[SPEC_KYHNT] 			= OBJECT_SPECIMEN_FORMAT;		/* �Լ���� */
			g_testParameter.indexArray[CORRECT_COF_KYHNT] 	= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
			g_testParameter.indexArray[SAMPLE_NUM_KYHNT] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYHNT] 		= pValue_KYHNT[1];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KYHNT] 	= pValue_KYHNT[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KYHNT] 		= pValue_KYHNT[3];
			g_testParameter.pParameterNameArray[SPEC_KYHNT] 			= pValue_KYHNT[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KYHNT] 		= pValue_KYHNT[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYHNT] 		= pValue_KYHNT[8];	
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KYHNT] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KYHNT]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KYHNT] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KYHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYHNT] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[2];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].parameterCnt	= 15;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].parameterCnt 			= 6;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].parameterCnt 	= 3;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYHNT].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KYHNT].pParameterNameArray 	= pStrength_grade_KZHNT;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KYHNT].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KYHNT].pParameterNameArray 			= pSpecimen_spec_KYHNT;
			g_testParameter.twoLevelMenu[CORRECT_COF_KYHNT].pParameterNameArray 	= pCorrect_coef_KYHNT;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYHNT].pParameterNameArray 		= pSample_num;
			
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYHNT].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KYHNT].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYHNT].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYHNT].saveType 	= TYPE_INT;	

			/* С����λ�� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KYHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KYHNT].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KYHNT].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYHNT].pointBit 	= 0;
			break;
		case KZHNT:
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[1];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 6;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KZHNT] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[STRANG_GRADE_KZHNT] 	= OBJECT_STRENGTH_GRADE;		/* ǿ�ȵȼ� */
			g_testParameter.indexArray[SAMPLE_AGE_KZHNT] 	= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[SPEC_KZHNT] 			= OBJECT_SPECIMEN_FORMAT;		/* �Լ���� */
			g_testParameter.indexArray[CORRECT_COF_KZHNT] 	= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
			g_testParameter.indexArray[SAMPLE_NUM_KZHNT] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KZHNT] 		= pValue_KZHNT[1];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KZHNT] 	= pValue_KZHNT[2];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KZHNT] 		= pValue_KZHNT[3];
			g_testParameter.pParameterNameArray[SPEC_KZHNT] 			= pValue_KZHNT[4];
			g_testParameter.pParameterNameArray[CORRECT_COF_KZHNT] 		= pValue_KZHNT[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KZHNT] 		= pValue_KZHNT[8];	
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KZHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KZHNT] 	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KZHNT]		= pUnitType[8];
			g_testParameter.pParameterUnitArray[SPEC_KZHNT] 			= "NULL";
			g_testParameter.pParameterUnitArray[CORRECT_COF_KZHNT] 		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KZHNT] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[3];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].parameterCnt	= 15;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].parameterCnt 			= 3;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].parameterCnt 	= 2;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZHNT].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZHNT].pParameterNameArray 	= pStrength_grade_KZHNT;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZHNT].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[SPEC_KZHNT].pParameterNameArray 			= pSpecimen_spec;
			g_testParameter.twoLevelMenu[CORRECT_COF_KZHNT].pParameterNameArray 	= pCorrect_coef;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZHNT].pParameterNameArray 		= pSample_num;
			
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZHNT].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZHNT].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[SPEC_KZHNT].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[CORRECT_COF_KZHNT].saveType 	= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KZHNT].saveType 	= TYPE_INT;	

			/* С����λ�� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZHNT].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SPEC_KZHNT].pointBit 			= 0;
			g_testParameter.oneLevelMenu[CORRECT_COF_KZHNT].pointBit 	= 2;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KZHNT].pointBit 	= 0;
			break;
		case KYQQZ:
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[3];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 6;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KYQQZ] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[TEST_TYPE_KYQQZ] 	= OBJECT_SPECIMEN_VARIETY;		/* �Լ�Ʒ�� */
			g_testParameter.indexArray[LENGTH_KYQQZ] 		= OBJECT_SPECIMEN_LENTH;		/* �Լ����� */
			g_testParameter.indexArray[WIDTH_KYQQZ] 		= OBJECT_SPECIMEN_WIDTH;		/* �Լ���� */
			g_testParameter.indexArray[HIGH_KYQQZ] 			= OBJECT_SPECIMEN_HIGH;			/* �Լ��߶� */
			g_testParameter.indexArray[SAMPLE_NUM_KYQQZ] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KYQQZ] 	= pValue_KYQQZ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KYQQZ] 	= pValue_KYQQZ[2];
			g_testParameter.pParameterNameArray[LENGTH_KYQQZ] 		= pValue_KYQQZ[3];
			g_testParameter.pParameterNameArray[WIDTH_KYQQZ] 		= pValue_KYQQZ[4];
			g_testParameter.pParameterNameArray[HIGH_KYQQZ] 		= pValue_KYQQZ[5];
			g_testParameter.pParameterNameArray[SAMPLE_NUM_KYQQZ] 	= pValue_KYQQZ[7];	
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KYQQZ] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KYQQZ] 	= "NULL";
			g_testParameter.pParameterUnitArray[LENGTH_KYQQZ]		= pUnitType[4];
			g_testParameter.pParameterUnitArray[WIDTH_KYQQZ] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[HIGH_KYQQZ] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[SAMPLE_NUM_KYQQZ] 	= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[4];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].parameterCnt		= 8;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].parameterCnt 		= 9;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].parameterCnt 			= 9;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].parameterCnt 			= 9;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].parameterCnt 	= 11;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].parameterType 	= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].parameterType 		= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].parameterType 		= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].parameterType 			= NONE_USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KYQQZ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KYQQZ].pParameterNameArray 		= pSample_type_KYQQZ;
			g_testParameter.twoLevelMenu[LENGTH_KYQQZ].pParameterNameArray 			= pLength_KYQQZ;
			g_testParameter.twoLevelMenu[WIDTH_KYQQZ].pParameterNameArray 			= pWidth_KYQQZ;
			g_testParameter.twoLevelMenu[HIGH_KYQQZ].pParameterNameArray 			= pHigh_KYQQZ;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KYQQZ].pParameterNameArray 		= pSample_num_KYQQZ;
			
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KYQQZ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KYQQZ].saveType 		= TYPE_CHAR;
			g_testParameter.oneLevelMenu[LENGTH_KYQQZ].saveType 		= TYPE_INT;
			g_testParameter.oneLevelMenu[WIDTH_KYQQZ].saveType 			= TYPE_CHAR;
			g_testParameter.oneLevelMenu[HIGH_KYQQZ].saveType 			= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[SAMPLE_NUM_KYQQZ].saveType 	= TYPE_INT;	

			/* С����λ�� */
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
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[6];
			
			/* �����׼ */
			g_testParameter.pTestStandard = "------";
		
			switch ( sampleShape )
			{
				case TYKY_SHAPE_RECTANGLE:
					/* �Կ���� */
					g_testParameter.curParameterNum = 6;
					
					/* ����ֵ */
					g_testParameter.indexArray[TEST_SERIAL_RECTANGLE_KYTY] 	= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
					g_testParameter.indexArray[SAMPLE_SHAPE_RECTANGLE_KYTY] = OBJECT_SHAPE;					/* �Լ���״ */
					g_testParameter.indexArray[SAMPLE_LENTH_RECTANGLE_KYTY] = OBJECT_SPECIMEN_LENTH;		/* ����	    */
					g_testParameter.indexArray[SAMPLE_WIDTH_RECTANGLE_KYTY] = OBJECT_SPECIMEN_WIDTH;		/* ��� 	*/
					g_testParameter.indexArray[CORRECT_COF_RECTANGLE_KYTY] 	= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
					g_testParameter.indexArray[SAMPLE_NUM_RECTANGLE_KYTY] 	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */			/* ��ʱʱ�� */
				
					/* �������� */
					g_testParameter.pParameterNameArray[TEST_SERIAL_RECTANGLE_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RECTANGLE_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_LENTH_RECTANGLE_KYTY] 	= pValue_KYTY[10];
					g_testParameter.pParameterNameArray[SAMPLE_WIDTH_RECTANGLE_KYTY] 	= pValue_KYTY[11];
					g_testParameter.pParameterNameArray[CORRECT_COF_RECTANGLE_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_RECTANGLE_KYTY] 		= pValue_KYTY[6];		
				
					/* ��λ */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_LENTH_RECTANGLE_KYTY]	= pUnitType[4];
					g_testParameter.pParameterUnitArray[SAMPLE_WIDTH_RECTANGLE_KYTY] 	= pUnitType[4];
					g_testParameter.pParameterUnitArray[CORRECT_COF_RECTANGLE_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_RECTANGLE_KYTY] 		= "NULL";
					
					/* �����˵��������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].parameterCnt 		= 7;
					
					/* �����˵����� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* �����˵������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].pParameterNameArray 	= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_RECTANGLE_KYTY].pParameterNameArray 	= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].pParameterNameArray 	= pSample_num;
					
					/* ���ݱ������� */
					g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_RECTANGLE_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].saveType 		= TYPE_INT;	

					/* С����λ�� */
					g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_LENTH_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[CORRECT_COF_RECTANGLE_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KYTY].pointBit 		= 0;
					break;
				case TYKY_SHAPE_ROUND:
					/* �Կ���� */
					g_testParameter.curParameterNum = 5;
					
					/* ����ֵ */
					g_testParameter.indexArray[TEST_SERIAL_ROUND_KYTY] 		= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
					g_testParameter.indexArray[SAMPLE_SHAPE_ROUND_KYTY] 	= OBJECT_SHAPE;					/* �Լ���״ */
					g_testParameter.indexArray[SAMPLE_DIAMETER_ROUND_KYTY] 	= OBJECT_ROUND_DIAMETER;				/* ֱ��	    */
					g_testParameter.indexArray[CORRECT_COF_ROUND_KYTY] 		= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
					g_testParameter.indexArray[SAMPLE_NUM_ROUND_KYTY] 		= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */			/* ��ʱʱ�� */
				
					/* �������� */
					g_testParameter.pParameterNameArray[TEST_SERIAL_ROUND_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_ROUND_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_DIAMETER_ROUND_KYTY] = pValue_KYTY[12];
					g_testParameter.pParameterNameArray[CORRECT_COF_ROUND_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_ROUND_KYTY] 		= pValue_KYTY[6];		
				
					/* ��λ */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_DIAMETER_ROUND_KYTY]	= pUnitType[4];
					g_testParameter.pParameterUnitArray[CORRECT_COF_ROUND_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_ROUND_KYTY] 		= "NULL";
					
					/* �����˵��������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].parameterCnt 	= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].parameterCnt 		= 7;
					
					/* �����˵����� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].parameterType 	= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* �����˵������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].pParameterNameArray 		= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].pParameterNameArray 	= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_ROUND_KYTY].pParameterNameArray 		= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KYTY].pParameterNameArray 		= pSample_num;
					
					/* ���ݱ������� */
					g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].saveType 	= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_ROUND_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KYTY].saveType 		= TYPE_INT;	

					/* С����λ�� */
					g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KYTY].pointBit 		= 0;
					g_testParameter.oneLevelMenu[SAMPLE_DIAMETER_ROUND_KYTY].pointBit 	= 2;
					g_testParameter.oneLevelMenu[CORRECT_COF_ROUND_KYTY].pointBit 		= 2;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KYTY].pointBit 		= 0;
					break;
				case TYKY_SHAPE_IRREGULAR:
					/* �Կ���� */
					g_testParameter.curParameterNum = 5;
					
					/* ����ֵ */
					g_testParameter.indexArray[TEST_SERIAL_IRREGULAR_KYTY] 		= OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
					g_testParameter.indexArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= OBJECT_SHAPE;					/* �Լ���״ */
					g_testParameter.indexArray[SAMPLE_AREA_IRREGULAR_KYTY] 		= OBJECT_AREA;					/* ���	    */
					g_testParameter.indexArray[CORRECT_COF_IRREGULAR_KYTY] 		= OBJECT_CORRECTION_FACTOR;		/* ����ϵ�� */
					g_testParameter.indexArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= OBJECT_SPECIMEN_NUMS;			/* �Լ����� */			/* ��ʱʱ�� */
				
					/* �������� */
					g_testParameter.pParameterNameArray[TEST_SERIAL_IRREGULAR_KYTY] 	= pValue_KYTY[1];
					g_testParameter.pParameterNameArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= pValue_KYTY[3];
					g_testParameter.pParameterNameArray[SAMPLE_AREA_IRREGULAR_KYTY] 	= pValue_KYTY[4];
					g_testParameter.pParameterNameArray[CORRECT_COF_IRREGULAR_KYTY] 	= pValue_KYTY[9];
					g_testParameter.pParameterNameArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= pValue_KYTY[6];	
				
					/* ��λ */
					g_testParameter.pParameterUnitArray[TEST_SERIAL_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_AREA_IRREGULAR_KYTY]		= pUnitType[5];
					g_testParameter.pParameterUnitArray[CORRECT_COF_IRREGULAR_KYTY] 	= "NULL";
					g_testParameter.pParameterUnitArray[SAMPLE_NUM_IRREGULAR_KYTY] 		= "NULL";
					
					/* �����˵��������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].parameterCnt		= 3;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].parameterCnt 		= 0;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].parameterCnt 		= 2;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].parameterCnt 		= 7;
					
					/* �����˵����� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].parameterType 		= IMMEDIATELY_PUTIN_SHIFT;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].parameterType 	= NONE_USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].parameterType 		= IMMEDIATELY_PUTIN_NONE;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].parameterType 		= USE_USER_DEFINE;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].parameterType 		= USE_USER_DEFINE;
					
					/* �����˵������� */
					g_testParameter.twoLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].pParameterNameArray 		= pSpecimen_sharp;
					g_testParameter.twoLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].pParameterNameArray 		= NULL;
					g_testParameter.twoLevelMenu[CORRECT_COF_IRREGULAR_KYTY].pParameterNameArray 		= pSpecimen_cof;
					g_testParameter.twoLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].pParameterNameArray 		= pSample_num;
					
					/* ���ݱ������� */
					g_testParameter.oneLevelMenu[TEST_SERIAL_IRREGULAR_KYTY].saveType 		= TYPE_CHAR;
					g_testParameter.oneLevelMenu[SAMPLE_SHAPE_IRREGULAR_KYTY].saveType 		= TYPE_INT;
					g_testParameter.oneLevelMenu[SAMPLE_AREA_IRREGULAR_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[CORRECT_COF_IRREGULAR_KYTY].saveType 		= TYPE_FLOAT;
					g_testParameter.oneLevelMenu[SAMPLE_NUM_IRREGULAR_KYTY].saveType 		= TYPE_INT;	

					/* С����λ�� */
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
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 7;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_KZSNJS] 	    = OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[TEST_TYPE_KZSNJS] 	    = OBJECT_SPECIMEN_VARIETY;		/* �Լ�Ʒ�� */
			g_testParameter.indexArray[STRANG_GRADE_KZSNJS] 	= OBJECT_STRENGTH_GRADE;		/* ǿ�ȵȼ� */
			g_testParameter.indexArray[SAMPLE_AGE_KZSNJS] 		= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[ZFX_LEN_KZSNJS] 	        = OBJECT_SPECIMEN_LENTH;		/* �Լ��߳� */
			g_testParameter.indexArray[ZFX_SPAN_KZSNJS] 	    = OBJECT_SPECIMEN_SPAN;			/* �Լ���� */
            g_testParameter.indexArray[SAMPLE_NUM_KZSNJS] 	    = OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_KZSNJS] 	= pValue_KZSNJS[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_KZSNJS] 	    = pValue_KZSNJS[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_KZSNJS] 	= pValue_KZSNJS[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_KZSNJS] 		= pValue_KZSNJS[4];
			g_testParameter.pParameterNameArray[ZFX_LEN_KZSNJS] 		= pValue_KZSNJS[5];
			g_testParameter.pParameterNameArray[ZFX_SPAN_KZSNJS] 		= pValue_KZSNJS[6];
            g_testParameter.pParameterNameArray[SAMPLE_NUM_KZSNJS] 		= pValue_KZSNJS[8];
		
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_KZSNJS] 	= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_KZSNJS] 	    = "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_KZSNJS]	= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_KZSNJS] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[ZFX_LEN_KZSNJS] 		= pUnitType[4];
			g_testParameter.pParameterUnitArray[ZFX_SPAN_KZSNJS] 		= pUnitType[4];
            g_testParameter.pParameterUnitArray[SAMPLE_NUM_KZSNJS] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[7];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].parameterCnt	    = 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].parameterCnt 	= 11;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].parameterCnt 	    = 5;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].parameterCnt 	    = 6;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].parameterType = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].parameterType 	    = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].parameterType 	= USE_USER_DEFINE;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_KZSNJS].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_KZSNJS].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_KZSNJS].pParameterNameArray 	= pStrength_grade;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_KZSNJS].pParameterNameArray 	= pSample_age;
			g_testParameter.twoLevelMenu[ZFX_LEN_KZSNJS].pParameterNameArray 		= pZfx_length;
			g_testParameter.twoLevelMenu[ZFX_SPAN_KZSNJS].pParameterNameArray 		= pZfx_span;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_KZSNJS].pParameterNameArray 	= pSample_num;
            
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZSNJS].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZSNJS].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[ZFX_LEN_KZSNJS].saveType 		= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[ZFX_SPAN_KZSNJS].saveType 		= TYPE_FLOAT;	
            g_testParameter.oneLevelMenu[SAMPLE_NUM_KZSNJS].saveType 	= TYPE_INT;	

			/* С����λ�� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[TEST_TYPE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[STRANG_GRADE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_KZSNJS].pointBit 	= 0;
			g_testParameter.oneLevelMenu[ZFX_LEN_KZSNJS].pointBit 		= 2;
			g_testParameter.oneLevelMenu[ZFX_SPAN_KZSNJS].pointBit 		= 2;
            g_testParameter.oneLevelMenu[SAMPLE_NUM_KZSNJS].pointBit 	= 0;
			break;
		case KZYJSNJ:
			/* �����׼ */
			g_testParameter.pTestStandard = pTestStandard[0];
		
			/* �Կ���� */
			g_testParameter.curParameterNum = 7;
			
			/* ����ֵ */
			g_testParameter.indexArray[TEST_SERIAL_YJSNJ] 	    = OBJECT_SPECIMEN_SERIAL;		/* �Լ���� */	
			g_testParameter.indexArray[TEST_TYPE_YJSNJ] 	    = OBJECT_SPECIMEN_VARIETY;		/* �Լ�Ʒ�� */
			g_testParameter.indexArray[STRANG_GRADE_YJSNJ] 		= OBJECT_STRENGTH_GRADE;		/* ǿ�ȵȼ� */
			g_testParameter.indexArray[SAMPLE_AGE_YJSNJ] 		= OBJECT_SPECIMEN_AGE;			/* �Լ����� */
			g_testParameter.indexArray[ZFX_LEN_YJSNJ] 	        = OBJECT_SPECIMEN_LENTH;		/* �Լ��߳� */
			g_testParameter.indexArray[ZFX_SPAN_YJSNJ] 	    	= OBJECT_SPECIMEN_SPAN;			/* �Լ���� */
            g_testParameter.indexArray[SAMPLE_NUM_YJSNJ] 	    = OBJECT_SPECIMEN_NUMS;			/* �Լ����� */
			
			/* �������� */
			g_testParameter.pParameterNameArray[TEST_SERIAL_YJSNJ] 		= pValue_KZYJSNJ[1];
			g_testParameter.pParameterNameArray[TEST_TYPE_YJSNJ] 	    = pValue_KZYJSNJ[2];
			g_testParameter.pParameterNameArray[STRANG_GRADE_YJSNJ] 	= pValue_KZYJSNJ[3];
			g_testParameter.pParameterNameArray[SAMPLE_AGE_YJSNJ] 		= pValue_KZYJSNJ[4];
			g_testParameter.pParameterNameArray[ZFX_LEN_YJSNJ] 			= pValue_KZYJSNJ[5];
			g_testParameter.pParameterNameArray[ZFX_SPAN_YJSNJ] 		= pValue_KZYJSNJ[6];
            g_testParameter.pParameterNameArray[SAMPLE_NUM_YJSNJ] 		= pValue_KZYJSNJ[8];
			
			/* ��λ */
			g_testParameter.pParameterUnitArray[TEST_SERIAL_YJSNJ] 		= "NULL";
			g_testParameter.pParameterUnitArray[TEST_TYPE_YJSNJ] 	    = "NULL";
			g_testParameter.pParameterUnitArray[STRANG_GRADE_YJSNJ]		= "NULL";
			g_testParameter.pParameterUnitArray[SAMPLE_AGE_YJSNJ] 		= pUnitType[8];
			g_testParameter.pParameterUnitArray[ZFX_LEN_YJSNJ] 			= pUnitType[4];
			g_testParameter.pParameterUnitArray[ZFX_SPAN_YJSNJ] 		= pUnitType[4];
            g_testParameter.pParameterUnitArray[SAMPLE_NUM_YJSNJ] 		= "NULL";
			
			/* ���� */
			g_testParameter.pTitle = pTestParameterTitleName[8];
			
			/* �����˵��������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].parameterCnt 	= 0;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].parameterCnt	    = 10;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].parameterCnt 	= 5;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].parameterCnt 	= 6;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].parameterCnt 	    = 5;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].parameterCnt 	    = 6;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].parameterCnt 	= 7;
			
			/* �����˵����� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].parameterType 	= IMMEDIATELY_PUTIN_SHIFT;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].parameterType 	= USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].parameterType 	    = USE_USER_DEFINE;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].parameterType 		= USE_USER_DEFINE;
            g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].parameterType 	= USE_USER_DEFINE;
			
			/* �����˵������� */
			g_testParameter.twoLevelMenu[TEST_SERIAL_YJSNJ].pParameterNameArray 	= NULL;
			g_testParameter.twoLevelMenu[TEST_TYPE_YJSNJ].pParameterNameArray 		= pSample_type;
			g_testParameter.twoLevelMenu[STRANG_GRADE_YJSNJ].pParameterNameArray 	= pStrength_grade_KZYJSNJ;
			g_testParameter.twoLevelMenu[SAMPLE_AGE_YJSNJ].pParameterNameArray 		= pSample_age;
			g_testParameter.twoLevelMenu[ZFX_LEN_YJSNJ].pParameterNameArray 		= pZfx_length;
			g_testParameter.twoLevelMenu[ZFX_SPAN_YJSNJ].pParameterNameArray 		= pZfx_span;
			g_testParameter.twoLevelMenu[SAMPLE_NUM_YJSNJ].pParameterNameArray 		= pSample_num;
            
			/* ���ݱ������� */
			g_testParameter.oneLevelMenu[TEST_SERIAL_YJSNJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[TEST_TYPE_YJSNJ].saveType 		= TYPE_CHAR;
			g_testParameter.oneLevelMenu[STRANG_GRADE_YJSNJ].saveType 	= TYPE_CHAR;
			g_testParameter.oneLevelMenu[SAMPLE_AGE_YJSNJ].saveType 	= TYPE_INT;
			g_testParameter.oneLevelMenu[ZFX_LEN_YJSNJ].saveType 		= TYPE_FLOAT;
			g_testParameter.oneLevelMenu[ZFX_SPAN_YJSNJ].saveType 		= TYPE_FLOAT;	
            g_testParameter.oneLevelMenu[SAMPLE_NUM_YJSNJ].saveType 	= TYPE_INT;	

			/* С����λ�� */
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
				
				/* ���� */
				g_testParameter.pTitle = pTestParameterTitleName[10];
				
				/* �����׼ */
				g_testParameter.pTestStandard = pTestStandard[4];
			
				switch ( sampleShape )
				{
					case JSSWKL_SHAPE_RECTANGLE:
						/* �Կ���� */
						g_testParameter.curParameterNum = 9;
						
						/* ����ֵ */
						g_testParameter.indexArray[TEST_SERIAL_RECTANGLE_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* �Լ����  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 				= OBJECT_SHAPE;					/* ��״ 	 */
						g_testParameter.indexArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW] 			= OBJECT_SPECIMEN_LENTH;		/* �Լ�����  */
						g_testParameter.indexArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 				= OBJECT_SPECIMEN_WIDTH;		/* �Լ���� 	 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 		= OBJECT_AREA;					/* ԭʼ����� */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* ����Ʊ�� */
						g_testParameter.indexArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* ԭʼ��� 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* ƽ�г��� 	 */
						g_testParameter.indexArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� 	 */
					
						/* �������� */
						g_testParameter.pParameterNameArray[TEST_SERIAL_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW] 		= pValue_KLJSSW[8];
						g_testParameter.pParameterNameArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 			= pValue_KLJSSW[9];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 	= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]	= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 		= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	    = pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	       	= pValue_KLJSSW[5];
						
						/* ��λ */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_RECTANGLE_KLJSSW] 			= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RECTANGLE_KLJSSW] 			= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_THICKNESS_RECTANGLE_KLJSSW]		= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_WIDTH_RECTANGLE_KLJSSW] 			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW] 	= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW]	= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_RECTANGLE_KLJSSW] 		= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_RECTANGLE_KLJSSW] 	    = pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_RECTANGLE_KLJSSW] 	       	= "NULL";
						
						/* �����˵��������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].parameterCnt			= 4;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].parameterCnt 		= 0;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].parameterCnt 	    	= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].parameterCnt  	   	= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].parameterCnt  	       	= 7;
						
						/* �����˵����� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].parameterType 			= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].parameterType 			= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].parameterType 			= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].parameterType 	= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].parameterType	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].parameterType 	    = IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].parameterType  	   	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].parameterType  	       	= USE_USER_DEFINE;
						
						/* �����˵������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].pParameterNameArray 		= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].pParameterNameArray	= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].pParameterNameArray  	   	= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].pParameterNameArray  	       	= pSample_num;
						
						/* ���ݱ������� */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RECTANGLE_KLJSSW].saveType 			= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RECTANGLE_KLJSSW].saveType 			= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_THICKNESS_RECTANGLE_KLJSSW].saveType 		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_WIDTH_RECTANGLE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RECTANGLE_KLJSSW].saveType 	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RECTANGLE_KLJSSW].saveType		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RECTANGLE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RECTANGLE_KLJSSW].saveType  	   	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RECTANGLE_KLJSSW].saveType  	       	= TYPE_INT;

						/* С����λ�� */
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
						/* �Կ���� */
						g_testParameter.curParameterNum = 8;
						
						/* ����ֵ */
						g_testParameter.indexArray[TEST_SERIAL_ROUND_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* �Լ����  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_ROUND_KLJSSW] 				= OBJECT_SHAPE;					/* ��״ 	 */
						g_testParameter.indexArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= OBJECT_ROUND_DIAMETER;		/* Բ��ֱ�� */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 		= OBJECT_AREA;					/* ԭʼ����� */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* ����Ʊ�� */
						g_testParameter.indexArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* ԭʼ��� 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_ROUND_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* ƽ�г��� 	 */
						g_testParameter.indexArray[SAMPLE_NUM_ROUND_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� 	 */
					
						/* �������� */
						g_testParameter.pParameterNameArray[TEST_SERIAL_ROUND_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_ROUND_KLJSSW] 					= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= pValue_KLJSSW[2];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_ROUND_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_ROUND_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* ��λ */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_ROUND_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_ROUND_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW] 		= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_ROUND_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_ROUND_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_ROUND_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_ROUND_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_ROUND_KLJSSW] 	       			= "NULL";
						
						/* �����˵��������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].parameterCnt 	    			= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].parameterCnt  	       			= 7;
						
						/* �����˵����� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].parameterType 		= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].parameterType		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].parameterType 	    	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* �����˵������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_ROUND_KLJSSW].pParameterNameArray 					= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].pParameterNameArray 				= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].pParameterNameArray  	   			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].pParameterNameArray  	       			= pSample_num;
						
						/* ���ݱ������� */
						g_testParameter.oneLevelMenu[TEST_SERIAL_ROUND_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_ROUND_KLJSSW].saveType 					= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_ROUND_DIAMETER_ROUND_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_ROUND_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_ROUND_KLJSSW].saveType				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_ROUND_KLJSSW].saveType 					= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_ROUND_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_ROUND_KLJSSW].saveType  	       			= TYPE_INT;

						/* С����λ�� */
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
						/* �Կ���� */
						g_testParameter.curParameterNum = 9;
						
						/* ����ֵ */
						g_testParameter.indexArray[TEST_SERIAL_TUBE_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* �Լ����  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_TUBE_KLJSSW] 				= OBJECT_SHAPE;					/* ��״ 	 */
						g_testParameter.indexArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW] 		= OBJECT_PIPE_THICKNESS;		/* �ܶκ��  */
						g_testParameter.indexArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= OBJECT_PIPE_OUTER_DIAMETER;	/* �ܶ���ֱ�� */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 		= OBJECT_AREA;					/* ԭʼ����� */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= OBJECT_EXTERNSOMETER_GAUGE;	/* ����Ʊ�� */
						g_testParameter.indexArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 	        	= OBJECT_ORIGINAL_GAUGE;		/* ԭʼ��� 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_TUBE_KLJSSW] 	        	= OBJECT_PARALLEL_LENTH;		/* ƽ�г��� 	 */
						g_testParameter.indexArray[SAMPLE_NUM_TUBE_KLJSSW] 	        		= OBJECT_SPECIMEN_NUMS;			/* �Լ����� 	 */
					
						/* �������� */
						g_testParameter.pParameterNameArray[TEST_SERIAL_TUBE_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_TUBE_KLJSSW] 					= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW] 			= pValue_KLJSSW[10];
						g_testParameter.pParameterNameArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= pValue_KLJSSW[11];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_TUBE_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_TUBE_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* ��λ */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_TUBE_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_TUBE_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW] 	= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_TUBE_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_TUBE_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_TUBE_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_TUBE_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_TUBE_KLJSSW] 	       			= "NULL";
						
						/* �����˵��������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].parameterCnt 		= 0;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].parameterCnt 	= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].parameterCnt 	    		= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].parameterCnt  	       			= 7;
						
						/* �����˵����� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].parameterType 	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].parameterType 		= NONE_PARAMETER;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].parameterType			= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].parameterType 	    		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* �����˵������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_TUBE_KLJSSW].pParameterNameArray 					= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].pParameterNameArray 					= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].pParameterNameArray 	= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].pParameterNameArray 			= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].pParameterNameArray  	   			= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].pParameterNameArray  	       			= pSample_num;
						
						/* ���ݱ������� */
						g_testParameter.oneLevelMenu[TEST_SERIAL_TUBE_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_TUBE_KLJSSW].saveType 				= TYPE_INT;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_THICKNESS_TUBE_KLJSSW].saveType 		= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_PIPE_OUTER_DIAMETER_TUBE_KLJSSW].saveType 	= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_TUBE_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_TUBE_KLJSSW].saveType			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_TUBE_KLJSSW].saveType 				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_TUBE_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_TUBE_KLJSSW].saveType  	       			= TYPE_INT;

						/* С����λ�� */
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
						/* �Կ���� */
						g_testParameter.curParameterNum = 7;
						
						/* ����ֵ */
						g_testParameter.indexArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 	    		= OBJECT_SPECIMEN_SERIAL;		/* �Լ����  */	
						g_testParameter.indexArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= OBJECT_SHAPE;					/* ��״ 	 */
						g_testParameter.indexArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 		= OBJECT_AREA;					/* ԭʼ����� */
						g_testParameter.indexArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]		= OBJECT_EXTERNSOMETER_GAUGE;	/* ����Ʊ�� */
						g_testParameter.indexArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 	        = OBJECT_ORIGINAL_GAUGE;		/* ԭʼ��� 	 */
						g_testParameter.indexArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	        = OBJECT_PARALLEL_LENTH;		/* ƽ�г��� 	 */
						g_testParameter.indexArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	        	= OBJECT_SPECIMEN_NUMS;			/* �Լ����� 	 */
					
						/* �������� */
						g_testParameter.pParameterNameArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 					= pValue_KLJSSW[1];
						g_testParameter.pParameterNameArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= pValue_KLJSSW[12];
						g_testParameter.pParameterNameArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 			= pValue_KLJSSW[3];
						g_testParameter.pParameterNameArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]			= pValue_KLJSSW[4];
						g_testParameter.pParameterNameArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 				= pValue_KLJSSW[7];
						g_testParameter.pParameterNameArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	   		 	= pValue_KLJSSW[6];		
						g_testParameter.pParameterNameArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	      		 	= pValue_KLJSSW[5];
						
						/* ��λ */
						g_testParameter.pParameterUnitArray[TEST_SERIAL_RIRREGULAR_KLJSSW] 					= "NULL";
						g_testParameter.pParameterUnitArray[SAMPLE_SHAPE_RIRREGULAR_KLJSSW] 				= "NULL";
						g_testParameter.pParameterUnitArray[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW] 			= pUnitType[5];
						g_testParameter.pParameterUnitArray[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW]			= pUnitType[4];
						g_testParameter.pParameterUnitArray[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW] 				= pUnitType[4];
						g_testParameter.pParameterUnitArray[PARALLEL_LENTH_RIRREGULAR_KLJSSW] 	   			= pUnitType[4];
						g_testParameter.pParameterUnitArray[SAMPLE_NUM_RIRREGULAR_KLJSSW] 	       			= "NULL";
						
						/* �����˵��������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].parameterCnt 					= 0;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].parameterCnt					= 4;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].parameterCnt 			= 0;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].parameterCnt 	    		= 0;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].parameterCnt  	   			= 0;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].parameterCnt  	       			= 7;
						
						/* �����˵����� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].parameterType 				= IMMEDIATELY_PUTIN_SHIFT;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].parameterType 				= NONE_USE_USER_DEFINE;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].parameterType 		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].parameterType		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].parameterType 	    	= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].parameterType  	   		= IMMEDIATELY_PUTIN_NONE;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].parameterType  	       		= USE_USER_DEFINE;
						
						/* �����˵������� */
						g_testParameter.twoLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].pParameterNameArray 				= pSpecimenSharp_KLJSSW;
						g_testParameter.twoLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].pParameterNameArray 		= NULL;
						g_testParameter.twoLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].pParameterNameArray			= NULL;
						g_testParameter.twoLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].pParameterNameArray 				= NULL;
						g_testParameter.twoLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].pParameterNameArray  	   		= NULL;
						g_testParameter.twoLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].pParameterNameArray  	       		= pSample_num;
						
						/* ���ݱ������� */
						g_testParameter.oneLevelMenu[TEST_SERIAL_RIRREGULAR_KLJSSW].saveType 					= TYPE_CHAR;
						g_testParameter.oneLevelMenu[SAMPLE_SHAPE_RIRREGULAR_KLJSSW].saveType 					= TYPE_INT;
						g_testParameter.oneLevelMenu[ORIGINAL_CROSS_AREA_RIRREGULAR_KLJSSW].saveType 			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[EXTERNSOMETER_GAUGE_RIRREGULAR_KLJSSW].saveType			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[ORIGINAL_GAUGE_RIRREGULAR_KLJSSW].saveType 				= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[PARALLEL_LENTH_RIRREGULAR_KLJSSW].saveType  	   			= TYPE_FLOAT;
						g_testParameter.oneLevelMenu[SAMPLE_NUM_RIRREGULAR_KLJSSW].saveType  	       			= TYPE_INT;

						/* С����λ�� */
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
 * Description    : ��ȡ�����������
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetTestParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t testIndex = 0xff;	//�����ֵ
	
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
 * Description    : ������
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
 * Description    : д����
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
 * Description    : һ���˵����ݴ���(�����������ֵԽ�紦��֮��)
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
			if (index)	//�ǡ��Զ��塱����������
			{
				pParaName = g_testParameter.twoLevelMenu[i].pParameterNameArray[index];
			
				strcpy(g_testParameter.parameterData[i],pParaName);
			}
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigTestParameterRectangleFrameCoordinate
 * Description    : ���������������GUI����
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
 * Description    : �����������GUI
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
 * Description    : �����������GUI
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
 * Description    : �����������GUI
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
 * Description    : �����������һ���˵�
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
 * Description    : �����������GUI
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
 * Description    : ��ʾһ���������
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
 * Description    : ��ʾһ���˵���ʾ����
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
 * Description    : ��ʾһ���˵�����
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
 * Description    : ��ʾһ�����������λ
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
 * Description    : ��ʾһ���˵���λ
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
 * Description    : ��ʾһ�����������ͷ
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
 * Description    : ���һ�����������ͷ
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
 * Description    : �����������
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
 * Description    : �����������ֵԽ�紦��
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
 * Description    : ��������ƶ�����ֵ
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
 * Description    : ��ԭ��ʾһ���˵���
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
 * Description    : ��ʾһ�������Ĳ˵���
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
 * Description    : �ƶ���괦��
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
 * Description    : ״̬����
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
 * Description    : ���봦��
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
 * Description    : ѹ������
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
 * Description    : ������������
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
 * Description    : �����˵���������
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
 * Description    : ����ֵ����
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
 * Description    : ����״̬
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
 * Description    : ��ݲ˵�����
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
 * Description    : ��1ѭ����
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
	else		//��������'-'�������ַ�
	{
		(*pCur_bit)++;			
	}	
}

/*------------------------------------------------------------
 * Function Name  : TestParameterSerialAddOne
 * Description    : ���������ż�1ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static SERIAL_ADD_ONE_STA TestParameterSerialAddOne( TEST_TYPE_TypeDef type, char *pSerial, uint8_t *pCnt )
{
	uint8_t cur_bit = 0, last_bit = 0,bit_num = 1;
	char buff[MAX_TEST_PARAMETER_PUTIN_BIT+1] = {0};	  //�ݴ������ı��
	const char EXTEND_CHARACTER = '0';	//��չһλ������ַ�

	strcpy(buff,pSerial);
	
	while ( FR_OK == report_exist(type,buff) )	//��Ŵ���ͬ��
	{ 	
		cur_bit = 0;
		last_bit = 0;
		
		StrCutBackReplace(buff);	//����ԭ�����ݴ���			
		
		while (last_bit <= cur_bit)
		{
			SerialAddOneCycle(buff,&cur_bit);				
			if (buff[cur_bit] == '\0')	 
			{
				buff[cur_bit] = EXTEND_CHARACTER;	
			}
			last_bit++;		 
		}
				
		StrCutBackReplace(buff);	//��ԭԭ�����ݴ���
	}
	
	bit_num = strlen(buff);
	
	if (bit_num > MAX_TEST_PARAMETER_PUTIN_BIT)		//Խ��			
	{
		return SERIAL_REACH_MAX_VALUE;
	}

	strcpy(pSerial,buff);

	*pCnt = bit_num;	
	
	return CHANGE_SERIAL_OK;		
}

/*------------------------------------------------------------
 * Function Name  : TestParameterCheckSerialRepeat
 * Description    : ����ظ��Լ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SERIAL_ADD_ONE_STA TestParameterCheckSerialRepeat( TEST_TYPE_TypeDef type, char *pStr, uint8_t *pNum )
{
	SERIAL_ADD_ONE_STA status;
	REPORT_TypeDef report;
	
	if(SET == FindStrIncludeIllegalChar(pStr,'*') ) 			//�ļ�ϵͳ��֧�ָ��ַ�	
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
 * Description    : ����Բ��ֱ����ȡ�����
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
 * Description    : ��������
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
					/* �����ݲ˵� */
					GUI_ClearShortcutMenu();
					
					/* ��ԭһ���˵� */
					g_testParameter.oneLevelMenu[g_testParameter.recordIndex].pointColor = \
						g_testParameter.oneLevelMenu[g_testParameter.recordIndex].recordPointColor;
					g_testParameter.oneLevelMenu[g_testParameter.recordIndex].backColor = \
						g_testParameter.oneLevelMenu[g_testParameter.recordIndex].recordBackColor;
					
					Show_TestParameterOneRowOneLevelMenuContent(g_testParameter.recordIndex);
					Show_TestParameterOneRowOneLevelMenuUnit(g_testParameter.recordIndex);	
					
					/* ���ö����˵� */
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
						
						if ((g_testParameter.twoLevelMenu[index].parameterType==USE_USER_DEFINE) && (pMenu->nowIndex==0))	//��һ�����Ĭ��Ϊ�Զ���ѡ��
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
 * Description    : ��������
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
 * Description    : �뿪ҳ���
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
 * Description    : ������ϢGUI
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
 * Description    : ���ػ�ȡ������Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadMainPageGetTestInfomationPage( void )
{
//	uint8_t indexTarget,indexSource;
	
	/* ������ʼ�� */
	TestParameterInit();
	
	/* �������� */
	TestParameterConfig();
	
	/* ��ȡ���� */
	TestParameterReadParameter();
	
	/* ����ֵ������� */
	TestParameterIndexCrossTheBorderProcess();
	
	/* ����һ���˵����� */
	TestParameterOneLevelMenuContentProcess();
	
	/* ��GUI��� */
	GUI_MainPageTestInfomation();
	
	/* ���� */
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
 * Description    : �л��Լ���״
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
