/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CMD_H
#define __CMD_H

/* Includes ------------------------------------------------------------------*/
#define	SPD_CAL_NUM						500		//每20ms记录一个数据，记录10秒的时间内的数据
#define UART_PACKAGE_BUFF_SIZE			500		//串口接收数据包缓存大小
#define	CMD_BUFF_SIZE					200		//命令缓冲长度
#define	PRM_BUFF_CHECK					64 		//写下位机参数长度

#define	SEND_ACODE						0		//发送绝对码 
#define	SEND_VALUE 						1		//发送值
#define	SEND_RCODE						2		//发送相对码

#define	MODE_KY_MANU					0		//抗压手动	 //有关命令CMD_CTRL_MODE的几种组合
#define	MODE_KY_AUTO					1		//抗压自动	 
#define	MODE_KZ_MANU 					2		//抗折手动
#define	MODE_KZ_AUTO					3		//抗折自动

#define MODE_OPEN_LOC					0		//定位开环
#define MODE_OPEN_POSITIVE				1		//开环正向
#define MODE_OPEN_NEGATIVE				2		//开环负向
#define MODE_OPEN_FEEDBACK				3		//待反馈的定位开环


/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)

//通用命令格式
typedef struct
{
	uint16_t cmd;
}CMD_GENERAL_TypeDef;
		
//主机特权访问命令
typedef struct
{ 
	uint16_t cmd;
	uint32_t password; 
}CMD_PRIVATE_ACCESS_TypeDef;

//读参数
typedef struct 
{ 
	uint16_t cmd;
	uint16_t start_addr; 
	uint16_t len;
}CMD_READ_PRM_TypeDef; 		
 		
//写参数
typedef struct 
{ 
	uint16_t cmd;
	uint16_t start_addr; 
	uint16_t len;
	uint8_t start_byte;
}CMD_WRITE_PRM_TypeDef; 

//退出连接
typedef struct 
{
	uint16_t cmd;
	uint32_t rsv;	
}CMD_SIGN_OFF_Typedef;

//主机发送开始命令
typedef struct
{
    uint16_t cmd;
    uint8_t type;
}CMD_SEND_START_TypeDef; 	 

//清零命令 		
typedef struct
{
	uint16_t cmd;
	uint8_t ch;
	uint8_t src;
	int32_t zero_code;
}CMD_SET_TARE_TypeDef;
 		
//控制模式选择  
typedef struct
{ 
	uint16_t cmd; 
	uint8_t ctrl_mode; 
}CMD_CTRL_MODE_TypeDef; 		
 		
 		
//位操作 		
typedef struct
{
	uint16_t cmd;
	uint16_t clr;
	uint16_t set;
	uint16_t rev;
}CMD_BIT_OPERATE_TypeDef; 		
 	
 		
//步骤命令(值方式) 
typedef struct
{ 
	uint16_t cmd; 
	STEP_TypeDef step;
}CMD_STEP_TypeDef; 

//下发开环控制量(旧协议)  
typedef struct
{ 
	uint16_t cmd; 
	int32_t open_degree;
}CMD_OPEN_LOOP_TypeDef;

//下发开环控制量(新协议)  
typedef struct
{ 
	uint16_t cmd; 
	int32_t open_degree;
	uint8_t type;
	uint8_t rev[3]; 
}CMD_OPEN_LOOP_EX_TypeDef;

//设置设备ID
typedef struct
{
	uint16_t cmd; 
	uint32_t id;
}CMD_SET_ID_TypeDef;


//--------------------------下位机回复数据包结构------------------------------//

//无回复数据
typedef struct
{
	uint16_t cmd;
	uint8_t status;
}RLP_NONE_TypeDef; 

//下位机回复硬件软件版本
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	uint8_t pcb_info[16];
	uint16_t fw_ver; 
}RPL_DEVICE_VERSION_TypeDef; 

//下位机回复错误代码 
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint32_t error_code;
}RPL_ERR_CODE_TypeDef;
 
//下位机回复读参数
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint16_t start_addr; 
	uint16_t len;
	uint8_t start_byte;
}RPL_READ_PRM_TypeDef;

//下位机回复写参数
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint16_t start_addr; 
	uint16_t len; 
}RPL_WRITE_PRM_TypeDef;

//下位机回复设置零点
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	int32_t zero_code;
}RLP_SET_TARE_TypeDef;  
  
//下位机回复获取设备ID
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	uint32_t id;
}RPL_GET_ID_TypeDef;

//--------------------------实时采样数据--------------------------------------//

//实时数据状态位结构体
typedef struct								
{
	unsigned char b_err	  	: 1;						//0正常,1有错
	unsigned char b_auto	: 1;						//0手动,1自动
	unsigned char b_run	  	: 2;						//0停止,1闭环,2开环,3未定义
	unsigned char b_dir	  	: 2;						//0保持,1加载,2卸载,3非常规控制
	unsigned char b_buff	: 1;						//0命令缓存空,1命令缓存满
	unsigned char b_rsv2 	: 1;						//保留位
	unsigned char b_ctrl	: 8;						//控制对象,0开环或停止,1传感器1.......
	unsigned short b_rsv	: 16;						//保留位
}CTRL_STATUS_TypeDef;
 

  
//实时数据帧结构体【码的方式传送】
typedef struct  
{
	uint16_t cmd;
	CTRL_STATUS_TypeDef status;					//状态字节
	uint16_t input;								//输入字节
	uint16_t sw_rol;							//只读输出		
	uint16_t sw_rw;								//可读写输出	
	int32_t ctrl_dgr; 							//控制量	
	uint32_t time_base;							//基准时间		
	uint16_t rsv;								//保留字节	
	int32_t smpl[SMPL_NUM];						//传感器码	
}SAMPLE_C_TypeDef;   
  
    

//--------------------------上/下位机通讯状态数据包结构-----------------------//  

  
//速度计算结构体
typedef struct
{
	uint16_t index;							//索引 
	uint32_t time[SPD_CAL_NUM];				//时间记录
	float val[3][SPD_CAL_NUM];				//3个通道的值记录  
	float speed[3];							//三个通道的速度值
	uint32_t start_t;						//开始记录的时间
	float start_v;							//开始记录的值
}Spd_Cal_Typedef;  
 
   
   
//命令状态
typedef enum
{
	CMD_IDLE=0,		//命令空闲
	CMD_TX,			//命令发送中	
	CMD_RX,	 		//命令接收中
	CMD_BUSY, 		//命令忙
	CMD_UNLINK,		//失去联接
}CMD_STATUS_TypeDef;	 
  
//上位机发送命令数据框架 
typedef struct  
{
	uint8_t index;		  //发送索引值
	uint16_t cmd;		  //发送命令
	uint8_t send_type;	  //开始发送实时数据类型
	uint8_t acess;		  //特权参数访问状态
	uint16_t addr;		  //写参数起始地址
	uint16_t len;		  //写参数长度
	uint8_t *dat; 		  //写参数起始字节
	uint8_t tare_ch;	  //清零通道
	uint8_t ctrl_mode;	  //控制模式
	uint16_t bit_clr;	  //清除保护(开关)
	uint16_t bit_set;	  //设置保护(开关)
	uint16_t bit_rev;	  //取反保护(开关)
	STEP_TypeDef step;	  //试验步骤
	int32_t open; 		  //开环控制量
}PCM_LSSUED_TypeDef; 


//下位机回复命令的数据框架
typedef struct 
{
	uint8_t status;			//回复状态
	uint8_t index;		  	//回复索引
	uint16_t cmd;		  	//回复命令
	char pcb_info[16];		//PCB版本
	uint16_t fw_ver;	  	//固件版本
	uint32_t err;		  	//错误代码
	uint32_t tare_code;	 	//零点码
	uint32_t id;			//设备ID
}PRM_REPLY_TypeDef;

typedef struct
{
	BoolStatus isRxReply;
	CMD_STATUS_TypeDef cmdStatus;
	PCM_LSSUED_TypeDef pcmLssued;
	PRM_REPLY_TypeDef  prmReply;
	uint8_t sendCnt;			
}CMD_MUTUAL_TypeDef;

//联机状态
typedef enum
{
	LINK_UNLINK=0,			//断开连接	
	LINK_SEND_STOP,			//发送停止
	LINK_SIGN_ON,			//SIGN ON 
	LINK_CHECK_PRM,			//检查参数		
	LINK_MDY_PRM,			//下发参数
	LINK_SEND_START,		//发送开始
	LINK_SUCCESS,			//连接成功
	LINK_IDLE,				//空闲状态	
}LINK_STATUS_TypeDef;

typedef struct
{
	BoolStatus isRxReply;
	CMD_STATUS_TypeDef  cmdStatus;
	LINK_STATUS_TypeDef linkStatus;	
	uint8_t sendIndex;
	PRM_REPLY_TypeDef  prmReply;
	uint8_t sendCnt;
	uint8_t process;
	uint8_t mdyCnt;
	uint32_t keepLiveCount;	//保活值
}LINK_MUTUAL_TypeDef;

typedef struct
{
	uint8_t *pSourceAddr;	//指向上位机参数区域的指针
	uint16_t targetAddr;	//下位机参数区域地址
	uint16_t len;			//写入长度
}WRITE_PRM_PARA_TypeDef;

typedef enum
{
	STOP_STATUS = 0,
	CLOSE_CONTROL,
	OPEN_CONTROL,
	UNDIFINED,
}CUR_STEP_STATUS_TypeDef;

typedef enum
{
	KEEP_STATUS = 0,
	LOAD_STATUS,
	UNLOAD_STATUS,
	UNDEFINE_STATUS,
}CUR_DIR_TypeDef;

typedef struct
{
	uint16_t st_input;					//外部的输入状态
	uint16_t st_sw_rol;					//只读输出					
	uint16_t st_sw_rw;					//可读写输出
}COMM_IO_STATUS_TypeDef;
	
#pragma pack()

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
float spd_cal(uint8_t chn, uint16_t gap);
void spd_cal_cycle(void);
float get_smpl_force(uint8_t chn);
float get_smpl_code(uint8_t chn);
float get_smpl_spd(uint8_t chn);
uint16_t uart_get_cmd( void );
uint8_t *uart_get_body(void);
uint8_t uart_get_index(void);
uint8_t uart_get_size(void);
uint8_t uart_get_addr(void);
void *uart_get_frame_addr(void);
CMD_STATUS_TypeDef comm_cycle( void );
void sample_cycle( void );

SAMPLE_C_TypeDef *GetSampleAddr( void );
int32_t GetSammpleCode( uint8_t channel );
FlagStatus GetPrmWarnStatus( void );

PRM_REPLY_TypeDef *GetPrmReplyAddr( void );
CMD_STATUS_TypeDef cmd_get_device_version_pkg( void );
CMD_STATUS_TypeDef cmd_get_error_pkg( void );
CMD_STATUS_TypeDef cmd_clear_error_pkg( void );
CMD_STATUS_TypeDef cmd_read_prm_pkg( uint16_t start_addr, uint16_t len );
CMD_STATUS_TypeDef cmd_write_prm_pkg( uint16_t start_addr, uint16_t len, uint8_t *pData );
CMD_STATUS_TypeDef cmd_send_start_pkg( uint8_t type );
CMD_STATUS_TypeDef cmd_send_stop_pkg( void );
CMD_STATUS_TypeDef cmd_set_tare_pkg( uint8_t chn );
CMD_STATUS_TypeDef cmd_ctrl_mode_pkg( uint8_t col_mode );
CMD_STATUS_TypeDef cmd_protect_pkg( uint16_t clr,uint16_t set,uint16_t rev );
CMD_STATUS_TypeDef cmd_stop_pkg( void );
CMD_STATUS_TypeDef cmd_step_norm_v_pkg( STEP_TypeDef step );
CMD_STATUS_TypeDef cmd_open_loop_pkg( int32_t open );
CMD_STATUS_TypeDef cmd_switch_pkg( uint16_t clr,uint16_t set,uint16_t rev );
CMD_STATUS_TypeDef cmd_get_device_id_pkg( void );
CMD_STATUS_TypeDef cmd_set_device_id_pkg( void );

void link_init( void );
LINK_STATUS_TypeDef link_cycle( void );
uint8_t GetLinkProcess( void );
LINK_STATUS_TypeDef GetLinkStatus( void );
uint32_t GetKeepLiveCount( void );
void SetKeepLiveCount( uint32_t keepLiveCount );

BoolStatus Uart_GetPackageCycle( void );
ErrorStatus uart_check(void);
uint8_t GetPrmReplyStatus( void );

uint16_t GetReadWriteOutput( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
