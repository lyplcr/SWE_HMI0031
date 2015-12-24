/****************************************Copyright (c)**************************************************
**                                杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: COMMAND.H
** 最后修改日期: 2009-10-10 23:16:14 
** 描        述: 协议命令及告警信息
** 版	     本: 
** 主  控  芯 片:  	晶振频率:
** IDE:
**********************************************************************************************************/

#ifndef __COMMAND_H
#define	__COMMAND_H

// *****************[ 串口通讯协议标志字节 ]***************************
#define MESSAGE_START                      	0x1B        //消息开始标志
#define TOKEN                              	0x0E		//记号标志
#define MESSAGE_END                   	   	0x16  		//消息结束标志   


//从 0x0000-0x01FF 公共命令，所有控制器所用到的命令
#define		CMD_SIGN_ON						0x0001				//发回设备名称			
#define		CMD_DEVICE_VERSION				0x0002				//返回PCB信息及固件版本
#define		CMD_RESET						0x0003				//复位设备 *还没有打算好怎么处理
#define		CMD_PRIVATE_ACCESS 				0x0004				//进入/退出特权访问状态
#define		CMD_GET_ERROR					0x0005				//发回错误代码
#define		CMD_CLEAR_ERROR					0x0006				//清除错误
#define		CMD_READ_PRM					0x0007				//读参数
#define		CMD_WRITE_PRM					0x0008				//写参数 
#define		CMD_READ_PRV					0x0009				//读取特权参数	
#define		CMD_WRITE_PRV					0x000A				//写特权参数
#define		CMD_BACKUP_PRM					0x000B				//备份参数
#define		CMD_RESTORE_PRM					0x000C				//还原参数	
#define		CMD_OBD_QUERY					0x000D  			//OBD码查询
#define		CMD_OBD_RESET					0x000E  			//OBD码复位
#define		CMD_KEEP_ALIVE 					0x00FF				//保活数据
#define		RTM_SAMPLE_VALUE				0x01C0				//发送相对值
#define		RTM_SAMPLE_CODE					0x01C1				//发送绝对码
#define		RTM_SAMPLE_RCODE				0x01C2				//发送相对码  
#define		CMD_SEND_START					0x0101				//发送开始	
#define		CMD_SEND_STOP					0x0102				//发送停止
#define		CMD_SAMPLE_ONE_C				0x0103				//仅采集一个通道一次【码】
#define		CMD_SAMPLE_ONE_V				0x0104				//仅采集一个通道一次【值】
#define		CMD_SET_TARE					0x0105				//设置零点 
#define		CMD_CTRL_MODE					0x0106				//控制模式选择
//#define		CMD_SET_TEST				0x0107 				//设置试验状态
#define		CMD_PROTECT						0x0108				//设备保护使能
#define		CMD_STOP 						0x0109				//强制中断
#define		CMD_STEP_NORM_V					0x010A				//发送一个常规步骤
#define		CMD_STEP_NORM_C 				0x010B				//下发一个试验步骤(码方式)
#define		CMD_STEP_SPEC					0x010C				//发送一个非常规步骤 
#define		CMD_OPEN_LOOP					0x010D				//开环参数
#define		CMD_SWITCH						0x010E				//开关量控制命令
#define		CMD_KBSPD_SEL					0x010F				//选择某小键盘速度组
#define		CMD_CALIBRATION					0x010F				//进入标定和退出校准检定命令 
#define		CMD_FORWARD_DIR					0x0110				//0表示进程时控制量往增大方向变化；1表示进程时控制量往减少方向变化 
#define		CMD_SET_DEVICE_ID				0x000F				//设置设备ID 
#define		CMD_GET_DEVICE_ID  				0x0010				//获取设备ID
#define		NTF_PROCESSING 					0x0180	
#define		NTF_FINISHED					0x0181	
#define		NTF_FUN_KEY						0x0182
#define		CMD_SAMPLE_PROTECT 				0x001D				//式样保护

#define		CMD_PC_READ						0x0011				//读取PC管理的存储区
#define		CMD_PC_WRITE					0x0012				//写入PC管理的存储区
#define		CMD_OPEN_LOOP_EX 				0x0111				//扩展的开度命令
#define	    CMD_SIGN_OFF						0x00FE				//断开连接
 
 
//骆挺增加的命令
//#define		CMD_HANDSET_ON		        	0xF001          	//手控盒联机	
//#define		CMD_HANDSET_COMM 				0xF002				//手控盒命令
//#define		STATUS_ERROR_STATUS		    	0x01                //当前主机拒绝接受此键功能
 

#define		CMD_HANDSET_LINK				0xF011
#define		CMD_HANDSET_KEY					0xF012
#define		CMD_HANDSET_CODING				0xF013
#define		CMD_HANDSET_LIVE				0xF0F0  
#define		NTF_HANDSET_STATUS				0xF100


//状态定义
#define		STATUS_OK						0x00	//正确响应		
#define 	STATUS_ERROR_SAMPLE_1			0x01	//0号传感器采集出错	 
#define		STATUS_ERROR_PROT_P_BASE		0x11	//0号传感器超过正向最大保护值 
#define		STATUS_ERROR_PROT_P_FH			0x11	//负荷传感器超过正向最大保护值 
#define		STATUS_ERROR_PROT_P_WY			0x12	//位移传感器超过正向最大保护值 
#define		STATUS_ERROR_PROT_P_END			0x20
	 
//0x11-0x20						保留		
#define		STATUS_ERROR_PROT_N_BASE		0x21	//0号传感器超过负向最大保护值 
#define		STATUS_ERROR_PROT_N_FH			0x21	//负荷传感器超过负向最大保护值 
#define		STATUS_ERROR_PROT_N_WY			0x22	//位移传感器超过负向最大保护值 
#define		STATUS_ERROR_PROT_N_END			0x30	 
	 
//0x21-0x30						保留

#define		STATUS_ERROR_FULL_SCALE_KY		0x40	//载荷传感器满码警告
#define		STATUS_ERROR_FULL_SCALE_FH		0x40	//载荷传感器满码警告  
#define		STATUS_ERROR_FULL_SCALE_KZ		0x41	//载荷传感器满码警告
#define		STATUS_ERROR_FULL_SCALE_0		0x40	//  
#define		STATUS_ERROR_FULL_SCALE_1		0x41	// 


//...

#define		STATUS_ERROR_EXE_DIR			0x50	//进程方向参数错误警告	
#define 	STATUS_ERROR_TARGET_PROP		0x51	//目标属性参数错误	
#define 	STATUS_ERROR_WY_ZERO			0x52	//位移零点数据错误
#define		STATUS_EMERGENCY_STOP			0x5E	//急停按钮按下
#define		STATUS_SERVO_DRIVE_WARN			0x5F	//伺服驱动器报警
 
#define		STATUS_ERROR_OVERSPEED_1		0x61	//传感器0过速保护
#define		STATUS_ERROR_OVERSPEED_FH		0x61	//负荷通道过速保护
#define		STATUS_ERROR_OVERSPEED_KY		0x61	//抗压通道过速保护
#define		STATUS_ERROR_OVERSPEED_KZ		0x62	//抗折通道过速保护
#define		STATUS_ERROR_OVERSPEED_2		0x62	//传感器1过速保护
#define		STATUS_ERROR_OVERSPEED_3		0x63	//传感器2过速保护
//.......
#define		STATUS_ERROR_OVERSPEED_6		0x66  	//传感器5过速保护
#define		STATUS_ERROR_OVER_ENTRY_KY		0x70 	//抗压超过入口力保护
#define		STATUS_ERROR_OVER_ENTRY_KZ		0x71 	//抗折超过入口力保护
#define		STATUS_ERROR_OVER_ENTRY_0		0x70 	//
#define		STATUS_ERROR_OVER_ENTRY_1		0x71 	// 
 
 
//通讯中的错误 

#define		STATUS_ERROR				0x80	
#define 	STATUS_ERROR_UNKNOWN		0x80	//未知错误
#define 	STATUS_ERROR_TIMEOUT		0x81	//超时错误
#define 	STATUS_ERROR_CRC			0x82	//校验错
#define 	STATUS_ERROR_DI				0x83	//数据标识不存在
#define 	STATUS_ERROR_LENGTH			0x84	//数据内容长度不正确
#define 	STATUS_ERROR_FORMAT			0x85 	//数据内容格式不正确
#define 	STATUS_ERROR_ADDR			0x86	//目标地址不存在
#define 	STATUS_ERROR_READ			0x87	//目标地址无法读取
#define 	STATUS_ERROR_WRITE			0x88	//目标地址无法写入
#define 	STATUS_ERROR_STATUS			0x89	//目前状态无法执行
#define		STATUS_ERROR_HANDSET_KEY	0x8A	//手控盒按键不支持
#define		STATUS_ERROR_FUNKEY			0x8A	//功能键不支持	
#define		STATUS_ERROR_LIMIT_POS		0x8B	//油缸限位
 


#define		STATUS_ERROR_CTRL_MAX		0x90	//到达控制最大输出错误	
#define		STATUS_ERROR_LIMIT			0x91	//限制指令执行	
#define		STATUS_ERROR_IVLD_PARA		0x92	//非法参数	
#define		STATUS_ERROR_TABLE			0x93	//标定表格错误
#define		STATUS_ERROR_ENTRY			0x94	//当前传感器参数过大（超过控制入口）	
#define		STATUS_ERROR_STEP			0x95	//步骤参数错误
#define		STATUS_ERROR_UNLIVE			0x96	//失去和上位机联系	
#define		STATUS_ERROR_HANDSET		0x97	//手控盒连接错误	
#define		STATUS_ERROR_PID_DIRE		0x98	//PID方向参数错误	
#define		STATUS_ERROR_SWLIMIT 		0x99	//限位状态错误
#define		STATUS_ERROR_UPLIMIT 		0x9A	//上限位	
#define		STATUS_ERROR_DWLIMIT 		0x9B	//下限位	
#define		STATUS_ERROR_UNEXPECT 		0x9C	//反预期保护	
#define		STATUS_ERROR_CRASH 			0x9D	//预碰撞保护
#define		STATUS_ERROR_ID_SET			0x9E	//设置ID出错（已经设置过ID,不允许再次设置）
#define		STATUS_ERROR_ID				0x9F	//错误的设备ID


//0xA0错误代码以上,必须重启才能解除
#define		WAR_ERROR_INSTUCT			0xA0	//指令方向非法参数
#define		WAR_ERROR_GAIN1				0xA1	//通道1增益非法参数错误
#define		WAR_ERROR_GAIN2				0xA2	//通道2增益非法参数错误
#define		WAR_ERROR_GAIN3				0xA3	//通道3增益非法参数错误
#define		WAR_ERROR_GAIN4				0xA4	//通道4增益非法参数错误
#define		WAR_ERROR_GAIN5				0xA5	//通道5增益非法参数错误
#define		WAR_ERROR_GAIN6				0xA6	//通道6增益非法参数错误
 		
 		
//传感器通道初始化错误 						
#define		WAR_ADC_1_INIT_ERR			0xB0	//ADC通道1初始化出错	
#define		WAR_ADC_2_INIT_ERR			0xB1	//ADC通道2初始化出错
#define		WAR_ADC_3_INIT_ERR			0xB2	//ADC通道3初始化出错
#define		WAR_ADC_4_INIT_ERR			0xB3	//ADC通道4初始化出错
#define		WAR_ELE_GEAR_ERR			0xC0	//电子齿轮参数超出范围


#endif
