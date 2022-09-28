/********************************************************************************
* @file      Photoelectric.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     ��翪��	   
******************************************************************************
* @attention	��ɫ-> ָʾ����-> 1
*						 ��  ��  ��  ��  ��  ��  ��  ��  ��  �� 
*						 B12 B14 A8  B1  A12 A11 B15 B7  B13  B8
*******************************************************************************/
 
#include "stm32f10x.h"
#include "Photoelectric.h"
#include "delay.h"

unsigned char Flag_Photoelectric = 0xC0;		//����״̬��־
								    
/**
* @name 	  Photoelectric_Init
* @brief  	��翪�س�ʼ��
* @param      
* @retval			
*/
void Photoelectric_Init(void) //IO��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure;		
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
//	NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� 

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_12 | GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� 
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;				 			//�˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 			 //�����趨������ʼ��GPIOB.0
	 GPIO_SetBits(GPIOB,GPIO_Pin_7 | GPIO_Pin_8);					 //PB.7/8�����
}

/**
* @name 	  Photoelectric_Start_OUT
* @brief  	���ÿ�ʼ����Ϊ���ģʽ
* @param    
* @retval			
*/
void Photoelectric_Start_OUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 	//LED-->PB.0 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 		//�����趨������ʼ��GPIOB.0
	GPIO_SetBits(GPIOB,GPIO_Pin_3);						 				//PB.0 �����
}

/**
* @name 	  Ducted_fan_Open
* @brief  	����������
* @param    
* @retval			
*/
void Ducted_fan_Open(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //PB.8 �����
}

/**
* @name 	  Ducted_fan_Off
* @brief  	�غ�������
* @param    
* @retval			
*/
void Ducted_fan_Off(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_8);						 //PB.8 �����
}

/**
* @name 	  Servo_Control
* @brief  	���ƶ��
* @param    
* @retval			
*/
void Servo_Control(signed char Angle){
	GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.8 �����
	delay_us(1500 + Angle * 11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);						 //PB.8 �����
	delay_us(500);
}

/**
* @name 	  Servo_Self_test
* @brief  	���&����Լ�
* @param    
* @retval			
*/
void Servo_Self_test(void){
	signed char Servo_Angle = -90;
	Servo_Control(-90);
	Ducted_fan_Open();
	for(Servo_Angle = -90;Servo_Angle < 90;Servo_Angle++){
		Servo_Control(Servo_Angle);
		delay_ms(5);
	}
	Ducted_fan_Off();
	Servo_Control(0);
}

/**
* @name 	  Servo_Sweep_field
* @brief  	���&���ɨ��
* @param    
* @retval			
*/
void Servo_Sweep_field(void){
	signed char Servo_Angle = -90;
	Servo_Control(-90);
	Ducted_fan_Open();
	delay_ms(500);
	for(Servo_Angle = -90;Servo_Angle < 90;Servo_Angle++){
		Servo_Control(Servo_Angle);
		delay_ms(10);
	}
	delay_ms(200);	
	Ducted_fan_Off();
	Servo_Control(0);
}
///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
