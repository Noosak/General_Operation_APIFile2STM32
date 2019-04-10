#include "ADC.h"
/*
						ADC1			ADC2		ADC3	
CH0					PA0				PA0			PA0
CH1					PA1				PA1			PA1
CH2					PA2				PA2			PA2
CH3					PA3				PA3			PC3
CH4					PA4				PA4			PF6
CH5					PA5				PA5			PF7
CH6					PA6				PA6			PF8
CH7					PA7				PA7			PF9
CH8					PB0				PB0			PF10
CH9					PB1				PB1			
CH10				PC0				PC0			PC0
CH11				PC1				PC1			PC1
CH12				PC2				PC2			PC2
CH13				PC3				PC3			PC3
CH14				PC4				PC4			
CH15				PC5				PC5			
CH16	 内部温度传感器
CH17	  内部参照电压
*/

void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIOB_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1 , ENABLE ); //使能 ADC1 通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置 ADC 分频因子 6
	//72M/6=12,ADC 最大时间不能超过 14M
	//PA1 作为模拟通道输入引脚
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_1|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA
	
	GPIOB_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_0;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOB, &GPIOB_InitStructure); //初始化 GPIOB
	
	ADC_DeInit(ADC1); //复位 ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2; //顺序进行规则转换的 ADC 通道的数目
	
	ADC_Init(ADC1, &ADC_InitStructure); //根据指定的参数初始化外设 ADCx 器
	
	ADC_Cmd(ADC1, ENABLE); //使能指定的 ADC1
	
	ADC_ResetCalibration(ADC1); //开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
	
	ADC_StartCalibration(ADC1); //开启 AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
}

void TempInit (void)
{
ADC_InitTypeDef ADC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE ); //使能 ADC1 通道时钟
RCC_ADCCLKConfig(RCC_PCLK2_Div6); //分频因子 6 时钟为 72M/6=12MHz
	
ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值
	
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 独立模式
ADC_InitStructure.ADC_ScanConvMode = DISABLE; //模数转换工作在单通道模式
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //软件触发
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的 ADC 通道的数目
	
ADC_Init(ADC1, &ADC_InitStructure); //根据指定的参数初始化 ADCx
	
ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
	
ADC_Cmd(ADC1, ENABLE); //使能指定的 ADC1
	
ADC_ResetCalibration(ADC1); //重置指定的 ADC1 的复位寄存器
while(ADC_GetResetCalibrationStatus(ADC1)); //等待教主年完成

ADC_StartCalibration(ADC1); //AD 校准
while(ADC_GetCalibrationStatus(ADC1)); //等待校准完成
}

//获得 ADC 值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)
{
	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 ); //通道 1
	//规则采样顺序值为 1,采样时间为 239.5 周期
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的 ADC1 的
	//软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1); //返回最近一次 ADC1 规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	delay_init(72);
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}

double GetTemp (u8 times)
{
	double adcx;
	double GETNUM;
	adcx=Get_Adc_Average(ADC_Channel_16,times);
	GETNUM=(1.43- adcx*3.3/4096)*1000 / 4.3+ 25;
	return GETNUM;
}
