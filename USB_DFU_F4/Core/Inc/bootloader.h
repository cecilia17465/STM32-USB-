#ifndef _BOOTLOADER_H
#define _BOOTLOADER_H

#include "stdint.h"
#include "stdio.h"

typedef void(*pfunction)(void);      //函数指针类型声明

void WFI_SET(void);                        
void INTX_DISABLE(void);                      
void INTX_ENABLE(void);                         
__asm void MSR_MSP(uint32_t addr);                   
void iap_jump(uint32_t addr);               
void bootloader_main(void);                           
uint16_t Xmodem_CRC16(uint8_t *data, uint16_t datalen);  

#endif
