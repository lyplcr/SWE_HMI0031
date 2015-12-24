/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/


/* Exported define -----------------------------------------------------------*/

/* 按键定义 */
#define		KEY_NONE				0x00000000
#define		KEY_ADD					0x00000001
#define		KEY_7					0x00000002
#define		KEY_4					0x00000004	
#define		KEY_1					0x00000008
#define		KEY_2					0x00000010
#define		KEY_5					0x00000020
#define		KEY_8					0x00000040
#define		KEY_0					0x00000080
#define		KEY_POINT				0x00000100
#define		KEY_9					0x00000200
#define		KEY_6					0x00000400
#define		KEY_3					0x00000800
#define		KEY_BACK				0x00001000
#define		KEY_LEFT				0x00002000
#define		KEY_SHIFT				0x00004000
#define		KEY_EXPORT				0x00008000		//导出
#define		KEY_DISPLACE_TARE		0x00008000		//位移清零
#define		KEY_PRINT				0x00010000		//打印
#define		KEY_DEFORMATE_TARE		0x00010000		//变形清零
#define		KEY_DOWN				0x00020000
#define		KEY_ENTER				0x00040000
#define		KEY_UP					0x00080000
#define		KEY_ESC					0x00100000
#define		KEY_RIGHT				0x00200000		
#define		KEY_FIND				0x00400000
#define		KEY_FORCE_TARE			0x00800000	
#define		KEY_BACKUP1				0x01000000
#define		KEY_OIL_RUN				0x02000000
#define		KEY_OIL_STOP			0x04000000
#define		KEY_BACKUP2				0x08000000
#define		KEY_F4					0x10000000
#define		KEY_F3					0x20000000
#define		KEY_F2					0x40000000	
#define		KEY_F1					0x80000000


//输入位定义
#define		BTN_START			0x00000001			//油泵电机启动按钮
#define		BTN_STOP			0x00000002			//油泵电机停止按钮
#define		BTN_POWER			0x00000004			//电源按钮	
#define		BTN_EMEG			0x00000008			//急停按钮
#define		BTN_UP_SCREW		0x00000010			//丝杆上升
#define		BTN_DW_SCREW		0x00000020			//丝杆下降	
#define		BTN_UP_PLATFORM		0x00000040			//油缸或者电动缸上升	
#define		BTN_DW_PLATFORM		0x00000080			//油缸或者电动缸下降
#define		BTN_UP_LOCK			0x00000100			//手控盒上钳口加紧按钮
#define		BTN_UP_UNLOCK		0x00000200			//手控盒上钳口松开按钮
#define		BTN_DW_LOCK			0x00000400			//手控盒下钳口夹紧按钮
#define		BTN_DW_UNLOCK		0x00000800			//手控盒下钳口松开按钮	
#define		BTN_TEST_RUN		0x00001000			//试验运行
#define		BTN_TEST_STOP		0x00002000			//试验停止		
#define		BTN_UP_CROSSHEAD	0x00004000			//手控盒上升按钮，横梁上升	
#define		BTN_DW_CROSSHEAD	0x00008000			//手控盒下降按钮，横梁上升	

#define		LIMIT_UP_COMPRESS	0x00010000			//抗压通道上限位，电拉上限位,或者200吨丝杆上限位
#define		LIMIT_UP			0x00010000
#define		LIMIT_DW_COMPRESS	0x00020000			//抗压通道下限位，电拉下限位，或者200吨丝杆下限位
#define		LIMIT_DOWN			0x00020000						
#define		LIMIT_UP_BEND		0x00040000			//EDD控制器，抗折上限位
#define		LIMIT_DW_BEND 		0x00080000			//EDD控制器，抗折下限位
#define		LIMIT_UP_OIL 		0x00100000			//主油缸的上限位
#define		LIMIT_OIL 			0x00100000			
#define		LIMIT_DW_OIL 		0x00200000			//主油缸的下限位（不常用）
#define		LIMIT_UP_CROSSHEAD 	0x00400000			//横梁上限位
#define		LIMIT_DW_CROSSHEAD 	0x00800000			//横梁下限位
#define		ALARM_SERVO 		0x01000000			//伺服电机报警输入
#define		BTN_BOOTRESET		0x80000000			//控制器复位按钮输入	


//输出位定义
#define		DO_MAIN_PUMP		0x00000001			//主油泵电机输出
#define		DO_QUICK_PUMP		0x00000002			//快升油泵电机输出
#define		DO_MOTOR_EN			0x00000004			//横梁或者丝杆电机使能输出	
#define		DO_MOTOR_DIR		0x00000008			//横梁或者丝杆电机方向输出		
#define		DO_OVERLOAD			0x00000010			//过载保护输出	
//...
#define		DO_VALVE_FWD		0x00000100			//进油电磁阀输出控制		
#define		DO_VALVE_RET		0x00000200			//回油电磁阀、抗折电磁阀输出		
#define		DO_VALVE_BEND		0x00000200			//回油电磁阀、抗折电磁阀输出		
#define		DO_UP_LOCK			0x00000400			//上钳口夹紧电磁阀	
#define		DO_UP_UNLOCK		0x00000800			//上钳口松开电磁阀		
#define		DO_DW_LOCK			0x00001000			//下钳口夹紧电磁阀	
#define		DO_DW_UNLOCK		0x00002000			//下钳口松开电磁阀		
//...
#define		DO_SERVO_ON			0x00010000			//伺服驱动器使能输出
#define		DO_INVT_FWD			0x00020000			//变频器正转输出信号	
#define		DO_INVT_REV			0x00040000			//变频器反转输出信号	
//...
#define		DO_FAN				0x01000000			//散热风机输出
#define		DO_ALARM			0x02000000			//警告输出	
#define		DO_OVERHEAT			0x04000000			//过热保护输出			
#define		DO_BEEP				0x80000000			//蜂鸣器输出 
 
//指示灯定义
#define		LAMP_RUN			0x00000001			//执行机构不动作是灭，动作时亮	
#define		LAMP_ALARM			0x00000002			//在出现警告状态时，2HZ闪烁
#define		LAMP_COM			0x00000004			//启动时灭，当收到数据后亮，连续接收数据时，10HZ闪烁
#define		LAMP_LINK			0x00000008			//当和PC机连机是亮，当脱机后灭
#define		LAMP_IP				0x00000010			//当IP地址是默认的IP地址时亮，其他状态下灭	
#define		LAMP_HOLD			0x00000020			//当进入保持状态时指示灯亮，当退出时灭
#define		LAMP_FWD			0x00000040			//加载状态时指示灯亮，其他状态时灭
#define		LAMP_RET			0x00000080			//卸载状态时指示灯亮，其他状态时灭
//...
#define		LAMP_TARE			0x00000100			//当处于相对显示时指示灯亮，绝对显示时指示灯灭。
#define		LAMP_PEAK			0x00000200			//当处于峰值显示状态时指示灯亮，其他状态灭

//...
#define		LAMP_PUMP_START		0x00010000			//当油泵启动时指示灯亮，油泵停止时灭
#define		LAMP_PUMP_STOP		0x00020000			//和油泵启动指示灯状态相反
#define		LAMP_POWER 			0x00040000			//当系统供电时，电源指示灯亮

//...
#define		LAMP_BOARD_STA 		0x01000000			//控制板状态指示灯
#define		LAMP_BOARD_COM 		0x02000000			//控制板通讯指示灯，未通讯时：灭。收到数据后亮，每收到一次数据灭一次。

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_InitIO(void);
uint32_t GetKey( void );
void SetLampVal( uint32_t led );

void ClrLampBit(uint32_t bit);
void SetLampBit(uint32_t bit);
void RevLampBit(uint32_t bit);
void SetLamp(uint32_t status);
uint32_t GetLamp( void );
void LampTask( void );

void ClrOutportBit(uint32_t bit);
void SetOutportBit(uint32_t bit);
void RevOutportBit(uint32_t bit);
void SetOutport(uint32_t status);
uint32_t GetOutport( void );
void OutportTask( void );

void SetBeep(uint16_t beep_on, uint16_t mute, uint16_t time);
void BeepISR( void );

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
