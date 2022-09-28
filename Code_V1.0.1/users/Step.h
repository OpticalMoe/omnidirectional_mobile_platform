/*****************************************************************************
* @file      Step.h
* @author    Chen
* @version
* @date      2017/11/17
* @brief     ����������Ƴ���		   
******************************************************************************
* @attention
*	  /    	 	   \ 
*    1				2
*   /        	  	 \
*          
*        -- 0 --     
*	   DIR=0 -> ��ʱ��ת
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

void Step_Init(void);														//��ʼ��������������������
void Calculation(float x, float y, float angle);//����
void Carried_out(unsigned int Frequency);				//ִ��
u8 Run(void);																		//����
void Start(void);																//ʹ�ܶ�ʱ��
void Stop(void);																//ʧ�ܶ�ʱ��
void Enable_Stepper(void);											//�������ʹ��
void Disenable_Stepper(void);										//�������ʧ��
void Enable_Debug_Step(void);										//ʹ�ܵ���
void Disable_Debug_Step(void);									//ʧ�ܵ���
unsigned char Subdivision(unsigned char Subdivision);	//ϸ��
unsigned long int Query_Step_Remain(void);			//��ѯʣ�ಽ��
unsigned long int Query_Step_Run(void);					//��ѯִ�в���
void Query_ARR(void);
signed short int Direction_correction_Calculate(float angle); //�������� ����
void Direction_correction_Run(signed short int step);					//�������� ִ��

#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
