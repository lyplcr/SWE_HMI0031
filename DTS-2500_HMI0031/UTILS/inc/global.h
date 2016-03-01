/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Includes ------------------------------------------------------------------*/
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
#include "list.h"

/* Exported define -----------------------------------------------------------*/
#define PIC_DIR_UP_DOWN			"image/DirUD.bin"			//上下方向键
#define PIC_DIR_LEFT_RIGHT		"image/DirLR.bin"			//左右方向键
#define PIC_ETHERNET_SHOW		"image/PIC_NET.bin"			//以太网联机标识
#define PIC_USB_SHOW			"image/PIC_USB.bin"			//USB连接标识

/* Exported types ------------------------------------------------------------*/
/* 可读写输出位 */
typedef enum
{
	BIT_PUMP = 0XF,		/* 启停油泵位 */
}SW_RW_OUTPUT_TypeDef;

/* 输入位 */
typedef enum
{
	BIT_UP_LIMIT 	= 0X00,
	BIT_DOWN_LIMIT 	= 0X01,
	BIT_OIL_LIMIT	= 0X02,
}ST_INPUT_TypeDef;

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
	TEST_YIELD,			//屈服
	TEST_DEFORM,		//变形
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
	float disPlacement;
	float deform;
	float fhSpeed;
	float wySpeed;
	float bxSpeed;
	float strength;
	float peak;
	LINK_STATUS_TypeDef linkStatus;
	TEST_STATUS_TypeDef testStatus;
	CALIBRATION_STATUS_TypeDef calibrationStatus;
	int32_t fhCode;
	int32_t wyCode;
	int32_t bxCode;
	BoolStatus usbConnect;
	BoolStatus ethernetConnect;
}INTERFACE_ELEMENT_TypeDef;

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
	STATUS_DRAW_LINE_START,
	STATUS_DRAW_LINE_LOAD,
	STATUS_DRAW_LINE_END,
}STATUS_COORDINATE_DRAW_LINE_TypeDef;

/* 坐标系 */
enum
{
	COORDINATE_USE_TIME = 0,
	COORDINATE_USE_DEFORM,
	COORDINATE_USE_FORCE,
};

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
	float xMaxValue;				
	float yMaxValue;
	uint8_t xType;				//X轴类型
	uint8_t yType;				//Y轴类型
	const char *pXUnit;			//X轴单位
	const char *pYUnit;			//Y轴单位 
}COORDINATE_TypeDef;

typedef struct
{
	uint8_t xType;					//X轴类型
	uint8_t yType;					//Y轴类型
	uint8_t baseIndex;				//基准索引值
	STATUS_COORDINATE_DRAW_LINE_TypeDef status;
	FlagStatus start;
	FunctionalState enableRedraw;	//使能重绘
	uint16_t originX;				//坐标原点
	uint16_t originY;
	uint16_t lenthX;
	uint16_t lenthY;			
	float xMaxValue;				
	float yMaxValue;
	float xIncrease;			//X轴重新加载一次增量
	float yIncrease;			//Y轴重新加载一次增量
	uint32_t nowTimePoint;		//当前画线的时间点
	uint16_t lineColor;
	float xScalingCoefficient;	//缩放系数（将当前值按照缩放系数修饰）
	float yScalingCoefficient;	//缩放系数（将当前值按照缩放系数修饰）
	uint16_t recordPointFreq;	//记录每个点频率
	void (*pDrawCoordinate)( uint8_t xType, uint8_t yType, void *xMaxValuePtr, void *yMaxValuePtr );	//画坐标系
	float force[DECORD_COORDINATE_FORCE_NUM];
	float deform[DECORD_COORDINATE_FORCE_NUM];
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
	SMPL_NAME_TypeDef channel;
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

typedef enum
{
	FH_UNIT_kN = 0,
	FH_UNIT_N,
}FH_UINT_TypeDef;

typedef enum
{
	WY_UNIT_MM = 0,
	WY_UNIT_CM,
	WY_UNIT_DM,
	WY_UNIT_M,
}WY_UINT_TypeDef;

typedef enum
{
	BX_UNIT_MM = 0,
	BX_UNIT_CM,
	BX_UNIT_DM,
	BX_UNIT_M,
}BX_UINT_TypeDef;

typedef enum
{
	TYKY_SHAPE_RECTANGLE = 0,
	TYKY_SHAPE_ROUND,
	TYKY_SHAPE_IRREGULAR,
}TYKY_TEST_SHAPE_TypeDef;

typedef enum
{
	JSSWKL_SHAPE_RECTANGLE = 0,
	JSSWKL_SHAPE_ROUND,
	JSSWKL_SHAPE_TUBE,
	JSSWKL_SHAPE_IRREGULAR,
}JSSWKL_TEST_SHAPE_TypeDef;

typedef enum
{
	SHOW_DISPLACEMENT = 0,
	SHOW_DEFORM,
}DISPLACEMENT_CONV_DEFORM_TypeDef;

typedef enum
{
	COMPRESSION_TEST = 0,	/* 压缩试验 */
	BENDING_TEST,			/* 弯曲试验 */
	STRETCH_TEST,			/* 拉伸试验 */
	INVALID_TEST,			/* 无效试验 */
}TEST_ATTRIBUTE_TypeDef;

typedef struct
{
	uint8_t index;				
	FlagStatus completeSignal;
}SAMPLE_PROCESS_TypeDef;

typedef void (*pFunctionDevide)(void *);

/* Includes ------------------------------------------------------------------*/
#include "printer.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define IS_FLOAT_EQUAL(x1,x2)		( (fabs(x1-x2) < 0.0001f) ? 1 : 0 )

/* Exported variables --------------------------------------------------------*/
extern HMI_TypeDef *pHmi;
extern TEST_TypeDef *pTest;
extern const char * const pTwoLevelMenu[];
extern const char * const pUnitType[];
extern const char * const pSelectMenuCue[];
extern const char * const LssuedParameterWarning[];
extern const char * const pSelectMenuConfirmCue[];

/* Exported functions ------------------------------------------------------- */
FH_UINT_TypeDef GetFH_SmplUnit( void );
WY_UINT_TypeDef GetWY_SmplUnit( void );
BX_UINT_TypeDef GetBX_SmplUnit( void );
void GetTestContent( void );
void ProcessMachineMatchTestType( void );
TestStatus CheckCurrentModel( void );
TestStatus TestUserPassword( const char *pPassword );
TestStatus TestManagerPassword( const char *pPassword );
CMD_STATUS_TypeDef PCM_CmdSendCycle( void );
void RefreshDynamicForce( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float force );
void RefreshDynamicDisplacement( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float displacement );
void RefreshDynamicDeform( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float deform );
void RefreshDynamicSpeed( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float speed );
void RefreshDynamicStrength( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float strength );
void RefreshDynamicPeak( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, float peak );
void InitInterfaceElement( void );
void SetInterfaceElementForce( float force );
void SetInterfaceElementDisPlacement( float disPlacement );
void SetInterfaceElementDeform( float deform );
void SetInterfaceElementFHSpeed( float speed );
void SetInterfaceElementWYSpeed( float speed );
void SetInterfaceElementBXSpeed( float speed );
void SetInterfaceElementStrength( float strength );
void SetInterfaceElementPeak( float peak );
void SetInterfaceLinkStatus( LINK_STATUS_TypeDef linkStatus );
void SetInterfaceCalibrationStatus( CALIBRATION_STATUS_TypeDef calibrationStatus );
void SetInterfaceTestStatus( TEST_STATUS_TypeDef testStatus );
void SetInterfaceElementFHCode( int32_t code );
void SetInterfaceElementWYCode( int32_t code );
void SetInterfaceElementBXCode( int32_t code );
void SetInterfaceElementUSBStatus( BoolStatus usbConnect );
void SetInterfaceElementEthernetConnectStatus( BoolStatus ethernetConnect );
float GetInterfaceElementForce( void );
float GetInterfaceElementDisPlacement( void );
float GetInterfaceElementDeform( void );
float GetInterfaceElementFHSpeed( void );
float GetInterfaceElementWYSpeed( void );
float GetInterfaceElementBXSpeed( void );
float GetInterfaceElementStrength( void );
float GetInterfaceElementPeak( void );
LINK_STATUS_TypeDef GetInterfaceLinkStatus( void );
TEST_STATUS_TypeDef GetInterfaceTestStatus( void );
CALIBRATION_STATUS_TypeDef GetInterfaceCalibrationStatus( void );
int32_t GetInterfaceElementCode( SMPL_NAME_TypeDef tureChannel );
BoolStatus GetInterfaceElementUSBStatus( void );
BoolStatus GetInterfaceElementEthernetConnectStatus( void );
void RefreshDynamicCommunicationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, LINK_STATUS_TypeDef linkStatus );
void RefreshDynamicTestStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, TEST_STATUS_TypeDef testStatus );
void RefreshDynamicCalibrationStatus( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, CALIBRATION_STATUS_TypeDef calibrationStatus );
void RefreshDynamicCode( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, int32_t code );
void RefreshDynamicDoubleCode( uint16_t x, uint16_t y, uint16_t pointColor, uint16_t backColor, int32_t code1, int32_t code2 );
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

void CoordinateDrawLineBodyCycle( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
COORDINATE_DRAW_LINE_TypeDef *GetDrawLineAddr( void );
uint32_t GetDrawLineNowTimePoint( void );
float GetDrawLineSomeTimePointForce( uint32_t nowTimePoint );
BoolStatus IsCoordinateRecordPointOverflow( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
void ReloadCoordinate( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );
void CoordinateRedrawLine( COORDINATE_DRAW_LINE_TypeDef *pDrawLine );

void GUI_DrawTestSaveProgressBar( uint16_t backColor );
ErrorStatus SendChannelTareCmd( SMPL_NAME_TypeDef channel );
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
void SetDynamicContentCode( SMPL_NAME_TypeDef tureChannel );
void SetDynamicContentUSBStatus( void );
void SetDynamicContentNETStatus( void );
void SetShortCutMenuCue( uint16_t pointColor, uint16_t backColor, const char *pCue);
void InitLssuedParameter( void );
void LssuedParameterProcess( void );
void SetLssuedParameter( void );
float GetCurveShowStartValue( SMPL_NAME_TypeDef channel );
void DrawCircleMark( DRAW_CIRCLE_MARK_TypeDef *pDrawCircle );

void InitJudgeBreakPoint( void );
void InitSampleProcess( uint8_t chn );
FlagStatus GetSampleCompleteFlag( uint8_t chn );
void JudgeBreakCalculateCycle( uint8_t chn );
float GetWithMaxForceDifference( SMPL_NAME_TypeDef channel );
float GetAdjoinTwoPointDifference( SMPL_NAME_TypeDef channel );
uint16_t GetBreakDownPoint( SMPL_NAME_TypeDef channel );
uint8_t GetAttenuationRate( SMPL_NAME_TypeDef channel );
float GetPeakValue( SMPL_NAME_TypeDef channel );

DISPLACEMENT_CONV_DEFORM_TypeDef GetDisplacementOrDeformShow( void );
void SetDisplacementOrDeformShow( DISPLACEMENT_CONV_DEFORM_TypeDef newFunc );

void SetDynamicContentForce( FH_UINT_TypeDef fhUnit );
void SetDynamicContentDispalcement( WY_UINT_TypeDef wyUnit );
void SetDynamicContentDeform( BX_UINT_TypeDef bxUnit );
void SetDynamicContentFHSpeed( FH_UINT_TypeDef fhUnit );
void SetDynamicContentWYSpeed( WY_UINT_TypeDef wyUnit );
void SetDynamicContentBXSpeed( BX_UINT_TypeDef bxUnit );

float GetExtensometerGauge( void );
float GetParallelLenth( void );
float GetOriginalGauge( void );

COORDINATE_POINT_TypeDef *GetCoordinatePointAddr( void );
void PrintfTypeDefSize( uint32_t len );

void UintDivisionTen( uint32_t *pSource );
void FloatDivisionTen( float *pSource );
void AccordUnitConvValue( SMPL_NAME_TypeDef channel, void *pVal, pFunctionDevide devideTen );
float CountPipeArea(float pipeOuterDiameter, float pipeThickness);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
