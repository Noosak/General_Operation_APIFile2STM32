#include "PWM.H"
/*
TIM引脚功能复用查表参考手册8.3.7 表42 43 44
复用需要添加“RCC_APB2Periph_AFIO”复用时钟支持至APB2初始化

TIM4复用功能重映像
					(没有重映像)(重映像)(1)
TIM4_CH1 			PB6 		 PD12
TIM4_CH2 			PB7  		 PD13
TIM4_CH3 			PB8 		 PD14
TIM4_CH4 			PB9      PD15
（1）重映像只适用于 100 和 144 脚的封装

TIM3复用功能重映像
					(没有重映像) (部分重映像) (完全重映像)(1)
TIM3_CH1 			PA6 				PB4 				PC6
TIM3_CH2 			PA7 				PB5 				PC7
TIM3_CH3 						PB0 							PC8
TIM3_CH4 						PB1 							PC9
(1)重映像只适用于 64、 100 和 144 脚的封装

TIM2复用功能重映像
								(没有重映像) (部分重映像) (部分重映像)(1) (完全重映像)(1)
TIM2_CH1_ETR(2) 		PA0 				PA15 				PA0 					PA15
TIM2_CH2 						PA1 				PB3 				PA1 					PB3
TIM2_CH3 									PA2 										PB10
TIM2_CH4 									PA3 										PB11
(1). 重映像不适用于 36 脚的封装
(2). TIM2_CH1 和 TIM2_ETR 共用一个引脚，但不能同时使用(因此在此使用这样的标记： TIM2_CH1_ETR)

TIM1复用功能重映像
					(没有重映像) (部分重映像) (完全重映像)(1)
TIM1_ETR 						PA12 							PE7
TIM1_CH1 						PA8 							PE9
TIM1_CH2 						PA9 							PE11
TIM1_CH3 						PA10 							PE13
TIM1_CH4 						PA11 							PE14
TIM1_BKIN 		PB12(2) 		PA6 				PE15
TIM1_CH1N 		PB13(2)		  PA7 				PE8
TIM1_CH2N 		PB14(2) 		PB0 				PE10
TIM1_CH3N 		PB15(2) 		PB1				  PE12
1．重映像只适用于100和144脚的封装
2．重映像不适用于36脚的封装
*/
void TIM1_PWM_Init(u16 arr,u16 psc)  
{  
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟  
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //TIM_CH1  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO  

//初始化 TIM1  
TIM_TimeBaseStructure.TIM_Period = arr;  //设置在自动重装载周期值  
TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置预分频值  
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式  
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //③初始化 TIMx  
	
//初始化 TIM1 Channel2 PWM 模式  
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
TIM_OCInitStructure.TIM_Pulse = 0;
TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC2  
TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //使能预装载寄存器  

TIM_CtrlPWMOutputs(TIM1, ENABLE);//PWM输出使能
//TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能
TIM_Cmd(TIM1, ENABLE);  //⑤使能 TIM3
}  
//定时器频率=((1+TIM_Prescaler )/72M)*(1+TIM_Period )
//舵机驱动50HZ 20000，71
//电调驱动400HZ 2500，71
void TIM3_PWM_Init(u16 arr,u16 psc)  
{  
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①使能定时器 3 时钟  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟  
	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5  
	
//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //TIM_CH2  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO  
	
//初始化 TIM3  
TIM_TimeBaseStructure.TIM_Period = arr;  //设置在自动重装载周期值  
TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置预分频值  
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //③初始化 TIMx  
	
//初始化 TIM3 Channel2 PWM 模式  
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高  
TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC2  
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器  

TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能

TIM_Cmd(TIM3, ENABLE);  //⑤使能 TIM3  
}  
//TIM_SetCompare x();调整PWM值

void ESC_Init()  //TIM3
{
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①使能定时器 3 时钟  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟  
	
//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5  
	
//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO  
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
GPIO_Init(GPIOA, &GPIO_InitStructure); 
//初始化 TIM3  
TIM_TimeBaseStructure.TIM_Period = 2500;  //设置在自动重装载周期值  
TIM_TimeBaseStructure.TIM_Prescaler =71;  //设置预分频值  
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //③初始化 TIMx  
	
//初始化 TIM3 Channel2 PWM 模式  
TIM_OCInitStructure.TIM_Pulse=1000;               //初始化关闭电机
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高  

TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC1  
TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC2  
TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC3  
TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC4  

TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器  
TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能

TIM_Cmd(TIM3, ENABLE);  //⑤使能 TIM3  

}

void ESC_Adjust()
{
TIM_SetCompare2(TIM3,1000);
}

void ESC_Stop()
{
	TIM_SetCompare1(TIM3,1000);
	TIM_SetCompare2(TIM3,1000);
	TIM_SetCompare3(TIM3,1000);
	TIM_SetCompare4(TIM3,1000);
}
