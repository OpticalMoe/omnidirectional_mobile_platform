#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"


void USART2_IRQHandler(void);
void usart2_Init(void);
extern u8  USART2_RX_BUF[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 

#endif
