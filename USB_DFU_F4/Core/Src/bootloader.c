#include "main.h"
#include "stdint.h"
#include "stdio.h"

#define STM32_APP_BASE 0x08010000

typedef void (*pfunction)(void);
pfunction reset_handler;

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
	__ASM volatile("wfi");
}
//关闭所有中断，不包括fault和NMI
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");
}
//设置栈顶地址
//addr：栈顶地址
__asm void MSR_MSP(uint32_t addr)
{
	MSR MSP, r0    //set main stack value
                BX r14
}

void iap_jump(uint32_t addr) 
{
	printf("[%s] %d: \r\n",__func__,__LINE__);
	//栈顶地址检查
	if(((*(uint32_t *)addr) & 0xff000000) == 0x20000000)
	{
		printf("[%s] %d: %x\r\n",__func__,__LINE__,((*(uint32_t *)addr) & 0xff000000));
		
		if(((*(uint32_t *)(addr+4)) & 0x0fff0000) == addr)
		{
			HAL_Delay(100);
			reset_handler = (pfunction)((*(uint32_t *)(addr+4)));
			printf("[%s] %d: reset_Handler = 0x%x\r\n",__func__,__LINE__, reset_handler);
			MSR_MSP(*(uint32_t *)addr);
			
			printf("[%s] %d: reset!!! \r\n",__func__,__LINE__);
            reset_handler();
		}
	}
	else
	{
		printf("[%s] %d:err \r\n",__func__,__LINE__);
	}
}

extern void usb_stop(void);

int boot_flag = 0;

void bootloader_main(void)
{
	HAL_Delay(500);
	printf("[%s] %d: \r\n",__func__,__LINE__);
    
//	HAL_Delay(3000);
//	usb_stop();
//	HAL_Delay(500);
//	iap_jump(STM32_APP_BASE);
	
	printf("[%s] %d: =====================================\r\n",__func__,__LINE__);
	printf("[%s] %d: ======== waiting for Upgrade ========\r\n",__func__,__LINE__);
	printf("[%s] %d: =====================================\r\n",__func__,__LINE__);
	
	while(1)
	{
		HAL_Delay(500);
		if(boot_flag)
		{
			usb_stop();
			HAL_Delay(500);
			
			printf("[%s] %d: =====================================\r\n",__func__,__LINE__);
			printf("[%s] %d: ======== Upgrade   Complete  ========\r\n",__func__,__LINE__);
			printf("[%s] %d: =====================================\r\n",__func__,__LINE__);
			
			iap_jump(STM32_APP_BASE);
		}
	}
}


/*-------------------------------------------------*/
/*函数名：Xmodem_CRC16校验                         */
/*参  数：data：数据指针  datalen：数据长度        */
/*返回值：校验后的数据                             */
/*-------------------------------------------------*/
uint16_t Xmodem_CRC16(uint8_t *data, uint16_t datalen){
	
	uint8_t i;                                               //用于for循环
	uint16_t Crcinit = 0x0000;                               //Xmdoem CRC校验的初始值，必须是0x0000
	uint16_t Crcipoly = 0x1021;                              //Xmdoem CRC校验的多项式，必须是0x1021
	
	while(datalen--){                                        //根据datalen大小，有多少字节循环多少次
		Crcinit = (*data << 8) ^ Crcinit;                    //先将带校验的字节，挪到高8位
		for(i=0;i<8;i++){                                    //每个字节8个二进制位，循环8次
			if(Crcinit&0x8000)                               //判断BIT15是1还是0,是1的话，进入if
				Crcinit = (Crcinit << 1) ^ Crcipoly;         //是1的话，先左移，再异或多项式
			else                                             //判断BIT15是1还是0,是0的话，进入else
				Crcinit = (Crcinit << 1);                    //是0的话，只左移
		}
		data++;                                              //下移，计算一个字节数据
	}
	return Crcinit;                                          //返回校验后的数据
}
