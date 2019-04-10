#include "USART.h"
u8 USART_RX_BUF[USART_REC_LEN];     																							//接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;																																//接收数组
																																									//变量=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
																																									//USART_RX_STA&0x8000可用于判断是否接收到结束位
																																									//操作结束需要清零数位标志USART_RX_STA
																																									
/*
USART_GetITStatus(GPIOx,);
* @arg USART_IT_CTS：CTS更改中断（不适用于UART4和UART5）
 * @arg USART_IT_LBD：LIN Break检测中断
 * @arg USART_IT_TXE：清空数据寄存器空中断
 * @arg USART_IT_TC：传输完成中断
 * @arg USART_IT_RXNE：接收数据寄存器不为空中断
 * @arg USART_IT_IDLE：空闲行检测中断
 * @arg USART_IT_ORE：OverRun错误中断
 * @arg USART_IT_NE：噪声错误中断
 * @arg USART_IT_FE：帧错误中断
 * @arg USART_IT_PE：奇偶校验错误中断
*/
									
									
/*
USART_SendData(USARTX,DATA);

*/





//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

void Usart1Init(u32 Baud)
{
		GPIO_InitTypeDef USART1GPIOINIT;                              								//GPIOA 9-10初始化结构体
		USART_InitTypeDef USART1INIT;																									//USART1结构体
		NVIC_InitTypeDef USARTInterrupt;																							//NVIC中断结构体
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);		//打开RCC时钟 USART1时钟、GPIOA时钟
	
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);																//复用PA9 PA10为USART1
		USART1GPIOINIT.GPIO_Pin=GPIO_Pin_9;																						//选择PA9
		USART1GPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;																			//模式为复用推挽
		USART1GPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;																		//50MHZ速度
		GPIO_Init(GPIOA,&USART1GPIOINIT);																							//GPIOA-9初始化
		USART1GPIOINIT.GPIO_Pin=GPIO_Pin_10;																					//选择PA10
		USART1GPIOINIT.GPIO_Mode=GPIO_Mode_IN_FLOATING;																//模式为浮空输入
		GPIO_Init(GPIOA,&USART1GPIOINIT);																							//GPIOA-10初始化
	
		USART1INIT.USART_BaudRate=Baud;																								//波特率设置
		USART1INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;					//硬件流控制关闭
		USART1INIT.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;														//使能接收、发送
		USART1INIT.USART_Parity=USART_Parity_No;																			//奇偶检验-无
		USART1INIT.USART_StopBits=USART_StopBits_1;																		//停止位-1
		USART1INIT.USART_WordLength=USART_WordLength_8b;															//数据字节-8
		USART_Init(USART1,&USART1INIT);																								//初始化USART1
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);																	//使能USART1接收中断
		USART_Cmd(USART1,ENABLE);																											//使能USART1
		
		USARTInterrupt.NVIC_IRQChannel=USART1_IRQn;																		//设置UASRT1中断优先级
		USARTInterrupt.NVIC_IRQChannelPreemptionPriority=0;														//设置抢占优先级
		USARTInterrupt.NVIC_IRQChannelSubPriority=0;																	//设置响应优先级
		USARTInterrupt.NVIC_IRQChannelCmd=ENABLE;																			//确认使能
		NVIC_Init(&USARTInterrupt);																										//初始化USART1中断
}

void USART1_IRQHandler (void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  													//接收中断(接收到的数据必须是回车结尾)
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);															//读取接收到的数据
			
			if((USART_RX_STA&0x8000)==0)																								//接收未完成
			{
				if(USART_RX_STA&0x4000)																										//接收到了0X2F
				{
					if(Res!=0X0A)USART_RX_STA=0;																						//接收错误,重新开始
					else USART_RX_STA|=0x8000;																							//接收完成了 
				}
				else 																																			//还没收到0X0D
				{	
					if(Res==0X0D)USART_RX_STA|=0x4000;
					else
					{
						USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
						USART_RX_STA++;
						if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;											//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		} 
}
