/**
  ******************************************************************************
  * @file    main.c
  * @author  Noosak
  * @version V1.0
  * @date    2016-08-20
  * @detell   ¹¤³ÌÄ£°å
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "Generic_SetHeadFile.h"

int main()
{
	Generic_GPIOInit GPIOAinit;
	MicroControlerInfo* STM32F103C8INFO;
	STM32F103C8INFO->SystemClock=72000000;
	Generic_DelayInit(STM32F103C8INFO);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIOAinit.Generic_GPIOType=Generic_GPIOA;
	GPIOAinit.Generic_GPIN_PIN=Generic_GPIO_Pin_15;
	GPIOAinit.Generic_GPIOMODE=Generic_OUTPUT_PP;
	GPIOAinit.Generic_GPIOSpeed=Generic_Speed_50MHz;
	Generic_pinMode(&GPIOAinit);
	
	while(1){
		unsigned char x;
		for(x=0;x<200;x++){
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		}
	}

}
