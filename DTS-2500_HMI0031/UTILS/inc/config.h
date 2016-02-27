/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: config.h
** 最后修改日期: 2014/5/5 18:17:17
** 描        述: 宏定义
** 版	     本: V1.0
** 主  控  芯 片:STM32F103  
** IDE:MDK4.12
**********************************************************************************************************/
	
#ifndef __CONFIG_H
#define	__CONFIG_H	

/* 宏定义软件为正式版本 */
//#define		VERSION_RELEASE	

#ifdef VERSION_RELEASE
	#define ENABLE_BEEP
#endif

#define 	USER_PROGRAM_START_ADDR	0x08020000							//应用程序起始地址
#define		SMPL_NUM				6									//传感器通道数	  
#define		CTRL_CHN				2   								//控制通道数
#define		PRIVATE_PASSWORD		0x2F8E4A7D							//特权访问密码
#define		DEFAULT_PASSWORD		"123"								//默认密码
#define		SUPER_PASSWORD			"88320524"							//管理员密码
#define		VALVE_RESET_DEGREE		-6400								//阀口复位开度
#define 	CTRL_FREQ				50									//控制频率
#define 	CTRL_PERIOD				(1000/CTRL_FREQ)					//控制周期
#define		SAMPLE_FREQ				10									//采样频率
#define 	SAMPLE_PERIOD			(1000/SAMPLE_FREQ)					//采样周期
#define		FLASH_FONT_PASSWORD		0x2F8E4A7D							//FLASH字库密码

#define		SYSTEM_MODEL			"SWE"								//系统型号
#define		SOFT_VERSION			"1.1.4.0"							//上位机软件版本	
#define		PCB_VERSION				"HMI0031"							//上位机PCB版本

#define 	CLI()     			 	__disable_irq()
#define 	SEI()      				__enable_irq()

/* 访问SD卡，使用DMA传输，要求变量4字节对齐 */
#if defined   (__CC_ARM)      /* ARM Compiler */
  #define __ALIGN_RAM    __align(4)  
#elif defined (__ICCARM__)    /* IAR Compiler */
  #define __ALIGN_RAM 
#elif defined  (__TASKING__)  /* TASKING Compiler */
  #define __ALIGN_RAM    __align(4) 
#endif /* __CC_ARM */ 

/*
	软件版本规则：
	（1）主版本号：当功能模块有较大的变动，比如增加模块或是整体架构发生变化。
	（2）次版本号：相对于主版本号而言，次版本号的升级对应的只是局部的变动，
		 但该局部的变动造成程序和以前版本不能兼容，或者对该程序以前的协作关系产生了破坏，
		 或者是功能上有大的改进或增强。
	（3）修订版本号：一般是Bug的修复或是一些小的变动或是一些功能的扩充，要经常发布修订版，
		 修复一个严重 Bug 即可发布一个修订版。
*/

typedef  void (*pFunction)(void);
typedef  void (*pFunctionArray[])(void);

//索引分类
typedef enum 
{
	BAUD_9600=9600,					//9600的波特率
	BAUD_19200=19200,				//19200的波特率
	BAUD_38400=38400ul,				//38400的波特率
	BAUD_57600=57600ul,				//57600的波特率
	BAUD_115200=115200ul,			//115200的波特率
}BAUD_RATE_TypeDef; 


//加载方向
typedef enum
{ 
	UP_DIRE = 0x00,		//进程加载
	DW_DIRE = 0x01		//回程加载
}LOAD_DIRE_TypeDef;  


//步骤状态枚举
typedef enum
{
	STATUS_STEP_INIT=0x00,		//步骤初始化阶段
	STATUS_STEP_LOAD,			//步骤加载阶段
	STATUS_STEP_TRANSITION,		//加载到保持过渡阶段
	STATUS_STEP_HOLD,			//保持阶段
}STEP_STATUS_TypeDef;
 
typedef enum
{
	EXE_UP = 0x00,				//横梁向上为进程方向
	EXE_DW = 0x01				//横梁向下为进程方向
}EXE_DIRE_TypeDef;

//传感器通道名称
typedef enum
{ 
	SMPL_FH_NUM = 0x00,			//负荷通道 
	SMPL_WY_NUM = 0x01,			//位移通道
	SMPL_BX_NUM = 0x02,			//变形通道
	SMPL_EX1_NUM = 0x03,		//扩展1通道
	SMPL_ECD1_NUM = 0x04,		//光电编码器1
	SMPL_ECD2_NUM = 005,		//光电编码器2
}SMPL_NAME_TypeDef; 

typedef enum
{ 
	SMPL_KY_NUM = 0x00,		//抗压通道 
	SMPL_KZ_NUM = 0x01		//抗折通道
}SMPL_NAME_TypeDef2;   
				
		
		
//如果按照1024字节为一个块，下位机参数在200块，PCM参数在204块，PRV参数在210块									
#define		PCM_DATA_BASE			(4<<10)
#define		PRV_DATA_BASE			(10<<10)

#define 	BACKUP_SD		0
#define 	BACKUP_USB		1

#pragma pack(1)

typedef struct
{
	//控制有关的参数
	uint8_t		load_type;					//加载方式
	uint8_t		load_dir;					//加载方向：加载的方向是设备判断【O】,还是强制正向【P】，或者强制负向【N】
	float		load_speed;					//加载速度
	//判断有关的参数
	uint8_t		target_type;				//目标方式
	uint8_t		target_prop;				//目标属性：A【目标示值】/R【增量示值】
	float 		target_value;				//目标值
	uint8_t		hold_type;					//保持方式
	uint32_t	hold_time;					//保持时间
	//执行有关的参数
	uint8_t		load_level;					//步骤级别	W【等待步骤】/I【立即步骤】
	uint8_t 	rsv[8];						//保留8个字节
}STEP_TypeDef;

#pragma pack()
 
#endif

 


