#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"
#include "SYS_Tick.h"

void Adc_Init(void);
void TempInit (void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
double GetTemp (u8 times);

#endif
