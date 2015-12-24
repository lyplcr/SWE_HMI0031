/****************************************Copyright (c)**************************************************
**                                 杭州鑫高科技有限公司
**                                     
**                                
** 文   件   名: PRM.H
** 最后修改日期: 2014/5/5 18:05:38
** 描        述: 参数读写函数
** 版	     本: V1.1
** 主  控  芯 片:STM32F103   
** IDE:MDK4.12
**********************************************************************************************************/
#ifndef __PRM_H
#define	__PRM_H	
 
 		   
#define		PRM_MEM_SIZE			(sizeof(SYS_PRM_TypeDef))				//参数存储器字节数	
#define		PRV_MEM_SIZE			(sizeof(SYS_PRV_TypeDef))				//特权存储器字节数	
#define		PRM_MEM_SIZE_2CH		(400+2*sizeof(PRM_SMPL_TypeDef))		//参数+2个通道的参数(对于抗折抗压机只有2个通道可用)

#pragma pack(1)

//特权参数有关的函数  
typedef enum
{
	PGA_64X=0x6464,				//64倍放大倍数	
	PGA_32X=0x3232,				//32倍放大倍数
}GAIN_TypeDef;  				//ADC增益类型  				 
typedef enum
{
	INSTRCT_DIRE_P=0x5050, 		//指令方向和控制量方向相同	
	INSTRCT_DIRE_N=0x4E4E, 		//指令方向和控制量方向相反
}INSTRCT_DIRE_TypeDef; 			//指令方向类型

#pragma pack()

uint8_t *get_prm(void);										//返回系统参数指针
uint8_t *get_prv(void);										//返回特权参数指针
uint32_t devc_ip_get(void);									//获取设备IP参数
uint8_t *devc_mac_get(void);								//获取设备mac地址指针

uint8_t devc_token_get(void);								//获取设备标识,表示哪种机型
void devc_token_set(uint8_t token);							//设置设别标识
 
 
//控制参数获取函数
uint32_t ctrl_max_get(void);								//获取最大控制量（最大开度）
uint16_t ctrl_max_cnt_get(void);							//获取允许最大控制量保护次数
uint16_t ctrl_max_one_get(void);							//获取单次最大控制量
uint16_t ctrl_min_one_get(void);							//获取单次最小控制量
uint32_t ctrl_open_get(void);								//获取开环控制量 
uint8_t ctrl_exc_dir_get(void);								//获取进程时横梁的运行方向，向上或者向下
 
//保护定义
#define	PROTECT_SMPL_KY			0x0001						//通道0最大值保护
#define	PROTECT_SMPL_KZ			0x0002						//通道1最大值保护
#define	PROTECT_SMPL_2			0x0004						//通道2最大值保护
#define	PROTECT_SMPL_3			0x0008						//通道3最大值保护
#define	PROTECT_SMPL_4			0x0010						//通道4最大值保护
#define	PROTECT_SMPL_5			0x0020						//通道5最大值保护
#define	PROTECT_LIMIT			0x0040						//限位保护
#define	PROTECT_CRASH_KY		0x0080						//抗压预碰撞安全保护
#define	PROTECT_CRASH_KZ		0x0100						//抗折预碰撞安全保护
#define	PROTECT_UNEXP			0x0200						//反预期保护
#define	PROTECT_SPEED_KY		0x0400						//抗压过速保护
#define	PROTECT_SPEED_KZ		0x0800						//抗折过速保护  
#define	PROTECT_DEFAULT			(PROTECT_SMPL_KY|PROTECT_SMPL_KZ|PROTECT_CRASH_KY|PROTECT_CRASH_KZ|PROTECT_SPEED_KY|PROTECT_SPEED_KZ)


uint16_t spcl_protect_get(void);							//获取保护位标志
//EDC-2300，EHC-2300的特殊参数  
uint32_t spcl_unload_delay_get(SMPL_NAME_TypeDef2 ch);		//获取卸载延时时间
uint32_t spcl_valve_init_get(SMPL_NAME_TypeDef2 ch);		//获取指定通道的阀口初始位置
uint32_t spcl_valve_back_get(SMPL_NAME_TypeDef2 ch);	 	//获取指定通道的阀口回退开度
uint32_t spcl_valve_middle_get(SMPL_NAME_TypeDef2 ch);		//获取指定通道的阀口中位
uint32_t spcl_quick_up_get(SMPL_NAME_TypeDef2 ch);	 		//获取指定通道的快上开度
uint32_t spcl_quick_down_get(SMPL_NAME_TypeDef2 ch); 		//获取指定通道的快下开度
uint32_t spcl_slow_up_get(SMPL_NAME_TypeDef2 ch);  			//获取指定通道的慢上开度
uint32_t spcl_slow_down_get(SMPL_NAME_TypeDef2 ch); 	 	//获取指定通道的慢下开度  
  
//关于传感器参数的函数  
float smpl_ctrl_entry_get(uint8_t ch);	 					//获取指定通道的控制入口	
float smpl_ctrl_full_p_get(uint8_t ch); 		  			//获取指定通道的正向满量程	 
float smpl_ctrl_full_n_get(uint8_t ch);   					//获取指定通道的反向满量程
float smpl_cof_ahead_get(uint8_t ch);						//获取指定通道的提前保持系数 
float smpl_cof_spd_get(uint8_t ch);							//获取指定通道的速度保持系数
float smpl_cof_break_get(uint8_t ch);						//获取指定通道的制动保持系数
float smpl_ctrl_crash_get(uint8_t ch);						//获取指定通道的预碰撞安全值
float smpl_ctrl_overspeed_get(uint8_t ch); 					//返回传感器控制参数过速保护值  
float smpl_pid_node_get(uint8_t ch,uint8_t num);			//获取指定通道的pid 指定点的节点的值
uint16_t smpl_pid_kp_get(uint8_t ch,uint8_t num);			//获取指定通道的pid 指定点的kp值
uint16_t smpl_pid_ki_get(uint8_t ch,uint8_t num);			//获取指定通道的pid 指定点的ki值
uint16_t smpl_pid_kd_get(uint8_t ch,uint8_t num);			//获取指定通道的pid 指定点的kd值
uint8_t smpl_pid_segs_get(uint8_t ch);						//获取指定通道的pid段数
uint8_t smpl_tab_num_get(uint8_t ch);						//获取指定通道的标定表项数
float smpl_tab_value_get(uint8_t ch,uint8_t num);			//获取指定通道的标定表 指定点的值
int32_t smpl_tab_code_get(uint8_t ch,uint8_t num);			//获取指定通道的标定表 指定点的码
int32_t smpl_tab_zero_get(uint8_t ch);						//获取指定通道的零点值   
float smpl_cal(int32_t adc_code,uint8_t chn);				//计算传感器值  
  
//参数备份和恢复
void prm_read(void);										//从flash区读取系统参数
void prv_read(void);										//从flash区读取特权参数
void prm_save(void);										//保存系统参数到flash
void prv_save(void);										//保存特权参数到flash
FRESULT prm_backup(uint8_t type); 							//系统参数备份
FRESULT prv_backup(uint8_t type);							//特权参数备份
FRESULT prm_recover(uint8_t type); 							//系统参数恢复,保留加密信息不变 
FRESULT prv_recover(uint8_t type);							//特权参数恢复  
void prm_write(uint8_t *dat,uint16_t addr,uint16_t len);	//写参数内存的数据
void prv_write(uint8_t *dat,uint16_t addr,uint16_t len);	//写特权参数内存的数据 
//下面函数备份和恢复在SD卡中以日期文件夹形式存储
FRESULT PrmBackupWithDate(void);
FRESULT PrvBackupWithDate(void);
FRESULT PrmRecoverWithDate( const char *pFile_name );
FRESULT PrvRecoverWithDate( const char *pFile_name );

 
void devc_ip_set(uint32_t ip);										//设置设备IP参数,只改变内存的参数，不改变flash参数。
void devc_mac_set(uint8_t *mac);									//设置设备mac地址指针
void ctrl_max_set(uint32_t arg);									//设置最大控制量 
void ctrl_max_cnt_set(uint16_t arg);								//设置允许最大控制量次数 
void ctrl_max_one_set(uint32_t arg);								//设置单次最大控制量 
void ctrl_min_one_set(uint16_t arg);								//设置单次最小控制量 
void ctrl_open_set(uint32_t arg);									//设置开环控制量 
void ctrl_exc_dir_set(uint8_t arg);									//设置开环控制量 
void spcl_protect_set(uint16_t arg);								//设置开环控制量 
void spcl_unload_delay_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置卸载延时时间
void spcl_valve_init_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置指定通道的阀口初始位置 
void spcl_valve_back_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置指定通道的阀口回退开度	
void spcl_valve_middle_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置指定通道的阀口中位
void spcl_quick_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);			//设置指定通道的快上开度
void spcl_quick_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置指定通道的快下开度
void spcl_slow_up_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);			//设置指定通道的慢上开度
void spcl_slow_down_set(SMPL_NAME_TypeDef2 ch,uint32_t arg);		//设置指定通道的快下开度
void smpl_ctrl_entry_set(uint8_t ch,float entry);					//设置指定通道的控制入口	
void smpl_ctrl_full_p_set(uint8_t ch,float full_p);					//设置指定通道的正向满量程
void smpl_ctrl_full_n_set(uint8_t ch,float full_n);					//设置指定通道的反向满量程
void smpl_cof_ahead_set(uint8_t ch,float ahead);					//设置保持提前系数
void smpl_cof_spd_set(uint8_t ch,float keep_spd);					//保持系数计算
void smpl_cof_break_set(uint8_t ch,float keep_brk);					//设置制动系数
void smpl_ctrl_crash_set(uint8_t ch,float crash);					//设置指定通道的预碰撞安全值
void smpl_ctrl_overspeed_set(uint8_t ch,float speed);				//设置传感器控制参数过速保护值
void smpl_pid_segs_set(uint8_t ch,uint8_t segs);					//设置指定通道的pid段数
void smpl_pid_node_set(uint8_t ch,uint8_t num,float node);			//设置指定通道的pid 指定点的节点值
void smpl_pid_kp_set(uint8_t ch,uint8_t num,uint16_t kp);			//设置指定通道的pid 指定点的kp值
void smpl_pid_ki_set(uint8_t ch,uint8_t num,uint16_t ki);			//设置指定通道的pid 指定点的ki值
void smpl_pid_kd_set(uint8_t ch,uint8_t num,uint16_t kd);			//设置指定通道的pid 指定点的kd值
void smpl_tab_value_set(uint8_t ch,uint8_t num,float value);		//设置指定通道的标定表 指定点的值
void smpl_tab_code_set(uint8_t ch,uint8_t num,int32_t code);		//设置指定通道的标定表 指定点的码
void smpl_tab_zero_set(uint8_t ch,int32_t code);					//设置指定通道的零点值
void smpl_tab_num_set(uint8_t ch,uint8_t table);					//设置指定通道的标定表项数
//void prv_ins_dir_set(INSTRCT_DIRE_TypeDef dire);					//设置指令方向
void prv_gain_set(uint8_t ch,uint16_t gain);						//设置指定通道的增益
void prv_ele_gear_set(uint32_t eg);									//设置电子齿轮
void prv_devc_key_set(uint32_t key);								//设置设备钥匙

 
uint8_t *prm_devc_info(uint32_t *addr,uint16_t *len);				//获取设备参数开始地址和长度
uint8_t *prm_ctrl_info(uint32_t *addr,uint16_t *len);				//获取控制参数开始地址和长度
uint8_t *prm_spcl_info(uint32_t *addr,uint16_t *len);				//获取特殊参数开始地址和长度
uint8_t *prm_insd_info(uint32_t *addr,uint16_t *len);				//获取内部参数开始地址和长度
uint8_t *prm_smpl_info(uint32_t *addr,uint16_t *len,uint8_t ch);	//获取传感器参数开始地址和长度(传感器参数、PID参数、标定表)




#endif








