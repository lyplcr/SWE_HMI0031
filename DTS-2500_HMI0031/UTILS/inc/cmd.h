/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CMD_H
#define __CMD_H

/* Includes ------------------------------------------------------------------*/
#define	SPD_CAL_NUM						500		//ÿ20ms��¼һ�����ݣ���¼10���ʱ���ڵ�����
#define UART_PACKAGE_BUFF_SIZE			500		//���ڽ������ݰ������С
#define	CMD_BUFF_SIZE					200		//����峤��
#define	PRM_BUFF_CHECK					64 		//д��λ����������

#define	SEND_ACODE						0		//���;����� 
#define	SEND_VALUE 						1		//����ֵ
#define	SEND_RCODE						2		//���������

#define	MODE_KY_MANU					0		//��ѹ�ֶ�	 //�й�����CMD_CTRL_MODE�ļ������
#define	MODE_KY_AUTO					1		//��ѹ�Զ�	 
#define	MODE_KZ_MANU 					2		//�����ֶ�
#define	MODE_KZ_AUTO					3		//�����Զ�

#define MODE_OPEN_LOC					0		//��λ����
#define MODE_OPEN_POSITIVE				1		//��������
#define MODE_OPEN_NEGATIVE				2		//��������
#define MODE_OPEN_FEEDBACK				3		//�������Ķ�λ����


/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)

//ͨ�������ʽ
typedef struct
{
	uint16_t cmd;
}CMD_GENERAL_TypeDef;
		
//������Ȩ��������
typedef struct
{ 
	uint16_t cmd;
	uint32_t password; 
}CMD_PRIVATE_ACCESS_TypeDef;

//������
typedef struct 
{ 
	uint16_t cmd;
	uint16_t start_addr; 
	uint16_t len;
}CMD_READ_PRM_TypeDef; 		
 		
//д����
typedef struct 
{ 
	uint16_t cmd;
	uint16_t start_addr; 
	uint16_t len;
	uint8_t start_byte;
}CMD_WRITE_PRM_TypeDef; 

//�˳�����
typedef struct 
{
	uint16_t cmd;
	uint32_t rsv;	
}CMD_SIGN_OFF_Typedef;

//�������Ϳ�ʼ����
typedef struct
{
    uint16_t cmd;
    uint8_t type;
}CMD_SEND_START_TypeDef; 	 

//�������� 		
typedef struct
{
	uint16_t cmd;
	uint8_t ch;
	uint8_t src;
	int32_t zero_code;
}CMD_SET_TARE_TypeDef;
 		
//����ģʽѡ��  
typedef struct
{ 
	uint16_t cmd; 
	uint8_t ctrl_mode; 
}CMD_CTRL_MODE_TypeDef; 		
 		
 		
//λ���� 		
typedef struct
{
	uint16_t cmd;
	uint16_t clr;
	uint16_t set;
	uint16_t rev;
}CMD_BIT_OPERATE_TypeDef; 		
 	
 		
//��������(ֵ��ʽ) 
typedef struct
{ 
	uint16_t cmd; 
	STEP_TypeDef step;
}CMD_STEP_TypeDef; 

//�·�����������(��Э��)  
typedef struct
{ 
	uint16_t cmd; 
	int32_t open_degree;
}CMD_OPEN_LOOP_TypeDef;

//�·�����������(��Э��)  
typedef struct
{ 
	uint16_t cmd; 
	int32_t open_degree;
	uint8_t type;
	uint8_t rev[3]; 
}CMD_OPEN_LOOP_EX_TypeDef;

//�����豸ID
typedef struct
{
	uint16_t cmd; 
	uint32_t id;
}CMD_SET_ID_TypeDef;


//--------------------------��λ���ظ����ݰ��ṹ------------------------------//

//�޻ظ�����
typedef struct
{
	uint16_t cmd;
	uint8_t status;
}RLP_NONE_TypeDef; 

//��λ���ظ�Ӳ������汾
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	uint8_t pcb_info[16];
	uint16_t fw_ver; 
}RPL_DEVICE_VERSION_TypeDef; 

//��λ���ظ�������� 
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint32_t error_code;
}RPL_ERR_CODE_TypeDef;
 
//��λ���ظ�������
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint16_t start_addr; 
	uint16_t len;
	uint8_t start_byte;
}RPL_READ_PRM_TypeDef;

//��λ���ظ�д����
typedef struct
{ 
	uint16_t cmd;
	uint8_t status;
	uint16_t start_addr; 
	uint16_t len; 
}RPL_WRITE_PRM_TypeDef;

//��λ���ظ��������
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	int32_t zero_code;
}RLP_SET_TARE_TypeDef;  
  
//��λ���ظ���ȡ�豸ID
typedef struct
{
	uint16_t cmd;
	uint8_t status;
	uint32_t id;
}RPL_GET_ID_TypeDef;

//--------------------------ʵʱ��������--------------------------------------//

//ʵʱ����״̬λ�ṹ��
typedef struct								
{
	unsigned char b_err	  	: 1;						//0����,1�д�
	unsigned char b_auto	: 1;						//0�ֶ�,1�Զ�
	unsigned char b_run	  	: 2;						//0ֹͣ,1�ջ�,2����,3δ����
	unsigned char b_dir	  	: 2;						//0����,1����,2ж��,3�ǳ������
	unsigned char b_buff	: 1;						//0������,1�������
	unsigned char b_rsv2 	: 1;						//����λ
	unsigned char b_ctrl	: 8;						//���ƶ���,0������ֹͣ,1������1.......
	unsigned short b_rsv	: 16;						//����λ
}CTRL_STATUS_TypeDef;
 

  
//ʵʱ����֡�ṹ�塾��ķ�ʽ���͡�
typedef struct  
{
	uint16_t cmd;
	CTRL_STATUS_TypeDef status;					//״̬�ֽ�
	uint16_t input;								//�����ֽ�
	uint16_t sw_rol;							//ֻ�����		
	uint16_t sw_rw;								//�ɶ�д���	
	int32_t ctrl_dgr; 							//������	
	uint32_t time_base;							//��׼ʱ��		
	uint16_t rsv;								//�����ֽ�	
	int32_t smpl[SMPL_NUM];						//��������	
}SAMPLE_C_TypeDef;   
  
    

//--------------------------��/��λ��ͨѶ״̬���ݰ��ṹ-----------------------//  

  
//�ٶȼ���ṹ��
typedef struct
{
	uint16_t index;							//���� 
	uint32_t time[SPD_CAL_NUM];				//ʱ���¼
	float val[3][SPD_CAL_NUM];				//3��ͨ����ֵ��¼  
	float speed[3];							//����ͨ�����ٶ�ֵ
	uint32_t start_t;						//��ʼ��¼��ʱ��
	float start_v;							//��ʼ��¼��ֵ
}Spd_Cal_Typedef;  
 
   
   
//����״̬
typedef enum
{
	CMD_IDLE=0,		//�������
	CMD_TX,			//�������	
	CMD_RX,	 		//���������
	CMD_BUSY, 		//����æ
	CMD_UNLINK,		//ʧȥ����
}CMD_STATUS_TypeDef;	 
  
//��λ�������������ݿ�� 
typedef struct  
{
	uint8_t index;		  //��������ֵ
	uint16_t cmd;		  //��������
	uint8_t send_type;	  //��ʼ����ʵʱ��������
	uint8_t acess;		  //��Ȩ��������״̬
	uint16_t addr;		  //д������ʼ��ַ
	uint16_t len;		  //д��������
	uint8_t *dat; 		  //д������ʼ�ֽ�
	uint8_t tare_ch;	  //����ͨ��
	uint8_t ctrl_mode;	  //����ģʽ
	uint16_t bit_clr;	  //�������(����)
	uint16_t bit_set;	  //���ñ���(����)
	uint16_t bit_rev;	  //ȡ������(����)
	STEP_TypeDef step;	  //���鲽��
	int32_t open; 		  //����������
}PCM_LSSUED_TypeDef; 


//��λ���ظ���������ݿ��
typedef struct 
{
	uint8_t status;			//�ظ�״̬
	uint8_t index;		  	//�ظ�����
	uint16_t cmd;		  	//�ظ�����
	char pcb_info[16];		//PCB�汾
	uint16_t fw_ver;	  	//�̼��汾
	uint32_t err;		  	//�������
	uint32_t tare_code;	 	//�����
	uint32_t id;			//�豸ID
}PRM_REPLY_TypeDef;

typedef struct
{
	BoolStatus isRxReply;
	CMD_STATUS_TypeDef cmdStatus;
	PCM_LSSUED_TypeDef pcmLssued;
	PRM_REPLY_TypeDef  prmReply;
	uint8_t sendCnt;			
}CMD_MUTUAL_TypeDef;

//����״̬
typedef enum
{
	LINK_UNLINK=0,			//�Ͽ�����	
	LINK_SEND_STOP,			//����ֹͣ
	LINK_SIGN_ON,			//SIGN ON 
	LINK_CHECK_PRM,			//������		
	LINK_MDY_PRM,			//�·�����
	LINK_SEND_START,		//���Ϳ�ʼ
	LINK_SUCCESS,			//���ӳɹ�
	LINK_IDLE,				//����״̬	
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
	uint32_t keepLiveCount;	//����ֵ
}LINK_MUTUAL_TypeDef;

typedef struct
{
	uint8_t *pSourceAddr;	//ָ����λ�����������ָ��
	uint16_t targetAddr;	//��λ�����������ַ
	uint16_t len;			//д�볤��
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
	uint16_t st_input;					//�ⲿ������״̬
	uint16_t st_sw_rol;					//ֻ�����					
	uint16_t st_sw_rw;					//�ɶ�д���
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
