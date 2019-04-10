#include "USART.h"
u8 USART_RX_BUF[USART_REC_LEN];     																							//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;																																//��������
																																									//����=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
																																									//USART_RX_STA&0x8000�������ж��Ƿ���յ�����λ
																																									//����������Ҫ������λ��־USART_RX_STA
																																									
/*
USART_GetITStatus(GPIOx,);
* @arg USART_IT_CTS��CTS�����жϣ���������UART4��UART5��
 * @arg USART_IT_LBD��LIN Break����ж�
 * @arg USART_IT_TXE��������ݼĴ������ж�
 * @arg USART_IT_TC����������ж�
 * @arg USART_IT_RXNE���������ݼĴ�����Ϊ���ж�
 * @arg USART_IT_IDLE�������м���ж�
 * @arg USART_IT_ORE��OverRun�����ж�
 * @arg USART_IT_NE�����������ж�
 * @arg USART_IT_FE��֡�����ж�
 * @arg USART_IT_PE����żУ������ж�
*/
									
									
/*
USART_SendData(USARTX,DATA);

*/





//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}

void Usart1Init(u32 Baud)
{
		GPIO_InitTypeDef USART1GPIOINIT;                              								//GPIOA 9-10��ʼ���ṹ��
		USART_InitTypeDef USART1INIT;																									//USART1�ṹ��
		NVIC_InitTypeDef USARTInterrupt;																							//NVIC�жϽṹ��
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//��RCCʱ�� USART1ʱ�ӡ�GPIOAʱ��
	
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);																//����PA9 PA10ΪUSART1
		USART1GPIOINIT.GPIO_Pin=GPIO_Pin_9;																						//ѡ��PA9
		USART1GPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;																			//ģʽΪ��������
		USART1GPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;																		//50MHZ�ٶ�
		GPIO_Init(GPIOA,&USART1GPIOINIT);																							//GPIOA-9��ʼ��
		USART1GPIOINIT.GPIO_Pin=GPIO_Pin_10;																					//ѡ��PA10
		USART1GPIOINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING;																//ģʽΪ��������
		GPIO_Init(GPIOA,&USART1GPIOINIT);																							//GPIOA-10��ʼ��
	
		USART1INIT.USART_BaudRate=Baud;																								//����������
		USART1INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;					//Ӳ�������ƹر�
		USART1INIT.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;														//ʹ�ܽ��ա�����
		USART1INIT.USART_Parity=USART_Parity_No;																			//��ż����-��
		USART1INIT.USART_StopBits=USART_StopBits_1;																		//ֹͣλ-1
		USART1INIT.USART_WordLength=USART_WordLength_8b;															//�����ֽ�-8
		USART_Init(USART1,&USART1INIT);																								//��ʼ��USART1
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);																	//ʹ��USART1�����ж�
		USART_Cmd(USART1,ENABLE);																											//ʹ��USART1
		
		USARTInterrupt.NVIC_IRQChannel=USART1_IRQn;																		//����UASRT1�ж����ȼ�
		USARTInterrupt.NVIC_IRQChannelPreemptionPriority=0;														//������ռ���ȼ�
		USARTInterrupt.NVIC_IRQChannelSubPriority=0;																	//������Ӧ���ȼ�
		USARTInterrupt.NVIC_IRQChannelCmd=ENABLE;																			//ȷ��ʹ��
		NVIC_Init(&USARTInterrupt);																										//��ʼ��USART1�ж�
}

void USART1_IRQHandler (void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  													//�����ж�(���յ������ݱ����ǻس���β)
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);															//��ȡ���յ�������
			
			if((USART_RX_STA&0x8000)==0)																								//����δ���
			{
				if(USART_RX_STA&0x4000)																										//���յ���0X2F
				{
					if(Res!=0X0A)USART_RX_STA=0;																						//���մ���,���¿�ʼ
					else USART_RX_STA|=0x8000;																							//��������� 
				}
				else 																																			//��û�յ�0X0D
				{	
					if(Res==0X0D)USART_RX_STA|=0x4000;
					else
					{
						USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
						USART_RX_STA++;
						if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;											//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
		} 
}
