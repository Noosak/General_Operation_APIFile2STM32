#ifndef _Generic_GPIO_H_
#define _Generic_GPIO_H_
#include "Generic_SetHeadFile.h"
enum {
	LOW,
	HIGH,
}; 

typedef enum {
	Generic_GPIOA,
	Generic_GPIOB,
	Generic_GPIOC,
	Generic_GPIOD,
	Generic_GPIOE,
	Generic_GPIOF,
	Generic_GPIOG,
}Generic_GPIOType;

typedef enum {

	Generic_GPIO_Pin_0=0x0001,  	/*!< Pin 0 selected */
	Generic_GPIO_Pin_1=0x0002,  	/*!< Pin 1 selected */
	Generic_GPIO_Pin_2=0x0004, 		/*!< Pin 2 selected */
	Generic_GPIO_Pin_3=0x0008,  	/*!< Pin 3 selected */
	Generic_GPIO_Pin_4=0x0010,  	/*!< Pin 4 selected */
	Generic_GPIO_Pin_5=0x0020,  	/*!< Pin 5 selected */
	Generic_GPIO_Pin_6=0x0040,  	/*!< Pin 6 selected */
	Generic_GPIO_Pin_7=0x0080,  	/*!< Pin 7 selected */
	Generic_GPIO_Pin_8=0x0100,  	/*!< Pin 8 selected */
	Generic_GPIO_Pin_9=0x0200, 		/*!< Pin 9 selected */
	Generic_GPIO_Pin_10=0x0400,  	/*!< Pin 10 selected */
	Generic_GPIO_Pin_11=0x0800,  	/*!< Pin 11 selected */
	Generic_GPIO_Pin_12=0x1000,  	/*!< Pin 12 selected */
	Generic_GPIO_Pin_13=0x2000, 	/*!< Pin 13 selected */
	Generic_GPIO_Pin_14=0x4000,  	/*!< Pin 14 selected */
	Generic_GPIO_Pin_15=0x8000,  	/*!< Pin 15 selected */
	Generic_GPIO_Pin_All=0xFFFF,  /*!< All pins selected */
}Generic_GPIN_PIN;

#ifdef GENERIC_STM32F10X
typedef enum {
	Generic_OUTPUT_PP,
	Generic_OUTPUT_OD,
	Generic_OUTPUT_AFPP,
	Generic_OUTPUT_AFOD,
	Generic_INPUT,
	Generic_INPUT_AN,
	Generic_INPUT_IPD,
	Generic_INPUT_IPU,
}Generic_GPIOMODE;

typedef enum {
	Generic_Speed_2MHz,
	Generic_Speed_10MHz,
	Generic_Speed_50MHz,
}Generic_GPIOSpeed;
#endif

typedef struct {
	uint8_t Generic_GPIOType;
	Generic_GPIN_PIN Generic_GPIN_PIN;
	Generic_GPIOMODE Generic_GPIOMODE;
	Generic_GPIOSpeed Generic_GPIOSpeed;
}Generic_GPIOInit;

void Generic_pinMode(Generic_GPIOInit* GPIOINITSTRUCT );
void Generic_DigitalWrite(uint16_t GPIOType,uint32_t GPIOPIN,uint8_t PINStatus);
uint8_t Generic_DigitalRead(uint16_t GPIOType,uint32_t GPIOPIN);
uint16_t Generic_DigitalReadGroup(uint16_t GPIOType);

#endif
