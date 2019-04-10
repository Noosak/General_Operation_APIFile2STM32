#ifndef _BH1750FVI_STM32_H_
#define _BH1750FVI_STM32_H_

#include "IIC_STM32.h"

#define	  BH1750FVI_Address_LOW   0x46 
#define	  BH1750FVI_Address_HIGH   0xB8 

#define	  BH1750FVI_PowerDown   0x00 
#define	  BH1750FVI_PowerOn   0x01 
#define	  BH1750FVI_Reset   0x07 
#define	  BH1750FVI_H_Resolution   0x10              //1LX   120MS
#define	  BH1750FVI_H_Resolution_2   0x11						 //0.5lx 120MS
#define	  BH1750FVI_L_Resolution   0x13              //41LX  16MS
#define	  BH1750FVI_H_Resolution_Onetime   0x20 			
#define	  BH1750FVI_H_Resolution_2_Onetime   0x21
#define	  BH1750FVI_L_Resolution_Onetime    0x23 

void BH1750FVI_Init(u8 GPIOx,u32 SCLGPIOPINx,u32 SDAGPIOPINx);
void BH1750_Mode(u8 Dev,u8 REG_Address);
u8 BH1750_Read(u8 Dev,u8 length,u8 *data);
u16 BH1750GetData(u8 Dev,u8 GPIOx,u32 SCLGPIOPINx,u32 SDAGPIOPINx);
#endif
