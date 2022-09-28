#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"


void USART2_IRQHandler(void);
void usart2_Init(void);
extern u8  USART2_RX_BUF[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

#endif
