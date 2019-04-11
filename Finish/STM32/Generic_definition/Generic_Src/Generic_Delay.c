#include "Generic_Delay.h"

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); Systick时钟选择
	
	//SysTick_Config(tick); tick<=LOAD寄存器大小=2^24-1  即设定重装值
	//SysTick_Handler() SysTick中断处理函数
	
	
static uint8_t  Pace_us=0;										//us延时节拍函数		   
static uint16_t Pace_ms=0;										//ms延时节拍数

void Generic_DelayInit(MicroControlerInfo* MCUInfo)
{
#ifdef GENERIC_STM32F10X
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	Pace_us=MCUInfo->SystemClock/8000000;				//对节拍数进行赋值以满足之后调用
	Pace_ms=(uint16_t)Pace_us*1000;	  					//对节拍数进行赋值以满足之后调用
#endif
}

void Generic_Delay_us(uint32_t nus)
{		
#ifdef GENERIC_STM32F10X
	uint32_t temp;	
	#ifndef USE_MICROLIB
		assert(nus<1864135);
	#endif
	SysTick->LOAD=nus*Pace_us; 									//时间加载
	SysTick->VAL=0x00;        									//清空VAL寄存器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 		//开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));				//等待定时器计数结束 
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 		//关闭定时器
	SysTick->VAL =0X00;     										//清空VAL寄存器
#endif	
}
//nms<=0xffffff*8*1000/SYSCLK
void Generic_Delay_ms(uint16_t nms)
{
#ifdef GENERIC_STM32F10X
	uint32_t temp;	
#ifndef USE_MICROLIB
	assert(nms<1864);
#endif	
	SysTick->LOAD=(u32)nms*Pace_ms;							//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;    //开始倒数
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));				//等待定时器计数结束  
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    //关闭定时器
	SysTick->VAL =0X00;     		  							//清空VAL寄存器  	 
#endif	
} 
//长延时
void Generic_Delay_ms_Long(uint16_t nms)
{	
#ifdef GENERIC_STM32F10X	
	uint8_t repeat=nms/500;											//对超过500ms的延时进行分块延时
	uint16_t remain=nms%500;
#ifndef USE_MICROLIB
	assert(nms<1864);
#endif	
	while(repeat)
	{
		Generic_Delay_ms(500);
		repeat--;
	}
	if(remain)
		Generic_Delay_ms(remain);
#endif
} 
