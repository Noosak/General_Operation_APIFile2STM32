#ifndef _PWM_H_
#define _PWM_H_
#include "stm32f10x.h"
#include "GPIO_INIT.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 psc,u16 arr);
#endif
