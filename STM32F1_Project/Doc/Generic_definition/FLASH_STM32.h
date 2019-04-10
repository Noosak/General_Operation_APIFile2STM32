#ifndef _FLASH_STM32_H_
#define _FLASH_STM32_H_

#include "SYS_Tick.h"

u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
void STMFLASH_Write_HalfWord(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
void STMFLASH_Change(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
#endif
