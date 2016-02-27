/****************************************Copyright (c)**************************************************
**                                 �����θ߿Ƽ����޹�˾
**                                     
**                                
** ��   ��   ��: report.h
** ����޸�����: 2014/5/6 15:35:54
** ��        ��: �����йصĺ���
** ��	     ��: V1.1
** ��  ��  о Ƭ:STM32F103 	   
** IDE:MDK4.12
**********************************************************************************************************/
#ifndef __REPORT_H
#define	__REPORT_H	
    

#pragma pack(1)

#define SUPPORT_TEST_NUM				12							//֧�ֵ��������

/* ��������ѡ�� */
typedef enum 
{
	NONE_TEST = 0,	//[0]���������
	KYSNJS,			//[1]ˮ�ེɰ��ѹ
	KYJZSJ,			//[2]����ɰ����ѹ	
	KYHNT,			//[3]��ͨ��������ѹ
	KZHNT,			//[4]��ͨ����������
	KYQQZ,			//[5]��ǽש��ѹ
	KYZJDH,			//[6]�𼶶���
	KYTY,			//[7]ͨ�ÿ�ѹ
	KZSNJS,			//[8]ˮ�ེɰ����
	KZYJSNJ,		//[9]ѹ��ˮ�ཬ����
	KZTY,			//[10]ͨ�ÿ���
	KLJSSW,			//[11]������������
}TEST_TYPE_TypeDef;


//������Ϣ�ṹ�� 
typedef struct
{
	char fname[17];
	tTime time;
}TEST_INFO_TypeDef;	 	
 	
 	
//���10��ʵ��ı����¼
typedef struct
{
	uint8_t index;			//0-9
	uint8_t type[10];
	char fname[10][17];
	tTime time[10];
}REPORT_LOG_TypeDef; 	 
		
//������� 	
typedef struct
{
	char test_standard[17];				//�����׼
	char sample_type[17];				//�Լ�Ʒ��
	char strength_grade[17];			//ǿ�ȵȼ�
	uint16_t sample_age;				//�Լ����� 
	char sample_spec[17];				//�Լ����
	float sample_span;					//�Լ����
	uint8_t sample_num;					//�����Լ�����
	uint8_t sample_serial;				//���鵱ǰ����
	uint8_t test_is_complete;			//һ�����������(0-δ��ɣ�1--���)
	float  force[20];					//������ֵ
	float  strength[20];				//����ǿ��
	float  force_valid[3];				//������Ч��ֵ
	float  strength_valid[3];			//������Чǿ��
	uint8_t result_valid;				//��������Ч�� ��0-�����Ч��1-��Чǿ�ȣ�2-ƽ��ǿ�ȣ�
	uint8_t test_process;				//�����Ƿ���� (0-���迹ѹ��1-��ѹ��ɣ�2-��ѹδ��ɣ�
	uint8_t collect_load[20];			//�Ƿ��ڼ����غ��ڣ�1��ʾ�� 0��ʾ��
	float length;						//����
	float width;						//���
	float high;							//�߶�
	float correct_cof;					//����ϵ��
	float gz_area;						//�������
	float bgz_area;						//���������
	uint8_t sample_shape_index;			//�Կ���״����
	float	yx_diameter;				//Բ��ֱ��
	
	/* ���ܻ����� */
	float maxForce[20];					//�����
	float maxStrength[20];				//���ǿ��
	float upYieldForce[20];				//��������
	float downYieldForce[20];			//��������
	float upYieldStrength[20];			//������ǿ��
	float downYieldStrength[20];		//������ǿ��
	float maxForceSumExtend[20];		//�����������
	float maxForceSumElongation[20];	//��������쳤��
	float parallelLenth;				//ƽ�г���
	float extensometerGauge;			//����Ʊ��
	float originalGauge;				//ԭʼ���
	float pipeThickness;				//�ܶκ��
	float pipeOuterDiameter;			//�ܶ��⾶
}REPORT_TypeDef;

#define RECORD_COORDINATE_PERIOD		100					//100ms��¼һ����
#define RECORD_COORDINATE_FREQ			(1000/RECORD_COORDINATE_PERIOD)	
#define RECORD_COORDINATE_TIME_SECOND	(5 * 60)			//5����	
#define DECORD_COORDINATE_FORCE_NUM		(RECORD_COORDINATE_TIME_SECOND * RECORD_COORDINATE_FREQ) 

typedef struct
{
	uint8_t xType;						//X������
	uint8_t yType;						//Y������
	uint8_t xUint;						//X�ᵥλ��0��s��1��ms��
	uint8_t yUint;						//Y�ᵥλ��0��kN��1��N��	
	float xMaxValue;					//X�����ֵ
	float yMaxValue;					//Y�����ֵ
	float systemMaxForce;				//ϵͳ�����ֵ
	uint16_t recordPointFreq;			//��¼ÿ����Ƶ��
	uint32_t nowUsePointNum;			//��ʹ�õĵ���
	uint32_t maxPointNum;				//֧�ֵ�������
	float xScalingCoefficient;			//����ϵ������X�ᵱǰֵ��������ϵ�����Σ�
	float yScalingCoefficient;			//����ϵ������Y�ᵱǰֵ��������ϵ�����Σ�	
	float force[DECORD_COORDINATE_FORCE_NUM];
	float deform[DECORD_COORDINATE_FORCE_NUM];
}COORDINATE_POINT_TypeDef;
    
#pragma pack()     

FRESULT get_free_space(float *space);												//��ȡSD��ʣ��ռ䣬��λ��Mb    								
FRESULT report_save(uint8_t pdrv, uint8_t type, const char * const file, const REPORT_TypeDef *report);    //������,type���������� file:�ļ��� report:ʵ�鱨��
FRESULT report_read(uint8_t type,const char * const file,REPORT_TypeDef *report);			//��ȡ���,type���������� file:�ļ��� report:ʵ�鱨��   
FRESULT report_delete(uint8_t type,const char * const file);						//ɾ��ָ����������ָ���ļ������ļ������type=0xFF��ɾ���������������и����������ļ�
FRESULT report_lately_10(REPORT_LOG_TypeDef *rlog);									//��ȡ���10��ʵ�����Ϣ
FRESULT report_exist(uint8_t type,const char * const file);								//��ѯָ����������ָ���ļ������ļ��Ƿ����

//functionName:FRESULT report_search_subsn(uint8_t type,char *subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb)
//description:ͨ�������Ų��ҽ��,��ָ������������������ֻҪ������subsn���е��ַ�������������
//type:��������
//subsn:�����ļ����а�����subsn���ַ���
//offset ƫ������һ��ֻ������10���ļ�����������������飬��Ҫʹ�ô˱��������ǰ����ļ�
//test:��෵��ƫ�ƺ��10��������Ϣ,test��ʱ��ʱ�������11�����飬���һ�������ļ���ȫ��Ϊ0��ʾ������
//fnumb:���ز��ҵ����ļ����� 
FRESULT report_search_sn(uint8_t type,const char * const subsn,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb);
//����ʱ��β�ѯ����
//type:��������
//date_start����ʼ����  date_end����������
//offset ƫ����
//test:��෵��ƫ�ƺ��10��������Ϣ
//fnumb:���ز��ҵ����ļ����� 
FRESULT report_search_date(uint8_t type,tTime date_start,tTime date_end,uint16_t offset,TEST_INFO_TypeDef *test,uint32_t *fnumb); 
  
FRESULT SetReportSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, \
			const char * const file);
FRESULT SetCurveSaveTime(uint8_t sourcePdrv, uint8_t targetPdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial);

FRESULT SaveCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, const COORDINATE_POINT_TypeDef * const pCoordinate );
FRESULT ReadCoordinatePoint( uint8_t pdrv, uint8_t testType, uint8_t sampleNum, \
			const char * const pSerial, COORDINATE_POINT_TypeDef * const pCoordinate );			
FRESULT DeleteCoordinateFolder( uint8_t pdrv, uint8_t testType, const char * const pSerial );	

ErrorStatus CopyFileFromDevices( const char * const pSourcePath, const char * const pTargetPath );
ErrorStatus CopyFolderFromDevices( const char * const pSourceFolderPath, const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );
ErrorStatus DeleteFileFromDevices( const char * const pTargetPath );
ErrorStatus DeleteFolderFromDevices( const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );

#endif








