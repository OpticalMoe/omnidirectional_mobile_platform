/********************************************************************************
* @file      led.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     角度   
******************************************************************************
* @attention
*******************************************************************************/

#include "Angle.h"
#include "usart3.h"
#include "delay.h"

unsigned short int Angle = 0;		//角度

/**
* @name 	    Angle_Init
* @brief  		陀螺仪初始化
* @param      
* @retval			Count_Complete 接收成功次数
*/
unsigned char Angle_Init(void){
	unsigned char Count_sampling = 100;	//采样次数
	unsigned char Count_Time = 15;			//超时 ms
	unsigned char Count_Complete = 0;		//成功次数
	unsigned long int angle = 0;
	Angle = 0;
Start:while(--Count_sampling){
				Count_Time = 15;
				while(--Count_Time){
					if(Query_Complete()){
						angle += (USART3_RX_BUF[5] << 8) | USART3_RX_BUF[4];
						Count_Complete++;
						goto Start;
					}	
					else delay_ms(3);
				}
			}
	Angle = angle / Count_Complete;
	return Count_Complete;
}

/**
* @name 	    Query_Angle_Init
* @brief  		查询初始化角度的原始数据值
* @param      
* @retval			Angle 初始化角度的原始数据值
*/
unsigned short Query_Angle_Initial(void){
	return (unsigned short)Angle;
}

/**
* @name 	    Query_Angle_Init
* @brief  		查询初始化角度
* @param      
* @retval			Angle 初始化角度
*/
float Query_Angle_Init(void){
	if(Angle > 32767) return (float)(-(65535 - Angle)) / 32768 * 180;
	else return (float)Angle / 32768 * 180;
}

/**
* @name 	    Query_Angle
* @brief  		查询角度
* @param      
* @retval			Angle 角度
*/
float Query_Angle(void){
	unsigned short int angle = ((USART3_RX_BUF[5] << 8) | USART3_RX_BUF[4]) - Angle; 
	if(angle > 32767) return (float)(-(65535 - angle)) / 32768 * 180;
	else return (float)angle / 32768 * 180;
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
