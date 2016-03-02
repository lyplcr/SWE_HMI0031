/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEWRITE_H
#define __TYPEWRITE_H

/* Includes ------------------------------------------------------------------*/
#define MAX_ALLOW_PUTIN_CNT			20

/* ���뷨Ƥ��UI���� */
#define PUTIN_SKIN_START_X			715
#define PUTIN_SKIN_START_Y			410
#define	PUTIN_SKIN_POINT_COLOR		LITTLE_BLUE_SKY

#define COLOR_PUTIN_FONT_POINT			GREEN			//���������ַ�����ɫ
#define COLOR_PUTIN_FONT_BACK			DARKBLUE		//���������ַ�����ɫ

#define PUTIN_RECANGLE_CUE			"������..."

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	STATUS_KEY_NO_PRESS = 0,	//����δ����
	STATUS_KEY_WAIT,			//�м�ȴ�״̬
	STATUS_KEY_PRESS_CONFIRM,	//ȷ�ϰ���
	STATUS_KEY_PRESS,			//��������
	STATUS_KEY_SHORT_PRESS,		//�����̰�
	STATUS_KEY_SHORT_WAIT,		//�̰��ȴ�
	STATUS_KEY_LONG_PRESS,		//��������
	STATUS_KEY_PRESS_LEAVE,		//�����ɿ�
}STATUS_KEY_TypeDef;


//������������
typedef struct
{
	uint32_t index;				//����
	char *pNum;					//���ֻ���
	char *pCap;					//��д��ĸ����
	char *pLow;					//Сд��ĸ����
}KB_INDEX_TypeDef;


//���뷨
typedef enum
{
	STA_NUM,					//����
	STA_CAP,					//��д��ĸ
	STA_LOW,					//Сд��ĸ
}TYPE_PUTIN_TypeDef;


typedef enum
{
	STATUS_DISABLE_PUTIN,		//δʹ������
	STATUS_TEXT_EDIT,			//�ı��༭״̬
	STATUS_EDIT_COMP,			//�������
	STATUS_CANCEL_PUTIN,		//ȡ������
}STATUS_PUTIN_TypeDef;			//����״̬

typedef enum
{
	TYPE_INT = 0,				//����
	TYPE_FLOAT,					//������
	TYPE_CHAR,					//�ַ�����
}TYPE_SAVE_TypeDef;				//�������ݱ�������

typedef enum
{
	ENABLE_SHIFT,				//ʹ���л����뷨
	DISABLE_SHIFT,				//��ʹ���л����뷨
}STATUS_SHIFT_TypeDef;

//�����ṹ��
typedef struct
{	
	uint32_t value;									//��ֵ
	uint32_t recordValue;							//��¼��һ�μ�ֵ
	STATUS_KEY_TypeDef status;						//����״̬
}KEY_TypeDef; 

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t pointColor;
	uint16_t backColor;
	FunctionalState open;
}PUTIN_SKIN_TypeDef;

//���뷨
typedef struct
{
	PUTIN_SKIN_TypeDef skin;
	STATUS_SHIFT_TypeDef NewShift;
	uint16_t x;
	uint16_t y;
	uint16_t putinFrameLenth;				//����򳤶�
	uint16_t putinFrameWidth;				//�������
	uint16_t MenuPointColor;
	uint16_t MenuBackColor;
	uint8_t AllowPutinBit;
	uint8_t FillBit;
	uint8_t *PutinNum;
	TYPE_SAVE_TypeDef SaveType;
	FunctionalState CancelFunc;		//ȡ�����빦��
	uint8_t FontSize;
	FunctionalState Encrypt;		//����
	char queueBuff[MAX_ALLOW_PUTIN_CNT];
	uint32_t value;
	uint32_t recordValue;
	uint8_t indexCycle;								//ͬһ��������Ӧ�Ĳ�ͬ����ĸ����ֵ
	FlagStatus putinCharIng;						//����������ĸ
	TYPE_PUTIN_TypeDef typewriting;					//���뷨
	BoolStatus validPutin;							//������Ч����
	STATUS_PUTIN_TypeDef putinStatus;				//����״̬
}PUTIN_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BoolStatus IsPressKey( void );
void PutinProcessCycle( void );
void KeyProcessCycle( void );
void InitPutinShow( void );
void KeyPutinChars( PUTIN_TypeDef *Putin );
int32_t *GetPutinIntDataAddr( void );
float *GetPutinFloatDataAddr( void );
char *GetPutinCharDataAddr( void );
PUTIN_TypeDef *GetPutinAddr( void );
uint32_t GetKeyVal( void );
STATUS_PUTIN_TypeDef GetPutinStatus( void );
void InitKeyCheck( void );
STATUS_KEY_TypeDef GetKeyStatus( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
