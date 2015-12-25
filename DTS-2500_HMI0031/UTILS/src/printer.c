/**
  ******************************************************************************
  * @file    printer.c
  * @author  SY
  * @version V3.5.0
  * @date    2015-5-15 10:37:14
  * @brief   打印文件
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
#define PRINT_DELAY		20			//向打印机发送数据延时
#define END_ROW_CNT		3			//打印结束空余行


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
	"标定表项数出错！",			//0
	"试验类型出错！",			//1
	"试块个数不在1~20范围！",	//2
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*------------------------------------------------------------
 * Function Name  : ConfigPrinterDefaultSet
 * Description    : 配置打印机默认设置
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
 * Description    : 配置字符同时换行
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
 * Description    : 打印虚线
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
 * Description    : 打印时间
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void PrintTime( tTime *t )
{
	char buff[12];
	
	print("日期：");
	usprintf(buff,"%04d/%02d/%02d", t->usYear,t->ucMon,t->ucMday);
	PrintWordsAndLineFeed(buff);
	
	print("时间：");
	usprintf(buff,"%02d:%02d:%02d", t->ucHour,t->ucMin,t->ucSec);	
	PrintWordsAndLineFeed(buff);
}

/*------------------------------------------------------------
 * Function Name  : PrintRowSpace
 * Description    : 打印空格
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
 * Description    : 打印标定表
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintCalibrationTable( SMPL_NAME_TypeDef2 tureChannel, SMPL_NAME_TypeDef2 showChannel )
{
	const uint8_t MIN_CALIRATION_SEGS = 2;	//最小标定表项数
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
		PrintWordsAndLineFeed("标定表项数出错！");
		PrintRowSpace(END_ROW_CNT);
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPrintErrorCue[0]);
		
		return ERROR;
	}
	
	tab_num--;		//去掉一个零点
	
	switch ( tureChannel )
	{
		case SMPL_KY_NUM:			
			PrintWordsAndLineFeed("     抗压通道标定表");
			break;
		
		case SMPL_KZ_NUM:
			PrintWordsAndLineFeed("     抗折通道标定表");
			break;
		
		default:
			PrintWordsAndLineFeed("        通道错误！");
			break;
	}
	
	PrintDottedLine();		//- - - - -
	time_cycle();
	t = get_time();
	PrintTime( &t );
	PrintDottedLine();	
	
	for (i=1; i<=tab_num; ++i)		//标定表数据
	{
		PrintWordsAndLineFeed(pPrint_Title[i-1]);
		
		print("值：");
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
		
		print("码：");
		force_code = (uint32_t)smpl_tab_code_get(tureChannel,i);		
		numtochar(MAX_DATA_BIT,force_code,value_buff);
		PrintWordsAndLineFeed(value_buff);
		
		bsp_DelayMS(PRINT_DELAY);
	}
	
	PrintRowSpace(END_ROW_CNT);	//防止被打印的数据卡在打印机内部
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : PrintForceCalibrationResultTable
 * Description    : 打印校准结果
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus PrintForceCalibrationResultTable( CALIBRATION_RSULT_PRINT_TypeDef *pCalibrationResult )
{	
	uint8_t i,j,index;
	const uint8_t COLUMN_CNT = 4;
	const uint8_t MAX_FORCE_BIT = 8;
	const uint8_t MAX_CALIRATION_SEGS = 10;	//包含回程点
	int32_t buff_u = 0;
	float buff_f = 0.0f;	
	char print_buff[20];
	tTime t;
	uint8_t calibrationPointNums = pCalibrationResult->calibrationPointNums;
	
	ConfigPrinterDefaultSet();	
	
	if ((calibrationPointNums==0) || (calibrationPointNums > MAX_CALIRATION_SEGS))
	{
		PrintWordsAndLineFeed("已打点个数出错！");
		PrintRowSpace(END_ROW_CNT);	
		
		return ERROR;
	}
	
	switch ( pCalibrationResult->tureChannel )
	{
		case SMPL_KY_NUM:			
			PrintWordsAndLineFeed("    抗压通道标定结果");
			break;
		
		case SMPL_KZ_NUM:
			PrintWordsAndLineFeed("    抗折通道标定结果");
			break;
		
		default:
			PrintWordsAndLineFeed("        通道错误！");
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
				case 0:		//检测点
					print("检测点：");
					
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
					
				case 1:		//实测力值
					print("实测值：");
				
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
				case 2:		//力码
					print("力码：  ");
					buff_u = pCalibrationResult->pCode[i];
				
					numtochar(MAX_FORCE_BIT,buff_u,print_buff);
					PrintWordsAndLineFeed(print_buff);
					break;
					
				case 3:		//误差
					print("误差：  ");
					
					buff_f = pCalibrationResult->pDeviation[i];
					floattochar(MAX_FORCE_BIT,2,buff_f,print_buff);
					print(print_buff);
					PrintWordsAndLineFeed("%");
					break;
			}
			
			bsp_DelayMS(PRINT_DELAY);		//必须加延时，否则数据发送太快导致丢失
		}	
	}
	
	PrintRowSpace(END_ROW_CNT);	//防止被打印的数据卡在打印机内部
		
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : PrintTestReport
 * Description    : 打印试验报告
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
	const uint8_t BIT_FORCE_DOT = 2;			//力值小数位
	const uint8_t BIT_COF_DOT = 2;				//修正系数小数位
	const uint8_t BIT_LENTH_DOT = 1;			//长度小数位
	const uint8_t BIT_AREA_DOT = 2;				//面积小数位
	const uint8_t BIT_AVAIL_STRENGTH_DOT = 1;	//有效强度小数位
	float force_buff = 0;
	
	ConfigPrinterDefaultSet();
	
	switch ( test_type )
	{
		case NONE_TEST:
			PrintWordsAndLineFeed("    无当前试验类型！");
			PrintRowSpace(END_ROW_CNT);
		
			return ERROR;

		case KYSNJS:
			PrintWordsAndLineFeed("  水泥胶砂抗压试验报告");
			break;
		
		case KYJZSJ:
			PrintWordsAndLineFeed("  建筑砂浆抗压试验报告");
			break;
		
		case KYHNT:
			PrintWordsAndLineFeed("   混凝土抗压试验报告");
			break;
		
		case KZHNT:
			PrintWordsAndLineFeed("   混凝土抗折试验报告");
			break;
		
		case KYQQZ:
			PrintWordsAndLineFeed("   砌墙砖抗压试验报告");
			break;
		
		case KYZJDH:
			PrintWordsAndLineFeed("   逐级定荷试验报告");
			break;
		
		case KYTY:
			PrintWordsAndLineFeed("    通用抗压试验报告");		
			break;
		
		case KZSNJS:
			PrintWordsAndLineFeed("  水泥胶砂抗折试验报告");
			break;
		
		case KZYJSNJ:
			PrintWordsAndLineFeed(" 压浆水泥浆抗折试验报告");
			break;
		
		case KZTY:
			PrintWordsAndLineFeed("    通用抗折试验报告");	
			break;
		
		case KLJSSW:
			PrintWordsAndLineFeed(" 金属室温拉伸试验报告");	
			break;
		
		default:
			PrintWordsAndLineFeed("     试验类型出错！");
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
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("品种：");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("强度：");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("龄期：");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("规格：");
			print(report->sample_spec);
			PrintWordsAndLineFeed("mm2");
			
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);			
			break;
		
		case KYJZSJ:
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("品种：");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("龄期：");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("规格：");	
			print(report->sample_spec);
			PrintWordsAndLineFeed("mm3");
		
			print("修正系数：");	
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
			
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KYHNT:
		case KZHNT:
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("强度：");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("龄期：");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
			
			print("规格：");		//单位不能添加，有些是mm3,有些不是此单位
			PrintWordsAndLineFeed(report->sample_spec);
			
			print("修正系数：");	
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
		
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;				
		
		case KYQQZ:
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("品种：");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("长度：");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->length,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm");
		
			print("宽度：");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->width,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
		
			print("高度：");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->high,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
			
		case KYZJDH:
			PrintWordsAndLineFeed("暂且不支持逐级定荷试验！");	
			break;
		
		case KYTY:
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("形状：");
			PrintWordsAndLineFeed(report->sample_shape);
		
			print("面积：");
			floattochar(MAX_SHOW_BIT,BIT_AREA_DOT,report->area,print_buff);			
			print(print_buff);
			PrintWordsAndLineFeed("mm2");
		
			print("修正系数：");
			floattochar(MAX_SHOW_BIT,BIT_COF_DOT,report->correct_cof,print_buff);
			PrintWordsAndLineFeed(print_buff);
			
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KZSNJS:
		case KZYJSNJ:
			print("标准：");
			PrintWordsAndLineFeed(report->test_standard);
		
			print("编号：");
			PrintWordsAndLineFeed(test_info->fname);
		
			print("品种：");
			PrintWordsAndLineFeed(report->sample_type);
		
			print("强度：");
			PrintWordsAndLineFeed(report->strength_grade);
		
			print("龄期：");
			numtochar(MAX_SHOW_BIT,report->sample_age,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("d");
		
			print("边长：");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->length,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("跨距：");
			floattochar(MAX_SHOW_BIT,BIT_LENTH_DOT,report->sample_span,print_buff);
			print(print_buff);
			PrintWordsAndLineFeed("mm");
			
			print("试件块数：");
			numtochar(MAX_SHOW_BIT,report->sample_num,print_buff);
			PrintWordsAndLineFeed(print_buff);
			break;
		
		case KZTY:
			PrintWordsAndLineFeed("暂且不支持通用抗折试验！");		
			
			return ERROR;	
		
		case KLJSSW:
			PrintWordsAndLineFeed("暂且不支持通用抗折试验！");		
			
			return ERROR;

		default:
			break;
	}
	
	print_enter();
	PrintWordsAndLineFeed("结果数据：");
	
	tab_num = report->sample_serial;
	if ( (tab_num<1) || (tab_num>MAX_TEST_NUM) )
	{
		PrintWordsAndLineFeed("试块个数不在1~20范围！");
		
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPrintErrorCue[2]);
		
		return ERROR;
	}
	
	for (i=0; i<tab_num; ++i)	//加载力值、强度
	{
		PrintWordsAndLineFeed(pPrint_Title[i]);
		
		print("力值：");
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
		
		print("强度：");
		floattochar(MAX_SHOW_BIT,BIT_AVAIL_STRENGTH_DOT,report->strength[i],print_buff);
		print(print_buff);	
		PrintWordsAndLineFeed("MPa");
		
		bsp_DelayMS(PRINT_DELAY);		//必须加延时，否则数据发送太快导致丢失
	}
	
	print_enter();
	print("有效力值：");
	force_buff = report->force_valid[0];
	
	if (report->force_valid[0] < MIN_FLOAT_PRECISION_DIFF_VALUE)
	{		
		PrintWordsAndLineFeed("无 效");
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
	
	print("有效强度：");
	force_buff = report->strength_valid[0];
	
	if (report->result_valid == 0)
	{
		PrintWordsAndLineFeed("无 效");
	}
	else
	{
		floattochar(MAX_SHOW_BIT,1,force_buff,print_buff);
		print(print_buff);
		PrintWordsAndLineFeed("MPa");
	}
	
	PrintRowSpace(END_ROW_CNT);	//防止被打印的数据卡在打印机内部
	
	return SUCCESS;
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
