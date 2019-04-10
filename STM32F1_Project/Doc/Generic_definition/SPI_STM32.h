#ifndef __SPI_STM32_H__
#define __SPI_STM32_H__

#include "stm32f10x.h"
#include "SYS_Tick.h"
#include "GPIO_INIT.h"

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
u8 SPI1_ReadByte(void);

#endif
