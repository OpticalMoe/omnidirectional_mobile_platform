/********************************************************************************
* @file      Remote.h
* @author    Chen
* @version
* @date      2019/2/2
* @brief     Ò£¿Ø  
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __REMOTE_H
#define __REMOTE_H	 
#include "stdio.h"	
#include "sys.h" 

void USART3_IRQHandler(void);
void USART3_DMA_Init(void);

extern int CH[5];
extern int S[2]; 

#endif

///************************ (C) COPYRIGHT 2019  Chen *****END OF FILE****/
