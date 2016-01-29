/**
  ******************************************************************************
  * @file    ModelSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-10 16:34:06
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ����ѡ�����
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "ModelSelect.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_POINT						WHITE
#define	COLOR_BACK						BLACK

#define MAX_MENU_NUM					2

#define CONFIG_MACHINE_PARAMETER_NUM	3
#define MAX_CONFIG_MACHINE_PUTIN_BIT	4

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	INDEX_UNIVERSAL_DZ = 0,		//�������ܻ�
	INDEX_UNIVERSAL_YY,			//Һѹ���ۻ�
}INDEX_MODEL_SELECT_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	uint8_t indexArray[MAX_MENU_NUM];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MAX_MENU_NUM];
	uint8_t menuCnt;	
	uint8_t rowNum;
	const char *pParameterNameArray[MAX_MENU_NUM];
	const char * pTitle;
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	LEAVE_PAGE_TypeDef leavePage;						
}MODEL_SELECT_TypeDef;

typedef enum
{
	INDEX_MACHINE_TONS = 0,		
	INDEX_MAX_DISPALCEMENT,
	INDEX_MAX_DEFORM,
}INDEX_CONFIG_MACHINE_TypeDef;

typedef enum
{
	OBJECT_MACHINE_TONS = 0,		
	OBJECT_MAX_DISPALCEMENT,
	OBJECT_MAX_DEFORM,
}OBJECT_CONFIG_MACHINE_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[CONFIG_MACHINE_PARAMETER_NUM][MAX_CONFIG_MACHINE_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[CONFIG_MACHINE_PARAMETER_NUM];
	const char *pParameterNameArray[CONFIG_MACHINE_PARAMETER_NUM];
	const char *pParameterUnitArray[CONFIG_MACHINE_PARAMETER_NUM];
	uint8_t indexArray[CONFIG_MACHINE_PARAMETER_NUM];
	BoolStatus isIndexMove;							//����ֵ�ƶ�
	uint8_t curParameterNum;						//��������
	uint8_t putinNum;								//�����ַ�����
	const char * pTitle;							//����
	LEAVE_PAGE_TypeDef leavePage;					//�뿪ҳ
	BoolStatus isCancelPutin;
	SMPL_NAME_TypeDef channel;
	FunctionalState refreshShortcut;				//ˢ�¿�ݲ˵�
}CONFIG_MACHINE_TypeDef;


/* Private constants ---------------------------------------------------------*/
const char * const pModelTypeName[] =
{
	"1������ʽ���ܻ�",
	"2��Һѹʽ���ܻ�",
};

const char * const pMachineConfigName[] =
{
	"������λ��",
	"���λ�ƣ�",
	"�����Σ�",
};

const char * const pConfigMachineWarn[] = 
{
	"��λ���뷶Χ��1 ~ 1000",		//0
	"���λ�����뷶Χ��1 ~ 10000",	//1
	"���������뷶Χ��1 ~ 10000",	//2
};

const char * const pModelSelectErrorCue[] =
{
	"���û��ͺ����������������",
	"�Ƿ�������",
};

const char * const pModelSelectMenuCue[] = 
{
	"  �� ��  ",
	"  ȡ ��  ",
};
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MODEL_SELECT_TypeDef g_modelSelect;
static CONFIG_MACHINE_TypeDef g_configMachine;

/* Private function prototypes -----------------------------------------------*/
static void ModelSelectInit( void );
static void ModelSelectConfig( void );
static void ModelSelectReadParameter( void );
static void GUI_ModelSelect( void );
static void ModelSelectMoveIndexProcess( void );
static void ModelSelectMoveCursorProcess( void );
static void ModelSelectKeyProcess( void );
static void ModelSelectLeavePageCheckCycle( void );

static void SaveDefaultParameter( void );

static void LoadConfigMachineParameterPage( void );
static void ConfigMachineParameterInit( void );
static void ConfigMachineParameterConfig( void );
static void ConfigMachineParameterReadParameter( void );
static void ConfigMachineParameterWriteParameter( void );
static void GUI_ConfigMachineParameter( void );
static void Traverse_ConfigMachineParameter( void );
static void ConfigMachineParameterPutinProcess( void );
static void ConfigMachineParameterMoveIndexProcess( void );
static void ConfigMachineParameterMoveCursorProcess( void );
static void ConfigMachineParameterKeyProcess( void );
static void ConfigMachineParameterLeavePageCheckCycle( void );
static uint8_t GetConfigMachineParameterIndex( uint8_t handle );
static void ConfigMachineParameterShortcutCycleTask( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadModelSelectPage
 * Description    : ����ѡ��ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadModelSelectPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);

	/* ������ʼ�� */
	ModelSelectInit();
	
	/* �������� */
	ModelSelectConfig();
	
	/* ��ȡ���� */
	ModelSelectReadParameter();
	
	/* ��GUI��� */
	GUI_ModelSelect();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_modelSelect.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		PutinProcessCycle();
		
		/* �ƶ�����ֵ */
		ModelSelectMoveIndexProcess();
		
		/* �ƶ���� */
		ModelSelectMoveCursorProcess();
		
		/* �������� */
		ModelSelectKeyProcess();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		ModelSelectLeavePageCheckCycle();
	}
}


/*------------------------------------------------------------
 * Function Name  : GetModelType
 * Description    : ��ȡ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
MODEL_TYPE_TypeDef GetModelType( void )
{
	MODEL_TYPE_TypeDef Type = MODEL_UNDEFINED;
	uint8_t Model = 0;	
	
	Model = devc_token_get();
	
	switch ( Model )
	{
		case MODEL_KY_DZ:
			Type = MODEL_KY;
			break;
		case MODEL_KZ_DZ:
			Type = MODEL_KZ;
			break;
		case MODEL_KZKY_DZ:
			Type = MODEL_KZKY;
			break;
		case MODEL_KY_YY:
			Type = MODEL_KY;
			break;
		case MODEL_KZKY_YY:
			Type = MODEL_KZKY;
			break;
		case MODEL_UNIVERSAL_DZ:
			Type = MODEL_UNIVERSAL;
			break;
		case MODEL_UNIVERSAL_YY:
			Type = MODEL_UNIVERSAL;
			break;
		default:
			Type = MODEL_UNDEFINED;
			break;
	}
	
	return Type;
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectInit( void )
{
	g_modelSelect.nowIndex = 0;
	g_modelSelect.recordIndex = 0xff;
	
	g_modelSelect.isIndexMove = NO;
	
	g_modelSelect.leavePage.flagLeavePage = RESET;
	g_modelSelect.leavePage.flagSaveData = RESET;	
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectConfig
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectConfig( void )
{
	/* �Կ���� */
	g_modelSelect.menuCnt = MAX_MENU_NUM;
	
	/* ���� */
	g_modelSelect.rowNum = MAX_MENU_NUM;
	
	/* ����ֵ */		
	g_modelSelect.indexArray[INDEX_UNIVERSAL_DZ] 			= MODEL_UNIVERSAL_DZ;	
	g_modelSelect.indexArray[INDEX_UNIVERSAL_YY] 			= MODEL_UNIVERSAL_YY;		
	
	/* ���� */
	g_modelSelect.pParameterNameArray[INDEX_UNIVERSAL_DZ]	= pModelTypeName[0];	
	g_modelSelect.pParameterNameArray[INDEX_UNIVERSAL_YY] 	= pModelTypeName[1];		
	
	/* ���� */
	g_modelSelect.pTitle = "����ѡ��";
}

/*------------------------------------------------------------
 * Function Name  : GetModelSelectIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetModelSelectIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_modelSelect.menuCnt; ++i)
	{
		if (g_modelSelect.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectReadParameter
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectReadParameter( void )
{
	uint8_t index = 0;
	uint8_t model = 0;	
	
	model = devc_token_get();
	
	index = GetModelSelectIndex(model);
	if (index != 0xff)
	{
		g_modelSelect.nowIndex = index;
	}
	else
	{
		g_modelSelect.nowIndex = 0;
	}
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectWriteParameter
 * Description    : д�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectWriteParameter( void )
{
	uint8_t model = 0;	
	
	model = g_modelSelect.indexArray[g_modelSelect.nowIndex];
	
	devc_token_set(model);
	
	prm_save();
}

/*------------------------------------------------------------
 * Function Name  : ConfigModelSelectRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigModelSelectRectangleFrameCoordinate( void )
{
	const uint16_t INIT_START_X = 60;
	const uint16_t INIT_START_Y = 81;
	uint16_t startX = INIT_START_X;
	uint16_t startY = INIT_START_Y;
	uint8_t i;
	
	for (i=0; i<g_modelSelect.menuCnt; ++i)
	{
		g_modelSelect.oneLevelMenu[i].x = startX;
		g_modelSelect.oneLevelMenu[i].y = startY;
		g_modelSelect.oneLevelMenu[i].pointColor = COLOR_BACK;
		g_modelSelect.oneLevelMenu[i].backColor = COLOR_POINT;
		g_modelSelect.oneLevelMenu[i].recordPointColor = COLOR_BACK;
		g_modelSelect.oneLevelMenu[i].recordBackColor = COLOR_POINT;
		g_modelSelect.oneLevelMenu[i].lenth = 320;
		g_modelSelect.oneLevelMenu[i].width = 36;
		g_modelSelect.oneLevelMenu[i].fontSize = 32;
		g_modelSelect.oneLevelMenu[i].rowDistance = 37;
		g_modelSelect.oneLevelMenu[i].columnDistance = 0;
		
		startY += g_modelSelect.oneLevelMenu[i].rowDistance + g_modelSelect.oneLevelMenu[i].width;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ModelSelectDrawOneRectangleFrame
 * Description    : һ�н���GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ModelSelectDrawOneRectangleFrame( uint8_t index )
{
	const uint16_t x = g_modelSelect.oneLevelMenu[index].x;
	const uint16_t y = g_modelSelect.oneLevelMenu[index].y;
	const uint16_t lenth = g_modelSelect.oneLevelMenu[index].lenth;
	const uint16_t width = g_modelSelect.oneLevelMenu[index].width;
	const uint16_t pointColor = g_modelSelect.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_modelSelect.oneLevelMenu[index].backColor;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr32At(x+16,y+1,pointColor,backColor,g_modelSelect.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ModelSelectRectangleFrame
 * Description    : ������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ModelSelectRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_modelSelect.menuCnt; ++i)
	{
		GUI_ModelSelectDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ModelSelect
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ModelSelect( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_modelSelect.pTitle);
	
	ConfigModelSelectRectangleFrameCoordinate();
	
	GUI_ModelSelectRectangleFrame();
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectMoveIndexProcess
 * Description    : �ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectMoveIndexProcess( void )
{
	INDEX_MANAGE_TypeDef indexObj;
	
	g_modelSelect.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_modelSelect.rowNum;
	indexObj.colNum = 1;
	indexObj.sumNum = g_modelSelect.menuCnt;
	indexObj.pNowIndex = &g_modelSelect.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_modelSelect.nowIndex != g_modelSelect.recordIndex)
	{		
		g_modelSelect.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectMoveCursorProcess( void )
{
	if (g_modelSelect.isIndexMove == YES)
	{		
		if (g_modelSelect.recordIndex != 0xff)
		{
			g_modelSelect.oneLevelMenu[g_modelSelect.recordIndex].pointColor = g_modelSelect.oneLevelMenu[g_modelSelect.recordIndex].recordPointColor;
			g_modelSelect.oneLevelMenu[g_modelSelect.recordIndex].backColor = g_modelSelect.oneLevelMenu[g_modelSelect.recordIndex].recordBackColor;
			
			GUI_ModelSelectDrawOneRectangleFrame(g_modelSelect.recordIndex);
			
			g_modelSelect.oneLevelMenu[g_modelSelect.nowIndex].pointColor = COLOR_POINT;
			g_modelSelect.oneLevelMenu[g_modelSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_ModelSelectDrawOneRectangleFrame(g_modelSelect.nowIndex);
		}
		else
		{
			g_modelSelect.oneLevelMenu[g_modelSelect.nowIndex].pointColor = COLOR_POINT;
			g_modelSelect.oneLevelMenu[g_modelSelect.nowIndex].backColor = COLOR_SELECT_BACK;
			
			GUI_ModelSelectDrawOneRectangleFrame(g_modelSelect.nowIndex);
		}
		
		g_modelSelect.recordIndex = g_modelSelect.nowIndex;
	}
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectShortcutCheckCycle
 * Description    : ��ݼ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectShortcutCheckCycle( void )
{	
	uint8_t keyValue = 0;
	
	keyValue = GetPressNum();
	
	if ((keyValue>0) && (keyValue <= g_modelSelect.menuCnt))
	{
		g_modelSelect.leavePage.flagLeavePage = SET;
		g_modelSelect.leavePage.flagSaveData = SET;
		
		g_modelSelect.nowIndex = keyValue - 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectKeyProcess( void )
{
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				g_modelSelect.leavePage.flagLeavePage = SET;
				g_modelSelect.leavePage.flagSaveData = SET;
				break;
			case KEY_ESC:
				g_modelSelect.leavePage.flagLeavePage = SET;
				g_modelSelect.leavePage.flagSaveData = RESET;
				break;
		}
	}
	
	ModelSelectShortcutCheckCycle();
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectPutinTons( void )
{
	uint8_t Model = 0;	
	
	Model = g_modelSelect.indexArray[g_modelSelect.nowIndex];
	
	switch ( Model )
	{
		case MODEL_UNIVERSAL_DZ:
			break;
		case MODEL_UNIVERSAL_YY:			
			break;
		default:
			break;
	}
	
	LoadConfigMachineParameterPage();
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectLeavePageCheckCycle( void )
{
	if (g_modelSelect.leavePage.flagLeavePage == SET)
	{
		if (g_modelSelect.leavePage.flagSaveData == SET)
		{			
			ModelSelectPutinTons();
			
			if (g_configMachine.isCancelPutin == YES)
			{
				SetPage(MODEL_SELECT_PAGE);
				
				return;
			}
			else
			{
				ModelSelectWriteParameter();
				
				SaveDefaultParameter();
				
				/* ѡ���Ƿ����� */
				while (1)
				{
					SetSelectPopWindowsInfomation(POP_PCM_ASKING,2,&pModelSelectErrorCue[0],pModelSelectMenuCue);
					PopWindowsProcessCycle();
					if ( GetPopWindowsSelectResult() == YES)
					{
						SystemSoftwareReset();
					}
				}				
			}
		}
		else
		{
			SetPage(ADVANCE_PARAMETER_SET_PAGE);
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SetIPAddr
 * Description    : ����IP��ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetIPAddr( const uint8_t *pIP )
{
	uint8_t i;
	uint32_t ip_addr = 0;
	
	for (i=0; i<4; ++i)
	{
		ip_addr <<= 8;
		ip_addr |= pIP[i];
	}
	
	devc_ip_set(ip_addr);	
}

/*------------------------------------------------------------
 * Function Name  : DefaultParameterSet
 * Description    : ��ȡĬ�ϲ�������(���ݾ���NΪ��λ)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DefaultParameterSet( MODEL_ATTRIBUTE_TypeDef type, uint16_t ton, \
								 uint16_t maxDisplacement, uint16_t maxDeform )
{	
	const uint8_t DEFAULT_IP[] = {192,168,0,234};
	const uint8_t USE_CHANNEL = 3;
	uint8_t index = 0;
	
	//��λ������
	pHmi->test_standard_index = KYSNJS;
	pHmi->attenuationRate[SMPL_FH_NUM] = 50;
	pHmi->curveStayTime = 5;
	if ((ton>=1) && (ton<=10))
	{
		pHmi->fhUnit = FH_UNIT_N;
	}
	else
	{
		pHmi->fhUnit = FH_UNIT_kN;
	}
	pHmi->wyUnit = WY_UNIT_MM;
	pHmi->bxUnit = BX_UNIT_MM;
	pHmi->wyConvBx = 0;
	
	for (index=0; index<USE_CHANNEL; ++index)
	{
		switch ( index )
		{
			case SMPL_FH_NUM:
				if ( FH_UNIT_kN == GetFH_SmplUnit() )
				{
					pHmi->start_force[index] 		= 5000;
					pHmi->run_wait_time[index] 	  	= 180;
					pHmi->break_judge_value[index]  = 10000;
					pHmi->break_point[index] 	 	= 1;	
					pHmi->break_condition[index]    = 0;	
					pHmi->break_max_value[index]    = 5000;	
					pHmi->break_border_value[index] = 5000;
				}
				else
				{
					pHmi->start_force[index] 		= 100;
					pHmi->run_wait_time[index] 	  	= 180;
					pHmi->break_judge_value[index]  = 1000;
					pHmi->break_point[index] 	 	= 1;	
					pHmi->break_condition[index]    = 0;	
					pHmi->break_max_value[index]    = 500;	
					pHmi->break_border_value[index] = 500;
				}			
				break;
			
			case SMPL_WY_NUM:
				break;
			
			case SMPL_BX_NUM:					
				break;		
		}
	}
	
	switch ( type )
	{
		case HYDRAULIC_PRESS:
			spcl_quick_up_set(SMPL_KY_NUM,33500);		//������ʼ����
			spcl_quick_down_set(SMPL_KY_NUM,500);		//��������
			spcl_slow_up_set(SMPL_KY_NUM,33500);		//������ʼ����
			spcl_slow_down_set(SMPL_KY_NUM,100);		//��������
			break;
		
		case ELECTRONIC:
			pHmi->manual_fast_open[SMPL_KY_NUM] = 800;	
			pHmi->manual_fast_step[SMPL_KY_NUM] = 400;
			pHmi->manual_slow_open[SMPL_KY_NUM] = 800;
			pHmi->manual_slow_step[SMPL_KY_NUM] = 100;
			break;
	}
	
	//�豸����
	SetIPAddr(DEFAULT_IP);
	
	//���Ʋ���
	ctrl_max_set(50000);
	ctrl_max_cnt_set(0);
	ctrl_max_one_set(500);
	ctrl_min_one_set(1);
	switch ( type )
	{
		case HYDRAULIC_PRESS:
			spcl_unload_delay_set(SMPL_KY_NUM,3);
			spcl_valve_init_set(SMPL_KY_NUM,33500);
			spcl_valve_middle_set(SMPL_KY_NUM,20000);
			spcl_valve_back_set(SMPL_KY_NUM,1000);
			break;
		
		case ELECTRONIC:
			spcl_unload_delay_set(SMPL_KY_NUM,3);
			spcl_valve_init_set(SMPL_KY_NUM,800);
			spcl_valve_back_set(SMPL_KY_NUM,400);	
			break;
	}
					
	//����������	
	for (index=0; index<USE_CHANNEL; ++index)
	{
		switch ( index )
		{
			case SMPL_FH_NUM:
				if ( FH_UNIT_kN == GetFH_SmplUnit() )
				{
					smpl_ctrl_full_p_set(index,10000*ton);	//��λ��T -> N
					smpl_ctrl_full_n_set(index,(-10000)*ton);	
					smpl_ctrl_entry_set(index,5000);
					smpl_cof_ahead_set(index,1);
					smpl_cof_break_set(index,1);
					smpl_cof_spd_set(index,3000);
					smpl_ctrl_crash_set(index,10000);
					smpl_ctrl_overspeed_set(index,10000);	
					
					/* �궨�� */
					smpl_tab_num_set(index,2);
					smpl_tab_value_set(index,0,0);
					smpl_tab_code_set(index,0,0);	
					smpl_tab_value_set(index,1,10000*ton);		
					smpl_tab_code_set(index,1,80000);
					
					/* PID */
					smpl_pid_segs_set(index,1);
					smpl_pid_node_set(index,0,10000*ton);
					smpl_pid_kp_set(index,0,800);
					smpl_pid_ki_set(index,0,200);
					smpl_pid_kd_set(index,0,0);
				}
				else
				{
					smpl_ctrl_full_p_set(index,10000*ton);	//��λ��T -> N
					smpl_ctrl_full_n_set(index,(-10000)*ton);	
					smpl_ctrl_entry_set(index,100);
					smpl_cof_ahead_set(index,1);
					smpl_cof_break_set(index,1);
					smpl_cof_spd_set(index,3000);
					smpl_ctrl_crash_set(index,500);
					smpl_ctrl_overspeed_set(index,500);	
					
					/* �궨�� */
					smpl_tab_num_set(index,2);
					smpl_tab_value_set(index,0,0);
					smpl_tab_code_set(index,0,0);
					smpl_tab_value_set(index,1,10000*ton);							
					smpl_tab_code_set(index,1,80000);
					
					/* PID */
					smpl_pid_segs_set(index,1);
					smpl_pid_node_set(index,0,10000*ton);
					smpl_pid_kp_set(index,0,800);
					smpl_pid_ki_set(index,0,200);
					smpl_pid_kd_set(index,0,0);
				}			
				break;
		
			case SMPL_WY_NUM:
				smpl_ctrl_full_p_set(index,maxDisplacement);		
				smpl_ctrl_full_n_set(index,-maxDisplacement);	
				
				/* �궨�� */
				smpl_tab_num_set(index,2);
				smpl_tab_value_set(index,0,0);
				smpl_tab_code_set(index,0,0);	
				smpl_tab_value_set(index,1,maxDisplacement);		
				smpl_tab_code_set(index,1,80000);
				break;
				
			case SMPL_BX_NUM:
				smpl_ctrl_full_p_set(index,maxDeform);		
				smpl_ctrl_full_n_set(index,-maxDeform);	
			
				/* �궨�� */
				smpl_tab_num_set(index,2);
				smpl_tab_value_set(index,0,0);
				smpl_tab_code_set(index,0,0);	
				smpl_tab_value_set(index,1,maxDeform);		
				smpl_tab_code_set(index,1,80000);
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : SaveDefaultParameter
 * Description    : ����Ĭ�ϲ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SaveDefaultParameter( void )
{	
	uint8_t model = 0;
	uint16_t ton = 0;
	uint16_t maxDisplacement = 0;
	uint16_t maxDeform = 0;
	uint8_t index = 0;

	index = GetConfigMachineParameterIndex(OBJECT_MACHINE_TONS);
	if (index != 0xff)
	{
		ton = ustrtoul(g_configMachine.parameterData[index],0,10);
	}

	index = GetConfigMachineParameterIndex(OBJECT_MAX_DISPALCEMENT);
	if (index != 0xff)
	{
		maxDisplacement = ustrtoul(g_configMachine.parameterData[index],0,10);
	}
	
	index = GetConfigMachineParameterIndex(OBJECT_MAX_DEFORM);
	if (index != 0xff)
	{
		maxDeform = ustrtoul(g_configMachine.parameterData[index],0,10);
	}
	
	model = devc_token_get();
	
	switch ( model )
	{	
		case MODEL_UNIVERSAL_DZ:
			DefaultParameterSet(ELECTRONIC,ton,maxDisplacement,maxDeform);
			break;
		case MODEL_UNIVERSAL_YY:
			DefaultParameterSet(HYDRAULIC_PRESS,ton,maxDisplacement,maxDeform);
			break;
	}
	
	pcm_save();
	prm_save();
	prv_save();
}

/*------------------------------------------------------------
 * Function Name  : LoadConfigMachineParameterPage
 * Description    : ���û�������ҳ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadConfigMachineParameterPage( void )
{
	/* �ر���Ļ */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* ������ʼ�� */
	ConfigMachineParameterInit();
	
	/* �������� */
	ConfigMachineParameterConfig();
	
	/* ��ȡ���� */
	ConfigMachineParameterReadParameter();
	
	/* ��GUI��� */
	GUI_ConfigMachineParameter();
	
	/* ���� */
	Traverse_ConfigMachineParameter();
	
	/* ����Ļ */
	SetBackLightEffectOpen();
	
	while (g_configMachine.leavePage.flagLeavePage == RESET)
	{
		/* ���봦�� */
		ConfigMachineParameterPutinProcess();
		
		/* �ƶ����� */
		ConfigMachineParameterMoveIndexProcess();
		
		/* �ƶ���� */
		ConfigMachineParameterMoveCursorProcess();
		
		/* �������� */
		ConfigMachineParameterKeyProcess();
		
		/* ��ݲ˵� */
		ConfigMachineParameterShortcutCycleTask();
		
		/* �������� */
		PopWindowsProcessCycle();
		
		/* �뿪ҳ */
		ConfigMachineParameterLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterInit
 * Description    : ��ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterInit( void )
{	
	g_configMachine.nowIndex = 0;
	g_configMachine.recordIndex = 0xff;
	g_configMachine.leavePage.flagLeavePage = RESET;
	g_configMachine.leavePage.flagSaveData = RESET;
	g_configMachine.isCancelPutin = NO;
	g_configMachine.refreshShortcut = ENABLE;
	
	InitKeyCheck();
}

/*------------------------------------------------------------
 * Function Name  : GetConfigMachineParameterIndex
 * Description    : ��ȡ����
 * Input          : None
 * Output         : None
 * Return         : 0xff����ʾδ�ҵ�������
 *------------------------------------------------------------*/
static uint8_t GetConfigMachineParameterIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//�����ֵ
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		if (g_configMachine.indexArray[i] == handle)
		{
			index = i;
			break;
		}
	}
	
	return index;
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterConfig
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterConfig( void )
{
	g_configMachine.pTitle = "��������";
	
	/* �������� */
	g_configMachine.curParameterNum = CONFIG_MACHINE_PARAMETER_NUM;
	
	/* ����ֵ */
	g_configMachine.indexArray[INDEX_MACHINE_TONS] 		= OBJECT_MACHINE_TONS;
	g_configMachine.indexArray[INDEX_MAX_DISPALCEMENT] 	= OBJECT_MAX_DISPALCEMENT;
	g_configMachine.indexArray[INDEX_MAX_DEFORM] 		= OBJECT_MAX_DEFORM;
	
	/* ������ */
	g_configMachine.pParameterNameArray[INDEX_MACHINE_TONS] 	= pMachineConfigName[0];
	g_configMachine.pParameterNameArray[INDEX_MAX_DISPALCEMENT] = pMachineConfigName[1];
	g_configMachine.pParameterNameArray[INDEX_MAX_DEFORM] 		= pMachineConfigName[2];
	
	/* �������� */
	g_configMachine.oneLevelMenu[INDEX_MACHINE_TONS].saveType 		= TYPE_INT;
	g_configMachine.oneLevelMenu[INDEX_MAX_DISPALCEMENT].saveType 	= TYPE_INT;
	g_configMachine.oneLevelMenu[INDEX_MAX_DEFORM].saveType 		= TYPE_INT;
	
	/* ��λ */
	g_configMachine.pParameterUnitArray[INDEX_MACHINE_TONS] 	= pUnitType[11];
	g_configMachine.pParameterUnitArray[INDEX_MAX_DISPALCEMENT] = pUnitType[4];
	g_configMachine.pParameterUnitArray[INDEX_MAX_DEFORM] 		= pUnitType[4];
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterReadParameter
 * Description    : ������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterReadParameter( void )
{
	uint8_t index = 0;
	
	index = GetConfigMachineParameterIndex(OBJECT_MACHINE_TONS);
	if (index != 0xff)
	{
		g_configMachine.parameterData[index][0] = NULL;
	}

	index = GetConfigMachineParameterIndex(OBJECT_MAX_DISPALCEMENT);
	if (index != 0xff)
	{
		g_configMachine.parameterData[index][0] = NULL;
	}
	
	index = GetConfigMachineParameterIndex(OBJECT_MAX_DEFORM);
	if (index != 0xff)
	{
		g_configMachine.parameterData[index][0] = NULL;
	}
}	

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterWriteParameter
 * Description    : д����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterWriteParameter( void )
{
	;
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterRectangleFrameCoordinate
 * Description    : ���ý���GUI����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 147;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		g_configMachine.oneLevelMenu[i].x = startX;
		g_configMachine.oneLevelMenu[i].y = startY;
		g_configMachine.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_configMachine.oneLevelMenu[i].backColor = COLOR_BACK;
		g_configMachine.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_configMachine.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_configMachine.oneLevelMenu[i].lenth = 114;
		g_configMachine.oneLevelMenu[i].width = 30;
		g_configMachine.oneLevelMenu[i].fontSize = 24;
		g_configMachine.oneLevelMenu[i].rowDistance = 48;
		g_configMachine.oneLevelMenu[i].columnDistance = 48;
		g_configMachine.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_configMachine.oneLevelMenu[i].width + g_configMachine.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ConfigMachineParameterDrawOneRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ConfigMachineParameterDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_configMachine.oneLevelMenu[index].x;
	rectangle.y = g_configMachine.oneLevelMenu[index].y;
	rectangle.lenth = g_configMachine.oneLevelMenu[index].lenth;
	rectangle.width = g_configMachine.oneLevelMenu[index].width;
	rectangle.lineWidth = g_configMachine.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_configMachine.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ConfigMachineParameterRectangleFrame
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ConfigMachineParameterRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		GUI_ConfigMachineParameterDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_ConfigMachineParameterDrawOneRowOneLevelMenu
 * Description    : ��������GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ConfigMachineParameterDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_configMachine.oneLevelMenu[index].x - 5 * g_configMachine.oneLevelMenu[index].fontSize;
	const uint16_t y = g_configMachine.oneLevelMenu[index].y + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_configMachine.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_configMachine.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_configMachine.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ConfigMachineParameterOneLevelMenu
 * Description    : ��������һ���˵�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ConfigMachineParameterOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		GUI_ConfigMachineParameterDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_ConfigMachineParameter
 * Description    : ����GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_ConfigMachineParameter( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_configMachine.pTitle);
	
	ConfigMachineParameterRectangleFrameCoordinate();
	
	GUI_ConfigMachineParameterRectangleFrame();
	
	GUI_ConfigMachineParameterOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_ConfigMachineParameterOneRowOneLevelMenuUnit
 * Description    : ��ʾһ�����������λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ConfigMachineParameterOneRowOneLevelMenuUnit( uint8_t index )
{
	const uint16_t x = g_configMachine.oneLevelMenu[index].x + g_configMachine.oneLevelMenu[index].lenth - \
					   g_configMachine.oneLevelMenu[index].lineWidth - 5 * 12;
	const uint16_t y = g_configMachine.oneLevelMenu[index].y + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_configMachine.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_configMachine.oneLevelMenu[index].backColor;
	
	if ( strcmp(g_configMachine.pParameterUnitArray[index],"NULL") )
	{
		GUI_DispStr24At(x,y,pointColor,backColor,g_configMachine.pParameterUnitArray[index]);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_TestParameterOneLevelMenuUnit
 * Description    : ��ʾһ���˵���λ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ConfigMachineParameterOneLevelMenuUnit( void )
{
	uint8_t i;
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		Show_ConfigMachineParameterOneRowOneLevelMenuUnit(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : Show_ConfigMachineParameterDataOneRowOneLevelMenuContent
 * Description    : ��ʾһ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ConfigMachineParameterDataOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_configMachine.oneLevelMenu[index].x + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_configMachine.oneLevelMenu[index].y + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_configMachine.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_configMachine.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_configMachine.oneLevelMenu[index].lenth - 2 * g_configMachine.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_configMachine.oneLevelMenu[index].width - 2 * g_configMachine.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_configMachine.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_ConfigMachineParameterDataOneLevelMenuContent
 * Description    : ��ʾһ���˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_ConfigMachineParameterDataOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_configMachine.curParameterNum; ++i)
	{
		Show_ConfigMachineParameterDataOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_ConfigMachineParameter
 * Description    : ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_ConfigMachineParameter( void )
{
	Show_ConfigMachineParameterDataOneLevelMenuContent();
	Show_ConfigMachineParameterOneLevelMenuUnit();
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterPutinProcess
 * Description    : ���봦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_configMachine.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_configMachine.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_configMachine.oneLevelMenu[index].x + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_configMachine.oneLevelMenu[index].y + g_configMachine.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_CONFIG_MACHINE_PUTIN_BIT;
	pPutin->FillBit = MAX_CONFIG_MACHINE_PUTIN_BIT;
	pPutin->putinFrameLenth = g_configMachine.oneLevelMenu[index].lenth - 2 * g_configMachine.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_configMachine.oneLevelMenu[index].width - 2 * g_configMachine.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_configMachine.putinNum;
	pPutin->SaveType = g_configMachine.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_configMachine.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterMoveIndexProcess
 * Description    : �����ƶ�����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterMoveIndexProcess( void )
{	
	INDEX_MANAGE_TypeDef indexObj;
	
	g_configMachine.isIndexMove = NO;
	
	indexObj.enableMoveIndex = ENABLE;
	indexObj.rowNum = g_configMachine.curParameterNum;
	indexObj.colNum = 1;
	indexObj.sumNum = indexObj.rowNum * indexObj.colNum;
	indexObj.pNowIndex = &g_configMachine.nowIndex;
		
	KeyIndexManage(&indexObj);
	
	if (g_configMachine.nowIndex != g_configMachine.recordIndex)
	{		
		g_configMachine.isIndexMove = YES;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterMoveCursorProcess
 * Description    : �ƶ���괦��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterMoveCursorProcess( void )
{
	if (g_configMachine.isIndexMove == YES)
	{		
		if (g_configMachine.recordIndex != 0xff)
		{
			g_configMachine.oneLevelMenu[g_configMachine.recordIndex].pointColor = g_configMachine.oneLevelMenu[g_configMachine.recordIndex].recordPointColor;
			g_configMachine.oneLevelMenu[g_configMachine.recordIndex].backColor = g_configMachine.oneLevelMenu[g_configMachine.recordIndex].recordBackColor;
			
			Show_ConfigMachineParameterDataOneRowOneLevelMenuContent(g_configMachine.recordIndex);
			Show_ConfigMachineParameterOneRowOneLevelMenuUnit(g_configMachine.recordIndex);
			
			g_configMachine.oneLevelMenu[g_configMachine.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_configMachine.oneLevelMenu[g_configMachine.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ConfigMachineParameterDataOneRowOneLevelMenuContent(g_configMachine.nowIndex);		
			Show_ConfigMachineParameterOneRowOneLevelMenuUnit(g_configMachine.nowIndex);
		}
		else
		{
			g_configMachine.oneLevelMenu[g_configMachine.nowIndex].pointColor = COLOR_SELECT_POINT;
			g_configMachine.oneLevelMenu[g_configMachine.nowIndex].backColor = COLOR_SELECT_BACK;
			
			Show_ConfigMachineParameterDataOneRowOneLevelMenuContent(g_configMachine.nowIndex);
			Show_ConfigMachineParameterOneRowOneLevelMenuUnit(g_configMachine.nowIndex);
		}
		
		g_configMachine.recordIndex = g_configMachine.nowIndex;
	}	
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterShortcutCycleTask
 * Description    : ��ݲ˵�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterShortcutCycleTask( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();
	
	if (g_configMachine.refreshShortcut == ENABLE)
	{
		g_configMachine.refreshShortcut = DISABLE;
			
		pShortCut->status = SHOW_F3 | SHOW_F4;
		pShortCut->pointColor = COLOR_SHORTCUT_POINT;
		pShortCut->backColor = COLOR_SHORTCUT_BACK;
		pShortCut->pContent[2] = pTwoLevelMenu[45];
		pShortCut->pContent[3] = pTwoLevelMenu[58];
		
		ShortcutMenuTask(pShortCut);
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterIncreaseIndex
 * Description    : ��������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterIncreaseIndex( void )
{
	g_configMachine.nowIndex++;
	
	g_configMachine.nowIndex %= g_configMachine.curParameterNum;
}	

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterUpdateStatus
 * Description    : ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterUpdateStatus( void )
{	
	ConfigMachineParameterIncreaseIndex();
	
	g_configMachine.refreshShortcut = ENABLE;
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterCheckDataCycle
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus ConfigMachineParameterCheckDataCycle( void )
{
	uint16_t tempu16 = 0;
	uint8_t index = 0;
	
	index = GetConfigMachineParameterIndex(OBJECT_MACHINE_TONS);
	if (index != 0xff)
	{
		tempu16 = (uint16_t)ustrtoul(g_configMachine.parameterData[index],0,10);
		
		if ((tempu16<1) || (tempu16>1000))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pConfigMachineWarn[0]);	
						
			return FAILED;
		}
	}
	
	index = GetConfigMachineParameterIndex(OBJECT_MAX_DISPALCEMENT);
	if (index != 0xff)
	{
		tempu16 = (uint16_t)ustrtoul(g_configMachine.parameterData[index],0,10);
		
		if ((tempu16<1) || (tempu16>10000))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pConfigMachineWarn[1]);	
						
			return FAILED;
		}
	}
	
	index = GetConfigMachineParameterIndex(OBJECT_MAX_DEFORM);
	if (index != 0xff)
	{
		tempu16 = (uint16_t)ustrtoul(g_configMachine.parameterData[index],0,10);
		
		if ((tempu16<1) || (tempu16>10000))
		{
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pConfigMachineWarn[2]);	
						
			return FAILED;
		}
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterKeyProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterKeyProcess( void )
{
	uint8_t index = g_configMachine.nowIndex;
	STATUS_PUTIN_TypeDef putinStatus;
	
	if (IsPressKey() == YES)
	{
		switch ( GetKeyVal() )
		{
			case KEY_ENTER:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						return;
					
					case STATUS_EDIT_COMP:							
						switch ( g_configMachine.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_configMachine.putinNum,*GetPutinIntDataAddr(),g_configMachine.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_configMachine.putinNum,g_configMachine.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_configMachine.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_configMachine.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
								
				ConfigMachineParameterUpdateStatus();
				break;
			
			case KEY_F3:
				if (ConfigMachineParameterCheckDataCycle() == FAILED)
				{
					g_configMachine.leavePage.flagLeavePage = SET;
					g_configMachine.leavePage.flagSaveData = RESET;
					break;
				}
				
				g_configMachine.leavePage.flagLeavePage = SET;
				g_configMachine.leavePage.flagSaveData = SET;
				break;
			case KEY_F4:
				g_configMachine.leavePage.flagLeavePage = SET;
				g_configMachine.leavePage.flagSaveData = RESET;
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						g_configMachine.leavePage.flagLeavePage = SET;
						g_configMachine.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						ConfigMachineParameterUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineParameterLeavePageCheckCycle
 * Description    : �뿪ҳ���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineParameterLeavePageCheckCycle( void )
{
	if (g_configMachine.leavePage.flagLeavePage == SET)
	{
		if (g_configMachine.leavePage.flagSaveData == SET)
		{
			g_configMachine.isCancelPutin = NO;
			
			ConfigMachineParameterWriteParameter();
		}
		else
		{
			g_configMachine.isCancelPutin = YES;
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
