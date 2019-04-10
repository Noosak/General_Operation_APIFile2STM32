#ifndef __GPIO_INIT_H
#define __GPIO_INIT_H
#include "stm32f10x.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define OUTPUT 0
#define OUTPUT_OD 1
#define OUTPUT_AF 2
#define OUTPUT_AFOD 3
#define INPUT 4
#define INPUT_AN 5
#define INPUT_IPD 6
#define INPUT_IPU 7

#define Speed_2MHz 0
#define Speed_10MHz 1
#define Speed_50MHz 2

static u8 const GPIOa=0x30;
static u8 const GPIOb=0x31;
static u8 const GPIOc=0x32;
static u8 const GPIOd=0x33;
static u8 const GPIOe=0x34;
static u8 const GPIOf=0x35;
static u8 const GPIOg=0x36;

void pinMode(u8 GPIOx,u32 GPIO_Pin,u8 MODE,u8 SPEED);
/*
void pinAMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinBMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinCMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinDMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinEMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinFMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
void pinGMode (u32 GPIO_Pin,u8 MODE,u8 SPEED);
*/
#endif
