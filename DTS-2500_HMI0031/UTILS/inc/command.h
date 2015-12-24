/****************************************Copyright (c)**************************************************
**                                �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: COMMAND.H
** ����޸�����: 2009-10-10 23:16:14 
** ��        ��: Э������澯��Ϣ
** ��	     ��: 
** ��  ��  о Ƭ:  	����Ƶ��:
** IDE:
**********************************************************************************************************/

#ifndef __COMMAND_H
#define	__COMMAND_H

// *****************[ ����ͨѶЭ���־�ֽ� ]***************************
#define MESSAGE_START                      	0x1B        //��Ϣ��ʼ��־
#define TOKEN                              	0x0E		//�Ǻű�־
#define MESSAGE_END                   	   	0x16  		//��Ϣ������־   


//�� 0x0000-0x01FF ����������п��������õ�������
#define		CMD_SIGN_ON						0x0001				//�����豸����			
#define		CMD_DEVICE_VERSION				0x0002				//����PCB��Ϣ���̼��汾
#define		CMD_RESET						0x0003				//��λ�豸 *��û�д������ô����
#define		CMD_PRIVATE_ACCESS 				0x0004				//����/�˳���Ȩ����״̬
#define		CMD_GET_ERROR					0x0005				//���ش������
#define		CMD_CLEAR_ERROR					0x0006				//�������
#define		CMD_READ_PRM					0x0007				//������
#define		CMD_WRITE_PRM					0x0008				//д���� 
#define		CMD_READ_PRV					0x0009				//��ȡ��Ȩ����	
#define		CMD_WRITE_PRV					0x000A				//д��Ȩ����
#define		CMD_BACKUP_PRM					0x000B				//���ݲ���
#define		CMD_RESTORE_PRM					0x000C				//��ԭ����	
#define		CMD_OBD_QUERY					0x000D  			//OBD���ѯ
#define		CMD_OBD_RESET					0x000E  			//OBD�븴λ
#define		CMD_KEEP_ALIVE 					0x00FF				//��������
#define		RTM_SAMPLE_VALUE				0x01C0				//�������ֵ
#define		RTM_SAMPLE_CODE					0x01C1				//���;�����
#define		RTM_SAMPLE_RCODE				0x01C2				//���������  
#define		CMD_SEND_START					0x0101				//���Ϳ�ʼ	
#define		CMD_SEND_STOP					0x0102				//����ֹͣ
#define		CMD_SAMPLE_ONE_C				0x0103				//���ɼ�һ��ͨ��һ�Ρ��롿
#define		CMD_SAMPLE_ONE_V				0x0104				//���ɼ�һ��ͨ��һ�Ρ�ֵ��
#define		CMD_SET_TARE					0x0105				//������� 
#define		CMD_CTRL_MODE					0x0106				//����ģʽѡ��
//#define		CMD_SET_TEST				0x0107 				//��������״̬
#define		CMD_PROTECT						0x0108				//�豸����ʹ��
#define		CMD_STOP 						0x0109				//ǿ���ж�
#define		CMD_STEP_NORM_V					0x010A				//����һ�����沽��
#define		CMD_STEP_NORM_C 				0x010B				//�·�һ�����鲽��(�뷽ʽ)
#define		CMD_STEP_SPEC					0x010C				//����һ���ǳ��沽�� 
#define		CMD_OPEN_LOOP					0x010D				//��������
#define		CMD_SWITCH						0x010E				//��������������
#define		CMD_KBSPD_SEL					0x010F				//ѡ��ĳС�����ٶ���
#define		CMD_CALIBRATION					0x010F				//����궨���˳�У׼�춨���� 
#define		CMD_FORWARD_DIR					0x0110				//0��ʾ����ʱ��������������仯��1��ʾ����ʱ�����������ٷ���仯 
#define		CMD_SET_DEVICE_ID				0x000F				//�����豸ID 
#define		CMD_GET_DEVICE_ID  				0x0010				//��ȡ�豸ID
#define		NTF_PROCESSING 					0x0180	
#define		NTF_FINISHED					0x0181	
#define		NTF_FUN_KEY						0x0182
#define		CMD_SAMPLE_PROTECT 				0x001D				//ʽ������

#define		CMD_PC_READ						0x0011				//��ȡPC����Ĵ洢��
#define		CMD_PC_WRITE					0x0012				//д��PC����Ĵ洢��
#define		CMD_OPEN_LOOP_EX 				0x0111				//��չ�Ŀ�������
#define	    CMD_SIGN_OFF						0x00FE				//�Ͽ�����
 
 
//��ͦ���ӵ�����
//#define		CMD_HANDSET_ON		        	0xF001          	//�ֿغ�����	
//#define		CMD_HANDSET_COMM 				0xF002				//�ֿغ�����
//#define		STATUS_ERROR_STATUS		    	0x01                //��ǰ�����ܾ����ܴ˼�����
 

#define		CMD_HANDSET_LINK				0xF011
#define		CMD_HANDSET_KEY					0xF012
#define		CMD_HANDSET_CODING				0xF013
#define		CMD_HANDSET_LIVE				0xF0F0  
#define		NTF_HANDSET_STATUS				0xF100


//״̬����
#define		STATUS_OK						0x00	//��ȷ��Ӧ		
#define 	STATUS_ERROR_SAMPLE_1			0x01	//0�Ŵ������ɼ�����	 
#define		STATUS_ERROR_PROT_P_BASE		0x11	//0�Ŵ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_P_FH			0x11	//���ɴ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_P_WY			0x12	//λ�ƴ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_P_END			0x20
	 
//0x11-0x20						����		
#define		STATUS_ERROR_PROT_N_BASE		0x21	//0�Ŵ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_N_FH			0x21	//���ɴ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_N_WY			0x22	//λ�ƴ���������������󱣻�ֵ 
#define		STATUS_ERROR_PROT_N_END			0x30	 
	 
//0x21-0x30						����

#define		STATUS_ERROR_FULL_SCALE_KY		0x40	//�غɴ��������뾯��
#define		STATUS_ERROR_FULL_SCALE_FH		0x40	//�غɴ��������뾯��  
#define		STATUS_ERROR_FULL_SCALE_KZ		0x41	//�غɴ��������뾯��
#define		STATUS_ERROR_FULL_SCALE_0		0x40	//  
#define		STATUS_ERROR_FULL_SCALE_1		0x41	// 


//...

#define		STATUS_ERROR_EXE_DIR			0x50	//���̷���������󾯸�	
#define 	STATUS_ERROR_TARGET_PROP		0x51	//Ŀ�����Բ�������	
#define 	STATUS_ERROR_WY_ZERO			0x52	//λ��������ݴ���
#define		STATUS_EMERGENCY_STOP			0x5E	//��ͣ��ť����
#define		STATUS_SERVO_DRIVE_WARN			0x5F	//�ŷ�����������
 
#define		STATUS_ERROR_OVERSPEED_1		0x61	//������0���ٱ���
#define		STATUS_ERROR_OVERSPEED_FH		0x61	//����ͨ�����ٱ���
#define		STATUS_ERROR_OVERSPEED_KY		0x61	//��ѹͨ�����ٱ���
#define		STATUS_ERROR_OVERSPEED_KZ		0x62	//����ͨ�����ٱ���
#define		STATUS_ERROR_OVERSPEED_2		0x62	//������1���ٱ���
#define		STATUS_ERROR_OVERSPEED_3		0x63	//������2���ٱ���
//.......
#define		STATUS_ERROR_OVERSPEED_6		0x66  	//������5���ٱ���
#define		STATUS_ERROR_OVER_ENTRY_KY		0x70 	//��ѹ�������������
#define		STATUS_ERROR_OVER_ENTRY_KZ		0x71 	//���۳������������
#define		STATUS_ERROR_OVER_ENTRY_0		0x70 	//
#define		STATUS_ERROR_OVER_ENTRY_1		0x71 	// 
 
 
//ͨѶ�еĴ��� 

#define		STATUS_ERROR				0x80	
#define 	STATUS_ERROR_UNKNOWN		0x80	//δ֪����
#define 	STATUS_ERROR_TIMEOUT		0x81	//��ʱ����
#define 	STATUS_ERROR_CRC			0x82	//У���
#define 	STATUS_ERROR_DI				0x83	//���ݱ�ʶ������
#define 	STATUS_ERROR_LENGTH			0x84	//�������ݳ��Ȳ���ȷ
#define 	STATUS_ERROR_FORMAT			0x85 	//�������ݸ�ʽ����ȷ
#define 	STATUS_ERROR_ADDR			0x86	//Ŀ���ַ������
#define 	STATUS_ERROR_READ			0x87	//Ŀ���ַ�޷���ȡ
#define 	STATUS_ERROR_WRITE			0x88	//Ŀ���ַ�޷�д��
#define 	STATUS_ERROR_STATUS			0x89	//Ŀǰ״̬�޷�ִ��
#define		STATUS_ERROR_HANDSET_KEY	0x8A	//�ֿغа�����֧��
#define		STATUS_ERROR_FUNKEY			0x8A	//���ܼ���֧��	
#define		STATUS_ERROR_LIMIT_POS		0x8B	//�͸���λ
 


#define		STATUS_ERROR_CTRL_MAX		0x90	//�����������������	
#define		STATUS_ERROR_LIMIT			0x91	//����ָ��ִ��	
#define		STATUS_ERROR_IVLD_PARA		0x92	//�Ƿ�����	
#define		STATUS_ERROR_TABLE			0x93	//�궨������
#define		STATUS_ERROR_ENTRY			0x94	//��ǰ�������������󣨳���������ڣ�	
#define		STATUS_ERROR_STEP			0x95	//�����������
#define		STATUS_ERROR_UNLIVE			0x96	//ʧȥ����λ����ϵ	
#define		STATUS_ERROR_HANDSET		0x97	//�ֿغ����Ӵ���	
#define		STATUS_ERROR_PID_DIRE		0x98	//PID�����������	
#define		STATUS_ERROR_SWLIMIT 		0x99	//��λ״̬����
#define		STATUS_ERROR_UPLIMIT 		0x9A	//����λ	
#define		STATUS_ERROR_DWLIMIT 		0x9B	//����λ	
#define		STATUS_ERROR_UNEXPECT 		0x9C	//��Ԥ�ڱ���	
#define		STATUS_ERROR_CRASH 			0x9D	//Ԥ��ײ����
#define		STATUS_ERROR_ID_SET			0x9E	//����ID�����Ѿ����ù�ID,�������ٴ����ã�
#define		STATUS_ERROR_ID				0x9F	//������豸ID


//0xA0�����������,�����������ܽ��
#define		WAR_ERROR_INSTUCT			0xA0	//ָ���Ƿ�����
#define		WAR_ERROR_GAIN1				0xA1	//ͨ��1����Ƿ���������
#define		WAR_ERROR_GAIN2				0xA2	//ͨ��2����Ƿ���������
#define		WAR_ERROR_GAIN3				0xA3	//ͨ��3����Ƿ���������
#define		WAR_ERROR_GAIN4				0xA4	//ͨ��4����Ƿ���������
#define		WAR_ERROR_GAIN5				0xA5	//ͨ��5����Ƿ���������
#define		WAR_ERROR_GAIN6				0xA6	//ͨ��6����Ƿ���������
 		
 		
//������ͨ����ʼ������ 						
#define		WAR_ADC_1_INIT_ERR			0xB0	//ADCͨ��1��ʼ������	
#define		WAR_ADC_2_INIT_ERR			0xB1	//ADCͨ��2��ʼ������
#define		WAR_ADC_3_INIT_ERR			0xB2	//ADCͨ��3��ʼ������
#define		WAR_ADC_4_INIT_ERR			0xB3	//ADCͨ��4��ʼ������
#define		WAR_ELE_GEAR_ERR			0xC0	//���ӳ��ֲ���������Χ


#endif
