/**
  ******************************************************************************
  * @file    tools.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   工具库
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "tools.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void RemoveStrChar( char *str, const char c );


/* Private functions ---------------------------------------------------------*/


/*------------------------------------------------------------
 * Function Name  : CharToChar
 * Description    : 填充空格
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CharToChar( uint8_t num, char *target, const char *source )
{
	uint8_t i = 0;
	uint8_t len = strlen(source);
	
	strcpy(target,source);
	
	if (len >= num)
	{
		return;
	}
	
	for (i=len; i<num; ++i)
	{
		target[i] = ' ';
	}
	
	target[num] = 0;
}

/*------------------------------------------------------------
 * Function Name  : RemoveStrChar
 * Description    : 移除字符串中指定的字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RemoveStrChar( char *str, const char c )
{
	uint8_t i = 0,pos = 0;
	uint8_t len = strlen(str);
	
	while ( str[i++] == c)
	{
		pos++;
	}
	
	if ( pos )
	{
		memmove(str,&str[pos],len+1-pos);
	}
}

/*------------------------------------------------------------
 * Function Name  : floattochar
 * Description    : 浮点数转换为字符串
 * Input          : max_bit：总位数(包含小数位)
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void floattochar( uint8_t max_bit,uint8_t point_n,float num,char *str )
{
	const uint8_t ALLOW_CONV_MAX_CHAR_LENTH = 50;
	char strBuff[ALLOW_CONV_MAX_CHAR_LENTH + 1] = {0};
	uint8_t intBit = 0;
	
	if (max_bit > ALLOW_CONV_MAX_CHAR_LENTH)
	{
		strcpy(str,"error");
		
		return;
	}
	
	if (max_bit <= point_n)
	{
		point_n = 0;
	}
	
	intBit = max_bit - point_n;

	float2str(num,strBuff,intBit,point_n);
	
	RemoveStrChar(strBuff,' ');
	
	strcpy(str,strBuff);
}

/*------------------------------------------------------------
 * Function Name  : FindPutcharIntIllegar
 * Description    : 查找输入字符中的非法字符（'-'、'.'）
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FlagStatus FindPutcharIntIllegar( uint8_t *pCnt, char *p )
{
	uint8_t i=0;

	while (i < *pCnt)		  	//禁止输入-或.
	{
		if ((p[i]=='-') || (p[i]=='.'))	   //第一位为非法字符
		{
			if (i == 0)
			{
				p[0] = '0';
				p[1] = '\0';
				*pCnt = 1;
			}
			else
			{
				p[i] = '\0';
				*pCnt = i;	
			}

			return SET;
		 }
	
		i++;
	}
	
	return RESET;
}


/*------------------------------------------------------------
 * Function Name  : numtochar
 * Description    : 数字转换为字符串
 * Input          : max_bit：总位数
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void numtochar(uint8_t maxBit,int32_t num,char *c)
{	
	const uint8_t ALLOW_CONV_MAX_CHAR_LENTH = 30;
	char s[ALLOW_CONV_MAX_CHAR_LENTH+1];
	
	if (maxBit > ALLOW_CONV_MAX_CHAR_LENTH)
	{
		strcpy(c,"error");
		
		return;
	}
		
	switch (maxBit)
	{
		case 1:
		   	usprintf(s,"%1d", num);
			break;
	
		case 2:
			usprintf(s,"%2d", num);
			break;
	
		case 3:
			usprintf(s,"%3d", num);
			break;
		
		case 4:
		   	usprintf(s,"%4d", num);
			break;
	
		case 5:
			usprintf(s,"%5d", num);
			break;
	
		case 6:
			usprintf(s,"%6d", num);
			break;
		
		case 7:
		   	usprintf(s,"%7d", num);
			break;
	
		case 8:
			usprintf(s,"%8d", num);
			break;
	
		case 9:
			usprintf(s,"%9d", num);
			break;
		
		case 10:
		   	usprintf(s,"%10d", num);
			break;
	
		case 11:
			usprintf(s,"%11d", num);
			break;
	
		case 12:
			usprintf(s,"%12d", num);
			break;
		
		case 13:
		   	usprintf(s,"%13d", num);
			break;
	
		case 14:
			usprintf(s,"%14d", num);
			break;
	
		case 15:
			usprintf(s,"%15d", num);
			break;
		
		case 16:
			usprintf(s,"%16d", num);
			break;	
	
	 	default:
			break;
	}
		
	RemoveStrChar(s,' ');
	
	if (strlen(s) > maxBit)
	{
		strcpy(c,"error");
	}
	else
	{
		strcpy(c,s);
	}
}
 
/*------------------------------------------------------------
 * Function Name  : KeyIndexManage
 * Description    : 按键索引值管理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void KeyIndexManage( INDEX_MANAGE_TypeDef *pIndexObj )
{
	uint8_t tempRow = 0,tempCol = 0,maxRow = 0;
		
	if (IsPressKey() == YES)	  			
	{		
		if ( pIndexObj->rowNum )
		{	
			tempRow = (*pIndexObj->pNowIndex) % pIndexObj->rowNum + 1;				//当前所处的行号(从1开始)
			tempCol = (*pIndexObj->pNowIndex) / pIndexObj->rowNum + 1;				//当前所处的列号(从1开始)
		}
		else
		{
			return;
		}
		
		if (pIndexObj->enableMoveIndex == ENABLE)	 				//使能方向键
		{
			switch ( GetKeyVal() )
			{
				case KEY_UP: 										
					if (tempRow == 1)					//当前所处的位置：首行
					{
						if (tempCol == pIndexObj->colNum)			//处于最后一列
						{
							*pIndexObj->pNowIndex = pIndexObj->sumNum - 1;
						}
						else							//处于其他列
						{
							*pIndexObj->pNowIndex += pIndexObj->rowNum - 1;	
						}
					}
					else								//其他行
					{		
						(*pIndexObj->pNowIndex)--;
					}
					break;
			
				case KEY_DOWN:				
					if((*pIndexObj->pNowIndex) == pIndexObj->sumNum - 1)					//当前所处的位置：最后一个元素，不一定是尾行
					{
						(*pIndexObj->pNowIndex) -= tempRow - 1;
					}
					else if (tempRow == pIndexObj->rowNum)				//当前所处的位置：尾行
					{
						*pIndexObj->pNowIndex -= pIndexObj->rowNum-1;
					}
					else									//其他行
					{
						(*pIndexObj->pNowIndex)++;	
					}
					break;
				
				case KEY_LEFT:
					if (tempCol == 1)						//首列
					{ 
						*pIndexObj->pNowIndex += pIndexObj->rowNum * (pIndexObj->colNum-1);
						if (*pIndexObj->pNowIndex > pIndexObj->sumNum-1)				   //超出范围
						{
							maxRow = (pIndexObj->sumNum-1) % pIndexObj->rowNum +1;	//计算最大值所在的行号
							*pIndexObj->pNowIndex = pIndexObj->sumNum - maxRow;
						}	
					}
					else									//其他列
					{
						*pIndexObj->pNowIndex -= pIndexObj->rowNum;	
					}
					break;
				
				case KEY_RIGHT:	
					if (tempCol == pIndexObj->colNum)					//处于尾列
					{
						*pIndexObj->pNowIndex = tempRow - 1;	
					}
					else								   //其他列
					{
						*pIndexObj->pNowIndex += pIndexObj->rowNum;
						if (*pIndexObj->pNowIndex > pIndexObj->sumNum-1)	 		//超出范围
						{
							maxRow = (pIndexObj->sumNum-1) % pIndexObj->rowNum +1;	//计算最大值所在的行号
							*pIndexObj->pNowIndex = pIndexObj->sumNum - maxRow;
						}
					}
					break;
				
				default:
					break;
			}
		}
		else
		{
		   ;
		}
	}	
}

/*------------------------------------------------------------
 * Function Name  : GetSampleSpecificationArea
 * Description    : 从试件规格中获取试块面积
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetSampleSpecificationArea( const char *pVal, float *pNum )
{
	uint8_t cnt = 0,i = 0,flag = 0;
	char buff[MAX_PUTIN_DATA_BIT+1];
	float temp[MAX_PUTIN_DATA_BIT+1];

	while (pVal[cnt] != '\0')
	{
		if ( ((pVal[cnt]>='0') && (pVal[cnt]<='9')) || (pVal[cnt]=='.') )
		{
			buff[i] = pVal[cnt];
			i++;	
			buff[i] = '\0';
		}
		else if (pVal[cnt] == '*')
		{
			temp[flag] = str2float(buff);
			pNum[flag] = temp[flag];
			flag++;	
			i = 0;
		}
		else if ( flag )	//有效信息录入完毕(遇到mm、空格等其他字符)	
		{	
			break;	
		}
		else		//非法符号：Φ
		{
			;
		}
		
		cnt++;
	}
	
	temp[flag] = str2float(buff);		//防止真实数据后面直接遇到结束符
	pNum[flag] = temp[flag];

	switch ( flag )						//求试件的面积(默认取前两项数据)
	{
		case 0:				
			return (temp[0]);
		case 1:				
			return (temp[0] * temp[1]); 
		case 2:
			return (temp[0] * temp[1]);
		default:
			return 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : JudgeCharIsEqual
 * Description    : 判断字符串前n个字节与指定字符串是否相等
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus JudgeCharIsEqual( uint8_t n, const char *source, const char *target )
{
	char buff[20];
	
	if (n > 20)
	{
		return FAILED;
	}
	
	strcpy(buff,source);
	buff[n] = '\0';
	
	if ( 0 == strcmp(buff,target) )
	{
		return PASSED;
	}
	else
	{
		return FAILED;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetCircularArea
 * Description    : 查计算圆柱面积
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetCircularArea( float diameter )
{
	float r = 0.0f;
	
	r = diameter / 2;
	
	return (PI * r * r);
}

/*------------------------------------------------------------
 * Function Name  : FindStrIncludeIllegalChar
 * Description    : 查找字符串是否包含给定字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FlagStatus FindStrIncludeIllegalChar( const char *p, const char target )
{
	uint8_t i = 0;
	uint8_t len = strlen(p);
	
	for (i=0; i<len; ++i)
	{
		if (p[i] == target)
		{
			return SET;
		}
	}
	
	return RESET;	
}

/*------------------------------------------------------------
 * Function Name  : FindStrIncludeIllegalChar
 * Description    : 字符串倒序替换原字符串
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus StrCutBackReplace( char *p )
{
	int8_t i=0,j=0;
	int8_t cnt = 0;
	char buff[21];	
	
	while (p[cnt] != '\0')
	{
		cnt++;	//统计字符串个数
	}

	if ((cnt>20) || (cnt<1))
	{
		return ERROR;
	}

	j = cnt - 1;
	while (j != -1)
	{
		buff[i++] = p[j--];
	}
	
	buff[i] = '\0';

	memcpy(p,buff,cnt+1);

	return SUCCESS;
}


/*------------------------------------------------------------
 * Function Name  : GetPressNum
 * Description    : 获取按键数值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetPressNum( void )
{
	uint8_t Key = 0xff;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_0:
				Key = 0;
				break;
			case KEY_1:
				Key = 1;
				break;
			case KEY_2:
				Key = 2;
				break;
			case KEY_3:
				Key = 3;
				break;
			case KEY_4:
				Key = 4;
				break;
			case KEY_5:
				Key = 5;
				break;
			case KEY_6:
				Key = 6;
				break;
			case KEY_7:
				Key = 7;
				break;
			case KEY_8:
				Key = 8;
				break;
			case KEY_9:
				Key = 9;
				break;	
			default:
				Key = 0xff;
				break;
		}	
	}	
	
	return Key;
}

/*------------------------------------------------------------
 * Function Name  : ChangeDateFormate
 * Description    : 改变日期格式，20101010 -> 2010-10-10
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ChangeDateFormate( const char *source, char *target, const char p )
{
	memcpy(target,source,4);
	target[4] = p;
	
	memcpy(&target[5],&source[4],2);
	target[7] = p;
	
	memcpy(&target[8],&source[6],2);
	
	target[10] = NULL;
}

/*------------------------------------------------------------
 * Function Name  : CharConvTimeFormate
 * Description    : 字符串转换为日期格式
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CharConvTimeFormate( const char *source, tTime *target )
{
	char buff[10];
	uint16_t time = 0;

	memcpy(buff,source,4);
	buff[4] = NULL;

	time = (uint16_t)ustrtoul(buff,0,10);	//年
	target->usYear = time;

	memcpy(buff,&source[4],2);
	buff[2] = NULL;

	time = (uint16_t)ustrtoul(buff,0,10);	//月
	target->ucMon = time;

	memcpy(buff,&source[6],2);
	buff[2] = NULL;

	time = (uint16_t)ustrtoul(buff,0,10);	//日
	target->ucMday = time;
}

/*------------------------------------------------------------
 * Function Name  : tTimeConvTimeFormate
 * Description    : 日期格式转换
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void tTimeConvTimeFormate( FunctionalState ConvSecond, const tTime t, char *p )
{	
	usprintf(p,"%04d",t.usYear);
	p[4] = '-';
	usprintf(&p[5],"%02d",t.ucMon);
	p[7] = '-';
	usprintf(&p[8],"%02d",t.ucMday);
	
	p[10] = ' ';
	
	usprintf(&p[11],"%02d",t.ucHour);
	p[13] = ':';
	usprintf(&p[14],"%02d",t.ucMin);
	
	if ( ConvSecond == DISABLE )
	{
		p[16] = NULL;
	}
	else
	{
		p[16] = ':';
		usprintf(&p[17],"%02d",t.ucSec);
		p[19] = NULL;
	}
}

/*------------------------------------------------------------
 * Function Name  : LowercaseConvCapital
 * Description    : 小写字母转换为大写字母
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LowercaseConvCapital( const char *lower, char *cap )
{
	uint8_t i = 0;
	
	while (lower[i] != '\0')
	{
		if ( (lower[i]>='a') && (lower[i]<='z') )
		{
			cap[i] = lower[i] - 32;
		}
		else
		{
			cap[i] = lower[i];
		}
		i++;
	}
	
	cap[i] = NULL;
}

/*------------------------------------------------------------
 * Function Name  : GetTwoMonthDays
 * Description    : 获取2月的天数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static uint8_t GetTwoMonthDays(uint16_t year)
{
	uint8_t day;
	
	if(((year%4==0) && (year%100!=0)) || (year%400==0))//判断是不是闰年
	{
		day = 29;
	}
	else
	{
		day = 28;
	}
  
	return day;
}

/*------------------------------------------------------------
 * Function Name  : GetAMonthDays
 * Description    : 获取某月的天数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetAMonthDays( uint16_t year, uint8_t mouth )
{
	uint8_t day = 0;

	switch (mouth)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			day = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			day = 30;
			break;
		case 2:
			day = GetTwoMonthDays(year);	//获取某一年的2月份的天数
			break;		
	}

	return day;
}

/*------------------------------------------------------------
 * Function Name  : FindStrNotIncludeNumBit
 * Description    : 查找字符串中不包含数字的位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FlagStatus FindStrNotIncludeNumBit( const char *pStr, uint8_t *bit )
{
	uint8_t i = 0;
	
	*bit = 0;
	
	if (strlen(pStr) > 0xff)
	{
		return RESET;
	}
	
	while ( pStr[i] != NULL )
	{
		if ((pStr[i]<'0') || (pStr[i]>'9'))
		{
			*bit = i;
			
			return SET;
		}
		
		i++;
	}
	
	return RESET;
}

/*------------------------------------------------------------
 * Function Name  : SystemSoftwareReset
 * Description    : 系统软复位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SystemSoftwareReset( void )
{	
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}

/*------------------------------------------------------------
 * Function Name  : StrFillSpecifyChar
 * Description    : 在字符串中填充特殊字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void StrFillSpecifyChar( const char *pSource, char *pTarget, const char specifyChar )
{
	uint8_t i;
	uint8_t strLenth = strlen(pSource);
	
	for (i=0; i<strLenth; ++i)
	{
		pTarget[i] = specifyChar;
	}
	pTarget[i] = NULL;
}	

/*------------------------------------------------------------
 * Function Name  : SetValueNotEqualZero
 * Description    : 设置值不为0
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float SetValueNotEqualZero( float value )
{
	float tempValue = value;
	
	if (fabs(value) < MIN_FLOAT_PRECISION_DIFF_VALUE)
	{
		tempValue = MIN_FLOAT_PRECISION_DIFF_VALUE;
	}
	
	return tempValue;
}

/*------------------------------------------------------------
 * Function Name  : GetIntBit
 * Description    : 获取整数位数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetIntBit( int32_t sourceValue )
{
	uint8_t bit = 0;
	BoolStatus isPositive = YES;	//正数
	
	if (sourceValue < 0)
	{
		isPositive = NO;
	}	
	
	do 
	{
		sourceValue /= 10;		
		bit++;
	}while (sourceValue);
	
	if (isPositive== NO)
	{
		bit++;
	}
	
	return bit;
}

/*------------------------------------------------------------
 * Function Name  : GetAverage
 * Description    : 获取平均值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetAverage( float *pVal, uint8_t num )
{
	uint8_t i;
	float sum = 0.0f;
	
	if (0 == num)
	{
		return 0.0f;
	}
		
	for (i=0; i<num; i++)
	{
		sum += pVal[i];	
	}

	return (sum / num);		
}

/*------------------------------------------------------------
 * Function Name  : FindArrayValNotZeroCount
 * Description    : 发现数组中不为零的个数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t FindArrayValNotZeroCount( uint8_t num, uint8_t *Array )
{
	uint8_t i = 0;
	uint8_t cnt = 0;
	
	for (i=0; i<num; ++i)
	{
		if ( Array[i] )
		{
			cnt++;
		}
	}
	
	return cnt;
}

/*------------------------------------------------------------
 * Function Name  : SortBubble
 * Description    : 冒泡排序
 * Input          : data：存储待排序的数据， num：待排序的数据个数，temp：存储临时数据，cmp：比较方法体
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SortBubble( void *data[], uint32_t num, void *temp, comp_t cmp)
{
	uint32_t i,j;
	
	if (num < 2)
	{
		return;
	}
	
	for (i=0; i<num-1; i++)
	{
		for (j=0; j<num-i-1; j++)
		{
			if (cmp(&data[j],&data[j+1]) > 0)
			{
				temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetArrayEqualZeroIndex
 * Description    : 获取数组中等于0的元素的下标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetArrayEqualZeroIndex( uint8_t all_cnt, const uint8_t *pArray, uint8_t *pIndex )
{
	uint8_t i,num = 0;
	
	for (i=0; i<all_cnt; ++i)
	{
		if ( pArray[i] )
		{
			num++;
		}
		else
		{
			*pIndex = i;
		}
	}
	
	return num;
}

/*------------------------------------------------------------
 * Function Name  : FindNotMatch_10_To_90_Condition
 * Description    : 找出不满足指定条件(10%~90%)的试块分布
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void FindNotMatch_10_To_90_Condition( uint8_t num, float *Array, uint8_t *cnt_match )
{
	uint8_t i = 0;
	float avarage_strength = 0.0f;
	
	avarage_strength = GetAverage(Array,num);
	
	for (i=0; i<num; ++i)
	{
		if ( (Array[i]<avarage_strength*0.9f) || (Array[i]>avarage_strength*1.1f) )
		{
			cnt_match[i] = 1;
		}
		else
		{
			cnt_match[i] = 0;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : FindArrayElementIsOneIndex
 * Description    : 找出数组中第一个元素不为0的元素索引值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t FindArrayElementIsOneIndex( uint8_t num, uint8_t *Array )
{
	uint8_t i = 0;
	
	for (i=0; i<num; ++i)
	{
		if ( Array[i] )
		{
			return i;
		}
	}
	
	return 0;
}

/*------------------------------------------------------------
 * Function Name  : DeleteArrayWhichOnePositionElement
 * Description    : 删除数组中一个float类型元素
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void DeleteArrayWhichOnePositionElement( float *p, uint8_t num, uint8_t pos )
{
	uint8_t i;
	
	for (i=pos; i<num-1; ++i)
	{
		p[i] = p[i+1];
	}
}

/*------------------------------------------------------------
 * Function Name  : ContrastFloatData
 * Description    : 比对类型数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus ContrastFloatDataEqual( uint8_t num, const float *Array, uint8_t *index, const float data )
{
	uint8_t i;
	
	for (i=0; i<num; ++i)
	{
		if ( fabs(data - Array[i] ) < MIN_FLOAT_PRECISION_DIFF_VALUE )
		{
			*index = i;
			
			return SUCCESS;
		}
	}
	
	return ERROR;
}

/*------------------------------------------------------------
 * Function Name  : ContrastFloatData
 * Description    : 不满足指定条件（其他值不在中间值的15%范围内）的试块分布
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float FindNotMatch_Per15_Condition( uint8_t num, float *Array, uint8_t *cnt_match )
{
	uint8_t i = 0,index;
	float avarage_strength = 0.0f; 
	float buff[20];
	
	memcpy(buff,Array,sizeof(float)*num);
	
	{	
		float tempf = 0;
		
		SortBubble((void *)buff,num,&tempf,compFloatData);
	}
	
	if ( (buff[1]-buff[0]>buff[1]*0.15f) && (buff[2]-buff[1]>buff[1]*0.15f) )
	{		
		ContrastFloatDataEqual(num,Array,&index,buff[1]);	//找出无问题的强度值在原始数组的中位置
		
		for (i=0; i<num; ++i)
		{
			if (i == index)
			{
				cnt_match[i] = 0;
			}
			else
			{
				cnt_match[i] = 1;
			}
		}
	
		return 0.0f;
	}
	else if (buff[1] - buff[0] > buff[1] * 0.15f)	//buff[0]数据异常
	{
		avarage_strength = buff[1];
		
		ContrastFloatDataEqual(num,Array,&index,buff[0]);
		
		for (i=0; i<num; ++i)
		{
			if (i == index)
			{
				cnt_match[i] = 1;
			}
			else
			{
				cnt_match[i] = 0;
			}
		}	
	}
	else if (buff[2] - buff[1] > buff[1] * 0.15f)  //buff[2]数据异常
	{
		avarage_strength = buff[1];
		
		ContrastFloatDataEqual(num,Array,&index,buff[2]);
		
		for (i=0; i<num; ++i)
		{
			if (i == index)
			{
				cnt_match[i] = 1;
			}
			else
			{
				cnt_match[i] = 0;
			}
		}	
	}
	else
	{
		avarage_strength = GetAverage(Array,num);
	
		for (i=0; i<num; ++i)
		{
			cnt_match[i] = 0;
		}
	}
	
	return avarage_strength;
}

/*------------------------------------------------------------
 * Function Name  : GetFloatSmallValue
 * Description    : 获取两个float类型的较小值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetFloatSmallValue( float a, float b )
{
	float val = (a > b) ? b: a;
	
	return val;
}

/*------------------------------------------------------------
 * Function Name  : CompareStrBuffIsEqualWithChar
 * Description    : 比较字符缓冲区每个元素是否和目标值相等
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus CompareStrBuffIsEqualWithChar( const uint8_t *pSorce, const uint8_t target, uint32_t lenth )
{
	uint32_t i;
	
	for (i=0; i<lenth; ++i)
	{
		if (pSorce[i] != target)
		{
			return FAILED;
		}
	}
	
	return PASSED;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

