#include "PWM.H"
/*
TIM���Ź��ܸ��ò��ο��ֲ�8.3.7 ��42 43 44
������Ҫ��ӡ�RCC_APB2Periph_AFIO������ʱ��֧����APB2��ʼ��

TIM4���ù�����ӳ��
					(û����ӳ��)(��ӳ��)(1)
TIM4_CH1 			PB6 		 PD12
TIM4_CH2 			PB7  		 PD13
TIM4_CH3 			PB8 		 PD14
TIM4_CH4 			PB9      PD15
��1����ӳ��ֻ������ 100 �� 144 �ŵķ�װ

TIM3���ù�����ӳ��
					(û����ӳ��) (������ӳ��) (��ȫ��ӳ��)(1)
TIM3_CH1 			PA6 				PB4 				PC6
TIM3_CH2 			PA7 				PB5 				PC7
TIM3_CH3 						PB0 							PC8
TIM3_CH4 						PB1 							PC9
(1)��ӳ��ֻ������ 64�� 100 �� 144 �ŵķ�װ

TIM2���ù�����ӳ��
								(û����ӳ��) (������ӳ��) (������ӳ��)(1) (��ȫ��ӳ��)(1)
TIM2_CH1_ETR(2) 		PA0 				PA15 				PA0 					PA15
TIM2_CH2 						PA1 				PB3 				PA1 					PB3
TIM2_CH3 									PA2 										PB10
TIM2_CH4 									PA3 										PB11
(1). ��ӳ�������� 36 �ŵķ�װ
(2). TIM2_CH1 �� TIM2_ETR ����һ�����ţ�������ͬʱʹ��(����ڴ�ʹ�������ı�ǣ� TIM2_CH1_ETR)

TIM1���ù�����ӳ��
					(û����ӳ��) (������ӳ��) (��ȫ��ӳ��)(1)
TIM1_ETR 						PA12 							PE7
TIM1_CH1 						PA8 							PE9
TIM1_CH2 						PA9 							PE11
TIM1_CH3 						PA10 							PE13
TIM1_CH4 						PA11 							PE14
TIM1_BKIN 		PB12(2) 		PA6 				PE15
TIM1_CH1N 		PB13(2)		  PA7 				PE8
TIM1_CH2N 		PB14(2) 		PB0 				PE10
TIM1_CH3N 		PB15(2) 		PB1				  PE12
1����ӳ��ֻ������100��144�ŵķ�װ
2����ӳ��������36�ŵķ�װ
*/
void TIM1_PWM_Init(u16 arr,u16 psc)  
{  
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��  
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//���ø�����Ϊ�����������,��� TIM3 CH2 �� PWM ���岨�� GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //TIM_CH1  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO  

//��ʼ�� TIM1  
TIM_TimeBaseStructure.TIM_Period = arr;  //�������Զ���װ������ֵ  
TIM_TimeBaseStructure.TIM_Prescaler =psc;  //����Ԥ��Ƶֵ  
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ  
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx  
	
//��ʼ�� TIM1 Channel2 PWM ģʽ  
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ�� PWM ģʽ 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
TIM_OCInitStructure.TIM_Pulse = 0;
TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC2  
TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���  

TIM_CtrlPWMOutputs(TIM1, ENABLE);//PWM���ʹ��
//TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ��
TIM_Cmd(TIM1, ENABLE);  //��ʹ�� TIM3
}  
//��ʱ��Ƶ��=((1+TIM_Prescaler )/72M)*(1+TIM_Period )
//�������50HZ 20000��71
//�������400HZ 2500��71
void TIM3_PWM_Init(u16 arr,u16 psc)  
{  
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʹ�ܶ�ʱ�� 3 ʱ��  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��  
	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM3_CH2->PB5  
	
//���ø�����Ϊ�����������,��� TIM3 CH2 �� PWM ���岨�� GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //TIM_CH2  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO  
	
//��ʼ�� TIM3  
TIM_TimeBaseStructure.TIM_Period = arr;  //�������Զ���װ������ֵ  
TIM_TimeBaseStructure.TIM_Prescaler =psc;  //����Ԥ��Ƶֵ  
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx  
	
//��ʼ�� TIM3 Channel2 PWM ģʽ  
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ�� PWM ģʽ 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�  
TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC2  
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���  

TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ��

TIM_Cmd(TIM3, ENABLE);  //��ʹ�� TIM3  
}  
//TIM_SetCompare x();����PWMֵ

void ESC_Init()  //TIM3
{
GPIO_InitTypeDef GPIO_InitStructure;  
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
TIM_OCInitTypeDef TIM_OCInitStructure;  
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʹ�ܶ�ʱ�� 3 ʱ��  
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��  
	
//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM3_CH2->PB5  
	
//���ø�����Ϊ�����������,��� TIM3 CH2 �� PWM ���岨�� GPIOB.5  
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO  
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
GPIO_Init(GPIOA, &GPIO_InitStructure); 
//��ʼ�� TIM3  
TIM_TimeBaseStructure.TIM_Period = 2500;  //�������Զ���װ������ֵ  
TIM_TimeBaseStructure.TIM_Prescaler =71;  //����Ԥ��Ƶֵ  
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim  
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ  
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx  
	
//��ʼ�� TIM3 Channel2 PWM ģʽ  
TIM_OCInitStructure.TIM_Pulse=1000;               //��ʼ���رյ��
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ�� PWM ģʽ 1  
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��  
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�  

TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC1  
TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC2  
TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC3  
TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC4  

TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���  
TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ��

TIM_Cmd(TIM3, ENABLE);  //��ʹ�� TIM3  

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
