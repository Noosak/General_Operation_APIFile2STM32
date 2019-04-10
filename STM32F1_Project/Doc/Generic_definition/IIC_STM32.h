#ifndef _IIC_STM32_H_
#define _IIC_STM32_H_
#include "GPIO_INIT.h"
#include "stm32f10x.h"
#include "SYS_TICK.h"
typedef unsigned char u8;
typedef unsigned int u32;

void SendLine_IIC(u8 GPIOx,u32 NSCL,u32 NSDA);
void SendLine_HIGH(u32 line);
void SendLine_LOW(u32 line);
void SendSDAmode(u8 Mode);
u8 ReadSDA(void);

u8 IIC_Start(void);
void IIC_Stop(void);
int IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 TXData);
u8 i2cWrite(u8 addr, u8 reg, u8 len, u8 *data);
u8 IIC_Read_Byte(u8 ack);
u8 i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
u8 I2C_ReadOneByte(u8 I2C_Addr,u8 addr);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICreadByte(u8 dev, u8 reg, u8 *data);
u8 IICwriteByte(u8 dev, u8 reg, u8 data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data);

#endif
