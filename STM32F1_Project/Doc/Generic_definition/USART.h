#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#define USART_REC_LEN  			200  	//�����������ֽ��� 200	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
void Usart1Init(u32 Baud);
#endif
