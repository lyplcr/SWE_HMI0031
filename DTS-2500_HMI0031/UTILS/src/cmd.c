/**
  ******************************************************************************
  * @file    cmd.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-10-25
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   ������ش������
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "cmd.h"

/* Private define ------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#pragma arm section rwdata = "RAM_CMD",zidata = "RAM_CMD"
	static Spd_Cal_Typedef	g_speedCalculate; 	//�ٶȼ���
	static SAMPLE_C_TypeDef g_sampleCycle;		//����ѭ����
	static float g_sampleForce[SMPL_NUM];		//��������ֵ
	static int32_t g_sampleCode[SMPL_NUM];		//��������ֵ
	static uint8_t g_uartPackageRxBuff[UART_PACKAGE_BUFF_SIZE];	//���ڽ������ݰ�����
	static CMD_MUTUAL_TypeDef g_cmdMutual;		//�����
	static LINK_MUTUAL_TypeDef g_linkMutual;	//��������
	static WRITE_PRM_PARA_TypeDef g_WritePrm;	//д��λ����������
	static COMM_IO_STATUS_TypeDef g_ioStatus;
#pragma arm section

/* Private function prototypes -----------------------------------------------*/
static void InitSendCmd( void );
static CMD_STATUS_TypeDef CmdMutualBody( void );
static void uart_send_package(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr);
static BoolStatus isAllowLssued( void );



/* Private functions ---------------------------------------------------------*/
// --------------------------- ���Ƿָ��� ----------------------------------- //

/*
	 ��λ���·����ݰ�(����ʵ�ʴ����͵����ݰ�) 
*/  
/**********************************************************************
functionName:void cmd_general(uint16_t cmd)
description:ͨ������ʺ���������Ϊ�յ�ָ��
**********************************************************************/  
void cmd_general(uint16_t cmd)
{
	uint8_t index;
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	uart_send_package(COM_ADD_UART,index,sizeof(cmd),(uint8_t *)&cmd);
} 
  
/**********************************************************************
functionName:void cmd_device_version( void )
description:CMD_DEVICE_VERSION / Ӳ�����̼��汾 
**********************************************************************/
void cmd_device_version( void )
{
	cmd_general(CMD_DEVICE_VERSION);	
}

/**********************************************************************
functionName:void cmd_reset( void )
description:CMD_RESET / ��λ
**********************************************************************/
void cmd_reset( void )
{
	cmd_general(CMD_RESET);	
}

/**********************************************************************
functionName:void cmd_private_access(uint8_t acess)
description:����CMD_PRIVATE_ACCESS
acess=0 �˳���Ȩ״̬  acess=1 ������Ȩ״̬
**********************************************************************/
void cmd_private_access(uint8_t acess)
{
	uint8_t index;
	CMD_PRIVATE_ACCESS_TypeDef private_acce;

	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	private_acce.cmd=CMD_PRIVATE_ACCESS;
	if(acess)
	private_acce.password=PRIVATE_PASSWORD;
	else
	private_acce.password=0x00000000;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_PRIVATE_ACCESS_TypeDef),(uint8_t *)&private_acce);  
	
}

/**********************************************************************
functionName:void cmd_get_error( void )
description:CMD_GET_ERROR / ��ȡ�豸����
**********************************************************************/
void cmd_get_error( void )
{
	cmd_general(CMD_GET_ERROR);	
}

/**********************************************************************
functionName:void cmd_clear_error( void )
description:CMD_CLEAR_ERROR / ����豸���� 
**********************************************************************/
void cmd_clear_error( void )
{
	cmd_general(CMD_CLEAR_ERROR);	
}

/**********************************************************************
functionName:void cmd_read_prm(uint8_t index,uint16_t start,uint16_t len)
description:����CMD_READ_PRM / ��ȡ����
**********************************************************************/
void cmd_read_prm(uint16_t start,uint16_t len)
{
	uint8_t index;
	CMD_READ_PRM_TypeDef read_prm;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	read_prm.cmd=CMD_READ_PRM;
	read_prm.start_addr=start;
	read_prm.len=len;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_READ_PRM_TypeDef),(uint8_t *)&read_prm);   
}

/**********************************************************************
functionName:void cmd_write_prm(uint8_t index,uint16_t start,uint16_t len,uint8_t *dat)
description:����CMD_WRITE_PRM / ���ò���
**********************************************************************/
void cmd_write_prm(uint16_t start,uint16_t len,uint8_t *dat)
{
	 uint8_t index;	
	 uint8_t buff[CMD_BUFF_SIZE];
	 CMD_WRITE_PRM_TypeDef *write_prm;
	 uint8_t *first_dat;		
	 
	 index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	
	 write_prm=(CMD_WRITE_PRM_TypeDef *)buff;
	 write_prm->cmd=CMD_WRITE_PRM;
	 write_prm->start_addr=start;
	 write_prm->len=len;
	 first_dat=&(write_prm->start_byte);
	
	 if(len>(CMD_BUFF_SIZE-sizeof(CMD_WRITE_PRM_TypeDef)))
	 len=CMD_BUFF_SIZE-sizeof(CMD_WRITE_PRM_TypeDef);	
	 memcpy(first_dat,dat,len);
	 
	 uart_send_package(COM_ADD_UART,index,sizeof(CMD_WRITE_PRM_TypeDef)+len-1,buff); 	 
}

/**********************************************************************
functionName:void cmd_read_prv(uint8_t index,uint16_t start,uint16_t len)
description:����CMD_READ_PRV / ��ȡ��Ȩ����
**********************************************************************/
void cmd_read_prv(uint16_t start,uint16_t len)
{
	uint8_t index;
	CMD_READ_PRM_TypeDef read_prm;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	read_prm.cmd=CMD_READ_PRV;
	read_prm.start_addr=start;
	read_prm.len=len;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_READ_PRM_TypeDef),(uint8_t *)&read_prm); 
}

/**********************************************************************
functionName:void cmd_write_prv(uint8_t index,uint16_t start,uint16_t len,uint8_t *dat)
description:����CMD_WRITE_PRV / ������Ȩ����
**********************************************************************/
void cmd_write_prv(uint16_t start,uint16_t len,uint8_t *dat)
{
	 uint8_t index;
	 uint8_t buff[CMD_BUFF_SIZE];
	 CMD_WRITE_PRM_TypeDef *write_prm;
	 uint8_t *first_dat;		
	 
	 index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	 write_prm=(CMD_WRITE_PRM_TypeDef *)buff;
	 write_prm->cmd=CMD_WRITE_PRV;
	 write_prm->start_addr=start;
	 write_prm->len=len;
	 first_dat=&(write_prm->start_byte);
	 if(len>(CMD_BUFF_SIZE-sizeof(CMD_WRITE_PRM_TypeDef)))
	 len=CMD_BUFF_SIZE-sizeof(CMD_WRITE_PRM_TypeDef);	
	 memcpy(first_dat,dat,len);
	 uart_send_package(COM_ADD_UART,index,sizeof(CMD_WRITE_PRM_TypeDef)+len-1,buff);  	 
}


/**********************************************************************
functionName:void cmd_sign_off(uint8_t index)
description:CMD_SIGN_OFF / �Ͽ�����
**********************************************************************/  
void cmd_sign_off(void)
{
	uint8_t index;
	CMD_SIGN_OFF_Typedef sign_off;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	sign_off.cmd=CMD_SIGN_OFF;
	sign_off.rsv=0;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_SIGN_OFF_Typedef),(uint8_t *)&sign_off);  
}

/**********************************************************************
functionName:void cmd_send_start(uint8_t index,uint8_t type)
0--������; 1--ʾֵ; 2--�����
description:����CMD_SEND_START / ��ʼ����ʵʱ����
**********************************************************************/
void cmd_send_start(uint8_t type)
{
	uint8_t index;
	CMD_SEND_START_TypeDef send_start;
 	
 	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
 	send_start.cmd=CMD_SEND_START;
 	send_start.type=type;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_SEND_START_TypeDef),(uint8_t *)&send_start); 
} 

/**********************************************************************
functionName:void cmd_send_stop( void )
description:CMD_SEND_STOP / ֹͣ����ʵʱ����
**********************************************************************/
void cmd_send_stop( void )
{
	cmd_general(CMD_SEND_STOP);	

} 

/**********************************************************************
functionName:void cmd_cmd_sign_on( void )
description:CMD_SIGN_ON / ����
**********************************************************************/
void cmd_cmd_sign_on( void )
{
	cmd_general(CMD_SIGN_ON);	
}

 
/**********************************************************************
functionName:void cmd_set_tare(uint8_t index,uint8_t chn,uint8_t src,int32_t code)
description:����CMD_SET_TARE
chn:ָ����ͨ���� 0-5
src:0��ʾ���豸��ȡ��1��ʾ����λ�����ã���code��ֵΪ���
**********************************************************************/
void cmd_set_tare(uint8_t chn,uint8_t src,int32_t code)
{
	uint8_t index;
	CMD_SET_TARE_TypeDef set_tare;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	set_tare.cmd=CMD_SET_TARE;
	set_tare.ch=chn;
	set_tare.src=src;
	set_tare.zero_code=code;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_SET_TARE_TypeDef),(uint8_t *)&set_tare);  
}
 
  
/**********************************************************************
functionName:void cmd_ctrl_mode(uint8_t index,uint8_t mode)
description:����CMD_CTRL_MODE
bit0:0��ʾ�ֶ���1��ʾ�Զ�
bit1:0��ʾ��ѹ��1��ʾ����
MODE_AUTO_KY	0x01 
MODE_AUTO_KZ 	0x03
**********************************************************************/
void cmd_ctrl_mode(uint8_t mode)
{
	uint8_t index;
	CMD_CTRL_MODE_TypeDef ctrl_mode;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	ctrl_mode.cmd=CMD_CTRL_MODE;
	ctrl_mode.ctrl_mode=mode;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_CTRL_MODE_TypeDef),(uint8_t *)&ctrl_mode);   
}


/**********************************************************************
functionName:void cmd_protect(uint8_t index,uint16_t clr,uint16_t set,uint16_t rev)
description:����CMD_PROTECT / �豸����
**********************************************************************/
void cmd_protect(uint16_t clr,uint16_t set,uint16_t rev)
{
	uint8_t index;
	CMD_BIT_OPERATE_TypeDef cmd_prot;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	cmd_prot.cmd=CMD_PROTECT;
	cmd_prot.clr=clr;
	cmd_prot.set=set;
	cmd_prot.rev=rev;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_BIT_OPERATE_TypeDef),(uint8_t *)&cmd_prot);   
}

/**********************************************************************
functionName:void cmd_stop( void )
description:����CMD_STOP / ǿ���ж�
**********************************************************************/
void cmd_stop( void )
{
	cmd_general(CMD_STOP);	
}


/**********************************************************************
functionName:void cmd_step_norm_v(STEP_TypeDef step)
description:����CMD_STEP_NORM_V
**********************************************************************/
void cmd_step_norm_v(STEP_TypeDef step)
{
	uint8_t index;
	CMD_STEP_TypeDef cmd_step;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	cmd_step.cmd=CMD_STEP_NORM_V;
	cmd_step.step=step;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_STEP_TypeDef),(uint8_t *)&cmd_step);   
}
  
 
/**********************************************************************
functionName:void cmd_open_loop(uint8_t index,int32_t open)
description:����CMD_OPEN_LOOP / ����������
**********************************************************************/
void cmd_open_loop(int32_t open)
{
	uint8_t index;
	CMD_OPEN_LOOP_TypeDef open_loop;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	open_loop.cmd=CMD_OPEN_LOOP;
	open_loop.open_degree=open;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_OPEN_LOOP_TypeDef),(uint8_t *)&open_loop);   
}


/**********************************************************************
functionName:void cmd_switch(uint8_t index,uint16_t clr,uint16_t set,uint16_t rev)
description:����CMD_SWITCH
**********************************************************************/
void cmd_switch(uint16_t clr,uint16_t set,uint16_t rev)
{
	uint8_t index;
	CMD_BIT_OPERATE_TypeDef swich;
	
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	swich.cmd=CMD_SWITCH;
	swich.clr=clr;
	swich.set=set;
	swich.rev=rev;
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_BIT_OPERATE_TypeDef),(uint8_t *)&swich);   
}


/**********************************************************************
functionName:void cmd_set_device_id( uint32_t id )
description:����CMD_SET_DEVICE_ID / �����豸ID
**********************************************************************/
void cmd_set_device_id( uint32_t id )
{
	uint8_t index;
	CMD_SET_ID_TypeDef id_num;
	
	index = next_index(INDEX_H_ASW,COM_ADD_UART); 
	
	id_num.cmd = CMD_SET_DEVICE_ID;
	id_num.id = id;
	
	uart_send_package(COM_ADD_UART,index,sizeof(CMD_SET_ID_TypeDef),(uint8_t *)(&id_num));		
}



// --------------------------- ���Ƿָ��� ----------------------------------- //


/*
	��λ���ظ����ݰ� 
*/

/**********************************************************************
functionName:void cmd_device_version_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:CMD_DEVICE_VERSION / Ӳ�����̼��汾 
**********************************************************************/
void cmd_device_version_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_DEVICE_VERSION_TypeDef *pDevice_version;

	pDevice_version = (RPL_DEVICE_VERSION_TypeDef *)pData;

	memcpy(pRpl->pcb_info,pDevice_version->pcb_info,16);	//��ȡPCB�汾
	pRpl->fw_ver = pDevice_version->fw_ver;					//��ȡ�̼��汾
	pRpl->status = pDevice_version->status;			
}

/**********************************************************************
functionName:reply_empty
description:�ظ���
**********************************************************************/
void reply_empty( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RLP_NONE_TypeDef *pRplEmpty = (RLP_NONE_TypeDef *)pData;

	pRpl->status = pRplEmpty->status;			
}

/**********************************************************************
functionName:void cmd_get_error_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:CMD_GET_ERROR / ��ȡ�豸����
**********************************************************************/
void cmd_get_error_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_ERR_CODE_TypeDef *pGet_error;
	
	pGet_error = (RPL_ERR_CODE_TypeDef *)pData;
	
	pRpl->err = pGet_error->error_code;			//��ȡ�������	
	pRpl->status = pGet_error->status;	
}

/**********************************************************************
functionName:void cmd_set_tare_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:case CMD_SET_TARE:			//�������
**********************************************************************/
void cmd_set_tare_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RLP_SET_TARE_TypeDef *pSet_tare;
	
	pSet_tare = (RLP_SET_TARE_TypeDef *)pData;
	
	pRpl->tare_code = pSet_tare->zero_code;			//��ȡ�������
	pRpl->status = pSet_tare->status;	
}


/**********************************************************************
functionName:void cmd_set_device_id( uint32_t id )
description:����CMD_GET_DEVICE_ID / �����豸ID
**********************************************************************/
void cmd_get_device_id_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_GET_ID_TypeDef *pGet_Id;
	
	pGet_Id = (RPL_GET_ID_TypeDef *)pData;
	
	pRpl->id = pGet_Id->id;
	pRpl->status = pGet_Id->status;	
}


// --------------------------- ���Ƿָ��� ----------------------------------- //
/*
	ͨѶ����(�����װ�·����ݰ������ͨ������ comm_cycle() ����) 
*/

/**********************************************************************
functionName:cmd_device_version_pkg
description:CMD_DEVICE_VERSION / Ӳ�����̼��汾 
**********************************************************************/
CMD_STATUS_TypeDef cmd_get_device_version_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_DEVICE_VERSION;				
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_get_error_pkg
description:CMD_GET_ERROR / ��ȡ�豸���� 
**********************************************************************/
CMD_STATUS_TypeDef cmd_get_error_pkg( void )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_GET_ERROR;				
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_clear_error_pkg
description:CMD_CLEAR_ERROR / ����豸���� 
**********************************************************************/
CMD_STATUS_TypeDef cmd_clear_error_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_CLEAR_ERROR;				
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_read_prm_pkg
description:����CMD_READ_PRM / ��ȡ���� 
**********************************************************************/
CMD_STATUS_TypeDef cmd_read_prm_pkg( uint16_t start_addr, uint16_t len )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 	= CMD_READ_PRM;
		g_cmdMutual.pcmLssued.addr 	= start_addr;
		g_cmdMutual.pcmLssued.len	= len;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_write_prm_pkg
description:����CMD_WRITE_PRM / ���ò��� 
**********************************************************************/
CMD_STATUS_TypeDef cmd_write_prm_pkg( uint16_t start_addr, uint16_t len, uint8_t *pData )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 	= CMD_WRITE_PRM;
		g_cmdMutual.pcmLssued.addr 	= start_addr;
		g_cmdMutual.pcmLssued.len	= len;
		g_cmdMutual.pcmLssued.dat	= pData;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_send_start_pkg
type������ʵʱ��������
description:����CMD_SEND_START / ��ʼ����ʵʱ���� 
**********************************************************************/
CMD_STATUS_TypeDef cmd_send_start_pkg( uint8_t type )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 		= CMD_SEND_START;
		g_cmdMutual.pcmLssued.send_type = type;	
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_send_stop_pkg
type������ʵʱ��������
description:CMD_SEND_STOP / ֹͣ����ʵʱ����
**********************************************************************/
CMD_STATUS_TypeDef cmd_send_stop_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_SEND_STOP;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}

/**********************************************************************
functionName:cmd_set_tare_pkg
description:����CMD_SET_TARE
chn:ָ����ͨ���� 0-5
**********************************************************************/
CMD_STATUS_TypeDef cmd_set_tare_pkg( uint8_t chn )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 		= CMD_SET_TARE;
		g_cmdMutual.pcmLssued.tare_ch 	= chn;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_ctrl_mode_pkg
description:����CMD_CTRL_MODE
col_mode������ģʽ
bit0:0��ʾ�ֶ���1��ʾ�Զ�
bit1:0��ʾ��ѹ��1��ʾ����
MODE_AUTO_KY	0x01 
MODE_AUTO_KZ 	0x03
**********************************************************************/
CMD_STATUS_TypeDef cmd_ctrl_mode_pkg( uint8_t col_mode )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 		= CMD_CTRL_MODE;
		g_cmdMutual.pcmLssued.ctrl_mode = col_mode;		
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_protect_pkg
description:����CMD_PROTECT / �豸����
**********************************************************************/
CMD_STATUS_TypeDef cmd_protect_pkg( uint16_t clr,uint16_t set,uint16_t rev )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 		= CMD_PROTECT;
		g_cmdMutual.pcmLssued.bit_clr 	= clr;
		g_cmdMutual.pcmLssued.bit_set 	= set;	
		g_cmdMutual.pcmLssued.bit_rev 	= rev;			
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_stop_pkg
description:����CMD_STOP / ǿ���ж�
**********************************************************************/
CMD_STATUS_TypeDef cmd_stop_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_STOP;		
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_step_norm_v_pkg
description:����CMD_STEP_NORM_V	/ �·����鲽��
**********************************************************************/
CMD_STATUS_TypeDef cmd_step_norm_v_pkg( STEP_TypeDef step )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_STEP_NORM_V;
		g_cmdMutual.pcmLssued.step = step;		
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;	
}


/**********************************************************************
functionName:cmd_open_loop_pkg
description:����CMD_OPEN_LOOP / ����������
**********************************************************************/
CMD_STATUS_TypeDef cmd_open_loop_pkg( int32_t open )
{	
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 	= CMD_OPEN_LOOP;
		g_cmdMutual.pcmLssued.open 	= open;	
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_switch_pkg
description:����CMD_SWITCH / ������
**********************************************************************/
CMD_STATUS_TypeDef cmd_switch_pkg( uint16_t clr,uint16_t set,uint16_t rev )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd 		= CMD_SWITCH;
		g_cmdMutual.pcmLssued.bit_clr 	= clr;
		g_cmdMutual.pcmLssued.bit_set 	= set;	
		g_cmdMutual.pcmLssued.bit_rev 	= rev;			
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_get_device_id_pkg
description:����CMD_GET_DEVICE_ID / ��ȡ�豸ID
**********************************************************************/
CMD_STATUS_TypeDef cmd_get_device_id_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_GET_DEVICE_ID;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}


/**********************************************************************
functionName:cmd_set_device_id_pkg
description:����CMD_SET_DEVICE_ID / �����豸ID
**********************************************************************/
CMD_STATUS_TypeDef cmd_set_device_id_pkg( void )
{
	if ( isAllowLssued() == YES )
	{
		InitSendCmd();
		
		g_cmdMutual.pcmLssued.cmd = CMD_SET_DEVICE_ID;
	}
	else
	{
		g_cmdMutual.cmdStatus = CMD_BUSY;		
	}

	return g_cmdMutual.cmdStatus;
}

/*------------------------------------------------------------
 * Function Name  : InitSendCmd
 * Description    : ��ʼ����������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void InitSendCmd( void )
{
	g_cmdMutual.cmdStatus = CMD_TX;
	g_cmdMutual.sendCnt = 0;
}

/*------------------------------------------------------------
 * Function Name  : CmdSendBody
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CmdSendBody( void ) 
{
	switch ( g_cmdMutual.pcmLssued.cmd )
	{
		case CMD_DEVICE_VERSION:	//PCB��Ϣ���̼��汾
			cmd_device_version();
			#ifdef DEBUG_CMD
				printf("�������CMD_DEVICE_VERSION\r\n");
			#endif
			break;
			
		case CMD_RESET:				//��������������
			cmd_reset();
			#ifdef DEBUG_CMD
				printf("�������CMD_RESET\r\n");
			#endif
			break;    

		case CMD_PRIVATE_ACCESS:	//����/�˳���Ȩ����״̬
			cmd_private_access(g_cmdMutual.pcmLssued.acess);
			#ifdef DEBUG_CMD
				printf("�������CMD_PRIVATE_ACCESS\r\n");
			#endif
			break;
			
		case CMD_GET_ERROR:			//��ȡ����
			cmd_get_error();	
			#ifdef DEBUG_CMD
				printf("�������CMD_GET_ERROR\r\n");
			#endif
			break;
		
		case CMD_CLEAR_ERROR:		//�������
			cmd_clear_error();
			#ifdef DEBUG_CMD
				printf("�������CMD_CLEAR_ERROR\r\n");
			#endif

			break;
			
		case CMD_READ_PRM:			//��ȡ����
			cmd_read_prm(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len);
			#ifdef DEBUG_CMD
				printf("�������CMD_READ_PRM\r\n");
			#endif
			break;
			
		case CMD_WRITE_PRM:			//���ò���
			cmd_write_prm(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len,g_cmdMutual.pcmLssued.dat);
			#ifdef DEBUG_CMD
				printf("�������CMD_WRITE_PRM\r\n");
			#endif
			break;
		
		case CMD_READ_PRV:			//��ȡ��Ȩ����
			cmd_read_prv(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len);
			#ifdef DEBUG_CMD
				printf("�������CMD_READ_PRV\r\n");
			#endif
			break;
			
		case CMD_WRITE_PRV:			//������Ȩ����
			cmd_write_prv(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len,g_cmdMutual.pcmLssued.dat);
			#ifdef DEBUG_CMD
				printf("�������CMD_WRITE_PRV\r\n");
			#endif
			break; 	
			
		case CMD_BACKUP_PRM:		//���ݲ���
			#ifdef DEBUG_CMD
				printf("�������CMD_BACKUP_PRM\r\n");
			#endif
			break;  				//��λ���Լ�����

		case CMD_RESTORE_PRM:		//��ԭ����
			#ifdef DEBUG_CMD
				printf("�������CMD_RESTORE_PRM\r\n");
			#endif
			break;					//��λ���Լ�����

		case CMD_SIGN_OFF:			//�Ͽ�����
			cmd_sign_off();
			#ifdef DEBUG_CMD
				printf("�������CMD_SIGN_OFF\r\n");
			#endif
			break;

		case CMD_KEEP_ALIVE:		//�������
			#ifdef DEBUG_CMD
				printf("�������CMD_KEEP_ALIVE\r\n");
			#endif
			break;					

		case CMD_SEND_START:		//��ʼ����ʵʱ����
			cmd_send_start(g_cmdMutual.pcmLssued.send_type);	   //����뷽ʽ
			#ifdef DEBUG_CMD
				printf("�������CMD_SEND_START\r\n");
			#endif
			break;

		case CMD_SEND_STOP:			//ֹͣ����ʵʱ����
			cmd_send_stop();
			#ifdef DEBUG_CMD
				printf("�������CMD_SEND_STOP\r\n");
			#endif
			break;

		case CMD_SET_TARE:			//�������
			cmd_set_tare(g_cmdMutual.pcmLssued.tare_ch,0,0);	//������豸��ȡ
			#ifdef DEBUG_CMD
				printf("�������CMD_SET_TARE\r\n");
			#endif
			break;

		case CMD_CTRL_MODE:			//����ģʽ
			cmd_ctrl_mode(g_cmdMutual.pcmLssued.ctrl_mode);
			#ifdef DEBUG_CMD
				printf("�������CMD_CTRL_MODE\r\n");
			#endif
			break;

		case CMD_PROTECT:			//�豸����ʹ��
			cmd_protect(g_cmdMutual.pcmLssued.bit_clr,g_cmdMutual.pcmLssued.bit_set,g_cmdMutual.pcmLssued.bit_rev);
			#ifdef DEBUG_CMD
				printf("�������CMD_PROTECT\r\n");
			#endif
			break;

		case CMD_STOP:				//ǿ��ֹͣ
			cmd_stop();
			#ifdef DEBUG_CMD
				printf("�������CMD_STOP\r\n");
			#endif
			break;

		case CMD_STEP_NORM_V:		//�·����鲽��(ֵ��ʽ)
			cmd_step_norm_v(g_cmdMutual.pcmLssued.step);
			#ifdef DEBUG_CMD
				printf("�������CMD_STEP_NORM_V\r\n");
			#endif
			break;
		
		case CMD_OPEN_LOOP:			//�·�����������
			cmd_open_loop(g_cmdMutual.pcmLssued.open);
			#ifdef DEBUG_CMD
				printf("�������CMD_OPEN_LOOP\r\n");
			#endif
			break;

		 case CMD_SWITCH:			//������
			cmd_switch(g_cmdMutual.pcmLssued.bit_clr,g_cmdMutual.pcmLssued.bit_set,g_cmdMutual.pcmLssued.bit_rev);
			#ifdef DEBUG_CMD
				printf("�������CMD_SWITCH\r\n");
			#endif
			break;
		 
		 case CMD_SET_DEVICE_ID:	//�����豸ID
			cmd_set_device_id(0x05D8FF31); 
			#ifdef DEBUG_CMD
				printf("�������CMD_SET_DEVICE_ID\r\n");
			#endif
			break;
		 
		 case CMD_GET_DEVICE_ID:	//��ȡ�豸ID
			cmd_general(CMD_GET_DEVICE_ID);
			#ifdef DEBUG_CMD
				printf("�������CMD_GET_DEVICE_ID\r\n");
			#endif
			break;

		default:
			#ifdef DEBUG_CMD
				printf("��������δ����\r\n");
			#endif
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : CmdReceiveBody
 * Description    : ���������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CmdReceiveBody( void ) 
{
	uint16_t rxCmd = 0;
	
	rxCmd = uart_get_cmd();
			
	switch ( rxCmd )
	{
		case CMD_DEVICE_VERSION:	//PCB��Ϣ���̼��汾
			cmd_device_version_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_DEVICE_VERSION\r\n\r\n");
			#endif
			break;		
	
		case CMD_RESET:				//��������������								 
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_RESET\r\n\r\n");
			#endif
			break;
		
		case CMD_PRIVATE_ACCESS:	//����/�˳���Ȩ����״̬
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_PRIVATE_ACCESS\r\n\r\n");
			#endif
			break; 
		
		case CMD_GET_ERROR:			//��ȡ����
			cmd_get_error_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_GET_ERROR\r\n\r\n");
			#endif					
			break; 

		case CMD_CLEAR_ERROR:		//�������
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_CLEAR_ERROR\r\n\r\n");
			#endif	
			break;

		case CMD_READ_PRM:			//��ȡ����	
			#ifdef DEBUG_CMD
				printf("�������CMD_READ_PRM\r\n\r\n");
			#endif
			break; 
		
		case CMD_WRITE_PRM:			//���ò���
			#ifdef DEBUG_CMD
				printf("�������CMD_WRITE_PRM\r\n\r\n");
			#endif	
			break;

		case CMD_READ_PRV:			//��ȡ��Ȩ����	
			#ifdef DEBUG_CMD
				printf("�������CMD_READ_PRV\r\n\r\n");
			#endif
			break; 
		
		case CMD_WRITE_PRV:			//������Ȩ����
			#ifdef DEBUG_CMD
				printf("�������CMD_WRITE_PRV\r\n\r\n");
			#endif
			break; 

		case CMD_BACKUP_PRM:		//���ݲ���
			#ifdef DEBUG_CMD
				printf("�������CMD_BACKUP_PRM\r\n\r\n");
			#endif
			break;  					//��λ���Լ�����

		case CMD_RESTORE_PRM:		//��ԭ����
			#ifdef DEBUG_CMD
				printf("�������CMD_RESTORE_PRM\r\n\r\n");
			#endif
			break;						//��λ���Լ����� 

		case CMD_SIGN_OFF:			//�Ͽ�����
			#ifdef DEBUG_CMD
				printf("�������CMD_SIGN_OFF\r\n\r\n");
			#endif
			break;

		case CMD_KEEP_ALIVE:		//�������
			#ifdef DEBUG_CMD
				printf("�������CMD_KEEP_ALIVE\r\n\r\n");
			#endif
			break;

		case CMD_SEND_START:		//��ʼ����ʵʱ����
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_SEND_START\r\n\r\n");
			#endif
			break;

		case CMD_SEND_STOP:		//ֹͣ����ʵʱ����
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_SEND_STOP\r\n\r\n");
			#endif
			break;

		case CMD_SET_TARE:			//�������
			cmd_set_tare_reply(uart_get_body(),&g_cmdMutual.prmReply);
			
			smpl_tab_zero_set(g_cmdMutual.pcmLssued.tare_ch,g_cmdMutual.prmReply.tare_code);	
			prm_save();
			#ifdef DEBUG_CMD
				printf("�������CMD_SET_TARE\r\n\r\n");
			#endif
			break;

	   case CMD_CTRL_MODE:			//����ģʽ
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);

			#ifdef DEBUG_CMD
				printf("�������CMD_CTRL_MODE\r\n\r\n");
			#endif
			break;

		case CMD_PROTECT:			//�豸����ʹ��
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			
			#ifdef DEBUG_CMD
				printf("�������CMD_PROTECT\r\n\r\n");
			#endif
			break;

		case CMD_STOP:				//ǿ��ֹͣ
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_STOP\r\n\r\n");
			#endif
			break;

		case CMD_STEP_NORM_V:		//�·����鲽��(ֵ��ʽ)
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_STEP_NORM_V\r\n\r\n");
			#endif					
			break;

		case CMD_OPEN_LOOP:			//�·�����������
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_OPEN_LOOP\r\n\r\n");
			#endif
			break;

		case CMD_SWITCH:			//������
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_SWITCH\r\n\r\n");
			#endif
			break;
			
		case CMD_SET_DEVICE_ID:	//�����豸ID
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_SET_DEVICE_ID\r\n\r\n");
			#endif
			break;
	 
		case CMD_GET_DEVICE_ID:	//��ȡ�豸ID
			cmd_get_device_id_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("�������CMD_GET_DEVICE_ID\r\n\r\n");
			#endif
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : CmdMutualBody
 * Description    : �������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static CMD_STATUS_TypeDef CmdMutualBody( void ) 
{	
	const uint8_t MAX_CMD_LUSSED_CNT = 3;

	switch ( g_cmdMutual.cmdStatus )
	{
		case CMD_IDLE:
			break;
		
		case CMD_BUSY:
			break;
		
		case CMD_UNLINK:
			break;
		
		case CMD_TX:
			g_cmdMutual.cmdStatus = CMD_RX;
	
			CmdSendBody();
		
			bsp_StartTimer(CMD_MUTUAL_TIMING,50);
			
			g_cmdMutual.pcmLssued.index = index_get(INDEX_H_ASW,COM_ADD_UART);
			break;
		
		case CMD_RX:		
			if ( bsp_CheckTimer(CMD_MUTUAL_TIMING) )
			{
				g_cmdMutual.cmdStatus = CMD_TX;
	
				g_cmdMutual.sendCnt++;
				if (g_cmdMutual.sendCnt >= MAX_CMD_LUSSED_CNT)
				{
					g_cmdMutual.cmdStatus = CMD_UNLINK;
				}
				
				#ifdef DEBUG_CMD
					printf("�������ʱ��\r\n\r\n");
				#endif			
				break;
			}
			
			if (g_cmdMutual.isRxReply == NO)
			{
				break;
			}
			
			g_cmdMutual.prmReply.index = uart_get_index();
				
			if (g_cmdMutual.pcmLssued.index != g_cmdMutual.prmReply.index)
			{
				g_cmdMutual.cmdStatus = CMD_TX;
				
				#ifdef DEBUG_CMD
					printf("����ֵ��һ�£�\r\n\r\n");
				#endif
				break;
			}
			
			g_cmdMutual.cmdStatus = CMD_IDLE;
			
			CmdReceiveBody();
			break;
	}
	
	return g_cmdMutual.cmdStatus;
}	

/*------------------------------------------------------------
 * Function Name  : GetReadWriteOutput
 * Description    : ��ȡ�ɶ�д���
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint16_t GetReadWriteOutput( void )
{
	return g_ioStatus.st_sw_rw;
}

/*------------------------------------------------------------
 * Function Name  : sample_cycle
 * Description    : ����ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void sample_cycle( void )
{	
	memcpy(&g_sampleCycle,(void *)uart_get_body(),uart_get_size());
	
	/* IO״̬ */
	g_ioStatus.st_input = g_sampleCycle.input;
	g_ioStatus.st_sw_rol = g_sampleCycle.sw_rol;
	g_ioStatus.st_sw_rw = g_sampleCycle.sw_rw;
	
	#ifdef REAL_TIME_DATA_FRAM
		printf("%x\r\n",g_ioStatus.st_sw_rw);
	#endif
	
	/* �����ֵ */
	g_sampleCode[SMPL_KY_NUM] = g_sampleCycle.smpl[SMPL_KY_NUM];
	g_sampleCode[SMPL_KZ_NUM] = g_sampleCycle.smpl[SMPL_KZ_NUM];
	
	/* �����ֵ */
	g_sampleForce[SMPL_KY_NUM] = smpl_cal(g_sampleCode[SMPL_KY_NUM],SMPL_KY_NUM);
	g_sampleForce[SMPL_KZ_NUM] = smpl_cal(g_sampleCode[SMPL_KZ_NUM],SMPL_KZ_NUM);
	
	#ifdef DEBUG_FORCE_KY
		printf("FORCE_KY��%f\r\n",g_sampleForce[SMPL_KY_NUM]);
	#endif
	
	#ifdef DEBUG_FORCE_KZ
		printf("FORCE_KZ��%f\r\n",g_sampleForce[SMPL_KZ_NUM]);
	#endif
	
	/* ����ٶ� */
	spd_cal_cycle();
}

/*------------------------------------------------------------
 * Function Name  : spd_cal
 * Description    : ����ĳһͨ���ٶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float spd_cal(uint8_t chn, uint16_t gap)
{
	uint16_t index1 = 0,index2 = 0;
	int32_t time1 = 0,time2 = 0;
	float val1 = 0,val2 = 0,temp = 0;	
	
	index1=g_speedCalculate.index;
	
	index2=g_speedCalculate.index+SPD_CAL_NUM-gap;
	if(index2>=SPD_CAL_NUM)
	index2-=SPD_CAL_NUM; 
	
	val1=g_speedCalculate.val[chn][index1];
	val2=g_speedCalculate.val[chn][index2];
	
	time1= g_speedCalculate.time[index1];
	time2= g_speedCalculate.time[index2]; 
	
	if(time1==time2)
	{
		temp=0;	
	}
	else
	{
		temp= (val1-val2)*1000/(time1-time2);
	}
	
	return temp;
}

/*------------------------------------------------------------
 * Function Name  : spd_cal_cycle
 * Description    : �ٶȼ���ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void spd_cal_cycle(void)
{ 
	g_speedCalculate.index++;
	
	g_speedCalculate.index %= SPD_CAL_NUM;
 		
	g_speedCalculate.time[g_speedCalculate.index] = g_sampleCycle.time_base;
	
	g_speedCalculate.val[SMPL_KY_NUM][g_speedCalculate.index]=get_smpl_force(SMPL_KY_NUM);
	g_speedCalculate.val[SMPL_KZ_NUM][g_speedCalculate.index]=get_smpl_force(SMPL_KZ_NUM);
	
	g_speedCalculate.speed[SMPL_KY_NUM]=spd_cal(SMPL_KY_NUM,50);
	g_speedCalculate.speed[SMPL_KZ_NUM]=spd_cal(SMPL_KZ_NUM,50);
	
	#ifdef DEBUG_SPEED
		printf("KY_Speed��%f\r\n",g_speedCalculate.speed[SMPL_KY_NUM]);
		printf("KZ_Speed��%f\r\n",g_speedCalculate.speed[SMPL_KZ_NUM]);
	#endif
}

/*------------------------------------------------------------
 * Function Name  : get_smpl_spd
 * Description    : ��ȡָ��ͨ���ٶ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float get_smpl_spd(uint8_t chn)
{
	return g_speedCalculate.speed[chn];
}

/*------------------------------------------------------------
 * Function Name  : get_smpl_force
 * Description    : ��ȡ��������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float get_smpl_force(uint8_t chn)
{
	return g_sampleForce[chn];
}

/*------------------------------------------------------------
 * Function Name  : get_smpl_code
 * Description    : ��ȡ��������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
float get_smpl_code(uint8_t chn)
{
	return g_sampleCode[chn];
}

/*------------------------------------------------------------
 * Function Name  : Uart_GetPackageCycle
 * Description    : ���ڻ�ȡ���ݰ�ѭ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
BoolStatus Uart_GetPackageCycle( void )
{
	uint8_t rxData = 0;
	static uint8_t rxStatus = ST_START;
	static uint8_t rxDataSize = 0;
	static uint8_t curPosIndex = 0;
	
	while ( ComGet(COM1,&rxData))
	{
		switch ( rxStatus )
		{
			case ST_START:
			{
				if (rxData == MESSAGE_START)
				{
					g_uartPackageRxBuff[0] = MESSAGE_START;
					rxStatus = ST_GET_ADDR;
				}
				break;
			}
			
			case ST_GET_ADDR:
			{
				g_uartPackageRxBuff[1] = rxData;
				rxStatus = ST_GET_INDEX;
				break;
			}
			
			case ST_GET_INDEX:
			{
				g_uartPackageRxBuff[2] = rxData;
				rxStatus = ST_MSG_SIZE;
				break;
			}
			
			case ST_MSG_SIZE:
			{
				rxDataSize = rxData;
				g_uartPackageRxBuff[3] = rxDataSize;
				rxStatus = ST_GET_TOKEN;
				break;
			}
			
			case ST_GET_TOKEN:
			{
				if (rxData == TOKEN)
				{
					g_uartPackageRxBuff[4] = TOKEN;
					curPosIndex = 5;
					rxStatus = ST_GET_DATA;
				}
				else
				{
					rxStatus = ST_START;
				}
				break;
			}
			
			case ST_GET_DATA:
			{
				g_uartPackageRxBuff[curPosIndex++] = rxData;
				if(curPosIndex - 5 == rxDataSize)
				{ 
					rxStatus = ST_GET_CHECK_L;
				}
				break;
			}
			
			case ST_GET_CHECK_L:
			{
				g_uartPackageRxBuff[curPosIndex++] = rxData;		
				rxStatus = ST_GET_CHECK_H;
				break;
			}
			
			case ST_GET_CHECK_H:
			{
				g_uartPackageRxBuff[curPosIndex++] = rxData;		
				rxStatus = ST_GET_END;
				break;
			}
			
			case ST_GET_END:
			{
				g_uartPackageRxBuff[curPosIndex++] = rxData;	
				rxStatus=ST_START;				
				if (rxData == MESSAGE_END)
				{
					return YES;
				}				
				break;
			}
			default:
			{
				rxStatus = ST_START;
				break;
			}
		}
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : uart_get_frame_addr
 * Description    : ��ȡ����֡��ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void *uart_get_frame_addr(void)
{	
	return ((void *)g_uartPackageRxBuff);
}

/*------------------------------------------------------------
 * Function Name  : uart_get_cmd
 * Description    : ��ȡ���ݰ�����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint16_t uart_get_cmd( void )
{
	uint16_t *cmd;
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart_get_frame_addr();
	
	cmd=(uint16_t *)(void *)(&(p_uart_head->data)); 
	
	return (*cmd);
} 

/*------------------------------------------------------------
 * Function Name  : uart_get_body
 * Description    : ��ȡ�����������ָ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t *uart_get_body(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart_get_frame_addr();
	
	return &(p_uart_head->data);
} 


/*------------------------------------------------------------
 * Function Name  : uart_get_index
 * Description    : ��ȡ������������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
uint8_t uart_get_index(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart_get_frame_addr();
	
	return p_uart_head->index; 
}

/*------------------------------------------------------------
 * Function Name  : uart_get_size
 * Description    : ��ȡ������Ϣ���С
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
uint8_t uart_get_size(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart_get_frame_addr();
	
	return p_uart_head->size;
}

/*------------------------------------------------------------
 * Function Name  : uart_get_size
 * Description    : ��ȡ������Ϣ��ADDR
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/ 
uint8_t uart_get_addr(void)
{
	UART_HEAD_Typdef *p_uart_head=(UART_HEAD_Typdef *)(void *)uart_get_frame_addr();
	
	return p_uart_head->addr;
}

/*------------------------------------------------------------
 * Function Name  : uart_check
 * Description    : ������ݰ��Ƿ���ȷ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
ErrorStatus uart_check(void)
{
 	uint16_t *p_check = NULL;
 	uint16_t check_cal = 0;
	uint8_t msg_size = 0;
	uint8_t *rx_frame_addr = uart_get_frame_addr();
	
	msg_size=uart_get_size();
	
	p_check=(uint16_t *)(rx_frame_addr+sizeof(UART_HEAD_Typdef)-1+msg_size);
	
	check_cal=get_crc16(0,(uint8_t *)rx_frame_addr,sizeof(UART_HEAD_Typdef)-1+msg_size);
	
	if(check_cal==*p_check)
	{
		return 	SUCCESS;
	}
	else
	{
		return 	ERROR;	
	}
}

/*------------------------------------------------------------
 * Function Name  : uart_send_package
 * Description    : ���ڷ������ݰ�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void uart_send_package(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr)
{
	uint8_t head[5];
	uint16_t crc16=0;
	uint8_t tail = MESSAGE_END;
	
	//����ͷ5���ֽ�
	head[0]=MESSAGE_START;
	head[1]=addr;						//��ַ�̶�Ϊ0x01
	head[2]=index;
	head[3]=msg_size;
	head[4]=TOKEN;
	
	ComSend(COM1,head,sizeof(head));
	ComSend(COM1,ptr,msg_size);
	crc16=get_crc16(0,head,sizeof(head));
	crc16=get_crc16(crc16,ptr,msg_size);		//����CRC16ֵ
	ComSend(COM1,(uint8_t *)&crc16,2);
	ComSend(COM1,&tail,1);
}

/*------------------------------------------------------------
 * Function Name  : GetLinkStatus
 * Description    : ��ȡ����״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
LINK_STATUS_TypeDef GetLinkStatus( void )
{
	return g_linkMutual.linkStatus;
}

/*------------------------------------------------------------
 * Function Name  : isAllowLssued
 * Description    : �Ƿ������·�
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static BoolStatus isAllowLssued( void )
{
	LINK_STATUS_TypeDef linkStatus = GetLinkStatus();
	
	if (linkStatus == LINK_IDLE)
	{
		return YES;
	}
	
	return NO;
}

/*------------------------------------------------------------
 * Function Name  : comm_cycle
 * Description    : ͨѶѭ����(��Ҫѭ�����ã�ֱ�����ͳɹ�������ʧ��)
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
CMD_STATUS_TypeDef comm_cycle( void )
{
	uint16_t rxCmd = 0;
	
	g_cmdMutual.isRxReply = NO;
	
	while ( Uart_GetPackageCycle() == YES )
	{
		if (ERROR == uart_check() )
		{
			#ifdef DEBUG_CMD
				printf("CRCУ�����\r\n\r\n");
			#endif
			continue;
		}
	
		rxCmd = uart_get_cmd();
		
		if ( (rxCmd==RTM_SAMPLE_VALUE) || (rxCmd==RTM_SAMPLE_CODE) || (rxCmd==RTM_SAMPLE_RCODE) )	
		{
			g_linkMutual.keepLiveCount = 0;
			
			sample_cycle();
		}
		else		
		{
			g_cmdMutual.isRxReply = YES;
			
			SetCommucationLampStart();
		}
	}
	
	CmdMutualBody();
	
	return g_cmdMutual.cmdStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetKeepLiveCount
 * Description    : ��ȡ����ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint32_t GetKeepLiveCount( void )
{
	return g_linkMutual.keepLiveCount;
}

/*------------------------------------------------------------
 * Function Name  : SetKeepLiveCount
 * Description    : ���ñ���ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void SetKeepLiveCount( uint32_t keepLiveCount )
{
	g_linkMutual.keepLiveCount = keepLiveCount;
}

/*------------------------------------------------------------
 * Function Name  : GetPrmReplyStatus
 * Description    : ��ȡ��λ���ظ�״̬
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetPrmReplyStatus( void )
{
	return g_cmdMutual.prmReply.status;
}

/*------------------------------------------------------------
 * Function Name  : link_init
 * Description    : ������ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void link_init( void )
{
	g_linkMutual.isRxReply = NO;
	g_linkMutual.cmdStatus = CMD_TX;
	g_linkMutual.linkStatus = LINK_SEND_STOP;
	g_linkMutual.sendIndex = 0;
	g_linkMutual.sendCnt = 0;
	g_linkMutual.process = 0;
	g_linkMutual.mdyCnt = 0;
	
	#ifdef DEBUG_LINK
		printf("������ʼ��\r\n");
	#endif
}

/*------------------------------------------------------------
 * Function Name  : LinkCmdSendBody
 * Description    : �����������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkCmdSendBody( void )
{
	g_linkMutual.cmdStatus = CMD_RX;
	
	switch ( g_linkMutual.linkStatus )
	{
		case LINK_UNLINK:			
			break;
		
		case LINK_SEND_STOP:
			cmd_send_stop();
		
			#ifdef DEBUG_LINK
				printf("���� LINK_SEND_STOP\r\n");
			#endif
			break;
		
		case LINK_SIGN_ON:
			cmd_cmd_sign_on();
		
			#ifdef DEBUG_LINK
				printf("���� LINK_SIGN_ON\r\n");
			#endif
			break;
		
		case LINK_CHECK_PRM:
			if ( g_WritePrm.len )
			{
				if (g_WritePrm.len > PRM_BUFF_CHECK)
				{
					cmd_read_prm(g_WritePrm.targetAddr,PRM_BUFF_CHECK);
				}
				else
				{
					cmd_read_prm(g_WritePrm.targetAddr,g_WritePrm.len);	//���һ�ζ�ȡ
				}

				#ifdef DEBUG_LINK
					printf("���� LINK_CHECK_PRM\r\n");	
				#endif										
			}
			else		//�����·����
			{
				g_linkMutual.linkStatus = LINK_SEND_START;
				g_linkMutual.cmdStatus = CMD_TX;	
			}
			break;
		
		case LINK_MDY_PRM:
			if (g_WritePrm.len > PRM_BUFF_CHECK)
			{
				cmd_write_prm(g_WritePrm.targetAddr,PRM_BUFF_CHECK,g_WritePrm.pSourceAddr);
			}
			else	
			{
				cmd_write_prm(g_WritePrm.targetAddr,g_WritePrm.len,g_WritePrm.pSourceAddr);
			}

			#ifdef DEBUG_LINK
				printf("���� LINK_MDY_PRM\r\n");	
			#endif	
			break;
		
		case LINK_SEND_START:
			cmd_send_start(SEND_RCODE);
			
			#ifdef DEBUG_LINK
				printf("���� LINK_SEND_START\r\n");		
			#endif
			break;
		
		case LINK_SUCCESS:
			g_linkMutual.linkStatus = LINK_IDLE;
			SetLampBit(LAMP_BOARD_COM);
			break;
		
		case LINK_IDLE:			
			break;
	}
	
	g_linkMutual.sendIndex = index_get(INDEX_H_ASW,COM_ADD_UART);
	
	bsp_StartTimer(LINK_MUTUAL_TIMING,50);
}

/*------------------------------------------------------------
 * Function Name  : WritePrmParameterInit
 * Description    : д��λ��������ʼ��
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void WritePrmParameterInit( void )
{
	g_WritePrm.pSourceAddr = get_prm();
	g_WritePrm.targetAddr = 0;
	g_WritePrm.len = PRM_MEM_SIZE;
}				

/*------------------------------------------------------------
 * Function Name  : LinkCmdReceiveBody
 * Description    : �������������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void LinkCmdReceiveBody( void )
{
	const uint8_t MAX_LINK_CMD_LUSSED_CNT = 3;
	const uint8_t LSSUED_PARA_TRY_CNT = 3;
	RPL_READ_PRM_TypeDef *pRplReadPrm = NULL;
	TestStatus compStatus;

	if ( bsp_CheckTimer(LINK_MUTUAL_TIMING) )
	{
		g_linkMutual.cmdStatus = CMD_TX;

		g_linkMutual.sendCnt++;
		if (g_linkMutual.sendCnt >= MAX_LINK_CMD_LUSSED_CNT)
		{
			g_linkMutual.cmdStatus = CMD_UNLINK;
			g_linkMutual.linkStatus = LINK_UNLINK;
		}
		
		#ifdef DEBUG_LINK
			printf("�������ʱ��\r\n\r\n");
		#endif	
		
		return;
	}
	
	if (g_linkMutual.isRxReply == NO)
	{
		return;
	}
	
	g_linkMutual.prmReply.index = uart_get_index();
		
	if (g_linkMutual.sendIndex != g_linkMutual.prmReply.index)
	{
		g_linkMutual.cmdStatus = CMD_TX;
		
		#ifdef DEBUG_LINK
			printf("����ֵ��һ�£�\r\n\r\n");
		#endif
		
		return;
	}
	
	switch ( g_linkMutual.linkStatus )
	{
		case LINK_UNLINK:			
			break;
		
		case LINK_SEND_STOP:
			reply_empty(uart_get_body(),&g_linkMutual.prmReply);
			
			if (g_linkMutual.prmReply.status == STATUS_OK)
			{
				g_linkMutual.linkStatus = LINK_SIGN_ON;
				
				#ifdef DEBUG_LINK
					printf("���� LINK_SEND_STOP\r\n\r\n");	
				#endif
			}
			else
			{
				g_linkMutual.linkStatus = LINK_UNLINK;
				
				#ifdef DEBUG_LINK
					printf("ʧ�ܽ��� LINK_SEND_STOP\r\n\r\n");	
				#endif
			}
			break;
		
		case LINK_SIGN_ON:
			reply_empty(uart_get_body(),&g_linkMutual.prmReply);
		
			if (g_linkMutual.prmReply.status == STATUS_OK)
			{
				g_linkMutual.linkStatus = LINK_CHECK_PRM;
				
				WritePrmParameterInit();
				
				#ifdef DEBUG_LINK
					printf("���� LINK_SIGN_ON\r\n\r\n");	
				#endif
			}
			else
			{
				g_linkMutual.linkStatus = LINK_UNLINK;
				
				#ifdef DEBUG_LINK
					printf("ʧ�ܽ��� LINK_SIGN_ON\r\n\r\n");	
				#endif
			}
			break;
		
		case LINK_CHECK_PRM:
			pRplReadPrm = (RPL_READ_PRM_TypeDef *)uart_get_body();
			
			if (g_WritePrm.len > PRM_BUFF_CHECK)
			{
				compStatus = buffcmp(g_WritePrm.pSourceAddr,&(pRplReadPrm->start_byte),PRM_BUFF_CHECK);
				if (PASSED == compStatus)
				{
					g_WritePrm.pSourceAddr 	+= PRM_BUFF_CHECK;	//������ʼ��ַ
					g_WritePrm.targetAddr 	+= PRM_BUFF_CHECK;	//��д���ַ	
					g_WritePrm.len		 	-= PRM_BUFF_CHECK;	//ʣ�೤��
					g_linkMutual.process 	= (float)(PRM_MEM_SIZE - g_WritePrm.len) / PRM_MEM_SIZE * 100;
				}
				else
				{
					g_linkMutual.linkStatus = LINK_MDY_PRM;
				}
			}
			else		//���һ�αȽ�
			{
				compStatus = buffcmp(g_WritePrm.pSourceAddr,&(pRplReadPrm->start_byte),g_WritePrm.len);
				if (PASSED == compStatus)
				{
					g_WritePrm.len = 0;
					g_linkMutual.process = 100;
				}
				else
				{
					g_linkMutual.linkStatus = LINK_MDY_PRM;
				}
			}
			
			#ifdef DEBUG_LINK
				printf("���� LINK_CHECK_PRM\r\n\r\n");	
			#endif
			break;
		
		case LINK_MDY_PRM:
			pRplReadPrm = (RPL_READ_PRM_TypeDef *)uart_get_body();
					
			if (STATUS_OK == pRplReadPrm->status)
			{
				g_linkMutual.linkStatus = LINK_CHECK_PRM;
				g_linkMutual.mdyCnt = 0;
			}
			else
			{
				g_linkMutual.mdyCnt++;
				if (g_linkMutual.mdyCnt >= LSSUED_PARA_TRY_CNT)
				{
					g_linkMutual.mdyCnt = 0;
					g_linkMutual.linkStatus = LINK_UNLINK;
				}
			}
			
			#ifdef DEBUG_LINK
				printf("���� LINK_MDY_PRM\r\n\r\n");	
			#endif
			break;
		
		case LINK_SEND_START:  
			g_linkMutual.linkStatus = LINK_SUCCESS;
			
			#ifdef DEBUG_LINK
				printf("�����ɹ�\r\n\r\n");
			#endif
			break;
		
		case LINK_SUCCESS:
			
			break;
		
		case LINK_IDLE:
			
			break;
	}
	
	g_linkMutual.cmdStatus = CMD_TX;
}

/*------------------------------------------------------------
 * Function Name  : GetLinkProcess
 * Description    : ��������
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
uint8_t GetLinkProcess( void )
{
	return g_linkMutual.process;
}

/*------------------------------------------------------------
 * Function Name  : link_cycle
 * Description    : ����ѭ����
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
LINK_STATUS_TypeDef link_cycle( void )
{
	uint16_t rxCmd = 0;
	
	g_linkMutual.isRxReply = NO;
	
	while ( Uart_GetPackageCycle() == YES )
	{
		rxCmd = uart_get_cmd();
		
		if ( (rxCmd==RTM_SAMPLE_VALUE) || (rxCmd==RTM_SAMPLE_CODE) || (rxCmd==RTM_SAMPLE_RCODE) )	
		{
			continue;
		}
		
		if (ERROR == uart_check() )
		{
			#ifdef DEBUG_LINK
				printf("CRCУ�����\r\n\r\n");
			#endif
			continue;
		}

		g_linkMutual.isRxReply = YES;
	}
	
	switch ( g_linkMutual.cmdStatus )
	{
		case CMD_TX:
			LinkCmdSendBody();
			break;
		
		case CMD_RX:
			LinkCmdReceiveBody();
			break;
		default:
			break;
	}
	
	return g_linkMutual.linkStatus;
}

/*------------------------------------------------------------
 * Function Name  : GetPrmReplyAddr
 * Description    : ��ȡ��λ���ظ���Ϣ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
PRM_REPLY_TypeDef *GetPrmReplyAddr( void )
{
	return &g_cmdMutual.prmReply;
}

/*------------------------------------------------------------
 * Function Name  : GetSampleAddr
 * Description    : ��ȡ������ַ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
SAMPLE_C_TypeDef *GetSampleAddr( void )
{
	return &g_sampleCycle;
}

/*------------------------------------------------------------
 * Function Name  : GetSammpleCode
 * Description    : ��ȡ������ֵ
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
int32_t GetSammpleCode( uint8_t channel )
{
	return g_sampleCycle.smpl[channel];
}

/*------------------------------------------------------------
 * Function Name  : GetPrmWarnStatus
 * Description    : �����λ�����棨b_err:0--�޾��棩
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
FlagStatus GetPrmWarnStatus( void )
{
	if ( g_sampleCycle.status.b_err )
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

