/********************************************************************************
* @file      S_curve.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     S���߳���  
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __S_curve_H
#define __S_curve_H	 
#include "sys.h"

void S_curve_Init(unsigned short int gain,float slope,unsigned short int panning,unsigned short int initial_gain);	//��ʼ��
unsigned short int S_curve_Calculate(unsigned int Step);										//S���߼���
void S_curve_Set(unsigned short int gain,unsigned short int initial_gain);	//�趨�����СƵ��
unsigned short int S_curve_Inquire_Highest_frequency(void);									//��ѯ���Ƶ��
unsigned short int S_curve_Inquire_Lowest_frequency(void);									//����ѯ��СƵ��
unsigned short int S_curve_Inquire_Ascending_steps(void);										//��ѯ��Ƶ����
void S_curve_Calculate_Init(void);																					//S���߳�ʼ�����㣬�����Ժ���
unsigned short int Query_Frequency(unsigned short int Step);								//��ѯƵ��
void Enable_Debug_S_curve(void);																						//ʹ�ܵ���
void Disable_Debug_S_curve(void);																						//ʧ�ܵ���

#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
