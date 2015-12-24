/****************************************Copyright (c)**************************************************
**                               	�����θ߿Ƽ����޹�˾
**                                   
**                                 
** ��   ��   ��: ustring.c
** ����޸�����: 2014/4/23 12:47:45
** ��        ��: �ַ�������
** ��	     ��: V1.0
** ��  ��  о Ƭ: 	 ����Ƶ��:	
** IDE:MDK4.12
**********************************************************************************************************/
#include <math.h>
#include "stm32f4xx.h"
#include "ustdlib.h"
#include "string.h"


/**********************************************************************
functionName:uint08 bcd2val_8(uint08 bcd)
description:BCD�뵽ֵ��ת��
**********************************************************************/
ErrorStatus bcd2val_8(uint8_t bcd,uint8_t *val)
{
	if((bcd&0x0f)>9)
	return ERROR;
	if((bcd>>4)>9) 
	return ERROR;	  		
	*val=(bcd>>4)*10+(bcd&0x0f);
	return SUCCESS;
}

/**********************************************************************
functionName:uint08 val2bcd_8(uint08 val)
description:ֵ��BCD���ת��
**********************************************************************/ 
uint8_t val2bcd_8(uint8_t val)
{
	return ((val/10)<<4+(val%10));
} 

/**********************************************************************
functionName:uint16 val2bcd_16(uint16 val)
description:ֵ��BCD���ת��16λ
**********************************************************************/ 
uint16_t val2bcd_16(uint16_t val)
{
	uint16_t bcd=0x0000;
	uint16_t temp16u;
	uint8_t i;
	for(i=0;i<4;i++)
	{
		temp16u=val%10;
		temp16u<<=(i<<2);
		bcd|=temp16u;
		val/=10;
	}		
	return bcd;
} 

/**********************************************************************
functionName:ErrorStatus bcd2val_16(uint16_t bcd,uint16_t *val)
description:ֵ��BCD���ת��16λ
**********************************************************************/  
ErrorStatus bcd2val_16(uint16_t bcd,uint16_t *val)
{ 
    uint8_t i;
    uint16_t temp16u; 
    uint16_t temp_val=0; 
    for(i=0;i<4;i++) 
    { 
        temp_val*= 10; 
        temp16u=(bcd & 0xf000) >> 12;
        if(temp16u>9)
        return 	ERROR;  
        temp_val+=temp16u; 
        bcd <<= 4; 
    } 
    *val=temp_val; 
 	return SUCCESS;
} 

/**********************************************************************
functionName:uint32_t val2bcd_32(uint32_t val)
description:ֵ��BCD���ת��32λ
**********************************************************************/  
uint32_t val2bcd_32(uint32_t val)
{
	uint32_t bcd=0x00000000;
	uint32_t temp32u;
	uint8_t i;
	for(i=0;i<8;i++) 
	{
		temp32u=val%10;
		temp32u<<=(i<<2);
		bcd|=temp32u;
		val/=10; 
	}
	return bcd;
}

/**********************************************************************
functionName:ErrorStatus bcd2val_32(uint32_t bcd,uint32_t *val) 
description:BCD�뵽ֵ��ת��32λ
**********************************************************************/  
ErrorStatus bcd2val_32(uint32_t bcd,uint32_t *val) 
{ 
    uint8_t i;    
    uint32_t temp32u; 
    uint32_t temp_val=0; 
    for(i=0;i<8;i++) 
    { 
        temp_val*= 10; 
        temp32u=(bcd & 0xf0000000) >> 28;
        if(temp32u>9)
        return 	ERROR; 
        temp_val+=temp32u; 
        bcd <<= 4; 
    } 
    *val=temp_val;
    return SUCCESS;
}

/**********************************************************************
functionName:ErrorStatus bcd2str_16(uint16_t bcd,char *str)
description:BCD�뵽�ַ�����ת��16λ
**********************************************************************/  
ErrorStatus bcd2str_16(uint16_t bcd,char *str)
{
	char *s;
	char tempc;
	uint8_t i;
	s=str;
	for(i=0;i<4;i++)
	{
		tempc=(char)(bcd>>12)+'0';
		if(tempc>'9')
		return 	ERROR;
		bcd<<=4;
		*s=tempc;
		s++;
	}
	*s=0;
	return SUCCESS;
} 

/**********************************************************************
functionName:ErrorStatus bcd2str_32(uint32_t bcd,char *str)
description:BCD�뵽�ַ�����ת��16λ
**********************************************************************/  
ErrorStatus bcd2str_32(uint32_t bcd,char *str)
{
	char *s;
	char tempc;
	uint8_t i;
	s=str;
	for(i=0;i<8;i++)
	{
		tempc=(char)(bcd>>28)+'0';
		if(tempc>'9')
		return 	ERROR;
		bcd<<=4;
		*s=tempc;
		s++;
	}
	*s=0;
	return SUCCESS;
}

const char hex_code[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/**********************************************************************
functionName:void hex2str_16(uint16_t hex,char *str)
description: ��32λ������16���Ƶķ�ʽת�����ַ���
**********************************************************************/  
void hex2str_32(uint32_t hex,char *str)
{	
	uint8_t i;
	char *s;
	s=str;
	for(i=0;i<8;i++)
	{ 
		*s=hex_code[(hex>>28)&0x0000000f];
		hex<<=4;
		s++;
	}
	*s=0;
}

/**********************************************************************
functionName:void hex2str_16(uint16_t hex,char *str)
description: ��16λ������16���Ƶķ�ʽת�����ַ���
**********************************************************************/  
void hex2str_16(uint16_t hex,char *str)
{	
	uint8_t i;
	char *s;
	s=str;
	for(i=0;i<4;i++)
	{ 
		*s=hex_code[(hex>>12)&0x000f];
		hex<<=4;
		s++;
	}
	*s=0;
}

/**********************************************************************
functionName:void hex2str_8(uint08 hex,char *str)
description: ��8λ������16���Ƶķ�ʽת�����ַ���
**********************************************************************/  
void hex2str_8(uint8_t hex,char *str)
{ 
	str[0]=hex_code[hex>>4];
	str[1]=hex_code[hex&0x0F];
	str[2]=0;
}


/**********************************************************************
functionName:ErrorStatus str2bcd_32(uint32 *bcd,char *str)
description:���ַ���ת��Ϊbcd�룬����ַ�����8���ַ����Ϸ�������ERROR
**********************************************************************/  
ErrorStatus str2bcd_32(uint32_t *bcd,char *str)
{
	uint8_t i;
	char tempc;
	*bcd=0x00000000; 
	for(i=0;i<8;i++)
	{
		if(str[i]>='0'&&(str[i]<='9'))
		{
			tempc=str[i]-'0';
			*bcd<<=4;
			*bcd+=tempc;
		}
		else
		return ERROR;  	
	}
	return SUCCESS;
}

/**********************************************************************
functionName:TestStatus  buffcmp(uint08 *buff1,uint08 *buff2,uint16 len)
description:�Ƚ�ָ�����ȵ����������Ƿ���ͬ
**********************************************************************/	 
TestStatus  buffcmp(uint8_t *buff1,uint8_t *buff2,uint16_t len)
{ 
	while(len--)
	{
		if(*buff1!=*buff2)
		return FAILED;	
		buff1++;
		buff2++;
	}
	return PASSED;
} 


/**********************************************************************
functionName:uint16 float2str(float f,char *str,uint08 int_len,uint08 dec_len)
description: ������ʵ�ֵ�Ŀ�ģ�
1���Ѹ�����ת����ָ���������ȵģ�ָ��С�����ȵ��ַ������м��С����
2���������ֲ��ܴ���6λ������������ʾΪ6λ
3���������ֲ���С��2λ������������ʾΪ2λ.
4��С�����ֲ��ܴ���3λ������С����ʾΪ3λ.
5�����С������Ϊ0��ֻ��ʾ��������
6�����ʵ����������>ָ���������ȣ���ת��С�����֣�ֻ��ʾ��������,
7���������ֳ������С��ָ�����ȣ�ǰ�油�Ͽո�
8�����������0~��-1��֮���С����������������2λ��0�͡�-����
9��С�����ֻ����3λ
10����������������ִ���999999������С��-99999������ʾ------
11��С�����־��������������ʾ
**********************************************************************/ 
uint16_t float2str(float f,char *str,uint8_t int_len,uint8_t dec_len)
{
	uint16_t cnt;
	uint16_t cnt_all;
	int32_t intg;		
	int32_t dec;
	char format[4];
	uint8_t i=0;   
	char str_buff[51] = {0};
	
	if(int_len>6)
	int_len=6;	
	else if(int_len<2)
	int_len=2;		 
	format[0]='%';
	format[1]=int_len+'0'; 
	format[2]='d';					
	format[3]=0;	//��������
	
	if((f>999999.5f)||(f<-99999.5f))		//����ܴ��������
	{
		for(i=0;i<(int_len+dec_len+1);i++)
		*(str_buff+i)='-';	 
		*(str_buff+i)=0;
		strcpy(str,str_buff);
		
		return(int_len+dec_len+1);
	}
	switch(dec_len)
	{ 	
		case 0:
		{
			if(f>0) 
			f+=0.501f;
			else 
			f-=0.501f;
			
			intg=(int32_t)f; 
			cnt=usprintf(str_buff,format,intg);
			strcpy(str,str_buff);
			
			return cnt;
		}	
		case 1:
		{
			if(f>0) 
			f+=0.0501f;
			else 
			f-=0.0501f;
			break;
		}	
		case 2:
		{
			if(f>0) 
			f+=0.00501f;
			else 
			f-=0.00501f;
			break;
		}
		default:
		{
			dec_len=3;
			if(f>0) 
			f+=0.000501f;
			else
			f-=0.000501f;
			break;
		}
	}	
	intg=(int32_t)f;	
	dec=fabs((f-intg))*1000;				//С������*1000   	 	
	cnt=usprintf(str_buff,format,intg);
	if(cnt>int_len)		//ֻ��ʾ��������
	{
		format[1]=int_len+1+dec_len+'0'; 
		cnt=usprintf(str_buff,format,intg);
		strcpy(str,str_buff);
		
		return cnt;
	} 
	if((f<0)&&(!intg)) 		//0~��-1�������ּӡ�-����
	str_buff[int_len-2]='-';    
	str_buff[cnt]='.'; 
	cnt_all=usprintf(&(str_buff[cnt+1]),"%03d",dec); 
	cnt_all+=cnt+1;
	switch(dec_len)
	{ 	
		case 1:
		{
			cnt_all-=2;
			str_buff[cnt_all]=0;
			break;
		}	
		case 2:
		{ 
			cnt_all-=1;
			str_buff[cnt_all]=0;
			break;
		}
		default:	 
		break;
	} 
	strcpy(str,str_buff);
	
	return cnt_all;
} 


/**********************************************************************
functionName:float str2float(char *str)
description: �ַ���ת��Ϊ�������ֵĺ���
**********************************************************************/ 
float str2float(char *str)
{
	uint32_t dec;
	int32_t intg;
	const char *ppc[1];
	char *s=str; 
	float f; 
	uint8_t i=0;
	uint8_t flag;
	char buff[4];
	intg=ustrtoul(str,ppc,10);
	if(**ppc!='.')
	return (float)intg;
	else
	{
		while(*s!='.')		
		*s++;
		*s++;
		flag=0;
		for(i=0;i<3;i++)
		{
			buff[i]=s[i];	
			if(!flag)
			{
				if((buff[i]<'0')||(buff[i]>'9'))
				{
					flag=1;
					buff[i]='0';	
				}	
			}	
			else
			buff[i]='0';	
		} 
		buff[3]=0;
		dec=ustrtoul(buff,0,10);
	}	
	dec+=5;	
	dec/=10;

	if(str[0]=='-')
	f=(float)intg-dec/100.0f;	
	else 
	f=(float)intg+dec/100.0f; 
	return f; 
}


/******************* (C) COPYRIGHT 2012 XinGao Tech *****END OF FILE****/
