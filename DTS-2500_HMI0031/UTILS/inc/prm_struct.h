/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: prm_struct.h
** 最后修改日期: 2011-1-24 15:07:25
** 描        述: 
** 版	     本: V1.0
** 主  控  芯 片:STM32F103 	  晶振频率:8MHZ 
** IDE:MDK3.50
**********************************************************************************************************/
#ifndef __PRM_STRUCT_H
#define	__PRM_STRUCT_H

 
#pragma pack(1)

//设备参数
typedef struct
{
	uint8_t  device_addr;				//设备地址
	uint32_t device_baud;				//波特率 
	uint8_t  send_freq;					//发送频率
	uint8_t  devc_sn[16];				//设备编号，通过软件设置机器编号
	uint8_t  devc_token;				//设备标识,表示哪种机型
	uint32_t device_ip_addr;			//设备IP地址
	uint8_t  mac_addr[6];				//设备mac地址
}PRM_DEVC_TypeDef;


  
//控制参数
typedef struct 
{
	uint32_t ctrl_max;					//最大控制量
	uint16_t ctrl_max_cnt;				//允许最大空置量次数，若连续到达最大控制量次数超过这个值，控制器保护，若这个值=0，没有此项保护功能。	
	uint16_t ctrl_max_one;				//单次最大控制量
	uint16_t ctrl_min_one;				//单次最小控制量
	uint32_t ctrl_open;					//开环控制量
	uint32_t ctrl_back;					//回退量
	uint8_t  exc_dir;					//进程时横梁的运行方向，向上或者向下		 
}
PRM_CTRL_TypeDef;

 
//快速慢速值
typedef struct 
{
	uint32_t fast_speed;
	uint32_t slow_speed;
}
PRM_SPCL_SPEED_TypeDef;


#ifdef	EDC_3000	
//特殊参数[EDC-3000]
typedef struct 			
{
	PRM_SPCL_SPEED_TypeDef hand_speed[2]; 
	uint16_t protect_bit;				//默认保护使能
}
PRM_SPCL_TypeDef;
#else
//特殊参数[包括EHC-2300,EDC-2300]
typedef struct			
{
	uint16_t protect_bit;				//默认保护使能
	uint8_t  rsv1[20-sizeof(uint16_t)];	
	uint32_t ky_unload_delay;
	uint32_t ky_valve_init;
	uint32_t ky_valve_back;
	uint32_t ky_valve_middle;
	uint32_t ky_valve_qup;			//快上
	uint32_t ky_valve_qdown;		//快下
	uint32_t ky_valve_sup;			//慢上
	uint32_t ky_valve_sdown;		//慢下
	uint8_t  rsv2[40-sizeof(uint32_t)*8];	
	uint32_t kz_unload_delay;
	uint32_t kz_valve_init;
	uint32_t kz_valve_back;
	uint32_t kz_valve_middle;
	uint32_t kz_valve_qup;	  		//快上
	uint32_t kz_valve_qdown;	  	//快下
	uint32_t kz_valve_sup;	  		//慢上
	uint32_t kz_valve_sdown;	  	//慢下
}
PRM_SPCL_TypeDef;
#endif
  
 
typedef struct
{
	uint16_t limit_pass;		//是否限制使用0x1111表示无限使用
	uint16_t use_time_max;		//最大使用次数，即运行次数
	uint16_t time_use;			//当前使用次数 
	uint16_t active_year;		//激活年 0x2011
	uint16_t active_date;		//激活日期 0x0624
	uint16_t product_idh;		//产品编号高4位  BCD码
	uint16_t product_idl;		//产品编号低4位	 BCD码	
	uint16_t expire_day;		//体验天数
	uint16_t pre_use_year;		//上一次使用年份
	uint16_t pre_use_date;		//上一次使用日期
}DEVICE_INFO_TypeDef;

    
//内部参数
typedef struct 	 
{
	uint32_t dvc_pwd;	 									
	uint8_t  dvc_info[32];								
}
PRM_INSD_TypeDef;

 
//***************************************************** 
//关于控制部分的传感器参数
typedef struct  
{
	uint8_t  smpl_type[2];					//类型			2
	float smpl_entry;						//控制入口		4
	float smpl_full_p;						//正向满量程		4
	float smpl_full_n;						//反向满量程		4
	float cof_keep_ahead;					//保持提前系数	4
	float cof_keep_spd;						//保持速度系数	4
	float cof_keep_brk;						//保持制动系数	4
	float crash_threshold;					//预碰撞安全值	4 
	float overspeed;						//传感器过速值	4
}PRM_SMPL_CTRL_TypeDef;
		
//PID结构体
typedef  struct 	 	
{
	float pid_node;				//PID节点
	uint16_t pid_kp;			//KP值
	uint16_t pid_ki;			//KI值
	uint16_t pid_kd;			//KD值
}SMPL_PID_TypeDef;

//关于pid部分的传感器参数
typedef struct 			 
{
	uint8_t  smpl_pid_segs;						//PID分段数
	SMPL_PID_TypeDef pid_str[5];				//PID参数
}PRM_SMPL_PID_TypeDef;
		 
//定标点结构体
typedef struct 	 
{
	float value;						//标定值
	int32_t code;						//标定码
}
TABLE_PT_TypeDef;
		
//关于表定标部分的传感器参数
typedef struct 						 
{
	int32_t smpl_zero;										//零点	
	uint8_t  table_num;										//标定表项数
	TABLE_PT_TypeDef table_pt[16];							//标定表
}PRM_SMPL_TAB_TypeDef;
	
//*****************传感器通道的大结构体,一共是250字节一个通道
typedef struct 		 
{
	PRM_SMPL_CTRL_TypeDef prm_smpl_ctrl;
	uint8_t  rsv_1[50-sizeof(PRM_SMPL_CTRL_TypeDef)];				//共50字节的控制部分的传感器参数
	
	PRM_SMPL_PID_TypeDef  prm_smpl_pid;
	uint8_t  rsv_2[60-sizeof(PRM_SMPL_PID_TypeDef)];		 
	
	PRM_SMPL_TAB_TypeDef prm_smpl_tab;
	uint8_t  rsv_3[140-sizeof(PRM_SMPL_TAB_TypeDef)];	
}PRM_SMPL_TypeDef;


//系统参数[1级参数]		
typedef struct 
{
	PRM_DEVC_TypeDef prm_devc;						//设备参数
	uint8_t  rsv_1[100-sizeof(PRM_DEVC_TypeDef)];
	PRM_CTRL_TypeDef prm_ctrl;						//控制参数
	uint8_t  rsv_2[100-sizeof(PRM_CTRL_TypeDef)];
	PRM_SPCL_TypeDef prm_spcl;						//特殊参数
	uint8_t  rsv_3[100-sizeof(PRM_SPCL_TypeDef)]; 
	PRM_INSD_TypeDef prm_insd;						//内部参数	
	uint8_t  rsv_4[100-sizeof(PRM_INSD_TypeDef)];
	PRM_SMPL_TypeDef  prm_smpl[SMPL_NUM];			//通道参数				
}SYS_PRM_TypeDef;   /* 共1900字节 */
  
  
  
//这些参数要通过调试工具修改，要进入特权访问才能修改,特权参数
typedef struct
{
	uint16_t	instrct_dire;				//指令方向，指令和控制量的关系
	uint16_t 	sensor_valid;				//可用传感器
	uint16_t 	sensor_used;				//以用传感器
	uint16_t  	gain[6];	 				//增益	
	uint32_t 	electronic_gear;			//电子齿轮值，即电机转一圈所需的脉冲数 
	uint32_t  	devc_key;					//设备钥匙
	uint8_t 	rsv[100-26];
}SYS_PRV_TypeDef;	/* 共100字节 */
  
  
#pragma pack()  

  
#endif
