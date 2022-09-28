/********************************************************************************
* @file      led.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     �Ƕ�   
******************************************************************************
* @attention
*******************************************************************************/

#include "Angle.h"
#include "usart3.h"
#include "delay.h"

unsigned short int Angle = 0;		//�Ƕ�

/**
* @name 	    Angle_Init
* @brief  		�����ǳ�ʼ��
* @param      
* @retval			Count_Complete ���ճɹ�����
*/
unsigned char Angle_Init(void){
	unsigned char Count_sampling = 100;	//��������
	unsigned char Count_Time = 15;			//��ʱ ms
	unsigned char Count_Complete = 0;		//�ɹ�����
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
* @brief  		��ѯ��ʼ���Ƕȵ�ԭʼ����ֵ
* @param      
* @retval			Angle ��ʼ���Ƕȵ�ԭʼ����ֵ
*/
unsigned short Query_Angle_Initial(void){
	return (unsigned short)Angle;
}

/**
* @name 	    Query_Angle_Init
* @brief  		��ѯ��ʼ���Ƕ�
* @param      
* @retval			Angle ��ʼ���Ƕ�
*/
float Query_Angle_Init(void){
	if(Angle > 32767) return (float)(-(65535 - Angle)) / 32768 * 180;
	else return (float)Angle / 32768 * 180;
}

/**
* @name 	    Query_Angle
* @brief  		��ѯ�Ƕ�
* @param      
* @retval			Angle �Ƕ�
*/
float Query_Angle(void){
	unsigned short int angle = ((USART3_RX_BUF[5] << 8) | USART3_RX_BUF[4]) - Angle; 
	if(angle > 32767) return (float)(-(65535 - angle)) / 32768 * 180;
	else return (float)angle / 32768 * 180;
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
