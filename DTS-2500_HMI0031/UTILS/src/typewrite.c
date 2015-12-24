/**
  ******************************************************************************
  * @file    typewrite.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-9 16:17:09
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ������
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
//����������
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

//���뷨���ݴ洢��
static int32_t TypeWriteInt = 0;				
static float   TypeWriteFloat = 0.0f;			
static char    TypeWriteChar[MAX_PUTIN_DATA_BIT+1] = {0};

/* Private function prototypes -----------------------------------------------*/
static void JudgePutinValidStatus( void );
static void AnalogKeyValue( uint32_t key );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : GetPutinAddr
 * Description    : ��ȡ�����ַ
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
 * Description    : ��������ѭ����
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
			if (KEY_NONE != g_key.value)	//��������
			{
				bsp_StartTimer(KEY_TIMING,CONFIRM_KEY_PRESS_CNT);
				g_key.status = STATUS_KEY_WAIT;
			}
			break;
			
		case STATUS_KEY_WAIT:
			if ( bsp_CheckTimer(KEY_TIMING) )
			{
				if (KEY_NONE != g_key.value)	//ȷ�ϰ�������
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
				if (KEY_NONE == g_key.value)	//�������º��ɿ�
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
				if (KEY_NONE == g_key.value)	//�������º��ɿ�
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
 * Description    : �Ƿ��°���
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
 * Description    : shift�����ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ShiftCheckCycle( void )
{
	static uint8_t shiftIndex = 0;		//ͳ��SHIFT�����µĴ���	
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
					case 0 : g_putin.typewriting = STA_NUM; 	break;		//����
					case 1 : g_putin.typewriting = STA_CAP; 	break;		//��д
					case 2 : g_putin.typewriting = STA_LOW; 	break; 		//Сд
					default:
						break;
				}	
			}
		}
	}
	else 
	{		
		g_putin.typewriting = STA_NUM;		//Ĭ�����뷨
		shiftIndex = 0;
	}
}

/*------------------------------------------------------------
 * Function Name  : JudgePutinValidStatus
 * Description    : ����������Ч״̬
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
 * Description    : ���뷨����ѭ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PutinProcessCycle( void )
{
	/* �������¼������ */
	KeyProcessCycle();
	
	/* Shift���������� */
	ShiftCheckCycle();
	
	/* ������Ч�Լ������ */
	JudgePutinValidStatus();
	
	/* ͨѶС������ */
	CommucationLampTask();
	
	/* �ͱ�״̬������ */
	PumpStatusLampTask();
	
	/* ִ�л������� */
	ExecuteTask();
	
	#if 0
		/* ������ */
		CheckSystemWarningStatusCycle();
	#endif
	
	/* USB���� */
	LinkUSBCycle();
	
	/* ��̫��ѭ������ */
	LwIP_Periodic_Handle(bsp_GetRunTime());
}

/*------------------------------------------------------------
 * Function Name  : InitKeyCheck
 * Description    : ��ʼ���������
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
 * Description    : ��ʼ�����뷨��ʾ
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
 * Description    : ��ʾ���뷨GUI
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
 * Description    : ��ʾ���뷨
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
 * Description    : ģ�ⰴ�°���
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
 * Description    : ɾ�������ַ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DeletePutinChars( uint8_t *cnt )
{
	static uint8_t num = 0;	//��¼��������ʱɾ���ַ�����
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
 * Description    : �����ַ�
 * Input          : buff�����뻺�棬cnt�������ַ�������max_putin_cnt�������û������ַ�������index_cycle�������ڲ�����ֵ
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static ErrorStatus PutinChar( char *buff, uint8_t cnt, uint8_t max_putin_cnt, uint8_t *index_cycle )
{
	uint8_t index_key;
	const uint8_t INDEX_TABLE_MAX_CNT = 12;
	const uint16_t CONFIRM_PUTIN = 500;		//ȷ������һ���ַ�ʱ��
	
	//�ڱ��в���������ַ�
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
	
	//���뵽���λ��������������ַ��Զ��������λ�ַ�
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
 * Description    : ��ʾ�����ַ�
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
 * Description    : ���������ַ�
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
			case TYPE_INT: 		//����Ϊ��������
				FindPutcharIntIllegar(&source_cnt,source);
				TypeWriteInt = ustrtoul(source,0,10);	 							
				break;

			case TYPE_FLOAT:	//����Ϊ����������														
				TypeWriteFloat = str2float(source);											
				break;

			case TYPE_CHAR:		//����Ϊ�ַ�������
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
 * Description    : ����״̬GUI����
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
* Description    : �Ӽ��������ַ��洢��pData(ע���洢ʱ���������ַ������洢����ʾʱ�����ı���ĳ�����ʾ���������ݲ�һ����ͬ)
* Input            enable_putin:	��������λ��		RESET-������; SET-����
*				   enable_shift:	�����л����뷨
*                  x:				x������
*				   y:				y������
*				   allow_putin_num:	���������ַ�����
*				   len_fill:		��������䳤��				   
*				   str_count��		�����ַ�����
*				   save_type:		���ݱ������ͣ�
*			       pType_int��		���ݱ��淽ʽ���������ݻ��� 
*				   pType_float��	���ݱ��淽ʽ�����������ݻ���
*				   pType_char��		���ݱ��淽ʽ: �ַ������ݻ���
* Return         : ����״̬
*ReadMe:˼·�����۱��淽ʽ��Σ����Ȱ����ݴ洢Ϊ�ַ������ݣ���������Ӧ��ת�����õ����ͻ��߸��������� 
*******************************************************************************/
void KeyPutinChars( PUTIN_TypeDef *Putin )
{
	STATUS_PUTIN_TypeDef putinStatus = STATUS_DISABLE_PUTIN;	
	uint8_t putin_cnt = 0;						//��ǰ�����ַ�����
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
				PutinProcessCycle();	//��ⰴ������
			
				ShowTypewrite(Putin);
				
				//���ڵ�һ�μ�ֵ�ѱ����ǣ����ģ���һ�����밴���ļ�ֵ
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
						case KEY_SHIFT:		//�л����뷨
							if (SET == Putin->putinCharIng)
							{
								Putin->putinCharIng = RESET;
								
								if (putin_cnt < Putin->AllowPutinBit)
								{
									putin_cnt++;
								}
							}								
							break;
						
						case KEY_BACK:			//ɾ����
							if (RESET == Putin->putinCharIng)
							{
								DeletePutinChars(&putin_cnt);
							}
							break;
						
						case KEY_ENTER:			//ȷ�ϼ�
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
						
						case KEY_ESC:			//�˳������	
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
					
					//ʵʱ��ʾ�ַ�
					if ( (SET == Putin->putinCharIng) && (putin_cnt != Putin->AllowPutinBit) )
					{
						ShowPutinChars(Putin->Encrypt,Putin->x,Putin->y,Putin->queueBuff,putin_cnt+1,Putin->FillBit);		
					}
					else
					{
						ShowPutinChars(Putin->Encrypt,Putin->x,Putin->y,Putin->queueBuff,putin_cnt,Putin->FillBit);
					}
				}

				//ȷ�������ַ�
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
 * Description    : ��ȡ�������ݵ�ַ
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
 * Description    : ��ȡ���������ݵ�ַ
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
 * Description    : ��ȡ�ַ������ݵ�ַ
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
 * Description    : ��ȡ������ֵ
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
 * Description    : ��ȡ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
STATUS_PUTIN_TypeDef GetPutinStatus( void )
{
	return g_putin.putinStatus;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

