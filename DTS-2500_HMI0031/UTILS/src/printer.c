/**
  ******************************************************************************
  * @file    printer.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-5-15 10:37:14
  * @brief   ��ӡ�ļ�
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "printer.h"
#include "ForceCalibration.h"


/* Private define ------------------------------------------------------------*/
#define END_ROW_CNT			2			//��ӡ����������


/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
const char * const pPrint_Title[] = 
{
	"1#",
	"2#",
	"3#",
	"4#",
	"5#",
	"6#",
	"7#",
	"8#",
	"9#",
	"10#",
	"11#",
	"12#",
	"13#",
	"14#",
	"15#",
	"16#",
	"17#",
	"18#",
	"19#",
	"20#",
};

const char * const pPrintErrorCue[] = 
{
	"�궨����������",			//0
	"�������ͳ���",			//1
	"�Կ��������1~20��Χ��",	//2
};

extern const char * const pSpecimen_sharp[];
extern const char * const pSpecimenSharp_KLJSSW[];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : ConfigPrinterDefaultSet
 * Description    : ���ô�ӡ��Ĭ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigPrinterDefaultSet( void )
{
	const uint8_t FORWARD_DIR = 1;
	
	print_chinese();
	print_direct(FORWARD_DIR);
	
	/* ���ÿ�δ�ӡ��һ������ֻ�ܴ�ӡ���°벿�� */
	print_enter();
}

/*------------------------------------------------------------
 * Function Name  : PrintWordsAndLineFeed
 * Description    : �����ַ�ͬʱ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintWordsAndLineFeed( const char *word )
{
	print((char *)word);
	print_enter();
}

/*------------------------------------------------------------
 * Function Name  : PrintDottedLine
 * Description    : ��ӡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintDottedLine( void )
{	
	print("- - - - - - - - - - - - ");
	
	print_enter();
}

/*------------------------------------------------------------
 * Function Name  : PrintTime
 * Description    : ��ӡʱ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintTime( tTime *t )
{
	char buff[12];
	
	print("���ڣ�");
	usprintf(buff,"%04d/%02d/%02d", t->usYear,t->ucMon,t->ucMday);
	PrintWordsAndLineFeed(buff);
	
	print("ʱ�䣺");
	usprintf(buff,"%02d:%02d:%02d", t->ucHour,t->ucMin,t->ucSec);	
	PrintWordsAndLineFeed(buff);
}

/*------------------------------------------------------------
 * Function Name  : PrintRowSpace
 * Description    : ��ӡ�ո�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintRowSpace( uint8_t num )
{
	uint8_t i;
	
	for (i=0; i<num; ++i)
	{
		print_enter();
	}
	
	bsp_DelayMS(1000);
}

/*------------------------------------------------------------
 * Function Name  : PrintUnit
 * Description    : ��ӡ��λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintUnit( SMPL_NAME_TypeDef channel )
{
	FH_UINT_TypeDef fhChannelUnit = GetFH_SmplUnit();
	WY_UINT_TypeDef wyChannelUnit = GetWY_SmplUnit();
	BX_UINT_TypeDef bxChannelUnit = GetBX_SmplUnit();
	
	switch ( channel )
	{
		case SMPL_FH_NUM:			
			switch (fhChannelUnit)
			{
				case FH_UNIT_kN:
					PrintWordsAndLineFeed("kN");
					break;
				case FH_UNIT_N:
					PrintWordsAndLineFeed("N");
					break;
			}
			break;
		
		case SMPL_WY_NUM:
			switch ( wyChannelUnit )
			{
				case WY_UNIT_MM:
					PrintWordsAndLineFeed("mm");
					break;
				case WY_UNIT_CM:
					PrintWordsAndLineFeed("cm");
					break;
				case WY_UNIT_DM:
					PrintWordsAndLineFeed("dm");
					break;
				case WY_UNIT_M:
					PrintWordsAndLineFeed("m");
					break; 
				default:
					break;
			}
			break;
		
		case SMPL_BX_NUM:
			switch ( bxChannelUnit )
			{
				case BX_UNIT_MM:
					PrintWordsAndLineFeed("mm");
					break;
				case BX_UNIT_CM:
					PrintWordsAndLineFeed("cm");
					break;
				case BX_UNIT_DM:
					PrintWordsAndLineFeed("dm");
					break;
				case BX_UNIT_M:
					PrintWordsAndLineFeed("m");
					break; 
				default:
					break;
			}
			break;
		
		default:				
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : PrintCalibrationTable
 * Description    : ��ӡ�궨��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintCalibrationTable( SMPL_NAME_TypeDef tureChannel )
{
	const uint8_t MIN_CALIRATION_SEGS = 2;	//��С�궨������
	const uint8_t MAX_CALIRATION_SEGS = 10;
	const uint8_t MAX_CNT = 20;
	const uint8_t MAX_DATA_BIT = 8;
	uint8_t i;
	uint8_t tab_num = 0;
	uint32_t value = 0,code = 0;
	char value_buff[MAX_CNT];
	tTime t;
	FH_UINT_TypeDef fhChannelUnit = GetFH_SmplUnit();
	WY_UINT_TypeDef wyChannelUnit = GetWY_SmplUnit();
	BX_UINT_TypeDef bxChannelUnit = GetBX_SmplUnit();
	
	ConfigPrinterDefaultSet();
	
	tab_num = smpl_tab_num_get(tureChannel);
	
	if ( (tab_num < MIN_CALIRATION_SEGS) || (tab_num > MAX_CALIRATION_SEGS + 1) )
	{
		PrintWordsAndLineFeed("�궨����������");
		PrintRowSpace(END_ROW_CNT);
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPrintErrorCue[0]);
		
		return ERROR;
	}
	
	tab_num--;		//ȥ��һ�����
	
	switch ( tureChannel )
	{
		case SMPL_FH_NUM:			
			PrintWordsAndLineFeed("     ����ͨ���궨��");
			break;
		
		case SMPL_WY_NUM:
			PrintWordsAndLineFeed("     λ��ͨ���궨��");
			break;
		
		case SMPL_BX_NUM:
			PrintWordsAndLineFeed("     ����ͨ���궨��");
			break;
		
		default:
			PrintWordsAndLineFeed("        ͨ������");
			break;
	}
	
	PrintDottedLine();		//- - - - -
	time_cycle();
	t = get_time();
	PrintTime( &t );
	PrintDottedLine();	
	
	for (i=1; i<=tab_num; ++i)		//�궨������
	{
		PrintWordsAndLineFeed(pPrint_Title[i-1]);
		
		print("ֵ��");
		value = (uint32_t)smpl_tab_value_get(tureChannel,i);
		
		AccordUnitConvValue(tureChannel,&value,(pFunctionDevide)UintDivisionTen);
		
		numtochar(MAX_DATA_BIT,value,value_buff);
		print(value_buff);
		
		PrintUnit(tureChannel);
		
		print("�룺");
		code = (uint32_t)smpl_tab_code_get(tureChannel,i);		
		numtochar(MAX_DATA_BIT,code,value_buff);
		PrintWordsAndLineFeed(value_buff);
	}
	
	PrintRowSpace(END_ROW_CNT);	//��ֹ����ӡ�����ݿ��ڴ�ӡ���ڲ�
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : PrintForceCalibrationResultTable
 * Description    : ��ӡУ׼���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintForceCalibrationResultTable( CALIBRATION_RSULT_PRINT_TypeDef *pCalibrationResult )
{	
	uint8_t i,j,index;
	const uint8_t COLUMN_CNT = 4;
	const uint8_t MAX_FORCE_BIT = 8;
	const uint8_t MAX_CALIRATION_SEGS = 10;	//�����س̵�
	int32_t buff_u = 0;
	float buff_f = 0.0f;	
	char print_buff[20];
	tTime t;
	uint8_t calibrationPointNums = pCalibrationResult->calibrationPointNums;
	FH_UINT_TypeDef fhChannelUnit = GetFH_SmplUnit();
	WY_UINT_TypeDef wyChannelUnit = GetWY_SmplUnit();
	BX_UINT_TypeDef bxChannelUnit = GetBX_SmplUnit();
	
	ConfigPrinterDefaultSet();	
	
	if ((calibrationPointNums==0) || (calibrationPointNums > MAX_CALIRATION_SEGS))
	{
		PrintWordsAndLineFeed("�Ѵ���������");
		PrintRowSpace(END_ROW_CNT);	
		
		return ERROR;
	}
	
	switch ( pCalibrationResult->channel )
	{
		case SMPL_FH_NUM:			
			PrintWordsAndLineFeed("    ����ͨ���궨���");
			break;
		
		case SMPL_WY_NUM:
			PrintWordsAndLineFeed("    λ��ͨ���궨���");
			break;
		
		case SMPL_BX_NUM:
			PrintWordsAndLineFeed("    ����ͨ���궨���");
			break;
		
		default:
			PrintWordsAndLineFeed("        ͨ������");
			break;
	}	
	
	PrintDottedLine();		//- - - - -
	time_cycle();
	t = get_time();
	PrintTime( &t );
	PrintDottedLine();
	
	for (i=0; i<calibrationPointNums; ++i)
	{
		PrintWordsAndLineFeed(pPrint_Title[i]);
		
		for (j=0; j<COLUMN_CNT; ++j)
		{
			index = j % COLUMN_CNT;
					
			switch ( index )
			{
				case 0:		//����
					print("���㣺");
					
					buff_u = pCalibrationResult->pCheckForce[i];
					
					AccordUnitConvValue(pCalibrationResult->channel,&buff_u,(pFunctionDevide)UintDivisionTen);
					
					numtochar(MAX_FORCE_BIT,buff_u,print_buff);
					print(print_buff);
					
					PrintUnit(pCalibrationResult->channel);					
					break;
					
				case 1:		//ʵ����ֵ
					print("ʵ��ֵ��");
				
					buff_f = pCalibrationResult->pRealForce[i];
				
					AccordUnitConvValue(pCalibrationResult->channel,&buff_f,(pFunctionDevide)FloatDivisionTen);

					floattochar(MAX_FORCE_BIT,2,buff_f,print_buff);
					print(print_buff);
					
					PrintUnit(pCalibrationResult->channel);		
					break;
				case 2:		//����
					print("���룺  ");
					buff_u = pCalibrationResult->pCode[i];
				
					numtochar(MAX_FORCE_BIT,buff_u,print_buff);
					PrintWordsAndLineFeed(print_buff);
					break;
					
				case 3:		//���
					print("��  ");
					
					buff_f = pCalibrationResult->pDeviation[i];
					floattochar(MAX_FORCE_BIT,2,buff_f,print_buff);
					print(print_buff);
					PrintWordsAndLineFeed("%");
					break;
			}
		}	
	}
	
	PrintRowSpace(END_ROW_CNT);	//��ֹ����ӡ�����ݿ��ڴ�ӡ���ڲ�
		
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : PrintTestReport
 * Description    : ��ӡ���鱨��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintTestReport( TEST_TYPE_TypeDef test_type, TEST_ATTRIBUTE_TypeDef testAttribute, \
				const REPORT_TypeDef *report, const TEST_INFO_TypeDef *test_info )
{
	tTime *t = NULL;
	char print_buff[20];
	const uint8_t MAX_SHOW_BIT = 9;
	uint8_t i,tab_num;
	const uint8_t MAX_TEST_NUM = 20;
	const uint8_t BIT_FORCE_DOT = 2;			//��ֵС��λ
	const uint8_t BIT_COF_DOT = 2;				//����ϵ��С��λ
	const uint8_t BIT_LENTH_DOT = 2;			//����С��λ
	const uint8_t BIT_AREA_DOT = 2;				//���С��λ
	const uint8_t BIT_AVAIL_STRENGTH_DOT = 1;	//��Чǿ��С��λ
	float force_buff = 0;
	FH_UINT_TypeDef fhChannelUnit = GetFH_SmplUnit();
	WY_UINT_TypeDef wyChannelUnit = GetWY_SmplUnit();
	BX_UINT_TypeDef bxChannelUnit = GetBX_SmplUnit();
	
	ConfigPrinterDefaultSet();
	
	switch ( test_type )
	{
		case NONE_TEST:
			PrintWordsAndLineFeed("    �޵�ǰ�������ͣ�");
			PrintRowSpace(END_ROW_CNT);
		
			return ERROR;

		case KYSNJS:
			PrintWordsAndLineFeed("  ˮ�ེɰ��ѹ���鱨��");
			break;
		
		case KYJZSJ:
			PrintWordsAndLineFeed("  ����ɰ����ѹ���鱨��");
			break;
		
		case KYHNT:
			PrintWordsAndLineFeed("   ��������ѹ���鱨��");
			break;
		
		case KZHNT:
			PrintWordsAndLineFeed("   �������������鱨��");
			break;
		
		case KYQQZ:
			PrintWordsAndLineFeed("   ��ǽש��ѹ���鱨��");
			break;
		
		case KYZJDH:
			PrintWordsAndLineFeed("   �𼶶������鱨��");
			break;
		
		case KYTY:
			PrintWordsAndLineFeed("    ͨ�ÿ�ѹ���鱨��");		
			break;
		
		case KZSNJS:
			PrintWordsAndLineFeed("  ˮ�ེɰ�������鱨��");
			break;
		
		case KZYJSNJ:
			PrintWordsAndLineFeed(" ѹ��ˮ�ཬ�������鱨��");
			break;
		
		case KZTY:
			PrintWordsAndLineFeed("    ͨ�ÿ������鱨��");	
			break;
		
		case KLJSSW:
			PrintWordsAndLineFeed(" ���������������鱨��");	
			break;
		
		default:
			PrintWordsAndLineFeed("     �������ͳ���");
			PrintRowSpace(END_ROW_CNT);
			
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pPrintErrorCue[1]);
		
			return ERROR;
	}
	
	PrintDottedLine();		//- - - - -
	t = (tTime *)( &(test_info->time) );
	PrintTime( t );	
	PrintDottedLine();
	
	switch ( test_type )
	{
		case NONE_TEST:
			break;

		case KYSNJS:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("Ʒ�֣�");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("ǿ�ȣ�");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("���ڣ�");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("���");
			print(report->sample_spec);
			PrintWordsAndLineFeed("mm2");
			
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);			
			break;
		
		case KYJZSJ:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("Ʒ�֣�");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("���ڣ�");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("���");	
			print(report->sample_spec);
			PrintWordsAndLineFeed("mm3");
		
			print("����ϵ����");	
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
			
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KYHNT:
		case KZHNT:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("ǿ�ȣ�");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("���ڣ�");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("���");		//��λ������ӣ���Щ��mm3,��Щ���Ǵ˵�λ
			PrintWordsAndLineFeed(report->sample_spec);
			
			print("����ϵ����");	
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
		
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;				
		
		case KYQQZ:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("Ʒ�֣�");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("���ȣ�");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->length,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm");
		
			print("��ȣ�");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->width,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
		
			print("�߶ȣ�");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->high,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
			
		case KYZJDH:
			PrintWordsAndLineFeed("���Ҳ�֧���𼶶������飡");	
			break;
		
		case KYTY:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("��״��");
			switch (report->sample_shape_index)
			{
				case TYKY_SHAPE_RECTANGLE:
					PrintWordsAndLineFeed(pSpecimen_sharp[0]);
					break;
				case TYKY_SHAPE_ROUND:
					PrintWordsAndLineFeed(pSpecimen_sharp[1]);
					break;
				case TYKY_SHAPE_IRREGULAR:
					PrintWordsAndLineFeed(pSpecimen_sharp[2]);
					break;
			}			
		
			{
				float area = 0;
				
				switch (report->sample_shape_index)
				{
					case TYKY_SHAPE_RECTANGLE:
						print("���ȣ�");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->length,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						print("��ȣ�");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->width,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						area = report->gz_area;
						break;
					case TYKY_SHAPE_ROUND:
						print("Բ��ֱ����");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->yx_diameter,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						area = report->gz_area;
						break;
					case TYKY_SHAPE_IRREGULAR:
						area = report->bgz_area;
						break;
				}
				print("�����");			
				floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,area,print_buff);			
				print(print_buff);
				PrintWordsAndLineFeed("mm2");
			}
		
			print("����ϵ����");
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
			
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KZSNJS:
		case KZYJSNJ:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("Ʒ�֣�");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("ǿ�ȣ�");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("���ڣ�");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
		
			print("�߳���");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->length,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("��ࣺ");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->sample_span,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("�Լ�������");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KZTY:
			PrintWordsAndLineFeed("���Ҳ�֧��ͨ�ÿ������飡");		
			
			return ERROR;	
		
		case KLJSSW:
			print("��׼��");
			PrintWordsAndLineFeed(report->test_standard);

			print("��ţ�");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("�Լ���״��");
			switch (report->sample_shape_index)
			{
				case JSSWKL_SHAPE_RECTANGLE:
					PrintWordsAndLineFeed(pSpecimenSharp_KLJSSW[0]);
					break;
				case JSSWKL_SHAPE_ROUND:
					PrintWordsAndLineFeed(pSpecimenSharp_KLJSSW[1]);
					break;
				case JSSWKL_SHAPE_TUBE:
					PrintWordsAndLineFeed(pSpecimenSharp_KLJSSW[2]);
					break;
				case JSSWKL_SHAPE_IRREGULAR:
					PrintWordsAndLineFeed(pSpecimenSharp_KLJSSW[3]);
					break;
			}

			{
				float area = 0;
				
				switch (report->sample_shape_index)
				{
					case JSSWKL_SHAPE_RECTANGLE:
						print("���κ�ȣ�");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->length,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						print("���ο�ȣ�");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->width,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						area = report->length * report->width;
						break;
					case JSSWKL_SHAPE_ROUND:
						print("Բ��ֱ����");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->yx_diameter,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						area = GetCircularArea(report->yx_diameter);
						break;
					case JSSWKL_SHAPE_TUBE:
						print("�ܶκ�ȣ�");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->pipeThickness,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						print("�ܶ��⾶��");			
						floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->pipeOuterDiameter,print_buff);			
						print(print_buff);
						PrintWordsAndLineFeed("mm");
					
						area = CountPipeArea(report->pipeOuterDiameter,report->pipeThickness);
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						area = report->bgz_area;
						break;
				}
				
				print("ԭʼ�������");			
				floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,area,print_buff);			
				print(print_buff);
				PrintWordsAndLineFeed("mm2");
			}
			
			print("����Ʊ�ࣺ");			
			floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->extensometerGauge,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("ԭʼ��ࣺ");			
			floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->originalGauge,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("ƽ�г��ȣ�");			
			floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->parallelLenth,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("�Լ�������");			
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("��");
			break;

		default:
			break;
	}
	
	print_enter();
	PrintWordsAndLineFeed("������ݣ�");
	
	tab_num = report->sample_serial;
	if ( (tab_num<1) || (tab_num>MAX_TEST_NUM) )
	{
		PrintWordsAndLineFeed("�Կ��������1~20��Χ��");
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPrintErrorCue[2]);
		
		return ERROR;
	}
	
	switch ( testAttribute )
	{
		case COMPRESSION_TEST:
		case BENDING_TEST:
			for (i=0; i<tab_num; ++i)	//������ֵ��ǿ��
			{
				PrintWordsAndLineFeed(pPrint_Title[i]);
				
				print("��ֵ��");
				force_buff = report->force[i];
				AccordUnitConvValue(SMPL_FH_NUM,&force_buff,(pFunctionDevide)FloatDivisionTen);
				floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force_buff,print_buff);
				print(print_buff);	
				PrintUnit(SMPL_FH_NUM);
				
				print("ǿ�ȣ�");
				floattochar(MAX_SHOW_BIT,BIT_AVAIL_STRENGTH_DOT,report->strength[i],print_buff);
				print(print_buff);	
				PrintWordsAndLineFeed("MPa");
			}
			
			print_enter();
			print("��Ч��ֵ��");
			force_buff = report->force_valid[0];
			
			if (report->force_valid[0] < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{		
				PrintWordsAndLineFeed("�� Ч");
			}
			else
			{	
				AccordUnitConvValue(SMPL_FH_NUM,&force_buff,(pFunctionDevide)FloatDivisionTen);
				floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force_buff,print_buff);
				print(print_buff);
							
				PrintUnit(SMPL_FH_NUM);
			}
			
			print("��Чǿ�ȣ�");
			force_buff = report->strength_valid[0];
			
			if (report->result_valid == 0)
			{
				PrintWordsAndLineFeed("�� Ч");
			}
			else
			{
				floattochar(MAX_SHOW_BIT,1,force_buff,print_buff);
				print(print_buff);
				PrintWordsAndLineFeed("MPa");
			}
			break;
		case STRETCH_TEST:
			for (i=0; i<tab_num; ++i)
			{			
				PrintWordsAndLineFeed(pPrint_Title[i]);
				
				{
					float force = report->maxForce[i];
					
					print("�������");
					
					AccordUnitConvValue(SMPL_FH_NUM,&force,(pFunctionDevide)FloatDivisionTen);
					
					floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force,print_buff);
					print(print_buff);
					
					PrintUnit(SMPL_FH_NUM);
				}
				
				{
					float strength = report->maxStrength[i];
					
					print("����ǿ�ȣ�");					
					
					floattochar(MAX_SHOW_BIT,1,strength,print_buff);
					print(print_buff);
					
					PrintWordsAndLineFeed("MPa");
				}
				
				{
					float force = report->upYieldForce[i];
					
					print("��������");
					
					AccordUnitConvValue(SMPL_FH_NUM,&force,(pFunctionDevide)FloatDivisionTen);
					
					floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force,print_buff);
					print(print_buff);
					
					PrintUnit(SMPL_FH_NUM);
				}
				
				{
					float force = report->downYieldForce[i];
					
					print("��������");
					
					AccordUnitConvValue(SMPL_FH_NUM,&force,(pFunctionDevide)FloatDivisionTen);
					
					floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force,print_buff);
					print(print_buff);
					
					PrintUnit(SMPL_FH_NUM);
				}
				
				{
					float strength = report->upYieldStrength[i];
					
					print("������ǿ�ȣ�");				
					
					floattochar(MAX_SHOW_BIT,1,strength,print_buff);
					print(print_buff);
					
					PrintWordsAndLineFeed("MPa");
				}
				
				{
					float strength = report->downYieldStrength[i];
					
					print("������ǿ�ȣ�");				
					
					floattochar(MAX_SHOW_BIT,1,strength,print_buff);
					print(print_buff);
					
					PrintWordsAndLineFeed("MPa");
				}
				
				{
					float tempf = report->maxForceSumExtend[i];
					
					print("����������죺");				
					
					floattochar(MAX_SHOW_BIT,2,tempf,print_buff);
					print(print_buff);
					
					PrintWordsAndLineFeed("mm");
				}
				
				{
					float tempf = report->maxForceSumElongation[i];
					
					print("��������쳤�ʣ�");				
					
					floattochar(MAX_SHOW_BIT,1,tempf,print_buff);
					print(print_buff);
					
					PrintWordsAndLineFeed("%");
				}		
			}
			break;
			
		case INVALID_TEST:
			break;
		default:
			break;
	}
	
	PrintRowSpace(END_ROW_CNT);	//��ֹ����ӡ�����ݿ��ڴ�ӡ���ڲ�
	
	return SUCCESS;
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
