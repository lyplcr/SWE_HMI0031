/**
  ******************************************************************************
  * @file    ModelSelect.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-10 16:34:06
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   机型选择程序
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

#define MAX_MENU_NUM					5

#define MACHINE_TONS_NUM				1
#define MAX_MACHINE_TONS_PUTIN_BIT		3

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	INDEX_KY_DZ = 0,		//电子抗压机
	INDEX_KZ_DZ,			//电子抗折机
	INDEX_KZKY_DZ,			//电子式抗折抗压一体机
	INDEX_KY_YY,			//液压抗压机
	INDEX_KZKY_YY,			//液压式抗折抗压一体机
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
	BoolStatus isIndexMove;							//索引值移动
	LEAVE_PAGE_TypeDef leavePage;						
}MODEL_SELECT_TypeDef;

typedef struct
{
	uint8_t nowIndex;
	uint8_t recordIndex;
	char parameterData[MACHINE_TONS_NUM][MAX_MACHINE_TONS_PUTIN_BIT+1];
	ONE_LEVEL_MENU_TYPE_TypeDef oneLevelMenu[MACHINE_TONS_NUM];
	const char *pParameterNameArray[MACHINE_TONS_NUM];
	uint8_t curParameterNum;						//参数个数
	uint8_t putinNum;								//输入字符个数
	const char * pTitle;							//标题
	LEAVE_PAGE_TypeDef leavePage;					//离开页
	BoolStatus isCancelPutin;
	SMPL_NAME_TypeDef2 channel;
	uint16_t putinKYTons;
	uint16_t putinKZTons;
}MACHINE_TONS_TypeDef;


/* Private constants ---------------------------------------------------------*/
const char * const pModelTypeName[] =
{
	"1、电子式抗压机",
	"2、电子式抗折机",
	"3、电子式抗折抗压机",
	"4、液压式抗压机",
	"5、液压式抗折抗压机",
};

const char * const pMachineTonsName[] =
{
	"抗压机器吨位：",
	"抗折机器吨位：",
};

const char * const pMachineTonsWarn[] = 
{
	"吨位输入范围：1 ~ 1000 T",		//0
};

const char * const pModelSelectErrorCue[] =
{
	"配置机型后必须重启控制器，",
	"是否重启？",
};

const char * const pModelSelectMenuCue[] = 
{
	"  重启  ",
	"  取消  ",
};
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MODEL_SELECT_TypeDef g_modelSelect;
static MACHINE_TONS_TypeDef g_machineTons;

/* Private function prototypes -----------------------------------------------*/
static void ModelSelectInit( void );
static void ModelSelectConfig( void );
static void ModelSelectReadParameter( void );
static void GUI_ModelSelect( void );
static void ModelSelectMoveIndexProcess( void );
static void ModelSelectMoveCursorProcess( void );
static void ModelSelectKeyProcess( void );
static void ModelSelectLeavePageCheckCycle( void );
static void MachineTonsInit( void );
static void MachineTonsParameterConfig( void );
static void MachineTonsReadParameter( void );
static void GUI_MachineTons( void );
static void MachineTonsPutinProcess( void );
static void Traverse_MachineTons( void );
static void MachineTonsMoveCursorProcess( void );
static void MachineTonsKeyProcess( void );
static void MachineTonsLeavePageCheckCycle( void );
static void LoadPutinMachineTonsPage( void );
static void SetMachineTonsChannel( SMPL_NAME_TypeDef2 channel );
static void SaveDefaultParameter( void );

/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : LoadModelSelectPage
 * Description    : 机型选择页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LoadModelSelectPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);

	/* 参数初始化 */
	ModelSelectInit();
	
	/* 参数配置 */
	ModelSelectConfig();
	
	/* 获取参数 */
	ModelSelectReadParameter();
	
	/* 画GUI框架 */
	GUI_ModelSelect();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_modelSelect.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		PutinProcessCycle();
		
		/* 移动索引值 */
		ModelSelectMoveIndexProcess();
		
		/* 移动光标 */
		ModelSelectMoveCursorProcess();
		
		/* 按键处理 */
		ModelSelectKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		ModelSelectLeavePageCheckCycle();
	}
}


/*------------------------------------------------------------
 * Function Name  : GetModelType
 * Description    : 获取机型类型
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
MODEL_TYPE_TypeDef GetModelType( void )
{
	MODEL_TYPE_TypeDef Type = MODEL_KY;
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
		
		default:
			Type = MODEL_KY;
			break;
	}
	
	return Type;
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectInit
 * Description    : 初始化
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
 * Description    : 配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ModelSelectConfig( void )
{
	/* 试块个数 */
	g_modelSelect.menuCnt = MAX_MENU_NUM;
	
	/* 行数 */
	g_modelSelect.rowNum = MAX_MENU_NUM;
	
	/* 索引值 */		
	g_modelSelect.indexArray[INDEX_KY_DZ] 		= MODEL_KY_DZ;	
	g_modelSelect.indexArray[INDEX_KZ_DZ] 		= MODEL_KZ_DZ;		
	g_modelSelect.indexArray[INDEX_KZKY_DZ] 	= MODEL_KZKY_DZ;		
	g_modelSelect.indexArray[INDEX_KY_YY] 		= MODEL_KY_YY;			
	g_modelSelect.indexArray[INDEX_KZKY_YY] 	= MODEL_KZKY_YY;
	
	/* 名称 */
	g_modelSelect.pParameterNameArray[INDEX_KY_DZ]		= pModelTypeName[0];	
	g_modelSelect.pParameterNameArray[INDEX_KZ_DZ] 		= pModelTypeName[1];		
	g_modelSelect.pParameterNameArray[INDEX_KZKY_DZ] 	= pModelTypeName[2];		
	g_modelSelect.pParameterNameArray[INDEX_KY_YY] 		= pModelTypeName[3];			
	g_modelSelect.pParameterNameArray[INDEX_KZKY_YY] 	= pModelTypeName[4];
	
	/* 标题 */
	g_modelSelect.pTitle = "机型选择";
}

/*------------------------------------------------------------
 * Function Name  : GetModelSelectIndex
 * Description    : 获取索引
 * Input          : None
 * Output         : None
 * Return         : 0xff：表示未找到配置项
 *------------------------------------------------------------*/
static uint8_t GetModelSelectIndex( uint8_t handle )
{
	uint8_t i;
	uint8_t index = 0xff;	//错误的值
	
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
 * Description    : 读取参数
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
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectWriteParameter
 * Description    : 写入参数
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
 * Description    : 配置界面GUI坐标
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
 * Description    : 一行界面GUI
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
 * Description    : 画界面GUI
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
 * Description    : 界面GUI
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
 * Description    : 移动索引值
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
 * Description    : 移动光标处理
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
 * Description    : 快捷键检测
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
 * Description    : 按键处理
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
 * Description    : 离开页检测
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
		case MODEL_KY_DZ:
			SetMachineTonsChannel(SMPL_KY_NUM);
			break;
		case MODEL_KZ_DZ:
			SetMachineTonsChannel(SMPL_KZ_NUM);
			break;
		case MODEL_KZKY_DZ:
			SetMachineTonsChannel(SMPL_KY_NUM);
			LoadPutinMachineTonsPage();
			if (g_machineTons.isCancelPutin == YES)
			{
				return;
			}
			SetMachineTonsChannel(SMPL_KZ_NUM);
			break;
		case MODEL_KY_YY:
			SetMachineTonsChannel(SMPL_KY_NUM);
			break;
		case MODEL_KZKY_YY:
			SetMachineTonsChannel(SMPL_KY_NUM);
			LoadPutinMachineTonsPage();
			if (g_machineTons.isCancelPutin == YES)
			{
				return;
			}
			SetMachineTonsChannel(SMPL_KZ_NUM);
			break;
	}
	
	LoadPutinMachineTonsPage();
}

/*------------------------------------------------------------
 * Function Name  : ModelSelectLeavePageCheckCycle
 * Description    : 离开页检测
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
			
			if (g_machineTons.isCancelPutin == YES)
			{
				SetPage(MODEL_SELECT_PAGE);
				
				return;
			}
			else
			{
				ModelSelectWriteParameter();
				
				SaveDefaultParameter();
				
				/* 选择是否重启 */
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
 * Description    : 设置IP地址
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
 * Description    : 获取默认参数设置(数据均以N为单位)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void DefaultParameterSet( SMPL_NAME_TypeDef2 ch, MODEL_ATTRIBUTE_TypeDef type, uint16_t ton )
{	
	const uint8_t DEFAULT_IP[] = {192,168,0,234};
	
	//上位机参数
	switch ( ch )
	{
		case SMPL_KY_NUM:
			pHmi->test_standard_index = KYSNJS;
			if ( UNIT_kN==GetCurChannel() )
			{
				pHmi->start_force[SMPL_KY_NUM] 		  = 5000;
				pHmi->run_wait_time[SMPL_KY_NUM] 	  = 180;
				pHmi->break_judge_value[SMPL_KY_NUM]  = 10000;
				pHmi->break_point[SMPL_KY_NUM] 	 	  = 1;	
				pHmi->break_condition[SMPL_KY_NUM]    = 0;	
				pHmi->break_max_value[SMPL_KY_NUM]    = 1000;	
				pHmi->break_border_value[SMPL_KY_NUM] = 1000;
			}
			else
			{
				pHmi->start_force[SMPL_KY_NUM] 		  = 100;
				pHmi->run_wait_time[SMPL_KY_NUM] 	  = 180;
				pHmi->break_judge_value[SMPL_KY_NUM]  = 1000;
				pHmi->break_point[SMPL_KY_NUM] 	 	  = 1;	
				pHmi->break_condition[SMPL_KY_NUM]    = 0;	
				pHmi->break_max_value[SMPL_KY_NUM]    = 100;	
				pHmi->break_border_value[SMPL_KY_NUM] = 100;
			}
			pHmi->attenuationRate[SMPL_KY_NUM] = 50;
			break;
		
		case SMPL_KZ_NUM:
			pHmi->test_standard_index = KZSNJS;
			pHmi->start_force[SMPL_KZ_NUM] 		  = 100;
			pHmi->run_wait_time[SMPL_KZ_NUM] 	  = 180;
			pHmi->break_judge_value[SMPL_KZ_NUM]  = 1000;
			pHmi->break_point[SMPL_KZ_NUM] 	 	  = 1;	
			pHmi->break_condition[SMPL_KZ_NUM]    = 0;	
			pHmi->break_max_value[SMPL_KZ_NUM]    = 100;	
			pHmi->break_border_value[SMPL_KZ_NUM] = 100;
			pHmi->attenuationRate[SMPL_KZ_NUM] = 50;
			break;
	}
	
	switch ( type )
	{
		case HYDRAULIC_PRESS:
			spcl_quick_up_set(ch,33500);		//快升初始开度
			spcl_quick_down_set(ch,500);		//快升步进
			spcl_slow_up_set(ch,33500);			//慢升初始开度
			spcl_slow_down_set(ch,100);			//慢升步进
			break;
		
		case ELECTRONIC:
			pHmi->manual_fast_open[ch] = 800;	
			pHmi->manual_fast_step[ch] = 400;
			pHmi->manual_slow_open[ch] = 800;
			pHmi->manual_slow_step[ch] = 100;
			break;
	}
	
	//设备参数
	SetIPAddr(DEFAULT_IP);
	
	//控制参数
	ctrl_max_set(50000);
	ctrl_max_cnt_set(0);
	ctrl_max_one_set(500);
	ctrl_min_one_set(1);
	switch ( type )
	{
		case HYDRAULIC_PRESS:
			switch ( ch )
			{
				case SMPL_KY_NUM:
					spcl_unload_delay_set(SMPL_KY_NUM,3);
					spcl_valve_init_set(SMPL_KY_NUM,33500);
					spcl_valve_middle_set(SMPL_KY_NUM,20000);
					spcl_valve_back_set(SMPL_KY_NUM,1000);
					break;
				
				case SMPL_KZ_NUM:
					spcl_unload_delay_set(SMPL_KZ_NUM,3);
					spcl_valve_init_set(SMPL_KZ_NUM,31500);
					spcl_valve_middle_set(SMPL_KZ_NUM,20000);
					spcl_valve_back_set(SMPL_KZ_NUM,1000);
					break;
			}
			break;
		
		case ELECTRONIC:
			spcl_unload_delay_set(ch,3);
			spcl_valve_init_set(ch,800);
			spcl_valve_back_set(ch,400);	
			break;
	}
					
	//传感器参数	
	switch ( ch )
	{
		case SMPL_KY_NUM:
			if ( UNIT_kN==GetCurChannel() )
			{
				smpl_ctrl_full_p_set(SMPL_KY_NUM,10000*ton);	//单位：T -> N
				smpl_ctrl_full_n_set(SMPL_KY_NUM,(-10000)*ton);	
				smpl_ctrl_entry_set(SMPL_KY_NUM,5000);
				smpl_cof_ahead_set(SMPL_KY_NUM,1);
				smpl_cof_break_set(SMPL_KY_NUM,1);
				smpl_cof_spd_set(SMPL_KY_NUM,3000);
				smpl_ctrl_crash_set(SMPL_KY_NUM,10000);
				smpl_ctrl_overspeed_set(SMPL_KY_NUM,10000);	
				smpl_tab_num_set(SMPL_KY_NUM,2);
				smpl_tab_value_set(SMPL_KY_NUM,0,0);
				smpl_tab_value_set(SMPL_KY_NUM,1,10000*ton);		
				smpl_tab_code_set(SMPL_KY_NUM,0,0);	
				smpl_pid_segs_set(SMPL_KY_NUM,1);
				smpl_pid_node_set(SMPL_KY_NUM,0,10000*ton);
				smpl_pid_kp_set(SMPL_KY_NUM,0,800);
				smpl_pid_ki_set(SMPL_KY_NUM,0,200);
				smpl_pid_kd_set(SMPL_KY_NUM,0,0);
			}
			else
			{
				smpl_ctrl_full_p_set(SMPL_KY_NUM,10000*ton);	//单位：T -> N
				smpl_ctrl_full_n_set(SMPL_KY_NUM,(-10000)*ton);	
				smpl_ctrl_entry_set(SMPL_KY_NUM,100);
				smpl_cof_ahead_set(SMPL_KY_NUM,1);
				smpl_cof_break_set(SMPL_KY_NUM,1);
				smpl_cof_spd_set(SMPL_KY_NUM,3000);
				smpl_ctrl_crash_set(SMPL_KY_NUM,500);
				smpl_ctrl_overspeed_set(SMPL_KY_NUM,500);	
				smpl_tab_num_set(SMPL_KY_NUM,2);
				smpl_tab_value_set(SMPL_KY_NUM,0,0);
				smpl_tab_value_set(SMPL_KY_NUM,1,10000*ton);		
				smpl_tab_code_set(SMPL_KY_NUM,0,0);	
				smpl_pid_segs_set(SMPL_KY_NUM,1);
				smpl_pid_node_set(SMPL_KY_NUM,0,10000*ton);
				smpl_pid_kp_set(SMPL_KY_NUM,0,800);
				smpl_pid_ki_set(SMPL_KY_NUM,0,200);
				smpl_pid_kd_set(SMPL_KY_NUM,0,0);
			}
			
			switch ( type )
			{
				case HYDRAULIC_PRESS:
					smpl_tab_code_set(SMPL_KY_NUM,1,80000);
					break;
				
				case ELECTRONIC:
					smpl_tab_code_set(SMPL_KY_NUM,1,80000);
					break;
			}			
			break;
	
		case SMPL_KZ_NUM:
			smpl_ctrl_full_p_set(SMPL_KZ_NUM,10000*ton);	//单位：T -> N
			smpl_ctrl_full_n_set(SMPL_KZ_NUM,(-10000)*ton);	
			smpl_ctrl_entry_set(SMPL_KZ_NUM,100);
			smpl_cof_ahead_set(SMPL_KZ_NUM,1);
			smpl_cof_break_set(SMPL_KZ_NUM,1);
			smpl_cof_spd_set(SMPL_KZ_NUM,3000);
			smpl_ctrl_crash_set(SMPL_KZ_NUM,500);
			smpl_ctrl_overspeed_set(SMPL_KZ_NUM,500);	
			smpl_tab_num_set(SMPL_KZ_NUM,2);
			smpl_tab_value_set(SMPL_KZ_NUM,0,0);
			smpl_tab_value_set(SMPL_KZ_NUM,1,10000*ton);		
			smpl_tab_code_set(SMPL_KZ_NUM,0,0);	
			smpl_pid_segs_set(SMPL_KZ_NUM,1);
			smpl_pid_node_set(SMPL_KZ_NUM,0,10000*ton);
			smpl_pid_kp_set(SMPL_KZ_NUM,0,800);
			smpl_pid_ki_set(SMPL_KZ_NUM,0,200);
			smpl_pid_kd_set(SMPL_KZ_NUM,0,0);
			switch ( type )
			{
				case HYDRAULIC_PRESS:
					smpl_tab_code_set(SMPL_KZ_NUM,1,80000);		//抗折通道的传感器都是负荷传感器
					break;
				
				case ELECTRONIC:
					smpl_tab_code_set(SMPL_KZ_NUM,1,80000);
					break;
			}			
			break;
	}
}


/*------------------------------------------------------------
 * Function Name  : SaveDefaultParameter
 * Description    : 保存默认参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SaveDefaultParameter( void )
{	
	uint8_t model = 0;

	model = devc_token_get();
	
	switch ( model )
	{
		case MODEL_KY_DZ:
			DefaultParameterSet(SMPL_KY_NUM,ELECTRONIC,g_machineTons.putinKYTons);	
			break;
		
		case MODEL_KZ_DZ:
			DefaultParameterSet(SMPL_KZ_NUM,ELECTRONIC,g_machineTons.putinKZTons);
			break;		
		
		case MODEL_KZKY_DZ:
			DefaultParameterSet(SMPL_KY_NUM,ELECTRONIC,g_machineTons.putinKYTons);
			DefaultParameterSet(SMPL_KZ_NUM,ELECTRONIC,g_machineTons.putinKZTons);
			break;
		
		case MODEL_KY_YY:
			DefaultParameterSet(SMPL_KY_NUM,HYDRAULIC_PRESS,g_machineTons.putinKYTons);
			break;
		
		case MODEL_KZKY_YY:
			DefaultParameterSet(SMPL_KY_NUM,HYDRAULIC_PRESS,g_machineTons.putinKYTons);
			DefaultParameterSet(SMPL_KZ_NUM,HYDRAULIC_PRESS,g_machineTons.putinKZTons);
			break;
	}
	
	pcm_save();
	prm_save();
	prv_save();
}

/*------------------------------------------------------------
 * Function Name  : LoadPutinMachineTonsPage
 * Description    : 输入机器吨位页
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LoadPutinMachineTonsPage( void )
{
	/* 关闭屏幕 */
	SetBackLightEffectClose(COLOR_BACK);
	
	/* 参数初始化 */
	MachineTonsInit();
	
	/* 参数配置 */
	MachineTonsParameterConfig();
	
	/* 获取参数 */
	MachineTonsReadParameter();
	
	/* 画GUI框架 */
	GUI_MachineTons();
	
	/* 遍历 */
	Traverse_MachineTons();
	
	/* 打开屏幕 */
	SetBackLightEffectOpen();
	
	while (g_machineTons.leavePage.flagLeavePage == RESET)
	{
		/* 输入处理 */
		MachineTonsPutinProcess();
		
		/* 移动光标 */
		MachineTonsMoveCursorProcess();
		
		/* 按键处理 */
		MachineTonsKeyProcess();
		
		/* 弹窗处理 */
		PopWindowsProcessCycle();
		
		/* 离开页 */
		MachineTonsLeavePageCheckCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsInit
 * Description    : 初始化
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsInit( void )
{	
	g_machineTons.nowIndex = 0;
	g_machineTons.recordIndex = 0xff;
	g_machineTons.leavePage.flagLeavePage = RESET;
	g_machineTons.leavePage.flagSaveData = RESET;
	g_machineTons.isCancelPutin = NO;
	InitKeyCheck();
}

/*------------------------------------------------------------
 * Function Name  : SetMachineTonsChannel
 * Description    : 设置通道
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetMachineTonsChannel( SMPL_NAME_TypeDef2 channel )
{
	g_machineTons.channel = channel;
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsParameterConfig
 * Description    : 参数配置
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsParameterConfig( void )
{
	g_machineTons.pTitle = "机型配置";
	
	g_machineTons.curParameterNum = MACHINE_TONS_NUM;
	
	if (g_machineTons.channel == SMPL_KY_NUM)
	{
		g_machineTons.pParameterNameArray[0] = pMachineTonsName[0];
	}
	else
	{
		g_machineTons.pParameterNameArray[0] = pMachineTonsName[1];
	}
	g_machineTons.oneLevelMenu[0].saveType = TYPE_INT;
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsReadParameter
 * Description    : 读参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsReadParameter( void )
{
	g_machineTons.parameterData[0][0] = NULL;
}	

/*------------------------------------------------------------
 * Function Name  : MachineTonsWriteParameter
 * Description    : 写参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsWriteParameter( void )
{
	uint16_t tons = (uint16_t)ustrtoul(g_machineTons.parameterData[0],0,10);
	
	if (g_machineTons.channel == SMPL_KY_NUM)
	{
		g_machineTons.putinKYTons = tons;
	}
	else
	{
		g_machineTons.putinKZTons = tons;
	}
}

/*------------------------------------------------------------
 * Function Name  : ConfigMachineTonsParameterRectangleFrameCoordinate
 * Description    : 配置界面GUI坐标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ConfigMachineTonsParameterRectangleFrameCoordinate( void )
{
	const uint16_t START_X = 349;
	const uint16_t START_Y = 229;
	uint16_t startX = START_X;
	uint16_t startY = START_Y;
	uint8_t i;
	
	for (i=0; i<g_machineTons.curParameterNum; ++i)
	{
		g_machineTons.oneLevelMenu[i].x = startX;
		g_machineTons.oneLevelMenu[i].y = startY;
		g_machineTons.oneLevelMenu[i].pointColor = COLOR_POINT;
		g_machineTons.oneLevelMenu[i].backColor = COLOR_BACK;
		g_machineTons.oneLevelMenu[i].recordPointColor = COLOR_POINT;
		g_machineTons.oneLevelMenu[i].recordBackColor = COLOR_BACK;
		g_machineTons.oneLevelMenu[i].lenth = 42;
		g_machineTons.oneLevelMenu[i].width = 30;
		g_machineTons.oneLevelMenu[i].fontSize = 24;
		g_machineTons.oneLevelMenu[i].rowDistance = 48;
		g_machineTons.oneLevelMenu[i].columnDistance = 48;
		g_machineTons.oneLevelMenu[i].lineWidth = 2;
		
		startY += g_machineTons.oneLevelMenu[i].width + g_machineTons.oneLevelMenu[i].rowDistance;
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_MachineTonsDrawOneRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MachineTonsDrawOneRectangleFrame( uint8_t index )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	
	rectangle.x = g_machineTons.oneLevelMenu[index].x;
	rectangle.y = g_machineTons.oneLevelMenu[index].y;
	rectangle.lenth = g_machineTons.oneLevelMenu[index].lenth;
	rectangle.width = g_machineTons.oneLevelMenu[index].width;
	rectangle.lineWidth = g_machineTons.oneLevelMenu[index].lineWidth;
	rectangle.lineColor = g_machineTons.oneLevelMenu[index].pointColor;
	
	GUI_DrawRectangleFrame(&rectangle);
}

/*------------------------------------------------------------
 * Function Name  : GUI_MachineTonsRectangleFrame
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MachineTonsRectangleFrame( void )
{
	uint8_t i;
	
	for (i=0; i<g_machineTons.curParameterNum; ++i)
	{
		GUI_MachineTonsDrawOneRectangleFrame(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : GUI_MachineTonsDrawOneRowOneLevelMenu
 * Description    : 参数界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MachineTonsDrawOneRowOneLevelMenu( uint8_t index )
{
	const uint16_t x = g_machineTons.oneLevelMenu[index].x - 7 * g_machineTons.oneLevelMenu[index].fontSize;
	const uint16_t y = g_machineTons.oneLevelMenu[index].y + g_machineTons.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_machineTons.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_machineTons.oneLevelMenu[index].backColor;
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_machineTons.pParameterNameArray[index]);
}

/*------------------------------------------------------------
 * Function Name  : GUI_MachineTonsOneLevelMenu
 * Description    : 参数界面一级菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MachineTonsOneLevelMenu( void )
{	
	uint8_t i;
	
	for (i=0; i<g_machineTons.curParameterNum; ++i)
	{
		GUI_MachineTonsDrawOneRowOneLevelMenu(i);
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_TimeDate
 * Description    : 界面GUI
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void GUI_MachineTons( void )
{
	GUI_DrawGeneralTitleRectangleFrame(g_machineTons.pTitle);
	
	ConfigMachineTonsParameterRectangleFrameCoordinate();
	
	GUI_MachineTonsRectangleFrame();
	
	GUI_MachineTonsOneLevelMenu();
}

/*------------------------------------------------------------
 * Function Name  : Show_MachineTonsDataOneRowOneLevelMenuContent
 * Description    : 显示一行试验参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MachineTonsDataOneRowOneLevelMenuContent( uint8_t index )
{
	const uint16_t x = g_machineTons.oneLevelMenu[index].x + g_machineTons.oneLevelMenu[index].lineWidth + 1;
	const uint16_t y = g_machineTons.oneLevelMenu[index].y + g_machineTons.oneLevelMenu[index].lineWidth + 1;
	const uint16_t pointColor = g_machineTons.oneLevelMenu[index].pointColor;
	const uint16_t backColor = g_machineTons.oneLevelMenu[index].backColor;
	const uint16_t lenth = g_machineTons.oneLevelMenu[index].lenth - 2 * g_machineTons.oneLevelMenu[index].lineWidth - 2;
	const uint16_t width = g_machineTons.oneLevelMenu[index].width - 2 * g_machineTons.oneLevelMenu[index].lineWidth - 2;
	
	lcd_fill(x,y,lenth,width,backColor);
	
	GUI_DispStr24At(x,y,pointColor,backColor,g_machineTons.parameterData[index]);
}

/*------------------------------------------------------------
 * Function Name  : Show_MachineTonsDataOneLevelMenuContent
 * Description    : 显示一级菜单内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Show_MachineTonsDataOneLevelMenuContent( void )
{
	uint8_t i;
	
	for (i=0; i<g_machineTons.curParameterNum; ++i)
	{
		Show_MachineTonsDataOneRowOneLevelMenuContent(i);
	}	
}

/*------------------------------------------------------------
 * Function Name  : Traverse_MachineTons
 * Description    : 遍历
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void Traverse_MachineTons( void )
{
	Show_MachineTonsDataOneLevelMenuContent();
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsPutinProcess
 * Description    : 输入处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsPutinProcess( void )
{
	PUTIN_TypeDef *pPutin = GetPutinAddr();
	uint8_t index = g_machineTons.nowIndex;
	
	PutinProcessCycle();
	
	pPutin->skin.x = PUTIN_SKIN_START_X;
	pPutin->skin.y = PUTIN_SKIN_START_Y;
	pPutin->skin.pointColor = PUTIN_SKIN_POINT_COLOR;
	pPutin->skin.backColor = g_machineTons.oneLevelMenu[index].recordBackColor;
	pPutin->MenuPointColor = COLOR_SHORTCUT_POINT;
	pPutin->MenuBackColor = COLOR_SHORTCUT_BACK;
	pPutin->skin.open = ENABLE;
	pPutin->NewShift = DISABLE_SHIFT;
	pPutin->x = g_machineTons.oneLevelMenu[index].x + g_machineTons.oneLevelMenu[index].lineWidth + 1;
	pPutin->y = g_machineTons.oneLevelMenu[index].y + g_machineTons.oneLevelMenu[index].lineWidth + 1;
	pPutin->AllowPutinBit = MAX_MACHINE_TONS_PUTIN_BIT;
	pPutin->FillBit = MAX_MACHINE_TONS_PUTIN_BIT;
	pPutin->putinFrameLenth = g_machineTons.oneLevelMenu[index].lenth - 2 * g_machineTons.oneLevelMenu[index].lineWidth - 2;
	pPutin->putinFrameWidth = g_machineTons.oneLevelMenu[index].width - 2 * g_machineTons.oneLevelMenu[index].lineWidth - 2;
	pPutin->PutinNum = &g_machineTons.putinNum;
	pPutin->SaveType = g_machineTons.oneLevelMenu[index].saveType;
	pPutin->FontSize = g_machineTons.oneLevelMenu[index].fontSize;
	pPutin->Encrypt = DISABLE;
	
	KeyPutinChars(pPutin);
}

/*------------------------------------------------------------
 * Function Name  : PasswordMoveCursorProcess
 * Description    : 移动光标处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsMoveCursorProcess( void )
{
	if (g_machineTons.recordIndex == 0xff)
	{
		g_machineTons.oneLevelMenu[g_machineTons.nowIndex].pointColor = COLOR_SELECT_POINT;
		g_machineTons.oneLevelMenu[g_machineTons.nowIndex].backColor = COLOR_SELECT_BACK;
		
		Show_MachineTonsDataOneRowOneLevelMenuContent(g_machineTons.nowIndex);
	}
	
	g_machineTons.recordIndex = g_machineTons.nowIndex;
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsUpdateStatus
 * Description    : 更新状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsUpdateStatus( void )
{
	g_machineTons.recordIndex = 0xff;
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsCheckDataCycle
 * Description    : 检测数据
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static TestStatus MachineTonsCheckDataCycle( void )
{
	uint16_t tons = 0;
	
	tons = (uint16_t)ustrtoul(g_machineTons.parameterData[0],0,10);
	
	if ((tons<1) || (tons>1000))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pMachineTonsWarn[0]);	
					
		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsKeyProcess
 * Description    : 按键处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsKeyProcess( void )
{
	uint8_t index = g_machineTons.nowIndex;
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
						switch ( g_machineTons.oneLevelMenu[index].saveType )
						{
							case TYPE_INT:
								numtochar(g_machineTons.putinNum,*GetPutinIntDataAddr(),g_machineTons.parameterData[index]);
								break;
							case TYPE_FLOAT:
								floattochar(g_machineTons.putinNum,g_machineTons.oneLevelMenu[index].pointBit,*GetPutinFloatDataAddr(),g_machineTons.parameterData[index]);
								break;
							case TYPE_CHAR:
								if ( strcmp(GetPutinCharDataAddr(),"") )
								{
									strcpy(g_machineTons.parameterData[index],GetPutinCharDataAddr());
								}
								break;
						}	
						break;						
					
					default:
						break;
				}
				if (MachineTonsCheckDataCycle() == FAILED)
				{
					g_machineTons.leavePage.flagLeavePage = SET;
					g_machineTons.leavePage.flagSaveData = RESET;
					break;
				}
				
				g_machineTons.leavePage.flagLeavePage = SET;
				g_machineTons.leavePage.flagSaveData = SET;
				
				MachineTonsUpdateStatus();
				break;
				
			case KEY_ESC:
				putinStatus = GetPutinStatus();
					
				switch ( putinStatus )
				{
					case STATUS_DISABLE_PUTIN:
						g_machineTons.leavePage.flagLeavePage = SET;
						g_machineTons.leavePage.flagSaveData = RESET;
						break;
					
					case STATUS_CANCEL_PUTIN:
						MachineTonsUpdateStatus();
						break;
					
					default:						
						break;
				}		
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : MachineTonsLeavePageCheckCycle
 * Description    : 离开页检测
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void MachineTonsLeavePageCheckCycle( void )
{
	if (g_machineTons.leavePage.flagLeavePage == SET)
	{
		if (g_machineTons.leavePage.flagSaveData == SET)
		{
			g_machineTons.isCancelPutin = NO;
			
			MachineTonsWriteParameter();
		}
		else
		{
			g_machineTons.isCancelPutin = YES;
			g_machineTons.putinKYTons = 0;
			g_machineTons.putinKZTons = 0;
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
