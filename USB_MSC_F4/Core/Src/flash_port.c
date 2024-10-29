#include "main.h"
#include "stm32f4xx_hal_flash.h"
#include "stdarg.h"
#include "flash_port.h"

#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  //16k
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  //16k
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  //16k
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  //16k
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  //64k
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  //128k
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  //128k
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  //128k
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  //128k
#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  //128k
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000)  //128k
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000)  //128k
#define ADDR_FLASH_SECTOR_12 ((uint32_t)0x08100000)  //16k
#define ADDR_FLASH_SECTOR_13 ((uint32_t)0x08104000)  //16k
#define ADDR_FLASH_SECTOR_14 ((uint32_t)0x08108000)  //16k
#define ADDR_FLASH_SECTOR_15 ((uint32_t)0x0810C000)  //16k
#define ADDR_FLASH_SECTOR_16 ((uint32_t)0x08110000)  //64k
#define ADDR_FLASH_SECTOR_17 ((uint32_t)0x08120000)  //128k
#define ADDR_FLASH_SECTOR_18 ((uint32_t)0x08140000)  //128k
#define ADDR_FLASH_SECTOR_19 ((uint32_t)0x08160000)  //128k
#define ADDR_FLASH_SECTOR_20 ((uint32_t)0x08180000)  //128k
#define ADDR_FLASH_SECTOR_21 ((uint32_t)0x081A0000)  //128k
#define ADDR_FLASH_SECTOR_22 ((uint32_t)0x081C0000)  //128k
#define ADDR_FLASH_SECTOR_23 ((uint32_t)0x081E0000)  //128k

static uint32_t stm32_get_sector(uint32_t address)
{
	uint32_t sector = 0;
	if((address < ADDR_FLASH_SECTOR_1) && (address > ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((address < ADDR_FLASH_SECTOR_2) && (address > ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((address < ADDR_FLASH_SECTOR_3) && (address > ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;
	}	
	else if((address < ADDR_FLASH_SECTOR_4) && (address > ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((address < ADDR_FLASH_SECTOR_5) && (address > ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((address < ADDR_FLASH_SECTOR_6) && (address > ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((address < ADDR_FLASH_SECTOR_7) && (address > ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;
	}
	else if((address < ADDR_FLASH_SECTOR_8) && (address > ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;
	}
	else if((address < ADDR_FLASH_SECTOR_9) && (address > ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;
	}
	else if((address < ADDR_FLASH_SECTOR_10)&& (address > ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_SECTOR_9;
	}
	else if((address < ADDR_FLASH_SECTOR_11)&& (address > ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;
	}
	else if((address < ADDR_FLASH_SECTOR_12)&& (address > ADDR_FLASH_SECTOR_11))
	{
		sector = FLASH_SECTOR_11;
	}	
	else
	{
		sector = FLASH_SECTOR_11;
	}
	return sector;
}

static uint32_t stm32_get_sector_size(uint32_t sector)
{
	switch(sector)
	{
		case FLASH_SECTOR_0:
			return 16*1024;
		case FLASH_SECTOR_1:
			return 16*1024;
		case FLASH_SECTOR_2:
			return 16*1024;
		case FLASH_SECTOR_3:
			return 16*1024;
		case FLASH_SECTOR_4:
			return 64*1024;
		case FLASH_SECTOR_5:
			return 128*1024;
		case FLASH_SECTOR_6:
			return 128*1024;
		case FLASH_SECTOR_7:
			return 128*1024;
		case FLASH_SECTOR_8:
			return 128*1024;
		case FLASH_SECTOR_9:
			return 128*1024;
		case FLASH_SECTOR_10:
			return 128*1024;
		case FLASH_SECTOR_11:
			return 128*1024;
		default:
			return 128*1024;
	}
}

int flash_port_read(uint32_t addr,uint32_t *buf,size_t size)
{
	int result = 0;
	uint8_t *buf_8 = (uint8_t *)buf;
	size_t i;
	for(i = 0; i < size; i++, addr++, buf_8++)
	{
		*buf_8 = *(uint8_t *)addr;
	}
	return result;
}

int flash_port_erase(uint32_t addr,size_t size)
{
	int result = 0;
	
	HAL_StatusTypeDef flash_status = HAL_OK;
	size_t erased_size = 0;
	uint32_t cur_erase_sector;
	
	HAL_FLASH_Unlock();
	
	while(erased_size < size)
	{
		cur_erase_sector = stm32_get_sector(addr + erased_size);
		FLASH_Erase_Sector(cur_erase_sector,FLASH_VOLTAGE_RANGE_3);
		if(flash_status != HAL_OK)
		{
			result = -1;
			break;
		}
		erased_size += stm32_get_sector_size(cur_erase_sector);
	}
	HAL_FLASH_Lock();
	return result;
}

int flash_port_write(uint32_t addr,const uint32_t *buf,size_t size)
{
	int result = 0;
	
	size_t i;
	uint32_t read_data;
	uint8_t *buf_8 = (uint8_t *)buf;
	
	HAL_FLASH_Unlock();
	
	for(i = 0; i < size; i++, addr++, buf_8++)
	{
		/*write*/
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr, *buf_8);
		read_data = *(uint8_t *)addr;
		/*check*/
		if(read_data != *buf_8)
		{
			result = -1;
			break;
		}
	}
	HAL_FLASH_Lock();
	return result;
}




