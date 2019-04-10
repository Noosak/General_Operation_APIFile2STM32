#ifndef __W25QXX_H__
#define __W25QXX_H__



#include "stm32f10x.h"
#include "GPIO_INIT.h"
#include "SPI_STM32.h"

u32 W25QXX_Init(u8 CSGPIOx,u32 CS_PIN);
void W25QXX_SectorErase(u32 SectorAddr);
void W25QXX_BulkErase(void);
void W25QXX_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void W25QXX_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void W25QXX_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32  W25QXX_ReadID(void);
u32  W25QXX_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void W25QXX_PowerDown(void);
void W25QXX_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void W25QXX_WriteEnable(void);
void W25QXX_WaitForWriteEnd(void);

void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

#endif /* __SPI_FLASH_H */

