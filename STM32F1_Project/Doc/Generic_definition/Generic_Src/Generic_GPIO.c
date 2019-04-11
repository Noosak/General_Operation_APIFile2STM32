#include "Generic_GPIO.h"

/*******************************************************************************   
Function:       // 函数名
Description:    // 函数功能描述 
Calls:          // 被本调用函数清单  
Input:          // 输入参数说明 
Output:         // 输出参数说明  
Return:         // 函数返回值的说明   
Others:         // 其他说明 
*******************************************************************************/
void Generic_pinMode(Generic_GPIOInit* GPIOINITSTRUCT)
{
#ifdef GENERIC_STM32F10X
	GPIO_InitTypeDef GPIOINIT;
	
	GPIOINIT.GPIO_Pin=GPIOINITSTRUCT->Generic_GPIN_PIN;
	switch(GPIOINITSTRUCT->Generic_GPIOMODE){
		case 0:GPIOINIT.GPIO_Mode=GPIO_Mode_Out_PP;			 break;
		case 1:GPIOINIT.GPIO_Mode=GPIO_Mode_Out_OD;			 break;
		case 2:GPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;		 	 break;
		case 3:GPIOINIT.GPIO_Mode=GPIO_Mode_AF_OD;		 	 break;
		case 4:GPIOINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; break;
		case 5:GPIOINIT.GPIO_Mode=GPIO_Mode_AIN; 				 break;
		case 6:GPIOINIT.GPIO_Mode=GPIO_Mode_IPD; 				 break;
		case 7:GPIOINIT.GPIO_Mode=GPIO_Mode_IPU;     		 break;
	}
	switch(GPIOINITSTRUCT->Generic_GPIOSpeed){
		case 0:GPIOINIT.GPIO_Speed=GPIO_Speed_2MHz;  break;
		case 1:GPIOINIT.GPIO_Speed=GPIO_Speed_10MHz; break;
		case 2:GPIOINIT.GPIO_Speed=GPIO_Speed_50MHz; break;
	}
	switch(GPIOINITSTRUCT->Generic_GPIOType){
		case Generic_GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);GPIO_Init(GPIOA,&GPIOINIT); break;
		case Generic_GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);GPIO_Init(GPIOB,&GPIOINIT); break;
		case Generic_GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);GPIO_Init(GPIOC,&GPIOINIT); break;
		case Generic_GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);GPIO_Init(GPIOD,&GPIOINIT); break;
		case Generic_GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);GPIO_Init(GPIOE,&GPIOINIT); break;
		case Generic_GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);GPIO_Init(GPIOF,&GPIOINIT); break;
		case Generic_GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);GPIO_Init(GPIOG,&GPIOINIT); break;
	}
#endif
} 

void Generic_DigitalWrite(uint16_t GPIOType,uint32_t GPIOPIN,uint8_t PINStatus)
{
#ifdef GENERIC_STM32F10X
	switch(PINStatus){
		case HIGH :{
				switch(GPIOType){
					case Generic_GPIOA:GPIO_SetBits(GPIOA,GPIOPIN); break;
					case Generic_GPIOB:GPIO_SetBits(GPIOB,GPIOPIN); break;
					case Generic_GPIOC:GPIO_SetBits(GPIOC,GPIOPIN); break;
					case Generic_GPIOD:GPIO_SetBits(GPIOD,GPIOPIN); break;
					case Generic_GPIOE:GPIO_SetBits(GPIOE,GPIOPIN); break;
					case Generic_GPIOF:GPIO_SetBits(GPIOF,GPIOPIN); break;
					case Generic_GPIOG:GPIO_SetBits(GPIOG,GPIOPIN); break;
				}
		}break;
		case LOW:{
				switch(GPIOType){
					case Generic_GPIOA:GPIO_ResetBits(GPIOA,GPIOPIN); break;
					case Generic_GPIOB:GPIO_ResetBits(GPIOB,GPIOPIN); break;
					case Generic_GPIOC:GPIO_ResetBits(GPIOC,GPIOPIN); break;
					case Generic_GPIOD:GPIO_ResetBits(GPIOD,GPIOPIN); break;
					case Generic_GPIOE:GPIO_ResetBits(GPIOE,GPIOPIN); break;
					case Generic_GPIOF:GPIO_ResetBits(GPIOF,GPIOPIN); break;
					case Generic_GPIOG:GPIO_ResetBits(GPIOG,GPIOPIN); break;
				}
		}break;
	}
#endif
}

uint8_t Generic_DigitalRead(uint16_t GPIOType,uint32_t GPIOPIN)
{
#ifdef GENERIC_STM32F10X
	uint8_t temp;
	switch(GPIOType){
	case Generic_GPIOA:temp = GPIO_ReadInputDataBit(GPIOA,GPIOPIN);
	case Generic_GPIOB:temp = GPIO_ReadInputDataBit(GPIOB,GPIOPIN);
	case Generic_GPIOC:temp = GPIO_ReadInputDataBit(GPIOC,GPIOPIN);
	case Generic_GPIOD:temp = GPIO_ReadInputDataBit(GPIOD,GPIOPIN);
	case Generic_GPIOE:temp = GPIO_ReadInputDataBit(GPIOE,GPIOPIN);
	case Generic_GPIOF:temp = GPIO_ReadInputDataBit(GPIOF,GPIOPIN);
	case Generic_GPIOG:temp = GPIO_ReadInputDataBit(GPIOG,GPIOPIN);
	}
	return temp;
#endif
}

uint16_t Generic_DigitalReadGroup(uint16_t GPIOType)
{
#ifdef GENERIC_STM32F10X
	uint16_t temp;
	switch(GPIOType){
	case Generic_GPIOA:temp = GPIO_ReadInputData(GPIOA);
	case Generic_GPIOB:temp = GPIO_ReadInputData(GPIOB);
	case Generic_GPIOC:temp = GPIO_ReadInputData(GPIOC);
	case Generic_GPIOD:temp = GPIO_ReadInputData(GPIOD);
	case Generic_GPIOE:temp = GPIO_ReadInputData(GPIOE);
	case Generic_GPIOF:temp = GPIO_ReadInputData(GPIOF);
	case Generic_GPIOG:temp = GPIO_ReadInputData(GPIOG);
	}
	return temp;
#endif
}
