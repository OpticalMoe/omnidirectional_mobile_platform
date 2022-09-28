/********************************************************************************
* @file      led.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     LED + RGB		   
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define Red1 PBout(9)			// PB0
#define Green1 PBout(8)		// PB0
#define Blue1 PBout(7)		// PB0

#define Red2 PBout(0)			// PB0
#define Green2 PBout(1)		// PB0
#define Blue2 PBout(2)		// PB0

#define Red3 PAout(7)			// PB0
#define Green3 PAout(5)		// PB0
#define Blue3 PAout(4)		// PB0

void RGB_Init(void);//≥ı ºªØ
void Colour(unsigned char Number,unsigned char Colour);
void RGB_Blinking(unsigned char Count,unsigned char Number,unsigned char colour,unsigned short Time,unsigned char Percentage);	
void RGB_Blink(unsigned short Time_Sum,unsigned char Number,unsigned char colour,unsigned short Time);

void Colour_All(unsigned char Colour);
void RGB_All_Blinking(unsigned char Count,unsigned char colour,unsigned short Time,unsigned char Percentage);	
void RGB_All_Blink(unsigned short Time_Sum,unsigned char colour,unsigned short Time);
	    
#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
