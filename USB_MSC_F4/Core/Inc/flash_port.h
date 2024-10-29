#ifndef _FLASH_PORT_H
#define _FLASH_PORT_H

#include "stdint.h"
#include "stdio.h"

static uint32_t stm32_get_sector(uint32_t address);
static uint32_t stm32_get_sector_size(uint32_t sector);
int flash_port_read(uint32_t addr,uint32_t *buf,size_t size);
int flash_port_erase(uint32_t addr,size_t size);
int flash_port_write(uint32_t addr,const uint32_t *buf,size_t size);

#endif
