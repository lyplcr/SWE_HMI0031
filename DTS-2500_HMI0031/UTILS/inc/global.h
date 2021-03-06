/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Includes ------------------------------------------------------------------*/
#include "MY_DEBUG.H"
#include "bsp.h"
#include "cmd.h"
#include "typewrite.h"
#include "gui.h"
#include "tools.h"
#include "pcm.h"
#include "prm.h"
#include "report.h"
#include "command.h"
#include "protocol.h"
#include "prm_struct.h"
#include "PageManage.h"
#include "usb.h"
#include "netconf.h"
#include "active.h"

/* Exported define -----------------------------------------------------------*/
#define PIC_DIR_UP_DOWN			"image/DirUD.bin"			//上下方向键
#define PIC_DIR_LEFT_RIGHT		"image/DirLR.bin"			//左右方向键
#define PIC_ETHERNET_SHOW		"image/PIC_NET.bin"			//以太网联机标识
#define PIC_USB_SHOW			"image/PIC_USB.bin"			//USB连接标识

#define RECORD_COORDINATE_PERIOD		100					//100ms记录一个点
#define RECORD_COORDINATE_FREQ			(1000/RECORD_COORDINATE_PERIOD)	
#define RECORD_COORDINATE_TIME_SECOND	(5 * 60)			//5分钟	
#define DECORD_COORDINATE_FORCE_NUM		(RECORD_COORDINATE_TIME_SECOND * RECORD_COORDINATE_FREQ) 

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t recordPointColor;
	uint16_t recordBackColor;
	uint16_t lenth;
	uint16_t width;
	uint8_t fontSize;
	uint16_t rowDistance;				//行间距
	uint16_t columnDistance;			//列间距
	uint16_t lineWidth;		
	uint8_t pointBit;					//小数点位数
	TYPE_SAVE_TypeDef saveType;
}ONE_LEVEL_MENU_TYPE_TypeDef;

typedef struct
{
	uint8_t index;
	uint16_t x;
	uint16_t y;
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t lenth;
	uint16_t width;
	uint16_t maxUpY;					//天花板高度
	uint16_t maxDownY;					//地板高度
	uint8_t fontSize;
	uint16_t rowDistance;				//行间距
	uint16_t columnDistance;			//列间距
	uint16_t lineWidth;				
	uint8_t parameterCnt;	
	PARAMETER_TYPE_TypeDef parameterType;
	const char * const *pParameterNameArray;
}TWO_LEVEL_MENU_TYPE_TypeDef;

typedef struct
{
	uint8_t rowIndex;
	uint8_t colIndex;
}TABLE_INDEX_TypeDef;

typedef enum
{
	TEST_IDLE = 0,		//空闲
	TEST_LOAD,			//加载
	TEST_KEEP,			//保持
	TEST_BREAK,			//判破
	TEST_UNLOAD,		//卸载
	TEST_SAVE,			//存储
}TEST_STATUS_TypeDef;

typedef enum
{
	STATUS_CALIBRATION_IDLE = 0,	//空闲状态
	STATUS_CALIBRATION_PROCESS,		//进程
	STATUS_CALIBRATION_BACKHAUL,	//回程
	STATUS_CALIBRATION_END,			//校准结束
}CALIBRATION_STATUS_TypeDef;

typedef struct
{
	float force;
	float speed;
	float strength;
	float peak;
	LINK_STATUS_TypeDef linkStatus;
	TEST_STATUS_TypeDef testStatus;
	CALIBRATION_STATUS_TypeDef calibrationStatus;
	int32_t code;
	BoolStatus usbConnect;
	BoolStatus ethernetConnect;
}INTERFACE_ELEMENT_TypeDef;

typedef struct
{
	uint16_t x;					//左上角起始坐标
	uint16_t y;					//左上角起始坐标
	uint16_t rowSpace;			//行间距
	uint16_t columnSpace;		//列间距
	uint16_t xLenth;			//X轴线长
	uint16_t yLenth;			//Y轴线长
	uint16_t fillFieldLenth;	//填充段长度
	uint16_t emptyFieldLenth;	//空白区域长度
	uint16_t lineWidth;			//线宽
	uint8_t rowFieldNum;		//行分段数
	uint8_t columnFieldNum;		//列分段数
	uint16_t mainBackColor;		//主体背景色
	uint16_t windowsBackColor;	//窗体背景色
	uint16_t rowLineColor;		//框线行线颜色
	uint16_t columnLineColor;	//框线列线颜色
	uint16_t fontPointColor;	//字体前景色
	uint16_t fontBackColor;		//字体背景色
	uint16_t xLinePointColor;	//X轴线前景色
	uint16_t yLinePointColor;	//Y轴线前景色
	float maxForce;				//最大力
	uint32_t maxTime;			//最大时间
	const char *pXUnit;			//X轴单位
	const char *pYUnit;			//Y轴单位
}COORDINATE_TypeDef;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t lenth;
	uint16_t width;
	uint16_t upperPointColor;
	uint16_t lowerPointColor;
	uint16_t lineColor;
	uint16_t backColor;
	uint16_t fontColor;
	uint16_t lineWidth;
	uint16_t curProgress;
	uint16_t totalProgress;
	const char *pTitle;
	uint8_t fontSize;
}PROGRESS_BAR_TypeDef;

typedef enum
{
	STATUS_DRAW_LINE_IDLE = 0,
	STATUS_DRAW_LINE_LOAD,
}STATUS_COORDINATE_DRAW_LINE_TypeDef;

typedef struct
{
	STATUS_COORDINATE_DRAW_LINE_TypeDef status;
	FlagStatus start;
	FunctionalState enableRedraw;	//使能重绘
	uint16_t originX;				//坐标原点
	uint16_t originY;
	uint16_t lenthX;
	uint16_t lenthY;
	float maxForce;					//单位：N
	uint32_t maxTime;				//单位：ms
	uint32_t nowTimePoint;			//当前画线的时间点
	uint16_t lineColor;
	float forceScalingCoefficient;	//缩放系数
	void (*pDrawCoordinate)( uint32_t maxForce, uint32_t maxTime );	//画坐标系
	float force[DECORD_COORDINATE_FORCE_NUM];
}COORDINATE_DRAW_LINE_TypeDef;

typedef enum
{
	SEND_TARE_CMD = 0,
	WAIT_TARE_END,
}TARE_CMD_STATUS_TypeDef;

typedef enum
{
	STATUS_CORE_WARNING = 0,				//报警状态	
	STATUS_CORE_OPEN_LOOP,					//开环状态
	STATUS_CORE_CLOSED_LOOP,				//闭环状态
	STATUS_CORE_MANULE,						//手动状态	
}CORE_STATUS_TypeDef;

typedef struct
{
	CORE_STATUS_TypeDef coreStatus;
	uint8_t errCode;
	const char * const *pErrCue;	//错误提示语
	uint8_t rowNum;					//行数
}SYSTEM_PROTECT_TypeDef;

typedef struct
{
	SMPL_NAME_TypeDef2 tureChannel;
	SMPL_NAME_TypeDef2 showChannel;
	uint8_t calibrationPointNums;
	const int32_t *pCheckForce;
	const float *pRealForce;
	const int32_t *pCode;
	const float *pDeviation;
}CALIBRATION_RSULT_PRINT_TypeDef;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t distance;		/* 2个圆间距 */
	uint32_t curSerial;
	uint32_t sumSerial;
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t radius;		/* 半径 */
}DRAW_CIRCLE_MARK_TypeDef;

/* Includes ------------------------------------------------------------------*/
#include "printer.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern HMI_TypeDef *pHmi;
extern TEST_TypeDef *pTest;
extern const char * const pTwoLevelMenu[];
extern const char * const pUnitType[];
extern const char * const pSelectMenuCue[];
extern const char * const LssuedParameterWarning[];
extern const char * const pSelectMenuConfirmCue[];

/* Exported functions ------------------------------------------------------- */
SMPL_NAME_TypeDef GetCurChannel( void );
FORCE_UINT_TypeDef GetFH_SmplUnit( void );
SMPL_NAME_TypeDef GetCurTestChannel( uint8_t test_type );
void GetTestContent( void );
void ProcessMachineMatchTestType( void );
TestStatus TestUserPassword( const char *pPassword );
TestStatus TestManagerPassword( const char *pPassword );
CMD_STATUS_TypeDef PCM_CmdSendCycle( void );
void RefreshDynamicForce( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float force );
void RefreshDynamicSpeed( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float speed );
void RefreshDynamicStrength( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float strength );
void RefreshDynamicPeak( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float peak );
void InitInterfaceElement( void );
void SetInterfaceElementForce( float force );
void SetInterfaceElementSpeed( float speed );
void SetInterfaceElementStrength( float strength );
void SetInterfaceElementPeak( float peak );
void SetInterfaceLinkStatus( LINK_STATUS_TypeDef linkStatus );
void SetInterfaceCalibrationStatus( CALIBRATION_STATUS_TypeDef calibrationStatus );
void SetInterfaceTestStatus( TEST_STATUS_TypeDef testStatus );
void SetInterfaceElementCode( int32_t code );
void SetInterfaceElementUSBStatus( BoolStatus usbConnect );
void SetInterfaceElementEthernetConnectStatus( BoolStatus ethernetConnect );
float GetInterfaceElementForce( void );
float GetInterfaceElementSpeed( void );
float GetInterfaceElementStrength( void );
float GetInterfaceElementPeak( void );
LINK_STATUS_TypeDef GetInterfaceLinkStatus( void );
TEST_STATUS_TypeDef GetInterfaceTestStatus( void );
CALIBRATION_STATUS_TypeDef GetInterfaceCalibrationStatus( void );
int32_t GetInterfaceElementCode( void );
BoolStatus GetInterfaceElementUSBStatus( void );
BoolStatus GetInterfaceElementEthernetConnectStatus( void );
void RefreshDynamicCommunicationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, LINK_STATUS_TypeDef linkStatus );
void RefreshDynamicTestStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, TEST_STATUS_TypeDef testStatus );
void RefreshDynamicCalibrationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, CALIBRATION_STATUS_TypeDef calibrationStatus );
void RefreshDynamicCode( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, int32_t code );
void RefreshDynamicSystemTime( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor );
void RefreshDynamicEthernetIco( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, BoolStatus ethernetConnect );
void RefreshDynamicUSBIco( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, BoolStatus usbConnect );
void GUI_DrawCoordinate( COORDINATE_TypeDef *pCoordinate );
COORDINATE_TypeDef * GetCoordinateDataAddr( void );
void GUI_ProgressBar( PROGRESS_BAR_TypeDef *pProgress );
void DrawProcessBody( PROGRESS_BAR_TypeDef *pProgress );
void RelinkPRM( uint16_t backColor );
float FromForceGetStrength( TEST_TYPE_TypeDef type, REPORT_TypeDef *report, float CurForce );
void CheckOfflineCycle( void );
TestStatus CheckSystemWarning( SMPL_NAME_TypeDef2 tureChannel );
void InitCoordinateDrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
void CoordinateDrawLineBodyCycle( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
COORDINATE_DRAW_LINE_TypeDef *GetDrawLineAddr( void );
BoolStatus IsCoordinateRecordPointOverflow( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
void ReloadCoordinate( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
void GUI_DrawTestSaveProgressBar( uint16_t backColor );
ErrorStatus SendChannelTareCmd( SMPL_NAME_TypeDef2 channel );
void ExecuteTask( void );
void SendOpenPumpCmd( void );
void SendClosePumpCmd( void );
void PumpStatusLampTask( void );
void InitCommucationLamp( void );
void CommucationLampTask( void );
void SetCommucationLampStart( void );
void SetCoreStatus( CORE_STATUS_TypeDef status );
CORE_STATUS_TypeDef GetCoreStatus( void );
void CheckSystemWarningStatusCycle( void );
SYSTEM_PROTECT_TypeDef *GetSystemProtectAddr( void );
void ShowPrintingShortcutMenu( void );
void ShowExportingShortcutMenu( void );
void SetDynamicContentLinkStatus( void );
void SetDynamicContentCode( SMPL_NAME_TypeDef2 tureChannel );
void SetDynamicContentUSBStatus( void );
void SetDynamicContentNETStatus( void );
void SetShortCutMenuCue( uint16_t pointColor, uint16_t backColor, const char *pCue);
void InitLssuedParameter( void );
void LssuedParameterProcess( void );
void SetLssuedParameter( void );
float GetCurveShowStartValue( SMPL_NAME_TypeDef2 channel );
void DrawCircleMark( DRAW_CIRCLE_MARK_TypeDef *pDrawCircle );

void InitJudgeBreakPoint( void );
void JudgeBreakCalculateCycle( uint8_t chn );
float GetWithMaxForceDifference( SMPL_NAME_TypeDef2 channel );
float GetAdjoinTwoPointDifference( SMPL_NAME_TypeDef2 channel );
uint16_t GetBreakDownPoint( SMPL_NAME_TypeDef2 channel );
uint8_t GetAttenuationRate( SMPL_NAME_TypeDef2 channel );
float GetPeakValue( SMPL_NAME_TypeDef2 channel );


#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
