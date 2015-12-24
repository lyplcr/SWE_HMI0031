/**
  ******************************************************************************
  * @file    typewrite.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-9 16:17:09
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   主程序
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "typewrite.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
//键盘索引表
const KB_INDEX_TypeDef kb[12] = 
{
	{KEY_0,		"0", " 0"	, " 0"	 },
	{KEY_1, 	"1", "*"	, "*"	 },
	{KEY_2, 	"2", "ABC"	, "abc"  },
	{KEY_3, 	"3", "DEF"	, "def"  },
	{KEY_4, 	"4", "GHI"	, "ghi"  },
	{KEY_5,	 	"5", "JKL"  , "jkl"  },
	{KEY_6, 	"6", "MNO"	, "mno"  },
	{KEY_7, 	"7", "PQRS" , "pqrs" },
	{KEY_8, 	"8", "TUV"	, "tuv"  },
	{KEY_9, 	"9", "WXYZ" , "wxyz" },
	{KEY_ADD, 	"-", "-"	, "-"    },
	{KEY_POINT, ".", "."	, "."    },
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static KEY_TypeDef g_key;
static PUTIN_TypeDef g_putin;

//输入法数据存储区
static int32_t TypeWriteInt = 0;				
static float   TypeWriteFloat = 0.0f;			
static char    TypeWriteChar[MAX_PUTIN_DATA_BIT+1] = {0};

/* Private function prototypes -----------------------------------------------*/
static void JudgePutinValidStatus( void );
static void AnalogKeyValue( uint32_t key );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : GetPutinAddr
 * Description    : 获取输入地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
PUTIN_TypeDef *GetPutinAddr( void )
{
	return &g_putin;
}


/*------------------------------------------------------------
 * Function Name  : KeyProcessCycle
 * Description    : 按键处理循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void KeyProcessCycle( void )
{	
	const uint16_t CONFIRM_KEY_PRESS_CNT = 20;
	const uint16_t CONFIRM_KEY_LONG_PRESS_CNT = 500;
	const uint16_t CONFIRM_KEY_CONTINUE_PRESS_CNT = 80;
	const uint16_t SHORT_PRESS_WAIT_CNT = 20;
	
	g_key.value = GetKey();
	
	if ((g_key.status==STATUS_KEY_NO_PRESS) && (KEY_NONE!=g_key.value))
	{
		g_key.recordValue = g_key.value;
	}
	
	switch ( g_key.status )
	{
		case STATUS_KEY_NO_PRESS:
			if (KEY_NONE != g_key.value)	//按键按下
			{
				bsp_StartTimer(KEY_TIMING,CONFIRM_KEY_PRESS_CNT);
				g_key.status = STATUS_KEY_WAIT;
			}
			break;
			
		case STATUS_KEY_WAIT:
			if ( bsp_CheckTimer(KEY_TIMING) )
			{
				if (KEY_NONE != g_key.value)	//确认按键按下
				{
					bsp_StartTimer(KEY_TIMING,CONFIRM_KEY_LONG_PRESS_CNT);
					g_key.status = STATUS_KEY_PRESS;
				}
				else
				{
					g_key.status = STATUS_KEY_NO_PRESS;
				}
			}
			break;
			
		case STATUS_KEY_PRESS:
			if ( bsp_CheckTimer(KEY_TIMING) )
			{				
				g_key.status = STATUS_KEY_LONG_PRESS;
			}
			else
			{
				if (KEY_NONE == g_key.value)	//按键按下后松开
				{
					g_key.value = g_key.recordValue;
					g_key.status = STATUS_KEY_SHORT_PRESS;
				}
			}
			break;
			
		case STATUS_KEY_SHORT_PRESS:
			g_key.status = STATUS_KEY_SHORT_WAIT;
			bsp_StartTimer(KEY_TIMING,SHORT_PRESS_WAIT_CNT);
			break;
		
		case STATUS_KEY_SHORT_WAIT:
			if ( bsp_CheckTimer(KEY_TIMING) )
			{
				g_key.status = STATUS_KEY_NO_PRESS;
			}
			break;
		
		case STATUS_KEY_LONG_PRESS:
			bsp_StartTimer(KEY_TIMING,CONFIRM_KEY_CONTINUE_PRESS_CNT);
			g_key.status = STATUS_KEY_PRESS_LEAVE;	
			break;
			
		case STATUS_KEY_PRESS_LEAVE:
			if ( bsp_CheckTimer(KEY_TIMING) )
			{
				g_key.value = g_key.recordValue;
				g_key.status = STATUS_KEY_LONG_PRESS;
			}
			else
			{
				if (KEY_NONE == g_key.value)	//按键按下后松开
				{
					g_key.status = STATUS_KEY_NO_PRESS;
				}
			}
			break;
		
		default:
			g_key.status = STATUS_KEY_NO_PRESS;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : IsPressKey
 * Description    : 是否按下按键
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus IsPressKey( void )
{
	if ((g_key.status==STATUS_KEY_SHORT_PRESS) || (g_key.status==STATUS_KEY_LONG_PRESS))
	{
		#ifdef ENABLE_BEEP
			BEEP_RING_ONE();
		#endif
		
		return YES;
	}
	
	return NO;
}


/*------------------------------------------------------------
 * Function Name  : ShiftCheckCycle
 * Description    : shift键检测循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ShiftCheckCycle( void )
{
	static uint8_t shiftIndex = 0;		//统计SHIFT键按下的次数	
	const uint8_t CNT_SHIFT_FUNC = 3;

	if (g_putin.NewShift == ENABLE_SHIFT)
	{	
		if (IsPressKey() == YES)	
		{
			if (KEY_SHIFT == g_key.value) 
			{				
				shiftIndex++;
				if (shiftIndex >= CNT_SHIFT_FUNC)
				{
					shiftIndex = 0;
				}
				switch ( shiftIndex )
				{
					case 0 : g_putin.typewriting = STA_NUM; 	break;		//数字
					case 1 : g_putin.typewriting = STA_CAP; 	break;		//大写
					case 2 : g_putin.typewriting = STA_LOW; 	break; 		//小写
					default:
						break;
				}	
			}
		}
	}
	else 
	{		
		g_putin.typewriting = STA_NUM;		//默认输入法
		shiftIndex = 0;
	}
}

/*------------------------------------------------------------
 * Function Name  : JudgePutinValidStatus
 * Description    : 输入数据有效状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void JudgePutinValidStatus( void )
{
	if (IsPressKey() == YES)
	{
		switch ( g_key.value )
		{
			case KEY_0 		: 
			case KEY_1 		:
			case KEY_2 		:
			case KEY_3 		:
			case KEY_4 		:
			case KEY_5 		:
			case KEY_6 		:
			case KEY_7 		:
			case KEY_8 		:
			case KEY_9 		:
			case KEY_ADD    :
			case KEY_POINT  :
				g_putin.validPutin = YES;
				break;
			default:
				g_putin.validPutin = NO;
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : PutinProcessCycle
 * Description    : 输入法处理循环
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PutinProcessCycle( void )
{
	/* 按键按下检测任务 */
	KeyProcessCycle();
	
	/* Shift键按下任务 */
	ShiftCheckCycle();
	
	/* 输入有效性检测任务 */
	JudgePutinValidStatus();
	
	/* 通讯小灯任务 */
	CommucationLampTask();
	
	/* 油泵状态灯任务 */
	PumpStatusLampTask();
	
	/* 执行机构任务 */
	ExecuteTask();
	
	#if 0
		/* 警告检测 */
		CheckSystemWarningStatusCycle();
	#endif
	
	/* USB任务 */
	LinkUSBCycle();
	
	/* 以太网循环任务 */
	LwIP_Periodic_Handle(bsp_GetRunTime());
}

/*------------------------------------------------------------
 * Function Name  : InitKeyCheck
 * Description    : 初始化按键检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitKeyCheck( void )
{
	g_key.value = 0;
	g_putin.validPutin = NO;
	g_putin.typewriting = STA_NUM;
	g_key.status = STATUS_KEY_NO_PRESS;
}

/*------------------------------------------------------------
 * Function Name  : InitPutinShow
 * Description    : 初始化输入法显示
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitPutinShow( void )
{
	bsp_StartAutoTimer(PUTIN_SHOW_TIMING,100);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ShowTypewrite
 * Description    : 显示输入法GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ShowTypewrite( PUTIN_TypeDef *Putin )
{
	const uint16_t pc = Putin->skin.pointColor;
	const uint16_t bc = Putin->skin.backColor;
	const uint16_t x = Putin->skin.x;
	const uint16_t y = Putin->skin.y;
	
	if (Putin->skin.open == ENABLE)
	{
		switch ( g_putin.typewriting )
		{
			case STA_NUM:
				GUI_DispStr32At(x,y,pc,bc,"123");
				break;				
			case STA_CAP:
				GUI_DispStr32At(x,y,pc,bc,"ABC");
				break;				
			case STA_LOW:
				GUI_DispStr32At(x,y,pc,bc,"abc");
				break;
			
			default:
				break;
		}	
	}
	else
	{
		GUI_DispStr32At(x,y,bc,bc,"   ");	  
	}			
}

/*------------------------------------------------------------
 * Function Name  : ShowTypewrite
 * Description    : 显示输入法
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ShowTypewrite( PUTIN_TypeDef *Putin )
{
	if ( bsp_CheckTimer(PUTIN_SHOW_TIMING) )
	{
		GUI_ShowTypewrite(Putin);
	}	
}

/*------------------------------------------------------------
 * Function Name  : AnalogKeyValue
 * Description    : 模拟按下按键
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/	
static void AnalogKeyValue( uint32_t key )
{
	g_key.status = STATUS_KEY_SHORT_PRESS;
	g_key.value = key;
}

/*------------------------------------------------------------
 * Function Name  : DeletePutinChars
 * Description    : 删除输入字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DeletePutinChars( uint8_t *cnt )
{
	static uint8_t num = 0;	//记录长按按键时删除字符个数
	const uint8_t DELETE_ALL_CHARS_CNT = 3;
	
	if ( *cnt )
	{
		switch ( g_key.status )
		{
			case STATUS_KEY_SHORT_PRESS:
				(*cnt)--;
				break;
			
			case STATUS_KEY_LONG_PRESS:
				num++;
				if (num > DELETE_ALL_CHARS_CNT)
				{
					*cnt = 0;
					num = 0;
				}
				else
				{
					(*cnt)--;
				}
				
				return;
			
			default:
				break;
		}			
	}
	
	num = 0;
}

/*------------------------------------------------------------
 * Function Name  : PutinChar
 * Description    : 输入字符
 * Input          : buff：输入缓存，cnt：输入字符个数，max_putin_cnt：允许用户输入字符个数，index_cycle：按键内部索引值
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus PutinChar( char *buff, uint8_t cnt, uint8_t max_putin_cnt, uint8_t *index_cycle )
{
	uint8_t index_key;
	const uint8_t INDEX_TABLE_MAX_CNT = 12;
	const uint16_t CONFIRM_PUTIN = 500;		//确认输入一个字符时间
	
	//在表中查找输入的字符
	for (index_key=0; index_key<INDEX_TABLE_MAX_CNT; index_key++)	
	{
		if (g_key.value	== kb[index_key].index)		
		{
			break;
		}					
	}	
	if (INDEX_TABLE_MAX_CNT == index_key)
	{
		return ERROR;
	}
	
	//输入到最大位数，最新输入的字符自动覆盖最大位字符
	if ( max_putin_cnt )
	{
		if (cnt == max_putin_cnt)
		{
			cnt--;
		}
	}
	
	switch ( g_putin.typewriting )
	{
		case STA_NUM:
			buff[cnt] = kb[index_key].pNum[0];
			break;
		
		case STA_CAP:
			if (kb[index_key].pCap[*index_cycle] == 0)	
			{
				buff[cnt] = kb[index_key].pNum[0];	
				(*index_cycle) = 0;	
			}
			else
			{
				buff[cnt] = kb[index_key].pCap[*index_cycle];		
				(*index_cycle)++;		   
			}
			bsp_StartTimer(PUTIN_WAIT_CHAR_TIMING,CONFIRM_PUTIN);
			break;
		
		case STA_LOW:
			if (kb[index_key].pLow[*index_cycle] == 0)	
			{
				buff[cnt] = kb[index_key].pNum[0];	
				(*index_cycle) = 0;	
			}
			else
			{
				buff[cnt] = kb[index_key].pLow[*index_cycle];		
				(*index_cycle)++;		   
			}
			bsp_StartTimer(PUTIN_WAIT_CHAR_TIMING,CONFIRM_PUTIN);
			break;
		
		default:
			break;
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------
 * Function Name  : ShowPutinChars
 * Description    : 显示输入字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ShowPutinChars( FunctionalState Encrypt, uint16_t x, uint16_t y, char *p, uint8_t cnt, uint8_t show_bit )
{
	char show_buff[MAX_ALLOW_PUTIN_CNT] = {0};
	char conv_buff[MAX_ALLOW_PUTIN_CNT] = {0};

	p[cnt] = 0;	
	
	if (Encrypt == ENABLE)
	{
		memset(conv_buff,'*',cnt*sizeof(char));
		conv_buff[cnt] = 0;	
		CharToChar(show_bit,show_buff,conv_buff);
	}
	else
	{
		CharToChar(show_bit,show_buff,p);
	}
	
	if (g_putin.FontSize == 24)
	{
		GUI_DispStr24At(x,y,COLOR_PUTIN_FONT_POINT,COLOR_PUTIN_FONT_BACK,show_buff);
	}
	else
	{
		GUI_DispStr16At(x,y,COLOR_PUTIN_FONT_POINT,COLOR_PUTIN_FONT_BACK,show_buff);
	}
}

/*------------------------------------------------------------
 * Function Name  : SavePutinChars
 * Description    : 保存输入字符
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus SavePutinChars( TYPE_SAVE_TypeDef save_type, char *source, uint8_t source_cnt, uint8_t **target_cnt )
{
	if ( source_cnt )
	{
		source[source_cnt] = 0;
		
		switch ( save_type )			
		{
			case TYPE_INT: 		//保存为整型数据
				FindPutcharIntIllegar(&source_cnt,source);
				TypeWriteInt = ustrtoul(source,0,10);	 							
				break;

			case TYPE_FLOAT:	//保存为浮点型数据														
				TypeWriteFloat = str2float(source);											
				break;

			case TYPE_CHAR:		//保存为字符型数据
				strcpy(TypeWriteChar,source);	
				break;

			default:
				break;
		}
		
		**target_cnt = source_cnt;
		
		return SUCCESS;
	}
	else
	{
		**target_cnt = 0;
		
		return ERROR;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_PutinReady
 * Description    : 输入状态GUI界面
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_PutinReady( PUTIN_TypeDef *Putin )
{
//	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	lcd_fill(Putin->x,Putin->y,Putin->putinFrameLenth,Putin->putinFrameWidth,COLOR_PUTIN_FONT_BACK);
				
//	pShortCut->status = SHOW_F3 | SHOW_F4;
//	pShortCut->pointColor = Putin->MenuPointColor;
//	pShortCut->backColor = Putin->MenuBackColor;
//	pShortCut->pContent[2] = pTwoLevelMenu[51];
//	pShortCut->pContent[3] = pTwoLevelMenu[58];
//	
//	ShortcutMenuTask(pShortCut);
	
	GUI_ClearShortcutMenu();
}


/*******************************************************************************
* Function Name  : putin_value
* Description    : 从键盘输入字符存储到pData(注：存储时按照输入字符个数存储，显示时按照文本框的长度显示，两个数据不一定相同)
* Input            enable_putin:	允许输入位：		RESET-不允许; SET-允许
*				   enable_shift:	允许切换输入法
*                  x:				x轴坐标
*				   y:				y轴坐标
*				   allow_putin_num:	允许输入字符个数
*				   len_fill:		数据区填充长度				   
*				   str_count：		输入字符个数
*				   save_type:		数据保存类型：
*			       pType_int：		数据保存方式：整型数据缓存 
*				   pType_float：	数据保存方式：浮点型数据缓存
*				   pType_char：		数据保存方式: 字符型数据缓存
* Return         : 输入状态
*ReadMe:思路：无论保存方式如何，首先把数据存储为字符型数据，最后进行相应的转换，得到整型或者浮点型数据 
*******************************************************************************/
void KeyPutinChars( PUTIN_TypeDef *Putin )
{
	STATUS_PUTIN_TypeDef putinStatus = STATUS_DISABLE_PUTIN;	
	uint8_t putin_cnt = 0;						//当前输入字符个数
	static BoolStatus isFirstPutin = YES; 
		
	ShowTypewrite(Putin);				
	
	if (DISABLE == Putin->skin.open)
	{
		Putin->putinStatus = STATUS_DISABLE_PUTIN;
		return;
	}
	
	while ( 1 )
	{				
		switch ( putinStatus )
		{
			case STATUS_DISABLE_PUTIN:
				if (YES == Putin->validPutin)	
				{		
					Putin->value = g_key.value;
					Putin->recordValue = 0;
					Putin->indexCycle = 0;
					Putin->putinCharIng = RESET;
					isFirstPutin = YES;
					
					GUI_PutinReady(Putin);
					
					putinStatus = STATUS_TEXT_EDIT;
				}
				else
				{	
					Putin->putinStatus = STATUS_DISABLE_PUTIN;
					return;
				}
				break;
				
			case STATUS_TEXT_EDIT:
				PutinProcessCycle();	//检测按键输入
			
				ShowTypewrite(Putin);
				
				//由于第一次键值已被覆盖，因此模拟第一次输入按键的键值
				if (isFirstPutin == YES)
				{
					isFirstPutin = NO;
					AnalogKeyValue(Putin->value);
				}
			
				if (IsPressKey() == YES)
				{
					if (g_key.value != Putin->recordValue)
					{
						Putin->recordValue = g_key.value;
						Putin->indexCycle = 0;
					}
					
					switch ( g_key.value )
					{
						case KEY_SHIFT:		//切换输入法
							if (SET == Putin->putinCharIng)
							{
								Putin->putinCharIng = RESET;
								
								if (putin_cnt < Putin->AllowPutinBit)
								{
									putin_cnt++;
								}
							}								
							break;
						
						case KEY_BACK:			//删除键
							if (RESET == Putin->putinCharIng)
							{
								DeletePutinChars(&putin_cnt);
							}
							break;
						
						case KEY_ENTER:			//确认键
							if (SET == Putin->putinCharIng)	
							{
								Putin->putinCharIng = RESET;
								
								if (putin_cnt < Putin->AllowPutinBit)
								{
									putin_cnt++;
								}
							}
							putinStatus = STATUS_EDIT_COMP;
							break;
						
						case KEY_ESC:			//退出输入键	
							if (SET == Putin->putinCharIng)
							{
								Putin->putinCharIng = RESET;
								
								if (putin_cnt < Putin->AllowPutinBit)
								{
									putin_cnt++;
								}
							}
							putinStatus = STATUS_CANCEL_PUTIN;						
							break;
						
						default:				
							if (YES == Putin->validPutin)
							{	
								if (ERROR == PutinChar(Putin->queueBuff,putin_cnt,Putin->AllowPutinBit,&Putin->indexCycle) )
								{
									break;
								}
								
								if (STA_NUM == Putin->typewriting)
								{
									if (putin_cnt < Putin->AllowPutinBit)
									{
										putin_cnt++;
									}		
								}
								else
								{
									Putin->putinCharIng = SET;	
								}
							}
							break;
					}					
					
					//实时显示字符
					if ( (SET == Putin->putinCharIng) && (putin_cnt != Putin->AllowPutinBit) )
					{
						ShowPutinChars(Putin->Encrypt,Putin->x,Putin->y,Putin->queueBuff,putin_cnt+1,Putin->FillBit);		
					}
					else
					{
						ShowPutinChars(Putin->Encrypt,Putin->x,Putin->y,Putin->queueBuff,putin_cnt,Putin->FillBit);
					}
				}

				//确认输入字符
				if (SET == Putin->putinCharIng)
				{
					if ( bsp_CheckTimer(PUTIN_WAIT_CHAR_TIMING) )	
					{
						Putin->putinCharIng = RESET;
						Putin->indexCycle = 0;
						
						if (putin_cnt < Putin->AllowPutinBit)
						{
							putin_cnt++;
						}
					}
				}

				break;
				
			case STATUS_EDIT_COMP:
				if (SUCCESS == SavePutinChars(Putin->SaveType,Putin->queueBuff,putin_cnt,(uint8_t **)&(Putin->PutinNum)))
				{
					Putin->putinStatus = STATUS_EDIT_COMP;
				}
				else
				{
					Putin->putinStatus = STATUS_CANCEL_PUTIN;
				}
				return;
			
			case STATUS_CANCEL_PUTIN:
				*(Putin->PutinNum) = 0;
					
				Putin->putinStatus = STATUS_CANCEL_PUTIN;
				return;
			
			default:
				
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GetPutinIntDataAddr
 * Description    : 获取整型数据地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
int32_t *GetPutinIntDataAddr( void )
{
	return &TypeWriteInt;
}

/*------------------------------------------------------------
 * Function Name  : GetPutinFloatDataAddr
 * Description    : 获取浮点型数据地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float *GetPutinFloatDataAddr( void )
{
	return &TypeWriteFloat;
}

/*------------------------------------------------------------
 * Function Name  : GetPutinCharDataAddr
 * Description    : 获取字符串数据地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
char *GetPutinCharDataAddr( void )
{
	return TypeWriteChar;
}
	
/*------------------------------------------------------------
 * Function Name  : GetKeyVal
 * Description    : 获取按键键值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t GetKeyVal( void )
{
	return g_key.value;
}

/*------------------------------------------------------------
 * Function Name  : GetPutinStatus
 * Description    : 获取输入状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
STATUS_PUTIN_TypeDef GetPutinStatus( void )
{
	return g_putin.putinStatus;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

