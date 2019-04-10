#ifndef __SPI_STM32_H__
#define __SPI_STM32_H__

#include "stm32f10x.h"
#include "SYS_Tick.h"
#include "GPIO_INIT.h"

void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
u8 SPI1_ReadByte(void);

#endif
