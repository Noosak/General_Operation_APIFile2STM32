#include "GPIO_INIT.h"

void pinMode(u8 GPIOx,u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOINIT;
	GPIOINIT.GPIO_Pin=GPIO_Pin;
	switch(MODE)
	{
		case 0:GPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;			 break;
		case 1:GPIOINIT.GPIO_Mode=GPIO_Mode_Out_OD;			 break;
		case 2:GPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;		 	 break;
		case 3:GPIOINIT.GPIO_Mode=GPIO_Mode_AF_OD;		 	 break;
		case 4:GPIOINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOINIT.GPIO_Mode=GPIO_Mode_AIN; 				 break;
		case 6:GPIOINIT.GPIO_Mode=GPIO_Mode_IPD; 				 break;
		case 7:GPIOINIT.GPIO_Mode=GPIO_Mode_IPU;     		 break;
	}
	switch(SPEED)
	{
		case 0:GPIOINIT.GPIO_Speed=GPIO_Speed_2MHz;  break;
		case 1:GPIOINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOINIT.GPIO_Speed=GPIO_Speed_50MHz; break;
	}
	switch(GPIOx)
	{
		case GPIOa:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);GPIO_Init(GPIOA,&GPIOINIT); break;
		case GPIOb:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);GPIO_Init(GPIOB,&GPIOINIT); break;
		case GPIOc:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);GPIO_Init(GPIOC,&GPIOINIT); break;
		case GPIOd:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);GPIO_Init(GPIOD,&GPIOINIT); break;
		case GPIOe:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);GPIO_Init(GPIOE,&GPIOINIT); break;
		case GPIOf:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);GPIO_Init(GPIOF,&GPIOINIT); break;
		case GPIOg:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);GPIO_Init(GPIOG,&GPIOINIT); break;
	}
} 
/*
void pinAMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOAINIT;
	GPIOAINIT.GPIO_Pin=GPIO_Pin;
	switch(MODE)
	{
		case 0:GPIOAINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOAINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOAINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOAINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOAINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOAINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOAINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOAINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOAINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOAINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOAINIT.GPIO_Speed=GPIO_Speed_50MHz; break;
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Init(GPIOA,&GPIOAINIT);
}

void pinBMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOBINIT;
	GPIOBINIT.GPIO_Pin=GPIO_Pin;
	switch(MODE)
	{
		case 0:GPIOBINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOBINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOBINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOBINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOBINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOBINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOBINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOBINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOBINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOBINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOBINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_Init(GPIOB,&GPIOBINIT);
}

void pinCMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOCINIT;
	GPIOCINIT.GPIO_Pin=GPIO_Pin;	
	switch(MODE)
	{
		case 0:GPIOCINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOCINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOCINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOCINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOCINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOCINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOCINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOCINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOCINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOCINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOCINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_Init(GPIOC,&GPIOCINIT);
}

void pinDMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIODINIT;
	switch(GPIO_Pin)
	{
		case GPIO_Pin_0:GPIODINIT.GPIO_Pin=GPIO_Pin_0; break;
		case GPIO_Pin_1:GPIODINIT.GPIO_Pin=GPIO_Pin_1; break;
		case GPIO_Pin_2:GPIODINIT.GPIO_Pin=GPIO_Pin_2; break;
		case GPIO_Pin_3:GPIODINIT.GPIO_Pin=GPIO_Pin_3; break;
		case GPIO_Pin_4:GPIODINIT.GPIO_Pin=GPIO_Pin_4; break;
		case GPIO_Pin_5:GPIODINIT.GPIO_Pin=GPIO_Pin_5; break;
		case GPIO_Pin_6:GPIODINIT.GPIO_Pin=GPIO_Pin_6; break;
		case GPIO_Pin_7:GPIODINIT.GPIO_Pin=GPIO_Pin_7; break;
		case GPIO_Pin_8:GPIODINIT.GPIO_Pin=GPIO_Pin_8; break;
		case GPIO_Pin_9:GPIODINIT.GPIO_Pin=GPIO_Pin_9; break;
		case GPIO_Pin_10:GPIODINIT.GPIO_Pin=GPIO_Pin_10; break;
		case GPIO_Pin_11:GPIODINIT.GPIO_Pin=GPIO_Pin_11; break;
		case GPIO_Pin_12:GPIODINIT.GPIO_Pin=GPIO_Pin_12; break;
		case GPIO_Pin_13:GPIODINIT.GPIO_Pin=GPIO_Pin_13; break;
		case GPIO_Pin_14:GPIODINIT.GPIO_Pin=GPIO_Pin_14; break;
		case GPIO_Pin_15:GPIODINIT.GPIO_Pin=GPIO_Pin_15; break;
		case GPIO_Pin_All:GPIODINIT.GPIO_Pin=GPIO_Pin_All; break;
	}
	switch(MODE)
	{
		case 0:GPIODINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIODINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIODINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIODINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIODINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIODINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIODINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIODINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIODINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIODINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIODINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_Init(GPIOD,&GPIODINIT);
}

void pinEMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOEINIT;
	switch(GPIO_Pin)
	{
		case GPIO_Pin_0:GPIOEINIT.GPIO_Pin=GPIO_Pin_0; break;
		case GPIO_Pin_1:GPIOEINIT.GPIO_Pin=GPIO_Pin_1; break;
		case GPIO_Pin_2:GPIOEINIT.GPIO_Pin=GPIO_Pin_2; break;
		case GPIO_Pin_3:GPIOEINIT.GPIO_Pin=GPIO_Pin_3; break;
		case GPIO_Pin_4:GPIOEINIT.GPIO_Pin=GPIO_Pin_4; break;
		case GPIO_Pin_5:GPIOEINIT.GPIO_Pin=GPIO_Pin_5; break;
		case GPIO_Pin_6:GPIOEINIT.GPIO_Pin=GPIO_Pin_6; break;
		case GPIO_Pin_7:GPIOEINIT.GPIO_Pin=GPIO_Pin_7; break;
		case GPIO_Pin_8:GPIOEINIT.GPIO_Pin=GPIO_Pin_8; break;
		case GPIO_Pin_9:GPIOEINIT.GPIO_Pin=GPIO_Pin_9; break;
		case GPIO_Pin_10:GPIOEINIT.GPIO_Pin=GPIO_Pin_10; break;
		case GPIO_Pin_11:GPIOEINIT.GPIO_Pin=GPIO_Pin_11; break;
		case GPIO_Pin_12:GPIOEINIT.GPIO_Pin=GPIO_Pin_12; break;
		case GPIO_Pin_13:GPIOEINIT.GPIO_Pin=GPIO_Pin_13; break;
		case GPIO_Pin_14:GPIOEINIT.GPIO_Pin=GPIO_Pin_14; break;
		case GPIO_Pin_15:GPIOEINIT.GPIO_Pin=GPIO_Pin_15; break;
		case GPIO_Pin_All:GPIOEINIT.GPIO_Pin=GPIO_Pin_All; break;
	}
	switch(MODE)
	{
		case 0:GPIOEINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOEINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOEINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOEINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOEINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOEINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOEINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOEINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOEINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOEINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOEINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_Init(GPIOE,&GPIOEINIT);
}

void pinFMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOFINIT;
	switch(GPIO_Pin)
	{
		case GPIO_Pin_0:GPIOFINIT.GPIO_Pin=GPIO_Pin_0; break;
		case GPIO_Pin_1:GPIOFINIT.GPIO_Pin=GPIO_Pin_1; break;
		case GPIO_Pin_2:GPIOFINIT.GPIO_Pin=GPIO_Pin_2; break;
		case GPIO_Pin_3:GPIOFINIT.GPIO_Pin=GPIO_Pin_3; break;
		case GPIO_Pin_4:GPIOFINIT.GPIO_Pin=GPIO_Pin_4; break;
		case GPIO_Pin_5:GPIOFINIT.GPIO_Pin=GPIO_Pin_5; break;
		case GPIO_Pin_6:GPIOFINIT.GPIO_Pin=GPIO_Pin_6; break;
		case GPIO_Pin_7:GPIOFINIT.GPIO_Pin=GPIO_Pin_7; break;
		case GPIO_Pin_8:GPIOFINIT.GPIO_Pin=GPIO_Pin_8; break;
		case GPIO_Pin_9:GPIOFINIT.GPIO_Pin=GPIO_Pin_9; break;
		case GPIO_Pin_10:GPIOFINIT.GPIO_Pin=GPIO_Pin_10; break;
		case GPIO_Pin_11:GPIOFINIT.GPIO_Pin=GPIO_Pin_11; break;
		case GPIO_Pin_12:GPIOFINIT.GPIO_Pin=GPIO_Pin_12; break;
		case GPIO_Pin_13:GPIOFINIT.GPIO_Pin=GPIO_Pin_13; break;
		case GPIO_Pin_14:GPIOFINIT.GPIO_Pin=GPIO_Pin_14; break;
		case GPIO_Pin_15:GPIOFINIT.GPIO_Pin=GPIO_Pin_15; break;
		case GPIO_Pin_All:GPIOFINIT.GPIO_Pin=GPIO_Pin_All; break;
	}
	switch(MODE)
	{
		case 0:GPIOFINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOFINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOFINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOFINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOFINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOFINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOFINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOFINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOFINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOFINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOFINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_Init(GPIOF,&GPIOFINIT);
}

void pinGMode (u32 GPIO_Pin,u8 MODE,u8 SPEED)
{
	GPIO_InitTypeDef GPIOGINIT;
	switch(GPIO_Pin)
	{
		case GPIO_Pin_0:GPIOGINIT.GPIO_Pin=GPIO_Pin_0; break;
		case GPIO_Pin_1:GPIOGINIT.GPIO_Pin=GPIO_Pin_1; break;
		case GPIO_Pin_2:GPIOGINIT.GPIO_Pin=GPIO_Pin_2; break;
		case GPIO_Pin_3:GPIOGINIT.GPIO_Pin=GPIO_Pin_3; break;
		case GPIO_Pin_4:GPIOGINIT.GPIO_Pin=GPIO_Pin_4; break;
		case GPIO_Pin_5:GPIOGINIT.GPIO_Pin=GPIO_Pin_5; break;
		case GPIO_Pin_6:GPIOGINIT.GPIO_Pin=GPIO_Pin_6; break;
		case GPIO_Pin_7:GPIOGINIT.GPIO_Pin=GPIO_Pin_7; break;
		case GPIO_Pin_8:GPIOGINIT.GPIO_Pin=GPIO_Pin_8; break;
		case GPIO_Pin_9:GPIOGINIT.GPIO_Pin=GPIO_Pin_9; break;
		case GPIO_Pin_10:GPIOGINIT.GPIO_Pin=GPIO_Pin_10; break;
		case GPIO_Pin_11:GPIOGINIT.GPIO_Pin=GPIO_Pin_11; break;
		case GPIO_Pin_12:GPIOGINIT.GPIO_Pin=GPIO_Pin_12; break;
		case GPIO_Pin_13:GPIOGINIT.GPIO_Pin=GPIO_Pin_13; break;
		case GPIO_Pin_14:GPIOGINIT.GPIO_Pin=GPIO_Pin_14; break;
		case GPIO_Pin_15:GPIOGINIT.GPIO_Pin=GPIO_Pin_15; break;
		case GPIO_Pin_All:GPIOGINIT.GPIO_Pin=GPIO_Pin_All; break;
	}
	switch(MODE)
	{
		case 0:GPIOGINIT.GPIO_Mode=GPIO_Mode_Out_PP; break;
		case 1:GPIOGINIT.GPIO_Mode=GPIO_Mode_Out_OD; break;
		case 2:GPIOGINIT.GPIO_Mode=GPIO_Mode_AF_PP; break;
		case 3:GPIOGINIT.GPIO_Mode=GPIO_Mode_AF_OD; break;
		case 4:GPIOGINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOGINIT.GPIO_Mode=GPIO_Mode_AIN; break;
		case 6:GPIOGINIT.GPIO_Mode=GPIO_Mode_IPD; break;
		case 7:GPIOGINIT.GPIO_Mode=GPIO_Mode_IPU; break;
	}
	switch(SPEED)
	{
		case 0:GPIOGINIT.GPIO_Speed=GPIO_Speed_2MHz; break;
		case 1:GPIOGINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOGINIT.GPIO_Speed=GPIO_Speed_50MHz; break;

	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_Init(GPIOG,&GPIOGINIT);
}
*/
