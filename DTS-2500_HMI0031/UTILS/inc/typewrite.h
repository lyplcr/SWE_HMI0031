/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEWRITE_H
#define __TYPEWRITE_H

/* Includes ------------------------------------------------------------------*/
#define MAX_ALLOW_PUTIN_CNT			20

/* 输入法皮肤UI属性 */
#define PUTIN_SKIN_START_X			715
#define PUTIN_SKIN_START_Y			410
#define	PUTIN_SKIN_POINT_COLOR		LITTLE_BLUE_SKY

#define COLOR_PUTIN_FONT_POINT			GREEN			//正在输入字符背景色
#define COLOR_PUTIN_FONT_BACK			DARKBLUE		//正在输入字符背景色

#define PUTIN_RECANGLE_CUE			"请输入..."

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	STATUS_KEY_NO_PRESS = 0,	//按键未按下
	STATUS_KEY_WAIT,			//中间等待状态
	STATUS_KEY_PRESS_CONFIRM,	//确认按下
	STATUS_KEY_PRESS,			//按键按下
	STATUS_KEY_SHORT_PRESS,		//按键短按
	STATUS_KEY_SHORT_WAIT,		//短按等待
	STATUS_KEY_LONG_PRESS,		//按键长按
	STATUS_KEY_PRESS_LEAVE,		//按键松开
}STATUS_KEY_TypeDef;


//键盘数据索引
typedef struct
{
	uint32_t index;				//索引
	char *pNum;					//数字缓存
	char *pCap;					//大写字母缓存
	char *pLow;					//小写字母缓存
}KB_INDEX_TypeDef;


//输入法
typedef enum
{
	STA_NUM,					//数字
	STA_CAP,					//大写字母
	STA_LOW,					//小写字母
}TYPE_PUTIN_TypeDef;


typedef enum
{
	STATUS_DISABLE_PUTIN,		//未使能输入
	STATUS_TEXT_EDIT,			//文本编辑状态
	STATUS_EDIT_COMP,			//输入完成
	STATUS_CANCEL_PUTIN,		//取消输入
}STATUS_PUTIN_TypeDef;			//输入状态

typedef enum
{
	TYPE_INT = 0,				//整型
	TYPE_FLOAT,					//浮点型
	TYPE_CHAR,					//字符类型
}TYPE_SAVE_TypeDef;				//输入数据保存类型

typedef enum
{
	ENABLE_SHIFT,				//使能切换输入法
	DISABLE_SHIFT,				//不使能切换输入法
}STATUS_SHIFT_TypeDef;

//按键结构体
typedef struct
{	
	uint32_t value;									//键值
	uint32_t recordValue;							//记录上一次键值
	STATUS_KEY_TypeDef status;						//按键状态
}KEY_TypeDef; 

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t pointColor;
	uint16_t backColor;
	FunctionalState open;
}PUTIN_SKIN_TypeDef;

//输入法
typedef struct
{
	PUTIN_SKIN_TypeDef skin;
	STATUS_SHIFT_TypeDef NewShift;
	uint16_t x;
	uint16_t y;
	uint16_t putinFrameLenth;				//输入框长度
	uint16_t putinFrameWidth;				//输入框宽度
	uint16_t MenuPointColor;
	uint16_t MenuBackColor;
	uint8_t AllowPutinBit;
	uint8_t FillBit;
	uint8_t *PutinNum;
	TYPE_SAVE_TypeDef SaveType;
	FunctionalState CancelFunc;		//取消输入功能
	uint8_t FontSize;
	FunctionalState Encrypt;		//加密
	char queueBuff[MAX_ALLOW_PUTIN_CNT];
	uint32_t value;
	uint32_t recordValue;
	uint8_t indexCycle;								//同一个按键对应的不同的字母索引值
	FlagStatus putinCharIng;						//正在输入字母
	TYPE_PUTIN_TypeDef typewriting;					//输入法
	BoolStatus validPutin;							//输入有效数据
	STATUS_PUTIN_TypeDef putinStatus;				//输入状态
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
