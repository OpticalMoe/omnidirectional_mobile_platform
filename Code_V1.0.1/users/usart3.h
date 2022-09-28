#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h" 

void USART3_IRQHandler(void);
void USART3_Init(void);
extern unsigned short int  USART3_RX_BUF[36]; 

extern int CH[5];
extern int S[2]; 

#endif


