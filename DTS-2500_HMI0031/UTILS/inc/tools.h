/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOOLS_H
#define __TOOLS_H

/* Includes ------------------------------------------------------------------*/


/* Exported define -----------------------------------------------------------*/
#define MAX_PUTIN_DATA_BIT				16							//支持最大输入位数

#define	MAX_TWO_MENU_HIGH_POS			45							//最高二级菜单位置			
#define	MIN_TWO_MENU_HIGH_POS			440							//最低二级菜单位置

#define MIN_FLOAT_PRECISION_DIFF_VALUE	0.0001f						//float最小精度差值，因为浮点数可能表示不精确 
#define PI								3.1415926f					//圆心率

typedef int32_t (*comp_t)(const void* const,const void* const);

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	NONE_PARAMETER = 0,				//无参数
	IMMEDIATELY_PUTIN_NONE,			//可立即输入
	IMMEDIATELY_PUTIN_SHIFT,		//可立即输入(允许切换输入法)
	NONE_USE_USER_DEFINE,			//无自定义功能
	USE_USER_DEFINE,				//带自定义功能	
}PARAMETER_TYPE_TypeDef;

typedef struct
{
	FunctionalState enableMoveIndex;
	uint8_t rowNum;
	uint8_t colNum;
	uint8_t sumNum;
	uint8_t *pNowIndex;
}INDEX_MANAGE_TypeDef;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : compFloatData
 * Description    : 比较float数据大小
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static __INLINE int32_t compFloatData( const void* const pSource, const void* const pTarget ) 
{
	const float source = *(const float*)pSource;
	const float target = *(const float*)pTarget;
	 
	if (source > target)
	{
		return 1;
	}
	else if (source < target)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/*------------------------------------------------------------
 * Function Name  : compUnsignedData
 * Description    : 比较无符号数据大小
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static __INLINE int32_t compUnsignedData( const void* const pSource, const void* const pTarget ) 
{
	const uint32_t source = *(const uint32_t*)pSource;
	const uint32_t target = *(const uint32_t*)pTarget;
	
	if (source > target)
	{
		return 1;
	}
	else if (source == target)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/* Exported variables --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void CharToChar( uint8_t num, char *target, const char *source );
void floattochar( uint8_t max_bit,uint8_t point_n,float num,char *str );
FlagStatus FindPutcharIntIllegar( uint8_t *pCnt, char *p );
void numtochar(uint8_t maxBit,int32_t num,char *c);
void KeyIndexManage( INDEX_MANAGE_TypeDef *pIndexObj );
float GetSampleSpecificationArea( const char *pVal, float *pNum );
FlagStatus FindStrIncludeIllegalChar( const char *p, const char target );
ErrorStatus StrCutBackReplace( char *p );
uint8_t GetPressNum( void );
void ChangeDateFormate( const char *source, char *target, const char p );
void CharConvTimeFormate( const char *source, tTime *target );
void tTimeConvTimeFormate( FunctionalState ConvSecond, const tTime t, char *p );
void LowercaseConvCapital( const char *lower, char *cap );
uint8_t GetAMonthDays( uint16_t year, uint8_t mouth );
FlagStatus FindStrNotIncludeNumBit( const char *pStr, uint8_t *bit );
void SystemSoftwareReset( void );
void StrFillSpecifyChar( const char *pSource, char *pTarget, const char specifyChar );
float SetValueNotEqualZero( float value );
uint8_t GetIntBit( int32_t sourceValue );
TestStatus JudgeCharIsEqual( uint8_t n, const char *source, const char *target );
float GetCircularArea( float diameter );
float GetAverage( float *pVal, uint8_t num );
uint8_t FindArrayValNotZeroCount( uint8_t num, uint8_t *Array );
void SortBubble( void *data[], uint32_t num, void *temp, comp_t cmp);
int32_t compUnsignedData( const void* const pSource, const void* const pTarget );
uint8_t GetArrayEqualZeroIndex( uint8_t all_cnt, const uint8_t *pArray, uint8_t *pIndex );
void FindNotMatch_10_To_90_Condition( uint8_t num, float *Array, uint8_t *cnt_match );
uint8_t FindArrayElementIsOneIndex( uint8_t num, uint8_t *Array );
void DeleteArrayWhichOnePositionElement( float *p, uint8_t num, uint8_t pos );
ErrorStatus ContrastFloatDataEqual( uint8_t num, const float *Array, uint8_t *index, const float data );
float FindNotMatch_Per15_Condition( uint8_t num, float *Array, uint8_t *cnt_match );
float GetFloatSmallValue( float a, float b );
TestStatus CompareStrBuffIsEqualWithChar( const uint8_t *pSorce, const uint8_t target, uint32_t lenth );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
