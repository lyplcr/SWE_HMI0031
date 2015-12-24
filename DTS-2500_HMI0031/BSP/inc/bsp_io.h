/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/


/* Exported define -----------------------------------------------------------*/

/* �������� */
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
#define		KEY_EXPORT				0x00008000		//����
#define		KEY_DISPLACE_TARE		0x00008000		//λ������
#define		KEY_PRINT				0x00010000		//��ӡ
#define		KEY_DEFORMATE_TARE		0x00010000		//��������
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


//����λ����
#define		BTN_START			0x00000001			//�ͱõ��������ť
#define		BTN_STOP			0x00000002			//�ͱõ��ֹͣ��ť
#define		BTN_POWER			0x00000004			//��Դ��ť	
#define		BTN_EMEG			0x00000008			//��ͣ��ť
#define		BTN_UP_SCREW		0x00000010			//˿������
#define		BTN_DW_SCREW		0x00000020			//˿���½�	
#define		BTN_UP_PLATFORM		0x00000040			//�͸׻��ߵ綯������	
#define		BTN_DW_PLATFORM		0x00000080			//�͸׻��ߵ綯���½�
#define		BTN_UP_LOCK			0x00000100			//�ֿغ���ǯ�ڼӽ���ť
#define		BTN_UP_UNLOCK		0x00000200			//�ֿغ���ǯ���ɿ���ť
#define		BTN_DW_LOCK			0x00000400			//�ֿغ���ǯ�ڼн���ť
#define		BTN_DW_UNLOCK		0x00000800			//�ֿغ���ǯ���ɿ���ť	
#define		BTN_TEST_RUN		0x00001000			//��������
#define		BTN_TEST_STOP		0x00002000			//����ֹͣ		
#define		BTN_UP_CROSSHEAD	0x00004000			//�ֿغ�������ť����������	
#define		BTN_DW_CROSSHEAD	0x00008000			//�ֿغ��½���ť����������	

#define		LIMIT_UP_COMPRESS	0x00010000			//��ѹͨ������λ����������λ,����200��˿������λ
#define		LIMIT_UP			0x00010000
#define		LIMIT_DW_COMPRESS	0x00020000			//��ѹͨ������λ����������λ������200��˿������λ
#define		LIMIT_DOWN			0x00020000						
#define		LIMIT_UP_BEND		0x00040000			//EDD����������������λ
#define		LIMIT_DW_BEND 		0x00080000			//EDD����������������λ
#define		LIMIT_UP_OIL 		0x00100000			//���͸׵�����λ
#define		LIMIT_OIL 			0x00100000			
#define		LIMIT_DW_OIL 		0x00200000			//���͸׵�����λ�������ã�
#define		LIMIT_UP_CROSSHEAD 	0x00400000			//��������λ
#define		LIMIT_DW_CROSSHEAD 	0x00800000			//��������λ
#define		ALARM_SERVO 		0x01000000			//�ŷ������������
#define		BTN_BOOTRESET		0x80000000			//��������λ��ť����	


//���λ����
#define		DO_MAIN_PUMP		0x00000001			//���ͱõ�����
#define		DO_QUICK_PUMP		0x00000002			//�����ͱõ�����
#define		DO_MOTOR_EN			0x00000004			//��������˿�˵��ʹ�����	
#define		DO_MOTOR_DIR		0x00000008			//��������˿�˵���������		
#define		DO_OVERLOAD			0x00000010			//���ر������	
//...
#define		DO_VALVE_FWD		0x00000100			//���͵�ŷ��������		
#define		DO_VALVE_RET		0x00000200			//���͵�ŷ������۵�ŷ����		
#define		DO_VALVE_BEND		0x00000200			//���͵�ŷ������۵�ŷ����		
#define		DO_UP_LOCK			0x00000400			//��ǯ�ڼн���ŷ�	
#define		DO_UP_UNLOCK		0x00000800			//��ǯ���ɿ���ŷ�		
#define		DO_DW_LOCK			0x00001000			//��ǯ�ڼн���ŷ�	
#define		DO_DW_UNLOCK		0x00002000			//��ǯ���ɿ���ŷ�		
//...
#define		DO_SERVO_ON			0x00010000			//�ŷ�������ʹ�����
#define		DO_INVT_FWD			0x00020000			//��Ƶ����ת����ź�	
#define		DO_INVT_REV			0x00040000			//��Ƶ����ת����ź�	
//...
#define		DO_FAN				0x01000000			//ɢ�ȷ�����
#define		DO_ALARM			0x02000000			//�������	
#define		DO_OVERHEAT			0x04000000			//���ȱ������			
#define		DO_BEEP				0x80000000			//��������� 
 
//ָʾ�ƶ���
#define		LAMP_RUN			0x00000001			//ִ�л������������𣬶���ʱ��	
#define		LAMP_ALARM			0x00000002			//�ڳ��־���״̬ʱ��2HZ��˸
#define		LAMP_COM			0x00000004			//����ʱ�𣬵��յ����ݺ�����������������ʱ��10HZ��˸
#define		LAMP_LINK			0x00000008			//����PC���������������ѻ�����
#define		LAMP_IP				0x00000010			//��IP��ַ��Ĭ�ϵ�IP��ַʱ��������״̬����	
#define		LAMP_HOLD			0x00000020			//�����뱣��״̬ʱָʾ���������˳�ʱ��
#define		LAMP_FWD			0x00000040			//����״̬ʱָʾ����������״̬ʱ��
#define		LAMP_RET			0x00000080			//ж��״̬ʱָʾ����������״̬ʱ��
//...
#define		LAMP_TARE			0x00000100			//�����������ʾʱָʾ������������ʾʱָʾ����
#define		LAMP_PEAK			0x00000200			//�����ڷ�ֵ��ʾ״̬ʱָʾ����������״̬��

//...
#define		LAMP_PUMP_START		0x00010000			//���ͱ�����ʱָʾ�������ͱ�ֹͣʱ��
#define		LAMP_PUMP_STOP		0x00020000			//���ͱ�����ָʾ��״̬�෴
#define		LAMP_POWER 			0x00040000			//��ϵͳ����ʱ����Դָʾ����

//...
#define		LAMP_BOARD_STA 		0x01000000			//���ư�״ָ̬ʾ��
#define		LAMP_BOARD_COM 		0x02000000			//���ư�ͨѶָʾ�ƣ�δͨѶʱ�����յ����ݺ�����ÿ�յ�һ��������һ�Ρ�

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
