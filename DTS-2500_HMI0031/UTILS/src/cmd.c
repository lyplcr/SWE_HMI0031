/**
  ******************************************************************************
  * @file    cmd.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-10-25
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   命令相关处理程序
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
	static Spd_Cal_Typedef	g_speedCalculate; 	//速度计算
	static SAMPLE_C_TypeDef g_sampleCycle;		//采样循环体
	static float g_sampleForce[SMPL_NUM];		//传感器力值
	static int32_t g_sampleCode[SMPL_NUM];		//传感器码值
	static uint8_t g_uartPackageRxBuff[UART_PACKAGE_BUFF_SIZE];	//串口接收数据包缓存
	static CMD_MUTUAL_TypeDef g_cmdMutual;		//命令交互
	static LINK_MUTUAL_TypeDef g_linkMutual;	//联机交互
	static WRITE_PRM_PARA_TypeDef g_WritePrm;	//写下位机参数区域
	static COMM_IO_STATUS_TypeDef g_ioStatus;
#pragma arm section

/* Private function prototypes -----------------------------------------------*/
static void InitSendCmd( void );
static CMD_STATUS_TypeDef CmdMutualBody( void );
static void uart_send_package(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr);
static BoolStatus isAllowLssued( void );



/* Private functions ---------------------------------------------------------*/
// --------------------------- 我是分割线 ----------------------------------- //

/*
	 上位机下发数据包(包含实际待发送的数据包) 
*/  
/**********************************************************************
functionName:void cmd_general(uint16_t cmd)
description:通用命令，适合所有内容为空的指令
**********************************************************************/  
void cmd_general(uint16_t cmd)
{
	uint8_t index;
	index=next_index(INDEX_H_ASW,COM_ADD_UART); 
	uart_send_package(COM_ADD_UART,index,sizeof(cmd),(uint8_t *)&cmd);
} 
  
/**********************************************************************
functionName:void cmd_device_version( void )
description:CMD_DEVICE_VERSION / 硬件、固件版本 
**********************************************************************/
void cmd_device_version( void )
{
	cmd_general(CMD_DEVICE_VERSION);	
}

/**********************************************************************
functionName:void cmd_reset( void )
description:CMD_RESET / 软复位
**********************************************************************/
void cmd_reset( void )
{
	cmd_general(CMD_RESET);	
}

/**********************************************************************
functionName:void cmd_private_access(uint8_t acess)
description:发送CMD_PRIVATE_ACCESS
acess=0 退出特权状态  acess=1 进入特权状态
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
description:CMD_GET_ERROR / 获取设备错误
**********************************************************************/
void cmd_get_error( void )
{
	cmd_general(CMD_GET_ERROR);	
}

/**********************************************************************
functionName:void cmd_clear_error( void )
description:CMD_CLEAR_ERROR / 清除设备错误 
**********************************************************************/
void cmd_clear_error( void )
{
	cmd_general(CMD_CLEAR_ERROR);	
}

/**********************************************************************
functionName:void cmd_read_prm(uint8_t index,uint16_t start,uint16_t len)
description:发送CMD_READ_PRM / 读取参数
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
description:发送CMD_WRITE_PRM / 设置参数
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
description:发送CMD_READ_PRV / 读取特权参数
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
description:发送CMD_WRITE_PRV / 设置特权参数
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
description:CMD_SIGN_OFF / 断开连接
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
0--绝对码; 1--示值; 2--相对码
description:发送CMD_SEND_START / 开始发送实时数据
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
description:CMD_SEND_STOP / 停止发送实时数据
**********************************************************************/
void cmd_send_stop( void )
{
	cmd_general(CMD_SEND_STOP);	

} 

/**********************************************************************
functionName:void cmd_cmd_sign_on( void )
description:CMD_SIGN_ON / 握手
**********************************************************************/
void cmd_cmd_sign_on( void )
{
	cmd_general(CMD_SIGN_ON);	
}

 
/**********************************************************************
functionName:void cmd_set_tare(uint8_t index,uint8_t chn,uint8_t src,int32_t code)
description:发送CMD_SET_TARE
chn:指的是通道号 0-5
src:0表示由设备自取，1表示由上位机设置，即code的值为零点
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
description:发送CMD_CTRL_MODE
bit0:0表示手动，1表示自动
bit1:0表示抗压，1表示抗折
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
description:发送CMD_PROTECT / 设备保护
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
description:发送CMD_STOP / 强制中断
**********************************************************************/
void cmd_stop( void )
{
	cmd_general(CMD_STOP);	
}


/**********************************************************************
functionName:void cmd_step_norm_v(STEP_TypeDef step)
description:发送CMD_STEP_NORM_V
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
description:发送CMD_OPEN_LOOP / 开环控制量
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
description:发送CMD_SWITCH
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
description:发送CMD_SET_DEVICE_ID / 设置设备ID
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



// --------------------------- 我是分割线 ----------------------------------- //


/*
	下位机回复数据包 
*/

/**********************************************************************
functionName:void cmd_device_version_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:CMD_DEVICE_VERSION / 硬件、固件版本 
**********************************************************************/
void cmd_device_version_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_DEVICE_VERSION_TypeDef *pDevice_version;

	pDevice_version = (RPL_DEVICE_VERSION_TypeDef *)pData;

	memcpy(pRpl->pcb_info,pDevice_version->pcb_info,16);	//获取PCB版本
	pRpl->fw_ver = pDevice_version->fw_ver;					//获取固件版本
	pRpl->status = pDevice_version->status;			
}

/**********************************************************************
functionName:reply_empty
description:回复空
**********************************************************************/
void reply_empty( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RLP_NONE_TypeDef *pRplEmpty = (RLP_NONE_TypeDef *)pData;

	pRpl->status = pRplEmpty->status;			
}

/**********************************************************************
functionName:void cmd_get_error_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:CMD_GET_ERROR / 获取设备错误
**********************************************************************/
void cmd_get_error_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_ERR_CODE_TypeDef *pGet_error;
	
	pGet_error = (RPL_ERR_CODE_TypeDef *)pData;
	
	pRpl->err = pGet_error->error_code;			//获取错误代码	
	pRpl->status = pGet_error->status;	
}

/**********************************************************************
functionName:void cmd_set_tare_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
description:case CMD_SET_TARE:			//设置零点
**********************************************************************/
void cmd_set_tare_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RLP_SET_TARE_TypeDef *pSet_tare;
	
	pSet_tare = (RLP_SET_TARE_TypeDef *)pData;
	
	pRpl->tare_code = pSet_tare->zero_code;			//获取错误代码
	pRpl->status = pSet_tare->status;	
}


/**********************************************************************
functionName:void cmd_set_device_id( uint32_t id )
description:发送CMD_GET_DEVICE_ID / 设置设备ID
**********************************************************************/
void cmd_get_device_id_reply( uint8_t *pData, PRM_REPLY_TypeDef *pRpl )
{
	RPL_GET_ID_TypeDef *pGet_Id;
	
	pGet_Id = (RPL_GET_ID_TypeDef *)pData;
	
	pRpl->id = pGet_Id->id;
	pRpl->status = pGet_Id->status;	
}


// --------------------------- 我是分割线 ----------------------------------- //
/*
	通讯命令(负责封装下发数据包的命令、通过函数 comm_cycle() 发出) 
*/

/**********************************************************************
functionName:cmd_device_version_pkg
description:CMD_DEVICE_VERSION / 硬件、固件版本 
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
description:CMD_GET_ERROR / 获取设备错误 
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
description:CMD_CLEAR_ERROR / 清除设备错误 
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
description:发送CMD_READ_PRM / 读取参数 
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
description:发送CMD_WRITE_PRM / 设置参数 
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
type：发送实时数据类型
description:发送CMD_SEND_START / 开始发送实时数据 
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
type：发送实时数据类型
description:CMD_SEND_STOP / 停止发送实时数据
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
description:发送CMD_SET_TARE
chn:指的是通道号 0-5
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
description:发送CMD_CTRL_MODE
col_mode：控制模式
bit0:0表示手动，1表示自动
bit1:0表示抗压，1表示抗折
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
description:发送CMD_PROTECT / 设备保护
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
description:发送CMD_STOP / 强制中断
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
description:发送CMD_STEP_NORM_V	/ 下发试验步骤
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
description:发送CMD_OPEN_LOOP / 开环控制量
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
description:发送CMD_SWITCH / 开关量
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
description:发送CMD_GET_DEVICE_ID / 获取设备ID
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
description:发送CMD_SET_DEVICE_ID / 设置设备ID
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
 * Description    : 初始化发送命令
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
 * Description    : 命令发送体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void CmdSendBody( void ) 
{
	switch ( g_cmdMutual.pcmLssued.cmd )
	{
		case CMD_DEVICE_VERSION:	//PCB信息及固件版本
			cmd_device_version();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_DEVICE_VERSION\r\n");
			#endif
			break;
			
		case CMD_RESET:				//控制器重新启动
			cmd_reset();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_RESET\r\n");
			#endif
			break;    

		case CMD_PRIVATE_ACCESS:	//进入/退出特权访问状态
			cmd_private_access(g_cmdMutual.pcmLssued.acess);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_PRIVATE_ACCESS\r\n");
			#endif
			break;
			
		case CMD_GET_ERROR:			//获取错误
			cmd_get_error();	
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_GET_ERROR\r\n");
			#endif
			break;
		
		case CMD_CLEAR_ERROR:		//清除错误
			cmd_clear_error();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_CLEAR_ERROR\r\n");
			#endif

			break;
			
		case CMD_READ_PRM:			//读取参数
			cmd_read_prm(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_READ_PRM\r\n");
			#endif
			break;
			
		case CMD_WRITE_PRM:			//设置参数
			cmd_write_prm(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len,g_cmdMutual.pcmLssued.dat);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_WRITE_PRM\r\n");
			#endif
			break;
		
		case CMD_READ_PRV:			//读取特权参数
			cmd_read_prv(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_READ_PRV\r\n");
			#endif
			break;
			
		case CMD_WRITE_PRV:			//设置特权参数
			cmd_write_prv(g_cmdMutual.pcmLssued.addr,g_cmdMutual.pcmLssued.len,g_cmdMutual.pcmLssued.dat);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_WRITE_PRV\r\n");
			#endif
			break; 	
			
		case CMD_BACKUP_PRM:		//备份参数
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_BACKUP_PRM\r\n");
			#endif
			break;  				//上位机自己处理

		case CMD_RESTORE_PRM:		//还原参数
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_RESTORE_PRM\r\n");
			#endif
			break;					//上位机自己处理

		case CMD_SIGN_OFF:			//断开连接
			cmd_sign_off();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SIGN_OFF\r\n");
			#endif
			break;

		case CMD_KEEP_ALIVE:		//保活机制
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_KEEP_ALIVE\r\n");
			#endif
			break;					

		case CMD_SEND_START:		//开始发送实时数据
			cmd_send_start(g_cmdMutual.pcmLssued.send_type);	   //相对码方式
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SEND_START\r\n");
			#endif
			break;

		case CMD_SEND_STOP:			//停止发送实时数据
			cmd_send_stop();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SEND_STOP\r\n");
			#endif
			break;

		case CMD_SET_TARE:			//设置零点
			cmd_set_tare(g_cmdMutual.pcmLssued.tare_ch,0,0);	//零点由设备自取
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SET_TARE\r\n");
			#endif
			break;

		case CMD_CTRL_MODE:			//控制模式
			cmd_ctrl_mode(g_cmdMutual.pcmLssued.ctrl_mode);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_CTRL_MODE\r\n");
			#endif
			break;

		case CMD_PROTECT:			//设备保护使能
			cmd_protect(g_cmdMutual.pcmLssued.bit_clr,g_cmdMutual.pcmLssued.bit_set,g_cmdMutual.pcmLssued.bit_rev);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_PROTECT\r\n");
			#endif
			break;

		case CMD_STOP:				//强制停止
			cmd_stop();
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_STOP\r\n");
			#endif
			break;

		case CMD_STEP_NORM_V:		//下发试验步骤(值方式)
			cmd_step_norm_v(g_cmdMutual.pcmLssued.step);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_STEP_NORM_V\r\n");
			#endif
			break;
		
		case CMD_OPEN_LOOP:			//下发开环控制量
			cmd_open_loop(g_cmdMutual.pcmLssued.open);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_OPEN_LOOP\r\n");
			#endif
			break;

		 case CMD_SWITCH:			//开关量
			cmd_switch(g_cmdMutual.pcmLssued.bit_clr,g_cmdMutual.pcmLssued.bit_set,g_cmdMutual.pcmLssued.bit_rev);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SWITCH\r\n");
			#endif
			break;
		 
		 case CMD_SET_DEVICE_ID:	//设置设备ID
			cmd_set_device_id(0x05D8FF31); 
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_SET_DEVICE_ID\r\n");
			#endif
			break;
		 
		 case CMD_GET_DEVICE_ID:	//获取设备ID
			cmd_general(CMD_GET_DEVICE_ID);
			#ifdef DEBUG_CMD
				printf("发送命令：CMD_GET_DEVICE_ID\r\n");
			#endif
			break;

		default:
			#ifdef DEBUG_CMD
				printf("发送命令未定义\r\n");
			#endif
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : CmdReceiveBody
 * Description    : 命令接收体
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
		case CMD_DEVICE_VERSION:	//PCB信息及固件版本
			cmd_device_version_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_DEVICE_VERSION\r\n\r\n");
			#endif
			break;		
	
		case CMD_RESET:				//控制器重新启动								 
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_RESET\r\n\r\n");
			#endif
			break;
		
		case CMD_PRIVATE_ACCESS:	//进入/退出特权访问状态
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_PRIVATE_ACCESS\r\n\r\n");
			#endif
			break; 
		
		case CMD_GET_ERROR:			//获取错误
			cmd_get_error_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_GET_ERROR\r\n\r\n");
			#endif					
			break; 

		case CMD_CLEAR_ERROR:		//清除错误
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_CLEAR_ERROR\r\n\r\n");
			#endif	
			break;

		case CMD_READ_PRM:			//读取参数	
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_READ_PRM\r\n\r\n");
			#endif
			break; 
		
		case CMD_WRITE_PRM:			//设置参数
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_WRITE_PRM\r\n\r\n");
			#endif	
			break;

		case CMD_READ_PRV:			//读取特权参数	
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_READ_PRV\r\n\r\n");
			#endif
			break; 
		
		case CMD_WRITE_PRV:			//设置特权参数
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_WRITE_PRV\r\n\r\n");
			#endif
			break; 

		case CMD_BACKUP_PRM:		//备份参数
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_BACKUP_PRM\r\n\r\n");
			#endif
			break;  					//上位机自己处理

		case CMD_RESTORE_PRM:		//还原参数
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_RESTORE_PRM\r\n\r\n");
			#endif
			break;						//上位机自己处理 

		case CMD_SIGN_OFF:			//断开连接
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SIGN_OFF\r\n\r\n");
			#endif
			break;

		case CMD_KEEP_ALIVE:		//保活机制
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_KEEP_ALIVE\r\n\r\n");
			#endif
			break;

		case CMD_SEND_START:		//开始发送实时数据
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SEND_START\r\n\r\n");
			#endif
			break;

		case CMD_SEND_STOP:		//停止发送实时数据
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SEND_STOP\r\n\r\n");
			#endif
			break;

		case CMD_SET_TARE:			//设置零点
			cmd_set_tare_reply(uart_get_body(),&g_cmdMutual.prmReply);
			
			smpl_tab_zero_set(g_cmdMutual.pcmLssued.tare_ch,g_cmdMutual.prmReply.tare_code);	
			prm_save();
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SET_TARE\r\n\r\n");
			#endif
			break;

	   case CMD_CTRL_MODE:			//控制模式
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);

			#ifdef DEBUG_CMD
				printf("接收命令：CMD_CTRL_MODE\r\n\r\n");
			#endif
			break;

		case CMD_PROTECT:			//设备保护使能
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_PROTECT\r\n\r\n");
			#endif
			break;

		case CMD_STOP:				//强制停止
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_STOP\r\n\r\n");
			#endif
			break;

		case CMD_STEP_NORM_V:		//下发试验步骤(值方式)
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_STEP_NORM_V\r\n\r\n");
			#endif					
			break;

		case CMD_OPEN_LOOP:			//下发开环控制量
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_OPEN_LOOP\r\n\r\n");
			#endif
			break;

		case CMD_SWITCH:			//开关量
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SWITCH\r\n\r\n");
			#endif
			break;
			
		case CMD_SET_DEVICE_ID:	//设置设备ID
			reply_empty(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_SET_DEVICE_ID\r\n\r\n");
			#endif
			break;
	 
		case CMD_GET_DEVICE_ID:	//获取设备ID
			cmd_get_device_id_reply(uart_get_body(),&g_cmdMutual.prmReply);
			#ifdef DEBUG_CMD
				printf("接收命令：CMD_GET_DEVICE_ID\r\n\r\n");
			#endif
			break;
	}
}

/*------------------------------------------------------------
 * Function Name  : CmdMutualBody
 * Description    : 命令交互体
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
					printf("接收命令超时！\r\n\r\n");
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
					printf("索引值不一致！\r\n\r\n");
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
 * Description    : 获取可读写输出
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
 * Description    : 采样循环体
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
void sample_cycle( void )
{	
	memcpy(&g_sampleCycle,(void *)uart_get_body(),uart_get_size());
	
	/* IO状态 */
	g_ioStatus.st_input = g_sampleCycle.input;
	g_ioStatus.st_sw_rol = g_sampleCycle.sw_rol;
	g_ioStatus.st_sw_rw = g_sampleCycle.sw_rw;
	
	#ifdef REAL_TIME_DATA_FRAM
		printf("%x\r\n",g_ioStatus.st_sw_rw);
	#endif
	
	/* 填充码值 */
	g_sampleCode[SMPL_KY_NUM] = g_sampleCycle.smpl[SMPL_KY_NUM];
	g_sampleCode[SMPL_KZ_NUM] = g_sampleCycle.smpl[SMPL_KZ_NUM];
	
	/* 填充力值 */
	g_sampleForce[SMPL_KY_NUM] = smpl_cal(g_sampleCode[SMPL_KY_NUM],SMPL_KY_NUM);
	g_sampleForce[SMPL_KZ_NUM] = smpl_cal(g_sampleCode[SMPL_KZ_NUM],SMPL_KZ_NUM);
	
	#ifdef DEBUG_FORCE_KY
		printf("FORCE_KY：%f\r\n",g_sampleForce[SMPL_KY_NUM]);
	#endif
	
	#ifdef DEBUG_FORCE_KZ
		printf("FORCE_KZ：%f\r\n",g_sampleForce[SMPL_KZ_NUM]);
	#endif
	
	/* 填充速度 */
	spd_cal_cycle();
}

/*------------------------------------------------------------
 * Function Name  : spd_cal
 * Description    : 计算某一通道速度
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
 * Description    : 速度计算循环体
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
		printf("KY_Speed：%f\r\n",g_speedCalculate.speed[SMPL_KY_NUM]);
		printf("KZ_Speed：%f\r\n",g_speedCalculate.speed[SMPL_KZ_NUM]);
	#endif
}

/*------------------------------------------------------------
 * Function Name  : get_smpl_spd
 * Description    : 获取指定通道速度
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
 * Description    : 获取传感器力值
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
 * Description    : 获取传感器码值
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
 * Description    : 串口获取数据包循环
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
 * Description    : 获取串口帧地址
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
 * Description    : 获取数据包命令
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
 * Description    : 获取串口数据体的指针
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
 * Description    : 获取串口数据索引值
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
 * Description    : 获取串口消息体大小
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
 * Description    : 获取串口消息体ADDR
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
 * Description    : 检查数据包是否正确
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
 * Description    : 串口发送数据包
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
static void uart_send_package(uint8_t addr,uint8_t index,uint8_t msg_size,uint8_t *ptr)
{
	uint8_t head[5];
	uint16_t crc16=0;
	uint8_t tail = MESSAGE_END;
	
	//发送头5个字节
	head[0]=MESSAGE_START;
	head[1]=addr;						//地址固定为0x01
	head[2]=index;
	head[3]=msg_size;
	head[4]=TOKEN;
	
	ComSend(COM1,head,sizeof(head));
	ComSend(COM1,ptr,msg_size);
	crc16=get_crc16(0,head,sizeof(head));
	crc16=get_crc16(crc16,ptr,msg_size);		//计算CRC16值
	ComSend(COM1,(uint8_t *)&crc16,2);
	ComSend(COM1,&tail,1);
}

/*------------------------------------------------------------
 * Function Name  : GetLinkStatus
 * Description    : 获取联机状态
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
 * Description    : 是否允许下发
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
 * Description    : 通讯循环体(需要循环调用，直至发送成功，或者失败)
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
				printf("CRC校验错误！\r\n\r\n");
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
 * Description    : 获取保活值
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
 * Description    : 设置保活值
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
 * Description    : 获取下位机回复状态
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
 * Description    : 联机初始化
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
		printf("联机初始化\r\n");
	#endif
}

/*------------------------------------------------------------
 * Function Name  : LinkCmdSendBody
 * Description    : 联机命令发送体
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
				printf("发送 LINK_SEND_STOP\r\n");
			#endif
			break;
		
		case LINK_SIGN_ON:
			cmd_cmd_sign_on();
		
			#ifdef DEBUG_LINK
				printf("发送 LINK_SIGN_ON\r\n");
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
					cmd_read_prm(g_WritePrm.targetAddr,g_WritePrm.len);	//最后一次读取
				}

				#ifdef DEBUG_LINK
					printf("发送 LINK_CHECK_PRM\r\n");	
				#endif										
			}
			else		//参数下发完毕
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
				printf("发送 LINK_MDY_PRM\r\n");	
			#endif	
			break;
		
		case LINK_SEND_START:
			cmd_send_start(SEND_RCODE);
			
			#ifdef DEBUG_LINK
				printf("发送 LINK_SEND_START\r\n");		
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
 * Description    : 写下位机参数初始化
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
 * Description    : 联机命令接收体
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
			printf("接收命令超时！\r\n\r\n");
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
			printf("索引值不一致！\r\n\r\n");
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
					printf("接收 LINK_SEND_STOP\r\n\r\n");	
				#endif
			}
			else
			{
				g_linkMutual.linkStatus = LINK_UNLINK;
				
				#ifdef DEBUG_LINK
					printf("失败接收 LINK_SEND_STOP\r\n\r\n");	
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
					printf("接收 LINK_SIGN_ON\r\n\r\n");	
				#endif
			}
			else
			{
				g_linkMutual.linkStatus = LINK_UNLINK;
				
				#ifdef DEBUG_LINK
					printf("失败接收 LINK_SIGN_ON\r\n\r\n");	
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
					g_WritePrm.pSourceAddr 	+= PRM_BUFF_CHECK;	//数据起始地址
					g_WritePrm.targetAddr 	+= PRM_BUFF_CHECK;	//已写入地址	
					g_WritePrm.len		 	-= PRM_BUFF_CHECK;	//剩余长度
					g_linkMutual.process 	= (float)(PRM_MEM_SIZE - g_WritePrm.len) / PRM_MEM_SIZE * 100;
				}
				else
				{
					g_linkMutual.linkStatus = LINK_MDY_PRM;
				}
			}
			else		//最后一次比较
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
				printf("接收 LINK_CHECK_PRM\r\n\r\n");	
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
				printf("接收 LINK_MDY_PRM\r\n\r\n");	
			#endif
			break;
		
		case LINK_SEND_START:  
			g_linkMutual.linkStatus = LINK_SUCCESS;
			
			#ifdef DEBUG_LINK
				printf("联机成功\r\n\r\n");
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
 * Description    : 联机进度
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
 * Description    : 联机循环体
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
				printf("CRC校验错误！\r\n\r\n");
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
 * Description    : 获取下位机回复信息
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
 * Description    : 获取采样地址
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
 * Description    : 获取采样码值
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
 * Description    : 检测下位机警告（b_err:0--无警告）
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

