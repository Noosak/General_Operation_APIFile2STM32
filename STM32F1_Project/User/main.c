/**
  ******************************************************************************
  * @file    main.c
  * @author  Noosak
  * @version V1.0
  * @date    2016-08-20
  * @brief   工程模板
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "GPIO_INIT.h"
#include "SYS_Tick.h"
#include "USART.h"
#include "ADC.h"
#include "PWM.h"
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//TIM1_PWM_Init(20000,71);
	//TIM_SetCompare1(TIM1,600);
//PB4默认用作调试口，如果用作普通的IO，需要加上以下两句 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	//初始化电机控制
	delay_init(72);
	pinMode(GPIOa,GPIO_Pin_8,OUTPUT,Speed_50MHz);
	pinMode(GPIOb,GPIO_Pin_4|GPIO_Pin_3,OUTPUT,Speed_50MHz);
	pinMode(GPIOc,GPIO_Pin_13,OUTPUT,Speed_50MHz);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_3);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	while(1){
		unsigned char x;
		for(x=0;x<200;x++){
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			delay_us(500);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			delay_us(500);
		}
	}

}
