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
CH16	 �ڲ��¶ȴ�����
CH17	  �ڲ����յ�ѹ
*/

void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIOB_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1 , ENABLE ); //ʹ�� ADC1 ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ��Ƶ���� 6
	//72M/6=12,ADC ���ʱ�䲻�ܳ��� 14M
	//PA1 ��Ϊģ��ͨ����������
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_1|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA
	
	GPIOB_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_0;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOB, &GPIOB_InitStructure); //��ʼ�� GPIOB
	
	ADC_DeInit(ADC1); //��λ ADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2; //˳����й���ת���� ADC ͨ������Ŀ
	
	ADC_Init(ADC1, &ADC_InitStructure); //����ָ���Ĳ�����ʼ������ ADCx ��
	
	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ���� ADC1
	
	ADC_ResetCalibration(ADC1); //������λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1); //���� AD У׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
}

void TempInit (void)
{
ADC_InitTypeDef ADC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE ); //ʹ�� ADC1 ͨ��ʱ��
RCC_ADCCLKConfig(RCC_PCLK2_Div6); //��Ƶ���� 6 ʱ��Ϊ 72M/6=12MHz
	
ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ����ģʽ
ADC_InitStructure.ADC_ScanConvMode = DISABLE; //ģ��ת�������ڵ�ͨ��ģʽ
ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת��ģʽ
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC �����Ҷ���
ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת���� ADC ͨ������Ŀ
	
ADC_Init(ADC1, &ADC_InitStructure); //����ָ���Ĳ�����ʼ�� ADCx
	
ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	
ADC_Cmd(ADC1, ENABLE); //ʹ��ָ���� ADC1
	
ADC_ResetCalibration(ADC1); //����ָ���� ADC1 �ĸ�λ�Ĵ���
while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ����������

ADC_StartCalibration(ADC1); //AD У׼
while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
}

//��� ADC ֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)
{
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 ); //ͨ�� 1
	//�������˳��ֵΪ 1,����ʱ��Ϊ 239.5 ����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ָ���� ADC1 ��
	//���ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1); //�������һ�� ADC1 �������ת�����
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
