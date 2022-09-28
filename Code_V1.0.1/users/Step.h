/*****************************************************************************
* @file      Step.h
* @author    Chen
* @version
* @date      2017/11/17
* @brief     步进电机控制程序		   
******************************************************************************
* @attention
*	  /    	 	   \ 
*    1				2
*   /        	  	 \
*          
*        -- 0 --     
*	   DIR=0 -> 逆时针转
*******************************************************************************/

#ifndef __Step_H
#define __Step_H	 
#include "sys.h"

#define DIR_0 PBout(5)		// PB5
#define DIR_1 PBout(4)		// PB4
#define DIR_2 PBout(3)		// PB3

#define MS_0  PBout(12)
#define MS_1  PBout(13)
#define MS_2  PBout(14)
#define EN    PBout(15)

void Step_Init(void);														//初始化步进电机所有相关设置
void Calculation(float x, float y, float angle);//计算
void Carried_out(unsigned int Frequency);				//执行
u8 Run(void);																		//运行
void Start(void);																//使能定时器
void Stop(void);																//失能定时器
void Enable_Stepper(void);											//步进电机使能
void Disenable_Stepper(void);										//步进电机失能
void Enable_Debug_Step(void);										//使能调试
void Disable_Debug_Step(void);									//失能调试
unsigned char Subdivision(unsigned char Subdivision);	//细分
unsigned long int Query_Step_Remain(void);			//查询剩余步数
unsigned long int Query_Step_Run(void);					//查询执行步数
void Query_ARR(void);
signed short int Direction_correction_Calculate(float angle); //方向修正 计算
void Direction_correction_Run(signed short int step);					//方向修正 执行

#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
