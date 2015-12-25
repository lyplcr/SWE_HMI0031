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
#define PRINT_DELAY		20			//���ӡ������������ʱ
#define END_ROW_CNT		3			//��ӡ����������


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
 * Function Name  : PrintCalibrationTable
 * Description    : ��ӡ�궨��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintCalibrationTable( SMPL_NAME_TypeDef2 tureChannel, SMPL_NAME_TypeDef2 showChannel )
{
	const uint8_t MIN_CALIRATION_SEGS = 2;	//��С�궨������
	const uint8_t MAX_CALIRATION_SEGS = 10;
	const uint8_t MAX_CNT = 20;
	const uint8_t MAX_DATA_BIT = 8;
	uint8_t i;
	uint8_t tab_num = 0;
	uint32_t force_value = 0,force_code = 0;
	char value_buff[MAX_CNT];
	tTime t;
	
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
		case SMPL_KY_NUM:			
			PrintWordsAndLineFeed("     ��ѹͨ���궨��");
			break;
		
		case SMPL_KZ_NUM:
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
		force_value = (uint32_t)smpl_tab_value_get(tureChannel,i);
				
		if (showChannel == SMPL_KY_NUM)
		{
			force_value /= 1000;
		}
		
		numtochar(MAX_DATA_BIT,force_value,value_buff);
		print(value_buff);
		if (showChannel == SMPL_KY_NUM)
		{
			PrintWordsAndLineFeed("kN");
		}
		else
		{
			PrintWordsAndLineFeed("N");
		}
		
		print("�룺");
		force_code = (uint32_t)smpl_tab_code_get(tureChannel,i);		
		numtochar(MAX_DATA_BIT,force_code,value_buff);
		PrintWordsAndLineFeed(value_buff);
		
		bsp_DelayMS(PRINT_DELAY);
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
	
	ConfigPrinterDefaultSet();	
	
	if ((calibrationPointNums==0) || (calibrationPointNums > MAX_CALIRATION_SEGS))
	{
		PrintWordsAndLineFeed("�Ѵ���������");
		PrintRowSpace(END_ROW_CNT);	
		
		return ERROR;
	}
	
	switch ( pCalibrationResult->tureChannel )
	{
		case SMPL_KY_NUM:			
			PrintWordsAndLineFeed("    ��ѹͨ���궨���");
			break;
		
		case SMPL_KZ_NUM:
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
					if (pCalibrationResult->showChannel == SMPL_KY_NUM)
					{
						buff_u /= 1000;
					}
					numtochar(MAX_FORCE_BIT,buff_u,print_buff);
					print(print_buff);
					if (pCalibrationResult->showChannel == SMPL_KY_NUM)
					{
						PrintWordsAndLineFeed("kN");
					}
					else
					{
						PrintWordsAndLineFeed("N");
					}					
					break;
					
				case 1:		//ʵ����ֵ
					print("ʵ��ֵ��");
				
					buff_f = pCalibrationResult->pRealForce[i];
					if (pCalibrationResult->showChannel == SMPL_KY_NUM)
					{
						buff_f /= 1000;
					}
					floattochar(MAX_FORCE_BIT,2,buff_f,print_buff);
					print(print_buff);
					if (pCalibrationResult->showChannel == SMPL_KY_NUM)
					{
						PrintWordsAndLineFeed("kN");
					}
					else
					{
						PrintWordsAndLineFeed("N");
					}
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
			
			bsp_DelayMS(PRINT_DELAY);		//�������ʱ���������ݷ���̫�쵼�¶�ʧ
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
ErrorStatus PrintTestReport( SMPL_NAME_TypeDef2 showChannel, TEST_TYPE_TypeDef test_type, \
							 const REPORT_TypeDef *report, const TEST_INFO_TypeDef *test_info )
{
	tTime *t = NULL;
	char print_buff[20];
	const uint8_t MAX_SHOW_BIT = 9;
	uint8_t i,tab_num;
	const uint8_t MAX_TEST_NUM = 20;
	const uint8_t BIT_FORCE_DOT = 2;			//��ֵС��λ
	const uint8_t BIT_COF_DOT = 2;				//����ϵ��С��λ
	const uint8_t BIT_LENTH_DOT = 1;			//����С��λ
	const uint8_t BIT_AREA_DOT = 2;				//���С��λ
	const uint8_t BIT_AVAIL_STRENGTH_DOT = 1;	//��Чǿ��С��λ
	float force_buff = 0;
	
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
			PrintWordsAndLineFeed(report->sample_shape);
		
			print("�����");
			floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->area,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm2");
		
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
			PrintWordsAndLineFeed("���Ҳ�֧��ͨ�ÿ������飡");		
			
			return ERROR;

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
	
	for (i=0; i<tab_num; ++i)	//������ֵ��ǿ��
	{
		PrintWordsAndLineFeed(pPrint_Title[i]);
		
		print("��ֵ��");
		force_buff = report->force[i];
		if (showChannel == SMPL_KY_NUM)
		{
			force_buff /= 1000;
		}
		floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force_buff,print_buff);
		print(print_buff);	
		if (showChannel == SMPL_KY_NUM)
		{
			PrintWordsAndLineFeed("kN");
		}
		else
		{
			PrintWordsAndLineFeed("N");
		}
		
		print("ǿ�ȣ�");
		floattochar(MAX_SHOW_BIT,BIT_AVAIL_STRENGTH_DOT,report->strength[i],print_buff);
		print(print_buff);	
		PrintWordsAndLineFeed("MPa");
		
		bsp_DelayMS(PRINT_DELAY);		//�������ʱ���������ݷ���̫�쵼�¶�ʧ
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
		if (showChannel == SMPL_KY_NUM)
		{
			 force_buff /= 1000;
		}
		floattochar(MAX_SHOW_BIT,BIT_FORCE_DOT,force_buff,print_buff);
		print(print_buff);
					
		if (showChannel == SMPL_KY_NUM)
		{
			PrintWordsAndLineFeed("kN");
		}
		else
		{
			PrintWordsAndLineFeed("N");
		}
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
	
	PrintRowSpace(END_ROW_CNT);	//��ֹ����ӡ�����ݿ��ڴ�ӡ���ڲ�
	
	return SUCCESS;
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
