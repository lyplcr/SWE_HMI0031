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
#define PIC_DIR_UP_DOWN			"image/DirUD.bin"			//���·����
#define PIC_DIR_LEFT_RIGHT		"image/DirLR.bin"			//���ҷ����
#define PIC_ETHERNET_SHOW		"image/PIC_NET.bin"			//��̫��������ʶ
#define PIC_USB_SHOW			"image/PIC_USB.bin"			//USB���ӱ�ʶ

#define RECORD_COORDINATE_PERIOD		100					//100ms��¼һ����
#define RECORD_COORDINATE_FREQ			(1000/RECORD_COORDINATE_PERIOD)	
#define RECORD_COORDINATE_TIME_SECOND	(5 * 60)			//5����	
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
	uint16_t rowDistance;				//�м��
	uint16_t columnDistance;			//�м��
	uint16_t lineWidth;		
	uint8_t pointBit;					//С����λ��
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
	uint16_t maxUpY;					//�컨��߶�
	uint16_t maxDownY;					//�ذ�߶�
	uint8_t fontSize;
	uint16_t rowDistance;				//�м��
	uint16_t columnDistance;			//�м��
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
	TEST_IDLE = 0,		//����
	TEST_LOAD,			//����
	TEST_KEEP,			//����
	TEST_BREAK,			//����
	TEST_UNLOAD,		//ж��
	TEST_SAVE,			//�洢
}TEST_STATUS_TypeDef;

typedef enum
{
	STATUS_CALIBRATION_IDLE = 0,	//����״̬
	STATUS_CALIBRATION_PROCESS,		//����
	STATUS_CALIBRATION_BACKHAUL,	//�س�
	STATUS_CALIBRATION_END,			//У׼����
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
	int32_t code;
	BoolStatus usbConnect;
	BoolStatus ethernetConnect;
}INTERFACE_ELEMENT_TypeDef;

typedef struct
{
	uint16_t x;					//���Ͻ���ʼ����
	uint16_t y;					//���Ͻ���ʼ����
	uint16_t rowSpace;			//�м��
	uint16_t columnSpace;		//�м��
	uint16_t xLenth;			//X���߳�
	uint16_t yLenth;			//Y���߳�
	uint16_t fillFieldLenth;	//���γ���
	uint16_t emptyFieldLenth;	//�հ����򳤶�
	uint16_t lineWidth;			//�߿�
	uint8_t rowFieldNum;		//�зֶ���
	uint8_t columnFieldNum;		//�зֶ���
	uint16_t mainBackColor;		//���屳��ɫ
	uint16_t windowsBackColor;	//���屳��ɫ
	uint16_t rowLineColor;		//����������ɫ
	uint16_t columnLineColor;	//����������ɫ
	uint16_t fontPointColor;	//����ǰ��ɫ
	uint16_t fontBackColor;		//���屳��ɫ
	uint16_t xLinePointColor;	//X����ǰ��ɫ
	uint16_t yLinePointColor;	//Y����ǰ��ɫ
	float maxForce;				//�����
	uint32_t maxTime;			//���ʱ��
	const char *pXUnit;			//X�ᵥλ
	const char *pYUnit;			//Y�ᵥλ
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
	FunctionalState enableRedraw;	//ʹ���ػ�
	uint16_t originX;				//����ԭ��
	uint16_t originY;
	uint16_t lenthX;
	uint16_t lenthY;
	float maxForce;					//��λ��N
	uint32_t maxTime;				//��λ��ms
	uint32_t nowTimePoint;			//��ǰ���ߵ�ʱ���
	uint16_t lineColor;
	float forceScalingCoefficient;	//����ϵ��
	void (*pDrawCoordinate)( uint32_t maxForce, uint32_t maxTime );	//������ϵ
	float force[DECORD_COORDINATE_FORCE_NUM];
}COORDINATE_DRAW_LINE_TypeDef;

typedef enum
{
	SEND_TARE_CMD = 0,
	WAIT_TARE_END,
}TARE_CMD_STATUS_TypeDef;

typedef enum
{
	STATUS_CORE_WARNING = 0,				//����״̬	
	STATUS_CORE_OPEN_LOOP,					//����״̬
	STATUS_CORE_CLOSED_LOOP,				//�ջ�״̬
	STATUS_CORE_MANULE,						//�ֶ�״̬	
}CORE_STATUS_TypeDef;

typedef struct
{
	CORE_STATUS_TypeDef coreStatus;
	uint8_t errCode;
	const char * const *pErrCue;	//������ʾ��
	uint8_t rowNum;					//����
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
	uint16_t distance;		/* 2��Բ��� */
	uint32_t curSerial;
	uint32_t sumSerial;
	uint16_t pointColor;
	uint16_t backColor;
	uint16_t radius;		/* �뾶 */
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
FH_UINT_TypeDef GetFH_SmplUnit( void );
WY_UINT_TypeDef GetWY_SmplUnit( void );
BX_UINT_TypeDef GetBX_SmplUnit( void );
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
void SetInterfaceElementCode( int32_t code );
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