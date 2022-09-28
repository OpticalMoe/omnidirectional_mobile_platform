/********************************************************************************
* @file      Buzzer.c
* @author    Chen
* @version
* @date      2019/1/31
* @brief     Buzzer	   
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"

#define Buzzer PAout(15)			// PB0

void Buzzer_Init(void);//≥ı ºªØ
void Buzzer_Blinking(unsigned char Count,unsigned short Time,unsigned char Percentage);	
void Buzzer_Blink(unsigned short Time_Sum,unsigned short Time);

#endif

///************************ (C) COPYRIGHT 2019  Chen *****END OF FILE****/
