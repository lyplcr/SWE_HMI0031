/**
  ******************************************************************************
  * @file    bsp_flash.c
  * @author  SY
  * @version V1.0.0
  * @date    2015-11-6 13:09:34
  * @IDE	 V4.70.0.0
  * @Chip    STM32F407VET6
  * @brief   芯片内部FLASH驱动文件
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private define ------------------------------------------------------------*/			
/* Private typedef -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t uwStartSector = 0;
uint32_t uwAddress = 0;

__IO uint32_t uwData32 = 0;


/* Private function prototypes -----------------------------------------------*/
static uint32_t GetSector(uint32_t Address);


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
ErrorStatus eep_erase( void )
{
	ErrorStatus Err = SUCCESS;
	
	/* Enable the flash control register access */
	FLASH_Unlock();

	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	/* Get the number of the start and end sectors */
	uwStartSector = GetSector(FLASH_USER_START_ADDR);

	/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
	   be done by word */ 
	if (FLASH_EraseSector(uwStartSector, VoltageRange_3) != FLASH_COMPLETE)
	{ 
	  /* Error occurred while sector erase. 
		 User can add here some code to deal with this error  */
		Err = ERROR;
	}
	
	FLASH_Lock(); 
	
	return Err;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
ErrorStatus eep_write( const uint8_t *data, uint32_t addr, uint16_t len )
{	
	uint32_t WriteData = 0;
	const uint8_t *pos = data;
	uint16_t len_offset = 0;
	uint8_t i = 0;
	
	if (!len)
	{
		return SUCCESS;
	}
	
	/* Unlock the Flash *********************************************************/
	/* Enable the flash control register access */
	FLASH_Unlock();

	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	uwAddress = 0;
	
	len_offset = len % 4;
	len -= len_offset;

	while (uwAddress < len)
	{
		WriteData = (pos[0] | (pos[1]<<8) | (pos[2]<<16) | (pos[3]<<24));
		
		if (FLASH_ProgramWord(FLASH_USER_START_ADDR + addr + uwAddress, WriteData) == FLASH_COMPLETE)
		{
			uwAddress += 4;
			pos += 4;
		}
		else
		{ 
		  /* Error occurred while writing data in Flash memory. 
			 User can add here some code to deal with this error */
			FLASH_Lock(); 
			return ERROR;
		}
	}
	
	if ( len_offset )
	{
		for (i=0; i<len_offset; ++i)
		{
			if (FLASH_ProgramByte(FLASH_USER_START_ADDR + addr + uwAddress, *pos) == FLASH_COMPLETE)
			{
				uwAddress++;
				pos++;
			}
			else
			{ 
			  /* Error occurred while writing data in Flash memory. 
				 User can add here some code to deal with this error */
				FLASH_Lock(); 
				return ERROR;
			}
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	 to protect the FLASH memory against possible unwanted operation) */
	FLASH_Lock(); 
	
	return SUCCESS;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
ErrorStatus eep_read( uint8_t *data, uint32_t addr, uint16_t len )
{
	uint16_t i;
	uint8_t *pos = data;
	uint16_t len_offset = 0;

	uwAddress = FLASH_USER_START_ADDR + addr;
	
	len_offset = len % 4;
	len -= len_offset;
	
	for (i=0; i<len; i+=4)
	{
		uwData32 = *(__IO uint32_t*)uwAddress;
		pos[i] 	 = (uint8_t)uwData32;
		pos[i+1] = (uint8_t)(uwData32 >> 8);
		pos[i+2] = (uint8_t)(uwData32 >> 16);
		pos[i+3] = (uint8_t)(uwData32 >> 24);
		
		uwAddress += 4;
	}
	
	if ( len_offset )
	{
		pos += len;
		
		for (i=0; i<len_offset; ++i)
		{
			*pos++ = *(__IO uint8_t*)uwAddress;
		}
	}
	
	return SUCCESS;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  
  return sector;
}

/*------------------------------------------------------------
 * Function Name  : WriteFlashWord
 * Description    : 向flash写入字
 * Input          : None
 * Output         : None
 * Return         : None
 *------------------------------------------------------------*/
TestStatus WriteFlashWord( const uint32_t WriteAddr, const uint32_t WriteData )
{
	TestStatus status = FAILED;
	
	/* Unlock the Flash *********************************************************/
	/* Enable the flash control register access */
	FLASH_Unlock();

	/* Erase the user Flash area ************************************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
				  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	/* Program the user Flash area word by word ********************************/
	/* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
	
	if (FLASH_ProgramWord(WriteAddr, WriteData) == FLASH_COMPLETE)
	{	
		status = PASSED;
	}
	
	/* Lock the Flash to disable the flash control register access (recommended
		 to protect the FLASH memory against possible unwanted operation) */
	FLASH_Lock(); 
	
	return status;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
