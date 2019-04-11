#ifndef _GENERIC_SETHEADFILE_H_
#define _GENERIC_SETHEADFILE_H_
#include "Generic_MicroControlerType.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
	typedef struct {
			unsigned int SystemClock;
		
		
		
	}MicroControlerInfo;
	
//#define USE_MICROLIB

//#define NDEBUG

#ifndef USE_MICROLIB
	#include <assert.h>
#endif

	
#if defined(GENERIC_STM32F10X)
	#include "stm32f10x.h"
	#include "Generic_GPIO.h"
	#include "Generic_Delay.h"

	//#include "Generic_GPIO.h"
#endif

#endif

