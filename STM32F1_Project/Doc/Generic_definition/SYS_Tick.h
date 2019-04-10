#include "stm32f10x.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_xms(u16 nms);
void delay_ms(u16 nms);
