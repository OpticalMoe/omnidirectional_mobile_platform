/********************************************************************************
* @file      S_curve.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     S曲线程序  
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __S_curve_H
#define __S_curve_H	 
#include "sys.h"

void S_curve_Init(unsigned short int gain,float slope,unsigned short int panning,unsigned short int initial_gain);	//初始化
unsigned short int S_curve_Calculate(unsigned int Step);										//S曲线计算
void S_curve_Set(unsigned short int gain,unsigned short int initial_gain);	//设定最大、最小频率
unsigned short int S_curve_Inquire_Highest_frequency(void);									//查询最大频率
unsigned short int S_curve_Inquire_Lowest_frequency(void);									//初查询最小频率
unsigned short int S_curve_Inquire_Ascending_steps(void);										//查询升频步数
void S_curve_Calculate_Init(void);																					//S曲线初始化计算，方便以后查表
unsigned short int Query_Frequency(unsigned short int Step);								//查询频率
void Enable_Debug_S_curve(void);																						//使能调试
void Disable_Debug_S_curve(void);																						//失能调试

#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
