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
	BIT_PUMP = 0XF,		/* 启停油泵位 */
}SW_RW_OUTPUT_TypeDef;

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
	"修正标定表",		//85
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
#pragma arm section


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*------------------------------------------------------------
 * Function Name  : GetCurChannel
 * Description    : 获取当前通道
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SMPL_NAME_TypeDef GetCurChannel( void )
{
	SMPL_NAME_TypeDef ch = SMPL_FH_NUM;
	
	switch ( pHmi->test_standard_index )
	{
		case KYSNJS:
		case KYJZSJ:
		case KYHNT:
		case KZHNT:
		case KYQQZ:
		case KYTY:
 		case KYZJDH:
			ch = SMPL_FH_NUM;
			break;
		
		case KZSNJS:
		case KZYJSNJ:
		case KZTY:
			ch = SMPL_FH_NUM;	
			break;
		
		case KLJSSW:
			ch = SMPL_WY_NUM;
			break;
		default:
			break;
	}	

	return ch;
}

/*------------------------------------------------------------
 * Function Name  : GetCurTestChannel
 * Description    : 获取试验通道
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SMPL_NAME_TypeDef GetCurTestChannel( uint8_t test_type )
{
	SMPL_NAME_TypeDef ch = SMPL_FH_NUM;
	
	switch ( test_type )
	{
		case KYSNJS:
		case KYJZSJ:
		case KYHNT:
		case KZHNT:
		case KYQQZ:
		case KYTY:
 		case KYZJDH:
			ch = SMPL_FH_NUM;
			break;
		
		case KZSNJS:
		case KZYJSNJ:
		case KZTY:
			ch = SMPL_FH_NUM;	
			break;
		
		case KLJSSW:
			ch = SMPL_WY_NUM;
			break;
		
		default:
			break;
	}	

	return ch;
}

/*------------------------------------------------------------
 * Function Name  : GetSmplFHUnit
 * Description    : 获取负荷通道单位
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FORCE_UINT_TypeDef GetFH_SmplUnit( void )
{
	if ( pHmi->unit )
	{
		return UNIT_N;	  //以N为单位
	}
	else
	{
		return UNIT_kN;	  //以kN为单位
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
		pHmi->test_standard_index = 0;
	}	
	pTest = pcm_test_get(pHmi->test_standard_index);
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
			if (testType > KYTY)
			{
				pHmi->test_standard_index = KYSNJS;
				pcm_save();
			}
			break;
		case MODEL_KZ:
			if (testType > KZTY)
			{
				pHmi->test_standard_index = KZSNJS;
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
	
	force = SetValueNotEqualZero(force);	
	
	if (absForce < 1000)
	{
		disp_value(force,DISP_CHN_FH,x,y,pointColor,backColor,3,2);
	}
	else if (absForce < 100000)
	{
		disp_value(force,DISP_CHN_FH,x,y,pointColor,backColor,4,1);
	}
	else
	{
		disp_syn(DISP_CHN_FH);
		lcd_num48(x,y,pointColor,backColor,"------");
	}
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
	
	speed = SetValueNotEqualZero(speed);
	
	if ( absSpeed < 100 )
	{
		disp_value(speed,DISP_CHN_SPEED,x,y,pointColor,backColor,2,2);
	}
	else if ( absSpeed < 10000 )
	{
		disp_value(speed,DISP_CHN_SPEED,x,y,pointColor,backColor,3,1);
	}
	else
	{
		disp_syn(DISP_CHN_SPEED);
		lcd_num48(x,y,pointColor,backColor,"-----");
	}
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
	
	peak = SetValueNotEqualZero(peak);
	
	if (absPeak < 1000)
	{
		disp_value(peak,DISP_CHN_PEAK,x,y,pointColor,backColor,3,2);
	}
	else if (absPeak < 100000)
	{
		disp_value(peak,DISP_CHN_PEAK,x,y,pointColor,backColor,4,1);
	}
	else
	{
		disp_syn(DISP_CHN_PEAK);
		lcd_num48(x,y,pointColor,backColor,"------");
	}
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
 * Function Name  : RefreshDynamicTestStatus
 * Description    : 刷新试验状态
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void RefreshDynamicTestStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, TEST_STATUS_TypeDef testStatus )
{
	static uint8_t s_loadCount = 0;
	const uint8_t MAX_SHOW_LOAD_COUNT = 5;
	static BOUND_TEXT_STATUS_TypeDef s_boundTextStatus = STATUS_BOUND_SHOW;
	
	switch ( testStatus )
	{
		case TEST_IDLE:
			GUI_DispStr16At(x,y,pointColor,backColor,"空闲");
			break;
		case TEST_LOAD:
			switch ( s_boundTextStatus )
			{
				case STATUS_BOUND_SHOW:
					GUI_DispStr16At(x,y,pointColor,backColor,"加载");
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
		case TEST_KEEP:
			GUI_DispStr16At(x,y,pointColor,backColor,"保持");
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
	g_interfaceElement.speed = 0;
	g_interfaceElement.strength = 0;
	g_interfaceElement.peak = 0;
	g_interfaceElement.linkStatus = LINK_IDLE;
	g_interfaceElement.testStatus = TEST_IDLE;
	g_interfaceElement.code = 0;
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
 * Function Name  : SetInterfaceElementSpeed
 * Description    : 设置界面速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementSpeed( float speed )
{
	g_interfaceElement.speed = speed;
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
 * Function Name  : SetInterfaceElementCode
 * Description    : 设置界面码值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetInterfaceElementCode( int32_t code )
{
	g_interfaceElement.code = code;
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
 * Function Name  : GetInterfaceElementSpeed
 * Description    : 获取界面速度
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetInterfaceElementSpeed( void )
{
	return g_interfaceElement.speed;
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
int32_t GetInterfaceElementCode( void )
{
	return g_interfaceElement.code;
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
	uint8_t i;
	uint32_t averageTime = 0;
	uint32_t curTime = 0;
	float averageForce = 0;
	uint32_t curForce = 0;
	char showBuff[10];
	uint8_t bit = 0;
	
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
	lcd_fill(pCoordinate->x,pCoordinate->y+pCoordinate->yLenth,pCoordinate->xLenth,\
			pCoordinate->lineWidth,pCoordinate->xLinePointColor);
	lcd_fill(pCoordinate->x,pCoordinate->y,pCoordinate->lineWidth,pCoordinate->yLenth,\
			pCoordinate->yLinePointColor);
	lcd_fill(pCoordinate->x,pCoordinate->y,pCoordinate->xLenth,\
			pCoordinate->lineWidth,pCoordinate->xLinePointColor);
	lcd_fill(pCoordinate->x+pCoordinate->xLenth,pCoordinate->y,pCoordinate->lineWidth,pCoordinate->yLenth,\
			pCoordinate->yLinePointColor);
			
	/* 限制条件 */
	if (pCoordinate->maxTime < 20)
 	{
 		pCoordinate->maxTime = 20;	
 	}

	if (pCoordinate->maxForce < 100)
	{
		pCoordinate->maxForce = 100;
	}
	
	/* 显示 X轴 时间 */
	averageTime = pCoordinate->maxTime / pCoordinate->columnFieldNum;
	
	for (i=0; i<=pCoordinate->columnFieldNum; ++i)
	{
		curTime = averageTime * i;
		usprintf(showBuff,"%d",curTime);
		
		if (i == 0)			//显示原点
		{
			bit = 0;	
		}	
		else 
		{
			bit = GetIntBit(curTime);
		}
		
		GUI_DispStr16At(pCoordinate->x+i*pCoordinate->columnSpace-bit*4,\
							pCoordinate->y+pCoordinate->yLenth+8,pCoordinate->fontPointColor,\
							pCoordinate->fontBackColor,showBuff);
	}
	
	/* 显示 Y轴 力值 */
	averageForce = pCoordinate->maxForce / pCoordinate->rowFieldNum;
	
	for (i=1; i<=pCoordinate->rowFieldNum; ++i)
	{
		curForce = averageForce * i;
		usprintf(showBuff,"%d",curForce);
		
		bit = GetIntBit(curForce);
		
		GUI_DispStr16At(pCoordinate->x-bit*8-8,pCoordinate->y+pCoordinate->yLenth-i*pCoordinate->rowSpace-8,\
							pCoordinate->fontPointColor,pCoordinate->fontBackColor,showBuff);
	}
	
	/* 显示单位 */
	GUI_DispStr16At(pCoordinate->x+pCoordinate->xLenth-24,\
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
			break;
			 
		case KYJZSJ:
			area = GetSampleSpecificationArea(report->sample_spec,temp);
			if (fabs(area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				area = 1;
			}
			MPa = report->correct_cof * CurForce / area;
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
			break;
		
		case KYZJDH:
			break;

		case KYTY:
			if (fabs(report->area) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				report->area = 1;
			}
			MPa = report->correct_cof * CurForce / report->area;
			break;

		case KZSNJS:
			width = report->length;
			span = report->sample_span;
			if (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				width = 1;
			}
			MPa = 1.5f * CurForce * span / (width * width * width);
			break;

		case KZYJSNJ:
			width = report->length;
			span = report->sample_span;
			if (fabs(width) < MIN_FLOAT_PRECISION_DIFF_VALUE)
			{
				width = 1;
			}
			MPa = 1.5f * CurForce * span / (width * width * width);
			break;
 
		case KZTY:
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
static void CheckSystemMaxForceWarning( SMPL_NAME_TypeDef2 tureChannel )
{
	float force = get_smpl_force(tureChannel);
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
			CheckSystemMaxForceWarning(SMPL_KY_NUM);
			break;
		case MODEL_KZ:
			CheckSystemMaxForceWarning(SMPL_KZ_NUM);
			break;
		case MODEL_KZKY:
			CheckSystemMaxForceWarning(SMPL_KY_NUM);
			CheckSystemMaxForceWarning(SMPL_KZ_NUM);
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
 * Function Name  : InitCoordinateDrawLine
 * Description    : 初始化坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void InitCoordinateDrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	g_coordinateDrawLine.status = pDrawLine->status;
	g_coordinateDrawLine.start = pDrawLine->start;
	g_coordinateDrawLine.enableRedraw = pDrawLine->enableRedraw;
	g_coordinateDrawLine.originX = pDrawLine->originX;
	g_coordinateDrawLine.originY = pDrawLine->originY;
	g_coordinateDrawLine.lenthX = pDrawLine->lenthX;
	g_coordinateDrawLine.lenthY = pDrawLine->lenthY;
	g_coordinateDrawLine.maxForce = pDrawLine->maxForce;
	g_coordinateDrawLine.maxTime = pDrawLine->maxTime;
	g_coordinateDrawLine.nowTimePoint = pDrawLine->nowTimePoint;
	g_coordinateDrawLine.pDrawCoordinate = pDrawLine->pDrawCoordinate;
	g_coordinateDrawLine.lineColor = pDrawLine->lineColor;
	g_coordinateDrawLine.forceScalingCoefficient = pDrawLine->forceScalingCoefficient;
	memset(g_coordinateDrawLine.force,0x00,sizeof(float)*DECORD_COORDINATE_FORCE_NUM);
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
	uint32_t lastTime = 0,nowTime = 0;
	float lastForce = 0,nowForce = 0;
	uint16_t lastTimeCoordinate = 0,nowTimeCoordinate = 0;
	uint16_t lastForceCoordinate = 0,nowForceCoordinate = 0;
	
	if (pDrawLine->nowTimePoint == 0)
	{
		return;
	}
	
	lastTime = (pDrawLine->nowTimePoint - 1) * RECORD_COORDINATE_PERIOD;
	nowTime = pDrawLine->nowTimePoint * RECORD_COORDINATE_PERIOD;
	
	lastTimeCoordinate = pDrawLine->originX + (float)lastTime / pDrawLine->maxTime * pDrawLine->lenthX;
	nowTimeCoordinate = pDrawLine->originX + (float)nowTime / pDrawLine->maxTime * pDrawLine->lenthX;
	
	lastForce = pDrawLine->force[pDrawLine->nowTimePoint - 1];
	nowForce = pDrawLine->force[pDrawLine->nowTimePoint];
	
	lastForceCoordinate = pDrawLine->originY - lastForce / pDrawLine->maxForce * pDrawLine->lenthY;
	nowForceCoordinate = pDrawLine->originY - nowForce / pDrawLine->maxForce * pDrawLine->lenthY;
	
	lcd_draw_line(lastTimeCoordinate,lastForceCoordinate,nowTimeCoordinate,nowForceCoordinate,pDrawLine->lineColor);
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
	pDrawLine->pDrawCoordinate(pDrawLine->maxForce * pDrawLine->forceScalingCoefficient,pDrawLine->maxTime / 1000);
}	

/*------------------------------------------------------------
 * Function Name  : CoordinateRedrawLine
 * Description    : 重绘坐标系画线
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CoordinateRedrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine )
{
	uint32_t lastTime = 0,nowTime = 0;
	float lastForce = 0,nowForce = 0;
	uint16_t lastTimeCoordinate = 0,nowTimeCoordinate = 0;
	uint16_t lastForceCoordinate = 0,nowForceCoordinate = 0;
	uint32_t timeIndex = 0;
	
	ReloadCoordinate(pDrawLine);
	
	for (timeIndex=0; timeIndex<=pDrawLine->nowTimePoint; ++timeIndex)
	{	
		lastTime = nowTime;
		nowTime += RECORD_COORDINATE_PERIOD;
		
		lastTimeCoordinate = pDrawLine->originX + (float)lastTime / pDrawLine->maxTime * pDrawLine->lenthX;
		nowTimeCoordinate = pDrawLine->originX + (float)nowTime / pDrawLine->maxTime * pDrawLine->lenthX;
		
		lastForce = nowForce;
		nowForce = pDrawLine->force[timeIndex];
		
		lastForceCoordinate = pDrawLine->originY - lastForce / pDrawLine->maxForce * pDrawLine->lenthY;
		nowForceCoordinate = pDrawLine->originY - nowForce / pDrawLine->maxForce * pDrawLine->lenthY;
		
		lcd_draw_line(lastTimeCoordinate,lastForceCoordinate,nowTimeCoordinate,nowForceCoordinate,pDrawLine->lineColor);
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
		
		CoordinateRedrawLine(pDrawLine);
	}
}

/*------------------------------------------------------------
 * Function Name  : SendKYChannelTareCmd
 * Description    : 抗压通道清零命令
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus SendChannelTareCmd( SMPL_NAME_TypeDef2 channel )
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
		}
	}
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
void SetDynamicContentCode( SMPL_NAME_TypeDef2 tureChannel )
{
	SetInterfaceElementCode(GetSammpleCode(tureChannel));
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
float GetCurveShowStartValue( SMPL_NAME_TypeDef2 channel )
{
	return pHmi->start_force[channel];
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
 * Function Name  : JudgeBreakCalculateCycle
 * Description    : 计算判破循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void JudgeBreakCalculateCycle( uint8_t chn )
{
	const uint8_t ADJOIN_DOWN_POINT_NUM = 5;	//每20ms一个点
	const uint16_t MAX_DOWN_POINT_NUM = 100;
	
	g_judgeBreak.index[chn]++;
	
	/* 100ms采样一个点 */
	if (g_judgeBreak.index[chn] >= ADJOIN_DOWN_POINT_NUM)	
	{
		g_judgeBreak.index[chn] = 0;
		
		g_judgeBreak.lastForcePoint[chn] = g_judgeBreak.nowForcePoint[chn];
		g_judgeBreak.nowForcePoint[chn] = get_smpl_force(chn);
		
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
		
		#ifdef DEBUG_SAMPLE_KY
			if (chn == SMPL_KY_NUM)
			{
				printf("下降点：    %d\r\n",g_judgeBreak.downPoint[SMPL_KY_NUM]);
				printf("最大力差值：%f\r\n",g_judgeBreak.maxForceDiff[SMPL_KY_NUM]);
				printf("相邻点差值：%f\r\n",g_judgeBreak.adjoinPointDiff[SMPL_KY_NUM]);
			}
		#endif	
			
		#ifdef DEBUG_SAMPLE_KZ	
			if (chn == SMPL_KZ_NUM)
			{
				printf("下降点：    %d\r\n",g_judgeBreak.downPoint[SMPL_KZ_NUM]);
				printf("最大力差值：%f\r\n",g_judgeBreak.maxForceDiff[SMPL_KZ_NUM]);
				printf("相邻点差值：%f\r\n",g_judgeBreak.adjoinPointDiff[SMPL_KZ_NUM]);
			}	
		#endif
	}
}

/*------------------------------------------------------------
 * Function Name  : GetWithMaxForceDifference
 * Description    : 获取最大力差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetWithMaxForceDifference( SMPL_NAME_TypeDef2 channel )
{
	return g_judgeBreak.maxForceDiff[channel];
}

/*------------------------------------------------------------
 * Function Name  : GetAdjoinTwoPointDifference
 * Description    : 获取相邻两点差值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetAdjoinTwoPointDifference( SMPL_NAME_TypeDef2 channel )
{
	return g_judgeBreak.adjoinPointDiff[channel];
}

/*------------------------------------------------------------
 * Function Name  : GetBreakDownPoint
 * Description    : 获取破碎下降点数
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint16_t GetBreakDownPoint( SMPL_NAME_TypeDef2 channel )
{
	return g_judgeBreak.downPoint[channel];
}

/*------------------------------------------------------------
 * Function Name  : GetAttenuationRate
 * Description    : 获取衰减率
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetAttenuationRate( SMPL_NAME_TypeDef2 channel )
{
	return g_judgeBreak.attenuationRate[channel];
}

/*------------------------------------------------------------
 * Function Name  : GetPeakValue
 * Description    : 获取峰值
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float GetPeakValue( SMPL_NAME_TypeDef2 channel )
{
	return g_judgeBreak.peakForce[channel];
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
