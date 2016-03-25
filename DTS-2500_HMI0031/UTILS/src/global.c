/**
  ******************************************************************************
  * @file    global.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-15 09:58:52
  * @IDE	 V5.16a
  * @Chip    STM32F407VET6
  * @brief   全局文件
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
#define COLOR_POINT							WHITE
#define	COLOR_BACK							BLACK


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	STATUS_BOUND_SHOW = 0,
	STATUS_BOUND_SHOW_KEEP,
	STATUS_BOUND_ERASE,
	STATUS_BOUND_ERASE_KEEP,
}BOUND_TEXT_STATUS_TypeDef;

typedef enum
{
	COMM_LED_IDLE = 0,
	COMM_LED_OFF,
	COMM_LED_DELAY,
}COMM_LED_STATUS_TypeDef;

typedef struct
{
	COMM_LED_STATUS_TypeDef status;
	uint16_t delayTime;
	FlagStatus startLamp; 
}COMM_LED_TypeDef;

typedef struct
{
	uint16_t backColor;
	BoolStatus isLssued;
	void (*lssued)(uint16_t backColor);
}LSSUED_PARAMETER_TypeDef;

typedef struct
{ 
	uint8_t index[SMPL_NUM];
	float lastForcePoint[SMPL_NUM];
	float nowForcePoint[SMPL_NUM];
	uint16_t downPoint[SMPL_NUM];		//下降点数
	float peakForce[SMPL_NUM];			//峰值
	float maxForceDiff[SMPL_NUM];		//与最大力差值
	float adjoinPointDiff[SMPL_NUM];	//相邻两点差值
	uint8_t attenuationRate[SMPL_NUM];	//衰减率
}JUDGE_BREAK_TypeDef;

/* Private constants ---------------------------------------------------------*/
/*功能键名称*/
const char * const pTwoLevelMenu[] = 
{
	" 试验选择 ",		//0		  
	" 试验参数 ",		//1
	" 阀口复位 ",		//2
	"  更  多  ",		//3
	" 控制参数 ",		//4
	" 设置编号 ",		//5
	"  标定表  ",		//6
	"  查  看  ",		//7
	"    <-    ",		//8
	"    ->    ",		//9
	" 查看警告 ",		//10
	"  清  除  ",		//11
	" 位置调整 ",		//12
	"  主界面  ",		//13
	"  快  速  ",		//14
	"  删  除  ",		//15
	" 重做试验 ",		//16
	" 力值校准 ",		//17
	" 力值检定 ",		//18
	"  打  印  ",		//19
	" 试验参数 ",		//20
	" 试验选择 ",		//21
	" 试验参数 ",		//22
	" 力码修正 ",		//23
	"  打  印  ",		//24
	"  修正 >> ",		//25
	"  删  除  ",		//26
	" 试验报告 ",		//27
	"  慢  速  ",		//28
	"  绝对码  ",		//29
	"  下一页  ",		//30
	"  上一页  ",		//31
	"  标定表  ",		//32
	" 正在打印 ",		//33
	" 打印整页 ",		//34
	" 打印失败 ",		//35
	" 撤销一块 ",		//36
	"  保  存  ",		//37
	" 试验报告 ",		//38
	"  警  告  ",		//39
	"  激  活  ",		//40
	"  打  印  ",		//41
	"  << 还原 ",		//42
	"备份(默认)",		//43
	"还原(默认)",		//44
	"  确  认  ",		//45
	" 撤销一块 ",		//46
	"  撤  销  ",		//47
	"  相对码  ",		//48
	"  查  看  ",		//49
	"  上一个  ",		//50
	"  确  认  ",		//51
	"  不  是  ",		//52
	" 力码修正 ",		//53
	" 正在保存 ",		//54
	"  格式化  ",		//55
	" 结束一组 ",		//56
	" 系统设置 ",		//57
	"  取  消  ",		//58
	" 系统设置 ",		//59	
	" 标定参数 ",		//60	
	"  保  存  ",		//61
	"  警  告  ",		//62
	"  搜  索  ",		//63
	" 结束一组 ",		//64
	"  打  点  ",		//65
	"  下一个  ",		//66
	"  退  出  ",		//67
	" 下一步   ",		//68
	"  完  成  ",		//69
	" 一键体检 ",		//70
	"  备  份  ",		//71
	"  是  的  ",		//72
	" 退出联机 ",		//73
	" 退出试验 ",		//74
	"  退  出  ",		//75
	"  忽  略  ",		//76
	"试验后处理",		//77
	"  导  出  ",		//78
	"  选  择  ",		//79
	"  进  入  ",		//80
	" 修改密码 ",		//81
	"备份(U 盘)",		//82
	"还原(U 盘)",		//83
	" 进入标定 ",		//84
	"写入标定表",		//85
	" 查看曲线 ",		//86
	"  导  入  ",		//87
	"  码取反  ",		//88
};

const char * const pUnitType[] = 
{
	"    N",		//0
	"   kN",		//1
	"  N/s",		//2
	" kN/s",		//3
	"   mm",		//4
	"  mm2",		//5
	"  mm3",		//6
	"    s",		//7
	"    d",		//8
	"MPa/s",		//9
	"    %",		//10
	"    T",		//11
	"   cm",		//12
	"   dm",		//13
	"    m",		//14
	"  MPa",		//15
};

const char * const pSelectMenuCue[] = 
{
	"  确  认  ",
	"  取  消  ",
};

const char * const pSelectMenuConfirmCue[] = 
{
	"  是  的  ",
	"  不  是  ",
};

const char * const LssuedParameterWarning[] = 
{
	"下发参数失败！",				//0
	"加载力值超过系统正向最大力！",	//1
	"加载力值超过系统负向最大力！",	//2
};

const char * const pPumpWarning[] = 
{
	"油泵启动失败！",		//0
	"油泵停止失败！",		//1
	"丝杆上升失败！",		//2
	"丝杆下降失败！",		//3
	"丝杆停止失败！",		//4
};

const char * const pGlobalWarning[] = 
{
	"机型配置错误！",
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
HMI_TypeDef *pHmi = NULL;	 		//人机界面杂项参数
TEST_TypeDef *pTest = NULL;			//试验参数

#pragma arm section rwdata = "RAM_GLOBAL",zidata = "RAM_GLOBAL"
	static INTERFACE_ELEMENT_TypeDef g_interfaceElement;	//界面元素
	static COORDINATE_TypeDef g_drawCoordinate;
	static COORDINATE_DRAW_LINE_TypeDef g_coordinateDrawLine;
	static COMM_LED_TypeDef g_commLed;
	static SYSTEM_PROTECT_TypeDef g_systemProtect;
	static LSSUED_PARAMETER_TypeDef g_lssuedParameter;
	static JUDGE_BREAK_TypeDef g_judgeBreak;	//判破点
	static SAMPLE_PROCESS_TypeDef g_sampleProcess[SMPL_NUM];
#pragma arm section

__ALIGN_RAM COORDINATE_POINT_TypeDef g_coordinatePoint;
__ALIGN_RAM REPORT_TypeDef g_readReport;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : GetFH_SmplUnit
 * Description    : 获取负荷通道单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FH_UINT_TypeDef GetFH_SmplUnit( void )
{
	if ( pHmi->fhUnit )
	{
		return FH_UNIT_N;	  //以N为单位
	}
	else
	{
		return FH_UNIT_kN;	  //以kN为单位
	}
}

/*------------------------------------------------------------
 * Function Name  : GetWY_SmplUnit
 * Description    : 获取位移通道单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
WY_UINT_TypeDef GetWY_SmplUnit( void )
{
	if (pHmi->wyUnit > WY_UNIT_M)
	{
		return WY_UNIT_MM;
	}
	else
	{
		return (WY_UINT_TypeDef)pHmi->wyUnit;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetBX_SmplUnit
 * Description    : 获取变形通道单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BX_UINT_TypeDef GetBX_SmplUnit( void )
{
	if (pHmi->bxUnit > BX_UNIT_M)
	{
		return BX_UNIT_M;
	}
	else
	{
		return (BX_UINT_TypeDef)pHmi->bxUnit;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetTestContent
 * Description    : 获取试验内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GetTestContent( void )
{
	pHmi = pcm_hmi_get();
	
	if (pHmi->test_standard_index >= SUPPORT_TEST_NUM)
	{
		pHmi->test_standard_index = NONE_TEST;
	}	
	pTest = pcm_test_get(pHmi->test_standard_index);
}

/*------------------------------------------------------------
 * Function Name  : CheckCurrentModel
 * Description    : 检测当前机型
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus CheckCurrentModel( void )
{
	MODEL_TYPE_TypeDef Type = GetModelType();
	
	if (Type != MODEL_UNIVERSAL)
	{
		return FAILED;
	}
	
	return PASSED;
}

/*------------------------------------------------------------
 * Function Name  : ProcessMachineMatchTestType
 * Description    : 处理机型与试验类型匹配
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ProcessMachineMatchTestType( void )
{
	MODEL_TYPE_TypeDef Type = MODEL_KY;
	uint8_t testType = pHmi->test_standard_index;
	
	Type = GetModelType();
	
	switch ( Type )
	{
		case MODEL_KY:
		case MODEL_KZ:
		case MODEL_KZKY:
		case MODEL_UNDEFINED:
			SetPopWindowsInfomation(POP_PCM_CUE,1,&pGlobalWarning[0]);		
			PopWindowsProcessCycle();
			break;
		
		case MODEL_UNIVERSAL:
			if (testType >= SUPPORT_TEST_NUM)
			{
				pHmi->test_standard_index = NONE_TEST;
				pcm_save();
			}
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : TestUserPassword
 * Description    : 测试用户密码
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus TestUserPassword( const char *pPassword )
{
	if ( strcmp(pHmi->paseword_system,pPassword) == 0)
	{
		return PASSED;
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : TestManagerPassword
 * Description    : 测试管理员密码
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus TestManagerPassword( const char *pPassword )
{
	if ( strcmp(SUPER_PASSWORD,pPassword) == 0)	//匹配
	{		
		return PASSED;
	}
	
	return FAILED;
}

/*------------------------------------------------------------
 * Function Name  : PCM_CmdSendCycle
 * Description    : 命令发送循环体(阻塞)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CMD_STATUS_TypeDef PCM_CmdSendCycle( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	
	if (GetLinkStatus() == LINK_UNLINK)
	{
		return CMD_UNLINK;
	}
	
	while (1)
	{
		cmdStatus = comm_cycle();
			
		if ((cmdStatus==CMD_IDLE) || (cmdStatus==CMD_BUSY) || (cmdStatus==CMD_UNLINK))
		{
			return cmdStatus;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicForce
 * Description    : 刷新动态力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicForce( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float force )
{
	float absForce = fabs(force);
	uint8_t intNum = 0,dotNum = 0;
	
	force = SetValueNotEqualZero(force);	
	
	if (absForce < 1000)
	{
		intNum = 3;
		dotNum = 2;
	}
	else if (absForce < 100000)
	{
		intNum = 4;
		dotNum = 1;
	}
	else
	{
		disp_syn(DISP_CHN_FH);
		lcd_num48(x,y,pointColor,backColor,"------");
		
		return;
	}
	
	disp_value(force,DISP_CHN_FH,x,y,pointColor,backColor,intNum,dotNum);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicDisplacement
 * Description    : 刷新动态位移
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicDisplacement( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float displacement )
{
	float absDisplacement = fabs(displacement);
	uint8_t intNum = 0,dotNum = 0;
	
	displacement = SetValueNotEqualZero(displacement);	
	
	if (absDisplacement < 1000)
	{
		intNum = 3;
		dotNum = 3;
	}
	else if (absDisplacement < 100000)
	{
		intNum = 4;
		dotNum = 2;
	}
	else
	{
		disp_syn(DISP_CHN_WY);
		lcd_num48(x,y,pointColor,backColor,"------");
		
		return;
	}
	
	disp_value(displacement,DISP_CHN_WY,x,y,pointColor,backColor,intNum,dotNum);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicDeform
 * Description    : 刷新动态变形值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicDeform( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float deform )
{
	float absDeform = fabs(deform);
	
	uint8_t intNum = 0,dotNum = 0;
	
	deform = SetValueNotEqualZero(deform);	
	
	if (absDeform < 1000)
	{
		intNum = 3;
		dotNum = 3;
	}
	else if (absDeform < 100000)
	{
		intNum = 4;
		dotNum = 2;
	}
	else
	{
		disp_syn(DISP_CHN_BX);
		lcd_num48(x,y,pointColor,backColor,"------");
		
		return;
	}
	
	disp_value(deform,DISP_CHN_BX,x,y,pointColor,backColor,intNum,dotNum);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicSpeed
 * Description    : 刷新速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicSpeed( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float speed )
{	
	float absSpeed = fabs(speed);
	uint8_t intNum = 0,dotNum = 0;
	
	speed = SetValueNotEqualZero(speed);
	
	if ( absSpeed < 100 )
	{
		intNum = 2;
		dotNum = 2;
	}
	else if ( absSpeed < 10000 )
	{
		intNum = 3;
		dotNum = 1;
	}
	else
	{
		disp_syn(DISP_CHN_SPEED);
		lcd_num48(x,y,pointColor,backColor,"-----");
		
		return;
	}
	
	disp_value(speed,DISP_CHN_SPEED,x,y,pointColor,backColor,intNum,dotNum);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicStrength
 * Description    : 刷新强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicStrength( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float strength )
{
	float absStrength = fabs(strength);
	
	strength = SetValueNotEqualZero(strength);
	
	if (absStrength < 1000)
	{
		disp_value(strength,DISP_CHN_STRENGTH,x,y,pointColor,backColor,3,1);
	}
	else
	{
		disp_syn(DISP_CHN_STRENGTH);
		lcd_num48(x,y,pointColor,backColor,"-----");
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshPeak
 * Description    : 刷新峰值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicPeak( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float peak )
{
	float absPeak = fabs(peak);
	uint8_t intNum = 0,dotNum = 0;
	
	peak = SetValueNotEqualZero(peak);
	
	if (absPeak < 1000)
	{
		intNum = 3;
		dotNum = 2;
	}
	else if (absPeak < 100000)
	{
		intNum = 4;
		dotNum = 1;
	}
	else
	{
		disp_syn(DISP_CHN_PEAK);
		lcd_num48(x,y,pointColor,backColor,"------");
		
		return;
	}
	
	disp_value(peak,DISP_CHN_PEAK,x,y,pointColor,backColor,intNum,dotNum);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicCommunicationStatus
 * Description    : 刷新通讯状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicCommunicationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, LINK_STATUS_TypeDef linkStatus )
{
	static uint8_t s_loadCount = 0;
	const uint8_t MAX_SHOW_LOAD_COUNT = 5;
	static BOUND_TEXT_STATUS_TypeDef s_boundTextStatus = STATUS_BOUND_SHOW;
	
	switch ( linkStatus )
	{
		case LINK_UNLINK:
			switch ( s_boundTextStatus )
			{
				case STATUS_BOUND_SHOW:
					GUI_DispStr16At(x,y,RED,BLACK,"脱机");
					s_boundTextStatus = STATUS_BOUND_SHOW_KEEP;
					s_loadCount = 0;
					break;
				case STATUS_BOUND_SHOW_KEEP:
					s_loadCount++;
					if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
					{
						s_boundTextStatus = STATUS_BOUND_ERASE;
					}
					break;
				case STATUS_BOUND_ERASE:
					GUI_DispStr16At(x,y,pointColor,backColor,"    ");
					s_boundTextStatus = STATUS_BOUND_ERASE_KEEP;
					s_loadCount = 0;
					break;
				case STATUS_BOUND_ERASE_KEEP:
					s_loadCount++;
					if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
					{
						s_boundTextStatus = STATUS_BOUND_SHOW;
					}
					break;
			}
			break;
		case LINK_IDLE:
			GUI_DispStr16At(x,y,pointColor,backColor,"联机");
			break;
		default:
			GUI_DispStr16At(x,y,pointColor,backColor,"----");
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : FefreshFlickerStatus
 * Description    : 刷新跳动状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void FefreshFlickerStatus( uint16_t x, uint16_t y, uint16_t pointColor, \
				uint16_t backColor, const char * const pChar, uint8_t fontSize)
{
	static uint8_t s_loadCount = 0;
	const uint8_t MAX_SHOW_LOAD_COUNT = 5;
	static BOUND_TEXT_STATUS_TypeDef s_boundTextStatus = STATUS_BOUND_SHOW;
	
	switch ( s_boundTextStatus )
	{
		case STATUS_BOUND_SHOW:
			switch ( fontSize )
			{
				case 16:
					GUI_DispStr16At(x,y,pointColor,backColor,pChar);
					break;
				default:
					GUI_DispStr24At(x,y,pointColor,backColor,pChar);
					break;
			}
			s_boundTextStatus = STATUS_BOUND_SHOW_KEEP;
			s_loadCount = 0;
			break;
		case STATUS_BOUND_SHOW_KEEP:
			s_loadCount++;
			if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
			{
				s_boundTextStatus = STATUS_BOUND_ERASE;
			}
			break;
		case STATUS_BOUND_ERASE:
			{
				char showBuff[20];
				uint8_t len = strlen(pChar);
				
				memset(showBuff,' ',len);
				showBuff[len] = NULL;
				
				switch ( fontSize )
				{
					case 16:
						GUI_DispStr16At(x,y,pointColor,backColor,"    ");
						break;
					default:
						GUI_DispStr24At(x,y,pointColor,backColor,"    ");
						break;
				}
				s_boundTextStatus = STATUS_BOUND_ERASE_KEEP;
				s_loadCount = 0;				
			}
			break;
		case STATUS_BOUND_ERASE_KEEP:
			s_loadCount++;
			if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
			{
				s_boundTextStatus = STATUS_BOUND_SHOW;
			}
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicTestStatus
 * Description    : 刷新试验状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicTestStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, TEST_STATUS_TypeDef testStatus )
{
	switch ( testStatus )
	{
		case TEST_IDLE:
			GUI_DispStr16At(x,y,pointColor,backColor,"空闲");
			break;
		case TEST_LOAD:
			FefreshFlickerStatus(x,y,pointColor,backColor,"加载",16);
			break;				
		case TEST_KEEP:
			GUI_DispStr16At(x,y,pointColor,backColor,"保持");
			break;
		case TEST_YIELD:
			GUI_DispStr16At(x,y,pointColor,backColor,"屈服");
			break;
		case TEST_DEFORM:
			FefreshFlickerStatus(x,y,pointColor,backColor,"加载",16);
			break;
		case TEST_BREAK:
			GUI_DispStr16At(x,y,pointColor,backColor,"判破");
			break;
		case TEST_UNLOAD:
			GUI_DispStr16At(x,y,pointColor,backColor,"卸载");
			break;
		case TEST_SAVE:
			GUI_DispStr16At(x,y,pointColor,backColor,"存储");
			break;
		default:
			GUI_DispStr16At(x,y,pointColor,backColor,"----");
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicCalibrationStatus
 * Description    : 刷新校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicCalibrationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, CALIBRATION_STATUS_TypeDef calibrationStatus )
{
	static uint8_t s_loadCount = 0;
	const uint8_t MAX_SHOW_LOAD_COUNT = 5;
	static BOUND_TEXT_STATUS_TypeDef s_boundTextStatus = STATUS_BOUND_SHOW;
	
	switch ( calibrationStatus )
	{
		case STATUS_CALIBRATION_IDLE:
			GUI_DispStr16At(x,y,pointColor,backColor,"空闲");
			break;
		case STATUS_CALIBRATION_PROCESS:
			switch ( s_boundTextStatus )
			{
				case STATUS_BOUND_SHOW:
					GUI_DispStr16At(x,y,pointColor,backColor,"进程");
					s_boundTextStatus = STATUS_BOUND_SHOW_KEEP;
					s_loadCount = 0;
					break;
				case STATUS_BOUND_SHOW_KEEP:
					s_loadCount++;
					if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
					{
						s_boundTextStatus = STATUS_BOUND_ERASE;
					}
					break;
				case STATUS_BOUND_ERASE:
					GUI_DispStr16At(x,y,pointColor,backColor,"    ");
					s_boundTextStatus = STATUS_BOUND_ERASE_KEEP;
					s_loadCount = 0;
					break;
				case STATUS_BOUND_ERASE_KEEP:
					s_loadCount++;
					if (s_loadCount >= MAX_SHOW_LOAD_COUNT)
					{
						s_boundTextStatus = STATUS_BOUND_SHOW;
					}
					break;
			}
			break;
		case STATUS_CALIBRATION_BACKHAUL:
			GUI_DispStr16At(x,y,pointColor,backColor,"回程");
			break;
		case STATUS_CALIBRATION_END:
			GUI_DispStr16At(x,y,pointColor,backColor,"结束");
			break;
		default:
			GUI_DispStr16At(x,y,pointColor,backColor,"----");
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicCode
 * Description    : 刷新码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicCode( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, int32_t code )
{
	char showBuff[10];
	
	usprintf(showBuff,"%07d",code);
	GUI_DispStr16At(x,y,pointColor,backColor,showBuff);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicDoubleCode
 * Description    : 刷新2个码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicDoubleCode( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, int32_t code1, int32_t code2 )
{
	char showBuff[10];
	const uint8_t FONT_SIZE = 16;
	
	usprintf(showBuff,"%07d",code1);
	GUI_DispStr16At(x,y,pointColor,backColor,showBuff);
	
	x += 8 * (FONT_SIZE >> 1);
	
	GUI_DispStr16At(x,y,pointColor,backColor,"┇");//┇Ψ
	
	x += 3 * (FONT_SIZE >> 1);
	
	usprintf(showBuff,"%07d",code2);
	GUI_DispStr16At(x,y,pointColor,backColor,showBuff);
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicSystemTime
 * Description    : 刷新系统时间
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicSystemTime( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor )
{
	char showBuff[20];
	tTime t;
	static uint8_t s_recordSecond = 0;
	
	time_cycle();
	t = get_time();	
	
	if (s_recordSecond != t.ucSec)
	{
		s_recordSecond = t.ucSec;
		
		usprintf(showBuff,"%04d-%02d-%02d  %02d:%02d:%02d",t.usYear,t.ucMon,t.ucMday,t.ucHour,t.ucMin,t.ucSec);
	
		GUI_DispStr16At(x,y,pointColor,backColor,showBuff);
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicUSBIco
 * Description    : 刷新USB连接图标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicUSBIco( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, BoolStatus usbConnect )
{
	switch ( usbConnect )
	{
		case YES:
			lcd_show_image(x,y,PIC_USB_SHOW);
			break;
		case NO:
			lcd_fill(x,y,16,16,backColor);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : RefreshDynamicEthernetIco
 * Description    : 刷新以太网连接图标
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicEthernetIco( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, BoolStatus ethernetConnect )
{
	switch ( ethernetConnect )
	{
		case YES:
			lcd_show_image(x,y,PIC_ETHERNET_SHOW);
			break;
		case NO:
			lcd_fill(x,y,16,16,backColor);
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : InitInterfaceElement
 * Description    : 初始化界面元素显示
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitInterfaceElement( void )
{
	g_interfaceElement.force = 0;
	g_interfaceElement.disPlacement = 0;
	g_interfaceElement.deform = 0;
	g_interfaceElement.fhSpeed = 0;
	g_interfaceElement.wySpeed = 0;
	g_interfaceElement.bxSpeed = 0;
	g_interfaceElement.strength = 0;
	g_interfaceElement.peak = 0;
	g_interfaceElement.linkStatus = LINK_IDLE;
	g_interfaceElement.testStatus = TEST_IDLE;
	g_interfaceElement.fhCode = 0;
	g_interfaceElement.wyCode = 0;
	g_interfaceElement.bxCode = 0;
	g_interfaceElement.usbConnect = NO;
	g_interfaceElement.ethernetConnect = NO;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementForce
 * Description    : 设置界面力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementForce( float force )
{
	g_interfaceElement.force = force;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementDisPlacement
 * Description    : 设置界面位移
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementDisPlacement( float disPlacement )
{
	g_interfaceElement.disPlacement = disPlacement;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementDeform
 * Description    : 设置界面变形
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementDeform( float deform )
{
	g_interfaceElement.deform = deform;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementFHSpeed
 * Description    : 设置界面负荷速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementFHSpeed( float speed )
{
	g_interfaceElement.fhSpeed = speed;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementWYSpeed
 * Description    : 设置界面位移速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementWYSpeed( float speed )
{
	g_interfaceElement.wySpeed = speed;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementBXSpeed
 * Description    : 设置界面变形速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementBXSpeed( float speed )
{
	g_interfaceElement.bxSpeed = speed;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementStrength
 * Description    : 设置界面强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementStrength( float strength )
{
	g_interfaceElement.strength = strength;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementPeak
 * Description    : 设置界面峰值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementPeak( float peak )
{
	g_interfaceElement.peak = peak;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceLinkStatus
 * Description    : 设置界面联机状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceLinkStatus( LINK_STATUS_TypeDef linkStatus )
{
	g_interfaceElement.linkStatus = linkStatus;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceTestStatus
 * Description    : 设置界面试验状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceTestStatus( TEST_STATUS_TypeDef testStatus )
{
	g_interfaceElement.testStatus = testStatus;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceCalibrationStatus
 * Description    : 设置界面校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceCalibrationStatus( CALIBRATION_STATUS_TypeDef calibrationStatus )
{
	g_interfaceElement.calibrationStatus = calibrationStatus;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementFHCode
 * Description    : 设置界面负荷通道码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementFHCode( int32_t code )
{
	g_interfaceElement.fhCode = code;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementWYCode
 * Description    : 设置界面位移通道码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementWYCode( int32_t code )
{
	g_interfaceElement.wyCode = code;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementBXCode
 * Description    : 设置界面变形通道码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementBXCode( int32_t code )
{
	g_interfaceElement.bxCode = code;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementUSBStatus
 * Description    : 设置界面USB状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementUSBStatus( BoolStatus usbConnect )
{
	g_interfaceElement.usbConnect = usbConnect;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceElementEthernetConnectStatus
 * Description    : 设置界面以太网连接状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementEthernetConnectStatus( BoolStatus ethernetConnect )
{
	g_interfaceElement.ethernetConnect = ethernetConnect;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementForce
 * Description    : 获取界面力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementForce( void )
{
	return g_interfaceElement.force;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementDisPlacement
 * Description    : 获取界面位移
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementDisPlacement( void )
{
	return g_interfaceElement.disPlacement;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementDeform
 * Description    : 获取界面变形
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementDeform( void )
{
	return g_interfaceElement.deform;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementFHSpeed
 * Description    : 获取界面负荷速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementFHSpeed( void )
{
	return g_interfaceElement.fhSpeed;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementWYSpeed
 * Description    : 获取界面位移速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementWYSpeed( void )
{
	return g_interfaceElement.wySpeed;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementBXSpeed
 * Description    : 获取界面变形速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementBXSpeed( void )
{
	return g_interfaceElement.bxSpeed;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementStrength
 * Description    : 获取界面强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementStrength( void )
{
	return g_interfaceElement.strength;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementPeak
 * Description    : 获取界面峰值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementPeak( void )
{
	return g_interfaceElement.peak;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceLinkStatus
 * Description    : 获取界面联机状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
LINK_STATUS_TypeDef GetInterfaceLinkStatus( void )
{
	return g_interfaceElement.linkStatus;
}

/*------------------------------------------------------------
 * Function Name  : SetInterfaceTestStatus
 * Description    : 获取界面试验状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TEST_STATUS_TypeDef GetInterfaceTestStatus( void )
{
	return g_interfaceElement.testStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceCalibrationStatus
 * Description    : 获取界面校准状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CALIBRATION_STATUS_TypeDef GetInterfaceCalibrationStatus( void )
{
	return g_interfaceElement.calibrationStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementCode
 * Description    : 获取界面码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
int32_t GetInterfaceElementCode( SMPL_NAME_TypeDef tureChannel )
{
	int32_t code = 0;
	
	switch ( tureChannel )
	{
		case SMPL_FH_NUM:
			code = g_interfaceElement.fhCode;
			break;
		case SMPL_WY_NUM:
			code = g_interfaceElement.wyCode;
			break;
		case SMPL_BX_NUM:
			code = g_interfaceElement.bxCode;
			break;
		default:
			break;
	}
	
	return code;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementUSBStatus
 * Description    : 获取界面USB状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus GetInterfaceElementUSBStatus( void )
{
	return g_interfaceElement.usbConnect;
}

/*------------------------------------------------------------
 * Function Name  : GetInterfaceElementEthernetConnectStatus
 * Description    : 获取界面以太网连接状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus GetInterfaceElementEthernetConnectStatus( void )
{
	return g_interfaceElement.ethernetConnect;
}

/*------------------------------------------------------------
 * Function Name  : GetCoordinateDataAddr
 * Description    :	获取坐标系数据地址坐标系
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
COORDINATE_TypeDef * GetCoordinateDataAddr( void )
{
	return &g_drawCoordinate;
}

/*------------------------------------------------------------
 * Function Name  : GUI_DrawCoordinate
 * Description    :	画坐标系
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawCoordinate( COORDINATE_TypeDef *pCoordinate )
{
	tDashedFrameObject dashed_frame;
	
	/* 画标题 */
	{
		uint16_t titlelenth = strlen(pCoordinate->pTitle);
		uint16_t x = pCoordinate->x + (pCoordinate->xLenth - titlelenth * 8) / 2;
		uint16_t y = pCoordinate->y - 28;
		
		GUI_DispStr16At(x,y,pCoordinate->fontPointColor,pCoordinate->fontBackColor,pCoordinate->pTitle);
	}
	
	//定义画虚线框的各个属性
	dashed_frame.x 				= pCoordinate->x;					//起点X坐标
	dashed_frame.y 				= pCoordinate->y;					//起点Y坐标
	dashed_frame.hori_num 		= pCoordinate->rowFieldNum;			//行数
	dashed_frame.hori_space 	= pCoordinate->rowSpace;			//行间距
	dashed_frame.veri_num 		= pCoordinate->columnFieldNum;		//列数
	dashed_frame.veri_space		= pCoordinate->columnSpace;			//列间距
	dashed_frame.width			= pCoordinate->lineWidth;			//线宽
	dashed_frame.fill			= pCoordinate->fillFieldLenth;		//填充长度
	dashed_frame.space			= pCoordinate->emptyFieldLenth;		//不填充长度
	dashed_frame.color_row		= pCoordinate->rowLineColor;		//框线颜色
	dashed_frame.color_col		= pCoordinate->columnLineColor;		//框线颜色
	
	/* 坐标系虚线框 */
	lcd_draw_dashed_frame(dashed_frame);	
	
	/* 轴线 */
	{
		uint8_t lineWidth = 2;
		
		lcd_fill(pCoordinate->x-5,pCoordinate->y+pCoordinate->yLenth,pCoordinate->xLenth+10,\
				lineWidth,pCoordinate->xLinePointColor);
		lcd_fill(pCoordinate->x-1,pCoordinate->y-5,lineWidth,pCoordinate->yLenth+10,\
				pCoordinate->yLinePointColor);
	//	lcd_fill(pCoordinate->x,pCoordinate->y,pCoordinate->xLenth,\
	//			lineWidth,pCoordinate->xLinePointColor);
	//	lcd_fill(pCoordinate->x+pCoordinate->xLenth,pCoordinate->y,lineWidth,pCoordinate->yLenth,\
	//			pCoordinate->yLinePointColor);
	}
	
	/* 显示 X/Y轴数据点 */
	{
		uint8_t i;
		uint32_t averageValue = 0;
		uint32_t curValue = 0;
		char showBuff[20];
		uint8_t bit = 0;

		averageValue = pCoordinate->xMaxValue / pCoordinate->columnFieldNum;	
		for (i=0; i<=pCoordinate->columnFieldNum; ++i)
		{
			curValue = averageValue * i;
			usprintf(showBuff,"%d",curValue);
			
			bit = GetIntBit(curValue);
			
			GUI_DispStr16At(pCoordinate->x+i*pCoordinate->columnSpace-bit*4,\
								pCoordinate->y+pCoordinate->yLenth+8,pCoordinate->fontPointColor,\
								pCoordinate->fontBackColor,showBuff);
		}
	
		averageValue = pCoordinate->yMaxValue / pCoordinate->rowFieldNum;	
		for (i=0; i<=pCoordinate->rowFieldNum; ++i)
		{
			curValue = averageValue * i;
			usprintf(showBuff,"%d",curValue);
			
			bit = GetIntBit(curValue);
			
			GUI_DispStr16At(pCoordinate->x-bit*8-8,pCoordinate->y+pCoordinate->yLenth-i*pCoordinate->rowSpace-8,\
						pCoordinate->fontPointColor,pCoordinate->fontBackColor,showBuff);
		}	
	}	
	
	/* 显示单位 */
	GUI_DispStr16At(pCoordinate->x+pCoordinate->xLenth-36,\
					pCoordinate->y+pCoordinate->yLenth-18,pCoordinate->fontPointColor,\
					pCoordinate->fontBackColor,pCoordinate->pXUnit);
	GUI_DispStr16At(pCoordinate->x+3,pCoordinate->y+2,pCoordinate->fontPointColor,\
					pCoordinate->fontBackColor,pCoordinate->pYUnit);
}

/*------------------------------------------------------------
 * Function Name  : GUI_ProgressBar
 * Description    : 进度条GUI界面
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_ProgressBar( PROGRESS_BAR_TypeDef *pProgress )
{
	RECTANGLE_FRAME_TypeDef rectangle;
	uint16_t strlenth = 0;
	
	rectangle.x = pProgress->x;
	rectangle.y = pProgress->y;
	rectangle.lenth = pProgress->lenth;
	rectangle.width = pProgress->width;
	rectangle.lineWidth = pProgress->lineWidth;
	rectangle.lineColor = pProgress->lineColor;
	
	GUI_DrawRectangleFrame(&rectangle);
	
	strlenth = strlen(pProgress->pTitle) * (pProgress->fontSize >> 1);
	
	GUI_DispStr16At(pProgress->x-strlenth,pProgress->y+((pProgress->width - pProgress->fontSize) >> 1),pProgress->fontColor,\
					pProgress->backColor,pProgress->pTitle);
}	

/*------------------------------------------------------------
 * Function Name  : DrawProcessBody
 * Description    : 画进度条
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void DrawProcessBody( PROGRESS_BAR_TypeDef *pProgress )
{	
	uint8_t halfWidth = 0;
	uint16_t progressLenth = 0;
	
	if (pProgress->curProgress == 0)
	{
		return;
	}	
	
	if (pProgress->curProgress > pProgress->totalProgress )
	{
		return;
	}
	
	progressLenth = (float)pProgress->curProgress / pProgress->totalProgress * \
					(pProgress->lenth - 2 * pProgress->lineWidth);
	halfWidth = (pProgress->width - 2 * pProgress->lineWidth) >> 1;
	
	lcd_fill(pProgress->x+pProgress->lineWidth,pProgress->y+pProgress->lineWidth,\
			 progressLenth,halfWidth,pProgress->upperPointColor);
	lcd_fill(pProgress->x+pProgress->lineWidth,pProgress->y+pProgress->lineWidth+halfWidth,\
			 progressLenth,halfWidth,pProgress->lowerPointColor);
}

/*------------------------------------------------------------
 * Function Name  : RelinkPRM
 * Description    : 重新连接下位机
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RelinkPRM( uint16_t backColor )
{
	PROGRESS_BAR_TypeDef progressBar;
	LINK_STATUS_TypeDef linkStatus;

	progressBar.x = 696;
	progressBar.y = LCD_WIDTH_Y - 54;
	progressBar.lenth = 104;
	progressBar.width = 22;
	progressBar.upperPointColor = PROCESS_UP_COLOR;
	progressBar.lowerPointColor = PROCESS_DN_COLOR;
	progressBar.lineColor = COLOR_POINT;
	progressBar.backColor = backColor;
	progressBar.lineWidth = 2;
	progressBar.curProgress = 0;
	progressBar.totalProgress = 100;
	progressBar.pTitle = "";
	progressBar.fontSize = 16;
	progressBar.fontColor = COLOR_POINT;
	
	GUI_ProgressBar(&progressBar);
	
	link_init();
	
	bsp_StartTimer(GENERAL_TIMEOUT_TIMING,10000);
	
	while (linkStatus != LINK_IDLE)
	{
		linkStatus = link_cycle();
		
		progressBar.curProgress = GetLinkProcess();
		DrawProcessBody(&progressBar);
		
		switch ( linkStatus )
		{
			case LINK_IDLE:	
				lcd_fill(progressBar.x+progressBar.lineWidth,progressBar.y+progressBar.lineWidth,\
						 progressBar.lenth-2*progressBar.lineWidth,\
						 progressBar.width-2*progressBar.lineWidth,progressBar.backColor);
				break;
			case LINK_UNLINK:
				if (bsp_CheckTimer(GENERAL_TIMEOUT_TIMING))
				{
					return;
				}
				link_init();
				break;
			default:
				break;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : GUI_DrawProgressBar
 * Description    : 画进度条
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void GUI_DrawTestSaveProgressBar( uint16_t backColor )
{
	PROGRESS_BAR_TypeDef progressBar;
	uint8_t curProgress = 0;

	progressBar.x = 696;
	progressBar.y = LCD_WIDTH_Y - 54;
	progressBar.lenth = 104;
	progressBar.width = 22;
	progressBar.upperPointColor = PROCESS_UP_COLOR;
	progressBar.lowerPointColor = PROCESS_DN_COLOR;
	progressBar.lineColor = COLOR_POINT;
	progressBar.backColor = backColor;
	progressBar.lineWidth = 2;
	progressBar.curProgress = 0;
	progressBar.totalProgress = 100;
	progressBar.pTitle = "";
	progressBar.fontSize = 16;
	progressBar.fontColor = COLOR_POINT;
	
	GUI_ProgressBar(&progressBar);
	
	while (1)
	{			
		progressBar.curProgress = curProgress;
		DrawProcessBody(&progressBar);
		
		bsp_DelayMS(1);
		curProgress++;
		
		if (curProgress > 100)		
		{
			lcd_fill(progressBar.x+progressBar.lineWidth,progressBar.y+progressBar.lineWidth,\
						 progressBar.lenth-2*progressBar.lineWidth,\
						 progressBar.width-2*progressBar.lineWidth,progressBar.backColor);
			
			return;
		}
	}
}

/*------------------------------------------------------------
 * Function Name  : FromForceGetStrength
 * Description    : 从力值获取强度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float FromForceGetStrength( TEST_TYPE_TypeDef type, REPORT_TypeDef *report, float CurForce )
{
	float MPa = 0.0f;
	float area = 0;
	float span = 0;	//跨距	
	float high = 0;	//高度
	float width = 0;//宽度
	float lenth = 0;//长度
	float temp[20];//必须大于4，防止其他函数修改其值导致内存溢出

	switch ( type )
	{
		case KYSNJS:
			area = GetSampleSpecificationArea(report->sample_spec,temp);
		
			if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{												
				area = 1;
			}
			MPa = CurForce / area;

			ECHO_ASSERT(fabs(area)>MIN_FLOAT_PRECISION_DIFF_VALUE,"面积除零错误！\r\n");
			break;
			 
		case KYJZSJ:
			area = GetSampleSpecificationArea(report->sample_spec,temp);
			if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{											
				area = 1;
			}
			MPa = report->correct_cof * CurForce / area;
			
			ECHO_ASSERT(fabs(area)>MIN_FLOAT_PRECISION_DIFF_VALUE,"面积除零错误！\r\n");
			break;

		case KYHNT:		//混凝土抗折/抗压需要手动设置修正系数，根据修正系数计算最终的强度值
			area = GetSampleSpecificationArea(report->sample_spec,temp);
			
			if ( PASSED == JudgeCharIsEqual(2,report->sample_spec,"Φ") )
			{
				area = GetCircularArea(temp[0]);
			}
			
			if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{											
				area = 1;
			}
			
			MPa = report->correct_cof * CurForce / area;
			
			ECHO_ASSERT(fabs(area)>MIN_FLOAT_PRECISION_DIFF_VALUE,"面积除零错误！\r\n");
			break;

		case KZHNT:
			GetSampleSpecificationArea(report->sample_spec,temp);
			high = temp[0];
			width = temp[1];
			span = high * 3;
			if ((fabs(high) < MIN_FLOAT_PRECISION_DIFF_VALUE) || (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE))
			{												
				high = 1;
				width = 1;
			}
			MPa = (report->correct_cof * CurForce * span) / (width * high * high);
			
			ECHO_ASSERT(fabs(high)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件高度除零错误！\r\n");
			ECHO_ASSERT(fabs(width)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件宽度除零错误！\r\n");
			break;

		case KYQQZ:			
			lenth = report->length;
			width = report->width;
			if ((fabs(lenth) < MIN_FLOAT_PRECISION_DIFF_VALUE) || (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE))
			{							
				lenth = 1;
				width = 1;
			}
			MPa = CurForce / (lenth * width);
			
			ECHO_ASSERT(fabs(lenth)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件长度除零错误！\r\n");
			ECHO_ASSERT(fabs(width)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件宽度除零错误！\r\n");
			break;
		
		case KYZJDH:
			break;

		case KYTY:
		{
			switch (report->sample_shape_index)
			{
				case TYKY_SHAPE_RECTANGLE:
				case TYKY_SHAPE_ROUND:	
					area = report->gz_area;
					break;
				case TYKY_SHAPE_IRREGULAR:
					area = report->bgz_area;
					break;
			}
			
			if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{									
				area = 1;
			}
			MPa = report->correct_cof * CurForce / area;
			
			ECHO_ASSERT(fabs(area)>MIN_FLOAT_PRECISION_DIFF_VALUE,"面积除零错误！\r\n");
			break;
		}

		case KZSNJS:
			width = report->length;
			span = report->sample_span;
			if (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{												
				width = 1;
			}
			MPa = 1.5f * CurForce * span / (width * width * width);
			
			ECHO_ASSERT(fabs(width)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件宽度除零错误！\r\n");
			break;

		case KZYJSNJ:
			width = report->length;
			span = report->sample_span;
			if (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{							
				width = 1;
			}
			MPa = 1.5f * CurForce * span / (width * width * width);
			
			ECHO_ASSERT(fabs(width)>MIN_FLOAT_PRECISION_DIFF_VALUE,"试件宽度除零错误！\r\n");
			break;
 
		case KZTY:
			break;
		
		case KLJSSW:
			{
				float area = 0;
				
				switch (report->sample_shape_index)
				{
					case JSSWKL_SHAPE_RECTANGLE:
						area = report->gz_area;
						break;
					case JSSWKL_SHAPE_ROUND:
						area = report->gz_area;
						break;
					case JSSWKL_SHAPE_TUBE:
						area = report->gz_area;
						break;
					case JSSWKL_SHAPE_IRREGULAR:
						area = report->bgz_area;
						break;
				}
				
				if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
				{													
					area = 1;
				}
				MPa = CurForce / area;
				
				ECHO_ASSERT(fabs(area)>MIN_FLOAT_PRECISION_DIFF_VALUE,"面积除零错误！\r\n");
			}
			break;
		
		default:
			break;
	}
	
	return MPa;
}

/*------------------------------------------------------------
 * Function Name  : CheckOfflineCycle
 * Description    : 检测脱机循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CheckOfflineCycle( void )
{
	const uint32_t MAX_KEEP_LIVE_TIME = 5 * CTRL_FREQ;		//5秒检测一次
	uint32_t keepLiveCount = GetKeepLiveCount();
	
	if (keepLiveCount > MAX_KEEP_LIVE_TIME)
	{
		RelinkPRM(LIGHT_GRAY2);
		SetKeepLiveCount(0);
	}
	else
	{
		keepLiveCount++;
		SetKeepLiveCount(keepLiveCount);
	}
}

/*------------------------------------------------------------
 * Function Name  : SetErrorStatus
 * Description    : 设置错误状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void SetErrorStatus( uint8_t errCode, const char * const *pCue, const uint8_t rowNum )
{
	uint16_t rwStatus = 0;
	
	SetCoreStatus(STATUS_CORE_WARNING);
	
	g_systemProtect.errCode = errCode;
	g_systemProtect.pErrCue = pCue;
	g_systemProtect.rowNum = rowNum;
	
	rwStatus = GetReadWriteOutput();
	
	if (rwStatus & (1<<BIT_PUMP))
	{
		SendClosePumpCmd();
	}
}

/*------------------------------------------------------------
 * Function Name  : ClearErrorStatus
 * Description    : 清除错误状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ClearErrorStatus( void )
{
	SetCoreStatus(STATUS_CORE_MANULE);
	
	g_systemProtect.errCode = STATUS_OK;
}

/*------------------------------------------------------------
 * Function Name  : CheckSystemMaxForceWarning
 * Description    : 检测系统最大力警告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CheckSystemMaxForceWarning( SMPL_NAME_TypeDef tureChannel )
{
	float force = get_smpl_value(tureChannel);
	float maxForce = smpl_ctrl_full_p_get(tureChannel);
	
	if (force > maxForce * 1.03f)
	{	
		SetErrorStatus(STATUS_ERROR_PROT_P_FH,&LssuedParameterWarning[1],1);
	
		return;
	}
	
	if (force < maxForce * (-1.03f))
	{	
		SetErrorStatus(STATUS_ERROR_PROT_P_FH,&LssuedParameterWarning[2],1);
		
		return;
	}
}

/*------------------------------------------------------------
 * Function Name  : CheckSystemWarningStatusCycle
 * Description    : 检测系统警告
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CheckSystemWarningStatusCycle( void )
{
	MODEL_TYPE_TypeDef modelType;
	
	modelType = GetModelType();
	
	ClearErrorStatus();
	
	switch (modelType)
	{
		case MODEL_KY:
			CheckSystemMaxForceWarning(SMPL_FH_NUM);
			break;
		case MODEL_KZ:
			CheckSystemMaxForceWarning(SMPL_FH_NUM);
			break;
		case MODEL_KZKY:
			CheckSystemMaxForceWarning(SMPL_FH_NUM);
			break;
		case MODEL_UNIVERSAL:
			CheckSystemMaxForceWarning(SMPL_FH_NUM);	
			break;
		case MODEL_UNDEFINED:			
			break;
		default:
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetDrawLineAddr
 * Description    : 获取坐标系画线地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
COORDINATE_DRAW_LINE_TypeDef *GetDrawLineAddr( void )
{
	return &g_coordinateDrawLine;
}

/*------------------------------------------------------------
 * Function Name  : GetDrawLineNowTimePoint
 * Description    : 获取坐标系画线当前时间点
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t GetDrawLineNowTimePoint( void )
{
	return g_coordinateDrawLine.nowTimePoint;
}


/*------------------------------------------------------------
 * Function Name  : GetDrawLineNextTimePoint
 * Description    : 获取坐标系画线下一个时间点
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t GetDrawLineNextTimePoint( void )
{
	uint32_t nowIndex = g_coordinateDrawLine.nowTimePoint;
	
	nowIndex++;
	
	if (nowIndex < DECORD_COORDINATE_FORCE_NUM)
	{
		return nowIndex;
	}
	else
	{
		return g_coordinateDrawLine.nowTimePoint;
	}
}

/*------------------------------------------------------------
 * Function Name  : GetDrawLineSomeTimePointForce
 * Description    : 获取坐标系画线某个时间点力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetDrawLineSomeTimePointForce( uint32_t nowTimePoint )
{
	if (nowTimePoint >= DECORD_COORDINATE_FORCE_NUM)
	{
		return 0.0f;
	}
	else
	{
		return g_coordinateDrawLine.force[nowTimePoint];
	}
}	

/*------------------------------------------------------------
 * Function Name  : IsCoordinateDrawLinePointOutOfRange
 * Description    : 坐标系画线点超出范围
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
__STATIC_INLINE BoolStatus IsCoordinateDrawLinePointOutOfRange( COORDINATE_DRAW_LINE_TypeDef *pDrawLine, \
						uint16_t curPointX, uint16_t curPointY )
{
	if ( (curPointX<pDrawLine->originX) || (curPointX>pDrawLine->originX+pDrawLine->lenthX) )
	{
		return YES;
	}
	
	if ( (curPointY>pDrawLine->originY) || (curPointY<pDrawLine->originY-pDrawLine->lenthY) )
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : CoordinateDrawLine
 * Description    : 坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CoordinateDrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	uint16_t lastXCoordinate = 0,nowXCoordinate = 0;
	uint16_t lastYCoordinate = 0,nowYCoordinate = 0;
	
	if (pDrawLine->nowTimePoint == 0)
	{
		return;
	}
	
	switch ( pDrawLine->xType )
	{
		case COORDINATE_USE_TIME:
			{	
				float lastX = (pDrawLine->nowTimePoint - 1) * RECORD_COORDINATE_PERIOD;
				float nowX = pDrawLine->nowTimePoint * RECORD_COORDINATE_PERIOD;
					
				lastXCoordinate = pDrawLine->originX + (float)lastX / \
									pDrawLine->xMaxValue * pDrawLine->lenthX;
				nowXCoordinate = pDrawLine->originX + (float)nowX / \
									pDrawLine->xMaxValue * pDrawLine->lenthX;
			}
			break;
		case COORDINATE_USE_DEFORM:
			{			
				float lastX = pDrawLine->deform[pDrawLine->nowTimePoint-1];
				float nowX = pDrawLine->deform[pDrawLine->nowTimePoint];
					
				lastXCoordinate = pDrawLine->originX + (float)lastX / \
									pDrawLine->xMaxValue * pDrawLine->lenthX;
				nowXCoordinate = pDrawLine->originX + (float)nowX / \
									pDrawLine->xMaxValue * pDrawLine->lenthX;
			}	
			break;
		default:
			break;
	}
	
	switch ( pDrawLine->yType )
	{
		case COORDINATE_USE_FORCE:
			{		
				float lastY = pDrawLine->force[pDrawLine->nowTimePoint-1];
				float nowY = pDrawLine->force[pDrawLine->nowTimePoint];
				
				lastYCoordinate = pDrawLine->originY - lastY / pDrawLine->yMaxValue * pDrawLine->lenthY;
				nowYCoordinate = pDrawLine->originY - nowY / pDrawLine->yMaxValue * pDrawLine->lenthY;
			}
			break;
		default:
			break;
	}
	
	if (IsCoordinateDrawLinePointOutOfRange(pDrawLine,lastXCoordinate,lastYCoordinate) == YES)
	{
		return;
	}
	
	if (IsCoordinateDrawLinePointOutOfRange(pDrawLine,nowXCoordinate,nowYCoordinate) == YES)
	{
		return;
	}

	lcd_draw_line(lastXCoordinate,lastYCoordinate,nowXCoordinate,nowYCoordinate,pDrawLine->lineColor);
}

/*------------------------------------------------------------
 * Function Name  : ReloadCoordinate
 * Description    : 重绘坐标系
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ReloadCoordinate( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	float xMaxValue = pDrawLine->xMaxValue * pDrawLine->xScalingCoefficient;
	float yMaxValue = pDrawLine->yMaxValue * pDrawLine->yScalingCoefficient;
	uint8_t xType = pDrawLine->xType;
	uint8_t yType = pDrawLine->yType;
	
	pDrawLine->pDrawCoordinate(xType,yType,(void *)&xMaxValue,(void *)&yMaxValue);
}	

/*------------------------------------------------------------
 * Function Name  : CoordinateRedrawLine
 * Description    : 重绘坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CoordinateRedrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	uint32_t lastTime = 0;
	uint32_t nowTime = 0;
	float lastX_f = 0,nowX_f = 0;
	float lastY_f = 0,nowY_f = 0;
	uint16_t lastXCoordinate = 0,nowXCoordinate = 0;
	uint16_t lastYCoordinate = 0,nowYCoordinate = 0;
	uint32_t index = 0;
	
	for (index=0; index<=pDrawLine->nowTimePoint; ++index)
	{	
		switch ( pDrawLine->xType )
		{
			case COORDINATE_USE_TIME:
				{										
					lastTime = nowTime;
					nowTime += 1000 / pDrawLine->recordPointFreq;
						
					lastXCoordinate = pDrawLine->originX + (float)lastTime / \
										pDrawLine->xMaxValue * pDrawLine->lenthX;
					nowXCoordinate = pDrawLine->originX + (float)nowTime / \
										pDrawLine->xMaxValue * pDrawLine->lenthX;
				}
				break;
			case COORDINATE_USE_DEFORM:
				{										
					lastX_f = nowX_f;
					nowX_f = pDrawLine->deform[index];
						
					lastXCoordinate = pDrawLine->originX + (float)lastX_f / \
										pDrawLine->xMaxValue * pDrawLine->lenthX;
					nowXCoordinate = pDrawLine->originX + (float)nowX_f / \
										pDrawLine->xMaxValue * pDrawLine->lenthX;
				}	
				break;
			default:
				break;
		}
		
		switch ( pDrawLine->yType )
		{
			case COORDINATE_USE_FORCE:
				{					
					lastY_f = nowY_f;
					nowY_f = pDrawLine->force[index];
					
					lastYCoordinate = pDrawLine->originY - lastY_f / pDrawLine->yMaxValue * pDrawLine->lenthY;
					nowYCoordinate = pDrawLine->originY - nowY_f / pDrawLine->yMaxValue * pDrawLine->lenthY;
				}
				break;
			default:
				break;
		}
		
		if (IsCoordinateDrawLinePointOutOfRange(pDrawLine,lastXCoordinate,lastYCoordinate) == YES)
		{
			continue;
		}
		
		if (IsCoordinateDrawLinePointOutOfRange(pDrawLine,nowXCoordinate,nowYCoordinate) == YES)
		{
			continue;
		}
	
		lcd_draw_line(lastXCoordinate,lastYCoordinate,nowXCoordinate,nowYCoordinate,pDrawLine->lineColor);				
	}
}	

/*------------------------------------------------------------
 * Function Name  : IsCoordinateRecordPointOverflow
 * Description    : 坐标系记录点溢出
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus IsCoordinateRecordPointOverflow( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	if (pDrawLine->nowTimePoint >= DECORD_COORDINATE_FORCE_NUM)
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : CoordinateDrawLineBodyCycle
 * Description    : 坐标系画线循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CoordinateDrawLineBodyCycle( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	if (pDrawLine->start == RESET)
	{
		return;
	}
	
	CoordinateDrawLine(pDrawLine);
	
	if (pDrawLine->enableRedraw == ENABLE)
	{
		pDrawLine->enableRedraw = DISABLE;
		
		ReloadCoordinate(pDrawLine);
		
		CoordinateRedrawLine(pDrawLine);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendChannelTareCmd
 * Description    : 通道清零命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus SendChannelTareCmd( SMPL_NAME_TypeDef channel )
{
	TARE_CMD_STATUS_TypeDef status = SEND_TARE_CMD;
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	bsp_StartTimer(SEND_CMD_TIMEOUT,100);
	
	while (1)
	{
		switch ( status )
		{
			case SEND_TARE_CMD:
				cmd_set_tare_pkg(channel);
				status = WAIT_TARE_END;
				break;
			case WAIT_TARE_END:
				cmdStatus = PCM_CmdSendCycle();
				replyStatus = GetPrmReplyStatus();
			
				if ( ( (cmdStatus==CMD_IDLE) && (replyStatus==STATUS_OK) ))
				{
					return SUCCESS;
				}
				
				if ( bsp_CheckTimer(SEND_CMD_TIMEOUT) )
				{
					return ERROR;
				}
				break;
		}
	}
}

typedef enum
{
	STATUS_SCREW_IDLE = 0,
	STATUS_SCREW_UP,
	STATUS_SCREW_DOWN,
	STATUS_SCREW_END,
}SCREW_UP_DOWN_STATUS_TypeDef;

/*------------------------------------------------------------
 * Function Name  : ScrewUpDownTask
 * Description    : 丝杆升降任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void ScrewUpDownTask( void )
{
	static SCREW_UP_DOWN_STATUS_TypeDef s_screwStatus = STATUS_SCREW_IDLE;
	static FlagStatus pushUP = RESET;
	static FlagStatus pushDOWN = RESET;
	
	if (IsPressKey() == YES)
	{					
		switch ( GetKeyVal() )
		{
			case KEY_SCREW_UP:
				pushUP = SET;
				break;
			case KEY_SCREW_DOWN:
				pushDOWN = SET;
				break;
			default:				
				break;
		}
	}
	
	if (GetKeyStatus() == STATUS_KEY_NO_PRESS)
	{
		pushUP = RESET;
		pushDOWN = RESET;
	}
	
	switch ( s_screwStatus )
	{
		case STATUS_SCREW_IDLE:
			ECHO(DEBUG_SCREW_UP_DOWN,"空闲\r\n");
			if (pushUP == SET)
			{
				SendScrewUpCmd();
				s_screwStatus = STATUS_SCREW_UP;
			}
			else if (pushDOWN == SET)
			{
				SendScrewDownCmd();
				s_screwStatus = STATUS_SCREW_DOWN;
			}
			break;
		case STATUS_SCREW_UP:
			ECHO(DEBUG_SCREW_UP_DOWN,"上升\r\n");
			if (pushUP == RESET)
			{
				s_screwStatus = STATUS_SCREW_END;
			}
			break;
		case STATUS_SCREW_DOWN:
			ECHO(DEBUG_SCREW_UP_DOWN,"下降\r\n");			
			if (pushDOWN == RESET)
			{
				s_screwStatus = STATUS_SCREW_END;
			}
			break;
		case STATUS_SCREW_END:
			ECHO(DEBUG_SCREW_UP_DOWN,"停止\r\n");
			SendScrewStopCmd();
			s_screwStatus = STATUS_SCREW_IDLE;
			break;
		default:
			s_screwStatus = STATUS_SCREW_IDLE;
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : ExecuteTask
 * Description    : 执行机构任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ExecuteTask( void )
{
	uint16_t rwStatus = 0; 
	
	if (IsPressKey() == YES)
	{	
		rwStatus = GetReadWriteOutput();
		
		switch ( GetKeyVal() )
		{
			case KEY_OIL_RUN:
				if ( !(rwStatus & (1<<BIT_PUMP)) )
				{
					SendOpenPumpCmd();
				}
				break;
			case KEY_OIL_STOP:
				if (rwStatus & (1<<BIT_PUMP))
				{
					SendClosePumpCmd();
				}
				break;
			default:
				break;
		}
	}

	ScrewUpDownTask();
}

/*------------------------------------------------------------
 * Function Name  : SendOpenPumpCmd
 * Description    : 发送打开油泵命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SendOpenPumpCmd( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	cmd_switch_pkg(0,(1<<BIT_PUMP),0);
	
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	if ( ( (cmdStatus!=CMD_IDLE) || (replyStatus!=STATUS_OK) ))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPumpWarning[0]);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendClosePumpCmd
 * Description    : 发送关闭油泵命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SendClosePumpCmd( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	cmd_switch_pkg((1<<BIT_PUMP),0,0);
	
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	if ( ( (cmdStatus!=CMD_IDLE) || (replyStatus!=STATUS_OK) ))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPumpWarning[1]);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendScrewUpCmd
 * Description    : 发送上升丝杆命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SendScrewUpCmd( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	cmd_switch_pkg(0,(0x01<<BIT_SCREW),0);
	
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	if ( ( (cmdStatus!=CMD_IDLE) || (replyStatus!=STATUS_OK) ))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPumpWarning[2]);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendScrewDownCmd
 * Description    : 发送下降丝杆命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SendScrewDownCmd( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	cmd_switch_pkg(0,(0x02<<BIT_SCREW),0);
	
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	if ( ( (cmdStatus!=CMD_IDLE) || (replyStatus!=STATUS_OK) ))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPumpWarning[3]);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendScrewStopCmd
 * Description    : 发送停止丝杆命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SendScrewStopCmd( void )
{
	CMD_STATUS_TypeDef cmdStatus;
	uint8_t replyStatus;
	
	cmd_switch_pkg((0x03<<BIT_SCREW),0,0);
	
	cmdStatus = PCM_CmdSendCycle();
	replyStatus = GetPrmReplyStatus();
	
	if ( ( (cmdStatus!=CMD_IDLE) || (replyStatus!=STATUS_OK) ))
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&pPumpWarning[4]);
	}
}

/*------------------------------------------------------------
 * Function Name  : PumpStatusLampTask
 * Description    : 油泵状态灯任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PumpStatusLampTask( void )
{
	uint16_t rwStatus = 0;
	
	rwStatus = GetReadWriteOutput();
	
	if (rwStatus & (1<<BIT_PUMP))
	{
		SetLampBit(LAMP_PUMP_START);
		ClrLampBit(LAMP_PUMP_STOP);
	}
	else
	{
		SetLampBit(LAMP_PUMP_STOP);
		ClrLampBit(LAMP_PUMP_START);
	}
	
	LampTask();
}

/*------------------------------------------------------------
 * Function Name  : CommucationLampTask
 * Description    : 通讯小灯任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitCommucationLamp( void )
{
	g_commLed.status = COMM_LED_IDLE;
	g_commLed.delayTime = 0;
	g_commLed.startLamp = RESET;
	
	ClrLampBit(LAMP_BOARD_COM);
	LampTask();
}

/*------------------------------------------------------------
 * Function Name  : SetCommucationLampStart
 * Description    : 设置通讯小灯执行
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetCommucationLampStart( void )
{
	g_commLed.startLamp = SET;
}

/*------------------------------------------------------------
 * Function Name  : CommucationLampTask
 * Description    : 通讯小灯任务
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void CommucationLampTask( void )
{
	switch ( g_commLed.status )
	{
		case COMM_LED_IDLE:
			if (g_commLed.startLamp == SET)
			{
				g_commLed.startLamp = RESET;
				g_commLed.delayTime = 10;
				
				ClrLampBit(LAMP_BOARD_COM);
				
				g_commLed.status = COMM_LED_OFF;
			}
			break;
			
		case COMM_LED_OFF:
			if (g_commLed.delayTime > 5)
			{
				g_commLed.delayTime--;
			}
			else
			{
				SetLampBit(LAMP_BOARD_COM);
				g_commLed.status = COMM_LED_DELAY;
			}
			break;
			
		case COMM_LED_DELAY:
			if (g_commLed.delayTime)
			{
				g_commLed.delayTime--;
			}
			else
			{
				g_commLed.status = COMM_LED_IDLE;
			}	
			break;
	}
	
	LampTask();
}

/*------------------------------------------------------------
 * Function Name  : SetCoreStatus
 * Description    : 设置系统核心状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetCoreStatus( CORE_STATUS_TypeDef status )
{
	g_systemProtect.coreStatus = status;
}

/*------------------------------------------------------------
 * Function Name  : GetCoreStatus
 * Description    : 获取系统核心状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CORE_STATUS_TypeDef GetCoreStatus( void )
{
	return g_systemProtect.coreStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetSystemProtectAddr
 * Description    : 获取系统保护地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SYSTEM_PROTECT_TypeDef *GetSystemProtectAddr( void )
{
	return &g_systemProtect;
}

/*------------------------------------------------------------
 * Function Name  : ShowPrintingShortcutMenu
 * Description    : 显示正在打印快捷菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ShowPrintingShortcutMenu( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();

	pShortCut->status = SHOW_F1;
	
	pShortCut->pContent[0] = pTwoLevelMenu[33];		
	
	pShortCut->pointColor = COLOR_SHORTCUT_POINT;
	pShortCut->backColor = COLOR_SHORTCUT_BACK;
	
	ShortcutMenuTask(pShortCut);	
}

/*------------------------------------------------------------
 * Function Name  : ShowExportingShortcutMenu
 * Description    : 显示正在导出快捷菜单
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void ShowExportingShortcutMenu( void )
{
	SHORTCUT_TypeDef *pShortCut = GetShortcutMenuAddr();

	pShortCut->status = SHOW_F1;
	
	pShortCut->pContent[0] = pTwoLevelMenu[78];		
	
	pShortCut->pointColor = COLOR_SHORTCUT_POINT;
	pShortCut->backColor = COLOR_SHORTCUT_BACK;
	
	ShortcutMenuTask(pShortCut);	
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentLinkStatus
 * Description    : 设置动态内容联机状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentLinkStatus( void )
{
	SetInterfaceLinkStatus(GetLinkStatus());
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentCode
 * Description    : 设置动态内容码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentCode( SMPL_NAME_TypeDef tureChannel )
{
	int32_t code = GetSammpleCode(tureChannel);
	
	switch ( tureChannel )
	{
		case SMPL_FH_NUM:
			SetInterfaceElementFHCode(code);
			break;
		case SMPL_WY_NUM:
			SetInterfaceElementWYCode(code);
			break;
		case SMPL_BX_NUM:
			SetInterfaceElementBXCode(code);
			break;
		default:
			break;
	}	
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentUSBStatus
 * Description    : 设置动态内容USB状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentUSBStatus( void )
{
	if (Get_USB_Status() == SUCCESS)
	{
		SetInterfaceElementUSBStatus(YES);
	}
	else
	{
		SetInterfaceElementUSBStatus(NO);
	}
}	

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentNETStatus
 * Description    : 设置动态内容NET状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentNETStatus( void )
{
	SetInterfaceElementEthernetConnectStatus(GetEthLinkStatus());
}	

/*------------------------------------------------------------
 * Function Name  : SetShortCutMenuCue
 * Description    : 设置快捷菜单区域提示内容
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetShortCutMenuCue( uint16_t pointColor, uint16_t backColor, const char *pCue)
{
	const uint8_t fontSize = 24;
	uint16_t x = (( LCD_LENTH_X - strlen(pCue) * (fontSize >> 1)) >> 1);
	uint16_t y = LCD_WIDTH_Y - 30;
	
	ClearShortCutMenuArea(backColor);
	
	GUI_DispStr24At(x,y+2,pointColor,backColor,pCue);
}

/*------------------------------------------------------------
 * Function Name  : InitLssuedParameter
 * Description    : 初始化下发参数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitLssuedParameter( void )
{
	g_lssuedParameter.backColor = COLOR_BACK;
	g_lssuedParameter.isLssued = NO;
	g_lssuedParameter.lssued = RelinkPRM;
}

/*------------------------------------------------------------
 * Function Name  : LssuedParameterProcess
 * Description    : 下发参数处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void LssuedParameterProcess( void )
{
	if (g_lssuedParameter.isLssued == NO)
	{
		return;
	}
		
	g_lssuedParameter.lssued(g_lssuedParameter.backColor);
	
	if (GetLinkStatus() != LINK_IDLE)
	{
		SetPopWindowsInfomation(POP_PCM_CUE,1,&LssuedParameterWarning[0]);
		
		PopWindowsProcessCycle();
	}
}

/*------------------------------------------------------------
 * Function Name  : SetLssuedParameter
 * Description    : 设置参数处理
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetLssuedParameter( void )
{
	g_lssuedParameter.isLssued = YES;
}

/*------------------------------------------------------------
 * Function Name  : GetCurveShowStartValue
 * Description    : 获取曲线显示起始力
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetCurveShowStartValue( SMPL_NAME_TypeDef channel )
{
	if (channel >= CTRL_CHN)
	{
		return 0;
	}
	else
	{
		return pHmi->start_force[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : DrawCircleMark
 * Description    : 画圆标记
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void DrawCircleMark( DRAW_CIRCLE_MARK_TypeDef *pDrawCircle )
{	
	uint16_t index;
	
	uint16_t x,y;
	
	x = pDrawCircle->x;
	y = pDrawCircle->y;
	
	for (index=1; index<=pDrawCircle->sumSerial; ++index)
	{
		if (index != pDrawCircle->curSerial)
		{
			lcd_draw_circle(x,y,pDrawCircle->radius,pDrawCircle->pointColor);
		}
		else
		{
			lcd_fill_circle(x,y,pDrawCircle->radius,pDrawCircle->pointColor);
		}
		
		x += pDrawCircle->distance + 2 * pDrawCircle->radius;
	}
}

/*------------------------------------------------------------
 * Function Name  : InitJudgeBreakPoint
 * Description    : 初始化判破
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitJudgeBreakPoint( void )
{
	uint8_t i;
	
	for (i=0; i<SMPL_NUM; ++i)
	{
		g_judgeBreak.index[i] = 0;	
		g_judgeBreak.lastForcePoint[i] = 0;
		g_judgeBreak.nowForcePoint[i] = 0;
		g_judgeBreak.downPoint[i] = 0;	
		g_judgeBreak.peakForce[i] = 0;
		g_judgeBreak.maxForceDiff[i] = 0;
		g_judgeBreak.adjoinPointDiff[i] = 0;
		g_judgeBreak.attenuationRate[i] = 0;
	}
}

/*------------------------------------------------------------
 * Function Name  : InitSampleProcess
 * Description    : 初始化采样过程
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitSampleProcess( uint8_t chn )
{
	if (chn < SMPL_NUM)
	{
		SAMPLE_PROCESS_TypeDef *samplePtr = &g_sampleProcess[chn];
		
		samplePtr->index = 0;
		samplePtr->completeSignal = RESET;	
	}
}

/*------------------------------------------------------------
 * Function Name  : GetSampleCompleteFlag
 * Description    : 获取采样完成标识
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FlagStatus GetSampleCompleteFlag( uint8_t chn )
{
	if (chn < SMPL_NUM)
	{
		SAMPLE_PROCESS_TypeDef *samplePtr = &g_sampleProcess[chn];
		
		return samplePtr->completeSignal;	
	}
	
	return RESET;
}

/*------------------------------------------------------------
 * Function Name  : JudgeBreakCalculateCycle
 * Description    : 计算判破循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void JudgeBreakCalculateCycle( uint8_t chn )
{
	const uint8_t INDEX_MAX = SAMPLE_PERIOD / CTRL_PERIOD;
	const uint16_t MAX_DOWN_POINT_NUM = 100;
	SAMPLE_PROCESS_TypeDef *samplePtr = &g_sampleProcess[chn];

	if (chn >= SMPL_NUM)
	{
		return;
	}
	
	ECHO(DEBUG_COORDINATE_DRAW_LINE,"采样索引：%d\r\n",samplePtr->index);
	
	samplePtr->index++;
	samplePtr->completeSignal = RESET;
	
	/* 100ms采样一个点 */
	if (samplePtr->index >= INDEX_MAX)	
	{		
		samplePtr->index = 0;
		samplePtr->completeSignal = SET;
		
		g_judgeBreak.lastForcePoint[chn] = g_judgeBreak.nowForcePoint[chn];
		g_judgeBreak.nowForcePoint[chn] = get_smpl_value(chn);
		
		/* 计算下降点 */
		if (fabs(g_judgeBreak.nowForcePoint[chn]) < fabs(g_judgeBreak.lastForcePoint[chn]))
		{
			g_judgeBreak.downPoint[chn]++;		
		}
		else
		{
			g_judgeBreak.downPoint[chn] = 0;
		}
		if (g_judgeBreak.downPoint[chn] > MAX_DOWN_POINT_NUM)
		{
			g_judgeBreak.downPoint[chn] = MAX_DOWN_POINT_NUM;
		}
		
		/* 计算峰值 */
		if (fabs(g_judgeBreak.nowForcePoint[chn]) > fabs(g_judgeBreak.peakForce[chn]))
		{
			g_judgeBreak.peakForce[chn] = g_judgeBreak.nowForcePoint[chn];
		}
		
		/* 计算与最大力差值 */
		if (fabs(g_judgeBreak.nowForcePoint[chn]) < fabs(g_judgeBreak.peakForce[chn]))
		{
			g_judgeBreak.maxForceDiff[chn] = fabs(g_judgeBreak.peakForce[chn] - g_judgeBreak.nowForcePoint[chn]);
		}
		else
		{
			g_judgeBreak.maxForceDiff[chn] = 0;
		}
		
		/* 计算相邻两点差值 */
		if (fabs(g_judgeBreak.nowForcePoint[chn]) < fabs(g_judgeBreak.lastForcePoint[chn]))
		{
			g_judgeBreak.adjoinPointDiff[chn] = fabs(g_judgeBreak.lastForcePoint[chn] - g_judgeBreak.nowForcePoint[chn]);
		}
		else
		{
			g_judgeBreak.adjoinPointDiff[chn] = 0;
		}
		
		/* 衰减率 */
		if (fabs(g_judgeBreak.nowForcePoint[chn]) < fabs(g_judgeBreak.peakForce[chn]))
		{
			g_judgeBreak.attenuationRate[chn] = fabs( (g_judgeBreak.nowForcePoint[chn] - g_judgeBreak.peakForce[chn])/ \
												g_judgeBreak.peakForce[chn] * 100 );
		}
		else
		{
			g_judgeBreak.attenuationRate[chn] = 0;
		}
		
		if (chn == SMPL_FH_NUM)
		{
			ECHO(DEBUG_SAMPLE_FH,"下降点：    %d\r\n",g_judgeBreak.downPoint[SMPL_FH_NUM]);
			ECHO(DEBUG_SAMPLE_FH,"最大力差值：%f\r\n",g_judgeBreak.maxForceDiff[SMPL_FH_NUM]);
			ECHO(DEBUG_SAMPLE_FH,"相邻点差值：%f\r\n",g_judgeBreak.adjoinPointDiff[SMPL_FH_NUM]);
		}
		
		ECHO(DEBUG_SAMPLE_FH,"判破力值：%f\r\n",g_judgeBreak.nowForcePoint[chn]);
	}
}

/*------------------------------------------------------------
 * Function Name  : GetWithMaxForceDifference
 * Description    : 获取最大力差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetWithMaxForceDifference( SMPL_NAME_TypeDef channel )
{
	if (channel >= SMPL_NUM)
	{
		return 0;
	}
	else
	{
		return g_judgeBreak.maxForceDiff[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetAdjoinTwoPointDifference
 * Description    : 获取相邻两点差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetAdjoinTwoPointDifference( SMPL_NAME_TypeDef channel )
{
	if (channel >= SMPL_NUM)
	{
		return 0;
	}
	else
	{
		return g_judgeBreak.adjoinPointDiff[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetBreakDownPoint
 * Description    : 获取破碎下降点数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint16_t GetBreakDownPoint( SMPL_NAME_TypeDef channel )
{
	if (channel >= SMPL_NUM)
	{
		return 0;
	}
	else
	{
		return g_judgeBreak.downPoint[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetAttenuationRate
 * Description    : 获取衰减率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetAttenuationRate( SMPL_NAME_TypeDef channel )
{
	if (channel >= SMPL_NUM)
	{
		return 0;
	}
	else
	{
		return g_judgeBreak.attenuationRate[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetPeakValue
 * Description    : 获取峰值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetPeakValue( SMPL_NAME_TypeDef channel )
{
	if (channel >= SMPL_NUM)
	{
		return 0;
	}
	else
	{
		return g_judgeBreak.peakForce[channel];
	}
}

/*------------------------------------------------------------
 * Function Name  : GetDisplacementOrDeformShow
 * Description    : 获取位移/变形显示
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
DISPLACEMENT_CONV_DEFORM_TypeDef GetDisplacementOrDeformShow( void )
{
	if (pHmi->wyConvBx == 0)
	{
		return SHOW_DISPLACEMENT;
	}
	else
	{
		return SHOW_DEFORM;
	}
}

/*------------------------------------------------------------
 * Function Name  : SetDisplacementOrDeformShow
 * Description    : 设置位移/变形显示
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDisplacementOrDeformShow( DISPLACEMENT_CONV_DEFORM_TypeDef newFunc )
{
	if (newFunc == SHOW_DISPLACEMENT)
	{
		pHmi->wyConvBx = 0;
	}
	else
	{
		pHmi->wyConvBx = 1;
	}
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentForce
 * Description    : 设置动态内容力值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentForce( FH_UINT_TypeDef fhUnit )
{
	float force = 0;
	
	force = get_smpl_value(SMPL_FH_NUM);
	
	if (fhUnit == FH_UNIT_kN)
	{
		force /= 1000;
	}

	SetInterfaceElementForce(force);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentDispalcement
 * Description    : 设置动态内容位移
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentDispalcement( WY_UINT_TypeDef wyUnit )
{
	float disPlacement = 0;
	
	disPlacement = get_smpl_value(SMPL_WY_NUM);
	
	switch ( wyUnit )
	{
		case WY_UNIT_MM:
				
			break;
		case WY_UNIT_CM:
			disPlacement /= 10;	
			break;
		case WY_UNIT_DM:
			disPlacement /= 100;		
			break;
		case WY_UNIT_M:
			disPlacement /= 1000;		
			break; 
		default:
			break;
	}

	SetInterfaceElementDisPlacement(disPlacement);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentDeform
 * Description    : 设置动态内容变形
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentDeform( BX_UINT_TypeDef bxUnit )
{
	float deform = 0;
	
	deform = get_smpl_value(SMPL_BX_NUM);
	
	switch ( bxUnit )
	{
		case BX_UNIT_MM:
				
			break;
		case BX_UNIT_CM:
			deform /= 10;	
			break;
		case BX_UNIT_DM:
			deform /= 100;		
			break;
		case BX_UNIT_M:
			deform /= 1000;		
			break; 
		default:
			break;
	}

	SetInterfaceElementDeform(deform);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentFHSpeed
 * Description    : 设置动态内容负荷速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentFHSpeed( FH_UINT_TypeDef fhUnit )
{
	float speed = 0;
	
	speed = get_smpl_spd(SMPL_FH_NUM);
	
	if (fhUnit == FH_UNIT_kN)
	{
		speed /= 1000;
	}	
	
	SetInterfaceElementFHSpeed(speed);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentWYSpeed
 * Description    : 设置动态内容位移速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentWYSpeed( WY_UINT_TypeDef wyUnit )
{
	float speed = 0;
	
	speed = get_smpl_spd(SMPL_WY_NUM);
	
	switch ( wyUnit )
	{
		case WY_UNIT_MM:
				
			break;
		case WY_UNIT_CM:
			speed /= 10;	
			break;
		case WY_UNIT_DM:
			speed /= 100;	
			break;
		case WY_UNIT_M:
			speed /= 1000;	
			break; 
		default:
			break;
	}

	SetInterfaceElementWYSpeed(speed);
}

/*------------------------------------------------------------
 * Function Name  : SetDynamicContentBXSpeed
 * Description    : 设置动态内容变形速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetDynamicContentBXSpeed( BX_UINT_TypeDef bxUnit )
{
	float speed = 0;
	
	speed = get_smpl_spd(SMPL_BX_NUM);
	
	switch ( bxUnit )
	{
		case BX_UNIT_MM:
				
			break;
		case BX_UNIT_CM:
			speed /= 10;	
			break;
		case BX_UNIT_DM:
			speed /= 100;	
			break;
		case BX_UNIT_M:
			speed /= 1000;	
			break; 
		default:
			break;
	}

	SetInterfaceElementBXSpeed(speed);
}

/*------------------------------------------------------------
 * Function Name  : GetExtensometerGauge
 * Description    : 获取引伸计标距
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetExtensometerGauge( void )
{
	return pTest->extensometerGauge;
}

/*------------------------------------------------------------
 * Function Name  : GetParallelLenth
 * Description    : 获取平行长度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetParallelLenth( void )
{
	return pTest->parallelLenth;
}

/*------------------------------------------------------------
 * Function Name  : GetOriginalGauge
 * Description    : 获取原始标距
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetOriginalGauge( void )
{
	return pTest->originalGauge;
}

/*------------------------------------------------------------
 * Function Name  : GetCoordinatePointAddr
 * Description    : 获取坐标点地址
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
COORDINATE_POINT_TypeDef *GetCoordinatePointAddr( void )
{
	return &g_coordinatePoint;
}

/*------------------------------------------------------------
 * Function Name  : PrintfTypeDefSize
 * Description    : 打印结构体类型大小
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void PrintfTypeDefSize( uint32_t len )
{
	printf("结构体占内存大小：%d\r\n",len);
}

/*------------------------------------------------------------
 * Function Name  : UintDivisionTen
 * Description    : 无符号整型除10
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void UintDivisionTen( uint32_t *pSource )
{
	*pSource /= 10;
}	

/*------------------------------------------------------------
 * Function Name  : FloatDivisionTen
 * Description    : 浮点数除10
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void FloatDivisionTen( float *pSource )
{
	*pSource /= 10;
}

/*------------------------------------------------------------
 * Function Name  : AccordUnitConvValue
 * Description    : 根据单位转换值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void AccordUnitConvValue( SMPL_NAME_TypeDef channel, void *pVal, pFunctionDevide devideTen )
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
					devideTen(pVal);
					devideTen(pVal);
					devideTen(pVal);
					break;
				case FH_UNIT_N:						
					break;
			}
			break;
		
		case SMPL_WY_NUM:
			switch ( wyChannelUnit )
			{
				case WY_UNIT_MM:						
					break;
				case WY_UNIT_CM:
					devideTen(pVal);
					break;
				case WY_UNIT_DM:
					devideTen(pVal);
					devideTen(pVal);
					break;
				case WY_UNIT_M:
					devideTen(pVal);
					devideTen(pVal);
					devideTen(pVal);
					break; 
				default:
					break;
			}
			break;
		
		case SMPL_BX_NUM:
			switch ( bxChannelUnit )
			{
				case BX_UNIT_MM:
					break;
				case BX_UNIT_CM:
					devideTen(pVal);
					break;
				case BX_UNIT_DM:
					devideTen(pVal);
					devideTen(pVal);
					break;
				case BX_UNIT_M:
					devideTen(pVal);
					devideTen(pVal);
					devideTen(pVal);
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
 * Function Name  : CountPipeArea
 * Description    : 计算管段面积
 * Input          : pipeOuterDiameter：管段外径，pipeThickness：管段厚度
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float CountPipeArea(float pipeOuterDiameter, float pipeThickness)
{
	return ( PI * pipeThickness * (pipeOuterDiameter - pipeThickness) );
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
